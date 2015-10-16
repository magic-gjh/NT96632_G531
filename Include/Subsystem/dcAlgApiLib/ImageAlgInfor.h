#ifndef _IMAGEALGINFOR_H
#define _IMAGEALGINFOR_H

//----------------------------------------------------------------------------------------------------
//************************************************************************
// File Name    : ImageAlgInfor.h
// Description  : Define the image quality selection items
//************************************************************************
#include "type.h"
//#NT#2009/11/04#Yuppie Lin -begin
//#NT# add for FaceDetection
#include "fde_lib.h"
//#NT#2009/11/04#Yuppie Lin -end
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
#include "dis_lib.h"
#include "JpgEnc.h"
#include "Alg_IPLMode.h"
//#NT#2009/11/27#Connie Yu -end

//----------Header file include-----------------------------------------------------------------------
//#include "MenuSystem.h"
//----------Macro Definition--------------------------------------------------------------------------

#define MAX_FB_W            640
#define MAX_FB_H            576


#define NT96xxxSimTool 0

//********AlgCallback Function********
//MessageID define
#define ALGMSG_PREVIEW              (1)     //The imagepipeline prepare entering preview mode, not in preview yet
#define ALGMSG_FOCUS                (1<<1)  //Prepare to enter auto focus process
#define ALGMSG_FOCUSEND             (1<<2)  //Auto focus process end
#define ALGMSG_PREFLASH             (1<<3)  //Prepare to preflash
#define ALGMSG_PREFLASHEND          (1<<4)  //Preflash end.
#define ALGMSG_FIRSTRAW             (1<<5)  //Prepare to start capture raw data
#define ALGMSG_JPEGOK               (1<<6)  //Jpeg file in dram is ready, quick view data is ready
#define ALGMSG_VIDEO_RECORDING      (1<<7)  //Prepare to enter ime 2 path out, one for display and the other for movie recording
#define ALGMSG_PREVIEWSTABLE        (1<<8)  //The imagepipeline is running in preview mode
#define ALGMSG_CHGDZOOMSTEP         (1<<9)  //Digital zoom has changed in one step, UI can update the zoom bar
#define ALGMSG_JPGEXIFMODIFY        (1<<10) //ImageInfor.c has write some basic exif data, the system F/W can write the needed exif data at this moment
#define ALGMSG_LENSJAM              (1<<11) //The Zoom lens is in a jam

#define ALGMSG_SUSKEY               (1<<12) //The imagepipeline prepare to enter raw capture mode, please the system F/W should block any key---switch mode/zoom
                                            //s1/s2/....
#define ALGMSG_SUSPHOTO             (1<<13) //The imagepipeline prepare to enter raw capture mode, photo task should not do any operation till next preview mode
#define ALGMSG_SUSFLOWPRV           (1<<14) //The imagepipeline prepare to enter raw capture mode, please the system F/W should block any key---switch mode/zoom
                                            //s1/s2/....,any opertion.
#define ALGMSG_RSMKEY               (1<<15) //When back to PreviewIdle mode, the system F/W return normal operation
#define ALGMSG_RSMPHOTO             (1<<16) //When back to PreviewIdle mode, the system F/W return normal operation
#define ALGMSG_RSMFLOWPRV           (1<<17) //When back to PreviewIdle mode, the system F/W return normal operation
#define ALGMSG_CHGOZOOMSTEP         (1<<18) //Optical zoom has changed in one step, UI can update the zoom bar
#define ALGMSG_CAPFSTOK             (1<<19) //Capture FST Write OK
#define ALGMSG_FDEND                (1<<20) //Face-Detection process end
#define ALGMSG_QVSTART              (1<<21) //Quick view data is ready.
#define ALGMSG_SDEND                (1<<22)
//#NT#20091229#YC Peng -begin
#define ALGMSG_SOUND_SHUTTER2       (1<<23)
#define ALGMSG_CAPEND               (1<<24)
//#NT#20091229#YC Peng -end
#define ALGMSG_HANDSHAKE            (1<<25)
//#NT#2010/09/21#ethanlau -begin
//for smartscene detect
#define ALGMSG_SMARTSCENE           (1<<26)
//#NT#2010/09/21#ethanlau -end
//#NT#2010/09/27#ethanlau -begin
//#NT#for cmos sensor PWDN request
#define ALGMSG_PWND                 (1<<27)
//#NT#2010/09/27#ethanlau -end
//********Image Information********
//  * @param out_format 0:444, 1:422 co-sited, 2:422 centered, 3:420 co-sited, 4:420 centered, 5-7:reserved.
#define IMEOUT2_FMT444              0
#define IMEOUT2_FMT422_1            1
#define IMEOUT2_FMT422_2            2
#define IMEOUT2_FMT420_1            3
#define IMEOUT2_FMT420_2            4

//********Alg Menu Information********
#define _IMGSatus_NoChange          0xF0

//********Calibration********
#define CAL_ISO                     0
#define CAL_MSHUTTER                1
#define CAL_DEFECTPIXEL             2
#define CAL_AWB                     3
#define CAL_FLASH                   4
#define CAL_FLASH_AWB               41
#define CAL_FLASH_REDEYE_TEST       42
#define CAL_FLASH_GNO_SHOTS         43
#define CAL_AF_FAR                  5
#define CAL_AF_NEAR                 6
#define CAL_WR2FILE                 8
#define CAL_READSTATUS              9
#define CAL_DNPSTATION              10
#define CAL_LENS                    11
#define CAL_OB                      12
#define CAL_VIG                     13
#define CAL_DEFECTPIXEL_WHITE       14
#define CAL_DEFECTPIXEL_DARK        15
#define CAL_MSHUTTER_LASTING        16

#define CAL_MSHUTTER_SW_TEST        17
#define CAL_APERTURE_SW_TEST        18
#define CAL_FAE_TEST                19
#define CAL_ZOOM_SPEED_TEST         20
#define CAL_ZOOM_STEPMISS_TEST      21
#define CAL_FOCUS_STEPMISS_TEST     22
#define CAL_AF_TEST                 23
#define CAL_STAIN_TEST              24
#define CAL_TVRESOLUTION_TEST       25
#define CAL_ZOOM_STOP2PR            26
#define CAL_FOCUSOFFSET             27
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
#define CAL_SNR                     28
//#NT#2010/12/15#JJ Huang -end


//----------Type Definition--------------------------------------------------------------------------

//********AlgCallback Function********
typedef struct _AlgMsgInfo_{
unsigned char   CapStatus_Mode;     //Alg Operating Mode
unsigned char   Alg_Mode;           //Alg Operating Mode
unsigned char   AF_Status;          //AF Status
unsigned char   AF_Window;
unsigned char   Jpeg_Num;
unsigned int    Jpeg0_Addr;
unsigned short  Jpeg0_Hsize;
unsigned short  Jpeg0_Vsize;
unsigned int    Jpeg1_Addr;
unsigned short  Jpeg1_Hsize;
unsigned short  Jpeg1_Vsize;
unsigned int    Jpeg2_Addr;
unsigned short  Jpeg2_Hsize;
unsigned short  Jpeg2_Vsize;
unsigned short  Thumbnail_Hsize;
unsigned short  Thumbnail_Vsize;
unsigned int    Thumbnail_Format;
unsigned int    Thumbnail0_Y_Addr;
unsigned int    Thumbnail0_Cb_Addr;
unsigned int    Thumbnail0_Cr_Addr;
unsigned int    Thumbnail1_Y_Addr;
unsigned int    Thumbnail1_Cb_Addr;
unsigned int    Thumbnail1_Cr_Addr;
unsigned int    Thumbnail2_Y_Addr;
unsigned int    Thumbnail2_Cb_Addr;
unsigned int    Thumbnail2_Cr_Addr;
unsigned char   DzoomIndex;
unsigned char   OzoomIndex;
unsigned int    JpgExifAddr;
UINT8           BlinkDetectStatus;
UINT8           FaceDetectNum;
//#NT#2009/11/04#Yuppie Lin -begin
//#NT# modify for FaceDetection
FACE            FaceDetectRect[36];
//#NT#2009/11/04#Yuppie Lin -end
UINT16          CapCurrentNum;
//#NT#2009/09/24#SA4 -begin
UINT32          DzoomRealRatio;
//#NT#2009/09/24#SA4 -end
//#NT#2010/09/21#ethanlau -begin
//#NT#for smartscene rst
UINT32          SmartSceneRst;
//#NT#2010/09/21#ethanlau -end
} AlgMsgInfo,*pAlgMsgInfo;

typedef void (*FPALGCALLBACK)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
typedef void (*FPMPEG4PutSrcImgIdxCallBack)(void);
void SetupMPEG4CallBackFun(FPMPEG4PutSrcImgIdxCallBack pmp4callback);

//********Sensor Photo Capture App Callback********
typedef struct _SENSOR_APPOBJ{
    void    (*FP_SetFlashCharge)(BOOL b_Chg, BOOL b_ChgC);   //Set Flash charge EX:void gpio_set_flash_charge(BOOL b_Chg, BOOL b_ChgC)
    BOOL    (*FP_CheckFlashReady)(void);        //Check if Flash charge ready
    UINT32  (*FP_CheckFlashADCVlaue)(void);     //Check flash ADC value
    void    (*FP_ShowOSDString)(char *date, unsigned short X, unsigned short Y,unsigned short Color);//Show OSD String at (X,Y) with different color
    void    (*FP_DrawOSDRect) (UINT16 X, UINT16 Y, UINT16 SizeX, UINT16 SizeY, UINT16 Color);
    void    (*FP_ClearOSD)(void);               //Clear OSD
    void    (*FP_PowerOffFlow)(BOOL bLensRet, BOOL bNandWrite);// It will be used when the lens is jam in boot sequence  Ex:void PowerOffFlow(BOOL bLensRet, BOOL bNandWrite) ,
    void (*FP_GPIOMap_TurnOnCCDPower)(void);
    void (*FP_GPIOMap_TurnOffCCDPower)(void);
    void (*FP_GPIOMap_CloseShutter)(void);
    void (*FP_GPIOMap_OpenShutter)(void);
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
    BOOL   bSieMClkEn;
//#NT#2009/10/08#Bowen Li -end
} SENSOR_APPOBJ,*PSENSOR_APPOBJ,SensorLibraryNeedInfo,*pSensorLibraryNeedInfo;//SensorLibraryNeedInfo,*pSensorLibraryNeedInfo;

typedef BOOL (*FPS2PRESSDET)(void);
typedef struct _CAPTURE_APPOBJ{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
    FPS2PRESSDET FpS2PressDet;
} CAPTURE_APPOBJ,*PCAPTURE_APPOBJ;//SensorLibraryNeedInfo,*pSensorLibraryNeedInfo;

//********Capture Mode********
typedef enum {
    CAPMODE_PRVIDLE,
    CAPMODE_PRVBUSY
} _CapMode;

//********Alg Operating Mode********
typedef enum {
    MODE_Preview,
    MODE_Captrue_Focusing,
    MODE_Captrue_FocusEnd,
    MODE_Captrue_FirstRaw,
    MODE_Captrue_LastSlice,
    MODE_Captrue_QVSTART,
    MODE_Captrue_JpegOK,
    MODE_Video_Preview,
    MODE_Video_Recording,
    MODE_PCCam
} _Alg_Callback_Mode;

//********AF Status********
typedef enum {
    AFStatus_OK,
    AFStatus_FAIL,
    AFStatus_TOONEAR,
    AFStatus_TOOFAR
} _Alg_Callback_AFStatus;

//********IME Scale and FB Param********
typedef struct _FBADDR_PRAM
{
    unsigned int y0_addr;
    unsigned int cb0_addr;
    unsigned int cr0_addr;

    unsigned int y1_addr;
    unsigned int cb1_addr;
    unsigned int cr1_addr;

    unsigned int y2_addr;
    unsigned int cb2_addr;
    unsigned int cr2_addr;

    unsigned int ime_format;
    unsigned int Ipe_fc;
    unsigned int Ime_fc;
} FBADDR_PRAM;

typedef struct _IME_FBADDR_PRAM
{
    unsigned int y0_addr;
    unsigned int cb0_addr;
    unsigned int cr0_addr;

    unsigned int y1_addr;
    unsigned int cb1_addr;
    unsigned int cr1_addr;

    unsigned int y2_addr;
    unsigned int cb2_addr;
    unsigned int cr2_addr;

    unsigned int y3_addr;
    unsigned int cb3_addr;
    unsigned int cr3_addr;

    unsigned int y4_addr;
    unsigned int cb4_addr;
    unsigned int cr4_addr;

    unsigned int y5_addr;
    unsigned int cb5_addr;
    unsigned int cr5_addr;

    unsigned int y6_addr;
    unsigned int cb6_addr;
    unsigned int cr6_addr;

    unsigned int y7_addr;
    unsigned int cb7_addr;
    unsigned int cr7_addr;

    unsigned int in_h;
    unsigned int in_v;
    unsigned int out_h;
    unsigned int out_v;

    unsigned int in_format;
    unsigned int out_format;

    unsigned int out_lineoffset;
//#NT#2009/06/24#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
    unsigned int ime_sten;
    unsigned int buf_num;
    unsigned int done_buf;
//#NT#2009/06/24#Jarkko Chang -end
    UINT32  in_YCC_pack;
    UINT32  out_YCC_pack;
    UINT32  src;
} IME_FBADDR_PRAM,*pIME_FBADDR_PRAM;

typedef struct _SLICEINFO_PRAM
{
    unsigned int bay_addr;
    unsigned int jpg_addr;
    unsigned int pingpong_addr;
    unsigned int bay_hsize;
    unsigned int bay_vsize;
    unsigned int jpg_hsize;
    unsigned int jpg_vsize;
    unsigned int jpg_filesize;
    unsigned int bay_packbit;
    unsigned int bay_startpix;
    unsigned int CF_addr;

} SLICEINFO_PRAM;


typedef struct _ACTFBADDR_PRAM
{
    unsigned int y_addr;
    unsigned int cb_addr;
    unsigned int cr_addr;
    unsigned int focus_value[16];
    BOOL bUsingDIS;
    UINT32 BorderY_Addr;
    UINT32 BorderU_Addr;
    UINT32 BorderV_Addr;
    UINT32 DisY_Addr;
    UINT32 DisU_Addr;
    UINT32 DisV_Addr;
    UINT32 SrcYLineOffset;
    UINT32 SrcUVLineOffset;
} ACTFBADDR_PRAM;

typedef enum {
    _bay_addr,
    _jpg_addr,
    _pingpong_addr,
    _bay_hsize,
    _bay_vsize,
    _jpg_hsize,
    _jpg_vsize,
    _jpg_filesize,
    _bay_packbit,
    _bay_startpix,    //20060510 YH add
    _EdgExtKernel5,
    _EdgExtKernel4,
    _EdgExtKernel3,
    _EdgExtKernel2v,
    _EdgExtKernel1v,
    _EdgExtKernel0v,
    _EdgExtKernel2h,
    _EdgExtKernel1h,
    _EdgExtKernel0h,
    _EdgExtDiv,
    _EdgEnhance,
    _EMapa,
    _EMapb,
    _EMapc,
    _EMapd,
    _EMapn,
    _NR_RBlpfw,
    _NR_Glpfw,
    _NR_RBw,
    _NR_Gw,
    _NRMap_nsvsb,
    _NRMap_nsdiv,
    _NRMap_hmedonly
 } _Slice_Item_sel;

#define Max_SliceItem _NRMap_hmedonly

//********IQ related********
typedef struct    t_ColorGainInfo{
    char    R_Adj;
    char    G_Adj;
    char    B_Adj;
} ColorGainInfo, *PColorGainInfo;

//********AAA********
typedef struct _FOCUS_WINDOW_INFOR{
    unsigned long    stx;///<VA starting x
    unsigned long    sty;///<VA starting y
    unsigned long    hsz;///<VA window h_size
    unsigned long    vsz;///<VA window v_size
}FOCUS_WINDOW_INFOR;

//********Calibration********
typedef void (*FN_CalShowString)(char *pStr, RECT *pRect, BOOL ClearFirst);
typedef struct{
    FN_CalShowString   FnShowString;
    UINT32        MemoryAddr;
    UINT32        MemorySize;

}ALG_CAL_PARAM, * PALG_CAL_PARAM;

//********Alg Menu Information********
typedef struct    t_ImageAlgInfo{
    UCHAR           DSCMode;            // Please reference enum:_DSCMode_sel
    UCHAR           SceneMode;          // Please reference enum:_IMGSceneMode_sel
    UCHAR           IMGSize;            // Please reference enum:_IMGSize_sel
    UCHAR           Quality;            // Please reference enum:_IMGQuality_sel
    UCHAR           DriveMode;          // Please reference enum:_IMGDriveMode_sel
    UCHAR           BurstDriveNum;      // Please reference enum:_IMGBurstNum_sel
    UCHAR           Sharpness;          // Please reference enum:_IMGSharpness_sel
    UCHAR           Saturation;         // Please reference enum:_IMGEffect_sel
    UCHAR           DZoomIndex;         // Please reference enum:_IMGDZoomIdx_sel
    UCHAR           FlashMode;          // Please reference enum:_IMGFlashMode_sel
    UCHAR           Frequency;          // Please reference enum:_POWERFreq_sel

    UCHAR           ImageEffect;        // Please reference enum:_IMGEffect_sel
    UCHAR           ContrastAdjust;     // Please reference enum:_IMGEffect_sel
    ColorGainInfo   ColorBalanceAdjust; // Please reference enum:_IMGEffect_sel
    UCHAR           ColorMonoAdjust;    // Please reference enum:_IMGEffect_sel

    UCHAR           WBMode;             // Please reference enum:_IMGWBMode_sel
    UCHAR           WBAdjust;           // Please reference enum:_IMGWBAdjust_sel
    UCHAR           AWBBracketing;      // Please reference enum:_IMGAWBBracketing_sel

    UCHAR           AEEVValue;          // Please reference enum:_IMGEVValue_sel
    UCHAR           AEMode;             // Please reference enum:_IMGAEMode_sel
    UCHAR           AEBracketing;       // Please reference enum:_IMGAEBracketing_sel
    UCHAR           ISO;                // Please reference enum:_IMGISO_sel

    UCHAR           AFMode;             // Please reference enum:_IMGAFMode_sel
    UCHAR           AFWindow;           // Please reference enum:_IMGAFWindow_sel
    UCHAR           AFPosition;         // Please reference enum:_IMGAFPosition_sel
    UCHAR           FileFmt;            // Please reference enum:_IMGFileFmt_sel
    UCHAR           SiePath;            // Please reference enum:_IMGSiePath_sel
    UCHAR           SieBitDepth;        // Please reference enum:_IMGSieBitDepth_sel

    UCHAR           PrePrvBitDepth;     // Please reference enum:_IMGPreBitDepth_sel
    UCHAR           PreCapBitDepth;     // Please reference enum:_IMGPreBitDepth_sel

    UCHAR           FaceDetectionMode;  // Please reference enum:_IMGFaceDetectionMode_sel
    UCHAR           SmileDetectionMode; // Please reference enum:_IMGSmileDetectionMode_sel
    UCHAR           BlinkDetectionMode; // Please reference enum:_IMGBlinkDetectionMode_sel
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    UINT8           DISMode;            ///< 34 Please reference enum:_IMGDISMode_sel
    //#NT#2009/11/27#Connie Yu -end
    UCHAR           ImageRatio;         // Please reference enum:_IMGRatio_sel
    UCHAR           DispRatio;          // Please reference enum:_IMGRatio_sel
    UCHAR           ImageFlip;          // Please reference enum:_IMGFlip_sel
//#NT#2009/09/24#SA4 -begin
//#NT#20090918#ethanlau -begin
    UCHAR           Vid_Resolution; // Please reference enum:_IMGResolution_sel
    UCHAR           Prv_Resolution; // Please reference enum:_IMGResolution_sel
//#NT#20090918#ethanlau -end
//#NT#20091008#ethanlau -begin
    UCHAR           StillAntiShaking;
//#NT#2009/09/24#SA4 -end
    UCHAR    TvInFormat;         //Please reference enum:_IMGTvInFormat_sel
} ImageAlgInfo, *PImageAlgInfo;

typedef struct    t_SceneModeInfo{
    unsigned char    SceneMode;
    unsigned char    FlashMode;
    unsigned char    EVComp;
    unsigned char    Saturation;
    unsigned char    Sharpness;
    unsigned char    WBMode;
    unsigned char    Focus;
    unsigned char    FlashChangeAE;
}  SceneModeInfo;

//********Alg UI Item********
/*UI Item*/
typedef enum {
    _DSCMode,               // Please reference enum:_DSCMode_sel
    _SceneMode,             // Please reference enum:_IMGSceneMode_sel
    _ImgSize,               // Please reference enum:_IMGSize_sel
    _Quality,               // Please reference enum:_IMGQuality_sel
    _DriveMode,             // Please reference enum:_IMGDriveMode_sel
    _BurstNum,              // Please reference enum:_IMGBurstNum_sel
    _Sharpness,             // Please reference enum:_IMGSharpness_sel
    _Saturation,            // Please reference enum:_IMGEffect_sel
    _DZoomIndex,            // Please reference enum:_IMGDZoomIdx_sel
    _FlashMode,             // Please reference enum:_IMGFlashMode_sel
    _Frequency,             // Please reference enum:_POWERFreq_sel
    _ImageEffect,           // Please reference enum:_IMGEffect_sel
    _AEMode,                // Please reference enum:_IMGAEMode_sel
    _AEEV,                  // Please reference enum:_IMGEVValue_sel
    _WBMode,                // Please reference enum:_IMGWBMode_sel
    _AFMode,                // Please reference enum:_IMGAFMode_sel
    _AFWindow,              // Please reference enum:_IMGAFWindow_sel
    _AFPosition,            // Please reference enum:_IMGAFPosition_sel
    _ISOIndex,              // Please reference enum:_IMGISO_sel
//  _VideoSize,             // Please reference enum:_IMGVideoSize_sel
    _FileFmt,               // Please reference enum:_IMGFileFmt_sel
    _SiePath,               // Please reference enum:_IMGSiePath_sel
    _SieBitDepth,           // Please reference enum:_IMGSieBitDepth_sel
    _PrePrvBitDepth,        // Please reference enum:_IMGPreBitDepth_sel
    _PreCapBitDepth,        // Please reference enum:_IMGPreBitDepth_sel
    _FaceDetectionMode,     // Please reference enum:_IMGFaceDetectionMode_sel
    _SmileDetectionMode,    // Please reference enum:_IMGSmileDetectionMode_sel
    _BlinkDetectionMode,    // Please reference enum:_IMGBlinkDetectionMode_sel
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    _DISMode,               // Please reference enum:_IMGDISMode_sel
    //#NT#2009/11/27#Connie Yu -end
    _ImageRatio,            // Please reference enum:_IMGRatio_sel
//#NT#2010/04/29#Jarkko Chang -begin
    _DispRatio,             // Please reference enum:_IMGRatio_sel
//#NT#2010/04/29#Jarkko Chang -end
    _ImageFlip,              // Please reference enum:_IMGFlip_sel
//#NT#2009/09/24#SA4 -begin
    //#NT#20090918#ethanlau -begin
    _Vid_Resolution,    // Please reference enum:_IMGResolution_sel
    _Prv_Resolution,    // Please reference enum:_IMGResolution_sel
    //#NT#20090918#ethanlau -end
    //#NT#20091008#ethanlau -begin
    _StillAntiShaking,       // Please reference enum:_IMGStillAntiShaking
    //#NT#20091008#ethanlau -end
//#NT#2009/09/24#SA4 -end
    _TvInFormat
} _UI_Item_sel;

//#NT#2010/05/28#ethanlau -begin
#define Max_UIItem _TvInFormat

typedef enum{
    ZoomIn,
    ZoomOut,
    Stop
}Direction;
//#NT#2010/05/28#ethanlau -end

//#NT#2009/09/24#SA4 -begin
//#NT#20091008#ethanlau -begin
//****************************************************************************************************
//_Level0 == disable
typedef enum{
    _Level0,
    _Level1,
    _Level2
}_IMGStillAntiShaking;

#define Max_AntiShakeLevel _Level2
//#NT#20091008#ethanlau -end
//****************************************************************************************************
//#NT#20090918#ethanlau -begin
//#NT#2009/10/09#Jarkko Chang -begin
typedef enum{
    _Resolution_320x240,
    _Resolution_352x240,
    _Resolution_640x480,
    _Resolution_720x480,
    _Resolution_1280x720,//dont use
    _Resolution_1280x960,
    _Resolution_1280x480,//dont use
    _Resolution_1440x1080,//dont use
    _Resolution_848x480,
    //#NT#2010/05/28#ethanlau -begin
    _Resolution_640x360,    //prv 16:9
    _Resolution_4352x3267,  //prv
    _Resolution_3264x2448,  //prv
    _Resolution_2592x1944,  //prv
    _Resolution_2048x1536,  //prv
    _Resolution_3648x2048,  //prv 16:9
    _Resolution_1920x1080,  //prv 16:9
    _Resolution_1280x400,  //prv dual sensor 16:5
    _Resolution_1920x720,  //prv dual sensor 8:3
    _Resolution_1600x1200,
    _Resolution_800x600,	
    _Resolution_Max
    //#NT#2010/05/28#ethanlau -end
}_IMGResolution_sel;
//#NT#2009/10/09#Jarkko Chang -end
#define Max_VidPrvSize _Resolution_Max

//#NT#20090918#ethanlau -end
//#NT#2009/09/24#SA4 -end
//****************************************************************************************************
/*DSC mode*/
typedef enum {
    _DSC_Auto,
    _DSC_Manual
//    _DSC_Vdeo,
//    _DSC_Playback
 } _DSCMode_sel;

#define Max_DSCMode _DSC_Manual

//****************************************************************************************************
/*SceneMode*/
typedef enum {
    _SceneMode_Auto,
    _SceneMode_Landscape,
    _SceneMode_Potrait,
    _SceneMode_Sport,
    _SceneMode_NightScene,
    _SceneMode_AV,
    _SceneMode_TV,
    _SceneMode_Manual,
    _SceneMode_NightPotrait,
    _SceneMode_NightLandscape,
    _SceneMode_Party,
    _SceneMode_SunSet,
    _SceneMode_BackLight,
    _SceneMode_Document,
    _SceneMode_Firework,
    _SceneMode_Beach,
    _SceneMode_Candle,
    _SceneMode_Smile,
    _SceneMode_UnderWater,
    _SceneMode_BMode,
    _SceneMode_Program,
    _SceneMode_Concert,
    //#NT#20091103#ethanlau -begin
    //for skylight
    _SceneMode_Smart,
    _SceneMode_MAX
    //#NT#20091103#ethanlau -end
} _IMGSceneMode_sel;

#define Max_SceneMode _SceneMode_MAX

//****************************************************************************************************
/*ImgSize*/
typedef enum {
//#NT#2009/09/24#SA4 -begin
//#NT#2009/08/13#Jarkko Chang -begin
//#NT#add for project using
    _IMGSize_16M,
    _IMGSize_14M,
//#NT#2009/08/13#Jarkko Chang -end
//#NT#2009/09/24#SA4 -end
    _IMGSize_12M,
    _IMGSize_10M,
    _IMGSize_9M,
    _IMGSize_8M,
    _IMGSize_7M,
    _IMGSize_6M,
    _IMGSize_5_4M,
    _IMGSize_5M,
    _IMGSize_4_9M,
    _IMGSize_4_4M,
    _IMGSize_4M,
    _IMGSize_3_8M,
    _IMGSize_3M,
    _IMGSize_2_6M,
    _IMGSize_2_1M,
    _IMGSize_2M,
    _IMGSize_1_5M,
    _IMGSize_1M,
    _IMGSize_0_9M,
    _IMGSize_0_8M,
    _IMGSize_0_6M,
    _IMGSize_0_4M,
    _IMGSize_VGA,
    _IMGSize_0_2M,
    _IMGSize_2M_16x9,
    _IMGSize_7M_16x9,
	_IMGSize_720Px2, //dual sensor
	_IMGSize_VGAx2
} _IMGSize_sel;
#define Max_ImgSize _IMGSize_VGAx2

//****************************************************************************************************
/*IMGQuality*/
typedef enum {
    _IMGQuality_Best,
    _IMGQuality_Fine,
    _IMGQuality_Normal,
    _IMGQuality_Economy
} _IMGQuality_sel;

#define Max_ImgQuality _IMGQuality_Economy

//****************************************************************************************************
/*IMGDriveMode*/
typedef enum {
    _IMGDriveMode_Single,
    _IMGDriveMode_Burst,
    _IMGDriveMode_InfBurst,
    _IMGDriveMode_SmallBurst16,
    _IMGDriveMode_AEBreaketing,
    _IMGDriveMode_AWBBreaketing,
    _IMGDriveMode_PreShot,
    _IMGDriveMode_CoupleShot,
    _IMGDriveMode_AlbumShot,
    _IMGDriveMode_BurstFlash,
    //#NT#2009/12/04#YC Peng -begin
    //#NT#Add drive mode AdjFocusShot for adjust FF lens
    _IMGDriveMode_AdjFocusShot,
    //#NT#2009/12/04#YC Peng -end
    _IMGDriveMode_DIS3Shots
} _IMGDriveMode_sel;

#define Max_DriveMode _IMGDriveMode_DIS3Shots
//****************************************************************************************************

//****************************************************************************************************
typedef enum {
    _IMGSharp_P1,
    _IMGSharp_Normal,
    _IMGSharp_N1
} _IMGSharpness_sel;

#define Max_Sharp _IMGSharp_N1

//****************************************************************************************************
typedef enum {
    _IMGSaturation_P1,
    _IMGSaturation_Normal,
    _IMGSaturation_N1
} _IMGSaturation_sel;

#define Max_Saturation _IMGSaturation_N1

//****************************************************************************************************
/*IMGDZoomIdx*/
typedef enum {
    IMGDZoomIdx_00,
    IMGDZoomIdx_01,
    IMGDZoomIdx_02,
    IMGDZoomIdx_03,
    IMGDZoomIdx_04,
    IMGDZoomIdx_05,
    IMGDZoomIdx_06,
    IMGDZoomIdx_07,
    IMGDZoomIdx_08,
    IMGDZoomIdx_09,
    IMGDZoomIdx_10,
    IMGDZoomIdx_11,
    IMGDZoomIdx_12,
    IMGDZoomIdx_13,
    IMGDZoomIdx_14,
    IMGDZoomIdx_15,
    IMGDZoomIdx_16,
    IMGDZoomIdx_17,
    IMGDZoomIdx_18,
    IMGDZoomIdx_19,
    IMGDZoomIdx_20,
    IMGDZoomIdx_21,
    IMGDZoomIdx_22,
    IMGDZoomIdx_23,
    IMGDZoomIdx_24,
    IMGDZoomIdx_25,
    IMGDZoomIdx_26,
    IMGDZoomIdx_27,
    IMGDZoomIdx_28,
    IMGDZoomIdx_29,
    IMGDZoomIdx_30,
    IMGDZoomIdx_31,
    IMGDZoomIdx_32,
    IMGDZoomIdx_33,
    IMGDZoomIdx_34,
    IMGDZoomIdx_35,
    IMGDZoomIdx_36,
    IMGDZoomIdx_37,
    IMGDZoomIdx_38,
    IMGDZoomIdx_39,
    IMGDZoomIdx_40,
    IMGDZoomIdx_41,
    IMGDZoomIdx_42,
    IMGDZoomIdx_43,
    IMGDZoomIdx_44,
    IMGDZoomIdx_45,
    IMGDZoomIdx_46,
    IMGDZoomIdx_47,
    IMGDZoomIdx_48,
    IMGDZoomIdx_49,
    IMGDZoomIdx_50
} IMGDZoomIdx_sel;
typedef enum {
    _IMGDZoomIdx_00,
    _IMGDZoomIdx_01,
    _IMGDZoomIdx_02,
    _IMGDZoomIdx_03,
    _IMGDZoomIdx_04,
    _IMGDZoomIdx_05,
    _IMGDZoomIdx_06,
    _IMGDZoomIdx_07,
    _IMGDZoomIdx_08,
    _IMGDZoomIdx_09,
    _IMGDZoomIdx_10,
    _IMGDZoomIdx_11,
    _IMGDZoomIdx_12,
    _IMGDZoomIdx_13,
    _IMGDZoomIdx_14,
    _IMGDZoomIdx_15,
    _IMGDZoomIdx_16,
    _IMGDZoomIdx_17,
    _IMGDZoomIdx_18,
    _IMGDZoomIdx_19,
    _IMGDZoomIdx_20,
    _IMGDZoomIdx_21,
    _IMGDZoomIdx_22,
    _IMGDZoomIdx_23,
    _IMGDZoomIdx_24,
    _IMGDZoomIdx_25,
    _IMGDZoomIdx_26,
    _IMGDZoomIdx_27,
    _IMGDZoomIdx_28,
    _IMGDZoomIdx_29,
    _IMGDZoomIdx_30,
    _IMGDZoomIdx_31,
    _IMGDZoomIdx_32,
    _IMGDZoomIdx_33,
    _IMGDZoomIdx_34,
    _IMGDZoomIdx_35,
    _IMGDZoomIdx_36,
    _IMGDZoomIdx_37,
    _IMGDZoomIdx_38,
    _IMGDZoomIdx_39,
    _IMGDZoomIdx_40,
    _IMGDZoomIdx_41,
    _IMGDZoomIdx_42,
    _IMGDZoomIdx_43,
    _IMGDZoomIdx_44,
    _IMGDZoomIdx_45,
    _IMGDZoomIdx_46,
    _IMGDZoomIdx_47,
    _IMGDZoomIdx_48,
    _IMGDZoomIdx_49,
    _IMGDZoomIdx_50
} _IMGDZoomIdx_sel;

//****************************************************************************************************
/*IMGFlashMode*/
typedef enum {
    _IMGFlashMode_Auto,
    _IMGFlashMode_ForceOFF,
    _IMGFlashMode_ForceON,
    _IMGFlashMode_AutoRedEye,
    _IMGFlashMode_ForceONRedEye,
    _IMGFlashMode_NightStrobe
} _IMGFlashMode_sel;

#define Max_FlashMode _IMGFlashMode_NightStrobe

//****************************************************************************************************
/*POWERFreq*/
typedef enum {
    _POWERFreq_50Hz,
    _POWERFreq_60Hz
} _POWERFreq_sel;

//****************************************************************************************************
//#NT#2009/12/29#YC Peng -begin
//#NT#Add Negative effect
/*IMGEffect*/
typedef enum {
    _IMGEffect_Off,
    _IMGEffect_BW,
    _IMGEffect_Sepia,
    _IMGEffect_Vivid,
    _IMGEffect_Neutral,
    _IMGEffect_Negative,
    _IMGEffect_LowSharping,
    _IMGEffect_Fasion1,
    _IMGEffect_Fasion2,
    _IMGEffect_CustomManual,
    _IMGEffect_Red,
    _IMGEffect_Green,
    _IMGEffect_Blue,
    _IMGEffect_Yellow,
    _IMGEffect_Purple

} _IMGEffect_sel;

#define Max_ImgEffect _IMGEffect_Purple
//#NT#2009/12/29#YC Peng -end

//****************************************************************************************************
/*IMGAWBBreaketing*/
typedef enum {
    _IMGAWBBreaketing_OFF,
    _IMGAWBBreaketing_ON
} _IMGAWBBreaketing_sel;

//****************************************************************************************************
/*IMGWBMode*/
typedef enum {
    _IMGWBMode_Auto,
    _IMGWBMode_Daylight,
    _IMGWBMode_Cloudy,
    _IMGWBMode_Tungsten,
    _IMGWBMode_Fluorescent1,
    _IMGWBMode_Fluorescent2,
    _IMGWBMode_Sunset,
    _IMGWBMode_BirthdayParty,
    _IMGWBMode_Customer1,
    _IMGWBMode_Customer2,
    _IMGWBMode_Customer3
}_IMGWBMode_sel;

#define Max_WBMode _IMGWBMode_Customer3

//****************************************************************************************************
/*IMGWBMode*/
typedef enum {
    _IMGWBAdjust_Off,
    _IMGWBAdjust_On
}_IMGWBAdjust_sel;

#define Max_WBAdjust _IMGWBAdjust_On

//****************************************************************************************************
/*IMGEVValue*/
typedef enum {
    _IMGEVValue_N10,
    _IMGEVValue_N9,
    _IMGEVValue_N8,
    _IMGEVValue_N7,
    _IMGEVValue_N6,
    _IMGEVValue_N5,
    _IMGEVValue_N4,
    _IMGEVValue_N3,
    _IMGEVValue_N2,
    _IMGEVValue_N1,
    _IMGEVValue_00,
    _IMGEVValue_P1,
    _IMGEVValue_P2,
    _IMGEVValue_P3,
    _IMGEVValue_P4,
    _IMGEVValue_P5,
    _IMGEVValue_P6,
    _IMGEVValue_P7,
    _IMGEVValue_P8,
    _IMGEVValue_P9,
    _IMGEVValue_P10
} _IMGEVValue_sel;

#define Max_EvValue _IMGEVValue_P10

//****************************************************************************************************
/*IMGAEMode*/
typedef enum {
    _IMGAEMode_CenterWeighted,
    _IMGAEMode_Spot,
    _IMGAEMode_Matrix,
    _IMGAEMode_Evaluative,
    _IMGAEMode_FaceDetection
} _IMGAEMode_sel;

#define Max_AeMode _IMGAEMode_FaceDetection

//****************************************************************************************************
/*IMGAEBreaketing*/
typedef enum {
    _IMGAEBreaketing_OFF,
    _IMGAEBreaketing_ON
} _IMGAEBreaketing_sel;

//****************************************************************************************************
/*IMGISO*/
typedef enum {
    _IMGISO_Auto,
    _IMGISO_50,
    _IMGISO_100,
    _IMGISO_200,
    _IMGISO_400,
    _IMGISO_800,
    _IMGISO_1600,
    _IMGISO_3200
} _IMGISO_sel;

#define Max_ISO _IMGISO_800

//****************************************************************************************************
/*IMGAFMode*/
typedef enum {
    _IMGAFMode_Normal,
    _IMGAFMode_Macro,
    _IMGAFMode_Super_Macro,
    _IMGAFMode_Inifinite,
    _IMGAFMode_ContinueFocus,
    _IMGAFMode_FaceDetection,
    _IMGAFMode_Manual
} _IMGAFMode_sel;

#define Max_AfMode _IMGAFMode_FaceDetection

//****************************************************************************************************
/*IMGAFWindow*/
typedef enum {
    _IMGAFWindow_Center,
    _IMGAFWindow_Auto,
    _IMGAFWindow_ManualPosition
} _IMGAFWindow_sel;

#define Max_AFWindow _IMGAFWindow_ManualPosition

//****************************************************************************************************
/*IMGAFPosition*/
typedef enum {
    _IMGAFPosition_Center,
    _IMGAFPosition_1,
    _IMGAFPosition_2,
    _IMGAFPosition_3,
    _IMGAFPosition_4,
    _IMGAFPosition_5,
    _IMGAFPosition_6
} _IMGAFPosition_sel;

#define Max_AFPosition _IMGAFPosition_4
//****************************************************************************************************
/*IMG_BurstNum*/
typedef enum {
    _BurstNum_Dummy = 0,
    _BurstNum_1,
    _BurstNum_2,
    _BurstNum_3,
    _BurstNum_4,
    _BurstNum_5,
    _BurstNum_Inf,
    Max_BurstNum
} _IMGBurstNum_sel;
//****************************************************************************************************
/*IMG_FileFmt*/
typedef enum {
    _FileFmt_None   =0x0,
    _FileFmt_Raw    =0x1,//bit 0
    _FileFmt_Jpg    =0x2,//bit 1
    _FileFmt_RawJpg =0x3,//bit 0 & 1
    _FileFmt_TIFF   =0x4, //bit 2
    _FileFmt_TIFFJpg =0x6 //bit 0 & 2
} _IMGFileFmt_sel;

#define Max_FileFmt _FileFmt_TIFFJpg

//****************************************************************************************************
/*IMG_SiePath*/
typedef enum {
    _PreColorGain,
    _PreGamma ,
    _PostGamma
} _IMGSiePath_sel;

#define Max_SiePath _PostGamma


//****************************************************************************************************
/*IMG_SieBitDepth*/
typedef enum {
    _SieBitDepth_8  =8,
    _SieBitDepth_10 =10,
    _SieBitDepth_12 =12,
    _SieBitDepth_16 =16
} _IMGSieBitDepth_sel;

#define Max_SieBitDepth _SieBitDepth_16

//****************************************************************************************************
/*IMG_SieBitDepth*/
typedef enum {
    _PreBitDepth_8  =8,
    _PreBitDepth_10 =10,
    _PreBitDepth_12 =12,
    _PreBitDepth_16 =16
} _IMGPreBitDepth_sel;

//****************************************************************************************************
/*FaceDetectionMode*/
typedef enum {
    _FaceDetectionMode_Off,
    _FaceDetectionMode_On
} _IMGFaceDetectionMode_sel;

#define Max_FaceDetectionMode _FaceDetectionMode_On

//****************************************************************************************************
/*SmileDetectionMode*/
typedef enum {
    _SmileDetectionMode_Off,
    _SmileDetectionMode_On
} _IMGSmileDetectionMode_sel;

#define Max_SmileDetectionMode _SmileDetectionMode_On

//****************************************************************************************************
/*BlinkDetectionMode*/
typedef enum {
    _BlinkDetectionMode_Off,
    _BlinkDetectionMode_On
} _IMGBlinkDetectionMode_sel;

#define Max_BlinkDetectionMode _BlinkDetectionMode_On

//****************************************************************************************************
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
/*DISMode (Digital Image Stabilizer mode)*/
typedef enum {
    _DISMode_Off,
    _DISMode_On
} _IMGDISMode_sel;
#define Max_DISMode _DISMode_On
//#NT#2009/11/27#Connie Yu -end
//****************************************************************************************************
/* pipeline ratio */
typedef enum {
    _Ratio_4_3,
    _Ratio_3_2,
    _Ratio_16_9,
    _Ratio_1_1,
    _Ratio_16_5,
    _Ratio_8_3,
    _Ratio_Num
} _IMGRatio_sel;

//****************************************************************************************************
/* image H or V flip */
typedef enum {
    _Flip_Off,
    _Flip_H,
    _Flip_V,
    _Flip_H_V,
    _Flip_Max_type
} _IMGFlip_sel;

//****************************************************************************************************
//#NT#2010/03/04#Jarkko Chang -begin
//#NT#fixed video recording 15 fps buffer issue
/* Video FPS */
typedef enum {
    _Fps_15 = 15,
    _Fps_30 = 30,
    ENUM_DUMMY4WORD(_VideoFps_sel)
} _VideoFps_sel;
//#NT#2010/03/04#Jarkko Chang -end
//****************************************************************************************************

typedef enum {
    _TvInFormat_OFF,
    _TvInFormat_Mode1,
    _TvInFormat_Mode2,
    _TvInFormat_Mode3,
    ENUM_DUMMY4WORD(_IMGTvInFormat_sel)
} _IMGTvInFormat_sel;
#define _TvInFormat_NTSC _TvInFormat_Mode1
#define _TvInFormat_PAL  _TvInFormat_Mode2

//Important
//Must the same sequence and number with struct IPPSizeInfor
typedef enum{
    _SieInHsize=0,//must increase from 0
    _SieInVsize,
    _SieXcenter,
    _SieYcenter,
    _PreCropHsize,
    _PreCropVsize,
    _PreOutHsize,
    _PreOutVsize,
    _PreOutLineOffset,
    _PreCropCapHsize,
    _PreCropCapVsize,
    _IpeOutHsize,
    _IpeOutVsize,
    _IpeCapOutHsize,
    _IpeCapOutVsize,
    _ImeOut1Hsize,
    _ImeOut1Vsize,
    _ImeOut1LineOffset,
    _ImeOut2Hsize,
    _ImeOut2Vsize,
    _ImeOut2LineOffset,
    _ImeCapOutHsize,
    _ImeCapOutVsize,
    _IdeDisplayHsize,
    _IdeDisplayVsize,
    _JpegCapHsize,
    _JpegCapVsize,
    _ThumbHsize,
    _ThumbVsize,
    _ScreenHsize,
    _ScreenVsize,
    _QVHsize,
    _QVVsize,
    _QVPreHsize,
    _QVPreVsize,
    _HDActX_Preview,
    _HDActY_Preview,
    _HDTotal_Preview,
    _HDSync_Preview,
    _VDTotal_Preview,
    _VDSync_Preview,
    _HDActX_Capture,
    _HDActY1_Capture,
    _HDActY2_Capture,
    _HDActY3_Capture,
    _HDActY4_Capture,
    _HDActY5_Capture,
    _HDActY6_Capture,
    _HDActY7_Capture,
    _HDActY8_Capture,
    _HDTotal_Captrue,
    _HDSync_Captrue,
    _VDTotal_Captrue,
    _VDSync_Captrue,
    _PrvBufEndAddr,
    _CapBufEndAddr,
    _CapTotalField,
    _FieldFld1,
    _FieldFld2,
    _FieldFld3,
    _FieldFld4,
    _FieldFld5,
    _FieldFld6,
    _FieldFld7,
    _FieldFld8,
    _FQVSampleRate,
    _CapIMESliceMode,
    _CAHsize,
    _CAVsize,
//#NT#2009/09/24#SA4 -begin
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for pre ca process
    _CAHSub,
    _CAVSub,
//#NT#2009/08/31#Jarkko Chang -end
//#NT#2009/09/24#SA4 -end
    _CALineOffset,
    _OBWinX0,
    _OBWinX1,
    _OBWinY0,
    _OBWinY1,
    _OBSubSample,
    _OBThreshold,
    _OBGain,
    _OBOffset,
    _SensorRatioIdx,
//#NT#2009/09/24#SA4 -begin
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    _DisDisplayHSize,
    _DisDisplayVSize,
    _DisRecordHSize,
    _DisRecordVSize,
    //#NT#2009/11/27#Connie Yu -end
//#NT#20090918#ethanlau -begin
    _Vid_ResolutionIdx,
    _Prv_ResolutionIdx,
//#NT#20090918#ethanlau -end
//#NT#2009/09/24#SA4 -end
//#NT#2010/09/08#ethanlau -begin
    _Binning,
//#NT#2010/09/09#ethanlau -end
    Max_IPPSizeEnum
}_IPPSize_enum;

//Important
//Must the same sequence and number with enum _IPPSize_enum
typedef struct  _IPPSizeInfor{
    UINT    SieInHsize;
    UINT    SieInVsize;
    UINT    SieXcenter;
    UINT    SieYcenter;
    UINT    PreCropHsize;
    UINT    PreCropVsize;
    UINT    PreOutHsize;
    UINT    PreOutVsize;
    UINT    PreOutLineOffset;
    UINT    PreCropCapHsize;
    UINT    PreCropCapVsize;
    UINT    IpeOutHsize;
    UINT    IpeOutVsize;
    UINT    IpeCapOutHsize;
    UINT    IpeCapOutVsize;
    UINT    ImeOut1Hsize;
    UINT    ImeOut1Vsize;
    UINT    ImeOut1LineOffset;
    UINT    ImeOut2Hsize;
    UINT    ImeOut2Vsize;
    UINT    ImeOut2LineOffset;
    UINT    ImeCapOutHsize;
    UINT    ImeCapOutVsize;
    UINT    IdeDisplayHsize;
    UINT    IdeDisplayVsize;
    UINT    JpegCapHsize;
    UINT    JpegCapVsize;
    UINT    ThumbHsize;
    UINT    ThumbVsize;
    UINT    ScreenHsize;
    UINT    ScreenVsize;
    UINT    QVHsize;
    UINT    QVVsize;
    UINT    QVPreHsize;
    UINT    QVPreVsize;
    UINT    HDActX_Preview;
    UINT    HDActY_Preview;
    UINT    HDTotal_Preview;
    UINT    HDSync_Preview;
    UINT    VDTotal_Preview;
    UINT    VDSync_Preview;
    UINT    HDActX_Capture;
    UINT    HDActY1_Capture;
    UINT    HDActY2_Capture;
    UINT    HDActY3_Capture;
    UINT    HDActY4_Capture;
    UINT    HDActY5_Capture;
    UINT    HDActY6_Capture;
    UINT    HDActY7_Capture;
    UINT    HDActY8_Capture;
    UINT    HDTotal_Captrue;
    UINT    HDSync_Captrue;
    UINT    VDTotal_Captrue;
    UINT    VDSync_Captrue;
    UINT    PrvBufEndAddr;
    UINT    CapBufEndAddr;
    UINT    CapTotalField;
    UINT    FieldFld1;
    UINT    FieldFld2;
    UINT    FieldFld3;
    UINT    FieldFld4;
    UINT    FieldFld5;
    UINT    FieldFld6;
    UINT    FieldFld7;
    UINT    FieldFld8;
    UINT    FQVSampleRate;
    UINT    CapIMESliceMode;
    UINT    CAHsize;
    UINT    CAVsize;
//#NT#2009/09/24#SA4 -begin
    UINT    CAHSub;
    UINT    CAVSub;
//#NT#2009/09/24#SA4 -end
    UINT    CALineOffset;
    UINT    OBWinX0;
    UINT    OBWinX1;
    UINT    OBWinY0;
    UINT    OBWinY1;
    UINT    OBSubSample;
    UINT    OBThreshold;
    UINT    OBGain;
    UINT    OBOffset;
    UINT    SensorRatioIdx;
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    UINT32  DisDisplayHsize;
    UINT32  DisDisplayVsize;
    UINT32  DisRecHsize;
    UINT32  DisRecVsize;
    //#NT#2009/11/27#Connie Yu -end
//#NT#2009/09/24#SA4 -begin
//#NT#20090918#ethanlau -begin
    UINT    Vid_ResolutionIdx;
    UINT    Prv_ResolutionIdx;
//#NT#20090918#ethanlau -end
//#NT#2009/09/24#SA4 -end
//#NT#2010/09/08#ethanlau -begin
    UINT    Binning;
//#NT#2010/09/08#ethanlau -end
}   IPPSizeInfor;

//IPP_FUNC_INFOR: Declared for IPP function integration
typedef struct IPP_FUNC_INFOR_STRUCT
{
    UINT32 DfsEn;
    UINT32 DfsCnt;
    UINT32 PRED2DEn;
    UINT32 NLightCapEn;
    UINT32 NLightCapSel;
    UINT32 FQVEn;
    UINT32 CapISO;
    UINT32 ChrFiltEn;
    UINT32 ChrFiltNum;
    UINT32 FQVInProcess;
    UINT32 PreD2DRotEn;
    UINT32 ChrCurveEn;
    UINT32 *ChrCBCurve;
    UINT32 *ChrCRCurve;
    UINT32 *CapGamma;
    //#NT#20091008#ethanlau -begin
    UINT32 StillAntiShakingIdx;
    //#NT#20091008#ethanlau -end
//#NT#2011/09/05#Spark Chou -begin
    //#NT#
    UINT32 DVIModeEn;
//#NT#2011/09/05#Spark Chou -end
} IPP_FUNC_INFOR;

//IPPFuncIDEnum: IPP functions ID
enum IPPFuncIDEnum
{
    DFS_EN,
    DFS_CNT,
    PRED2D_EN,
    NLIGHT_CAP_EN,
    NLIGHT_CAP_SEL,
    FQV_EN,
    CAP_ISO,
    CHRFILT_EN,
    CHRFILT_NUM,
    FQV_INPROCESS,
    PRE_D2D_ROT_EN,
    CHRCURVE_EN,
    CHRCBCURVE,
    CHRCRCURVE,
    CAPGAMMACURVE,
    //#NT#20091008#ethanlau -begin
    _StillAntiShakingIdx,
    //#NT#20091008#ethanlau -end
//#NT#2011/09/05#Spark Chou -begin
    //#NT#
    DVI_MODE_EN,
//#NT#2011/09/05#Spark Chou -end
    IPPFUNC_MAXID
};

enum NlightCapSelEnum
{
    NLCap_PrvDataIn,
    NLCap_QVDataIn,
    NLCap_MaxID
};

#define BLACK_Y     0
#define BLACK_CB    128
#define BLACK_CR    128

typedef struct _IPE_IME_FM_PARAM
{
    UINT BayerAddr;
    UINT BayerHsize;
    UINT BayerVsize;
    UINT OutputAddr1Y;
    UINT OutputAddr1Cb;
    UINT OutputAddr1Cr;
    UINT ImeHsize1;
    UINT ImeVsize1;
    UINT ImeHofst1;
    UINT ImeFmt1;
    UINT OutputAddr2Y;
    UINT OutputAddr2Cb;
    UINT OutputAddr2Cr;
    UINT ImeHsize2;
    UINT ImeVsize2;
    UINT ImeHofst2;
    UINT ImeFmt2;
    UINT ImeOutPath;
} IPE_IME_FM_PARAM;
//----------Variables and Functions Extern-----------------------------------------------------------

INT32 Open_Sensor(PSENSOR_APPOBJ pSensorObj);
extern PSENSOR_APPOBJ   pSensorInfor;
extern SENSOR_APPOBJ    SensorInfor;
extern PCAPTURE_APPOBJ  pCapInfor;
extern CAPTURE_APPOBJ   CapInfor;


//********AlgCallback Function********
extern FPALGCALLBACK    g_fpAlgExport ;
extern void def_AlgCallBackFun(FPALGCALLBACK pAlgcallback);

//********SIE IPE IME JPG Information********
extern unsigned int Get_SieFrameCounter(void);
extern const unsigned short JPGIMG_SIZE[Max_ImgSize+1][2];
extern void SetImeVideoFormat(UINT ImeFmt);
extern UINT GetImeVideoFormat(void);
extern void GetActImgFBAddr(ACTFBADDR_PRAM *pActFBAddr_para);
extern void GetActImgFBAddr_Path1(ACTFBADDR_PRAM *pActFBAddr_para);
extern void GetActImgFBAddr_Path2(ACTFBADDR_PRAM *pActFBAddr_para);
extern unsigned int GetIpeVACnt(void);
extern void DrawCAWindow(void);
extern void ime_SetDzoom2Video_Path1(pIME_FBADDR_PRAM  pIME_Para1);
extern void ime_SetDzoom2Video_Path1_TV(pIME_FBADDR_PRAM  pIME_Para1);
extern void ime_SetDzoom2Video_Path2(pIME_FBADDR_PRAM  pIME_Para2);
//#NT#2010/01/22#Connie Yu -begin
//#NT#DIS Update
extern void SetImeIdeResolution(UINT ImeOutHsize,UINT ImeOutVsize, UINT IdeOutHsize,UINT IdeOutVsize);
//#NT#2010/01/22#Connie Yu -end
extern unsigned int    GetIpeOutHsize(void);
extern unsigned int    GetIpeOutVsize(void);
//********AAA and Mode Switch********
extern void aaa_AWBprocess(void);
extern void aaa_AEprocess(void);
//#NT#2009/09/24#SA4 -begin
//extern unsigned char AF_Process( void );
//#NT#2009/09/24#SA4 -end
//extern void aaa_FDprocess(void);
//extern void fde_FdReset(void);

extern void sd_SmileDetectionProcess(void);
//#NT#2010/10/27#Connie Yu -begin
extern void dis_accumUpdateProcess(void);
extern MOTION_VECTOR dis_getIdeCorrVec(void);
extern void dis_updateJpegLofs(PJPGCmpParam JpgParam);
extern void dis_processLock(BOOL lock);
//extern void dis_Need2UpdateDisplaySz(BOOL update);
extern DIS_PROC_EVENT dis_getProcEvent(void);
extern void dis_eventStart(void);
extern void dis_eventPause(void);
extern UINT8 dis_getImeBufferID(void);
//extern BOOL dis_getFirstUpdStatus(void);
//extern void dis_setFirstUpdStatus(BOOL bfirst);
extern void dis_processOpen(void);
extern void dis_processClose(void);
extern void dis_processLock(BOOL lock);
extern UINT32 dis_chgSizeByDisViewRatio(UINT32 size);
extern void dis_updateDisplaySize(void);
extern UINT32 Get_DISPrvMaxBuffer(void);
//#NT#2010/10/27#Connie Yu -end

extern void SetDzoom2Preview(unsigned int ZoomIndex);
extern void SetDzoom2Video(unsigned int ZoomIndex);
extern UB PCC2CAP;
extern ER SliceMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize);

//********ISR and Operation********

extern unsigned int WriteDebugData(unsigned char *Add);
//********Dzoom********
//#NT#2010/05/28#ethanlau -begin
extern Direction Get_DzoomFlag(void);
extern void Set_DzoomFlag(Direction CDZoom);
//#NT#2010/05/28#ethanlau -end
extern void DzoomStop(void);
extern unsigned short Get_DzoomMaxIndex(void);

//********IQ related********
extern void SetColorMode(unsigned char color);
extern void SetSharpness(unsigned char Index);
extern void SetSaturation(unsigned char Index);
extern void aaa_MWBprocess(unsigned char mode);
extern void ResetAE(void);
extern void ISO_AE(unsigned char ISO_Speed);
extern void AntiFlicker_AE(UINT8 nFlicker);
//#NT#2009/09/24#SA4 -begin
//#NT#20090703#ETHAN LAU -BEGIN
//extern void SceneMode_AE(PImageAlgInfo Info);
//#NT#20090703#ETHAN LAU -END
//#NT#2009/09/24#SA4 -end
//extern void AEMode_AE(UINT8 nMode);
//********UI related********
extern ImageAlgInfo gImageAlgInfo;
extern SceneModeInfo SceneModeSet;
//#NT#2007/03/05#Bowen Li -end
extern INT32 UI_SetParameter(UB item, UB set_value);
extern ImageAlgInfo* UI_GetParameter(void);
extern void GetSceneParam(UB mode, UB* Flashmode,UB* EVcomp,UB* WBmode,UB* Saturation,UB* Sharpness);
void UI_Param_Reset(void);
void UI_SetAllParam(void);
extern void     Write_EXIF_Infor(UINT paddress);
extern UINT     GetAlgCapUsedSize(void);
extern void     SetAlgCapUsedSize(UINT value);
extern INT32    UI_CheckParameter(void);    //Connie_UI_SETTING_CHECK 2005.12.30
extern UCHAR    Get_CaptureDriverMode(void);
extern UINT32   Sensor_CheckNeedSize(void);
extern UINT32   Sensor_IsParameterError(void);

//********Calibration********
extern void Cal_Mode(void);
ER Alg_Cal(char    AlgCal_item, ALG_CAL_PARAM *pParam);

//********Slice Mode Process********
extern char     message[];


extern UINT GetJpgBufSize(void);
extern UINT32 SetJpgBufSize(UINT32 uiSetValue);
extern void GetThumbAddr(UINT ImgNum);
extern UINT GetSlcBufAddr(void);
extern void CaptureNIn1 (UINT8 NumBlockX, UINT8 NumBlockY, UINT32 BlockWidth, UINT32 BlockHeight, UINT8 DelayVD);
//********Exif Related********
void Write_AE_EXIF_Infor(UINT paddress);
//********Lens Related********
//zoom
//extern void Zoom_Init_Start(void);
//extern unsigned char Zoom_init_Finish(void);
//extern void zoom_dc_forward_rotation(void);
//extern void zoom_dc_reverse_rotation(void);
//extern unsigned char Get_Zoom_Section(void);
//extern void Zoom_Retract(void);
extern void LensInit(void);
extern void lens_pins_init(void);
extern INT32 LensRetract(void);
extern void ZOOM_RunStep(unsigned char dir,unsigned short step);
extern void FocusPowerOnOff(BOOL On);
extern void Focus_RunStep(unsigned char dir,unsigned short step);
extern BOOL Check_Zoom_PI(void);
extern BOOL Check_Zoom_PR(void);
extern BOOL Check_Focus_PI(void);
//#NT#2007/3/15#Connie Yu -begin
//Clear warnings
extern void Lens_OnOff(unsigned char ON_OFF);
//#NT#2007/3/15#Connie Yu -end
//********Calibration interface Related********
extern UINT GetDPAdd_Prv(void);
extern UINT GetDPAdd_Cap(void);
extern void SetDPAdd(UINT DP_Prv, UINT DP_Cap);
extern UINT gDP_Add_Prv;
extern UINT gDP_Add_Cap;
//for Calibration: DP
extern UINT32 GetCFAPattern(void);
extern UINT32 GetSieCapXCenter(void);
extern UINT32 GetSieCapYCenter(void);
extern UINT32 GetVDSyncPrv(void);
extern UINT32 GetHDSyncPrv(void);
extern UINT32 GetVDTotalPrv(void);
extern UINT32 GetHDTotalPrv(void);
extern UINT32 GetACTStartXPrv(void);
extern UINT32 GetACTStartYPrv(void);
extern UINT32 GetVDSyncCap(void);
extern UINT32 GetHDSyncCap(void);
extern UINT32 GetVDTotalCap(void);
extern UINT32 GetHDTotalCap(void);
extern UINT32 GetACTStartXCap(void);
extern UINT32 GetACTStartYCap(void);
extern unsigned char Calc_CapExpoFrame(void);
//********TIFF FileFmt Related********
extern void FrameMode_Process(UINT BayerAddr,
                            UINT BayerHsize,
                            UINT BayerVsize,
                            UINT ImeHsize,
                            UINT ImeVsize,
                            UINT ImeOutAddr );
extern UINT TIFF_RGB_CopyHeader(UINT TIFFHeaderAddr,
                            UINT TIFFHsize,
                            UINT TIFFVsize);

extern UINT SetIPPSizeInfor(UINT item, UINT value);
extern UINT GetIPPSizeInfor(UINT    item);
extern void AeSet_isr(void);

extern unsigned int GetSieInHsize(void);
extern unsigned int GetSieInVsize(void);
extern unsigned int GetCapImeOutHsize(void);
extern unsigned int GetCapImeOutVsize(void);
extern void SetPreImeOutHsize(unsigned int  size);
extern void SetPreImeOutVsize(unsigned int  size);
extern void SetPreImeOutOffset(unsigned int size);
//#NT#2009/12/22#YC Peng -begin
//#Add Calibration flow
extern void AeSetAGCGainStep(UINT32 Step);
extern void AeSetPrvExpoTime(UINT32 ExpoTime);
extern void AeSetCapExpoTime(UINT32 ExpoTime);
extern void CalCaptureOne(UINT32 BayerAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 PrePath, UINT32 ObOfs, UINT32 ShutterTimeUs);
//#NT#2007/04/17#YC Peng -end

typedef struct _PHOTO_APPOBJ{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
} PHOTO_APPOBJ,*PPHOTO_APPOBJ;


void ipe_ime_Framemode(IPE_IME_FM_PARAM fmInfo);
extern void SetIPPFuncInfor(UINT32 FuncsID, UINT32 SetValue);
extern UINT32 GetIPPFuncInfor(UINT32 FuncsID);

/***************************************************************************
* Interface from IPPBuffer.h, keep it updated with IPPBuffer.h                                               *
****************************************************************************/
#include "IPPBuffer.h"
/***************************************************************************
* Interface from DataStamp.h, keep it updated with DataStamp.h                                               *
****************************************************************************/
#include "DataStamp.h"

/***************************************************************************
* Old-APIs(Before 96433) Keep it to avoid compile error                    *
****************************************************************************/

typedef struct    _STAMP_INFOR{
    unsigned int    AddrY;
    unsigned int    AddrCb;
    unsigned int    AddrCr;
    unsigned short  Width;
    unsigned short  Height;
    unsigned char   ColorKeyY;
    unsigned char   ColorKeyCb;
    unsigned char   ColorKeyCr;
    unsigned char   Status;
} STAMP_INFOR, *pSTAMP_INFOR;

typedef struct    _STAMP_SRC_INFOR{
    unsigned int    SrcAddrY;
    unsigned int    SrcAddrCb;
    unsigned int    SrcAddrCr;
    unsigned int    SrcYLineOffset;
    unsigned int    SrcHsize;
    unsigned int    SrcVsize;
    unsigned int    StartH;
    unsigned int    StartV;
} STAMP_SRC_INFOR, *pSTAMP_SRC_INFOR;

extern void DateTimeImprintYCC(STAMP_SRC_INFOR *StampSrcInfo,STAMP_INFOR *StampInfo);
//#NT#2008/09/09#Chris Yang - begin
extern UINT32 IQ_SimCB(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf);
//#NT#2008/09/09#Chris Yang - end
extern UINT8 Sen_GetJpgQuality(void);

typedef enum
{
    SEN_ISR_PHOTO_SIE = 0,
    SEN_ISR_PHOTO_PRE,
    SEN_ISR_PHOTO_IPE,
    SEN_ISR_PHOTO_IME,
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    SEN_ISR_PHOTODIS_DIS,
    SEN_ISR_PHOTODIS_IPE,
    //#NT#2009/11/27#Connie Yu -end
    SEN_ISR_FP_NUM
} SEN_ISR_FPID;
typedef void (*SEN_ISR_FP)(UINT32 uiEventStatus);
ER Sen_IsrRegisterEvent(SEN_ISR_FPID IsrId, SEN_ISR_FP EventFp);
void Sen_IsrEventProc(SEN_ISR_FPID IsrId, UINT32 uiEventStatus);
void Sen_SetPhotoInfo(PHOTO_APPOBJ *PhotoObj);


UINT8 Sen_GetJpgQuality(void);
UINT32 Sen_GetBurstNum(void);
UINT32 Sen_GetRawNum(void);
UINT32 Sen_GetJpgNum(void);
void Set2SieCap_Init(void);
void Set2SieCap_SetParam(void);
void Set2SieCap(ImageAlgInfo *AlgInfo, UINT32 BufIdx);
int Get_DzoomIndex(void);
void Set_DzoomIndex(INT idx);
//#NT#20091018#etahnlau -begin
BOOL GetAntishaking_RawbiningEnalbe(void);
void SetAntishaking_RawbiningEnalbe(BOOL enable);
//#NT#20091018#ethanlau -end
void StopPreviewDramData(void);
IME_FBADDR_PRAM* Sen_GetDispParam(void);
extern IME_FBADDR_PRAM* Sen_GetImePath1Param(void);
extern IME_FBADDR_PRAM* Sen_GetImePath2Param(void);

UINT16 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
UINT16 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
UINT32 Adj_VSize(UINT32 InSize, UINT16 VRatio, UINT16 Align);
UINT32 Adj_HSize(UINT32 InSize, UINT16 HRatio, UINT16 Align);

extern ACTFBADDR_PRAM FB_para1;
extern AlgMsgInfo gAlgMsg;
extern ACTFBADDR_PRAM  FB_para1;
extern UINT JpgSize;
extern UCHAR JPEG_Quality[];
extern unsigned char AEB_Idx;
extern PHOTO_APPOBJ DispSizeInfo;
extern const UINT16 usImgRatioTable[_Ratio_Num][_Ratio_Num];

//#NT#2009/10/29#Linkin Huang -begin
//#NT# Data re-arrange using Graphic engine
extern ER GRAPH_ReArrange(UINT32 width, UINT32 height, UINT32 inAddr);
//#NT#2009/10/29#Linkin Huang -end

//#NT#2010/03/04#Jarkko Chang -begin
extern _VideoFps_sel GetVideoFrameRate(void);
extern void SetVideoFrameRate(_VideoFps_sel VideoFps);
//#NT#2010/03/04#Jarkko Chang -end
/***************************************************************************
* FILE END                                                                 *
****************************************************************************/

#endif
