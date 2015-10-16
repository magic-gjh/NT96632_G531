/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of GPIO mapping
                This file is the header file of GPIO mapping

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _GPIOMAPPING_H
#define _GPIOMAPPING_H

#include "pad.h"
#include "GPIO.h"
#include "SysCfg.h"

#if (_PCB_ == _PCB_EVB_)
#include "PcbEVB/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_WS_)
#include "PcbWS/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_PP_)
#include "PcbPP/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_WS_5D4_)
#include "PcbWS5D4/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_DV566_)
#include "PcbDV566/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_HJT_)
#include "PcbHJT/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_TPK_)
#include "PcbTPK/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_DV565_)
#include "PcbDV565/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_HD500C7_REAL_)
#include "HD500C7_real/GPIOMappingHD500C7.h"
#elif (_PCB_ == _PCB_HD500C7_TOUCH_)
#include "HD500C7_touch/GPIOMappingHD500C7.h"
#elif (_PCB_ == _PCB_HD500C6_REAL_)
#include "HD500C6_real/GPIOMappingHD500C6.h"
#elif (_PCB_ == _PCB_RPDVR_)
#include "PcbRPDVR/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_DV119_)
#include "PcbDV119/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_X1_)
#include "PcbX1/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_JXW_)
#include "PcbJXW/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_MDV200_)
#include "PcbMDV200/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_V010_)
#include "PcbMDV200/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_GTA_)
#include "PcbGTA/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_DVR_)
#include "PcbDVR/GPIOMappingPcb.h"
#elif (_PCB_ == _PCB_405_)
#include "Pcb405/GPIOMappingPcb.h"
#else
#include "PcbEVB/GPIOMappingPcb.h"
#endif

//--------------------------------------------------------------------
// Definitions for APIs
//--------------------------------------------------------------------
// Definitions for GPIOMap_EnableLED() and GPIOMap_IsLEDOn
#define GPIOMAP_LED_GREEN           0
#define GPIOMAP_LED_RED             1
#define GPIOMAP_LED_FCS             2
#define GPIOMAP_LED_WHITE           3
#define GPIOMAP_LED_ORINGE          4
#define GPIOMAP_LED_GPS         5

// Definitions for Mode Dial
#define GPIOMAP_MD_PLAYBACK         0x00000006      // Modedial PLAYBACK
#define GPIOMAP_MD_MP3              0x00000007      // Modedial MP3
#define GPIOMAP_MD_VOICE            0x00000003      // Modedial VOICE
#define GPIOMAP_MD_USB              0x0000000B      // Modedial USB
#define GPIOMAP_MD_SETUP            0x00000009      // Modedial SETUP
#define GPIOMAP_MD_MOVIE            0x0000000D      // Modedial MOVIE
#define GPIOMAP_MD_PHOTO            0x0000000F      // Modedial PHOTO
#define GPIOMAP_MD_BROWSER          0x0000000E      // Modedial BROWSER
#define GPIOMAP_MD_MASK             0x0000000F      // Modedial mask


extern void     GPIOMap_Init(void);

extern UINT32   GPIOMap_DetKey(void);
extern UINT32   GPIOMap_DetModeDial(void);

extern BOOL     GPIOMap_DetStrgCard(void);
extern BOOL     GPIOMap_DetStrgCardWP(void);

extern BOOL     GPIOMap_DetSDIO2Card(void);
extern BOOL     GPIOMap_DetSDIO2CardWP(void);

extern BOOL     GPIOMap_DetTV(void);
extern BOOL     GPIOMap_DetHDMI(void);
extern BOOL     GPIOMap_DetACPower(void);
extern BOOL     GPIOMap_DetAudio(void);
extern BOOL     GPIOMap_DetPoweroff(void);
extern BOOL     GPIOMap_DetMacro(void);
extern BOOL     GPIOMap_DetEngMode(void);

extern void     GPIOMap_TurnOnLCDBacklight(void);
extern void     GPIOMap_TurnOffLCDBacklight(void);
extern BOOL     GPIOMap_IsLCDBacklightOn(void);

extern void     GPIOMap_TurnOnLCDPower(void);
extern void     GPIOMap_TurnOffLCDPower(void);
extern BOOL     GPIOMap_IsLCDPowerOn(void);

extern BOOL     GPIOMap_DetLCDRotate(void);

extern void     GPIOMap_TurnOnCCDPower(void);
extern void     GPIOMap_TurnOffCCDPower(void);
extern BOOL     GPIOMap_IsCCDPowerOn(void);

extern void     GPIOMap_TurnOnLED(UINT32 uiLED);
extern void     GPIOMap_TurnOffLED(UINT32 uiLED);
extern BOOL     GPIOMap_IsLEDOn(UINT32 uiLED);
// GPS POWER
extern void     GPIOMap_TurnOnGPSPower(void);
extern void     GPIOMap_TurnOffGPSPower(void);
// G-SENSOR POWER
extern void     GPIOMap_TurnOnGSNPower(void);
extern void     GPIOMap_TurnOffGSNPower(void);

extern void     GPIOMap_ChargeFlash(BOOL bCharge, BOOL bHighCurrent);
extern BOOL     GPIOMap_IsFlashCharging(void);
extern BOOL     GPIOMap_IsFlashReady(void);

extern void     GPIOMap_SetPoweroffCtrlHigh(void);
extern void     GPIOMap_SetPoweroffCtrlLow(void);

extern void     GPIOMap_SetNANDWp(void);

extern void     GPIOMap_MShutOpen(void);
extern void     GPIOMap_MShutClose(void);
extern void     GPIOMap_MShutOff(void);

#endif

//@}
