#include "IDE.h"
#include "GlobalVar.h"
#include "SysCfg.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "UIMenuSetupSettings.h"
#include "UICustomer.h"
#define OSD_USE_DOUBLE_BUFFER
//#define VDO_USE_DOUBLE_BUFFER

extern LCDSIZE g_LCDSize;
SIZE_2D  tmpImageSize={30,20};
DC* _sys_pDC[4];
POINT_2D _sys_Orig[4];
BOOL  bShowArabic = FALSE;
FONT* UIFontTable[FONT_TABLE_TOTAL_NUM];
typedef DC** DCLIST;

static BOOL bNotFlip = FALSE; //for flip control

static BOOL bChagePalette = FALSE;
static UINT8 g_LayerID;
static UINT16 g_uiStartID;
static UINT16 g_uiCount;
static const PALETTE_ITEM* g_pTable;
MAPPING_ITEM *colorTable[5];
MAPPING_ITEM ColorMap1[16]={0,0,14,3,4,5,6,7,8,9,10,11,12,13,14,15};    // index2->index14 (white->blue)
MAPPING_ITEM ColorMap2[16]={0,0,2,3,4,5,9,7,8,6,10,11,12,13,14,1};      // ??
MAPPING_ITEM ColorMap3[16]={0,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15};     // index1->index0 (black->transparent)
MAPPING_ITEM ColorMap4[16]={0,0,3,3,4,5,6,7,8,9,10,11,12,13,14,15};     // index2->index3 (white->red)

void UI_Begin_test()
{
    GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, 0);
    GxDisplay_Flush(LAYER_OSD1);
}
void UI_BeginDisplay(UINT8 iDD, BOOL bClear)
{
    RESULT r;
    LAYER_INIT LayerInit;
    UINT32 w = OSD_W;//g_LCDSize.uiWinWidth;
    UINT32 h = OSD_H;//g_LCDSize.uiWinHeight;
    UINT32 vdow = g_LCDSize.uiWinWidth;
    UINT32 vdoh = g_LCDSize.uiWinHeight;
    UINT32 uiBufBlk;
    UINT32 uiBufAddr;

    switch(iDD)
    {
    case DD_OSD:

    uiBufBlk = POOL_ID_OSD1;
    get_blk((void *)&uiBufAddr, uiBufBlk);
    rel_blk((ID)uiBufBlk, (VP)uiBufAddr);

    LayerInit.uiType = TYPE_FB;
#if (_OSD_COLOR_ == _OSD_COLOR_8_BIT_)
    LayerInit.uiPxlfmt = PXLFMT_INDEX8;
#else
    LayerInit.uiPxlfmt = PXLFMT_INDEX4;
#endif
    LayerInit.uiWidth = w;
    LayerInit.uiHeight = h;
#ifdef OSD_USE_DOUBLE_BUFFER
    LayerInit.uiBufCount = 1;
    LayerInit.uiSwapEffect = SWAPEFFECT_COPY;
#else
    LayerInit.uiBufCount = 0;
    LayerInit.uiSwapEffect = SWAPEFFECT_DISCARD;
#endif
    switch(LayerInit.uiPxlfmt)
    {
    case PXLFMT_INDEX1: LayerInit.uiBufSize = (w*h)>>3;break;
    case PXLFMT_INDEX2: LayerInit.uiBufSize = (w*h)>>2;break;
    case PXLFMT_INDEX4: LayerInit.uiBufSize = (w*h)>>1;break;
    case PXLFMT_INDEX8: LayerInit.uiBufSize = (w*h)>>0;break;
    }
    LayerInit.pBufAddr[0] = uiBufAddr;
#ifdef OSD_USE_DOUBLE_BUFFER
    LayerInit.pBufAddr[1] = uiBufAddr + LayerInit.uiBufSize;
#else
    LayerInit.pBufAddr[1] = 0;
#endif
    LayerInit.pBufAddr[2] = 0;
    LayerInit.win.x= 0;
    LayerInit.win.y = 0;
    LayerInit.win.w = g_LCDSize.uiWinWidth;
    LayerInit.win.h = g_LCDSize.uiWinHeight;
    LayerInit.uiWinAttr = 0;

    //GxDisplay_InitScale(g_LCDSize.uiWinWidth, g_LCDSize.uiWinHeight);
    debug_err(("OSD1 buf=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize));
    debug_err(("OSD1 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight));
    debug_err(("OSD1 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h));
    r = GxDisplay_InitLayer(LAYER_OSD1, &LayerInit, bClear);

    GxDisplay_WaitVD();

    //initial DC palette
#if (_OSD_COLOR_ == _OSD_COLOR_8_BIT_)
    //GxDisplay_SetPalette(LAYER_OSD1,0,256,gDemo_Palette_Palette);
    GxDisplay_SetPalette(LAYER_OSD1,0,256,gDemoKit_Palette_Palette);
#else
    //GxDisplay_SetPalette(LAYER_OSD1,0,16,gDemo_Palette_Palette);
    GxDisplay_SetPalette(LAYER_OSD1,0,16,gDemoKit_Palette_Palette);
#endif
    GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, 1);
    GxDisplay_Flush(LAYER_OSD1);

    break;

    case DD_VDO:

    uiBufBlk = POOL_ID_BG;
    get_blk((void *)&uiBufAddr, uiBufBlk);
    rel_blk((ID)uiBufBlk, (VP)uiBufAddr);

    LayerInit.uiType = TYPE_FB;
    LayerInit.uiPxlfmt = PXLFMT_YUV422;
        LayerInit.uiWidth = vdow;
        LayerInit.uiHeight = vdoh;
#ifdef VDO_USE_DOUBLE_BUFFER
    LayerInit.uiBufCount = 1;
    LayerInit.uiSwapEffect = SWAPEFFECT_COPY;
#else
    LayerInit.uiBufCount = 0;
    LayerInit.uiSwapEffect = SWAPEFFECT_DISCARD;
#endif
    switch(LayerInit.uiPxlfmt)
    {
        case PXLFMT_YUV422: LayerInit.uiBufSize = (vdow*vdoh)*2;break;
    }
    LayerInit.pBufAddr[0] = uiBufAddr;
#ifdef VDO_USE_DOUBLE_BUFFER
    LayerInit.pBufAddr[1] = uiBufAddr + LayerInit.uiBufSize;
#else
    LayerInit.pBufAddr[1] = 0;
#endif
    LayerInit.pBufAddr[2] = 0;
    LayerInit.win.x= 0;
    LayerInit.win.y = 0;
        LayerInit.win.w = g_LCDSize.uiWinWidth;
        LayerInit.win.h = g_LCDSize.uiWinHeight;

    debug_err(("VDO1 buf=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize));
    debug_err(("VDO1 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight));
    debug_err(("VDO1 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h));
    r = GxDisplay_InitLayer(LAYER_VDO1, &LayerInit, FALSE);

    GxDisplay_WaitVD();
    GxDisplay_Set(LAYER_VDO1, LAYER_STATE_ENABLE, 1);
    GxDisplay_Flush(LAYER_VDO1);

#if 0
    //initial DC temp buffer for JPEG decode
    uiBufBlk = POOL_ID_CAPTURE;
    get_blk((void *)&uiBufAddr, uiBufBlk);
    rel_blk((ID)uiBufBlk, (VP)uiBufAddr);
    GxGfx_AssignTempPool((UINT8*)uiBufAddr, POOL_SIZE_CAPTURE);
#endif
    break;
    case DD_VDO2:

        uiBufBlk = POOL_ID_BGTEMP;
        get_blk((void *)&uiBufAddr, uiBufBlk);
        rel_blk((ID)uiBufBlk, (VP)uiBufAddr);

        LayerInit.uiType = TYPE_FB;
        LayerInit.uiPxlfmt = PXLFMT_YUV422;
        LayerInit.uiWidth = w;
        LayerInit.uiHeight = h;
    #ifdef VDO_USE_DOUBLE_BUFFER
        LayerInit.uiBufCount = 1;
        LayerInit.uiSwapEffect = SWAPEFFECT_COPY;
    #else
        LayerInit.uiBufCount = 0;
        LayerInit.uiSwapEffect = SWAPEFFECT_DISCARD;
    #endif
        switch(LayerInit.uiPxlfmt)
        {
        case PXLFMT_YUV422: LayerInit.uiBufSize = (vdow*vdoh)*2;break;
        }
        LayerInit.pBufAddr[0] = uiBufAddr;
    #ifdef VDO_USE_DOUBLE_BUFFER
        LayerInit.pBufAddr[1] = uiBufAddr + LayerInit.uiBufSize;
    #else
        LayerInit.pBufAddr[1] = 0;
    #endif
        LayerInit.pBufAddr[2] = 0;
        LayerInit.win.x = 0;
        LayerInit.win.y = 0;
        LayerInit.win.w = g_LCDSize.uiWinWidth;
        LayerInit.win.h = g_LCDSize.uiWinHeight;
        LayerInit.uiWinAttr = 0;

        debug_err(("VDO2 buf=%08x, size=%08x\n\r",LayerInit.pBufAddr[0], LayerInit.uiBufSize));
        debug_err(("VDO2 buf.w=%d, buf.h=%d;\n\r",LayerInit.uiWidth, LayerInit.uiHeight));
        debug_err(("VDO2 win.w=%d, win.h=%d;\n\r",LayerInit.win.w, LayerInit.win.h));
        r = GxDisplay_InitLayer(LAYER_VDO2, &LayerInit, FALSE);

        GxDisplay_WaitVD();
        GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, 0); //ricky@0131/2008,control by UI_Show_BG();
        GxDisplay_Flush(LAYER_VDO2);
    #if 0
        //initial DC temp buffer for JPEG decode
        uiBufBlk = POOL_ID_BGTEMP;//POOL_ID_CAPTURE;
        get_blk((void *)&uiBufAddr, uiBufBlk);
        rel_blk((ID)uiBufBlk, (VP)uiBufAddr);
        GxGfx_AssignTempPool((UINT8*)uiBufAddr, POOL_SIZE_BGTEMP );//POOL_SIZE_CAPTURE
    #endif
        break;
    }

    //set default Pen ,text, image Strok,Pen Color
    GxGfx_SetAllDefault();
    GxGfx_SetTextColor(CLRID_IDX_BLACK, CLRID_IDX_WHITE, TEXTFORECOLOR3_DEFAULT);
    GxGfx_SetImageStroke(ROP_KEY,IMAGEPARAM_DEFAULT);
    GxGfx_SetImageColor(CLRID_IDX_TRANSPART,IMAGEPARAM_DEFAULT);

    //initial general BR state
    GxGfx_SetImageTable((const IMAGE_TABLE*)&gDemoKit_Image);

    GxGfx_SetTextStroke((const FONT*)&gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
    UIFontTable[FONT_TABLE_0]= (FONT*)(&gDemoKit_Font);
    UIFontTable[FONT_TABLE_1]= NULL;   //mp3 font size is very large, only assigned whenever entering mp3 mode
    Ux_SetFontTable(UIFontTable);

    colorTable[0]=IMAGEMAPPING_DEFAULT;
    colorTable[1]=ColorMap1;
    colorTable[2]=ColorMap2;
    colorTable[3]=ColorMap3;
    colorTable[4]=ColorMap4;
    Ux_SetColorMapTable(colorTable);

    GxGfx_SetStringTable((const STRING_TABLE*)Get_LanguageValue(Get_LanguageIndex()));
}

void UI_EndDisplay(UINT8 iDD)
{
    switch(iDD)
    {
    case DD_OSD:

    GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, 0);
    GxDisplay_Flush(LAYER_OSD1);
    break;

    case DD_VDO:

    GxDisplay_Set(LAYER_VDO1, LAYER_STATE_ENABLE, 0);
    GxDisplay_Flush(LAYER_VDO1);
    break;

    case DD_VDO2:

    GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, 0);
    GxDisplay_Flush(LAYER_VDO2);
    break;
    }
}

UIScreen UI_BeginScreen(void)
{
    wai_sem(SEMID_UIGRAPHIC);

    _sys_pDC[0] = GxDisplay_BeginDraw(LAYER_OSD1);
   // _sys_pDC[1] = GxDisplay_BeginDraw(LAYER_OSD2);
   // _sys_pDC[2] = GxDisplay_BeginDraw(LAYER_VDO1);
    _sys_pDC[3] = GxDisplay_BeginDraw(LAYER_VDO2);
//    debug_err(("[UI_BeginScreen]_sys_pDC[3] = 0x%x\n\r",_sys_pDC[3]));
    _sys_Orig[0] = GxGfx_GetOrigin(_sys_pDC[0]);
   // _sys_Orig[1] = GxGfx_GetOrigin(_sys_pDC[1]);
   // _sys_Orig[2] = GxGfx_GetOrigin(_sys_pDC[2]);
    _sys_Orig[3] = GxGfx_GetOrigin(_sys_pDC[3]);
    return (UIScreen)(DCLIST)(_sys_pDC);
}

void UI_EndScreen(UIScreen ScreenObj)
{
    GxGfx_SetOrigin(_sys_pDC[0], _sys_Orig[0].x, _sys_Orig[0].y);
    GxDisplay_EndDraw(LAYER_OSD1, _sys_pDC[0]);

    #if 0 //if not init VDO ,should not call end draw //Janice 20090203
    GxGfx_SetOrigin(_sys_pDC[1], _sys_Orig[1].x, _sys_Orig[1].y);
    GxDisplay_EndDraw(LAYER_VDO1, _sys_pDC[1]);
    #endif

    #if 0
    GxGfx_SetOrigin(_sys_pDC[2], _sys_Orig[2].x, _sys_Orig[2].y);
    GxDisplay_EndDraw(LAYER_VDO1, _sys_pDC[2]);
    #endif

    GxGfx_SetOrigin(_sys_pDC[3], _sys_Orig[3].x, _sys_Orig[3].y);
    GxDisplay_EndDraw(LAYER_VDO2, _sys_pDC[3]);


    if(bChagePalette)
    {
        GxDisplay_SetPalette(g_LayerID,g_uiStartID,g_uiCount,g_pTable);
        bChagePalette = FALSE;
    }

    if(bNotFlip)
    {
        bNotFlip = FALSE;
    }
    else
    {   //2009/03/11 Janice ::update Gx lib and set TURE for fix screen blink

        if(ide_get_en())
            GxDisplay_Flip(TRUE); // bFlipWaitFrameEnd
    }
    sig_sem(SEMID_UIGRAPHIC);
}

void UI_BeginWindow(VControl* pWnd, UIScreen ScreenObj)
{
    DC** pDCList = (DC**)ScreenObj;
    //set relative position by Window
    GxGfx_SetOrigin(pDCList[GxGfx_OSD],
        pWnd->rect.x1,
        pWnd->rect.y1);
    #if 0
    GxGfx_SetOrigin(pDCList[GxGfx_VDO],
        pWnd->rect.x1,
        pWnd->rect.y1);
    #endif
    GxGfx_SetOrigin(pDCList[GxGfx_VDO2],
        pWnd->rect.x1,
        pWnd->rect.y1);
}

void UI_EndWindow(VControl* pWnd, UIScreen ScreenObj)
{
}

void UI_BeginControl(VControl* pParent, VControl* pControl, UIScreen ScreenObj)
{
    DC** pDCList = (DC**)ScreenObj;
    Ux_RECT  rect= {0,0,0,0};
    rect.x1 =  pControl->rect.x1;
    rect.y1 =  pControl->rect.y1;

    while(pParent)
    {
        rect.x1+= pParent->rect.x1;
        rect.y1+= pParent->rect.y1;
        pParent= pParent->pParent;
    }

    //2009/03/29::Janice::
    //OSD positon is depend on 240 layout,but LCD has 234 dots
    //shift 3 pilex higher (240-234)/2 = 3
    GxGfx_SetOrigin(pDCList[GxGfx_OSD], rect.x1,rect.y1);
    #if 0
    GxGfx_SetOrigin(pDCList[GxGfx_VDO], rect.x1,rect.y1);
    #endif
    GxGfx_SetOrigin(pDCList[GxGfx_VDO2], rect.x1,rect.y1);

}

void UI_EndControl(VControl* pCtrl, VControl* pControl, UIScreen ScreenObj)
{
}

void UI_ClearScreen(UIScreen ScreenObj)
{
    DC** pDCList = (DC**)ScreenObj;

    GxGfx_Clear(pDCList[GxGfx_OSD]);
}

void UI_ClearOSD(UINT32 uiColorIdx)
{
    DC** pDCList;

    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(0, uiColorIdx, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD], 0, 0, pDCList[GxGfx_OSD]->size.w, pDCList[GxGfx_OSD]->size.h);
    UI_EndScreen((UINT32)pDCList);
}

void UI_DrawOsdString(char *str, UINT32 x, UINT32 y, UINT32 uiColor)
{
    DC** pDCList;

    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetTextColor(uiColor, _OSD_INDEX_TRANSPART, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], x, y, str);
    UI_EndScreen((UINT32)pDCList);
}

UIRender demoRender =
{
    UI_BeginScreen,
    UI_EndScreen,
    UI_BeginWindow,
    UI_EndWindow,
    UI_BeginControl,
    UI_EndControl,
    UI_ClearScreen
};

//set next redraw not change buffer
//after next redraw ,bNotFlip would be set true
void UI_Set_NotFlip(void)
{
    bNotFlip = TRUE;
}
//not set palette to HW directly,until UI_EndScreen to set palette ,for change color in current screen
void UI_GxDisplay_SetPalette(UINT8 LayerID, UINT16 uiStartID, UINT16 uiCount, const PALETTE_ITEM* pTable)
{
    bChagePalette = TRUE;
    g_LayerID = LayerID;
    g_uiStartID =  uiStartID;
    g_uiCount = uiCount;
    g_pTable = pTable;

}

void UI_Set_FontTable(FONT_TABLE FontID, FONT* pFont)
{
    GxGfx_SetTextStroke((const FONT*)pFont, FONTSTYLE_NORMAL, SCALE_1X);
    UIFontTable[FontID]= (FONT*)pFont;
    Ux_SetFontTable(UIFontTable);
}

#if(SUP_ARABIC==ENABLE)

RESULT GxGfx_UITextRect(DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR* pszSrc)
{
    RESULT result;
    if((Get_LanguageIndex()!=ARABIC)&&(!bShowArabic))
    {
        return GxGfx_TextRect(pDestDC, x1,y1,x2, y2, pszSrc);
    }
    else
    {

        GxGfx_SetImageLayout(BR_IMAGELAYOUT, GxGfx_Get(BR_TEXTALIGN));
        gMenu_TextColor[2]= GxGfx_Get(BR_TEXTFORECOLOR1);
        GxGfx_SetImageColor(0, (const MAPPING_ITEM*)gMenu_TextColor);
        result = GxGfx_ImageRect(pDestDC, x1,y1,x2,y2, (const IMAGE *)pszSrc);
        GxGfx_SetImageTable((const IMAGE_TABLE*)&gDemo_Image);
        GxGfx_SetImageColor(_OSD_INDEX_TRANSPART,IMAGEPARAM_DEFAULT);
        bShowArabic = FALSE;
        return result;
    }
}

#else
RESULT GxGfx_UITextRect(DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR* pszSrc)
{
    return GxGfx_TextRect(pDestDC, x1,y1,x2, y2, pszSrc);
}

#endif





#if(SUP_ARABIC==ENABLE)
const TCHAR* GxGfx_UIGetStringID(IVALUE id)
{
    if(id == STRID_LANG_ARABIC)
    {
        bShowArabic = TRUE;
    }

    if((Get_LanguageIndex()!=ARABIC)&&(!bShowArabic))
    {
        return GxGfx_GetStringID(id);
    }
    else
    {
        GxGfx_SetImageTable((const IMAGE_TABLE*)&gDemo_Arabic);
        return (const TCHAR*)GxGfx_GetImageID(id);
    }

}
#else
const TCHAR* GxGfx_UIGetStringID(IVALUE id)
{
    return GxGfx_GetStringID(id);
}
#endif






#if(SUP_ARABIC==ENABLE)
SIZE_2D GxGfx_UIGetLastTextSize()
{
    if(Get_LanguageIndex()!=ARABIC)
    {
        return GxGfx_GetTextLastSize();
    }
    else
    {
        return tmpImageSize;
    }
}
#else
SIZE_2D GxGfx_UIGetLastTextSize()
{
    return GxGfx_GetTextLastSize();
}
#endif
