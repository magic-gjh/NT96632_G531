/**
    @file       ImgEnh.h
    @ingroup

    @brief      Header file for ImgEnh module.

                This file is the header file that define the API and data type
                for Image Enhance module.

    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.
*/

#ifndef _IMGENH_H
#define _IMGENH_H


#include "Type.h"
#include "ErrorNo.h"
#include "JPEG.h"

/**
    @addtogroup
*/

//@{

/**
    @name   ImgEnh Mono atttibute paramter

    Definition for ImgEnh Attribute parameter
*/
//@{
#define IMGENH_COLOR_MONO        0x00000000    ///< Default Mono color. ie, Black & White
#define IMGENH_COLOR_SEPIA       0x00000001    ///< Mono color yellow.
#define IMGENH_CONTRAST_HIGH     0x00010000    ///< Apply High Contrast
//@}

/**
    @name   ImgEnh Pathwork attribute parameter

    Definition for ImgEnh Attribute parameter
*/
//@{
#define IMGENH_PATCHWORK_CELL_8x8       0x00    ///< patchwork cell is 8 * 8
#define IMGENH_PATCHWORK_CELL_16x16     0x01    ///< patchwork cell is 16 * 16
#define IMGENH_PATCHWORK_CELL_32x32     0x02    ///< patchwork cell is 32 * 32
#define IMGENH_PATCHWORK_CELL_64x64     0x03    ///< patchwork cell is 64 * 64
#define IMGENH_PATCHWORK_CELL_128x128   0x04    ///< patchwork cell is 128 * 128
//@}

/**
    @name   ImgEnh Precision definition

    Definition for ImgEnh Precision
*/
//@{
#define IMGENH_PRECISION8     0x00              ///< Y, U, V for each pixel is 8 bits
#define IMGENH_PRECISION16    0x01              ///< Y, U, V for each pixel is 16 bits
//@}

/**
    ImgEnh image information structure
*/
typedef struct
{
    UINT32 uiWidth;               ///< Input image active Width. Unit: Bytes
    UINT32 uiHeight;              ///< Line Count. Unit: Lines

    UINT32 uiSrcLineOffset;       ///< Source Line Offset. Unit: Bytes
    UINT32 uiSrcAddress_Y;        ///< Starting Address of Source Image Y component
    UINT32 uiSrcAddress_U;        ///< Starting Address of Source Image U component
    UINT32 uiSrcAddress_V;        ///< Starting Address of Source Image V component

    UINT32 uiDstLineOffset;       ///< Destination Line Offset   Unit: Bytes
    UINT32 uiDstAddress_Y;        ///< Starting Address of Destination Image Y component
    UINT32 uiDstAddress_U;        ///< Starting Address of Destination Image Y component
    UINT32 uiDstAddress_V;        ///< Starting Address of Destination Image Y component
} IMGENH_IMGINFO,*PIMGENH_IMGINFO;

/**
    ImgEnh computation buffer structure
*/
typedef struct
{
    UINT32 uiBuffAdr;           ///< Buffer Starting Address
    UINT32 uiBuffSize;          ///< Buffer Size. Units: Bytes

} IMGENH_BUFINFO, PIMGENH_BUFINFO;

/**
    ImgEnh main structure
*/
typedef struct
{
    UINT32          uiImageFormat;      ///< Input image format. Use:
                                        ///< - @b JPEG_FORMAT_422
                                        ///< - @b JPEG_FORMAT_420
    UINT32          uiImagePrecision;   ///< Input image precision. Use:
                                        ///< - @b IMGENH_PRECISION8
                                        ///< - @b IMGENH_PRECISION16
    UINT32          uiAttribute1;       ///< Attribute Parameter. Use:
                                        ///< - IMGENH_*, Ex: IMGENH_COLOR_MONO / IMGENH_CONTRAST_HIGH
    IMGENH_IMGINFO  ImageInfo;          ///< Image information structure
    IMGENH_BUFINFO  Buffer;             ///< Computation buffer structure

} IMGENH_IMGSETTING, *PIMGENH_IMGSETTING;

// API
extern  ER  ImgEnh_applyMono(PIMGENH_IMGSETTING pImageSetting);
extern  ER  ImgEnh_applyInvert(PIMGENH_IMGSETTING pImageSetting);
extern  ER  ImgEnh_applyCutout(PIMGENH_IMGSETTING pImageSetting);
extern  ER  ImgEnh_applyPatchwork(PIMGENH_IMGSETTING pImageSetting);
extern  ER  ImgEnh_applyStamp(PIMGENH_IMGSETTING pImageSetting);

//@}

#endif
