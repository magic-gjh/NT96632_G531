/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GenDateImprintData.h
    @ingroup    Unknown

    @brief      Sample Header file for Date imprint data generation.
                Sample Header file for Date imprint data generation.

    @note       Nothing.

    @version    V1.00.000
    @author     Shirley Cheng
    @date       2006/02/15
*/

/** \addtogroup Unknown*/
//@{

#ifndef _GENDATEIMPRINTDATA_H
#define _GENDATEIMPRINTDATA_H

#include "draw_lib.h" 

// bit [0]
#define STAMP_ON                0x0000
#define STAMP_OFF               0x0001
#define STAMP_SWITCH_MASK       0x0001

// bit [1]
#define STAMP_AUTO              0x0000
#define STAMP_MANUAL            0x0002
#define STAMP_OPERATION_MASK    0x0002

// bit [3:2]
#define STAMP_DATE              0x0000
#define STAMP_DATE_TIME         0x0004
#define STAMP_TIME              0x0008
#define STAMP_DATE_TIME_MASK    0x000C

// bit [5:4]
#define STAMP_BOTTOM_LEFT       0x0000
#define STAMP_BOTTOM_RIGHT      0x0010
#define STAMP_TOP_LEFT          0x0020
#define STAMP_TOP_RIGHT         0x0030
#define STAMP_POSITION_MASK     0x0030

// bit [7:6]
#define STAMP_YY_MM_DD          0x0000
#define STAMP_DD_MM_YY          0x0040
#define STAMP_MM_DD_YY          0x0080
#define STAMP_DATE_FORMAT_MASK  0x00C0

// bit [8]
#define STAMP_BG_TRANSPARENT    0x0000  // default
#define STAMP_BG_FILL_COLOR     0x0100
#define STAMP_BG_FORMAT_MASK    0x0100

// bit [9]
#define STAMP_POS_NORMAL        0x0000  // default
#define STAMP_POS_END           0x0200
#define STAMP_POS_END_MASK      0x0200

/**
//RGB: 16~235?
#define RGB_GET_Y(r,g,b)    ( ((77*((INT32)r) + 150*((INT32)g) + 29*((INT32)b) + 128)>>8) )
#define RGB_GET_U(r,g,b)    ( ((-43*((INT32)r) - 85*((INT32)g) + 128*((INT32)b) + 128)>>8)+128 )
#define RGB_GET_V(r,g,b)    ( ((128*((INT32)r) - 107*((INT32)g) - 21*((INT32)b) + 128)>>8)+128 )
**/
#define cst_prom0   21
#define cst_prom1   79
#define cst_prom2   29
#define cst_prom3   43
#define RGB_GET_Y(r,g,b)    (((INT32)g) + ((cst_prom1 * (((INT32)r)-((INT32)g))) >> 8) + ((cst_prom2 * (((INT32)b)-((INT32)g))) >> 8) )
#define RGB_GET_U(r,g,b)    (128 + ((cst_prom3 * (((INT32)g)-((INT32)r))) >> 8) + ((((INT32)b)-((INT32)g)) >> 1) )
#define RGB_GET_V(r,g,b)    (128 + ((cst_prom0 * (((INT32)g)-((INT32)b))) >> 8) + ((((INT32)r)-((INT32)g)) >> 1) )

typedef struct
{
    UINT32    uiX;             ///< the X offset
    UINT32    uiY;             ///< the Y offset
} STAMP_POS;

typedef struct
{
    UINT8     uiY;             ///< the Y value
    UINT8     uiCB;            ///< the CB value
    UINT8     uiCR;            ///< the CR value
} COLOR_YCBCR, *PCOLOR_YCBCR;

typedef struct
{
    INT8 *pi8Str;              ///< The string of date.
    ICON_DB const *pDataBase;  ///< The font database for date.
    UINT32 ui32FontWidth;      ///< The max width of font
    UINT32 ui32FontHeight;     ///< The max height of font
    UINT32 ui32DstHeight;      ///< The destination height of font
    COLOR_YCBCR colormap[4];   ///< The Color map from 2-bit data to 24-bit data.
} DATEIMPRINT_INFO, *PDATEIMPRINT_INFO;

//#NT#2010/07/30#Meg Lin -begin
#define DATEDRAW_TYPE_422   1
#define DATEDRAW_TYPE_420UV 2
typedef struct
{
    UINT32 yAddr;
    UINT32 cbAddr;
    UINT32 crAddr;
    UINT32 lineOffsetY;
    UINT32 lineOffsetUV;
    UINT32 type;//422 or 420UV
        
} DATEDRAW_INFO;
typedef struct 
{
    UINT32 srcAddr;
    UINT32 dstAddr;
    UINT16 lineOffset;
    UINT16 width;
    UINT16 height;
    UINT8  xValue;
    UINT8  rev;

} DATEDRAW_COPYINFO;
//#NT#2010/07/30#Meg Lin -end

extern void     DateImprint_Setup(UINT32 flag);
extern void     DateImprint_SetPos(UINT32 x, UINT32 y);
extern void     DateImprint_SetPosThumb(UINT32 x, UINT32 y);
extern void     DateImprint_SetColor(PCOLOR_YCBCR pStampColorBg, PCOLOR_YCBCR pStampColorFr, PCOLOR_YCBCR pStampColorFg);

// Date stamp for movie
extern void     MovieDateImprint_SetPos(UINT32 x, UINT32 y);
extern void     MovieDateImprint_SetColor(PCOLOR_YCBCR pStampColorBg, PCOLOR_YCBCR pStampColorFr, PCOLOR_YCBCR pStampColorFg);
extern void     MovieDateImprint_SetDrawAddr(UINT32 uiAddr);
extern void     MovieDateImprint_SetDrawType(UINT32 uiType);
extern UINT32   MovieDateImprint_GetMaxFontHeight(void);
extern void     MovieDateImprint_CopyData(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth);

#endif
//@}