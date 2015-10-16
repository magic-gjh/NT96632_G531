/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIFlowPlaybackAVI.c
    @ingroup    mIPRJAPUIFlow

    @brief      Sample UI & Flow in Playback AVI mode for DSC system

    @note       Nothing.

    @date       2005/10/25
*/

/** \addtogroup mIPRJAPUIFlow */
//@{

#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include "KERNEL.h"
#include "SysCfg.h"
#include "PlaybackTsk.h"
#include "UART.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "MediaPlayAPI.h"
#include "UIPlayMov.h"
#include "NVTSysCommand.h"
#include "NVTUserCommand.h"
#include "VideoDecode.h"


static MEDIAPLAY_OBJ g_UIFlowMediaPlayObj;


UINT32 PlayMov_GetSpeedValue(UINT32 uiIndex)
{
    MEDIAPLAY_SPEEDUP_TYPE val;

    switch(uiIndex)
    {
        case PLAYMOV_SPEED_BWD_8X:
        case PLAYMOV_SPEED_FWD_8X:
            val = SPEED_H264_7_5X;
            break;
        case PLAYMOV_SPEED_BWD_4X:
        case PLAYMOV_SPEED_FWD_4X:
            val = SPEED_H264_3_75X;
            break;
        case PLAYMOV_SPEED_BWD_2X:
        case PLAYMOV_SPEED_FWD_2X:
            val = SPEED_H264_1_5X;
            break;
        case PLAYMOV_SPEED_FWD_1X:
        default:
            val = SPEED_NORMAL;
            break;
    }
    return val;
}

void PlayMov_PauseMedia(void)
{
    MediaPlay_Pause();
}

void PlayMov_ResumeMedia(void)
{
    UINT32 uiCurrSpeedIndex = Get_PlayMovSpeedIndex();
    UINT32 uiSpeedUp = PlayMov_GetSpeedValue(uiCurrSpeedIndex);

    MediaPlay_SpeedUp(uiSpeedUp);
    if(uiCurrSpeedIndex >= PLAYMOV_SPEED_FWD_1X)
    {
        MediaPlay_FWD();
    }
    else
    {
        MediaPlay_BWD();
    }

    if(uiSpeedUp == SPEED_NORMAL)
    {
        MediaPlay_WaitFFBW1XFinish();
        hdmitx_setAudMute(FALSE);//2010/03/16 Meg
    }
    else
    {
        MediaPlay_WaitStepFinish();
    }
}

void PlayMov_StopMedia(void)
{
    PLAY_OBJ    PlayObj;

    MediaPlay_Stop();
    MediaPlay_Close();
    //#NT#2010/03/17#Meg Lin -begin
    //fixbug: after playing finished, should not mute
    hdmitx_setAudMute(FALSE);
    //#NT#2010/03/17#Meg Lin -end
    PlayObj.ui1stPlayMode = PLAYMODE_MOVMJPG;
    PB_Open(&PlayObj);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
}

void PlayMov_PlayMedia(MovPlayCb movPlayCB)
{
    UINT32      y1, y2, cb1, cb2, cr1, cr2;
    PLAY_OBJ    PlayObj;
    ER          openMedia = E_OK;
    PPLAY_OBJ   pPlayObj;
    UINT32      jumpFFLevel = 0;
    UINT32      jumpBWLevel = 0;
    MEDIA_PLAY_1ST_INFO  play1stParam;
    VIDEO_DECODER *pDecobj;
    //----- Draw " >>>   0 " -----------------------
    debug_err(("enter MediaPlayFunc\r\n"));

    Set_PlayMovSpeedIndex(PLAYMOV_SPEED_FWD_1X);

    MediaPlay_Close();
    PB_Close(PB_WAIT_INFINITE);

    pPlayObj = PB_GetPBObject();

    g_UIFlowMediaPlayObj.uiMemAddr = PB_GetCurrDataBufAddr()+0x300000;
    g_UIFlowMediaPlayObj.uiMemSize = PB_GetCurrDataBufSize()-0x300000;
    g_UIFlowMediaPlayObj.u16OutWidth = pPlayObj->uiDisplayFBWidth;
    g_UIFlowMediaPlayObj.u16OutHeight= pPlayObj->uiDisplayFBHeight;
    if(KeyScan_IsHDMIPlugIn() == TRUE)
    {
        g_UIFlowMediaPlayObj.u16OutWratio= g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
        g_UIFlowMediaPlayObj.u16OutHratio= g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
    }
    else if(KeyScan_IsTVPlugIn() == TRUE)
    {
        g_UIFlowMediaPlayObj.u16OutWratio= 4;//g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
        g_UIFlowMediaPlayObj.u16OutHratio= 3;//g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
    }
    else
    {//LCD panel
        #if (_LCDTYPE_ == _LCDTYPE_TXDT300C_)
        g_UIFlowMediaPlayObj.u16OutWratio= g_LCDSize.uiWidth;//PB_PANEL_NO_SCALING;
        g_UIFlowMediaPlayObj.u16OutHratio= g_LCDSize.uiHeight;//PB_PANEL_NO_SCALING;
        #else
        g_UIFlowMediaPlayObj.u16OutWratio= 4;
        g_UIFlowMediaPlayObj.u16OutHratio= 3;
        #endif
    }
    g_UIFlowMediaPlayObj.CallBackFunc = movPlayCB;
    //g_UIFlowMediaPlayObj.bHWVideo = TRUE;
    g_UIFlowMediaPlayObj.bHWAudio = FALSE;
    g_UIFlowMediaPlayObj.bPlayH264 = TRUE;
    //#NT#2007/03/09#Corey Ke -begin
    //#Disable AVI task to play all files without DCF
    //g_UIFlowMediaPlayObj.bPlayAllFiles = FALSE;
    //#NT#2007/03/09#Corey Ke -end
    //g_UIFlowMediaPlayObj.bPlayWithBG = FALSE;

    //g_UIFlowMediaPlayObj.u16fileType = MEDIA_FILEFORMAT_MOV;
    // Get display frame buffer
    PB_GetCurr3FBAddr(&y1, &cb1, &cr1);
    // Get temp display frame buffer
    PB_GetCurr3TempFBAddr(&y2, &cb2, &cr2);
    g_UIFlowMediaPlayObj.panelInfo.y1Addr = y1;
    g_UIFlowMediaPlayObj.panelInfo.cb1Addr = cb1;
    g_UIFlowMediaPlayObj.panelInfo.cr1Addr = cr1;
    g_UIFlowMediaPlayObj.panelInfo.y2Addr = y2;
    g_UIFlowMediaPlayObj.panelInfo.cb2Addr = cb2;
    g_UIFlowMediaPlayObj.panelInfo.cr2Addr = cr2;

    //before MediaPlay_Open( ), MediaPlay_Decode1stVideo( ) should be called firstly (by Playbacktsk)
    //
    MediaPlay_GetDecode1stParam(&play1stParam);
    if ((play1stParam.filetype == 0)||
        (play1stParam.videoCodec == 0))
    {
        debug_err(("MediaPlay_Decode1stVideo( ) should be called firstly\r\n"));
        return;
    }
    if (play1stParam.videoCodec == MEDIAPLAY_VIDEO_H264)
    {
        pDecobj = MP_H264Dec_getVideoDecodeObject();
        MediaPlay_SetVideoDecObj(pDecobj);
        debug_err(("videoCodec = MEDIAPLAY_VIDEO_H264!!\r\n "));
    }
    else if (play1stParam.videoCodec == MEDIAPLAY_VIDEO_MJPG)
    {
        pDecobj = MP_MjpgDec_getVideoDecodeObject();
        MediaPlay_SetVideoDecObj(pDecobj);
        debug_err(("videoCodec = MEDIAPLAY_VIDEO_MJPG!!\r\n "));
    }

    // Set output source
    if (KeyScan_IsTVPlugIn() == TRUE)
    {
        g_UIFlowMediaPlayObj.iAudioType = AUDIO_OUTPUT_HP;
    }
    else if (KeyScan_IsHDMIPlugIn() == TRUE)
    {
        g_UIFlowMediaPlayObj.iAudioType = AUDIO_OUTPUT_NONE;
    }
    else
    {
        g_UIFlowMediaPlayObj.iAudioType = AUDIO_OUTPUT_SPK;
    }

    MediaPlay_Open(&g_UIFlowMediaPlayObj);
    openMedia = MediaPlay_WaitReady();
    aud_setOutput(g_UIFlowMediaPlayObj.iAudioType);//2009/10/14 Meg added
    hdmitx_setAudMute(FALSE);//2010/03/16 Meg

    //#NT#2009/08/26#Randy -begin
    if (openMedia != E_OK)
    {
        PlayMov_StopMedia();
        return;
    }
    //#NT#2009/08/26#Randy -end
    jumpFFLevel = 0;
    jumpBWLevel = 0;
    MediaPlay_FWD();
}

void PlayMov_FwdMedia(UINT32 uiIndex)
{
    Set_PlayMovSpeedIndex(uiIndex);
    MediaPlay_SpeedUp(PlayMov_GetSpeedValue(Get_PlayMovSpeedIndex()));
    MediaPlay_FWD();
    MediaPlay_WaitStepFinish();
}

void PlayMov_BwdMedia(UINT32 uiIndex)
{
    Set_PlayMovSpeedIndex(uiIndex);
    MediaPlay_SpeedUp(PlayMov_GetSpeedValue(Get_PlayMovSpeedIndex()));
    MediaPlay_BWD();
    MediaPlay_WaitStepFinish();
}

void PlayMov_StopFwdMedia(void)
{
    Set_PlayMovSpeedIndex(PLAYMOV_SPEED_FWD_1X);
    MediaPlay_SpeedUp(SPEED_NORMAL);
    MediaPlay_FWD();
    MediaPlay_WaitFFBW1XFinish();
    hdmitx_setAudMute(FALSE);//2010/03/16 Meg
}

void PlayMov_StopBwdMedia(void)
{
    //H.264 can't support 1x speed BWD.
    PlayMov_StopFwdMedia();
}
