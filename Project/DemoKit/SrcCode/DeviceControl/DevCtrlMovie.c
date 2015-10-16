/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlMovie.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for movie mode
                Control LCD/LCD backlight on/off, CCD power on/off.
                Lens...

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "DeviceCtrl.h"
#include "GPIOMapping.h"
#include "ImageAlgInfor.h"
#include "ide.h"
#include "sie_lib.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "Alg_IPLMode.h"
#include "UIMenuMovieSettings.h"
/**
  Control the devices for Movie mode

  Control the devices before entering Movie mode.

  @param void
  @return void
*/
extern BOOL gbKscanTVPlug;

void DevCtrl_ModeMovie(void)
{
    IPL_OBJ IplObj;

    // Turn on CCD power
    if (GPIOMap_IsCCDPowerOn() == FALSE)
    {
        GPIOMap_TurnOnCCDPower();
    }
#if 0	
    if(Get_ImageRotateIndex() == IMAGE_ROTATE_OFF)
    {
    		UI_SetParameter(_ImageFlip,_Flip_Off);		
    }
    else
    {
		UI_SetParameter(_ImageFlip,_Flip_H_V);   		
    }
#endif	
    if (IPL_GetMode() != IPL_MODE_VIDEO)
    {
        // Disable IDE video1/video2 output
        ide_disable_video(IDE_VIDEOID_1);
        ide_disable_video(IDE_VIDEOID_2);

        IplObj.uiCmd    = IPL_CMD_CHG_MODE;
        IplObj.Mode     = IPL_MODE_VIDEO;
        IPL_SetCmd(&IplObj);

        PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);

        // Start zoom initialization
        Photo_OpticalZoom_InitStart();

        //#NT#20091018#ethanlau -begin
//use callback to open ide
        // Delay 2 frames to avoid seeing error picture
        //sie_waitVD(2);
        // Enable IDE video1 output
        //ide_enable_video(IDE_VIDEOID_1);
//#NT#20091018#ethanlau -end

        // Wait for zoom initialization done
        Photo_OpticalZoom_InitEnd();
    }

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE  &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        //GPIOMap_TurnOnLCDBacklight();
    }
    if(gbKscanTVPlug==FALSE){	
    KeyScan_TurnOnLED(KEYSCAN_LED_GREEN);
    KeyScan_TurnOffLED(KEYSCAN_LED_RED);	}
}

//@}
