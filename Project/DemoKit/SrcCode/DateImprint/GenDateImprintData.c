/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GenDateImprintData.c
    @ingroup    Unknown

    @brief      Sample Code for Date imprint data generation.
                Sample Code for Date imprint data generation.

    @note       Nothing.

    @version    V1.00.000
    @author     Shirley Cheng
    @date       2006/02/15

*/


/** \addtogroup Unknown*/
//@{

#include <string.h>
#include "Type.h"
#include "kernel.h"
#include "OSCommon.h"
#include "SysCfg.h"
#include "GenDateImprintData.h"
#include "DataStamp.h"
#include "ImageAlgInfor.h"
#include "filesystsk.h"
#include "DateStampFont10x16.h"
#include "DateStampFont12x20.h"
#include "DateStampFont18x30.h"
#include "DateStampFont26x44.h"
#include "DateStampFont30x52.h"
#include "DateStampFont36x60.h"
#include "DateStampFont42x72.h"
#include "DateStampFont50x86.h"
#include "DateStampFont56x96.h"

//function declare
static void DateImprint_Open(void);
static void DateImprint_Close(void);
static void DateImprint_SetStrInfo(PDATEIMPRINT_INFO pDateImprintData, PDATEIMPRINT_INFO pDateImprintDataThumb, UINT32 flag);
static void DateImprint_Gen(PDATEIMPRINT_INFO DateImprintData);
static void DateImprint_GenThumb(PDATEIMPRINT_INFO DateImprintDataThumb);
static BOOL DateImprint_GenStr_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiMemAddr, UINT32 uiLineOffset);
static BOOL DateImprint_GenIcon_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset);
static void DateImprint_Gen_Scalar_FW(PDATEIMPRINT_INFO DateImprintData);
static BOOL DateImprint_GenStr_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiDstLineOffset, UINT32 uiDstHeight);
static BOOL DateImprint_GenIcon_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset, UINT32 uiIconWidth, UINT32 uiIconHeight);

//variable declare
static UINT32       uiMemoryAddr;
static UINT32       uiMemoryAddrThumb;
static STAMP_POS    g_StampPos = {0, 0};        // stamp position for primary image
static STAMP_POS    g_StampPosThumb = {0, 0};   // stamp position for thumbnail
static COLOR_YCBCR  g_StampColorBg = {0, 0, 0}; // date stamp background color
static COLOR_YCBCR  g_StampColorFr = {0, 0, 0}; // date stamp frame color
static COLOR_YCBCR  g_StampColorFg = {RGB_GET_Y(250, 160, 10), RGB_GET_U(250, 160, 10), RGB_GET_V(250, 160, 10)}; // date stamp foreground color

DATEIMPRINT_INFO    DateInfo, DateInfoThumb;
char                DateStampStr[32], DateStampThumbStr[32];

void DateImprint_Setup(UINT32 flag)
{
    if ((flag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        DateImprint_Close();
    }
    else
    {
        DateImprint_Open();
        DateInfo.pi8Str = DateStampStr;
        DateInfoThumb.pi8Str = DateStampThumbStr;
        DateImprint_SetStrInfo(&DateInfo, &DateInfoThumb, flag);
        DateImprint_Gen_Scalar_FW(&DateInfo);
        DateImprint_GenThumb(&DateInfoThumb);
    }
}

void DateImprint_SetPos(UINT32 x, UINT32 y)
{
    g_StampPos.uiX = x;
    g_StampPos.uiY = y;
}

void DateImprint_SetPosThumb(UINT32 x, UINT32 y)
{
    g_StampPosThumb.uiX = x;
    g_StampPosThumb.uiY = y;
}

void DateImprint_SetColor(PCOLOR_YCBCR pStampColorBg, PCOLOR_YCBCR pStampColorFr, PCOLOR_YCBCR pStampColorFg)
{
    // Stamp background color
    g_StampColorBg.uiY  = pStampColorBg->uiY;
    g_StampColorBg.uiCB = pStampColorBg->uiCB;
    g_StampColorBg.uiCR = pStampColorBg->uiCR;

    // Stamp frame color
    g_StampColorFr.uiY  = pStampColorFr->uiY;
    g_StampColorFr.uiCB = pStampColorFr->uiCB;
    g_StampColorFr.uiCR = pStampColorFr->uiCR;

    // Stamp foreground color (text body)
    g_StampColorFg.uiY  = pStampColorFg->uiY;
    g_StampColorFg.uiCB = pStampColorFg->uiCB;
    g_StampColorFg.uiCR = pStampColorFg->uiCR;
}

/**
    Set DateInfo values.

    Set DateInfo values.

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @return None.
*/
#define THUMB_WIDTH     640//320
#define THUMB_HEIGHT    480//240
#define VIRTUAL_WIDTH   1024
#define VIRTUAL_HEIGHT  1024

static void DateImprint_SetStrInfo(PDATEIMPRINT_INFO pDateImprintData, PDATEIMPRINT_INFO pDateImprintDataThumb, UINT32 flag)
{
	RTC_DATE        CurDate;
	RTC_TIME        CurTime;
    _IMGSize_sel    size;
    UINT32          uiImageWidth, uiImageHeight;
    UINT32          uiStampWidth;
    STAMP_POS       StampPos, StampPosThumb;
    COLOR_YCBCR     StampColorBg, StampColorFr, StampColorFg;

    // get RCT's Date/Time
    CurDate = rtc_getDate();
    CurTime = rtc_getTime();

    // Get stamp font database according to image size
    size = Get_SizeValue(Get_SizeIndex());
    uiImageWidth  = JPGIMG_SIZE[size][0];
    uiImageHeight = JPGIMG_SIZE[size][1];

    switch (size)
    {
    case _IMGSize_14M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 148;
        break;
    case _IMGSize_12M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 136;
        break;

    case _IMGSize_10M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 124;
        break;

    case _IMGSize_9M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 116;
        break;

    case _IMGSize_8M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 110;
        break;

    case _IMGSize_7M:
    //case _IMGSize_7M_169:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight = 102;
        break;

    case _IMGSize_6M:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight  = 96;
        break;

    case _IMGSize_5_4M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 90;
        break;

    case _IMGSize_5M:
    case _IMGSize_4_9M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 86;
        break;

    case _IMGSize_4_4M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 82;
        break;

    case _IMGSize_4M:
    case _IMGSize_3_8M:
        pDateImprintData->pDataBase = &gDateStampFont50x86;
        pDateImprintData->ui32FontWidth  = 50;
        pDateImprintData->ui32FontHeight = 86;
        pDateImprintData->ui32DstHeight  = 76;
        break;

    case _IMGSize_3M:
        pDateImprintData->pDataBase = &gDateStampFont42x72;
        pDateImprintData->ui32FontWidth  = 42;
        pDateImprintData->ui32FontHeight = 72;
        pDateImprintData->ui32DstHeight  = 72;
        break;

    case _IMGSize_2_6M:
        pDateImprintData->pDataBase = &gDateStampFont42x72;
        pDateImprintData->ui32FontWidth  = 42;
        pDateImprintData->ui32FontHeight = 72;
        pDateImprintData->ui32DstHeight  = 66;
        break;

    case _IMGSize_2_1M:
    case _IMGSize_2M:
    //case _IMGSize_2M_169:
        pDateImprintData->pDataBase = &gDateStampFont36x60;
        pDateImprintData->ui32FontWidth  = 36;
        pDateImprintData->ui32FontHeight = 60;
        pDateImprintData->ui32DstHeight  = 60;
        break;

    case _IMGSize_1_5M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 48;
        break;

    case _IMGSize_1M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 44;
        break;

    case _IMGSize_0_8M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 38;
        break;

    case _IMGSize_0_6M:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 32;
        break;

    case _IMGSize_0_4M:
    case _IMGSize_VGA:
    case _IMGSize_0_2M:
        pDateImprintData->pDataBase = &gDateStampFont18x30;
        pDateImprintData->ui32FontWidth  = 18;
        pDateImprintData->ui32FontHeight = 30;
        pDateImprintData->ui32DstHeight  = 30;
        break;

    default:
        pDateImprintData->pDataBase = &gDateStampFont56x96;
        pDateImprintData->ui32FontWidth  = 56;
        pDateImprintData->ui32FontHeight = 96;
        pDateImprintData->ui32DstHeight  = 96;
        break;
    }

    // Stamp font database and size for screen-nail (320x240)
    //pDateImprintDataThumb->pDataBase      = &gDateStampFont10x16;
    //pDateImprintDataThumb->ui32FontWidth  = 10;
    //pDateImprintDataThumb->ui32FontHeight = pDateImprintDataThumb->ui32DstHeight = 16;
    // Stamp font database and size for screen-nail (640x480)
    pDateImprintDataThumb->pDataBase      = &gDateStampFont18x30;
    pDateImprintDataThumb->ui32FontWidth  = 18;
    pDateImprintDataThumb->ui32FontHeight = pDateImprintDataThumb->ui32DstHeight = 30;

    uiStampWidth = (pDateImprintData->ui32DstHeight * pDateImprintData->ui32FontWidth) / pDateImprintData->ui32FontHeight;

    // Prepare stamp date-time strings
    if ((flag & STAMP_DATE_TIME_MASK) == STAMP_DATE)
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.day, CurDate.s.month, CurDate.s.year);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d", CurDate.s.day, CurDate.s.month, CurDate.s.year);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d", CurDate.s.month, CurDate.s.day, CurDate.s.year);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d", CurDate.s.month, CurDate.s.day, CurDate.s.year);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day);
            sprintf(pDateImprintDataThumb->pi8Str, "%04d/%02d/%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day);
            break;
        }
    }
    else
    {
        switch (flag & STAMP_DATE_FORMAT_MASK)
        {
        case STAMP_DD_MM_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            break;

        case STAMP_MM_DD_YY:
            sprintf(pDateImprintData->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            sprintf(pDateImprintDataThumb->pi8Str, "%02d/%02d/%04d %02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute);
            break;

        default:
            sprintf(pDateImprintData->pi8Str, "%04d/%02d/%02d %02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute);
            sprintf(pDateImprintDataThumb->pi8Str, "%04d/%02d/%02d %02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute);
            break;
        }
    }

    // Set date stamp position
    if ((flag & STAMP_OPERATION_MASK) == STAMP_MANUAL)
    {
        StampPosThumb.uiX = g_StampPosThumb.uiX;
        StampPosThumb.uiY = g_StampPosThumb.uiY;
        StampPos.uiX = g_StampPos.uiX;
        StampPos.uiY = g_StampPos.uiY;
    }
    else
    {
        // Primary image is related to virtual coordinate 1024x1024
        // Thumbnail (Screen-nail) is related to real size 320x240
        // [Modification] Thumbnail (Screen-nail) is also related virtual coordinate 1024x1024
        switch (flag & STAMP_POSITION_MASK)
        {
        case STAMP_BOTTOM_LEFT:
            StampPos.uiX = (uiStampWidth * 2 * VIRTUAL_WIDTH) / uiImageWidth;
            StampPosThumb.uiX = (pDateImprintDataThumb->ui32FontWidth * 2 * VIRTUAL_WIDTH) / THUMB_WIDTH;
            break;

        //case STAMP_BOTTOM_RIGHT:
        default:
            StampPos.uiX = (uiImageWidth - uiStampWidth * (strlen(pDateImprintData->pi8Str) + 2)) * VIRTUAL_WIDTH / uiImageWidth;
            StampPosThumb.uiX = (THUMB_WIDTH - pDateImprintDataThumb->ui32FontWidth * (strlen(pDateImprintDataThumb->pi8Str) + 2)) * VIRTUAL_WIDTH / THUMB_WIDTH;
            break;
        }

        StampPos.uiY = (uiImageHeight - pDateImprintData->ui32DstHeight * 2) * VIRTUAL_HEIGHT / uiImageHeight;
        StampPosThumb.uiY = (THUMB_HEIGHT - pDateImprintDataThumb->ui32FontHeight * 2) * VIRTUAL_HEIGHT / THUMB_HEIGHT;
    }

    datastamp_SetPosition(StampPos.uiX, StampPos.uiY);
    datastamp_SetPositionThumb(StampPosThumb.uiX, StampPosThumb.uiY);

    // Set date stamp color
    if ((flag & STAMP_OPERATION_MASK) == STAMP_MANUAL)
    {
        // Stamp background color
        StampColorBg.uiY  = g_StampColorBg.uiY;
        StampColorBg.uiCB = g_StampColorBg.uiCB;
        StampColorBg.uiCR = g_StampColorBg.uiCR;
        // Stamp frame color
        StampColorFr.uiY  = g_StampColorFr.uiY;
        StampColorFr.uiCB = g_StampColorFr.uiCB;
        StampColorFr.uiCR = g_StampColorFr.uiCR;
        // Stamp foreground color (text body)
        StampColorFg.uiY  = g_StampColorFg.uiY;
        StampColorFg.uiCB = g_StampColorFg.uiCB;
        StampColorFg.uiCR = g_StampColorFg.uiCR;
    }
    else
    {
        // Stamp background color: color index 0, transparent
        StampColorBg.uiY  = 0x00;
        StampColorBg.uiCB = 0x00;
        StampColorBg.uiCR = 0x00;
        // Stamp frame color: color index 0, transparent
        StampColorFr.uiY  = 0x00;
        StampColorFr.uiCB = 0x00;
        StampColorFr.uiCR = 0x00;
        // Stamp foreground color (text body)
        StampColorFg.uiY  = RGB_GET_Y(250, 160, 10);
        StampColorFg.uiCB = RGB_GET_U(250, 160, 10);
        StampColorFg.uiCR = RGB_GET_V(250, 160, 10);
    }

    pDateImprintData->colormap[0].uiY   = pDateImprintDataThumb->colormap[0].uiY   = StampColorBg.uiY;
    pDateImprintData->colormap[0].uiCB  = pDateImprintDataThumb->colormap[0].uiCB  = StampColorBg.uiCB;
    pDateImprintData->colormap[0].uiCR  = pDateImprintDataThumb->colormap[0].uiCR  = StampColorBg.uiCR;
    pDateImprintData->colormap[1].uiY   = pDateImprintDataThumb->colormap[1].uiY   = StampColorFr.uiY;
    pDateImprintData->colormap[1].uiCB  = pDateImprintDataThumb->colormap[1].uiCB  = StampColorFr.uiCB;
    pDateImprintData->colormap[1].uiCR  = pDateImprintDataThumb->colormap[1].uiCR  = StampColorFr.uiCR;
    pDateImprintData->colormap[2].uiY   = pDateImprintDataThumb->colormap[2].uiY   = StampColorFg.uiY;
    pDateImprintData->colormap[2].uiCB  = pDateImprintDataThumb->colormap[2].uiCB  = StampColorFg.uiCB;
    pDateImprintData->colormap[2].uiCR  = pDateImprintDataThumb->colormap[2].uiCR  = StampColorFg.uiCR;
}

/**
    Open Date Imprint.

    Open Date Imprint.

    @param None.
    @return None.
*/
static void DateImprint_Open(void)
{
    UINT32  Error_check;
    UINT32  uiFBAddrY;

    Error_check = get_blk((void*)&(uiFBAddrY), POOL_ID_SICD);
    rel_blk(POOL_ID_SICD, (VP) uiFBAddrY);

    if (Error_check == E_OK)
    {
	    uiMemoryAddr = uiFBAddrY;
	    uiMemoryAddrThumb = uiFBAddrY + POOL_SIZE_SICD / 2;
        datastamp_Enable(ENABLE);
    }
}

/**
    Close Date Imprint.

    Close Date Imprint.

    @param None.
    @return None.
*/
static void DateImprint_Close(void)
{
    datastamp_Enable(DISABLE);
}

/**
    DateImprint_Gen

    Date Imprint Date Generation.

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
*/
static void DateImprint_Gen(PDATEIMPRINT_INFO DateImprintData)
{
    UINT32  i, j, k;
    UINT32  ui32Y_Addr, ui32CB_Addr, ui32CR_Addr;
    UINT32  uiLineOffset;
    UINT32  ui32StrLen, uiDateHeight;
    UINT32  ui32CB_Addr2, ui32CR_Addr2;
    DS_STAMP_INFOR StampInfo;

    ui32StrLen = strlen(DateImprintData->pi8Str);
    uiDateHeight = DateImprintData->ui32FontHeight;
    uiLineOffset = DateImprintData->ui32FontWidth * ui32StrLen;

    /*
    if (uiLineOffset % 16 != 0)
    {
        uiLineOffset = ((uiLineOffset >> 4) + 1) << 4;
    }
    */

    if (uiLineOffset&0x7)
    {
        uiLineOffset = (uiLineOffset&0xfffffff8)+0x8;
    }

    DateImprint_GenStr_ToMem(DateImprintData, &ui32Y_Addr, &ui32CB_Addr, &ui32CR_Addr, uiMemoryAddr, uiLineOffset);

    {
        ui32CB_Addr2 = ui32CB_Addr;
        ui32CR_Addr2 = ui32CR_Addr;
        j = ui32CB_Addr;
        k = ui32CR_Addr;
        for (i = 0; i < uiLineOffset * uiDateHeight; i += 2)
        {
            memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
            memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
            j ++;
            k ++;
            ui32CB_Addr2 +=2;
            ui32CR_Addr2 +=2;
        }
    }

    StampInfo.AddrY      = ui32Y_Addr;
    StampInfo.AddrCb     = ui32CB_Addr;
    StampInfo.AddrCr     = ui32CR_Addr;
    StampInfo.LineOffset = uiLineOffset;
    StampInfo.Width      = uiLineOffset;
    StampInfo.Height     = DateImprintData->ui32FontHeight;
    StampInfo.ColorKeyY  = 0;
    StampInfo.ColorKeyCb = 0;
    StampInfo.ColorKeyCr = 0;
    StampInfo.Format     = DS_YCCFORMAT_422;
    datastamp_SetStampInfor(&StampInfo);
}

static void DateImprint_GenThumb(PDATEIMPRINT_INFO DateImprintDataThumb)
{
    UINT32  i, j, k;
    UINT32  ui32Y_Addr, ui32CB_Addr, ui32CR_Addr;
    UINT32  uiLineOffset;
    UINT32  ui32StrLen, uiDateHeight;
    UINT32  ui32CB_Addr2, ui32CR_Addr2;
    DS_STAMP_INFOR StampInfoThumb;

    ui32StrLen = strlen(DateImprintDataThumb->pi8Str);
    uiDateHeight = DateImprintDataThumb->ui32FontHeight;
    uiLineOffset = DateImprintDataThumb->ui32FontWidth * ui32StrLen;

    /*
    if (uiLineOffset % 16 != 0)
    {
        uiLineOffset = ((uiLineOffset >> 4) + 1) << 4;
    }
    */

    if (uiLineOffset&0x7)
    {
        uiLineOffset = (uiLineOffset&0xfffffff8)+0x8;
    }

    DateImprint_GenStr_ToMem(DateImprintDataThumb, &ui32Y_Addr, &ui32CB_Addr, &ui32CR_Addr, uiMemoryAddrThumb, uiLineOffset);

    // 444->422
    ui32CB_Addr2 = ui32CB_Addr;
    ui32CR_Addr2 = ui32CR_Addr;
    j = ui32CB_Addr;
    k = ui32CR_Addr;

    for (i = 0; i < uiLineOffset * uiDateHeight; i += 2)
    {
        memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
        memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
        j ++;
        k ++;
        ui32CB_Addr2 +=2;
        ui32CR_Addr2 +=2;
    }

    StampInfoThumb.AddrY      = ui32Y_Addr;
    StampInfoThumb.AddrCb     = ui32CB_Addr;
    StampInfoThumb.AddrCr     = ui32CR_Addr;
    StampInfoThumb.LineOffset = uiLineOffset;
    StampInfoThumb.Width      = uiLineOffset;
    StampInfoThumb.Height     = DateImprintDataThumb->ui32FontHeight;
    StampInfoThumb.ColorKeyY  = 0;
    StampInfoThumb.ColorKeyCb = 0;
    StampInfoThumb.ColorKeyCr = 0;
    StampInfoThumb.Format     = DS_YCCFORMAT_422;
    datastamp_SetStampInforThumb(&StampInfoThumb);
}

/**
    DateImprint_GenStr_ToMem

    @param[in]  PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[out] UINT *puiY_Addr the Y starting address of this data generation.
    @param[out] UINT *puiCb_Addr the CB starting address of this data generation.
    @param[out] UINT *puiCr_Addr the CR starting address of this data generation.
    @param[in]  UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenStr_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiMemAddr, UINT32 uiLineOffset)
{
    UINT8   uiStrLen;
    UINT32  i, j;
    ICON_DB const *pDB;
    UINT16  uiIconID;
    UINT16  uiStroffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;

    uiStrLen = strlen(DateImprintData->pi8Str);
    //pointer to font data base
    pDB = DateImprintData->pDataBase;
    uiStroffset = pDB->uiDrawStrOffset;

    uiFBAddrY = uiMemAddr;
    uiFBAddrCb = uiFBAddrY + uiLineOffset * DateImprintData->ui32FontHeight;
    uiFBAddrCr = uiFBAddrCb + uiLineOffset * DateImprintData->ui32FontHeight;

    *puiY_Addr = uiFBAddrY;
    *puiCb_Addr = uiFBAddrCb;
    *puiCr_Addr = uiFBAddrCr;

    memset((UINT8 *)uiFBAddrY, 0x00, uiLineOffset * DateImprintData->ui32FontHeight);
    memset((UINT8 *)uiFBAddrCb, 0x00, uiLineOffset * DateImprintData->ui32FontHeight);
    memset((UINT8 *)uiFBAddrCr, 0x00, uiLineOffset * DateImprintData->ui32FontHeight);

    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = DateImprintData->pi8Str[i] - uiStroffset ;

        j = pDB ->pIconHeader[uiIconID].uiWidth;

        if (j % 2 != 0)
        {
            j++;
        }

        DateImprint_GenIcon_ToMem(DateImprintData, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, uiLineOffset);
        uiFBAddrY  += (j);
        uiFBAddrCb += (j);
        uiFBAddrCr += (j);
    }

    return TRUE;
}

/**
    DateImprint_GenIcon_ToMem

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[in] UINT16 uiIconID Icon ID.
    @param[in] UINT uiFBY the CB starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenIcon_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    UINT16  uiIconWidth;
    UINT16  uiIconHeight;
    UINT8   const *uiIconData;
    UINT16  uipixelcount;
    UINT32  i, j;
    UINT32  uiFBY_tmp, uiFBCb_tmp, uiFBCr_tmp;
    UINT8   uiIndex;
    ICON_DB const *pDB;

    pDB = DateImprintData->pDataBase;
    pIconHeader = pDB ->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB ->pIconData) + (pIconHeader.uiOffset);

    uipixelcount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        uiFBY_tmp = uiFBY;
        uiFBCb_tmp = uiFBCb;
        uiFBCr_tmp = uiFBCr;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;

            memset((UINT8 *)uiFBY_tmp,  DateImprintData->colormap[uiIndex].uiY, 1);
            memset((UINT8 *)uiFBCb_tmp, DateImprintData->colormap[uiIndex].uiCB, 1);
            memset((UINT8 *)uiFBCr_tmp, DateImprintData->colormap[uiIndex].uiCR, 1);

            uiFBY_tmp += 1;
            uiFBCb_tmp += 1;
            uiFBCr_tmp += 1;

            uipixelcount ++;
            if (uipixelcount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        uiFBCb += uiLineOffset;
        uiFBCr += uiLineOffset;
    }
    return TRUE;
}

/**
    DateImprint_Gen_Scalar_FW

    Date Imprint Date Generation with scaling function executed by FW

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[in] UINT32 uiDstHeight the wanted height of final icon image
*/
static void DateImprint_Gen_Scalar_FW(PDATEIMPRINT_INFO DateImprintData)
{
	UINT32 i, j, k;
    UINT32 ui32Y_Addr, ui32CB_Addr, ui32CR_Addr;
    UINT32 ui32CB_Addr2, ui32CR_Addr2;
    UINT32 uiDstLineOffset;
    UINT32 ui32StrLen;
    DS_STAMP_INFOR StampInfo;

    ui32StrLen = strlen(DateImprintData->pi8Str);

    uiDstLineOffset = ((DateImprintData->ui32FontWidth * ui32StrLen) * DateImprintData->ui32DstHeight) / DateImprintData->ui32FontHeight;

    if (uiDstLineOffset&0x7)
    {
        uiDstLineOffset = (uiDstLineOffset&0xfffffff8)+0x8;
    }

    DateImprint_GenStr_ToMem_Scalar_FW(DateImprintData, &ui32Y_Addr, &ui32CB_Addr, &ui32CR_Addr, uiDstLineOffset, DateImprintData->ui32DstHeight);

    {//444->422
        ui32CB_Addr2 = ui32CB_Addr;
        ui32CR_Addr2 = ui32CR_Addr;
        j = ui32CB_Addr;
        k = ui32CR_Addr;
        for (i = 0; i < uiDstLineOffset * DateImprintData->ui32DstHeight; i += 2)
        {
            memcpy((UINT8 *)j, (UINT8 *)ui32CB_Addr2, 1);
            memcpy((UINT8 *)k, (UINT8 *)ui32CR_Addr2, 1);
            j ++;
            k ++;
            ui32CB_Addr2 +=2;
            ui32CR_Addr2 +=2;
        }
    }

    StampInfo.AddrY      = ui32Y_Addr;
    StampInfo.AddrCb     = ui32CB_Addr;
    StampInfo.AddrCr     = ui32CR_Addr;
    StampInfo.LineOffset = uiDstLineOffset;
    StampInfo.Width      = uiDstLineOffset;
    StampInfo.Height     = DateImprintData->ui32DstHeight;
    StampInfo.ColorKeyY  = 0;
    StampInfo.ColorKeyCb = 0;
    StampInfo.ColorKeyCr = 0;
    StampInfo.Format     = DS_YCCFORMAT_422;
    datastamp_SetStampInfor(&StampInfo);
}

/**
    DateImprint_GenStr_ToMem_Scalar_FW

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[out] UINT *puiY_Addr the Y starting address of this data generation.
    @param[out] UINT *puiCb_Addr the CB starting address of this data generation.
    @param[out] UINT *puiCr_Addr the CR starting address of this data generation.
    @param[in] UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenStr_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT32 *puiY_Addr, UINT32 *puiCb_Addr, UINT32 *puiCr_Addr, UINT32 uiDstLineOffset, UINT32 uiDstHeight)
{
    UINT8   uiStrLen;
    UINT32  i;
    ICON_DB const *pDB;
    UINT16  uiIconID;
    UINT16  uiStroffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
    UINT16  uiIconWidth;

    uiStrLen = strlen(DateImprintData->pi8Str);

    //pointer to font data base
    pDB = DateImprintData->pDataBase;
    uiStroffset = pDB->uiDrawStrOffset;

    uiFBAddrY = uiMemoryAddr;
    uiFBAddrCb = uiFBAddrY + uiDstLineOffset * uiDstHeight;
    uiFBAddrCr = uiFBAddrCb + uiDstLineOffset * uiDstHeight;

    *puiY_Addr = uiFBAddrY;
    *puiCb_Addr = uiFBAddrCb;
    *puiCr_Addr = uiFBAddrCr;

    memset((UINT8 *)uiFBAddrY, 0x00, uiDstLineOffset * uiDstHeight);
    memset((UINT8 *)uiFBAddrCb, 0x00, uiDstLineOffset * uiDstHeight);
    memset((UINT8 *)uiFBAddrCr, 0x00, uiDstLineOffset * uiDstHeight);

    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = DateImprintData->pi8Str[i] - uiStroffset;
        //#NT#2009/09/09#Randy -begin
        //avoid divid zero case
        if(pDB ->pIconHeader[uiIconID].uiHeight != 0)
        {
        uiIconWidth = (pDB ->pIconHeader[uiIconID].uiWidth*uiDstHeight)
                        /pDB ->pIconHeader[uiIconID].uiHeight;
        }
        //#NT#2009/09/09#Randy -end

        DateImprint_GenIcon_ToMem_Scalar_FW(DateImprintData, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, uiDstLineOffset, uiIconWidth, uiDstHeight);
        uiFBAddrY  += (uiIconWidth);
        uiFBAddrCb += (uiIconWidth);
        uiFBAddrCr += (uiIconWidth);
    }

    return TRUE;
}

/**
    DateImprint_GenIcon_ToMem_Scalar_FW

    @param[in] PDATEIMPRINT_INFO DateImprintData The struct point of data imprint.
    @param[in] UINT16 uiIconID Icon ID.
    @param[in] UINT uiFBY the CB starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiFBCb the CR starting address of this icon.
    @param[in] UINT uiLineOffset the line-offset of this data generation.
*/
static BOOL DateImprint_GenIcon_ToMem_Scalar_FW(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset, UINT32 uiIconWidth, UINT32 uiIconHeight)
{
    ICON_HEADER pIconHeader;
    UINT16  uiIconOriWidth;
    UINT16  uiIconOriHeight;
    UINT8   const *uiIconData;
    UINT16  uipixelcount;
    UINT32  i, j, k;
    UINT32  uiFBY_tmp,uiFBCb_tmp,uiFBCr_tmp;
    UINT8   uiIndex;
    ICON_DB const *pDB;
    UINT32  uiLastX, uiLastY, uiCurrX, uiCurrY;
    UINT8   uiYtmp, uiCBtmp, uiCRtmp;
    UINT8   uiActiveX, uiActiveY;
    COLOR_YCBCR *pCMtmp;

    pCMtmp = DateImprintData->colormap;
    pDB = DateImprintData->pDataBase;
    pIconHeader = pDB ->pIconHeader[uiIconID];
    uiIconOriWidth = pIconHeader.uiWidth;
    uiIconOriHeight = pIconHeader.uiHeight;
    uiIconData = (pDB ->pIconData) + (pIconHeader.uiOffset);

    if(uiIconOriHeight>uiIconHeight)
    {//scaling down
        uipixelcount = 0;
        {//Y initial
            uiLastY = uiCurrY = 0;
        }
        for(i = 0; i < uiIconOriHeight; i++)
        {
            uiActiveY = (uiCurrY>=uiLastY)?1:0;

            {//X initial
                uiLastX = uiCurrX = 0;

                uiFBY_tmp = uiFBY;
                uiFBCb_tmp = uiFBCb;
                uiFBCr_tmp = uiFBCr;
            }

            for(j = 0; j < uiIconOriWidth; j ++)
            {
                uiActiveX = (uiCurrX>=uiLastX)?1:0;

                if(uiActiveX && uiActiveY)
                {//Copy this pixel
                    uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;
                    uiYtmp  = (*(pCMtmp+uiIndex)).uiY;
                    uiCBtmp = (*(pCMtmp+uiIndex)).uiCB;
                    uiCRtmp = (*(pCMtmp+uiIndex)).uiCR;
                    memset((UINT8 *)uiFBY_tmp, uiYtmp, 1);
                    memset((UINT8 *)uiFBCb_tmp, uiCBtmp, 1);
                    memset((UINT8 *)uiFBCr_tmp, uiCRtmp, 1);
                }

                {//moving input address
                    uipixelcount ++;
                    if (uipixelcount % 4 == 0)
                        uiIconData ++;
                }

                {//X accumulation
                    if(uiActiveX)
                    {
                        uiLastX += uiIconOriWidth;
                        uiFBY_tmp ++;
                        uiFBCb_tmp ++;
                        uiFBCr_tmp ++;
                    }
                    uiCurrX += uiIconWidth;
                }
            }
            {//Y accumulation
                if(uiActiveY)
                {
                    uiLastY += uiIconOriHeight;
                    uiFBY += uiLineOffset;
                    uiFBCb += uiLineOffset;
                    uiFBCr += uiLineOffset;
                }
                uiCurrY += uiIconHeight;
            }
        }
    }
    else
    {//scaling up
        uipixelcount = 0;
        {//Y initial
            uiCurrY = 0;
        }
        for(i = 0; i < uiIconOriHeight; i++)
        {
            {//X initial
                uiCurrX = 0;

                uiFBY_tmp  = uiFBY;
                uiFBCb_tmp = uiFBCb;
                uiFBCr_tmp = uiFBCr;
            }

            for(j = 0; j < uiIconOriWidth; j ++)
            {
                {//X computing
                    uiCurrX += uiIconWidth;
                    uiActiveX = uiCurrX/uiIconOriWidth;
                    uiCurrX -= uiActiveX*uiIconOriWidth;
                }

                {//Copy this pixel in X direction
                    uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;
                    uiYtmp  = (*(pCMtmp+uiIndex)).uiY;
                    uiCBtmp = (*(pCMtmp+uiIndex)).uiCB;
                    uiCRtmp = (*(pCMtmp+uiIndex)).uiCR;
                    memset((UINT8 *)uiFBY_tmp,  uiYtmp,  uiActiveX);
                    memset((UINT8 *)uiFBCb_tmp, uiCBtmp, uiActiveX);
                    memset((UINT8 *)uiFBCr_tmp, uiCRtmp, uiActiveX);
                }

                {//moving input address
                    uipixelcount ++;
                    if (uipixelcount % 4 == 0)
                        uiIconData ++;
                }

                {//X accumulation
                    uiFBY_tmp  +=uiActiveX;
                    uiFBCb_tmp +=uiActiveX;
                    uiFBCr_tmp +=uiActiveX;
                }
            }

            {//Y computing
                uiCurrY += uiIconHeight;
                uiActiveY = uiCurrY/uiIconOriHeight;
                uiCurrY -= uiActiveY*uiIconOriHeight;
            }

            {//copy in Y direction
                {
                    uiFBY_tmp  = uiFBY;
                    uiFBCb_tmp = uiFBCb;
                    uiFBCr_tmp = uiFBCr;
                }
                for(k=1; k<uiActiveY; k++)
                {//copy in Y direction
                    uiFBY_tmp  += uiLineOffset;
                    uiFBCb_tmp += uiLineOffset;
                    uiFBCr_tmp += uiLineOffset;
                    memcpy((void *)uiFBY_tmp,  (void *)uiFBY,  uiIconWidth);
                    memcpy((void *)uiFBCb_tmp, (void *)uiFBCb, uiIconWidth);
                    memcpy((void *)uiFBCr_tmp, (void *)uiFBCr, uiIconWidth);
                }
            }

            {//Y accumulation
                uiFBY  += uiActiveY*uiLineOffset;
                uiFBCb += uiActiveY*uiLineOffset;
                uiFBCr += uiActiveY*uiLineOffset;
            }
        }
    }

    return TRUE;
}
//@}