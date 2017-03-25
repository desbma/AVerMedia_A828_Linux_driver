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
#ifndef __CALSAAUDIO_HPP__
#define __CALSAAUDIO_HPP__

#include "osdep_alsa.h"

#ifdef __cplusplus

class CAlsaPcm;

class CAlsaCard {

friend class CAlsaPcm;

public:
	CAlsaCard(int surprise_removable = true);
	virtual ~CAlsaCard();

#if 0 
	int Init(char *drivername, void *device);
#else
	int Init(const char *drivername, const char *devicename,
			const char *deviceid, void *device);
#endif
	int Finish(bool surprise_removable);
	int Register();

	alsa_card_t GetCard() { return m_card; }
	int GetCardIndex() { return SysAlsaGetCardIndex(m_card); } 

protected:

	alsa_card_t	m_card;
	int		m_surpriseRemovable;
};


struct CAlsaPcmWrapper
{
	CAlsaPcm *ptr;
};

class CAlsaPcm {

public:
	CAlsaPcm(CAlsaCard *parent);
	virtual ~CAlsaPcm();

	void SetHwFormat(unsigned int format, unsigned int rate, unsigned int channels, unsigned int input_buffer_size);
	int AddDevice(int index, const char *pcmname);

	virtual int CaptureOpen() { return 0; }
	virtual int CaptureClose() { return 0; }
	virtual int CaptureTrigger(bool start) { return 0; }
	virtual void *CaptureHwParams(unsigned long dmasize) { return 0; }
	virtual int CaptureHwFree(void *memory) { return 0; }
	virtual int CapturePrepare() { return 0; }
	virtual unsigned long CapturePointer() { return 0; }

protected:

	int		m_index;
	CAlsaCard	*m_parent;
	struct alsa_hw_t m_hw;
	CAlsaPcmWrapper	m_wrapper;
};

#endif 

#ifdef __cplusplus
extern "C" {
#endif



int StaticCaptureOpen(void *context);
int StaticCaptureClose(void *context);
int StaticCaptureTrigger(void *context, int start);
void *StaticCaptureHwParams(void *context, unsigned long dmasize);
int StaticCaptureHwFree(void *context, void *memory);
int StaticCapturePrepare(void *context);
unsigned long StaticCapturePointer(void *context);


#ifdef __cplusplus
}
#endif

#endif 
