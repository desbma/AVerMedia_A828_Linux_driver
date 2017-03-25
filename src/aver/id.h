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
#ifndef	__ID_H__
#define	__ID_H__

#include "osdep.h" 




#define	USB_VID_DiBcom		0x10B8
#define	USB_VID_Buffalo		0x0411
#define USB_VID_AVer		0x07CA




#define	USB_PID_LDR7700		0x0064
#define	USB_PID_7700		0x0065
#define	USB_PID_M803		0x0084

#define	USB_PID_A300		0xA300
#define USB_PID_A300LDR		0xA2FF

#define	USB_PID_A302		0xA302
#define USB_PID_A302LDR		0xA301

#define	USB_PID_A808		0xA808
#define USB_PID_A808LDR		0xA807

#define USB_PID_E568		0xE568 

#define USB_PID_B300		0xB300
#define USB_PID_B302		0xB302
#define USB_PID_B808		0xB808
#define USB_PID_B568		0xB568 

#define USB_PID_A828		0xA828 
#define USB_PID_A301		0xA301 

#define USB_PID_A333		0xA333
#define USB_PID_A825		0xA825 

#define USB_PID_C038		0xC038 

#define USB_PID_H8261		0x0826 
#define USB_PID_H8262		0x2826 

#define USB_PID_H826D1		0x1826 
#define USB_PID_H826D2		0x3826 
#define USB_PID_H826D3		0x5826 


#define	USB_PID_A321D		0x0321 
#define	USB_PID_A321		0xA321 

#define USB_PID_A827		0xA827 
#define USB_PID_A827J		0x0827 
#define USB_PID_A827A1		0xB827 
#define USB_PID_A827A2		0x1827 
#define USB_PID_A827A3		0x3827 




typedef enum usb_board_type {
	  DiBcom_7700 = 1
	, DiBcom_LDR7700
	, Buffalo_M803
	, AVer_A300
	, AVer_A300LDR
	, AVer_A302
	, AVer_A302LDR
	, AVer_A808
	, AVer_A808LDR
	, AVer_E568	
	, AVer_B300
	, AVer_B302
	, AVer_B808
	, AVer_B568	
	, AVer_A828 	
	, AVer_A301 	
	, AVer_A333
	, AVer_A825	
	, AVer_C038		
	, AVer_H8261		
	, AVer_H8262		
	, AVer_H826D1	
	, AVer_H826D2	
	, AVer_H826D3	
	, AVer_A321D	
	, AVer_A321		
	, AVer_A827		
	, AVer_A827J		
	, AVer_A827A1		
	, AVer_A827A2		
	, AVer_A827A3		
	, AVer_Max_Board_ID	
} usb_board_type;





#ifdef __cplusplus
extern "C"
{
#endif 
	extern const char *aver_usb_board_name[];
#ifdef __cplusplus
}
#endif 


#endif	
