/**************************************************************************//**
 * @file     N9H26TouchPanel.h
 * @brief    N9H26 series emWin touch header file
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#ifndef __N9H26TOUCHPANEL_H__
#define __N9H26TOUCHPANEL_H__

#ifdef __800x480__
#define XSIZE_PHYS  800
#define YSIZE_PHYS  480
#else
#define XSIZE_PHYS  320
#define YSIZE_PHYS  240
#endif

#define LCD_XSIZE       XSIZE_PHYS
#define LCD_YSIZE       YSIZE_PHYS


int Init_TouchPanel(void);
int Read_TouchPanel(int *x, int *y);
int Uninit_TouchPanel(void);
#endif
