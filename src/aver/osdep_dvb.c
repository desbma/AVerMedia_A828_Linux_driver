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
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/smp_lock.h>

#include <linux/dvb/video.h>
#include <linux/dvb/audio.h>
#include <linux/dvb/dmx.h>
#include <linux/dvb/ca.h>
#include <linux/dvb/osd.h>
#include <linux/dvb/net.h>

#include "dvbdev.h"
#include "demux.h"
#include "dvb_demux.h"
#include "dmxdev.h"
#include "dvb_filter.h"
#include "dvb_net.h"
#include "dvb_ringbuffer.h"
#include "dvb_frontend.h"
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
#include "dvb_i2c.h"
#endif

#include "osdep.h"
#include "osdep_dvb.h"


#include "debug.h"

#if (defined(DVB_FC4_HACK) && DVB_FC4_HACK!=0) || (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,12) && (!defined(DVB_2612_HACK) || !DVB_2612_HACK) )
	#define DVB_REG_NEWSTYLE 1
#else
	#define DVB_REG_NEWSTYLE 0
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
#define DECLARE_DEV 	DVBDev *p = fe->demodulator_priv
#else
#define DECLARE_DEV 	DVBDev *p = fe->data
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,25) 
DVB_DEFINE_MOD_OPT_ADAPTER_NR(dvb_adapter_nr);
#endif 

typedef struct DVBDev
{
	unsigned char		name[64];

	struct dvb_adapter	*adapter;
	struct dvb_frontend	fe;
	__INSERT_GAURD_BUFFER__(__LINE__);			
	struct dmxdev		dmxdev_sw;
	__INSERT_GAURD_BUFFER__(__LINE__);			
	__INSERT_GAURD_BUFFER__(__LINE__);			
	struct dvb_demux	demux_sw;
	__INSERT_GAURD_BUFFER__(__LINE__);			
	__INSERT_GAURD_BUFFER__(__LINE__);			

	struct dmx_frontend	hw_frontend;
	__INSERT_GAURD_BUFFER__(__LINE__);			
	__INSERT_GAURD_BUFFER__(__LINE__);			
	struct dmx_frontend	mem_frontend;
	__INSERT_GAURD_BUFFER__(__LINE__);			
	__INSERT_GAURD_BUFFER__(__LINE__);			

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10) && LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18) 
	struct dvb_frontend_ops	dvbfe_ops;
	__INSERT_GAURD_BUFFER__(__LINE__);			
	#else
	struct dvb_i2c_bus	*i2c_bus;
	struct dvb_frontend_info *dvbfe_info;
	#endif

    #if 0  
	struct semaphore	feedlock;
    #else
    pSemaphore  feedlock;
    #endif
	int			feedcount;

	void			*context;
} DVBDev;


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
static int master_xfer (struct dvb_i2c_bus *i2c, const struct i2c_msg msgs[],
		int num);
static int dvb_fe_ioctl(struct dvb_frontend *fe, unsigned int cmd, void *arg);
#endif
static int demux_sw_start_feed(struct dvb_demux_feed *feed);
static int demux_sw_stop_feed(struct dvb_demux_feed *feed);
static int dvbfe_set_parameters(struct dvb_frontend *fe,
			struct dvb_frontend_parameters *param);
static int dvbfe_get_parameters(struct dvb_frontend *fe,
			struct dvb_frontend_parameters *param);
static int dvbfe_read_status(struct dvb_frontend* fe, fe_status_t* status);
static int dvbfe_read_ber(struct dvb_frontend* fe, u32* ber);
static int dvbfe_read_signal_strength(struct dvb_frontend* fe, u16* strength);
static int dvbfe_read_snr(struct dvb_frontend* fe, u16* snr);
static int dvbfe_read_ucblocks(struct dvb_frontend* fe, u32* ucblocks);
static int dvbfe_get_tune_settings(struct dvb_frontend *fe,
			struct dvb_frontend_tune_settings *settings);
static int dvbfe_init(struct dvb_frontend* fe);
static int dvbfe_sleep(struct dvb_frontend* fe);


#if	defined(USE_SWIC2) && (USE_SWIC2!=0)
static int dvbfe_diseqc_reset_overload(struct dvb_frontend* fe);
static int dvbfe_diseqc_send_master_cmd(struct dvb_frontend* fe, struct dvb_diseqc_master_cmd* arg);
static int dvbfe_diseqc_recv_slave_reply(struct dvb_frontend* fe, struct dvb_diseqc_slave_reply* arg);
static int dvbfe_diseqc_send_burst(struct dvb_frontend* fe, fe_sec_mini_cmd_t arg);
static int dvbfe_set_tone(struct dvb_frontend* fe, fe_sec_tone_mode_t arg);
static int dvbfe_set_voltage(struct dvb_frontend* fe, fe_sec_voltage_t arg);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,16)
static int dvbfe_enable_high_lnb_voltage(struct dvb_frontend *fe, int arg);
#else
static int dvbfe_enable_high_lnb_voltage(struct dvb_frontend* fe, long arg);
#endif
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
static void dvbfe_release(struct dvb_frontend* fe);
#endif


void *Get_Adapter_or_I2CBus(dvb_t dev)
{
        DVBDev *p = (DVBDev *)dev;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
	return p->i2c_bus;
#else
	return p->adapter;
#endif	
}


dvb_t SysDVBInit(void *c)
{
	DVBDev *p = NULL;

#if 0 
	p = kmalloc(sizeof(*p), GFP_KERNEL);
#else
	p = SysKMalloc(sizeof(*p), eKMALLOC_KERNEL);
#endif 
	if(p) {
#if 0 
		memset(p, 0, sizeof(*p));
#else
		SysMemSet(p, 0, sizeof(*p));
#endif
		p->context = c;
#if 0 
		init_MUTEX(&p->feedlock);
#else
        if( SysSemInit(&p->feedlock, st_Mutex) ) {
            SysKFree(p);
            return NULL;
        }
#endif 
	}
    else {
        return NULL;
    }

	return (dvb_t)(p);
}

void SysDVBFini(dvb_t dev)
{
	DVBDev *p = (DVBDev *)dev;
    SysSemFini(p->feedlock); 
#if 0 
	if( p )	kfree(p);
#else
	if( p ) SysKFree(p);
#endif 
}


int SysDVBRegisterFrontend(dvb_t dev, struct dvb_frontend_info *info,
			void *dvb_adapter_or_i2c_bus)
{
	int ret = 0;
	DVBDev *p = (DVBDev *)dev;

	DBG_fCDVB("setting up frontend ops\n");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)

    memset(&(p->fe.ops), 0, sizeof(p->fe.ops)); 


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)

	p->fe.ops.info = *info;
	p->fe.ops.release = dvbfe_release;
	p->fe.ops.set_frontend = dvbfe_set_parameters;
	p->fe.ops.get_frontend = dvbfe_get_parameters;
	p->fe.ops.read_status = dvbfe_read_status;
	p->fe.ops.read_ber = dvbfe_read_ber;
	p->fe.ops.read_signal_strength =
					dvbfe_read_signal_strength;
	p->fe.ops.read_snr = dvbfe_read_snr;
	p->fe.ops.get_tune_settings = dvbfe_get_tune_settings;
	p->fe.ops.read_ucblocks = dvbfe_read_ucblocks;
	p->fe.ops.init = dvbfe_init;
	p->fe.ops.sleep = dvbfe_sleep;

        #if     defined(USE_SWIC2) && (USE_SWIC2!=0)  
        if( FE_QPSK == info->type){
                p->fe.ops.diseqc_reset_overload = dvbfe_diseqc_reset_overload;
                p->fe.ops.diseqc_send_master_cmd = dvbfe_diseqc_send_master_cmd;
                p->fe.ops.diseqc_recv_slave_reply = dvbfe_diseqc_recv_slave_reply;
                p->fe.ops.diseqc_send_burst = dvbfe_diseqc_send_burst;
                p->fe.ops.set_tone = dvbfe_set_tone;
                p->fe.ops.set_voltage = dvbfe_set_voltage;
                p->fe.ops.enable_high_lnb_voltage = dvbfe_enable_high_lnb_voltage;
        }
        #endif  


    
    strncpy(p->fe.ops.tuner_ops.info.name, info->name, sizeof(p->fe.ops.tuner_ops.info.name));
    p->fe.ops.tuner_ops.info.frequency_min = info->frequency_min;
    p->fe.ops.tuner_ops.info.frequency_max = info->frequency_max;
    p->fe.ops.tuner_ops.info.frequency_step = info->frequency_stepsize;
    p->fe.ops.tuner_ops.info.bandwidth_min = 6000000;
    p->fe.ops.tuner_ops.info.bandwidth_max = 8000000;
    p->fe.ops.tuner_ops.info.bandwidth_step = 1000000;
    

#else
	p->fe.ops = &p->dvbfe_ops;
	p->fe.ops->info = *info;
	p->fe.ops->release = dvbfe_release;
	p->fe.ops->set_frontend = dvbfe_set_parameters;
	p->fe.ops->get_frontend = dvbfe_get_parameters;
	p->fe.ops->read_status = dvbfe_read_status;
	p->fe.ops->read_ber = dvbfe_read_ber;
	p->fe.ops->read_signal_strength =
					dvbfe_read_signal_strength;
	p->fe.ops->read_snr = dvbfe_read_snr;
	p->fe.ops->get_tune_settings = dvbfe_get_tune_settings;
	p->fe.ops->read_ucblocks = dvbfe_read_ucblocks;
	p->fe.ops->init = dvbfe_init;
	p->fe.ops->sleep = dvbfe_sleep;

	#if     defined(USE_SWIC2) && (USE_SWIC2!=0)  
        if( FE_QPSK == info->type){
                p->fe.ops->diseqc_reset_overload = dvbfe_diseqc_reset_overload;
                p->fe.ops->diseqc_send_master_cmd = dvbfe_diseqc_send_master_cmd;
                p->fe.ops->diseqc_recv_slave_reply = dvbfe_diseqc_recv_slave_reply;
                p->fe.ops->diseqc_send_burst = dvbfe_diseqc_send_burst;
                p->fe.ops->set_tone = dvbfe_set_tone;
                p->fe.ops->set_voltage = dvbfe_set_voltage;
                p->fe.ops->enable_high_lnb_voltage = dvbfe_enable_high_lnb_voltage;
        }
	#endif  
#endif


#if 0  

#if	defined(USE_SWIC2) && (USE_SWIC2!=0)
	if( FE_QPSK == info->type){
		p->fe.ops->diseqc_reset_overload = dvbfe_diseqc_reset_overload;
		p->fe.ops->diseqc_send_master_cmd = dvbfe_diseqc_send_master_cmd;
		p->fe.ops->diseqc_recv_slave_reply = dvbfe_diseqc_recv_slave_reply;
		p->fe.ops->diseqc_send_burst = dvbfe_diseqc_send_burst;
		p->fe.ops->set_tone = dvbfe_set_tone;
		p->fe.ops->set_voltage = dvbfe_set_voltage;
		p->fe.ops->enable_high_lnb_voltage = dvbfe_enable_high_lnb_voltage;
	}
#endif

#endif 
	p->fe.demodulator_priv = p; 

	if(dvb_adapter_or_i2c_bus)
		p->adapter = dvb_adapter_or_i2c_bus;
	ret = dvb_register_frontend(p->adapter, &p->fe);
#else
	p->dvbfe_info = info;
	if(dvb_adapter_or_i2c_bus)
		p->i2c_bus = dvb_adapter_or_i2c_bus;
	ret = dvb_register_frontend(dvb_fe_ioctl, p->i2c_bus, (void *)p,
					info);
#endif
	if(ret) {
		 printk(KERN_WARNING "%s: failed to register frontend\n",
		 		p->name);
	}

	return ret;
}


#if 0
int SysDVBRegisterAdapter(dvb_t dev, const char *name, int max_filters)
#else
int SysDVBRegisterAdapter(dvb_t dev, const char *name, int max_filters, void *device)
#endif

{
	int ret = 0;
	DVBDev *p = (DVBDev *)dev;

	DBG_fCDVB("\n");
	if( !p ) return -ENODEV;

	
	#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)
	if( !device ) return -EINVAL;
	#endif
	

#if 0 
	strncpy(p->name, name, sizeof(p->name));
#else
	SysStrNCpy(p->name, name, sizeof(p->name));
#endif 

	
	#if DVB_REG_NEWSTYLE
	#if 0 
	p->adapter = kmalloc(sizeof(struct dvb_adapter), GFP_KERNEL);
	#else
	p->adapter = SysKMalloc(sizeof(struct dvb_adapter)+512, eKMALLOC_KERNEL); 
	#endif
	DBG_fCDVB("p->adapter=%p\n", p->adapter);
	if(NULL == p->adapter) {
		DBG_fCDVB("cannot kmalloc DVB adapter\n");
		ret = -ENOMEM;
		goto fail0;
	}

	#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,25) 
	p->adapter->device = device;
	ret = dvb_register_adapter(p->adapter, p->name, THIS_MODULE, (struct device *)device, dvb_adapter_nr);
	
	#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)
	p->adapter->device = device;
	ret = dvb_register_adapter(p->adapter, p->name, THIS_MODULE, (struct device *)device);
	#else
	ret = dvb_register_adapter(p->adapter, p->name, THIS_MODULE);
	#endif
	
	
	#else 
	ret = dvb_register_adapter(&p->adapter, p->name, THIS_MODULE);
	#endif 

	if(ret<0) {
#if 0 
		printk(KERN_WARNING "%s: failed to register adapter(%d)",
			p->name, ret);
#else
		SysPrintk(KERN_WARNING "%s: failed to register adapter(%d)",
                        p->name, ret);
#endif 
		goto fail1;
	}
#if 0 
	printk(KERN_INFO "%s registered DVB adapter %d\n", 
		name, p->adapter->num);
#else
	SysPrintk(KERN_INFO "%s registered DVB adapter %d\n", 
	                name, p->adapter->num);
#endif 

	
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
	p->i2c_bus = dvb_register_i2c_bus(master_xfer, dev,
						  p->adapter, 111);
	if(!p->i2c_bus) {
		dvb_unregister_adapter(p->adapter);
#if 0 
		printk(KERN_WARNING "%s: failed to register dvb-i2c\n",
			p->name);
#else
		SysPrintk(KERN_WARNING "%s: failed to register dvb-i2c\n",
			p->name);
#endif 
		return -ENOMEM;
	}
	DBG_fCDVB("registered dvb-i2c %p\n", p->i2c_bus);
	#endif

	
	DBG_fCDVB("init software demux, demux_sw=%p\n", &p->demux_sw);
#if 0 
	memset(&p->demux_sw, 0, sizeof(struct dvb_demux)); 
#else
	SysMemSet(&p->demux_sw, 0, sizeof(struct dvb_demux)); 
#endif 
	p->demux_sw.priv = (void *)p;
	p->demux_sw.filternum = max_filters;
	p->demux_sw.feednum = max_filters;
	p->demux_sw.start_feed = demux_sw_start_feed;
	p->demux_sw.stop_feed = demux_sw_stop_feed;
	p->demux_sw.write_to_decoder = NULL;
	p->demux_sw.dmx.capabilities = 
			(DMX_TS_FILTERING |
			 DMX_SECTION_FILTERING |
			 DMX_MEMORY_BASED_FILTERING
			);

	ret = dvb_dmx_init(&p->demux_sw);
	if(ret) {
		printk(KERN_WARNING "%s: failed to init demux(%d)\n",
			p->name, ret);
		goto fail2;
	}

	
	DBG_fCDVB("init demux devices, dmxdev_sw=%p\n", &p->dmxdev_sw);
	p->dmxdev_sw.filternum = max_filters;
	p->dmxdev_sw.demux = &p->demux_sw.dmx;
	p->dmxdev_sw.capabilities = 0;

	ret = dvb_dmxdev_init(&p->dmxdev_sw, p->adapter);
	if(ret) {
#if 0 
		printk(KERN_WARNING "%s: failed to init dmxdev\n",
			p->name);
#else
		SysPrintk(KERN_WARNING "%s: failed to init dmxdev\n",
			p->name);
#endif 
		goto fail3;
	}

	
	DBG_fCDVB("add h/w frontend\n");
	p->hw_frontend.source = DMX_FRONTEND_0;
	ret = p->demux_sw.dmx.add_frontend(&p->demux_sw.dmx,
						   &p->hw_frontend);
	if(ret < 0) {
#if 0 
		printk(KERN_WARNING "%s: cannot add hw frontend\n",
			p->name);
#else
		SysPrintk(KERN_WARNING "%s: cannot add hw frontend, ret=%d\n",
			p->name, ret);
#endif 
		goto fail4;
	}

	
	DBG_fCDVB("add memory frontend\n");
	p->mem_frontend.source = DMX_MEMORY_FE;
	ret = p->demux_sw.dmx.add_frontend(&p->demux_sw.dmx,
					 &p->mem_frontend);
	if(ret < 0) {
#if 0 
		printk(KERN_WARNING "%s: cannot add mem frontend\n",
			p->name);
#else
		SysPrintk(KERN_WARNING "%s: cannot add mem frontend\n",
			p->name);
#endif 
		goto fail5;
	}

	ret = p->demux_sw.dmx.connect_frontend(
					&p->demux_sw.dmx,
					&p->mem_frontend);
	if(ret < 0) {
#if 0 
		printk(KERN_WARNING "%s: cannot connect frontend\n",
			p->name);
#else
		SysPrintk(KERN_WARNING "%s: cannot connect frontend, ret=%d\n",
			p->name, ret);
#endif 
		goto fail6;
	}

	DBG_fCDVB("done\n");
	goto done;

fail6:
	p->demux_sw.dmx.remove_frontend(&p->demux_sw.dmx,
						&p->mem_frontend);
fail5:
	p->demux_sw.dmx.remove_frontend(&p->demux_sw.dmx,
						&p->hw_frontend);
fail4:
	dvb_dmxdev_release(&p->dmxdev_sw);
fail3:
	dvb_dmx_release(&p->demux_sw);
fail2:
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
	dvb_unregister_i2c_bus(master_xfer, p->i2c_bus->adapter,
				p->i2c_bus->id);
	#endif
	dvb_unregister_adapter(p->adapter);
fail1:
	#if DVB_REG_NEWSTYLE
	#if 0 
	kfree(p->adapter);
	#else
	SysKFree(p->adapter);
	#endif 
fail0:
	#endif
done:
	return ret;

}

int SysDVBUnRegisterFrontend(dvb_t dev)
{
	DVBDev *p = (DVBDev *)dev;
	DBG_fCDVB("\n");

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
	dvb_unregister_frontend(&p->fe);
	#else 
	dvb_unregister_frontend(dvb_fe_ioctl, p->i2c_bus);
	#endif

	return 0;
}

int SysDVBUnRegisterAdapter(dvb_t dev)
{
	DVBDev *p = (DVBDev *)dev;
	
	DBG_fCDVB("\n");
	
	p->demux_sw.dmx.remove_frontend(
					&p->demux_sw.dmx,
					&p->mem_frontend);
	p->demux_sw.dmx.remove_frontend(
					&p->demux_sw.dmx,
					&p->hw_frontend);
	dvb_dmxdev_release(&p->dmxdev_sw);
	dvb_dmx_release(&p->demux_sw);
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
	dvb_unregister_i2c_bus(master_xfer, p->i2c_bus->adapter,
				p->i2c_bus->id);
	#endif
	dvb_unregister_adapter(p->adapter);
	#if DVB_REG_NEWSTYLE
	#if 0 
	kfree(p->adapter);
	#else
	SysKFree(p->adapter);
	#endif 
	#endif

	DBG_fCDVB("done\n");
	return 0;
}

void SysDVBFeedTSPackets(dvb_t dev, unsigned char *buf, unsigned int len)
{
	DVBDev *p = (DVBDev *)dev;

	dvb_dmx_swfilter_packets(&p->demux_sw, buf, len/188);
}







#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
static int master_xfer (struct dvb_i2c_bus *i2c, const struct i2c_msg msgs[],
			int num)
{
	return 0; 
}
#endif

static int demux_sw_start_feed(struct dvb_demux_feed *feed)
{
	struct dvb_demux *demux = feed->demux;
	DVBDev *dev = demux->priv;
	int status = 0;
	int ret = 0;

	DBG_fCDVB("pid %d start\n", feed->pid);
#if 0 
	down(&dev->feedlock);
#else
    #if 0 
	SysSemDown(&dev->feedlock);
    #else
	SysSemDown(dev->feedlock);
    #endif
#endif 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
	feed->pusi_seen = 0;
#endif
	status = ++ dev->feedcount;
#if 0 
	up(&dev->feedlock);
#else
    #if 0  
	SysSemUp(&dev->feedlock);
    #else
	SysSemUp(dev->feedlock);
    #endif
#endif 

	
	if(1 == status) {
		DBG_fCDVB("calling StartFeed()\n");
		ret = StartFeedCallback(dev->context);
		if(ret < 0) status = --dev->feedcount;
		
	}


#if PID_MONITOR
    StartPIDFeedEvent(dev->context, feed->pid);
#endif 


#if 0
	if(ret >= 0)
		HandlePIDFilter(dev->context, feed->pid, 1);
#endif
#if 0  
	return status;
#else  
	return ret;
#endif 
}

static int demux_sw_stop_feed(struct dvb_demux_feed *feed)
{
	struct dvb_demux *demux = feed->demux;
	DVBDev *dev = demux->priv;
	int status = 0;

	DBG_fCDVB("pid %d stop\n", feed->pid);
#if 0 
	down(&dev->feedlock);
	status = --(dev->feedcount);
	up(&dev->feedlock);
#else
#if 0 
	SysSemDown(&dev->feedlock);
	status = --(dev->feedcount);
	SysSemUp(&dev->feedlock);
#else
	SysSemDown(dev->feedlock);
	status = --(dev->feedcount);
	SysSemUp(dev->feedlock);
#endif 
#endif 

#if 0
	HandlePIDFilter(dev->context, feed->pid, 0);
#endif

	if(0 == status) {
		DBG_fCDVB("calling StopFeed()\n");
		StopFeedCallback(dev->context);
	}

#if PID_MONITOR
    StopPIDFeedEvent(dev->context, feed->pid);
#endif 


	return status;
}

static int dvbfe_set_parameters(struct dvb_frontend *fe,
				struct dvb_frontend_parameters *param)
{
	DECLARE_DEV;
	DBG_fCDVB("calling SetParameters()\n");
	return SetParameters(p->context, param);
}

static int dvbfe_get_parameters(struct dvb_frontend *fe,
				struct dvb_frontend_parameters *param)
{
	DECLARE_DEV;
	DBG_fCDVB("calling GetParameters()\n");
	return GetParameters(p->context, param);
}

static int dvbfe_read_status(struct dvb_frontend* fe, fe_status_t* status)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling GetStatus()\n");
	return GetStatus(p->context, status);
}

static int dvbfe_read_ber(struct dvb_frontend* fe, u32* ber)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling GetBER()\n");
	return GetBER(p->context, ber);
}

static int dvbfe_read_signal_strength(struct dvb_frontend* fe, u16* strength)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling GetSignalStrength()\n");
	return GetSignalStrength(p->context, strength);
}

static int dvbfe_read_snr(struct dvb_frontend* fe, u16* snr)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling GetSNR()\n");
	return GetSNR(p->context, snr);
}

static int dvbfe_read_ucblocks(struct dvb_frontend* fe, u32* ucblocks)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling GetUCBlocks()\n");
	return GetUCBlocks(p->context, ucblocks);
}

static int dvbfe_init(struct dvb_frontend* fe)
{
	DECLARE_DEV;
	DBG_fCDVB("calling FEInit()\n");
	return FEInit(p->context);
}

static int dvbfe_sleep(struct dvb_frontend* fe)
{
	DECLARE_DEV;
	DBG_fCDVB("calling FESleep()\n");
	return FESleep(p->context);
}

static int dvbfe_get_tune_settings(struct dvb_frontend *fe,
			struct dvb_frontend_tune_settings *settings)
{
	DBG_fCDVB("Get tune settings.\n");
	settings->min_delay_ms = 100;
	settings->max_drift = 0;
	settings->step_size = 0;

	return 0;
}


#if	defined(USE_SWIC2) && (USE_SWIC2!=0)
static int dvbfe_diseqc_reset_overload(struct dvb_frontend *fe)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling \n");
	return DiseqcResetOverload(p->context);
}

static int dvbfe_diseqc_send_master_cmd(struct dvb_frontend *fe ,struct dvb_diseqc_master_cmd* arg)
{
	DECLARE_DEV;
	DBG_fCDVBFE("calling \n");
	return DiseqcSendMasterCMD(p->context, arg);
}

static int dvbfe_diseqc_recv_slave_reply(struct dvb_frontend *fe ,struct dvb_diseqc_slave_reply* arg)
{
        DECLARE_DEV;
        DBG_fCDVBFE("calling \n");
        return DiseqcRecvSlaveReply(p->context, arg);
}

static int dvbfe_diseqc_send_burst(struct dvb_frontend *fe, fe_sec_mini_cmd_t arg)
{
        DECLARE_DEV;
        DBG_fCDVBFE("calling \n");
        return DiseqcSendBurst(p->context, arg);
}

static int dvbfe_set_tone(struct dvb_frontend *fe, fe_sec_tone_mode_t arg)
{
        DECLARE_DEV;
        DBG_fCDVBFE("calling \n");
        return SetTone(p->context, arg);
}

static int dvbfe_set_voltage(struct dvb_frontend *fe, fe_sec_voltage_t arg)
{
        DECLARE_DEV;
        DBG_fCDVBFE("calling \n");
        return SetVoltage(p->context, arg);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,16)
static int dvbfe_enable_high_lnb_voltage(struct dvb_frontend *fe, int arg)
#else
static int dvbfe_enable_high_lnb_voltage(struct dvb_frontend *fe, long arg)
#endif
{
        DECLARE_DEV;
        DBG_fCDVBFE("calling \n");
        return EnableHighLNBVoltage(p->context, arg);
}

#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
static void dvbfe_release(struct dvb_frontend* fe)
{
	return; 
}
#endif


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
static int dvb_fe_ioctl(struct dvb_frontend *fe, unsigned int cmd, void *arg)
{
	DECLARE_DEV;

	switch(cmd) {
	case FE_GET_INFO:
		memcpy(arg, p->dvbfe_info, sizeof(struct dvb_frontend_info));
		return 0;
	case FE_DISEQC_RESET_OVERLOAD:
	case FE_DISEQC_SEND_MASTER_CMD:
	case FE_DISEQC_RECV_SLAVE_REPLY:
	case FE_DISEQC_SEND_BURST:
	case FE_SET_TONE:
	case FE_SET_VOLTAGE:
	case FE_ENABLE_HIGH_LNB_VOLTAGE:
	case FE_GET_EVENT:
		return -EOPNOTSUPP;

	case FE_READ_UNCORRECTED_BLOCKS:
		return dvbfe_read_ucblocks(fe, (u32*)arg);

	case FE_READ_STATUS:
		return dvbfe_read_status(fe, (fe_status_t *)arg);
	
	case FE_READ_BER:
		return dvbfe_read_ber(fe, (u32 *)arg);

	case FE_READ_SIGNAL_STRENGTH:
		return dvbfe_read_signal_strength(fe, (u16 *)arg);

	case FE_READ_SNR:
		return dvbfe_read_snr(fe, (u16 *)arg);

	case FE_SET_FRONTEND:
		return dvbfe_set_parameters(fe,
					(struct dvb_frontend_parameters *)arg);
	
	case FE_GET_FRONTEND:
		return dvbfe_get_parameters(fe,
					(struct dvb_frontend_parameters *)arg);

	case FE_INIT:
		return dvbfe_init(fe);
		
	case FE_SLEEP:
		return dvbfe_sleep(fe);

	case FE_GET_TUNE_SETTINGS:
		return dvbfe_get_tune_settings(fe,
#if 0 
					(struct dvbfe_get_tune_settings*) arg);
#else
					(struct dvb_frontend_tune_settings*) arg);
#endif
		
	}

	return -EINVAL;
}
#endif

