/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: SensorSys_MI5100_630.c                                           *
* Description:     Layer 3 API of Alg                                      *
* Author: Adrian                                                           *
****************************************************************************/
#include    "sensor_model.h"
#if MI5100_TVP5150_630
#include    "pll.h"
#include    "interrupt.h"
#include    "IDE.h"
#include    "Top.h"
#include    "sif.h"
#include    "DMA.h"
#include    "Pad.h"
#include    "JpgEnc.h"
#include    "filesystsk.h"
#include    "OSCommon.h"
#include    "Exif.h"
#include    "grph.h"
#include    "cal_api.h"
#include    "AlbumShot.h"
#include    "Cal_Alg.h"
#include    "AF_Lib.h"
#include    "Caf_lib.h"
//#include    "ColorShading.h"

#define CCIR656_EN 0
#if(SENSOR_GRS_MODE == 1)
volatile BOOL GRS_MODE = TRUE;
 #else
volatile BOOL GRS_MODE = FALSE;
#endif

//extern AF_INFO g_AFInfo;
//#NT#2009/10/27#Jarkko Chang -begin
//#NT#add for csc
//UINT g_CS_GainTblBufAdd[9555];//14400];  //80x60x3
//#NT#2009/10/27#Jarkko Chang -end

#define _DONTCARE 255
#define Total_UICondition    31
#define Total_UIItem        8
int IMGUI_FALSE_SET[Total_UICondition][Total_UIItem]={
//SceneMode                Quality                DriveMode                    FlashMode                    WBMode                    AEEVValue    FileFmt         _SieBitDepth
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _IMGFlashMode_Auto,            _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//0
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _IMGFlashMode_ForceON,        _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//1
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _IMGFlashMode_AutoRedEye,    _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//2
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _IMGFlashMode_ForceONRedEye,_DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//3
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _IMGFlashMode_NightStrobe,    _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//4
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _IMGFlashMode_Auto,            _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//5
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _IMGFlashMode_ForceON,        _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//6
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _IMGFlashMode_AutoRedEye,    _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//7
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _IMGFlashMode_ForceONRedEye,_DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//8
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _IMGFlashMode_NightStrobe,    _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//9
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _IMGFlashMode_Auto,            _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//10
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _IMGFlashMode_ForceON,        _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//11
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _IMGFlashMode_AutoRedEye,    _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//12
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _IMGFlashMode_ForceONRedEye,_DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//13
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _IMGFlashMode_NightStrobe,    _DONTCARE,                _DONTCARE,    _DONTCARE,      _DONTCARE},//14
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//15
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//16
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//17
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//18
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//19
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//20
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _DONTCARE,                    _DONTCARE,                _DONTCARE,    _FileFmt_RawJpg,_DONTCARE},//21
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_10},//22
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                  _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_12},//23
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_Burst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_16},//24
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_10},//25
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_12},//26
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_InfBurst,        _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_16},//27
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_10},//28
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_12},//29
{_DONTCARE,                _DONTCARE,            _IMGDriveMode_SmallBurst16,    _DONTCARE,                    _DONTCARE,                _DONTCARE,    _DONTCARE,      _SieBitDepth_16},//30
};

const UINT32 CapBufAlloc[Max_BurstNum][3]=
{
    //{raw, jpeg, CapCnt},
    {0x01, 0x01, 0x01},  //_BurstNum_Dummy
    {0x01, 0x02, 0x01},  //_BurstNum_1
    {0x01, 0x02, 0x02},  //_BurstNum_2
    {0x01, 0x02, 0x03},  //_BurstNum_3
    {0x01, 0x02, 0x04},  //_BurstNum_4
    {0x01, 0x02, 0x05},  //_BurstNum_5
    {0x01, 0x02, 0xff},  //_BurstNum_Inf
};

volatile UINT32 DZoom_Flag = DZOOM_IDLE;
//#NT#2009/10/20#Jarkko Chang -begin
//#NT#add for dzoom using
extern const UINT8 gucDzoomTable[];
//#NT#2009/10/20#Jarkko Chang -end
//--------------------------------------------------------------------------------------------------------------------------
STR_SIE_DVI_INFO SIE_DVI_PARAM;

#if 0
#pragma mark -
#endif

UINT32 GammaInverse(UINT8 InputLum)
{
    UINT32 i,Y_Pregamma;
    UINT32 Weight;
debug_err(("GammaInverse  gImageAlgInfo.TvInFormat = %d\n\r",gImageAlgInfo.TvInFormat));

if ( gImageAlgInfo.TvInFormat == _TvInFormat_OFF )
{
    for(i=1; i<129; i++)
    {
        if(InputLum <GammaLUT_128Tab[i]/4)
        {
            //OutY=i-1;
            break;
        }
    }
    Weight = (InputLum-GammaLUT_128Tab[i-1]/4)*100*4/((GammaLUT_128Tab[i]-GammaLUT_128Tab[i-1]));


    Y_Pregamma = (i-1)*8 + Weight*8/100;

    //debug_ind(("in=%d, out=%d\r\n",InputLum,Y_Pregamma));

    return Y_Pregamma;
}else{
	    return 0;
}
}

//#NT#20090918#ethanlau -begin
UINT32  SensorModeMap(_IMGResolution_sel _mode)
{

    static UINT32 PrvTVmode,CurTVmode;
debug_err(("SensorModeMap  gImageAlgInfo.TvInFormat = %d\n\r",gImageAlgInfo.TvInFormat));

//##for DVI MODE## begin
    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if ( gImageAlgInfo.TvInFormat == _TvInFormat_NTSC )
	{
	    	UI_SetParameter(_ImageRatio, _Ratio_4_3);
        	return NTSC_MODE;
        	//return MONITOR_MODE;

	}
    else if ( gImageAlgInfo.TvInFormat == _TvInFormat_PAL )
	{
		UI_SetParameter(_ImageRatio, _Ratio_4_3);
        	return PAL_MODE;
        	//return MONITOR_MODE;

	}
    //#NT#2011/08/25#Spark Chou -end
    else
//##for DVI MODE## end
    {
	    switch(_mode)
	    {
	        case _Resolution_320x240:
	        case _Resolution_640x480:

	            UI_SetParameter(_ImageRatio, _Ratio_4_3);
	            return MONITOR_MODE;
	        case _Resolution_4352x3267:  //prv
	        case _Resolution_3264x2448:  //prv
	        case _Resolution_2592x1944:  //prv
	        case _Resolution_2048x1536:  //prv
	            UI_SetParameter(_ImageRatio, _Ratio_4_3);
	            return MONITOR_MODE_2;
		 case _Resolution_848x480:
	        case _Resolution_1280x720:
	        case _Resolution_3648x2048:  //prv 16:9
	        case _Resolution_1920x1080:  //prv 16:9
	            UI_SetParameter(_ImageRatio, _Ratio_16_9);
	            return MONITOR_MODE_2;
	        case _Resolution_1440x1080:
	            UI_SetParameter(_ImageRatio, _Ratio_4_3);
	          //  UI_SetParameter(_ImageRatio, _Ratio_16_9);
	            return MONITOR_MODE_2;
	        default:
	            UI_SetParameter(_ImageRatio, _Ratio_4_3);
	            return MONITOR_MODE;
	        break;
	    }
    	}
}
//#NT#20090918#ethanlau -end

UINT32 testdpFirst = FALSE;

void Set2Preview(IPL_OBJ *Obj)
{
    Cal_Struct *CalInfo;
    UINT32 i;

    CalInfo = Cal_GetCalibrationData();
    debug_err(("Set2Preview\r\n"));
    //AppInit_Protect();
    if (Obj->uiCmd & IPL_CMD_CHG_SIZE_IME1)
    {
        DispSizeInfo.uiDisplayFBWidth = Obj->uiImeOutWidth1;
        DispSizeInfo.uiDisplayFBHeight = Obj->uiImeOutHeight1;
    }

    gAlgMsg.Alg_Mode=MODE_Preview;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_PREVIEW, &gAlgMsg);
    }

    //#NT#20090918#ethanlau -begin
//    SetIPPSizeInfor(_Prv_ResolutionIdx,gImageAlgInfo.Prv_Resolution);
//    SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Prv_ResolutionIdx));//SENSOR_PRV_MODE;

    if (IPL_GetNextMode() == IPL_MODE_PREVIEW)
    {
        debug_err(("\r\nSensorMode CHG(PRV) , from %d to ",GetIPPSizeInfor(_Prv_ResolutionIdx)));
        SetIPPSizeInfor(_Prv_ResolutionIdx,gImageAlgInfo.Prv_Resolution);
        debug_err(("%d \r\n",GetIPPSizeInfor(_Prv_ResolutionIdx)));
        SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Prv_ResolutionIdx));//SENSOR_PRV_MODE;
    }
    else //if (IPL_GetNextMode() == IPL_MODE_VIDEO)
    {
        debug_err(("\r\nSensorMode CHG(VID) , from %d to ",GetIPPSizeInfor(_Vid_ResolutionIdx)));
        SetIPPSizeInfor(_Vid_ResolutionIdx,gImageAlgInfo.Vid_Resolution);
        debug_err(("%d \r\n",GetIPPSizeInfor(_Vid_ResolutionIdx)));
        SIE_PRE_Para_prv.ModeSel =  SensorModeMap(GetIPPSizeInfor(_Vid_ResolutionIdx));//SENSOR_PRV_MODE;
    }
//    SIE_PRE_Para_prv.ModeSel = SENSOR_PRV_MODE;
    //#NT#20090918#etahnlau -end

    SIE_PRE_Para_prv.VGAType = SENSOR_PRV_VGATYPE;
    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);
    Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);
	debug_err(("Set2Preview  gImageAlgInfo.TvInFormat = %d\n\r",gImageAlgInfo.TvInFormat));

    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    SIE_PRE_Para_prv.SieBp2 = PRV_SIE_BP2;
    SIE_PRE_Para_prv.PreIntEn = PRE_INTE_FLDEND|PRE_INTE_BP1;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;
//#NT#2009/10/16#Jarkko Chang -begin
    SIE_PRE_Para_prv.PreSrcSel = PATH_POST_GAMMA;
    SIE_PRE_Para_prv.PreFuncEn =  PRE_GAMMA_EN|PRE_CA_EN|PRE_CG_EN;//|PRE_VIG_EN;
    SIE_PRE_Para_prv.PreCfaPat = CFAPAT_PRV;
    SIE_PRE_Para_prv.PreCfaPat2 = CFAPAT_PRV;
    SIE_PRE_Para_prv.SieVDInv = Invert;
    SIE_PRE_Para_prv.SieHDInv = Invert;

    ippbuf_ExecuteAssigner(IPPBUF_MODE_PRV);

    if (SIE_PRE_Para_prv.AutoObEn == DISABLE)
    {
        SetIPPSizeInfor(_OBOffset, OB_OFS_PRV);
    }
    else
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
    }
    debug_err(("CalStatus_PrvDP_B = 0x%x,CalStatus_PrvDP_W = 0x%x,ModeSel  = %d\n\r",CalInfo->CalStatus_PrvDP_B,CalInfo->CalStatus_PrvDP_W,SIE_PRE_Para_prv.ModeSel ));

    if ((CalInfo->CalStatus_PrvDP_B == _Prv_B_Status) && (CalInfo->CalStatus_PrvDP_W == _Prv_W_Status))
    {
        SIE_PRE_Para_prv.SiePreAddr2 = CalInfo->Cal_DP_PreviewAddr;

        SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
    }

    //initial ipe parameter
    IPE_Para.CfaPat = CFAPAT_PRV;
    IPE_Para.DVIFormat = DVI_FORMAT;
    IPE_Para.DVIHsub = DVI_HSUB;

    //#NT#2010/04/30#Jarkko Chang -begin
    //#NT#add for stripe mode change
    if (SIE_PRE_Para_prv.ModeSel == MONITOR_MODE)
    {
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPE_Para.StripeMode = IPE_SSTP;
    }
    else
    {
        IME_D1_Para.ime_sten = IMEALG_MULTI;
        IPE_Para.StripeMode = IPE_MSTP;
    }
    //    debug_msg("\r\n------->Set2Preview stripe mode %d\r\n",IPE_Para.StripeMode);
    //#NT#2010/04/30#Jarkko Chang -end

    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        debug_err(("^RDVI_MODE_EN\r\n"));
        SetIPPSizeInfor(_OBOffset, 0x0);
        SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
        SIE_PRE_Para_prv.PreFuncEn = DISABLE;
        //#NT#2012/01/03#Jarkko Chang -begin
        //#NT# modify DVI MST setting
        //Single stripe
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPE_Para.StripeMode = IPE_DVIMODE;
        //Multi stripe
        //IME_D1_Para.ime_sten = IMEALG_MULTI;
        //IPE_Para.StripeMode = IPE_MSTP;
        //#NT#2012/01/03#Jarkko Chang -end
        IPE_Para.DVIFormat = DVI_FORMAT;
        IPE_Para.DVIHsub = DVI_HSUB;
        SIE_PRE_Para_prv.SieVDInv = Notinvert;
        SIE_PRE_Para_prv.SieHDInv = Notinvert;
        SIE_PRE_Para_prv.SieHDPhase = SieLoad_RisingEdge ;
        SIE_PRE_Para_prv.SieVDPhase = SieLoad_RisingEdge;
        SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;

#if CCIR656_EN
        //#NT#2011/11/07#YHLee -begin
        //#NT#Add DVI API
        SIE_DVI_PARAM.DVIFormat = DVI_CCIR_656 ;
        SIE_DVI_PARAM.DVIMode = DVI_SD_MODE;
        SIE_DVI_PARAM.DVI656ActEnd = ACT_END_EAV_CODE;
        sie_setDVI(&SIE_DVI_PARAM);
#endif
    }
    //#NT#2011/08/25#Spark Chou -end

    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.Ipe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct = &IME_D1_Para;
    Set2Prv_Common(&Ipp_Param);
    sie_waitVD(1);
    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
//#NT#2009/12/28#Jarkko Chang -begin
//#NT#move to SensorMode() to aviod ide disable/enable twice.

    if(g_fpAlgExport)
    {
        WaitAeStable(3);
        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
    }
//#NT#2009/12/28#Jarkko Chang -end
}

//#NT#20090912#ethanlau -begin
void SensorMode(IPL_MODE mode)
{
    PImageAlgInfo  pimgalginfo;
    static UINT ModeSel,imgratio;
    Cal_Struct *CalInfo;

    //#NT#2011/11/21#Spark Chou -begin
    //#NT# DVI Mode
    static UINT32 PrvTVmode=_TvInFormat_OFF,CurTVmode;

    if (GetIPPFuncInfor(DVI_MODE_EN) == TRUE)
    {
        PrvTVmode = gImageAlgInfo.TvInFormat;
        CurTVmode=GetCurretInputMode();
        if(CurTVmode!=PrvTVmode)
        {
            gImageAlgInfo.TvInFormat = CurTVmode;
            PrvTVmode = CurTVmode;
        }
        else
        {
            return ;
        }
    }
    //#NT#2011/11/21#Spark Chou -end

    CalInfo = Cal_GetCalibrationData();
    if((mode != IPL_MODE_PREVIEW) && (mode != IPL_MODE_VIDEO))
    {
        debug_err(("Only support IPL_MODE_PREVIEW|IPL_MODE_VIDEO!\r\n"));
        return;
    }

    pimgalginfo = UI_GetParameter();
    ModeSel = SIE_PRE_Para_prv.ModeSel;
    imgratio = pimgalginfo->ImageRatio;

    if (mode == IPL_MODE_PREVIEW)
    {
        debug_msg("\r\nSensorMode CHG(PRV) , from %d to ",GetIPPSizeInfor(_Prv_ResolutionIdx));
        SetIPPSizeInfor(_Prv_ResolutionIdx,gImageAlgInfo.Prv_Resolution);
        debug_msg("%d \r\n",GetIPPSizeInfor(_Prv_ResolutionIdx));
        SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Prv_ResolutionIdx));//SENSOR_PRV_MODE;
    }
    else //if (IPL_GetNextMode() == IPL_MODE_VIDEO)
    {
        debug_msg("\r\nSensorMode CHG(VID) , from %d to ",GetIPPSizeInfor(_Vid_ResolutionIdx));
        SetIPPSizeInfor(_Vid_ResolutionIdx,gImageAlgInfo.Vid_Resolution);
        debug_msg("%d \r\n",GetIPPSizeInfor(_Vid_ResolutionIdx));
        SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Vid_ResolutionIdx));//SENSOR_PRV_MODE;
    }

    if( (ModeSel == SIE_PRE_Para_prv.ModeSel)&&(imgratio == pimgalginfo->ImageRatio))
    {
        debug_msg("sensor mode and ratio are same with previous\r\n");
        if(g_fpAlgExport)
        {
            g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
        }
        return;
    }

    gSenCBInfo.IPPImgMode = IPPImg_SenModeChg;
    SenFlowFunc(AAFD_Switch,&gSenCBInfo);

    StopPreviewDramData();
    SIE_PRE_Para_prv.VGAType = SENSOR_PRV_VGATYPE;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;
    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);
    Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);
    debug_err(("CalStatus_PrvDP_B = 0x%x,CalStatus_PrvDP_W = 0x%x,ModeSel  = %d\n\r",CalInfo->CalStatus_PrvDP_B,CalInfo->CalStatus_PrvDP_W,SIE_PRE_Para_prv.ModeSel ));

    if ((CalInfo->CalStatus_PrvDP_B == _Prv_B_Status) && (CalInfo->CalStatus_PrvDP_W == _Prv_W_Status))
    {
        SIE_PRE_Para_prv.SiePreAddr2 = CalInfo->Cal_DP_PreviewAddr;
        SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
    }
    //#NT#2010/04/30#Jarkko Chang -begin
    //#NT#add for stripe mode change
    if (SIE_PRE_Para_prv.ModeSel == MONITOR_MODE)
    {
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPE_Para.StripeMode = IPE_DVIMODE;
    }
    else
    {
        IME_D1_Para.ime_sten = IMEALG_MULTI;
        IPE_Para.StripeMode = IPE_MSTP;
    }
    //#NT#2010/04/30#Jarkko Chang -end

    //#NT#2011/09/09#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
        SIE_PRE_Para_prv.PreFuncEn = DISABLE;
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPE_Para.StripeMode = IPE_DVIMODE;
        IPE_Para.DVIFormat = DVI_FORMAT;
        IPE_Para.DVIHsub = DVI_HSUB;

#if CCIR656_EN
        //#NT#2011/11/07#YHLee -begin
        //#NT#Add DVI API
        SIE_DVI_PARAM.DVIFormat = DVI_CCIR_656 ;
        SIE_DVI_PARAM.DVIMode = DVI_SD_MODE;
        SIE_DVI_PARAM.DVI656ActEnd = ACT_END_EAV_CODE;
        sie_setDVI(&SIE_DVI_PARAM);
#endif
    }
    //#NT#2011/09/09#Spark Chou -end

    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.Ipe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct = &IME_D1_Para;


    Set2Prv_Common(&Ipp_Param);

    sie_waitVD(1);

    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch,&gSenCBInfo);

    if(g_fpAlgExport)
    {
//        WaitAeStable(2);
        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
    }
}
//#NT#20090912#ethanlau -end

void Set2VideoLiveview(IPL_OBJ *Obj)
{
    gAlgMsg.Alg_Mode=MODE_Preview;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_PREVIEW, &gAlgMsg);
    }

    StartPreviewDramData();


    if (Obj->uiCmd & IPL_CMD_CHG_SIZE_IME1)
    {
        DispSizeInfo.uiDisplayFBWidth = Obj->uiImeOutWidth1;
        DispSizeInfo.uiDisplayFBHeight = Obj->uiImeOutHeight1;
    }

    Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);
    IME_D1_Para.y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    IME_D1_Para.cb0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y1_addr=IME_D1_Para.y0_addr+2*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y2_addr=IME_D1_Para.y0_addr+4*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);

    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IME_D1_Para.in_format=IME_IN_FMT_422;
    IME_D1_Para.out_format=IME_OUT_FMT_422_COSITED;
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);

    //#NT#2011/09/08#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        //IME_D1_Para.in_h = (GetIPPSizeInfor(_IpeOutHsize) + 16)>>1;
        IME_D1_Para.in_h = GetIPPSizeInfor(_PreOutHsize)>>1;
    }
    //#NT#2011/08/25#Spark Chou -end

    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    ime_Set2Preview(&IME_D1_Para);
    if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
    {
        SenFlowFunc(AE_PRV_SETTING, &gSenCBInfo);
        SenFlowFunc(AWB_PRV_SETTING, &gSenCBInfo);
    }

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
}

void Set2Video(IPL_OBJ *Obj)
{
    gAlgMsg.Alg_Mode=MODE_Video_Recording;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_VIDEO_RECORDING, &gAlgMsg);
    }

//#NT#2010/05/14#Jarkko Chang -begin
//#NT#Add for video change gamma
    pre_waitFrameEnd(TRUE);
    CPU_WriteGammaTab(GammaLUT_Vid_128Tab, GammaLUT_Vid_128Tab, GammaLUT_Vid_128Tab);
//#NT#2010/05/14#Jarkko Chang -end

    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        sie_SensorDataEnable(ON);
        sie_Load();
    }

    StartPreviewDramData();
    if (Obj->uiCmd & IPL_CMD_CHG_SIZE_IME2)
    {
        SetIPPSizeInfor(_ImeOut2Hsize, Obj->uiImeOutWidth2);
        SetIPPSizeInfor(_ImeOut2Vsize, Obj->uiImeOutHeight2);
        SetIPPSizeInfor(_ImeOut2LineOffset, Obj->uiImeOutWidth2);
    }
    else
    {
        SetIPPSizeInfor(_ImeOut2Hsize, GetIPPSizeInfor(_ImeOut1Hsize));
        SetIPPSizeInfor(_ImeOut2Vsize, GetIPPSizeInfor(_ImeOut1Vsize));
        SetIPPSizeInfor(_ImeOut2LineOffset, GetIPPSizeInfor(_ImeOut1LineOffset));
    }

    ippbuf_ExecuteAssigner(IPPBUF_MODE_VDO);
    IME_D1_Para.y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    IME_D1_Para.cb0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y1_addr=IME_D1_Para.y0_addr+2*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y2_addr=IME_D1_Para.y0_addr+4*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);

    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IME_D1_Para.in_format=IME_IN_FMT_422;
    IME_D1_Para.out_format=IME_OUT_FMT_422_COSITED;
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);

    IME_D2_Para.y0_addr = ippbuf_GetBuf(IPPBUF_PRV_IME_P2);
    IME_D2_Para.cb0_addr=(IME_D2_Para.y0_addr+GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize));
    IME_D2_Para.cr0_addr=(IME_D2_Para.y0_addr+GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize)*3/2);
    IME_D2_Para.y1_addr=IME_D2_Para.y0_addr+2*GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize);
    IME_D2_Para.cb1_addr=(IME_D2_Para.y1_addr+GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize));
    IME_D2_Para.cr1_addr=(IME_D2_Para.y1_addr+GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize)*3/2);
    IME_D2_Para.y2_addr=IME_D2_Para.y0_addr+4*GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize);
    IME_D2_Para.cb2_addr=(IME_D2_Para.y2_addr+GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize));
    IME_D2_Para.cr2_addr=(IME_D2_Para.y2_addr+GetIPPSizeInfor(_ImeOut2Hsize)*GetIPPSizeInfor(_ImeOut2Vsize)*3/2);

    IME_D2_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D2_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D2_Para.out_h=GetIPPSizeInfor(_ImeOut2Hsize);
    IME_D2_Para.out_v=GetIPPSizeInfor(_ImeOut2Vsize);
    IME_D2_Para.in_format=IME_IN_FMT_422;
    IME_D2_Para.out_format=GetImeVideoFormat();
    IME_D2_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut2LineOffset);


    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        //IPE_Para.InSizeH = (GetIPPSizeInfor(_IpeOutHsize) + 16) >> 1;
        IPE_Para.InSizeH = GetIPPSizeInfor(_PreOutHsize)>>1;
        IME_D1_Para.in_h = IPE_Para.InSizeH;
        IME_D2_Para.in_h = IPE_Para.InSizeH;
    }
    //#NT#2011/08/25#Spark Chou -end


    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    ime_Set2Video(&IME_D1_Para,&IME_D2_Para);

    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
    {
        SenFlowFunc(AE_PRV_SETTING, &gSenCBInfo);
        SenFlowFunc(AWB_PRV_SETTING, &gSenCBInfo);
    }
    //#NT#2011/08/25#Spark Chou -end

    gSenCBInfo.IPPImgMode = IPPImg_Video;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
}


void Set2Pcc(IPL_OBJ *Obj)
{
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();
    debug_err(("^R\r\n\r\nPCC Mode\r\n\r\n"));
    if (Obj->uiCmd & IPL_CMD_CHG_SIZE_IME1)
    {
        DispSizeInfo.uiDisplayFBWidth = Obj->uiImeOutWidth1;
        DispSizeInfo.uiDisplayFBHeight = Obj->uiImeOutHeight1;
    }

    gAlgMsg.Alg_Mode=MODE_PCCam;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_PREVIEW, &gAlgMsg);
    }

    //#NT#20090918#ethanlau -begin
    SetIPPSizeInfor(_Prv_ResolutionIdx,gImageAlgInfo.Prv_Resolution);
    SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Prv_ResolutionIdx));//SENSOR_PRV_MODE;
    //#NT#20090918#etahnlau -end
    SIE_PRE_Para_prv.VGAType = SENSOR_PRV_VGATYPE;

    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);
    Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);

    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    SIE_PRE_Para_prv.SieBp2 = PRV_SIE_BP2;
    SIE_PRE_Para_prv.PreIntEn = PRE_INTE_FLDEND|PRE_INTE_BP1;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;
//    SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
//    SIE_PRE_Para_prv.PreFuncEn = DISABLE;
    SIE_PRE_Para_prv.PreSrcSel = PATH_POST_GAMMA;
    SIE_PRE_Para_prv.PreFuncEn = PRE_GAMMA_EN|PRE_CA_EN|PRE_CG_EN;//|PRE_VIG_EN;
    SIE_PRE_Para_prv.PreCfaPat = CFAPAT_PRV;
    SIE_PRE_Para_prv.PreCfaPat2 = CFAPAT_PRV;
    SIE_PRE_Para_prv.SieVDInv = Invert;
    SIE_PRE_Para_prv.SieHDInv = Invert;

    ippbuf_ExecuteAssigner(IPPBUF_MODE_PRV);

    if (SIE_PRE_Para_prv.AutoObEn == DISABLE)
    {
        SetIPPSizeInfor(_OBOffset, OB_OFS_PRV);
    }
    else
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
    }

    if ((CalInfo->CalStatus_PrvDP_B == _Prv_B_Status) && (CalInfo->CalStatus_PrvDP_W == _Prv_W_Status))
    {
        SIE_PRE_Para_prv.SiePreAddr2 = CalInfo->Cal_DP_PreviewAddr;
        SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
    }

    //initial ipe parameter
    IPE_Para.CfaPat = CFAPAT_PRV;

    //#NT#2010/04/30#Jarkko Chang -begin
    //#NT#add for stripe mode change
    if (SIE_PRE_Para_prv.ModeSel == MONITOR_MODE)
    {
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPE_Para.StripeMode = IPE_SSTP;
    }
    else
    {
        IME_D1_Para.ime_sten = IMEALG_MULTI;
        IPE_Para.StripeMode = IPE_MSTP;
    }
    //    debug_msg("\r\n------->Set2Preview stripe mode %d\r\n",IPE_Para.StripeMode);
    //#NT#2010/04/30#Jarkko Chang -end


    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
        SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
        SIE_PRE_Para_prv.PreFuncEn = DISABLE;
        //#NT#2012/01/03#Jarkko Chang -begin
        //#NT# modify DVI MST setting
        //Single stripe
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPE_Para.StripeMode = IPE_DVIMODE;
        //Multi stripe
        //IME_D1_Para.ime_sten = IMEALG_MULTI;
        //IPE_Para.StripeMode = IPE_MSTP;
        //#NT#2012/01/03#Jarkko Chang -end
        IPE_Para.DVIFormat = DVI_FORMAT;
        IPE_Para.DVIHsub = DVI_HSUB;
        SIE_PRE_Para_prv.SieVDInv = Notinvert;
        SIE_PRE_Para_prv.SieHDInv = Notinvert;
        SIE_PRE_Para_prv.SieHDPhase = SieLoad_RisingEdge ;
        SIE_PRE_Para_prv.SieVDPhase = SieLoad_RisingEdge;
        SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;

#if CCIR656_EN
        //#NT#2011/11/07#YHLee -begin
        //#NT#Add DVI API
        SIE_DVI_PARAM.DVIFormat = DVI_CCIR_656 ;
        SIE_DVI_PARAM.DVIMode = DVI_SD_MODE;
        SIE_DVI_PARAM.DVI656ActEnd = ACT_END_EAV_CODE;
        sie_setDVI(&SIE_DVI_PARAM);
#endif

    }
    //#NT#2011/08/25#Spark Chou -end

    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.Ipe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct = &IME_D1_Para;
    Set2Prv_Common(&Ipp_Param);
    sie_waitVD(1);

    ime_clrIntFlag();
    ime_waitFlagFrameEnd();
    ime_waitFlagFrameEnd();
    ime_waitFlagFrameEnd();

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
    }
}

void SetDzoom2Video(unsigned int ZoomIndex)
{
    Set_digitalzoom(ZoomIndex, &SIE_PRE_Para_prv);

    SIE_PRE_Para_prv.PreCropSizeH = GetIPPSizeInfor(_PreCropHsize);
    SIE_PRE_Para_prv.PreCropSizeV = GetIPPSizeInfor(_PreCropVsize);
    SIE_PRE_Para_prv.PreOutSizeH = GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF;
    SIE_PRE_Para_prv.PreOutLineOfs = GetIPPSizeInfor(_PreOutLineOffset);

    IPE_Para.InSizeH = SIE_PRE_Para_prv.PreOutSizeH;
    IPE_Para.InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    IPE_Para.InLineOfs = SIE_PRE_Para_prv.PreOutLineOfs;

    IME_D1_Para.y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    IME_D1_Para.cb0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y1_addr=IME_D1_Para.y0_addr+2*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y2_addr=IME_D1_Para.y0_addr+4*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);

    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IME_D1_Para.in_format=IME_IN_FMT_422;
    IME_D1_Para.out_format=IME_OUT_FMT_422_COSITED;
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);

    IME_D2_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D2_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D2_Para.out_h=GetIPPSizeInfor(_ImeOut2Hsize);
    IME_D2_Para.out_v=GetIPPSizeInfor(_ImeOut2Vsize);
    IME_D2_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut2LineOffset);


    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        IPE_Para.InSizeH = SIE_PRE_Para_prv.PreOutSizeH >> 1;
        IME_D1_Para.in_h = IPE_Para.InSizeH;
        IME_D2_Para.in_h = IPE_Para.InSizeH;
        DVISkipFrmEn = DISABLE;
        //TimerDelayMs(30);
        sie_waitVD(1);
        sie_SensorDataEnable(ON);
        sie_Load();

        Clr_DZoomFlag(DZOOM_IDLE);
        Set_DZoomFlag(DZOOM_SET_SIEPRE);
        while(!(Get_DZoomFlag() & DZOOM_IDLE));

        DVISkipFrmEn = ENABLE;
    }
    else
    {
        Clr_DZoomFlag(DZOOM_IDLE);
        Set_DZoomFlag(DZOOM_SET_SIEPRE);
        while(!(Get_DZoomFlag() & DZOOM_IDLE));
    }
    //#NT#2011/08/25#Spark Chou -end


    gAlgMsg.DzoomIndex=ZoomIndex;
//#NT#2009/10/20#Jarkko Chang -begin
    gAlgMsg.DzoomRealRatio = gucDzoomTable[ZoomIndex-10];
//#NT#2009/10/20#Jarkko Chang -end
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_CHGDZOOMSTEP, &gAlgMsg);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void SetDzoom2Preview(unsigned int ZoomIndex)
{
    gSenCBInfo.IPPImgMode = IPPImg_DzoomEnter;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    Set_digitalzoom(ZoomIndex, &SIE_PRE_Para_prv);

    SIE_PRE_Para_prv.PreCropSizeH = GetIPPSizeInfor(_PreCropHsize);
    SIE_PRE_Para_prv.PreCropSizeV = GetIPPSizeInfor(_PreCropVsize);
    SIE_PRE_Para_prv.PreOutSizeH = GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF;
    SIE_PRE_Para_prv.PreOutLineOfs = GetIPPSizeInfor(_PreOutLineOffset);

    IPE_Para.InSizeH = SIE_PRE_Para_prv.PreOutSizeH;
    IPE_Para.InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    IPE_Para.InLineOfs = SIE_PRE_Para_prv.PreOutLineOfs;

    if (ASModeIsEnable() == ENABLE)
    {
        //because Set_digitalzoom function already modified ImePath1 output parameters.
        //must reload original parameters.
        SetIPPSizeInfor(_ImeOut1Hsize,IME_D1_Para.out_h);
        SetIPPSizeInfor(_ImeOut1Vsize,IME_D1_Para.out_v);
        SetIPPSizeInfor(_ImeOut1LineOffset,IME_D1_Para.out_lineoffset);
    }
    else
    {
        IME_D1_Para.y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
        IME_D1_Para.cb0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
        IME_D1_Para.cr0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
        IME_D1_Para.y1_addr=IME_D1_Para.y0_addr+2*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
        IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
        IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
        IME_D1_Para.y2_addr=IME_D1_Para.y0_addr+4*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
        IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
        IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    }

    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IME_D1_Para.in_format=IME_IN_FMT_422;
    IME_D1_Para.out_format=IME_OUT_FMT_422_COSITED;
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);

    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        SIE_PRE_Para_prv.PreOutSizeH = GetIPPSizeInfor(_PreOutHsize);
        IPE_Para.InSizeH = SIE_PRE_Para_prv.PreOutSizeH >> 1;
        IME_D1_Para.in_h = SIE_PRE_Para_prv.PreOutSizeH >> 1; //SIE_PRE_Para_prv.PreOutSizeH//IPE_Para.InSizeH;

        DVISkipFrmEn = DISABLE;
        //TimerDelayMs(30);
        sie_waitVD(1);
        sie_SensorDataEnable(ON);
        sie_Load();

        Clr_DZoomFlag(DZOOM_IDLE);
        Set_DZoomFlag(DZOOM_SET_SIEPRE);
        while(!(Get_DZoomFlag() & DZOOM_IDLE));
        DVISkipFrmEn = ENABLE;
    }
    else
    {
        CHKPNT;
        Clr_DZoomFlag(DZOOM_IDLE);
        Set_DZoomFlag(DZOOM_SET_SIEPRE);
        while(!(Get_DZoomFlag() & DZOOM_IDLE));
    }
    //#NT#2011/08/25#Spark Chou -end

    gSenCBInfo.IPPImgMode = IPPImg_DzoomLeave;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    gAlgMsg.DzoomIndex=ZoomIndex;
//#NT#2009/10/20#Jarkko Chang -begin
    gAlgMsg.DzoomRealRatio = gucDzoomTable[ZoomIndex-10];
//#NT#2009/10/20#Jarkko Chang -end
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_CHGDZOOMSTEP, &gAlgMsg);
    }
}

#if 0
#pragma mark -
#endif

void AF_PrvModeSel(UINT32 PrvModeSel)
{
}

#if 0
#pragma mark -
#endif

#if 1
void CaptureOne(SIE_PRE_PARAM *SiePreParam)
{
    Cal_Struct *CalInfo;
    UINT32 Addr1, Addr2,i;

    CalInfo = Cal_GetCalibrationData();

    if (GetIPPFuncInfor(DFS_EN))
    {
        SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
        Addr1 = SiePreParam->PreOutAdd0;
        Addr2 = SiePreParam->SiePreAddr2;
        SiePreParam->PreOutAdd0 = Addr2;

          debug_err(("CalInfo->CalStatus_CapDP_B = 0x%x,CalInfo->CalStatus_CapDP_W = 0x%x\n\r",CalInfo->CalStatus_CapDP_W,CalInfo->CalStatus_CapDP_W));

        if ((CalInfo->CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo->CalStatus_CapDP_W == _Cap_W_Status))
        {
            SiePreParam->SiePreAddr2 = CalInfo->Cal_DP_CaptureAddr;
            SiePreParam->PreFuncEn |= PRE_BLM_EN;
            SiePreParam->PreFuncEn |= PRE_DF_EMBED_EN;
        }

        sie_Set2Capture(SiePreParam);
        Sensor_Cap2Prv();

        SetIPPFuncInfor(DFS_CNT,DFS_SECONDPIC);
        SiePreParam->PreSrcSel = PATH_PRE_GAMMA;
        SiePreParam->PreOutAdd0 = Addr1;
        SiePreParam->SiePreAddr2 = Addr2;
        sie_Set2Capture(SiePreParam);
        Sensor_Cap2Prv();
        SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
    }
    else
    {
          debug_err(("CalInfo->CalStatus_CapDP_B = 0x%x,CalInfo->CalStatus_CapDP_W = 0x%x\n\r",CalInfo->CalStatus_CapDP_B,CalInfo->CalStatus_CapDP_W));
        Addr2 = SiePreParam->SiePreAddr2;
        if ((CalInfo->CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo->CalStatus_CapDP_W == _Cap_W_Status))
        {
            SiePreParam->SiePreAddr2 = CalInfo->Cal_DP_CaptureAddr;
            SiePreParam->PreFuncEn |= PRE_BLM_EN;
        }
	#if 0
   debug_err(("\n\rDP capture*******************************************************\n\r"));
debug_err(("SiePreParam->SiePreAddr2 = %x, SiePreParam->PreFuncEn = %x\n\r\n", SiePreParam->SiePreAddr2,SiePreParam->PreFuncEn));

       for(i = 0; i < 100; i++)
       {

		debug_err(("%x", *( (UB*)(SiePreParam->SiePreAddr2+i) ) ));
		if((i % 8 == 0) && (i>0))
		  debug_err(("\n\r"));
	 }
	debug_err(("\n\rDP write*******************************************************\n\r"));
#endif
        sie_Set2Capture(SiePreParam);
        Sensor_Cap2Prv();
        SiePreParam->SiePreAddr2 = Addr2;
    }

    if(gImageAlgInfo.FileFmt & _FileFmt_Raw)
    {
        debug_err(("CAP Raw H%d V%d\r\n", SiePreParam->PreOutLineOfs, SiePreParam->PreCropSizeV));
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)SiePreParam->PreOutAdd0,
                        SiePreParam->PreCropSizeV * SiePreParam->PreOutLineOfs,
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
    }

}
#else   //add csc
BOOL bSHOT3PIC = FALSE;
BOOL bCSCBayer = FALSE;
void CaptureOne(SIE_PRE_PARAM *SiePreParam)
{
    Cal_Struct *CalInfo;
    UINT32 Addr1, Addr2;

    CalInfo = Cal_GetCalibrationData();

    if(AAA_Adjust.CSC_Status!=_CSC_Status)
    {
        debug_err(("-------->AAA_Adjust.CSC_Status = %d\r\n",AAA_Adjust.CSC_Status));   //Jarkko tmp
        if (GetIPPFuncInfor(DFS_EN))
        {
            SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
            Addr1 = SiePreParam->PreOutAdd0;
            Addr2 = SiePreParam->SiePreAddr2;
            SiePreParam->PreOutAdd0 = Addr2;

            if ((CalInfo->CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo->CalStatus_CapDP_W == _Cap_W_Status))
            {
                SiePreParam->SiePreAddr2 = CalInfo->Cal_DP_CaptureAddr;
                SiePreParam->PreFuncEn |= PRE_BLM_EN;
                SiePreParam->PreFuncEn |= PRE_DF_EMBED_EN;
            }

            sie_Set2Capture(SiePreParam);
            Sensor_Cap2Prv();

            SetIPPFuncInfor(DFS_CNT,DFS_SECONDPIC);
            SiePreParam->PreSrcSel = PATH_PRE_GAMMA;
            SiePreParam->PreOutAdd0 = Addr1;
            SiePreParam->SiePreAddr2 = Addr2;
            sie_Set2Capture(SiePreParam);
            Sensor_Cap2Prv();
            SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
        }
        else
        {
            Addr2 = SiePreParam->SiePreAddr2;
            if ((CalInfo->CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo->CalStatus_CapDP_W == _Cap_W_Status))
            {
                SiePreParam->SiePreAddr2 = CalInfo->Cal_DP_CaptureAddr;
                SiePreParam->PreFuncEn |= PRE_BLM_EN;
            }
            sie_Set2Capture(SiePreParam);
            Sensor_Cap2Prv();
            SiePreParam->SiePreAddr2 = Addr2;
        }
/*
        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(SiePreParam->PreOutAdd0 +GetIPPSizeInfor(_PreCropCapHsize)*(GetIPPSizeInfor(_PreCropCapVsize)-3), GetIPPSizeInfor(_PreCropCapHsize), 1,((SiePreParam->PreOutLineOfs*gImageAlgInfo.SieBitDepth)>>3));
        grph_setImg2(SiePreParam->PreOutAdd0+GetIPPSizeInfor(_PreCropCapHsize)*(GetIPPSizeInfor(_PreCropCapVsize)-1),SiePreParam->PreOutLineOfs*gImageAlgInfo.SieBitDepth/8);
        grph_setAOP(gImageAlgInfo.SieBitDepth,GRPH_DST_2,GRPH_AOP_A_COPY,0);
        grph_disableDMA();
        grph_close();
*/
        if(gImageAlgInfo.FileFmt & _FileFmt_Raw)
        {
            FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                            (UB*)SiePreParam->PreOutAdd0,
                            SiePreParam->PreCropSizeV * SiePreParam->PreOutLineOfs,
                            0,
                            1,
                            FST_FMT_RAW,
                            FST_TIME_INFINITE);
        }
    }
    else
    {
        UINT16 R,G,B;//,Rgain_Binning,Bgain_Binning;
        UINT32 IR_Addr;
        int i, j,k;
        UINT32 GainTableaAdr;
        unsigned short Data_R,Data_G,Data_B;
        unsigned int aRgain, aGgain, aBgain;
        UW Temp1,Temp2,TableStart,LineStart0,LineStart1;
        UINT CSCBayerAddr;
        debug_err(("CaptureOne:BayerAddr=0x%X, BayHsize=%d, BayVsize=%d SlcBuf =0x%X\r\n",SiePreParam->PreOutAdd0,SiePreParam->PreOutLineOfs,SiePreParam->PreCropSizeV, GetSlcBufAddr()));
//        SetSieInHsize(SiePreParam->PreOutLineOfs);    //Jarkko tmp mark
//        SetSieInVsize(SiePreParam->PreCropSizeV); //Jarkko tmp mark

        gImageAlgInfo.SiePath = _PreColorGain;
        gImageAlgInfo.SieBitDepth = _SieBitDepth_8;

        CSCBayerAddr =   SiePreParam->PreOutAdd0 + GetIPPSizeInfor(_PreCropCapHsize)*2;
        debug_err(("ColorShading_Cal %x %x\r\n", GetSieCapXCenter(), GetSieCapYCenter()));

        if (GetIPPFuncInfor(DFS_EN))
        {
            SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
            Addr1 = SiePreParam->PreOutAdd0;
            Addr2 = SiePreParam->SiePreAddr2;
            SiePreParam->PreOutAdd0 = Addr2;

            if ((CalInfo->CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo->CalStatus_CapDP_W == _Cap_W_Status))
            {
                SiePreParam->SiePreAddr2 = CalInfo->Cal_DP_CaptureAddr;
                SiePreParam->PreFuncEn |= PRE_BLM_EN;
                SiePreParam->PreFuncEn |= PRE_DF_EMBED_EN;
            }

            sie_Set2Capture(SiePreParam);
            Sensor_Cap2Prv();

            SetIPPFuncInfor(DFS_CNT,DFS_SECONDPIC);
            SiePreParam->PreSrcSel = PATH_PRE_GAMMA;
            SiePreParam->PreOutAdd0 = Addr1;
            SiePreParam->SiePreAddr2 = Addr2;
            sie_Set2Capture(SiePreParam);
            Sensor_Cap2Prv();
            SetIPPFuncInfor(DFS_CNT,DFS_FIRSTPIC);
        }
        else
        {
            Addr2 = SiePreParam->SiePreAddr2;
            if ((CalInfo->CalStatus_CapDP_B == _Cap_B_Status) && (CalInfo->CalStatus_CapDP_W == _Cap_W_Status))
            {
                SiePreParam->SiePreAddr2 = CalInfo->Cal_DP_CaptureAddr;
                SiePreParam->PreFuncEn |= PRE_BLM_EN;
            }
            sie_Set2Capture(SiePreParam);

/*
            grph_open();
            grph_swReset();
            grph_enableDMA();
            grph_setImg1(CSCBayerAddr+GetIPPSizeInfor(_PreCropCapHsize)*(GetIPPSizeInfor(_PreCropCapVsize)-3), GetIPPSizeInfor(_PreCropCapHsize), 1,((GetIPPSizeInfor(_PreCropCapHsize)*gImageAlgInfo.SieBitDepth)>>3));
            grph_setImg2(CSCBayerAddr+GetIPPSizeInfor(_PreCropCapHsize)*(GetIPPSizeInfor(_PreCropCapVsize)-1),GetIPPSizeInfor(_PreCropCapHsize)*gImageAlgInfo.SieBitDepth/8);
            grph_setAOP(gImageAlgInfo.SieBitDepth,GRPH_DST_2,GRPH_AOP_A_COPY,0);
            grph_disableDMA();
            grph_close();
*/

#if 0
            FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                             (UB*)CSCBayerAddr,
                             BayHsize*BayVsize*gImageAlgInfo.SieBitDepth/8,
                             0,
                             1,
                             FST_FMT_RAW,
                             FST_TIME_INFINITE);
#endif
            TimerCountUp(1);

#if 0
            ColorShadingCorrection(g_CS_GainTblBufAdd,CSCBayerAddr,GetIPPSizeInfor(_PreCropCapHsize),GetIPPSizeInfor(_PreCropCapVsize),Color_R);
            FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                             (UB*)g_CS_GainTblBufAdd,
                             ColorShadingTbl_H*ColorShadingTbl_V*3*gImageAlgInfo.SieBitDepth/8,
                             0,
                             1,
                             FST_FMT_RAW,
                             FST_TIME_INFINITE);
#else
        //color shading add
        if(bSHOT3PIC)
            GainTableaAdr = SiePreParam->PreOutAdd0+SiePreParam->PreCropSizeV * SiePreParam->PreOutLineOfs;
        else
            GainTableaAdr = GetSlcBufAddr();
        //GainTableaAdr = GetSlcBufAddr()+IMEBufSize+ThumbBufSize;
        if(bCSCBayer == FALSE)
        {

            debug_err(("LoadGainTable\r\n"));
            for(k=0;k<3;k++)
            {
                TableStart = k*ColorShadingTbl_H*ColorShadingTbl_V;
                for(j=0;j<ColorShadingTbl_V-1;j++)
                {
                    LineStart0 = j*ColorShadingTbl_H;
                    LineStart1 = (j+1)*ColorShadingTbl_H;
                    for(i=0;i<ColorShadingTbl_H-1;i+=4)
                    {
                        Temp1 = (g_CS_GainTblBufAdd[TableStart + LineStart0+i]+g_CS_GainTblBufAdd[TableStart + LineStart0+i+1]+g_CS_GainTblBufAdd[TableStart + LineStart1+i]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+1])/4;
                        Temp2 = Temp1&0xff;
                        Temp1 = (g_CS_GainTblBufAdd[TableStart + LineStart0+i+1]+g_CS_GainTblBufAdd[TableStart + LineStart0+i+2]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+1]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+2])/4;
                        Temp2 |= (Temp1&0xff)<<8;
                        Temp1 = (g_CS_GainTblBufAdd[TableStart + LineStart0+i+2]+g_CS_GainTblBufAdd[TableStart + LineStart0+i+3]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+2]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+3])/4;
                        Temp2 |= (Temp1&0xff)<<16;
                        Temp1 = (g_CS_GainTblBufAdd[TableStart + LineStart0+i+3]+g_CS_GainTblBufAdd[TableStart + LineStart0+i+4]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+3]+g_CS_GainTblBufAdd[TableStart + LineStart1+i+4])/4;
                        Temp2 |= (Temp1&0xff)<<24;

                        *(UW *)GainTableaAdr = Temp2;
                        GainTableaAdr += 4;

                    }
                }
            }
            //memcpy((UB*)(GainTableaAdr), (UB*)(g_CS_GainTblBufAdd), ColorShadingTbl_H*ColorShadingTbl_V*3);
            if(bSHOT3PIC)
                GainTableaAdr = SiePreParam->PreOutAdd0+SiePreParam->PreCropSizeV * SiePreParam->PreOutLineOfs;
            else
                GainTableaAdr = GetSlcBufAddr();
        }
#if 0
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                         (UB*)GainTableaAdr,
                         64*48*3,
                         0,
                         1,
                         FST_FMT_RAW,
                         FST_TIME_INFINITE);
#endif
        if(bCSCBayer== FALSE)
        {
            debug_msg("@______________________________________@\r\n");
            ColorShading_Reset();
            ColorShading_SetEnable(TRUE);
            ColorShading_SetCapMode(COLORSHADING_CAPMODE_BURST);
            if(ColorShading_GetEnable_CheckCapMode(COLORSHADING_CAPMODE_BURST))
            {
                ColorShading_SetGainBayAdd(GainTableaAdr+(ColorShadingTbl_H-1)*(ColorShadingTbl_V-1)*3);
                //ColorShading_SetGainTblBufAdd(GetSlcBufAddr());
                ColorShading_SetGainTblBufAdd(GainTableaAdr);

                ColorShading_SetBayerHSize(GetIPPSizeInfor(_PreCropCapHsize));
                ColorShading_SetBayerVSize(GetIPPSizeInfor(_PreCropCapVsize));
                ColorShading_SetTableHSize(ColorShadingTbl_H-1);
                ColorShading_SetTableVSize(ColorShadingTbl_V-1);
                ColorShading_SetCapFldNum(SensorCapTotalFld);
                ColorShading_SetRawBitDepth(gImageAlgInfo.PreCapBitDepth);
                ColorShading_SetBayPattern(Color_R);
                ColorShading_SetRawBayAdd_ByIndex(CSCBayerAddr, 0);
                ColorShading_SetBurstDivision(1);
                ColorShading_SetBurstPicNum(1);
                ColorShading_Initial();
                //Stage-1,
                ColorShading_Proceed(COLORSHADING_STAGE_A_INITIAL,   TRUE);
                //Stage-2,
                ColorShading_Proceed(COLORSHADING_STAGE_B_GET_TABLE, TRUE);
                //Stage-3,
                ColorShading_Proceed(COLORSHADING_STAGE_C_DO_PARTIAL,TRUE);
                //Stage-4,
                //ColorShading_Proceed(COLORSHADING_STAGE_D_END,       TRUE);
            }
        }
        //GainBayer x ImgBayer
        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(GainTableaAdr+(ColorShadingTbl_H-1)*(ColorShadingTbl_V-1)*3, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize),((GetIPPSizeInfor(_PreCropCapHsize)*gImageAlgInfo.PreCapBitDepth)>>3));
        grph_setImg2(CSCBayerAddr,GetIPPSizeInfor(_PreCropCapHsize)*gImageAlgInfo.PreCapBitDepth/8);
//        grph_setAOP2_NoWait(gImageAlgInfo.SieBitDepth,GRPH_DST_2,GRPH_AOP_MULTIPLY_DIV,6,0,0);
        grph_setAOP2(gImageAlgInfo.PreCapBitDepth,GRPH_DST_2,GRPH_AOP_MULTIPLY_DIV,6,0,0);
        grph_disableDMA();
        grph_close();

        //color shading end
#if 0
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                         (UB*)CSCBayerAddr,//GainTableaAdr+(ColorShadingTbl_H-1)*(ColorShadingTbl_V-1)*3,
                         BayHsize*BayVsize*gImageAlgInfo.SieBitDepth/8,
                         0,
                         1,
                         FST_FMT_RAW,
                         FST_TIME_INFINITE);
#endif
#endif

        //debug_err(("Preview RGBGain = %d %d %d\r\n", Rgain, Ggain, Bgain));

/*  //tmp mark
        aaa_CapAWBprocess(CSCBayerAddr, 0);
            aRgain = Rgain;
            aGgain = Ggain;
            aBgain = Bgain;
*/

        debug_err(("sie_Dram2Dram %x  --> %x    RGBGain = %d %d %d\r\n", CSCBayerAddr, SiePreParam->PreOutAdd0, aRgain, aGgain, aBgain));


        SIE_PRE_Para_d2d.SiePreAddr0 = CSCBayerAddr;
        SIE_PRE_Para_d2d.PreOutAdd0 = SiePreParam->PreOutAdd0;
        SIE_PRE_Para_d2d.PreInBitDepth = _PreBitDepth_12;
        SIE_PRE_Para_d2d.PreBitDepth = _PreBitDepth_8;
        SIE_PRE_Para_d2d.PreInLineOfs = SiePreParam->PreCropSizeH;
        SIE_PRE_Para_d2d.PreDramInSizeH = SiePreParam->PreCropSizeH;
        SIE_PRE_Para_d2d.PreOutSizeH = SiePreParam->PreOutLineOfs;
        SIE_PRE_Para_d2d.PreDramInSizeV = SiePreParam->PreCropSizeV;
        SIE_PRE_Para_d2d.PreOutLineOfs = SiePreParam->PreOutLineOfs;
        SIE_PRE_Para_d2d.PreSrcSel = PATH_POST_GAMMA;

        pre_Dram2Dram(&SIE_PRE_Para_d2d);
//        pre_Dram2Dram(CSCBayerAddr, SiePreParam->PreOutAdd0, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize), aRgain, aGgain, aBgain, GammaLUT_T1, 8);
        debug_err(("pre_Dram2Dram end\r\n"));

        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1((SiePreParam->PreOutAdd0+SiePreParam->PreOutLineOfs*((SiePreParam->PreCropSizeV-SiePreParam->PreCropSizeV)/2)+(SiePreParam->PreOutLineOfs-SiePreParam->PreOutLineOfs)/2), SiePreParam->PreOutLineOfs, SiePreParam->PreCropSizeV,((SiePreParam->PreOutLineOfs*gImageAlgInfo.SieBitDepth)>>3));
        //grph_setImg1(Source,((offset*bitdepth)>>3),V_size,((H_size*Bitdepth)>>3));
        grph_setImg2(SiePreParam->PreOutAdd0,SiePreParam->PreOutLineOfs*gImageAlgInfo.SieBitDepth/8);

        grph_setAOP(gImageAlgInfo.SieBitDepth,GRPH_DST_2,GRPH_AOP_A_COPY,0);
        grph_disableDMA();
        grph_close();
        debug_err(("d2d crop\r\n"));

        Sensor_Cap2Prv();
        SiePreParam->SiePreAddr2 = Addr2;
        }

        if(gImageAlgInfo.FileFmt & _FileFmt_Raw)
        {
            FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                            (UB*)SiePreParam->PreOutAdd0,
                            SiePreParam->PreCropSizeV * SiePreParam->PreOutLineOfs,
                            0,
                            1,
                            FST_FMT_RAW,
                            FST_TIME_INFINITE);
        }
    }

}
#endif

UINT32 getCapRawSize(void)
{
    UINT32 RawHigh, RawWidth, CurrRawSize;

    //calculate raw size
    RawHigh = GetIPPSizeInfor(_PreCropCapVsize);
    RawWidth = GetIPPSizeInfor(_PreCropCapHsize);
    RawHigh = (RawHigh + GetIPPSizeInfor(_CapTotalField) - 1) / GetIPPSizeInfor(_CapTotalField);
    RawHigh = ((RawHigh + 3) / 4) * 4;
    RawHigh = RawHigh * GetIPPSizeInfor(_CapTotalField);

    CurrRawSize = RawWidth * RawHigh * gImageAlgInfo.PreCapBitDepth >> 3;
    CurrRawSize -= (CurrRawSize % 4);

    return CurrRawSize;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void siepre_GetCapRaw(SIE_PRE_PARAM *SiePreParam)
{
    UINT32 FieldV;

    while (!(sie_ReadIntrStatus() & SIE_INT_FLDEND));
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_IntrptEnable(ENABLE, SIE_INTE_FLDEND|SIE_INTE_BP1);
    ena_int(_INT_SIE);

    sie_waitBP1(TRUE);
    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        Sensor_SetMode_TVP5150(SEN_MODE_FULL);
    }
    else
    {
        Sensor_SetMode(SEN_MODE_FULL);
        AeCapSet();
    }
    //#NT#2011/09/09#Spark Chou -end

    sie_waitBP1(FALSE);

    if ( PRE_ExecIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("PRE_ExecIPP() error \n"));
    pre_start();

    if ( SIE_ExecIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
    sie_start();
    sie_waitBP1(FALSE);

    sie_pause();
    pre_pause();
    sie_waitVD(1);
//    AeCap2Prv();
//    Sensor_SetMode(SEN_MODE_VGA);

    if (GetIPPFuncInfor(FQV_EN) == ENABLE)
    {
        FieldV = ((SiePreParam->PreCropSizeV + GetIPPSizeInfor(_FQVSampleRate) - 1) / GetIPPSizeInfor(_FQVSampleRate));
        FieldV = FieldV / 4 * 4;
        grph_open();
        grph_enableDMA();
        grph_setImg1((SiePreParam->PreOutAdd0 + (Field_Num1 - 1) * (SiePreParam->PreCropSizeH * gImageAlgInfo.PreCapBitDepth >> 3)),
                        (SiePreParam->PreCropSizeH * GetIPPSizeInfor(_FQVSampleRate) * gImageAlgInfo.PreCapBitDepth >> 3),
                        FieldV, (SiePreParam->PreCropSizeH * gImageAlgInfo.PreCapBitDepth >> 3));
        grph_setImg2(ippbuf_GetBuf(IPPBUF_CAP_QV_BUF), (SiePreParam->PreCropSizeH * gImageAlgInfo.PreCapBitDepth >> 3));
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
        grph_disableDMA();
        grph_swReset();
        grph_close();
    }
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void pre_PrvParameter(struct  PUBLIC_STR_JOBREQS *job_req)
#if 1
{
  UINT16 i;

  //CG and Ofs
  if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_CG_EN){
      job_req->pre_cg_ofs.OfsR = 0;
      job_req->pre_cg_ofs.OfsGr = 0;
      job_req->pre_cg_ofs.OfsGb = 0;
      job_req->pre_cg_ofs.OfsB = 0;
//#NT#2009/08/25#Jarkko Chang -begin
//#NT#modify for pre ca process
      job_req->pre_cg_ofs.GainR = Rgain;
      job_req->pre_cg_ofs.GainGr = 256;
      job_req->pre_cg_ofs.GainGb = 256;
      job_req->pre_cg_ofs.GainB = Bgain;
//#NT#2009/08/25#Jarkko Chang -end
      job_req->pre_cg_ofs.GainSel = 0;
  }

	debug_err(("pre_PrvParameter  job_req->SIE_PRE_PRV->ModeSel = %d\n\r",job_req->SIE_PRE_PRV->ModeSel));

  //VIG
  if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_VIG_EN){
      if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE)
      {
    //#NT#2009/08/04#Jarkko Chang -begin
          job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R;
          job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_G;
          job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_B;
    //#NT#2009/08/04#Jarkko Chang -end
          job_req->pre_vigline_setting.VIG_R2_LUT = Shading_LineTable;
          job_req->pre_vigline_setting.VIG_G2_LUT = Shading_LineTable;
          job_req->pre_vigline_setting.VIG_B2_LUT = Shading_LineTable;

          job_req->pre_vig_setting.VIG_XR = 321;
          job_req->pre_vig_setting.VIG_YR = 239;
          job_req->pre_vig_setting.VIG_XGR = 321;
          job_req->pre_vig_setting.VIG_YGR = 239;
          job_req->pre_vig_setting.VIG_XGB = 321;
          job_req->pre_vig_setting.VIG_YGB = 239;
          job_req->pre_vig_setting.VIG_XB = 321;
          job_req->pre_vig_setting.VIG_YB = 239;
          job_req->pre_vig_setting.VIG_YDIST = 0;
          job_req->pre_vig_setting.VIG_MUL = 85;
          job_req->pre_vig_setting.VIG_T = 0;
          job_req->pre_vig_setting.VIG_XDIV = 50;
          job_req->pre_vig_setting.VIG_YDIV = 50;
       }
      else if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE_2)
      {
    //#NT#2009/08/04#Jarkko Chang -begin
          job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R_MD2;
          job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_G_MD2;
          job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_B_MD2;
    //#NT#2009/08/04#Jarkko Chang -end
          job_req->pre_vigline_setting.VIG_R2_LUT = Shading_LineTable;
          job_req->pre_vigline_setting.VIG_G2_LUT = Shading_LineTable;
          job_req->pre_vigline_setting.VIG_B2_LUT = Shading_LineTable;

          job_req->pre_vig_setting.VIG_XR = 671;
          job_req->pre_vig_setting.VIG_YR = 480;
          job_req->pre_vig_setting.VIG_XGR = 671;
          job_req->pre_vig_setting.VIG_YGR = 480;
          job_req->pre_vig_setting.VIG_XGB = 671;
          job_req->pre_vig_setting.VIG_YGB = 480;
          job_req->pre_vig_setting.VIG_XB = 671;
          job_req->pre_vig_setting.VIG_YB = 480;
          job_req->pre_vig_setting.VIG_YDIST = 0;
          job_req->pre_vig_setting.VIG_MUL = 87;
          job_req->pre_vig_setting.VIG_T = 0;
          job_req->pre_vig_setting.VIG_XDIV = 24;
          job_req->pre_vig_setting.VIG_YDIV = 24;
        } else if( (job_req->SIE_PRE_PRV->ModeSel==NTSC_MODE) ||(job_req->SIE_PRE_PRV->ModeSel==PAL_MODE))
        {
    //#NT#2009/08/04#Jarkko Chang -begin
          job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R;
          job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_G;
          job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_B;
    //#NT#2009/08/04#Jarkko Chang -end
          job_req->pre_vigline_setting.VIG_R2_LUT = Shading_LineTable;
          job_req->pre_vigline_setting.VIG_G2_LUT = Shading_LineTable;
          job_req->pre_vigline_setting.VIG_B2_LUT = Shading_LineTable;

          job_req->pre_vig_setting.VIG_XR = 321;
          job_req->pre_vig_setting.VIG_YR = 239;
          job_req->pre_vig_setting.VIG_XGR = 321;
          job_req->pre_vig_setting.VIG_YGR = 239;
          job_req->pre_vig_setting.VIG_XGB = 321;
          job_req->pre_vig_setting.VIG_YGB = 239;
          job_req->pre_vig_setting.VIG_XB = 321;
          job_req->pre_vig_setting.VIG_YB = 239;
          job_req->pre_vig_setting.VIG_YDIST = 0;
          job_req->pre_vig_setting.VIG_MUL = 85;
          job_req->pre_vig_setting.VIG_T = 0;
          job_req->pre_vig_setting.VIG_XDIV = 50;
          job_req->pre_vig_setting.VIG_YDIV = 50;
       }
 }

  //GEO
  if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_GEO_EN){
      job_req->pre_geo_setting.GEO_CENTX = job_req->PRV_INFO.PreScaleOutH >> 1;
      job_req->pre_geo_setting.GEO_CENTY = job_req->PRV_INFO.PreOutVsize >> 1;
      job_req->pre_geo_setting.GEO_CENTXS = job_req->pre_geo_setting.GEO_CENTX * job_req->pre_geo_setting.GEO_CENTX;
      job_req->pre_geo_setting.GEO_CENTYS = job_req->pre_geo_setting.GEO_CENTY * job_req->pre_geo_setting.GEO_CENTY;
      job_req->pre_geo_setting.GEO_DISTSEL = 0;
      job_req->pre_geo_setting.GEO_XDIST= 255;
      job_req->pre_geo_setting.GEO_YDIST = 255;
      i = 0;
      while((1 << i) < (job_req->pre_geo_setting.GEO_CENTXS + job_req->pre_geo_setting.GEO_CENTYS))
        i++;
      job_req->pre_geo_setting.GEO_NORMBIT = i;
      job_req->pre_geo_setting.GEO_NORMFACT = (UINT8)((1 << (i + 7)) / (job_req->pre_geo_setting.GEO_CENTXS + job_req->pre_geo_setting.GEO_CENTYS));
      job_req->pre_geo_setting.GEO_RLUTGAIN = job_req->pre_geo_setting.GEO_GLUTGAIN = 4091;
      job_req->pre_geo_setting.GEO_BLUTGAIN = (1 << 12) - 1;
      job_req->pre_geo_setting.GEO_CORREN = 1;
      job_req->pre_geo_setting.GEO_CORRNORM = 0;
      job_req->pre_geo_setting.GEO_RCORRGAIN = job_req->pre_cg_ofs.GainR >> 2;
      job_req->pre_geo_setting.GEO_GRCORRGAIN = job_req->pre_cg_ofs.GainGr >> 2;
      job_req->pre_geo_setting.GEO_GBCORRGAIN = job_req->pre_cg_ofs.GainGb >> 2;
      job_req->pre_geo_setting.GEO_BCORRGAIN = job_req->pre_cg_ofs.GainB >> 2;
      job_req->pre_geo_setting.GEO_Lut = PreGeoLUT;
  }
//#NT#2009/10/16#Jarkko Chang -begin
  if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_GAMMA_EN)
  {
        job_req->pre_gamma_r = GammaLUT_128Tab;
        job_req->pre_gamma_g = GammaLUT_128Tab;
        job_req->pre_gamma_b = GammaLUT_128Tab;
  }
  if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_CA_EN)
  {
        job_req->pre_ca_setting.pre_CA_OutputAdd = ippbuf_GetBuf(IPPBUF_CA);
        //#NT#2011/03/02#ethanlau -begin
        //#NT#fix flip bug
        job_req->pre_ca_setting.pre_CA_OutputAdd +=
        Sen_GetPREAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_CAVsize), GetIPPSizeInfor(_CALineOffset), 8);
        //#NT#2011/03/02#ethanlau -end
        job_req->pre_ca_setting.pre_CA_OutputPtah = PATH_PRE_CLOLRGAIN;
        job_req->pre_ca_setting.pre_CA_Hsub = PreCA_Subsample_1_2;
        job_req->pre_ca_setting.pre_CA_Vsub = PreCA_Subsample_1_2;
        job_req->pre_ca_setting.pre_CA_OutputLineofs = (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF)>>(job_req->pre_ca_setting.pre_CA_Hsub+1);
        /*
        job_req->pre_CA_OutputAdd = ippbuf_GetBuf(IPPBUF_CA);
        job_req->pre_CA_OutputPtah = PATH_PRE_CLOLRGAIN;
        job_req->pre_CA_Hsub = PreCA_Subsample_1_2;
        job_req->pre_CA_Vsub = PreCA_Subsample_1_2;
        job_req->pre_CA_OutputLineofs = (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF)>>(job_req->pre_CA_Hsub+1);
        */
  }
//#NT#2009/10/16#Jarkko Chang -begin

  //BP
  if(job_req->SIE_PRE_PRV->PreIntEn & PRE_INTE_BP1)
    job_req->SIE_PRE_PRV->PreBp1 = job_req->SIE_PRE_PRV->PreCropSizeV * 68 / 100;//BP for CA

}
#endif
void pre_CapParameter(struct  PUBLIC_STR_JOBREQS *job_req){
  UINT16 i;

  if(GetIPPFuncInfor(DFS_EN)){
      if(GetIPPFuncInfor(DFS_CNT) == DFS_FIRSTPIC){
        job_req->SIE_PRE_CAP->PreFuncEn &= (~PRE_DF_SUB_EN);
        job_req->SIE_PRE_CAP->PreFuncEn |= PRE_CG_EN;
      }
      else{
        job_req->SIE_PRE_CAP->PreFuncEn |= PRE_DF_SUB_EN;
        job_req->SIE_PRE_CAP->PreFuncEn &= (~PRE_BLM_EN);
        job_req->SIE_PRE_CAP->PreFuncEn |= PRE_CG_EN;
      }
  }

  //CG and Ofs
  if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_CG_EN){
      job_req->pre_cg_ofs.OfsR = 0;
      job_req->pre_cg_ofs.OfsGr = 0;
      job_req->pre_cg_ofs.OfsGb = 0;
      job_req->pre_cg_ofs.OfsB = 0;
//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca process
  if(Flash_Param.FlshEna)
  {
      job_req->pre_cg_ofs.GainR = Flash_Rg;
      job_req->pre_cg_ofs.GainB = Flash_Bg;
  }
  else
  {
      job_req->pre_cg_ofs.GainR = Rgain;
      job_req->pre_cg_ofs.GainB = Bgain;
  }
      job_req->pre_cg_ofs.GainGr = Ggain;
      job_req->pre_cg_ofs.GainGb = Ggain;
      job_req->pre_cg_ofs.GainSel = 0;
//#NT#2009/10/16#Jarkko Chang -end
  }

  //gain ctrl
  if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_GAINCTRL_EN){
      job_req->pre_gainctrl_A.MapA = 255;
      job_req->pre_gainctrl_A.MapB = 255;
      job_req->pre_gainctrl_A.GainA = 0;
      job_req->pre_gainctrl_A.GainB = 0;
      job_req->pre_gainctrl_A.Switch = 0;
      job_req->pre_gainctrl_A.OffsetA = 0;
      job_req->pre_gainctrl_A.OffsetB = 0;
      job_req->pre_gainctrl_B.MapA = 255;
      job_req->pre_gainctrl_B.MapB = 255;
      job_req->pre_gainctrl_B.GainA = 0;
      job_req->pre_gainctrl_B.GainB = 0;
      job_req->pre_gainctrl_B.Switch = 0;
      job_req->pre_gainctrl_B.OffsetA = 0;
      job_req->pre_gainctrl_B.OffsetB = 0;
  }

  //VIG
  if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_VIG_EN){
      job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R_CAP;
      job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_G_CAP;
      job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_B_CAP;
      job_req->pre_vigline_setting.VIG_R2_LUT = Shading_LineTable;
      job_req->pre_vigline_setting.VIG_G2_LUT = Shading_LineTable;
      job_req->pre_vigline_setting.VIG_B2_LUT = Shading_LineTable;

      job_req->pre_vig_setting.VIG_XR = 1408;
      job_req->pre_vig_setting.VIG_YR = 981;
      job_req->pre_vig_setting.VIG_XGR = 1408;
      job_req->pre_vig_setting.VIG_YGR = 981;
      job_req->pre_vig_setting.VIG_XGB = 1408;
      job_req->pre_vig_setting.VIG_YGB = 981;
      job_req->pre_vig_setting.VIG_XB = 1408;
      job_req->pre_vig_setting.VIG_YB = 981;
      job_req->pre_vig_setting.VIG_YDIST = 0;
      job_req->pre_vig_setting.VIG_MUL = 96;
      job_req->pre_vig_setting.VIG_T = 0;
      job_req->pre_vig_setting.VIG_XDIV = 11;
      job_req->pre_vig_setting.VIG_YDIV = 11;
  }

  //GEO
  if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_GEO_EN){
      job_req->pre_geo_setting.GEO_CENTX = job_req->CAP_INFO.PreScaleOutH >> 1;
      job_req->pre_geo_setting.GEO_CENTY = job_req->CAP_INFO.PreOutVsize >> 1;
      job_req->pre_geo_setting.GEO_CENTXS = job_req->pre_geo_setting.GEO_CENTX * job_req->pre_geo_setting.GEO_CENTX;
      job_req->pre_geo_setting.GEO_CENTYS = job_req->pre_geo_setting.GEO_CENTY * job_req->pre_geo_setting.GEO_CENTY;
      job_req->pre_geo_setting.GEO_DISTSEL = 0;
      job_req->pre_geo_setting.GEO_XDIST= 255;
      job_req->pre_geo_setting.GEO_YDIST = 255;
      i = 0;
      while((1 << i) < (job_req->pre_geo_setting.GEO_CENTXS + job_req->pre_geo_setting.GEO_CENTYS))
        i++;
      job_req->pre_geo_setting.GEO_NORMBIT = i;
      job_req->pre_geo_setting.GEO_NORMFACT = (UINT8)((1 << (i + 7)) / (job_req->pre_geo_setting.GEO_CENTXS + job_req->pre_geo_setting.GEO_CENTYS));
      job_req->pre_geo_setting.GEO_RLUTGAIN = job_req->pre_geo_setting.GEO_GLUTGAIN = 4091;
      job_req->pre_geo_setting.GEO_BLUTGAIN = (1 << 12) - 1;
      job_req->pre_geo_setting.GEO_CORREN = 1;
      job_req->pre_geo_setting.GEO_CORRNORM = 0;
      job_req->pre_geo_setting.GEO_RCORRGAIN = job_req->pre_cg_ofs.GainR >> 2;
      job_req->pre_geo_setting.GEO_GRCORRGAIN = job_req->pre_cg_ofs.GainGr >> 2;
      job_req->pre_geo_setting.GEO_GBCORRGAIN = job_req->pre_cg_ofs.GainGb >> 2;
      job_req->pre_geo_setting.GEO_BCORRGAIN = job_req->pre_cg_ofs.GainB >> 2;
      job_req->pre_geo_setting.GEO_Lut = PreGeoLUT;
  }

  //DFS
  if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_DF_SUB_EN){
      job_req->pre_dfs_setting.DFS_OP = AMinusB;
      job_req->pre_dfs_setting.DFS_TH = 0xfff;//16;//DP threshold
      job_req->pre_dfs_setting.DFS_THSRC = SrcSie;
  }
}

void pre_D2DParameter(struct  PUBLIC_STR_JOBREQS *job_req){
  UINT16 i;

  //CG and Ofs
  if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_CG_EN){
      job_req->pre_cg_ofs.OfsR = 0;
      job_req->pre_cg_ofs.OfsGr = 0;
      job_req->pre_cg_ofs.OfsGb = 0;
      job_req->pre_cg_ofs.OfsB = 0;
      job_req->pre_cg_ofs.GainR = 256;
      job_req->pre_cg_ofs.GainGr = 256;
      job_req->pre_cg_ofs.GainGb = 256;
      job_req->pre_cg_ofs.GainB = 256;
      job_req->pre_cg_ofs.GainSel = 0;
  }

  //gain ctrl
  if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_GAINCTRL_EN){
      job_req->pre_gainctrl_A.MapA = 255;
      job_req->pre_gainctrl_A.MapB = 255;
      job_req->pre_gainctrl_A.GainA = 0;
      job_req->pre_gainctrl_A.GainB = 0;
      job_req->pre_gainctrl_A.Switch = 0;
      job_req->pre_gainctrl_A.OffsetA = 0;
      job_req->pre_gainctrl_A.OffsetB = 0;
      job_req->pre_gainctrl_B.MapA = 255;
      job_req->pre_gainctrl_B.MapB = 255;
      job_req->pre_gainctrl_B.GainA = 0;
      job_req->pre_gainctrl_B.GainB = 0;
      job_req->pre_gainctrl_B.Switch = 0;
      job_req->pre_gainctrl_B.OffsetA = 0;
      job_req->pre_gainctrl_B.OffsetB = 0;
  }

  //VIG
  if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_VIG_EN){
      job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R;
      job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_G;
      job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_B;
  }

  //GEO
  if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_GEO_EN){
      job_req->pre_geo_setting.GEO_CENTX = job_req->SIE_PRE_D2D->PreDramInSizeH >> 1;
      job_req->pre_geo_setting.GEO_CENTY = job_req->SIE_PRE_D2D->PreDramInSizeV >> 1;
      job_req->pre_geo_setting.GEO_CENTXS = job_req->pre_geo_setting.GEO_CENTX * job_req->pre_geo_setting.GEO_CENTX;
      job_req->pre_geo_setting.GEO_CENTYS = job_req->pre_geo_setting.GEO_CENTY * job_req->pre_geo_setting.GEO_CENTY;
      job_req->pre_geo_setting.GEO_DISTSEL = 0;
      job_req->pre_geo_setting.GEO_XDIST= 255;
      job_req->pre_geo_setting.GEO_YDIST = 255;

      i = 0;
      while((1 << i) < (job_req->pre_geo_setting.GEO_CENTXS + job_req->pre_geo_setting.GEO_CENTYS))
        i++;
      job_req->pre_geo_setting.GEO_NORMBIT = i;
      job_req->pre_geo_setting.GEO_NORMFACT = (UINT8)((1 << (i + 7)) / (job_req->pre_geo_setting.GEO_CENTXS + job_req->pre_geo_setting.GEO_CENTYS));
      job_req->pre_geo_setting.GEO_RLUTGAIN = job_req->pre_geo_setting.GEO_GLUTGAIN = 4091;
      job_req->pre_geo_setting.GEO_BLUTGAIN = (1 << 12) - 1;
      job_req->pre_geo_setting.GEO_CORREN = 1;
      job_req->pre_geo_setting.GEO_CORRNORM = 0;
      job_req->pre_geo_setting.GEO_RCORRGAIN = job_req->pre_cg_ofs.GainR >> 2;
      job_req->pre_geo_setting.GEO_GRCORRGAIN = job_req->pre_cg_ofs.GainGr >> 2;
      job_req->pre_geo_setting.GEO_GBCORRGAIN = job_req->pre_cg_ofs.GainGb >> 2;
      job_req->pre_geo_setting.GEO_BCORRGAIN = job_req->pre_cg_ofs.GainB >> 2;
      job_req->pre_geo_setting.GEO_Lut = PreGeoLUT;
  }
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif

void SensorIF_SetOB(BOOL Auto_OB, unsigned short OB)
{
}

unsigned short SensorIF_GetOB(void)
{
    return 0;
}


void Sensor_Cap2Prv(void)
{
    SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER);
    SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER);
    sie_Set2Preview(&SIE_PRE_Para_prv);
}

BOOL IS_SensorPreviewVGA(void)
{
    if ((SIE_PRE_Para_prv.VGAType == SENSOR_OUT_THREE_LINE) || (SIE_PRE_Para_prv.VGAType == SENSOR_OUT_TWO_LINE))
    {
        return TRUE;
    }
    return FALSE;
}

BOOL Check_FlashFire(void)
{
    if((gImageAlgInfo.FlashMode == _IMGFlashMode_ForceON) || (gImageAlgInfo.FlashMode == _IMGFlashMode_ForceONRedEye))
        return TRUE;
    else if(gImageAlgInfo.FlashMode == _IMGFlashMode_ForceOFF)
        return FALSE;
    else if((gImageAlgInfo.FlashMode == _IMGFlashMode_Auto) || (gImageAlgInfo.FlashMode == _IMGFlashMode_AutoRedEye))
    {
        return FALSE;
    }
    else
        return FALSE;
}

void VideoDzoomSet(void)
{
    UINT32 AddrOfs;
    if(gAlgMsg.Alg_Mode==MODE_Video_Recording)
        ime_SetDzoom2Video(&IME_D1_Para,&IME_D2_Para);
    else
        ime_SetDzoom2Video_Path1(&IME_D1_Para);

    IPEModeInfo.SIZEINFO.INSIZE.hsize = IPE_Para.InSizeH;
    IPEModeInfo.SIZEINFO.INSIZE.vsize = IPE_Para.InSizeV;
    IPEModeInfo.SIZEINFO.LofsIn = IPE_Para.InLineOfs;
    IPEModeInfo.SIZEINFO.CfaPat = Sen_GetIPECfaPat(IPE_Para.CfaPat, gImageAlgInfo.ImageFlip);
    AddrOfs = Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreOutHsize), SIE_PRE_Para_prv.PreOutLineOfs, gImageAlgInfo.PrePrvBitDepth);
    IPEModeInfo.ININFO.Adr0 = IPE_Para.Add0 + AddrOfs;
    IPEModeInfo.ININFO.Adr1 = IPE_Para.Add1 + AddrOfs;

    //#NT#2010/04/30#Jarkko Chang -begin
    //#NT#add for stripe mode change
    IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX5;
    if (IME_D1_Para.ime_sten == IMEALG_MULTI)
    {
        ime_to_ipe_Stripe_trans(&IPEModeInfo.SIZEINFO.HSTRIPE,&IPEModeInfo.SIZEINFO.VSTRIPE);
        IPEModeInfo.SIZEINFO.StripeMode = IPE_MSTP;
    }
    else
    {
        IPEModeInfo.SIZEINFO.StripeMode = IPE_SSTP;
    }
    //#NT#2010/04/30#Jarkko Chang -end

    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEXRESERVED;
        //#NT#2012/01/03#Jarkko Chang -begin
        //#NT# add DVI MST setting
        //Single stripe
        IME_D1_Para.ime_sten = IMEALG_SINGLE;
        IPEModeInfo.SIZEINFO.StripeMode = IPE_DVIMODE;
        //Multi stripe
        //IME_D1_Para.ime_sten = IMEALG_MULTI;
        //IPEModeInfo.SIZEINFO.StripeMode = IPE_MSTP;
    }
    //#NT#2011/08/25#Spark Chou -end
    ipe_changeSize(&IPEModeInfo.SIZEINFO);
    ipe_changeInput(&IPEModeInfo.ININFO);
}

#if 0
#pragma mark -
#endif


//#NT#20090912#ethanlau -begin
//add sensor mode interface
ER IPP_ClockIOInit(PRE_VGAFormatEnum Type,UINT32 sen_mode)
{
//    pinmux_select_mclk(PINMUX_MCLK);

    switch(Type)
    {
        case SENSOR_OUT_TWO_LINE:
        case SENSOR_OUT_THREE_LINE:
//            pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_480);
//            pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(0x13));
            pll_setClockFreq(SIEMCLK_FREQ, 24000000);
            break;

        default:
        case SENSOR_OUT_PROGRESSIVE:
//            pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_480);
//            pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(0x06));
//            pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(0x27));
            pll_setClockFreq(SIEMCLK_FREQ, 12000000);
            break;
    }
    pll_setClockRate(PLL_CLKSEL_PRE_CLKSRC, PLL_CLKSEL_PRE_CLKSRC_PXCLK);
    return E_OK;
}
void ShutterPin_GPIO(BOOL GPIO_Mode)
{
    if(GPIO_Mode)
        pinmux_enable_mes(0);
    else
        pinmux_enable_mes(PINMUX_MES0|PINMUX_MES1);
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
unsigned int Get_SieFrameCounter(void)
{
    return     sie_fc;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

void ShutterSet(unsigned short open)
{
    return;
    //shutter_move(open);
}


void Write_AE_EXIF_Infor(UINT paddress)
{
    UH Aperture, Exp_Time;
    H  Shutterspeed;
    UINT32 ISO_EXIF, ExpoTime_EXIF;
    Aperture=32;
    Shutterspeed=50;
    ExpoTime_EXIF = AE_Param.ExpoTime_Cap;
    ISO_EXIF = AE_Param.ISO_Gain_Cap;

    if(ExpoTime_EXIF == 485000)
        ExpoTime_EXIF = 500000;
    if(ExpoTime_EXIF < 500)
        ExpoTime_EXIF = 500;

    Exp_Time=1000000/(ExpoTime_EXIF);

//    JPEGWriteAPP1ExposureTime(paddress, 1,  32);//ExpTime=1/32s
    if((ExpoTime_EXIF)<=100000)
        JPEGWriteAPP1ExposureTime(paddress, 1,  Exp_Time);
    else
        JPEGWriteAPP1ExposureTime(paddress, ((ExpoTime_EXIF)/1000),  1000);

    if((ExpoTime_EXIF) <= 1000000)
        Shutterspeed=RatioLog2(1000000,(ExpoTime_EXIF));
    else
        Shutterspeed=-(RatioLog2((ExpoTime_EXIF),1000000));
    JPEGWriteAPP1ShutterSpeed(paddress, Shutterspeed,  10);//ExpTime=1/(2^(Shutterspeed/10))s

//    JPEGWriteAPP1FNumber(paddress, 28, 10);//Fno=2.8

    JPEGWriteAPP1Aperture(paddress,Aperture, 10);//Fno=2^((Aperture/10)/2)
    JPEGWriteAPP1MaxAperture(paddress, Aperture, 10);

    JPEGWriteAPP1ISOSpeed(paddress, ISO_EXIF);//ISO=100

    if(gImageAlgInfo.AEEVValue==_IMGEVValue_N6)
        JPEGWriteAPP1ExposureBias(paddress,-4,2);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N5)
        JPEGWriteAPP1ExposureBias(paddress,-3,2);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N4)
        JPEGWriteAPP1ExposureBias(paddress,-4,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N3)
        JPEGWriteAPP1ExposureBias(paddress,-3,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N2)
        JPEGWriteAPP1ExposureBias(paddress,-2,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_N1)
        JPEGWriteAPP1ExposureBias(paddress,-1,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_00)
        JPEGWriteAPP1ExposureBias(paddress, 0,1); //(paddress, 0,0);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P1)
        JPEGWriteAPP1ExposureBias(paddress, 1,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P2)
        JPEGWriteAPP1ExposureBias(paddress, 2,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P3)
        JPEGWriteAPP1ExposureBias(paddress, 3,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P4)
        JPEGWriteAPP1ExposureBias(paddress, 4,3);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P5)
        JPEGWriteAPP1ExposureBias(paddress, 3,2);
    else if(gImageAlgInfo.AEEVValue==_IMGEVValue_P6)
        JPEGWriteAPP1ExposureBias(paddress, 4,2);
    else
        JPEGWriteAPP1ExposureBias(paddress, 0,1);

}

INT32 UI_CheckParameter(void)    //Connie_UI_SETTING_CHECK 2005.12.30
{
    INT32    ErrorCode=E_OK;
    int i, j;
    int mismatch;
    int Cur_UIParam[Total_UIItem];

    Cur_UIParam[0]=gImageAlgInfo.SceneMode;
    Cur_UIParam[1]=gImageAlgInfo.Quality;
    Cur_UIParam[2]=gImageAlgInfo.DriveMode;
    Cur_UIParam[3]=gImageAlgInfo.FlashMode;
    Cur_UIParam[4]=gImageAlgInfo.WBMode;
    Cur_UIParam[5]=gImageAlgInfo.AEEVValue;
    Cur_UIParam[6]=gImageAlgInfo.FileFmt;
    Cur_UIParam[7]=gImageAlgInfo.SieBitDepth;

    for(i=0; i<Total_UICondition; i++){
        mismatch=0;
        for(j=0; j<Total_UIItem; j++){
            if(IMGUI_FALSE_SET[i][j]!=_DONTCARE){
                if(Cur_UIParam[j]!=IMGUI_FALSE_SET[i][j])
                    mismatch++;
            }
        }
        if(mismatch==0){//Meet one of the false setting conditions
            ErrorCode=-1;
            return ErrorCode;
        }
    }


    return ErrorCode;

}

void SetColorMode(unsigned char color)
{
    IPE_IQINFO IQInfo;

    IQInfo.FuncUpdateSel = IPE_FUNC_NOUPDATE;
    IQInfo.ParamUpdateSel = IPE_SET_COLOREFFECT;

    IQInfo.COLOR_EFFECT_PARAM = &ColorEffectTable[color];
    IQInfo.HueTab = NULL;
    IQInfo.SatTab = NULL;
    IQInfo.IntTab = NULL;
    IQInfo.DDSTab = NULL;

    ipe_changeParam(&IQInfo);
}

void SetSharpness(unsigned char Index)
{
    IPE_IQINFO IQInfo;
    STR_IPE_EDGE_ENHMAP_PARA EdgInfo;

    EdgInfo = *IPEModeInfo.IQINFO.EDGE_PARAM;
    IQInfo.EDGE_PARAM = &EdgInfo;

    IQInfo.FuncUpdateSel = IPE_FUNC_NOUPDATE;
    IQInfo.ParamUpdateSel = IPE_SET_EDGE;
//#NT#2009/12/02#Jarkko Chang -begin
/*
    if(Index == _IMGSharp_P1)
        IQInfo.EDGE_PARAM->EdgEnhance = IQInfo.EDGE_PARAM->EdgEnhance*2;
    else if(Index == _IMGSharp_Normal)
        IQInfo.EDGE_PARAM->EdgEnhance = IQInfo.EDGE_PARAM->EdgEnhance;
    else if(Index == _IMGSharp_N1)
        IQInfo.EDGE_PARAM->EdgEnhance = IQInfo.EDGE_PARAM->EdgEnhance/2;
*/
//Sharpness[GetISOIdx(AE_Param.ISO_Gain)][Index];
    IQInfo.EDGE_PARAM->EdgEnhance = Sharpness[GetISOIdx(AE_Param.ISO_Gain)][Index];
//#NT#2009/12/02#Jarkko Chang -end
    IQInfo.EdgeKerH = NULL;
    IQInfo.EdgeKerV = NULL;
    IQInfo.Emap = NULL;
    IQInfo.Estab = NULL;
    //IQInfo.EExtHVopt = 0xff;
    ipe_changeParam(&IQInfo);
}

void SetSaturation(unsigned char Index)
{
    IPE_IQINFO IQInfo;
    IPE_COLOR_EFFECT_PARAM ColorEffectInfo;
if (1)
{
    ColorEffectInfo = *IPEModeInfo.IQINFO.COLOR_EFFECT_PARAM;
    IQInfo.COLOR_EFFECT_PARAM = &ColorEffectInfo;

    IQInfo.FuncUpdateSel = IPE_FUNC_NOUPDATE;
    IQInfo.ParamUpdateSel = IPE_SET_COLOREFFECT;

    if(Index == _IMGSaturation_P1)
        IQInfo.COLOR_EFFECT_PARAM->SatOfs = 0x20;
    else if(Index == _IMGSaturation_Normal)
        IQInfo.COLOR_EFFECT_PARAM->SatOfs = 0x0;
    else if(Index == _IMGSaturation_N1)
        IQInfo.COLOR_EFFECT_PARAM->SatOfs = 0xe0;

        IQInfo.COLOR_EFFECT_PARAM->SatOfs = 0x10;

    IQInfo.HueTab = NULL;
    IQInfo.SatTab = NULL;
    IQInfo.IntTab = NULL;
    IQInfo.DDSTab = NULL;

    ipe_changeParam(&IQInfo);
}
else
{

}
}

unsigned int WriteDebugData(unsigned char *Add)
{
    UH i;
    unsigned char *buff;
    char endl[2]={0X0D,0X0A};
    char endqty[2]={0XCC,0XBB};

    buff = Add;

    //insert Qty tag
    buff += sprintf((char *)buff,"%s",endqty);

    buff += sprintf((char *)buff,"%s",endl);
//    buff += sprintf((char *)buff,"IQ: 09_12_08",endl);
//    buff += sprintf((char *)buff,"%s\t  1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t11\t12%s", endl, endl);
//    *buff++= '\t';
//    for(  i = 0; i < 80; i++ )
//        *buff++ = '-' ;
//    buff += sprintf((char *)buff,"%s", endl);
    /*
    buff += sprintf((char *)buff,"(OX)\t: %c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c%s",
                    AAA_Adjust.Mshutter_Status==_Mshutter_Status ? 'O':'X',        // 1
                    AAA_Adjust.ISO_Status==_ISO_Status ? 'O':'X',                // 2
                    AAA_Adjust.AWB_Status==_AWB_Status ? 'O':'X',                // 3
                    AAA_Adjust.AWB_Status==_AWB_Status ? 'O':'X',                // 4
                    AAA_Adjust.DP_Status.Prv_White==_Prv_W_Status ? 'O':'X',    // 5
                    AAA_Adjust.DP_Status.Prv_Black==_Prv_B_Status ? 'O':'X',    // 6
                    AAA_Adjust.DP_Status.Cap_White==_Cap_W_Status ? 'O':'X',    // 7
                    AAA_Adjust.DP_Status.Cap_Black==_Cap_B_Status ? 'O':'X',    // 8
                    AAA_Adjust.Flash_Status==_Flash_Status ? 'O':'X',            // 9
                    AAA_Adjust.OB_Status==_OB_Status ? 'O':'X',                    // 10
                    //AAA_Adjust.FTP_Status==_FTP_Status ? 'O':'X',                // 11
                    endl
                    );
    */


    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"a:\r\n ExpectY = \t%d\r\n YAverage = \t%d\r\n EV_Value = \t%d\r\n AE_GetCapISO = \t%d\r\n ISO_Gain = \t%d\r\n AeUseIris = \t%d\r\n ExpoTime = \t%d\r\n ExpoLine = \t%d\r\n AE.StableCnt = \t%d\r\n AE.ExpoFrame = \t%d\r\n AE_AddY = \t%d\r\n Rgain = \t%d\r\n Ggain = \t%d\r\n Bgain = \t%d\r\n AE__GetLV = \t%d\r\n AE_GetPrvISO = \t%d\r\n AE__GetNYAdj = \t%d\r\n AE_GetHistHighCnt = \t%d\r\n SensorIF_GetOB = \t%d\r\n",
                   AE_Debug.AE_ExpectY,
                   YAverage,
                   AE_Param.EV_Value,
                   AE_Param.ISO_Gain_Cap,
                   AE_Debug.AE_CapISO,
                   AE_Param.AeUseIris, //no 5
                   AE_Param.ExpoTime_Cap,
                   AE_Param.ExpoLine,
                   AE_Param.StableCnt,
                   AE_Param.ExpoFrame,
                   AE_Debug.AE_AddY, //no 10
                   Rgain,
                   Ggain,
                   Bgain,
                   AE__GetLV(), //lv_info,
                   AE_GetPrvISO(),
                   endl,
                   AE__GetNYAdj(),
                   AE_GetHistHighCnt(),
                   SensorIF_GetOB(),
                   endl
                   );

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"b:\r\n SceneMode = \t%d\r\n IMGSize = \t%d\r\n Quality = \t%d\r\n DriveMode = \t%d\r\n DZoomIndex = \t%d\r\n FlashMode = \t%d\r\n ImageEffect = \t%d\r\n WBMode = \t%d\r\n AEEVValue = \t%d\r\n AEMode = \t%d\r\n ISO = \t%d\r\n DSCMode = \t%d\r\n Frequency = \t%d%s",
                    gImageAlgInfo.SceneMode,                            // b1
                    gImageAlgInfo.IMGSize,                                // b2
                    gImageAlgInfo.Quality,                                // b3
                    gImageAlgInfo.DriveMode,                            // b4
                    gImageAlgInfo.DZoomIndex,                            // b5
                    gImageAlgInfo.FlashMode,                            // b6
                    gImageAlgInfo.ImageEffect,                            // b7
                    gImageAlgInfo.WBMode,                                // b8
                    gImageAlgInfo.AEEVValue,                            // b9
                    gImageAlgInfo.AEMode,                                // b10
                    gImageAlgInfo.ISO,                                    // b11
                    gImageAlgInfo.DSCMode,                                // b12
                    gImageAlgInfo.Frequency,                              // b13
                    endl
                    );

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    for(i=0;i<8;i++)
    {
        buff += sprintf((char *)buff,"W  \t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%s",
            gAWBWeight[0+i*8],
            gAWBWeight[1+i*8],
            gAWBWeight[2+i*8],
            gAWBWeight[3+i*8],
            gAWBWeight[4+i*8],
            gAWBWeight[5+i*8],
            gAWBWeight[6+i*8],
            gAWBWeight[7+i*8],
            endl
            );
    }

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    for(i=0;i<8;i++)
    {
        buff += sprintf((char *)buff,"RGB\t: %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d%s",
            R_acc[(0+i*8)],G_acc[(0+i*8)],B_acc[(0+i*8)],
            R_acc[(1+i*8)],G_acc[(1+i*8)],B_acc[(1+i*8)],
            R_acc[(2+i*8)],G_acc[(2+i*8)],B_acc[(2+i*8)],
            R_acc[(3+i*8)],G_acc[(3+i*8)],B_acc[(3+i*8)],
            R_acc[(4+i*8)],G_acc[(4+i*8)],B_acc[(4+i*8)],
            R_acc[(5+i*8)],G_acc[(5+i*8)],B_acc[(5+i*8)],
            R_acc[(6+i*8)],G_acc[(6+i*8)],B_acc[(6+i*8)],
            R_acc[(7+i*8)],G_acc[(7+i*8)],B_acc[(7+i*8)],
            endl
            );
    }

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

/*   buff += sprintf((char *)buff,"AF\t: Max Step: %3d  Max VA:%3d  AF time:%3dms%s%s",
        g_AFInfo.MaxIdx,
        g_AFInfo.MaxVA,
        g_AFInfo.AF_time,
        endl
        );

    for(i=0;i<AF_FOCUS_ARRAY_SIZE;i++)
    {
        buff += sprintf((char *)buff,"AF\t: Step: %3d VA:%3d%s",
            g_AFInfo.Step[i],
            g_AFInfo.Value[i],
            endl
            );
    }
 */
    buff += sprintf((char *)buff,"%s", endl);
    buff += sprintf((char *)buff,"%s", endl);

    if ((buff-Add) > CAP_DEBUGINFO_SIZE)
    {
        debug_err(("WriteDebugData() buffer overflow %d\r\n",(buff-Add)));
    }

    return (buff-Add);
}

UINT32    Sensor_CheckNeedSize(void)
{
    return 0xc00000;
}

void SetSensorMode(INT32 Mode)
{
	debug_err(("SetSensorMode = %d,TvInFormat = %d\n\r",Mode,gImageAlgInfo.TvInFormat));
    switch(Mode)
    {
        default:
        case MONITOR_MODE:

            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv);

            SetIPPSizeInfor(_OBWinX0, PRV_OB_X0);
            SetIPPSizeInfor(_OBWinX1, PRV_OB_X1);
            SetIPPSizeInfor(_OBWinY0, PRV_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, PRV_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, PRV_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, PRV_OB_THRES);
            SetIPPSizeInfor(_OBGain, PRV_OB_GAIN);
            SetIPPSizeInfor(_CAHSub, PreCA_Subsample_1_2);
            SetIPPSizeInfor(_CAVSub, PreCA_Subsample_1_2);

            break;

        case MONITOR_MODE_2:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_1);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_1);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_1);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_1);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_1);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_1);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_1);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_1);

            SetIPPSizeInfor(_OBWinX0, PRV_OB_X0);
            SetIPPSizeInfor(_OBWinX1, PRV_OB_X1);
            SetIPPSizeInfor(_OBWinY0, PRV_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, PRV_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, PRV_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, PRV_OB_THRES);
            SetIPPSizeInfor(_OBGain, PRV_OB_GAIN);
            SetIPPSizeInfor(_CAHSub, PreCA_Subsample_1_2);
            SetIPPSizeInfor(_CAVSub, PreCA_Subsample_1_2);
            break;

        case MONITOR_MODE_3:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_2);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_2);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_2);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_2);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_2);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_2);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_2);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_2);

            SetIPPSizeInfor(_OBWinX0, PRV_OB_X0);
            SetIPPSizeInfor(_OBWinX1, PRV_OB_X1);
            SetIPPSizeInfor(_OBWinY0, PRV_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, PRV_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, PRV_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, PRV_OB_THRES);
            SetIPPSizeInfor(_OBGain, PRV_OB_GAIN);
            SetIPPSizeInfor(_CAHSub, PreCA_Subsample_1_2);
            SetIPPSizeInfor(_CAVSub, PreCA_Subsample_1_2);
            break;

        case STILL_MODE:
            SetIPPSizeInfor(_CapTotalField, SensorCapTotalFld);
            SetIPPSizeInfor(_FieldFld1, Field_Num1);
            SetIPPSizeInfor(_FieldFld2, Field_Num2);
            SetIPPSizeInfor(_FieldFld3, Field_Num3);
            SetIPPSizeInfor(_FieldFld4, Field_Num4);
            SetIPPSizeInfor(_FieldFld5, Field_Num5);
            SetIPPSizeInfor(_FieldFld6, Field_Num6);
            SetIPPSizeInfor(_FieldFld7, Field_Num7);
            SetIPPSizeInfor(_FieldFld8, Field_Num8);
            SetIPPSizeInfor(_SieXcenter, SIE_CAP_XCENTER);
            SetIPPSizeInfor(_SieYcenter, SIE_CAP_YCENTER);
            SetIPPSizeInfor(_HDActX_Capture, ACT_STRX_Cap);
            SetIPPSizeInfor(_HDActY1_Capture, ACT_STRY_Cap);
            SetIPPSizeInfor(_HDActY2_Capture, ACT_STRY_Cap2);
            SetIPPSizeInfor(_HDActY3_Capture, ACT_STRY_Cap3);
            SetIPPSizeInfor(_HDActY4_Capture, ACT_STRY_Cap4);
            SetIPPSizeInfor(_HDActY5_Capture, ACT_STRY_Cap5);
            SetIPPSizeInfor(_HDActY6_Capture, ACT_STRY_Cap6);
            SetIPPSizeInfor(_HDActY7_Capture, ACT_STRY_Cap7);
            SetIPPSizeInfor(_HDActY8_Capture, ACT_STRY_Cap8);
            SetIPPSizeInfor(_HDTotal_Captrue, HD_Total_Cap);
            SetIPPSizeInfor(_HDSync_Captrue, HD_Sync_Cap);
            SetIPPSizeInfor(_VDTotal_Captrue, VD_Total_Cap);
            SetIPPSizeInfor(_VDSync_Captrue, VD_Sync_Cap);

            SetIPPSizeInfor(_OBWinX0, CAP_OB_X0);
            SetIPPSizeInfor(_OBWinX1, CAP_OB_X1);
            SetIPPSizeInfor(_OBWinY0, CAP_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, CAP_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, CAP_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, CAP_OB_THRES);
            SetIPPSizeInfor(_OBGain, CAP_OB_GAIN);

            //#NT#2011/09/05#Spark Chou -begin
            //#NT#
            if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
            {
                /*
                SetIPPSizeInfor(_CapTotalField, 1);
                SetIPPSizeInfor(_FieldFld1, 1);
                SetIPPSizeInfor(_FieldFld2, 0xff);
                SetIPPSizeInfor(_FieldFld3, 0xff);
                SetIPPSizeInfor(_FieldFld4, 0xff);
                SetIPPSizeInfor(_FieldFld5, 0xff);
                SetIPPSizeInfor(_FieldFld6, 0xff);
                SetIPPSizeInfor(_FieldFld7, 0xff);
                SetIPPSizeInfor(_FieldFld8, 0xff);
*/
                if ( gImageAlgInfo.TvInFormat == _TvInFormat_NTSC )
                {
                    SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_NTSC);
                    SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_NTSC);
                    SetIPPSizeInfor(_HDTotal_Captrue, HD_Total_Prv_NTSC);
                    SetIPPSizeInfor(_HDSync_Captrue, HD_Sync_Prv_NTSC);
                    SetIPPSizeInfor(_VDTotal_Captrue, VD_Total_Prv_NTSC);
                    SetIPPSizeInfor(_VDSync_Captrue, VD_Sync_Prv_NTSC);
                    SetIPPSizeInfor(_HDActX_Capture, ACT_STRX_Prv_NTSC);
                    SetIPPSizeInfor(_HDActY1_Capture, ACT_STRY_Prv_NTSC);
                }
                else if ( gImageAlgInfo.TvInFormat == _TvInFormat_PAL )
                {
                    SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_PAL);
                    SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_PAL);
                    SetIPPSizeInfor(_HDTotal_Captrue, HD_Total_Prv_PAL);
                    SetIPPSizeInfor(_HDSync_Captrue, HD_Sync_Prv_PAL);
                    SetIPPSizeInfor(_VDTotal_Captrue, VD_Total_Prv_PAL);
                    SetIPPSizeInfor(_VDSync_Captrue, VD_Sync_Prv_PAL);
                    SetIPPSizeInfor(_HDActX_Capture, ACT_STRX_Prv_PAL);
                    SetIPPSizeInfor(_HDActY1_Capture, ACT_STRY_Prv_PAL);

                }
            }
            //#NT#2011/09/05#Spark Chou -end
            break;

        case NTSC_MODE:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_NTSC);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_NTSC);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_NTSC);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_NTSC);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_NTSC);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_NTSC);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_NTSC);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_NTSC);

            SetIPPSizeInfor(_OBWinX0, PRV_OB_X0);
            SetIPPSizeInfor(_OBWinX1, PRV_OB_X1);
            SetIPPSizeInfor(_OBWinY0, PRV_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, PRV_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, PRV_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, PRV_OB_THRES);
            SetIPPSizeInfor(_OBGain, PRV_OB_GAIN);
            SetIPPSizeInfor(_CAHSub, PreCA_Subsample_1_2);
            SetIPPSizeInfor(_CAVSub, PreCA_Subsample_1_2);
            break;
	 case PAL_MODE:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_PAL);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_PAL);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_PAL);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_PAL);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_PAL);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_PAL);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_PAL);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_PAL);

            SetIPPSizeInfor(_OBWinX0, PRV_OB_X0);
            SetIPPSizeInfor(_OBWinX1, PRV_OB_X1);
            SetIPPSizeInfor(_OBWinY0, PRV_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, PRV_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, PRV_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, PRV_OB_THRES);
            SetIPPSizeInfor(_OBGain, PRV_OB_GAIN);
            SetIPPSizeInfor(_CAHSub, PreCA_Subsample_1_2);
            SetIPPSizeInfor(_CAVSub, PreCA_Subsample_1_2);
            break;
  //##for DVI MODE## end

    }
}

void DFS_OnOffSwitch(void)
{
    if(0)
        SetIPPFuncInfor(DFS_EN,ON);
    else
        SetIPPFuncInfor(DFS_EN,OFF);
}


void IPPFunc_OnOffSwitch(void)
{
    //Integrated IPP function setting point, ON/OFF switch and parameters setting
    //DFS
    //SetIPPFuncInfor(DFS_EN,OFF);
    DFS_OnOffSwitch();
    //PRE D2D after cature
    SetIPPFuncInfor(PRED2D_EN,OFF);
    //PRE D2D 90 degree rotation, for geometric correction
    SetIPPFuncInfor(PRE_D2D_ROT_EN,OFF);
    //Nova-light in capture flow
    SetIPPFuncInfor(NLIGHT_CAP_EN,OFF);
    //Nova-light input data selection
    SetIPPFuncInfor(NLIGHT_CAP_SEL,NLCap_QVDataIn);
    //Chroma filter
    SetIPPFuncInfor(CHRFILT_EN,OFF);
    //Chroma curve
    SetIPPFuncInfor(CHRCURVE_EN,OFF);
//    SetIPPFuncInfor(CHRCURVE_EN,ON);

    SetIPPFuncInfor(CHRCBCURVE,(UINT32)ChrCurveLUT);
    SetIPPFuncInfor(CHRCRCURVE,(UINT32)ChrCurveLUT);
    SetIPPFuncInfor(CAPGAMMACURVE,(UINT32)GammaLUT_128Tab);

}

void Get_CapInfo(UINT InfoID, UINT *ReturnInfo)
{
     switch(InfoID)
    {
        case Cap_CFAPat:
            *ReturnInfo = CFAPAT_CAP;
        break;
        case Cap_SyncMode:
            *ReturnInfo = STILL_MODE;
        break;

        case Cap_ChromaParam:
            ReturnInfo = CHR_Param[0];
        break;

        default:
            debug_err(("Err InfoID(%d) > MaxID(%d)\r\n", InfoID, Cap_MaxID));
        break;
     }
}

UINT32* Get_ChromaInfo(void)
{
    return (UINT32 *)&CHR_Param[0][0];
}

IME_FILT_PARA* Get_ImeFilterInfo(void)
{
    return (IME_FILT_PARA*)IMECHROMAPARAM;
}
void Set_DZoomFlag(UINT32 Flag)
{
    DZoom_Flag |= Flag;
}

void Clr_DZoomFlag(UINT32 Flag)
{
    DZoom_Flag &= (~Flag);
}

UINT32 Get_DZoomFlag(void)
{
    return DZoom_Flag;
}


//#NT#2010/02/10#Jarkko Chang -begin
//#NT#modify for caf
void AAFD_LockSwitch(UINT32 ImgMode)
{
    static BOOL caf_lock = LOCK;
	debug_err(("AAFD_LockSwitch  ImgMode = %d\n\r",ImgMode));
	debug_err(("AAFD_LockSwitch  gImageAlgInfo.TvInFormat = %d\n\r",gImageAlgInfo.TvInFormat));

    switch(ImgMode)
    {
        case IPPImg_Preview:
	    if ( gImageAlgInfo.TvInFormat != _TvInFormat_OFF )
	    	{
		     aaa_AwbLock(LOCK);
	            aaa_AeLock(LOCK);
	            caf_Lock(LOCK);
	            fde_FdLockCap(LOCK);
		}else{
	            aaa_AwbLock(UNLOCK);
	            aaa_AeLock(UNLOCK);
	            caf_Lock(UNLOCK);
	            fde_FdLockCap(UNLOCK);
	    	}
            break;
        case IPPImg_Video:
        case IPPImg_PCC:
		 if ( gImageAlgInfo.TvInFormat != _TvInFormat_OFF )
	    	{
			aaa_AwbLock(LOCK);
	            aaa_AeLock(LOCK);
	            aaa_FdLock(LOCK);
	            //aaa_CafLock(UNLOCK);
	            caf_Lock(LOCK);
	            //#NT#2010/10/18#JJ -begin
	            aaa_SdLock(LOCK);
	            //#NT#2010/10/18#JJ -end
		}else{
	            aaa_AwbLock(UNLOCK);
	            aaa_AeLock(UNLOCK);
	            aaa_FdLock(LOCK);
	            //aaa_CafLock(UNLOCK);
	            caf_Lock(UNLOCK);
	            //#NT#2010/10/18#JJ -begin
	            aaa_SdLock(LOCK);
	            //#NT#2010/10/18#JJ -end
		}
            break;
        case IPPImg_AF:
		 if ( gImageAlgInfo.TvInFormat != _TvInFormat_OFF )
	    	{
			aaa_AwbLock(LOCK);
			aaa_AeLock(LOCK);
			aaa_FdLock(LOCK);
			//#NT#2010/10/18#JJ -begin
			aaa_SdLock(LOCK);
			//#NT#2010/10/18#JJ -end
		 }else{
	            aaa_AwbLock(UNLOCK);
	            aaa_AeLock(UNLOCK);
	            aaa_FdLock(LOCK);
	            //#NT#2010/10/18#JJ -begin
	            aaa_SdLock(LOCK);
	            //#NT#2010/10/18#JJ -end
		 }
            break;

        case IPPImg_BeforCap:
            aaa_AwbLock(LOCK);
            aaa_AeLock(LOCK);
            aaa_FdLock(LOCK);
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            fde_FdLockCap(LOCK);
            caf_Lock(LOCK);
            break;

        case IPPImg_DzoomEnter:
            aaa_FdLock(LOCK);
/*
            caf_lock = Is_cafLock();
            if (caf_lock == UNLOCK)
            {
                caf_Lock(LOCK);
            }
*/
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            break;
        case IPPImg_DzoomLeave:
//#NT#2009/12/07#Jarkko Chang -begin
            if (gImageAlgInfo.FaceDetectionMode == _FaceDetectionMode_On)
            {
                aaa_FdLock(UNLOCK);
            }
            if (caf_lock == UNLOCK)
            {
//                caf_Lock(UNLOCK);
            }
//#NT#2009/12/07#Jarkko Chang -end
            break;
//#NT#20090912#ethanlau -begin
        case IPPImg_SenModeChg:
            aaa_AwbLock(LOCK);
            aaa_AeLock(LOCK);
            aaa_FdLock(LOCK);
            fde_FdLockCap(LOCK);
            break;
//#NT#20090912#ethanlau -end
        default:
            debug_err(("-E- ImgMode(%d) > MaxID(%d)\r\n", ImgMode, IPPImg_MaxID));
        break;
    }

}
//#NT#2010/02/10#Jarkko Chang -end

void SetSiePreCapParam(SIE_PRE_PARAM *SiePreInfo)
{
    SiePreInfo->PreIntEn = PRE_INTE_FLDEND;
    SiePreInfo->SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1;
    SiePreInfo->SieBp1 = CAP_SIE_BP1;
    SiePreInfo->SieBp2 = CAP_SIE_BP2;
    SiePreInfo->SieMaster = SlaveMode;
    SiePreInfo->SieHDPhase = SieLoad_FallingEdge;
    SiePreInfo->SieVDPhase = SieLoad_FallingEdge;
    SiePreInfo->SieDataPhase = SieLoad_RisingEdge;
    SiePreInfo->AutoObEn = DISABLE;
    if (SiePreInfo->AutoObEn == DISABLE)
    {
        SetIPPSizeInfor(_OBOffset, OB_OFS_CAP);
    }
    else
    {
        SetIPPSizeInfor(_OBOffset, 0x0);
    }
    SiePreInfo->SieVDInv = Invert;
    SiePreInfo->SieHDInv = Invert;
    SiePreInfo->PreCfaPat = CFAPAT_CAP;
    SiePreInfo->PreCfaPat2 = CFAPAT_CAP;
//#NT#2009/10/16#Jarkko Chang -begin
//    SiePreInfo->PreSrcSel = PATH_PRE_CLOLRGAIN;
//    SiePreInfo->PreSrcSel = PATH_PRE_GAMMA;
    SiePreInfo->PreSrcSel = PATH_POST_GAMMA;
//#NT#2009/10/16#Jarkko Chang -end
    SiePreInfo->SieOutDest = OUT_PRE_DEST;

    SiePreInfo->HDActX = GetIPPSizeInfor(_HDActX_Capture);
    SiePreInfo->HDActY[0] = GetIPPSizeInfor(_HDActY1_Capture);
    SiePreInfo->HDActY[1] = GetIPPSizeInfor(_HDActY2_Capture);
    SiePreInfo->HDActY[2] = GetIPPSizeInfor(_HDActY3_Capture);
    SiePreInfo->HDActY[3] = GetIPPSizeInfor(_HDActY4_Capture);
    SiePreInfo->HDActY[4] = GetIPPSizeInfor(_HDActY5_Capture);
    SiePreInfo->HDActY[5] = GetIPPSizeInfor(_HDActY6_Capture);
    SiePreInfo->HDActY[6] = GetIPPSizeInfor(_HDActY7_Capture);
    SiePreInfo->HDActY[7] = GetIPPSizeInfor(_HDActY8_Capture);
    SiePreInfo->SieFieldNum.TotalFld = GetIPPSizeInfor(_CapTotalField) - 1;
    SiePreInfo->SieFieldNum.Fld1 = GetIPPSizeInfor(_FieldFld1) - 1;
    SiePreInfo->SieFieldNum.Fld2 = GetIPPSizeInfor(_FieldFld2) - 1;
    SiePreInfo->SieFieldNum.Fld3 = GetIPPSizeInfor(_FieldFld3) - 1;
    SiePreInfo->SieFieldNum.Fld4 = GetIPPSizeInfor(_FieldFld4) - 1;
    SiePreInfo->SieFieldNum.Fld5 = GetIPPSizeInfor(_FieldFld5) - 1;
    SiePreInfo->SieFieldNum.Fld6 = GetIPPSizeInfor(_FieldFld6) - 1;
    SiePreInfo->SieFieldNum.Fld7 = GetIPPSizeInfor(_FieldFld7) - 1;
    SiePreInfo->SieFieldNum.Fld8 = GetIPPSizeInfor(_FieldFld8) - 1;
    SiePreInfo->VDTotal = GetIPPSizeInfor(_VDTotal_Captrue);
    SiePreInfo->VDSync = GetIPPSizeInfor(_VDSync_Captrue);
    SiePreInfo->HDTotal = GetIPPSizeInfor(_HDTotal_Captrue);
    SiePreInfo->HDSync = GetIPPSizeInfor(_HDSync_Captrue);
    SiePreInfo->RawCenterX = GetIPPSizeInfor(_SieXcenter);
    SiePreInfo->RawCenterY = GetIPPSizeInfor(_SieYcenter);
    SiePreInfo->PreCropSizeH = GetIPPSizeInfor(_PreCropCapHsize);
    SiePreInfo->PreCropSizeV = GetIPPSizeInfor(_PreCropCapVsize);
    SiePreInfo->PreOutSizeH = GetIPPSizeInfor(_PreCropCapHsize);
    SiePreInfo->PreOutLineOfs = GetIPPSizeInfor(_PreOutLineOffset);
    SiePreInfo->MShutterTime = DEFAULT_SHUTTER_TIME;
//#NT#2009/10/16#Jarkko Chang -begin
//    SiePreInfo->PreFuncEn = DISABLE;
    SiePreInfo->PreFuncEn = PRE_CG_EN|PRE_GAMMA_EN;//|PRE_VIG_EN;
//#NT#2009/10/16#Jarkko Chang -end

    SiePreInfo->SieObOfs = GetIPPSizeInfor(_OBOffset);
    if (SiePreInfo->AutoObEn == ENABLE)
    {
        SiePreInfo->SieObParam.X0 = GetIPPSizeInfor(_OBWinX0);
        SiePreInfo->SieObParam.X1 = GetIPPSizeInfor(_OBWinX1);
        SiePreInfo->SieObParam.Y0 = GetIPPSizeInfor(_OBWinY0);
        SiePreInfo->SieObParam.Y1 = GetIPPSizeInfor(_OBWinY1);
        SiePreInfo->SieObParam.SubSample = GetIPPSizeInfor(_OBSubSample);
        SiePreInfo->SieObParam.Thres = GetIPPSizeInfor(_OBThreshold);
        SiePreInfo->SieObGain = GetIPPSizeInfor(_OBGain);
    }


    if (gImageAlgInfo.SieBitDepth== _SieBitDepth_8)
    {
        SiePreInfo->SieBitDepth = SIE_OUT_PACK_8BIT;//SIE out
        SiePreInfo->PreInBitDepth = PRE_INPACK_8BIT;//PRE in
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_12)
    {
        SiePreInfo->SieBitDepth = SIE_OUT_PACK_12BIT;//SIE out
        SiePreInfo->PreInBitDepth = PRE_INPACK_12BIT;//PRE in
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_16)
    {
        SiePreInfo->SieBitDepth = SIE_OUT_PACK_16BIT;//SIE out
        SiePreInfo->PreInBitDepth = PRE_INPACK_16BIT;//PRE in
    }
    else
    {
        debug_err(("Cap SieBitDepth error\r\n"));
    }
    if (gImageAlgInfo.PreCapBitDepth == _PreBitDepth_8)
    {
        SiePreInfo->PreBitDepth = PRE_OUTPACK_8BIT;//PRE out
    }
    else if (gImageAlgInfo.PreCapBitDepth == _PreBitDepth_12)
    {
        SiePreInfo->PreBitDepth = PRE_OUTPACK_12BIT;//PRE out
    }
    else if (gImageAlgInfo.PreCapBitDepth == _PreBitDepth_16)
    {
        SiePreInfo->PreBitDepth = PRE_OUTPACK_16BIT;//PRE out
    }
    else
    {
        debug_err(("Cap PreBitDepth error\r\n"));
    }

    //#NT#2011/09/09#Spark Chou -begin
    //#NT#
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        SiePreInfo->SieVDInv = Notinvert;
        SiePreInfo->SieHDInv = Notinvert;

        SiePreInfo->SieHDPhase = SieLoad_FallingEdge;
        SiePreInfo->SieVDPhase = SieLoad_FallingEdge;
        SiePreInfo->SieDataPhase = SieLoad_RisingEdge;
        SiePreInfo->PreSrcSel = PATH_PRE_CLOLRGAIN;
        SiePreInfo->PreFuncEn = DISABLE;
        SetIPPSizeInfor(_OBOffset, 0x0);
        SiePreInfo->SieObOfs = GetIPPSizeInfor(_OBOffset);
    }
    //#NT#2011/09/09#Spark Chou -end
}

/*************************** calibration function ***************************/
void CalCaptureOne(UINT32 BayerAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 PrePath, UINT32 ObOfs, UINT32 ShutterTimeUs)
{
    IPL_OBJ IplObj;
    UINT32 tmpV;

    gSenCBInfo.IPPImgMode = IPPImg_BeforCap;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
    }
    UI_SetParameter(_PreCapBitDepth, _PreBitDepth_8);
    SetIPPSizeInfor(_PreCropCapHsize, RawSizeH);
    SetIPPSizeInfor(_PreCropCapVsize, RawSizeV);
    SetIPPSizeInfor(_PreOutLineOffset, (RawSizeH * gImageAlgInfo.PreCapBitDepth / 8));
    SetSensorMode(STILL_MODE);
    SetIPPSizeInfor(_OBOffset, ObOfs);
    SetSiePreCapParam(&SIE_PRE_Para_cap);

    AePrv2Cap();

    SIE_PRE_Para_cap.AutoObEn = DISABLE;
    SIE_PRE_Para_cap.MShutterTime = ShutterTimeUs;
    SIE_PRE_Para_cap.PreFuncEn = DISABLE;
    SIE_PRE_Para_cap.PreOutAdd0 = BayerAddr;
    SIE_PRE_Para_cap.PreSrcSel = PrePath;
    tmpV = (RawSizeV + (GetIPPSizeInfor(_CapTotalField) - 1)) / (GetIPPSizeInfor(_CapTotalField) - 0);
//    tmpV = (tmpV + (4*GetIPPSizeInfor(_HDDivision_Num_Cap)-1)) / (4*GetIPPSizeInfor(_HDDivision_Num_Cap)) * (4*GetIPPSizeInfor(_HDDivision_Num_Cap));
    tmpV = tmpV * GetIPPSizeInfor(_CapTotalField);
    SIE_PRE_Para_cap.SieOutAdd0 = BayerAddr + (RawSizeH * tmpV);
    SIE_PRE_Para_cap.SieOutAdd1 = SIE_PRE_Para_cap.SieOutAdd0 + Get_DzoomCapMaxFieldBuffer_SIE(&SIE_PRE_Para_cap);
    SetIPPFuncInfor(DFS_EN, OFF);
    SetIPPFuncInfor(FQV_EN, DISABLE);

    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_CAP;
    IPL_SetCmd(&IplObj);

    sie_Set2Capture(&SIE_PRE_Para_cap);
}

//#NT#2009/11/23#Jarkko Chang -begin
//#NT#add for pc calibration using
void CalCaptureOne_LSC(UINT32 BayerAddr, UINT32 RawSizeH, UINT32 RawSizeV, UINT32 PrePath, UINT32 ObOfs, UINT32 ShutterTimeUs)
{
    IPL_OBJ IplObj;
    UINT32 tmpV;

    gSenCBInfo.IPPImgMode = IPPImg_BeforCap;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
    }
    UI_SetParameter(_PreCapBitDepth, _PreBitDepth_8);
    SetIPPSizeInfor(_PreCropCapHsize, RawSizeH);
    SetIPPSizeInfor(_PreCropCapVsize, RawSizeV);
    SetIPPSizeInfor(_PreOutLineOffset, (RawSizeH * gImageAlgInfo.PreCapBitDepth / 8));

            SetIPPSizeInfor(_CapTotalField, SensorCapTotalFld);
            SetIPPSizeInfor(_FieldFld1, Field_Num1);
            SetIPPSizeInfor(_FieldFld2, Field_Num2);
            SetIPPSizeInfor(_FieldFld3, Field_Num3);
            SetIPPSizeInfor(_FieldFld4, Field_Num4);
            SetIPPSizeInfor(_FieldFld5, Field_Num5);
            SetIPPSizeInfor(_FieldFld6, Field_Num6);
            SetIPPSizeInfor(_FieldFld7, Field_Num7);
            SetIPPSizeInfor(_FieldFld8, Field_Num8);
//            SetIPPSizeInfor(_SieXcenter, SIE_CAP_XCENTER);
//            SetIPPSizeInfor(_SieYcenter, SIE_CAP_YCENTER);
//            SetIPPSizeInfor(_HDActX_Capture, ACT_STRX_Cap);
//            SetIPPSizeInfor(_HDActY1_Capture, ACT_STRY_Cap);
            SetIPPSizeInfor(_HDActY2_Capture, ACT_STRY_Cap2);
            SetIPPSizeInfor(_HDActY3_Capture, ACT_STRY_Cap3);
            SetIPPSizeInfor(_HDActY4_Capture, ACT_STRY_Cap4);
            SetIPPSizeInfor(_HDActY5_Capture, ACT_STRY_Cap5);
            SetIPPSizeInfor(_HDActY6_Capture, ACT_STRY_Cap6);
            SetIPPSizeInfor(_HDActY7_Capture, ACT_STRY_Cap7);
            SetIPPSizeInfor(_HDActY8_Capture, ACT_STRY_Cap8);
            SetIPPSizeInfor(_HDTotal_Captrue, HD_Total_Cap);
            SetIPPSizeInfor(_HDSync_Captrue, HD_Sync_Cap);
            SetIPPSizeInfor(_VDTotal_Captrue, VD_Total_Cap);
            SetIPPSizeInfor(_VDSync_Captrue, VD_Sync_Cap);

            SetIPPSizeInfor(_OBWinX0, CAP_OB_X0);
            SetIPPSizeInfor(_OBWinX1, CAP_OB_X1);
            SetIPPSizeInfor(_OBWinY0, CAP_OB_Y0);
            SetIPPSizeInfor(_OBWinY1, CAP_OB_Y1);
            SetIPPSizeInfor(_OBSubSample, CAP_OB_SUBSAMPLE);
            SetIPPSizeInfor(_OBThreshold, CAP_OB_THRES);
            SetIPPSizeInfor(_OBGain, CAP_OB_GAIN);

    SetIPPSizeInfor(_OBOffset, ObOfs);
    SetSiePreCapParam(&SIE_PRE_Para_cap);

    AePrv2Cap();

    SIE_PRE_Para_cap.AutoObEn = DISABLE;
    SIE_PRE_Para_cap.MShutterTime = ShutterTimeUs;
    SIE_PRE_Para_cap.PreFuncEn = DISABLE;
    SIE_PRE_Para_cap.PreOutAdd0 = BayerAddr;
    SIE_PRE_Para_cap.PreSrcSel = PrePath;
    tmpV = (RawSizeV + (GetIPPSizeInfor(_CapTotalField) - 1)) / (GetIPPSizeInfor(_CapTotalField) - 0);
//    tmpV = (tmpV + (4*GetIPPSizeInfor(_HDDivision_Num_Cap)-1)) / (4*GetIPPSizeInfor(_HDDivision_Num_Cap)) * (4*GetIPPSizeInfor(_HDDivision_Num_Cap));
    tmpV = tmpV * GetIPPSizeInfor(_CapTotalField);
    SIE_PRE_Para_cap.SieOutAdd0 = BayerAddr + (RawSizeH * tmpV);
    SIE_PRE_Para_cap.SieOutAdd1 = SIE_PRE_Para_cap.SieOutAdd0 + Get_DzoomCapMaxFieldBuffer_SIE(&SIE_PRE_Para_cap);
    SetIPPFuncInfor(DFS_EN, OFF);
    SetIPPFuncInfor(FQV_EN, DISABLE);

    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_CAP;
    IPL_SetCmd(&IplObj);

//    SIE_PRE_Para_cap.RawCenterX = 1316;
//    SIE_PRE_Para_cap.RawCenterY = 981;
    debug_err(("------>center =  %d,%d\r\n",SIE_PRE_Para_cap.RawCenterX,SIE_PRE_Para_cap.RawCenterY));//Jarkko tmp
    sie_Set2Capture(&SIE_PRE_Para_cap);
}
//#NT#2009/11/23#Jarkko Chang -end

void CalPreviewOne(UINT32 RawAddr, UINT32 bitDepth, UINT32 *RawHsize, UINT32 *RawVsize, UINT32 ObOfs)
{
    StopPreviewDramData();
    UI_SetParameter(_SieBitDepth, bitDepth);
    Set_digitalzoom(10, &SIE_PRE_Para_prv);

    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    SIE_PRE_Para_prv.SieBp2 = PRV_SIE_BP2;
    SIE_PRE_Para_prv.PreIntEn = PRE_INTE_FLDEND|PRE_INTE_BP1;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PINGPONGBUF_DEST;
    SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
    SIE_PRE_Para_prv.PreFuncEn = DISABLE;
    SIE_PRE_Para_prv.PreCfaPat = CFAPAT_PRV;
    SIE_PRE_Para_prv.PreCfaPat2 = CFAPAT_PRV;
    SIE_PRE_Para_prv.SieVDInv = Invert;
    SIE_PRE_Para_prv.SieHDInv = Invert;

    SetIPPSizeInfor(_OBOffset, ObOfs);
    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.Ipe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct = &IME_D1_Para;
    SetPrvParams(&Ipp_Param);

    SIE_PRE_Para_prv.SieOutAdd0 = RawAddr;
    SIE_PRE_Para_prv.SieOutAdd1= RawAddr;

    IPPMgrReq.SIE_PRE_PRV = &SIE_PRE_Para_prv;
    if (Sensor_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    if ( SIE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
    sie_waitVD(1);
    sie_start();
    sie_waitVD(1);
    sie_pause();
    if (gImageAlgInfo.SieBitDepth == _SieBitDepth_8)
    {
        *RawHsize = SIE_PRE_Para_prv.SieOutLineOfs;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_12)
    {
        *RawHsize = (SIE_PRE_Para_prv.SieOutLineOfs/12*8);
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_16)
    {
        *RawHsize = SIE_PRE_Para_prv.SieOutLineOfs/2;
    }
    else
    {
        debug_err(("ERROR!!\r\n"));
    }

    *RawVsize = SIE_PRE_Para_prv.SieOutSizeV;
}

//#NT#2009/10/14#Jarkko Chang -end

void CalPreviewOnePRE(UINT32 RawAddr, UINT32 bitDepth, UINT32 *RawHsize, UINT32 *RawVsize, UINT32 ObOfs)
{
    UINT32 Hsize, Vsize, LineOft;

    //++  First Get RawHsize & RawVsize begin
    gAlgMsg.Alg_Mode=MODE_Preview;

    SIE_PRE_Para_prv.ModeSel = SENSOR_PRV_MODE;
    SIE_PRE_Para_prv.VGAType = SENSOR_PRV_VGATYPE;


    SetSensorMode(SIE_PRE_Para_prv.ModeSel);

    //initial sie & pre parameter
    memset(&SIE_PRE_Para_prv.SieFieldNum, 0, sizeof(STR_FLDNUM));
    SIE_PRE_Para_prv.VDTotal = GetIPPSizeInfor(_VDTotal_Preview);
    SIE_PRE_Para_prv.VDSync = GetIPPSizeInfor(_VDSync_Preview);
    SIE_PRE_Para_prv.HDActX = GetIPPSizeInfor(_HDActX_Preview);
    SIE_PRE_Para_prv.HDActY[0] = GetIPPSizeInfor(_HDActY_Preview);
    SIE_PRE_Para_prv.HDTotal = GetIPPSizeInfor(_HDTotal_Preview);
    SIE_PRE_Para_prv.HDSync = GetIPPSizeInfor(_HDSync_Preview);
    SIE_PRE_Para_prv.RawCenterX = GetIPPSizeInfor(_SieXcenter);
    SIE_PRE_Para_prv.RawCenterY = GetIPPSizeInfor(_SieYcenter);

    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    SIE_PRE_Para_prv.SieBp2 = PRV_SIE_BP2;
    SIE_PRE_Para_prv.SieMaster = 1;
    SIE_PRE_Para_prv.SieHDPhase = 1;
    SIE_PRE_Para_prv.SieVDPhase = 0;
    SIE_PRE_Para_prv.SieDataPhase = 0;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.SieObOfs = ObOfs;
    SIE_PRE_Para_prv.SieOutDest = OUT_DRAM_DEST;
    SIE_PRE_Para_prv.SieOutAdd0= RawAddr;
    SIE_PRE_Para_prv.SieVDInv = Invert;
    SIE_PRE_Para_prv.SieHDInv = Invert;


    if (gImageAlgInfo.SieBitDepth == _SieBitDepth_8)
    {
        SIE_PRE_Para_prv.SieBitDepth = SIE_OUT_PACK_8BIT;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_12)
    {
        SIE_PRE_Para_prv.SieBitDepth = SIE_OUT_PACK_12BIT;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_16)
    {
        SIE_PRE_Para_prv.SieBitDepth = SIE_OUT_PACK_16BIT;
    }
    else
    {
        debug_err(("ERROR!!\r\n"));
    }

    IPPMgrReq.SIE_PRE_PRV = &SIE_PRE_Para_prv;

    if (Sensor_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    if (gImageAlgInfo.SieBitDepth == _SieBitDepth_8)
    {
        Hsize = SIE_PRE_Para_prv.SieOutLineOfs;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_12)
    {
        Hsize = (SIE_PRE_Para_prv.SieOutLineOfs*12)/8;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_16)
    {
        Hsize = SIE_PRE_Para_prv.SieOutLineOfs/2;
    }
    else
    {
        Hsize = SIE_PRE_Para_prv.SieOutLineOfs;
        debug_err(("ERROR!!\r\n"));
    }

    Vsize = SIE_PRE_Para_prv.SieOutSizeV;
    debug_msg("SieOutSizeH = %d\r\n",Hsize);
    debug_msg("SieOutSizeV = %d\r\n",Vsize);

    gImageAlgInfo.PrePrvBitDepth = bitDepth;

    SIE_PRE_Para_prv.ModeSel = SENSOR_PRV_MODE;
    SIE_PRE_Para_prv.VGAType = SENSOR_OUT_PROGRESSIVE;
    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);
    Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);
    IMEReset();
    IPEReset();
    SIEReset();

    SetSensorMode(SIE_PRE_Para_prv.ModeSel);

    //initial sie & pre parameter
    SIE_PRE_Para_prv.VDTotal = GetIPPSizeInfor(_VDTotal_Preview);
    SIE_PRE_Para_prv.VDSync = GetIPPSizeInfor(_VDSync_Preview);
    SIE_PRE_Para_prv.HDActX = GetIPPSizeInfor(_HDActX_Preview);
    SIE_PRE_Para_prv.HDActY[0] = GetIPPSizeInfor(_HDActY_Preview);
    SIE_PRE_Para_prv.HDTotal = GetIPPSizeInfor(_HDTotal_Preview);
    SIE_PRE_Para_prv.HDSync = GetIPPSizeInfor(_HDSync_Preview);
    SIE_PRE_Para_prv.RawCenterX = GetIPPSizeInfor(_SieXcenter);
    SIE_PRE_Para_prv.RawCenterY = GetIPPSizeInfor(_SieYcenter);
    SIE_PRE_Para_prv.PreCropSizeH = Hsize;
    SIE_PRE_Para_prv.PreCropSizeV = Vsize;
    SIE_PRE_Para_prv.PreOutSizeH = Hsize;
    SIE_PRE_Para_prv.PreOutAdd0 = RawAddr;
    SIE_PRE_Para_prv.PreOutAdd1 = RawAddr;


    if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_8)
    {
        SIE_PRE_Para_prv.PreBitDepth = PRE_OUTPACK_8BIT;
        LineOft = Hsize;
    }
    else if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_10)
    {
        SIE_PRE_Para_prv.PreBitDepth = PRE_OUTPACK_10BIT;
        LineOft = (Hsize*10)/8;
    }
    else if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_12)
    {
        SIE_PRE_Para_prv.PreBitDepth = PRE_OUTPACK_12BIT;
        LineOft = (Hsize*12)/8;
    }
    else if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_16)
    {
        SIE_PRE_Para_prv.PreBitDepth = PRE_OUTPACK_16BIT;
        LineOft = Hsize*2;
    }
    else
    {
        SIE_PRE_Para_prv.PreBitDepth = PRE_OUTPACK_8BIT;
        LineOft = Hsize;
    }

    SIE_PRE_Para_prv.PreOutLineOfs = ((LineOft + 3)/4)*4;

    SIE_PRE_Para_prv.PreIntEn = PRE_INTE_FLDEND|PRE_INTE_BP1;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieBp1 = 100;
    SIE_PRE_Para_prv.SieBp2 = 176;
    SIE_PRE_Para_prv.SieMaster = 1;
    SIE_PRE_Para_prv.SieHDPhase = 1;
    SIE_PRE_Para_prv.SieVDPhase = 0;
    SIE_PRE_Para_prv.SieDataPhase = 0;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;
    SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
    SIE_PRE_Para_prv.PreFuncEn = DISABLE;
    SIE_PRE_Para_prv.SieObOfs = ObOfs;

    sie_Set2Preview(&SIE_PRE_Para_prv);
    pre_SetHcropSize(0, Hsize);
    pre_SetVcropSize(0, Vsize);

    sie_waitVD(1);

    sie_start();

    sie_waitVD(1);

    sie_pause();
    sie_waitVD(1);

    *RawHsize = Hsize;
    *RawVsize = Vsize;

}

UINT32 Sen_GetMemMode(void)
{
    return SEN_MEM_SEPARATE_MODE;
}
#endif
