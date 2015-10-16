//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIFlowWndMovieRes.c"
#include "UIFlowWndMovie.h"
//---------------------UIFlowWndMovieCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndMovie)
CTRL_LIST_ITEM(UIFlowWndMovie_Static_camera)
CTRL_LIST_ITEM(UIFlowWndMovie_Status_battery)
CTRL_LIST_ITEM(UIFlowWndMovie_Panel)
CTRL_LIST_ITEM(UIFlowWndMovie_Static_time)
CTRL_LIST_ITEM(UIFlowWndMovie_Static_maxtime)
CTRL_LIST_ITEM(UIFlowWndMovie_Static_resolution)
CTRL_LIST_ITEM(UIFlowWndMovie_Zoom_Static)
CTRL_LIST_ITEM(UIFlowWndMovie_YMD_Static)
CTRL_LIST_ITEM(UIFlowWndMovie_HMS_Static)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_Storage)
CTRL_LIST_ITEM(UIFlowWndMovie_StaticIcon_PIM)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_CyclicRec)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_MotionDet)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_LEDLight)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_Audio)
CTRL_LIST_ITEM(UIFlowWndMovie_StaticIcon_Lock)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_Color)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_EV)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_WB)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_GSENSOR)
CTRL_LIST_ITEM(UIFlowWndMovie_Speed)
CTRL_LIST_ITEM(UIFlowWndMovie_GPS)
CTRL_LIST_ITEM(UIFlowWndMovie_StatusICN_GPS)
CTRL_LIST_END

//----------------------UIFlowWndMovieCtrl Event---------------------------
INT32 UIFlowWndMovie_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyShutter2(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyZoomin(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyZoomout(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnKeyPlayback(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnBattery(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnChildClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnMovieFinish(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnMovieOneSec(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnMovieFull(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnOZoomStepChange(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnDZoomStepChange(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnStorageChange(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnMacroChange(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndMovie_OnTimer(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndMovie)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndMovie_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndMovie_OnClose)
EVENT_ITEM(NVTEVT_KEY_MENU,UIFlowWndMovie_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_UP,UIFlowWndMovie_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIFlowWndMovie_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIFlowWndMovie_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIFlowWndMovie_OnKeyRight)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIFlowWndMovie_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIFlowWndMovie_OnKeyShutter2)
EVENT_ITEM(NVTEVT_KEY_ZOOMIN,UIFlowWndMovie_OnKeyZoomin)
EVENT_ITEM(NVTEVT_KEY_ZOOMOUT,UIFlowWndMovie_OnKeyZoomout)
EVENT_ITEM(NVTEVT_KEY_MODE,UIFlowWndMovie_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_PLAYBACK,UIFlowWndMovie_OnKeyPlayback)
EVENT_ITEM(NVTEVT_BATTERY,UIFlowWndMovie_OnBattery)
EVENT_ITEM(NVTEVT_CHILD_CLOSE,UIFlowWndMovie_OnChildClose)
EVENT_ITEM(NVTEVT_CB_MOVIE_FINISH,UIFlowWndMovie_OnMovieFinish)
EVENT_ITEM(NVTEVT_CB_MOVIE_ONE_SEC,UIFlowWndMovie_OnMovieOneSec)
EVENT_ITEM(NVTEVT_CB_MOVIE_FULL,UIFlowWndMovie_OnMovieFull)
EVENT_ITEM(NVTEVT_CB_OZOOMSTEPCHG,UIFlowWndMovie_OnOZoomStepChange)
EVENT_ITEM(NVTEVT_CB_DZOOMSTEPCHG,UIFlowWndMovie_OnDZoomStepChange)
EVENT_ITEM(NVTEVT_STORAGE_CHANGE,UIFlowWndMovie_OnStorageChange)
EVENT_ITEM(NVTEVT_MACRO_CHANGE,UIFlowWndMovie_OnMacroChange)
EVENT_ITEM(NVTEVT_TIMER,UIFlowWndMovie_OnTimer)
EVENT_END

INT32 UIFlowWndMovie_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyZoomin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyZoomout(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnKeyPlayback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnBattery(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnChildClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CHILD_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnMovieFinish(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnMovieOneSec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnMovieFull(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnOZoomStepChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnDZoomStepChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnStorageChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnMacroChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndMovie_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_TIMER,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//----------------------UIFlowWndMovie_Static_cameraCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Static_camera)
EVENT_END

//----------------------UIFlowWndMovie_Status_batteryCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Status_battery)
EVENT_END

//---------------------UIFlowWndMovie_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndMovie_Panel)
CTRL_LIST_END

//----------------------UIFlowWndMovie_PanelCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Panel)
EVENT_END

//----------------------UIFlowWndMovie_Static_timeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Static_time)
EVENT_END

//----------------------UIFlowWndMovie_Static_maxtimeCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Static_maxtime)
EVENT_END

//----------------------UIFlowWndMovie_Static_resolutionCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Static_resolution)
EVENT_END

//----------------------UIFlowWndMovie_Zoom_StaticCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Zoom_Static)
EVENT_END

//----------------------UIFlowWndMovie_YMD_StaticCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_YMD_Static)
EVENT_END

//----------------------UIFlowWndMovie_HMS_StaticCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_HMS_Static)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_StorageCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_Storage)
EVENT_END

//----------------------UIFlowWndMovie_StaticIcon_PIMCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StaticIcon_PIM)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_CyclicRecCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_CyclicRec)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_MotionDetCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_MotionDet)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_LEDLightCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_LEDLight)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_AudioCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_Audio)
EVENT_END

//----------------------UIFlowWndMovie_StaticIcon_LockCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StaticIcon_Lock)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_ColorCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_Color)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_EVCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_EV)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_WBCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_WB)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_GSENSORCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_GSENSOR)
EVENT_END

//----------------------UIFlowWndMovie_SpeedCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_Speed)
EVENT_END

//----------------------UIFlowWndMovie_GPSCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_GPS)
EVENT_END

//----------------------UIFlowWndMovie_StatusICN_GPSCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMovie_StatusICN_GPS)
EVENT_END
