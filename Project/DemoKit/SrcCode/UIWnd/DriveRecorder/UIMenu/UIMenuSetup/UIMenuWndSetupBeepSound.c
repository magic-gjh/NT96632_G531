//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndSetupBeepSoundRes.c"
#include "UIMenuWndSetupBeepSound.h"
//#NT#2009/08/19#Chris Chung -begin
#include "UISystemObj.h"
#include "PrimaryTsk.h"
//#include "UIMenuWndPhotoStill.h"
#include "UIMenuWndMovieVideo.h"
#include "UIMenuWndPlayback.h"
#include "UIMenuSetupSettings.h"
//#NT#2009/08/19#Chris Chung -end

//---------------------UIMenuWndSetupBeepSoundCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndSetupBeepSound)
CTRL_LIST_ITEM(UIMenuWndSetupBeepSound_List_Text)
CTRL_LIST_END

//----------------------UIMenuWndSetupBeepSoundCtrl Event---------------------------
INT32 UIMenuWndSetupBeepSound_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupBeepSound_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupBeepSound_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupBeepSound_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupBeepSound_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupBeepSound)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndSetupBeepSound_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndSetupBeepSound_OnClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndSetupBeepSound_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndSetupBeepSound_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndSetupBeepSound_OnKeyMode)
EVENT_END

INT32 UIMenuWndSetupBeepSound_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/19#Chris Chung -begin
    UxList_SetData(&UIMenuWndSetupBeepSound_List_TextCtrl, LST_CURITM, Get_BeepIndex());
    //#NT#2009/08/19#Chris Chung -end
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupBeepSound_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupBeepSound_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndSetupBeepSoundCtrl, 0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupBeepSound_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/17#Chris Chung -begin
    switch(Primary_GetCurrentMode())
    {
    case PRIMARY_MODE_MOVIE:
        Ux_CloseWindow(&UIMenuWndMovieVideoCtrl,0);
        break;
    case PRIMARY_MODE_PHOTO:
//        Ux_CloseWindow(&UIMenuWndPhotoStillCtrl,0);
        break;
    case PRIMARY_MODE_PLAYBACK:
        Ux_CloseWindow(&UIMenuWndPlaybackCtrl,0);
        break;
    }
    //#NT#2009/08/17#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupBeepSound_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/17#Chris Chung -begin
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    //#NT#2009/08/17#Chris Chung -end
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndSetupBeepSound_List_TextCtrl Event---------------------------
INT32 UIMenuWndSetupBeepSound_List_Text_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupBeepSound_List_Text_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupBeepSound_List_Text_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupBeepSound_List_Text)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndSetupBeepSound_List_Text_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndSetupBeepSound_List_Text_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndSetupBeepSound_List_Text_OnKeyEnter)
EVENT_END

INT32 UIMenuWndSetupBeepSound_List_Text_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupBeepSound_List_Text_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupBeepSound_List_Text_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 ui32CurrListItem;

    ui32CurrListItem = UxList_GetData(pCtrl, LST_CURITM);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_BEEP,1,ui32CurrListItem);
    Ux_CloseWindow(&UIMenuWndSetupBeepSoundCtrl, 0);
    return NVTEVT_CONSUME;
}