/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JpgEec.h
    @ingroup    mIAVJPEG

    @brief      Jpeg encoder header

*/

#ifndef _JPGENC_H
#define _JPGENC_H

#include "Type.h"
#include "OSCommon.h"
#include "JPEG.h"
#include "Exif.h"

/** \addtogroup mIAVJPEG
*/
//@{

#define JPGHeaderSize               sizeof(JPG_HEADER)
//#NT#2008/11/11#Daniel Lu - begin
//#Add for supporting JPEG gray format.
#define JPGHeaderSizeGray           sizeof(JPG_HEADER_GRAY)
//#NT#2008/11/11#Daniel Lu - end

#define THUMBNAIL_WIDTH             160     //320
#define THUMBNAIL_HEIGHT            120     //240
#define VERSION_CODE_ADDRESS        0x80004028

/**
    JPEG Encode Parameter.
*/
typedef struct
{
    UINT    RawImageAddr[3];                ///< Raw Image Y address. CB CR will following this address.

    UINT    JPGBSAddr;                      ///< Compressed data ouput location.
    UINT    *JPGBSBufSize;                  ///< Buffer Size allocated for JPG ouput bitstream. (IN/OUT Parameter)
//#NT#2007/01/11#Scottie -begin
//#Add for supporting JPEG-DCOutput.
    UINT    JPGDCAddr[3];                   ///< JPEG-DCOutput_Y location. CB CR will following this address.
    BOOL    bOuputJpgDC;                    ///< Enable 1/8 scaling down output (JPEG-DCOutput).
//#NT#2007/01/11#Scottie -end
    UINT    Width;                          ///< Raw Image Width, This value should be 16-multiplier.
    UINT    Height;                         ///< Raw Image Height, This value should be 8 or 16 multiplier, depends on output bs format.
    UINT    Mode;                           ///< Encoding Mode. Including Slice/JPG BS format
//#NT#2008/11/11#Daniel Lu - begin
//#Add for supporting rotate mode, restart interval mode and thumbnail output mode.
    UINT8   ucTPOSEnable;                   ///< Rotate Mode
    USHORT  usRstInterval;                  ///< restart interval (initially set to 0)
    UINT8   ThumbEn;
    UINT16  LineOffsetY;
    UINT16  LineOffsetCbCr;
    UINT32  YStartAddrOffset;
    UINT32  CStartAddrOffset;
//#NT#2008/11/11#Daniel Lu - end
} JPGCmpParam, *PJPGCmpParam;
//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
typedef struct
{
    UINT32  frameCount;
    UINT32  baseOffset;
    UINT32  addOffset;
}  MJPEG_MOVVIDEOINFO;
//#NT#2007/10/17#Meg Lin -end
/**
    JPEG Encode parameter.
*/
typedef struct
{
    UINT    RawImageAddr_Y;                 ///< Thumbnail Raw Image Address for Y,  size 160*120. (width*height)
    UINT    RawImageAddr_CB;                ///< Thumbnail Raw Image Address for CB, size depends on format.
    UINT    RawImageAddr_CR;                ///< Thumbnail Raw Image Address for CR,

    UINT    EXIFBSAddr;                     ///< EXIF Header.
    UINT    *EXIFBufSize;                   ///< Buffer Size allocated for EXIF ouput bitstream. (IN/OUT Parameter)
                                            ///< This variable will also return the output result buffer size!!
//#NT#2007/02/13#Scottie -begin
//#Add for supporting JPEG-DCOutput.
    UINT    JPGDCAddr[3];                   ///< JPEG-DCOutput_Y location. CB CR will following this address.
    BOOL    bOuputJpgDC;                    ///< Enable 1/8 scaling down output (JPEG-DCOutput).
//#NT#2007/02/13#Scottie -end
    UINT    Width;                          ///< Raw Image Width, This value should be 16-multiplier. Remark:This value is ORIGINAL image Size!!
    UINT    Height;                         ///< Raw Image Height, Remark:This value is ORIGINAL image Size!!

    UINT    Mode;                           ///< Thumbnail Encoding Mode.
} EXIFEncParam, *PEXIFEncParam;


/**
    Supporting Hidden Thumbnail Images.
    This structure used for JpegCopyEXIFHeader only.
*/
typedef struct
{
    UINT32    *EXIFIMGAddr;                 ///< EXIF IMAGE START ADDRESS.

    UINT32    *EXIFIMGBufSize;              ///< Buffer Size allocated for WHOLE EXIF ouput bitstream. (IN/OUT Parameter)
                                            ///< This variable will also return the output result buffer size!!
} EXIFIMGPARAM, *PEXIFIMGPARAM;

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
//#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end

/**
    Exif GPS tag information structure.
*/
typedef struct
{
    UINT    Latitude_DegNum;
    UINT    Latitude_DegDenum;
    UINT    Latitude_SecNum;
    UINT    Latitude_SecDenum;
    UINT    Latitude_MinNum;
    UINT    Latitude_MinDenum;

    UINT    Longitude_DegNum;
    UINT    Longitude_DegDenum;
    UINT    Longitude_SecNum;
    UINT    Longitude_SecDenum;
    UINT    Longitude_MinNum;
    UINT    Longitude_MinDenum;

    UINT8   LatitudeRef;                            //N or S
    UINT8   LongitudeRef;                           //E or W
    UINT16  Padding;
} EXIFGPSINFO, *PEXIFGPSINFO;
//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
//#endif
//#NT#2009/03/18#Photon Lin -begin


/**
@name JPEG compression quality setting.
    These values are predefined meaning with specific range.
*/
//@{
#define  Quality_Undefine           0       ///< use default quality value
#define  Quality_SuperHigh          1       ///< uiQuality as 0, 97~95
#define  Quality_High               2       ///< uiQuality as 94~90
#define  Quality_Economy            3       ///< uiQuality as 89~80
//@}

//#NT#2007/03/06#Scottie -begin
/**
@name JPEG quantization value macros.
    These values are predefined meaning with specific compression ratio (vs YUV422 RAW Data).
*/
//@{
#define  JPEG_Q_NORMAL_2P7          98      ///< normal situation, compression ratio: 2.7x
#define  JPEG_Q_NORMAL_4            95      ///< normal situation, compression ratio: 4x
#define  JPEG_Q_NORMAL_6            90      ///< normal situation, compression ratio: 6x
#define  JPEG_Q_NORMAL_7            87      ///< normal situation, compression ratio: 7x
#define  JPEG_Q_NORMAL_8            84      ///< normal situation, compression ratio: 8x

#define  JPEG_Q_PORTRAIT_2P7        99      ///< Portrait, compression ratio: 2.7x
#define  JPEG_Q_PORTRAIT_4          96      ///< Portrait, compression ratio: 4x
#define  JPEG_Q_PORTRAIT_6          93      ///< Portrait, compression ratio: 6x
#define  JPEG_Q_PORTRAIT_7          92      ///< Portrait, compression ratio: 7x
#define  JPEG_Q_PORTRAIT_8          90      ///< Portrait, compression ratio: 8x

#define  JPEG_Q_LANDSCAPE_2P7       97      ///< Landscape, compression ratio: 2.7x
#define  JPEG_Q_LANDSCAPE_4         94      ///< Landscape, compression ratio: 4x
#define  JPEG_Q_LANDSCAPE_6         85      ///< Landscape, compression ratio: 6x
#define  JPEG_Q_LANDSCAPE_7         81      ///< Landscape, compression ratio: 7x
#define  JPEG_Q_LANDSCAPE_8         78      ///< Landscape, compression ratio: 8x
//@}
//#NT#2007/03/06#Scottie -end


//#NT#2007/03/20#KH Chang -begin
//#JPEG header with/without huffman table
/**
@name JPEG Header with/without huffman table.

*/
//@{
#define  JPEG_HDR_HUFFMANTBL_WITHOUT        0
#define  JPEG_HDR_HUFFMANTBL_WITH           1
//@}
//#NT#2007/03/20#KH Chang -end

//
//  Not support yet... wil be added in near future.
//
//---- Functions relesse -------------------------------

extern void     JPEGWriteAPP1Orientation(UINT, UINT32);
extern void     JPEGWriteAPP1ExposureTime(UINT, UINT, UINT);
extern void     JPEGWriteAPP1FNumber(UINT, UINT, UINT);
extern void     JPEGWriteAPP1ShutterSpeed(UINT, INT, INT);
extern void     JPEGWriteAPP1Aperture(UINT, UINT, UINT);
extern void     JPEGWriteAPP1MaxAperture(UINT, UINT, UINT);
extern void     JPEGWriteAPP1FocalLength(UINT, UINT, UINT);
extern void     JPEGWriteAPP1ExposureBias(UINT, INT, INT);
extern void     JPEGWriteAPP1ExposureProgram(UINT, INT);
extern void     JPEGWriteAPP1ISOSpeed(UINT, INT);
extern void     JPEGWriteAPP1LightSource(UINT, INT);
extern void     JPEGWriteAPP1StrobeFlash(UINT, INT);

#ifdef _Support_Brightness_Tag
extern void     JPEGWriteAPP1BrightnessValue(UINT, INT, INT);
#endif

extern void     JPEGWriteAPP1ExposureMode(UINT, INT);
extern void     JPEGWriteAPP1SensingMethod(UINT, INT);
extern void     JPEGWriteAPP1MeteringMode(UINT, INT);
extern void     JPEGWriteAPP1WhiteBalance(UINT, INT);
extern void     JPEGWriteAPP1SceneCaptureType(UINT, INT);
//#NT#2010/01/15#Scottie -begin
//#NT#Support JPEG EXIF-Sharpness
extern void     JPEGWriteAPP1Sharpness(UINT, INT);
//#NT#2010/01/15#Scottie -end
extern void     JPEGWriteAPP1CustomRendered(UINT, INT);
extern void     JPEGWriteJPGQulaity(UINT, UCHAR );

extern void     JPEGWriteAPP1ADC(UINT , USHORT);
extern void     JPEGWriteAPP1Iteration(UINT, USHORT);

extern ER       JpegEncEXIFHeader(PEXIFEncParam pEXIFParam);
extern ER       JpegEncOnePic(PJPGCmpParam JpgParam, UINT uiQuality);
extern UINT     WaitJPGEncFinish(void);
extern void     JpegInit(void);

extern ER   JpegCopyEXIFHeader(PEXIFEncParam pEXIFParam);

// In order to add a hidden thumbnail image inside MakerNote, we modify the prototype of JpegCopyEXIFHeader
// If you don't have the second hidden thumbnail, set pEXIFHIDDENImg= NULL
// pEXIFDCFThumnail could not be NULL.
extern ER       JpegCopyEXIFHeader2(PEXIFIMGPARAM pEXIFIMG, PEXIFEncParam pEXIFDCFThumnail, PEXIFEncParam pEXIFHIDDENImg);

// In order to add debug message into MakerNote data area, we provide this new API for the caller
// to know where it can put its data. (maxmium bytes is MakeNoteLen_DEBUGBUFSIZE bytes)
extern UINT32   JpegGetEXIFHeaderMakeNoteAddr(UINT32 EXIFAddrs);


//For QV 5.0 AVI
//#NT#2007/03/20#KH Chang -begin
//#Add one parameter to select JPEG header with/without huffman table
extern ER       JpegEncQV5OnePic(PJPGCmpParam JpgParam, UINT8 *QTable, BOOL bHuffmanTbl);
//#NT#2007/03/20#KH Chang -end
extern void     JpegSetEncQTable(UINT uiQuality, UINT8 *QTable);
//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
extern ER JpegEncMovMjpgOnePic(PJPGCmpParam JpgParam, UINT8 *QTable, BOOL bHuffmanTbl);
//#NT#2007/10/17#Meg Lin -end

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
extern void  JPEGWriteAPP1GPSInfo(PEXIFGPSINFO pGPSInfo);
//extern void  JPEGWriteAPP1GPSInfo(UINT pAPP1, PEXIFGPSINFO pGPSInfo);
//#NT#2009/03/18#Photon Lin -end
#endif
//#NT#2007/3/15#Connie Yu -begin
//Clear warnings
ER JpegEncThumbnail(PEXIFEncParam pEXIFParam, UINT uiQuality);
//#NT#2007/3/15#Connie Yu -end

//#NT#2009/11/17#SA4(Hideo Lin) -begin
//#NT#Added for setting social network value in JPEG APP1 header
extern void JpgEnc_SetEXIFSocialNetworkValue(UINT32 uiValue);
//#NT#2009/11/17#SA4(Hideo Lin) -end

//---- data relesse ------------------------------------


//------------------------------------------------------

//@}

#endif // _JPGENC_H