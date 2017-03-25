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
#include	<linux/usb.h>


#include	<linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)
	#include	<linux/usb/ch9.h>
#else
	#include	<linux/usb_ch9.h> 
#endif


#include	"osdep_usb.h"
#include	"debug.h"




void SysUsbSetIntfdata(pUsb_interface pintf, void *context)
{
	struct usb_interface *intf = (struct usb_interface *)pintf;

	usb_set_intfdata(intf, context);
}




void *SysUsbGetIntfdata(pUsb_interface pintf)
{
	struct usb_interface *intf = (struct usb_interface *)pintf;

	return usb_get_intfdata(intf);
}

unsigned int
SysUsbBulkPipe(pUsb_device pUdev, unsigned char addr, unsigned char dir)
{
	struct usb_device *udev = (struct usb_device *) pUdev;

	if( USB_DIR_IN==dir ) return usb_rcvbulkpipe(udev, addr);
	return usb_sndbulkpipe(udev, addr);
}


unsigned int
SysUsbIsocPipe(pUsb_device pUdev, unsigned char addr, unsigned char dir)
{
	struct usb_device *udev = (struct usb_device *) pUdev;

	if( USB_DIR_IN==dir ) return usb_rcvisocpipe(udev, addr);
	return usb_sndisocpipe(udev, addr);
}

unsigned int
SysUsbIntPipe(pUsb_device pUdev, unsigned char addr, unsigned char dir)
{
        struct usb_device *udev = (struct usb_device *) pUdev;

        if( USB_DIR_IN==dir ) return usb_rcvintpipe(udev, addr);
        return usb_sndintpipe(udev, addr);
}





int SysIntfGetNumEP(pUsb_interface pintf)
{
	struct usb_interface *intf = (struct usb_interface *)pintf;

	return intf->cur_altsetting->desc.bNumEndpoints;
}





 
#if 0
struct usb_endpoint_descriptor *SysGetEPDesc(pUsb_interface pintf, int idx)
{
	struct usb_interface *intf = (struct usb_interface *)pintf;

	return &(intf->cur_altsetting->endpoint[idx].desc);
}
#else
struct UsbEndPointDescriptor SysGetEPDesc(pUsb_interface pintf, int idx)
{
	struct usb_interface *intf = (struct usb_interface *)pintf;
	struct usb_endpoint_descriptor *odesc = &(intf->cur_altsetting->endpoint[idx].desc);
	struct UsbEndPointDescriptor desc;
	
	desc.bLength = odesc->bLength;
	desc.bDescriptorType = odesc->bDescriptorType;
	desc.bEndpointAddress = odesc->bEndpointAddress;
	desc.bmAttributes = odesc->bmAttributes;
	desc.wMaxPacketSize = odesc->wMaxPacketSize;
	desc.bInterval = odesc->bInterval;
	desc.bRefresh = odesc->bRefresh;
	desc.bSynchAddress = odesc->bSynchAddress;

	return desc;
}

#endif











pUsb_device SysUsbGetId(pUsb_interface pintf,
				unsigned short *vid, unsigned short *pid)
{
	struct usb_interface *intf = (struct usb_interface *) pintf;
	
	struct usb_device *pusb = usb_get_dev(interface_to_usbdev(intf));

	*vid = pusb->descriptor.idVendor;
	*pid = pusb->descriptor.idProduct;

	return (pUsb_device) pusb;
}




void SysUsbPutDev(pUsb_device usb)
{
	struct usb_device *udev = (struct usb_device *) usb;

	usb_put_dev(udev);
}

int SysUsbRegister(pUsb_driver driver)
{
	struct usb_driver *drv = (struct usb_driver *) driver;
	if( drv ) return usb_register(drv);

	return -ENODEV;
}

void SysUsbDeregister(pUsb_driver driver)
{
	struct usb_driver *drv = (struct usb_driver *) driver;
	if( drv ) usb_deregister(drv);
}




const char *SysUsbDriverName(pUsb_driver driver)
{
	struct usb_driver *drv = (struct usb_driver *) driver;
	if( drv ) return drv->name;

	return NULL;
}




int SysUsbBulkMsg(pUsb_device pUdev, unsigned int pipe, 
			unsigned char *buf, int len, int *actlen, int timeout)
{
	struct usb_device *udev = (struct usb_device *) pUdev;
	return usb_bulk_msg(udev, pipe, buf, len, actlen, timeout*HZ);
}





int SysUsbBulkMsg2(pUsb_device pUdev, unsigned int pipe, 
			unsigned char *buf, int len, int *actlen, int timeoutMiliSec)
{
	struct usb_device *udev = (struct usb_device *) pUdev;
	return usb_bulk_msg(udev, pipe, buf, len, actlen, timeoutMiliSec*HZ/1000);
}






int SysUsbControlMsg(pUsb_device pUdev, u8 req, u8 req_type,
		u16 value, u16 index, void *data, u16 size, int timeout)
{
    return SysUsbControlMsg2(pUdev, req, req_type, value, index, data, size, timeout*1000);
}







#if 0 
int SysUsbControlMsg(pUsb_device pUdev, u8 req, u8 req_type,
		u16 value, u16 index, void *data, u16 size, int timeout)
#else
int SysUsbControlMsg2(pUsb_device pUdev, u8 req, u8 req_type,
		u16 value, u16 index, void *data, u16 size, int timeoutMiliSec)
#endif 
{
	struct usb_device *udev = (struct usb_device *) pUdev;

	if( req_type & USB_DIR_IN ) {
		int i, ret;

		for( i=0; i<3; ++i ) {
			ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
				req, req_type, value, index, data,
				size, timeoutMiliSec*HZ/1000 );
			if( 0==ret || -EPIPE==ret ) continue;
			break;
		}
		return ret;
	} else
		return usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
		       req, req_type, value, index, data, size, timeoutMiliSec*HZ/1000 );
}







int SysUsbSubmitUrb(pUrb urb, kmalloc_type type)
{
	switch( type ) {
	case eKMALLOC_ATOMIC:
		return usb_submit_urb((struct urb *)urb, GFP_ATOMIC);
	case eKMALLOC_KERNEL:
		return usb_submit_urb((struct urb *)urb, GFP_KERNEL);
	default: break;
	}

	return -EINVAL;
}

void SysUsbFillBulkUrb(pUrb urb, pUsb_device dev, u32 pipe, void *buf,
			dma_addr_t pa,
			int size, cb_urb_complete complete, void *context)
{
	usb_fill_bulk_urb((struct urb *)urb,
			  (struct usb_device *)dev,
			  pipe,
			  buf,
			  size,
	#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,19)		
			  (void (*)(struct urb *))complete,
	#else
			  (void (*)(struct urb *, struct pt_regs *))complete,
	#endif
			  context);
	

	if(pa) {
		struct urb *purb = (struct urb *)urb;

		purb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
		purb->transfer_dma = pa;
	}
}


void SysUsbFillIntUrb(pUrb urb, pUsb_device dev, u32 pipe, void *buf,
                        dma_addr_t pa,
                        int size, cb_urb_complete complete, void *context,  unsigned char interval)
{
        usb_fill_int_urb((struct urb *)urb,
                          (struct usb_device *)dev,
                          pipe,
                          buf,
                          size,
	#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,19)         
                          (void (*)(struct urb *))complete,
	#else
                          (void (*)(struct urb *, struct pt_regs *))complete,
	#endif
			  context,
			  interval);

        if(pa) {
                struct urb *purb = (struct urb *)urb;

                purb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
                purb->transfer_dma = pa;
        }	
}
void SysUsbFillIsocUrb(pUrb urb, pUsb_device dev, u32 pipe, void *buf,
                        dma_addr_t pa, int frames_per_desc, int frame_size_per_desc,
			cb_urb_complete complete, void *context,  unsigned char interval)
{
	u32 j;
	struct urb *purb=(struct urb *)urb;


	purb->dev = (struct usb_device *)dev;
	purb->context = context;
	purb->pipe = pipe;
	purb->interval = interval;
	purb->transfer_flags = URB_ISO_ASAP;
	purb->transfer_buffer = buf;
	purb->complete = (usb_complete_t)complete;
	purb->number_of_packets = frames_per_desc;
	purb->transfer_buffer_length = frames_per_desc * frame_size_per_desc;

	for (j=0; j < frames_per_desc; j++) {
        	purb->iso_frame_desc[j].offset = frame_size_per_desc * j;
	        purb->iso_frame_desc[j].length = frame_size_per_desc;
	}
}








#if 0 
pUrb *SysUsbAllocUrb(int iso, kmalloc_type type)
#else
pUrb SysUsbAllocUrb(int iso, kmalloc_type type)
#endif
{
	switch( type ) {
	case eKMALLOC_ATOMIC: return (pUrb )usb_alloc_urb(iso, GFP_ATOMIC);
	case eKMALLOC_KERNEL: return (pUrb )usb_alloc_urb(iso, GFP_KERNEL);
	default: break;
	}

	return NULL;
}


pUrb SysUsbGetUrb(pUrb urb) { return (pUrb) usb_get_urb((struct urb *)urb); }

void SysUsbFreeUrb(pUrb urb) { usb_free_urb((struct urb *)urb); }
void SysUsbKillUrb(pUrb urb) { usb_kill_urb((struct urb *)urb); }
void SysUsbUnlinkUrb(pUrb urb) { usb_unlink_urb((struct urb *)urb); }

void *SysGetUrbContext(pUrb urb)
{
	return ((struct urb *)urb)->context;
}

int SysGetUrbStatus(pUrb urb)
{
	return ((struct urb *)urb)->status;
}

int SysGetUrbActualLength(pUrb urb)
{
	return ((struct urb *)urb)->actual_length;
}

void SysSetUrbXferFlag(pUrb urb, u32 flag)
{
	((struct urb *)urb)->transfer_flags = flag;
}

int SysUsbSetInterface(pUsb_device pUdev, int intf, int alt)
{
	return usb_set_interface((struct usb_device *)pUdev, intf, alt);
}

int SysUsbClearHalt(pUsb_device pUdev, unsigned int pipe)
{
	struct usb_device *udev = (struct usb_device *) pUdev;
	return usb_clear_halt(udev, pipe);
}


#if 0
enum usb_device_speed SysUsbDeviceSpeed(pUsb_device pUdev)
{
	return ((struct usb_device *)pUdev)->speed;
}


const struct usb_device_descriptor *SysUsbGetDeviceDescriptor(pUsb_device pUdev)
{
	return &((struct usb_device *)pUdev)->descriptor;
}

#else

enum UsbDeviceSpeed
SysUsbDeviceSpeed(pUsb_device pUdev)
{

	switch(((struct usb_device *)pUdev)->speed) {
	default:
	case USB_SPEED_UNKNOWN:
		return USB_Speed_Unknown;

	case USB_SPEED_LOW:
		return USB_Speed_Low;
		
	case USB_SPEED_FULL:
		return USB_Speed_Full;

	case USB_SPEED_HIGH:
		return USB_Speed_High;
	}

	return USB_Speed_Unknown;
}

struct UsbDeviceDescriptor
SysUsbGetDeviceDescriptor(pUsb_device pUdev)
{
	struct UsbDeviceDescriptor desc;
	struct usb_device *dev = (struct usb_device *)pUdev;

	desc.bLength = dev->descriptor.bLength;
	desc.bDescriptorType = dev->descriptor.bDescriptorType;
	desc.bcdUSB = dev->descriptor.bcdUSB;
	desc.bDeviceClass = dev->descriptor.bDeviceClass;
	desc.bDeviceSubClass = dev->descriptor.bDeviceSubClass;
	desc.bDeviceProtocol = dev->descriptor.bDeviceProtocol;
	desc.bMaxPacketSize0 = dev->descriptor.bMaxPacketSize0;
	desc.idVendor = dev->descriptor.idVendor;
	desc.idProduct = dev->descriptor.idProduct;
	desc.bcdDevice = dev->descriptor.bcdDevice;
	desc.iManufacturer = dev->descriptor.iManufacturer;
	desc.iProduct = dev->descriptor.iProduct;
	desc.iSerialNumber = dev->descriptor.iSerialNumber;
	desc.bNumConfigurations = dev->descriptor.bNumConfigurations;

	return desc;
}

#endif



unsigned char SysUsbGetMaxPower(pUsb_device pUdev)
{
	struct usb_device *udev = (struct usb_device *) pUdev;
	return udev->config->desc.bMaxPower;
}

unsigned char SysGetInfSubClass(pUsb_interface pintf)
{
        struct usb_interface *intf = (struct usb_interface *)pintf;
        if( NULL==intf || NULL==intf->cur_altsetting ) return 0x0; 
        return intf->cur_altsetting->desc.bInterfaceSubClass;  
}



int SysGetUrbTransBufLen(pUrb urb)
{
	return ((struct urb *)urb)->transfer_buffer_length;
}

int SysGetUrbActualLen(pUrb urb)
{
        return ((struct urb *)urb)->actual_length;
}

unsigned char *SysGetUrbTransBuf(pUrb urb)
{
        return ((struct urb *)urb)->transfer_buffer;
}

void SysGetIsocDescData(pUrb urb, int num, int *len, int *offset, int *status)
{
	*len = ((struct urb *)urb)->iso_frame_desc[num].actual_length;
	*offset = ((struct urb *)urb)->iso_frame_desc[num].offset;
	*status = ((struct urb *)urb)->iso_frame_desc[num].status;
}


#if 0
void *SysGetUSBDevice(pUsb_interface pintf)
#else
void *SysGetUSBDeviceFromIntf(pUsb_interface pintf)
#endif

{
	struct usb_interface *intf = (struct usb_interface *)pintf;
        return (void *)(&(intf->dev));
}




void *SysGetUSBDevice(pUsb_device pUdev)
{
	struct usb_device *usbdev = (struct usb_device *) pUdev;
	return (void *)(&usbdev->dev);
}


