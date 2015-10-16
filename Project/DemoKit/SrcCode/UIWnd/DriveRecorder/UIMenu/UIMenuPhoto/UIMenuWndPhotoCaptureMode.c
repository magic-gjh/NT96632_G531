//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndPhotoCaptureModeRes.c"
#include "UIMenuWndPhotoCaptureMode.h"
#include "UIMenuWndPhotoStill.h"
#include "UIPhotoObj.h"
#include "UISystemObj.h"
#include "KeyScanTsk.h"

//---------------------UIMenuWndPhotoCaptureModeCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoCaptureMode)
CTRL_LIST_ITEM(UIMenuWndPhotoCaptureMode_Panel)
CTRL_LIST_END

//----------------------UIMenuWndPhotoCaptureModeCtrl Event---------------------------
INT32 UIMenuWndPhotoCaptureMode_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoCaptureMode_OnClose(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoCaptureMode)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndPhotoCaptureMode_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndPhotoCaptureMode_OnClose)
EVENT_END

INT32 UIMenuWndPhotoCaptureMode_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UxMenu_SetData(&UIMenuWndPhotoCaptureMode_MenuCtrl,MNU_CURITM,Get_CaptureModeIndex());
    /* Set key press/release/continue mask */
    Ux_FlushEvent();
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
    Ux_RedrawAllWind();
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoCaptureMode_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Reset key press/release/continue mask to default */
    Ux_FlushEvent();
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPhotoCaptureMode_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoCaptureMode_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoCaptureMode_Menu)
CTRL_LIST_END

//----------------------UIMenuWndPhotoCaptureMode_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoCaptureMode_Panel)
EVENT_END

//----------------------UIMenuWndPhotoCaptureMode_MenuCtrl Event---------------------------
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoCaptureMode_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndPhotoCaptureMode_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndPhotoCaptureMode_Menu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPhotoCaptureMode_Menu_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndPhotoCaptureMode_Menu_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndPhotoCaptureMode_Menu_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndPhotoCaptureMode_Menu_OnKeyMode)
EVENT_END

INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndPhotoCaptureModeCtrl,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32   value;
    value = UxMenu_GetData(pCtrl,MNU_CURITM);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_CAPTUREMODE,1,value);
    Ux_CloseWindow(&UIMenuWndPhotoCaptureModeCtrl,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndPhotoStillCtrl,0); // close photo still menu
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoCaptureMode_Menu_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    return NVTEVT_CONSUME;
}