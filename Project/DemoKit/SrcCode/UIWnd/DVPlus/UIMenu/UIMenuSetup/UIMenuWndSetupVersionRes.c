//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDSETUPVERSIONRES_H
#define UIMENUWNDSETUPVERSIONRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Rect4,0,0,285,158,272,0,5,255,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Rect4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,285,158)

SHOW_MAP_BEGIN(Skin6)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin6,0,0,285,158)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text8,0,0,285,158,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal5)
SHOW_MAP_BODY(Skin6)
SHOW_MAP_BODY(Text8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal5,17,50,302,208)

SHOW_MAP_BEGIN(UIMenuWndSetupVersion_StaticTXT_FwVer)
SHOW_MAP_BODY(Normal5)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupVersion_StaticTXT_FwVer)
CREATE_STATIC_DATA(UIMenuWndSetupVersion_StaticTXT_FwVer,STRID_MODE)
CREATE_STATIC_CTRL(UIMenuWndSetupVersion_StaticTXT_FwVer,0,0,0,285,158)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupVersion_Panel)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupVersion_Panel)
DECLARE_CTRL_LIST(UIMenuWndSetupVersion_Panel)
CREATE_CTRL(UIMenuWndSetupVersion_Panel,UIMenuWndSetupVersion_Panel,CTRL_WND,NULL,0 ,17,50,302,208)
SHOW_MAP_BEGIN(UIMenuWndSetupVersion)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupVersion)
DECLARE_CTRL_LIST(UIMenuWndSetupVersion)
CREATE_CTRL(UIMenuWndSetupVersion,UIMenuWndSetupVersion,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
