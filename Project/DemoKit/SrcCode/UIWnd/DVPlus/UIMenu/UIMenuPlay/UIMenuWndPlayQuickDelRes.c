//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDPLAYQUICKDELRES_H
#define UIMENUWNDPLAYQUICKDELRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_ITEM_RECT(CMD_Rectangle,Skin1Rect,0,0,319,239,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_BODY(Skin1Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon4, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal2)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_BODY(Icon4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal2,12,5,39,32)

CREATE_STATE_ITEM_DATA(ICON_MODE_PLAYBACK5,STRID_NULL,ICON_MODE_PLAYBACK,NULL) 
CREATE_STATE_ITEM_DATA(ICON_FILE_VIDEO6,STRID_NULL,ICON_FILE_VIDEO,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPlayQuickDel_Status)
SHOW_MAP_BODY(Normal2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayQuickDel_Status)
STATE_LIST_BEGIN(UIMenuWndPlayQuickDel_Status)
STATE_LIST_ITEM(ICON_MODE_PLAYBACK5)
STATE_LIST_ITEM(ICON_FILE_VIDEO6)
STATE_LIST_END

CREATE_STATE_DATA(UIMenuWndPlayQuickDel_Status,0)
CREATE_STATE_CTRL(UIMenuWndPlayQuickDel_Status,0,12,5,39,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Rect9,0,0,248,155,16,0,5,255,0,0,0)
SHOW_MAP_BEGIN(Skin7)
SHOW_MAP_BODY(Rect9)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin7,0,0,248,155)

SHOW_ITEM_RECT(CMD_Rectangle,Skin11Rect,0,0,203,41,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin11)
SHOW_MAP_BODY(Skin11Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin11,0,0,203,41)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text13,0,0,203,31,STRID_NULL,0,0,0,0,2,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal10)
SHOW_MAP_BODY(Skin11)
SHOW_MAP_BODY(Text13)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal10,59,54,262,95)

SHOW_ITEM_IMAGE(CMD_Image,Image16, 0, 0, 43,31, ICON_SEL_BAR_BEGIN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image17, 32, 0, 77,30, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image18, 64, 0, 107,30, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image19, 96, 0, 145,29, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image20, 128, 0, 176,29, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image21, 149, 0, 192,29, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image22, 171, 0, 205,30, ICON_SEL_BAR_END,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin15)
SHOW_MAP_BODY(Image16)
SHOW_MAP_BODY(Image17)
SHOW_MAP_BODY(Image18)
SHOW_MAP_BODY(Image19)
SHOW_MAP_BODY(Image20)
SHOW_MAP_BODY(Image21)
SHOW_MAP_BODY(Image22)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin15,0,0,203,41)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text24,0,0,203,31,STRID_NULL,0,0,0,0,14,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused14)
SHOW_MAP_BODY(Skin15)
SHOW_MAP_BODY(Text24)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused14,59,54,262,95)

SHOW_ITEM_RECT(CMD_Rectangle,Skin26Rect,0,0,203,41,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin26)
SHOW_MAP_BODY(Skin26Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin26,0,0,203,41)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text28,0,0,203,31,STRID_NULL,0,0,0,0,1,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable25)
SHOW_MAP_BODY(Skin26)
SHOW_MAP_BODY(Text28)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable25,59,54,262,95)

SHOW_ITEM_RECT(CMD_Rectangle,Skin30Rect,0,0,203,41,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin30)
SHOW_MAP_BODY(Skin30Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin30,0,0,203,41)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text32,0,0,203,31,STRID_NULL,0,0,0,0,1,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable29)
SHOW_MAP_BODY(Skin30)
SHOW_MAP_BODY(Text32)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable29,59,54,262,95)

SHOW_ITEM_RECT(CMD_Rectangle,Skin34Rect,0,0,203,41,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin34)
SHOW_MAP_BODY(Skin34Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin34,0,0,203,41)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text36,0,0,203,31,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed33)
SHOW_MAP_BODY(Skin34)
SHOW_MAP_BODY(Text36)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed33,59,54,262,95)

CREATE_MENU_ITEM_DATA(STRID_DELETECURRENT37,STRID_DELETECURRENT,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_DELETEALL38,STRID_DELETEALL,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPlayQuickDel_Menu)
SHOW_MAP_BODY(Normal10)
SHOW_MAP_BODY(Focused14)
SHOW_MAP_BODY(Normal_Disable25)
SHOW_MAP_BODY(Focused_Disable29)
SHOW_MAP_BODY(Pressed33)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayQuickDel_Menu)
MENU_LIST_BEGIN(UIMenuWndPlayQuickDel_Menu)
MENU_LIST_ITEM(STRID_DELETECURRENT37)
MENU_LIST_ITEM(STRID_DELETEALL38)
MENU_LIST_END

CREATE_MENU_DATA(UIMenuWndPlayQuickDel_Menu,0,2,MENU_LAYOUT_VERTICAL|MENU_DISABLE_SHOW|MENU_SCROLL_CYCLE|MENU_SCROLL_NEXT_PAGE|MENU_DISABLE_NORMAL|MENU_DRAW_IMAGE)

CREATE_MENU_CTRL(UIMenuWndPlayQuickDel_Menu,UIMenuWndPlayQuickDel_Menu,0,23,7,226,48)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndPlayQuickDel_Panel)
SHOW_MAP_BODY(Skin7)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayQuickDel_Panel)
DECLARE_CTRL_LIST(UIMenuWndPlayQuickDel_Panel)
CREATE_CTRL(UIMenuWndPlayQuickDel_Panel,UIMenuWndPlayQuickDel_Panel,CTRL_WND,NULL,0 ,36,47,284,202)
SHOW_MAP_BEGIN(UIMenuWndPlayQuickDel)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayQuickDel)
DECLARE_CTRL_LIST(UIMenuWndPlayQuickDel)
CREATE_CTRL(UIMenuWndPlayQuickDel,UIMenuWndPlayQuickDel,CTRL_WND,NULL,0 ,0,0,319,239)
#endif