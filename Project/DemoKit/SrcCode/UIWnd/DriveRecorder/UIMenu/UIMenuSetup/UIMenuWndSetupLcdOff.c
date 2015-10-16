//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndSetupLcdOffRes.c"
#include "UIMenuWndSetupLcdOff.h"
#include "UISystemObj.h"
#include "PrimaryTsk.h"
//#include "UIMenuWndPhotoStill.h"
#include "UIMenuWndMovieVideo.h"
#include "UIMenuWndPlayback.h"
#include "UIMenuSetupSettings.h"
//#NT#2009/08/19#Chris Chung -end
//---------------------UIMenuWndSetupLcdOffCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndSetupLcdOff)
CTRL_LIST_ITEM(UIMenuWndSetupLcdOff_List_Text)
CTRL_LIST_END

//----------------------UIMenuWndSetupLcdOffCtrl Event---------------------------
INT32 UIMenuWndSetupLcdOff_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_OnKeyShutter2(VControl *, UINT32, UINT32 *);

EVENT_BEGIN(UIMenuWndSetupLcdOff)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndSetupLcdOff_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndSetupLcdOff_OnClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndSetupLcdOff_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndSetupLcdOff_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndSetupLcdOff_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndSetupLcdOff_OnKeyShutter2)

EVENT_END

INT32 UIMenuWndSetupLcdOff_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UxList_SetData(&UIMenuWndSetupLcdOff_List_TextCtrl, LST_CURITM, Get_LcdOffIndex());

    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupLcdOff_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupLcdOff_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndSetupLcdOffCtrl, 0);

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndSetupLcdOff_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
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
INT32 UIMenuWndSetupLcdOff_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/17#Chris Chung -begin
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    //#NT#2009/08/17#Chris Chung -end
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndSetupLcdOff_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndSetupLcdOffCtrl, 0);

    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndSetupLcdOff_List_TextCtrl Event---------------------------
INT32 UIMenuWndSetupLcdOff_List_Text_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_List_Text_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupLcdOff_List_Text_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupLcdOff_List_Text)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndSetupLcdOff_List_Text_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndSetupLcdOff_List_Text_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndSetupLcdOff_List_Text_OnKeyEnter)
EVENT_END

INT32 UIMenuWndSetupLcdOff_List_Text_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupLcdOff_List_Text_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupLcdOff_List_Text_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 ui32CurrListItem;

    ui32CurrListItem = UxList_GetData(pCtrl, LST_CURITM);	
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LCDOFF,1,ui32CurrListItem);
	
    Ux_CloseWindow(&UIMenuWndSetupLcdOffCtrl, 0);
    return NVTEVT_CONSUME;
}
