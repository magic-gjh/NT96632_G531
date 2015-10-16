//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndPlayDPOFRes.c"
#include "UIMenuWndPlayDPOF.h"
//#NT#2009/09/14#Chris Chung -begin
#include "UIMenuWndPlaySetDPOF.h"
#include "UISystemObj.h"
#include "UIMenuWndPlayback.h"
#include "KeyScanTsk.h"
//#NT#2009/09/14#Chris Chung -end

//---------------------UIMenuWndPlayDPOFCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPlayDPOF)
CTRL_LIST_ITEM(UIMenuWndPlayDPOF_Panel)
CTRL_LIST_END

//----------------------UIMenuWndPlayDPOFCtrl Event---------------------------
INT32 UIMenuWndPlayDPOF_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayDPOF_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayDPOF_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPlayDPOF)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndPlayDPOF_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndPlayDPOF_OnClose)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndPlayDPOF_OnKeyMode)
EVENT_END

INT32 UIMenuWndPlayDPOF_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/09/14#Chris Chung -begin
    UxMenu_SetData(&UIMenuWndPlayDPOF_MenuCtrl, MNU_CURITM, 0);

    /* Set key press/release/continue mask */
    Ux_FlushEvent();
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
    //#NT#2009/09/14#Chris Chung -end
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayDPOF_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
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
INT32 UIMenuWndPlayDPOF_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/09/14#Chris Chung -begin
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    //#NT#2009/09/14#Chris Chung -end
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPlayDPOF_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPlayDPOF_Panel)
CTRL_LIST_ITEM(UIMenuWndPlayDPOF_Menu)
CTRL_LIST_END

//----------------------UIMenuWndPlayDPOF_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPlayDPOF_Panel)
EVENT_END

//----------------------UIMenuWndPlayDPOF_MenuCtrl Event---------------------------
INT32 UIMenuWndPlayDPOF_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayDPOF_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayDPOF_Menu_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayDPOF_Menu_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPlayDPOF_Menu_OnKeyMenu(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPlayDPOF_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndPlayDPOF_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndPlayDPOF_Menu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndPlayDPOF_Menu_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPlayDPOF_Menu_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndPlayDPOF_Menu_OnKeyMenu)
EVENT_END

INT32 UIMenuWndPlayDPOF_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayDPOF_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayDPOF_Menu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/06/02#Chris Chung -begin
    UINT32   curItem;

    curItem = UxMenu_GetData(pCtrl,MNU_CURITM);

    Ux_OpenWindow((VControl *)(&UIMenuWndPlaySetDPOFCtrl), 1, curItem);
    //#NT#2009/06/02#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayDPOF_Menu_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/07/27#Chris Chung -begin
    Ux_CloseWindow((VControl *)(&UIMenuWndPlayDPOFCtrl), 0);
    //#NT#2009/07/27#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPlayDPOF_Menu_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/04#Chris Chung -begin
    Ux_CloseWindow(&UIMenuWndPlaybackCtrl,0);
    //#NT#2009/08/04#Chris Chung -end
    return NVTEVT_CONSUME;
}
