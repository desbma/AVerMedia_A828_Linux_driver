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



















#ifndef	__AVER_DEBUG_H__
#define	__AVER_DEBUG_H__

#if defined(__KERNEL__) && defined(__cplusplus)
#include "osdep.h"
#endif





#ifdef	__KERNEL__
	#ifdef	__cplusplus
		#define	DBG_NO_FUNC(fmt, arg...)						do { SysPrintk(fmt, ## arg); } while( 0 )

		#define DBG_FUNC(fmt, arg...)		 					do {										SysPrintk("%s: "fmt,									 __FUNCTION__, ##arg);				} while( 0 )

	#else	
		#define	DBG_NO_FUNC(fmt, arg...)						do { printk(fmt, ## arg); } while( 0 )

		#define DBG_FUNC(fmt, arg...)		 					do {										printk("%s: "fmt, __FUNCTION__, ##arg);				} while( 0 )
	#endif	

#else	
	#define	DBG_NO_FUNC(fmt, arg...)						do { printf(fmt, ## arg); } while( 0 )

	#define DBG_FUNC(fmt, arg...)		 					do {										printf("%s: " fmt, __FUNCTION__, ## arg);			} while( 0 )

#endif	

#define	DBG_NULL(fmt, arg...)			do { } while( 0 )












#if defined(DEBUG_PRINT) && (DEBUG_PRINT!=0)
	#define DBG_fPRINT(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_PRINT(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_fPRINT(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_PRINT(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_INT) && (DEBUG_INT!=0)
	#define DBG_fINT(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_INT(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_fINT(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_INT(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_I2C) && (DEBUG_I2C!=0)
	#define DBG_fI2C(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_I2C(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_fI2C(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_I2C(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_GPIO) && (DEBUG_GPIO!=0)
	#define DBG_fGPIO(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_GPIO(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_fGPIO(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_GPIO(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_OSDEP) && (DEBUG_OSDEP!=0)
	#define DBG_OSDEP(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
	#define DBG_fOSDEP(fmt, arg...)		DBG_FUNC(fmt, ##arg)

#else	
	#define DBG_OSDEP(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_fOSDEP(fmt, arg...)		DBG_NULL(fmt, ##arg)

#endif	

#if defined(DEBUG_CUSB) && (DEBUG_CUSB!=0)
	#define DBG_fCUSB(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_CUSB(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
#else	
	#define DBG_fCUSB(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_CUSB(fmt, arg...)		DBG_NULL(fmt, ## arg)
#endif	

#if defined(DEBUG_CUSBEP) && (DEBUG_CUSBEP!=0)
	#define DBG_fCUSBEP(fmt, arg...)	DBG_FUNC(fmt, ## arg)
	#define DBG_CUSBEP(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
#else	
	#define DBG_fCUSBEP(fmt, arg...)	DBG_NULL(fmt, ## arg)
	#define DBG_CUSBEP(fmt, arg...)		DBG_NULL(fmt, ## arg)
#endif	

#if defined(DEBUG_CDVB) && (DEBUG_CDVB!=0)
	#define DBG_fCDVB(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_CDVB(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fCDVB(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_CDVB(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_CDVBFE) && (DEBUG_CDVBFE!=0)
	#define DBG_fCDVBFE(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_CDVBFE(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fCDVBFE(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_CDVBFE(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if 0 
#if defined(DEBUG_IA) && (DEBUG_IA!=0)
	#define DBG_fIA(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_IA(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fIA(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_IA(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	
#endif

#if defined(DEBUG_DBGDEV) && (DEBUG_DBGDEV!=0)
	#define DBG_fDBGDEV(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_DBGDEV(fmt, arg...)			DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fDBGDEV(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_DBGDEV(fmt, arg...)			DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_VDEV) && (DEBUG_VDEV!=0)
	#define DBG_fVDEV(fmt, arg...)	DBG_FUNC(fmt, ## arg)
	#define DBG_VDEV(fmt, arg...)	DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fVDEV(fmt, arg...)	DBG_NULL(fmt, ## arg)
	#define DBG_VDEV(fmt, arg...)	DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_CSTREAM) && (DEBUG_CSTREAM!=0)
	#define DBG_fCSTREAM(fmt, arg...)	DBG_FUNC(fmt, ## arg)
	#define DBG_CSTREAM(fmt, arg...)	DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fCSTREAM(fmt, arg...)	DBG_NULL(fmt, ## arg)
	#define DBG_CSTREAM(fmt, arg...)	DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_CBUF) && (DEBUG_CBUF!=0)
	#define DBG_fCBUF(fmt, arg...)	DBG_FUNC(fmt, ## arg)
	#define DBG_CBUF(fmt, arg...)	DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_fCBUF(fmt, arg...)	DBG_NULL(fmt, ## arg)
	#define DBG_CBUF(fmt, arg...)	DBG_NULL(fmt, ## arg)

#endif	


#if defined(DEBUG_SG) && (DEBUG_SG!=0)
	#define DBG_fSG(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_SG(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)
	
#else	
	#define DBG_SG(fmt, arg...)		DBG_NULL(fmt, ## arg)
	#define DBG_SG(fmt, arg...)		DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_CTRLPIPE) && (DEBUG_CTRLPIPE!=0)
	#define DBG_CTRLPIPE(fmt, arg...)	DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_CTRLPIPE(fmt, arg...)	DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_ALSA) && (DEBUG_ALSA!=0)
	#define DBG_fALSA(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_ALSA(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_fALSA(fmt, arg...)	DBG_NULL(fmt, ## arg)
	#define DBG_ALSA(fmt, arg...)	DBG_NULL(fmt, ## arg)

#endif	

#if defined(DEBUG_BUFFIFO) && (DEBUG_BUFFIFO!=0)
	#define DBG_fBFIFO(fmt, arg...)		DBG_FUNC(fmt, ## arg)
	#define DBG_BFIFO(fmt, arg...)		DBG_NO_FUNC(fmt, ## arg)

#else	
	#define DBG_fBFIFO(fmt, arg...)	DBG_NULL(fmt, ## arg)
	#define DBG_BFIFO(fmt, arg...)	DBG_NULL(fmt, ## arg)

#endif	

#endif	
