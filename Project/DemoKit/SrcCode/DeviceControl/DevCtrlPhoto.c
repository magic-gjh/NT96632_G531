/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DevCtrlPhoto.c
    @ingroup    mIPRJAPCfg

    @brief      Control device for photo mode
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
#include "KeyScanTsk.h"
#include "SystemClock.h"
#include "UIMenuMovieSettings.h"


/**
  Control the devices for Photo mode

  Control the devices before entering Photo mode.

  @param void
  @return void
*/
void DevCtrl_ModePhoto(void)
{
    IPL_OBJ IplObj;
    // Turn on CCD power
    if (GPIOMap_IsCCDPowerOn() == FALSE)
    {
        GPIOMap_TurnOnCCDPower();
    }
		
    if(Get_ImageRotateIndex() == IMAGE_ROTATE_OFF)
    {
    		//UI_SetParameter(_ImageFlip,_Flip_Off);		
    }
    else
    {
		//UI_SetParameter(_ImageFlip,_Flip_H_V);   		
    }
	
    if (IPL_GetMode() != IPL_MODE_PREVIEW)
    {
        // Disable IDE video1/video2 output
        ide_disable_video(IDE_VIDEOID_1);
        ide_disable_video(IDE_VIDEOID_2);

        // Set to preview mode
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_PREVIEW;
        IPL_SetCmd(&IplObj);
        PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, TRUE);

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
        Lens_Shutter_Move(OPEN);
        Photo_Aperture_Move(0);
    }

    // Turn on LCD backlight if TV is un plugged
    if (GPIOMap_IsLCDBacklightOn() == FALSE  &&
        KeyScan_IsTVPlugIn() == FALSE &&
        KeyScan_IsHDMIPlugIn() == FALSE)
    {
        GPIOMap_TurnOnLCDBacklight();
    }

    //SysClk_SetClk(SPEED_ID_PHOTO);
}

//@}
