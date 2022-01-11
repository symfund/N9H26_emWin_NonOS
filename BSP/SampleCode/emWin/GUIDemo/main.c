/**************************************************************************//**
 * @file     main.c
 * @brief    To utilize emWin library to demonstrate  widgets feature.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "N9H26.h"

#include "GUI.h"
#include "LCDConf.h"

#include "WM.h"
#include "TEXT.h"

#include "N9H26TouchPanel.h"

#ifdef __ICCARM__
#pragma data_alignment = 32
UINT8 u8FrameBuf[XSIZE_PHYS*YSIZE_PHYS*2];
#else
UINT8 ALIGNED_(32) u8FrameBuf[XSIZE_PHYS * YSIZE_PHYS * 2];
#endif

UINT8 *u8FrameBufPtr;

extern volatile int OS_TimeMS;
volatile int g_enable_Touch;

extern int ts_writefile(int hFile);
extern int ts_readfile(int hFile);
extern int ts_calibrate(int xsize, int ysize);
extern void TouchTask(void);

#if defined (STORAGE_SD) && (STORAGE_SD == 0)
#define NAND_2      1   // comment to use 1 disk foor NAND, uncomment to use 2 disk
UINT32 StorageForNAND(void);

static NDISK_T ptNDisk;
static NDRV_T _nandDiskDriver0 =
{
    nandInit0,
    nandpread0,
    nandpwrite0,
    nand_is_page_dirty0,
    nand_is_valid_block0,
    nand_ioctl,
    nand_block_erase0,
    nand_chip_erase0,
    0
};

#define NAND1_1_SIZE     32 /* MB unit */

UINT32 StorageForNAND(void)
{

    UINT32 block_size, free_size, disk_size;
    UINT32 u32TotalSize;

    fsAssignDriveNumber('C', DISK_TYPE_SMART_MEDIA, 0, 1);
#ifdef NAND_2
    fsAssignDriveNumber('D', DISK_TYPE_SMART_MEDIA, 0, 2);
#endif

    sicOpen();

    /* Initialize GNAND */
    if(GNAND_InitNAND(&_nandDiskDriver0, &ptNDisk, TRUE) < 0)
    {
        sysprintf("GNAND_InitNAND error\n");
        goto halt;
    }

    if(GNAND_MountNandDisk(&ptNDisk) < 0)
    {
        sysprintf("GNAND_MountNandDisk error\n");
        goto halt;
    }

    /* Get NAND disk information*/
    u32TotalSize = ptNDisk.nZone* ptNDisk.nLBPerZone*ptNDisk.nPagePerBlock*ptNDisk.nPageSize;
    sysprintf("Total Disk Size %d\n", u32TotalSize);
    /* Format NAND if necessery */
#ifdef NAND_2
    if ((fsDiskFreeSpace('C', &block_size, &free_size, &disk_size) < 0) ||
            (fsDiskFreeSpace('D', &block_size, &free_size, &disk_size) < 0))
    {
        sysprintf("unknow disk type, format device .....\n");
        if (fsTwoPartAndFormatAll((PDISK_T *)ptNDisk.pDisk, NAND1_1_SIZE*1024, (u32TotalSize- NAND1_1_SIZE*1024)) < 0)
        {
            sysprintf("Format failed\n");
            goto halt;
        }
        fsSetVolumeLabel('C', "NAND1-1\n", strlen("NAND1-1"));
        fsSetVolumeLabel('D', "NAND1-2\n", strlen("NAND1-2"));
    }
#endif

halt:
    sysprintf("systen exit\n");
    return 0;

}
#endif

/*********************************************************************
*
*       TMR0_IRQHandler
*/
void TMR0_IRQHandler(void)
{
#ifndef _MSC_VER
    OS_TimeMS++;
#endif
}

/*********************************************************************
*
*       TMR0_IRQHandler_TouchTask
*/
void TMR0_IRQHandler_TouchTask(void)
{
    if ( g_enable_Touch == 1 )
    {
        TouchTask();
    }
}

/*********************************************************************
*
*       _SYS_Init
*/
static void _SYS_Init(void)
{
    WB_UART_T uart;
    UINT32 u32ExtFreq;
    LCDFORMATEX lcdFormat;

    u32ExtFreq = sysGetExternalClock();
    sysUartPort(1);
    uart.uart_no = WB_UART_1;
    uart.uiFreq = u32ExtFreq;   //use APB clock
    uart.uiBaudrate = 115200;
    uart.uiDataBits = WB_DATA_BITS_8;
    uart.uiStopBits = WB_STOP_BITS_1;
    uart.uiParity = WB_PARITY_NONE;
    uart.uiRxTriggerLevel = LEVEL_1_BYTE;
    sysInitializeUART(&uart);

    sysInvalidCache();
    sysEnableCache(CACHE_WRITE_BACK);
    sysFlushCache(I_D_CACHE);

    /**********************************************************************************************
     * Clock Constraints:
     * (a) If Memory Clock > System Clock, the source clock of Memory and System can come from
     *     different clock source. Suggestion MPLL for Memory Clock, UPLL for System Clock
     * (b) For Memory Clock = System Clock, the source clock of Memory and System must come from
     *     same clock source
     *********************************************************************************************/
#if 0
    /**********************************************************************************************
     * Slower down system and memory clock procedures:
     * If current working clock fast than desired working clock, Please follow the procedure below
     * 1. Change System Clock first
     * 2. Then change Memory Clock
     *
     * Following example shows the Memory Clock = System Clock case. User can specify different
     * Memory Clock and System Clock depends on DRAM bandwidth or power consumption requirement.
     *********************************************************************************************/
    sysSetSystemClock(eSYS_EXT, 12000000, 12000000);
    sysSetDramClock(eSYS_EXT, 12000000, 12000000);
#else
    /**********************************************************************************************
     * Speed up system and memory clock procedures:
     * If current working clock slower than desired working clock, Please follow the procedure below
     * 1. Change Memory Clock first
     * 2. Then change System Clock
     *
     * Following example shows to speed up clock case. User can specify different
     * Memory Clock and System Clock depends on DRAM bandwidth or power consumption requirement.
     *********************************************************************************************/
    sysSetDramClock(eSYS_MPLL, 360000000, 360000000);
    sysSetSystemClock(eSYS_UPLL,            //E_SYS_SRC_CLK eSrcClk,
                      240000000,            //UINT32 u32PllKHz,
                      240000000);           //UINT32 u32SysKHz,
#endif

    /* start timer 0 */
#ifndef _MSC_VER
    OS_TimeMS = 0;
#endif

    sysSetTimerReferenceClock(TIMER0, u32ExtFreq);  //External Crystal
    sysStartTimer(TIMER0, 1000, PERIODIC_MODE);     /* 1000 ticks/per sec ==> 1tick/1ms */
    sysSetTimerEvent(TIMER0,  1, (PVOID)TMR0_IRQHandler);           /* 1  tick   call back */
    sysSetTimerEvent(TIMER0, 20, (PVOID)TMR0_IRQHandler_TouchTask); /* 20 ticks  call back */

    sysprintf("fsInitFileSystem.\n");
    fsInitFileSystem();

#if defined (STORAGE_SD) && (STORAGE_SD == 1)
    /*-----------------------------------------------------------------------*/
    /*  Init SD card                                                         */
    /*-----------------------------------------------------------------------*/
    /* clock from PLL */
    sicIoctl(SIC_SET_CLOCK, sysGetPLLOutputHz(eSYS_UPLL, u32ExtFreq)/1000, 0, 0);
    sicOpen();
    sysprintf("total sectors (%x)\n", sicSdOpen0());
#else
    StorageForNAND();
#endif

// LCD initial
//  LCD_initial();
    lcdFormat.ucVASrcFormat = DRVVPOST_FRAME_RGB565;
    vpostLCMInit(&lcdFormat, (UINT32*)u8FrameBuf);
    u8FrameBufPtr  = (UINT8 *)((UINT32)u8FrameBuf | 0x80000000);
}

/*********************************************************************
*
*       main
*/
extern void MainTask(void);

int main(void)
{
    char szFileName[20];
    char szCalibrationFile[40];
    int hFile;

#if GUI_SUPPORT_TOUCH
    g_enable_Touch = 0;
#endif

    _SYS_Init();

#if GUI_SUPPORT_TOUCH
    Init_TouchPanel();

    sprintf(szFileName, "C:\\ts_calib");
    fsAsciiToUnicode(szFileName, szCalibrationFile, TRUE);
    hFile = fsOpenFile(szCalibrationFile, szFileName, O_RDONLY | O_FSEEK);
    sysprintf("file = %d\n", hFile);
    if (hFile < 0)
    {
        // file does not exists, so do calibration
        hFile = fsOpenFile(szCalibrationFile, szFileName, O_CREATE|O_RDWR | O_FSEEK);
        if (hFile < 0)
        {
            sysprintf("CANNOT create the calibration file\n");
            //return -1;
        }

        GUI_Init();

        ts_calibrate(LCD_XSIZE, LCD_YSIZE);

        if (hFile)
        {
        	ts_writefile(hFile);
        }
    }
    else
    {
        ts_readfile(hFile);
    }

    if (hFile)
    {
    	fsCloseFile(hFile);
    }

#if defined (STORAGE_SD) && (STORAGE_SD == 0)
    GNAND_UnMountNandDisk(&ptNDisk);
    sicClose();
#endif
    g_enable_Touch = 1;
#endif

    MainTask();
//    while(1);

//      Uninit_TouchPanel();
//      sysStopTimer(TIMER0);
//    sysDisableCache();
    return 0;
}

/*************************** End of file ****************************/

#ifdef _MSC_VER

void sysUartPort(UINT32 u32Port) {}

INT32 sysInitializeUART(WB_UART_T* uart)
{
    return 0;
}

VOID sysprintf(PINT8 pcStr, ...) {}

INT nand_ioctl(INT param1, INT param2, INT param3, INT param4) 
{ 
    return 0; 
}

INT nandInit0(NDISK_T* NDISK_info)
{
    return 0;
}

INT nandpread0(INT PBA, INT page, UINT8* buff)
{
    return 0;
}

INT nandpwrite0(INT PBA, INT page, UINT8* buff)
{
    return 0;
}

INT nand_is_page_dirty0(INT PBA, INT page)
{
    return 0;
}

INT nand_is_valid_block0(INT PBA)
{
    return 0;
}

INT nand_block_erase0(INT PBA)
{
    return 0;
}

INT nand_chip_erase0(void)
{
    return 0;
}

INT  GNAND_InitNAND(NDRV_T* ndriver, NDISK_T* ptNDisk, BOOL bEraseIfNotGnandFormat) 
{
    return 0;
}

INT  GNAND_MountNandDisk(NDISK_T* ptNDisk)
{
    return 0;
}

VOID GNAND_UnMountNandDisk(NDISK_T* ptNDisk) {}

INT32 sysEnableCache(UINT32 uCacheOpMode)
{
    return 0;
}

VOID sysFlushCache(INT32 nCacheType) {}
VOID sysInvalidCache() {}

INT32 DrvADC_Open(void)
{
    return 0;
}

INT32 DrvADC_Close(void)
{
    return 0;
}

INT32 adc_read(unsigned char mode, unsigned short int* x, unsigned short int* y)
{
    return 0;
}

INT32 IsPenDown(void)
{
    return 0;
}

INT  fsInitFileSystem(VOID)
{
    return 0;
}

INT  fsAssignDriveNumber(INT nDriveNo, INT disk_type, INT instance, INT partition)
{
    return 0;
}

INT  fsDiskFreeSpace(INT nDriveNo, UINT32* puBlockSize, UINT32* puFreeSize, UINT32* puDiskSize)
{
    return 0;
}

INT  fsTwoPartAndFormatAll(PDISK_T* ptPDisk, INT firstPartSize, INT secondPartSize)
{
    return 0;
}

INT  fsSetVolumeLabel(INT nDriveNo, CHAR* szLabel, INT nLen)
{
    return 0;
}

INT  fsOpenFile(CHAR* suFileName, CHAR* szAsciiName, UINT32 uFlag)
{
    return 0;
}

INT  fsCloseFile(INT hFile)
{
    return 0;
}

INT  fsAsciiToUnicode(VOID* pvASCII, VOID* pvUniStr, BOOL bIsNullTerm)
{
    return 0;
}

INT  fsReadFile(INT hFile, UINT8* pucPtr, INT nBytes, INT* pnReadCnt)
{
    return 0;
}

INT  fsWriteFile(INT hFile, UINT8* pucBuff, INT nBytes, INT* pnWriteCnt)
{
    return 0;
}

INT64  fsFileSeek(INT hFile, INT64 n64Offset, INT16 usWhence)
{
    return 0;
}

void sicOpen(void) {}
void sicClose(void) {}

INT32 vpostLCMInit(PLCDFORMATEX plcdformatex, UINT32* pFramebuf)
{
    return 0;
}

INT32 sysSetTimerReferenceClock(INT32 nTimeNo, UINT32 uClockRate)
{
    return 0;
}

INT32 sysStartTimer(INT32 nTimeNo, UINT32 uTicksPerSecond, INT32 nOpMode)
{
    return 0;
}

INT32 sysSetTimerEvent(INT32 nTimeNo, UINT32 uTimeTick, PVOID pvFun)
{
    return 0;
}

UINT32 sysGetExternalClock(void)
{
    return 0;
}

ERRCODE
sysSetSystemClock(E_SYS_SRC_CLK eSrcClk,        // Specified the system clock come from external clock, APLL or UPLL
    UINT32 u32PllHz,            // Specified the APLL/UPLL clock
    UINT32 u32SysHz         // Specified the system clock
)
{
    return 0;
}

UINT32 sysSetDramClock(E_SYS_SRC_CLK eSrcClk, UINT32 u32PLLClockHz, UINT32 u32DdrClock)
{
    return 0;
}

#endif
