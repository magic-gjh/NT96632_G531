//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIFlowWndPlayRes.c"
#include "UIFlowWndPlay.h"
//#NT#2009/10/30#Chris Chung -begin
#include "UIMenuWndPlayback.h"
#include "UIPlayObj.h"
#include "UISystemObj.h"
#include "UIFlowWndPlayFuncs.h"
#include "UIFlowWndPlayThumb.h"
#include "UIFlowWndPlayMagnify.h"
#include "UIMenuWndPlayQuickDel.h"
#include "UIFlowWndWrnMsg.h"
#include "UIMenuSetupSettings.h"
#include "UISystemStatusSettings.h"
#include "UIFlowWndCommonAPI.h"
#include "KeyScanTsk.h"
#include "Audio.h"
#include "AppInitPlayback.h"


INT32   UIFlowWndPlay_UpdateIcons(void);

BOOL    g_bUIFlowWndPlayNoImgWndOpened = FALSE;
UINT32  g_uiUIFlowWndPlayCurrentVolume = AUDIO_VOL_MUTE;
extern BOOL ACC5MinMode;

PLB_TASK_DATA g_PlbData = {0};
//#NT#2009/10/30#Chris Chung -end
//---------------------UIFlowWndPlayCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlay)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticICN_DSCMode)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Flash)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_EV)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Size)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_WB)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Quality)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Sharpness)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Storage)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Battery)
CTRL_LIST_ITEM(UIFlowWndPlay_TabHistogram)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Filename)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticICN_Protect)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Date)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Speed)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_Time)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_Volumn)
CTRL_LIST_ITEM(UIFlowWndPlay_Histo)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovPlay)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovStop)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticTXT_MovPlayTime)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovBwd)
CTRL_LIST_ITEM(UIFlowWndPlay_StatusICN_MovFwd)
CTRL_LIST_ITEM(UIFlowWndPlay_StaticICN_Print)
CTRL_LIST_END

//----------------------UIFlowWndPlayCtrl Event---------------------------
INT32 UIFlowWndPlay_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyZoomin(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyZoomout(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyDisplay(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyShutter2(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyPlayback(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnKeyDel(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnBattery(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnMovieFinish(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnMovieOneSec(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnStorageInit(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlay_OnStorageChange(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndPlay_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndPlay_OnClose)
EVENT_ITEM(NVTEVT_KEY_UP,UIFlowWndPlay_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIFlowWndPlay_OnKeyDown)
//EVENT_ITEM(NVTEVT_KEY_LEFT,UIFlowWndPlay_OnKeyLeft)
//EVENT_ITEM(NVTEVT_KEY_RIGHT,UIFlowWndPlay_OnKeyRight)
EVENT_ITEM(NVTEVT_KEY_ZOOMIN,UIFlowWndPlay_OnKeyZoomin)
EVENT_ITEM(NVTEVT_KEY_ZOOMOUT,UIFlowWndPlay_OnKeyZoomout)
EVENT_ITEM(NVTEVT_KEY_MENU,UIFlowWndPlay_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_DISPLAY,UIFlowWndPlay_OnKeyDisplay)
EVENT_ITEM(NVTEVT_KEY_MODE,UIFlowWndPlay_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIFlowWndPlay_OnKeyShutter2)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIFlowWndPlay_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_PLAYBACK,UIFlowWndPlay_OnKeyPlayback)
EVENT_ITEM(NVTEVT_KEY_DEL,UIFlowWndPlay_OnKeyDel)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIFlowWndPlay_OnChildClose)
EVENT_ITEM(NVTEVT_BATTERY,UIFlowWndPlay_OnBattery)
EVENT_ITEM(NVTEVT_CB_MOVIE_FINISH,UIFlowWndPlay_OnMovieFinish)
EVENT_ITEM(NVTEVT_CB_MOVIE_ONE_SEC,UIFlowWndPlay_OnMovieOneSec)
EVENT_ITEM(NVTEVT_STORAGE_INIT,UIFlowWndPlay_OnStorageInit)
EVENT_ITEM(NVTEVT_STORAGE_CHANGE,UIFlowWndPlay_OnStorageChange)
EVENT_END

//#NT#2009/10/30#Chris Chung -begin
static void UIFlowWndPlay_Dummy(void)
{
    /* add to show UIFlowWndPlay_OpenNoImageWnd in source insight */
}

void UIFlowWndPlay_OpenNoImageWnd(FLOWWRNMSG_TIMER timer_keep)
{
    #if (_PLAYBACK_FILESHOW_ == _PLAYBACK_FILESHOW_SEPA_)
    switch(AppInit_ModePlayback_Get_Mode())
    {
        case APP_PLAYBACK_BR_JPG:
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_NO_IMAGE,timer_keep);
            break;
        case APP_PLAYBACK_BR_VIDEO:
        default:
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_NO_MOVIE,timer_keep);
            break;
    }
    #else
    Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_NO_IMAGE,timer_keep);
    #endif
}
//#NT#2009/10/30#Chris Chung -end

static INT32 UIFlowWndPlay_OnExeUpKeyAct(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiEvtKeyAct = 0;
    UINT32 uiCurrSpeedIndex = Get_PlayMovSpeedIndex();
    debug_msg("22222222222222------");
    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];
    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
            switch(g_PlbData.State)
            {
            case PLB_ST_FULL:
                Ux_FlushEvent();
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
                Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYPREV,0);
                FlowPB_UpdateIcons(1);
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                break;            

            case PLB_ST_PLAY_MOV:
            case PLB_ST_FWD_MOV:
            case PLB_ST_BWD_MOV:

                if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_8X)
                {
                    uiCurrSpeedIndex ++;

                    Ux_FlushEvent();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);

                    if(uiCurrSpeedIndex > PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_FWD_MOV;
                    }
                    else if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVBWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_BWD_MOV;
                    }
                    else   //uiCurrSpeedIndex == PLAYMOV_SPEED_FWD_1X
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWDSTOP,0);
                        g_PlbData.State = PLB_ST_PLAY_MOV;
                        /* Since Media FWD/BWD will set volume mute, we should restore current volume. */
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_uiUIFlowWndPlayCurrentVolume);
                    }

                    FlowPB_IconDrawMovPlaySpeed();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                }
                break;
				
            }
            break;

        case NVTEVT_KEY_RELEASE:
            //Rsvd
            break;
    }
    //#NT#2009/10/22#Chris Chung -end
    return NVTEVT_CONSUME;
}

static INT32 UIFlowWndPlay_OnExeDownKeyAct(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiEvtKeyAct = 0;
    UINT32 uiCurrSpeedIndex = Get_PlayMovSpeedIndex();

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:

            switch(g_PlbData.State)
            {
            case PLB_ST_FULL:
                Ux_FlushEvent();
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
                Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYNEXT,0);
                FlowPB_UpdateIcons(1);
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                break;

            case PLB_ST_PLAY_MOV:
            case PLB_ST_FWD_MOV:
            case PLB_ST_BWD_MOV:				
                if(uiCurrSpeedIndex > PLAYMOV_SPEED_BWD_8X)
                {
                    uiCurrSpeedIndex --;

                    Ux_FlushEvent();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);

                    if(uiCurrSpeedIndex > PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_FWD_MOV;
                    }
                    else if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVBWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_BWD_MOV;
                    }
                    else   //uiCurrSpeedIndex == PLAYMOV_SPEED_FWD_1X
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWDSTOP,0);
                        g_PlbData.State = PLB_ST_PLAY_MOV;
                        /* Since Media FWD/BWD will set volume mute, we should restore current volume. */
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_uiUIFlowWndPlayCurrentVolume);
                    }

                    FlowPB_IconDrawMovPlaySpeed();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                }
                break;
							
            }
            break;

        case NVTEVT_KEY_RELEASE:
            //Rsvd
            break;
    }
    return NVTEVT_CONSUME;
}

INT32 UIFlowWndPlay_OnExeEnterKeyAct(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    static BOOL bFirstEnter = TRUE;

    switch(g_PlbData.State)
    {
        case PLB_ST_FULL:
        {
            switch(UIPlay_GetData(PLAY_CURRMODE))
            {
            case PLAYMODE_AVI:
            case PLAYMODE_MOVMJPG:
                if (UIPlay_GetData(PLAY_PBSTATUS) == PB_STA_DONE)
                {
                    if(bFirstEnter)
                    {
                        g_uiUIFlowWndPlayCurrentVolume = AUDIO_VOL_7;//Get_VolumeValue(Get_VolumeIndex());
                        bFirstEnter = FALSE;
                    }
                    KeyScan_EnableAutoPoweroff(FALSE);
                    KeyScan_EnableUSBDet(FALSE);
                    KeyScan_EnableKeyTone(FALSE);
                    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_uiUIFlowWndPlayCurrentVolume);
                    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVPLAY,1,PlayFlowMoviePlayCBFunc);
                    g_PlbData.State = PLB_ST_PLAY_MOV;
                    FlowPB_IconDrawMovPlay();
                    FlowPB_IconDrawMovPlayTime();
                    FlowPB_IconDrawMovPlaySpeed();
                    //FlowPB_IconDrawMovPlayVolumn(g_uiUIFlowWndPlayCurrentVolume);
                }
                break;
            }
            break;
        }

        case PLB_ST_PLAY_MOV:
        case PLB_ST_FWD_MOV:
        case PLB_ST_BWD_MOV:
        {
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVPAUSE,0);
            g_PlbData.State = PLB_ST_PAUSE_MOV;
            KeyScan_EnableAutoPoweroff(TRUE);
            KeyScan_EnableUSBDet(TRUE);
            FlowPB_IconDrawMovPlay();
            break;
        }

        case PLB_ST_PAUSE_MOV:
        {
            UINT32 uiCurrSpeedIndex = Get_PlayMovSpeedIndex();

            KeyScan_EnableAutoPoweroff(FALSE);
            KeyScan_EnableUSBDet(FALSE);
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVRESUME,0);
            if(uiCurrSpeedIndex > PLAYMOV_SPEED_FWD_1X)
            {
                g_PlbData.State = PLB_ST_FWD_MOV;
            }
            else if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_1X)
            {
                g_PlbData.State = PLB_ST_BWD_MOV;
            }
            else   //uiCurrSpeedIndex == PLAYMOV_SPEED_FWD_1X
            {
                g_PlbData.State = PLB_ST_PLAY_MOV;
            }
            FlowPB_IconDrawMovPlay();
            break;
        }
    }
    return NVTEVT_CONSUME;
}

INT32 UIFlowWndPlay_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/10/30#Chris Chung -begin
    g_bUIFlowWndPlayNoImgWndOpened = FALSE;

    if(UIPlay_GetData(PLAY_FILENUM)==0)
    {
        UIFlowWndPlay_OpenNoImageWnd(FLOWWRNMSG_TIMER_KEEP);
        g_bUIFlowWndPlayNoImgWndOpened = TRUE;
    }
    else
    {
        /* Set key press/release/continue mask */
        Ux_FlushEvent();
	#if 0
	if(ACC5MinMode==TRUE)
	{
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_MASK_NULL);
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_MASK_NULL);	
	}
    else
	{		
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
        KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
    }
	#endif
        g_PlbData.State = PLB_ST_FULL;
        FlowPB_UpdateIcons(1);
    }
    //KeyScan_SetDetGSPNAVIEn(TRUE);
	
    //#NT#2009/10/30#Chris Chung -end
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* reset original volume for key sound */
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,Get_VolumeValue(Get_VolumeIndex()));

    KeyScan_EnableAutoPoweroff(TRUE);
    KeyScan_EnableUSBDet(TRUE);
    KeyScan_EnableKeyTone(Get_BeepValue(Get_BeepIndex()));

    //#NT#2009/09/14#Chris Chung -begin
    /* Reset key press/release/continue mask to default */
    Ux_FlushEvent();
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
    //#NT#2009/09/14#Chris Chung -end
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    debug_msg("11111111111111--------");
    return UIFlowWndPlay_OnExeUpKeyAct(pCtrl, paramNum, paramArray);
}
INT32 UIFlowWndPlay_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndPlay_OnExeDownKeyAct(pCtrl, paramNum, paramArray);
}
INT32 UIFlowWndPlay_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiEvtKeyAct = 0;
    UINT32 uiCurrSpeedIndex = Get_PlayMovSpeedIndex();

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:

            switch(g_PlbData.State)
            {
            case PLB_ST_FULL:
                Ux_FlushEvent();
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
                Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYNEXT,0);
                FlowPB_UpdateIcons(1);
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                break;

            case PLB_ST_PLAY_MOV:
            case PLB_ST_FWD_MOV:
            case PLB_ST_BWD_MOV:				
                if(uiCurrSpeedIndex > PLAYMOV_SPEED_BWD_8X)
                {
                    uiCurrSpeedIndex --;

                    Ux_FlushEvent();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);

                    if(uiCurrSpeedIndex > PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_FWD_MOV;
                    }
                    else if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVBWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_BWD_MOV;
                    }
                    else   //uiCurrSpeedIndex == PLAYMOV_SPEED_FWD_1X
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWDSTOP,0);
                        g_PlbData.State = PLB_ST_PLAY_MOV;
                        /* Since Media FWD/BWD will set volume mute, we should restore current volume. */
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_uiUIFlowWndPlayCurrentVolume);
                    }

                    FlowPB_IconDrawMovPlaySpeed();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                }
                break;
							
            }
            break;

        case NVTEVT_KEY_RELEASE:
            //Rsvd
            break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiEvtKeyAct = 0;
    UINT32 uiCurrSpeedIndex = Get_PlayMovSpeedIndex();

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
            switch(g_PlbData.State)
            {
            case PLB_ST_FULL:
                Ux_FlushEvent();
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
                Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYPREV,0);
                FlowPB_UpdateIcons(1);
                KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                break;            

            case PLB_ST_PLAY_MOV:
            case PLB_ST_FWD_MOV:
            case PLB_ST_BWD_MOV:

                if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_8X)
                {
                    uiCurrSpeedIndex ++;

                    Ux_FlushEvent();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);

                    if(uiCurrSpeedIndex > PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_FWD_MOV;
                    }
                    else if(uiCurrSpeedIndex < PLAYMOV_SPEED_FWD_1X)
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVBWD,1,uiCurrSpeedIndex);
                        g_PlbData.State = PLB_ST_BWD_MOV;
                    }
                    else   //uiCurrSpeedIndex == PLAYMOV_SPEED_FWD_1X
                    {
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVFWDSTOP,0);
                        g_PlbData.State = PLB_ST_PLAY_MOV;
                        /* Since Media FWD/BWD will set volume mute, we should restore current volume. */
                        Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_uiUIFlowWndPlayCurrentVolume);
                    }

                    FlowPB_IconDrawMovPlaySpeed();
                    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
                }
                break;
				
            }
            break;

        case NVTEVT_KEY_RELEASE:
            //Rsvd
            break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyZoomin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndPlay_OnExeUpKeyAct(pCtrl, paramNum, paramArray);
}
INT32 UIFlowWndPlay_OnKeyZoomout(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndPlay_OnExeDownKeyAct(pCtrl, paramNum, paramArray);
}
INT32 UIFlowWndPlay_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/04#Chris Chung -begin
    switch(g_PlbData.State)
    {
        case PLB_ST_FULL:
            Ux_OpenWindow((VControl *)(&UIMenuWndPlaybackCtrl), 0);
            break;
    }
    //#NT#2009/08/04#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/11/24#Chris Chung -begin
    switch(g_PlbData.State)
    {
        case PLB_ST_FULL:
//            Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_PLAYBACK_MODE,0);
    //#NT#2009/09/14#Chris Chung -begin
    //Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    //#NT#2009/09/14#Chris Chung -end
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_CHANGE_DSCMODE,1,DSCMODE_CHGTO_NEXT);   
            break;

        case PLB_ST_PLAY_MOV:
        case PLB_ST_PAUSE_MOV:
        case PLB_ST_FWD_MOV:
        case PLB_ST_BWD_MOV:
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVSTOP,0);
            g_PlbData.State = PLB_ST_FULL;
            KeyScan_EnableAutoPoweroff(TRUE);
            KeyScan_EnableUSBDet(TRUE);
            KeyScan_EnableKeyTone(Get_BeepValue(Get_BeepIndex()));
            FlowPB_UpdateIcons(1);
            /* reset original volume for key sound */
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,Get_VolumeValue(Get_VolumeIndex()));
            break;	
    	}
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndPlay_OnExeEnterKeyAct(pCtrl, paramNum, paramArray);
}
INT32 UIFlowWndPlay_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndPlay_OnExeEnterKeyAct(pCtrl, paramNum, paramArray);
}
INT32 UIFlowWndPlay_OnKeyPlayback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/11/24#Chris Chung -begin
    switch(g_PlbData.State)
    {
        case PLB_ST_FULL:
//            Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_PLAYBACK_MODE,0);
            Ux_SendEvent(&UISystemObjCtrl,NVTEVT_CHANGE_DSCMODE,1,DSCMODE_CHGTO_NEXT);
            break;

        case PLB_ST_PLAY_MOV:
        case PLB_ST_PAUSE_MOV:
        case PLB_ST_FWD_MOV:
        case PLB_ST_BWD_MOV:
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVSTOP,0);
            g_PlbData.State = PLB_ST_FULL;
            KeyScan_EnableAutoPoweroff(TRUE);
            KeyScan_EnableUSBDet(TRUE);
            KeyScan_EnableKeyTone(Get_BeepValue(Get_BeepIndex()));
            FlowPB_UpdateIcons(1);
            /* reset original volume for key sound */
            Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,Get_VolumeValue(Get_VolumeIndex()));
            break;
    }
    //#NT#2009/11/24#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnKeyDel(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    switch(g_PlbData.State)
    {
        case PLB_ST_FULL:
            Ux_OpenWindow((VControl *)(&UIMenuWndPlayQuickDelCtrl), 0);
            break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    //KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, PLB_KEY_RELEASE_MASK);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);

    //#NT#2009/10/30#Chris Chung -begin
    if(paramNum > 0)
    {
        //Return from thumbnail, magnify or delete mode and play current image again.
        switch(paramArray[0])
        {
            case NVTRET_THUMBNAIL:
            case NVTRET_MAGNIFY:
                Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYCURR,0);
                FlowPB_UpdateIcons(1);
                break;
            case NVTRET_DELETE:
            case NVTRET_DELETEALL:
                if(UIPlay_GetData(PLAY_FILENUM)==0)
                {
                    if(!g_bUIFlowWndPlayNoImgWndOpened)
                    {
                        UIFlowWndPlay_OpenNoImageWnd(FLOWWRNMSG_TIMER_KEEP);
                        g_bUIFlowWndPlayNoImgWndOpened = TRUE;
                    }
                }
                else
                {
                    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYCURR,0);
                    FlowPB_UpdateIcons(1);
                }
                break;
            case NVTRET_ENTER_MENU:
                Ux_OpenWindow((VControl *)(&UIMenuWndPlaybackCtrl), 0);
                g_bUIFlowWndPlayNoImgWndOpened = FALSE;
                break;
        }
    }
    else
    {
        if(UIPlay_GetData(PLAY_FILENUM)==0)
        {
            if(!g_bUIFlowWndPlayNoImgWndOpened)
            {
                UIFlowWndPlay_OpenNoImageWnd(FLOWWRNMSG_TIMER_KEEP);
                g_bUIFlowWndPlayNoImgWndOpened = TRUE;
            }
        }
        else
        {
            //Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYCURR,0);
            FlowPB_UpdateIcons(1);
        }
    }
    //#NT#2009/10/30#Chris Chung -end
    Ux_DefaultEvent(pCtrl,NVTEVT_CHILD_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnBattery(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/07/28#Chris Chung -begin
    UxState_SetData(&UIFlowWndPlay_StatusICN_BatteryCtrl, STATE_CURITEM, GetBatteryLevel());
    //#NT#2009/07/28#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnMovieFinish(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/11/24#Chris Chung -begin
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_NULL);
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVSTOP,0);
    g_PlbData.State = PLB_ST_FULL;
    KeyScan_EnableAutoPoweroff(TRUE);
    KeyScan_EnableUSBDet(TRUE);
    KeyScan_EnableKeyTone(Get_BeepValue(Get_BeepIndex()));
    FlowPB_UpdateIcons(1);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    /* reset original volume for key sound */
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,Get_VolumeValue(Get_VolumeIndex()));
    //#NT#2009/11/24#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnMovieOneSec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    switch(g_PlbData.State)
    {
        case PLB_ST_PLAY_MOV:
        case PLB_ST_PAUSE_MOV:
        case PLB_ST_FWD_MOV:
        case PLB_ST_BWD_MOV:
        case PLB_ST_SLOW_MOV:
            FlowPB_IconDrawMovPlayTime();
            break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnStorageInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlay_OnStorageChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StaticICN_DSCModeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticICN_DSCMode)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_FlashCtrl Event---------------------------
INT32 UIFlowWndPlay_StatusICN_Flash_OnKeyRight(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Flash)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIFlowWndPlay_StatusICN_Flash_OnKeyRight)
EVENT_END

INT32 UIFlowWndPlay_StatusICN_Flash_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StatusICN_EVCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_EV)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_SizeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Size)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_WBCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_WB)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_QualityCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Quality)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_SharpnessCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Sharpness)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_StorageCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Storage)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_BatteryCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Battery)
EVENT_END

//----------------------UIFlowWndPlay_TabHistogramCtrl Event---------------------------
INT32 UIFlowWndPlay_TabHistogram_OnRedraw(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay_TabHistogram)
EVENT_ITEM(NVTEVT_REDRAW,UIFlowWndPlay_TabHistogram_OnRedraw)
EVENT_END

INT32 UIFlowWndPlay_TabHistogram_OnRedraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_REDRAW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StaticTXT_FilenameCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Filename)
EVENT_END

//----------------------UIFlowWndPlay_StaticICN_ProtectCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticICN_Protect)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_DateCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Date)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_SpeedCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Speed)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_TimeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_Time)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_VolumnCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_Volumn)
EVENT_END

//----------------------UIFlowWndPlay_HistoCtrl Event---------------------------
INT32 UIFlowWndPlay_Histo_OnRedraw(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlay_Histo)
EVENT_ITEM(NVTEVT_REDRAW,UIFlowWndPlay_Histo_OnRedraw)
EVENT_END

INT32 UIFlowWndPlay_Histo_OnRedraw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/21#Chris Chung -begin
    UIFlowWndCommon_GetHistogram(pCtrl,paramNum,paramArray);
    //#NT#2009/08/21#Chris Chung -end
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndPlay_StatusICN_MovPlayCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovPlay)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_MovStopCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovStop)
EVENT_END

//----------------------UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticTXT_MovPlayTime)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_MovBwdCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovBwd)
EVENT_END

//----------------------UIFlowWndPlay_StatusICN_MovFwdCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StatusICN_MovFwd)
EVENT_END

//----------------------UIFlowWndPlay_StaticICN_PrintCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlay_StaticICN_Print)
EVENT_END

