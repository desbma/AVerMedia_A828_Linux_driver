/*********************************************************************************

AVerMedia TECHNOLOGIES, Inc.
AVerMedia Linux Driver License Agreement

 

IMPORTANT: This AVerMeida Linux Driver is supplied to you by
AVerMedia TECHNOLOGIES, Inc. ("AVerMedia") in consideration 
of your agreement to the following terms, and your use, installation, 
modification or redistribution of this AVerMedia Linux Driver 
constitutes acceptance of these terms. If you do not agree with 
these terms, please do not use, install, modify or redistribute 
this AVerMedia Linux Driver.

1. License. Any driver sources, binaries, utilities, documentation 
and other materials accompanying this License, whether on disk, 
print or electronic documentation, in read only memory, or any 
other media, (collectively, the "AVerMedia Linux Driver") are 
licensed, not sold, to you by AVerMedia for use only under the terms
of this License, and AVerMedia reserves all rights not expressly
granted to you. The rights granted herein are limited to AVerMedia's
intellectual property rights in the AVerMedia Linux Driver and do not
include any other patents or intellectual property rights. You own
the media on which the AVerMedia Linux Driver is recorded but AVerMedia
retains ownership of the AVerMedia Linux Driver itself and its
derivative works. The AVerMedia Linux Driver in this package and
any copies, modifications and derivative works which this License
authorizes you to make are subject to this License. The binaries
part of AVerMedia Linux Driver is ported from Windows driver and
is under some other NDA, so the original source code will not be
released.

2. Permitted Uses and Restrictions. You may use, copy, redistribute
or install the original AVerMedia Linux Driver to test or demonstrate
the AVerMedia Linux Driver with AVerMedia's products. You may also
modify the released source code in order to improve the compatibilities,
operability and functionalities with AVerMedia's products as long as
this License in whole are remained and subsequently use, copy,
redistribute or install the derivative works. You have no right to
reverse engineer, decompile, disassemble the accompanying object files
through the released source code, nor to modify, incorporate into or
compile in combination with your own programs to operate with any
product other than from AVerMedia. Your rights under this License
will terminate automatically without notice from AVerMedia if you
fail to comply with any term(s) of this License.

3. Disclaimer of Warranty. The AVerMedia Linux Driver may be "alpha",
"beta", "development", pre-release, untested, and/or not fully tested
and may contain errors that could cause failures or loss of data, be
incomplete or contain inaccuracies. YOU EXPRESSLY ACKNOWLEDGE AND AGREE
THAT USE OF THE AVERMEDIA LINUX DRIVER IS AT YOUR SOLE RISK AND THAT THE
ENTIRE RISK AS TO SATISFACTORY QUALITY, PERFORMANCE, ACCURACY AND EFFORT
IS WITH YOU. THE AVERMEDIA LINUX DRIVER IS PROVIDED "AS IS" WITH ALL
FAULTS. THE USER ASSUMES TOTAL RESPONSIBILITY AND ALL RISKS ASSOCIATED
WITH USE OF THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, RISKS OF ANY
DEFECTS, INACCURACIES, PROGRAM ERRORS, DAMAGE TO OR LOSS OF DATA,
PROGRAMS OR EQUIPMENT, UNAVAILABILITY OR INTERRUPTION OF OPERATIONS.
AVERMEDIA DOES NOT MAKE, AND EXPRESSLY DISCLAIM, ANY EXPRESS OR IMPLIED
WARRANTIES OF ANY KIND WHATSOEVER, INCLUDING, WITHOUT LIMITATION,
ANY WARRANTIES OR FITNESS FOR A PARTICULAR PURPOSE, AND ANY WARRANTIES
OF TITLE OR NON-INFRINGEMENT. AVERMEDIA DOES NOT WARRANT THAT THE USE
OR OPERATION OF THE SOFTWARE WILL BE UNITERRUPTED OR ERROR FREE.

4. Complete Agreement. This License constitutes the entire agreement
between the parties with respect to the use of the AVerMedia Linux
Driver licensed hereunder and supersedes all prior or contemporaneous
understandings regarding such subject matter. No amendment to or
modification of this License will be binding unless in writing and
signed by AVerMedia. Any translation of this License is done for
local requirements and in the event of a dispute between the English
and any non-English versions, the English version of this License
shall govern.

*********************************************************************************/
#ifndef __OSDEP_USB_H__
#define __OSDEP_USB_H__

#if 0 
#include	<linux/usb_ch9.h>
#endif
#include	<linux/version.h>
#include	"osdep.h"











#ifndef __AVER_USB_PRE_NAME__
#define __AVER_USB_PRE_NAME__ ""
#endif


#define AVERCONCAT(x, y)	x ## y
#define AVERPRECONCAT(x, y)	AVERCONCAT(x, y)
#define AVERXFORM_NAME(y)	AVERPRECONCAT(__AVER_USB_PRE__, y)

#define SysUsbSetIntfdata	AVERXFORM_NAME(SysUsbSetIntfdata)
#define SysUsbGetIntfdata	AVERXFORM_NAME(SysUsbGetIntfdata)
#define SysUsbBulkPipe		AVERXFORM_NAME(SysUsbBulkPipe)
#define SysUsbIsocPipe		AVERXFORM_NAME(SysUsbIsocPipe)
#define SysUsbIntPipe		AVERXFORM_NAME(SysUsbIntPipe)
#define SysIntfGetNumEP		AVERXFORM_NAME(SysIntfGetNumEP)
#define SysGetEPDesc		AVERXFORM_NAME(SysGetEPDesc)
#define SysUsbGetId		AVERXFORM_NAME(SysUsbGetId)
#define SysUsbPutDev		AVERXFORM_NAME(SysUsbPutDev)
#define SysUsbRegister		AVERXFORM_NAME(SysUsbRegister)
#define SysUsbDeregister	AVERXFORM_NAME(SysUsbDeregister)
#define SysUsbDriverName	AVERXFORM_NAME(SysUsbDriverName)
#define SysUsbBulkMsg		AVERXFORM_NAME(SysUsbBulkMsg)
#define SysUsbControlMsg	AVERXFORM_NAME(SysUsbControlMsg)
#define SysUsbBulkMsg2		AVERXFORM_NAME(SysUsbBulkMsg2)      
#define SysUsbControlMsg2	AVERXFORM_NAME(SysUsbControlMsg2)   
#define SysUsbSubmitUrb		AVERXFORM_NAME(SysUsbSubmitUrb)
#define SysUsbFillBulkUrb	AVERXFORM_NAME(SysUsbFillBulkUrb)
#define SysUsbFillIntUrb	AVERXFORM_NAME(SysUsbFillIntUrb)
#define SysUsbFillIsocUrb	AVERXFORM_NAME(SysUsbFillIsocUrb)
#define SysUsbAllocUrb		AVERXFORM_NAME(SysUsbAllocUrb)
#define SysUsbGetUrb		AVERXFORM_NAME(SysUsbGetUrb)
#define SysUsbFreeUrb		AVERXFORM_NAME(SysUsbFreeUrb)
#define SysUsbKillUrb		AVERXFORM_NAME(SysUsbKillUrb)
#define SysUsbUnlinkUrb		AVERXFORM_NAME(SysUsbUnlinkUrb)
#define SysGetUrbContext	AVERXFORM_NAME(SysGetUrbContext)
#define SysGetUrbStatus		AVERXFORM_NAME(SysGetUrbStatus)
#define SysGetUrbActualLength	AVERXFORM_NAME(SysGetUrbActualLength)
#define SysSetUrbXferFlag	AVERXFORM_NAME(SysSetUrbXferFlag)
#define SysUsbSetInterface	AVERXFORM_NAME(SysUsbSetInterface)
#define SysUsbClearHalt		AVERXFORM_NAME(SysUsbClearHalt)
#define SysUsbDeviceSpeed	AVERXFORM_NAME(SysUsbDeviceSpeed)
#define SysUsbGetDeviceDescriptor	AVERXFORM_NAME(SysUsbGetDeviceDescriptor)
#define SysUsbGetMaxPower	AVERXFORM_NAME(SysUsbGetMaxPower)
#define SysGetInfSubClass	AVERXFORM_NAME(SysGetInfSubClass)
#define SysGetUrbTransBufLen	AVERXFORM_NAME(SysGetUrbTransBufLen)
#define SysGetUrbActualLen	AVERXFORM_NAME(SysGetUrbActualLen)
#define SysGetUrbTransBuf	AVERXFORM_NAME(SysGetUrbTransBuf)
#define SysGetIsocDescData	AVERXFORM_NAME(SysGetIsocDescData)
#define SysGetUSBDeviceFromIntf	AVERXFORM_NAME(SysGetUSBDeviceFromIntf)
#define SysGetUSBDevice		AVERXFORM_NAME(SysGetUSBDevice)




typedef void *pUsb_device;
typedef void *pUsb_interface;
typedef void *pUsb_driver;
typedef void *pUrb;
typedef void *pIsocDesc;
#ifndef	pPt_regs
typedef void *pPt_regs;
#endif	


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,19)
typedef void (*cb_urb_complete)(pUrb);			
#else
typedef void (*cb_urb_complete)(pUrb, pPt_regs);	
#endif


#ifdef __cplusplus
extern "C" {
#endif	

unsigned int
SysUsbBulkPipe(pUsb_device pUdev, unsigned char addr, unsigned char dir);

unsigned int
SysUsbIsocPipe(pUsb_device pUdev, unsigned char addr, unsigned char dir);
unsigned int
SysUsbIntPipe(pUsb_device pUdev, unsigned char addr, unsigned char dir);

void SysUsbSetIntfdata(pUsb_interface pintf, void *context);
void *SysUsbGetIntfdata(pUsb_interface pintf);
pUsb_device
SysUsbGetId(pUsb_interface pintf, unsigned short *vid, unsigned short *pid);
void SysUsbPutDev(pUsb_device usb);
int SysIntfGetNumEP(pUsb_interface pintf);


#if 0
struct usb_endpoint_descriptor *SysGetEPDesc(pUsb_interface pintf, int idx);
#else

struct UsbEndPointDescriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;

	unsigned char  bEndpointAddress;
	unsigned char  bmAttributes;
	unsigned short wMaxPacketSize;
	unsigned char  bInterval;

	unsigned char  bRefresh;
	unsigned char  bSynchAddress;
};

struct UsbEndPointDescriptor SysGetEPDesc(pUsb_interface pintf, int idx);
#endif


int SysUsbRegister(pUsb_driver driver);
void SysUsbDeregister(pUsb_driver driver);
const char *SysUsbDriverName(pUsb_driver driver);
int SysUsbBulkMsg(pUsb_device pUdev, unsigned int pipe,
			unsigned char *buf, int len, int *actlen, int timeout);
int SysUsbControlMsg(pUsb_device pUdev, u8 req, u8 req_type,
		u16 value, u16 index, void *data, u16 size, int timeout);

int SysUsbBulkMsg2(pUsb_device pUdev, unsigned int pipe,
			unsigned char *buf, int len, int *actlen, int timeoutMiliSec);
int SysUsbControlMsg2(pUsb_device pUdev, u8 req, u8 req_type,
		u16 value, u16 index, void *data, u16 size, int timeoutMIliSec);

int SysUsbSubmitUrb(pUrb urb, kmalloc_type type);
void SysUsbFillBulkUrb(pUrb urb, pUsb_device dev, u32 pipe, void *buf,
			dma_addr_t pa, int size, cb_urb_complete complete,
			void *context);

void SysUsbFillIntUrb(pUrb urb, pUsb_device dev, u32 pipe, void *buf,
                        dma_addr_t pa, int size, cb_urb_complete complete, 
			void *context, unsigned char interval);
void SysUsbFillIsocUrb(pUrb urb, pUsb_device dev, u32 pipe, void *buf,
                        dma_addr_t pa, int frames_per_desc, int frame_size_per_desc,
                        cb_urb_complete complete, void *context,  unsigned char interval);

#if 0 
pUrb *SysUsbAllocUrb(int iso, kmalloc_type type);
#else
pUrb SysUsbAllocUrb(int iso, kmalloc_type type);
#endif
pUrb SysUsbGetUrb(pUrb urb); 
void SysUsbFreeUrb(pUrb urb);
void SysUsbKillUrb(pUrb urb);
void SysUsbUnlinkUrb(pUrb urb);
void *SysGetUrbContext(pUrb urb);
int SysGetUrbStatus(pUrb urb);
int SysGetUrbActualLength(pUrb urb);
void SysSetUrbXferFlag(pUrb urb, u32 flag);

int SysUsbSetInterface(pUsb_device pUdev, int intf, int alt);
int SysUsbClearHalt(pUsb_device pUdev, unsigned int pipe);


#if 0
enum usb_device_speed SysUsbDeviceSpeed(pUsb_device pUdev);
const struct usb_device_descriptor *SysUsbGetDeviceDescriptor(pUsb_device pUdev); 
#else

enum UsbDeviceSpeed {
	USB_Speed_Unknown = 0,
	USB_Speed_Low, USB_Speed_Full,
	USB_Speed_High
};



struct UsbDeviceDescriptor {
	unsigned char  bLength;
	unsigned char  bDescriptorType;

	unsigned short bcdUSB;
	unsigned char  bDeviceClass;
	unsigned char  bDeviceSubClass;
	unsigned char  bDeviceProtocol;
	unsigned char  bMaxPacketSize0;
	unsigned short idVendor;
	unsigned short idProduct;
	unsigned short bcdDevice;
	unsigned char  iManufacturer;
	unsigned char  iProduct;
	unsigned char  iSerialNumber;
	unsigned char  bNumConfigurations;
};

enum UsbDeviceSpeed SysUsbDeviceSpeed(pUsb_device pUdev);
struct UsbDeviceDescriptor SysUsbGetDeviceDescriptor(pUsb_device pUdev);
#endif


unsigned char SysUsbGetMaxPower(pUsb_device pUdev); 
unsigned char SysGetInfSubClass(pUsb_interface pintf); 
int SysGetUrbTransBufLen(pUrb urb); 
int SysGetUrbActualLen(pUrb urb); 
unsigned char *SysGetUrbTransBuf(pUrb urb); 
void SysGetIsocDescData(pUrb urb, int num, int *len, int *offset, int *status); 

#if 0
void *SysGetUSBDevice(pUsb_interface pintf); 
#else
void *SysGetUSBDeviceFromIntf(pUsb_interface pintf); 
void *SysGetUSBDevice(pUsb_device pUdev);
#endif


#ifdef __cplusplus
}
#endif	

#endif 
