/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: SensorSys_OV5653_630.c                                           *
* Description:     Layer 3 API of Alg                                      *
* Author:                                                            *
****************************************************************************/
#include    "sensor_model.h"
#if OV5653_630
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
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
#include    "sen_snr.h"
//#NT#2010/12/15#JJ Huang -end

#define DP_Prv_MAX 1000
#define DP_Prv_MAX_2 1000
#define DP_Prv_MAX_3 1000

#if(SENSOR_GRS_MODE == 1)
volatile BOOL GRS_MODE = TRUE;
 #else
volatile BOOL GRS_MODE = FALSE;
#endif

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

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//#NT#20090918#ethanlau -begin
UINT32  SensorModeMap(_IMGResolution_sel _mode)
{
    switch(_mode)
    {
        case _Resolution_320x240:
        case _Resolution_640x480:
            UI_SetParameter(_ImageRatio, _Ratio_4_3);
            //return MONITOR_MODE_4;
            return MONITOR_MODE_3;
        case _Resolution_848x480:
            UI_SetParameter(_ImageRatio, _Ratio_16_9);
            //return MONITOR_MODE_4;
            return MONITOR_MODE_3;
        case _Resolution_720x480:
            UI_SetParameter(_ImageRatio, _Ratio_3_2);
            return MONITOR_MODE_4;
        case _Resolution_1280x480:
            UI_SetParameter(_ImageRatio, _Ratio_16_9);
            return MONITOR_MODE_4;
        case _Resolution_1280x720:
            UI_SetParameter(_ImageRatio, _Ratio_16_9);
            return MONITOR_MODE_6;
        case _Resolution_1440x1080:
            UI_SetParameter(_ImageRatio, _Ratio_4_3);
            return MONITOR_MODE_5;
        case _Resolution_1280x960:
            UI_SetParameter(_ImageRatio, _Ratio_4_3);
            return MONITOR_MODE_3;
        case _Resolution_1920x1080:
            UI_SetParameter(_ImageRatio, _Ratio_16_9);
            return MONITOR_MODE_7;
        case _Resolution_640x360:
            UI_SetParameter(_ImageRatio, _Ratio_16_9);
            return MONITOR_MODE_6;
        default:
            UI_SetParameter(_ImageRatio, _Ratio_4_3);
            return MONITOR_MODE;
        break;
    }
}
//#NT#20090918#ethanlau -end

void Set2Preview(IPL_OBJ *Obj)
{
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();
    debug_msg("- Set2Preview enter\r\n");
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

    if(IPL_GetNextMode()==IPL_MODE_PREVIEW)
    {
        SetIPPSizeInfor(_Prv_ResolutionIdx,gImageAlgInfo.Prv_Resolution);
        debug_msg("gImageAlgInfo.Prv_Resolution= %d\r\n",gImageAlgInfo.Prv_Resolution);
        if(GetIPPSizeInfor(_Prv_ResolutionIdx) >= _Resolution_Max)
            debug_err(("Resolution error\r\n"));
        else
            SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Prv_ResolutionIdx));//SENSOR_PRV_MODE;
    }
    else if(IPL_GetNextMode()==IPL_MODE_VIDEO)
    {
        SetIPPSizeInfor(_Vid_ResolutionIdx,gImageAlgInfo.Vid_Resolution);
        debug_msg("gImageAlgInfo.Vid_Resolution= %d\r\n",gImageAlgInfo.Vid_Resolution);
        if(GetIPPSizeInfor(_Vid_ResolutionIdx) >= _Resolution_Max)
            debug_err(("Resolution error\r\n"));
        else
            SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Vid_ResolutionIdx));//SENSOR_PRV_MODE;
    }
    else
    {
        debug_err(("IPL next mode error\r\n"));
    }
    //#NT#20090918#etahnlau -end
    SIE_PRE_Para_prv.VGAType = SENSOR_PRV_VGATYPE;
    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);
    Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);

    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    SIE_PRE_Para_prv.SieBp2 = PRV_SIE_BP2;
    SIE_PRE_Para_prv.PreIntEn = PRE_INTE_FLDEND|PRE_INTE_BP1;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;//OUT_PINGPONGBUF_DEST;//OUT_PRE_DEST;
    SIE_PRE_Para_prv.PreSrcSel = PATH_POST_GAMMA;
    //SIE_PRE_Para_prv.PreFuncEn = DISABLE;
    SIE_PRE_Para_prv.PreFuncEn = PRE_GAMMA_EN|PRE_CA_EN|PRE_CG_EN; //;PRE_VIG_EN|PRE_GAMMA_EN|PRE_CA_EN|PRE_CG_EN;
    SIE_PRE_Para_prv.PreCfaPat = CFAPAT_PRV;
    SIE_PRE_Para_prv.PreCfaPat2 = CFAPAT_PRV;
    SIE_PRE_Para_prv.SieVDInv = Notinvert;
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
    //#NT#20091120#ethanlau -begin
    if ((CalInfo->CalStatus_PrvDP_B == _Prv_B_Status) && (CalInfo->CalStatus_PrvDP_W == _Prv_W_Status))
    {
        #if 0
        switch(SIE_PRE_Para_prv.ModeSel)
        {
            case MONITOR_MODE_4:
                SIE_PRE_Para_prv.SiePreAddr2 = CalInfo->Cal_DP_PreviewAddr;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
            case MONITOR_MODE_2:
                SIE_PRE_Para_prv.SiePreAddr2 = (CalInfo->Cal_DP_PreviewAddr) + DP_Prv_MAX<<2;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
            case MONITOR_MODE_5:
                SIE_PRE_Para_prv.SiePreAddr2 = (CalInfo->Cal_DP_PreviewAddr) + (DP_Prv_MAX+DP_Prv_MAX_2)<<2;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
        }
        #else
            SIE_PRE_Para_prv.SiePreAddr2 = CalInfo->Cal_DP_PreviewAddr;
            SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
        #endif
    }
    else
    {
        debug_err(("_Prv_B_Status=(%d)%d ,_Prv_W_Status=(%d)%d",_Prv_B_Status,CalInfo->CalStatus_PrvDP_B,_Prv_W_Status,CalInfo->CalStatus_PrvDP_W));
    }
    //#NT#20091120#ethanlau -end
    //initial ipe parameter
    IPE_Para.CfaPat = CFAPAT_PRV;
    //#NT#2010/04/30#Jarkko Chang -begin
    //#NT#add for stripe mode change
    IME_D1_Para.ime_sten = IMEALG_MULTI;
    IPE_Para.StripeMode = IPE_MSTP;
    //#NT#2010/04/30#Jarkko Chang -end

    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.Ipe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct = &IME_D1_Para;
    Set2Prv_Common(&Ipp_Param);
    sie_waitVD(1);
    //#NT#2010/09/16#ethanlau -begin
    //#NT#for ae data ouput
    sie_waitVD(1);
    //#NT#2010/09/16#ethanlau -end
    ime_clrIntFlag();
    ime_waitFlagFrameEnd();
    ime_waitFlagFrameEnd();
    ime_waitFlagFrameEnd();

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    if(g_fpAlgExport)
    {
        Ae_FastConverge2TargetLumi(GammaLUT_128Tab,_SieBitDepth_10,0);
        //WaitAeStable(2);
        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
    }
    debug_msg("- Set2Preview end\r\n");
}

//#NT#20090912#ethanlau -begin
//for sensor chg mode(stop sie crop,not to disable ide)
void SensorMode(IPL_MODE mode)
{
    static BOOL _first=TRUE;
    PImageAlgInfo  pimgalginfo;
    static UINT ModeSel,imgratio;
    debug_msg("- SensorMode enter\r\n");
//    if((mode != IPL_MODE_PREVIEW) && (mode != IPL_MODE_VIDEO))
    if((mode != IPL_MODE_PREVIEW) && (mode != IPL_MODE_VIDEO) && (mode != IPL_MODE_PCC))
    {
        debug_err(("Only support IPL_MODE_PREVIEW|IPL_MODE_VIDEO!\r\n"));
        return;
    }
    pimgalginfo = UI_GetParameter();
    ModeSel = SIE_PRE_Para_prv.ModeSel;
    imgratio = pimgalginfo->ImageRatio;
    if(mode == IPL_MODE_PREVIEW)
    {
        debug_msg("\r\nSensorMode CHG(PRV) , from %d to ",GetIPPSizeInfor(_Prv_ResolutionIdx));
        SetIPPSizeInfor(_Prv_ResolutionIdx,pimgalginfo->Prv_Resolution);
        debug_msg("%d \r\n",GetIPPSizeInfor(_Prv_ResolutionIdx));
        SIE_PRE_Para_prv.ModeSel = SensorModeMap(GetIPPSizeInfor(_Prv_ResolutionIdx));//SENSOR_PRV_MODE;
    }
    else
    {
        debug_msg("\r\nSensorMode CHG(VID) , from %d to ",GetIPPSizeInfor(_Vid_ResolutionIdx));
        SetIPPSizeInfor(_Vid_ResolutionIdx,pimgalginfo->Vid_Resolution);
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
    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);

    if(!_first)
        Set_digitalzoom(GetDZoomIdxFromRatio(gAlgMsg.DzoomRealRatio,&SIE_PRE_Para_prv),&SIE_PRE_Para_prv);
    else
        Set_digitalzoom(Get_DzoomIndex(), &SIE_PRE_Para_prv);

    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;
    Ipp_Param.Sie_Struct = &SIE_PRE_Para_prv;
    Ipp_Param.Ipe_Struct = &IPE_Para;
    Ipp_Param.Ime_Struct = &IME_D1_Para;

    Set2Prv_Common(&Ipp_Param);

    sie_waitVD(1);
    //#NT#2010/09/16#ethanlau -begin
    //#NT#for ae data ouput
    sie_waitVD(1);
    //#NT#2010/09/16#ethanlau -end
    ime_clrIntFlag();
    ime_waitFlagFrameEnd();
    ime_waitFlagFrameEnd();
    ime_waitFlagFrameEnd();

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch,&gSenCBInfo);
    if(g_fpAlgExport)
    {
        Ae_FastConverge2TargetLumi(GammaLUT_128Tab,_SieBitDepth_10,0);
        //WaitAeStable(2);
        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
    }
    _first = FALSE;
    debug_msg("- SensorMode leave\r\n");
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

    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    ime_Set2Preview(&IME_D1_Para);
    SenFlowFunc(AE_PRV_SETTING, &gSenCBInfo);
    SenFlowFunc(AWB_PRV_SETTING, &gSenCBInfo);

    gSenCBInfo.IPPImgMode = IPPImg_Preview;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
//#NT#2009/09/02#Jarkko Chang -end
}

void Set2Video(IPL_OBJ *Obj)
{
    gAlgMsg.Alg_Mode=MODE_Video_Recording;
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_VIDEO_RECORDING, &gAlgMsg);
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

    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    ime_Set2Video(&IME_D1_Para,&IME_D2_Para);
    SenFlowFunc(AE_PRV_SETTING, &gSenCBInfo);
    SenFlowFunc(AWB_PRV_SETTING, &gSenCBInfo);

    gSenCBInfo.IPPImgMode = IPPImg_Video;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
}


void Set2Pcc(IPL_OBJ *Obj)
{
#if 1
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();

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

    //#NT#20090921#ethanlau -begin
//add for init in video mode
    //#NT#20091006#ethanlau -begin
    //move to systeminit
    //Set_SIE_DMA_Priority((P0_PRE_DVI|P0_PRE_DPC|P0_PRE_ECS|P0_PRE_PREO));
    //#NT#20091006#ethanlau -end
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
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;//OUT_PINGPONGBUF_DEST;//OUT_PRE_DEST;
//    SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
//    SIE_PRE_Para_prv.PreFuncEn = PRE_VIG_EN;
    SIE_PRE_Para_prv.PreSrcSel = PATH_POST_GAMMA;
    SIE_PRE_Para_prv.PreFuncEn = PRE_VIG_EN|PRE_GAMMA_EN|PRE_CA_EN|PRE_CG_EN;
//#NT#2009/09/11#Jarkko Chang -end
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
        switch(SIE_PRE_Para_prv.ModeSel)
        {
            case MONITOR_MODE_4:
                SIE_PRE_Para_prv.SiePreAddr2 = CalInfo->Cal_DP_PreviewAddr;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
            case MONITOR_MODE_2:
                SIE_PRE_Para_prv.SiePreAddr2 = (CalInfo->Cal_DP_PreviewAddr) + DP_Prv_MAX<<2;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
            case MONITOR_MODE_5:
                SIE_PRE_Para_prv.SiePreAddr2 = (CalInfo->Cal_DP_PreviewAddr) + (DP_Prv_MAX+DP_Prv_MAX_2)<<2;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
            case MONITOR_MODE_6:
                SIE_PRE_Para_prv.SiePreAddr2 = (CalInfo->Cal_DP_PreviewAddr) + (DP_Prv_MAX+DP_Prv_MAX_2)<<2;
                SIE_PRE_Para_prv.PreFuncEn |= PRE_BLM_EN;
            break;
        }
    }
    else
    {
        debug_err(("_Prv_B_Status=(%d)%d ,_Prv_W_Status=(%d)%d",_Prv_B_Status,CalInfo->CalStatus_PrvDP_B,_Prv_W_Status,CalInfo->CalStatus_PrvDP_W));
    }
    //#NT#20091120#ethanlau -end

    //initial ipe parameter
    IPE_Para.CfaPat = CFAPAT_PRV;

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
        Ae_FastConverge2TargetLumi(GammaLUT_128Tab,_SieBitDepth_10,0);
        g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
    }
#endif
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

    Clr_DZoomFlag(DZOOM_IDLE);
    Set_DZoomFlag(DZOOM_SET_SIEPRE);
    while(!(Get_DZoomFlag() & DZOOM_IDLE));

    gAlgMsg.DzoomIndex=ZoomIndex;
    gAlgMsg.DzoomRealRatio = GetRealDZoomRatio(ZoomIndex,&SIE_PRE_Para_prv);
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
    Clr_DZoomFlag(DZOOM_IDLE);
    Set_DZoomFlag(DZOOM_SET_SIEPRE);
    while(!(Get_DZoomFlag() & DZOOM_IDLE));

    gSenCBInfo.IPPImgMode = IPPImg_DzoomLeave;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);
    gAlgMsg.DzoomIndex=ZoomIndex;
    gAlgMsg.DzoomRealRatio = GetRealDZoomRatio(ZoomIndex,&SIE_PRE_Para_prv);
    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_CHGDZOOMSTEP, &gAlgMsg);
    }
}

#if 0
#pragma mark -
#endif



#if 0
#pragma mark -
#endif

void CaptureOne(SIE_PRE_PARAM *SiePreParam)
{
    Cal_Struct *CalInfo;
    UINT32 Addr1, Addr2;//,FieldV;
    CalInfo = Cal_GetCalibrationData();
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
        if(GetIPPFuncInfor(PRED2D_EN))
        {
            SiePreParam->PreFuncEn &= ~(PRE_CG_EN|PRE_GAMMA_EN);
        }
        sie_Set2Capture(SiePreParam);
        //for awb re-cal
        if(GetIPPFuncInfor(PRED2D_EN))
        {
            SIE_PRE_Para_d2d.PreFuncEn = (PRE_CG_EN|PRE_GAMMA_EN);
            SIE_PRE_Para_d2d.PreCfaPat = SiePreParam->PreCfaPat;
            SIE_PRE_Para_d2d.PreCfaPat2 = SiePreParam->PreCfaPat2;
            debug_msg("CaptureOne- %d %d\r\n",SIE_PRE_Para_d2d.PreCfaPat,SIE_PRE_Para_d2d.PreCfaPat2);
        }
        //if(!GetIPPFuncInfor(PRED2D_EN))
            Sensor_Cap2Prv();
        SiePreParam->SiePreAddr2 = Addr2;
    }
#if 0
    {
        unsigned short r,g,b;
        debug_msg("Cap Raw Y  = %d\r\n",CalculateRawY(SiePreParam->PreOutAdd0,0,0,SiePreParam->PreOutLineOfs,SiePreParam->PreCropSizeV,8,0,&r,&g,&b));
    }
#endif
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

//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
    if(SenSNR_IsSNRMeasureEnable())
    {
        UINT32 uiSNR, uiMean;

        uiSNR = SenSNR_Calculate_SNR(  SiePreParam->PreOutAdd0,
            SiePreParam->PreCropSizeH,
            SiePreParam->PreCropSizeV,
            IPE_PATGB,//CFAPAT_CAP,
            16,
            64,
            64,
            NULL,
            NULL);

        {// show on OSD
            extern void Cal_ShowString(INT8 *pStr, PRECT pRect, BOOL bClear);
            extern char message[160];
            RECT rectDialog={50,50,100,100};
            UINT32 uiVal;
            uiVal = SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_SNR);
            sprintf(message, "SNR %3d . %03d", uiVal/1000, uiVal%1000);
            Cal_ShowString(message, &rectDialog, TRUE);
            uiVal = SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_MEAN);
            sprintf(message, "MEAN %3d . %03d", uiVal/1000, uiVal%1000);
            rectDialog.uiTop=100;
            Cal_ShowString(message, &rectDialog, TRUE);
            uiVal = SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_NONUNIFORMITY);
            sprintf(message, "NUFM %3d . %03d", uiVal/1000, uiVal%1000);
            rectDialog.uiTop=130;
            Cal_ShowString(message, &rectDialog, TRUE);

            TimerDelayMs(1500);
        }

    }
//#NT#2010/12/15#JJ Huang -end

}

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
    UINT32 Org_PreFunc=SiePreParam->PreFuncEn;
    //#NT#2010/09/17#ethanlau -begin
    //#NT# chg mclk
    IPP_ClockIOInit(SiePreParam->VGAType,SEN_MODE_FULL);
    //#NT#2010/09/17#ethanlau -end

    while (!(sie_ReadIntrStatus() & SIE_INT_FLDEND));
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_IntrptEnable(ENABLE, SIE_INTE_FLDEND|SIE_INTE_BP1);
    ena_int(_INT_SIE);
    sie_waitBP1(TRUE);
    //#NT#20091022#ethanlau -begin
    flash_ConfigMainFlash();
    flash_TriggerFlash();
    //#NT#20091022#ethanlau -end
    Sensor_SetMode(SEN_MODE_FULL);
    sie_waitBP1(TRUE);
    AeCapSet();

    if(GetAntishaking_RawbiningEnalbe())
    {
        Org_PreFunc = SiePreParam->PreFuncEn;
        SiePreParam->PreFuncEn |= PRE_HSCL_EN;
        pre_setHBining(1,0);
        pre_HScale(SiePreParam->PreCropSizeH,SiePreParam->PreOutSizeH);
        debug_msg("PRE : 2X Bining enable\r\n");
    }
    else
    {
        pre_setHBining(0,0);
    }

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
    if(GetAntishaking_RawbiningEnalbe())
        SiePreParam->PreFuncEn = Org_PreFunc;
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void pre_PrvParameter(struct  PUBLIC_STR_JOBREQS *job_req){
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

  //VIG
  if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_VIG_EN)
  {
//#NT#2009/08/04#Jarkko Chang -begin
//#NT#20091102#ethanlau -begin
        if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE_4)
        {
            job_req->pre_vig_setting.VIG_R_LUT = Shading_Table1280X480;
            job_req->pre_vig_setting.VIG_G_LUT = Shading_Table1280X480;
            job_req->pre_vig_setting.VIG_B_LUT = Shading_Table1280X480;

            job_req->pre_vig_setting.VIG_XR = 629;
            job_req->pre_vig_setting.VIG_YR = 246;
            job_req->pre_vig_setting.VIG_XGR = 629;
            job_req->pre_vig_setting.VIG_YGR = 246;
            job_req->pre_vig_setting.VIG_XGB = 629;
            job_req->pre_vig_setting.VIG_YGB = 246;
            job_req->pre_vig_setting.VIG_XB = 629;
            job_req->pre_vig_setting.VIG_YB = 246;
            job_req->pre_vig_setting.VIG_MUL = 113;
            job_req->pre_vig_setting.VIG_T = 0;
            job_req->pre_vig_setting.VIG_XDIV = 25;
            job_req->pre_vig_setting.VIG_YDIV = 25;
        }
        else if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE_2)
        {
            job_req->pre_vig_setting.VIG_R_LUT = Shading_Table720p;
            job_req->pre_vig_setting.VIG_G_LUT = Shading_Table720p;
            job_req->pre_vig_setting.VIG_B_LUT = Shading_Table720p;

            job_req->pre_vig_setting.VIG_XR = 628;
            job_req->pre_vig_setting.VIG_YR = 369;
            job_req->pre_vig_setting.VIG_XGR = 628;
            job_req->pre_vig_setting.VIG_YGR = 369;
            job_req->pre_vig_setting.VIG_XGB = 628;
            job_req->pre_vig_setting.VIG_YGB = 369;
            job_req->pre_vig_setting.VIG_XB = 628;
            job_req->pre_vig_setting.VIG_YB = 369;
            job_req->pre_vig_setting.VIG_MUL = 97;
            job_req->pre_vig_setting.VIG_T = 0;
            job_req->pre_vig_setting.VIG_XDIV = 25;
            job_req->pre_vig_setting.VIG_YDIV = 25;
        }
        else if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE_3)
        {
            job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R;
            job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_R;
            job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_R;

            job_req->pre_vig_setting.VIG_XR = 629;
            job_req->pre_vig_setting.VIG_YR = 493;
            job_req->pre_vig_setting.VIG_XGR = 629;
            job_req->pre_vig_setting.VIG_YGR = 493;
            job_req->pre_vig_setting.VIG_XGB = 629;
            job_req->pre_vig_setting.VIG_YGB = 493;
            job_req->pre_vig_setting.VIG_XB = 629;
            job_req->pre_vig_setting.VIG_YB = 493;
            job_req->pre_vig_setting.VIG_MUL = 82;
            job_req->pre_vig_setting.VIG_T = 0;
            job_req->pre_vig_setting.VIG_XDIV = 25;
            job_req->pre_vig_setting.VIG_YDIV = 25;
        }
        else if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE_5)
        {
            job_req->pre_vig_setting.VIG_R_LUT = Shading_Table1080p;
            job_req->pre_vig_setting.VIG_G_LUT = Shading_Table1080p;
            job_req->pre_vig_setting.VIG_B_LUT = Shading_Table1080p;

            job_req->pre_vig_setting.VIG_XR = 1259;
            job_req->pre_vig_setting.VIG_YR = 494;
            job_req->pre_vig_setting.VIG_XGR = 1259;
            job_req->pre_vig_setting.VIG_YGR = 494;
            job_req->pre_vig_setting.VIG_XGB = 1259;
            job_req->pre_vig_setting.VIG_YGB = 494;
            job_req->pre_vig_setting.VIG_XB = 1259;
            job_req->pre_vig_setting.VIG_YB = 494;
            job_req->pre_vig_setting.VIG_MUL = 122;
            job_req->pre_vig_setting.VIG_T = 0;
            job_req->pre_vig_setting.VIG_XDIV = 12;
            job_req->pre_vig_setting.VIG_YDIV = 12;
        }
        else if(job_req->SIE_PRE_PRV->ModeSel == MONITOR_MODE_6)
        {
            job_req->pre_vig_setting.VIG_R_LUT = Shading_Table720p;
            job_req->pre_vig_setting.VIG_G_LUT = Shading_Table720p;
            job_req->pre_vig_setting.VIG_B_LUT = Shading_Table720p;

            job_req->pre_vig_setting.VIG_XR = 1259;
            job_req->pre_vig_setting.VIG_YR = 360;
            job_req->pre_vig_setting.VIG_XGR = 1259;
            job_req->pre_vig_setting.VIG_YGR = 360;
            job_req->pre_vig_setting.VIG_XGB = 1259;
            job_req->pre_vig_setting.VIG_YGB = 360;
            job_req->pre_vig_setting.VIG_XB = 1259;
            job_req->pre_vig_setting.VIG_YB = 360;
            job_req->pre_vig_setting.VIG_MUL = 122;
            job_req->pre_vig_setting.VIG_T = 0;
            job_req->pre_vig_setting.VIG_XDIV = 12;
            job_req->pre_vig_setting.VIG_YDIV = 12;
        }
//#NT#20091102#ethanlau -end
//#NT#2009/08/04#Jarkko Chang -end
        job_req->pre_vigline_setting.VIG_R2_LUT = Shading_LineTable;
        job_req->pre_vigline_setting.VIG_G2_LUT = Shading_LineTable;
        job_req->pre_vigline_setting.VIG_B2_LUT = Shading_LineTable;
    }

  //GEO
    if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_GEO_EN)
    {
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
        //#NT#2009/11/02#Bowen Li -begin
        //#NT#modify CA function for 2 & 3 line
        job_req->pre_ca_setting.pre_CA_Hsub = GetIPPSizeInfor(_CAHSub);
        job_req->pre_ca_setting.pre_CA_Vsub = GetIPPSizeInfor(_CAVSub);
        job_req->pre_ca_setting.pre_CA_OutputLineofs = GetIPPSizeInfor(_CALineOffset);
        //#NT#2009/11/02#Bowen Li -end
    }
    //#NT#2009/10/16#Jarkko Chang -begin

    //BP
    if(job_req->SIE_PRE_PRV->PreIntEn & PRE_INTE_BP1)
        job_req->SIE_PRE_PRV->PreBp1 = job_req->SIE_PRE_PRV->PreCropSizeV * 68 / 100;//BP for CA

}

void pre_CapParameter(struct  PUBLIC_STR_JOBREQS *job_req)
{
    UINT16 i;

    if(GetIPPFuncInfor(DFS_EN))
    {
        if(GetIPPFuncInfor(DFS_CNT) == DFS_FIRSTPIC)
        {
            job_req->SIE_PRE_CAP->PreFuncEn &= (~PRE_DF_SUB_EN);
            job_req->SIE_PRE_CAP->PreFuncEn |= PRE_CG_EN;
        }
        else
        {
            job_req->SIE_PRE_CAP->PreFuncEn |= PRE_DF_SUB_EN;
            job_req->SIE_PRE_CAP->PreFuncEn &= (~PRE_BLM_EN);
            job_req->SIE_PRE_CAP->PreFuncEn |= PRE_CG_EN;
        }
    }

    //CG and Ofs
    if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_CG_EN)
    {
        job_req->pre_cg_ofs.OfsR = 0;
        job_req->pre_cg_ofs.OfsGr = 0;
        job_req->pre_cg_ofs.OfsGb = 0;
        job_req->pre_cg_ofs.OfsB = 0;
        //#NT#2009/08/31#Jarkko Chang -begin
        //#NT#modify for pre ca process
        if(Flash_Param.FlshEna>=Flash_Enable)
        {
            job_req->pre_cg_ofs.GainR = Flash_Rg;
            job_req->pre_cg_ofs.GainB = Flash_Bg;
            debug_msg("Cap PRE(Flash Flash gain): Gain = %d %d\r\n",job_req->pre_cg_ofs.GainR,job_req->pre_cg_ofs.GainB);
        }
        else
        {
            job_req->pre_cg_ofs.GainR = Rgain;
            job_req->pre_cg_ofs.GainB = Bgain;
            debug_msg("Cap PRE: Gain = %d %d %d\r\n",job_req->pre_cg_ofs.GainR,Ggain,job_req->pre_cg_ofs.GainB);
        }
        job_req->pre_cg_ofs.GainGr = Ggain;
        job_req->pre_cg_ofs.GainGb = Ggain;
        job_req->pre_cg_ofs.GainSel = 0;
        //#NT#2009/08/31#Jarkko Chang -end
    }

    //gain ctrl
    if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_GAINCTRL_EN)
    {
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
    if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_VIG_EN)
    {
        //#NT#2009/08/04#Jarkko Chang -begin
        //#NT#20091102#ethanlau -begin
        job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R;
        job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_R;
        job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_R;

        job_req->pre_vig_setting.VIG_XR = 1274;
        job_req->pre_vig_setting.VIG_YR = 998;
        job_req->pre_vig_setting.VIG_XGR = 1274;
        job_req->pre_vig_setting.VIG_YGR = 998;
        job_req->pre_vig_setting.VIG_XGB = 1274;
        job_req->pre_vig_setting.VIG_YGB = 998;
        job_req->pre_vig_setting.VIG_XB = 1274;
        job_req->pre_vig_setting.VIG_YB = 998;
        job_req->pre_vig_setting.VIG_MUL = 87;
        job_req->pre_vig_setting.VIG_T = 0;
        job_req->pre_vig_setting.VIG_XDIV = 12;
        job_req->pre_vig_setting.VIG_YDIV = 12;
        //#NT#20091102#ethanlau -end
        //#NT#2009/08/04#Jarkko Chang -end
        job_req->pre_vigline_setting.VIG_R2_LUT = Shading_LineTable;
        job_req->pre_vigline_setting.VIG_G2_LUT = Shading_LineTable;
        job_req->pre_vigline_setting.VIG_B2_LUT = Shading_LineTable;
    }

    //GEO
    if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_GEO_EN)
    {
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
    if(job_req->SIE_PRE_CAP->PreFuncEn & PRE_DF_SUB_EN)
    {
        job_req->pre_dfs_setting.DFS_OP = AMinusB;
        job_req->pre_dfs_setting.DFS_TH = 0xfff;//16;//DP threshold
        job_req->pre_dfs_setting.DFS_THSRC = SrcSie;
    }
}

void pre_D2DParameter(struct  PUBLIC_STR_JOBREQS *job_req)
{
    UINT16 i;

    //CG and Ofs
    if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_CG_EN)
    {
        job_req->pre_cg_ofs.OfsR = 0;
        job_req->pre_cg_ofs.OfsGr = 0;
        job_req->pre_cg_ofs.OfsGb = 0;
        job_req->pre_cg_ofs.OfsB = 0;
        //#NT#20091008#ethanlau -begin
        //For antishaking (D2D)
#if 0
        job_req->pre_cg_ofs.GainR = 256;
        job_req->pre_cg_ofs.GainB = 256;
#else
        if(Flash_Param.FlshEna>=Flash_Enable)
        {
            job_req->pre_cg_ofs.GainR =350;//256;// Flash_Rg;
            job_req->pre_cg_ofs.GainB =333;//256;// Flash_Bg;
            debug_msg("PRE_D2D(Flash): Gain = %d %d\r\n",job_req->pre_cg_ofs.GainR,job_req->pre_cg_ofs.GainB);
        }
        else
        {
            job_req->pre_cg_ofs.GainR = CapRgain;
            job_req->pre_cg_ofs.GainB = CapBgain;
            debug_msg("PRE_D2D: Gain = %d %d\r\n",job_req->pre_cg_ofs.GainR,job_req->pre_cg_ofs.GainB);
        }
#endif
        //#NT#20091008#ethanlau -end
        job_req->pre_cg_ofs.GainGr = 256;
        job_req->pre_cg_ofs.GainGb = 256;
        job_req->pre_cg_ofs.GainSel = 0;
    }

    //gain ctrl
    if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_GAINCTRL_EN)
    {
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
    if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_VIG_EN)
    {
        job_req->pre_vig_setting.VIG_R_LUT = Shading_Table_R;
        job_req->pre_vig_setting.VIG_G_LUT = Shading_Table_G;
        job_req->pre_vig_setting.VIG_B_LUT = Shading_Table_B;
    }
    if(job_req->SIE_PRE_PRV->PreFuncEn & PRE_GAMMA_EN)
    {
        job_req->pre_gamma_r = GammaLUT_128Tab;
        job_req->pre_gamma_g = GammaLUT_128Tab;
        job_req->pre_gamma_b = GammaLUT_128Tab;
    }
    //GEO
    if(job_req->SIE_PRE_D2D->PreFuncEn & PRE_GEO_EN)
    {
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
    //#NT#2010/09/20#ethanlau -begin
    SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER);
    SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER);
    //#NT#2010/09/17#ethanlau -begin
    //#NT# chg mclk
    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SEN_MODE_1280_480);
    //#NT#2010/09/17#ethanlau -end
    Sensor_SetMode(SEN_MODE_1280_480);
    SIE_PRE_Para_prv.ModeSel = Get_SensorMode();
    SIE_PRE_Para_prv.VGAType = SENSOR_PRV_VGATYPE;
    SIE_PRE_Para_prv.SieBp1 = PRV_SIE_BP1;
    SIE_PRE_Para_prv.SieBp2 = PRV_SIE_BP2;
    SIE_PRE_Para_prv.PreIntEn = PRE_INTE_FLDEND|PRE_INTE_BP1;
    SIE_PRE_Para_prv.SieIntEn = SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2|SIE_INTE_OVFL1;
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;//OUT_PINGPONGBUF_DEST;//OUT_PRE_DEST;
    SIE_PRE_Para_prv.PreSrcSel = PATH_POST_GAMMA;
    //SIE_PRE_Para_prv.PreFuncEn = DISABLE;
    SIE_PRE_Para_prv.PreFuncEn = PRE_VIG_EN|PRE_GAMMA_EN|PRE_CA_EN|PRE_CG_EN;
    SIE_PRE_Para_prv.PreCfaPat = CFAPAT_PRV;
    SIE_PRE_Para_prv.PreCfaPat2 = CFAPAT_PRV;
    SIE_PRE_Para_prv.SieVDInv = Notinvert;
    SIE_PRE_Para_prv.SieHDInv = Invert;
    SetSensorMode(SIE_PRE_Para_prv.ModeSel);
    SenFlowFunc(AE_CAP2PRV_SETTING, &gSenCBInfo);
    sie_Set2Preview(&SIE_PRE_Para_prv);
    //#NT#2010/09/20#ethanlau -end
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
//#NT#2009/06/25#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
    ime_to_ipe_Stripe_trans(&IPEModeInfo.SIZEINFO.HSTRIPE,&IPEModeInfo.SIZEINFO.VSTRIPE);
    IPEModeInfo.SIZEINFO.StripeMode = IPE_MSTP;
//  IPEModeInfo.SIZEINFO.StripeMode = IPE_SSTP;
//#NT#2009/06/25#Jarkko Chang -end
    IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX9;
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
    //pinmux_enable_mes(PINMUX_MES0|PINMUX_MES1);
    pinmux_select_flctr(PINMUX_FLCTR);
    pinmux_select_mclk(PINMUX_MCLK);

    switch(Type)
    {
        case SENSOR_OUT_TWO_LINE:
        case SENSOR_OUT_THREE_LINE:
            //pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_480);
            //pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(0x13));
            pll_setClockFreq(SIEMCLK_FREQ, 24000000);
            pll_setClockRate(PLL_CLKSEL_IPE, PLL_CLKSEL_IPE_80);
            pll_setClockRate(PLL_CLKSEL_IME, PLL_CLKSEL_IME_120);
            break;

        default:
        case SENSOR_OUT_PROGRESSIVE:
            debug_err(("^Rsen mode = %d\r\n", sen_mode));
            //pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_480);

            if(sen_mode == SEN_MODE_1280_480)
                //pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(0x18));
                pll_setClockFreq(SIEMCLK_FREQ, 20000000);
                //pll_setClockFreq(SIEMCLK_FREQ, 19200000);
#if 0
            else if(sen_mode == SEN_MODE_1280_960)
                pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV, PLL_SIE_MCLKDIV(0x1f));
#endif
            else if(sen_mode == SEN_MODE_2560_960)
                pll_setClockFreq(SIEMCLK_FREQ, 20000000);
            else if(sen_mode == SEN_MODE_2560_720)

                pll_setClockFreq(SIEMCLK_FREQ, 24000000);
            else if(sen_mode == SEN_MODE_2560_1080)
                pll_setClockFreq(SIEMCLK_FREQ, 20000000);
            else
                pll_setClockFreq(SIEMCLK_FREQ, 24000000);
            pll_setClockRate(PLL_CLKSEL_IPE, PLL_CLKSEL_IPE_60);
            pll_setClockRate(PLL_CLKSEL_IME, PLL_CLKSEL_IME_80);
            break;
    }
    pll_enableClock(SIE_MCLK);
    pll_enableClock(SIE_PXCLK);
    pll_enableClock(IPE_CLK);
    pll_enableClock(IME_CLK);
    pll_enableClock(PRE_CLK);
    pll_setClockRate(PLL_CLKSEL_PRE_CLKSRC, PLL_CLKSEL_PRE_CLKSRC_PXCLK);
    return E_OK;
}
//#NT#20090912#ethanlau -end
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
#if 0
    shutter_move(open);
#endif
}


void Write_AE_EXIF_Infor(UINT paddress)
{
    UH Aperture;
    H  Shutterspeed;
    UH expotime ;
    H  tmp;
    Aperture=30;
    Shutterspeed=50;

    JPEGWriteAPP1ShutterSpeed(paddress, Shutterspeed,  10);//ExpTime=1/(2^(Shutterspeed/10))s
    JPEGWriteAPP1FNumber(paddress, 28, 10);//Fno=2.8
    JPEGWriteAPP1Aperture(paddress,Aperture, 10);//Fno=2^((Aperture/10)/2)
    JPEGWriteAPP1MaxAperture(paddress, Aperture, 10);
    //#NT#2010/12/15#JJ Huang -begin
    //#NT#For SNR Measure, remain the accurate ISO value
    if(SenSNR_IsSNRMeasureEnable())
    {
        JPEGWriteAPP1ISOSpeed(paddress, AE_Param.ISO_Gain_Cap);
        expotime = AE_Param.ExpoTime_Cap;
    }
    else
    {
        switch(gImageAlgInfo.ISO)
        {
                case _IMGISO_50:
                    JPEGWriteAPP1ISOSpeed(paddress, 50);
                    expotime = AE_Param.ISO_Gain_Cap*AE_Param.ExpoTime_Cap/50;
                    break;
                case _IMGISO_100:
                    JPEGWriteAPP1ISOSpeed(paddress, 100);
                    expotime = AE_Param.ISO_Gain_Cap*AE_Param.ExpoTime_Cap/100;
                    break;
                case _IMGISO_200:
                    JPEGWriteAPP1ISOSpeed(paddress, 200);
                    expotime = AE_Param.ISO_Gain_Cap*AE_Param.ExpoTime_Cap/200;
                    break;
                case _IMGISO_400:
                    JPEGWriteAPP1ISOSpeed(paddress, 400);
                    expotime = AE_Param.ISO_Gain_Cap*AE_Param.ExpoTime_Cap/400;
                    break;
                case _IMGISO_Auto:
                case _IMGISO_800:
                case _IMGISO_1600:
            default:
                JPEGWriteAPP1ISOSpeed(paddress, AE_Param.ISO_Gain_Cap);
                expotime = AE_Param.ExpoTime_Cap;
                break;
        }
    }
    //#NT#2010/12/15#JJ Huang -end
    //#NT#2010/12/14#JJ Huang -begin
    //#NT#EXIF BUG
    #if 1
    {
        if((expotime)<=100000)
            JPEGWriteAPP1ExposureTime(paddress, 1,  1000000 /(expotime));
        else
            JPEGWriteAPP1ExposureTime(paddress, (expotime/100000),  10);
    }
    #else
    JPEGWriteAPP1ExposureTime(paddress, 1,  expotime / 1000/*32*/);//ExpTime=1/32s
    #endif
    //#NT#2010/12/14#JJ Huang -end
    if(gImageAlgInfo.DriveMode != _IMGDriveMode_AEBreaketing)
    {
        tmp = (H)(gImageAlgInfo.AEEVValue - _IMGEVValue_00);
        JPEGWriteAPP1ExposureBias(paddress,tmp,3);
    }
    else
    {
        JPEGWriteAPP1ExposureBias(paddress, (gImageAlgInfo.AEEVValue-6)*2-3+AEB_Idx*3, 6);//step 0.3
        AEB_Idx++;
    }

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
    //#NT#20091127#ethanlau -begin
    IPE_IQINFO IQInfo;
    STR_IPE_EDGE_ENHMAP_PARA EdgInfo;

    EdgInfo = *IPEModeInfo.IQINFO.EDGE_PARAM;
    IQInfo.EDGE_PARAM = &EdgInfo;

    IQInfo.FuncUpdateSel = IPE_FUNC_NOUPDATE;
    IQInfo.ParamUpdateSel = IPE_SET_EDGE;

    IQInfo.EDGE_PARAM->EdgEnhance = Sharpness[GetISOIdx(AE_Param.ISO_Gain)][Index];
    //#NT#20091127#ethanlau -end
    IQInfo.EdgeKerH = NULL;
    IQInfo.EdgeKerV = NULL;
    IQInfo.Emap = NULL;
    IQInfo.Estab = NULL;

    ipe_changeParam(&IQInfo);
}

void SetSaturation(unsigned char Index)
{
    //#NT#20091127#ethanlau -begin
    IPE_IQINFO IQInfo;
    IPE_COLOR_EFFECT_PARAM ColorEffectInfo;

    ColorEffectInfo = *IPEModeInfo.IQINFO.COLOR_EFFECT_PARAM;
    IQInfo.COLOR_EFFECT_PARAM = &ColorEffectInfo;

    IQInfo.FuncUpdateSel = IPE_FUNC_NOUPDATE;
    IQInfo.ParamUpdateSel = IPE_SET_COLOREFFECT;

    IQInfo.COLOR_EFFECT_PARAM->SatOfs = Saturation[Index];
    //#NT#20091127#ethanlau -end
    IQInfo.HueTab = NULL;
    IQInfo.SatTab = NULL;
    IQInfo.IntTab = NULL;
    IQInfo.DDSTab = NULL;

    ipe_changeParam(&IQInfo);
}
//#NT#20090711#ETHAN LAU -BEGIN
unsigned int WriteDebugData(unsigned char *Add)
{
    UH i, j;//,j,x,y;
    unsigned char *buff;
    char endl[2]={0x0D,0x0A};
    char endqty[2]={0xCC,0xBB};
    unsigned long tmpHist[64];
    UINT8* _ae_window;
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();

    buff = Add;

    //insert Qty tag
    buff += sprintf((char *)buff,"%s",endqty);

    buff += sprintf((char *)buff,"%s",endl);

    aaa_Read_HIST_RSLT((UW*)tmpHist);
    buff += sprintf((char *)buff,"%s\t\t  1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t11\t12%s", endl, endl);
    *buff++= '\t';
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
#if 1
    buff += sprintf((char *)buff,"(OX)\t: %c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c\t%c%s",
                    CalInfo->CalStatus_Mshutter==_Mshutter_Status ? 'O':'X',     // g1
                    CalInfo->CalStatus_ISO==_ISO_Status ? 'O':'X',               // g2
                    CalInfo->CalStatus_AWB==_AWB_Status ? 'O':'X',               // g3
                    CalInfo->CalStatus_FocusFar==_FF_Status ? 'O':'X',             // g4
                    CalInfo->CalStatus_FocusNear==_NF_Status ? 'O':'X',             // g5
                    CalInfo->CalStatus_PrvDP_W==_Prv_W_Status ? 'O':'X',    // g6
                    CalInfo->CalStatus_PrvDP_B==_Prv_B_Status ? 'O':'X',    // g7
                    CalInfo->CalStatus_CapDP_W==_Cap_W_Status ? 'O':'X',    // g8
                    CalInfo->CalStatus_CapDP_B==_Cap_B_Status ? 'O':'X',    // g9
                    CalInfo->CalStatus_FlashFire==_Flash_Status ? 'O':'X',           // g10
                    CalInfo->CalStatus_OB==_OB_Status ? 'O':'X',                 // g11
                    CalInfo->CalStatus_LensShading==_Shading_Status ? 'O':'X',
                    endl
                    );
    *buff++= '\t';
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"DPC Number (Prv) = %5d%sDPC Number (Cap) = %ld%sDPC Status (PrvW) = %X%sDPC Status (PrvB) = %X%sDPC Status (CapW) = %X%sDPC Status (CapB) = %X%s",
                    CalInfo->Cal_DP_PreviewNum,endl,
                    CalInfo->Cal_DP_CaptureNum,endl,
                    CalInfo->CalStatus_PrvDP_W,endl,
                    CalInfo->CalStatus_PrvDP_B,endl,
                    CalInfo->CalStatus_CapDP_W,endl,
                    CalInfo->CalStatus_CapDP_B,endl
                    );

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
     buff += sprintf((char *)buff,"data\t: %d\t%d\t%d\t%ld\t%d\t%d\t%d\t%d\t%d\t%d%s",
                    0,//CalInfo->Cal_Mshutterr_Data[0],                                // g1
                    CalInfo->Cal_ISO_Data[0],                                // g2
                    CalInfo->Cal_AWBStatistic.Rgain,                                // g3
                    CalInfo->Cal_AWBStatistic.Bgain,                                // g4
                    0,                                                     // g5
                    0,                                                     // g6
                    0,                                                     // g7
                    0,                                                     // g8
                    0,//CalInfo->Cal_Flash,                                // g9
                    0,//CalInfo->Cal_OB,                                  // g10
                    endl
                    );
#endif
/*
     buff += sprintf((char *)buff,"data1\t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%s",
                    AAA_Adjust.Far_Focus[0],
                    AAA_Adjust.Far_Focus[1],
                    AAA_Adjust.Far_Focus[2],
                    AAA_Adjust.Far_Focus[3],
                    AAA_Adjust.Far_Focus[4],
                    AAA_Adjust.Far_Focus[5],
                    AAA_Adjust.Far_Focus[6],
                    AAA_Adjust.Far_Focus[7],
                    AAA_Adjust.Far_Focus[8],
                    AAA_Adjust.Far_Focus[9],
                    AAA_Adjust.Far_Focus[10],
                    endl
                    );
     buff += sprintf((char *)buff,"data2\t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%s",
                    AAA_Adjust.Near_Focus[0],
                    AAA_Adjust.Near_Focus[1],
                    AAA_Adjust.Near_Focus[2],
                    AAA_Adjust.Near_Focus[3],
                    AAA_Adjust.Near_Focus[4],
                    AAA_Adjust.Near_Focus[5],
                    AAA_Adjust.Near_Focus[6],
                    AAA_Adjust.Near_Focus[7],
                    AAA_Adjust.Near_Focus[8],
                    AAA_Adjust.Near_Focus[9],
                    AAA_Adjust.Near_Focus[10],
                    endl
                    );
*/

     buff += sprintf((char *)buff,"ExpectY=%d%sYAverage=%ld%sEV_Value=%ld%sISO_Gain=%d,%d%sPgaGain=%d%sExpoTime=%ld,%ld%sExpoLine=%ld,%ld%sStableCnt=%ld%sExpoFrame=%d%sAddY=%d%sFlash_ISO=%d%s",
                    AE_Debug.AE_ExpectY,endl,
                    YAverage,endl,
                    AE_Param.EV_Value,endl,
                    AE_Param.ISO_Gain,AE_Param.ISO_Gain_Cap,endl,
                    AE_Param.AePgaGain_fra,endl,
                    AE_Param.ExpoTime,AE_Param.ExpoTime_Cap,endl,
                    AE_Param.ExpoLine,AE_Param.ExpoLine_Cap,endl,
                    AE_Param.StableCnt,endl,
                    AE_Param.ExpoFrame,endl,
                    AE_Debug.AE_AddY,endl,
                    AE_Debug.AE_FlashISO,endl
                    );

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

     buff += sprintf((char *)buff,"b:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%\t%d%\t%d%s",
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
                    gImageAlgInfo.Frequency,
                    gAlgMsg.Alg_Mode,//_Alg_Callback_Mode
                    endl
                    );
                    //Rgain

    for(  i = 0; i < 80; i++ )
    *buff++ = '-' ;

    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"ColorGain = {%3d, %3d, %3d} Cap:{%3d, %3d, %3d}%sFlash gain = {%3d,%3d}%sLV, Yavg  = {%3d, %3d}%s",
                    Rgain,Ggain,Bgain,CapRgain,CapGgain,CapBgain,endl,
                    Flash_Rg,Flash_Bg,endl,
                    AE__GetLV(),YAverage,endl
                    );

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;

    buff += sprintf((char *)buff,"%s", endl);
#if 0
    buff += sprintf((char *)buff,"FlashGain = {%3d, %3d, %3d}%sPre-Flash ImeOut = {%3d, %3d, %3d}%s No-Flash Y = %3d%sPre-Flash Y = %3d%sMainAGC = %3d%s",
                    Flash_Rg,Flash_Gg,Flash_Bg,endl,
                    YY[0],YY[1],YY[2],endl,
                    YNoFlash,endl,
                    YPreFlash, endl,
                    Flash_Param.MainAGC, endl
                    );
#endif
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

     buff += sprintf((char *)buff,"e:\t%d\t%d\t%d\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t\t",
                    Flash_Param.FlshEna,                                // d1
                    Flash_Param.RedeyeEn,                                // d2
                    Flash_Param.ExpChg,                                    // d3
                    Flash_Param.PreAGC,                                    // d4
                    Flash_Param.PreWidth,                                // d5
                    Flash_Param.MainAGC,                                // d6
                    Flash_Param.MainWidth,                                // d7
                    Flash_Param.DelayLines,                                // d8
                    Flash_Param.Delay2Pix,                                // d9
                    Flash_Param.FTP,                                    // d10
                    Flash_Param.ChargeStatus,                                    // d11
                    //YNoFlash,                                            // d12
                    //YPreFlash,                                            // d13
                    //YMainFlash,//JJ 20050814
                    //Y_Flash_Gain,
                    //Y_Flash_Dram,
                    endl
                    );
#if 0
     buff += sprintf((char *)buff,"O  \t: %lX\t%lX\t%lX\t%lX\t%lX\t%s",
                    AAA_Adjust.Shading_Gain[0],
                    AAA_Adjust.Shading_Gain[1],
                    AAA_Adjust.Shading_Gain[2],
                    AAA_Adjust.Shading_Gain[3],
                    AAA_Adjust.Shading_Gain[4],
                    endl
                    );
#endif
/*
      for(i=0;i<8;i++)
      {
      buff += sprintf((char *)buff,"O  \t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%s",
                    Flash_Detect[0+i*8],
                    Flash_Detect[1+i*8],
                    Flash_Detect[2+i*8],
                    Flash_Detect[3+i*8],
                    Flash_Detect[4+i*8],
                    Flash_Detect[5+i*8],
                    Flash_Detect[6+i*8],
                    Flash_Detect[7+i*8],
                    endl
                    );
       }
*/

/*
     buff += sprintf((char *)buff,"d  \t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%s",
                     uiSensorFlags&(SENSOR_FLAG_FLASHLIGHT),                // d1
                    Flash_Expo_Chg_En,                                    // d2
                    Red_Eye_Reduction,                                    // d3
                    F_TargetY,                                            // d4
                       F_YAverage,                                            // d5
                    Yavg_flash,                                            // d6
                    EV,                                                    // d7
                    flash_width,                                        // d8
                    flash_AGC,                                            // d9
                    FLASH_EV_TH,                                        // d10
                       CAL_AvgY,                                            // d11
                       Default_Flash_Y_Avg,                                // d12
                       Flash_offsetD,                                        // d13
                    endl
                    );
*/

    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"F:\t");
#if 0
    for(i=0;i<=AF_Debug.AF_SearchStep;i++)
        buff += sprintf((char *)buff,"%d\t",AF_Param[4][i].AF_Value);
    buff += sprintf((char *)buff,"%s", endl);
    buff += sprintf((char *)buff,"g  \t: ");
    for(i=0;i<5;i++)
        for(j=0;j<2;j++)
            buff += sprintf((char *)buff,"%d\t",Cap_OB[i][j]);
    buff += sprintf((char *)buff,"%s", endl);
#endif

    _ae_window = aaa_GetAeWindow();
    buff += sprintf((char *)buff,"AE_WINDOW: %s", endl, endl);
    for(j=0;j<8;j++)
    {
        for(i=0;i<8;i++)
        {
            buff += sprintf((char *)buff,"\t%3d",_ae_window[j*8+i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }


    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

    for(i=0;i<8;i++)
    {
        buff += sprintf((char *)buff,"W  \t: %3d\t%3d\t%3d\t%3d\t%3d\t%3d\t%3d\t%3d%s",
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

    //if(Flash_Param.FlshEna)
    if(0)//((Flash_Param.FlshEna)&&(AE_Param.EV_Value < Flash_Param.FTP))
    {
#if 0
        for(i=0;i<8;i++)
        {
            buff += sprintf((char *)buff,"FRGB\t: %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d%s",
                    FR_acc[(0+i*8)],FG_acc[(0+i*8)],FB_acc[(0+i*8)],
                    FR_acc[(1+i*8)],FG_acc[(1+i*8)],FB_acc[(1+i*8)],
                    FR_acc[(2+i*8)],FG_acc[(2+i*8)],FB_acc[(2+i*8)],
                    FR_acc[(3+i*8)],FG_acc[(3+i*8)],FB_acc[(3+i*8)],
                    FR_acc[(4+i*8)],FG_acc[(4+i*8)],FB_acc[(4+i*8)],
                    FR_acc[(5+i*8)],FG_acc[(5+i*8)],FB_acc[(5+i*8)],
                    FR_acc[(6+i*8)],FG_acc[(6+i*8)],FB_acc[(6+i*8)],
                    FR_acc[(7+i*8)],FG_acc[(7+i*8)],FB_acc[(7+i*8)],
                    endl
                    );
        }
#endif
    }
    else
    {
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
#if 0
        buff += sprintf((char *)buff,"\t: ----------------------------------------------------------------%s",endl);
        for(i=0;i<8;i++)
        {
            buff += sprintf((char *)buff,"CAL_RGB\t: %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d%s",
                    R_acc1[(0+i*8)],G_acc1[(0+i*8)],B_acc1[(0+i*8)],
                    R_acc1[(1+i*8)],G_acc1[(1+i*8)],B_acc1[(1+i*8)],
                    R_acc1[(2+i*8)],G_acc1[(2+i*8)],B_acc1[(2+i*8)],
                    R_acc1[(3+i*8)],G_acc1[(3+i*8)],B_acc1[(3+i*8)],
                    R_acc1[(4+i*8)],G_acc1[(4+i*8)],B_acc1[(4+i*8)],
                    R_acc1[(5+i*8)],G_acc1[(5+i*8)],B_acc1[(5+i*8)],
                    R_acc1[(6+i*8)],G_acc1[(6+i*8)],B_acc1[(6+i*8)],
                    R_acc1[(7+i*8)],G_acc1[(7+i*8)],B_acc1[(7+i*8)],
                    endl
                    );
        }
#endif
        for(  i = 0; i < 80; i++ )
            *buff++ = '-' ;
        buff += sprintf((char *)buff,"%s", endl);

        for(i=0;i<8;i++)
        {
            buff += sprintf((char *)buff,"W  \t: %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d%s",
                    gCapAWBWeight[0+i*8],
                    gCapAWBWeight[1+i*8],
                    gCapAWBWeight[2+i*8],
                    gCapAWBWeight[3+i*8],
                    gCapAWBWeight[4+i*8],
                    gCapAWBWeight[5+i*8],
                    gCapAWBWeight[6+i*8],
                    gCapAWBWeight[7+i*8],
                    endl
                    );
        }
        for(  i = 0; i < 80; i++ )
            *buff++ = '-' ;
        buff += sprintf((char *)buff,"%s", endl);

        for(i=0;i<8;i++)
        {
            buff += sprintf((char *)buff,"CapRGB\t: %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d\t %3d %3d %3d%s",
                    CapR_acc[(0+i*8)],CapG_acc[(0+i*8)],CapB_acc[(0+i*8)],
                    CapR_acc[(1+i*8)],CapG_acc[(1+i*8)],CapB_acc[(1+i*8)],
                    CapR_acc[(2+i*8)],CapG_acc[(2+i*8)],CapB_acc[(2+i*8)],
                    CapR_acc[(3+i*8)],CapG_acc[(3+i*8)],CapB_acc[(3+i*8)],
                    CapR_acc[(4+i*8)],CapG_acc[(4+i*8)],CapB_acc[(4+i*8)],
                    CapR_acc[(5+i*8)],CapG_acc[(5+i*8)],CapB_acc[(5+i*8)],
                    CapR_acc[(6+i*8)],CapG_acc[(6+i*8)],CapB_acc[(6+i*8)],
                    CapR_acc[(7+i*8)],CapG_acc[(7+i*8)],CapB_acc[(7+i*8)],
                    endl
                    );
        }
    }
/*
    buff += sprintf((char *)buff,"%s\n", endl);
    for(i=0;i<8;i++){
        buff += sprintf((char *)buff,"Hist\t: %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t\n",
                tmpHist[(0+i*8)],
                tmpHist[(1+i*8)],
                tmpHist[(2+i*8)],
                tmpHist[(3+i*8)],
                tmpHist[(4+i*8)],
                tmpHist[(5+i*8)],
                tmpHist[(6+i*8)],
                tmpHist[(7+i*8)]
                );
    }
*/
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);
#if 0
    //if(Flash_Param.FlshEna)
    if((Flash_Param.FlshEna)&&(AE_Param.EV_Value < Flash_Param.FTP))
    {
        for(i=0;i<8;i++){
            buff += sprintf((char *)buff,"FAWB\t: %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t\n",
                    FAWB_Weight[(0+i*8)],
                    FAWB_Weight[(1+i*8)],
                    FAWB_Weight[(2+i*8)],
                    FAWB_Weight[(3+i*8)],
                    FAWB_Weight[(4+i*8)],
                    FAWB_Weight[(5+i*8)],
                    FAWB_Weight[(6+i*8)],
                    FAWB_Weight[(7+i*8)]
                    );
        }
    }
#endif
    for(  i = 0; i < 80; i++ )
        *buff++ = '-' ;
    buff += sprintf((char *)buff,"%s", endl);

#if 0
    for(i=0;i<4;i++){
        buff += sprintf((char *)buff,"Gamma2\t: %d\t %d\t %d\t %d\t %d\t\n",
                AutoGamma[(0+i*5)],
                AutoGamma[(1+i*5)],
                AutoGamma[(2+i*5)],
                AutoGamma[(3+i*5)],
                AutoGamma[(4+i*5)]
                );
    }
#endif
    //#NT#2010/12/15#JJ Huang -begin
    //#NT#For SNR Measure
    if(SenSNR_IsSNRMeasureEnable())
    {
         buff += sprintf((char *)buff,"\tISO\t%d%s", AE_Param.ISO_Gain_Cap, endl );
         buff += sprintf((char *)buff,"\tUniformity\t%6d\t%6d\t%6d\t%6d\t%6d%s",
                        SenSNR_GetSNRData(SenSNR_DATA_UNIFORMITY_C0),
                        SenSNR_GetSNRData(SenSNR_DATA_UNIFORMITY_C1),
                        SenSNR_GetSNRData(SenSNR_DATA_UNIFORMITY_C2),
                        SenSNR_GetSNRData(SenSNR_DATA_UNIFORMITY_C3),
                        SenSNR_GetSNRData(SenSNR_DATA_UNIFORMITY_C4),
                        endl
                        );
         buff += sprintf((char *)buff,"\tMean\t%6d\t%6d\t%6d\t%6d%s",
                        SenSNR_GetSNRData(SenSNR_DATA_MEAN_CH0),
                        SenSNR_GetSNRData(SenSNR_DATA_MEAN_CH1),
                        SenSNR_GetSNRData(SenSNR_DATA_MEAN_CH2),
                        SenSNR_GetSNRData(SenSNR_DATA_MEAN_CH3),
                        endl
                        );
         buff += sprintf((char *)buff,"\tSNR\t%6d\t%6d\t%6d\t%6d%s",
                        SenSNR_GetSNRData(SenSNR_DATA_SNR_CH0),
                        SenSNR_GetSNRData(SenSNR_DATA_SNR_CH1),
                        SenSNR_GetSNRData(SenSNR_DATA_SNR_CH2),
                        SenSNR_GetSNRData(SenSNR_DATA_SNR_CH3),
                        endl
                        );
        {// show on Jpeg Debug Msg
            UINT32 uiVal;
            uiVal = SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_SNR);
            buff += sprintf((char *)buff,"\tSNR\t%3d.%03d%s", uiVal/1000, uiVal%1000, endl);
            uiVal = SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_MEAN);
            buff += sprintf((char *)buff,"\tMEAN\t%3d.%03d%s", uiVal/1000, uiVal%1000, endl);
            uiVal = SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_NONUNIFORMITY);
            buff += sprintf((char *)buff,"\tNUFM\t%3d.%03d%s", uiVal/1000, uiVal%1000, endl);
        }
    }
    //#NT#2010/12/15#JJ Huang -end

    return (buff-Add);
}
//#NT#20090711#ETHAN LAU -end
UINT32    Sensor_CheckNeedSize(void)
{
    return 0xc00000;
}
//#NT#20090912#ethanlua -begin
//add mode
void SetSensorMode(INT32 Mode)
{
    UINT tmp;
    //#NT#2010/09/09#ethanlau -begin
    if(Mode != STILL_MODE)
    {
        BiningSetting(Mode, &tmp);
        SetIPPSizeInfor(_Binning, tmp);
    }
    //#NT#2010/09/09#ethanlau -end
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

        case MONITOR_MODE_3:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_3);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_3);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_3);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_3);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_3);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_3);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_3);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_3);

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

        case MONITOR_MODE_4:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_4);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_4);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_4);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_4);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_4);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_4);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_4);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_4);

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
        case MONITOR_MODE_5:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_5);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_5);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_5);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_5);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_5);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_5);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_5);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_5);

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
        case MONITOR_MODE_6:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_6);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_6);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_6);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_6);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_6);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_6);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_6);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_6);

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
        case MONITOR_MODE_7:
            SetIPPSizeInfor(_SieXcenter, SIE_PRV_XCENTER_7);
            SetIPPSizeInfor(_SieYcenter, SIE_PRV_YCENTER_7);
            SetIPPSizeInfor(_HDTotal_Preview, HD_Total_Prv_7);
            SetIPPSizeInfor(_HDSync_Preview, HD_Sync_Prv_7);
            SetIPPSizeInfor(_HDActX_Preview, ACT_STRX_Prv_7);
            SetIPPSizeInfor(_HDActY_Preview, ACT_STRY_Prv_7);
            SetIPPSizeInfor(_VDTotal_Preview, VD_Total_Prv_7);
            SetIPPSizeInfor(_VDSync_Preview, VD_Sync_Prv_7);

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
            break;
    }
}
//#NT#20090912#ethanlau -end
void DFS_OnOffSwitch(void)
{
    if(0)
        SetIPPFuncInfor(DFS_EN,ON);
    else
        SetIPPFuncInfor(DFS_EN,OFF);
}


void IPPFunc_OnOffSwitch(void)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
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
    if(pimginfo->DriveMode == _IMGDriveMode_Burst)
        SetIPPFuncInfor(CHRCURVE_EN,OFF);
    else
    {
        SetIPPFuncInfor(CHRCURVE_EN,ON);
    }
    SetIPPFuncInfor(CHRCURVE_EN,OFF);
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

//#NT#2010/05/28#ethanlau -begin
//#NT#ime engine chroma filter info
IME_FILT_PARA* Get_ImeFilterInfo(void)
{
    return (IME_FILT_PARA*)IMECHROMAPARAM;
}
//#NT#2010/05/28#ethanlau -end

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


void AAFD_LockSwitch(UINT32 ImgMode)
{
    switch(ImgMode)
    {
        case IPPImg_Preview:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            fde_FdLockCap(UNLOCK);
            break;
        case IPPImg_Video:
        case IPPImg_PCC:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            aaa_FdLock(LOCK);
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            break;
        case IPPImg_AF:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            aaa_FdLock(LOCK);
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            break;

        case IPPImg_BeforCap:
            aaa_AwbLock(LOCK);
            aaa_AeLock(LOCK);
            aaa_FdLock(LOCK);
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            fde_FdLockCap(LOCK);
            break;

        case IPPImg_DzoomEnter:
            aaa_AwbLock(LOCK);
            aaa_AeLock(LOCK);
            aaa_FdLock(LOCK);
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            break;
        case IPPImg_DzoomLeave:
            aaa_AwbLock(UNLOCK);
            aaa_AeLock(UNLOCK);
            break;
//#NT#20090912#ethanlau -begin
        case IPPImg_SenModeChg:
            aaa_AwbLock(LOCK);
            aaa_AeLock(LOCK);
            aaa_FdLock(LOCK);
            //#NT#2010/10/18#JJ -begin
            aaa_SdLock(LOCK);
            //#NT#2010/10/18#JJ -end
            fde_FdLockCap(LOCK);
            break;
//#NT#20090912#ethanlau -end
        default:
            debug_err(("-E- ImgMode(%d) > MaxID(%d)\r\n", ImgMode, IPPImg_MaxID));
        break;
    }

}


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
    //#NT#2010/12/15#JJ Huang -begin
    //#NT#For SNR Measure
    if(SenSNR_IsSNRMeasureEnable())
    {
        SiePreInfo->PreSrcSel = PATH_PRE_CLOLRGAIN;
    }
    else
    {
        SiePreInfo->PreSrcSel = PATH_POST_GAMMA;
    }
    //#NT#2010/12/15#JJ Huang -end
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
//    SiePreInfo->PreFuncEn = DISABLE;
    SiePreInfo->PreFuncEn = PRE_VIG_EN|PRE_CG_EN|PRE_GAMMA_EN;
//#NT#2009/08/31#Jarkko Chang -end
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
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PINGPONGBUF_DEST;
    SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
    SIE_PRE_Para_prv.PreFuncEn = DISABLE;
    SIE_PRE_Para_prv.PreCfaPat = CFAPAT_PRV;
    SIE_PRE_Para_prv.PreCfaPat2 = CFAPAT_PRV;
    SIE_PRE_Para_prv.SieVDInv = Notinvert;
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
    sie_waitVD(2);
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



void CalPreviewOnePRE(UINT32 RawAddr, UINT32 bitDepth, UINT32 *RawHsize, UINT32 *RawVsize, UINT32 ObOfs)
{
    UINT32 Hsize, Vsize, LineOft;

    //++  First Get RawHsize & RawVsize begin
    StopPreviewDramData();
    gAlgMsg.Alg_Mode=MODE_Preview;

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
    SIE_PRE_Para_prv.SieMaster = SlaveMode;
    SIE_PRE_Para_prv.SieHDPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.SieVDPhase = SieLoad_FallingEdge;
    SIE_PRE_Para_prv.SieDataPhase = SieLoad_RisingEdge;
    SIE_PRE_Para_prv.AutoObEn = DISABLE;
    SIE_PRE_Para_prv.SieObOfs = ObOfs;
    SIE_PRE_Para_prv.SieOutDest = OUT_DRAM_DEST;
    SIE_PRE_Para_prv.SieOutAdd0= RawAddr;
    gImageAlgInfo.PrePrvBitDepth = bitDepth;
    gImageAlgInfo.SieBitDepth = bitDepth;
    debug_msg("gImageAlgInfo.SieBitDepth= %d \r\n",gImageAlgInfo.SieBitDepth);
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


    IPP_ClockIOInit(SIE_PRE_Para_prv.VGAType,SIE_PRE_Para_prv.ModeSel);
    Set_digitalzoom(10, &SIE_PRE_Para_prv);
    IMEReset();
    IPEReset();
    SIEReset();

    Hsize = GetIPPSizeInfor(_PreCropHsize);
    Vsize = GetIPPSizeInfor(_PreCropVsize);
    SIE_PRE_Para_prv.PreCropSizeH = Hsize;
    SIE_PRE_Para_prv.PreCropSizeV = Vsize;
    SIE_PRE_Para_prv.PreOutSizeH = GetIPPSizeInfor(_PreOutHsize);
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
    SIE_PRE_Para_prv.IsPadding = Padding_Prv;
    SIE_PRE_Para_prv.SieOutDest = OUT_PRE_DEST;
    SIE_PRE_Para_prv.PreSrcSel = PATH_PRE_CLOLRGAIN;
    SIE_PRE_Para_prv.PreFuncEn = DISABLE;

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
