/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanHDMI.c
    @ingroup    mIPRJAPKey

    @brief      Detect HDMI is plugging in or unplugged
                Detect HDMI is plugging in or unplugged

    @note       Nothing.

    @date       2009/07/06
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "KeyScanTsk.h"
#include "KeyScanInt.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "PlaybackTsk.h"
#include "UIFlow.h"
#include "IDE.h"
#include "Top.h"
#include "GlobalVar.h"
#include "Utility.h"
#include "UIMenuSetupSettings.h"
#include "hdmitx.h"
#include "MediaPlayAPI.h"
#include "UIFlowWndPlayFuncs.h"
#include "Audio.h"
#include "SystemInit.h"
#include "UIGraphics.h"
//#NT#2010/04/26#Klins Chen -begin
#include "UIFramework.h"
#include "cec.h"
#include "SysCfg.h"
//#NT#2010/04/26#Klins Chen -end

static BOOL     bHDMIStatus = FALSE;
static BOOL     bLastHDMIDet     = FALSE;
static BOOL     bLastHDMIStatus  = FALSE;

//#NT#2010/04/26#Klins Chen -begin
#if (_CEC_CONTROL_ == ENABLE)
UINT32 NVTPressedKey = 0xFFFFFFFF;
UINT32 CecPressedKey = 0xFFFFFFFF;

UINT32 CecKeyStatus  = 0x00;
#define KEYDOWN       0x01
#define KEYRELEASE    0x02
ER KeyScan_parseRemoteKey(CECKEY Key, BOOL Press)
{
    if(Press)
    {
        //Key Down
        if((CecKeyStatus&KEYDOWN)&&(CecPressedKey == Key))
            return E_OK;

        CecKeyStatus |=  KEYDOWN;
        CecKeyStatus &= ~KEYRELEASE;
    }
    else
    {
        debug_msg(("CEC Key Release!\r\n"));
        if(CecKeyStatus&KEYDOWN)
        {
            CecKeyStatus &= ~KEYDOWN;
            CecKeyStatus |= KEYRELEASE;

            //
            //  This can used to notify UI Key Release Event if needed
            //
            //Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_RELEASE);

        }
        return E_OK;
    }


    switch(Key)
    {
        case CECKEY_PLAY:
        case CECKEY_SELECT:
        case CECKEY_ENTER:
            debug_msg(("CEC Key Press: Select/Enter \r\n"));
            NVTPressedKey = NVTEVT_KEY_ENTER;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_UP:
            debug_msg(("CEC Key Press: UP \r\n"));
            NVTPressedKey = NVTEVT_KEY_UP;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_DOWN:
            debug_msg(("CEC Key Press: DOWN \r\n"));
            NVTPressedKey = NVTEVT_KEY_DOWN;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_LEFT:
            debug_msg(("CEC Key Press: LEFT \r\n"));
            NVTPressedKey = NVTEVT_KEY_LEFT;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_RIGHT:
            debug_msg(("CEC Key Press: RIGHT \r\n"));
            NVTPressedKey = NVTEVT_KEY_RIGHT;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_ROOT_MENU:
        case CECKEY_SETUP_MENU:
            debug_msg(("CEC Key Press: MENU \r\n"));
            NVTPressedKey = NVTEVT_KEY_MENU;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_EXIT:
            debug_msg(("CEC Key Press: EXIT \r\n"));
            break;

        case CECKEY_POWER:
            debug_msg(("CEC Key Press: POWER \r\n"));
            NVTPressedKey = NVTEVT_KEY_POWER;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_VOLUME_UP:
        case CECKEY_VOLUME_DOWN:
            debug_msg(("CEC Key Press: VOLUME_UP/DOWN \r\n"));
            break;

        case CECKEY_MUTE:
            debug_msg(("CEC Key Press: MUTE \r\n"));
            break;

        case CECKEY_STOP:
            debug_msg(("CEC Key Press: STOP \r\n"));
            break;

        case CECKEY_PAUSE:
            debug_msg(("CEC Key Press: PAUSE \r\n"));
            break;

        case CECKEY_RECORD:
            debug_msg(("CEC Key Press: RECORD \r\n"));
            break;

        case CECKEY_FORWARD:
        case CECKEY_FAST_FORWARD:
            debug_msg(("CEC Key Press: FORWARD\r\n"));
            NVTPressedKey = NVTEVT_KEY_RIGHT;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;

        case CECKEY_REWIND:
        case CECKEY_BACKWARD:
            debug_msg(("CEC Key Press: BACKWARD \r\n"));
            NVTPressedKey = NVTEVT_KEY_LEFT;
            Ux_PostEvent(NVTPressedKey, 1, NVTEVT_KEY_PRESS);
            break;



        case CECKEY_F1://BLUE
            debug_msg(("CEC Key Press: F1/BLUE \r\n"));
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);
            break;
        case CECKEY_F2://RED
            debug_msg(("CEC Key Press: F2/RED \r\n"));
            Primary_ChangeMode(PRIMARY_MODE_MOVIE);
            break;
        case CECKEY_F4://YELLOW
        case CECKEY_F3://GREEN
            debug_msg(("CEC Key Press: F3/GREEN \r\n"));
            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
            break;




        default:
            //cec_cmdFeatureAbort(DstAddr, CECCMD_USERCTL_PRESSED, CECABORT_INVALID_OPERAND);
            //debug_err(("Feature Abort Cmd Code (0x%02X)\r\n",CECCMD_USERCTL_PRESSED));
            return E_PAR;
    }

    CecPressedKey = Key;

    return E_OK;
}
#endif
//#NT#2010/04/26#Klins Chen -end

//#NT#2009/12/02#Philex -begin
static BOOL     bStartupHDMIStatus  = FALSE;
//#NT#2009/12/02#Philex -end

//#NT#2009/12/10#Philex -begin
void KeyScan_HDMIAudioSetting(AUDIO_SETTING AudioSetting, BOOL bEnable)
{
    if (aud_getLockStatus() == TASK_LOCKED)
    {
        aud_setI2SClockCtrl(bEnable);
        aud_setOutput(AudioSetting.Output);
        aud_setSamplingRate(AudioSetting.SamplingRate);
    }
    else
    {
      AUDIO_DEVICE_OBJ    AudObj;

        aud_getDeviceObject(&AudObj);
        aud_open(&AudObj);
        aud_setI2SClockCtrl(bEnable);
        aud_setOutput(AudioSetting.Output);
        aud_setSamplingRate(AudioSetting.SamplingRate);
        aud_close();
    }
}
//#NT#2009/12/10#Philex -end

/**
  Detect HDMI is plugging in or unplugged

  Detect HDMI is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetHDMI(void)
{
    BOOL            bCurHDMIDet, bCurHDMIStatus;
    UINT32          i, uiWindowEn;
    PrimaryModeID   CurrMode;
    IPL_OBJ         IplObj;
    IMEIDE_ATTR     ImeIdeAttr;
    //#NT#2009/08/18#Klins Chen#[0006129] -begin
    //#Add to select audio channel to HDMI interface
    AUDIO_SETTING   AudioSetting;
    AudioSetting.SamplingRate       = AUDIO_SR_32000;               // Sampling rate = Clock / 256
    //#NT#2009/08/18#Klins Chen#[0006129] -end

    if ((KeyScan_IsUSBPlug() ==TRUE) || (KeyScan_IsTVPlugIn()==TRUE))
    {
         return;
    }

    bCurHDMIDet = GPIOMap_DetHDMI();

    bCurHDMIStatus = (BOOL)(bCurHDMIDet & bLastHDMIDet);

    if (bCurHDMIStatus != bLastHDMIStatus)
    {
        // Get current mode
        CurrMode = Primary_GetCurrentMode();

        Primary_ChangeMode(PRIMARY_MODE_DUMMY);
        Primary_Wait4ChangeModeDone();

        // HDMI is plugging in
        if (bCurHDMIStatus == TRUE)
        {
            debug_err(("# HDMI is plugging in #\n\r"));
            bHDMIStatus = TRUE;

            // Turn off LCD backlight
            GPIOMap_TurnOffLCDBacklight();

            ide_disable_video(IDE_VIDEOID_1);
            ide_disable_video(IDE_VIDEOID_2);

            // Close LCD panel
            ClosePanel();
            GPIOMap_TurnOffLCDPower();
            pinmux_select_lcd(PINMUX_GPIO);

            // Disable all windows
            uiWindowEn = ide_get_window_en();
            ide_set_allwindow_dis();
            ide_set_load();
            ide_wait_frame_end();

            //Init HDMI
            // This settings is customized by project
            InitTVHDMI(HDMI_1280X720P60, HDMI_AUDIO32KHZ);
            OpenTVHDMI(&g_LCDCtrlObj,&g_LCDSize);
            //#NT#2010/04/26#Klins Chen -begin
            #if (_CEC_CONTROL_ == ENABLE)
            cec_open();
            cec_setDeviceVendorID(CECVENDOR_NONE);
            cec_hookRemoteCallBack(KeyScan_parseRemoteKey, NULL);
            #endif
            //#NT#2010/04/26#Klins Chen -end

            //Display ratio
            ImgCapture_SetParameter(_DispRatio, _Ratio_16_9);

            // Scaling OSD to new size
            KeyScan_ScaleDisplay();

            // Enable IDE
            ide_waitFrameEnd();
            ide_setEn(TRUE);

            // Delay to wait for IDE stable
            for (i=6; i>0; i--)
            {
                ide_wait_frame_end();
            }

            // Enable all windows
            ide_set_allwindow_en(uiWindowEn);
            ide_set_load();

            //#NT#2009/11/24#Philex - begin
            // change audio output path/sampling rate
            AudioSetting.Output             = AUDIO_OUTPUT_NONE;
            KeyScan_HDMIAudioSetting(AudioSetting, TRUE);
            //#NT#2009/11/24#Philex - end

            // forced set to playback mode
            CurrMode = PRIMARY_MODE_PLAYBACK;
        }
        // HDMI is unplugged
        else
        {
            debug_err(("# HDMI is unplugged #\n\r"));

            bHDMIStatus = FALSE;

            ide_disable_video(IDE_VIDEOID_1);
            ide_disable_video(IDE_VIDEOID_2);

            //#NT#2010/04/26#Klins Chen -begin
            #if (_CEC_CONTROL_ == ENABLE)
            cec_hookRemoteCallBack(NULL, NULL);
            cec_close();
            #endif
            //#NT#2010/04/26#Klins Chen -end

            // Close HDMI
            ide_wait_frame_end();
            CloseTVHDMI();

            // Disable all windows
            uiWindowEn = ide_get_window_en();
            ide_set_allwindow_dis();
            ide_set_load();
            ide_wait_frame_end();

            // Open LCD panel
            GPIOMap_TurnOnLCDPower();
            OpenPanel(&g_LCDCtrlObj, &g_LCDSize);

            //Display ratio
            #if (_LCDTYPE_ == _LCDTYPE_TXDT300C_)
            ImgCapture_SetParameter(_DispRatio, _Ratio_16_9);
            #else
            ImgCapture_SetParameter(_DispRatio, _Ratio_4_3);
            #endif

            // Scaling OSD to new size
            KeyScan_ScaleDisplay();

            //ImeIdeAttr.uiImeOutH    = g_LCDSize.uiWidth;
            //ImeIdeAttr.uiImeOutV    = g_LCDSize.uiHeight;
            //ImeIdeAttr.uiIdeOutH    = g_LCDSize.uiWinWidth;
            //ImeIdeAttr.uiIdeOutV    = g_LCDSize.uiWinHeight;

            // Enable IDE
			ide_wait_frame_end();
            ide_setEn(TRUE);

            // Delay 6 IDE frames to avoid seeing garbage display
            for (i=6; i>0; i--)
            {
                ide_wait_frame_end();
            }

            // Enable all windows
            ide_set_allwindow_en(uiWindowEn);
            ide_set_load();

            //#NT#2009/08/18#Klins Chen#[0006129] -begin
            //#Add to select audio channel to HDMI interface
            AudioSetting.Output             = AUDIO_OUTPUT_SPK;
            KeyScan_HDMIAudioSetting(AudioSetting, FALSE);
            //#NT#2009/08/18#Klins Chen#[0006129] -end

            TimerDelayMs(150); // delay time before turn on LCD backlight
        }

        Primary_ChangeMode(CurrMode);
        Primary_Wait4ChangeModeDone();
    }

    bLastHDMIDet     = bCurHDMIDet;
    bLastHDMIStatus  = bCurHDMIStatus;
}

/**
  Return HDMI plug in or unplugged

  Return HDMI plug in or unplugged.
  TRUE  -> HDMI plug in.
  FALSE -> HDMI is unplugged.

  @param void
  @return BOOL: TRUE -> HDMI plug in, FALSE -> HDMI is unplugged
*/
BOOL KeyScan_IsHDMIPlugIn(void)
{
    return bHDMIStatus;
}

//#NT#2009/12/2#philex - begin
BOOL KeyScan_IsHDMIStartup(void)
{
    return bStartupHDMIStatus;
}

void KeyScan_SetHDMIStartup(BOOL bzHDMIStatus)
{
    bStartupHDMIStatus = bzHDMIStatus;
}

//#NT#2009/12/2#philex - end
//@}
