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
#include <linux/version.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/smp_lock.h>
#include <linux/fs.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18) 
	#include <media/v4l2-dev.h>
#else  
	#include "videodev.h"
#endif 

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26) 
	#include <media/v4l2-ioctl.h>
#endif

#include "osdep.h"
#include "osdep_v4l2.h"

#include "debug.h"

struct videodev_context
{
	void			*obj_container;
	struct video_device	*vfd;
	struct list_head	list;
};

struct videofh_context
{
	struct videodev_context	*cont;
	void			*private_data;
};

LIST_HEAD(context_list);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,9) 
DEFINE_SPINLOCK(context_list_lock);
#else  

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17) 
static spinlock_t context_list_lock = __SPIN_LOCK_UNLOCKED(context_list_lock);
#else
static spinlock_t context_list_lock = SPIN_LOCK_UNLOCKED;
#endif 

#endif 



static ssize_t video_read(struct file *file, char *data, size_t count, loff_t *ppos);
static ssize_t video_write(struct file *file, const char *data, size_t count, loff_t *ppos);
static int video_mmap(struct file *file, struct vm_area_struct * vma);

static unsigned int video_poll(struct file *file, struct poll_table_struct *poll);

static int video_open(struct inode *inode, struct file *file);
static int video_release(struct inode *inode, struct file *file);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29) 

static int video_ioctl(struct inode *inode, struct file *file,
		       unsigned int cmd, unsigned long arg);

static struct file_operations video_fops =
{
	.owner    = THIS_MODULE,
	.open     = video_open,
	.release  = video_release,
	.read     = video_read,
	.write    = video_write,
	.poll     = video_poll, 
	.mmap     = video_mmap,
	.ioctl    = video_ioctl,

};

#else 

static int video_new_open(struct file *file);
static int video_new_release(struct file *file);
static long video_new_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct v4l2_file_operations video_new_fops =
{
	.owner    = THIS_MODULE,
	.open     = video_new_open,
	.release  = video_new_release,
	.read     = video_read,
	.write    = video_write,
	.poll     = video_poll,
	.mmap     = video_mmap,
	.ioctl    = video_new_ioctl,
};

#endif  

int SysVideoDevRegister(const char *name, unsigned int type,
			int *minor, int *number , void *obj, void *dev, void **context)
{
	int ret = 0;
	unsigned int devtype;
	struct videodev_context *cont;

#if 0  
	switch(type) {
	case vdtVideo:
		devtype = VFL_TYPE_GRABBER; break;
	case vdtVbi:
		devtype = VFL_TYPE_VBI; break;
	case vdtRadio:
		devtype = VFL_TYPE_RADIO; break;
	case vdtTeletext:
		devtype = VFL_TYPE_VTX; break;
	default:
		return -EINVAL;
	}
#endif 

	cont = kmalloc(sizeof(*cont), GFP_KERNEL);
	if( !cont ) return -ENOMEM;

	*context = cont;
	cont->obj_container = obj;

	cont->vfd = video_device_alloc();
	if( !cont->vfd ) {
		ret = -ENOMEM;
		goto fail1;
	}
	memset(cont->vfd, 0, sizeof(cont->vfd));

	
	cont->vfd->name[sizeof(cont->vfd->name)-1] = 0;
	strncpy(cont->vfd->name, name, sizeof(cont->vfd->name)-1);
	cont->vfd->minor = *minor;


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)
	cont->vfd->fops = &video_fops;
#else
	cont->vfd->fops = &video_new_fops;
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26) 
	cont->vfd->parent = (struct device *)dev;
	cont->vfd->release = video_device_release;
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	cont->vfd->dev = (struct device *)dev;
	cont->vfd->release = video_device_release;
#endif

        switch(type) {
        case vdtVideo:
                devtype = VFL_TYPE_GRABBER; 
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)	
		cont->vfd->type = VID_TYPE_CAPTURE | VID_TYPE_TUNER;
#endif
		break;
        case vdtVbi:
                devtype = VFL_TYPE_VBI; 
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)	
		cont->vfd->type = VID_TYPE_TUNER | VID_TYPE_TELETEXT;
#endif
		break;
        case vdtRadio:
                devtype = VFL_TYPE_RADIO; 
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)	
		cont->vfd->type = VID_TYPE_TUNER;
#endif
		break;
        case vdtTeletext:
                devtype = VFL_TYPE_VTX;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) 
		cont->vfd->type = VID_TYPE_TUNER | VID_TYPE_TELETEXT;
#endif
		break;
        default:
                return -EINVAL;
        }

	ret = video_register_device(cont->vfd, devtype, *minor);
	if( ret )
		goto fail2;

	video_set_drvdata(cont->vfd, cont);
	*minor = cont->vfd->minor;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28) 
    *number = cont->vfd->minor;
#else
    *number = cont->vfd->num; 
#endif

	spin_lock(&context_list_lock);
	list_add_tail(&cont->list, &context_list);
	spin_unlock(&context_list_lock);

	goto done; 

fail2:
	video_device_release(cont->vfd);
fail1:
	kfree(cont);
done:
	return ret;
}

void SysVideoDevUnRegister(void *context)
{
	struct videodev_context *cont = context;
	video_unregister_device(cont->vfd);

	spin_lock(&context_list_lock); 
	list_del(&cont->list);
	spin_unlock(&context_list_lock); 

	cont->obj_container = NULL; 
	kfree(cont);
}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)  
static int video_new_open(struct file *file)
{
    return video_open(NULL, file);
}

static int video_new_release(struct file *file)
{
    return video_release(NULL, file);
}

#endif

static int video_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)  
    int minor = video_devdata(file)->minor;
#else
    int minor = iminor(inode);
#endif 

	struct videodev_context *h=NULL, *cont = NULL;
	struct videofh_context *fh;
	struct list_head *list;
	int ret = 0;

	
	spin_lock(&context_list_lock); 
	list_for_each(list, &context_list) {
		h = list_entry(list, struct videodev_context, list);
		if( h->vfd && h->vfd->minor==minor) {
			cont = h;
			break;	
		}
	}
	spin_unlock(&context_list_lock); 

	if( !cont ) return -ENODEV;

	fh = kmalloc(sizeof(*fh), GFP_KERNEL);
	if( !fh ) return -ENOMEM;
	memset(fh, 0, sizeof(*fh));
	fh->cont = cont;
	file->private_data = fh;

	ret = CVideoDevBaseOpen(h->obj_container, &fh->private_data, inode, file);
	return ret;
}

static int video_release(struct inode *inode, struct file *file)
{
	struct videofh_context *fh = file->private_data;
	int ret = 0;

	ret = CVideoDevBaseRelease(fh->cont->obj_container, fh->private_data, inode, file);


	fh->cont = NULL; 
	kfree(fh); 
	file->private_data = NULL; 
	return ret;
}

static ssize_t
video_read(struct file *file, char *data, size_t count, loff_t *ppos)
{
	struct videofh_context *fh = file->private_data;
	int ret = 0;

	if(!fh) return -ENOMEM; 

	ret = CVideoDevBaseRead(fh->cont->obj_container, fh->private_data, file,
				data, count);
	return ret;
}

static ssize_t
video_write(struct file *file, const char *data, size_t count, loff_t *ppos)
{
	struct videofh_context *fh = file->private_data;
	int ret = 0;

	if(!fh) return -ENOMEM; 

	ret = CVideoDevBaseWrite(fh->cont->obj_container, fh->private_data, file,
				data, count);

	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)  

static long video_new_do_ioctl(struct file *file,
                          unsigned int cmd, void *arg)
{
	struct videofh_context *fh = file->private_data;
	int ret = 0;

	if(!fh) return -ENOMEM;

	ret = CVideoDevBaseIOCtrl(fh->cont->obj_container, fh->private_data, 
				NULL, file, 
				cmd, arg);
	return ret;
}

static long video_new_ioctl(struct file *file,
		       unsigned int cmd, unsigned long arg)
{
	struct videofh_context *fh = file->private_data;
	if(!fh) return -ENOMEM;
	return video_usercopy(file, cmd, arg, video_new_do_ioctl);
}

#else 

static int video_do_ioctl(struct inode *inode, struct file *file,
                          unsigned int cmd, void *arg)
{
	struct videofh_context *fh = file->private_data;
	int ret = 0;

	if(!fh) return -ENOMEM; 

	ret = CVideoDevBaseIOCtrl(fh->cont->obj_container, fh->private_data, 
				inode, file, 
				cmd, arg);
	return ret;
}

static int video_ioctl(struct inode *inode, struct file *file,
		       unsigned int cmd, unsigned long arg)
{
	struct videofh_context *fh = file->private_data; 
	if(!fh) return -ENOMEM;  
	return video_usercopy(inode, file, cmd, arg, video_do_ioctl);
}

#endif 

static int
video_mmap(struct file *file, struct vm_area_struct * vma)
{
	struct videofh_context *fh = file->private_data;
	struct VMAreaStruct myvm;
	int ret = 0;

	if(!fh) return -ENOMEM;  

	myvm.Start = vma->vm_start;
	myvm.End = vma->vm_end;
	myvm.PageOffset = vma->vm_pgoff;
	myvm.PrivateData = vma->vm_private_data;
	myvm.KVMAreaStructPtr = (void *)vma;

	ret = CVideoDevBaseMMap(fh->cont->obj_container, fh->private_data, file,
				&myvm);

	return ret;
}


static unsigned int video_poll(struct file *file, struct poll_table_struct *poll)
{
	struct videofh_context *fh = file->private_data;
	unsigned int ret = 0;
	
	if(!fh) return -ENOMEM;  
	
	ret = CVideoDevBasePoll(fh->cont->obj_container, 
				fh->private_data, file,
				poll);
	return ret;
}

