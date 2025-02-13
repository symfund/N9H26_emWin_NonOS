/*
 * $Id: usbkbd.c,v 1.16 2000/08/14 21:05:26 vojtech Exp $
 *
 *  Copyright (c) 1999-2000 Vojtech Pavlik
 *
 *  USB HIDBP Keyboard support
 *
 *  Sponsored by SuSE
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@suse.cz>, or by paper mail:
 * Vojtech Pavlik, Ucitelska 1576, Prague 8, 182 00 Czech Republic
 */
#ifdef ECOS
#include "stdio.h"
#include "string.h"
#include "drv_api.h"
#include "diag.h"
#include "wbtypes.h"
#include "wbio.h"
#else
#include <stdio.h>
#include <string.h>
#include "wbtypes.h"
#include "wblib.h"
#endif
#include "N9H26_USBH.h"
#include "usbkbd.h"


USB_KBD_T  *_my_usbkbd = NULL;

//MODULE_AUTHOR("Vojtech Pavlik <vojtech@suse.cz>");
//MODULE_DESCRIPTION("USB HID Boot Protocol keyboard driver");

#if 0
/* See the HID Usage Tables Document, this is the AT-101 key position */
static UINT8 usb_kbd_keycode[256] = 
{
      0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,
     50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,
      4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,
     27, 43, 84, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,
     65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,
    105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,
     72, 73, 82, 83, 86,127,116,117, 85, 89, 90, 91, 92, 93, 94, 95,
    120,121,122,123,134,138,130,132,128,129,131,137,133,135,136,113,
    115,114,  0,  0,  0,124,  0,181,182,183,184,185,186,187,188,189,
    190,191,192,193,194,195,196,197,198,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,
    150,158,159,128,136,177,178,176,142,152,173,140
};
#endif


/* YCHuang: I translate the key position into key code. */
static UINT8  _UsbKeyCodeMap[256][20] = 
{
    "N/A",
    "N/A",
    "N/A",
    "N/A",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "[Enter]",
    "[Escape]",
    "[BackSpc]",
    "[Tab]",
    " ",
    "-",
    "=",
    "[",
    "]",
    "\\",
    "`",
    ";",
    "'",
    "[Grave]",
    ",",
    ".",
    "/",
    "[Caps Lock]",
    "[F1]",
    "[F2]",
    "[F3]",
    "[F4]",
    "[F5]",
    "[F6]",
    "[F7]",
    "[F8]",
    "[F9]",
    "[F10]",
    "[F11]",
    "[F12]",
    "[PrtScr]",
    "[ScrLock]",
    "[Pause]",
    "[Insert]",
    "[Home]",
    "[PageUp]",
    "[Delete]",
    "[End]",
    "[PageDown]",
    "[->]",
    "[<-]",
    "[DownArrow]",
    "[UpArrow]",
    "[NumLock]",
    "/",
    "*",
    "-",
    "+",
    "[Enter]",
    "1",
    "2",
    "3",
    "4", 
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "[Del]",
    "[?]",
    "[Application]",
    "[Power]",
    "=",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "[Execute]",
    "[Help]",
    "[Menu]",
    "[Select]",
    "[Stop]",
    "[Again]",
    "[Undo]",
    "[Cut]",
    "[Copy]",
    "[Paste]",
    "[Find]",
    "[Mute]",
    "[VolumeUp]",
    "[VolumeDown]",
    "[Lock NumLock]",
    "[Lock ScrLock]",
    "[Comma]",
    "[Equal Sign]",
    "[International 1]",
    "[International 2]",
    "[International 3]",
    "[International 4]",
    "[International 5]",
    "[International 6]",
    "[International 7]",
    "[International 8]",
    "[International 9]",
    "[LANG 1]",
    "[LANG 2]",
    "[LANG 3]",
    "[LANG 4]",
    "[LANG 5]",
    "[LANG 6]",
    "[LANG 7]",
    "[LANG 8]",
    "[LANG 9]",
    "[Alt Erase]",
    "[SysReq]",
    "[Cancel]",
    "[Clear]",
    "[Prior]",
    "[Return]",
    "[Seperate]",
    "[Out]",
    "[Oper]",
    "[Clear/Again]",
    "[CrSel]",
    "[ExSel]",
};

#if 0
/* Offset 224 */
static UINT8  _UsbControlKey[8][24] =
{
    "[Left Ctrl]",
    "[Left Shift]",
    "[Left Alt]",
    "[Left GUI]",
    "[Right Ctrl]",
    "[Right Shift]",
    "[Right Alt]",
    "[Right GUI]"
};
#endif


/* 
 *  The interrupt in pipe of usb keyboard returns 8 bytes.
 *  Byte 0: modifier bytes
 *  Byte 1: reserved byte
 *  Byte 2: [0..4] LED report, [5..7] LED report padding
 *  Byte 3~7: Key arrays
 */
 #ifdef __TEST__
 UINT32 u32Event = 0;
 #endif
static VOID usb_kbd_irq(URB_T *urb)
{
    USB_KBD_T *kbd = urb->context;
    INT  i;

    if (urb->status) 
    {
        sysprintf("usb_kbd_irq, urb error:%d\n", urb->status);
        return;
    }
    
    for (i=2; i<8; i++)
        if (kbd->newData[i])
            sysprintf("%s\n", _UsbKeyCodeMap[kbd->newData[i]]);
#ifdef __TEST__
     u32Event = u32Event+1;
 #endif
#if 0
    for (i=0; i<8; i++)
        input_report_key(&kbd->dev, usb_kbd_keycode[i + 224], (kbd->newData[0] >> i) & 1);

    for (i=2; i<8; i++) 
    {
        if ((kbd->oldData[i] > 3) && 
            (memscan(kbd->newData + 2, kbd->oldData[i], 6) == kbd->newData + 8)) 
        {
            if (usb_kbd_keycode[kbd->oldData[i]])
                input_report_key(&kbd->dev, usb_kbd_keycode[kbd->oldData[i]], 0);
            else
                sysprintf("usb_kbd_irq - Unknown key (scancode %#x) released.\n", kbd->oldData[i]);
        }

        if ((kbd->newData[i] > 3) && 
            (memscan(kbd->oldData + 2, kbd->newData[i], 6) == kbd->oldData + 8)) 
        {
            if (usb_kbd_keycode[kbd->newData[i]])
                input_report_key(&kbd->dev, usb_kbd_keycode[kbd->newData[i]], 1);
            else
                info("Unknown key (scancode %#x) pressed.", kbd->newData[i]);
        }
    }
    memcpy(kbd->oldData, kbd->newData, 8);
#endif
}


#if 0
INT usb_kbd_event(struct input_dev *dev, UINT32 type, UINT32 code, INT value)
{
    USB_KBD_T *kbd = dev->private;

    if (type != EV_LED) return -1;

        kbd->newleds = (!!test_bit(LED_KANA,    dev->led) << 3) | (!!test_bit(LED_COMPOSE, dev->led) << 3) |
                       (!!test_bit(LED_SCROLLL, dev->led) << 2) | (!!test_bit(LED_CAPSL,   dev->led) << 1) |
                       (!!test_bit(LED_NUML,    dev->led));

        if (kbd->urbLed.status == -EINPROGRESS)
                return 0;

        if (kbd->leds == kbd->newleds)
                return 0;

        kbd->leds = kbd->newleds;
        kbd->urbLed.dev = kbd->usbdev;
        if (USB_SubmitUrb(&kbd->urbLed))
                err("USB_SubmitUrb(leds) failed");

        return 0;
}
#endif


static VOID usb_kbd_led(URB_T *urb)
{
    USB_KBD_T  *kbd = urb->context;
    INT   ret;

    if (urb->status)
        sysprintf("Warning - led urb status %d received\n", urb->status);
        
    if (kbd->leds == kbd->newleds)
        return;

    kbd->leds = kbd->newleds;
    kbd->urbLed.dev = kbd->usbdev;
    ret = USB_SubmitUrb(&kbd->urbLed);
    if (ret)
        sysprintf("Error - USB_SubmitUrb(leds) failed\n");
}


VOID USBKeyboardLED(URB_T *urb)
{
    USB_KBD_T *kbd = urb->context;

    if (urb->status)
        sysprintf("Warning - led urb status %d received\n", urb->status);
        
    if (kbd->leds == kbd->newleds)
        return;

    kbd->leds = kbd->newleds;
    kbd->urbLed.dev = kbd->usbdev;
    if (USB_SubmitUrb(&kbd->urbLed))
        sysprintf("Error - USB_SubmitUrb(leds) failed\n");
}


INT USBKeyboardOpen(USB_KBD_T *kbd)
{
    if (USB_SubmitUrb(&kbd->urbIrq))
        return -1; /* -EIO; */
    return 0;
}

VOID USBKeyboardClose(USB_KBD_T *kbd)
{
    USB_UnlinkUrb(&kbd->urbIrq);
}

USB_KBD_T *kbd_dummy;
static VOID *usb_kbd_probe(USB_DEV_T *dev, UINT32 ifnum, const USB_DEV_ID_T *id)
{
    struct usb_interface *iface;
    struct usb_interface_descriptor *interface;
    struct usb_endpoint_descriptor *endpoint;
    USB_KBD_T *kbd;
    INT    pipe, maxp;
    char  buf[64];
    
    /* added by YCHuang, reject interface 1 */
    if (ifnum != 0)
        return NULL;

    iface = &dev->actconfig->interface[ifnum];
    interface = &iface->altsetting[iface->act_altsetting];

    if (interface->bNumEndpoints != 1) 
        return NULL;

    endpoint = interface->endpoint + 0;
    if (!(endpoint->bEndpointAddress & 0x80)) 
        return NULL;
    if ((endpoint->bmAttributes & 3) != 3) 
        return NULL;

    pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
    maxp = usb_maxpacket(dev, pipe, usb_pipeout(pipe));

    USB_SetProtocol(dev, interface->bInterfaceNumber, 0);
    USB_SetIdle(dev, interface->bInterfaceNumber, 0, 0);  /* duration:indefinite, for all reports */

    kbd = USB_malloc(sizeof(USB_KBD_T), 4);
    if (!kbd) 
        return NULL;
    memset(kbd, 0, sizeof(USB_KBD_T));

    kbd->usbdev = dev;
    kbd_dummy->usbdev = dev;
#if 0
    kbd->dev.evbit[0] = BIT(EV_KEY) | BIT(EV_LED) | BIT(EV_REP);
    kbd->dev.ledbit[0] = BIT(LED_NUML) | BIT(LED_CAPSL) | BIT(LED_SCROLLL) | BIT(LED_COMPOSE) | BIT(LED_KANA);

    for (i=0; i<255; i++)
         set_bit(usb_kbd_keycode[i], kbd->dev.keybit);
    clear_bit(0, kbd->dev.keybit);
        
    kbd->dev.private = kbd;
    kbd->dev.event = usb_kbd_event;
    kbd->dev.open = usb_kbd_open;
    kbd->dev.close = usb_kbd_close;
#endif    

    sysprintf("DEVICE SLOW: %d  %d\n", dev->slow, usb_pipeslow(pipe));
    dev->slow = 1;
    
#ifdef ETST_ALIGNMENT_INT
    kbd->newData = (UINT8 *)0xC103FFFF;
    //kbd->newData = (UINT8 *)0xC105FFF1;
    //kbd->newData = (UINT8 *)0xC107FFFD;
    //kbd->newData = (UINT8 *)0xC1090000;
#endif    

    FILL_INT_URB(&kbd->urbIrq, dev, pipe, kbd->newData, maxp > 8 ? 8 : maxp,
                usb_kbd_irq, kbd, endpoint->bInterval);

    kbd->dr.requesttype = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
    kbd->dr.request = USB_REQ_SET_REPORT;
    kbd->dr.value = 0x200;
    kbd->dr.index = interface->bInterfaceNumber;
    kbd->dr.length = 1;

#if 0
    kbd->dev.name = kbd->name;
    kbd->dev.idbus = BUS_USB;
    kbd->dev.idvendor = dev->descriptor.idVendor;
    kbd->dev.idproduct = dev->descriptor.idProduct;
    kbd->dev.idversion = dev->descriptor.bcdDevice;
#endif

    if (dev->descriptor.iManufacturer &&
        (USB_TranslateString(dev, dev->descriptor.iManufacturer, buf, 63) > 0))
        sysprintf(kbd->name, "%s %s", kbd->name, buf);
    if (dev->descriptor.iProduct &&
        (USB_TranslateString(dev, dev->descriptor.iProduct, buf, 63) > 0))
        sysprintf(kbd->name, "%s %s", kbd->name, buf);

    if (!strlen(kbd->name))
        sysprintf(kbd->name, "USB HIDBP Keyboard %04x:%04x",
                          dev->descriptor.idVendor, dev->descriptor.idProduct);

    FILL_CONTROL_URB(&kbd->urbLed, dev, usb_sndctrlpipe(dev, 0),
                    (VOID*) &kbd->dr, &kbd->leds, 1, usb_kbd_led, kbd);

    //input_register_device(&kbd->dev);

    //printk(KERN_INFO "input%d: %s on on usb%d:%d.%d\n",
    //        kbd->dev.number, kbd->name, dev->bus->busnum, dev->devnum, ifnum);
    sysprintf("usb_kbd_probe - %s on usb%d:%d.%d\n", kbd->name, dev->bus->busnum, dev->devnum, ifnum);
    _my_usbkbd = kbd;
    USBKeyboardOpen(_my_usbkbd);
    return kbd;
}


void usbSetFeature(void)
{
        USB_SendControlMessage(kbd_dummy->usbdev,                        /* USB_DEV_T *dev, */
                                usb_sndctrlpipe(  kbd_dummy->usbdev, 0), /* UINT32 pipe, */
                                USB_REQ_SET_FEATURE,                     /* UINT8 request, */
                                 0,                                      /* UINT8 requesttype,  - Host to device */
                                 1,                                      /* UINT16 value,       - DEVICE REMOTE WAKEUP */
                                 0,                                      /* UINT16 index, */
                                 NULL,                                   /* VOID *data, */
                                 0,                                      /* UINT16 size, */
                                 200 * 1000);                            /* INT timeout) */
}       

static VOID usb_kbd_disconnect(USB_DEV_T *dev, VOID *ptr)
{
    USB_KBD_T  *kbd = ptr;
    
    USB_UnlinkUrb(&kbd->urbIrq);
    //input_unregister_device(&kbd->dev);
    USB_free(kbd);
    sysprintf("USB keyboard disconnect\n");
}


static USB_DEV_ID_T usb_kbd_id_table[] = 
{
    USB_DEVICE_ID_MATCH_INT_INFO,      /* match_flags */
    0, 0, 0, 0, 0, 0, 0,
    3,                                 /* bInterfaceClass */
    1,                                 /* bInterfaceSubClass */
    1,                                 /* bInterfaceProtocol */
    0 
};


USB_DRIVER_T  usb_kbd_driver = 
{
    "keyboard",
    usb_kbd_probe,
    usb_kbd_disconnect,
    {NULL,NULL},                       /* driver_list */ 
 //   {0},                               /* semaphore */
    NULL,                              /* *ioctl */
    usb_kbd_id_table,
    NULL,                              /* suspend */
    NULL                               /* resume */
};


INT  USBKeyboardInit()
{
    USB_RegisterDriver(&usb_kbd_driver);
    return 0;
}
VOID USBKBM_Exit(void)
{
    if (_my_usbkbd == NULL) 
        return;
    sysprintf("USB Keyboard Exit()\n");     
    USB_DeregisterDriver(&usb_kbd_driver);
}


