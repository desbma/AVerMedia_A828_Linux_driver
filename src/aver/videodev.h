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
#ifndef __LINUX_VIDEODEV_H
#define __LINUX_VIDEODEV_H

#include <linux/types.h>
#include <linux/version.h>

#define HAVE_V4L2 1
#include "videodev2.h"

#ifdef __KERNEL__

#include <linux/poll.h>
#include <linux/mm.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,69)
#include <linux/devfs_fs_kernel.h>
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
#include <linux/device.h>
#endif

struct video_device
{
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
	struct device *dev;
#endif
	char name[32];
	int type;       
	int type2;      
	int hardware;
	int minor;

	
	struct file_operations *fops;
	void (*release)(struct video_device *vfd);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	
	int (*open)(struct video_device *, int mode);
	void (*close)(struct video_device *);
	long (*read)(struct video_device *, char *, unsigned long, int noblock);
	long (*write)(struct video_device *, const char *, unsigned long, int noblock);
	unsigned int (*poll)(struct video_device *, struct file *, poll_table *);
	int (*ioctl)(struct video_device *, unsigned int , void *);
	int (*mmap)(struct video_device *, const char *, unsigned long);
	int (*initialize)(struct video_device *);       
#endif

#if 1 
	
	struct module *owner;



	
	void *priv;
#endif

	
	int users;                     
	struct semaphore lock;         
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,69)
	devfs_handle_t devfs_handle;   
#else
	char devfs_name[64];           
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
	struct class_device class_dev; 
#endif
};

#define VIDEO_MAJOR	81

#define VFL_TYPE_GRABBER	0
#define VFL_TYPE_VBI		1
#define VFL_TYPE_RADIO		2
#define VFL_TYPE_VTX		3

extern int video_register_device(struct video_device *, int type, int nr);
extern void video_unregister_device(struct video_device *);
extern struct video_device* video_devdata(struct file*);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
#define to_video_device(cd) container_of(cd, struct video_device, class_dev)
static inline void
video_device_create_file(struct video_device *vfd,
			 struct class_device_attribute *attr)
{
	class_device_create_file(&vfd->class_dev, attr);
}
static inline void
video_device_remove_file(struct video_device *vfd,
			 struct class_device_attribute *attr)
{
	class_device_remove_file(&vfd->class_dev, attr);
}
#endif



struct video_device *video_device_alloc(void);
void video_device_release(struct video_device *vfd);


static inline void *video_get_drvdata(struct video_device *dev)
{
	return dev->priv;
}

static inline void video_set_drvdata(struct video_device *dev, void *data)
{
	dev->priv = data;
}

extern int video_exclusive_open(struct inode *inode, struct file *file);
extern int video_exclusive_release(struct inode *inode, struct file *file);
extern int video_usercopy(struct inode *inode, struct file *file,
			  unsigned int cmd, unsigned long arg,
			  int (*func)(struct inode *inode, struct file *file,
				      unsigned int cmd, void *arg));
#endif 

#define VID_TYPE_CAPTURE	1	
#define VID_TYPE_TUNER		2	
#define VID_TYPE_TELETEXT	4	
#define VID_TYPE_OVERLAY	8	
#define VID_TYPE_CHROMAKEY	16	
#define VID_TYPE_CLIPPING	32	
#define VID_TYPE_FRAMERAM	64	
#define VID_TYPE_SCALES		128	
#define VID_TYPE_MONOCHROME	256	
#define VID_TYPE_SUBCAPTURE	512	
#define VID_TYPE_MPEG_DECODER	1024	
#define VID_TYPE_MPEG_ENCODER	2048	
#define VID_TYPE_MJPEG_DECODER	4096	
#define VID_TYPE_MJPEG_ENCODER	8192	

struct video_capability
{
	char name[32];
	int type;
	int channels;	
	int audios;	
	int maxwidth;	
	int maxheight;	
	int minwidth;	
	int minheight;	
};


struct video_channel
{
	int channel;
	char name[32];
	int tuners;
	__u32  flags;
#define VIDEO_VC_TUNER		1	
#define VIDEO_VC_AUDIO		2	
	__u16  type;
#define VIDEO_TYPE_TV		1
#define VIDEO_TYPE_CAMERA	2	
	__u16 norm;			
};

struct video_tuner
{
	int tuner;
	char name[32];
	unsigned long rangelow, rangehigh;	
	__u32 flags;
#define VIDEO_TUNER_PAL		1
#define VIDEO_TUNER_NTSC	2
#define VIDEO_TUNER_SECAM	4
#define VIDEO_TUNER_LOW		8	
#define VIDEO_TUNER_NORM	16	
#define VIDEO_TUNER_STEREO_ON	128	
#define VIDEO_TUNER_RDS_ON      256     
#define VIDEO_TUNER_MBS_ON      512     
	__u16 mode;			
#define VIDEO_MODE_PAL		0
#define VIDEO_MODE_NTSC		1
#define VIDEO_MODE_SECAM	2
#define VIDEO_MODE_AUTO		3
	__u16 signal;			
};

struct video_picture
{
	__u16	brightness;
	__u16	hue;
	__u16	colour;
	__u16	contrast;
	__u16	whiteness;	
	__u16	depth;		
	__u16   palette;	
#define VIDEO_PALETTE_GREY	1	
#define VIDEO_PALETTE_HI240	2	
#define VIDEO_PALETTE_RGB565	3	
#define VIDEO_PALETTE_RGB24	4	
#define VIDEO_PALETTE_RGB32	5		
#define VIDEO_PALETTE_RGB555	6	
#define VIDEO_PALETTE_YUV422	7	
#define VIDEO_PALETTE_YUYV	8
#define VIDEO_PALETTE_UYVY	9	
#define VIDEO_PALETTE_YUV420	10
#define VIDEO_PALETTE_YUV411	11	
#define VIDEO_PALETTE_RAW	12	
#define VIDEO_PALETTE_YUV422P	13	
#define VIDEO_PALETTE_YUV411P	14	
#define VIDEO_PALETTE_YUV420P	15	
#define VIDEO_PALETTE_YUV410P	16	
#define VIDEO_PALETTE_PLANAR	13	
#define VIDEO_PALETTE_COMPONENT 7	
};

struct video_audio
{
	int	audio;		
	__u16	volume;		
	__u16	bass, treble;
	__u32	flags;
#define VIDEO_AUDIO_MUTE	1
#define VIDEO_AUDIO_MUTABLE	2
#define VIDEO_AUDIO_VOLUME	4
#define VIDEO_AUDIO_BASS	8
#define VIDEO_AUDIO_TREBLE	16	
#define VIDEO_AUDIO_BALANCE	32
	char    name[16];
#define VIDEO_SOUND_MONO	1
#define VIDEO_SOUND_STEREO	2
#define VIDEO_SOUND_LANG1	4
#define VIDEO_SOUND_LANG2	8
        __u16   mode;
        __u16	balance;	
        __u16	step;		
};

struct video_clip
{
	__s32	x,y;
	__s32	width, height;
	struct	video_clip *next;	
};

struct video_window
{
	__u32	x,y;			
	__u32	width,height;		
	__u32	chromakey;
	__u32	flags;
	struct	video_clip *clips;	
	int	clipcount;
#define VIDEO_WINDOW_INTERLACE	1
#define VIDEO_WINDOW_CHROMAKEY	16	
#define VIDEO_CLIP_BITMAP	-1

#define VIDEO_CLIPMAP_SIZE	(128 * 625)
};

struct video_capture
{
	__u32 	x,y;			
	__u32	width, height;		
	__u16	decimation;		
	__u16	flags;			
#define VIDEO_CAPTURE_ODD		0	
#define VIDEO_CAPTURE_EVEN		1
};

struct video_buffer
{
	void	*base;
	int	height,width;
	int	depth;
	int	bytesperline;
};

struct video_mmap
{
	unsigned	int frame;		
	int		height,width;
	unsigned	int format;		
};

struct video_key
{
	__u8	key[8];
	__u32	flags;
};


#define VIDEO_MAX_FRAME		128 

struct video_mbuf
{
	int	size;		
	int	frames;		
	int	offsets[VIDEO_MAX_FRAME];
};
	

#define 	VIDEO_NO_UNIT	(-1)

	
struct video_unit
{
	int 	video;		
	int	vbi;		
	int	radio;		
	int	audio;		
	int	teletext;	
};

struct vbi_format {
	__u32	sampling_rate;	
	__u32	samples_per_line;
	__u32	sample_format;	
	__s32	start[2];	
	__u32	count[2];	
	__u32	flags;
#define	VBI_UNSYNC	1	
#define	VBI_INTERLACED	2	
};



struct video_info
{
	__u32	frame_count;	
	__u32	h_size;		
	__u32	v_size;		
	__u32	smpte_timecode;	
	__u32	picture_type;	
	__u32	temporal_reference;	
	__u8	user_data[256];	
	
};


struct video_play_mode
{
	int	mode;
	int	p1;
	int	p2;
};


struct video_code
{
	char	loadwhat[16];	
	int	datasize;
	__u8	*data;
};

#define VIDIOCGCAP		_IOR('v',1,struct video_capability)	
#define VIDIOCGCHAN		_IOWR('v',2,struct video_channel)	
#define VIDIOCSCHAN		_IOW('v',3,struct video_channel)	
#define VIDIOCGTUNER		_IOWR('v',4,struct video_tuner)		
#define VIDIOCSTUNER		_IOW('v',5,struct video_tuner)		
#define VIDIOCGPICT		_IOR('v',6,struct video_picture)	
#define VIDIOCSPICT		_IOW('v',7,struct video_picture)	
#define VIDIOCCAPTURE		_IOW('v',8,int)				
#define VIDIOCGWIN		_IOR('v',9, struct video_window)	
#define VIDIOCSWIN		_IOW('v',10, struct video_window)	
#define VIDIOCGFBUF		_IOR('v',11, struct video_buffer)	
#define VIDIOCSFBUF		_IOW('v',12, struct video_buffer)	
#define VIDIOCKEY		_IOR('v',13, struct video_key)		
#define VIDIOCGFREQ		_IOR('v',14, unsigned long)		
#define VIDIOCSFREQ		_IOW('v',15, unsigned long)		
#define VIDIOCGAUDIO		_IOR('v',16, struct video_audio)	
#define VIDIOCSAUDIO		_IOW('v',17, struct video_audio)	
#define VIDIOCSYNC		_IOW('v',18, int)			
#define VIDIOCMCAPTURE		_IOW('v',19, struct video_mmap)		
#define VIDIOCGMBUF		_IOR('v',20, struct video_mbuf)		
#define VIDIOCGUNIT		_IOR('v',21, struct video_unit)		
#define VIDIOCGCAPTURE		_IOR('v',22, struct video_capture)	
#define VIDIOCSCAPTURE		_IOW('v',23, struct video_capture)	
#define VIDIOCSPLAYMODE		_IOW('v',24, struct video_play_mode)	
#define VIDIOCSWRITEMODE	_IOW('v',25, int)			
#define VIDIOCGPLAYINFO		_IOR('v',26, struct video_info)		
#define VIDIOCSMICROCODE	_IOW('v',27, struct video_code)		
#define	VIDIOCGVBIFMT		_IOR('v',28, struct vbi_format)		
#define	VIDIOCSVBIFMT		_IOW('v',29, struct vbi_format)		


#define BASE_VIDIOCPRIVATE	192		


#define VID_WRITE_MPEG_AUD		0
#define VID_WRITE_MPEG_VID		1
#define VID_WRITE_OSD			2
#define VID_WRITE_TTX			3
#define VID_WRITE_CC			4
#define VID_WRITE_MJPEG			5


#define VID_PLAY_VID_OUT_MODE		0
	
#define VID_PLAY_GENLOCK		1
	
	 
#define VID_PLAY_NORMAL			2
#define VID_PLAY_PAUSE			3
#define VID_PLAY_SINGLE_FRAME		4
#define VID_PLAY_FAST_FORWARD		5
#define VID_PLAY_SLOW_MOTION		6
#define VID_PLAY_IMMEDIATE_NORMAL	7
#define VID_PLAY_SWITCH_CHANNELS	8
#define VID_PLAY_FREEZE_FRAME		9
#define VID_PLAY_STILL_MODE		10
#define VID_PLAY_MASTER_MODE		11
	
#define		VID_PLAY_MASTER_NONE	1
#define		VID_PLAY_MASTER_VIDEO	2
#define		VID_PLAY_MASTER_AUDIO	3
#define VID_PLAY_ACTIVE_SCANLINES	12
	
#define VID_PLAY_RESET			13
#define VID_PLAY_END_MARK		14



#define VID_HARDWARE_BT848	1
#define VID_HARDWARE_QCAM_BW	2
#define VID_HARDWARE_PMS	3
#define VID_HARDWARE_QCAM_C	4
#define VID_HARDWARE_PSEUDO	5
#define VID_HARDWARE_SAA5249	6
#define VID_HARDWARE_AZTECH	7
#define VID_HARDWARE_SF16MI	8
#define VID_HARDWARE_RTRACK	9
#define VID_HARDWARE_ZOLTRIX	10
#define VID_HARDWARE_SAA7146    11
#define VID_HARDWARE_VIDEUM	12	
#define VID_HARDWARE_RTRACK2	13
#define VID_HARDWARE_PERMEDIA2	14	
#define VID_HARDWARE_RIVA128	15	
#define VID_HARDWARE_PLANB	16	
#define VID_HARDWARE_BROADWAY	17	
#define VID_HARDWARE_GEMTEK	18
#define VID_HARDWARE_TYPHOON	19
#define VID_HARDWARE_VINO	20	
#define VID_HARDWARE_CADET	21	
#define VID_HARDWARE_TRUST	22	
#define VID_HARDWARE_TERRATEC	23	
#define VID_HARDWARE_CPIA	24
#define VID_HARDWARE_ZR36120	25	
#define VID_HARDWARE_ZR36067	26	
#define VID_HARDWARE_OV511	27	
#define VID_HARDWARE_ZR356700	28	
#define VID_HARDWARE_W9966	29
#define VID_HARDWARE_SE401	30	
#define VID_HARDWARE_PWC	31	
#define VID_HARDWARE_MEYE	32	
#define VID_HARDWARE_CPIA2	33
#define VID_HARDWARE_VICAM      34
#define VID_HARDWARE_SF16FMR2	35
#define VID_HARDWARE_W9968CF    36
#define VID_HARDWARE_SAA7114H   37
#endif 






