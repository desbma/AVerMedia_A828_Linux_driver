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
#include <linux/types.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26) 
#include <asm/semaphore.h>
#else
#include <linux/semaphore.h>
#endif
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kref.h>
#include <linux/smp_lock.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <asm/scatterlist.h>
#include <linux/vmalloc.h>
#include <linux/timer.h>
#include <linux/pci.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,8) 
#include <linux/hardirq.h>
#endif

#include <linux/delay.h> 
#include <linux/interrupt.h>
#include <asm/atomic.h> 
#include <asm/io.h> 
#include <linux/signal.h> 

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,15) 
#include <linux/mutex.h>
#endif

#include <linux/poll.h> 
#include <linux/scatterlist.h> 

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20) 
#include <asm/bug.h>
#else
#include <linux/bug.h> 
#endif 

#include <linux/videodev2.h>

#include "osdep.h"
#include "debug.h"
#include "id.h"	








int SysWaitQueueHeadInit(pWait_queue_head_t *wq)
{
	wait_queue_head_t *pWQ = kmalloc(sizeof(*pWQ), GFP_KERNEL);

	if( pWQ ) {
		init_waitqueue_head(pWQ);
		*wq = (pWait_queue_head_t) pWQ;
		return 0;
	}

	*wq = NULL;
	return -ENOMEM;
}

int SysWaitQueueHeadFini(pWait_queue_head_t wq)
{
	if( wq ) {
		kfree(wq);
		return 0;
	}

	return -EINVAL;
}




int SysWaitQueueInit(pWait_queue_t *wq)
{
	DECLARE_WAITQUEUE(tmp_wait_queue, current);
	*wq = (pWait_queue_t) kmalloc(sizeof(wait_queue_t), GFP_KERNEL);

	if( !(*wq) ) return -ENOMEM;
	memcpy(*wq, &tmp_wait_queue, sizeof(wait_queue_t));
	return 0;
}

int SysWaitQueueFini(pWait_queue_t wq)
{
	if( wq ) {
		kfree(wq);
		return 0;
	}
	
	return -EINVAL;
}

void SysAddWaitQueue(pWait_queue_head_t wqh, pWait_queue_t wq)
{
	wait_queue_head_t *pWQH = (wait_queue_head_t *) wqh;
	wait_queue_t *pWQ = (wait_queue_t *) wq;

#if	LINUX_VERSION_CODE < KERNEL_VERSION(2,6,13)
	pWQ->task = current;
#else
	pWQ->private = current;
#endif
	add_wait_queue(pWQH, pWQ);
}





void SysRemoveWaitQueue(pWait_queue_head_t wqh, pWait_queue_t wq)
{
	wait_queue_head_t *pWQH = (wait_queue_head_t *) wqh;
	wait_queue_t *pWQ = (wait_queue_t *) wq;

	remove_wait_queue(pWQH, pWQ);
}

void SysWakeUp(pWait_queue_head_t wqh)
{
	wake_up((wait_queue_head_t *)wqh);
}


void SysPollWait(void *FileContext, pWait_queue_head_t head, void *pollstruct)
{
	poll_wait((struct file *)FileContext,
			(wait_queue_head_t *)head,
			(struct poll_table_struct *)pollstruct);
}



#if 0 
#if 0 





int SysSpinLockInit(pSpinlock_t *slock)
{
	spinlock_t *plock = NULL;
	
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	plock = kmalloc(sizeof(*plock), GFP_KERNEL);

	if( plock ) {
		*plock = SPIN_LOCK_UNLOCKED;
		*slock = (pSpinlock_t) plock;
		return 0;
	}
	#else
	plock = kmalloc(1, GFP_KERNEL);
	*slock = (pSpinlock_t) plock;
	if( plock )
		return 0;
	#endif
	
	*slock = NULL;			
	return -ENOMEM;
}

void SysSpinLock(pSpinlock_t slock)
{
	spinlock_t *lock = slock;
	if( lock==NULL ) { BUG();  return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_lock(lock);
	#else
	
	#endif 
}

void SysSpinUnlock(pSpinlock_t slock)
{
	spinlock_t *lock = slock;
	if( lock==NULL ) { BUG();  return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_unlock(lock);
	#else
	
	#endif 
}

void SysSpinLockIrqsave(pSpinlock_t slock, unsigned long *flags)
{
	spinlock_t *lock = slock;
	if( lock==NULL ) { BUG(); return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_lock_irqsave(lock, *flags);
	#else
	
	#endif 
}

void SysSpinUnlockIrqrestore(pSpinlock_t slock, unsigned long flags)
{
	spinlock_t *lock = slock;
	if( lock==NULL ) { BUG(); return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)  
	spin_unlock_irqrestore(lock, flags);
	#else
	
	#endif 
}

int SysSpinLockFini(pSpinlock_t slock)
{
	if( slock ) {
		kfree(slock);
		return 0;
	}

	return -EINVAL;
}

#else 

typedef struct _aver_spinlock_t
{
    spinlock_t  lock;
    unsigned int index;
    int used;
}_aver_spinlock_t;

#define MAX_AVER_SPINLOCK 256
static struct _aver_spinlock_t g_spinlock_ar[MAX_AVER_SPINLOCK];
static atomic_t g_spinlock_lock;


int SysSpinLockInit(pSpinlock_t *slock)
{
    unsigned int i=0;
    *slock = NULL;

	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 

    
    while(1) {
        if( SysAtomicAddReturn(1, &g_spinlock_lock)==1 ) break;
        else {
            SysAtomicSubReturn(1, &g_spinlock_lock);
            mdelay(1);
        }
    }

    for(i=0; i<MAX_AVER_SPINLOCK; ++i) {
        if( g_spinlock_ar[i].used==0 ) {
            g_spinlock_ar[i].used = 1;
            g_spinlock_ar[i].index = i;
            spin_lock_init(&g_spinlock_ar[i].lock);

            *slock = (pSpinlock_t) (g_spinlock_ar+i);
            break;
        }
    }

    
    SysAtomicSubReturn(1, &g_spinlock_lock);

    if( *slock ) {
    
        return 0;
    }
    else {
    
        BUG(); 
    }

	#else
	*slock = (pSpinlock_t) 1;
    return 0;
	#endif
	
	return -ENOMEM;
}

void SysSpinLock(pSpinlock_t slock)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG();  return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_lock(&lock->lock);
	#else
	
	#endif 
}

void SysSpinUnlock(pSpinlock_t slock)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG();  return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_unlock(&lock->lock);
	#else
	
	#endif 
}

void SysSpinLockIrqsave(pSpinlock_t slock, unsigned long *flags)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG(); return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_lock_irqsave(&lock->lock, *flags);
	#else
	
	#endif 
}

void SysSpinUnlockIrqrestore(pSpinlock_t slock, unsigned long flags)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG(); return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)  
	spin_unlock_irqrestore(&lock->lock, flags);
	#else
	
	#endif 
}

int SysSpinLockFini(pSpinlock_t slock)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;

	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	lock->used = 0;
	return 0;
    #endif

}
#endif 
#endif 

int SysSemInit(pSemaphore *sem, enum sem_type type)
{
	struct semaphore *tmp;
	
	*sem = NULL;
	tmp = kmalloc(sizeof(struct semaphore), GFP_KERNEL);
	if( !tmp ) return -ENOMEM;
	
	if(st_Mutex == type) {
		init_MUTEX(tmp);
		*sem = (pSemaphore) tmp;
	}
	else if(st_MutexLocked == type) {
		init_MUTEX_LOCKED(tmp);
		*sem = (pSemaphore) tmp;
	}
	else {
		kfree(tmp);
		return -EINVAL;
	}

	return 0;
}

int SysSemDown(pSemaphore sem)
{
	if(!sem) BUG();
#if 0 
	down_interruptible((struct semaphore *)sem);
#else
	down((struct semaphore *)sem);
#endif
	return 0;
}


int SysSemDownInterruptible(pSemaphore sem)
{
	if(!sem) BUG();
	return down_interruptible((struct semaphore *)sem);
}


void SysSemUp(pSemaphore sem)
{
	if(!sem) BUG();
	up((struct semaphore *)sem);
}

void SysSemFini(pSemaphore sem)
{
	if(!sem) BUG();
	kfree(sem);
}

#if 1 


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)

#if 0 
int _SysMutexInit(pMutex *pmutex, const char *name)
{
	*pmutex = kmalloc(sizeof(Mutex), GFP_KERNEL);
	 if( *pmutex==NULL ) return -ENOMEM;

	 (*pmutex)->context = kmalloc(sizeof(struct mutex), GFP_KERNEL);
	 if( (*pmutex)->context==NULL ) { 
	 	kfree((*pmutex));
		return -ENOMEM;
	 }
	 (*pmutex)->key = kmalloc(sizeof(struct lock_class_key), GFP_KERNEL);
	 if( (*pmutex)->key==NULL ) {
		kfree((*pmutex)->context);
		kfree(*pmutex);
		return -ENOMEM;
	 }

	 __mutex_init((struct mutex *)(*pmutex)->context,
	 		name,
			(struct lock_class_key *)(*pmutex)->key);
	 return 0;
}

void SysMutexLock(pMutex mutex)
{
	mutex_lock((struct mutex *)mutex->context);
}

int SysMutexLockInterruptible(pMutex mutex)
{
	return mutex_lock_interruptible((struct mutex *)mutex->context);
}

void SysMutexUnlock(pMutex mutex)
{
	mutex_unlock((struct mutex *)mutex->context);
}

void SysMutexFree(pMutex mutex)
{
	kfree(mutex->context);
	kfree(mutex->key);
	kfree(mutex);
}

#else 

typedef struct _aver_mutex_t
{
    struct mutex lock;
    struct lock_class_key key;
    unsigned int index;
    int used;
}_aver_mutex_t;

#define MAX_AVER_MUTEX 256
static struct _aver_mutex_t g_mutex_ar[MAX_AVER_MUTEX];
static atomic_t g_mutex_lock;


int _SysMutexInit(pMutex *pmutex, const char *name)
{
    unsigned int i=0;
    *pmutex = NULL;

    
    while(1) {
        if( SysAtomicAddReturn(1, &g_mutex_lock)==1 ) break;
        else {
            SysAtomicSubReturn(1, &g_mutex_lock);
            mdelay(1);
        }
    }

    for(i=0; i<MAX_AVER_MUTEX; ++i) {
        if( g_mutex_ar[i].used==0 ) {
            g_mutex_ar[i].used = 1;
            g_mutex_ar[i].index = i;
            __mutex_init(&g_mutex_ar[i].lock, name, &g_mutex_ar[i].key);

            *pmutex = (pMutex)&(g_mutex_ar[i]);
            break;
        }
    }

    
    SysAtomicSubReturn(1, &g_mutex_lock);

    if( *pmutex ) return 0;
    else return -ENOMEM;
}

void SysMutexLock(pMutex mutex)
{
	mutex_lock(& ((_aver_mutex_t *)mutex)->lock);
}

int SysMutexLockInterruptible(pMutex mutex)
{
	return mutex_lock_interruptible(& ((_aver_mutex_t *)mutex)->lock);
}

void SysMutexUnlock(pMutex mutex)
{
	mutex_unlock(& ((_aver_mutex_t *)mutex)->lock);
}

void SysMutexFree(pMutex mutex)
{
    _aver_mutex_t *m = (_aver_mutex_t *)mutex;

    m->used = 0;
}

#endif 

#else



int _SysMutexInit(pMutex *pmutex, const char *name)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,15)
	*pmutex = kmalloc(sizeof(struct mutex), GFP_KERNEL);
	 if( *pmutex==NULL ) return -ENOMEM;

	 __mutex_init((struct mutex *)*pmutex, name);
	 return 0;
#else 
	return SysSemInit((pSemaphore *)pmutex, st_Mutex);
#endif
}

void SysMutexLock(pMutex mutex)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,15)
	mutex_lock((struct mutex *)mutex);
#else 
	SysSemDown((pSemaphore)mutex);
#endif
}

int SysMutexLockInterruptible(pMutex mutex)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,15)
	return mutex_lock_interruptible((struct mutex *)mutex);
#else 
	return SysSemDownInterruptible((pSemaphore)mutex);
#endif
}

void SysMutexUnlock(pMutex mutex)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,15)
	mutex_unlock((struct mutex *)mutex);
#else 
	SysSemUp((pSemaphore)mutex);
#endif
}

void SysMutexFree(pMutex mutex)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,15)
	kfree(mutex);
#else 
	SysSemFini((pSemaphore)mutex);
#endif
}

#endif

#endif 


void SysJSleep(unsigned long j)
{
#if 0 
	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_timeout(j);
#else
	set_current_state(TASK_INTERRUPTIBLE);
	while(j) {
        j = schedule_timeout(j);
    }
#endif 
}


void SysMSleep(int miliSec)
{
#if 0 
	unsigned long j = (HZ*miliSec)/1000;
	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_timeout(j);
#else
    unsigned long j = msecs_to_jiffies(miliSec);
	set_current_state(TASK_INTERRUPTIBLE);
    while(j) {
        j = schedule_timeout(j);
    }
#endif 
}

void SysUSleep(int microSec)
{
	udelay(microSec);
}


void SysMDelay(int miliSec)
{
	mdelay(miliSec);
}

int SysInAtomic(void)
{
	if(in_atomic() || irqs_disabled()) {	
		return 1;
	}
	else {
		return 0;
	}
}


#if 0





void * SysKMalloc(unsigned int size, kmalloc_type flag)
{
	u32	type;
	void	*ptr = NULL;

	
	if(in_atomic() || irqs_disabled()) {
		flag = eKMALLOC_ATOMIC;
	}
	

	switch( flag ) {
	case eKMALLOC_ATOMIC:
		type = GFP_ATOMIC;
		break;

	case eKMALLOC_KERNEL:
		type = GFP_KERNEL;
		break;

	case eKMALLOC_DMA:
		type = GFP_DMA;
		break;

	default:
		return ptr;
	}

	if(in_atomic()) type = GFP_ATOMIC;
	ptr = kmalloc(size, type);
	if( ptr ) memset(ptr, 0, size);
	DBG_OSDEP("kmalloc %s @ %p\n", 
		(GFP_DMA==type)? "DMA" :
		(GFP_KERNEL==type)? "KERN" :
		(GFP_ATOMIC==type)? "ATOMIC" : "Unknown",
		ptr);

	return ptr;
}

void SysKFree(void *ptr)
{
	DBG_OSDEP("kfree %p\n", ptr);
	if(!ptr) BUG();
	kfree(ptr);
}


#else

#define AVER_MEM_BARRIER_LEN (16)
#define AVER_MEM_BARRIER (0x7a)

typedef struct __aver_mem_hdr
{
    const char      *file;  
    unsigned long   line;   
    unsigned long   length; 
    kmalloc_type    flag;   

    unsigned char   barrier[AVER_MEM_BARRIER_LEN];    
}__aver_mem_hdr;

static void PrintBarrier(unsigned char *barrier, unsigned long len)
{
    unsigned long i;
    for(i=0; i<len; ++i) {

        if( i%8 == 0 ) {
            SysPrintk("\nBarrier[%d] : ", i);
        }

        SysPrintk("0x%02x ", barrier[i]);
    }

    SysPrintk("\n");
}





void * SysKMallocWithTag(unsigned int size, kmalloc_type flag, const char *file, unsigned long line)
{
	u32	type;
	void *ptr = NULL;
    __aver_mem_hdr *hdr=NULL;

	
	if(in_atomic() || irqs_disabled()) {
		flag = eKMALLOC_ATOMIC;
	}
	

	switch( flag ) {
	case eKMALLOC_ATOMIC:
		type = GFP_ATOMIC;
		break;

	case eKMALLOC_KERNEL:
		type = GFP_KERNEL;
		break;

	case eKMALLOC_DMA:
		type = GFP_DMA;
		break;

	default:
		return ptr;
	}

	if(in_atomic()) type = GFP_ATOMIC;

    
	ptr = kmalloc(size+sizeof(__aver_mem_hdr)+AVER_MEM_BARRIER_LEN, type);

    
	if( ptr ) {
        hdr = (__aver_mem_hdr *)ptr;
        SysMemSet(ptr, 0, size+sizeof(__aver_mem_hdr)+AVER_MEM_BARRIER_LEN);

        
        hdr->file = file;
        hdr->line = line;
        hdr->length = size;
        hdr->flag = flag;
        
        SysMemSet(hdr->barrier, AVER_MEM_BARRIER, AVER_MEM_BARRIER_LEN);
        
        SysMemSet(ptr+size+sizeof(__aver_mem_hdr), AVER_MEM_BARRIER, AVER_MEM_BARRIER_LEN);

        ptr = ((unsigned char *)ptr) + sizeof(__aver_mem_hdr);
    }

	DBG_fOSDEP(" %s @ %p from %s line %ld, hdr @ %p\n", 
		(GFP_DMA==type)? "DMA" :
		(GFP_KERNEL==type)? "KERN" :
		(GFP_ATOMIC==type)? "ATOMIC" : "Unknown",
		ptr, file, line, hdr);

	return ptr;
}

void SysKFreeWithTag(void *ptr, const char *file, unsigned long line)
{
    unsigned long i;
    __aver_mem_hdr *hdr;
	DBG_fOSDEP(" %p, from %s line %ld\n", ptr, file, line);
	if(!ptr) BUG();

    
    hdr = (__aver_mem_hdr *) ((unsigned char *)ptr-sizeof(__aver_mem_hdr));
	DBG_fOSDEP("SysKFree header @  %p\n", hdr);
    
    
    for(i=0; i<AVER_MEM_BARRIER_LEN; ++i) {
        if( hdr->barrier[i] != AVER_MEM_BARRIER ) {
            SysPrintk("\n==============================================================\n");
            SysPrintk("SysKFree got memory %p with corrupted header, which follows.", ptr);        
            PrintBarrier(hdr->barrier, AVER_MEM_BARRIER_LEN);
            SysPrintk("==============================================================\n\n");
            return; 
        }
    }

    DBG_fOSDEP("Mem allocate from %s, len %ld\n", hdr->file, hdr->line);

    
    for(i=0; i<AVER_MEM_BARRIER_LEN; ++i) {
        if( ((unsigned char *)ptr + hdr->length)[i] != AVER_MEM_BARRIER ) {

            SysPrintk("\n==============================================================\n");
            SysPrintk("SysKFree detected memory corruption on ptr %p, size=%ld, flag=0x%x, allocated in file %s line %ld",
                    ptr, hdr->length, hdr->flag, hdr->file, hdr->line);

            PrintBarrier(((unsigned char *)ptr + hdr->length), AVER_MEM_BARRIER_LEN);
            SysPrintk("==============================================================\n\n");
        }
    }

	kfree(hdr);
}

#endif 


void SysMemSet(void *ptr, unsigned char val, unsigned int size)
{
	memset(ptr, val, size);
}

void *SysMemCpy(void *dest, const void *src, unsigned int size)
{
	return memcpy(dest, src, size);
}

int SysMemCmp(const void *dest, const void *src, unsigned int size)
{
	return memcmp(dest, src, size);
}

void *SysReAlloc(void *ptr, unsigned int oldsize, unsigned int newsize)
{
	void *newptr = NULL;

	newptr = SysKMalloc(newsize, eKMALLOC_KERNEL);
	SysMemCpy(newptr, ptr, oldsize);
	SysKFree(ptr);
	return newptr;
}

void *SysMemMove(void *ptr, const void *src, unsigned int size)
{
	return memmove(ptr, src, size);
}


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,8)


int SysPrintk(const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start(args, fmt);
	ret = vprintk(fmt, args);
	va_end(args);

#if 0
	if(SysInAtomic())
		SysMDelay(500);
	else
		SysMSleep(500);
#endif	
	return ret;
}

int SysVPrintk(const char *fmt, va_list args)
{
	return vprintk(fmt, args);
}


#else

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17) 
static spinlock_t logbuf_lock = __SPIN_LOCK_UNLOCKED(logbuf_lock);
#else
static spinlock_t logbuf_lock = SPIN_LOCK_UNLOCKED;
#endif 

static char logbuf[1024];

int SysPrintk(const char *fmt, ...)
{
	va_list args;
	int ret;

	va_start(args, fmt);
	ret = SysVPrintk(fmt, args);
	va_end(args);

	return ret;
}

int SysVPrintk(const char *fmt, va_list args)
{
	int ret;
	unsigned long flags;
	
	spin_lock_irqsave(&logbuf_lock, flags);
	ret = vsprintf(logbuf, fmt, args); 
	spin_unlock_irqrestore(&logbuf_lock, flags);

	ret = printk(logbuf);

	return ret;
}

#endif


int SysSnPrintf(char *buf, unsigned int size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsnprintf(buf, size, fmt, args);
	va_end(args);
	return i;
}






#if 0 
void SysKrefInit(pKref *ppkref)
#else
void SysKrefInit(pKref *ppkref, cb_kref cb_func)
#endif
{
	struct kref *pkref;

	pkref = kmalloc(sizeof(*pkref), GFP_KERNEL);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,8) 
	if( pkref ) kref_init(pkref);			
#else
	if( pkref ) kref_init(pkref, (void (*)(struct kref *))cb_func);			
#endif
	*ppkref = pkref;
}




void SysKrefPut(pKref kref, cb_kref cb_func)
{
	struct kref *pkref = (struct kref *) kref;

	DBG_fOSDEP("pkfre=%p, func=%p\n", pkref, cb_func);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,8) 
	kref_put(pkref, (void (*)(struct kref *))cb_func);
#else
	kref_put(pkref);
#endif	
}





void SysKrefGet(pKref kref)
{
	struct kref *pkref = (struct kref *) kref;
	DBG_fOSDEP("pkfre=%p, func=%p\n", pkref);
    kref_get(pkref);
}





void SysKrefFree(pKref kref)
{
	if(!kref) BUG();
	kfree(kref);
}

void SysLockKernel()
{
	lock_kernel();
}

void SysUnlockKernel()
{
	unlock_kernel();
}

unsigned long SysCopyToUser(void *to, const void *from, unsigned long n)
{
	return copy_to_user(to, from, n);
}

unsigned long SysCopyFromUser(void *to, const void *from, unsigned long n)
{
	return copy_from_user(to, from, n);
}

unsigned int SysStrLen(const char *str)
{
	return strlen(str);
}


unsigned long SysSimpleStrtoul(const char *cp,char **endp,unsigned int base)
{
	return simple_strtoul(cp, endp, base);
}


char *SysStrNCpy(char *dest, const char *src, size_t count)
{
	return strncpy(dest, src, count);
}

void SysSetCurrentState(int state)
{
	int tmp;

	switch( state ) {
	case PROC_RUNNING:
		tmp = TASK_RUNNING; break;
	case PROC_INTERRUPTIBLE:
		tmp = TASK_INTERRUPTIBLE; break;
	case PROC_UNINTERRUPTIBLE:
		tmp = TASK_UNINTERRUPTIBLE; break;
	default:
		DBG_OSDEP("Task state unknown\n");
		return;
	}

	set_current_state(tmp);
}

int SysSignalPending(void)
{
	return signal_pending(current);
}

void SysSchedule(void)
{
	schedule();
}



signed long SysScheduleTimeout(signed long timeout)
{
        return schedule_timeout(timeout);
}

unsigned long SysGetHZ(void)
{
	return HZ;
}


void SysGetPageParam(unsigned int *pg_sz, unsigned int *pg_shift)
{
	if( pg_sz ) *pg_sz = PAGE_SIZE;
	if( pg_shift ) *pg_shift = PAGE_SHIFT;
}

pPage SysVmallocToPage(u8 *va)
{
	struct page *p = vmalloc_to_page(va);
	
	return (pPage) p;
}

void *SysPageAddress(pPage pg)
{
	return page_address((struct page *)pg);
}

int SysPageHighMem(pPage pg)
{
	struct page *p = (struct page *)pg;
	REF_PARAM(p);
	
	return PageHighMem(p);
}

void SysGetPage(pPage pg)
{
	struct page *p = (struct page *)pg;
	get_page(p);
}

void SysPciDmaSyncSingleForCpu(pPci_dev pci, dma_addr_t pa, int size, 
				dma_type_t type)
{
	pci_dma_sync_single_for_cpu(pci, pa, size, type);
}

void SysPciDmaSyncSgForCpu(pPci_dev pci, pScatterlist pSGList, int count,
				dma_type_t type)
{
	struct scatterlist *sg = (struct scatterlist *)pSGList;
	pci_dma_sync_sg_for_cpu(pci, sg, count, type);
}







void SysFillSG(pScatterlist pSGList, int i, pPage pg, int sz)
{
	struct scatterlist *sg = (struct scatterlist *)pSGList;


	
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,23) 
	sg[i].page = (struct page *) pg;
	sg[i].length = sz;
#else
	sg_set_page(sg+i, pg, sz, 0);
    sg_dma_len(sg+i) = sz; 
#endif 
}

dma_addr_t SysGetSgDmaAddress(pScatterlist pSG, int i)
{
	struct scatterlist *sg = (struct scatterlist *)pSG;
	
	return sg_dma_address(&sg[i]);
}

size_t SysGetSgDmaLen(pScatterlist pSG, int i)
{
	struct scatterlist *sg = (struct scatterlist *)pSG;
	

	return sg_dma_len(&sg[i]);
}

pPage SysGetSgPage(pScatterlist pSG, int i)
{
	struct scatterlist *sg = (struct scatterlist *)pSG;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,23) 
	return sg[i].page;
#else
	return sg_page(sg+i);
#endif 
}

void SysSetSgDmaAddress(pScatterlist pSG, int i, dma_addr_t pa)
{
	struct scatterlist *sg = (struct scatterlist *)pSG;
	
	sg_dma_address(&sg[i]) = pa;
}

void SysSetSgDmaLen(pScatterlist pSG, int i, size_t len)
{
	struct scatterlist *sg = (struct scatterlist *)pSG;
	
	sg_dma_len(&sg[i]) = len;
}

pScatterlist SysSGAlloc(int count)
{
	struct scatterlist *sg = 
		SysKMalloc(sizeof(*sg)*count, eKMALLOC_KERNEL);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,23) 
	if( sg ) sg_init_table(sg, count);
#endif
	return (pScatterlist)sg;
}





int SysPciMapSg(pPci_dev pci, pScatterlist pSG, int entry, dma_type_t type)
{
	enum dma_data_direction dir = DMA_NONE;
	switch( type ) {
	case eDMA_DEV_WRITE:	
		dir = DMA_TO_DEVICE;
		break;

	case eDMA_DEV_READ:	
		dir = DMA_FROM_DEVICE;
		break;

	default:
		return -EINVAL;
	}

	return pci_map_sg((struct pci_dev *)pci, (struct scatterlist *)pSG,
			entry, dir);
}

void SysPciUnmapSg(pPci_dev pci, pScatterlist pSG, int entry, dma_type_t type)
{
	enum dma_data_direction dir = DMA_NONE;
	switch( type ) {
	case eDMA_DEV_WRITE:	
		dir = DMA_TO_DEVICE;
		break;

	case eDMA_DEV_READ:	
		dir = DMA_FROM_DEVICE;
		break;

	default:
		return;
	}

	pci_unmap_sg((struct pci_dev *)pci, (struct scatterlist *)pSG,
			entry, dir);
}





dma_addr_t SysPciMapSingle(pPci_dev pci, void *va, int size, dma_type_t type)
{
	dma_addr_t pa = (dma_addr_t)0;

	enum dma_data_direction dir = DMA_NONE;
	switch( type ) {
	case eDMA_DEV_WRITE:	
		dir = DMA_TO_DEVICE;
		break;

	case eDMA_DEV_READ:	
		dir = DMA_FROM_DEVICE;
		break;

	default:
		return pa;
	}

	pa = pci_map_single((struct pci_dev *)pci, va, size, dir);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26) 
	if( pci_dma_mapping_error((struct pci_dev *)pci, pa) ) pa = (dma_addr_t)0;
#else
	if( pci_dma_mapping_error(pa) ) pa = (dma_addr_t)0;
#endif
	return pa;
}

void SysPciUnmapSingle(pPci_dev pci, dma_addr_t pa, int size, dma_type_t type)
{
	enum dma_data_direction dir = DMA_NONE;
	switch( type ) {
	case eDMA_DEV_WRITE:	
		dir = DMA_TO_DEVICE;
		break;

	case eDMA_DEV_READ:	
		dir = DMA_FROM_DEVICE;
		break;

	default:
		return;
	}

	pci_unmap_single((struct pci_dev *)pci, pa, size, dir);
}

void *SysPciAllocConsistent(pPci_dev pci, size_t sz, dma_addr_t *pa)
{
	return pci_alloc_consistent((struct pci_dev *)pci, sz, pa);
}

void SysPciFreeConsistent(pPci_dev pci, size_t sz, void *va, dma_addr_t pa)
{
	return pci_free_consistent((struct pci_dev *)pci, sz, va, pa);
}

int SysPciEnableDevice(pPci_dev pci)
{
	return pci_enable_device((struct pci_dev *)pci);
}

void SysPciSetMaster(pPci_dev pci)
{
	pci_set_master((struct pci_dev *)pci);
}

int SysPciMMIOMap(pPci_dev pci, int resource, const char *devname,
			unsigned long *mmio, unsigned int *mmio_size)
{
	int err = 0;
	struct pci_dev *pci_dev = (struct pci_dev *)pci;

	DBG_OSDEP("pci=0x%p, resource=%d, name=%s\n", pci, resource, devname);

	if (!request_mem_region(pci_resource_start(pci_dev, resource),
				pci_resource_len(pci_dev, resource),
				devname)) {
		DBG_OSDEP("request_mem_region failed, err=%d\n", err);
		err = -EBUSY;
		goto done;
	}

	*mmio = (unsigned long)ioremap(pci_resource_start(pci_dev, resource),
			pci_resource_len(pci_dev, resource));
	if(!(*mmio)) {
		DBG_OSDEP("ioremap failed\n");
		err = -ENOMEM;
	}

	*mmio_size = pci_resource_len(pci_dev, resource);

	DBG_OSDEP("done, mmio=0x%lx, size=%d\n", *mmio, *mmio_size);
done:
	return err;
}

void SysPciMMIOUnMap(pPci_dev pci, int resource, unsigned long mmio)
{
	struct pci_dev *pci_dev = (struct pci_dev *)pci;
	DBG_OSDEP("pci=0x%p, resource=%d, mmio=0x%lx\n", pci, resource,
			mmio);
	iounmap((void *)mmio);
	release_mem_region(pci_resource_start(pci_dev, resource),
				pci_resource_len(pci_dev, resource));
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,8) 
unsigned int SysIORead32(void *addr)
{
	return ioread32(addr);
}

void SysIOWrite32(void *addr, unsigned int val)
{
	iowrite32(val, addr);
}


unsigned int SysIORead16(void *addr)
{
	return ioread16(addr);
}

void SysIOWrite16(void *addr, unsigned int val)
{
	iowrite16(val, addr);
}

unsigned int SysIORead8(void *addr)
{
	return ioread8(addr);
}

void SysIOWrite8(void *addr, unsigned int val)
{
	iowrite8(val, addr);
}


#else

unsigned int SysIORead32(void *addr)
{
	return readl(addr);
}

void SysIOWrite32(void *addr, unsigned int val)
{
	return writel(val, addr);
}


unsigned int SysIORead16(void *addr)
{
	return readw(addr);
}

void SysIOWrite16(void *addr, unsigned int val)
{
	return writew(val, addr);
}

unsigned int SysIORead8(void *addr)
{
	return readb(addr);
}

void SysIOWrite8(void *addr, unsigned int val)
{
	return writeb(val, addr);
}


#endif

int SysPciReadCfgByte(pPci_dev pci, unsigned int w, u8 *v)
{
	return pci_read_config_byte((struct pci_dev *)pci, w, v);
}

int SysPciReadCfgWord(pPci_dev pci, unsigned int w, u16 *v)
{
	return pci_read_config_word((struct pci_dev *)pci, w, v);
}

int SysPciReadCfgDWord(pPci_dev pci, unsigned int w, u32 *v)
{
	return pci_read_config_dword((struct pci_dev *)pci, w, v);
}

int SysPciWriteCfgByte(pPci_dev pci, unsigned int w, u8 v)
{
	return pci_write_config_byte((struct pci_dev *)pci, w, v);
}

int SysPciWriteCfgWord(pPci_dev pci, unsigned int w, u16 v)
{
	return pci_write_config_word((struct pci_dev *)pci, w, v);
}

int SysPciWriteCfgDWord(pPci_dev pci, unsigned int w, u32 v)
{
	return pci_write_config_dword((struct pci_dev *)pci, w, v);
}

unsigned short SysPciVendorId(pPci_dev pci)
{
	return ((struct pci_dev *)pci)->vendor;
}

unsigned short SysPciDeviceId(pPci_dev pci)
{
	return ((struct pci_dev *)pci)->device;
}

unsigned short SysPciSubVendorId(pPci_dev pci)
{
	return ((struct pci_dev *)pci)->subsystem_vendor;
}

unsigned short SysPciSubDeviceId(pPci_dev pci)
{
	return ((struct pci_dev *)pci)->subsystem_device;
}

void *SysPciGetDevice(pPci_dev pci)
{
	return &((struct pci_dev *)pci)->dev;
}

void SysPciSetDrvData(pPci_dev pci, void *data)
{
	pci_set_drvdata((struct pci_dev *)pci, data);
}

void *SysPciGetDrvData(pPci_dev pci)
{
	return pci_get_drvdata((struct pci_dev *)pci);
}

unsigned int SysPciGetIrq(pPci_dev pci)
{
	return ((struct pci_dev *)pci)->irq;
}


void SysPciGetId(pPci_dev pci, unsigned short *vendor,
		unsigned short *device, unsigned short *subvendor,
		unsigned short *subdevice)
{
	if( !pci )
		return;
	if(vendor)
		*vendor = ((struct pci_dev *)pci)->vendor;
	if(device)
		*device = ((struct pci_dev *)pci)->device;
	if(subvendor)
		*subvendor = ((struct pci_dev *)pci)->subsystem_vendor;
	if(subdevice)
		*subdevice = ((struct pci_dev *)pci)->subsystem_device;
}


const char *SysPciName(pPci_dev pci)
{
	return pci_name((struct pci_dev *)pci);
}





u8 *SysVmalloc32(unsigned long size)
{
	return vmalloc_32(size);
}

void SysVFree(void *addr)
{
	vfree(addr);
}






int SysInitTimer(pTimer_list *pTimer)
{
	
	struct timer_list *timer = SysKMalloc(sizeof(*timer), eKMALLOC_KERNEL); 

	*pTimer = (pTimer_list) timer;
	if( timer ) {
		init_timer(timer);
		return 0;
	}

	return -ENOMEM;
}





void SysFillTimer(pTimer_list Timer, cb_timer func, void *data)
{
	struct timer_list *timer = (struct timer_list *)Timer;

	timer->function = func;
	timer->data = (unsigned long)data;
}





int SysModTimer(pTimer_list Timer, unsigned long ms)
{
	ms *= HZ;
	ms /= 1000;		
	return mod_timer((struct timer_list *)Timer, ms+jiffies);
}

int SysDelTimer(pTimer_list Timer)
{
	return del_timer_sync((struct timer_list *)Timer);
}


unsigned long SysGetJiffies(void)
{
	return jiffies;
}



#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
struct jump_struct
{
	struct work_struct w;
	void *data;
	cb_workqueue func;
};


static void work_handler(struct work_struct *work)
{
	struct jump_struct *tmp   = list_entry(work, struct jump_struct, w);
	void *user_data 	  = tmp->data;
	cb_workqueue user_handler = tmp->func;
	
	user_handler(user_data);
}
#endif

int SysInitWork(pWorkStruct *work, cb_workqueue func, void *data)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	struct jump_struct *jump;

    jump = SysKMalloc(sizeof(struct jump_struct), eKMALLOC_KERNEL); 
	memset(jump,0,sizeof(jump));
	
	jump->data   = data;
	jump->func   = func;
	
	
	*work	     = (pWorkStruct) (jump);
	INIT_WORK( &(jump->w), work_handler);
#else


    struct work_struct *w = SysKMalloc(sizeof(*w), eKMALLOC_KERNEL); 
	if( !w ) return -ENOMEM;
	*work = (pWorkStruct) w;
	INIT_WORK(w, func, data);
#endif

	return 0;
}


int SysSubmitWork(pWorkStruct work)
{



#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	struct jump_struct *j = (struct jump_struct*)work;
	return schedule_work(&(j->w));
#else
  	struct work_struct *w = (struct work_struct *)work;
	return schedule_work(w);
#endif

}

void SysFlushWork(void)
{
	flush_scheduled_work();
}

struct IrqContext
{
	unsigned int	irq;
	KIrqHandler	handler;
	void*		data;
};



#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
static irqreturn_t irq_handler(int irq, void *context)
#else
static irqreturn_t irq_handler(int irq, void *context, struct pt_regs *regs)
#endif

{
	int handled = 0;
	struct IrqContext *ic = context;

	
	if( !ic || !ic->handler ) return IRQ_RETVAL(handled);
	handled = (ic->handler(ic->data)==0)? 1 : 0;

	return IRQ_RETVAL(handled);
}

KIrqObject SysRequestIrq(unsigned int irq, KIrqHandler handler,
			const char *name, void *data)
{
	int ret = 0;
	struct IrqContext *ic = SysKMalloc(sizeof(*ic), eKMALLOC_KERNEL);

	if( !ic ) return NULL;
	ic->irq = irq;
	ic->handler = handler;
	ic->data = data;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24) 
	ret = request_irq(irq, irq_handler, SA_SHIRQ|SA_INTERRUPT, name, ic);
#else
	ret = request_irq(irq, irq_handler, IRQF_SHARED, name, ic);
#endif 
	if( ret ) {
		DBG_fOSDEP("request_irq failed with %d\n", ret);
		SysKFree(ic);
		return NULL;
	}

	return (KIrqObject)ic;
}

void SysFreeIrq(KIrqObject obj)
{
	struct IrqContext *ic = obj;
	
	if( !ic ) return;
	
	free_irq(ic->irq, ic);
	SysKFree(ic);
}

int SysPciEnableMsi(pPci_dev pci) 
{
#ifdef CONFIG_PCI_MSI
	struct pci_dev *pci_dev = (struct pci_dev *)pci;
	return pci_enable_msi(pci_dev);
#else
	return 0;
#endif
}

void SysPciDisableMsi(pPci_dev pci)
{
#ifdef CONFIG_PCI_MSI
	struct pci_dev *pci_dev = (struct pci_dev *)pci;
	pci_disable_msi(pci_dev);
#else
	return;
#endif
}

int SysInitCompletion(pCompletion *pobj)
{
	struct completion *comp = kmalloc(sizeof(*comp), GFP_KERNEL);
	DBG_fOSDEP("obj=%p\n", comp);

	if( !comp ) return -ENOMEM;

	init_completion(comp);
	*pobj = (void *)comp;

	return 0;
}


int  SysReInitCompletion(pCompletion pobj)
{
	struct completion *comp = pobj;
	DBG_fOSDEP("obj=%p\n", comp);
	if( !comp ) return -EINVAL;

	init_completion(comp);
	return 0;
}


int SysCompleteAndExit(pCompletion obj, int retval)
{
	DBG_fOSDEP("obj=%p\n", obj);
	if( !obj ) return -EINVAL;
	complete_and_exit((struct completion *)obj, retval);
	return 0;
}

int SysComplete(pCompletion obj)
{
	DBG_fOSDEP("obj=%p\n", obj);
	if( !obj ) return -EINVAL;
	complete((struct completion *)obj);
	return 0;
}

int SysCompleteAll(pCompletion obj)
{
	DBG_fOSDEP("obj=%p\n", obj);
	if( !obj ) return -EINVAL;
	complete_all((struct completion *)obj);
	return 0;
}

int SysWaitForCompletion(pCompletion obj)
{
	DBG_fOSDEP("obj=%p\n", obj);
	wait_for_completion((struct completion *)obj);
	DBG_fOSDEP("obj=%p, done\n", obj);
	return 0;
}



long SysWaitForCompletionTimeout(pCompletion obj, int timeoutMiliSec)
{
    unsigned long timeout;
    long ret;
    DBG_fOSDEP("obj=%p, timeout=%ld\n", obj, timeoutMiliSec);
    timeout = (timeoutMiliSec*HZ)/1000;
    ret = wait_for_completion_timeout((struct completion *)obj, timeout);
	DBG_fOSDEP("obj=%p, done\n", obj);
    return ret;
}


void SysFiniCompletion(pCompletion obj)
{
	DBG_fOSDEP("obj=%p\n", obj);
	kfree(obj);
}

int SysFileFlagIsNonBlock(void *f)
{
	struct file *file = f;

	return (file->f_flags & O_NONBLOCK);
}

void SysGetTimeOfDay(struct timeval *tv)
{
	do_gettimeofday(tv);
}



int SysAtomicInit(pAtomic *patm)
{
	atomic_t *pv = NULL;
	
	if( !patm ) return -EINVAL;
	

	pv = SysKMalloc(sizeof(atomic_t), eKMALLOC_KERNEL); 
	if( !pv ) return -ENOMEM;

	atomic_set(pv, 0);
	*patm = (void *)pv;
	return 0;
}

void SysAtomicSet(pAtomic atm, int value)
{
	atomic_set((atomic_t *)atm, value);
}

int SysAtomicGet(pAtomic atm)
{
	return atomic_read((atomic_t *)atm);
}

void SysAtomicAdd(int value, pAtomic atm)
{
	atomic_add(value, (atomic_t *)atm);
}

void SysAtomicSub(int value, pAtomic atm)
{
        atomic_sub(value, (atomic_t *)atm);
}

void SysAtomicFini(pAtomic atm)
{

	SysKFree(atm); 
}




int SysAtomicAddReturn(int value, pAtomic atm)
{
	return atomic_add_return(value, atm);
}

int SysAtomicSubReturn(int value, pAtomic atm)
{
	return atomic_sub_return(value, atm);
}



void *SysGetCurrent(void)
{
	return (void *)(current);
}




void SysSendSignalToSelf(int sig)
{
    send_sig(sig, current, 0);
}






#ifndef CONFIG_CC_STACKPROTECTOR
void __stack_chk_fail(void)
{
	return;
}
#endif 




const char * aver_usb_board_name[] = {
	[DiBcom_7700]	= "Dibcom 7700",
	[DiBcom_LDR7700]= "Dibcom 7700",
	[Buffalo_M803]	= "M803",
	[AVer_A300]		= "A300",
	[AVer_A300LDR]	= "A300",
	[AVer_A302]		= "A302",
	[AVer_A302LDR]	= "A302",
	[AVer_A808]		= "A808",
	[AVer_A808LDR]	= "A808",
	[AVer_E568]		= "E568",
	[AVer_B300]		= "A300",
	[AVer_B302]		= "A302",
	[AVer_B808]		= "A808",
	[AVer_B568]		= "E568",
	[AVer_A828]		= "A828",
	[AVer_A301]		= "A301",
	[AVer_A333]		= "A333",
	[AVer_A825]		= "A825",	
	[AVer_C038]		= "C038",	
	[AVer_H8261]	= "H826",	
	[AVer_H8262]	= "H826",	
	[AVer_H826D1]	= "H826D",	
	[AVer_H826D2]	= "H826D",	
	[AVer_H826D3]	= "H826D",	
	[AVer_A321D]	= "A321D",	
	[AVer_A321]		= "A321",	
	[AVer_A827]		= "A827",   
	[AVer_A827J]	= "A827",   
	[AVer_A827A1]	= "A827",   
	[AVer_A827A2]	= "A827",   
	[AVer_A827A3]	= "A827",   
	[AVer_Max_Board_ID]	= "Unknown"	
};



const char *SysGetV4L2IOCTLName(unsigned int ioctl)
{
    switch(ioctl) {
     
	case VIDIOC_QUERYCAP :		return "VIDIOC_QUERYCAP";
	case VIDIOC_RESERVED :		return "VIDIOC_RESERVED";
	case VIDIOC_ENUM_FMT :		return "VIDIOC_ENUM_FMT";
	case VIDIOC_G_FMT :		return "VIDIOC_G_FMT";
	case VIDIOC_S_FMT :		return "VIDIOC_S_FMT";
	case VIDIOC_REQBUFS :		return "VIDIOC_REQBUFS";
	case VIDIOC_QUERYBUF :		return "VIDIOC_QUERYBUF";
	case VIDIOC_G_FBUF :		return "VIDIOC_G_FBUF";
	case VIDIOC_S_FBUF :		return "VIDIOC_S_FBUF";
	case VIDIOC_OVERLAY :		return "VIDIOC_OVERLAY";
	case VIDIOC_QBUF :		return "VIDIOC_QBUF";
	case VIDIOC_DQBUF :		return "VIDIOC_DQBUF";
	case VIDIOC_STREAMON :		return "VIDIOC_STREAMON";
	case VIDIOC_STREAMOFF :		return "VIDIOC_STREAMOFF";
	case VIDIOC_G_PARM :		return "VIDIOC_G_PARM";
	case VIDIOC_S_PARM :		return "VIDIOC_S_PARM";
	case VIDIOC_G_STD :		return "VIDIOC_G_STD";
	case VIDIOC_S_STD :		return "VIDIOC_S_STD";
	case VIDIOC_ENUMSTD :		return "VIDIOC_ENUMSTD";
	case VIDIOC_ENUMINPUT :		return "VIDIOC_ENUMINPUT";
	case VIDIOC_G_CTRL :		return "VIDIOC_G_CTRL";
	case VIDIOC_S_CTRL :		return "VIDIOC_S_CTRL";
	case VIDIOC_G_TUNER :		return "VIDIOC_G_TUNER";
	case VIDIOC_S_TUNER :		return "VIDIOC_S_TUNER";
	case VIDIOC_G_AUDIO :		return "VIDIOC_G_AUDIO";
	case VIDIOC_S_AUDIO :		return "VIDIOC_S_AUDIO";
	case VIDIOC_QUERYCTRL :		return "VIDIOC_QUERYCTRL";
	case VIDIOC_QUERYMENU :		return "VIDIOC_QUERYMENU";
	case VIDIOC_G_INPUT :		return "VIDIOC_G_INPUT";
	case VIDIOC_S_INPUT :		return "VIDIOC_S_INPUT";
	case VIDIOC_G_OUTPUT :		return "VIDIOC_G_OUTPUT";
	case VIDIOC_S_OUTPUT :		return "VIDIOC_S_OUTPUT";
	case VIDIOC_ENUMOUTPUT :		return "VIDIOC_ENUMOUTPUT";
	case VIDIOC_G_AUDOUT :		return "VIDIOC_G_AUDOUT";
	case VIDIOC_S_AUDOUT :		return "VIDIOC_S_AUDOUT";
	case VIDIOC_G_MODULATOR :		return "VIDIOC_G_MODULATOR";
	case VIDIOC_S_MODULATOR :		return "VIDIOC_S_MODULATOR";
	case VIDIOC_G_FREQUENCY :		return "VIDIOC_G_FREQUENCY";
	case VIDIOC_S_FREQUENCY :		return "VIDIOC_S_FREQUENCY";
	case VIDIOC_CROPCAP :		return "VIDIOC_CROPCAP";
	case VIDIOC_G_CROP :		return "VIDIOC_G_CROP";
	case VIDIOC_S_CROP :		return "VIDIOC_S_CROP";
	case VIDIOC_G_JPEGCOMP :		return "VIDIOC_G_JPEGCOMP";
	case VIDIOC_S_JPEGCOMP :		return "VIDIOC_S_JPEGCOMP";
	case VIDIOC_QUERYSTD :		return "VIDIOC_QUERYSTD";
	case VIDIOC_TRY_FMT :		return "VIDIOC_TRY_FMT";
	case VIDIOC_ENUMAUDIO :		return "VIDIOC_ENUMAUDIO";
	case VIDIOC_ENUMAUDOUT :		return "VIDIOC_ENUMAUDOUT";
	case VIDIOC_G_PRIORITY :		return "VIDIOC_G_PRIORITY";
	case VIDIOC_S_PRIORITY :		return "VIDIOC_S_PRIORITY";
	case VIDIOC_G_SLICED_VBI_CAP :		return "VIDIOC_G_SLICED_VBI_CAP";
	case VIDIOC_LOG_STATUS :		return "VIDIOC_LOG_STATUS";

	#ifdef VIDIOC_G_EXT_CTRLS
	case VIDIOC_G_EXT_CTRLS :		return "VIDIOC_G_EXT_CTRLS";
	#endif 

	#ifdef VIDIOC_S_EXT_CTRLS
	case VIDIOC_S_EXT_CTRLS :		return "VIDIOC_S_EXT_CTRLS";
	#endif 

	#ifdef VIDIOC_TRY_EXT_CTRLS
	case VIDIOC_TRY_EXT_CTRLS :		return "VIDIOC_TRY_EXT_CTRLS";
	#endif 

	#ifdef VIDIOC_ENUM_FRAMESIZES
	case VIDIOC_ENUM_FRAMESIZES :		return "VIDIOC_ENUM_FRAMESIZES";
	#endif 

	#ifdef VIDIOC_ENUM_FRAMEINTERVALS
	case VIDIOC_ENUM_FRAMEINTERVALS :		return "VIDIOC_ENUM_FRAMEINTERVALS";
	#endif 

	#ifdef VIDIOC_G_ENC_INDEX
	case VIDIOC_G_ENC_INDEX :		return "VIDIOC_G_ENC_INDEX";
	#endif 

	#ifdef VIDIOC_ENCODER_CMD
	case VIDIOC_ENCODER_CMD :		return "VIDIOC_ENCODER_CMD";
	#endif 

	#ifdef VIDIOC_TRY_ENCODER_CMD
	case VIDIOC_TRY_ENCODER_CMD :		return "VIDIOC_TRY_ENCODER_CMD";
	#endif 

	#ifdef VIDIOC_G_CHIP_IDENT
	case VIDIOC_G_CHIP_IDENT :		return "VIDIOC_G_CHIP_IDENT";
	#endif 

    #ifdef VIDIOC_S_HW_FREQ_SEEK
	case VIDIOC_S_HW_FREQ_SEEK :		return "VIDIOC_S_HW_FREQ_SEEK";
    #endif 

    #ifdef __OLD_VIDIOC_
	case VIDIOC_OVERLAY_OLD :		return "VIDIOC_OVERLAY_OLD";
	case VIDIOC_S_PARM_OLD :		return "VIDIOC_S_PARM_OLD";
	case VIDIOC_S_CTRL_OLD :		return "VIDIOC_S_CTRL_OLD";
	case VIDIOC_G_AUDIO_OLD :		return "VIDIOC_G_AUDIO_OLD";
	case VIDIOC_G_AUDOUT_OLD :		return "VIDIOC_G_AUDOUT_OLD";
	case VIDIOC_CROPCAP_OLD :		return "VIDIOC_CROPCAP_OLD";
    #endif 
    default: return "Unknown IOCtrl";
    }

    return "Unknown IOCtrl";
}





DEFINE_SPINLOCK(g_spinlock_raw_0);
DEFINE_SPINLOCK(g_spinlock_raw_1);
DEFINE_SPINLOCK(g_spinlock_raw_2);
DEFINE_SPINLOCK(g_spinlock_raw_3);
DEFINE_SPINLOCK(g_spinlock_raw_4);
DEFINE_SPINLOCK(g_spinlock_raw_5);
DEFINE_SPINLOCK(g_spinlock_raw_6);
DEFINE_SPINLOCK(g_spinlock_raw_7);
DEFINE_SPINLOCK(g_spinlock_raw_8);
DEFINE_SPINLOCK(g_spinlock_raw_9);
DEFINE_SPINLOCK(g_spinlock_raw_10);
DEFINE_SPINLOCK(g_spinlock_raw_11);
DEFINE_SPINLOCK(g_spinlock_raw_12);
DEFINE_SPINLOCK(g_spinlock_raw_13);
DEFINE_SPINLOCK(g_spinlock_raw_14);
DEFINE_SPINLOCK(g_spinlock_raw_15);
DEFINE_SPINLOCK(g_spinlock_raw_16);
DEFINE_SPINLOCK(g_spinlock_raw_17);
DEFINE_SPINLOCK(g_spinlock_raw_18);
DEFINE_SPINLOCK(g_spinlock_raw_19);
DEFINE_SPINLOCK(g_spinlock_raw_20);
DEFINE_SPINLOCK(g_spinlock_raw_21);
DEFINE_SPINLOCK(g_spinlock_raw_22);
DEFINE_SPINLOCK(g_spinlock_raw_23);
DEFINE_SPINLOCK(g_spinlock_raw_24);
DEFINE_SPINLOCK(g_spinlock_raw_25);
DEFINE_SPINLOCK(g_spinlock_raw_26);
DEFINE_SPINLOCK(g_spinlock_raw_27);
DEFINE_SPINLOCK(g_spinlock_raw_28);
DEFINE_SPINLOCK(g_spinlock_raw_29);
DEFINE_SPINLOCK(g_spinlock_raw_30);
DEFINE_SPINLOCK(g_spinlock_raw_31);
DEFINE_SPINLOCK(g_spinlock_raw_32);
DEFINE_SPINLOCK(g_spinlock_raw_33);
DEFINE_SPINLOCK(g_spinlock_raw_34);
DEFINE_SPINLOCK(g_spinlock_raw_35);
DEFINE_SPINLOCK(g_spinlock_raw_36);
DEFINE_SPINLOCK(g_spinlock_raw_37);
DEFINE_SPINLOCK(g_spinlock_raw_38);
DEFINE_SPINLOCK(g_spinlock_raw_39);
DEFINE_SPINLOCK(g_spinlock_raw_40);
DEFINE_SPINLOCK(g_spinlock_raw_41);
DEFINE_SPINLOCK(g_spinlock_raw_42);
DEFINE_SPINLOCK(g_spinlock_raw_43);
DEFINE_SPINLOCK(g_spinlock_raw_44);
DEFINE_SPINLOCK(g_spinlock_raw_45);
DEFINE_SPINLOCK(g_spinlock_raw_46);
DEFINE_SPINLOCK(g_spinlock_raw_47);
DEFINE_SPINLOCK(g_spinlock_raw_48);
DEFINE_SPINLOCK(g_spinlock_raw_49);
DEFINE_SPINLOCK(g_spinlock_raw_50);
DEFINE_SPINLOCK(g_spinlock_raw_51);
DEFINE_SPINLOCK(g_spinlock_raw_52);
DEFINE_SPINLOCK(g_spinlock_raw_53);
DEFINE_SPINLOCK(g_spinlock_raw_54);
DEFINE_SPINLOCK(g_spinlock_raw_55);
DEFINE_SPINLOCK(g_spinlock_raw_56);
DEFINE_SPINLOCK(g_spinlock_raw_57);
DEFINE_SPINLOCK(g_spinlock_raw_58);
DEFINE_SPINLOCK(g_spinlock_raw_59);
DEFINE_SPINLOCK(g_spinlock_raw_60);
DEFINE_SPINLOCK(g_spinlock_raw_61);
DEFINE_SPINLOCK(g_spinlock_raw_62);
DEFINE_SPINLOCK(g_spinlock_raw_63);
DEFINE_SPINLOCK(g_spinlock_raw_64);
DEFINE_SPINLOCK(g_spinlock_raw_65);
DEFINE_SPINLOCK(g_spinlock_raw_66);
DEFINE_SPINLOCK(g_spinlock_raw_67);
DEFINE_SPINLOCK(g_spinlock_raw_68);
DEFINE_SPINLOCK(g_spinlock_raw_69);
DEFINE_SPINLOCK(g_spinlock_raw_70);
DEFINE_SPINLOCK(g_spinlock_raw_71);
DEFINE_SPINLOCK(g_spinlock_raw_72);
DEFINE_SPINLOCK(g_spinlock_raw_73);
DEFINE_SPINLOCK(g_spinlock_raw_74);
DEFINE_SPINLOCK(g_spinlock_raw_75);
DEFINE_SPINLOCK(g_spinlock_raw_76);
DEFINE_SPINLOCK(g_spinlock_raw_77);
DEFINE_SPINLOCK(g_spinlock_raw_78);
DEFINE_SPINLOCK(g_spinlock_raw_79);
DEFINE_SPINLOCK(g_spinlock_raw_80);
DEFINE_SPINLOCK(g_spinlock_raw_81);
DEFINE_SPINLOCK(g_spinlock_raw_82);
DEFINE_SPINLOCK(g_spinlock_raw_83);
DEFINE_SPINLOCK(g_spinlock_raw_84);
DEFINE_SPINLOCK(g_spinlock_raw_85);
DEFINE_SPINLOCK(g_spinlock_raw_86);
DEFINE_SPINLOCK(g_spinlock_raw_87);
DEFINE_SPINLOCK(g_spinlock_raw_88);
DEFINE_SPINLOCK(g_spinlock_raw_89);
DEFINE_SPINLOCK(g_spinlock_raw_90);
DEFINE_SPINLOCK(g_spinlock_raw_91);
DEFINE_SPINLOCK(g_spinlock_raw_92);
DEFINE_SPINLOCK(g_spinlock_raw_93);
DEFINE_SPINLOCK(g_spinlock_raw_94);
DEFINE_SPINLOCK(g_spinlock_raw_95);
DEFINE_SPINLOCK(g_spinlock_raw_96);
DEFINE_SPINLOCK(g_spinlock_raw_97);
DEFINE_SPINLOCK(g_spinlock_raw_98);
DEFINE_SPINLOCK(g_spinlock_raw_99);
DEFINE_SPINLOCK(g_spinlock_raw_100);
DEFINE_SPINLOCK(g_spinlock_raw_101);
DEFINE_SPINLOCK(g_spinlock_raw_102);
DEFINE_SPINLOCK(g_spinlock_raw_103);
DEFINE_SPINLOCK(g_spinlock_raw_104);
DEFINE_SPINLOCK(g_spinlock_raw_105);
DEFINE_SPINLOCK(g_spinlock_raw_106);
DEFINE_SPINLOCK(g_spinlock_raw_107);
DEFINE_SPINLOCK(g_spinlock_raw_108);
DEFINE_SPINLOCK(g_spinlock_raw_109);
DEFINE_SPINLOCK(g_spinlock_raw_110);
DEFINE_SPINLOCK(g_spinlock_raw_111);
DEFINE_SPINLOCK(g_spinlock_raw_112);
DEFINE_SPINLOCK(g_spinlock_raw_113);
DEFINE_SPINLOCK(g_spinlock_raw_114);
DEFINE_SPINLOCK(g_spinlock_raw_115);
DEFINE_SPINLOCK(g_spinlock_raw_116);
DEFINE_SPINLOCK(g_spinlock_raw_117);
DEFINE_SPINLOCK(g_spinlock_raw_118);
DEFINE_SPINLOCK(g_spinlock_raw_119);
DEFINE_SPINLOCK(g_spinlock_raw_120);
DEFINE_SPINLOCK(g_spinlock_raw_121);
DEFINE_SPINLOCK(g_spinlock_raw_122);
DEFINE_SPINLOCK(g_spinlock_raw_123);
DEFINE_SPINLOCK(g_spinlock_raw_124);
DEFINE_SPINLOCK(g_spinlock_raw_125);
DEFINE_SPINLOCK(g_spinlock_raw_126);
DEFINE_SPINLOCK(g_spinlock_raw_127);
DEFINE_SPINLOCK(g_spinlock_raw_128);
DEFINE_SPINLOCK(g_spinlock_raw_129);
DEFINE_SPINLOCK(g_spinlock_raw_130);
DEFINE_SPINLOCK(g_spinlock_raw_131);
DEFINE_SPINLOCK(g_spinlock_raw_132);
DEFINE_SPINLOCK(g_spinlock_raw_133);
DEFINE_SPINLOCK(g_spinlock_raw_134);
DEFINE_SPINLOCK(g_spinlock_raw_135);
DEFINE_SPINLOCK(g_spinlock_raw_136);
DEFINE_SPINLOCK(g_spinlock_raw_137);
DEFINE_SPINLOCK(g_spinlock_raw_138);
DEFINE_SPINLOCK(g_spinlock_raw_139);
DEFINE_SPINLOCK(g_spinlock_raw_140);
DEFINE_SPINLOCK(g_spinlock_raw_141);
DEFINE_SPINLOCK(g_spinlock_raw_142);
DEFINE_SPINLOCK(g_spinlock_raw_143);
DEFINE_SPINLOCK(g_spinlock_raw_144);
DEFINE_SPINLOCK(g_spinlock_raw_145);
DEFINE_SPINLOCK(g_spinlock_raw_146);
DEFINE_SPINLOCK(g_spinlock_raw_147);
DEFINE_SPINLOCK(g_spinlock_raw_148);
DEFINE_SPINLOCK(g_spinlock_raw_149);
DEFINE_SPINLOCK(g_spinlock_raw_150);
DEFINE_SPINLOCK(g_spinlock_raw_151);
DEFINE_SPINLOCK(g_spinlock_raw_152);
DEFINE_SPINLOCK(g_spinlock_raw_153);
DEFINE_SPINLOCK(g_spinlock_raw_154);
DEFINE_SPINLOCK(g_spinlock_raw_155);
DEFINE_SPINLOCK(g_spinlock_raw_156);
DEFINE_SPINLOCK(g_spinlock_raw_157);
DEFINE_SPINLOCK(g_spinlock_raw_158);
DEFINE_SPINLOCK(g_spinlock_raw_159);
DEFINE_SPINLOCK(g_spinlock_raw_160);
DEFINE_SPINLOCK(g_spinlock_raw_161);
DEFINE_SPINLOCK(g_spinlock_raw_162);
DEFINE_SPINLOCK(g_spinlock_raw_163);
DEFINE_SPINLOCK(g_spinlock_raw_164);
DEFINE_SPINLOCK(g_spinlock_raw_165);
DEFINE_SPINLOCK(g_spinlock_raw_166);
DEFINE_SPINLOCK(g_spinlock_raw_167);
DEFINE_SPINLOCK(g_spinlock_raw_168);
DEFINE_SPINLOCK(g_spinlock_raw_169);
DEFINE_SPINLOCK(g_spinlock_raw_170);
DEFINE_SPINLOCK(g_spinlock_raw_171);
DEFINE_SPINLOCK(g_spinlock_raw_172);
DEFINE_SPINLOCK(g_spinlock_raw_173);
DEFINE_SPINLOCK(g_spinlock_raw_174);
DEFINE_SPINLOCK(g_spinlock_raw_175);
DEFINE_SPINLOCK(g_spinlock_raw_176);
DEFINE_SPINLOCK(g_spinlock_raw_177);
DEFINE_SPINLOCK(g_spinlock_raw_178);
DEFINE_SPINLOCK(g_spinlock_raw_179);
DEFINE_SPINLOCK(g_spinlock_raw_180);
DEFINE_SPINLOCK(g_spinlock_raw_181);
DEFINE_SPINLOCK(g_spinlock_raw_182);
DEFINE_SPINLOCK(g_spinlock_raw_183);
DEFINE_SPINLOCK(g_spinlock_raw_184);
DEFINE_SPINLOCK(g_spinlock_raw_185);
DEFINE_SPINLOCK(g_spinlock_raw_186);
DEFINE_SPINLOCK(g_spinlock_raw_187);
DEFINE_SPINLOCK(g_spinlock_raw_188);
DEFINE_SPINLOCK(g_spinlock_raw_189);
DEFINE_SPINLOCK(g_spinlock_raw_190);
DEFINE_SPINLOCK(g_spinlock_raw_191);
DEFINE_SPINLOCK(g_spinlock_raw_192);
DEFINE_SPINLOCK(g_spinlock_raw_193);
DEFINE_SPINLOCK(g_spinlock_raw_194);
DEFINE_SPINLOCK(g_spinlock_raw_195);
DEFINE_SPINLOCK(g_spinlock_raw_196);
DEFINE_SPINLOCK(g_spinlock_raw_197);
DEFINE_SPINLOCK(g_spinlock_raw_198);
DEFINE_SPINLOCK(g_spinlock_raw_199);
DEFINE_SPINLOCK(g_spinlock_raw_200);
DEFINE_SPINLOCK(g_spinlock_raw_201);
DEFINE_SPINLOCK(g_spinlock_raw_202);
DEFINE_SPINLOCK(g_spinlock_raw_203);
DEFINE_SPINLOCK(g_spinlock_raw_204);
DEFINE_SPINLOCK(g_spinlock_raw_205);
DEFINE_SPINLOCK(g_spinlock_raw_206);
DEFINE_SPINLOCK(g_spinlock_raw_207);
DEFINE_SPINLOCK(g_spinlock_raw_208);
DEFINE_SPINLOCK(g_spinlock_raw_209);
DEFINE_SPINLOCK(g_spinlock_raw_210);
DEFINE_SPINLOCK(g_spinlock_raw_211);
DEFINE_SPINLOCK(g_spinlock_raw_212);
DEFINE_SPINLOCK(g_spinlock_raw_213);
DEFINE_SPINLOCK(g_spinlock_raw_214);
DEFINE_SPINLOCK(g_spinlock_raw_215);
DEFINE_SPINLOCK(g_spinlock_raw_216);
DEFINE_SPINLOCK(g_spinlock_raw_217);
DEFINE_SPINLOCK(g_spinlock_raw_218);
DEFINE_SPINLOCK(g_spinlock_raw_219);
DEFINE_SPINLOCK(g_spinlock_raw_220);
DEFINE_SPINLOCK(g_spinlock_raw_221);
DEFINE_SPINLOCK(g_spinlock_raw_222);
DEFINE_SPINLOCK(g_spinlock_raw_223);
DEFINE_SPINLOCK(g_spinlock_raw_224);
DEFINE_SPINLOCK(g_spinlock_raw_225);
DEFINE_SPINLOCK(g_spinlock_raw_226);
DEFINE_SPINLOCK(g_spinlock_raw_227);
DEFINE_SPINLOCK(g_spinlock_raw_228);
DEFINE_SPINLOCK(g_spinlock_raw_229);
DEFINE_SPINLOCK(g_spinlock_raw_230);
DEFINE_SPINLOCK(g_spinlock_raw_231);
DEFINE_SPINLOCK(g_spinlock_raw_232);
DEFINE_SPINLOCK(g_spinlock_raw_233);
DEFINE_SPINLOCK(g_spinlock_raw_234);
DEFINE_SPINLOCK(g_spinlock_raw_235);
DEFINE_SPINLOCK(g_spinlock_raw_236);
DEFINE_SPINLOCK(g_spinlock_raw_237);
DEFINE_SPINLOCK(g_spinlock_raw_238);
DEFINE_SPINLOCK(g_spinlock_raw_239);
DEFINE_SPINLOCK(g_spinlock_raw_240);
DEFINE_SPINLOCK(g_spinlock_raw_241);
DEFINE_SPINLOCK(g_spinlock_raw_242);
DEFINE_SPINLOCK(g_spinlock_raw_243);
DEFINE_SPINLOCK(g_spinlock_raw_244);
DEFINE_SPINLOCK(g_spinlock_raw_245);
DEFINE_SPINLOCK(g_spinlock_raw_246);
DEFINE_SPINLOCK(g_spinlock_raw_247);
DEFINE_SPINLOCK(g_spinlock_raw_248);
DEFINE_SPINLOCK(g_spinlock_raw_249);
DEFINE_SPINLOCK(g_spinlock_raw_250);
DEFINE_SPINLOCK(g_spinlock_raw_251);
DEFINE_SPINLOCK(g_spinlock_raw_252);
DEFINE_SPINLOCK(g_spinlock_raw_253);
DEFINE_SPINLOCK(g_spinlock_raw_254);
DEFINE_SPINLOCK(g_spinlock_raw_255);

typedef struct _aver_spinlock_t
{
    spinlock_t  *lock;
    unsigned int index;
    int used;
}_aver_spinlock_t;

#define MAX_AVER_SPINLOCK 256
static struct _aver_spinlock_t g_spinlock_ar[MAX_AVER_SPINLOCK] =
{
    [0]={ &g_spinlock_raw_0, 0, 0 },     [1]={ &g_spinlock_raw_1, 1, 0 }, 
    [2]={ &g_spinlock_raw_2, 2, 0 },     [3]={ &g_spinlock_raw_3, 3, 0 }, 
    [4]={ &g_spinlock_raw_4, 4, 0 },     [5]={ &g_spinlock_raw_5, 5, 0 }, 
    [6]={ &g_spinlock_raw_6, 6, 0 },     [7]={ &g_spinlock_raw_7, 7, 0 }, 
    [8]={ &g_spinlock_raw_8, 8, 0 },     [9]={ &g_spinlock_raw_9, 9, 0 }, 
    [10]={ &g_spinlock_raw_10, 10, 0 },     [11]={ &g_spinlock_raw_11, 11, 0 }, 
    [12]={ &g_spinlock_raw_12, 12, 0 },     [13]={ &g_spinlock_raw_13, 13, 0 }, 
    [14]={ &g_spinlock_raw_14, 14, 0 },     [15]={ &g_spinlock_raw_15, 15, 0 }, 
    [16]={ &g_spinlock_raw_16, 16, 0 },     [17]={ &g_spinlock_raw_17, 17, 0 }, 
    [18]={ &g_spinlock_raw_18, 18, 0 },     [19]={ &g_spinlock_raw_19, 19, 0 }, 
    [20]={ &g_spinlock_raw_20, 20, 0 },     [21]={ &g_spinlock_raw_21, 21, 0 }, 
    [22]={ &g_spinlock_raw_22, 22, 0 },     [23]={ &g_spinlock_raw_23, 23, 0 }, 
    [24]={ &g_spinlock_raw_24, 24, 0 },     [25]={ &g_spinlock_raw_25, 25, 0 }, 
    [26]={ &g_spinlock_raw_26, 26, 0 },     [27]={ &g_spinlock_raw_27, 27, 0 }, 
    [28]={ &g_spinlock_raw_28, 28, 0 },     [29]={ &g_spinlock_raw_29, 29, 0 }, 
    [30]={ &g_spinlock_raw_30, 30, 0 },     [31]={ &g_spinlock_raw_31, 31, 0 }, 
    [32]={ &g_spinlock_raw_32, 32, 0 },     [33]={ &g_spinlock_raw_33, 33, 0 }, 
    [34]={ &g_spinlock_raw_34, 34, 0 },     [35]={ &g_spinlock_raw_35, 35, 0 }, 
    [36]={ &g_spinlock_raw_36, 36, 0 },     [37]={ &g_spinlock_raw_37, 37, 0 }, 
    [38]={ &g_spinlock_raw_38, 38, 0 },     [39]={ &g_spinlock_raw_39, 39, 0 }, 
    [40]={ &g_spinlock_raw_40, 40, 0 },     [41]={ &g_spinlock_raw_41, 41, 0 }, 
    [42]={ &g_spinlock_raw_42, 42, 0 },     [43]={ &g_spinlock_raw_43, 43, 0 }, 
    [44]={ &g_spinlock_raw_44, 44, 0 },     [45]={ &g_spinlock_raw_45, 45, 0 }, 
    [46]={ &g_spinlock_raw_46, 46, 0 },     [47]={ &g_spinlock_raw_47, 47, 0 }, 
    [48]={ &g_spinlock_raw_48, 48, 0 },     [49]={ &g_spinlock_raw_49, 49, 0 }, 
    [50]={ &g_spinlock_raw_50, 50, 0 },     [51]={ &g_spinlock_raw_51, 51, 0 }, 
    [52]={ &g_spinlock_raw_52, 52, 0 },     [53]={ &g_spinlock_raw_53, 53, 0 }, 
    [54]={ &g_spinlock_raw_54, 54, 0 },     [55]={ &g_spinlock_raw_55, 55, 0 }, 
    [56]={ &g_spinlock_raw_56, 56, 0 },     [57]={ &g_spinlock_raw_57, 57, 0 }, 
    [58]={ &g_spinlock_raw_58, 58, 0 },     [59]={ &g_spinlock_raw_59, 59, 0 }, 
    [60]={ &g_spinlock_raw_60, 60, 0 },     [61]={ &g_spinlock_raw_61, 61, 0 }, 
    [62]={ &g_spinlock_raw_62, 62, 0 },     [63]={ &g_spinlock_raw_63, 63, 0 }, 
    [64]={ &g_spinlock_raw_64, 64, 0 },     [65]={ &g_spinlock_raw_65, 65, 0 }, 
    [66]={ &g_spinlock_raw_66, 66, 0 },     [67]={ &g_spinlock_raw_67, 67, 0 }, 
    [68]={ &g_spinlock_raw_68, 68, 0 },     [69]={ &g_spinlock_raw_69, 69, 0 }, 
    [70]={ &g_spinlock_raw_70, 70, 0 },     [71]={ &g_spinlock_raw_71, 71, 0 }, 
    [72]={ &g_spinlock_raw_72, 72, 0 },     [73]={ &g_spinlock_raw_73, 73, 0 }, 
    [74]={ &g_spinlock_raw_74, 74, 0 },     [75]={ &g_spinlock_raw_75, 75, 0 }, 
    [76]={ &g_spinlock_raw_76, 76, 0 },     [77]={ &g_spinlock_raw_77, 77, 0 }, 
    [78]={ &g_spinlock_raw_78, 78, 0 },     [79]={ &g_spinlock_raw_79, 79, 0 }, 
    [80]={ &g_spinlock_raw_80, 80, 0 },     [81]={ &g_spinlock_raw_81, 81, 0 }, 
    [82]={ &g_spinlock_raw_82, 82, 0 },     [83]={ &g_spinlock_raw_83, 83, 0 }, 
    [84]={ &g_spinlock_raw_84, 84, 0 },     [85]={ &g_spinlock_raw_85, 85, 0 }, 
    [86]={ &g_spinlock_raw_86, 86, 0 },     [87]={ &g_spinlock_raw_87, 87, 0 }, 
    [88]={ &g_spinlock_raw_88, 88, 0 },     [89]={ &g_spinlock_raw_89, 89, 0 }, 
    [90]={ &g_spinlock_raw_90, 90, 0 },     [91]={ &g_spinlock_raw_91, 91, 0 }, 
    [92]={ &g_spinlock_raw_92, 92, 0 },     [93]={ &g_spinlock_raw_93, 93, 0 }, 
    [94]={ &g_spinlock_raw_94, 94, 0 },     [95]={ &g_spinlock_raw_95, 95, 0 }, 
    [96]={ &g_spinlock_raw_96, 96, 0 },     [97]={ &g_spinlock_raw_97, 97, 0 }, 
    [98]={ &g_spinlock_raw_98, 98, 0 },     [99]={ &g_spinlock_raw_99, 99, 0 }, 
    [100]={ &g_spinlock_raw_100, 100, 0 },     [101]={ &g_spinlock_raw_101, 101, 0 }, 
    [102]={ &g_spinlock_raw_102, 102, 0 },     [103]={ &g_spinlock_raw_103, 103, 0 }, 
    [104]={ &g_spinlock_raw_104, 104, 0 },     [105]={ &g_spinlock_raw_105, 105, 0 }, 
    [106]={ &g_spinlock_raw_106, 106, 0 },     [107]={ &g_spinlock_raw_107, 107, 0 }, 
    [108]={ &g_spinlock_raw_108, 108, 0 },     [109]={ &g_spinlock_raw_109, 109, 0 }, 
    [110]={ &g_spinlock_raw_110, 110, 0 },     [111]={ &g_spinlock_raw_111, 111, 0 }, 
    [112]={ &g_spinlock_raw_112, 112, 0 },     [113]={ &g_spinlock_raw_113, 113, 0 }, 
    [114]={ &g_spinlock_raw_114, 114, 0 },     [115]={ &g_spinlock_raw_115, 115, 0 }, 
    [116]={ &g_spinlock_raw_116, 116, 0 },     [117]={ &g_spinlock_raw_117, 117, 0 }, 
    [118]={ &g_spinlock_raw_118, 118, 0 },     [119]={ &g_spinlock_raw_119, 119, 0 }, 
    [120]={ &g_spinlock_raw_120, 120, 0 },     [121]={ &g_spinlock_raw_121, 121, 0 }, 
    [122]={ &g_spinlock_raw_122, 122, 0 },     [123]={ &g_spinlock_raw_123, 123, 0 }, 
    [124]={ &g_spinlock_raw_124, 124, 0 },     [125]={ &g_spinlock_raw_125, 125, 0 }, 
    [126]={ &g_spinlock_raw_126, 126, 0 },     [127]={ &g_spinlock_raw_127, 127, 0 }, 
    [128]={ &g_spinlock_raw_128, 128, 0 },     [129]={ &g_spinlock_raw_129, 129, 0 }, 
    [130]={ &g_spinlock_raw_130, 130, 0 },     [131]={ &g_spinlock_raw_131, 131, 0 }, 
    [132]={ &g_spinlock_raw_132, 132, 0 },     [133]={ &g_spinlock_raw_133, 133, 0 }, 
    [134]={ &g_spinlock_raw_134, 134, 0 },     [135]={ &g_spinlock_raw_135, 135, 0 }, 
    [136]={ &g_spinlock_raw_136, 136, 0 },     [137]={ &g_spinlock_raw_137, 137, 0 }, 
    [138]={ &g_spinlock_raw_138, 138, 0 },     [139]={ &g_spinlock_raw_139, 139, 0 }, 
    [140]={ &g_spinlock_raw_140, 140, 0 },     [141]={ &g_spinlock_raw_141, 141, 0 }, 
    [142]={ &g_spinlock_raw_142, 142, 0 },     [143]={ &g_spinlock_raw_143, 143, 0 }, 
    [144]={ &g_spinlock_raw_144, 144, 0 },     [145]={ &g_spinlock_raw_145, 145, 0 }, 
    [146]={ &g_spinlock_raw_146, 146, 0 },     [147]={ &g_spinlock_raw_147, 147, 0 }, 
    [148]={ &g_spinlock_raw_148, 148, 0 },     [149]={ &g_spinlock_raw_149, 149, 0 }, 
    [150]={ &g_spinlock_raw_150, 150, 0 },     [151]={ &g_spinlock_raw_151, 151, 0 }, 
    [152]={ &g_spinlock_raw_152, 152, 0 },     [153]={ &g_spinlock_raw_153, 153, 0 }, 
    [154]={ &g_spinlock_raw_154, 154, 0 },     [155]={ &g_spinlock_raw_155, 155, 0 }, 
    [156]={ &g_spinlock_raw_156, 156, 0 },     [157]={ &g_spinlock_raw_157, 157, 0 }, 
    [158]={ &g_spinlock_raw_158, 158, 0 },     [159]={ &g_spinlock_raw_159, 159, 0 }, 
    [160]={ &g_spinlock_raw_160, 160, 0 },     [161]={ &g_spinlock_raw_161, 161, 0 }, 
    [162]={ &g_spinlock_raw_162, 162, 0 },     [163]={ &g_spinlock_raw_163, 163, 0 }, 
    [164]={ &g_spinlock_raw_164, 164, 0 },     [165]={ &g_spinlock_raw_165, 165, 0 }, 
    [166]={ &g_spinlock_raw_166, 166, 0 },     [167]={ &g_spinlock_raw_167, 167, 0 }, 
    [168]={ &g_spinlock_raw_168, 168, 0 },     [169]={ &g_spinlock_raw_169, 169, 0 }, 
    [170]={ &g_spinlock_raw_170, 170, 0 },     [171]={ &g_spinlock_raw_171, 171, 0 }, 
    [172]={ &g_spinlock_raw_172, 172, 0 },     [173]={ &g_spinlock_raw_173, 173, 0 }, 
    [174]={ &g_spinlock_raw_174, 174, 0 },     [175]={ &g_spinlock_raw_175, 175, 0 }, 
    [176]={ &g_spinlock_raw_176, 176, 0 },     [177]={ &g_spinlock_raw_177, 177, 0 }, 
    [178]={ &g_spinlock_raw_178, 178, 0 },     [179]={ &g_spinlock_raw_179, 179, 0 }, 
    [180]={ &g_spinlock_raw_180, 180, 0 },     [181]={ &g_spinlock_raw_181, 181, 0 }, 
    [182]={ &g_spinlock_raw_182, 182, 0 },     [183]={ &g_spinlock_raw_183, 183, 0 }, 
    [184]={ &g_spinlock_raw_184, 184, 0 },     [185]={ &g_spinlock_raw_185, 185, 0 }, 
    [186]={ &g_spinlock_raw_186, 186, 0 },     [187]={ &g_spinlock_raw_187, 187, 0 }, 
    [188]={ &g_spinlock_raw_188, 188, 0 },     [189]={ &g_spinlock_raw_189, 189, 0 }, 
    [190]={ &g_spinlock_raw_190, 190, 0 },     [191]={ &g_spinlock_raw_191, 191, 0 }, 
    [192]={ &g_spinlock_raw_192, 192, 0 },     [193]={ &g_spinlock_raw_193, 193, 0 }, 
    [194]={ &g_spinlock_raw_194, 194, 0 },     [195]={ &g_spinlock_raw_195, 195, 0 }, 
    [196]={ &g_spinlock_raw_196, 196, 0 },     [197]={ &g_spinlock_raw_197, 197, 0 }, 
    [198]={ &g_spinlock_raw_198, 198, 0 },     [199]={ &g_spinlock_raw_199, 199, 0 }, 
    [200]={ &g_spinlock_raw_200, 200, 0 },     [201]={ &g_spinlock_raw_201, 201, 0 }, 
    [202]={ &g_spinlock_raw_202, 202, 0 },     [203]={ &g_spinlock_raw_203, 203, 0 }, 
    [204]={ &g_spinlock_raw_204, 204, 0 },     [205]={ &g_spinlock_raw_205, 205, 0 }, 
    [206]={ &g_spinlock_raw_206, 206, 0 },     [207]={ &g_spinlock_raw_207, 207, 0 }, 
    [208]={ &g_spinlock_raw_208, 208, 0 },     [209]={ &g_spinlock_raw_209, 209, 0 }, 
    [210]={ &g_spinlock_raw_210, 210, 0 },     [211]={ &g_spinlock_raw_211, 211, 0 }, 
    [212]={ &g_spinlock_raw_212, 212, 0 },     [213]={ &g_spinlock_raw_213, 213, 0 }, 
    [214]={ &g_spinlock_raw_214, 214, 0 },     [215]={ &g_spinlock_raw_215, 215, 0 }, 
    [216]={ &g_spinlock_raw_216, 216, 0 },     [217]={ &g_spinlock_raw_217, 217, 0 }, 
    [218]={ &g_spinlock_raw_218, 218, 0 },     [219]={ &g_spinlock_raw_219, 219, 0 }, 
    [220]={ &g_spinlock_raw_220, 220, 0 },     [221]={ &g_spinlock_raw_221, 221, 0 }, 
    [222]={ &g_spinlock_raw_222, 222, 0 },     [223]={ &g_spinlock_raw_223, 223, 0 }, 
    [224]={ &g_spinlock_raw_224, 224, 0 },     [225]={ &g_spinlock_raw_225, 225, 0 }, 
    [226]={ &g_spinlock_raw_226, 226, 0 },     [227]={ &g_spinlock_raw_227, 227, 0 }, 
    [228]={ &g_spinlock_raw_228, 228, 0 },     [229]={ &g_spinlock_raw_229, 229, 0 }, 
    [230]={ &g_spinlock_raw_230, 230, 0 },     [231]={ &g_spinlock_raw_231, 231, 0 }, 
    [232]={ &g_spinlock_raw_232, 232, 0 },     [233]={ &g_spinlock_raw_233, 233, 0 }, 
    [234]={ &g_spinlock_raw_234, 234, 0 },     [235]={ &g_spinlock_raw_235, 235, 0 }, 
    [236]={ &g_spinlock_raw_236, 236, 0 },     [237]={ &g_spinlock_raw_237, 237, 0 }, 
    [238]={ &g_spinlock_raw_238, 238, 0 },     [239]={ &g_spinlock_raw_239, 239, 0 }, 
    [240]={ &g_spinlock_raw_240, 240, 0 },     [241]={ &g_spinlock_raw_241, 241, 0 }, 
    [242]={ &g_spinlock_raw_242, 242, 0 },     [243]={ &g_spinlock_raw_243, 243, 0 }, 
    [244]={ &g_spinlock_raw_244, 244, 0 },     [245]={ &g_spinlock_raw_245, 245, 0 }, 
    [246]={ &g_spinlock_raw_246, 246, 0 },     [247]={ &g_spinlock_raw_247, 247, 0 }, 
    [248]={ &g_spinlock_raw_248, 248, 0 },     [249]={ &g_spinlock_raw_249, 249, 0 }, 
    [250]={ &g_spinlock_raw_250, 250, 0 },     [251]={ &g_spinlock_raw_251, 251, 0 }, 
    [252]={ &g_spinlock_raw_252, 252, 0 },     [253]={ &g_spinlock_raw_253, 253, 0 }, 
    [254]={ &g_spinlock_raw_254, 254, 0 },     [255]={ &g_spinlock_raw_255, 255, 0 }, 
};

static atomic_t g_spinlock_lock;

int SysSpinLockInit(pSpinlock_t *slock)
{
    unsigned int i=0;
    *slock = NULL;

	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 

    
    while(1) {
        if( SysAtomicAddReturn(1, &g_spinlock_lock)==1 ) break;
        else {
            SysAtomicSubReturn(1, &g_spinlock_lock);
            mdelay(1);
        }
    }

    for(i=0; i<MAX_AVER_SPINLOCK; ++i) {
        if( g_spinlock_ar[i].used==0 ) {
            g_spinlock_ar[i].used = 1;

            *slock = (pSpinlock_t) (g_spinlock_ar+i);
            break;
        }
    }

    
    SysAtomicSubReturn(1, &g_spinlock_lock);

    if( *slock ) {
    
        return 0;
    }
    else {
    
        BUG(); 
    }

	#else
	*slock = (pSpinlock_t) 1;
    return 0;
	#endif
	
	return -ENOMEM;
}

void SysSpinLock(pSpinlock_t slock)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG();  return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_lock(lock->lock);
	#else
	
	#endif 
}

void SysSpinUnlock(pSpinlock_t slock)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG();  return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_unlock(lock->lock);
	#else
	
	#endif 
}

void SysSpinLockIrqsave(pSpinlock_t slock, unsigned long *flags)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG(); return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	spin_lock_irqsave(lock->lock, *flags);
	#else
	
	#endif 
}

void SysSpinUnlockIrqrestore(pSpinlock_t slock, unsigned long flags)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;
	if( lock==NULL ) { BUG(); return; }
	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)  
	spin_unlock_irqrestore(lock->lock, flags);
	#else
	
	#endif 
}

int SysSpinLockFini(pSpinlock_t slock)
{
    _aver_spinlock_t *lock = (_aver_spinlock_t *)slock;

	#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK) 
	lock->used = 0;
	return 0;
    #endif

}




