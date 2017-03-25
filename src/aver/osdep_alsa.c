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
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/wait.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/version.h> 
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25) 
#include <sound/driver.h>
#endif 
#include <sound/core.h>
#include <sound/control.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <linux/interrupt.h>

#include "debug.h"
#include "osdep.h"
#include "osdep_alsa.h"
#include "CAlsaAudio.hpp"

#if 0 
static int index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX;
static char *id[SNDRV_CARDS] = SNDRV_DEFAULT_STR;
static int enable[SNDRV_CARDS] = {1, [1 ... (SNDRV_CARDS - 1)] = 1};
static struct snd_card *alsa_snd_cards[SNDRV_CARDS];

static int devno=0;
#endif 

struct alsa_card_context;

struct alsa_pcm_context {
	struct snd_pcm			*pcm;
	struct alsa_card_context	*parent;
	void 				*pcm_private;
	struct snd_pcm_hardware		hw;
	int				index;
};

struct alsa_card_context {
	struct snd_card			*card;
	struct alsa_pcm_context		pcm[MAX_ALSA_PCM];
	struct snd_pcm_substream	*substream;
	void				*card_private;

	unsigned int			dma_size;
	unsigned int			period_size;
	unsigned int			num_periods;
};

#if 0 
#if 0 
int SysAlsaInitCard(alsa_card_t *acard, char *drivername, void *device, void *private_data)
#else
int SysAlsaInitCard(alsa_card_t *acard, const char *drivername, const char *devicename,
			const char *deviceid,  void *device, void *private_data)
#endif 
{
	struct snd_card *card = NULL;
	struct alsa_card_context *context = NULL;
	int i;

	DBG_fALSA(" name=%s, device=0x%p, private=0x%p\n", drivername, device, private_data);

	if( devno >= SNDRV_CARDS ) {
		DBG_fALSA(" number of cards max out\n");
		return -ENODEV;
	}

	if(!enable[devno]) {
		DBG_fALSA(" slot %d disabled\n", devno);
		++devno;
		return -ENOENT;
	}

	
	card = snd_card_new(index[devno], id[devno], THIS_MODULE, sizeof(struct alsa_card_context));
	if( NULL == card ) {
		DBG_fALSA(" calling snd_card_new failed\n");
		return -ENOMEM;
	}
	else {
		DBG_fALSA(" got snd_card ptr 0x%p\n", card);
	}

	

#if 0 
	SysStrNCpy(card->driver, drivername, sizeof(card->driver));
	SysStrNCpy(card->shortname, drivername, sizeof(card->shortname));
	SysStrNCpy(card->longname, drivername, sizeof(card->longname));
#else
	SysSnPrintf(card->driver, sizeof(card->driver), "%s", drivername);
	SysSnPrintf(card->shortname, sizeof(card->shortname), "%s", devicename);
	SysSnPrintf(card->longname, sizeof(card->longname), "AVerMedia %s %s", drivername, devicename); 
	SysSnPrintf(card->id, sizeof(card->id), "%s", deviceid);
	SysSnPrintf(card->mixername, sizeof(card->mixername), "%s", deviceid);
#endif 
	
	
	context = (struct alsa_card_context *)(card->private_data);
	DBG_fALSA(" context ptr = 0x%p\n", context);
	SysMemSet(context, 0, sizeof(struct alsa_card_context));
	for(i=0; i<MAX_ALSA_PCM; ++i) {
		context->pcm[i].index = i;
	}
	context->card = card;
	context->card_private = private_data;
	*acard = context;
	
	alsa_snd_cards[devno] = card;
	snd_card_set_dev(card, device);

	devno++;
	return 0;
}

#else

int SysAlsaInitCard(alsa_card_t *acard, const char *drivername, const char *devicename,
			const char *deviceid,  void *device, void *private_data)
{
	struct snd_card *card = NULL;
	struct alsa_card_context *context = NULL;
	int i;

	DBG_fALSA(" name=%s, device=0x%p, private=0x%p\n", drivername, device, private_data);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30) 
	card = snd_card_new(-1, deviceid, THIS_MODULE, sizeof(struct alsa_card_context));
	if( NULL == card ) {
		DBG_fALSA(" calling snd_card_new failed\n");
		return -ENOMEM;
	}
	else {
		DBG_fALSA(" got snd_card ptr 0x%p\n", card);
	}
#else
	i = snd_card_create(-1, deviceid, THIS_MODULE, sizeof(struct alsa_card_context), &card);
	if( i<0 ) {
		DBG_fALSA(" calling snd_card_new failed, ret=%d\n", i);
		return i;
	}
	else {
		DBG_fALSA(" got snd_card ptr 0x%p\n", card);
	}
#endif 

	

	SysSnPrintf(card->driver, sizeof(card->driver), "%s", drivername);
	SysSnPrintf(card->shortname, sizeof(card->shortname), "%s", devicename);
	SysSnPrintf(card->longname, sizeof(card->longname), "AVerMedia %s %s", drivername, devicename); 
	SysSnPrintf(card->mixername, sizeof(card->mixername), "%s", deviceid);
	
	
	context = (struct alsa_card_context *)(card->private_data);
	DBG_fALSA(" context ptr = 0x%p\n", context);
	SysMemSet(context, 0, sizeof(struct alsa_card_context));
	for(i=0; i<MAX_ALSA_PCM; ++i) {
		context->pcm[i].index = i;
	}
	context->card = card;
	context->card_private = private_data;
	*acard = context;
	
	snd_card_set_dev(card, device);

	return 0;
}
#endif 


int SysAlsaRegisterCard(alsa_card_t acard)
{
	int ret = 0;
	struct alsa_card_context *context = (struct alsa_card_context *)acard;
	struct snd_card *card = (context)? context->card : NULL;

	DBG_fALSA(" + \n");

	if( NULL == card ) {
		DBG_fALSA(" error, card=0x%p, context=0x%p\n", card, context);
		return -EINVAL;
	}

	ret = snd_card_register(card);

	DBG_fALSA("context=0x%p, card=0x%p\n", context, card);
	DBG_fALSA("index=%d\n", card->number);
	DBG_fALSA("driver=%s\n", card->driver);
	DBG_fALSA("shortname=%s\n", card->shortname);
	DBG_fALSA("longname=%s\n", card->longname);
	DBG_fALSA("id=%s\n", card->id);
	DBG_fALSA("mixername=%s\n", card->mixername);
	DBG_fALSA("last_numid=%d\n", card->last_numid);
	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}



int SysAlsaGetCardIndex(alsa_card_t acard)
{
	struct alsa_card_context *context = (struct alsa_card_context *)acard;
	struct snd_card *card = (context)? context->card : NULL;

	if( NULL == context ) {
		DBG_fALSA(" error, context=0x%p\n", context);
		return -EINVAL;
	}
	if( NULL == card ) {
		DBG_fALSA(" error, card=0x%p\n", card);
		return -EINVAL;
	}

	return card->number;
}


int SysAlsaPcmPeriodElapsed(alsa_card_t acard)
{
	struct alsa_card_context *context = (struct alsa_card_context *)acard;

	if( NULL == context ) {
		DBG_fALSA(" error, context=0x%p\n", context);
		return -EINVAL;
	}
	if( context->substream ) {
		snd_pcm_period_elapsed(context->substream);
		return 0;
	}
	else {
		DBG_fALSA(" error, no substream\n");
		return -EINVAL;
	}
}

int SysAlsaFinishCard(alsa_card_t acard, int surprise_removable)
{
	int ret = 0;
	struct alsa_card_context *context = (struct alsa_card_context *)acard;
	struct snd_card *card = (context)? context->card : NULL;
	
	DBG_fALSA(" + \n");

	if( NULL == card ) {
		DBG_fALSA(" error, card=0x%p, context=0x%p\n", card, context);
		return -EINVAL;
	}

	if( surprise_removable ) {



		ret = snd_card_free(card);

	}
	else {
		ret = snd_card_free(card);
	}

	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}

static snd_pcm_uframes_t
alsa_pcm_capture_pointer(struct snd_pcm_substream * substream)
{
	snd_pcm_uframes_t ret;
	unsigned long count;
	struct snd_pcm_runtime *runtime = substream? substream->runtime : NULL;
	struct alsa_pcm_context *pcmcontext = substream? snd_pcm_substream_chip(substream) : NULL;
	struct alsa_card_context *card = pcmcontext? pcmcontext->parent : NULL;
	if( NULL==card ) {
		DBG_fALSA(" error, runtime=0x%p, pcmcontext=0x%p, card=0x%p\n", runtime, pcmcontext, card);
		return -EINVAL;
	}

	count = StaticCapturePointer(pcmcontext->pcm_private);
	ret = bytes_to_frames(runtime, count);
	DBG_fALSA(" count=%ld, frames=%ld\n", count, ret);


	return ret;
}

static int alsa_pcm_capture_trigger(struct snd_pcm_substream * substream, int cmd)
{
	int ret = 0;
	
	struct alsa_pcm_context *pcmcontext = snd_pcm_substream_chip(substream);

	DBG_fALSA(" + \n");

	if( NULL==pcmcontext ) {
		DBG_fALSA(" error, pcmcontext=0x%p\n", pcmcontext);
		return -EINVAL;
	}

	if(cmd == SNDRV_PCM_TRIGGER_START)
		ret = StaticCaptureTrigger(pcmcontext->pcm_private, 1);
	else if(cmd == SNDRV_PCM_TRIGGER_STOP)
		ret = StaticCaptureTrigger(pcmcontext->pcm_private, 0);
	else
		ret = -EINVAL;

	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}

static int alsa_pcm_capture_prepare(struct snd_pcm_substream * substream)
{
	int ret = 0;
#if DEBUG_ALSA
	struct snd_pcm_runtime *runtime = substream? substream->runtime : NULL;
#endif
	struct alsa_pcm_context *pcmcontext = substream? snd_pcm_substream_chip(substream) : NULL;
	struct alsa_card_context *card = pcmcontext? pcmcontext->parent : NULL;

	DBG_fALSA(" + \n");

	if( NULL==card ) {
		DBG_fALSA(" error, runtime=0x%p, pcmcontext=0x%p, card=0x%p\n", runtime, pcmcontext, card);
		return -EINVAL;
	}

	ret = StaticCapturePrepare(pcmcontext->pcm_private);

	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}

static int alsa_pcm_capture_hw_free(struct snd_pcm_substream * substream)
{
	int ret = 0;
#if DEBUG_ALSA
	struct snd_pcm_runtime *runtime = substream? substream->runtime : NULL;
#endif
	struct alsa_pcm_context *pcmcontext = substream? snd_pcm_substream_chip(substream) : NULL;
	struct alsa_card_context *card = pcmcontext? pcmcontext->parent : NULL;

	DBG_fALSA(" +\n");

	if( NULL==card ) {
		DBG_fALSA(" error, runtime=0x%p, pcmcontext=0x%p, card=0x%p\n", runtime, pcmcontext, card);
		return -EINVAL;
	}

	
	if( substream->runtime->dma_area ) {
		ret = StaticCaptureHwFree(pcmcontext->pcm_private, substream->runtime->dma_area);
		substream->runtime->dma_area = NULL;
	}

	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}

static int alsa_pcm_capture_hw_params(struct snd_pcm_substream * substream,
					struct snd_pcm_hw_params * hw_params)
{
	int ret = 0;
#if DEBUG_ALSA
	struct snd_pcm_runtime *runtime = substream? substream->runtime : NULL;
#endif
	struct alsa_pcm_context *pcmcontext = substream? snd_pcm_substream_chip(substream) : NULL;
	struct alsa_card_context *card = pcmcontext? pcmcontext->parent : NULL;

	DBG_fALSA(" +\n");

	if( NULL==card ) {
		DBG_fALSA(" error, runtime=0x%p, pcmcontext=0x%p, card=0x%p\n", runtime, pcmcontext, card);
		return -EINVAL;
	}

	
	if( substream->runtime->dma_area ) {
		ret = StaticCaptureHwFree(pcmcontext->pcm_private, substream->runtime->dma_area);
		substream->runtime->dma_area = NULL;
	}

	card->period_size = params_period_bytes(hw_params);
	card->num_periods = params_periods(hw_params);
	card->dma_size = card->period_size * params_periods(hw_params);

	DBG_fALSA(" period_size=%d, periods=%d, dmasize=%d\n", card->period_size, card->num_periods, card->dma_size);

	
	substream->runtime->dma_area = StaticCaptureHwParams(pcmcontext->pcm_private, card->dma_size);

	if( NULL==substream->runtime->dma_area ) ret = -ENOMEM;
	else ret = 0;

	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}


static int alsa_pcm_capture_close(struct snd_pcm_substream * substream)
{
	int ret = 0;
#if DEBUG_ALSA
	struct snd_pcm_runtime *runtime = substream? substream->runtime : NULL;
#endif
	struct alsa_pcm_context *pcmcontext = substream? snd_pcm_substream_chip(substream) : NULL;
	struct alsa_card_context *card = pcmcontext? pcmcontext->parent : NULL;

	DBG_fALSA(" +\n");

	if( NULL==card ) {
		DBG_fALSA(" error, runtime=0x%p, pcmcontext=0x%p, card=0x%p\n", runtime, pcmcontext, card);
		return -EINVAL;
	}

	ret = StaticCaptureClose(pcmcontext->pcm_private);

	card->substream = NULL;
	
	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}

static int alsa_pcm_capture_open(struct snd_pcm_substream * substream)
{
	int ret = 0;
	struct snd_pcm_runtime *runtime = substream? substream->runtime : NULL;
	struct alsa_pcm_context *pcmcontext = substream? snd_pcm_substream_chip(substream) : NULL;
	struct alsa_card_context *card = pcmcontext? pcmcontext->parent : NULL;

	DBG_fALSA(" +\n");

	if( NULL==card ) {
		DBG_fALSA(" error, runtime=0x%p, pcmcontext=0x%p, card=0x%p\n", runtime, pcmcontext, card);
		return -EINVAL;
	}

	ret = snd_pcm_hw_constraint_integer(runtime, SNDRV_PCM_HW_PARAM_PERIODS);
	if( ret < 0 ) {
		DBG_fALSA(" hw constraint error, ret=%d\n", ret);
		return ret;
	}


	runtime->hw = pcmcontext->hw;
	card->substream = substream;

	ret = StaticCaptureOpen(pcmcontext->pcm_private);

	DBG_fALSA(" - ret=%d\n", ret);
	return ret;
}

#if 0
static int alsa_pcm_capture_copy(struct snd_pcm_substream *substream, int channel,
				snd_pcm_uframes_t pos,
				void __user *buf, snd_pcm_uframes_t count)
{

}
#endif

static struct snd_pcm_ops snd_card_capture_ops = {
	.open =		alsa_pcm_capture_open,
	.close =	alsa_pcm_capture_close,
	.ioctl =	snd_pcm_lib_ioctl,
	.hw_params =	alsa_pcm_capture_hw_params,
	.hw_free = 	alsa_pcm_capture_hw_free,
	.prepare =	alsa_pcm_capture_prepare,
	.trigger =	alsa_pcm_capture_trigger,
	.pointer =	alsa_pcm_capture_pointer,
	
};

int SysAlsaAddPCM(alsa_card_t acard, int index, const char *pcmname, const struct alsa_hw_t *hw, void *private_data)
{
	int ret = 0;
	struct alsa_card_context *context = (struct alsa_card_context *)acard;
	struct snd_card *card = (context)? context->card : NULL;
	char buf[16];

	DBG_fALSA("\n");

	if( NULL==card || NULL==hw ) {
		DBG_fALSA(" error, card=0x%p, context=0x%p, hw=0x%p\n", card, context, hw);
		return -EINVAL;
	}

	snprintf(buf, sizeof(buf), "%s PCM", pcmname); 
	

	ret = snd_pcm_new(card, buf, index, 0, 1, &context->pcm[index].pcm); 
	if( ret < 0 ) {
		DBG_fALSA(" calling snd_pcm_new returned %d\n", ret);
		return ret;
	}

	context->pcm[index].pcm_private = private_data;
	context->pcm[index].parent = context;

	
	context->pcm[index].hw.info = SNDRV_PCM_INFO_INTERLEAVED | SNDRV_PCM_INFO_BLOCK_TRANSFER;

	switch(hw->format) {
	case ALSA_PCM_FMT_S8: context->pcm[index].hw.formats = SNDRV_PCM_FMTBIT_S8; break;
	case ALSA_PCM_FMT_U8: context->pcm[index].hw.formats = SNDRV_PCM_FMTBIT_U8; break;
	case ALSA_PCM_FMT_S16LE: context->pcm[index].hw.formats = SNDRV_PCM_FMTBIT_S16_LE; break;
	case ALSA_PCM_FMT_S16BE: context->pcm[index].hw.formats = SNDRV_PCM_FMTBIT_S16_BE; break;
	case ALSA_PCM_FMT_U16LE: context->pcm[index].hw.formats = SNDRV_PCM_FMTBIT_U16_LE; break;
	default:
	case ALSA_PCM_FMT_U16BE: context->pcm[index].hw.formats = SNDRV_PCM_FMTBIT_U16_LE; break;
	}

	switch(hw->rate) {
	case ALSA_PCM_RATE_32000:
		context->pcm[index].hw.rates = SNDRV_PCM_RATE_32000;
		context->pcm[index].hw.rate_min = 32000;
		context->pcm[index].hw.rate_max = 32000;
		break;

	case ALSA_PCM_RATE_44100:
		context->pcm[index].hw.rates = SNDRV_PCM_RATE_44100;
		context->pcm[index].hw.rate_min = 44100;
		context->pcm[index].hw.rate_max = 44100;
		break;

	default:
	case ALSA_PCM_RATE_48000:
		context->pcm[index].hw.rates = SNDRV_PCM_RATE_48000;
		context->pcm[index].hw.rate_min = 48000;
		context->pcm[index].hw.rate_max = 48000;
		break;
	}

	context->pcm[index].hw.channels_min = hw->channels;
	context->pcm[index].hw.channels_max = hw->channels;
	context->pcm[index].hw.buffer_bytes_max = hw->input_buffer_size;
	context->pcm[index].hw.period_bytes_min = (hw->input_buffer_size)/2;
	context->pcm[index].hw.period_bytes_max = (hw->input_buffer_size)/2;
	context->pcm[index].hw.periods_min = 2;
	context->pcm[index].hw.periods_max = 2;
	context->pcm[index].hw.fifo_size = 0; 

	snd_pcm_set_ops(context->pcm[index].pcm, SNDRV_PCM_STREAM_CAPTURE, &snd_card_capture_ops);
	context->pcm[index].pcm->private_data = &context->pcm[index];
	context->pcm[index].pcm->info_flags = SNDRV_PCM_INFO_INTERLEAVED;
	SysStrNCpy(context->pcm[index].pcm->name, pcmname, sizeof(context->pcm[index].pcm->name));

	return 0;
}
