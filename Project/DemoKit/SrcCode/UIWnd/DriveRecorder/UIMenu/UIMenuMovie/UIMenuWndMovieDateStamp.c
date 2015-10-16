//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndMovieDateStampRes.c"
#include "UIMenuWndMovieDateStamp.h"
#include "UIMovieObj.h"
#include "UISystemObj.h"
#include "UIMenuWndMovieVideo.h"
//---------------------UIMenuWndMovieDateStampCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndMovieDateStamp)
CTRL_LIST_ITEM(UIMenuWndMovieDateStamp_Panel)
CTRL_LIST_END

//----------------------UIMenuWndMovieDateStampCtrl Event---------------------------
INT32 UIMenuWndMovieDateStamp_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieDateStamp_OnClose(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndMovieDateStamp)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndMovieDateStamp_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndMovieDateStamp_OnClose)
EVENT_END

INT32 UIMenuWndMovieDateStamp_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UxMenu_SetData(&UIMenuWndMovieDateStamp_MenuCtrl,MNU_CURITM,Get_MovieDateImprintIndex());
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieDateStamp_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndMovieDateStamp_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndMovieDateStamp_Panel)
CTRL_LIST_ITEM(UIMenuWndMovieDateStamp_Menu)
CTRL_LIST_END

//----------------------UIMenuWndMovieDateStamp_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndMovieDateStamp_Panel)
EVENT_END

//----------------------UIMenuWndMovieDateStamp_MenuCtrl Event---------------------------
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndMovieDateStamp_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndMovieDateStamp_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndMovieDateStamp_Menu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndMovieDateStamp_Menu_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndMovieDateStamp_Menu_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndMovieDateStamp_Menu_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndMovieDateStamp_Menu_OnKeyMode)
EVENT_END

INT32 UIMenuWndMovieDateStamp_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndMovieDateStampCtrl, 0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
 INT32 ui32CurrListItem;

    ui32CurrListItem = UxMenu_GetData(pCtrl,MNU_CURITM);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_DATEIMPRINT,1,ui32CurrListItem);
    Ux_CloseWindow(&UIMenuWndMovieDateStampCtrl, 0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndMovieVideoCtrl,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieDateStamp_Menu_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    return NVTEVT_CONSUME;
}