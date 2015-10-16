/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanInt.h
    @ingroup    mIPRJAPKeyIO

    @brief      Internal header file of KeyScan task
                Internal function/defintions...

    @note       Nothing.

    @date       2005/11/18
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _KEYSCANINT_H
#define _KEYSCANINT_H

#include "SysCfg.h"

//-----------------------------------------------------------------------------
// KeyScan internal definitions
//-----------------------------------------------------------------------------
// Use interrupt to detect SD card insert/remove or not
#define KEYSCAN_CARDDET_INT             DISABLE

// Use for demo board that don't have these pins
#define KEYSCAN_POWEROFF_DET            ENABLE
#define KEYSCAN_AUTOPOWEROFF_DET        ENABLE
#define KEYSCAN_BATTERY_DET             DISABLE//ENABLE
#define KEYSCAN_KEY_DET                 ENABLE

// If DISABLE, keyscan task will read SD controller to get the status.
// If ENABLE, keyscan task will read GPIO pin to get the status.
#define KEYSCAN_SDWRPTDET_GPIO          ENABLE

#define KEYSCAN_CARD_REMOVED            0
#define KEYSCAN_CARD_INSERTED           1
#define KEYSCAN_CARD_UNKNOWN            0xFFFFFFFF

// SD card hot plug/unplug f/w function support
#define KEYSCAN_CARD_HOTPLUG_FUNC       ENABLE

//add for TDC
#define KEYSCAN_MACRO_OFF               0
#define KEYSCAN_MACRO_ON                1
#define KEYSCAN_MACRO_UNKNOWN           0xFFFFFFFF

#define KEYSCAN_PWROFF_INIT_STATE       0
#define KEYSCAN_PWROFF_RELEASE_STATE    1
#define KEYSCAN_PWROFF_PRESS_STATE      2

#define KEYSCAN_PWRKEY_UNKNOWN          0xFFFFFFFF
#define KEYSCAN_PWRKEY_RELEASED         0
#define KEYSCAN_PWRKEY_PRESSED          1

// Define card detect interrupt
#define KEYSCAN_CD_GPIO_INT             GPIO_INT_00

//-----------------------------------------------------------------------------
// KeyScan internal variables
//-----------------------------------------------------------------------------
extern UINT32   g_uiKeyScanTimerID;
extern UINT32   g_uiKeyScanAutoPoweroffCnt;
extern BOOL     g_bKeyScanAutoPoweroffEn;
extern BOOL     g_bKeyScanKeyDetEn;
extern BOOL     g_bKeyScanMDDetEn;
extern          UINT32  g_uiKeyScanFlags;
extern          BOOL    g_bFilesysAbortCmd;

extern UINT32 uiWhiteLedLightStatus;   //jack lan 

typedef enum
{
	LED_OFF = 0,
	LED_AUTO,		
	LED_ON
}LED_LIGHT_Sta;
//-----------------------------------------------------------------------------
// KeyScan internal APIs
//-----------------------------------------------------------------------------
// Flash light
extern void     KeyScan_DetFlash(void);

// Key
extern void     KeyScan_DetKey(void);
extern void     KeyScan_DetModedial(BOOL bForceChangeMode);

// LED
extern void     KeyScan_ToggleLED(void);
extern void     KeyScan_DetBusy(void);

// Power
extern void     KeyScan_DetACPower(void);
extern void     KeyScan_DetPoweroff(void);
extern void     KeyScan_DetBattery(void);
extern void     KeyScan_DetAutoPoweroff(void);
extern void     KeyScan_PoweroffFlow(void);

// Storage card
extern void     KeyScan_DetStrgCard(void);
extern void     KeyScan_StorageCB(UINT32 uiStatus);

// Sound
extern void     KeyScan_DetAudio(void);

// TV
extern void     KeyScan_DetTV(void);

// HDMI
extern void     KeyScan_DetHDMI(void);

// USB
extern void     KeyScan_DetUSB(void);

// LCD rotate
extern void     KeyScan_DetLCDRotate(void);

//Macro
extern void     KeyScan_DetMacro(void);

// Scaling for TV / HDMI
extern void     KeyScan_ScaleDisplay(void);

// Set Date/Time
extern void     KeyScan_SetTime(void);


extern UINT32 KeyScan_GetLedLightSta(void);

extern void KeyScan_SetLedLightSta(UINT32 Sta);

extern void KeyScan_DetAC(void);

extern void  KeyScanACCDet(void);

extern void  KeyScanACCDetPowerOn(void);

extern void  KeyScan_DetGsensor();

extern void KeyScan_DetGSPNAVI(void);

extern void KeyScan_SetDetGSPNAVIEn(BOOL En);

#endif

//@}
