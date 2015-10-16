/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanTV.c
    @ingroup    mIPRJAPKey

    @brief      Detect TV is plugging in or unplugged
                Detect TV is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "Alg_IPLMode.h"
#include "PlaybackTsk.h"
#include "AVIPlayAPI.h"
#include "MediaPlayAPI.h"
#include "UIFlow.h"
#include "IDE.h"
#include "Top.h"
#include "GlobalVar.h"
#include "Utility.h"
#include "Audio.h"
#include "UIMenuSetupSettings.h"

static UINT32   uiCurrentTVMode;
static BOOL     bTVStatus       = FALSE;
static BOOL     bLastTVDet      = FALSE;
static BOOL     bLastTVStatus   = FALSE;

BOOL gbKscanTVPlug = FALSE;
extern BOOL gUIAviIsRecording;
/**
  Scaling OSD size between pannel and TV

  Scaling OSD size between pannel and TV.
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_ScaleDisplay(void)
{
    VOSD_WINDOW_ATTR    FB_win;
    UINT32              uiSourceW, uiSourceH, uiSourceOff;
    IDE_COLOR_FORMAT    ColorFormat;

    // OSD1
    ide_get_osd1_buf_dim(&uiSourceW, &uiSourceH, &uiSourceOff);
    ide_get_osd1_win_format(&ColorFormat);

    FB_win.Source_W     = uiSourceW;
    FB_win.Source_H     = uiSourceH;
    FB_win.Des_W        = g_LCDSize.uiWinWidth;
    FB_win.Des_H        = g_LCDSize.uiWinHeight;
    FB_win.Win_Format   = ColorFormat;
    FB_win.Win_X        = 0;
    FB_win.Win_Y        = 0;
    FB_win.High_Addr    = 0;
    ide_set_osd_win_attr(IDE_OSDID_1, &FB_win);

    // Video1
    ide_get_video1_buf_dim(&uiSourceW, &uiSourceH, &uiSourceOff);
    ide_get_v1_win_format(&ColorFormat);

    FB_win.Source_W     = uiSourceW;
    FB_win.Source_H     = uiSourceH;
    FB_win.Des_W        = g_LCDSize.uiWinWidth;
    FB_win.Des_H        = g_LCDSize.uiWinHeight;
    FB_win.Win_Format   = ColorFormat;
    FB_win.Win_X        = 0;
    FB_win.Win_Y        = 0;
    FB_win.High_Addr    = 0;
    ////ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);

    //video2
    ide_get_video2_buf_dim(&uiSourceW, &uiSourceH, &uiSourceOff);
    ide_get_v2_win_format(&ColorFormat);

    FB_win.Source_W = uiSourceW;
    FB_win.Source_H = uiSourceH;
    FB_win.Des_W = g_LCDSize.uiWinWidth;
    FB_win.Des_H = g_LCDSize.uiWinHeight;
    FB_win.Win_Format = ColorFormat;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;
    ide_set_video_win_attr(IDE_VIDEOID_2, &FB_win);
}

/**
  Detect TV is plugging in or unplugged

  Detect TV is plugging in or unplugged.

  @param void
  @return void
*/
void KeyScan_DetTV(void)
{
    BOOL                bCurTVDet, bCurTVStatus;
    IMEIDE_ATTR         ImeIdeAttr;
    UINT32              i, uiWindowEn;
    PrimaryModeID       CurrMode;
    TV_DEVICE_OBJ       TVObj;
    AUDIO_OUTPUT        AudioOutput;
    IPL_OBJ             IplObj;
    DisplaySizeInfor    Displaysizeinfor;
    static BOOL first_2 = TRUE;
    static UINT32 changModeDelay =0;
    if ((KeyScan_IsUSBPlug() == TRUE) || (KeyScan_IsHDMIPlugIn() == TRUE))
         return;

    bCurTVDet = GPIOMap_DetTV();

    bCurTVStatus = (BOOL)(bCurTVDet & bLastTVDet);

    if (bCurTVStatus != bLastTVStatus)
    {		
//-----------jack lan ---------------------	
       KeyScan_SetDetGSPNAVIEn(FALSE);
	ide_disable_video(IDE_VIDEOID_1);
	ide_disable_video(IDE_VIDEOID_2);
       if(gUIAviIsRecording == TRUE)
    	{	 
        gbKscanTVPlug = TRUE;
	 //Ux_PostEvent(NVTEVT_MOVIEAUTORECORDSTOP, 0);	
	// for(changModeDelay=0;changModeDelay<300;changModeDelay++)
	 //{}
	 }	
	 else
	 {
        gbKscanTVPlug = FALSE;
	 }
	 
//---------------------------------------    
        CurrMode = Primary_GetCurrentMode();

        // Enter dummy mode first
        Primary_ChangeMode(PRIMARY_MODE_DUMMY);
        Primary_Wait4ChangeModeDone();

        // TV is plugging in
        if (bCurTVStatus == TRUE)
        {
            debug_err(("TV is plugged, GPIOMap_DetTV = %d\r\n", bCurTVDet));

            bTVStatus = TRUE;

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
			
            TVObj.bIDESrcPLL2 = g_LCDCtrlObj.bIDESrcPLL2;
            TVObj.uiIDESrcCLK = g_LCDCtrlObj.uiIDESrcCLK;
		TVObj.uiTVFSADJ = TV_FSADJ_3_3_K;

            // Init TV object
            if (Get_TVModeIndex() == TV_MODE_NTSC)
            {
                // NTSC
                InitTVNTSC_OBJ(&TVObj);
                OpenTVNTSC(&g_LCDSize);
                uiCurrentTVMode = TV_MODE_NTSC;
            }
            else
            {
                // PAL
                InitTVPAL_OBJ(&TVObj);
                OpenTVPAL(&g_LCDSize);
                uiCurrentTVMode = TV_MODE_PAL;
            }

            // Scaling OSD to new size
            KeyScan_ScaleDisplay();

            #if 0
            switch (Primary_GetCurrentMode())
            {
            // If you won't call playback task to display last image in preview mode,
            // you don't have to call PB_SetIMEIDESize() in PHOTO mode.
            case PRIMARY_MODE_PHOTO:

                IplObj.uiCmd = IPL_CMD_CHG_SIZE_IME1;
                IplObj.uiImeOutWidth1 = g_LCDSize.uiWidth;
                IplObj.uiImeOutHeight1 = g_LCDSize.uiHeight;
                IPL_SetCmd(&IplObj);

                Displaysizeinfor.DisplayInHsize = g_LCDSize.uiWidth;
                Displaysizeinfor.DisplayInVsize = g_LCDSize.uiHeight;
                Displaysizeinfor.DisplayInLineoffset = g_LCDSize.uiWidth;
                Displaysizeinfor.DisplayOutHsize = g_LCDSize.uiWinWidth;
                Displaysizeinfor.DisplayOutVsize = g_LCDSize.uiWinHeight;
                PhotoDisplay_ChgSize(&Displaysizeinfor,TRUE);

                ImeIdeAttr.uiImeOutH    = g_LCDSize.uiWidth;
                ImeIdeAttr.uiImeOutV    = g_LCDSize.uiHeight;
                ImeIdeAttr.uiIdeOutH    = g_LCDSize.uiWinWidth;
                ImeIdeAttr.uiIdeOutV    = g_LCDSize.uiWinHeight;

                PB_SetIMEIDESize(&ImeIdeAttr);
                break;

            case PRIMARY_MODE_MOVIE:
                // Set to 1/4 size to reduce memory bandwith requirement
                IplObj.uiCmd = IPL_CMD_CHG_SIZE_IME1;
                IplObj.uiImeOutWidth1 = g_LCDSize.uiWidth >> 1;
                IplObj.uiImeOutHeight1 = g_LCDSize.uiHeight >> 1;
                IPL_SetCmd(&IplObj);

                Displaysizeinfor.DisplayInHsize = g_LCDSize.uiWidth >> 1;
                Displaysizeinfor.DisplayInVsize = g_LCDSize.uiHeight >> 1;
                Displaysizeinfor.DisplayInLineoffset = g_LCDSize.uiWidth;
                Displaysizeinfor.DisplayOutHsize = g_LCDSize.uiWinWidth;
                Displaysizeinfor.DisplayOutVsize = g_LCDSize.uiWinHeight;
                PhotoDisplay_ChgSize(&Displaysizeinfor,TRUE);
                break;

            case PRIMARY_MODE_PLAYBACK:
                //Todo: FlowPB_UpdateThumbRect();
                ImeIdeAttr.uiImeOutH    = g_LCDSize.uiWidth;
                ImeIdeAttr.uiImeOutV    = g_LCDSize.uiHeight;
                ImeIdeAttr.uiIdeOutH    = g_LCDSize.uiWinWidth;
                ImeIdeAttr.uiIdeOutV    = g_LCDSize.uiWinHeight;
                PB_SetIMEIDESize(&ImeIdeAttr);
                break;

            default:
                break;
            }
            #endif

            // Enable IDE
	     ide_wait_frame_end();
            //ide_set_en(TRUE);

            // Delay to wait for TV DAC stable
            for (i=16; i>0; i--)
            {
                ide_wait_frame_end();
            }
            ide_set_en(TRUE);

            // Enable all windows
            ide_set_allwindow_en(uiWindowEn);
            ide_set_load();

            // forced set to playback mode
            //CurrMode = PRIMARY_MODE_PLAYBACK;
        }
        // TV is unplugged
        else
        {
            debug_err(("TV is unplugged, GPIOMap_DetTV = %d\r\n", bCurTVDet));
            bTVStatus = FALSE;
            GPIOMap_TurnOffLCDBacklight();

            // Disable all windows
            uiWindowEn = ide_get_window_en();
            ide_set_allwindow_dis();
            ide_set_load();
            ide_wait_frame_end();

            // Close TV
            if (uiCurrentTVMode == TV_MODE_NTSC)
            {
                // NTSC
                CloseTVNTSC();
            }
            else
            {
                // PAL
                CloseTVPAL();
            }
        	for(changModeDelay=0;changModeDelay<2;changModeDelay++)
	 	{
			TimerDelayMs(300);
		}
            // Open LCD panel
            //GPIOMap_TurnOnLCDPower();
            OpenPanel(&g_LCDCtrlObj, &g_LCDSize);

            // Scaling OSD to new size
            KeyScan_ScaleDisplay();
            #if 0
            ImeIdeAttr.uiImeOutH    = g_LCDSize.uiWidth;
            ImeIdeAttr.uiImeOutV    = g_LCDSize.uiHeight;
            ImeIdeAttr.uiIdeOutH    = g_LCDSize.uiWinWidth;
            ImeIdeAttr.uiIdeOutV    = g_LCDSize.uiWinHeight;

            IplObj.uiCmd = IPL_CMD_CHG_SIZE_IME1;
            IplObj.uiImeOutWidth1 = ImeIdeAttr.uiImeOutH;
            IplObj.uiImeOutHeight1 = ImeIdeAttr.uiImeOutV;

            Displaysizeinfor.DisplayInHsize = ImeIdeAttr.uiImeOutH;
            Displaysizeinfor.DisplayInVsize = ImeIdeAttr.uiImeOutV;
            Displaysizeinfor.DisplayInLineoffset= ImeIdeAttr.uiIdeOutH;
            Displaysizeinfor.DisplayOutHsize = ImeIdeAttr.uiIdeOutH;
            Displaysizeinfor.DisplayOutVsize = ImeIdeAttr.uiIdeOutV;


            switch (Primary_GetCurrentMode())
            {
            // If you won't call playback task to display last image in preview mode,
            // you don't have to call PB_SetIMEIDESize() in PHOTO mode.
            case PRIMARY_MODE_PHOTO:
                IPL_SetCmd(&IplObj);
                PhotoDisplay_ChgSize(&Displaysizeinfor,TRUE);
                PB_SetIMEIDESize(&ImeIdeAttr);
                break;

            case PRIMARY_MODE_MOVIE:
                IPL_SetCmd(&IplObj);
                PhotoDisplay_ChgSize(&Displaysizeinfor,TRUE);
                break;

            case PRIMARY_MODE_PLAYBACK:
                //Todo: FlowPB_UpdateThumbRect();
                PB_SetIMEIDESize(&ImeIdeAttr);
                break;

            default:
                break;
            }
            #endif

            // Enable all windows
            ide_set_allwindow_en(uiWindowEn);
            ide_set_load();

            // Delay 6 IDE frames to avoid seeing garbage display
            for (i=6; i>0; i--)
            {
                ide_wait_frame_end();
            }
            // Turn on LCD backlight
            //GPIOMap_TurnOnLCDBacklight();
            TimerDelayMs(150); // delay time before turn on LCD backlight
        }
        Primary_ChangeMode(CurrMode);               // back to current mode
        Primary_Wait4ChangeModeDone();
    }

    bLastTVDet     = bCurTVDet;
    bLastTVStatus  = bCurTVStatus;
}

/**
  Return TV plug in or unplugged

  Return TV plug in or unplugged.
  TRUE  -> TV plug in.
  FALSE -> TV is unplugged.

  @param void
  @return BOOL: TRUE -> TV plug in, FALSE -> TV is unplugged
*/
BOOL KeyScan_IsTVPlugIn(void)
{
    return bTVStatus;
}

void KeyScan_ChangeTVMode(void)
{
    TV_DEVICE_OBJ   TVObj;
    UINT32          i, uiWindowEn;

    // Disable all windows
    uiWindowEn = ide_get_window_en();
    ide_set_allwindow_dis();
    ide_set_load();
    ide_wait_frame_end();

    // Close TV
    if (uiCurrentTVMode == TV_MODE_NTSC)
    {
        CloseTVNTSC();
    }
    else
    {
        CloseTVPAL();
    }

    // Open LCD panel
    OpenPanel(&g_LCDCtrlObj, &g_LCDSize);

    // Enable all windows
    ide_set_allwindow_en(uiWindowEn);
    ide_set_load();

    // Delay 6 IDE frames to avoid seeing garbage display
    for (i=6; i>0; i--)
    {
        ide_wait_frame_end();
    }

    // Disable all windows
    uiWindowEn = ide_get_window_en();
    ide_set_allwindow_dis();
    ide_set_load();
    ide_wait_frame_end();

    // Close LCD panel
    ClosePanel();

    TVObj.bIDESrcPLL2 = g_LCDCtrlObj.bIDESrcPLL2;
    TVObj.uiIDESrcCLK = g_LCDCtrlObj.uiIDESrcCLK;
    TVObj.uiTVFSADJ = TV_FSADJ_3_3_K;

    // Init TV object
    if (Get_TVModeIndex() == TV_MODE_NTSC)
    {
        // NTSC
        InitTVNTSC_OBJ(&TVObj);
        OpenTVNTSC(&g_LCDSize);
        uiCurrentTVMode = TV_MODE_NTSC;
        debug_err(("Switch and set TV NTSC...\r\n"));
    }
    else
    {
        // PAL
        InitTVPAL_OBJ(&TVObj);
        OpenTVPAL(&g_LCDSize);
        uiCurrentTVMode = TV_MODE_PAL;
        debug_err(("Switch and set TV PAL..\r\n"));
    }	
    // Scaling OSD to new size
    KeyScan_ScaleDisplay();
	
    // Scaling ime ide
    //KeyScan_ChangeIMEIDESize(Primary_GetCurrentMode());

    ide_wait_frame_end();
    ide_set_en(TRUE);

    for (i=6; i>0; i--)
    {
        ide_wait_frame_end();
    }

    // Enable all windows
    ide_set_allwindow_en(uiWindowEn);
    ide_set_load();

}

//@}
