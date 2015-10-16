//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndSetupFormatRes.c"
#include "UIMenuWndSetupFormat.h"
//#NT#2009/08/17#Chris Chung -begin
#include "UIMenuWndSetupFormatConfirm.h"
#include "PrimaryTsk.h"
#include "UIMenuWndPhotoStill.h"
#include "UIMenuWndMovieVideo.h"
#include "UIMenuWndPlayback.h"
#include "UISystemObj.h"
//#NT#2009/08/17#Chris Chung -end

//---------------------UIMenuWndSetupFormatCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndSetupFormat)
CTRL_LIST_ITEM(UIMenuWndSetupFormat_List_Text)
CTRL_LIST_END

//----------------------UIMenuWndSetupFormatCtrl Event---------------------------
INT32 UIMenuWndSetupFormat_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupFormat_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupFormat_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupFormat_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupFormat_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupFormat)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndSetupFormat_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndSetupFormat_OnClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndSetupFormat_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndSetupFormat_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndSetupFormat_OnKeyMode)
EVENT_END

INT32 UIMenuWndSetupFormat_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UxList_SetData(&UIMenuWndSetupFormat_List_TextCtrl, LST_CURITM, 0);
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupFormat_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupFormat_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndSetupFormatCtrl, 0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupFormat_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    debug_err(("##UIMenuWndSetupFormatConfirm_OnKeyMenu\n\r"));
    //#NT#2009/07/28#Chris Chung -begin
    switch(Primary_GetCurrentMode())
    {
    case PRIMARY_MODE_MOVIE:
        Ux_CloseWindow(&UIMenuWndMovieVideoCtrl,0);
        break;
    case PRIMARY_MODE_PHOTO:
        Ux_CloseWindow(&UIMenuWndPhotoStillCtrl,0);
        break;
    case PRIMARY_MODE_PLAYBACK:
        Ux_CloseWindow(&UIMenuWndPlaybackCtrl,0);
        break;
    }
    //#NT#2009/07/28#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupFormat_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/17#Chris Chung -begin
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    //#NT#2009/08/17#Chris Chung -end
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndSetupFormat_List_TextCtrl Event---------------------------
INT32 UIMenuWndSetupFormat_List_Text_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupFormat_List_Text_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupFormat_List_Text_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupFormat_List_Text)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndSetupFormat_List_Text_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndSetupFormat_List_Text_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndSetupFormat_List_Text_OnKeyEnter)
EVENT_END

INT32 UIMenuWndSetupFormat_List_Text_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupFormat_List_Text_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupFormat_List_Text_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 ui32CurrListItem;

    ui32CurrListItem = UxList_GetData(pCtrl, LST_CURITM);
    Ux_OpenWindow(&UIMenuWndSetupFormatConfirmCtrl, 1, ui32CurrListItem);
    return NVTEVT_CONSUME;
}
