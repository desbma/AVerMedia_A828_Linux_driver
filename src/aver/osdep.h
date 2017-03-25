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
#ifndef __OSDEP_H__
#define __OSDEP_H__

#include <stdarg.h>
#include <linux/types.h>
#include <asm/types.h>

#ifdef __KERNEL__ 
#include <linux/version.h>
#endif

#undef __KERNEL__
#include <linux/time.h>
#define __KERNEL__




#define	REF_PARAM(x)	(x)=(x)











#ifndef __AVER_USB_PRE_NAME__
#define __AVER_USB_PRE_NAME__ ""
#endif


#define AVERCONCAT(x, y)	x ## y
#define AVERPRECONCAT(x, y)	AVERCONCAT(x, y)
#define AVERXFORM_NAME(y)	AVERPRECONCAT(__AVER_USB_PRE__, y)

#define SysWaitQueueHeadInit AVERXFORM_NAME(SysWaitQueueHeadInit)
#define SysWaitQueueHeadFini AVERXFORM_NAME(SysWaitQueueHeadFini)
#define SysWaitQueueInit AVERXFORM_NAME(SysWaitQueueInit)
#define SysWaitQueueFini AVERXFORM_NAME(SysWaitQueueFini)
#define SysAddWaitQueue AVERXFORM_NAME(SysAddWaitQueue)
#define SysRemoveWaitQueue AVERXFORM_NAME(SysRemoveWaitQueue)
#define SysWakeUp AVERXFORM_NAME(SysWakeUp)
#define SysPollWait AVERXFORM_NAME(SysPollWait)
#define SysSpinLockInit AVERXFORM_NAME(SysSpinLockInit)
#define SysSpinLock AVERXFORM_NAME(SysSpinLock)
#define SysSpinUnlock AVERXFORM_NAME(SysSpinUnlock)
#define SysSpinLockIrqsave AVERXFORM_NAME(SysSpinLockIrqsave)
#define SysSpinUnlockIrqrestore AVERXFORM_NAME(SysSpinUnlockIrqrestore)
#define SysSpinLockFini AVERXFORM_NAME(SysSpinLockFini)
#define SysSemInit AVERXFORM_NAME(SysSemInit)
#define SysSemDown AVERXFORM_NAME(SysSemDown)
#define SysSemDownInterruptible AVERXFORM_NAME(SysSemDownInterruptible)
#define SysSemUp AVERXFORM_NAME(SysSemUp)
#define SysSemFini AVERXFORM_NAME(SysSemFini)
#define _SysMutexInit AVERXFORM_NAME(_SysMutexInit)
#define SysMutexLock AVERXFORM_NAME(SysMutexLock)
#define SysMutexLockInterruptible AVERXFORM_NAME(SysMutexLockInterruptible)
#define SysMutexUnlock AVERXFORM_NAME(SysMutexUnlock)
#define SysMutexFree AVERXFORM_NAME(SysMutexFree)
#define SysJSleep AVERXFORM_NAME(SysJSleep)
#define SysMSleep AVERXFORM_NAME(SysMSleep)
#define SysUSleep AVERXFORM_NAME(SysUSleep)
#define SysMDelay AVERXFORM_NAME(SysMDelay)
#define SysInAtomic AVERXFORM_NAME(SysInAtomic)

#define SysKMallocWithTag AVERXFORM_NAME(SysKMallocWithTag) 

#define SysKFreeWithTag AVERXFORM_NAME(SysKFreeWithTag) 
#define SysMemSet AVERXFORM_NAME(SysMemSet)
#define SysMemCpy AVERXFORM_NAME(SysMemCpy)
#define SysMemCmp AVERXFORM_NAME(SysMemCmp)
#define SysReAlloc AVERXFORM_NAME(SysReAlloc)
#define SysMemMove AVERXFORM_NAME(SysMemMove)
#define SysPrintk AVERXFORM_NAME(SysPrintk)
#define SysVPrintk AVERXFORM_NAME(SysVPrintk)
#define SysSnPrintf AVERXFORM_NAME(SysSnPrintf)
#define SysKrefInit AVERXFORM_NAME(SysKrefInit)
#define SysKrefPut AVERXFORM_NAME(SysKrefPut)
#define SysKrefGet AVERXFORM_NAME(SysKrefGet)
#define SysKrefFree AVERXFORM_NAME(SysKrefFree)
#define SysLockKernel AVERXFORM_NAME(SysLockKernel)
#define SysUnlockKernel AVERXFORM_NAME(SysUnlockKernel)
#define SysCopyToUser AVERXFORM_NAME(SysCopyToUser)
#define SysCopyFromUser AVERXFORM_NAME(SysCopyFromUser)
#define SysStrLen AVERXFORM_NAME(SysStrLen)
#define SysSimpleStrtoul AVERXFORM_NAME(SysSimpleStrtoul)
#define SysStrNCpy AVERXFORM_NAME(SysStrNCpy)
#define SysSetCurrentState AVERXFORM_NAME(SysSetCurrentState)
#define SysSignalPending AVERXFORM_NAME(SysSignalPending)
#define SysSchedule AVERXFORM_NAME(SysSchedule)
#define SysScheduleTimeout AVERXFORM_NAME(SysScheduleTimeout)
#define SysGetHZ AVERXFORM_NAME(SysGetHZ)
#define SysGetPageParam AVERXFORM_NAME(SysGetPageParam)
#define SysVmallocToPage AVERXFORM_NAME(SysVmallocToPage)
#define SysPageAddress AVERXFORM_NAME(SysPageAddress)
#define SysPageHighMem AVERXFORM_NAME(SysPageHighMem)
#define SysGetPage AVERXFORM_NAME(SysGetPage)
#define SysPciDmaSyncSingleForCpu AVERXFORM_NAME(SysPciDmaSyncSingleForCpu)
#define SysPciDmaSyncSgForCpu AVERXFORM_NAME(SysPciDmaSyncSgForCpu)
#define SysFillSG AVERXFORM_NAME(SysFillSG)
#define SysGetSgDmaAddress AVERXFORM_NAME(SysGetSgDmaAddress)
#define SysGetSgDmaLen AVERXFORM_NAME(SysGetSgDmaLen)
#define SysGetSgPage AVERXFORM_NAME(SysGetSgPage)
#define SysSetSgDmaAddress AVERXFORM_NAME(SysSetSgDmaAddress)
#define SysSetSgDmaLen AVERXFORM_NAME(SysSetSgDmaLen)
#define SysSGAlloc AVERXFORM_NAME(SysSGAlloc)
#define SysPciMapSg AVERXFORM_NAME(SysPciMapSg)
#define SysPciUnmapSg AVERXFORM_NAME(SysPciUnmapSg)
#define SysPciMapSingle AVERXFORM_NAME(SysPciMapSingle)
#define SysPciUnmapSingle AVERXFORM_NAME(SysPciUnmapSingle)
#define SysPciAllocConsistent AVERXFORM_NAME(SysPciAllocConsistent)
#define SysPciFreeConsistent AVERXFORM_NAME(SysPciFreeConsistent)
#define SysPciEnableDevice AVERXFORM_NAME(SysPciEnableDevice)
#define SysPciSetMaster AVERXFORM_NAME(SysPciSetMaster)
#define SysPciMMIOMap AVERXFORM_NAME(SysPciMMIOMap)
#define SysPciMMIOUnMap AVERXFORM_NAME(SysPciMMIOUnMap)
#define SysIORead32 AVERXFORM_NAME(SysIORead32)
#define SysIOWrite32 AVERXFORM_NAME(SysIOWrite32)
#define SysIORead16 AVERXFORM_NAME(SysIORead16)
#define SysIOWrite16 AVERXFORM_NAME(SysIOWrite16)
#define SysIORead8 AVERXFORM_NAME(SysIORead8)
#define SysIOWrite8 AVERXFORM_NAME(SysIOWrite8)
#define SysPciReadCfgByte AVERXFORM_NAME(SysPciReadCfgByte)
#define SysPciReadCfgWord AVERXFORM_NAME(SysPciReadCfgWord)
#define SysPciReadCfgDWord AVERXFORM_NAME(SysPciReadCfgDWord)
#define SysPciWriteCfgByte AVERXFORM_NAME(SysPciWriteCfgByte)
#define SysPciWriteCfgWord AVERXFORM_NAME(SysPciWriteCfgWord)
#define SysPciWriteCfgDWord AVERXFORM_NAME(SysPciWriteCfgDWord)
#define SysPciVendorId AVERXFORM_NAME(SysPciVendorId)
#define SysPciDeviceId AVERXFORM_NAME(SysPciDeviceId)
#define SysPciSubVendorId AVERXFORM_NAME(SysPciSubVendorId)
#define SysPciSubDeviceId AVERXFORM_NAME(SysPciSubDeviceId)
#define SysPciGetDevice AVERXFORM_NAME(SysPciGetDevice)
#define SysPciSetDrvData AVERXFORM_NAME(SysPciSetDrvData)
#define SysPciGetDrvData AVERXFORM_NAME(SysPciGetDrvData)
#define SysPciGetIrq AVERXFORM_NAME(SysPciGetIrq)
#define SysPciGetId AVERXFORM_NAME(SysPciGetId)
#define SysPciName AVERXFORM_NAME(SysPciName)
#define SysVmalloc32 AVERXFORM_NAME(SysVmalloc32)
#define SysVFree AVERXFORM_NAME(SysVFree)
#define SysInitTimer AVERXFORM_NAME(SysInitTimer)
#define SysFillTimer AVERXFORM_NAME(SysFillTimer)
#define SysModTimer AVERXFORM_NAME(SysModTimer)
#define SysDelTimer AVERXFORM_NAME(SysDelTimer)
#define SysGetJiffies AVERXFORM_NAME(SysGetJiffies)
#define SysInitWork AVERXFORM_NAME(SysInitWork)
#define SysSubmitWork AVERXFORM_NAME(SysSubmitWork)
#define SysFlushWork AVERXFORM_NAME(SysFlushWork)
#define SysRequestIrq AVERXFORM_NAME(SysRequestIrq)
#define SysFreeIrq AVERXFORM_NAME(SysFreeIrq)
#define SysPciEnableMsi AVERXFORM_NAME(SysPciEnableMsi)
#define SysPciDisableMsi AVERXFORM_NAME(SysPciDisableMsi)
#define SysInitCompletion AVERXFORM_NAME(SysInitCompletion)
#define SysReInitCompletion AVERXFORM_NAME(SysReInitCompletion)
#define SysCompleteAndExit AVERXFORM_NAME(SysCompleteAndExit)
#define SysComplete AVERXFORM_NAME(SysComplete)
#define SysCompleteAll AVERXFORM_NAME(SysCompleteAll)
#define SysWaitForCompletion AVERXFORM_NAME(SysWaitForCompletion)
#define SysWaitForCompletionTimeout AVERXFORM_NAME(SysWaitForCompletionTimeout)
#define SysFiniCompletion AVERXFORM_NAME(SysFiniCompletion)
#define SysFileFlagIsNonBlock AVERXFORM_NAME(SysFileFlagIsNonBlock)
#define SysGetTimeOfDay AVERXFORM_NAME(SysGetTimeOfDay)
#define SysAtomicInit AVERXFORM_NAME(SysAtomicInit)
#define SysAtomicSet AVERXFORM_NAME(SysAtomicSet)
#define SysAtomicGet AVERXFORM_NAME(SysAtomicGet)
#define SysAtomicAdd AVERXFORM_NAME(SysAtomicAdd)
#define SysAtomicSub AVERXFORM_NAME(SysAtomicSub)
#define SysAtomicFini AVERXFORM_NAME(SysAtomicFini)
#define SysAtomicAddReturn AVERXFORM_NAME(SysAtomicAddReturn)
#define SysAtomicSubReturn AVERXFORM_NAME(SysAtomicSubReturn)
#define SysGetCurrent AVERXFORM_NAME(SysGetCurrent)
#define SysSendSignalToSelf AVERXFORM_NAME(SysSendSignalToSelf)
#define SysGetV4L2IOCTLName AVERXFORM_NAME(SysGetV4L2IOCTLName)

#define aver_usb_board_name AVERXFORM_NAME(aver_usb_board_name)






#define SysKMalloc(s, f)    SysKMallocWithTag(s, f, __FILE__, __LINE__)
#define SysKFree(p)         SysKFreeWithTag(p, __FILE__, __LINE__)













typedef void *pSemaphore;

typedef enum sem_type
{
	st_Mutex,	
	st_MutexLocked, 
} sem_type;

#ifdef __cplusplus
extern "C" {
#endif	

int SysSemInit(pSemaphore *sem, enum sem_type type);
#if 0 
int SysSemDown(pSemaphore sem);
int SysSemUp(pSemaphore sem);
int SysSemFini(pSemaphore sem);
#else
int SysSemDown(pSemaphore sem);

int SysSemDownInterruptible(pSemaphore sem); 
void SysSemUp(pSemaphore sem);
void SysSemFini(pSemaphore sem);
#endif
#ifdef __cplusplus
}
#endif	





#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)
typedef struct {
	void *context;
	void *key;
} Mutex, *pMutex;
#else
typedef void *pMutex;
#endif

#define SysMutexInit(pmutex) _SysMutexInit(pmutex, __FUNCTION__)

#ifdef __cplusplus
extern "C" {
#endif	

int _SysMutexInit(pMutex *mutex, const char *name);
#if 0 
int SysMutexLock(pMutex mutex);
#else
void SysMutexLock(pMutex mutex);

int SysMutexLockInterruptible(pMutex mutex);
#endif 
void SysMutexUnlock(pMutex mutex);
void SysMutexFree(pMutex mutex);

#ifdef __cplusplus
}
#endif	





typedef void *pSpinlock_t;

#ifdef __cplusplus
extern "C" {
#endif	

int SysSpinLockInit(pSpinlock_t *slock);
void SysSpinLock(pSpinlock_t slock);
void SysSpinUnlock(pSpinlock_t slock);
void SysSpinLockIrqsave(pSpinlock_t slock, unsigned long *flags);
void SysSpinUnlockIrqrestore(pSpinlock_t slock, unsigned long flags);
int SysSpinLockFini(pSpinlock_t slock);

#ifdef __cplusplus
}
#endif	




typedef void *pWait_queue_head_t;
typedef void *pWait_queue_t;



enum {
	SYSPOLLIN	= 0x1,
	SYSPOLLPRI	= 0x2,
	SYSPOLLOUT	= 0x4,
	SYSPOLLERR	= 0x8,
	SYSPOLLHUP	= 0x10,
	SYSPOLLNVAL	= 0x20,
	SYSPOLLRDNORM	= 0x40,
	SYSPOLLRDBAND	= 0x80,
	SYSPOLLWRNORM	= 0x100,
	SYSPOLLWRBAND	= 0x200,
	SYSPOLLMSG	= 0x400,
	SYSPOLLREMOVE	= 0x1000,
	SYSPOLLRDHUP	= 0x2000,
};


#ifdef __cplusplus
extern "C" {
#endif	

int SysWaitQueueHeadInit(pWait_queue_head_t *wq);
int SysWaitQueueHeadFini(pWait_queue_head_t wq);

int SysWaitQueueInit(pWait_queue_t *wq);
int SysWaitQueueFini(pWait_queue_t wq);

void SysAddWaitQueue(pWait_queue_head_t wqh, pWait_queue_t wq);
void SysRemoveWaitQueue(pWait_queue_head_t wqh, pWait_queue_t wq);
void SysWakeUp(pWait_queue_head_t wqh);


void SysPollWait(void *FileContext, pWait_queue_head_t head, void *pollstruct);

#ifdef __cplusplus
}
#endif	




#ifdef __cplusplus
extern "C" {
#endif 
void SysMSleep(int miliSec);
void SysUSleep(int microSec);
void SysMDelay(int miliSec); 
int SysInAtomic(void);
void SysJSleep(unsigned long j); 
#ifdef __cplusplus
}
#endif	



typedef enum kmalloc_type {
	eKMALLOC_ATOMIC,	
	eKMALLOC_KERNEL,	
	eKMALLOC_DMA		
} kmalloc_type;

#ifdef __cplusplus
extern "C" {
#endif	



void * SysKMallocWithTag(unsigned int size, kmalloc_type flag, const char *file, unsigned long line); 
void SysKFreeWithTag(void *ptr, const char *file, unsigned long line); 
void SysMemSet(void *ptr, unsigned char val, unsigned int size);
void *SysMemCpy(void *dest, const void *src, unsigned int size);
int SysMemCmp(const void *dest, const void *src, unsigned int size);
void *SysReAlloc(void *ptr, unsigned int oldsize, unsigned int newsize);
void *SysMemMove(void *ptr, const void *src, unsigned int size);

#ifdef __cplusplus
}
#endif	




#ifdef __cplusplus
extern "C" {
#endif	

void SysLockKernel(void);
void SysUnlockKernel(void);

#ifdef __cplusplus
}
#endif	




typedef	void *pKref;
typedef void (*cb_kref)(pKref);		

#ifdef __cplusplus
extern "C" {
#endif	

#if 0 
void SysKrefInit(pKref *ppkref);
#else
void SysKrefInit(pKref *ppkref, cb_kref cb_func);
#endif
void SysKrefPut(pKref kref, cb_kref cb_func);
void SysKrefGet(pKref kref); 
void SysKrefFree(pKref kref);

#ifdef __cplusplus
}
#endif	




#ifdef __cplusplus
extern "C" {
#endif	

int SysPrintk(const char *fmt, ...);
int SysVPrintk(const char *fmt, va_list args);
int SysSnPrintf(char *buf, unsigned int size, const char *fmt, ...);

#ifdef __cplusplus
}
#endif	




typedef int (*ioctl_func_t) (void *, void *, unsigned int, void *);

#ifdef __cplusplus
extern "C" {
#endif	

int SysUserCopy(void *inode, void *file,
		unsigned int cmd, unsigned long arg,
		ioctl_func_t func);

unsigned long SysCopyToUser(void *to, const void *from, unsigned long n);
unsigned long SysCopyFromUser(void *to, const void *from, unsigned long n);

#ifdef __cplusplus
}
#endif	



#ifdef __cplusplus
extern "C" {
#endif	

unsigned int SysStrLen(const char *str);
unsigned long SysSimpleStrtoul(const char *cp,char **endp,unsigned int base); 
char *SysStrNCpy(char *dest, const char *src, size_t count);
unsigned long SysGetJiffies(void); 

#ifdef __cplusplus
}
#endif	




#define		PROC_UNINTERRUPTIBLE	0
#define		PROC_INTERRUPTIBLE	1
#define		PROC_RUNNING		2

#ifdef __cplusplus
extern "C" {
#endif	

void SysSetCurrentState(int state);
int SysSignalPending(void);
void SysSchedule(void);
signed long SysScheduleTimeout(signed long timeout); 
unsigned long SysGetHZ(void); 
void *SysGetCurrent(void); 

#ifdef __cplusplus
}
#endif	





typedef void *pScatterlist;
typedef void *pPage;
typedef void *pPci_dev;

typedef enum {
	eDMA_DEV_WRITE,		
	eDMA_DEV_READ,		
} dma_type_t;

#ifdef __cplusplus
extern "C" {
#endif	

void SysVFree(void *addr);
u8 *SysVmalloc32(unsigned long size);
pScatterlist SysSGAlloc(int count);
dma_addr_t SysGetSgDmaAddress(pScatterlist pSG, int i);
size_t SysGetSgDmaLen(pScatterlist pSG, int i);
pPage SysGetSgPage(pScatterlist pSG, int i);
void SysSetSgDmaAddress(pScatterlist pSG, int i, dma_addr_t pa);
void SysSetSgDmaLen(pScatterlist pSG, int i, size_t len);
void SysGetPageParam(unsigned int *pg_sz, unsigned int *pg_shift);
pPage SysVmallocToPage(u8 *va);
void *SysPageAddress(pPage pg);
int SysPageHighMem(pPage pg);
void SysGetPage(pPage pg);
void SysFillSG(pScatterlist psg, int i, pPage pg, int size);
int SysPciMapSg(pPci_dev pci, pScatterlist pSG, int entry, dma_type_t type);
void SysPciUnmapSg(pPci_dev pci, pScatterlist pSG, int entry, dma_type_t type);
void *SysPciAllocConsistent(pPci_dev pci, size_t sz, dma_addr_t *pa);
void SysPciFreeConsistent(pPci_dev pci, size_t sz, void *va, dma_addr_t pa);
dma_addr_t SysPciMapSingle(pPci_dev pci, void *va, int size, dma_type_t type);
void SysPciUnmapSingle(pPci_dev pci, dma_addr_t pa, int size, dma_type_t type);
void SysPciDmaSyncSingleForCpu(pPci_dev pci, dma_addr_t pa, int size, 
				dma_type_t type);
void SysPciDmaSyncSgForCpu(pPci_dev pci, pScatterlist pSGList, int count,
				dma_type_t type);
int SysPciEnableDevice(pPci_dev pci);
void SysPciSetMaster(pPci_dev pci);
int SysPciMMIOMap(pPci_dev pci, int resource, const char *devname,
			unsigned long *mmio, unsigned int *mmio_size);
void SysPciMMIOUnMap(pPci_dev pci, int resource, unsigned long mmio);


unsigned int SysIORead32(void *addr);
void SysIOWrite32(void *addr, unsigned int val);


unsigned int SysIORead16(void *addr);
void SysIOWrite16(void *addr, unsigned int val);
unsigned int SysIORead8(void *addr);
void SysIOWrite8(void *addr, unsigned int val);


int SysPciReadCfgByte(pPci_dev pci, unsigned int w, u8 *v);
int SysPciReadCfgWord(pPci_dev pci, unsigned int w, u16 *v);
int SysPciReadCfgDWord(pPci_dev pci, unsigned int w, u32 *v);
int SysPciWriteCfgByte(pPci_dev pci, unsigned int w, u8 v);
int SysPciWriteCfgWord(pPci_dev pci, unsigned int w, u16 v);
int SysPciWriteCfgDWord(pPci_dev pci, unsigned int w, u32 v);

unsigned short SysPciVendorId(pPci_dev pci);
unsigned short SysPciDeviceId(pPci_dev pci);
unsigned short SysPciSubVendorId(pPci_dev pci);
unsigned short SysPciSubDeviceId(pPci_dev pci);

void *SysPciGetDevice(pPci_dev pci);

void SysPciSetDrvData(pPci_dev pci, void *data);
void *SysPciGetDrvData(pPci_dev pci);

unsigned int SysPciGetIrq(pPci_dev pci);




void SysPciGetId(pPci_dev pci, unsigned short *vendor,
		unsigned short *device, unsigned short *subvendor,
		unsigned short *subdevice);


const char *SysPciName(pPci_dev pci);

#ifdef __cplusplus
}
#endif	




typedef void *pTimer_list;
typedef void (*cb_timer)(unsigned long);

#ifdef __cplusplus
extern "C" {
#endif	

int SysInitTimer(pTimer_list *pTimer);
void SysFillTimer(pTimer_list Timer, cb_timer func, void *data);
int SysModTimer(pTimer_list Timer, unsigned long ms);
int SysDelTimer(pTimer_list Timer);

#ifdef __cplusplus
}
#endif	




typedef void *pWorkStruct;
typedef void (*cb_workqueue)(void *);

#ifdef __cplusplus
extern "C" {
#endif	

int SysInitWork(pWorkStruct *work, cb_workqueue func, void *data);

int SysSubmitWork(pWorkStruct work);
void SysFlushWork(void);

#ifdef __cplusplus
}
#endif	







typedef int (*KIrqHandler)(void *context);
typedef void * KIrqObject;

#ifdef __cplusplus
extern "C" {
#endif  

KIrqObject SysRequestIrq(unsigned int irq, KIrqHandler handler,
			const char *name, void *data);

void SysFreeIrq(const KIrqObject obj);

int SysPciEnableMsi(pPci_dev pci); 
void SysPciDisableMsi(pPci_dev pci);

#ifdef __cplusplus
}
#endif  




typedef void *pCompletion;

#ifdef __cplusplus
extern "C" {
#endif  

int SysInitCompletion(pCompletion *pobj);
int SysReInitCompletion(pCompletion obj);
int SysComplete(pCompletion obj);
int SysCompleteAll(pCompletion obj);
int SysCompleteAndExit(pCompletion obj, int retval);
int SysWaitForCompletion(pCompletion obj);
void SysFiniCompletion(pCompletion obj);
long SysWaitForCompletionTimeout(pCompletion obj, int timeoutMiliSec); 

#ifdef __cplusplus
}
#endif  





typedef void *pAtomic;

#ifdef __cplusplus
extern "C" {
#endif  

int SysAtomicInit(pAtomic *patm);
void SysAtomicSet(pAtomic atm, int value);
int SysAtomicGet(pAtomic atm);
void SysAtomicAdd(int value, pAtomic atm);
void SysAtomicSub(int value, pAtomic atm);
void SysAtomicFini(pAtomic atm);

int SysAtomicAddReturn(int value, pAtomic atm);
int SysAtomicSubReturn(int value, pAtomic atm);


#ifdef __cplusplus
}
#endif  




#ifdef __cplusplus
extern "C" {
#endif  


int SysFileFlagIsNonBlock(void *file);

void SysGetTimeOfDay(struct timeval *tv);


void SysSendSignalToSelf(int sig);


const char *SysGetV4L2IOCTLName(unsigned int ioctl); 




#ifdef __cplusplus
}
#endif  



typedef struct { unsigned char t[256]; } av_guard_buffer_t; 
#define __CONCAT__(x, l) x ## l
#define __INSERT_GAURD_BUFFER__(line) 	__CONCAT__(av_guard_buffer_t guardbuf_ , line)


#endif 
