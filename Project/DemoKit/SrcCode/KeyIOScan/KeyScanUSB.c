/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanUSB.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect USB is plugging in or unplugged
                Detect USB is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "USB.h"
#include "KeyScanInt.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "FilesysTsk.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIGraphics.h"
#include "MSDCVendorSample.h"
#define  USB_SKIP_POWEROFF_COUNT        10 // 1000 ms (9 KeyScan_DetPoweroff time + 1 debounce )
UINT32   bUSBDetectedSkipPowerCount     = 0;
volatile BOOL    g_bKeyScanUSBDetEn = TRUE;
static volatile BOOL    g_bUSBPlug =  FALSE;    //#NT#2007/08/21#JaniceHuang::when usb dectect skip low battery check,for HW printer bug
static volatile BOOL    g_bUSBPower=  FALSE;

extern BOOL    gUIAviIsRecording;
BOOL gbUSNMenuOpen = FALSE;

/**
  Detect USB is plugging in or unplugged

  Detect USB is plugging in or unplugged and change corresponding mode
  [KeyScan internal API]

  @param void
  @return void
*/
BOOL KeyScan_IsUSBPower(void);
void KeyScan_SetUSBPower(BOOL bEn);

void KeyScan_DetUSB(void)
{
    static BOOL bLastUSBDet     = FALSE;
    static BOOL bLastUSBStatus  = FALSE;
    BOOL        bCurUSBDet, bCurUSBStatus;

    //2009/03/16 Janice::USB would not redear ,need to wait UIframework
    if (!Ux_IsInitMessage())
    {
        return ;
    }
    if ((KeyScan_IsTVPlugIn() ==TRUE) || (KeyScan_IsHDMIPlugIn()==TRUE))
    {
         return;
    }
    bCurUSBDet = usb_StateChange();

    bCurUSBStatus = (BOOL)(bCurUSBDet & bLastUSBDet);

    if (bCurUSBStatus != bLastUSBStatus)
    {
        // USB plugging in
        if (bCurUSBStatus == TRUE)
        {
            debug_err(("USB is plugged......\r\n"));

            bUSBDetectedSkipPowerCount =  USB_SKIP_POWEROFF_COUNT;
            //#NT#2007/08/08#JaniceHuang:: avoid HW bug,plug USB would power btn would detect,need to skip detect pwr off.
            //g_bKeyScanUSBDetEn move here after setting bUSBDetectedSkipPowerCount,
            // g_bKeyScanUSBDetEn had better in keyscantsk ,before KeyScan_DetUSB,but cannot now
            if(!g_bKeyScanUSBDetEn)
                return;
            g_bUSBPlug =  TRUE;

            KeyScan_EnableAutoPoweroff(FALSE);
            //#NT#2007/07/27#Stan Lin -begin
            //KeyScan_ResetSleep();
            //KeyScan_EnableSleep(FALSE);
            //#NT#2007/07/27#Stan Lin -end
            //check filesys, if busy skip USB dectect ##can not wait inifinit,would lock keyscantsk
            //donot change bCurUSBStatus for dectect again,until FST not busy
            //#NT#2007/07/28@Steven Wang begin
            //Fix usb plug while wait moment window is executing
            if (FilesysWaitCmdFinish(FST_TIME_NO_WAIT) == FST_STA_BUSY || Ux_GetBackgroundTskBusy() == TRUE)
            {
                if(Ux_GetBackgroundTskExeuteCommand() == NVTEVT_BKW_WAIT_PTR_INIT && FilesysWaitCmdFinish(FST_TIME_NO_WAIT) != FST_STA_BUSY)
                {
                    debug_err(("UIBackgroundReset ... while print init\r\n"));
                    Ux_ResetBackgroundTsk();
                }
                else
                {
                    debug_err(("^R UIBackground busy\r\n"));
                    return;
                }
            }
            //#NT#2007/07/28@Steven Wang end
            //Fix power on and plug in USB would multi enter change mode
            if(Primary_CheckIdle(PRI_TIME_NO_WAIT) != PRI_STA_IDLE)
            {
                return; //return would not update bLastUSBDet ,next time would detect again
            }

            // You can change to MODE_CUSTOM2 instead of select USB mode
            // to speed up the writing progress.
            // To do this, unmark the following code and mark "Primary_ChangeMode(PRIMARY_MODE_CUSTOM1);"
            //Primary_ChangeMode(PRIMARY_MODE_CUSTOM2);
#if (0)
            Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
#else

            // for USB timeout check
            if (usb_open()==E_TMOUT)
            {
                debug_msg("USB is connected to charger\r\n");        
                bLastUSBDet     = bCurUSBDet;
                bLastUSBStatus  = bCurUSBStatus;
                KeyScan_SetUSBPower(TRUE);
				
                usb_close();
		   if(Primary_GetCurrentMode() == PRIMARY_MODE_MOVIE)
		   {
				if (gUIAviIsRecording == FALSE)
				{
                                Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);					
				}
		   }
                return;
            }
            usb_close();
            KeyScan_SetUSBPower(FALSE);
			
//#NT#2009/10/12#Winston Li -begin
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);  // need to enter preview mode firstly, why?
            TimerDelayMs(200);
//#NT#2009/10/12#Winston Li -end

            //janice for screen ¸õ°Ê and ´Ý¼v
            UI_Set_NotFlip();
            // Open USB mode select task
            // USB mode select task will disable modedial detection
            //#NT#2009/07/30#Niven Cho -begin
            //#NT#Added., for MSDCExt with NVT
            #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
            /*
            //------------------------------------------------------------------------------
            // Single Module Selection
            //------------------------------------------------------------------------------
            #define SI_MODULE_NONE          0  //!< No Single Module
            #define SI_MODULE_CUSTOM_SI     1  //!< Customer Develop Module
            #define SI_MODULE_IQSIM         2  //!< Image Quality Simulation Tool
            #define SI_MODULE_TIME_SI       3  //!< PC Time Sync
            */
                if (MsdcNvt_GetSIModule()==1)
                {
                    MSDCVendorSample_Open();
                } else {
                    Primary_ChangeMode(PRIMARY_MODE_CUSTOM1);  //change to USB menu janice
                }
            #else
            Primary_ChangeMode(PRIMARY_MODE_CUSTOM1);  //change to USB menu janice
            #endif
            Primary_Wait4ChangeModeDone();
#endif
        }
        // USB unplugged
        else
        {
            debug_err(("USB is unplugged......\r\n"));

            //#2007/08/17#JaniceHuang::do power off while unplug
            //KeyScan_PoweroffFlow();

            // Enable modedial detection
            KeyScan_EnableMDDet(TRUE);
            if((Primary_GetCurrentMode() == PRIMARY_MODE_USBMSDC)||(gbUSNMenuOpen == TRUE))
            	{
			KeyScan_PoweroffFlow();
		}
	      else
	      	{
            //KeyScan_EnableAutoPoweroff(TRUE);
            //KeyScan_SetAutoPoweroffTime(10);	//jack lan 
            	   KeyScan_SetUSBPower(FALSE);            
	      	}
            KeyScan_EnableKeyDet(TRUE);

            // Detect modedial and force change mode
            // Unnecessary here
            // KeyScan_DetModedial(TRUE);
            // #NT#2007/07/26@StevenWang begin
            // When USB un plug, using Get previous mode to back to previous mode
            // uiPreModeID = Primary_GetPreviousMode();
            // #NT#2007/07/31@Janice begin -- always back to photo mode
            //#NT#2009/07/30#Niven Cho -begin
            //#NT#Added., for MSDCExt with NVT
            #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
            MSDCVendorSample_Close();
            #endif
            //#NT#2009/07/30#Niven Cho -end
            //Primary_ChangeMode(PRIMARY_MODE_PHOTO);
            // #NT#2007/07/26@StevenWang end
        }
    }

    bLastUSBDet     = bCurUSBDet;
    bLastUSBStatus  = bCurUSBStatus;
}

/**
  Enable/Disable USB detection

  Enable/Disable USB detection

  @param BOOL bEn: TRUE -> Enable USB detection, FALSE -> Disable USB detection
  @return void
*/
void KeyScan_EnableUSBDet(BOOL bEn)
{
    g_bKeyScanUSBDetEn = bEn;
}

BOOL KeyScan_IsUSBPlug()
{
    return g_bUSBPlug;
}

//#NT#2011/11/21@Philex Lin - begin
void KeyScan_SetUSBPower(BOOL bEn)
{
    g_bUSBPower = bEn;
}

BOOL KeyScan_IsUSBPower(void)
{
    return g_bUSBPower;
}

BOOL KeyScan_CheckUSBInSysInit(void)
{
    if (usb_StateChange())
    {
        // Is USB charger ???
        // for USB timeout check
        if (usb_open()==E_TMOUT)
        {
            debug_msg("USB is connected to charger\r\n");
            usb_close();
            return FALSE;
        }
        else
        {
            usb_close();
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }

    //return (usb_StateChange());
}

//@}
