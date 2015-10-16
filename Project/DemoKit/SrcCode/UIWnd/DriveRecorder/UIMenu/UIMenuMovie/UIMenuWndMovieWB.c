//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndMovieWBRes.c"
#include "UIMenuWndMovieWB.h"
#include "UIPhotoObj.h"
#include "UISystemObj.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"

static UINT32 g_uiWBValue = 0; // for WB setting backup

//---------------------UIMenuWndMovieWBCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndMovieWB)
CTRL_LIST_ITEM(UIMenuWndMovieWB_Panel)
CTRL_LIST_END

//----------------------UIMenuWndMovieWBCtrl Event---------------------------
INT32 UIMenuWndMovieWB_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieWB_OnClose(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndMovieWB)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndMovieWB_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndMovieWB_OnClose)
EVENT_END

INT32 UIMenuWndMovieWB_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    g_uiWBValue = Get_WBIndex(); // backup WB setting
    UxMenu_SetData(&UIMenuWndMovieWB_MenuCtrl,MNU_CURITM,g_uiWBValue);
    /* Set key press/release/continue mask */
    Ux_FlushEvent();
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
    Ux_RedrawAllWind();
	
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieWB_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,1,g_uiWBValue); // recall WB setting
    /* Reset key press/release/continue mask to default */
    Ux_FlushEvent();
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_RELEASE, FLGKEY_KEY_REL_MASK_DEFAULT);
    KeyScan_SetKeyMask(KEYSCAN_KEYMODE_CONTINUE, FLGKEY_KEY_CONT_MASK_DEFAULT);
	
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndMovieWB_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndMovieWB_Panel)
CTRL_LIST_ITEM(UIMenuWndMovieWB_Menu)
CTRL_LIST_END

//----------------------UIMenuWndMovieWB_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndMovieWB_Panel)
EVENT_END

//----------------------UIMenuWndMovieWB_MenuCtrl Event---------------------------
INT32 UIMenuWndMovieWB_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieWB_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieWB_Menu_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieWB_Menu_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieWB_Menu_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndMovieWB_Menu_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndMovieWB_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndMovieWB_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndMovieWB_Menu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndMovieWB_Menu_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndMovieWB_Menu_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndMovieWB_Menu_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndMovieWB_Menu_OnKeyMode)
EVENT_END

INT32 UIMenuWndMovieWB_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  value;
    UINT32 uhSelect = 0;
    UINT32 uiEvtKeyAct = 0;

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
	    case NVTEVT_KEY_PRESS:
	    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
	    value = UxMenu_GetData(pCtrl,MNU_CURITM);
	    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,1,value);
	    break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieWB_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  value;
    UINT32 uhSelect = 0;
    UINT32 uiEvtKeyAct = 0;

    if(paramNum > 0)
        uiEvtKeyAct = paramArray[0];

    switch(uiEvtKeyAct)
    {
        case NVTEVT_KEY_PRESS:
	    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
	    value = UxMenu_GetData(pCtrl,MNU_CURITM);
	    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,1,value);
	    break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieWB_Menu_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieWB_Menu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    g_uiWBValue = UxMenu_GetData(pCtrl,MNU_CURITM); // change WB setting
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,1,g_uiWBValue);
    Ux_CloseWindow(&UIMenuWndMovieWBCtrl,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieWB_Menu_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndMovieWBCtrl,0);

    return NVTEVT_CONSUME;
}
INT32 UIMenuWndMovieWB_Menu_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);

    return NVTEVT_CONSUME;
}