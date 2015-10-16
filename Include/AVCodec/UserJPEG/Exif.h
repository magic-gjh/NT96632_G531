/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       Exif.h
    @ingroup    mIAVJPEG

    @brief      JPEG EXIF(DCF2.2) header file.

*/

#ifndef _EXIF_H
#define _EXIF_H

#include "Type.h"

/** \addtogroup mIAVJPEG
*/
//@{

//#define _Support_QV5_AVI_   1           //If you want to support AVI format for Apple QuicklyView 5.0, set this 1

#define    HiWord(data)     (((int)data) & 0xffff0000)>>16              //Bug inside, (USHORT)HiWord(-20) is not 0xFFFF
#define    LoWord(data)     (((int)data) & 0x0000ffff)
#define    HiByte(data)     (((int)data) & 0x0000ff00) >> 8
#define    LoByte(data)     (((int)data) & 0x000000ff)

#define    EXIF_DATE_STR    0x01
#define    DPOF_DATE_STR    0x02

//----- JPEG Marker (inverse) -----------
#define JPEGMarkerSOI    0xD8FF
#define JPEGMarkerAPP1   0xE1FF

#define JPEGMarkerDHT    0xC4FF
#define JPEGMarkerDQT    0xDBFF
#define JPEGMarkerSOF    0xC0FF
#define JPEGMarkerSOS    0xDAFF
#define JPEGMarkerCOM    0xFEFF
#define JPEGMarkerEOI    0xD9FF

#define ByteOrderLittleEndian    0x4949
#define ByteOrderBigEndian       0x4D4D
#define TIFFIdentify             42
//-----------------------------------------
//      TIFF IFD Type
//-----------------------------------------
#define TypeBYTE                 1
#define TypeASCII                2
#define TypeSHORT                3
#define TypeLONG                 4
#define TypeRATIONAL             5
#define TypeUNDEFINED            7
#define TypeSLONG                9
#define TypeSRATIONAL            10

//-----------------------------------------
//      TIFF IFD Tag Numbers
//-----------------------------------------
//#NT#2009/03/18#Photon Lin -begin
//#GPS feature
#define _Support_GPSInfo_Tag    0
//#NT#2009/03/18#Photon Lin -end

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
    //#NT#2009/11/17#SA4(Hideo Lin) -begin
    //#NT#Added for social network value
    #define TagNumsOf0thIFD          13//12                 //Add GPS IFD pointer.
#else
    #define TagNumsOf0thIFD          12//11
    //#NT#2009/11/17#SA4(Hideo Lin) -end
#endif

#ifdef _Support_Brightness_Tag
//#NT#2010/09/16#Hideo Lin -begin
//#NT#Add User Comment Tag
    #define TagNumsOfExifIFD         31//30
#else
    #define TagNumsOfExifIFD         30//29
//#NT#2010/09/16#Hideo Lin -end
#endif

#define TagNumsOfGPSIFD          5                      //Change this value if you suppport more.

#define TagNumsOfInterOpIFD      2
#define TagNumsOf1thIFD          6

//-----------------------------------------
//      TIFF IFD Tag ID
//-----------------------------------------
//----- 0th IFD -----------------------
#define TagImageDescription       0x010E
#define TagMake                   0x010F
#define TagModel                  0x0110
#define TagOrientation            0x0112
#define TagXResolution            0x011A
#define TagYResolution            0x011B
#define TagResolutionUnit         0x0128
#define TagSoftVer                0x0131
#define TagDataTime               0x0132
#define TagYCbCrPositioning       0x0213
//#NT#2009/11/17#SA4(Hideo Lin) -begin
//#NT#Added for social network value (temporary)
#define TagSocialNetwork          0xEFEE
//#NT#2009/11/17#SA4(Hideo Lin) -end

#define TagGPSIFDPtr              0x8825

#define TagExifIFDPtr             0x8769

//----- Exif IFD -----------------------
// If you want to to add/delete tags into ExifSubIFD,
// You should also modify TagNumsOfExifIFD value.
#define TagExposureTime           0x829A
#define TagFNumber                0x829D
#define TagExposureProgram        0x8822
#define TagISOSpeedRatings        0x8827
#define TagExifVersion            0x9000
#define TagDateTimeOriginal       0x9003
#define TagDateTimeDigitized      0x9004
#define TagComponentsCfg          0x9101
#define TagShutterSpeed           0x9201
#define TagAperture               0x9202
#define TagBrightness             0x9203
#define TagExposureBias           0x9204
#define TagMaxAperture            0x9205
#define TagMeteringMode           0x9207
#define TagLightSource            0x9208
#define TagStrobeFlash            0x9209
#define TagFocalLength            0x920A
//#NT#2010/09/16#Hideo Lin -begin
//#NT#Add User Comment tag
#define TagUserComment            0x9286
//#NT#2010/09/16#Hideo Lin -end
#define TagMakerNote              0x927C
#define TagFlashPixVersion        0xA000
#define TagColorspace             0xA001
#define TagPixelXDimension        0xA002
#define TagPixelYDimension        0xA003
#define TagInterOperateIFDPtr     0xA005
#define TagSensingMethod          0xA217
#define TagFileSource             0xA300
#define TagCustomRendered         0xA401
#define TagExposureMode           0xA402
#define TagWhiteBalance           0xA403
#define TagSceneCaptureType       0xA406
//#define TagGainControl        0xA407
//#define TagContrast           0xA408
//#define TagSaturation         0xA409
//#NT#2009/09/24#SA4 -begin
//#NT#2007/09/05#Scottie -begin
//#Add EXIF-Sharpness tag
#define TagSharpness              0xA40A
//#NT#2007/09/05#Scottie -end
//#NT#2009/09/24#SA4 -end


//----- GPS IFD  -----
// Until 2006/12, We still do NOT have GPS module. This information is defined for future used.
// If you required more information, please refer to EXIF Spec.
#define TagGPSVersionID           0x0000
#define TagGPSLatitudeRef         0x0001
#define TagGPSLatitude            0x0002
#define TagGPSLongitudeRef        0x0003
#define TagGPSLongitude           0x0004

//----- InterOperability IFD ----------
#define TagInterOpIndex            0x0001
#define TagExifR98Ver              0x0002
//----- 1th IFD -------------------------------
#define TagCompression                     0x0103
#define TagJPEGInterchangeFormat           0x0201
#define TagJPEGInterchangeFormatLength     0x0202


//-----------------------------------------
//      TIFF IFD Tag Length
//-----------------------------------------
//----- 0th IFD -----------------------
#define TagImgDescLen             16
#define TagMakeLen                32
#define TagModelLen               16
#define TagOrientLen              1
#define TagXResolLen              1
#define TagYResolLen              1
#define TagResolUnitLen           1
#define TagSoftVerLen             20
#define TagDataTimeLen            20
#define TagYCbCrPosLen            1
#define TagGPSPtrLen              1
#define TagExifPtrLen             1
//#NT#2009/11/17#SA4(Hideo Lin) -begin
//#NT#Added for social network value
#define TagSocialNetLen           1
//#NT#2009/11/17#SA4(Hideo Lin) -end
//----- Exif IFD -----------------------
#define TagExpTimeLen             1
#define TagFNumberLen             1
#define TagExpProgLen             1
#define TagISOSpeedLen            1
#define TagExifVerLen             4
#define TagDatTimOriLen           20
#define TagDatTimDigLen           20
#define TagCompCfgLen             4
#define TagShutterLen             1
#define TagApertureLen            1
#define TagBrightnessLen          1
#define TagExpoBiasLen            1
#define TagMaxApertureLen         1
#define TagMeteringLen            1
#define TagLightSrcLen            1
#define TagStrobeLen              1
#define TagFocalLenLen            1

// Until 2006/12, We still do NOT have GPS module. This information is defined for future used.
#define TagGPSVersionLen          4
#define TagGPSLatitudeRefLen      2
#define TagGPSLatitudeLen         3
#define TagGPSLongitudeRefLen     TagGPSLatitudeRefLen
#define TagGPSLongitudeLen        TagGPSLatitudeLen

//#NT#2010/09/16#Hideo Lin -begin
//#NT#Add User Comment tag
#define TagUserCommentLen         32
#define TagUserCommentSNIDLen     7     // length of Social Network identifier in User Comment (7 bytes)
//#NT#2010/09/16#Hideo Lin -end

///////////////////////////////////////////////////////////////////////////
//we reserve 128 bytes for normal data (only 64 bytes in real),
//4096 for debug message.
#define MakeNoteLen_DEBUGBUFSIZE  1024
#define MakeNoteLen_Real          64

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
    //Since GPS IFD is 4n+2... To make the whole EXIF is 4n, we use 62 byte for Uninit area.
    #define MakeNoteLen_UnInit        62
#else
    #define MakeNoteLen_UnInit        64
#endif

#define TagMakeNoteLen            MakeNoteLen_Real+MakeNoteLen_UnInit+MakeNoteLen_DEBUGBUFSIZE

#define TagFlashVerLen            4
#define TagColorSpcLen            1
#define TagPXDimenLen             1
#define TagPYDimenLen             1
#define TagOpIFDPtrLen            1
#define TagSensingLen             1
#define TagFileSrcLen             1
#define TagCMRenderLen            1
#define TagExpModeLen             1
#define TagWhiteBNLen             1
#define TagSNECaptLen             1
//#NT#2009/09/24#SA4 -begin
//#NT#2007/09/05#Scottie -begin
//#Add EXIF-Sharpness tag
#define TagSharpLen               1
//#NT#2007/09/05#Scottie -end
//#NT#2009/09/24#SA4 -end


//----- InterOperability IFD ----------
#define TagInterOpIndexLen        4
#define TagExifR98VerLen          4
//----- 1th IFD -----------------------
#define TagCompressionLen         1
#define TagJPEGInterLen           1
#define TagJPEGLengthLen          1

//-----------------------------------------
// Exif file header data structures
//-----------------------------------------
typedef struct {
    USHORT    Tag;
    USHORT    Type;
    USHORT    Count_LoWord;
    USHORT    Count_HiWord;
    USHORT    Offset_LoWord;
    USHORT    Offset_HiWord;
} FIELD;

typedef struct {
    USHORT    NumeratorLoWord;
    USHORT    NumeratorHiWord;
    USHORT    DenominatorLoWord;
    USHORT    DenominatorHiWord;
} RATIONAL;

typedef struct {
    SHORT    SNumeratorLoWord;
    SHORT    SNumeratorHiWord;
    SHORT    SDenominatorLoWord;
    SHORT    SDenominatorHiWord;
} SRATIONAL;

typedef struct {
    USHORT    IFDLoWord;
    USHORT    IFDHiWord;
} NextIFD;

typedef struct {
    UCHAR     year[4];
    UCHAR     colon1;        // :
    UCHAR     month[2];
    UCHAR     colon2;        // :
    UCHAR     day[2];
    UCHAR     space;
    UCHAR     hour[2];
    UCHAR     colon3;        // :
    UCHAR     minute[2];
    UCHAR     colon4;        // :
    UCHAR     second[2];
    UCHAR     endmark;
} RTCStruct;



typedef struct {
    USHORT   TagNumbers;
    FIELD    ImageDescription;
    FIELD    Maker;
    FIELD    Model;
    FIELD    Orientation;
    FIELD    XResolution;
    FIELD    YResolution;
    FIELD    ResolutionUnit;
    FIELD    SoftVersion;
    FIELD    DateTime;
    FIELD    YCbCrPositioning;
    //#NT#2009/11/17#SA4(Hideo Lin) -begin
    //#NT#Added for social network value
    FIELD    SocialNetwork;
    //#NT#2009/11/17#SA4(Hideo Lin) -end
    FIELD    ExifIFDPtr;

    // Until 2006/12, We still do NOT have GPS module. This information is defined for future used.
//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
    FIELD    GPSIFDPtr;
#endif

    NextIFD  NextIFDOffset;
} IFD_0TH;

typedef struct {
    USHORT   TagNumbers;
    FIELD    ExposureTime;             //unsigned rational
    FIELD    FNumber;                  //unsigned rational
    FIELD    ExposureProgram;          //unsigned short
    FIELD    ISOSpeedRatings;          //unsigned short
    FIELD    ExifVersion;              //undefined 4 units: 48 50 50 48
    FIELD    DateTimeOri;              //ascii string
    FIELD    DateTimeDig;              //ascii string
    FIELD    ComponentCfg;             //undefined 4 units: 01 02 03 00
    FIELD    ShutterSpeed;             //signed rational
    FIELD    Aperture;                 //unsigned rational

    #ifdef _Support_Brightness_Tag
    FIELD    Brightness;               //signed ration.
    #endif

    FIELD    ExposureBias;             //signed rational
    FIELD    MaxAperture;              //unsigned rational
    FIELD    MeteringMode;             //unsigned short
    FIELD    LightSource;              //unsigned short
    FIELD    StrobeFlash;              //unsigned short
    FIELD    FocalLength;              //unsigned rational
//#NT#2010/09/16#Hideo Lin -begin
//#NT#Add User Comment tag
    FIELD    UserComment;              //undefined
//#NT#2010/09/16#Hideo Lin -end
    FIELD    MakerNote;                //undefined
    FIELD    FlashPixVersion;          //undefined: 48 49 48 48
    FIELD    ColorSpace;               //unsigned short
    FIELD    PixelXDimemsion;          //unsigned short
    FIELD    PixelYDimemsion;          //unsigned short
    FIELD    InterOperateIFDPtr;          //unsigned long
    FIELD    SensingMethod;            //unsigned short
    FIELD    FileSource;               //undefined. 3 (DSC)
    FIELD    CustomRenderedMode;       //unsigned short
    FIELD    ExposureMode;             //unsigned short
    FIELD    WhiteBalance;             //unsigned short
    FIELD    SceneCaptureType;         //unsigned short
//#NT#2009/09/24#SA4 -begin
//#NT#2007/09/05#Scottie -begin
//#Add EXIF-Sharpness tag
    FIELD    Sharpness;                //unsigned short
//#NT#2007/09/05#Scottie -end
//#NT#2009/09/24#SA4 -end
    NextIFD  NextIFDOffset;
} IFD_EXIF;

typedef struct {
    USHORT   TagNumbers;
    FIELD    InterOpIndex;
    FIELD    ExifR98Ver;
    NextIFD  NextIFDOffset;
} IFD_INTEROP;

typedef struct {
    USHORT   TagNumbers;
    FIELD    Compression;
    FIELD    XResolution;
    FIELD    YResolution;
    FIELD    ResolutionUnit;
    FIELD    JPEGInterchangeFormat;
    FIELD    JPEGInterchangeFormatLength;
    NextIFD  NextIFDOffset;
} IFD_1TH;

/**
    IFD0 for Primary image, and IFD1 for thumbnail image.
    The size of EXIF_HEAD is 0x34C (if TagMakeNoteLen is 128).
*/
typedef struct {
    USHORT     SOI;
    USHORT     AppMarker1;
    UCHAR      AppMarker1SizeH;
    UCHAR      AppMarker1SizeL;
    CHAR       ExifID[6];
    // TIFF header
    USHORT     TiffOrder;
    USHORT     TiffID;
    NextIFD    NextIFD0th;
    // 0th IFD
    IFD_0TH    IFD0th;
    // Value longer than 4bytes of 0th IFD
    CHAR       ImageDescriptionString[TagImgDescLen];
    CHAR       MakerString[TagMakeLen];
    CHAR       ModelString[TagModelLen];
    CHAR       SoftwareVer[TagSoftVerLen];
    RTCStruct  DateTime;
    RATIONAL   XResolValue;
    RATIONAL   YResolValue;
    // Exif IFD
    IFD_EXIF   IFDexif;
    // Value longer than 4bytes of exif IFD
    RATIONAL   ExposureTimeValue;
    RATIONAL   FNumberValue;

    RTCStruct  OriDateTime;
    RTCStruct  DigDateTime;

    SRATIONAL  ShutterSpeed;
    RATIONAL   ApertureValue;

    #ifdef _Support_Brightness_Tag
    SRATIONAL  BrightnessValue;
    #endif

    SRATIONAL  ExposureBias;
    RATIONAL   MaxApertureValue;
    RATIONAL   FocalLengthValue;

    //#NT#2010/09/16#Hideo Lin -begin
    //#NT#Add User Comment tag
    CHAR       UserCommentString[TagUserCommentLen];
    //#NT#2010/09/16#Hideo Lin -end

    //*****************************************************************************************************
    // Since Debug message will be put into TagMakeNoteLen, It is large size, however, we can't
    // allocate too many space in header. We should remember the difference in physical and logical.
    //*****************************************************************************************************
    CHAR       MakeNoteString[MakeNoteLen_Real];
} EXIF_HEAD, *PEXIF_HEAD;


//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
/**
    GPS tag information.
*/
typedef struct {

    USHORT   TagNumbers;
    FIELD    GPSVersion;
    FIELD    GPSLatitudeRef;
    FIELD    GPSLatitude;
    FIELD    GPSLongitudeRef;
    FIELD    GPSLongitude;
    NextIFD  NextIFDOffset;

}GPS_IFD, *PGPS_IFD;
#endif


/**
    EXIF header information.

    @note
        In order to add the second thumbnail image inside MakeNoteString, we should separate
        the original EXIF HEAD structure. The data member in structure EXIF_HEAD which is after
        MakeNoteString should be moved to this new structure. Since we will insert an "UNFIXED"
        JPG thumbnail image inside MakeNoteString.
        Why do we insert an "UNFIXED" JPG thumbnail image? If we allocate too small space, it can't save
        the data. If we allocate too large space, it will waste Memory card space.
*/
typedef struct {

    //#NT#2009/03/18#Photon Lin -begin
    //#Do not use #ifdef
    #if _Support_GPSInfo_Tag
    //#NT#2009/03/18#Photon Lin -end
    GPS_IFD        GPSIFD;

    RATIONAL       LatitudeDegVal;
    RATIONAL       LatitudeMinVal;
    RATIONAL       LatitudeSecVal;
    RATIONAL       LongitudeDegVal;
    RATIONAL       LongitudeMinVal;
    RATIONAL       LongitudeSecVal;
    #endif


    // InterOperability IFD
    IFD_INTEROP    IFDInterOperability;
    // 1th IFD
    IFD_1TH        IFD1th;
    // Value longer than 4bytes of 1th IFD
    RATIONAL       XResolValue1th;
    RATIONAL       YResolValue1th;
    // (SOI) Thumbnail
} EXIF_HEAD_AFTERMAKERNOTE, *PEXIF_HEAD_AFTERMAKERNOTE;

//----- Exposure Program ---------------------------
#define ExpPrgNoDef            0
#define ExpPrgManual           1
#define ExpPrgNormal           2
#define ExpPrgAperture         3    // Aperture Priority
#define ExpPrgShutter          4    // Shutter Priority
#define ExpPrgCreative         5
#define ExpPrgAction           6
#define ExpPrgPortrait         7
#define ExpPrgLandscape        8
//----- ISO Speed -----------------------------------
#define ISOSpeed50             50
#define ISOSpeed100            100
#define ISOSpeed200            200
#define ISOSpeed400            400
//----- Metering Mode -------------------------------
#define MeterUnknown           0
#define MeterAverage           1
#define MeterCenterWeightedAvg 2
#define MeterSpot              3
#define MeterMultiSpot         4
#define MeterPattern           5
#define MeterPartial           6
//----- Light Source -------------------------------
#define LightUnknown           0
#define LightDaylight          1
#define LightFluorescent       2
#define LightTungsten          3
#define LightStandardA         17
#define LightStandardB         18
#define LightStandardC         19
//----- Strobe Flash -------------------------------
#define StrobeNoFired          0
#define StrobeFired            1
//#NT#2009/09/24#SA4 -begin
#define StrobeFlashAuto             ((1<<4)+(1<<3))
#define StrobeFlashForceOFF         ((1<<4))
#define StrobeFlashForceON          ((1<<3))
#define StrobeFlashAutoRedEye       ((1<<6)+(1<<4)+(1<<3))
#define StrobeFlashForceONRedEye    ((1<<6)+(1<<3))
#define StrobeFlashNightStrobe      ((1<<6)+(1<<4)+(1<<3))
//#NT#2009/09/24#SA4 -end
//----- Color Space --------------------------------
#define ColorSpaceSRGB         1
//----- Sensing Mode -------------------------------
#define SensingNoDef           1
#define SensingOneChip         2    // OneChipColorArea sensor
#define SensingTwoChip         3
#define SensingThreeChip       4
#define SensingColorSeqArea    5
#define SensingTriLinear       7
#define SensingColorSeqLinear  8
//----- CustomRendered ----------------------------
#define NormalRendered         0
#define CustomRendered         1
//-----Exposure Mode-------------------------------
#define AutoExposure           0
#define ManualExposure         1
#define AutoBracket            2
//-----White Balance ------------------------------
#define AutoWhite              0
#define ManualWhite            1

//-----SceneCapture Type --------------------------
#define StandardScene          0
#define LandScapeScene         1
#define PortraitScene          2
#define NightScene             3
//#NT#2009/09/24#SA4 -begin
//#NT#2007/09/05#Scottie -begin
//#Add EXIF-Sharpness tag
//-----Sharpness-----------------------------------
#define NORMAL_SHARPNESS       0
#define SOFT_SHARPNESS         1
#define HARD_SHARPNESS         2
//#NT#2007/09/05#Scottie -end
//#NT#2009/09/24#SA4 -end

//----- Compression -------------------------------
#define UnCompressed           1
#define JPEGCompression        6

//--------------------------------------------------------------
//#define Commentdatalength        17

typedef __packed struct {
    UCHAR    TblIndex;
    UCHAR    symbol_nums[16];
    UCHAR    symbol_data[12];
} HuffmanTblDC;

typedef __packed struct {
    UCHAR    TblIndex;
    UCHAR    symbol_nums[16];
    UCHAR    symbol_data[162];
} HuffmanTblAC;

typedef __packed struct {
    UCHAR    TblIndex;
    UCHAR    q_data[64];
} QuantTbl;

typedef __packed struct {
    UCHAR    ComponentID;
    UCHAR    SampleHVRate;        // Horizontal & Vertical rate
    UCHAR    ComponentQTblIndex;    // Q-table index
} SOF_COMPINFO;

typedef __packed struct {
    UCHAR    ComponentID;
    UCHAR    ComponentHufTblIndex;    // AC & DC Huffman table index
} SOS_COMPINFO;

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    HuffmanTblDC    HuffDC_0th;        // 00->0th DC Huffman table (Y)
    HuffmanTblAC    HuffAC_0th;        // 10->0th AC Huffman table (Y)
    HuffmanTblDC    HuffDC_1th;        // 01->1th DC Huffman table (UV)
    HuffmanTblAC    HuffAC_1th;        // 11->1th AC Huffman table (UV)
} Tag_DHT;    // Huffman tables

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    QuantTbl        Qtbl_0th;        // 00: 0th Q-table
    QuantTbl        Qtbl_1th;        // 01: 1th Q-table
} Tag_DQT;    // Quantization tables

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           BitsPerPixel;
    UCHAR           ImgHeight[2];
    UCHAR           ImgWidth[2];
    UCHAR           ComponentNums;
    SOF_COMPINFO    Component1;
    SOF_COMPINFO    Component2;
    SOF_COMPINFO    Component3;
} Tag_SOF;    // Start of Frame

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           ComponentNums;
    SOS_COMPINFO    Component1;
    SOS_COMPINFO    Component2;
    SOS_COMPINFO    Component3;
    UCHAR           OtherData[3];
} Tag_SOS;    // Start of Scan

typedef __packed struct {
    USHORT        SOI;
    Tag_DQT       MarkDQT;
    Tag_SOF       MarkSOF;
    Tag_DHT       MarkDHT;
    Tag_SOS       MarkSOS;
} JPG_HEADER, *PJPG_HEADER;

//#NT#2008/11/11#Daniel Lu - begin
//#Add for supporting restart interval header and JPEG gray format header
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           RestartNums[2];
} Tag_DRI;    // Distance of restart interval

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           BitsPerPixel;
    UCHAR           ImgHeight[2];
    UCHAR           ImgWidth[2];
    UCHAR           ComponentNums;
    SOF_COMPINFO    Component1;
} Tag_SOF_GRAY;    // Start of Frame for gray format

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           ComponentNums;
    SOS_COMPINFO    Component1;
    UCHAR           OtherData[3];
} Tag_SOS_GRAY;    // Start of Scan for gray format

typedef __packed struct {
    USHORT        SOI;
    Tag_DQT       MarkDQT;
    Tag_SOF       MarkSOFGray;
    Tag_DHT       MarkDHT;
    Tag_SOS       MarkSOSGray;
} JPG_HEADER_GRAY, *PJPG_HEADER_GRAY;
//#NT#2008/11/11#Daniel Lu - begin

//For QV 5.0 AVI
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           Comment[14];
} Tag_CMAVI;    // Start of Scan

/**
    Quicktime version 5 AVI header.
*/
typedef __packed struct {
    USHORT        SOI;
    Tag_CMAVI     MarkCME0;
    Tag_DQT       MarkDQT;
    Tag_SOF       MarkSOF;
    Tag_SOS       MarkSOS;
} QV50AVI_HEADER, *PQV50AVI_HEADER;

//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
#define  QV5MOVMJPGHDRSIZE    0x299
                          //FFD8+ FFE0+ FFFE+ FFE1+ FFDB+ FFC4 + FFC0 +FFDA
                          //0x02+ 0x12+ 0x0E+ 0x2C+ 0x86+ 0x1A4+ 0x13 +0x0E

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           Comment[10];
} Tag_CMFE;    // Start of Scan
//For QV 5.0 AVI
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           Comment[8];
    UINT32           FieldSize;
    UINT32           PaddedSize;
    UINT32           Offset2Next;
    UCHAR           Offset1[4];
    UCHAR           Offset2[4];
    UCHAR           Offset3[4];
    UCHAR           Offset4[4];
    UCHAR           Offset5[4];

} Tag_CME1;    // Start of Scan


/**
    Quicktime version 5 AVI header.
*/
typedef __packed struct {
    USHORT        SOI;
    Tag_CMAVI     MarkCME0;
    Tag_CMFE      MarkCMFE;
    Tag_CME1      MarkCME1;
    Tag_DQT       MarkDQT;
    Tag_DHT       MarkDHT;
    Tag_SOF       MarkSOF;
    Tag_SOS       MarkSOS;
} MOVMJPG_HEADER, *PMOVMJPG_HEADER;

//#NT#2007/10/17#Meg Lin -end


//@}

#endif    // _EXIF_H
