#include "OSCommon.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_Disp.h"
#include "GlobalVar.h"
#include <string.h>

#define DISP_CHIP_450 0
#define DISP_CHIP_630 1

#define CFG_DISP_CHIP DISP_CHIP_630

#ifndef CFG_DISP_CHIP
#error "Current Chip doesn't support to MsdcNvtCb_Disp.c"
#else
//Internal Data Types
typedef struct _MAP_COLOR_FORMAT{
    IDE_COLOR_FORMAT FmtIde;
    eMSDCEXT_DISP_FORMAT FmtMsdc;
}tMAPMAP_COLOR_FORMAT;

#if (CFG_DISP_CHIP == DISP_CHIP_450)
#define CFG_IDE_GAMMA_LEVEL 32
static tMAPMAP_COLOR_FORMAT MsdcDisp_MapFormat[] = { //Map IDE_COLOR_FORMAT To eMSDCEXT_DISP_FORMAT
    {COLOR_4_BIT,MSDCEXT_DISP_FORMAT_4_BIT},      // Color format 4 bit
    {COLOR_2_BIT,MSDCEXT_DISP_FORMAT_2_BIT},      // Color format 2 bit
    {COLOR_1_BIT,MSDCEXT_DISP_FORMAT_1_BIT},      // Color format 1 bit
    {COLOR_8_BIT,MSDCEXT_DISP_FORMAT_8_BIT},      // Color format 8 bit
    {COLOR_YCBCR444,MSDCEXT_DISP_FORMAT_YCBCR444},   // Color format YCBCR444
    {COLOR_YCBCR422,MSDCEXT_DISP_FORMAT_YCBCR422},   // Color format YCBCR422
    {COLOR_YCBCR420,MSDCEXT_DISP_FORMAT_YCBCR420},   // Color format YCBCR420
    {COLOR_ARGB4565,MSDCEXT_DISP_FORMAT_ARGB4565},   // Color format ARGB4565
    {COLOR_ARGB8565,MSDCEXT_DISP_FORMAT_ARGB8565},   // Color format ARGB8565
};
#elif (CFG_DISP_CHIP == DISP_CHIP_630)
#define CFG_IDE_GAMMA_LEVEL 16
static tMAPMAP_COLOR_FORMAT MsdcDisp_MapFormat[] = { //Map IDE_COLOR_FORMAT To eMSDCEXT_DISP_FORMAT
    {COLOR_4_BIT,MSDCEXT_DISP_FORMAT_4_BIT},      // Color format 4 bit
    {COLOR_2_BIT,MSDCEXT_DISP_FORMAT_2_BIT},      // Color format 2 bit
    {COLOR_1_BIT,MSDCEXT_DISP_FORMAT_1_BIT},      // Color format 1 bit
    {COLOR_8_BIT,MSDCEXT_DISP_FORMAT_8_BIT},      // Color format 8 bit
    {COLOR_YCBCR444,MSDCEXT_DISP_FORMAT_YCBCR444},   // Color format YCBCR444
    {COLOR_YCBCR422,MSDCEXT_DISP_FORMAT_YCBCR422},   // Color format YCBCR422
    {COLOR_YCBCR420,MSDCEXT_DISP_FORMAT_YCBCR420},   // Color format YCBCR420
    {COLOR_YCBCR422_UVPACK,MSDCEXT_DISP_FORMAT_YCBCR422_UVPACK},   // Color format YCBCR422 with UV packing
    {COLOR_YCBCR420_UVPACK,MSDCEXT_DISP_FORMAT_YCBCR420_UVPACK},   // Color format YCBCR420 with UV packing
};
#endif

#define DEFAULT_ICST_COEF_NUM_BITS_INTEGER  4
#define DEFAULT_ICST_COEF_NUM_BITS_FRACTION 8
#define DEFAULT_BRIGHTNESS_MAX             63
#define DEFAULT_BRIGHTNESS_MIN            -64
#define DEFAULT_BRIGHTNESS_DEFAULT          0
#define DEFAULT_CONTRAST_MAX              255
#define DEFAULT_CONTRAST_MIN                0
#define DEFAULT_CONTRAST_DEFAULT           64
#define DEFAULT_SATURATION_MAX            511
#define DEFAULT_SATURATION_MIN              0
#define DEFAULT_SATURATION_DEFAULT         64

//Internal Variable
static const INT32 DefaultICSTPreMatrixYUV[9] =
{
       76,   150,    29,
      -43,   -84,   128,
      128,  -107,   -20
};
static const INT32 DefaultICSTPreMatrixRGB[9] =
{
      256,     0,     0,
        0,   256,     0,
        0,     0,   256
};
static const INT32 DefaultICSTPostMatrix[9] =
{
      256,     0,   358,
      256,   -88,  -182,
      256,   453,     0
};

//Internal Function
static eMSDCEXT_DISP_FORMAT MsdcNvtCb_DispMapFormatIDEToMSDC(IDE_COLOR_FORMAT Fmt);

//Function Implementation
void MsdcNvtCb_DispCfgGet(void* pData)
{
    UINT i;
    IDE_COLOR_FORMAT Fmt;
    VIDEO_BUF_ATTR   AttrBufVdo;
    OSD_BUF_ATTR     AttrBufOsd;
    VIDEO_BUF_ADDR   AddrBufVdo;
    void (*fpGetVdoFmt[2])(IDE_COLOR_FORMAT *uiFmt) = {ide_getV1Fmt,ide_getV2Fmt};
    void (*fpGetOsdFmt[2])(IDE_COLOR_FORMAT *uiFmt) = {ide_getO1Fmt,ide_getO2Fmt};
    tMSDCEXT_DISP_CFG* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_CFG,pData);
    if(pDesc==NULL)
        return;

    for(i=0;i<2;i++)
    {
        tMSDCEXT_DISP_VDO_INFO* pVdo = &pDesc->tInfoVdo[i];
        tMSDCEXT_DISP_OSD_INFO* pOsd = &pDesc->tInfoOsd[i];

        //Get Video Info
        fpGetVdoFmt[i](&Fmt);
        ide_getVideoBufAttr(IDE_VIDEOID_1+i,&AttrBufVdo);
        ide_getVideoBufAddr(IDE_VIDEOID_1+i,&AddrBufVdo);
        pVdo->bEnable   = ide_getVideoEnable(IDE_VIDEOID_1+i);
        pVdo->Format    = MsdcNvtCb_DispMapFormatIDEToMSDC(Fmt);
        pVdo->AddrY[0]  = AddrBufVdo.B0_Y;
        pVdo->AddrY[1]  = AddrBufVdo.B1_Y;
        pVdo->AddrY[2]  = AddrBufVdo.B2_Y;
        pVdo->AddrU[0]  = AddrBufVdo.B0_CB;
        pVdo->AddrU[1]  = AddrBufVdo.B1_CB;
        pVdo->AddrU[2]  = AddrBufVdo.B2_CB;
        pVdo->AddrV[0]  = AddrBufVdo.B0_CR;
        pVdo->AddrV[1]  = AddrBufVdo.B1_CR;
        pVdo->AddrV[2]  = AddrBufVdo.B2_CR;
        pVdo->Width     = AttrBufVdo.Buf_W;
        pVdo->Height    = AttrBufVdo.Buf_H;
        pVdo->PitchY    = AttrBufVdo.Buf_Lineoffset << 2; //Due to Engine Unit is DWROD
        if(pVdo->Format==MSDCEXT_DISP_FORMAT_ARGB4565||pVdo->Format==MSDCEXT_DISP_FORMAT_ARGB8565)
            pVdo->PitchUV   = (pVdo->Width+3)&(~0x3);
        else
            pVdo->PitchUV   = AttrBufVdo.Buf_Lineoffset << 1; //Due to Engine Unit is DWROD

        //Get Osd Info
        fpGetOsdFmt[i](&Fmt);
        ide_getOsdBufAttr(IDE_OSDID_1+i,&AttrBufOsd);
        ide_getOsdBufAddr(IDE_OSDID_1+i,&pOsd->AddrBuf);
        pOsd->bEnable   = ide_getOsdEnable(IDE_OSDID_1+i);
        pOsd->Format    = MsdcNvtCb_DispMapFormatIDEToMSDC(Fmt);
        pOsd->Width     = AttrBufOsd.Buf_W;
        pOsd->Height    = AttrBufOsd.Buf_H;
        pOsd->PitchBuf  = AttrBufOsd.Buf_Lineoffset << 2; //Due to Engine Unit is DWROD
        pOsd->PitchAlpha= (pOsd->Width+3)&(~0x3);
    }

    #if (CFG_DISP_CHIP == DISP_CHIP_450)
    ide_getO1AlphaBufAddr(&pDesc->tInfoOsd[0].AddrAlpha);
    #endif

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispVdoBufGetCurrIdx(void* pData)
{
    IDE_BJMODE uiBJMODE;
    IDE_OP_BUF uiOPTBUF;
    IDE_BUF_NUM uiBUFNUM;
    tMSDCEXT_DISP_VDO_CURR_BUFF_IDX* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_VDO_CURR_BUFF_IDX,pData);
    if(pDesc==NULL)
        return;

    ide_getV1BufOp(&uiBJMODE,&uiOPTBUF,&uiBUFNUM);
    pDesc->iSel[0] = (UINT32)uiOPTBUF;

    ide_getV2BufOp(&uiBJMODE,&uiOPTBUF,&uiBUFNUM);
    pDesc->iSel[1] = (UINT32)uiOPTBUF;

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispEngineEnableSet(void* pData)
{
    void (*fpEnable[4])(BOOL b_en) = {ide_setV1En,ide_setV2En,ide_setO1En,ide_setO2En};
    tMSDCEXT_DISP_ENG_ONOFF* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_ENG_ONOFF,pData);
    if(pDesc==NULL)
        return;

    fpEnable[pDesc->eEngine](pDesc->bEnable);
    ide_set_load();

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispPaletteSet(void* pData)
{
    UINT32 i;
    UINT32 iEntry;

    tMSDCEXT_DISP_OSD_PALETTE* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_OSD_PALETTE,pData);
    if(pDesc==NULL)
        return;

    if(pDesc->iSel>1)
    {
        debug_err(("MsdcNvtCb_DispPaletteSet(): Invalid iSel\r\n"));
        return;
    }

    iEntry = (pDesc->iSel)<<8;

    for(i=0;i<256;i++)
    {
        UINT8* pPalette = (UINT8*)pDesc->Palette[i];
        ide_setPalEntry(iEntry++,pPalette[0],pPalette[1], pPalette[2], pPalette[3]);
    }

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispPaletteGet(void* pData)
{
    UINT32 i;
    UINT32 iEntry,nEntry = 256;
    UINT8  Value[4];

    tMSDCEXT_DISP_OSD_PALETTE* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_OSD_PALETTE,pData);
    if(pDesc==NULL)
        return;

    if(pDesc->iSel>1)
    {
        debug_err(("MsdcNvtCb_DispPaletteGet(): Invalid iSel\r\n"));
        return;
    }

    if(sizeof(IDE_OV_BLEND_OP)!=sizeof(Value[3]))
    {
        debug_err(("MsdcNvtCb_DispPaletteGet(): sizeof(IDE_OV_BLEND_OP)!=sizeof(Value[3])\r\n"));
        return;
    }

    iEntry = (pDesc->iSel)<<8;

    if(nEntry>pDesc->nEntrys)
        nEntry = pDesc->nEntrys;

    for(i=0;i<nEntry;i++)
    {
        #if (CFG_DISP_CHIP == DISP_CHIP_450)
        ide_getPalEntry(iEntry++,&Value[0],&Value[1],&Value[2],&Value[3]);
        #elif (CFG_DISP_CHIP == DISP_CHIP_630)
        ide_getPalEntry(iEntry++,&Value[0],&Value[1],&Value[2],(IDE_OV_BLEND_OP*)&Value[3]);
        #else
        #error "Unknown CFG_DISP_CHIP"
        #endif
        pDesc->Palette[i] = *(UINT32*)Value;
    }

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispGammaEnableSet(void* pData)
{
     tMSDCEXT_BOOL* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_BOOL,pData);
     if(pDesc==NULL)
        return;

     ide_setGammaEn(pDesc->bValue);

     pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispGammaTblGetSize(void* pData)
{
    tMSDCEXT_UINT32* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_UINT32,pData);
    if(pDesc==NULL)
        return;

     pDesc->uiValue = CFG_IDE_GAMMA_LEVEL+1; //for n Levels, TblSize = n+1

     pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispGammaTblSetTbl(void* pData)
{
    tMSDCEXT_DISP_GAMMA_TBL* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_GAMMA_TBL,pData);
    if(pDesc==NULL)
        return;

    ide_fillGamma(pDesc->GammaTbl);

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispGammaTblGetTbl(void* pData)
{
    tMSDCEXT_DISP_GAMMA_TBL* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_GAMMA_TBL,pData);
    if(pDesc==NULL)
        return;

    //ide_getGamma(pDesc->GammaTbl);

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispIcstGetInfo(void* pData)
{
    tMSDCEXT_DISP_ICST_COEF_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_ICST_COEF_INFO,pData);
    if(pDesc==NULL)
        return;

    pDesc->NumBitsInteger  = DEFAULT_ICST_COEF_NUM_BITS_INTEGER;
    pDesc->NumBitsFraction = DEFAULT_ICST_COEF_NUM_BITS_FRACTION;

    switch(g_LCDCtrlObj.uiLCDMode)
    {
        case LCD_MODE_RGB:
        case LCD_MODE_RGBD:
        case LCD_MODE_RGBD_360:
        case LCD_MODE_RGB_THROUGH:
            memcpy(pDesc->PreMatrix,DefaultICSTPreMatrixRGB,sizeof(DefaultICSTPreMatrixRGB));
            break;

        case LCD_MODE_YUV:
        case LCD_MODE_YUV_360:
            memcpy(pDesc->PreMatrix,DefaultICSTPreMatrixYUV,sizeof(DefaultICSTPreMatrixYUV));
            break;

        default:
            debug_err(("MsdcNvtCb_DispIcstGetInfo: Not Support to g_LCDCtrlObj.uiLCDMode = %d\r\n",g_LCDCtrlObj.uiLCDMode));
            return;

    }
    memcpy(pDesc->PostMatrix,DefaultICSTPostMatrix,sizeof(DefaultICSTPostMatrix));

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispIcstSet(void* pData)
{
    tMSDCEXT_DISP_ICST_COEF* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_ICST_COEF,pData);
    if(pDesc==NULL)
        return;

    ide_setICST(TRUE,CST_RGB2YCBCR);
    ide_setOutOffset(0,0,0);
    ide_setIcstCoef(ACCRCY_1_2_8,pDesc->CoefTrans);

    switch(g_LCDCtrlObj.uiLCDMode)
    {
        case LCD_MODE_RGB:
        case LCD_MODE_RGBD:
        case LCD_MODE_RGBD_360:
        case LCD_MODE_RGB_THROUGH:
            ide_setIcstPreOffset(0x00,0x80,0x80);
            break;

        case LCD_MODE_YUV:
        case LCD_MODE_YUV_360:
            ide_setIcstPreOffset(0x00,0x00,0x00);
            break;

        default:
            debug_err(("MsdcNvtCb_DispIcstSet: Not Support to g_LCDCtrlObj.uiLCDMode = %d\r\n",g_LCDCtrlObj.uiLCDMode));
            return;
    }

    ide_setLoad();

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispIcstGet(void* pData)
{
    tMSDCEXT_DISP_ICST_COEF* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_ICST_COEF,pData);
    if(pDesc==NULL)
        return;

    /*
    if(ide_getICST())
    {
        ide_getIcstCoef((INT32*)pDesc->CoefTrans);
    }
    else
    */
    {
        switch(g_LCDCtrlObj.uiLCDMode)
        {
            case LCD_MODE_RGB:
            case LCD_MODE_RGBD:
            case LCD_MODE_RGBD_360:
            case LCD_MODE_RGB_THROUGH:
                //Also Equal to DefaultICSTPostMatrix
                memcpy(pDesc->CoefTrans,DefaultICSTPostMatrix,sizeof(DefaultICSTPostMatrix));
                break;

            case LCD_MODE_YUV:
            case LCD_MODE_YUV_360:
                //Also Equal to DefaultICSTPreMatrixRGB
                memcpy(pDesc->CoefTrans,DefaultICSTPreMatrixRGB,sizeof(DefaultICSTPreMatrixRGB));
                break;

            default:
                debug_err(("MsdcNvtCb_DispIcstGet: Not Support to g_LCDCtrlObj.uiLCDMode = %d\r\n",g_LCDCtrlObj.uiLCDMode));
                return;
        }
    }

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispSaturationGetInfo(void* pData)
{
    tMSDCEXT_DISP_SATURATION_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_SATURATION_INFO,pData);
    if(pDesc==NULL)
        return;

    pDesc->ValMax = DEFAULT_SATURATION_MAX;
    pDesc->ValMin = DEFAULT_SATURATION_MIN;
    pDesc->ValDefault = DEFAULT_SATURATION_DEFAULT;


    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispSaturationSet(void* pData)
{
    tMSDCEXT_INT32* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_INT32,pData);
    if(pDesc==NULL)
        return;

    ide_setCmults((UINT32)pDesc->iValue);

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispContrastGetInfo(void* pData)
{
    tMSDCEXT_DISP_CONTRAST_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_CONTRAST_INFO,pData);
    if(pDesc==NULL)
        return;

    pDesc->ValMax = DEFAULT_CONTRAST_MAX;
    pDesc->ValMin = DEFAULT_CONTRAST_MIN;
    pDesc->ValDefault = DEFAULT_CONTRAST_DEFAULT;

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispContrastSet(void* pData)
{
    tMSDCEXT_INT32* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_INT32,pData);
    if(pDesc==NULL)
        return;

    ide_setCtrst((UINT32)pDesc->iValue);

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispBrightnessGetInfo(void* pData)
{
    tMSDCEXT_DISP_BRIGHTNESS_INFO* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISP_BRIGHTNESS_INFO,pData);
    if(pDesc==NULL)
        return;

    pDesc->ValMax = DEFAULT_BRIGHTNESS_MAX;
    pDesc->ValMin = DEFAULT_BRIGHTNESS_MIN;
    pDesc->ValDefault = DEFAULT_BRIGHTNESS_DEFAULT;

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_DispBrightnessSet(void* pData)
{
    tMSDCEXT_INT32* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_INT32,pData);
    if(pDesc==NULL)
        return;

    ide_setBrt((UINT32)pDesc->iValue);

    pDesc->tParent.bOK = TRUE;
}

static eMSDCEXT_DISP_FORMAT MsdcNvtCb_DispMapFormatIDEToMSDC(IDE_COLOR_FORMAT Fmt)
{
    UINT32 i,nItem = sizeof(MsdcDisp_MapFormat)/sizeof(MsdcDisp_MapFormat[0]);
    for(i=0;i<nItem;i++)
    {
        if(MsdcDisp_MapFormat[i].FmtIde==Fmt)
            return MsdcDisp_MapFormat[i].FmtMsdc;
    }

    debug_err(("MsdcNvtCb_DispMapFormatIDEToMSDC: Unknown Format\r\n"));
    return MSDCEXT_DISP_FORMAT_UNKNOWN;
}

#endif