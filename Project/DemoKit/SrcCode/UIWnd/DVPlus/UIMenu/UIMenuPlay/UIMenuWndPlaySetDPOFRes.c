//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDPLAYSETDPOFRES_H
#define UIMENUWNDPLAYSETDPOFRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Rect4,0,0,248,158,272,0,5,255,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Rect4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,248,155)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar7, 0, 0, 39,56, ICON_SEL_BAR_DATE,0,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up8, 12, 2, 25,15, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down9, 12, 45, 25,56, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin6Rect,0,0,39,56,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin6)
SHOW_MAP_BODY(Skin6Rect)
SHOW_MAP_BODY(Image_Sel_Bar7)
SHOW_MAP_BODY(Image_Up8)
SHOW_MAP_BODY(Image_Down9)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin6,0,0,39,56)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text11,0,0,39,56,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal5)
SHOW_MAP_BODY(Skin6)
SHOW_MAP_BODY(Text11)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal5,161,90,200,146)

SHOW_ITEM_RECT(CMD_Rectangle,Skin13Rect,0,0,39,56,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin13)
SHOW_MAP_BODY(Skin13Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin13,0,0,39,56)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text15,0,0,39,56,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused12)
SHOW_MAP_BODY(Skin13)
SHOW_MAP_BODY(Text15)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused12,161,90,200,146)

SHOW_ITEM_RECT(CMD_Rectangle,Skin17Rect,0,0,39,56,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin17)
SHOW_MAP_BODY(Skin17Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin17,0,0,39,56)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text19,4,15,37,44,STRID_NULL,0,0,0,0,1,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable16)
SHOW_MAP_BODY(Skin17)
SHOW_MAP_BODY(Text19)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable16,161,90,200,146)

SHOW_ITEM_RECT(CMD_Rectangle,Skin21Rect,0,0,39,56,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin21)
SHOW_MAP_BODY(Skin21Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin21,0,0,39,56)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text23,4,15,37,44,STRID_NULL,0,0,0,0,1,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable20)
SHOW_MAP_BODY(Skin21)
SHOW_MAP_BODY(Text23)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable20,161,90,200,146)

SHOW_ITEM_RECT(CMD_Rectangle,Skin25Rect,0,0,39,56,0,0,255,255,0,0,0)
SHOW_MAP_BEGIN(Skin25)
SHOW_MAP_BODY(Skin25Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin25,0,0,39,56)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text27,4,15,37,44,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed24)
SHOW_MAP_BODY(Skin25)
SHOW_MAP_BODY(Text27)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed24,161,90,200,146)

CREATE_BUTTON_ITEM_DATA(STRID_A228,STRID_A2,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPlaySetDPOF_Button)
SHOW_MAP_BODY(Normal5)
SHOW_MAP_BODY(Focused12)
SHOW_MAP_BODY(Normal_Disable16)
SHOW_MAP_BODY(Focused_Disable20)
SHOW_MAP_BODY(Pressed24)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlaySetDPOF_Button)
BUTTON_LIST_BEGIN(UIMenuWndPlaySetDPOF_Button)
BUTTON_LIST_ITEM(STRID_A228)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndPlaySetDPOF_Button,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndPlaySetDPOF_Button,UIMenuWndPlaySetDPOF_Button,0,105,40,144,96)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin30)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin30,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text32,0,0,91,27,STRID_NULL,0,0,0,0,2,1,1,0,0,18,0,0,0,0)
SHOW_MAP_BEGIN(Normal29)
SHOW_MAP_BODY(Skin30)
SHOW_MAP_BODY(Text32)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal29,69,105,160,132)

SHOW_MAP_BEGIN(UIMenuWndPlaySetDPOF_Static)
SHOW_MAP_BODY(Normal29)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlaySetDPOF_Static)
CREATE_STATIC_DATA(UIMenuWndPlaySetDPOF_Static,STRID_PRINT)
CREATE_STATIC_CTRL(UIMenuWndPlaySetDPOF_Static,0,13,55,104,82)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin35)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin35,0,0,91,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text37,0,0,91,27,STRID_NULL,0,0,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal34)
SHOW_MAP_BODY(Skin35)
SHOW_MAP_BODY(Text37)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal34,201,105,292,132)

SHOW_MAP_BEGIN(UIMenuWndPlaySetDPOF_Static2)
SHOW_MAP_BODY(Normal34)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlaySetDPOF_Static2)
CREATE_STATIC_DATA(UIMenuWndPlaySetDPOF_Static2,STRID_IMAGES)
CREATE_STATIC_CTRL(UIMenuWndPlaySetDPOF_Static2,0,145,55,236,82)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndPlaySetDPOF_Panel)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlaySetDPOF_Panel)
DECLARE_CTRL_LIST(UIMenuWndPlaySetDPOF_Panel)
CREATE_CTRL(UIMenuWndPlaySetDPOF_Panel,UIMenuWndPlaySetDPOF_Panel,CTRL_WND,NULL,0 ,56,50,304,205)
SHOW_MAP_BEGIN(UIMenuWndPlaySetDPOF)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPlaySetDPOF)
DECLARE_CTRL_LIST(UIMenuWndPlaySetDPOF)
CREATE_CTRL(UIMenuWndPlaySetDPOF,UIMenuWndPlaySetDPOF,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
