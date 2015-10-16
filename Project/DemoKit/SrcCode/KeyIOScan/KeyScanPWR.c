/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanPWR.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan power off key and do power off, voltage detection
                Scan power off key and do power off, voltage detection

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "KeyScanTsk.h"
#include "KeyScanInt.h"
#include "PhotoTsk.h"
#include "FilesysTsk.h"
#include "GPIOMapping.h"
#include "RTC.h"
#include "Timer.h"
#include "AppInit.h"
#include "GlobalVar.h"
#include "VoltageDet.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UISystemStatusSettings.h"
#include "UIGraphics.h"
#include "SystemInit.h"
#include "BG_Images.h"
#include "GPIOMapping.h"
#include "top.h"
#include "Utility.h"
#include "PrimaryTsk.h"
#include "LCDTV.h"
#include "PlaySoundTsk.h"
#include "PlaySoundInt.h"
#include "ide.h"
#include "UICustomer.h"
#include "UIMenuMovieSettings.h"
#include "Adc.h"

static volatile BOOL     bACPower               = FALSE;
static volatile BOOL     bDoPoweroff            = FALSE;
static volatile UINT32   uiAutoPoweroffTime     = KEYSCAN_AUTOPOWEROFF_DISABLED;
static volatile UINT32   uiAutoLCDoffTime     = KEYSCAN_AUTOPOWEROFF_DISABLED;

static volatile UINT32   uiBatteryLvl           = VOLDET_BATTERY_LVL_3;

UINT32 uiWhiteLedLightStatus = LED_OFF;

static BOOL     bACStatus       = FALSE;
static BOOL     bLastACDet      = FALSE;
static BOOL     bLastACStatus   = FALSE;
extern BOOL gUIAviIsRecording;
BOOL bEngnorePWRPlus = FALSE;
extern UINT32 gbACCStatus;
extern UINT32 TimerCounter5MinMode;
extern BOOL ACC5MinMode;
extern BOOL PowerOnMau;
extern BOOL ACCHalfHourMode;
extern BOOL gbFirstPWR;
extern BOOL gBKeyscanDetACCEn;

static volatile UINT32  guiKeyscanPowerDelayTimerID;
BOOL guiKeyscanPowerDelay=FALSE;
extern UINT32      g_uiKeyScanLcdOffCnt;


#define KEY_ADC_TV         867 //UP KEY
#define KEY_ADC_ERR                 20
/**
  Detect AC power is plugging in or unplugged

  Detect AC power is plugging in or unplugged.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetACPower(void)
{
    bACPower = FALSE;
}

/**
  Detect power off key is pressed or not.

  Detect power off key is pressed or not. If power off key is changing state
  in the following sequence RELEASED -> PRESSED -> RELEASED, then do the
  power off flow.
  [KeyScan internal API]

  @param void
  @return void
*/
#define KEYSCAN_POWEROFF_COUNT  10 // 1 sec in unit of 100 ms
void KeyScan_DetPoweroff(void)
{
    static UINT32 uiPowerKey    = KEYSCAN_PWRKEY_UNKNOWN;
    static UINT32 uiPWRState    = KEYSCAN_PWROFF_INIT_STATE;
    static UINT32 uiPowerKeyPressCount = 0;

    static UINT32 uiModeKeyPressCount = 0;	
    static UINT32 uiModeKey    = KEYSCAN_PWRKEY_UNKNOWN;
    static UINT32 uiModeState    = KEYSCAN_PWROFF_INIT_STATE;	

    UINT32 ad1 = 0;
	
    // Power off request from another task
    if (bDoPoweroff == TRUE)
    {
        KeyScan_PoweroffFlow();
    }
	
   ad1 = adc_readData(ADC_CHANNEL_1);

#if 1//(OSD_DUMP == OSD_DUMP_ENABLE) // using POWER key to dump OSD screen
    // Detect power off key
    if (rtc_getPWRStatus())
    {
        // Reset shutdown timer
        rtc_resetShutdownTimer();

        // Debounce
        if (uiPowerKey == KEYSCAN_PWRKEY_PRESSED)
        {
            if (uiPWRState == KEYSCAN_PWROFF_RELEASE_STATE)
            {
                uiPowerKeyPressCount++;
                if (uiPowerKeyPressCount > KEYSCAN_POWEROFF_COUNT)
                {
                    uiPWRState = KEYSCAN_PWROFF_PRESS_STATE;
                }
            }

            if(uiPWRState == KEYSCAN_PWROFF_PRESS_STATE)
            {
                KeyScan_PoweroffFlow();
            }
        }
        else
        {
            uiPowerKey = KEYSCAN_PWRKEY_PRESSED;
        }
    }
    else
    {
        // Debounce
        if (uiPowerKey == KEYSCAN_PWRKEY_RELEASED)
        {
            if (uiPWRState == KEYSCAN_PWROFF_INIT_STATE)
            {
                uiPWRState = KEYSCAN_PWROFF_RELEASE_STATE;
            }

            if (uiPowerKeyPressCount)
            {
                uiPowerKeyPressCount = 0;


		//----------change to half hour mode from 5mins ide mode  begin--------------------
			 //TimerCounter5MinMode=0;
			 if(ACC5MinMode == TRUE)
			 {
				ACC5MinMode=FALSE;
		 		PowerOnMau=TRUE;
		 		ACCHalfHourMode=TRUE;
		 	      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
		 	      KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_DEFAULT);		
			 }		
		//----------change to half hour mode from 5mins ide mode  end--------------------	 
		//----------trig lcd backlight   begin--------------------
		
		        if((!KeyScan_IsHDMIPlugIn())&&(!KeyScan_IsTVPlugIn()))
		        {
		              if((Primary_GetCurrentMode()!= PRIMARY_MODE_USBMSDC)&&(Primary_GetCurrentMode()!= PRIMARY_MODE_USBPCC))
		              {
			        if(!GPIOMap_IsLCDBacklightOn())
			        {
					GPIOMap_TurnOnLCDBacklight();
					g_uiKeyScanLcdOffCnt = 0;

					return;
				  }
		              }
		        }
		//----------trig lcd backlight   end --------------------		

		//----------engnore PWR Plus  begin--------------------
    	       debug_err((" ---keyscanpwr---gbFirstPWR :%d--- \r\n",gbFirstPWR));	  		
			if(gUIAviIsRecording == TRUE){	
			   if(gbFirstPWR == TRUE)
			   {
				gbFirstPWR = FALSE;		      
				return;
			   }			  
		//----------engnore PWR Plus  end--------------------

		//----------change audio item  begin--------------------
		         switch(Get_MovieAudioIndex())
			    {
			        case MOVIE_AUDIO_OFF:
			            aud_setGain(AUDIO_GAIN_6);			
			            Set_MovieAudioIndex(MOVIE_AUDIO_ON);
			            break;
			        case MOVIE_AUDIO_ON:
			            aud_setGain(AUDIO_GAIN_MUTE);
			            Set_MovieAudioIndex(MOVIE_AUDIO_OFF);			
			            break;
			    }		
			   Ux_PostEvent(NVTEVT_OSD_REFLASH, 0);
            	}
		//----------change audio item  end--------------------	   

		}
	             
        }
        else
        {
            uiPowerKey = KEYSCAN_PWRKEY_RELEASED;
        }
    }

///-------------TV OUT MANUAL ----------------------------

if((ad1 > KEY_ADC_TV - KEY_ADC_ERR) && (ad1 < KEY_ADC_TV + KEY_ADC_ERR))
{
        // Debounce
        if (uiModeKey == KEYSCAN_PWRKEY_PRESSED)
        {
            if (uiModeState == KEYSCAN_PWROFF_RELEASE_STATE)
            {
                uiModeKeyPressCount++;
		   debug_err(("uiModeKeyPressCount : %d...\r\n",uiModeKeyPressCount));
                if (uiModeKeyPressCount > 15)
                {
                    uiModeState = KEYSCAN_PWROFF_PRESS_STATE;
                }
            }

            if (uiModeState == KEYSCAN_PWROFF_PRESS_STATE)
            {
                  uiModeKeyPressCount = 0;
			uiModeKey    = KEYSCAN_PWRKEY_UNKNOWN;
			uiModeState    = KEYSCAN_PWROFF_INIT_STATE;				  
                KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);
	        if(!KeyScan_IsHDMIPlugIn()&&(!KeyScan_IsTVPlugIn()))
	        {
		        if(!GPIOMap_IsLCDBacklightOn())
		        {
				GPIOMap_TurnOnLCDBacklight();
			       g_uiKeyScanAutoPoweroffCnt = 0;
				  return;
			}
	        }
			
		  if(GPIOMap_DetTV()==TRUE)
		  {
			GPIOMap_SetTV(FALSE);
		  }
		  else
		  {
		      GPIOMap_SetTV(TRUE);
		  }
                  //Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS); //playback mode   
		}
          }
        else
        {
            uiModeKey = KEYSCAN_PWRKEY_PRESSED;
        }

}
    else
    {
        // Debounce
        if (uiModeKey == KEYSCAN_PWRKEY_RELEASED)
        {
            if (uiModeState == KEYSCAN_PWROFF_INIT_STATE)
            {
                uiModeState = KEYSCAN_PWROFF_RELEASE_STATE;
            }

            if (uiModeKeyPressCount)
            {
                uiModeKeyPressCount = 0;
                KeyScan_PlaySound(KEYSCAN_SOUND_KEY_OTHERS);
	        if(!KeyScan_IsHDMIPlugIn()&&(!KeyScan_IsTVPlugIn()))
	        {
		        if(!GPIOMap_IsLCDBacklightOn())
		        {
				GPIOMap_TurnOnLCDBacklight();
			       g_uiKeyScanAutoPoweroffCnt = 0;
				 return;
			}
	        }					   
                //Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS); 
            }
        }
        else
        {
            uiModeKey = KEYSCAN_PWRKEY_RELEASED;
        }
    }
//----------------------TV OUT MANUAL -------------------------------------	

#else // normal POWER key handling process
    // Detect power off key
    if ( rtc_getPWRStatus() || GPIOMap_DetPoweroff())
    {
        // Reset shutdown timer
        rtc_resetShutdownTimer();

        // Debounce
        if (uiPowerKey == KEYSCAN_PWRKEY_PRESSED)
        {
            if (uiPWRState == KEYSCAN_PWROFF_RELEASE_STATE)
            {
                uiPWRState = KEYSCAN_PWROFF_PRESS_STATE;
            }
            //#NT#2009/03/10#Photon Lin -begin
            //#Remove KeyScan_PoweroffFlow() to case KEYSCAN_PWRKEY_PRESSED
            if(uiPWRState == KEYSCAN_PWROFF_PRESS_STATE)
            {
                KeyScan_PoweroffFlow();
            }
            //#NT#2009/03/10#Photon Lin -end

        }
        else
        {
            uiPowerKey = KEYSCAN_PWRKEY_PRESSED;
        }
    }
    else
    {
        // Debounce
        if (uiPowerKey == KEYSCAN_PWRKEY_RELEASED)
        {
            if (uiPWRState == KEYSCAN_PWROFF_INIT_STATE)
            {
                uiPWRState = KEYSCAN_PWROFF_RELEASE_STATE;
            }
            else if (uiPWRState == KEYSCAN_PWROFF_PRESS_STATE)
            {
                KeyScan_PoweroffFlow();
            }
        }
        else
        {
            uiPowerKey = KEYSCAN_PWRKEY_RELEASED;
        }
    }
#endif
}

/**
  Detect battery voltage level

  Detect battery voltage level and store it in uiBatteryLvl
  If the battery voltage is VOLDET_BATTERY_LVL_EMPTY, then power off the system.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetBatteryInSysInit(void)
{
#if (KEYSCAN_POWEROFF_DET == ENABLE)

UINT32  uiDetLowBattery = 0;

    while (1)
    {
        uiBatteryLvl = VolDet_GetBatteryLevel();

        if (uiBatteryLvl == VOLDET_BATTERY_LVL_EMPTY)
        {
            uiDetLowBattery++;
            if (uiDetLowBattery >= 6)   // if continuously detect battery low over 600 ms
            {
                debug_err(("*********KeyScan_DetBatteryInSysInit: Low battery!!!\r\n"));
                FlowWrnBatteryEmpty_Open();
                TimerDelayMs(1500);
                KeyScan_PoweroffFlow();
                //#NT#2009/02/05#Hideo Lin -begin
                //#NT#It's not necessary to turn on LCD backlight here
                //GPIOMap_TurnOnLCDBacklight();
                //#NT#2009/02/05#Hideo Lin -end
            }
            TimerDelayMs(100);
        }
        else
            break;
    }

#endif //KEYSCAN_POWEROFF_DET
}

void KeyScan_DetBattery(void)
{
#if (KEYSCAN_POWEROFF_DET == ENABLE)

    UINT32  ui_battery_level =0;
    static UINT32   uiDetLowBattery = 0;
    static UINT32   ui_Battery_val[] = {BATTERY_FULL,BATTERY_MED,BATTERY_LOW,BATTERY_EMPTY,BATTERY_EXHAUSTED};
    
   // if (KeyScan_IsFlashCharging() || KeyScan_IsUSBPlug())
       // return;
    //debug_err(("KeyScan_IsUSBPlug : %d\r\n",KeyScan_IsUSBPlug()));
    if((usb_StateChange())||(GPIOMap_GetAcc12V()==TRUE))
    {
	switch(uiBatteryLvl)
	{
		case VOLDET_BATTERY_LVL_3:
			uiBatteryLvl = VOLDET_BATTERY_LVL_1;
			break;
		case VOLDET_BATTERY_LVL_1:
			uiBatteryLvl = VOLDET_BATTERY_LVL_2;			
			break;			
		case VOLDET_BATTERY_LVL_2:
			uiBatteryLvl = VOLDET_BATTERY_LVL_3;
			break;		
		default:
			uiBatteryLvl = VOLDET_BATTERY_LVL_1;
			break;			
	}
            SetBatteryLevel(ui_Battery_val[uiBatteryLvl]);
            Ux_PostEvent(NVTEVT_BATTERY, 0);
		return;
    }
    else
    {
    	uiBatteryLvl = VolDet_GetBatteryLevel();
      ui_battery_level = GetBatteryLevel();		
    }
    //debug_err(("battery level: new %d, old %d\n\r",uiBatteryLvl,ui_battery_level));
    switch(ui_battery_level)
    {
    case BATTERY_FULL:
        if(uiBatteryLvl != VOLDET_BATTERY_LVL_3)
        {
            SetBatteryLevel(ui_Battery_val[uiBatteryLvl]);
            Ux_PostEvent(NVTEVT_BATTERY, 0);
        }
        break;
    case BATTERY_MED:
        if(uiBatteryLvl != VOLDET_BATTERY_LVL_2)
        {
            SetBatteryLevel(ui_Battery_val[uiBatteryLvl]);
            Ux_PostEvent(NVTEVT_BATTERY, 0);
        }
        break;
    case BATTERY_LOW:
        if(uiBatteryLvl != VOLDET_BATTERY_LVL_1)
        {
            SetBatteryLevel(ui_Battery_val[uiBatteryLvl]);
            Ux_PostEvent(NVTEVT_BATTERY, 0);
        }
        break;
    case BATTERY_EMPTY:
        if(uiBatteryLvl != VOLDET_BATTERY_LVL_0)
        {
            SetBatteryLevel(ui_Battery_val[uiBatteryLvl]);
            Ux_PostEvent(NVTEVT_BATTERY, 0);
        }
        break;
    case BATTERY_EXHAUSTED:
        if(uiBatteryLvl != VOLDET_BATTERY_LVL_EMPTY)
        {
            SetBatteryLevel(ui_Battery_val[uiBatteryLvl]);
            Ux_PostEvent(NVTEVT_BATTERY, 0);
        }
        break;
    }

    if (uiBatteryLvl == VOLDET_BATTERY_LVL_EMPTY)
    {
        uiDetLowBattery++;
        if (uiDetLowBattery >= 2)   // if continuously detect battery low over 2 sec
        {
            debug_err(("*********KeyScan_DetBattery: Empty battery!!!\r\n"));
            KeyScan_PlaySound(PLAYSOUND_SOUND_KEY_OTHERS);
            TimerDelayMs(500);
            FlowWrnBatteryEmpty_Open();
            TimerDelayMs(2000);
            KeyScan_PoweroffFlow();
        }
    }
    else
    {
        uiDetLowBattery = 0;
    }
#endif //KEYSCAN_POWEROFF_DET
}

/**
  Detect auto power off

  Detect auto power off.
  If the auto power off counter reach auto power off setting, then do power off
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetAutoPoweroff(void)
{
    g_uiKeyScanAutoPoweroffCnt++;

    if (g_uiKeyScanAutoPoweroffCnt >= KeyScan_GetAutoPoweroffTime())
    {
        KeyScan_PoweroffFlow();
    }
}

/**
  Power off

  Power off. Add any code here that you want to do during power off flow.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_PowerDelayTimerHdl(void)
{
    timer_close(guiKeyscanPowerDelayTimerID);
    guiKeyscanPowerDelay=FALSE;
}
void KeyScan_PowerDelay(void)
{
    if (timer_open((UINT *)&guiKeyscanPowerDelayTimerID, KeyScan_PowerDelayTimerHdl) != E_OK)
        return;
    debug_err(("*********KeyScan_PowerDelay ----- \r\n"));	
    timer_set(guiKeyscanPowerDelayTimerID, 800, _TIMER_CTRL_ONE_SHOT | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
}
extern BOOL    gUIAviIsRecording ;
void KeyScan_PoweroffFlow(void)
{
#if (KEYSCAN_POWEROFF_DET == ENABLE)

    UINT32 i;

    debug_err(("*********KeyScan_PoweroffFlow\r\n"));

	//SysInit_DisplaySplashScreen(LOGO_POWEROFF,0,0);
    //TimerDelayMs(3000);
    //GPIOMap_TurnOffLCDBacklight();//for test

    if(GPIOMap_GetAcc12V()==FALSE)
    {
    		TimerDelayMs(500);	
	    if(GPIOMap_GetAcc12V()==TRUE)
	    {
	       debug_err(("*********KeyScan_PoweroffFlow  return \r\n"));	
		GPIOMap_TurnOnLCDBacklight();//for test
		return;
	    }			
    }

	
    gBKeyscanDetACCEn = FALSE;
    // stop recording before powering off if video is recording now
    if (Primary_GetCurrentMode() == PRIMARY_MODE_MOVIE)
    {
        if (gUIAviIsRecording==TRUE)
        {
            Ux_PostEvent(NVTEVT_KEY_ENTER, 2, NVTEVT_KEY_PRESS, FLGKEY_PRESSED);
        }
    }

    //#NT#2009/03/23#Janice ::wait primary tsk idle for change mode busy
    Primary_CheckIdle(PRI_TIME_INFINITE);

    //#NT#2007/07/24#Janice -begin
    //make sure is not bad card,then check fst finish
    if (GetFSStatus() == FS_INIT_OK)
    {
        // Wait for filesystem done,if normal power off,else skip wait
        if(!g_bFilesysAbortCmd)
        {
            //check fst busy,wait inifinit would lock keyscantsk
            if (FilesysWaitCmdFinish(FST_TIME_NO_WAIT) == FST_STA_BUSY)
            {
                    KeyScan_Poweroff(); //force power off make sure KeyScan_DetPoweroff would enter KeyScan_PoweroffFlow again
                    return;
            }
         }
    }
    //#NT#2007/07/24#Janice -end

    // Pause KeyScan timer
    timer_pausePlay(g_uiKeyScanTimerID, _TIMER_PAUSE);

    // Terminate all task first
    debug_err(("*********AppInit_Close\r\n"));
    AppInit_Close();

    // Save menu info
    debug_err(("*********Save_MenuInfo\r\n"));
    Save_MenuInfo();
    KeyScan_PlaySound(KEYSCAN_SOUND_STARTUP);

    // Display poweroff logo
    if (GPIOMap_IsLCDBacklightOn() == FALSE &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        //OpenPanel(&g_LCDCtrlObj, &g_LCDSize);
        //SysInit_DisplaySplashScreen(LOGO_POWEROFF,0,0);
        //UI_ClearOSD(_OSD_INDEX_TRANSPART); // clear OSD
        //ide_disable_osd(IDE_OSDID_1);
        //ide_enable_video(IDE_VIDEOID_1);
        //TimerDelayMs(100);
        //GPIOMap_TurnOnLCDBacklight();
    }
    else
    {
       SysInit_DisplaySplashScreen(LOGO_POWEROFF,0,0);
        UI_ClearOSD(_OSD_INDEX_TRANSPART); // clear OSD
        TimerDelayMs(3000);
        GPIOMap_TurnOffLCDBacklight();//for test
    }
	//SysInit_DisplaySplashScreen(LOGO_POWEROFF,0,0);
    //TimerDelayMs(500);
    
    UI_ClearOSD(_OSD_INDEX_TRANSPART); // clear OSD
    ide_disable_video(IDE_VIDEOID_1);
    ide_disable_video(IDE_VIDEOID_2);
    TimerDelayMs(30);
    
	

    // Clear screen
    if (KeyScan_IsHDMIPlugIn() == TRUE)
    {
        UI_ClearOSD(_OSD_INDEX_BLACK);
        ide_wait_frame_end();
        CloseTVHDMI();
    }
    else if (KeyScan_IsTVPlugIn() == TRUE)
        UI_ClearOSD(_OSD_INDEX_BLACK); //for TV & HDMI.(sent black pattern when power off)
    else
        UI_ClearOSD(_OSD_INDEX_WHITE); //for Toppoly LCD.(need to sent white pattern when power off)

    // Close Panel
    ClosePanel();

    TimerDelayMs(700);
    //for (i=0x10000; i>0; i--);

    // Panel enter sleep mode
    Lens_Shutter_Move(CLOSE);
    //GPIOMap_TurnOffLCDPower();
    //TimerDelayMs(200);

    // Retract lens
    Photo_OpticalZoom_Retract();

    // Turn off LCD, CCD...
    GPIOMap_TurnOffCCDPower();

    // Turn off green and red LED
    KeyScan_TurnOffLED(KEYSCAN_LED_GREEN);
    KeyScan_TurnOffLED(KEYSCAN_LED_RED);

    // set NAND write protect to stop NAND (write) accessing
    GPIOMap_SetNANDWp();

    debug_err(("*********rtc_poweroffPWR\r\n"));
    // Power off
#if 0
    GPIOMap_SetPoweroffCtrlLow();
#else
    rtc_poweroffPWR();
#endif

    // Loop forever
    while (1)
    {
        debug_err(("*"));
    }

#endif //KEYSCAN_POWEROFF_DET
}

/**
  Enable/Disable auto power off function

  Detect auto power off.
  If the auto power off function is disabled and you want to enable it,
  the power off counter will reset to 0.

  @param void
  @return void
*/
void KeyScan_EnableAutoPoweroff(BOOL bEn)
{

    // Reset auto power off counter
    if (g_bKeyScanAutoPoweroffEn == FALSE && bEn == TRUE)
    {
        g_uiKeyScanAutoPoweroffCnt = 0;
    }
    if (KeyScan_IsUSBPlug() == TRUE)
    {
        g_bKeyScanAutoPoweroffEn = FALSE;
    } else {
        g_bKeyScanAutoPoweroffEn = bEn;
    }
}

/**
  Set/Get auto power off time value

  Set/Get auto power off time value (in seconds)

  @param
  @return
*/
void KeyScan_SetAutoPoweroffTime(UINT32 uiTime)
{
    uiAutoPoweroffTime = uiTime;
}
UINT32 KeyScan_GetAutoPoweroffTime(void)
{
    return uiAutoPoweroffTime;
}

void KeyScan_SetAutoLCDoffTime(UINT32 uiTime)
{
    uiAutoLCDoffTime = uiTime;
}
UINT32 KeyScan_GetAutoLCDoffTime(void)
{
    return uiAutoLCDoffTime;
}
/**
  Ask KeyScan task to do power off

  Ask KeyScan task to do power off. The timing of key scan task to handle
  this event will be related to the timing of calling KeyScan_DetPoweroff() API.

  @param void
  @return void
*/
void KeyScan_Poweroff(void)
{
    bDoPoweroff = TRUE;
}

UINT32 KeyScan_GetLedLightSta(void)
{
	return uiWhiteLedLightStatus;
}
void KeyScan_SetLedLightSta(UINT32 Sta)
{
	uiWhiteLedLightStatus = Sta;
}

/**
  Detect AC is plugging in or unplugged

  Detect AC is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetAC(void)
{
    #define AC_PLUS_COUNT 10
    BOOL            bCurACDet, bCurACStatus;
    static UINT32   ACCount = 0;
    static BOOL     bACUnplus = FALSE;

    if ((KeyScan_IsHDMIPlugIn() == TRUE) || (KeyScan_IsTVPlugIn() == TRUE))
        return;

    //check acpower 15 seconds,if AC Power is unplused,call stop record and poweroff
    bCurACDet = KeyScan_IsUSBPower();

    if (bCurACDet)
    {
        //ACCount = AC_PLUS_COUNT;
	 if(Get_DelayPowerOffIndex() == DELAY_POWER_OFF_OFF)
	 	ACCount = 1;
	 else if(Get_DelayPowerOffIndex() == DELAY_POWER_OFF_15S)
	 	ACCount = 15;
	 else if(Get_DelayPowerOffIndex() == DELAY_POWER_OFF_1MIN)
	 	ACCount = 60;	 	
	 else if(Get_DelayPowerOffIndex() == DELAY_POWER_OFF_5MIN)
	 	ACCount = 300;	 
	 else if(Get_DelayPowerOffIndex() == DELAY_POWER_OFF_30MIN)
	 	ACCount = 1800;	 
	 else if(Get_DelayPowerOffIndex() == DELAY_POWER_OFF_60MIN)
	 	ACCount = 3600;	  
	 else
	 	ACCount =1;
    }
    else if (bACUnplus)
    {
        debug_err(("ACCount = %d\r\n", ACCount));

        if (ACCount)
        {
            ACCount -- ;
        }
        if (ACCount == 0)
        {
            KeyScan_Poweroff();
        }
    }

    bCurACStatus = (BOOL)(bCurACDet & bLastACDet);

    //debug_err(("AC is plugged, GPIOMap_DetAC = %d\r\n", bCurACDet));

    if (bCurACStatus != bLastACStatus)
    {
        // AC is plugging in
        if (bCurACStatus == TRUE)
        {
            bACStatus = TRUE;
            bACUnplus = FALSE;
          }
        // AC is unplugged
        else
        {
            debug_err(("AC is unplugged, GPIOMap_DetAC = %d\r\n", bCurACDet));
            bACStatus = FALSE;
            bACUnplus = TRUE;
        }
    }

    bLastACDet     = bCurACDet;
    bLastACStatus  = bCurACStatus;
}

//@}
