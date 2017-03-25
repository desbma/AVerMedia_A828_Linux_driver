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
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include "osdep_vbuf.h"


#include "debug.h"

static void
vbuf_vm_open(struct vm_area_struct *vma)
{
	DBG_fCSTREAM("\n");
	CStrmBase_VM_Open(vma->vm_private_data);
}

static void
vbuf_vm_close(struct vm_area_struct *vma)
{
	DBG_fCSTREAM("\n");
	CStrmBase_VM_Close(vma->vm_private_data);
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,25) 
static int
vbuf_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct page *pg;



	pg = (struct page *)CStrmBase_VM_NoPage(vma->vm_private_data, (unsigned long)vmf->virtual_address);

	if (!pg)
		return VM_FAULT_OOM;
	clear_user_page(page_address(pg), (unsigned long)vmf->virtual_address,
			pg);
	vmf->page = pg;
	return 0;
}
#else
static struct page*
vbuf_vm_nopage(struct vm_area_struct *vma, unsigned long vaddr, int *type)
{
	struct page *pg; 
	

	if( vaddr > vma->vm_end ) return NOPAGE_SIGBUS;
	
	pg = (struct page *)CStrmBase_VM_NoPage(vma->vm_private_data, vaddr);
	if( !pg ) return NOPAGE_OOM;
	if( type ) *type = VM_FAULT_MINOR;

	return pg;
}
#endif 


static struct vm_operations_struct vbuf_vm_ops =
{
        .open     = vbuf_vm_open,
        .close    = vbuf_vm_close,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,25) 
	.fault    = vbuf_vm_fault,
#else
        .nopage   = vbuf_vm_nopage,
#endif 
};


int SysCheckVideoVma(void *v)
{
	struct vm_area_struct *vma = v;
	int ret = -EINVAL;
	
	if (!(vma->vm_flags & VM_WRITE)) {
		return ret;
	}
	if (!(vma->vm_flags & VM_SHARED)) {
		return ret;
	}

	return 0;
}

void SysSetUpVideoVma(void *v, void *mmap_context)
{
	struct vm_area_struct *vma = v;

	vma->vm_ops   = &vbuf_vm_ops;
	vma->vm_flags |= VM_DONTEXPAND | VM_RESERVED;
	vma->vm_flags &= ~VM_IO;
	vma->vm_private_data = mmap_context;
}

