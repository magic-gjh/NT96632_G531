#ifndef _UI_GRAPHICS_H_
#define _UI_GRAPHICS_H_

#include "GxDisplay.h"
#include "GxGfx.h"
#include "UIResource.h"

//Device Display
#define DD_OSD 0
#define DD_OSD2 1
#define DD_VDO 2
#define DD_VDO2 3

//Device Context
#define GxGfx_OSD 0
#define GxGfx_OSD2 1
#define GxGfx_VDO 2
#define GxGfx_VDO2 3

//Font Table
typedef enum
{
    FONT_TABLE_0 = 0,
    FONT_TABLE_1,

    // The following two elements must be the last elements
    // Don't change the order
    FONT_TABLE_TOTAL_NUM,
    ENUM_DUMMY4WORD(FONT_TABLE)
} FONT_TABLE;

extern void UI_BeginDisplay(UINT8 iDD, BOOL bClear);
extern void UI_EndDisplay(UINT8 iDD);

extern UINT32 UI_BeginScreen(void);
extern void UI_EndScreen(UINT32 ScreenObj);

extern void UI_ClearOSD(UINT32 uiColorIdx);
extern void UI_DrawOsdString(char *str, UINT32 x, UINT32 y, UINT32 uiColor);

extern void UI_Set_NotFlip(void);
extern void UI_GxDisplay_SetPalette(UINT8 LayerID, UINT16 uiStartID, UINT16 uiCount, const PALETTE_ITEM* pTable);
extern UIRender demoRender;
extern void UI_Set_FontTable(FONT_TABLE FontID, FONT* pFont);

extern RESULT GxGfx_UITextRect(DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR* pszSrc); //multi-line layout to rect, support ESC command
extern const TCHAR* GxGfx_UIGetStringID(IVALUE id);
extern SIZE_2D GxGfx_UIGetLastTextSize(void);
#endif //_UI_GRAPHICS_H_
