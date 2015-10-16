//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDPLAYBACKRES_H
#define UIMENUWNDPLAYBACKRES_H

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

SHOW_ITEM_RECT(CMD_Rectangle,Skin2Rect,0,0,295,169,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Skin2Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,295,169)

SHOW_MAP_BEGIN(PlaybackMenuBackground)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(PlaybackMenuBackground)
DECLARE_CTRL_LIST(PlaybackMenuBackground)
CREATE_CTRL(PlaybackMenuBackground,PlaybackMenuBackground,CTRL_WND,NULL,0 ,12,44,307,213)
SHOW_ITEM_IMAGE(CMD_Image,Image4, 118, 0, 178,39, ICON_TITLE_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image5, 176, 0, 236,39, ICON_TITLE_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image6, 235, 0, 295,39, ICON_TITLE_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image7, 0, 0, 60,39, ICON_MENU_PLAYBACK_ON,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image8, 60, 0, 120,39, ICON_MENU_SETUP_OFF,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_BODY(Image4)
SHOW_MAP_BODY(Image5)
SHOW_MAP_BODY(Image6)
SHOW_MAP_BODY(Image7)
SHOW_MAP_BODY(Image8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,295,39)

SHOW_ITEM_IMAGE(CMD_Image,Image11, 2, 0, 48,37, ICON_TITLE_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image12, 48, 0, 94,37, ICON_TITLE_BAR,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin10)
SHOW_MAP_BODY(Image11)
SHOW_MAP_BODY(Image12)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin10,0,0,108,39)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text14,6,6,99,30,STRID_NULL,0,0,0,0,2,1,1,0,0,18,0,0,0,0)
SHOW_MAP_BEGIN(Normal9)
SHOW_MAP_BODY(Skin10)
SHOW_MAP_BODY(Text14)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal9,197,5,305,44)

CREATE_STATE_ITEM_DATA(STRID_PLAYBACK15,STRID_PLAYBACK,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(STRID_DELETE16,STRID_DELETE,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(STRID_PROTECT17,STRID_PROTECT,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(STRID_SLIDE_SHOW18,STRID_SLIDE_SHOW,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(STRID_DPOF19,STRID_DPOF,ICONID_NULL,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPlayback_TitleName)
SHOW_MAP_BODY(Normal9)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_TitleName)
STATE_LIST_BEGIN(UIMenuWndPlayback_TitleName)
STATE_LIST_ITEM(STRID_PLAYBACK15)
STATE_LIST_ITEM(STRID_DELETE16)
STATE_LIST_ITEM(STRID_PROTECT17)
STATE_LIST_ITEM(STRID_SLIDE_SHOW18)
STATE_LIST_ITEM(STRID_DPOF19)
STATE_LIST_END

CREATE_STATE_DATA(UIMenuWndPlayback_TitleName,0)
CREATE_STATE_CTRL(UIMenuWndPlayback_TitleName,0,185,0,293,39)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndPlayback_TitleBar)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_TitleBar)
DECLARE_CTRL_LIST(UIMenuWndPlayback_TitleBar)
CREATE_CTRL(UIMenuWndPlayback_TitleBar,UIMenuWndPlayback_TitleBar,CTRL_WND,NULL,0 ,12,5,307,44)
SHOW_ITEM_IMAGE(CMD_Image,Image21, 0, 0, 31,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image22, 31, 0, 62,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image23, 62, 0, 93,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image24, 93, 0, 124,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image25, 124, 0, 155,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image26, 155, 0, 186,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image27, 186, 0, 217,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image28, 217, 0, 248,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image29, 248, 0, 279,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image30, 264, 0, 295,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin20)
SHOW_MAP_BODY(Image21)
SHOW_MAP_BODY(Image22)
SHOW_MAP_BODY(Image23)
SHOW_MAP_BODY(Image24)
SHOW_MAP_BODY(Image25)
SHOW_MAP_BODY(Image26)
SHOW_MAP_BODY(Image27)
SHOW_MAP_BODY(Image28)
SHOW_MAP_BODY(Image29)
SHOW_MAP_BODY(Image30)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin20,0,0,295,22)

SHOW_MAP_BEGIN(Skin32)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin32,0,0,57,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon33, 0, 2, 54,24, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal31)
SHOW_MAP_BODY(Skin32)
SHOW_MAP_BODY(Icon33)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal31,24,212,81,239)

SHOW_MAP_BEGIN(UIMenuWndPlayback_ReturnTips)
SHOW_MAP_BODY(Normal31)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_ReturnTips)
CREATE_STATIC_DATA(UIMenuWndPlayback_ReturnTips,ICON_MENU_RETURN)
CREATE_STATIC_CTRL(UIMenuWndPlayback_ReturnTips,0,12,-2,69,25)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_IMAGE(CMD_Image,Image36, 0, 2, 16,21, ICON_DIR_LEFT,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image37, 31, 6, 45,25, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image38, 50, 2, 64,18, ICON_DIR_RIGHT,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin35)
SHOW_MAP_BODY(Image36)
SHOW_MAP_BODY(Image37)
SHOW_MAP_BODY(Image38)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin35,0,0,82,27)

SHOW_MAP_BEGIN(UIMenuWndPlayback_LDRTips)
SHOW_MAP_BODY(Skin35)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_LDRTips)
DECLARE_CTRL_LIST(UIMenuWndPlayback_LDRTips)
CREATE_CTRL(UIMenuWndPlayback_LDRTips,UIMenuWndPlayback_LDRTips,CTRL_WND,NULL,0 ,148,0,230,27)
SHOW_ITEM_IMAGE(CMD_Image,Image40, 0, 2, 16,21, ICON_DIR_LEFT,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image41, 14, 5, 28,24, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image42, 31, 6, 45,22, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image43, 50, 2, 64,18, ICON_DIR_RIGHT,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin39)
SHOW_MAP_BODY(Image40)
SHOW_MAP_BODY(Image41)
SHOW_MAP_BODY(Image42)
SHOW_MAP_BODY(Image43)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin39,0,0,83,28)

SHOW_MAP_BEGIN(UIMenuWndPlayback_LUDRTips)
SHOW_MAP_BODY(Skin39)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_LUDRTips)
DECLARE_CTRL_LIST(UIMenuWndPlayback_LUDRTips)
CREATE_CTRL(UIMenuWndPlayback_LUDRTips,UIMenuWndPlayback_LUDRTips,CTRL_WND,NULL,0 ,148,0,231,28)
SHOW_ITEM_IMAGE(CMD_Image,Image45, 0, 2, 16,21, ICON_DIR_LEFT,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image46, 14, 5, 28,24, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image47, 31, 6, 45,22, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image48, 68, 2, 82,18, ICON_OK,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin44)
SHOW_MAP_BODY(Image45)
SHOW_MAP_BODY(Image46)
SHOW_MAP_BODY(Image47)
SHOW_MAP_BODY(Image48)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin44,0,0,110,27)

SHOW_MAP_BEGIN(UIMenuWndPlayback_LUDOKTips)
SHOW_MAP_BODY(Skin44)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_LUDOKTips)
DECLARE_CTRL_LIST(UIMenuWndPlayback_LUDOKTips)
CREATE_CTRL(UIMenuWndPlayback_LUDOKTips,UIMenuWndPlayback_LUDOKTips,CTRL_WND,NULL,0 ,148,0,258,27)
SHOW_MAP_BEGIN(UIMenuWndPlayback_TipsBar)
SHOW_MAP_BODY(Skin20)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_TipsBar)
DECLARE_CTRL_LIST(UIMenuWndPlayback_TipsBar)
CREATE_CTRL(UIMenuWndPlayback_TipsBar,UIMenuWndPlayback_TipsBar,CTRL_WND,NULL,0 ,12,214,307,236)
SHOW_ITEM_RECT(CMD_Rectangle,Skin50Rect,0,0,284,40,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin50)
SHOW_MAP_BODY(Skin50Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin50,0,0,284,40)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon51, 2, 7, 29,34, ICONID_NULL,83886080,0,0,3,0,0,0,0)
SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text53,40,7,284,31,STRID_NULL,0,0,0,0,2,1,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal49)
SHOW_MAP_BODY(Skin50)
SHOW_MAP_BODY(Icon51)
SHOW_MAP_BODY(Text53)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal49,18,49,302,89)

SHOW_ITEM_IMAGE(CMD_Image,Image56, 0, 5, 32,36, ICON_SEL_BAR_BEGIN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image57, 32, 5, 64,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image58, 64, 5, 96,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image59, 96, 5, 128,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image60, 126, 5, 158,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image61, 157, 5, 189,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image62, 189, 5, 221,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image63, 220, 5, 252,36, ICON_SEL_BAR_MED,0,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image64, 252, 5, 284,36, ICON_SEL_BAR_END,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin55Rect,0,0,284,40,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin55)
SHOW_MAP_BODY(Skin55Rect)
SHOW_MAP_BODY(Image56)
SHOW_MAP_BODY(Image57)
SHOW_MAP_BODY(Image58)
SHOW_MAP_BODY(Image59)
SHOW_MAP_BODY(Image60)
SHOW_MAP_BODY(Image61)
SHOW_MAP_BODY(Image62)
SHOW_MAP_BODY(Image63)
SHOW_MAP_BODY(Image64)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin55,0,0,284,40)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon65, 2, 7, 29,34, ICONID_NULL,83886080,0,0,1,0,0,0,0)
SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text67,40,7,284,31,STRID_NULL,0,0,0,0,14,5,0,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused54)
SHOW_MAP_BODY(Skin55)
SHOW_MAP_BODY(Icon65)
SHOW_MAP_BODY(Text67)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused54,18,49,302,89)

SHOW_ITEM_RECT(CMD_Rectangle,Skin69Rect,0,0,284,40,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin69)
SHOW_MAP_BODY(Skin69Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin69,0,0,284,40)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon70, 2, 7, 29,34, ICONID_NULL,83886080,0,0,3,0,0,0,0)
SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text72,40,7,284,31,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable68)
SHOW_MAP_BODY(Skin69)
SHOW_MAP_BODY(Icon70)
SHOW_MAP_BODY(Text72)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable68,18,49,302,89)

SHOW_ITEM_RECT(CMD_Rectangle,Skin74Rect,0,0,284,40,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin74)
SHOW_MAP_BODY(Skin74Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin74,0,0,284,40)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon75, 2, 7, 29,34, ICONID_NULL,83886080,0,0,3,0,0,0,0)
SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text77,40,7,284,31,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable73)
SHOW_MAP_BODY(Skin74)
SHOW_MAP_BODY(Icon75)
SHOW_MAP_BODY(Text77)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable73,18,49,302,89)

SHOW_ITEM_RECT(CMD_Rectangle,Skin79Rect,0,0,284,40,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin79)
SHOW_MAP_BODY(Skin79Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin79,0,0,284,40)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon80, 2, 7, 29,34, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text82,40,7,284,31,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed78)
SHOW_MAP_BODY(Skin79)
SHOW_MAP_BODY(Icon80)
SHOW_MAP_BODY(Text82)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed78,18,49,302,89)

CREATE_MENU_ITEM_DATA(STRID_DELETE83,STRID_DELETE,ICON_DELETE,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_PROTECT84,STRID_PROTECT,ICON_PROTECT,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_SLIDE_SHOW85,STRID_SLIDE_SHOW,ICON_SLIDE_SHOW,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_DPOF86,STRID_DPOF,ICON_DPOF,STATUS_ENABLE,NULL,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPlayback_Item)
SHOW_MAP_BODY(Normal49)
SHOW_MAP_BODY(Focused54)
SHOW_MAP_BODY(Normal_Disable68)
SHOW_MAP_BODY(Focused_Disable73)
SHOW_MAP_BODY(Pressed78)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_Item)
MENU_LIST_BEGIN(UIMenuWndPlayback_Item)
MENU_LIST_ITEM(STRID_DELETE83)
MENU_LIST_ITEM(STRID_PROTECT84)
MENU_LIST_ITEM(STRID_SLIDE_SHOW85)
MENU_LIST_ITEM(STRID_DPOF86)
MENU_LIST_END

CREATE_MENU_DATA(UIMenuWndPlayback_Item,0,4,MENU_LAYOUT_VERTICAL|MENU_DISABLE_SHOW|MENU_SCROLL_CYCLE|MENU_SCROLL_NEXT_PAGE|MENU_DISABLE_NORMAL|MENU_DRAW_IMAGE)

CREATE_MENU_CTRL(UIMenuWndPlayback_Item,UIMenuWndPlayback_Item,0,18,49,302,89)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_IMAGE(CMD_Image,Image89, 0, 0, 37,22, ICON_HINT_BAR,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin88)
SHOW_MAP_BODY(Image89)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin88,0,0,37,22)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text91,0,0,37,22,STRID_NULL,0,0,0,0,2,1,1,256,0,18,0,0,0,0)
SHOW_MAP_BEGIN(Normal87)
SHOW_MAP_BODY(Skin88)
SHOW_MAP_BODY(Text91)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal87,270,214,307,236)

SHOW_MAP_BEGIN(UIMenuWndPlayback_PageNum_Static)
SHOW_MAP_BODY(Normal87)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback_PageNum_Static)
CREATE_STATIC_DATA(UIMenuWndPlayback_PageNum_Static,STRID_MODE)
CREATE_STATIC_CTRL(UIMenuWndPlayback_PageNum_Static,0,270,214,307,236)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndPlayback)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlayback)
DECLARE_CTRL_LIST(UIMenuWndPlayback)
CREATE_CTRL(UIMenuWndPlayback,UIMenuWndPlayback,CTRL_WND,NULL,0 ,0,0,319,239)
#endif