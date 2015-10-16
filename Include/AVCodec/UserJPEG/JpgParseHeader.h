/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       JpgParseHeader.h
    @ingroup    mIAVJPEG

    @brief      Jpeg parser header

*/


#ifndef _JPEGPARSEHEADER_H
#define _JPEGPARSEHEADER_H

#include "Type.h"
#include "Exif.h"
#include "JPEG.h"

/** \addtogroup mIAVJPEG
*/
//@{

//#NT#2007/03/13#Scottie -begin
//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
//#Add DEC_THUMB_ONLY for decoding thumbnail only mode
/**
    Decode JPEG type.

    One jpeg file contains one primary image, one 160x120 thumbnail image,
    and one hidden 320x240 thumbnail image.
    For MJPEG AVI file, the frame inside also a JPEG file.
*/
typedef enum {
    DEC_PRIMARY,        ///<0x00, decode primary image
    DEC_THUMBNAIL,      ///<0x01, decode thumbnail, or decode primary image
    DEC_AVI,            ///<0x02, decode MJPEG AVI first frame
    DEC_HIDDEN,         ///<0x03, decode hidden thumb, or decode primary image
    DEC_THUMB_ONLY,     ///<0x04, decode thumbnail only
    DEC_MOVMJPG         ///<0x05, decode motion jpg MOV
} JPG_DEC_TYPE;
//#NT#2007/10/17#Meg Lin -end
//#NT#2007/03/13#Scottie -end

//#NT#2008/12/01#Daniel Lu -begin
//#NT#The definition move into JPEG.h
/**
    JPEG macroblock color format.
*/
#if 0
typedef enum {
    JPGFormatYUV211,    ///<0, JPEG format macroblock color format is YCbCr422
    JPGFormatYUV420,    ///<1, JPEG format macroblock color format is YCbCr420
    JPGFormatYUV444,    ///<2, JPEG format macroblock color format is YCbCr444
//#NT#2007/10/17#Scottie -begin
//#Support YUV422 format
    JPGFormatYUV422     ///<3, JPEG format macroblock color format is YCbCr422
//#NT#2007/10/17#Scottie -end
} JPG_COLORFMT;
#endif
//#NT#2008/12/01#Daniel Lu -end

/**
    JPEG rotate setting.
*/
typedef enum {
    JPGROTATE_DISABLE,  ///<0, Disable JPEG rotate
    JPGROTATE_CW,       ///<1, Enable JPEG clockwise rotate
    JPGROTATE_CCW,      ///<2, Enable JPEG counter-clockwise rotate
    JPGROTATE_180       ///<3, Enable JPEG 180 degree clockwise rotate
} JPG_ROTATE_TYPE;

/**
    Parse EXIF Information.

    These elements are filled with EXIF information in JPEG header parsing.
*/
typedef struct
{
    // decode hidden thumb
    //USHORT          OffsetBitstreamStartAddr;
    USHORT          OffsetQTabY;
    USHORT          OffsetQTabUV;
    USHORT          OffsetAPP1Size;
    USHORT          OffsetThumbJPGSize;
    USHORT          ThumbAPP1Size;
    USHORT          ThumbExifHeaderSize;
    USHORT          ThumbJPGHeaderSize;
    USHORT          ThumbBitstreamSize;
    USHORT          OffsetHiddenJPG;
    USHORT          OffsetHiddenQTabY;
    USHORT          OffsetHiddenQTabUV;
    USHORT          OffsetFFD8;
    USHORT          ThunbJPGSize;
    USHORT          OffsetMakerNoteSize;
    USHORT          MakerNoteSize;
    USHORT          Reserved1;

//#NT#2007/03/07#Scottie -begin
//#Add EXIF-Model info.
    UCHAR           ModelStr[32];                   //Model (tag:0x0110)
//#NT#2007/03/07#Scottie -end
    USHORT          Orientation;                    //Orientation (tag:0x0112)

    USHORT          OffsetPixelXDimension;
    USHORT          OffsetPixelYDimension;

    UCHAR           JPGQuality;
    UCHAR           ByteOrder;
    UCHAR           MakerNoteConfirm;
    UCHAR           Reserved[1];

    USHORT          ISOValue;                       //ISO speed ratings (tag:0x8827)
    USHORT          ExpProg;                        //Exposure Program. It's index. (tag:0x8822)

    USHORT          ImageWidth;                     //Not necessary information in EXIF.
    USHORT          ImageHeight;                    //We will provide this information as we can, otherwise 0.

    USHORT          WhiteBalance;                   //White Balance Index, (tag:0xA403)
//#NT#2009/09/24#SA4 -begin
     SHORT          LightSource;                    //Hideo@0904 for Light Source Index, (tag:0x9208)
//#NT#2009/09/24#SA4 -end
    USHORT          MeteringMode;                   //Metering Mode, Index. (tag:0x9207)
//#NT#2009/09/24#SA4 -begin
//#NT#2007/09/05#Scottie -begin
//#Add EXIF-Sharpness tag
    USHORT          Sharpness;                      //Sharpness Index, (tag:0xA40A)
//#NT#2007/09/05#Scottie -end
//#NT#2009/09/24#SA4 -end
    //#NT#2009/11/17#SA4(Hideo Lin) -begin
    //#NT#Added for social network value
    USHORT          SocialNetVal;                   //Social network (tag:0xEFEE, temporary)
    //#NT#2009/11/17#SA4(Hideo Lin) -end

    UCHAR           ExpBiasValue[6];                //ExposureBiasValue (tag:0x9204) -99.9 ~ 99.9
    USHORT          ExposureMode;                   //Exposure Mode. Index. 0: Auto, 1:Manual, 2: Auto bracket (tag:A402)
    // add StrobeFlash & ExposureTime info.
    USHORT          StrobeFlash;                    //StrobeFlash (tag:0x9209)
    UCHAR           ExposureTime[8];                //ExposureTime (tag:0x829A)  Format: X/XX
    UCHAR           FNumber[8];                     //F number (tag:0x829D)  Format: X.X
    UCHAR           ApertureValue[8];               //Aperture (tag:0x9202)  Format: X.X

    RTCStruct       CaptureTime;                    //20 bytes string, zero-end string. define in EXIF.h

    //#NT#2009/03/18#Photon Lin -begin
    UINT32          GPSVer;
    UCHAR           GPSLatRef;
    UCHAR           GPSLonRef;
    UINT32          GPSLatNum1;
    UINT32          GPSLatNum2;
    UINT32          GPSLatNum3;
    UINT32          GPSLatDenNum1;
    UINT32          GPSLatDenNum2;
    UINT32          GPSLatDenNum3;
    UINT32          GPSLonNum1;
    UINT32          GPSLonNum2;
    UINT32          GPSLonNum3;
    UINT32          GPSLonDenNum1;
    UINT32          GPSLonDenNum2;
    UINT32          GPSLonDenNum3;
    //#NT#2009/03/18#Photon Lin -end

    //#NT#2010/09/16#Hideo Lin -begin
    //#NT#Add User Comment tag
    char            UserComment[TagUserCommentLen];
    //#NT#2010/09/16#Hideo Lin -end

} EXIFHeaderInfo, *PEXIFHeaderInfo;

/**
    JPEG decode configuration.
*/
typedef struct _JPGHEAD_DEC_CFG{
    UINT    headerlen;          ///< length of header
    //#NT#2007/08/30#JR Huang begin
    //#NT#2008/03/03#JR Huang begin
    //force to replace Huffman table
    //BOOL    bstandardHT;        ///< check  standard Hufman table ?
    //#NT#2008/03/03#JR Huang end
    //#NT#2007/08/30#JR Huang end
    UCHAR   *inbuf;             ///< input buffer start address
    UCHAR   *pQTabY;            ///< start addr of 0th-Quantization table
    UCHAR   *pQTabUV;           ///< start addr of 1th-Quantization table
    UCHAR   *pHuffDC0th;        ///< start addr of 0th-Huff-DC table
    UCHAR   *pHuffDC1th;        ///< start addr of 1th-Huff-DC table
    UCHAR   *pHuffAC0th;        ///< start addr of 0th-Huff-AC table
    UCHAR   *pHuffAC1th;        ///< start addr of 1th-Huff-AC table

    UCHAR   *pTagSOF;           ///< start addr of SOF (image-width, image-height, image-format)

    USHORT  imageheight;        ///< image height
    USHORT  imagewidth;         ///< image width
    USHORT  numcomp;            ///< number of components
    USHORT  scanfreq[3][2];     ///< h and v freqs for each of 3 components
    USHORT  qtype[3];           ///< quantization table type for each component
    USHORT  fileformat;         ///< 0->YUV422 ; 1->YUV420 ;
    USHORT  rstinterval;        ///< restart interval (initially set to 0)
    USHORT  lineoffsetY;        ///< Y  line offset after scaling
    USHORT  lineoffsetUV;       ///< UV line offset after scaling
    USHORT  ori_imageheight;    ///< ori-image height before scaling
    USHORT  ori_imagewidth;     ///< ori-image width  before scaling
//#NT#2009/06/10#Corey Ke -begin
//#NT#Support for H.264
    USHORT  uvPack;
//#NT#2009/06/10#Corey Ke -end
    PEXIFHeaderInfo     pExifInfo;

} JPGHEAD_DEC_CFG, *PJPGHEAD_DEC_CFG;

//---- Functions relesse -------------------------------
UINT    JpegDecParseHeader(JPGHEAD_DEC_CFG *, JPG_DEC_TYPE);
//#NT#2007/03/29#KH Chang -begin
//#Unify AVI parser to AVIUtilities
//UINT    JpegDecParseAVIHeader(JPGHEAD_DEC_CFG *);
//#NT#2007/03/29#KH Chang -end
UCHAR   GetCurByte(UCHAR **);
USHORT  GetCurSHORT(UCHAR **, BOOL);
UINT    GetCurUINT(UCHAR **, BOOL);

UINT    Parse_Exif(UCHAR **buf_p, PEXIFHeaderInfo pEXIFHeader);

//---- data relesse ------------------------------------
extern UINT   Exif_Width_Offset, Exif_Height_Offset;
extern UINT   ExifByteOrder, EXIFHeaderSize;
//------------------------------------------------------
/**
@name JPEG decoder error messages.
*/
//@{
#define ERROR_JPGD_MARKER       0xFFFF      ///< invalid or reserved marker
#define ERROR_JPGD_SOI_NF       0xD8FF      ///< SOI marker not found
#define ERROR_JPGD_DQT_LEN      0xDB00      ///< invalid DQT segment length
#define ERROR_JPGD_DQT_TYPE     0xDB01      ///< unknown quant table type
#define ERROR_JPGD_SOF_NBL      0xC1CF      ///< encoding mode is not baseline
#define ERROR_JPGD_SOF_P        0xC004      ///< sample precision != 8
#define ERROR_JPGD_SOF_NC       0xC005      ///< num components/frame != 3
#define ERROR_JPGD_SOF_SFY      0xC005      ///< MCU format != YUV420/422
#define ERROR_JPGD_DHT_LEN      0xC400      ///< invalid DHT segment length
#define ERROR_JPGD_DHT_TYPE     0xC401      ///< unknown huffman table type
#define ERROR_JPGD_SOS_NF       0xDAFF      ///< SOS marker not found
#define ERROR_JPGD_RST_NF       0xD0FF      ///< RST marker not found
#define ERROR_JPGD_EOI_NF       0xD9FF      ///< EOI marker not found

#define ERROR_JPGD_APP1         0xE1FF      ///< EOI marker not found
#define PARSE_JPGD_OK           0x0000      ///< JPEG decode OK

#define ERROR_JPGD_AVI_NF       0x5249      ///< AVI marker not found
//@}

/*
#define EXIF_HEADER_BUFFER_LEN      0xFFFF
#define     _MeteringMode               0x9207
#define     _Flash                      0x9209
#define     _ColorSpace                 0xa001
#define     _ExifImageWidth             0xa002
#define     _ExifImageHeight            0xa003
#define     _SensingMethod              0xa217
#define     _CustomRendered             0xa401
#define     _FileSource                 0xa300
#define     _ExpoureMode                0xa402
#define     _WhiteBalance               0xa403
#define     _SceneCaptureType           0xa406
#define     _ExposureProgram            0x8822
#define     _ISOSpeedRatings            0x8827
#define     _LightSource                0x9208
#define     _SceneType                  0xa301
#define     _GainControl                0xa407
#define     _Contrast                   0xa408
#define     _Saturation                 0xa409
#define     _Sharpness                  0xa40a
#define     _ThumbOffset                0x0201
#define     _ThumbSize                  0x0202
#define     _ExposureTime               0x829a
#define     _Fnumber                    0x829d
#define     _FocalLength                0x920a
#define     _FocalPlaneXResolution      0xa20e
#define     _FocalPlaneYResolution      0xa20f
#define     _FocalPlaneResolution       0xa210
#define     _DigialZoomRation           0xa404
#define     _ExposureBiasValue          0x9204
#define     _MaxApertureVAlue           0x9205
#define     _CompressedBitsPerPixel     0x9102
#define     _DateTimeOriginal           0x9003
#define     _Orientation                0x0112

//#define   Tag_Offset(a)   (((UINT)&(a))-((UINT)&(ExifHead.Order)))
#define Tag_USHORT(a)   (((USHORT)*((UCHAR *)(a))) | ((USHORT)*((UCHAR *)(a + 1)) << 8))
#define Tag_UINT(a)     (((UINT)*((UCHAR *)(a))) | ((UINT)*((UCHAR *)(a + 1)) << 8) | ((UINT)*((UCHAR *)(a + 2)) << 16) | ((UINT)*((UCHAR *)(a + 3)) << 24))
//#define   COUNT(a, b)     ((((UINT)&(a))-((UINT)&(b)))/sizeof(FIELD))

// Error definitions for Exif parsing --- philex added, 2003/9/29
#define E_EXIF_OK       (0)
#define E_EXIF_FAIL     (-90)       // Exif header parsing error
#define E_EXIF_NoApp1   (-91)       // No APP1 marker in jpeg file header
#define E_EXIF_NoTags   (-92)       // No fully tags in Exif header
#define E_EXIF_Larger   (-93)       // APP1 maker is longer than 64K
*/

//@}

#endif // _JPEGPARSEHEADER_H
