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
#include <linux/module.h>
#include <linux/version.h>





#include "osdep_usb.c"

EXPORT_SYMBOL(SysUsbSetIntfdata);
EXPORT_SYMBOL(SysUsbGetIntfdata);
EXPORT_SYMBOL(SysUsbBulkPipe);
EXPORT_SYMBOL(SysUsbIsocPipe);
EXPORT_SYMBOL(SysUsbIntPipe);
EXPORT_SYMBOL(SysIntfGetNumEP);
EXPORT_SYMBOL(SysGetEPDesc);
EXPORT_SYMBOL(SysUsbGetId);
EXPORT_SYMBOL(SysUsbPutDev);
EXPORT_SYMBOL(SysUsbRegister);
EXPORT_SYMBOL(SysUsbDeregister);
EXPORT_SYMBOL(SysUsbDriverName);
EXPORT_SYMBOL(SysUsbBulkMsg);
EXPORT_SYMBOL(SysUsbControlMsg);
EXPORT_SYMBOL(SysUsbBulkMsg2); 
EXPORT_SYMBOL(SysUsbControlMsg2); 
EXPORT_SYMBOL(SysUsbSubmitUrb);
EXPORT_SYMBOL(SysUsbFillBulkUrb);
EXPORT_SYMBOL(SysUsbFillIntUrb);
EXPORT_SYMBOL(SysUsbFillIsocUrb);
EXPORT_SYMBOL(SysUsbAllocUrb);
EXPORT_SYMBOL(SysUsbGetUrb);
EXPORT_SYMBOL(SysUsbFreeUrb);
EXPORT_SYMBOL(SysUsbKillUrb);
EXPORT_SYMBOL(SysUsbUnlinkUrb);
EXPORT_SYMBOL(SysGetUrbContext);
EXPORT_SYMBOL(SysGetUrbStatus);
EXPORT_SYMBOL(SysGetUrbActualLength);
EXPORT_SYMBOL(SysSetUrbXferFlag);
EXPORT_SYMBOL(SysUsbSetInterface);
EXPORT_SYMBOL(SysUsbClearHalt);
EXPORT_SYMBOL(SysUsbDeviceSpeed);
EXPORT_SYMBOL(SysUsbGetDeviceDescriptor);
EXPORT_SYMBOL(SysUsbGetMaxPower);
EXPORT_SYMBOL(SysGetInfSubClass);
EXPORT_SYMBOL(SysGetUrbTransBufLen);
EXPORT_SYMBOL(SysGetUrbActualLen);
EXPORT_SYMBOL(SysGetUrbTransBuf);
EXPORT_SYMBOL(SysGetIsocDescData);
EXPORT_SYMBOL(SysGetUSBDeviceFromIntf);
EXPORT_SYMBOL(SysGetUSBDevice);


#include "osdep.c"
EXPORT_SYMBOL(SysWaitQueueHeadInit);
EXPORT_SYMBOL(SysWaitQueueHeadFini);
EXPORT_SYMBOL(SysWaitQueueInit);
EXPORT_SYMBOL(SysWaitQueueFini);
EXPORT_SYMBOL(SysAddWaitQueue);
EXPORT_SYMBOL(SysRemoveWaitQueue);
EXPORT_SYMBOL(SysWakeUp);
EXPORT_SYMBOL(SysPollWait);
EXPORT_SYMBOL(SysSpinLockInit);
EXPORT_SYMBOL(SysSpinLock);
EXPORT_SYMBOL(SysSpinUnlock);
EXPORT_SYMBOL(SysSpinLockIrqsave);
EXPORT_SYMBOL(SysSpinUnlockIrqrestore);
EXPORT_SYMBOL(SysSpinLockFini);
EXPORT_SYMBOL(SysSemInit);
EXPORT_SYMBOL(SysSemDown);
EXPORT_SYMBOL(SysSemDownInterruptible);
EXPORT_SYMBOL(SysSemUp);
EXPORT_SYMBOL(SysSemFini);
EXPORT_SYMBOL(_SysMutexInit);
EXPORT_SYMBOL(SysMutexLock);
EXPORT_SYMBOL(SysMutexLockInterruptible);
EXPORT_SYMBOL(SysMutexUnlock);
EXPORT_SYMBOL(SysMutexFree);
EXPORT_SYMBOL(SysJSleep);
EXPORT_SYMBOL(SysMSleep);
EXPORT_SYMBOL(SysUSleep);
EXPORT_SYMBOL(SysMDelay);
EXPORT_SYMBOL(SysInAtomic);

EXPORT_SYMBOL(SysKMallocWithTag); 

EXPORT_SYMBOL(SysKFreeWithTag); 
EXPORT_SYMBOL(SysMemSet);
EXPORT_SYMBOL(SysMemCpy);
EXPORT_SYMBOL(SysMemCmp);
EXPORT_SYMBOL(SysReAlloc);
EXPORT_SYMBOL(SysMemMove);
EXPORT_SYMBOL(SysPrintk);
EXPORT_SYMBOL(SysVPrintk);
EXPORT_SYMBOL(SysSnPrintf);
EXPORT_SYMBOL(SysKrefInit);
EXPORT_SYMBOL(SysKrefPut);
EXPORT_SYMBOL(SysKrefGet);
EXPORT_SYMBOL(SysKrefFree);
EXPORT_SYMBOL(SysLockKernel);
EXPORT_SYMBOL(SysUnlockKernel);
EXPORT_SYMBOL(SysCopyToUser);
EXPORT_SYMBOL(SysCopyFromUser);
EXPORT_SYMBOL(SysStrLen);
EXPORT_SYMBOL(SysSimpleStrtoul);
EXPORT_SYMBOL(SysStrNCpy);
EXPORT_SYMBOL(SysSetCurrentState);
EXPORT_SYMBOL(SysSignalPending);
EXPORT_SYMBOL(SysSchedule);
EXPORT_SYMBOL(SysScheduleTimeout);
EXPORT_SYMBOL(SysGetHZ);
EXPORT_SYMBOL(SysGetPageParam);
EXPORT_SYMBOL(SysVmallocToPage);
EXPORT_SYMBOL(SysPageAddress);
EXPORT_SYMBOL(SysPageHighMem);
EXPORT_SYMBOL(SysGetPage);
EXPORT_SYMBOL(SysPciDmaSyncSingleForCpu);
EXPORT_SYMBOL(SysPciDmaSyncSgForCpu);
EXPORT_SYMBOL(SysFillSG);
EXPORT_SYMBOL(SysGetSgDmaAddress);
EXPORT_SYMBOL(SysGetSgDmaLen);
EXPORT_SYMBOL(SysGetSgPage);
EXPORT_SYMBOL(SysSetSgDmaAddress);
EXPORT_SYMBOL(SysSetSgDmaLen);
EXPORT_SYMBOL(SysSGAlloc);
EXPORT_SYMBOL(SysPciMapSg);
EXPORT_SYMBOL(SysPciUnmapSg);
EXPORT_SYMBOL(SysPciMapSingle);
EXPORT_SYMBOL(SysPciUnmapSingle);
EXPORT_SYMBOL(SysPciAllocConsistent);
EXPORT_SYMBOL(SysPciFreeConsistent);
EXPORT_SYMBOL(SysPciEnableDevice);
EXPORT_SYMBOL(SysPciSetMaster);
EXPORT_SYMBOL(SysPciMMIOMap);
EXPORT_SYMBOL(SysPciMMIOUnMap);
EXPORT_SYMBOL(SysIORead32);
EXPORT_SYMBOL(SysIOWrite32);
EXPORT_SYMBOL(SysIORead16);
EXPORT_SYMBOL(SysIOWrite16);
EXPORT_SYMBOL(SysIORead8);
EXPORT_SYMBOL(SysIOWrite8);
EXPORT_SYMBOL(SysPciReadCfgByte);
EXPORT_SYMBOL(SysPciReadCfgWord);
EXPORT_SYMBOL(SysPciReadCfgDWord);
EXPORT_SYMBOL(SysPciWriteCfgByte);
EXPORT_SYMBOL(SysPciWriteCfgWord);
EXPORT_SYMBOL(SysPciWriteCfgDWord);
EXPORT_SYMBOL(SysPciVendorId);
EXPORT_SYMBOL(SysPciDeviceId);
EXPORT_SYMBOL(SysPciSubVendorId);
EXPORT_SYMBOL(SysPciSubDeviceId);
EXPORT_SYMBOL(SysPciGetDevice);
EXPORT_SYMBOL(SysPciSetDrvData);
EXPORT_SYMBOL(SysPciGetDrvData);
EXPORT_SYMBOL(SysPciGetIrq);
EXPORT_SYMBOL(SysPciGetId);
EXPORT_SYMBOL(SysPciName);
EXPORT_SYMBOL(SysVmalloc32);
EXPORT_SYMBOL(SysVFree);
EXPORT_SYMBOL(SysInitTimer);
EXPORT_SYMBOL(SysFillTimer);
EXPORT_SYMBOL(SysModTimer);
EXPORT_SYMBOL(SysDelTimer);
EXPORT_SYMBOL(SysGetJiffies);
EXPORT_SYMBOL(SysInitWork);
EXPORT_SYMBOL(SysSubmitWork);
EXPORT_SYMBOL(SysFlushWork);
EXPORT_SYMBOL(SysRequestIrq);
EXPORT_SYMBOL(SysFreeIrq);
EXPORT_SYMBOL(SysPciEnableMsi);
EXPORT_SYMBOL(SysPciDisableMsi);
EXPORT_SYMBOL(SysInitCompletion);
EXPORT_SYMBOL(SysReInitCompletion);
EXPORT_SYMBOL(SysCompleteAndExit);
EXPORT_SYMBOL(SysComplete);
EXPORT_SYMBOL(SysCompleteAll);
EXPORT_SYMBOL(SysWaitForCompletion);
EXPORT_SYMBOL(SysWaitForCompletionTimeout);
EXPORT_SYMBOL(SysFiniCompletion);
EXPORT_SYMBOL(SysFileFlagIsNonBlock);
EXPORT_SYMBOL(SysGetTimeOfDay);
EXPORT_SYMBOL(SysAtomicInit);
EXPORT_SYMBOL(SysAtomicSet);
EXPORT_SYMBOL(SysAtomicGet);
EXPORT_SYMBOL(SysAtomicAdd);
EXPORT_SYMBOL(SysAtomicSub);
EXPORT_SYMBOL(SysAtomicFini);
EXPORT_SYMBOL(SysAtomicAddReturn);
EXPORT_SYMBOL(SysAtomicSubReturn);
EXPORT_SYMBOL(SysGetCurrent);
EXPORT_SYMBOL(SysSendSignalToSelf);
EXPORT_SYMBOL(SysGetV4L2IOCTLName);
EXPORT_SYMBOL(aver_usb_board_name);


#include "averusb_version.h"

#define AVERUSB_DRIVER_NAME "AVerMedia USB Wrapper"

#ifdef __AVER_USB_PRE__ 
MODULE_DESCRIPTION(AVERUSB_DRIVER_NAME " " __AVER_USB_PRE_NAME__ " v" DRIVER_VER);
#else
MODULE_DESCRIPTION(AVERUSB_DRIVER_NAME " v" DRIVER_VER);
#endif  
MODULE_AUTHOR("MPD Linux Team");
MODULE_LICENSE("GPL");


static int __init mod_init(void)
{

	printk("%s for %s version %s loaded\n", AVERUSB_DRIVER_NAME, __AVER_USB_PRE_NAME__, DRIVER_VER); 
	return 0;
}

static void __exit mod_exit(void)
{

	printk("%s for %s version %s unloaded\n", AVERUSB_DRIVER_NAME, __AVER_USB_PRE_NAME__, DRIVER_VER); 
}

module_init (mod_init);
module_exit (mod_exit);

