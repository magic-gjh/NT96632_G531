/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GenDateImprintData4Movie.c
    @ingroup    Unknown

    @brief      Sample Code for Date imprint data generation for movie.
                Sample Code for Date imprint data generation for movie.

    @note       Nothing.

    @version    V1.00.000
    @author
    @date

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
#include "UIMenuMovieSettings.h"
#include "grph.h"
#include "BG_Images.h"
#include "UIMovieObj.h"

extern UINT32 SP_1[12];
extern GPSDATA gpsdata;

#if (_MOVIE_DATE_STAMP_ == ENABLE)

//function declare
static BOOL DateIcon_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset);
static BOOL DateIcon_ToMem420uv(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset);
static void MovieDateImprint_ResetParam(void);

//variable declare
static STAMP_POS    g_MovieStampPos      = {0, 0};              // stamp position for primary image
static COLOR_YCBCR  g_MovieStampColorBg  = {0x00, 0x80, 0x80};  // date stamp background color
static COLOR_YCBCR  g_MovieStampColorFr  = {0x00, 0x80, 0x80};  // date stamp frame color
static COLOR_YCBCR  g_MovieStampColorFg  = {RGB_GET_Y(250, 160, 10), RGB_GET_U(250, 160, 10), RGB_GET_V(250, 160, 10)}; // date stamp foreground color
static char         g_cMovieDateStampStr[128];
static UINT32       g_uiMovieDateStampSetup = STAMP_OFF; // default!
static DATEIMPRINT_INFO gDateImprintData;
static RTC_TIME     gCurTime;
static UINT32       gDrawYoffset=0, gDrawUVoffset=0;
static UINT32       gDrawStrYAddr=0, gDrawStrUAddr, gDrawStrVAddr;
static UINT32       gDrawType = DATEDRAW_TYPE_422;
static UINT32       gDrawStrwidth;
static UINT32       gDrawFontHeight;
static UINT32       gDrawMaxHeight = 44; // maximum height of date stamp font (gDateStampFont26x44)
extern UINT32 Speed[2];
extern BOOL GPSDataInputFlag;

void MovieDateImprint_Setup(UINT32 flag)
{
    PDATEIMPRINT_INFO   pDateImprintData;
    UINT32 speed_hi,speed_lo,speed_total;
	
    UINT32  uiMovieIndex = Get_MovieSizeIndex();

    g_uiMovieDateStampSetup = flag;

    if ((flag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    pDateImprintData = &gDateImprintData;
    pDateImprintData->pi8Str = &g_cMovieDateStampStr[0];

    // set date stamp font data base
    switch (uiMovieIndex)
    {
#if 0
    case MOVIE_SIZE_1080P:
    case MOVIE_SIZE_720P:
        pDateImprintData->pDataBase = &gDateStampFont26x44;
        pDateImprintData->ui32FontWidth  = 26;
        pDateImprintData->ui32FontHeight = 44;
        pDateImprintData->ui32DstHeight  = 44;  // no scaling
        break;

    case MOVIE_SIZE_QVGA:
        pDateImprintData->pDataBase = &gDateStampFont10x16;
        pDateImprintData->ui32FontWidth  = 10;
        pDateImprintData->ui32FontHeight = 16;
        pDateImprintData->ui32DstHeight  = 16;  // no scaling
        break;

    default:
        pDateImprintData->pDataBase = &gDateStampFont18x30;
        pDateImprintData->ui32FontWidth  = 18;
        pDateImprintData->ui32FontHeight = 30;
        pDateImprintData->ui32DstHeight  = 30;  // no scaling
        break;
#else
    //case MOVIE_SIZE_1080P_HD:
        //pDateImprintData->pDataBase = &gDateStampFont18x30;
        //pDateImprintData->ui32FontWidth  = 18;
        //pDateImprintData->ui32FontHeight = 30;
        //pDateImprintData->ui32DstHeight  = 30;  // no scaling
        //break;		
    //case MOVIE_SIZE_1080P:
    case MOVIE_SIZE_720P:
        pDateImprintData->pDataBase = &gDateStampFont18x30;
        pDateImprintData->ui32FontWidth  = 18;
        pDateImprintData->ui32FontHeight = 30;
        pDateImprintData->ui32DstHeight  = 30;  // no scaling
        break;		
    case MOVIE_SIZE_WVGA:
    case MOVIE_SIZE_VGA:
    case MOVIE_SIZE_QVGA:
    default:
        pDateImprintData->pDataBase = &gDateStampFont10x16;
        pDateImprintData->ui32FontWidth  = 10;
        pDateImprintData->ui32FontHeight = 16;
        pDateImprintData->ui32DstHeight  = 16;  // no scaling
        break;
#endif
    }
    gDrawFontHeight = pDateImprintData->ui32DstHeight;
    speed_total = ((Speed[0]<<8)&0xff00)|(Speed[1]&0xff);	
    speed_hi = speed_total/10;
    speed_lo = speed_total%10;
    // set stamp string (for calculating stamp position)
    if ((flag & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME)
    {
        sprintf(pDateImprintData->pi8Str, "0000/00/00 00:00:00  Speed:000.0 km/h  SP:0000.0000.0000");
    }
    else
    {
        sprintf(pDateImprintData->pi8Str, "0000/00/00  Speed:000.0 km/h  SP:0000.0000.0000");
    }

    // Set date stamp position
    if ((flag & STAMP_OPERATION_MASK) == STAMP_AUTO)
    {
        UINT32  uiImageWidth = GetMovieSizeWidth(uiMovieIndex);
        UINT32  uiImageHeight = GetMovieSizeHeight(uiMovieIndex);
        UINT32  uiStampWidth = (pDateImprintData->ui32DstHeight * pDateImprintData->ui32FontWidth) / pDateImprintData->ui32FontHeight;

        switch (flag & STAMP_POSITION_MASK)
        {
        case STAMP_TOP_LEFT:
            if ((flag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos.uiX = 0;
                g_MovieStampPos.uiY = 0;
            }
            else
            {
                g_MovieStampPos.uiX = uiStampWidth * 2;
                g_MovieStampPos.uiY = pDateImprintData->ui32DstHeight;
            }
            break;

        case STAMP_TOP_RIGHT:
            if ((flag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos.uiX = uiImageWidth - uiStampWidth * strlen(pDateImprintData->pi8Str);
                g_MovieStampPos.uiY = 0;
            }
            else
            {
                g_MovieStampPos.uiX = uiImageWidth - uiStampWidth * (strlen(pDateImprintData->pi8Str) + 2);
                g_MovieStampPos.uiY = pDateImprintData->ui32DstHeight;
            }
            break;

        case STAMP_BOTTOM_LEFT:
            if ((flag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos.uiX = 0;
                g_MovieStampPos.uiY = uiImageHeight - pDateImprintData->ui32DstHeight;
            }
            else
            {
                g_MovieStampPos.uiX = uiStampWidth * 2;
                g_MovieStampPos.uiY = uiImageHeight - pDateImprintData->ui32DstHeight * 2;
            }
            break;
    
        case STAMP_BOTTOM_RIGHT:
        default:
            if ((flag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos.uiX = uiImageWidth - uiStampWidth * strlen(pDateImprintData->pi8Str);
                g_MovieStampPos.uiY = uiImageHeight - pDateImprintData->ui32DstHeight;
            }
            else
            {
                g_MovieStampPos.uiX = uiImageWidth - uiStampWidth * (strlen(pDateImprintData->pi8Str) + 2);
                g_MovieStampPos.uiY = uiImageHeight - pDateImprintData->ui32DstHeight * 2;
            }
            break;
        }
    }

    MovieDateImprint_ResetParam();
}

void MovieDateImprint_SetPos(UINT32 x, UINT32 y)
{
    g_MovieStampPos.uiX = x;
    g_MovieStampPos.uiY = y;
}

void MovieDateImprint_SetColor(PCOLOR_YCBCR pStampColorBg, PCOLOR_YCBCR pStampColorFr, PCOLOR_YCBCR pStampColorFg)
{
    // Stamp background color
    g_MovieStampColorBg.uiY  = pStampColorBg->uiY;
    g_MovieStampColorBg.uiCB = pStampColorBg->uiCB;
    g_MovieStampColorBg.uiCR = pStampColorBg->uiCR;

    // Stamp frame color
    g_MovieStampColorFr.uiY  = pStampColorFr->uiY;
    g_MovieStampColorFr.uiCB = pStampColorFr->uiCB;
    g_MovieStampColorFr.uiCR = pStampColorFr->uiCR;

    // Stamp foreground color (text body)
    g_MovieStampColorFg.uiY  = pStampColorFg->uiY;
    g_MovieStampColorFg.uiCB = pStampColorFg->uiCB;
    g_MovieStampColorFg.uiCR = pStampColorFg->uiCR;
}

UINT32 MovieDateImprint_GetMaxFontHeight(void)
{
    return gDrawMaxHeight;
}

//#NT#2010/07/30#Meg Lin -begin
static UINT32 DrawStr_ToMem(char *pStr, DATEDRAW_INFO *pDrawInfo)
{
    UINT8   uiStrLen;
    UINT32  i, j;
    ICON_DB const *pDB;
    UINT16  uiIconID;
    UINT16  uiStroffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
    UINT32  len;

    uiStrLen = strlen(pStr);
    
    pDB = gDateImprintData.pDataBase;
    uiStroffset = pDB->uiDrawStrOffset;

    uiFBAddrY = pDrawInfo->yAddr;
    uiFBAddrCb = pDrawInfo->cbAddr;
    uiFBAddrCr = pDrawInfo->crAddr;

    len = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = pStr[i] - uiStroffset ;
        j = pDB ->pIconHeader[uiIconID].uiWidth;
        len +=j;

        if (j % 2 != 0)
        {
            j++;
        }

        if (pDrawInfo->type == DATEDRAW_TYPE_422)
        {
            DateIcon_ToMem(&gDateImprintData, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pDrawInfo->lineOffsetY);
            uiFBAddrY  += (j);
            uiFBAddrCb += (j/2);
            uiFBAddrCr += (j/2);
        }
        else if (pDrawInfo->type == DATEDRAW_TYPE_420UV)
        {
            DateIcon_ToMem420uv(&gDateImprintData, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pDrawInfo->lineOffsetY);
            uiFBAddrY  += (j);
            uiFBAddrCb += (j);
            uiFBAddrCr += (j);
        }
    }

    return len;
}

static BOOL DateIcon_ToMem(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
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
    UINT8   *pChar, *pCharU, *pCharV;

    pDB = DateImprintData->pDataBase;
    pIconHeader = pDB ->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB ->pIconData) + (pIconHeader.uiOffset);

    uipixelcount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCr;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1)   // frame color
                {
                    *pChar = g_MovieStampColorFr.uiY;
                }
                else
                {
                    *pChar = g_MovieStampColorFg.uiY;
                }

                if ((j & 1)==0)
                {
                    if (uiIndex == 1)   // frame color
                    {
                        *pCharU = g_MovieStampColorFr.uiCB;
                        *pCharV = g_MovieStampColorFr.uiCR;
                    }
                    else
                    {
                        *pCharU = g_MovieStampColorFg.uiCB;
                        *pCharV = g_MovieStampColorFg.uiCR;
                    }
                }
            }
            pChar++;
            if ((j & 1)==0)
            {
                pCharU++;
                pCharV++;
            }
            uipixelcount ++;
            if (uipixelcount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        uiFBCb += uiLineOffset/2;
        uiFBCr += uiLineOffset/2;
    }
    return TRUE;
}

static BOOL DateIcon_ToMem420uv(PDATEIMPRINT_INFO DateImprintData, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
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
    UINT8   *pChar, *pCharU, *pCharV;

    pDB = DateImprintData->pDataBase;
    pIconHeader = pDB ->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB ->pIconData) + (pIconHeader.uiOffset);

    uipixelcount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;
            
        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uipixelcount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1)   // frame color
                {
                    *pChar = g_MovieStampColorFr.uiY;
                }
                else
                {
                    *pChar = g_MovieStampColorFg.uiY;
                }

                if (((j&1)==0)&&((i&1)==0))
                {
                    if (uiIndex == 1)   // frame color
                    {
                        *pCharU = g_MovieStampColorFr.uiCB;
                        *pCharV = g_MovieStampColorFr.uiCR;
                    }
                    else
                    {
                        *pCharU = g_MovieStampColorFg.uiCB;
                        *pCharV = g_MovieStampColorFg.uiCR;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uipixelcount ++;
            if (uipixelcount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;
}

static void DateStr_CopyToMem(DATEDRAW_COPYINFO *pInfo)
{
    UINT8   *pchar, *pDst;
    UINT32  *pS, *pD;
    UINT32  i, j;

    for (j=0;j<pInfo->height;j++)
    {
        pchar = (UINT8 *)pInfo->srcAddr+j*pInfo->lineOffset;
        pS = (UINT32 *)(pInfo->srcAddr+j*pInfo->lineOffset);
        pDst = (UINT8 *)pInfo->dstAddr+j*pInfo->lineOffset;
        pD = (UINT32 *)(pInfo->dstAddr+j*pInfo->lineOffset);

        for (i=0;i<pInfo->width/4;i++)
        {
            //if (*pchar != pInfo->xValue)
            //    *pDst = *pchar;
            *pD = *pS;
    
            //pchar++;
            //pDst++;
            pS++;
            pD++;
        }
    }
}

void MovieDateImprint_SetDrawAddr(UINT32 uiAddr)
{
    gDrawStrYAddr = uiAddr;
}

void MovieDateImprint_SetDrawType(UINT32 uiType)
{
    gDrawType = uiType;
}

static void MovieDateImprint_ResetParam(void)
{
    UINT32  lineoffset, height;

    lineoffset = GetMovieSizeWidth(Get_MovieSizeIndex());
    //height = MovieDateImprint_GetMaxFontHeight();
    height = gDrawFontHeight;

    if (gDrawType == DATEDRAW_TYPE_422)
    {
        gDrawStrUAddr = gDrawStrYAddr+lineoffset*height;
        gDrawStrVAddr = gDrawStrUAddr+lineoffset*height/2;
    }
    else//420UV
    {
        gDrawStrUAddr = gDrawStrYAddr+lineoffset*height;
        gDrawStrVAddr = gDrawStrUAddr;
    }

    memset((UINT8 *)gDrawStrYAddr, 0, lineoffset*height*2);
    gCurTime.s.second = 61;//reset value
    gDrawStrwidth = 0;
    gDrawYoffset=0;
    gDrawUVoffset=0;
}
#if 0
//#NT#2010/08/03#Meg Lin -begin
//modify to use graphic engine
void MovieDateImprint_CopyData(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    DATEDRAW_INFO   DrawInfo;
    RTC_DATE        CurDate;
    RTC_TIME        CurTime;
    DATEDRAW_COPYINFO copyinfo;
    UINT32  y, x;
    UINT8   backColor = 0x01;
    UINT16  uwUVData;
    UINT32  uiSize;
    UINT32 speed_hi,speed_lo,speed_total;

    if ((g_uiMovieDateStampSetup & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    x = g_MovieStampPos.uiX;
    y = g_MovieStampPos.uiY;

    CurDate = rtc_getDate();
    CurTime = rtc_getTime();

    speed_total = ((Speed[0]<<8)&0xff00)|(Speed[1]&0xff);	
    speed_hi = speed_total/10;
    speed_lo = speed_total%10;
	
    if (gDrawType == DATEDRAW_TYPE_422)
    {
        if (gDrawYoffset == 0)
        {
            gDrawYoffset = lineY*y + x;
            gDrawUVoffset = gDrawYoffset/2;
        }
        DrawInfo.lineOffsetY = lineY;
        DrawInfo.lineOffsetUV = lineY/2;
    }
    else //420uv
    {
        if (gDrawYoffset == 0)
        {
            gDrawYoffset = lineY*y + x;
            gDrawUVoffset= lineY*y/2+x;
        }
        DrawInfo.lineOffsetY = imgWidth;
        DrawInfo.lineOffsetUV = imgWidth;
    }

    if (gCurTime.s.second != CurTime.s.second)
    {
        DrawInfo.yAddr = gDrawStrYAddr;
        DrawInfo.cbAddr = gDrawStrUAddr;
        DrawInfo.crAddr = gDrawStrVAddr;
        DrawInfo.type = gDrawType;
        gCurTime.s.second = CurTime.s.second;
    
        // Prepare date-time string
        if ((g_uiMovieDateStampSetup & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME)
        {
            switch (g_uiMovieDateStampSetup & STAMP_DATE_FORMAT_MASK)
            {
            case STAMP_DD_MM_YY:
		   if(GPSDataInputFlag == TRUE){
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d  Speed: %03d.%d km/h  SP: %d%d%d%d.%d%d%d%d.%d%d%d%d", CurDate.s.day, CurDate.s.month, CurDate.s.year, 
					CurTime.s.hour, CurTime.s.minute, CurTime.s.second,speed_hi,speed_lo,SP_1[0],SP_1[1],SP_1[2],SP_1[3],SP_1[4],SP_1[5],SP_1[6],SP_1[7],SP_1[8],
			SP_1[9],SP_1[10],SP_1[11]);}
		   else{
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
			}		   	
                break;
            case STAMP_MM_DD_YY:
		   if(GPSDataInputFlag == TRUE){				
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d  Speed: %03d.%d km/h  SP: %d%d%d%d.%d%d%d%d.%d%d%d%d", CurDate.s.month, CurDate.s.day, CurDate.s.year, 
					CurTime.s.hour, CurTime.s.minute, CurTime.s.second,speed_hi,speed_lo,SP_1[0],SP_1[1],SP_1[2],SP_1[3],SP_1[4],SP_1[5],SP_1[6],SP_1[7],SP_1[8],
			SP_1[9],SP_1[10],SP_1[11]);}
		   else{
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
		   	}
                break;
            default:
		   if(GPSDataInputFlag == TRUE){								
                sprintf(g_cMovieDateStampStr, "%04d/%02d/%02d %02d:%02d:%02d  Speed: %03d.%d km/h  SP: %d%d%d%d.%d%d%d%d.%d%d%d%d", CurDate.s.year, CurDate.s.month, CurDate.s.day, 
					CurTime.s.hour, CurTime.s.minute, CurTime.s.second,speed_hi,speed_lo,SP_1[0],SP_1[1],SP_1[2],SP_1[3],SP_1[4],SP_1[5],SP_1[6],SP_1[7],SP_1[8],
			SP_1[9],SP_1[10],SP_1[11]);}
		    else{
                sprintf(g_cMovieDateStampStr, "%04d/%02d/%02d %02d:%02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
		    	}
                break;
            }
        }
        else
        {
	     if(GPSDataInputFlag == TRUE){			        
            sprintf(g_cMovieDateStampStr, "%02d:%02d:%02d  Speed: %03d.%d km/h  SP: %d%d%d%d.%d%d%d%d.%d%d%d%d", CurTime.s.hour, CurTime.s.minute, CurTime.s.second
				,speed_hi,speed_lo,SP_1[0],SP_1[1],SP_1[2],SP_1[3],SP_1[4],SP_1[5],SP_1[6],SP_1[7],SP_1[8],
			SP_1[9],SP_1[10],SP_1[11]);}
		else
		{
            sprintf(g_cMovieDateStampStr, "%02d:%02d:%02d", CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
		}
        }
    
        // Set background data
        //Perf_Open();
        //Perf_Mark();
        uiSize = imgWidth * gDrawFontHeight;
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            hwmem_open();
            hwmem_memset(gDrawStrYAddr, backColor, uiSize);
            if (gDrawType == DATEDRAW_TYPE_422)
            {
                hwmem_memset(gDrawStrUAddr, backColor, uiSize / 2);
                hwmem_memset(gDrawStrVAddr, backColor, uiSize / 2);
            }
            else
            {
                hwmem_memset(gDrawStrUAddr, backColor, uiSize / 2);
            }
            hwmem_close();
        }
        else
        {
            hwmem_open();
            hwmem_memset(gDrawStrYAddr, g_MovieStampColorBg.uiY, uiSize);
            if (gDrawType == DATEDRAW_TYPE_422)
            {
                hwmem_memset(gDrawStrUAddr, g_MovieStampColorBg.uiCB, uiSize / 2);
                hwmem_memset(gDrawStrVAddr, g_MovieStampColorBg.uiCR, uiSize / 2);
            }
            else
            {
                uwUVData = ((UINT16)g_MovieStampColorBg.uiCR << 8) | g_MovieStampColorBg.uiCB;
                hwmem_memset16(gDrawStrUAddr, uwUVData, uiSize / 2);
            }
            hwmem_close();
        }
        //debug_err((">> time1: %d us\r\n", Perf_GetDuration()));
        //Perf_Close();
    
        // Draw string by getting data from date stamp font
        //Perf_Open();
        //Perf_Mark(); 
        gDrawStrwidth = DrawStr_ToMem(&g_cMovieDateStampStr[0], &DrawInfo);
        //debug_err((">> time2: %d us\r\n", Perf_GetDuration()));
        //Perf_Close();
    }

    // copy data from date stamp draw buffer to image buffer
    //Perf_Open();
    //Perf_Mark();
    grph_open();

    // Handle Y
    grph_setImg1(yAddr+gDrawYoffset, lineY, gDrawFontHeight, gDrawStrwidth);
    grph_setImg2(gDrawStrYAddr, imgWidth);
    if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
    else
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);

    if (gDrawType == DATEDRAW_TYPE_422)
    {
        // Handle Cb/Cr (YUV422)
        grph_setImg1(cbAddr+gDrawUVoffset, lineY/2, gDrawFontHeight, gDrawStrwidth/2);
        grph_setImg2(gDrawStrUAddr, imgWidth/2);
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
        else
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);

        grph_setImg1(crAddr+gDrawUVoffset, lineY/2, gDrawFontHeight, gDrawStrwidth/2);
        grph_setImg2(gDrawStrVAddr, imgWidth/2);
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
        else
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);
    }
    else
    {
        // Handle Cb/Cr (YUV420, UV pack)
        grph_setImg1(cbAddr+gDrawUVoffset, lineY, gDrawFontHeight/2, gDrawStrwidth);
        grph_setImg2(gDrawStrUAddr, imgWidth);
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
        else
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);
    }

#if (_WATERMARK_EMBEDDED_ == ENABLE)
    // embedded watermark logo
    // Handle Y (YUV420, UV pack)
    grph_setImg1((UINT32)g_ucWaterMarkY, 64, 64, 64);
    grph_setImg2(yAddr, lineY);
    grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    // Handle Cb/Cr (YUV420, UV pack)
    if (gDrawType == DATEDRAW_TYPE_422)
    {
        //Cb
        grph_setImg1((UINT32)g_ucWaterMarkUVPack, 32, 64, 32);
        grph_setImg2((yAddr+4096), lineY/2);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
        //Cr
        grph_setImg1((UINT32)(g_ucWaterMarkUVPack+2048), 32, 64, 32);
        grph_setImg2((yAddr+6144), lineY/2);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    } else {
        grph_setImg1((UINT32)g_ucWaterMarkUVPack, 64, 32, 64);
        grph_setImg2(cbAddr, lineY);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }
#endif

    grph_close();
    //debug_err((">> time3: %d us\r\n", Perf_GetDuration()));
    //Perf_Close();
//#NT#2010/08/03#Meg Lin -end
}
//#NT#2010/07/30#Meg Lin -end
#else
void MovieDateImprint_CopyData(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    DATEDRAW_INFO   DrawInfo;
    RTC_DATE        CurDate;
    RTC_TIME        CurTime;
    DATEDRAW_COPYINFO copyinfo;
    UINT32  y, x;
    UINT8   backColor = 0x01;
    UINT16  uwUVData;
    UINT32  uiSize;

    if ((g_uiMovieDateStampSetup & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    x = g_MovieStampPos.uiX;
    y = g_MovieStampPos.uiY;

    CurDate = rtc_getDate();
    CurTime = rtc_getTime();

	
    if (gDrawType == DATEDRAW_TYPE_422)
    {
        if (gDrawYoffset == 0)
        {
            gDrawYoffset = lineY*y + x;
            gDrawUVoffset = gDrawYoffset/2;
        }
        DrawInfo.lineOffsetY = lineY;
        DrawInfo.lineOffsetUV = lineY/2;
    }
    else //420uv
    {
        if (gDrawYoffset == 0)
        {
            gDrawYoffset = lineY*y + x;
            gDrawUVoffset= lineY*y/2+x;
        }
        DrawInfo.lineOffsetY = imgWidth;
        DrawInfo.lineOffsetUV = imgWidth;
    }

    if (gCurTime.s.second != CurTime.s.second)
    {
        DrawInfo.yAddr = gDrawStrYAddr;
        DrawInfo.cbAddr = gDrawStrUAddr;
        DrawInfo.crAddr = gDrawStrVAddr;
        DrawInfo.type = gDrawType;
        gCurTime.s.second = CurTime.s.second;
    
        // Prepare date-time string
        if ((g_uiMovieDateStampSetup & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME)
        {
            switch (g_uiMovieDateStampSetup & STAMP_DATE_FORMAT_MASK)
            {
            case STAMP_DD_MM_YY:
		   if(GPSDataInputFlag == TRUE){
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d  Speed: %f km/h", CurDate.s.day, CurDate.s.month, CurDate.s.year, 
					CurTime.s.hour, CurTime.s.minute, CurTime.s.second,gpsdata.rmcinfo.Speed);}
		   else{
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.day, CurDate.s.month, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
			}		   	
                break;
            case STAMP_MM_DD_YY:
		   if(GPSDataInputFlag == TRUE){				
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d  Speed: %f km/h", CurDate.s.month, CurDate.s.day, CurDate.s.year, 
					CurTime.s.hour, CurTime.s.minute, CurTime.s.second,gpsdata.rmcinfo.Speed);}
		   else{
                sprintf(g_cMovieDateStampStr, "%02d/%02d/%04d %02d:%02d:%02d", CurDate.s.month, CurDate.s.day, CurDate.s.year, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
		   	}
                break;
            default:
		   if(GPSDataInputFlag == TRUE){								
                sprintf(g_cMovieDateStampStr, "%04d/%02d/%02d %02d:%02d:%02d  Speed: %f km/h", CurDate.s.year, CurDate.s.month, CurDate.s.day, 
					CurTime.s.hour, CurTime.s.minute, CurTime.s.second,gpsdata.rmcinfo.Speed);}
		    else{
                sprintf(g_cMovieDateStampStr, "%04d/%02d/%02d %02d:%02d:%02d", CurDate.s.year, CurDate.s.month, CurDate.s.day, CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
		    	}
                break;
            }
        }
        else
        {
	     if(GPSDataInputFlag == TRUE){			        
            sprintf(g_cMovieDateStampStr, "%02d:%02d:%02d  Speed: %f km/h", CurTime.s.hour, CurTime.s.minute, CurTime.s.second,gpsdata.rmcinfo.Speed);}
		else
		{
            sprintf(g_cMovieDateStampStr, "%02d:%02d:%02d", CurTime.s.hour, CurTime.s.minute, CurTime.s.second);
		}
        }
    
        // Set background data
        //Perf_Open();
        //Perf_Mark();
        uiSize = imgWidth * gDrawFontHeight;
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            hwmem_open();
            hwmem_memset(gDrawStrYAddr, backColor, uiSize);
            if (gDrawType == DATEDRAW_TYPE_422)
            {
                hwmem_memset(gDrawStrUAddr, backColor, uiSize / 2);
                hwmem_memset(gDrawStrVAddr, backColor, uiSize / 2);
            }
            else
            {
                hwmem_memset(gDrawStrUAddr, backColor, uiSize / 2);
            }
            hwmem_close();
        }
        else
        {
            hwmem_open();
            hwmem_memset(gDrawStrYAddr, g_MovieStampColorBg.uiY, uiSize);
            if (gDrawType == DATEDRAW_TYPE_422)
            {
                hwmem_memset(gDrawStrUAddr, g_MovieStampColorBg.uiCB, uiSize / 2);
                hwmem_memset(gDrawStrVAddr, g_MovieStampColorBg.uiCR, uiSize / 2);
            }
            else
            {
                uwUVData = ((UINT16)g_MovieStampColorBg.uiCR << 8) | g_MovieStampColorBg.uiCB;
                hwmem_memset16(gDrawStrUAddr, uwUVData, uiSize / 2);
            }
            hwmem_close();
        }
        //debug_err((">> time1: %d us\r\n", Perf_GetDuration()));
        //Perf_Close();
    
        // Draw string by getting data from date stamp font
        //Perf_Open();
        //Perf_Mark(); 
        gDrawStrwidth = DrawStr_ToMem(&g_cMovieDateStampStr[0], &DrawInfo);
        //debug_err((">> time2: %d us\r\n", Perf_GetDuration()));
        //Perf_Close();
    }

    // copy data from date stamp draw buffer to image buffer
    //Perf_Open();
    //Perf_Mark();
    grph_open();

    // Handle Y
    grph_setImg1(yAddr+gDrawYoffset, lineY, gDrawFontHeight, gDrawStrwidth);
    grph_setImg2(gDrawStrYAddr, imgWidth);
    if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
    else
        grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);

    if (gDrawType == DATEDRAW_TYPE_422)
    {
        // Handle Cb/Cr (YUV422)
        grph_setImg1(cbAddr+gDrawUVoffset, lineY/2, gDrawFontHeight, gDrawStrwidth/2);
        grph_setImg2(gDrawStrUAddr, imgWidth/2);
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
        else
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);

        grph_setImg1(crAddr+gDrawUVoffset, lineY/2, gDrawFontHeight, gDrawStrwidth/2);
        grph_setImg2(gDrawStrVAddr, imgWidth/2);
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
        else
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);
    }
    else
    {
        // Handle Cb/Cr (YUV420, UV pack)
        grph_setImg1(cbAddr+gDrawUVoffset, lineY, gDrawFontHeight/2, gDrawStrwidth);
        grph_setImg2(gDrawStrUAddr, imgWidth);
        if ((g_uiMovieDateStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_EQ, backColor);
        else
            grph_setAOP(FALSE, GRPH_DST_1, GRPH_AOP_COLOR_LE, 0x00);
    }

#if (_WATERMARK_EMBEDDED_ == ENABLE)
    // embedded watermark logo
    // Handle Y (YUV420, UV pack)
    grph_setImg1((UINT32)g_ucWaterMarkY, 64, 64, 64);
    grph_setImg2(yAddr, lineY);
    grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    // Handle Cb/Cr (YUV420, UV pack)
    if (gDrawType == DATEDRAW_TYPE_422)
    {
        //Cb
        grph_setImg1((UINT32)g_ucWaterMarkUVPack, 32, 64, 32);
        grph_setImg2((yAddr+4096), lineY/2);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
        //Cr
        grph_setImg1((UINT32)(g_ucWaterMarkUVPack+2048), 32, 64, 32);
        grph_setImg2((yAddr+6144), lineY/2);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    } else {
        grph_setImg1((UINT32)g_ucWaterMarkUVPack, 64, 32, 64);
        grph_setImg2(cbAddr, lineY);
        grph_setAOP(FALSE, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }
#endif

    grph_close();
    //debug_err((">> time3: %d us\r\n", Perf_GetDuration()));
    //Perf_Close();
//#NT#2010/08/03#Meg Lin -end
}
//#NT#2010/07/30#Meg Lin -end


#endif

#endif
//@}
