/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       BurnIn.c
    @ingroup    mIPRJAPTest

    @brief      BurnIn process
                This file is the BurnIn process

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <string.h>
#include <stdio.h>
#include "Kernel.h"
#include "CalBurnIn.h"
#include "GlobalVar.h"
#include "Timer.h"
#include "KeyScanTsk.h"
#include "PrimaryTsk.h"
#include "VoltageDet.h"
#include "CalibrationTsk.h"

#if (_CALIBRATION_MODE_ == ENABLE)

typedef __packed struct
{
    UINT32      uiKeyFlagID;
    UINT32      uiKeyEvent;
    UINT16      uiInterval;
    UINT8       uiRepeatCnt;
    UINT8       uiKeyStatus;
} SIM_EVENT, *PSIM_EVENT;

#define TIMER_INTERVAL              50
#define FLASH_TIMEOUT               30000       // 30 Sec
#define WAITING_TIMEOUT             60000       // 60 Sec

#define SIM_END                     0xFFFFFFFF

#define KEY_STATUS_PRESS            0
#define KEY_STATUS_RELEASE          1

#define BURNIN_OP_BURNIN            0
#define BURNIN_OP_ZOOMAF            1
#define BURNIN_OP_FLASH             2
#define BURNIN_OP_168H              3

#define FLG_ID_BURNIN               FLG_ID_MP3
#define FLGBURNIN_TIMER             0x00000001
#define FLGBURNIN_WAITING           0x00000002
#define FLGBURNIN_TIMEOUT           0x00000004
#define FLGBURNIN_MODE_PHOTO        0x00000008
#define FLGBURNIN_MODE_ASF          0x00000010
#define FLGBURNIN_MODE_PLAYBACK     0x00000020
#define FLGBURNIN_MODE_SETUP        0x00000040
#define FLGBURNIN_ALL               0x0000007F

static RECT                     BurnInStringRect = { 64, 192, 256, 24 };
static UINT32                   uiIteration;
static volatile UINT32          uiTimeoutValue, uiTimerCnt;

const SIM_EVENT BurnInEvent[] =
{
    // Set to Tool mode
    {FLG_ID_BURNIN, FLGBURNIN_MODE_SETUP, 500, 1, KEY_STATUS_PRESS},           //  1
    // Delay 1 sec then format SD
    {FLG_ID_KEY, FLGKEY_DOWN, 1000+500, 1, KEY_STATUS_PRESS},               //  2
    {FLG_ID_KEY, FLGKEY_ENTER, 500, 1, KEY_STATUS_PRESS},                   //  3
    {FLG_ID_KEY, FLGKEY_UP, 500, 1, KEY_STATUS_PRESS},                      //  4
    {FLG_ID_KEY, FLGKEY_ENTER, 500, 1, KEY_STATUS_PRESS},                   //  5
    // Wait for formating
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               //  6
    // Set to Manual mode
    {FLG_ID_BURNIN, FLGBURNIN_MODE_PHOTO, 500, 1, KEY_STATUS_PRESS},   //  7
    // Delay 1 sec then set Flash mode to No Flash
    {FLG_ID_KEY, FLGKEY_LEFT, 1000+500, 1, KEY_STATUS_PRESS},               //  8
    {FLG_ID_KEY, FLGKEY_DOWN, 200, 1, KEY_STATUS_PRESS},                    //  9
    {FLG_ID_KEY, FLGKEY_ENTER, 200, 2, KEY_STATUS_PRESS},                   // 10
    /*
    // Delay 1 sec then set AV F2.8
    {FLG_ID_KEY, FLGKEY_ENTER, 1000+500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_LEFT, 200, 5, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_ENTER, 500, 1, KEY_STATUS_PRESS},
    */
    // Delay 1 sec then Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1000+500, 1, KEY_STATUS_PRESS},           // 11
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},               // 12
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},              // 13
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 14
    // Tele
    {FLG_ID_KEY, FLGKEY_ZOOMIN, 500, 1, KEY_STATUS_PRESS},                  // 15
    {FLG_ID_KEY, FLGKEY_ZOOMIN, 4000, 1, KEY_STATUS_RELEASE},               // 16
    // Delay 1 sec then Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1000+500, 1, KEY_STATUS_PRESS},           // 17
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},               // 18
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},              // 19
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 20
    // Wide
    {FLG_ID_KEY, FLGKEY_ZOOMOUT, 500, 1, KEY_STATUS_PRESS},                 // 21
    {FLG_ID_KEY, FLGKEY_ZOOMOUT, 4000, 1, KEY_STATUS_RELEASE},              // 22
    // Delay 1 sec than set Flash mode to Auto Flash
    {FLG_ID_KEY, FLGKEY_LEFT, 1000+500, 1, KEY_STATUS_PRESS},               // 23
    {FLG_ID_KEY, FLGKEY_UP, 200, 1, KEY_STATUS_PRESS},                      // 24
    {FLG_ID_KEY, FLGKEY_ENTER, 200, 2, KEY_STATUS_PRESS},                   // 25
    // Delay 1 sec then set AV F6.7
    {FLG_ID_KEY, FLGKEY_ENTER, 1000+500, 1, KEY_STATUS_PRESS},              // 26
    {FLG_ID_KEY, FLGKEY_RIGHT, 500, 5, KEY_STATUS_PRESS},                   // 27
    {FLG_ID_KEY, FLGKEY_ENTER, 500, 1, KEY_STATUS_PRESS},                   // 28
    // Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 500, 1, KEY_STATUS_PRESS},                // 29
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},               // 30
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},              // 31
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 32
    // Set self timer to 10+2
    {FLG_ID_KEY, FLGKEY_RIGHT, 500, 1, KEY_STATUS_PRESS},                   // 33
    {FLG_ID_KEY, FLGKEY_DOWN, 200, 3, KEY_STATUS_PRESS},                    // 34
    {FLG_ID_KEY, FLGKEY_ENTER, 200, 2, KEY_STATUS_PRESS},                   // 35
    // Delay 1 sec then Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1000+500, 1, KEY_STATUS_PRESS},           // 36
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},               // 37
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},              // 38
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 39
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 40
    // Set to Playback mode
    {FLG_ID_KEY, FLGBURNIN_MODE_PLAYBACK, 500, 1, KEY_STATUS_PRESS},                // 41
    // Wait for preview
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 42
    // Delete
    {FLG_ID_KEY, FLGKEY_DEL, 500, 1, KEY_STATUS_PRESS},                     // 43
    {FLG_ID_KEY, FLGKEY_ENTER, 500, 2, KEY_STATUS_PRESS},                   // 44
    // Wait for deleting
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 45
    // Set to Movie mode
    {FLG_ID_BURNIN, FLGBURNIN_MODE_ASF, 500, 1, KEY_STATUS_PRESS},          // 46
    // Delay 1 sec then Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1000+500, 1, KEY_STATUS_PRESS},           // 47
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},               // 48
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},              // 49
    // Delay 10 sec for recording ASF then shutter2 (stop)
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 10000+500, 1, KEY_STATUS_PRESS},          // 50
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 51
    // Set to Playback mode
    {FLG_ID_KEY, FLGBURNIN_MODE_PLAYBACK, 500, 1, KEY_STATUS_PRESS},                // 52
    // Wait for preview
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},                               // 53
    // Play movie
    {FLG_ID_KEY, FLGKEY_ENTER, 500, 1, KEY_STATUS_PRESS},                   // 54
    // 10 sec for ASF playback and end (+1S)
    {SIM_END, 0, 10000+1000, 0, 0}                                          // 55
};

const SIM_EVENT ZoomAFEvent[] =
{
    // Wide
    {FLG_ID_KEY, FLGKEY_ZOOMOUT, 500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_ZOOMOUT, 4000, 1, KEY_STATUS_RELEASE},
    // Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},
    // Tele
    {FLG_ID_KEY, FLGKEY_ZOOMIN, 500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_ZOOMIN, 4000, 1, KEY_STATUS_RELEASE},
    // Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 200, 1, KEY_STATUS_RELEASE},
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},
    // End
    {SIM_END, 0, 0, 0, 0}
};

const SIM_EVENT FlashEvent[] =
{
    // Shutter1 -> Shutter2
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 500, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER2, 2000, 1, KEY_STATUS_PRESS},
    {FLG_ID_KEY, FLGKEY_SHUTTER1, 1500, 1, KEY_STATUS_RELEASE},
    // Wait for saving
    {FLG_ID_BURNIN, FLGBURNIN_WAITING, 0, 0, 0},
    // End
    {SIM_END, 0, 0, 0, 0}
};

/**
  Show BurnIn loop number

  Show BurnIn loop number
  [CalBurnIn internal API]

  @param UINT32 uiLoop: Loop number
  @param UINT32 uiItem: Item number
  @return void
*/
static void Cal_BurnInShowLoop(UINT32 uiLoop, UINT32 uiItem)
{
    INT8 StringBuf[32];

    sprintf(StringBuf, "Loop=%ld,Item=%ld", uiLoop, uiItem);

    Cal_ShowString(StringBuf, &BurnInStringRect, TRUE);
}

/**
  BurnIn timer ISR

  BurnIn timer ISR
  [CalBurnIn internal API]

  @param UINT32 uiLoop: Loop number
  @param UINT32 uiItem: Item number
  @return void
*/
static void Cal_BurnInTimerISR(void)
{
    UINT32 uiFlag;

    uiFlag = FLGBURNIN_TIMER;
    if (uiTimeoutValue != 0)
    {
        uiTimerCnt += TIMER_INTERVAL;
        if (uiTimerCnt >= uiTimeoutValue)
        {
            uiFlag |= FLGBURNIN_TIMEOUT;
        }
    }
    set_flg(FLG_ID_BURNIN, uiFlag);
}

/**
  BurnIn main loop

  BurnIn main loop
  [CalBurnIn internal API]

  @param UINT32 uiOperation: Which operation you want to run
    BURNIN_OP_BURNIN            (Burn In)
    BURNIN_OP_ZOOMAF            (ZoomAF Lasting)
    BURNIN_OP_FLASH             (Flash Lasting)
    BURNIN_OP_168H              (168H)
  @return void
*/
static void Cal_BurnIn(UINT32 uiOperation)
{
    //#NT#2009/09/01#KS Hung -begin
    FLGPTN      uiFlag;
    UINT32      uiTimerID;
    //#NT#2009/09/01#KS Hung -end
    UINT32      uiKeyFlagID, uiKeyEvent;
//    UINT32      uiOldPressConfig, uiOldReleaseConfig;
    UINT32      uiTotalIteration;
    UINT32      uiInterval;
    UINT32      uiEventCnt;
    PSIM_EVENT  pSimEvent, pStartEvent;
    UINT8       uiKeyStatus, uiRepeatCnt;


    // Disable scan key function of KeySacn task
    /*
    uiOldPressConfig = get_scankey_press_config();
    uiOldReleaseConfig = get_scankey_release_config();
    set_scankey_press_config(0x00);
    set_scankey_release_config(0x00);
    */

    // Open timer
    if (timer_open((UINT *)&uiTimerID, Cal_BurnInTimerISR) != E_OK)
    {
        /*
        set_scankey_press_config(uiOldPressConfig);
        set_scankey_release_config(uiOldReleaseConfig);
        */
        return;
    }

    // Set image size to VGA, quality to economy and Drive mode to single;
    /*
    gMenuPhotoInfo.uiIMGSize        = IMGSIZE_VGA;
    gMenuPhotoInfo.uiQuality        = IMGQUAL_ECONOMY;
    gMenuPhotoInfo.uiDriveMode      = IMGDRIVE_SINGLE;
    gMenuSetupInfo.uiAutoPowerOff   = AUTOPOWEROFF_OFF;

    Set_ImageInfo_IMGSize(gMenuPhotoInfo.uiIMGSize);
    Set_ImageInfo_Quality(gMenuPhotoInfo.uiQuality);
    Set_ImageInfo_DriveMoce(gMenuPhotoInfo.uiDriveMode);
    */

    switch (uiOperation)
    {
    case BURNIN_OP_BURNIN:
        uiTotalIteration    = 30;
        pStartEvent         = (PSIM_EVENT)&BurnInEvent;
        break;

    case BURNIN_OP_ZOOMAF:
        uiTotalIteration    = 1000;
        pStartEvent         = (PSIM_EVENT)&ZoomAFEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        break;

    case BURNIN_OP_FLASH:
        uiTotalIteration    = 500;
        pStartEvent         = (PSIM_EVENT)&FlashEvent;
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        break;

    case BURNIN_OP_168H:
        uiTotalIteration    = 0xFFFFFFFF;
        pStartEvent         = (PSIM_EVENT)&BurnInEvent;
        break;
    }

    // Init variable
    uiTimeoutValue  = 0;

    // Enable timer and clear all flags
    clr_flg(FLG_ID_BURNIN, FLGBURNIN_ALL);
    timer_set(uiTimerID, TIMER_INTERVAL, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

    for (uiIteration=1; uiIteration<=uiTotalIteration; uiIteration++)
    {
        Cal_BurnInShowLoop(uiIteration, uiEventCnt);
        debug_ind(("\n\r ***Loop = %ld,Event= %ld ***\r\n", uiIteration, uiEventCnt));
        pSimEvent   = pStartEvent;
        uiRepeatCnt = 0;
        uiInterval  = 0;
        uiEventCnt  = 0;
        while (1)
        {
            if (pSimEvent == pStartEvent)
            {
                switch (uiOperation)
                {
                case BURNIN_OP_BURNIN:
                    // Set Flash Auto, disable Ditital Zoom and set AV F2.8
                    /*
                    gMenuPhotoInfo.uiFlashMode      = IMGFLASH_AUTO;
                    gMenuPhotoInfo.uiISDigitalZoom  = IMGDZ_OFF;
                    gMenuPhotoInfo.uiFValue         = 0;

                    Set_ImageInfo_FlashMode(gMenuPhotoInfo.uiFlashMode);
                    MenuOff_ShowFlash(gMenuPhotoInfo.uiFlashMode);

                    Set_ImageInfo_FValue(gMenuPhotoInfo.uiFValue);
                    */
                    break;

                case BURNIN_OP_ZOOMAF:
                    // Disable Flash and Ditital Zoom
                    /*
                    gMenuPhotoInfo.uiFlashMode      = IMGFLASH_FORCEOFF;
                    gMenuPhotoInfo.uiISDigitalZoom  = IMGDZ_OFF;

                    Set_ImageInfo_FlashMode(gMenuPhotoInfo.uiFlashMode);
                    MenuOff_ShowFlash(gMenuPhotoInfo.uiFlashMode);
                    */
                    break;

                case BURNIN_OP_FLASH:
                    // Enable Flash and disable Ditital Zoom
                    /*
                    gMenuPhotoInfo.uiFlashMode      = IMGFLASH_FORCEON;
                    gMenuPhotoInfo.uiISDigitalZoom  = IMGDZ_OFF;

                    Set_ImageInfo_FlashMode(gMenuPhotoInfo.uiFlashMode);
                    MenuOff_ShowFlash(gMenuPhotoInfo.uiFlashMode);
                    */
                    break;

                default:
                    break;
                }
            }

            wai_flg((FLGPTN *)&uiFlag, FLG_ID_BURNIN, FLGBURNIN_TIMER, TWF_ORW | TWF_CLR);

            uiInterval += TIMER_INTERVAL;
            if (uiInterval >= pSimEvent->uiInterval)
            {
                /*
                if (pSimEvent->uiKeyEvent == FLGKEY_SHUTTER2 &&
                    gMenuPhotoInfo.uiFlashMode != IMGFLASH_FORCEOFF &&
                    gucLastDialMode != FLGOPMODE_MOVIE &&
                    gucLastDialMode != FLG_BURNIN_SETUP &&
                    VolDetGetFlashLevel() == FLASH_LEVEL_EMPTY)
                {
                    if (uiInterval < ((TIMER_INTERVAL << 1) + pSimEvent->uiInterval))
                    {
                        debug_wrn(("Wait for flash voltage to be full\r\n"));
                    }

                    // Check timeout
                    if (uiInterval >= (FLASH_TIMEOUT + pSimEvent->uiInterval))
                    {
                        debug_err(("Wait for flash voltage to be full timeout\r\n"));
                    }
                    else
                    {
                        continue;
                    }
                }
                */

                uiInterval = 0;

                // Wait for specific flag, or change mode
                if (pSimEvent->uiKeyFlagID == FLG_ID_BURNIN)
                {
                    if (pSimEvent->uiKeyEvent == FLGBURNIN_WAITING)
                    {
                        // Enable timeout
                        uiTimeoutValue  = WAITING_TIMEOUT;
                        uiTimerCnt      = 0;
                        clr_flg(FLG_ID_BURNIN, FLGBURNIN_TIMEOUT);

                        wai_flg((FLGPTN *)&uiFlag, FLG_ID_BURNIN, pSimEvent->uiKeyEvent | FLGBURNIN_TIMEOUT, TWF_ORW | TWF_CLR);

                        if (uiFlag & FLGBURNIN_TIMEOUT)
                        {
                            debug_err(("Wait for specific flag timeout\r\n"));
                        }

                        // Disable timeout
                        uiTimeoutValue = 0;

                        // Clear timer flag
                        clr_flg(FLG_ID_BURNIN, FLGBURNIN_TIMER);

                        uiRepeatCnt = 0;
                        pSimEvent++;
                        uiEventCnt++;
                        debug_ind(("\n\r ***Loop = %ld,Event= %ld ***\r\n", uiIteration, uiEventCnt));
                        continue;
                    }
                    else
                    {
                        switch (pSimEvent->uiKeyEvent)
                        {
                        case FLGBURNIN_MODE_PHOTO:
                            Primary_ChangeMode(PRIMARY_MODE_PHOTO);
                            break;

                        case FLGBURNIN_MODE_ASF:
                            Primary_ChangeMode(PRIMARY_MODE_MOVIE);// ther is no PRIMARY_MODE_ASF);
                            break;
                        case FLGBURNIN_MODE_PLAYBACK:
                            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
                            break;
                        case FLGBURNIN_MODE_SETUP:
                            Primary_ChangeMode(PRIMARY_MODE_SETUP);
                            break;
                        }
                    }
                }

                // End of event
                if (pSimEvent->uiKeyFlagID == SIM_END)
                    break;

                uiKeyFlagID = pSimEvent->uiKeyFlagID;
                uiKeyEvent  = pSimEvent->uiKeyEvent;
                uiKeyStatus = pSimEvent->uiKeyStatus;
                uiRepeatCnt++;

                // Move to next event
                if (uiRepeatCnt == pSimEvent->uiRepeatCnt)
                {
                    uiRepeatCnt = 0;
                    pSimEvent++;
                    uiEventCnt++;
                    debug_ind(("******************Loop = %ld,Event= %ld ***************\r\n", uiIteration, uiEventCnt));
                }

                // Set key event
                /*
                if (uiKeyFlagID != NULL && uiKeyEvent != NULL)
                {
                    if (uiKeyStatus == KEY_STATUS_PRESS)
                    {
                        set_key_status(uiKeyEvent);
                    }
                    else
                    {
                        set_key_status(~uiKeyEvent);
                    }
                    set_flg(uiKeyFlagID, uiKeyEvent);
                }
                */
            }
            Cal_BurnInShowLoop(uiIteration, uiEventCnt);

        }
    }

    timer_close(uiTimerID);
    /*
    set_scankey_press_config(uiOldPressConfig);
    set_scankey_release_config(uiOldReleaseConfig);
    */

    // Power off DSC after burn in done
    // Premier don't want to power off now (2005.10.16)
    /*
    if (uiOperation == BURNIN_OP_BURNIN)
    {
        PowerOffFlow(TRUE, FALSE);
    }
    */
}

/**
  Get BurnIn iteration (loop number)

  Get BurnIn iteration (loop number)

  @param void
  @return UINT32: Iteration
*/
UINT32 Cal_BurnInGetIteration(void)
{
    return uiIteration;
}

/**
  Run burn in

  Run burn in

  @param void
  @return void
*/
void Cal_BurnInBurnIn(void)
{
    Cal_BurnIn(BURNIN_OP_BURNIN);
}

/**
  Run Zoom AF lasting

  Run Zoom AF lasting

  @param void
  @return void
*/
void Cal_BurnInZoomAF(void)
{
    Cal_BurnIn(BURNIN_OP_ZOOMAF);
}

/**
  Run Flash lasting

  Run Flash lasting

  @param void
  @return void
*/
void Cal_BurnInFlash(void)
{
    Cal_BurnIn(BURNIN_OP_FLASH);
}

/**
  Run 168H

  Run 168H

  @param void
  @return void
*/
void Cal_BurnIn168H(void)
{
    Cal_BurnIn(BURNIN_OP_168H);
}

#endif

//@}
