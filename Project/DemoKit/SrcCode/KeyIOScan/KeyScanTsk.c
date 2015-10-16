/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTsk.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, I/O task
                Scan key, I/O task

    @note       Nothing.

    @date       2005/11/18
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "GlobalVar.h"
#include "ide.h"
#include "KeyScanTsk.h"
#include "KeySCanInt.h"
#include "Timer.h"
#include "RTC.h"
#include "PrimaryTsk.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "Debug.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "SystemInit.h"
#include "FileSysTsk.h"
#include "Ramdisk.h"
#include "NAND.h"
#include "UIMenuMovieSettings.h"
#include "Gesture_Public.h"
#include "TouchPanel.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIMenuPhotoSettings.h"
#include "NVTToolCommand.h"
#include "UIMovieObj.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#include "CalibrationTsk.h"
#endif
#include "UIMenuSetupSettings.h"
#include "UISystemStatusSettings.h"

// KeyScan timer interval 20 ms
#define KEYSCAN_TIMER_INTERVAL      20

TIMER_INFO  _sys_timer[TIMER_NUM];

// ---------------------------------------------------------------------
// Internal Global variables
// ---------------------------------------------------------------------
// Declare all global variables used in KeyScan task here.
// Don't reference these variables in your code.
UINT32      g_uiKeyScanTimerID;
UINT32      g_uiKeyScanAutoPoweroffCnt  = 0;
BOOL        g_bKeyScanAutoPoweroffEn    = TRUE;
BOOL        g_bKeyScanKeyDetEn          = TRUE;
BOOL        g_bKeyScanMDDetEn           = TRUE;
UINT32      g_uiKeyScanFlags;
BOOL        g_bKeyScanTP = TRUE;

UINT32      g_uiKeyScanLcdOffCnt  = 0;
static BOOL bKeyScanLcdOffEn  =  TRUE;
extern BOOL PowerOnMau;
extern BLINFO_OBJ gBLInfo;

static void     KeyScan_initTimer(void);
static ER       KeyScan_lockTimer(void);
static void     KeyScan_countTimer(void);
static ER       KeyScan_unlockTimer(void);
static UINT32   KeyScan_getFreeTimer(void);
static void     KeyScan_PostNvtTPEvent(GESTURE_GESTURE_STRUCT* pGesture);
static void KeyScan_wdt_setTrig(void);

#define ACCON2OFF    0X01
#define ACCOFF2ON    0X02
#define ACCUNKNOW    0XFF

extern BOOL    gUIAviIsRecording;
UINT32 TimerCounter = 0;
UINT32 TimerCounter5MinMode=0;
UINT32 gbACCStatus=ACCUNKNOW;
UINT32 gbACCStatusPre=ACCUNKNOW;

static BOOL KeyscanACCEn=FALSE;
BOOL ACC5MinMode = TRUE;
BOOL ACCHalfHourMode=TRUE;

extern BOOL bEngnorePWRPlus;
BOOL gbFirstPWR = FALSE;
BOOL gBKeyscanDetACCEn =TRUE;

extern BOOL gbMoviFileLock;

static BOOL volatile gBKeyscanDetGSPNAVIEn =TRUE;

extern INT32 Get_GSensorSensitivity(void);

/**
  KeyScan task

  KeyScan task

  @param void
  @return void
*/
void KeyScanTsk(void)
{
    UINT32 uiKeyScanTimerCnt;
    UINT32 uiKeyScanQSecCnt;

    #if (_TouchPanel_ != DISABLE)
    GESTURE_GESTURE_STRUCT  TempGesture;
    #endif

    // Initial Message Queue
    // KeyScan may send commands in the boot stage before UIFrameworkTsk starts.
    // MessageQueue has to be initialized before KeyScan running.
    Ux_InitMessage(MBXID_SYS_EVENT, MBXID_BKG_EVENT);

    // Init the callback function of Primary task for file system initialization
    Primary_SetFSInitCB(KeyScan_StorageCB);

    // Set LED toggle speed
    KeyScan_SetLEDToggleSpeed(KEYSCAN_LED_GREEN,  KEYSCAN_LED_TSPEED_SLOW);
    KeyScan_SetLEDToggleSpeed(KEYSCAN_LED_RED,    KEYSCAN_LED_TSPEED_SLOW);
    KeyScan_SetLEDToggleSpeed(KEYSCAN_LED_FCS,    KEYSCAN_LED_TSPEED_NORMAL);

    KeyScan_initTimer();

    if (timer_open((UINT *)&g_uiKeyScanTimerID, NULL) != E_OK)
    {
        debug_err(("KEYSCAN ERR: Error open keyscan timer\r\n"));
        while (1)
        {
            ;
        }
    }

    // Turn on green LED and turn off red LED
    KeyScan_TurnOnLED(KEYSCAN_LED_GREEN);
    KeyScan_TurnOffLED(KEYSCAN_LED_RED);
    KeyScan_TurnOffLED(KEYSCAN_LED_FCS);

    //Already clr_flg during timer_open procedure
    //clr_flg(FLG_ID_INT2, FLGINT2_TIMER0 << g_uiKeyScanTimerID);
    timer_set(g_uiKeyScanTimerID, KEYSCAN_TIMER_INTERVAL, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

    // Init variables
    uiKeyScanTimerCnt   = 0;
    uiKeyScanQSecCnt    = 0;

    // Init Card and File System
    KeyScan_DetStrgCard();
    KeyScan_DetStrgCard();
    //
    KeyScanACCDet();               

    #if (KEYSCAN_BATTERY_DET == ENABLE)
    if (!KeyScan_CheckUSBInSysInit())
    {
        KeyScan_DetBatteryInSysInit();
    }
    #endif

#if (_DATETIME_SETTING_ == ENABLE)
    //KeyScan_SetTime();
#endif
    #if (KEYSCAN_KEY_DET == ENABLE)
    KeyScan_DetKey();

    if (!KeyScan_CheckEngMode() && !KeyScan_CheckUSBInSysInit())
    {
        Primary_ChangeMode(PRIMARY_MODE_MOVIE);
        //Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        //Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
        Primary_Wait4ChangeModeDone();
    }
    #endif

    // Init UI USB detection
    //KeyScan_DetUSB();
    //KeyScan_DetUSB();

    // Init UI battery level
    //KeyScan_DetBattery();
    //KeyScan_DetBattery();         
    //get the acc 
    if((GPIOMap_GetAcc12V() == FALSE)&&(gbACCStatus != ACCON2OFF))
    {
        debug_err(("PowerOnMau TRUE and turn to half an hour mode-----\r\n"));
	  PowerOnMau=TRUE;
    }
    //---------------------------------------------------------------------------------------------------
    // KeyScan Main Loop
    //---------------------------------------------------------------------------------------------------
    while (1)
    {
        //wai_flg((UINT *)&uiFlag, FLG_ID_INT2, FLGINT2_TIMER0 << g_uiKeyScanTimerID, TWF_ORW | TWF_CLR);
        timer_waitTimeup(g_uiKeyScanTimerID);
        uiKeyScanTimerCnt++;

        g_uiKeyScanFlags = 0;

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 20ms
        //-----------------------------------------------------------------------------------------------
        // Detect key
        #if (KEYSCAN_KEY_DET == ENABLE)
        if (g_bKeyScanKeyDetEn == TRUE)
        {
            KeyScan_DetKey();
        }
        #endif
        KeyScanACCDet();               

        // Detect IR Remote key
        #if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
        // Check IR data expiration, frequency value must be smaller than 100ms
        KeyScan_CheckIrRxExp();
        // Detect IR remote control receiver detected key, frequency value must be smaller than 50ms
        KeyScan_DetIrRxKey();
        #endif
        if(Ux_GetBackgroundTskBusy() == TRUE)
        {
            // Detect storage card,prevent card unplug
            KeyScan_DetStrgCard();
        }

        KeyScan_countTimer();

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 80ms
        //-----------------------------------------------------------------------------------------------
        if ((uiKeyScanTimerCnt & 3) == 0)
        {
            // Detect touch panel
            #if (_TouchPanel_ != DISABLE)
            if(g_bKeyScanTP)
            {
                Gesture_DetGesture(&TempGesture);
                if(TempGesture.Event != GESTURE_EVENT_IDLE)
                {
                    KeyScan_PostNvtTPEvent(&TempGesture);
                }
            }
            #endif

            // Toggle LED
            KeyScan_ToggleLED();
            // Detect flash light
            #if (_FLASHLIGHT_ != _FLASHLIGHT_NONE_)
            KeyScan_DetFlash();
            #endif

        }

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 100ms
        //-----------------------------------------------------------------------------------------------
        if ((uiKeyScanTimerCnt % 5) == 0)
        {
        
            #if (KEYSCAN_POWEROFF_DET == ENABLE)
            // Detect power off key
                KeyScan_DetPoweroff();
            #endif
			
            //-------------------------------------------------------------------------------------------
            // Do the following task every 1s
            //-------------------------------------------------------------------------------------------
            if ((uiKeyScanTimerCnt % 50) == 0)
            {
                #if (KEYSCAN_AUTOPOWEROFF_DET == ENABLE)
                // Detect auto poweroff
                if ((KeyScan_GetAutoPoweroffTime() != KEYSCAN_AUTOPOWEROFF_DISABLED) && (g_bKeyScanAutoPoweroffEn == TRUE))
                {
                    KeyScan_DetAutoPoweroff();
                }
                #endif

                //#if (_WDT_CONTROL_ == ENABLE)
                //wdt_setTrig();
                //#endif
            }
        }

        //-----------------------------------------------------------------------------------------------
        // Do the following task every 250ms
        //-----------------------------------------------------------------------------------------------
        if ((uiKeyScanTimerCnt % 12) == 0)
        {
            uiKeyScanQSecCnt++;

            #if (KEYSCAN_KEY_DET == ENABLE)
            // Detect modedial
            //if (g_bKeyScanMDDetEn == TRUE)
            //{
            //    KeyScan_DetModedial(FALSE);
            //}
            #endif

            // Detect flash light
            //KeyScan_DetFlash();

            // Detect USB
            //KeyScan_DetUSB();

            //Detect macro mode
            KeyScan_DetMacro();

	      	//Detect Gsensor
	     	KeyScan_DetGsensor();
			
            //-------------------------------------------------------------------------------------------
            // Do the following task every 500ms
            //-------------------------------------------------------------------------------------------
            if (uiKeyScanQSecCnt & 1)
            {
                #if (KEYSCAN_CARDDET_INT == DISABLE)
                // Detect storage card
                KeyScan_DetStrgCard();
                #endif

                // Detect HDMI
                KeyScan_DetHDMI();

                // Detect TV
                KeyScan_DetTV();

                // Detect External Audio (headphone...)
                //KeyScan_DetAudio();

                // Detect LCD rotate
                //KeyScan_DetLCDRotate();

                // Detect system is busy or not
                KeyScan_DetBusy();

				#if (_WDT_CONTROL_ == ENABLE)
                wdt_setTrig();
			     //debug_msg("magic_watch_dog\r\n");
            #endif

            }

            //-------------------------------------------------------------------------------------------
            // Do the following task every 1s
            //-------------------------------------------------------------------------------------------
            if ((uiKeyScanQSecCnt % 4) == 0)
            {
                #if (KEYSCAN_BATTERY_DET == ENABLE)
                //if (!KeyScan_IsUSBPlug()) {
                    //KeyScan_DetBattery();
               // }
                #endif
			
//--------------------------------jack lan begin-------------------------------	
                if ((KeyScan_GetAutoLCDoffTime() != 0))
                {
		  		KeyScan_DeLcdOff();
                }					

               //KeyScan_DetAC();		
		if(gBKeyscanDetGSPNAVIEn == TRUE)
		{
		   //KeyScan_DetGSPNAVI();		
		}
#if 0				   
		  if(KeyScan_GetLedLightSta()== LED_AUTO)
		  {
                if (AE__GetLV()>50)
                {
                		if(GPIOMap_IsWhiteLightOn())
                			{
				GPIOMap_TurnOffWhiteLight();
                			}
                } 
		   else 
                {                
                		if(!GPIOMap_IsWhiteLightOn())
                			{
				GPIOMap_TurnOnWhiteLight();	        
                			}
                }				
		  }
#endif		  
//--------------------------------jack lan end-------------------------------	

            }
			#if (_WDT_CONTROL_ == ENABLE)
			    KeyScan_wdt_setTrig();
			     //debug_msg("magic_watch_dog1\r\n");
            #endif
            //-------------------------------------------------------------------------------------------
            // Do the following task every 5s
            //-------------------------------------------------------------------------------------------			
            if ((uiKeyScanQSecCnt % 20) == 0)			
		{
			KeyScan_NAVIComunicate();
		}
        }
        //-----------------------------------------------------------------------------------------------
        // End of KeyScanTimer divider
        //-----------------------------------------------------------------------------------------------

        //-----------------------------------------------------------------------------------------------
        // Start to set key flag
        //-----------------------------------------------------------------------------------------------
        if (g_uiKeyScanFlags != 0)
        {
#if (_CALIBRATION_MODE_ == ENABLE)
            set_flg(FLG_ID_KEY, g_uiKeyScanFlags);
#endif
            switch(g_uiKeyScanFlags & FLGKEY_ACT_MASK)
            {
                case FLGKEY_PRESSED:
                    Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 2, NVTEVT_KEY_PRESS, FLGKEY_PRESSED);
                break;

                case FLGKEY_RELEASED:
                    Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 1, NVTEVT_KEY_RELEASE);
                break;

                case FLGKEY_HOLD:
                    Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 2, NVTEVT_KEY_PRESS, FLGKEY_HOLD);
                break;

                case FLGKEY_LONGHOLD:
                    Ux_PostEvent(Get_NVTCMD(g_uiKeyScanFlags), 2, NVTEVT_KEY_PRESS, FLGKEY_LONGHOLD);
                break;

                default:
                    debug_err(("KeyScanTsk: Unknown key action (0x%.8X)\r\n", (g_uiKeyScanFlags & FLGKEY_ACT_MASK)));
                break;
            }
            g_uiKeyScanFlags = 0;
        }
    }
}

void KeyScan_wdt_setTrig(void)
{
	if(gpio_getPin(C_GPIO_54))
	{
		
		gpio_clearPin(C_GPIO_54);
	}
	else
	{
		gpio_setPin(C_GPIO_54);	
	}
}
UINT32 Get_NVTCMD(UINT32 keys)
{
    if(keys & FLGKEY_MODE)
    {
        return NVTEVT_KEY_MODE;
    }
    if(keys & FLGKEY_MENU)
    {
        return NVTEVT_KEY_MENU;
    }
    if(keys & FLGKEY_LEFT)
    {
        return NVTEVT_KEY_LEFT;
    }
    if(keys & FLGKEY_RIGHT)
    {
        return NVTEVT_KEY_RIGHT;
    }
    if(keys & FLGKEY_ENTER)
    {
        return NVTEVT_KEY_ENTER;
    }
    if(keys & FLGKEY_UP)
    {
        return NVTEVT_KEY_UP;
    }
    if(keys & FLGKEY_DOWN)
    {
        return NVTEVT_KEY_DOWN;
    }
    if(keys & FLGKEY_ZOOMOUT)
    {
        return NVTEVT_KEY_ZOOMOUT;
    }
    if(keys & FLGKEY_ZOOMIN)
    {
        return NVTEVT_KEY_ZOOMIN;
    }
    if(keys & FLGKEY_DEL)
    {
        return NVTEVT_KEY_DEL;
    }
    if(keys & FLGKEY_PLAYBACK)
    {
        return NVTEVT_KEY_PLAYBACK;
    }
    if(keys & FLGKEY_SHUTTER1)
    {
        return NVTEVT_KEY_SHUTTER1;
    }
    if(keys & FLGKEY_SHUTTER2)
    {
        return NVTEVT_KEY_SHUTTER2;
    }
    else
    {
        debug_err(("Get_NVTCMD: Unknown key (0x%.8X)\r\n", keys));
        return NVTEVT_NULL;
    }
}

/**
  Get_KeyEvt

  Transfer NVTEVT to key event --> only physical key map

  @param NVTEVT
  @return Key event
*/
UINT32 Get_KeyEvt(NVTEVT evt)
{
    UINT32 result = 0;

    switch(evt)
    {
        case NVTEVT_KEY_UP:
            result = FLGKEY_UP;
            break;

        case NVTEVT_KEY_DOWN:
            result = FLGKEY_DOWN;
            break;

        case NVTEVT_KEY_LEFT:
            result = FLGKEY_LEFT;
            break;

        case NVTEVT_KEY_RIGHT:
            result = FLGKEY_RIGHT;
            break;

        case NVTEVT_KEY_ENTER:
            result = FLGKEY_ENTER;
            break;

        case NVTEVT_KEY_MENU:
            result = FLGKEY_MENU;
            break;

        case NVTEVT_KEY_MODE:
            result = FLGKEY_MODE;
            break;

        case NVTEVT_KEY_ZOOMOUT:
            result = FLGKEY_ZOOMOUT;
            break;

        case NVTEVT_KEY_ZOOMIN:
            result = FLGKEY_ZOOMIN;
            break;

        case NVTEVT_KEY_SHUTTER1:
            result = FLGKEY_SHUTTER1;
            break;

        case NVTEVT_KEY_SHUTTER2:
            result = FLGKEY_SHUTTER2;
            break;

        case NVTEVT_KEY_DEL:
            result = FLGKEY_DEL;
            break;

        case NVTEVT_KEY_PLAYBACK:
            result = FLGKEY_PLAYBACK;
            break;

        default:
            break;
    }
    return result;
}

/**
  KeyScan_initTimer

  Start a timer control array

  @param void
  @return void
*/
static void KeyScan_initTimer(void)
{
    UINT32   i;
    for(i = 0; i < TIMER_NUM; i++)
    {
        _sys_timer[i].counter = 0;
        _sys_timer[i].interval = 0;
        _sys_timer[i].eventID = NVTEVT_NULL;
        _sys_timer[i].isEnabled = FALSE;
        _sys_timer[i].type = ONE_SHOT;
    }
}

/**
  KeyScan_getFreeTimer

  Get a free timer of timer control array

  @param void
  @return void
*/
static UINT32 KeyScan_getFreeTimer(void)
{
    UINT32   i;
    for(i = 0; i < TIMER_NUM; i++)
    {
        if(_sys_timer[i].isEnabled == FALSE)
            return i;
    }
    return NULL_TIMER;
}

/**
  KeyScan_lockTimer

  Get a sem to lock the resource

  @param void
  @return void
*/
static ER KeyScan_lockTimer(void)
{
    ER erReturn;
    erReturn = wai_sem(SEMID_KNLTIMER);
    if (erReturn != E_OK)
    {
        return erReturn;
    }
    return E_OK;
}

/**
  KeyScan_unlockTimer

  ulock the resource

  @param void
  @return void
*/
static ER KeyScan_unlockTimer(void)
{
    return sig_sem(SEMID_KNLTIMER);
}

/**
  KeyScan_countTimer

  Count the timer ...

  @param void
  @return void
*/
static void KeyScan_countTimer(void)
{
    UINT32      i;

    KeyScan_lockTimer();
    for(i = 0; i < TIMER_NUM; i++)
    {
        if(_sys_timer[i].isEnabled == TRUE)
        {
            _sys_timer[i].counter ++;
            if((_sys_timer[i].counter * KEYSCAN_TIMER_INTERVAL) ==  _sys_timer[i].interval)
            {
                Ux_PostEvent(NVTEVT_TIMER, 1, _sys_timer[i].eventID);
                _sys_timer[i].counter = 0;

                if(_sys_timer[i].type == ONE_SHOT)
                {
                    _sys_timer[i].eventID = NVTEVT_NULL;
                    //_sys_timer[i].isBusy = FALSE;
                    _sys_timer[i].isEnabled = FALSE;
                }
            }
        }
    }
    KeyScan_unlockTimer();
}
/**
  KNLstartTimer

  @param void
  @return timer ID
*/
UINT32  KeyScan_startTimer(UINT32 interval, UINT32 eventID, TIMER_TYPE type)
{
    UINT32 freeTimer = NULL_TIMER;

    KeyScan_lockTimer();
    freeTimer = KeyScan_getFreeTimer();

    if(freeTimer != NULL_TIMER)
    {
        _sys_timer[freeTimer].eventID = eventID;
        _sys_timer[freeTimer].interval = interval;
        _sys_timer[freeTimer].isEnabled = TRUE;
        _sys_timer[freeTimer].type = type;
        _sys_timer[freeTimer].counter = 0;
    }
    KeyScan_unlockTimer();

    return freeTimer;
}

/**
  KNLstopTimer

  @param timerID
  @return timer ID
*/
void KeyScan_stopTimer(UINT32 * timerID)
{
    if((!timerID)||((* timerID)>TIMER_NUM))
    {
        debug_err(("^R stopTimer id error\r\n"));
        return ;
    }
    KeyScan_lockTimer();

    _sys_timer[*timerID].counter = 0;
    _sys_timer[*timerID].interval = 0;
    _sys_timer[*timerID].eventID = NVTEVT_NULL;
    _sys_timer[*timerID].isEnabled = FALSE;
    _sys_timer[*timerID].type = ONE_SHOT;
    KeyScan_unlockTimer();

    *timerID = NULL_TIMER;
}

static char *KeyScan_GetTPEventString(GESTURE_EVENT_ENUM event)
{
    switch (event)
    {
    case GESTURE_EVENT_PRESS:
        return "PRESS";

    case GESTURE_EVENT_RELEASE:
        return "RELEASE";

    case GESTURE_EVENT_MOVE:
        return "MOVE";

    case GESTURE_EVENT_HOLD:
        return "HOLD";

    case GESTURE_EVENT_CLICK:
        return "CLICK";

    case GESTURE_EVENT_DOUBLE_CLICK:
        return "DOUBLE_CLICK";

    case GESTURE_EVENT_SLIDE_UP:
        return "SLIDE_UP";

    case GESTURE_EVENT_SLIDE_DOWN:
        return "SLIDE_DOWN";

    case GESTURE_EVENT_SLIDE_LEFT:
        return "SLIDE_LEFT";

    case GESTURE_EVENT_SLIDE_RIGHT:
        return "SLIDE_RIGHT";

    default:
    case GESTURE_EVENT_IDLE:
        return "IDLE";
    }
}

static void KeyScan_PostNvtTPEvent(GESTURE_GESTURE_STRUCT* pGesture)
{
    CONTROL_TP_VENT TempTPEvent;

    //debug_msg("TP:Event=%d, X=%03d, Y=%03d\r\n",pGesture->Event,pGesture->PosX,pGesture->PosY);
    //debug_msg("TP:Event=%s, X=%03d, Y=%03d\r\n",KeyScan_GetTPEventString(pGesture->Event),pGesture->PosX,pGesture->PosY);

    switch(pGesture->Event)
    {
        case GESTURE_EVENT_PRESS:
        {
            TempTPEvent = NVTEVT_PRESS;
            break;
        }

        case GESTURE_EVENT_RELEASE:
        {
            TempTPEvent = NVTEVT_RELEASE;
            break;
        }

        case GESTURE_EVENT_MOVE:
        {
            TempTPEvent = NVTEVT_MOVE;
            break;
        }

        case GESTURE_EVENT_CLICK:
        {
            TempTPEvent = NVTEVT_CLICK;
            break;
        }

        case GESTURE_EVENT_DOUBLE_CLICK:
        {
            TempTPEvent = NVTEVT_DOUBLECLICK;
            break;
        }

        case GESTURE_EVENT_SLIDE_UP:
        {
            TempTPEvent = NVTEVT_SLIDE_UP;
            break;
        }

        case GESTURE_EVENT_SLIDE_DOWN:
        {
            TempTPEvent = NVTEVT_SLIDE_DOWN;
            break;
        }

        case GESTURE_EVENT_SLIDE_LEFT:
        {
            TempTPEvent = NVTEVT_SLIDE_LEFT;
            break;
        }

        case GESTURE_EVENT_SLIDE_RIGHT:
        {
            TempTPEvent = NVTEVT_SLIDE_RIGHT;
            break;
        }

        default:
        {
            debug_err(("Gesture %d is not handled!",pGesture->Event));
            return;
        }
    }

    Ux_PostEvent(TempTPEvent, 2, pGesture->PosX, pGesture->PosY);
}

void KeyScan_SetDetTP(BOOL IsEnable)
{
    g_bKeyScanTP = IsEnable;
}
UINT32  KeyScan_DeLcdOff(void)
{
    if(Get_LcdOffTime() == 0)
   return;

   //if(KeyScan_IsUSBPlug())
    //return;

   if(bKeyScanLcdOffEn == FALSE)
    return ;
   
    g_uiKeyScanLcdOffCnt++;

    if (g_uiKeyScanLcdOffCnt > Get_LcdOffTime())
    {
        if(GPIOMap_IsLCDBacklightOn())
        {
		GPIOMap_TurnOffLCDBacklight();
	  }
	g_uiKeyScanLcdOffCnt = 0;
    }
}

 UINT32  KeyScan_EnableDeLcdOff(BOOL En)
 {
	bKeyScanLcdOffEn = En;
 }


void KeyScan_NAVIComunicate(void)
{
	UINT32 i = 0;
       UINT8 data[7];	 
	static UINT32 counter =0;
	
	data[0] = 0X01;
	data[1] = 0XDD;
	data[2] = 0X55;
	data[3] = counter;
	data[4] = 0X01;
	data[5] =0X00;
	data[6] = ((data[0]+data[1]+data[2]+data[3]
		+data[4]+data[5])&0xff);

	counter++;
      //debug_err(("data :"));
	
	for(i=0;i<7;i++)
	{
		uart2_putChar(data[i]);
		//debug_msg("0x%02x ",data[i]);
	}
      debug_err(("\r\n"));
	
}
void KeyScan_SetDetGSPNAVIEn(BOOL En)
{
	gBKeyscanDetGSPNAVIEn=En;
}
void KeyScanACCDet(void)
{
    static UINT32   uiACC12VPreSts = 0xFFFFFFFF;
    UINT32          uiACC12VCurSts;

    if(gBKeyscanDetACCEn==FALSE)
	return;
    if(GPIOMap_GetAcc12V()==TRUE)
    {
        uiACC12VCurSts =  ACC12V_PLUGIN;
    }
    else
    {
        uiACC12VCurSts =  ACC12V_UNPLUG;
		KeyScan_PoweroffFlow();
    }

    //debug_err(("uiACC12VPreSts %x uiACC12VCurSts %x  \r\n", uiACC12VPreSts, uiACC12VCurSts));

    if(uiACC12VPreSts != uiACC12VCurSts)
    {
            if(uiACC12VCurSts ==  ACC12V_PLUGIN)
            {
    			//debug_err((" ------11---ACC ON--- \r\n"));	            
                   if(gbACCStatusPre == ACCON2OFF)
			{
    				debug_err((" ------11---ACCOFF2ON gbACCStatus %x  \r\n", gbACCStatus));						
				gbACCStatus=ACCOFF2ON;
				ACC5MinMode=FALSE;
				if(bEngnorePWRPlus == TRUE)
				{
    				      debug_err((" ------11---gbFirstPWR :TRUE--- \r\n"));	     				
					gbFirstPWR = TRUE;
				}			
                   	}
	 	      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
	 	      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);				   
			gbACCStatusPre=ACCOFF2ON;
	      }
	     else
	     {	
    				//debug_err((" ------22--->ACC OFF---%s--- \r\n",gBLInfo.strStorageInt));	             	     
                   if(gbACCStatusPre == ACCOFF2ON)
                   	{
    				debug_err(("-------22--- ACCON2OFF gbACCStatus %x  \r\n", gbACCStatus));                   	
	             	gbACCStatus = ACCON2OFF;	
				PowerOnMau=FALSE;		
				ACC5MinMode=TRUE;				
 			      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
 			      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);				
                   	}				   
			else if(!strncmp(gBLInfo.strStorageInt,"ACCPOWERON", 10))//(strcmp((char *)&pBLInfo->strStorageInt,"ACCPOWERON")==0)				
                   	{
    				debug_err(("-------22--- ACC PowerOn...  \r\n"));                   	
	             	gbACCStatus = ACCON2OFF;	
				PowerOnMau=FALSE;		
				ACC5MinMode=TRUE;				
 			      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
 			      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);				
                   	}
			gbACCStatusPre=ACCON2OFF;
			bEngnorePWRPlus = TRUE;
	     }
            uiACC12VPreSts = uiACC12VCurSts;			     
		 
     }
}


void KeyScanACCDetPowerOn(void)
{}


void KeyScan_DetGsensor()
{
    static UINT32 Counter;
    Gsensor_Data GS_Data;

    INT32 gsensor_trig;
    static UINT32 X,Y,Z;
    static BOOL first_powron=TRUE;
	
    if(Get_MovieGsensorIndex()!=MOVIE_GSENSOR_OFF)
    {
    if(gsensor_gettype()==GSENSOR_DMARD07)
    {
	    Counter++;
	    if(Counter<12)
	    {
		 return;
	    }
	    if (gpio_getPin(GPIO_GSENSOR_INT) == 1)
	    {       
	       Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_PRESS);                           
		 debug_err(("DMARD07 TRIG...\r\n"));
	        gsensor_ReadSensitivity();			
	    }	
    }
    else if(gsensor_gettype()==GSENSOR_GMA301)
    {
          //Counter++;
	    //if(Counter%2==0)
	    {
#if 0// interrupte type	    
		    if (gpio_getPin(GPIO_GSENSOR_INT) == 0)
		    {       
			  debug_err(("GMA301 TRIG....\r\n"));
		        gsensor_ReadSensitivity();	
		       // if(gbMoviFileLock == FALSE)
		       // {
		           //  gbMoviFileLock=TRUE;
		        	Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_PRESS);                           
		       // }
		    }	
#else
			gsensor_GetData(&GS_Data);
			if(first_powron ==FALSE)
			{
	            		gsensor_trig=Get_GSensorSensitivity();
	                   if((abs(GS_Data.Axis.Xacc-X)>gsensor_trig)||(abs(GS_Data.Axis.Yacc-Y)>gsensor_trig)
					||(abs(GS_Data.Axis.Zacc-Z)>gsensor_trig))
	                   	{
	            			debug_msg("gsensor trig TRUE ...%d,%d,%d, threshold:%d\r\n",GS_Data.Axis.Xacc,GS_Data.Axis.Yacc,GS_Data.Axis.Zacc,gsensor_trig);
				    	//if(gbMoviFileLock == FALSE)
				    	//{
				         //	gbMoviFileLock=TRUE;
				    		Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_PRESS);                           
				    	//}	
	                   	}
				X=GS_Data.Axis.Xacc;
				Y=GS_Data.Axis.Yacc;
				Z=GS_Data.Axis.Zacc;		
			}
			else
			{
				first_powron=FALSE;
				X=GS_Data.Axis.Xacc;
				Y=GS_Data.Axis.Yacc;
				Z=GS_Data.Axis.Zacc;		
			}
		            
#endif
	    }
    }
	else if(gsensor_gettype()==GSENSOR_DA380)
	{
		gsensor_GetData(&GS_Data);
		if(first_powron ==FALSE)
		{
	    	gsensor_trig=Get_GSensorSensitivity();
			//debug_err(("gsensor  ...%d,%d,%d,%d\r\n",abs(GS_Data.Axis.Xacc-X),abs(GS_Data.Axis.Yacc-Y),abs(GS_Data.Axis.Zacc-Z),gsensor_trig));
	        if((abs(GS_Data.Axis.Xacc-X)>gsensor_trig)||(abs(GS_Data.Axis.Yacc-Y)>gsensor_trig)
					||(abs(GS_Data.Axis.Zacc-Z)>gsensor_trig))
	        {
	        	debug_msg("gsensor trig TRUE ...%d,%d,%d, threshold:%d\r\n",GS_Data.Axis.Xacc,GS_Data.Axis.Yacc,GS_Data.Axis.Zacc,gsensor_trig);
		
			    Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_PRESS);                           
					
	        }
				X=GS_Data.Axis.Xacc;
				Y=GS_Data.Axis.Yacc;
				Z=GS_Data.Axis.Zacc;		
		}
		else
		{
			first_powron=FALSE;
			X=GS_Data.Axis.Xacc;
			Y=GS_Data.Axis.Yacc;
			Z=GS_Data.Axis.Zacc;		
			}
	}
    else
    {}
    }
}

void KeyScan_DetGSPNAVI(void)
{

      static UINT32 Counter=0;
	//if(GPSDataInputFlag==FALSE)
	//{
		//Counter++;
		//if(Counter>5)
	     // {
	    	     //debug_err(("KeyScan_DetGSPNAVI ...\r\n" ));				     
			if(PowerOnMau == TRUE)
			{
		 	      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
		 	      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);
						  						
	    			//debug_err(("Half Hour Power off mode  ...\r\n" ));						
			       if(ACCHalfHourMode==TRUE){
				TimerCounter++;
	    			//debug_err(("Half Hour Mode TimerCounter--11-- %d ...\r\n", TimerCounter));			
				if(TimerCounter>1800)
				{
					KeyScan_PoweroffFlow();
				}
			     }
			   Counter=0;
			}			
			else if((gbACCStatus == ACCON2OFF))
			{
	    			//debug_err(("TimerCounter5MinMode ...\r\n"));			
			      if(ACC5MinMode == TRUE){
				if(GPIOMap_IsLCDBacklightOn()==TRUE)
				{
					GPIOMap_TurnOffLCDBacklight();
				}			
				TimerCounter5MinMode++;
	    			//debug_err(("TimerCounter5MinMode--11-- %d ...\r\n", TimerCounter5MinMode ));			
	
				if(TimerCounter5MinMode>300)
				{
					KeyScan_PoweroffFlow();
				}				
	 	      			KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
	 	      			KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);					
			      	}
				Counter=0;				  
			}		
			else if((gbACCStatus == ACCOFF2ON))
			{
	    			//debug_err(("Normal mode--22-- %d ...\r\n", TimerCounter));
				PowerOnMau=FALSE;					
				if(GPIOMap_IsLCDBacklightOn()==FALSE)
				{
					if(GPIOMap_DetTV()==FALSE){
						if((bKeyScanLcdOffEn==TRUE)&&(KeyScan_GetAutoLCDoffTime()!=0))
						{
							//GPIOMap_TurnOnLCDBacklight();
						}
						else
						{
							GPIOMap_TurnOnLCDBacklight();
						}		
					}
				}	
				TimerCounter5MinMode=0;
				TimerCounter=0;
				Counter=0;
			}			
			else//ACC OFF TO ON
			{
	    			//debug_err(("Normal mode-- %d ...\r\n" ,TimerCounter));
	 	      		KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
	 	      		KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);		    			
				if(GPIOMap_IsLCDBacklightOn()==FALSE)
				{  
				      Counter++;
					if(Counter>3){
					Counter=0;
						if(GPIOMap_DetTV()==FALSE)
						{
							    //debug_err(("Normal mode,g_uiKeyScanLcdOffCnt %d ...%d...\r\n" ,g_uiKeyScanLcdOffCnt,bKeyScanLcdOffEn));

							if((bKeyScanLcdOffEn==TRUE)&&(KeyScan_GetAutoLCDoffTime()!=0))
							{
								//GPIOMap_TurnOnLCDBacklight();
							}
							else
							{
								GPIOMap_TurnOnLCDBacklight();
							}						
						}
					}
				}	
				TimerCounter5MinMode=0;
			}
	//	}
	//}
	//else
	//{
 	    //  KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
 	  //    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_DEFAULT);	
	//	Counter=0;
	//	if(GPIOMap_IsLCDBacklightOn()==FALSE)
	//	{
	//		if(GPIOMap_DetTV()==FALSE)
	//		GPIOMap_TurnOnLCDBacklight();
	//	}			
//	}
}

//@}
