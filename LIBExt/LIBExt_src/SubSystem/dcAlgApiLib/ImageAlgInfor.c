#include "sensor_model.h"
#include "exif.h"
#include "JpgEnc.h"
//#NT#2009/10/29#Linkin Huang -begin
//#NT# add graphic engine header file
#include "grph.h"
//#NT#2009/10/29#Linkin Huang -end

ACTFBADDR_PRAM  FB_para1;
AlgMsgInfo gAlgMsg;
PHOTO_APPOBJ DispSizeInfo;
IPPSizeInfor gIPPSizeInfor={0};
volatile SEN_ISR_FP gFPIsrEventFunc[SEN_ISR_FP_NUM];
UINT JpgSize=0x500000;
UCHAR JPEG_Quality[3] = {95, 93, 80};
//#NT#20091018#ethanlau -begin
BOOL RAWBinning_ENABLE = FALSE;
//#NT#20091018#ethanlau -end
//#NT#2010/03/04#Jarkko Chang -begin
//#NT#fixed video recording 15 fps buffer issue
//UINT32 Video_Frame_Rate = _Fps_30;
_VideoFps_sel Video_Frame_Rate = _Fps_30;
//#NT#2010/03/04#Jarkko Chang -end
//AlgCallback Function
FPALGCALLBACK g_fpAlgExport=NULL ;

ImageAlgInfo gImageAlgInfo=
{
_DSC_Auto,                  //    UCHAR    DSCMode;        // Please reference enum:_DSCMode_sel
_SceneMode_Auto,            //    UCHAR    SceneMode;        // Please reference enum:_IMGSceneMode_sel
_IMGSize_5M,                //    UCHAR    IMGSize;         // Please reference enum:_IMGSize_sel
_IMGQuality_Fine,           //    UCHAR    Quality;        // Please reference enum:_IMGQuality_sel
_IMGDriveMode_Single,       //    UCHAR    DriveMode;         // Please reference enum:_IMGDriveMode_sel
_BurstNum_1,                //    UCHAR    BurstDriveNum;    // Please reference enum:_IMGBurstNum_sel
_IMGSharp_Normal    ,       //    UCHAR    Sharpness;        // Please reference enum:_IMGSharpness_sel
_IMGSaturation_Normal,      //    UCHAR    Saturation;        // Please reference enum:_IMGEffect_sel
_IMGDZoomIdx_00,            //    UCHAR    DZoomIndex;        // Please reference enum:_IMGDZoomIdx_sel
_IMGFlashMode_ForceOFF,     //    UCHAR    FlashMode;        // Please reference enum:_IMGFlashMode_sel
_POWERFreq_60Hz,            //    UCHAR    Frequency;                   // Please reference enum:_POWERFreq_sel
_IMGEffect_Off,             //    UCHAR    ImageEffect;                 // Please reference enum:_IMGEffect_sel
0,                          //    UCHAR    ContrastAdjust;              // Please reference enum:_IMGEffect_sel
{255,255,255},              //    ColorGainInfo    ColorBalanceAdjust;  // Please reference enum:_IMGEffect_sel
0,                          //    UCHAR    ColorMonoAdjust;// Please reference enum:_IMGEffect_sel
_IMGWBMode_Auto,            //    UCHAR    WBMode;            // Please reference enum:_IMGWBMode_sel
_IMGWBAdjust_Off,           //    UCHAR    WBAdjust;        // Please reference enum:_IMGWBAdjust_sel
0,                          //    UCHAR    AWBBracketing;    // Please reference enum:_IMGAWBBracketing_sel
_IMGEVValue_00,             //    UCHAR    AEEVValue;        // Please reference enum:_IMGEVValue_sel
_IMGAEMode_CenterWeighted,  //    UCHAR    AEMode;            // Please reference enum:_IMGAEMode_sel
0,                          //    UCHAR    AEBracketing;    // Please reference enum:_IMGAEBracketing_sel
_IMGISO_Auto,               //    UCHAR    ISO;            // Please reference enum:_IMGISO_sel
_IMGAFMode_Normal,          //    UCHAR    AFMode;            // Please reference enum:_IMGAFMode_sel
_IMGAFWindow_Center,        //    UCHAR    AFWindow;        // Please reference enum:_IMGAFWindow_sel
_IMGAFPosition_Center,      //    UCHAR    AFPosition;        // Please reference enum:_IMGAFPosition_sel
_FileFmt_Jpg,               //    UCHAR    FileFmt;        // Please reference enum:_IMGFileFmt_sel
_PostGamma,                 //    UCHAR    SiePath;        // Please reference enum:_IMGSiePath_sel
_SieBitDepth_8,             //    UCHAR    SieBitDepth;        // Please reference enum:_IMGSieBitDepth_sel
//#NT#2009/06/30#Jarkko Chang -begin
//#NT#modify for 630 prv flow
_SieBitDepth_8,            //    UCHAR    PrePrvBitDepth;        // Please reference enum:_IMGPreBitDepth_sel
//_PreBitDepth_8,            //    UCHAR    PrePrvBitDepth;        // Please reference enum:_IMGPreBitDepth_sel
//#NT#2009/06/30#Jarkko Chang -end
_PreBitDepth_8,             //    UCHAR    PreCapBitDepth;        // Please reference enum:_IMGPreBitDepth_sel
_FaceDetectionMode_Off,     //    UCHAR    FaceDetectionMode;        // Please reference enum:_IMGFaceDetectionMode_sel
_SmileDetectionMode_Off,    //    UCHAR    SmileDetectionMode;        // Please reference enum:_IMGSmileDetectionMode_sel
_BlinkDetectionMode_Off,    //    UCHAR    BlinkDetectionMode;        // Please reference enum:_IMGSceneMode_sel
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
_DISMode_Off,               //    UINT8    DISMode;            // Please reference enum:_IMGDISMode_sel
//#NT#2009/11/27#Connie Yu -end
_Ratio_4_3,
_Ratio_4_3,
//#NT#2010/10/15#ethanlau -begin
//#NT#for 565 demo only
//#NT#2011/01/13#JJ Huang -begin
//#NT# OV5653 Sensor Flip Switch
//For Normal(HJT/TPK/EVB), no-flip
_Flip_Off,//for HJT//_Flip_H,
//#NT#2011/01/13#JJ Huang -end
//#NT#2010/10/15#ethanlau -end
_Resolution_848x480,
_Resolution_640x480,
_Level0,
_TvInFormat_OFF
};
//#NT#2008/10/14#Yuppie Lin -end

unsigned char AEB_Idx=0;
const unsigned short JPGIMG_SIZE[Max_ImgSize+1][2]=
{
//#NT#2009/10/13#Jarkko Chang -begin
//#NT#add for project using
    {4608,3464}, //16M
    {4320,3240}, //14M
//#NT#2009/10/13#Jarkko Chang -end
    {4000,3000}, //12M
    {3648,2736}, // 10M
    {3456,2592}, // 9M
    {3200,2400}, // 8M
    {3072,2304}, // 7M
    {2848,2136}, // 6M
//  {2560,1920}, // 5M
    {2688,2016}, // 5.4M
    {2592,1944}, // 5M
    {2560,1920}, // 4.9M
    {2432,1824}, // 4.4M
    //{2304,1728}, // 4M
    {2272,1704}, // 4M
    {2240,1680}, // 3.8M
    {2048,1536}, // 3M
    {1856,1392}, // 2.6M
    {1664,1248}, // 2.1M
    {1600,1200}, // 2M
    {1408,1056}, // 1.5M
    {1280,960}, // 1M
    {1280,720}, // 0.9M
    {1088,816}, // 0.8M
    {896,672}, // 0.6M
    {768,576}, // 0.4M
    {640,480}, // VGA
    {576,432},  // 0.2M
    //{ 640, 496} // VGA DC8365 temp solution
    //{ 320, 240} // QVGA
    {1920,1080},  //_IMGSize_2M_16x9,
    {3648,2048},  //_IMGSize_7M_16x9,
    {1920, 720},   //Dual Sensor
	{1280, 480},  //Dual Sensor
};


#define Total_SceneMode 15
SceneModeInfo gSceneModeInfo[Total_SceneMode]={
//  SceneMode                   FlashMode                   Evcomp                          Saturation              Sharpness               WB                  Focus   FlashChangeAE
{_SceneMode_Auto,           _IMGFlashMode_Auto,             _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1},
{_SceneMode_Program,        _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGSatus_NoChange,         0,      1},
{_SceneMode_Landscape,      _IMGFlashMode_ForceOFF,         _IMGEVValue_00,             _IMGSaturation_P1,      _IMGSharp_P1,           _IMGWBMode_Auto,            0,      1},
{_SceneMode_Potrait,        _IMGFlashMode_AutoRedEye,       _IMGEVValue_P1,             _IMGSaturation_Normal,  _IMGSharp_N1,           _IMGWBMode_Auto,            0,      1},
{_SceneMode_NightScene,     _IMGFlashMode_ForceON,          _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      0},
{_SceneMode_Sport,          _IMGFlashMode_ForceOFF,         _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1},
{_SceneMode_AV,             _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
{_SceneMode_TV,             _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
{_SceneMode_Manual,         _IMGSatus_NoChange,             _IMGSatus_NoChange,         _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
{_SceneMode_NightPotrait,   _IMGFlashMode_NightStrobe,      _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      0},
{_SceneMode_NightLandscape, _IMGFlashMode_ForceOFF,         _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Daylight,        0,      0},
{_SceneMode_Party,          _IMGFlashMode_AutoRedEye,       _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1},
{_SceneMode_SunSet,         _IMGFlashMode_ForceOFF,         _IMGEVValue_N1,             _IMGSaturation_P1,      _IMGSharp_Normal,       _IMGWBMode_Daylight,        0,      0},
{_SceneMode_BackLight,      _IMGFlashMode_ForceON,          _IMGEVValue_00,             _IMGSatus_NoChange,     _IMGSatus_NoChange,     _IMGWBMode_Auto,            0,      0},
{_SceneMode_Smart,          _IMGFlashMode_Auto,             _IMGEVValue_00,             _IMGSaturation_Normal,  _IMGSharp_Normal,       _IMGWBMode_Auto,            0,      1}
};

const UINT16 usImgRatioTable[_Ratio_Num][_Ratio_Num] =
{
    //4:3                               3:2                                16:9                             1:1                             16_5                           8:3
    {IMG_RATIO_UINT,    228,                              192,                             IMG_RATIO_UINT, 106,                           128                         }, //4:3
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,     216,                            IMG_RATIO_UINT, 120,                           148                         }, //3:2
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,     IMG_RATIO_UINT,  IMG_RATIO_UINT, IMG_RATIO_UINT, IMG_RATIO_UINT}, //16:9
    {192,                              170,                               144,                            IMG_RATIO_UINT, IMG_RATIO_UINT, IMG_RATIO_UINT}, //1:1
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,     IMG_RATIO_UINT,  IMG_RATIO_UINT, IMG_RATIO_UINT, IMG_RATIO_UINT}, //16:5
    {IMG_RATIO_UINT,    IMG_RATIO_UINT,     IMG_RATIO_UINT,  IMG_RATIO_UINT, IMG_RATIO_UINT, IMG_RATIO_UINT}  //8:3
};

#if 0//move to SensorSystem
/*
void SetColorMode(unsigned char color)
{
    if(color == _IMGEffect_BW)
        ipe_Set_CbCrcolorAdj(0x80,0x80,0,0);
    else if(color == _IMGEffect_Sepia)
        ipe_Set_CbCrcolorAdj(0x60,0xa0,0,0);
    else
        ipe_Set_CbCrcolorAdj(0x80,0x80,0x80,0);
}

void SetSharpness(unsigned char Index)
{
    if(Index == _IMGSharp_P1)
        ipe_SetYcolorAdj(0x180,128);
    else if(Index == _IMGSharp_Normal)
        ipe_SetYcolorAdj(0xf0,128);
    else if(Index == _IMGSharp_N1)
        ipe_SetYcolorAdj(0x50,128);
}

void SetSaturation(unsigned char Index)
{
    if(Index == _IMGSaturation_P1)
        ipe_SetIntSatOffset(0,0x20);
    else if(Index == _IMGSaturation_Normal)
        ipe_SetIntSatOffset(0,0x0);
    else if(Index == _IMGSaturation_N1)
        ipe_SetIntSatOffset(0,0xe0);
}
*/
#endif
void ClearAEB_Idx(void)
{
    AEB_Idx = 0;
}

void SetAWB(unsigned char WB)
{
    aaa_AwbLock(1);
    //sie_waitVD(4);
    if(WB ==_IMGWBMode_Auto)
        aaa_AwbLock(0);
    else
        aaa_MWBprocess(WB);
}
//#NT#2007/03/05#Bowen Li -begin
//fix typing error
SceneModeInfo SceneModeSet;

void GetSceneParam(UB mode, UB* Flashmode,UB* EVcomp,UB* WBmode,UB* Saturation,UB* Sharpness)
{
    UB i;

    for(i=0;i<Total_SceneMode;i++)
    {
        if(mode == gSceneModeInfo[i].SceneMode)
            break;
    }

    *(UB*)Flashmode=gSceneModeInfo[i].FlashMode;
    *(UB*)EVcomp=gSceneModeInfo[i].EVComp;
    *(UB*)WBmode=gSceneModeInfo[i].WBMode;
    *(UB*)Saturation=gSceneModeInfo[i].Saturation;
    *(UB*)Sharpness=gSceneModeInfo[i].Sharpness;
}
//#NT#2007/03/05#Bowen Li -end

ImageAlgInfo* UI_GetParameter(void)
{
    return &gImageAlgInfo;
}


INT32 UI_SetParameter(UB item, UB set_value)
{

    sprintf(message,"Item=%d\t value=%d\n\r",item,set_value);
    SEN_INDMSG(message);

    switch(item)
    {
        case _DSCMode: // 0
            if(set_value>Max_DSCMode)
                return E_PAR;
            gImageAlgInfo.DSCMode=set_value;
            ResetAE();
            if(gImageAlgInfo.DSCMode==_DSC_Auto)
            {
                gImageAlgInfo.WBMode=_IMGWBMode_Auto;
//Harry             Ip_UImodeReset();
//Harry             AWB_Lock(0);
            }
            break;
        case _SceneMode: // 1
            if(set_value>Max_SceneMode)
                return E_PAR;
            gImageAlgInfo.SceneMode=set_value;
            //#NT#2010/09/21#ethanlau -begin
            //#NT#remove,scenemode relative setting chg to be controled by proj level
            /*
            GetSceneParam(gImageAlgInfo.SceneMode,
                            &SceneModeSet.FlashMode,
                            &SceneModeSet.EVComp,
                            &SceneModeSet.WBMode,
                            &SceneModeSet.Saturation,
                            &SceneModeSet.Sharpness);
            */
            //#NT#2010/09/21#ethanlau -end
            break;
        case _ImgSize: // 2
            if(set_value>Max_ImgSize)
                return E_PAR;
            gImageAlgInfo.IMGSize=set_value;
            SetImageOutSize(JPGIMG_SIZE[gImageAlgInfo.IMGSize][0],JPGIMG_SIZE[gImageAlgInfo.IMGSize][1]);
            break;
        case _Quality: // 3
            if(set_value>Max_ImgQuality)
                return E_PAR;
            gImageAlgInfo.Quality=set_value;
            break;
/* // need to cancel, it is counted by FW //Harry
        case _SelfTimer:
            gImageAlgInfo.SelfDriveTimer=set_value;
            break;
*/
        case _FlashMode: // 4
            if(set_value>Max_FlashMode)
                return E_PAR;
            gImageAlgInfo.FlashMode=set_value;
            flash_Evaluate();
            break;
        case _ImageEffect: // 5
            if(set_value>Max_ImgEffect)
                return E_PAR;
            gImageAlgInfo.ImageEffect=set_value;
            debug_err(("color effect : %d \r\n",set_value));
            //SetColorMode(gImageAlgInfo.ImageEffect);
            break;
        case _AEMode: // 6
            if(set_value>Max_AeMode)
                return E_PAR;
            gImageAlgInfo.AEMode=set_value;
            ResetAE();
            break;
        case _AEEV: // 7
            if(set_value>Max_EvValue)
                return E_PAR;
            gImageAlgInfo.AEEVValue=set_value;
            ResetAE();
            break;
        case _WBMode:  // 8
            if(set_value>Max_WBMode)
                return E_PAR;
            gImageAlgInfo.WBMode=set_value;
            SetAWB(gImageAlgInfo.WBMode);
            break;
        case _AFMode: // 9
            if(set_value>Max_AfMode)
                return E_PAR;
            gImageAlgInfo.AFMode=set_value;
            break;
        case _AFWindow: // 10
            if(set_value>Max_AFWindow)
                return E_PAR;
            gImageAlgInfo.AFWindow=set_value;
            break;
        case _AFPosition: // 11
            if(set_value>Max_AFPosition)
                return E_PAR;
            gImageAlgInfo.AFPosition=set_value;
            break;
        case _ISOIndex: // 12
            if(set_value > Max_ISO)
                return E_PAR;
            gImageAlgInfo.ISO=set_value;
            //ISO_AE(gImageAlgInfo.ISO);
            ResetAE();
            break;
        case _DZoomIndex: // 13
            gImageAlgInfo.DZoomIndex=set_value;
            //Set_digitalzoom(gImageAlgInfo.DZoomIndex);
            SetDzoom2Preview(gImageAlgInfo.DZoomIndex);
            break;
      case _BurstNum:  // 14
          gImageAlgInfo.BurstDriveNum=set_value;
          break;
//      case _VideoSize:  // 15
//          if(set_value>Max_VideoSize)
//              return E_PAR;
//          gImageAlgInfo.VideoSize=set_value;
//          break;
        case _DriveMode:  // 16
            if(set_value>Max_DriveMode)
                return E_PAR;
            gImageAlgInfo.DriveMode=set_value;
            break;
        case _Sharpness:  // 17
            if(set_value>Max_Sharp)
                return E_PAR;
            gImageAlgInfo.Sharpness=set_value;
            //SetSharpness(gImageAlgInfo.Sharpness);
            break;
        case _Saturation:  // 18
            if(set_value>Max_Saturation)
                return E_PAR;
            gImageAlgInfo.Saturation=set_value;
            //SetSaturation(gImageAlgInfo.Saturation);
            break;
        //JJ_UI_FileFmt
        case _FileFmt:  // x
            gImageAlgInfo.FileFmt=set_value;
            break;
        case _SieBitDepth:  // x
            gImageAlgInfo.SieBitDepth=set_value;
            break;
        case _SiePath:  // x
            gImageAlgInfo.SiePath=set_value;
            break;
        case _PrePrvBitDepth:
            gImageAlgInfo.PrePrvBitDepth=set_value;
            break;
        case _PreCapBitDepth:
            gImageAlgInfo.PreCapBitDepth=set_value;
            break;
//#NT#2008/10/14#Yuppie Lin -begin
//#NT#Arrange code and add FD/SD/BD member

        case _FaceDetectionMode:
            if(set_value > Max_FaceDetectionMode)
                return E_PAR;
            gImageAlgInfo.FaceDetectionMode = set_value;
            break;
        case _SmileDetectionMode:
            if(set_value > Max_SmileDetectionMode)
                return E_PAR;
            gImageAlgInfo.SmileDetectionMode = set_value;
            break;
        case _BlinkDetectionMode:
            if(set_value > Max_BlinkDetectionMode)
                return E_PAR;
            gImageAlgInfo.BlinkDetectionMode = set_value;
            break;
//#NT#2008/10/14#Yuppie Lin -end
        //#NT#2009/11/27#Connie Yu -begin
        //#NT#Add DIS process
        case _DISMode:
            if(set_value > Max_DISMode)
                return E_PAR;
            if ( set_value == _DISMode_On )
                debug_err(("^RWarning: Enable DIS\r\n"));
            gImageAlgInfo.DISMode = set_value;
            break;
        //#NT#2009/11/27#Connie Yu -end
        case _ImageRatio:
            if (set_value > _Ratio_Num)
                return E_PAR;
            gImageAlgInfo.ImageRatio = set_value;
            break;

//#NT#2010/04/29#Jarkko Chang -begin
        case _DispRatio:
            if (set_value > _Ratio_Num)
                return E_PAR;
            gImageAlgInfo.DispRatio = set_value;
            break;
//#NT#2010/04/29#Jarkko Chang -end
//#NT#20091016#ethanlau -begin
        case _Vid_Resolution:
            if(set_value > Max_VidPrvSize)
                return E_PAR;
            gImageAlgInfo.Vid_Resolution = set_value;
            //#NT#2010/09/10#ethanlau -begin
            SensorMode(IPL_GetMode());
            //#NT#2010/09/10#ethanlau -end
            break;
        case _Prv_Resolution:
            if(set_value > Max_VidPrvSize)
                return E_PAR;
            gImageAlgInfo.Prv_Resolution = set_value;
            //#NT#2010/09/10#ethanlau -begin
            SensorMode(IPL_GetMode());
            //#NT#2010/09/10#ethanlau -end
            break;
//#NT#20091016#ethanlau -end
        case _ImageFlip:
            if (set_value > _Flip_Max_type)
                return E_PAR;
//#NT#20120820#clifflin -begin
//#NT#Use sensor ISP to achieve flip function
//#ifdef NT99140_632
           // gImageAlgInfo.ImageFlip = _Flip_Off;
            //gSenCBInfo.SENSORFlipType = set_value;
           // SenFlowFunc(SENSOR_FLIP_TYPE, &gSenCBInfo);
//#elif NT99141_632
            gImageAlgInfo.ImageFlip = _Flip_Off;
            gSenCBInfo.SENSORFlipType = set_value;
            SenFlowFunc(SENSOR_FLIP_TYPE, &gSenCBInfo);
//#else
   //         gImageAlgInfo.ImageFlip = set_value;
//#endif
//#NT#20120820#clifflin -end
            break;
//#NT#20091008#ethanlau -begin
        case _StillAntiShaking:
            if (set_value > Max_AntiShakeLevel)
                return E_PAR;
            gImageAlgInfo.StillAntiShaking = set_value;
            break;
//#NT#20091008#ethanlau -end
//#NT#2009/10/29#Jarkko Chang -begin
        case _Frequency:
            if (set_value > _POWERFreq_60Hz)
                return E_PAR;
            gImageAlgInfo.Frequency = set_value;
            AntiFlicker_AE(gImageAlgInfo.Frequency);
            break;
//#NT#2009/10/28#Jarkko Chang -end
        case _TvInFormat:
            gImageAlgInfo.TvInFormat=set_value;
            if ( set_value == _TvInFormat_OFF )
            {
                SetIPPFuncInfor(DVI_MODE_EN, DISABLE);
            }
            else
            {
                SetIPPFuncInfor(DVI_MODE_EN, ENABLE);
            }
            break;
        default:
            return E_PAR;
    }
    return E_OK;
}

void UI_SetAllParam(void)
{
    //UI_SetParameter(_SceneMode, gImageAlgInfo.SceneMode);
    UI_SetParameter(_ImgSize,gImageAlgInfo.IMGSize);
    UI_SetParameter(_Quality,gImageAlgInfo.Quality);
    UI_SetParameter(_ImageEffect,gImageAlgInfo.ImageEffect);
    UI_SetParameter(_AEMode,gImageAlgInfo.AEMode);
    UI_SetParameter(_FlashMode,gImageAlgInfo.FlashMode);
    UI_SetParameter(_AEEV,gImageAlgInfo.AEEVValue);
    UI_SetParameter(_WBMode,gImageAlgInfo.WBMode);
    UI_SetParameter(_ISOIndex,gImageAlgInfo.ISO);
    UI_SetParameter(_Sharpness,gImageAlgInfo.Sharpness);
    UI_SetParameter(_Saturation,gImageAlgInfo.Saturation);
    UI_SetParameter(_AFMode,gImageAlgInfo.AFMode);
    UI_SetParameter(_AFWindow,gImageAlgInfo.AFWindow);
    UI_SetParameter(_AFPosition,gImageAlgInfo.AFPosition);
}
void Write_EXIF_Infor(UINT paddress)
{

    UH Flash_Mode;//,Exp_Time,Aperture;
    //H  Shutterspeed;

    Flash_Mode=0;
    Write_AE_EXIF_Infor(paddress);
//20060209 move toWrite_AE_EXIF_Infor(paddress); for different sensor
/*//#if Sharp_8M||Panasonic_5M
    Write_AE_EXIF_Infor(paddress);
#else
    Aperture=30;
    Shutterspeed=50;
    JPEGWriteAPP1ExposureTime(paddress, 1,  32);//ExpTime=1/32s
    JPEGWriteAPP1ShutterSpeed(paddress, Shutterspeed,  10);//ExpTime=1/(2^(Shutterspeed/10))s
    JPEGWriteAPP1FNumber(paddress, 28, 10);//Fno=2.8
    JPEGWriteAPP1Aperture(paddress,Aperture, 10);//Fno=2^((Aperture/10)/2)
    JPEGWriteAPP1MaxAperture(paddress, Aperture, 10);
    JPEGWriteAPP1ISOSpeed(paddress, 100);//ISO=100
#endif
#if Sharp_8M
    JPEGWriteAPP1FocalLength(paddress,Get_FocalLength(),10);
#else
    JPEGWriteAPP1FocalLength(paddress,73,10);
#endif
*/

//20060209 move toWrite_AE_EXIF_Infor(paddress); for different sensor
/*  if(gImageAlgInfo.DriveMode != _IMGDriveMode_AEBreaketing)
        JPEGWriteAPP1ExposureBias(paddress, (gImageAlgInfo.AEEVValue-6), 3);//step 0.3
    else
    {
        JPEGWriteAPP1ExposureBias(paddress, (gImageAlgInfo.AEEVValue-6)*2-3+AEB_Idx*3, 6);//step 0.3
        AEB_Idx++;
    }
*/
    //AEMode
    switch(gImageAlgInfo.AEMode)
    {
        case _IMGAEMode_CenterWeighted:
            JPEGWriteAPP1MeteringMode(paddress,MeterCenterWeightedAvg);
        break;
        case _IMGAEMode_Spot:
            JPEGWriteAPP1MeteringMode(paddress,MeterSpot);
        break;
        case _IMGAEMode_Matrix:
            JPEGWriteAPP1MeteringMode(paddress,MeterPattern);
        break;
        case _IMGAEMode_Evaluative:
            JPEGWriteAPP1MeteringMode(paddress,MeterPattern);
        break;
    }
    //Lightsource
    if(Flash_Param.FlshEna)
    {
        JPEGWriteAPP1LightSource(paddress, 4);
    }
    else
    {
        switch(gImageAlgInfo.WBMode)
        {
            case _IMGWBMode_Auto:
                JPEGWriteAPP1LightSource(paddress, LightUnknown);
            break;
            case _IMGWBMode_Daylight:
                JPEGWriteAPP1LightSource(paddress, LightDaylight);
            break;
            case _IMGWBMode_Cloudy :
                JPEGWriteAPP1LightSource(paddress, 10);
            break;
            case _IMGWBMode_Tungsten:
                JPEGWriteAPP1LightSource(paddress, LightTungsten);
            break;
            case _IMGWBMode_Fluorescent1:
                JPEGWriteAPP1LightSource(paddress, LightFluorescent);
            break;
            default:
                JPEGWriteAPP1LightSource(paddress, LightUnknown);
            break;
        }
    }
    //WhiteBalance
    if(gImageAlgInfo.WBMode == _IMGWBMode_Auto)
    {
        JPEGWriteAPP1WhiteBalance(paddress, AutoWhite);
    }
    else
    {
        JPEGWriteAPP1WhiteBalance(paddress, ManualWhite);
    }
    debug_ind(("gImageAlgInfo.FlashMode=%d\r\n",gImageAlgInfo.FlashMode));
    //Flashlight
    switch(gImageAlgInfo.FlashMode)
    {
        case _IMGFlashMode_Auto:
            Flash_Mode=(1<<4)+(1<<3);
        break;
        case _IMGFlashMode_ForceOFF:
            Flash_Mode=(1<<4);
        break;
        case _IMGFlashMode_ForceON:
            Flash_Mode=(1<<3);
        break;
        case _IMGFlashMode_AutoRedEye:
            Flash_Mode=(1<<6)+(1<<4)+(1<<3);
        break;
        case _IMGFlashMode_ForceONRedEye:
            Flash_Mode=(1<<6)+(1<<3);
        break;
        case _IMGFlashMode_NightStrobe:
            Flash_Mode=(1<<6)+(1<<4)+(1<<3);
        break;
        default:
            Flash_Mode=(1<<4)+(1<<3);
        break;
    }
    if(Flash_Param.FlshEna)
        JPEGWriteAPP1StrobeFlash(paddress,StrobeFired+Flash_Mode);
    else
        JPEGWriteAPP1StrobeFlash(paddress,StrobeNoFired+Flash_Mode);
    //AEB
    if(gImageAlgInfo.AEBracketing == _IMGAEBreaketing_ON)
        JPEGWriteAPP1ExposureMode(paddress,AutoBracket);
    else
        JPEGWriteAPP1ExposureMode(paddress,AutoExposure);
    //ExposureProgram
    if(gImageAlgInfo.SceneMode==_SceneMode_Auto)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgNormal);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Manual)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgManual);
    else if(gImageAlgInfo.SceneMode==_SceneMode_AV)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgAperture);
    else if(gImageAlgInfo.SceneMode==_SceneMode_TV)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgShutter);
    else if((gImageAlgInfo.SceneMode==_SceneMode_Party)||(gImageAlgInfo.SceneMode==_SceneMode_SunSet))
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgCreative);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Sport)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgAction);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Potrait)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgPortrait);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Landscape)
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgLandscape);
    else
        JPEGWriteAPP1ExposureProgram(paddress,ExpPrgNoDef);
    //SceneCaptureType
    if(gImageAlgInfo.SceneMode==_SceneMode_Landscape)
        JPEGWriteAPP1SceneCaptureType(paddress,LandScapeScene);
    else if(gImageAlgInfo.SceneMode==_SceneMode_Potrait)
        JPEGWriteAPP1SceneCaptureType(paddress,PortraitScene);
    else if((gImageAlgInfo.SceneMode==_SceneMode_NightScene)||(gImageAlgInfo.SceneMode==_SceneMode_NightPotrait)||(gImageAlgInfo.SceneMode==_SceneMode_NightLandscape))
        JPEGWriteAPP1SceneCaptureType(paddress,NightScene);
    else
        JPEGWriteAPP1SceneCaptureType(paddress,StandardScene);
    //ImageEffect(Normal/Sepia/BW)
    if(gImageAlgInfo.ImageEffect ==_IMGEffect_Off)
        JPEGWriteAPP1CustomRendered(paddress,NormalRendered);
    else
        JPEGWriteAPP1CustomRendered(paddress,CustomRendered);
    //Quality
    if(gImageAlgInfo.Quality ==_IMGQuality_Best)
        JPEGWriteJPGQulaity(paddress, Quality_SuperHigh);
    else if(gImageAlgInfo.Quality ==_IMGQuality_Fine)
        JPEGWriteJPGQulaity(paddress, Quality_High);
    else if(gImageAlgInfo.Quality ==_IMGQuality_Normal)
        JPEGWriteJPGQulaity(paddress, Quality_Economy);
    else if(gImageAlgInfo.Quality ==_IMGQuality_Economy)
        JPEGWriteJPGQulaity(paddress, Quality_Undefine);
    else
        JPEGWriteJPGQulaity(paddress, Quality_Undefine);

    gAlgMsg.JpgExifAddr=paddress;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_JPGEXIFMODIFY, &gAlgMsg);
    }


}

/**
    Get current AF Window Position
    @param  Focus_Window Structure for AF window stx,sty,hsize,vsize
*/
void Get_AF_Window_Position(unsigned char af_pos,FOCUS_WINDOW_INFOR *Focus_Window)//not finish yet
{
    //STR_VA_INFOR *VA_infor;
    unsigned long stx,sty,hsz,vsz;

    //ipe_GetVA_Infor(VA_infor);
    stx = 320/5;
    sty = 240/5;
    hsz = 320/5;
    vsz = 240/5;

    if(af_pos ==_IMGAFPosition_1)
    {
        Focus_Window->stx = stx+hsz;
        Focus_Window->sty = sty;
    }
    else if(af_pos ==_IMGAFPosition_2)
    {
        Focus_Window->stx = stx;
        Focus_Window->sty = sty+vsz;
    }
    else if(af_pos ==_IMGAFPosition_3)
    {
        Focus_Window->stx = stx+2*hsz;
        Focus_Window->sty = sty+vsz;
    }
    else if(af_pos ==_IMGAFPosition_4)
    {
        Focus_Window->stx = stx+hsz;
        Focus_Window->sty = sty+2*vsz;
    }
    else
    {
        Focus_Window->stx = stx+hsz;
        Focus_Window->sty = sty+vsz;
    }
    Focus_Window->hsz = hsz;
    Focus_Window->vsz = vsz;

}

UCHAR Get_CaptureDriverMode(void)
{

    return gImageAlgInfo.DriveMode;
}

void def_AlgCallBackFun(FPALGCALLBACK pAlgcallback)
{
    g_fpAlgExport = pAlgcallback;
}

ER Sen_IsrRegisterEvent(SEN_ISR_FPID IsrId, SEN_ISR_FP EventFp)
{
    if (IsrId > SEN_ISR_FP_NUM)
    {
        return E_SYS;
    }
    gFPIsrEventFunc[IsrId] = EventFp;
    return E_OK;
}

void Sen_IsrEventProc(SEN_ISR_FPID IsrId, UINT32 uiEventStatus)
{
    if (gFPIsrEventFunc[IsrId] != NULL)
    {
        gFPIsrEventFunc[IsrId](uiEventStatus);
    }
}

void Sen_SetPhotoInfo(PHOTO_APPOBJ *PhotoObj)
{
    DispSizeInfo = *PhotoObj;
}

UINT SetIPPSizeInfor(UINT item, UINT value)
{
    UINT *pArray1;

    pArray1=(UINT *) &gIPPSizeInfor;
    *(pArray1+item)=value;
    return E_OK;
}
UINT GetIPPSizeInfor(UINT    item)
{
    UINT value;
    UINT *pArray1;

    pArray1=(UINT *) &gIPPSizeInfor;
    value=    *(pArray1+item);
    return value;
}

IME_FBADDR_PRAM* Sen_GetImePath1Param(void)
{
    return &IME_D1_Para;
}

IME_FBADDR_PRAM* Sen_GetImePath2Param(void)
{
    return &IME_D2_Para;
}
//#NT#20091018#etahnlau -begin
BOOL GetAntishaking_RawbiningEnalbe(void)
{
    return RAWBinning_ENABLE;
}

void SetAntishaking_RawbiningEnalbe(BOOL enable)
{
    RAWBinning_ENABLE = enable;
}
//#NT#20091018#ethanlau -end

//#NT#2010/03/04#Jarkko Chang -begin
//#NT#fixed video recording 15 fps buffer issue
_VideoFps_sel GetVideoFrameRate(void)
{
    return Video_Frame_Rate;
}

void SetVideoFrameRate(_VideoFps_sel VideoFps)
{
    if (VideoFps == _Fps_15)
    {
        Video_Frame_Rate = _Fps_15;
    }
    else
    {
        Video_Frame_Rate = _Fps_30;
    }
}
//#NT#2010/03/04#Jarkko Chang -end

//#NT#2009/10/29#Linkin Huang -begin
//#NT# Data re-arrange using Graphic engine
ER GRAPH_ReArrange(UINT32 width, UINT32 height, UINT32 inAddr)
{
    UINT32 i = 0, rm = 0;
    UINT32 width_8x = 0, height_8x = 0, Tmp_de = 0, TmpWidth = 0;
    UINT32 SourceRot1 = 0, TargetRot1 = 0;

    if((width%8) != 0)
        width_8x  = width + (8 - (width%8)); // multiple of 8
    else
        width_8x = width;

    if((height%8) != 0)
        height_8x = height + (8 - (height%8)); // multiple of 8
    else
        height_8x = height;

    Tmp_de = ((inAddr + (width_8x * height_8x)));

    grph_open();
    //#NT#2009/11/05#Linkin Huang -begin
    //#NT# remove code
    //grph_enableDMA();
    //#NT#2009/11/05#Linkin Huang -end

    grph_setImg1(inAddr, width_8x, height_8x, width_8x);
    grph_setImg2(Tmp_de, height_8x);
    grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);

    grph_setImg1(Tmp_de, height_8x, width_8x, height_8x);
    grph_setImg2(inAddr, height_8x);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    grph_setImg1(inAddr, (height_8x*4), (width_8x/4), (height_8x*4));
    grph_setImg2((Tmp_de+(4*height_8x)), (height_8x*4*2));
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);


    for(i = 0; i < 4; i++)
    {
      grph_setImg1((Tmp_de+(height_8x*(i+4))+1), (height_8x*8), width_8x, (height_8x-1));
      grph_setImg2((Tmp_de+(height_8x*i)), height_8x*8);
      grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }

    TmpWidth = height_8x >> 1;

    grph_setImg1(Tmp_de, height_8x, width_8x*2, TmpWidth);
    grph_setImg2(inAddr, width_8x*2);
    grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);

    // copy
    SourceRot1 = Tmp_de + (height_8x/2);
    TargetRot1 = Tmp_de + (height_8x*width_8x);
    grph_setImg1(SourceRot1, height_8x, width_8x, height_8x/2);
    grph_setImg2(TargetRot1, height_8x);
    grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);

    TmpWidth = (TmpWidth / 8) * 8;
    rm = (height_8x >> 1) - TmpWidth + 1;

    SourceRot1 = Tmp_de + (height_8x*(width_8x)) + (height_8x/2);
    TargetRot1 = Tmp_de;
    grph_setImg1(SourceRot1, height_8x, width_8x, TmpWidth);
    grph_setImg2(TargetRot1, width_8x*2);
    grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);


    SourceRot1 = Tmp_de + (height_8x * width_8x);
    TargetRot1 = Tmp_de + width_8x;
    grph_setImg1(SourceRot1, height_8x, width_8x, TmpWidth);
    grph_setImg2(TargetRot1, width_8x*2);
    grph_setGOP(FALSE, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);


    //Last line
    for(i = rm; i >=1; i--)
    {
      grph_setImg1(inAddr+(width_8x*2*(height-rm-1)), (width_8x*2), 1, width_8x*2);
      grph_setImg2(inAddr+(width_8x*2*(height-i+1)), (width_8x*2));
      grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
    }

    //#NT#2009/11/05#Linkin Huang -begin
    //#NT# remove code
    //grph_disableDMA();
    //grph_swReset();
    //#NT#2009/11/05#Linkin Huang end

    grph_close();

    return E_OK;
}
//#NT#2009/10/29#Linkin Huang -end