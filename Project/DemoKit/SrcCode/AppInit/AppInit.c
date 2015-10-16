/*
    Init appication required information. Get the memory,
    init application parameters...

    @file       AppInit.c
    @ingroup    mIPRJAPCfg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "PrimaryTsk.h"
#include "AppInit.h"
#include "DeviceCtrl.h"
#include "UIFlow.h"
#include "SystemClock.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "Usicd.h"
#include "SystemClock.h"
#if (_IQ_MODE_ == ENABLE)
#include "PhotoTsk.h"
#include "SieCapTsk.h"
#include "PhotoDisTsk.h"
#endif
#if (_WAVSTUDIO_MODE_ == ENABLE)
#include "WavStudioTsk.h"
#endif
#include "MediaPlayAPI.h"
#include "MediaRecAPI.h"
#include "KeyScanTsk.h"
#include "ide.h"
//#NT#20091022#ethanlau -begin
#include "PhotoDisplayTsk.h"
//#NT#20091022#ethanlau -end
#if (_MOVIE_PIM_MODE_ == ENABLE)
#include "RawEncApi.h"
#endif
//#NT#2009/11/23#Linkin Huang -begin
//#NT# add for simulation tool
#if (_MSDCVENDOR_ == _MSDCVENDOR_NVT_)
#include "ImageQualitySim.h"
#endif
#include "GPS.h"
//#NT#2009/11/23#Linkin Huang -end

/**
    @addtogroup mIPRJAPCfg
*/
//@{

/**
  Initialize application/device.

  Primary task will call this API when try to change different mode.
  Here we do devic control and initialize application and UI.
  The flow should be the following:
  1. Initialize device
  2. Initialize application and open application
  3. Initialize UI and open UI

  @param[in] uiMode       Which mode to open
  @return void
*/
void AppInit_Open(UINT32 uiMode)
{
    static UINT32 uiLastMode = PRIMARY_MODE_PHOTO;
    static BOOL   bzFolderInit = FALSE;


    debug_err(("AppInit_Open(%d) +++\r\n", uiMode));

    if (uiLastMode == PRIMARY_MODE_USBMSDC ||
       uiLastMode == PRIMARY_MODE_USBPCC)
    {
        DevCtrl_ModeUSBRemove();
    }
#if (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
        if (bzFolderInit==FALSE)
        {
            bzFolderInit=TRUE;
            FilesysWaitInitFinish(FST_TIME_INFINITE);
            FilesysWaitCmdFinish(FST_TIME_INFINITE);
            USERFolder_set();
        }
#endif

    uiLastMode = uiMode;

    switch (uiMode)
    {
    case PRIMARY_MODE_PHOTO:
		#if (_IQ_MODE_ == ENABLE)
        //--------------------------------------------------------------------------
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Note !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // The flow should be 1. DevCtrl 2. AppInit 3. UIFlow
        // But the DevCtrl will set image pipeline to new mode, and the sensor
        // library require some golbal variables to set the correct memory address.
        // The variables are assigned in Photo_Open() API now. we have to run
        // AppInit before DevCtrl now!
        // The sensor library have to be modified. All modes that need sensor will
        // have the same situation.
        // Ex: Photo, ASF/AVI recording, PCC mode...
        //--------------------------------------------------------------------------

        // Init Photo and Capture task and open
        AppInit_ModePhoto();
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModePhoto();
        // Open UI Flow Photo
        FlowPhoto_Open();
		#endif
        break;

    case PRIMARY_MODE_SETUP:
        AppInit_ModeSetup();
        break;

    case PRIMARY_MODE_ENGINEER:
		#if (_CALIBRATION_MODE_ == ENABLE)
		// Init Calibration function
        Cal_Open();
        // Open Menu Flow Calibration
        MenuCalibration_Open();
        //MenuPCB_Open();
		#endif
        break;

    case PRIMARY_MODE_PLAYBACK:
		#if (_PLAYBACK_MODE_ == ENABLE)
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModePlayback();
        // Init playback task and open
        AppInit_ModePlayback();
        // Open UI Flow Playback
        FlowPB_Open();
		#endif
        break;

    case PRIMARY_MODE_USBMSDC:
		#if (_USBMSDC_MODE_ == ENABLE)
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeUSBMSDC();
        // Init USB MSDC task and open
        AppInit_ModeUSBMSDC();
		#endif
        break;

    case PRIMARY_MODE_USBSICD:
		#if (_USBSICD_MODE_ == ENABLE)
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeUSBSICD();
        // Init USB SICD task and open
        AppInit_ModeUSBSICD();
        // Open Menu Flow Print
        MenuPrint_Open();
		#endif
        break;

    case PRIMARY_MODE_USBPCC:
		#if (_USBPCC_MODE_ == ENABLE)
        // Init USB PCC task and open
        AppInit_ModeUSBPCC();
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeUSBPCC();
		#endif
        break;

    case PRIMARY_MODE_MUSIC:
        #if (_MP3_MODE_ == ENABLE)
        DevCtrl_ModeMp3();
        AppInit_ModeMp3();
        FlowMP3_Open();
        #endif
        break;

    case PRIMARY_MODE_MOVIE:
        // Init movie mode application
        AppInit_ModeMovie();
        // Control CCD/LCD power on/off, lens, shutter...
        DevCtrl_ModeMovie();

        // Open UI Flow Movie
        FlowMovie_Open();
        break;

    case PRIMARY_MODE_CUSTOM1:
        MenuUSB_Open();
        break;

    case PRIMARY_MODE_CUSTOM2:
        #if (_USBWRITE_MODE_ == ENABLE)
        DevCtrl_ModeCustom2();
        AppInit_ModeCustom2();
        #endif
        break;

    case PRIMARY_MODE_DUMMY:
        //ide_disable_video(IDE_VIDEOID_1);
        DevCtrl_ModeDummy();
        break;

    default:
        debug_wrn(("Unknown operation mode\r\n"));
        break;
    }
    debug_err(("AppInit_Open(%d) ---\r\n", uiMode));
}

/**
  Close application

  Primary task will call this API when try to change different mode.
  Add all the tasks that you want to close when changing mode.

  @param void
  @return void
*/
void AppInit_Close(void)
{
    debug_err(("AppInit_Close +++\r\n"));

    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CHGMOD_PRESS, 0);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, 0);

    /* Flush Cmd to avoid key-in-queue comes into UI while switching mode */
    Ux_FlushEvent();

    /* All UI flow close should be further than App close!!! */
    /* All UI flow close should be further than App close!!! */
    /* All UI flow close should be further than App close!!! */

    /* Close media */
    FlowMovie_Close();

    MediaRec_Close();
    MediaPlay_Close();
    //#NT#2010/03/17#Meg Lin -begin
    //fixbug: after playing finished, should not mute
    hdmitx_setAudMute(FALSE);
    //#NT#2010/03/17#Meg Lin -end

    #if (_MOVIE_PIM_MODE_ == ENABLE)
    RawEnc_Close();
    #endif

    /* Close photo */
    FlowPhoto_Close();

    #if (_IQ_MODE_ == ENABLE)
    PhotoDisplay_Close();
    Photo_Close();
    ImgCapture_Close();
    PhotoDis_Close();
    #endif

    /* Close playback */
    FlowPB_Close();

    #if (_PLAYBACK_MODE_ == ENABLE)
    PB_Close(PB_WAIT_INFINITE);
    #endif

    #if (_WAVSTUDIO_MODE_ == ENABLE)
    WavStudio_Close();
    #endif

    /* Close mp3 */
    FlowMP3_Close();

    #if (_MP3_MODE_ == ENABLE)
    MP3PlayClose();
    if(GetCardStatus() == CARD_REMOVED)
    {
        MP3PlayFileRead_Close_NoCloseFile();
    }
    else
    {
        MP3PlayFileRead_Close();
    }
    #endif

    /* Close USB */
    MenuPrint_Close();
    MenuUSB_Close();

    #if ((_USBMSDC_MODE_ == ENABLE) && (_MSDCVENDOR_ != _MSDCVENDOR_NVT_))
    //#NT#2009/07/30#Niven Cho -begin
    //#NT#Modified., for MSDCExt with NVT
    //If Running in MSDC_NVT Mode, Don't Disconnect the USB
    USBMSDC_Close();
    //#NT#2009/07/30#Niven Cho -end
    #endif

    #if (_USBSICD_MODE_ == ENABLE)
    //#NT#2009/03/24#Marx Chiu -begin
    //MarxChiu_20090324 usbsicd_close();
    Usicd_Close();
    //#NT#2009/03/24#Marx Chiu -end
    #endif

    #if (_USBPCC_MODE_ == ENABLE)
    UPccClose();
    #endif

    /* Close calibration */
    #if (_CALIBRATION_MODE_ == ENABLE)
    //MenuCalibration_Close();
    //MenuPCB_Close();
    #endif

    #if(_GPS_RECEIVER_ == ENABLE)
    GPSRec_Close();
    #endif


    /* Flush Cmd to avoid key-in-queue comes into UI while switching mode */
    Ux_FlushEvent();

    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CHGMOD_PRESS , FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS , FLGKEY_KEY_MASK_DEFAULT);

    debug_err(("AppInit_Close ---\r\n"));
}

//@}
