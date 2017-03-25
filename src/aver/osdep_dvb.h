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
#ifndef __OSDEP_DVB_H__
#define __OSDEP_DVB_H__

typedef void * dvb_t; 



#define PID_MONITOR 0


#ifndef __OSDEP_DVB_NO_FUNC__ 

#ifdef __cplusplus
extern "C" {
#endif	

dvb_t SysDVBInit(void *context);
void SysDVBFini(dvb_t dev);


#if 0
int SysDVBRegisterAdapter(dvb_t dev, const char *name, int max_filters);
#else
int SysDVBRegisterAdapter(dvb_t dev, const char *name, int max_filters, void *device);
#endif

int SysDVBUnRegisterAdapter(dvb_t dev);
int SysDVBRegisterFrontend(dvb_t dev, struct dvb_frontend_info *info, void *dvb_adapter_or_i2c_bus);
int SysDVBUnRegisterFrontend(dvb_t dev);

void SysDVBFeedTSPackets(dvb_t dev, unsigned char *buf, unsigned int len);

void *Get_Adapter_or_I2CBus(dvb_t dev); 


#if PID_MONITOR
int StartPIDFeedEvent(void *context, int pid);
int StopPIDFeedEvent(void *context, int pid);
#endif 



int StartFeedCallback(void *context);
int StopFeedCallback(void *context);

int GetUCBlocks(void *context, unsigned int *blocks);
int GetStatus(void *context, fe_status_t *stat);
int GetBER(void *context, unsigned int *ber);
int GetSignalStrength(void *context, unsigned short *signal);
int GetSNR(void *context, unsigned short *snr);
int SetParameters(void *context, struct dvb_frontend_parameters *param);
int GetParameters(void *context, struct dvb_frontend_parameters *param);
int FEInit(void *context);
int FESleep(void *context);


#if	defined(USE_SWIC2) && (USE_SWIC2!=0)
int DiseqcResetOverload(void *context);
int DiseqcSendMasterCMD(void *context, struct dvb_diseqc_master_cmd *cmd);
int DiseqcRecvSlaveReply(void *context, struct dvb_diseqc_slave_reply* reply);
int DiseqcSendBurst(void *context, fe_sec_mini_cmd_t burst);
int SetTone(void *context, fe_sec_tone_mode_t tone);
int SetVoltage(void *context, fe_sec_voltage_t voltage);
int EnableHighLNBVoltage(void *context, int enable);
#endif




#ifdef __cplusplus
}
#endif	

#endif 

#endif 

