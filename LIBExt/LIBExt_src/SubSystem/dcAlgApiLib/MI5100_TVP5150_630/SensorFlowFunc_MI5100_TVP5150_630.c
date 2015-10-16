#include    "sensor_model.h"
#if MI5100_TVP5150_630
#include "ErrorNo.h"
#include "Pll.h"

SenCBInfo gSenCBInfo = {0, &IPPMgrReq, &SIE_PRE_Para_cap, &SIE_PRE_Para_prv, &IPEModeInfo, IPL_SLEEP_LEVEL_0};

//#NT#2009/12/09#Jarkko Chang -begin
#define ChromaEnableThres NormalExpectY*(100-YConRange)/100
//#NT#2009/12/09#Jarkko Chang -end
#if 0
#define ISO0_THRES  80
#define ISO1_THRES 120
#define ISO2_THRES 170
#else
#define ISO_OUTDOOR_THR 80
#define ISO0_THRES 120
#define ISO1_THRES 240
//#define ISO2_THRES 400
#define ISO2_THRES 360
#endif
//#NT#20091127#ethanlau -begin
IPE_ISONumEnum GetISOIdx(UINT32 ISO)
{
    if(ISO <= ISO0_THRES)
        return ISONUM0;
    else if(ISO <= ISO1_THRES)
        return  ISONUM1;
    else
        return ISONUM2;

    if(IPL_GetMode()== IPL_MODE_CAP)
    {
        if(ISO<=ISO0_THRES)
            return ISONUM4;
        else if(ISO<ISO1_THRES)
            return ISONUM5;
        else
            return ISONUM6;
    }
}
//#NT#20091127#ethanlau -end

//#NT#2009/12/09#Jarkko Chang -begin
ER ime_ChromPram(UINT32 ParamMode)
{
    IME_FILT_PARA ime_filt_para;

    ime_filt_para.crv_center = IMECHROMAPARAM[ISONUM0].crv_center;
    ime_filt_para.crv_delta = IMECHROMAPARAM[ISONUM0].crv_delta;
    ime_filt_para.havg_coeff = IMECHROMAPARAM[ISONUM0].havg_coeff;
    ime_filt_para.havg_size = IMECHROMAPARAM[ISONUM0].havg_size;

    switch(ParamMode)
    {
        case IME_Chrom_Prv_SETTING:

            ime_filt_para.crv_en = IMEALG_ENABLE;
            ime_filt_para.havg_en = IMEALG_ENABLE;
            break;
        case IME_Chrom_Prv_AE_SETTING:
        case IME_Chrom_Cap_SETTING:
            if(YAverage <= ChromaEnableThres)
            {
                ime_filt_para.crv_delta =
                    ( (ChromaEnableThres - YAverage)*ime_filt_para.crv_delta
                     + (ChromaEnableThres-1) )/ChromaEnableThres;
                ime_filt_para.crv_en = IMEALG_ENABLE;
                ime_filt_para.havg_en = IMEALG_ENABLE;
            }
            else
            {
                ime_filt_para.crv_en = IMEALG_DISABLE;
                ime_filt_para.havg_en = IMEALG_DISABLE;
            }
            break;
            /*
        case IME_Chrom_Cap_SETTING:
            ime_filt_para.crv_en = IMEALG_DISABLE;
            ime_filt_para.havg_en = IMEALG_DISABLE;

            //#NT#2009/12/09#Jarkko Chang -begin
            if(AE_Param.ISO_Gain<=ISO0_THRES)
            {
                SetCbCrRadius(0);
            }
            else if(AE_Param.ISO_Gain<=ISO1_THRES)
            {
                SetCbCrRadius(2);
            }
            else
            {
                SetCbCrRadius(4);
            }
            //#NT#2009/12/09#Jarkko Chang -end

            break;
            */
        default:
            debug_err(("ime_ChromPram() param mode selection error!\r\n"));
            return E_SYS;
            break;
    }

    ime_setChromaFilter(&ime_filt_para);
    return E_OK;

}
//#NT#2009/12/09#Jarkko Chang -end

ER ipe_IQparam(UINT32 ParamMode, IPE_IQINFO *IQInfo)
{
    UINT32 ISO_Value = AE_Param.ISO_Gain;
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();

    if(ParamMode == IPE_CAP_SETTING)
    {
        if(AE_Param.ISO_Gain<=ISO0_THRES)
            ISO_Value =ISONUM4;
        else if(AE_Param.ISO_Gain<ISO1_THRES)
            ISO_Value =ISONUM5;
        else if(AE_Param.ISO_Gain<ISO2_THRES)
            ISO_Value =ISONUM6;
        else
            ISO_Value =ISONUM3;

//        ISO_Value =ISONUM4;
    }
    else
    {
        if (AE_Param.ISO_Gain < ISO0_THRES)
            ISO_Value = ISONUM0;
        else if (AE_Param.ISO_Gain < ISO1_THRES)
            ISO_Value = ISONUM1;
        else
            ISO_Value = ISONUM2;
    }
//    debug_err(("iso num = %d\r\n",ISO_Value));
    switch(ParamMode)
    {
//#NT#2009/06/29#Jarkko Chang -begin
//#NT#prepare for 96630 preview using
        case IPE_PRV_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;

//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for using pre gamma/cg
            IQInfo->FuncSel = IPE_RLPF_EN |
                                         IPE_GLPF_EN |
                                         IPE_BLPF_EN |
                                         IPE_CCR_EN |
                                         IPE_CTRL_EN |
                                         IPE_HADJ_EN |
                                         IPE_CADJ_EN |
                                         IPE_CADJ_YENH_EN |
                                         IPE_CADJ_YCON_EN |
                                         IPE_CADJ_CCON_EN |
                                         IPE_CADJ_CLPF_EN |
//                                         IPE_CGAIN_EN |
//                                         IPE_PREGAMMA_EN;
                                         IPE_NR_EN ;

            //#NT#2011/08/25#Spark Chou -begin
            //#NT# DVI Input
            if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
                IQInfo->FuncSel = 0;
            //#NT#2011/08/25#Spark Chou -end

            IQInfo->ParamUpdateSel = IPE_SET_ALL&~IPE_SET_GAMMA;
//            IQInfo->ParamUpdateSel = IPE_SET_ALL;
//#NT#2009/10/16#Jarkko Chang -end

//#NT#2010/06/18#Spark Chou -begin
            if (AE_Param.ISO_Gain < ISO_OUTDOOR_THR)
                IQInfo->FuncSel = IQInfo->FuncSel & (~( IPE_RLPF_EN | IPE_GLPF_EN | IPE_BLPF_EN ));
//#NT#2010/06/18#Spark Chou -end
//#NT#2010/03/09#Jarkko Chang -begin
            /*if (!Photo_IsCAFLock())
            {
               IQInfo->FuncSel |= (IPE_VACC_EN);
            }*/
//#NT#2010/03/09#Jarkko Chang -end

            IQInfo->GammaLut[IPE_R_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_G_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_B_CHANNEL] = GammaLUT_128Tab;

            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#2009/12/02#Jarkko Chang -begin
            IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //#NT#2009/12/02#Jarkko Chang -end
            IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;

            //#NT#2010/06/20#Spark Chou -begin
            //IQInfo->EExtHVopt = HVOPT_MAX;
            //#NT#2010/06/20#Spark Chou -end

            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
            IQInfo->HueTab = NULL;
            IQInfo->SatTab = NULL;
            IQInfo->IntTab = NULL;
            IQInfo->DDSTab = NULL;

            IQInfo->COLOR_PARAM.CGRange=IPE_CG_2_8;
            IQInfo->COLOR_PARAM.ColorGain.ChR=Rgain;
            IQInfo->COLOR_PARAM.ColorGain.ChGr=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb=256;
            IQInfo->COLOR_PARAM.ColorGain.ChB=Bgain;
            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
//            IQInfo->COLOR_PARAM.ColorCorrect=Indenity_CC;
            IQInfo->COLOR_PARAM.ColorSupWeight=22;
        break;
//#NT#2009/06/29#Jarkko Chang -end

        case IPE_CAP_SETTING :
        case IPE_D2D_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;

//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for using pre gamma/cg
            IQInfo->FuncSel = IPE_RLPF_EN |
                                         IPE_GLPF_EN |
                                         IPE_BLPF_EN |
                                         IPE_CCR_EN |
                                         IPE_CTRL_EN |
                                         IPE_HADJ_EN |
                                         IPE_CADJ_EN |
                                         IPE_CADJ_YENH_EN |
                                         IPE_CADJ_YCON_EN |
                                         IPE_CADJ_CCON_EN |
                                         IPE_CADJ_CLPF_EN |
//                                         IPE_CGAIN_EN |
//                                         IPE_PREGAMMA_EN;
                                         IPE_NR_EN ;

            IQInfo->ParamUpdateSel = IPE_SET_ALL&~IPE_SET_GAMMA;
//            IQInfo->ParamUpdateSel = IPE_SET_ALL;
//#NT#2009/10/16#Jarkko Chang -end
            //#NT#2011/08/25#Spark Chou -begin
            //#NT# DVI Input
            if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
                IQInfo->FuncSel = 0;
            //#NT#2011/08/25#Spark Chou -end

            IQInfo->GammaLut[IPE_R_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_G_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_B_CHANNEL] = GammaLUT_128Tab;

            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#2009/12/02#Jarkko Chang -begin
            IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //#NT#2009/12/02#Jarkko Chang -end
            IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            //#NT#2010/06/20#Spark Chou -begin
            //IQInfo->EExtHVopt = HVOPT_AVG;
            //#NT#2010/06/20#Spark Chou -end
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[gImageAlgInfo.ImageEffect];
            IQInfo->HueTab = NULL;
            IQInfo->SatTab = NULL;
            IQInfo->IntTab = NULL;
            IQInfo->DDSTab = NULL;

            IQInfo->COLOR_PARAM.CGRange=IPE_CG_2_8;
            IQInfo->COLOR_PARAM.ColorGain.ChR=Rgain;
            IQInfo->COLOR_PARAM.ColorGain.ChGr=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb=256;
            IQInfo->COLOR_PARAM.ColorGain.ChB=Bgain;
            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
//            IQInfo->COLOR_PARAM.ColorCorrect=Indenity_CC;
            IQInfo->COLOR_PARAM.ColorSupWeight=22;
        break;
        case IPE_PRV_AE_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;

            IQInfo->ParamUpdateSel = IPE_SET_NR | IPE_SET_EDGE | IPE_SET_COLOR;

            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#2009/12/02#Jarkko Chang -begin
            IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //#NT#2009/12/02#Jarkko Chang -end
            IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            //#NT#2010/06/20#Spark Chou -begin
           // IQInfo->EExtHVopt = HVOPT_MAX;
            //#NT#2010/06/20#Spark Chou -end
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;

            if(IPL_GetMode()== IPL_MODE_VIDEOREC)
            {
                IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC_Video;
            }
            else
            {
                IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
            }
//            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
//            IQInfo->COLOR_PARAM.ColorCorrect=Indenity_CC;
            IQInfo->COLOR_PARAM.ColorSupWeight=22;
        break;
        case IPE_PRV_AF_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_ENABLE;
            IQInfo->FuncSel = IPE_VACC_EN;

            IQInfo->ParamUpdateSel = IPE_SET_EDGE | IPE_SET_VA;

//            IQInfo->EdgeKerH = IPE_AFedgeKernelDiv;
//            IQInfo->EdgeKerV = IPE_AFedgeKernelDiv;
            //#NT#2010/06/20#Spark Chou -begin
            //IQInfo->EExtHVopt = HVOPT_AVG;
            //#NT#2010/06/20#Spark Chou -end
        break;
        default :
            debug_err(("ipe_IQparam() param mode selection error!\r\n"));
            return E_SYS;
        break;
    }
    //Move Gamma setting out from IPE driver
    if(IQInfo->ParamUpdateSel & IPE_SET_GAMMA)
    {
        CPU_WriteGammaTab(IQInfo->GammaLut[IPE_R_CHANNEL], IQInfo->GammaLut[IPE_G_CHANNEL], IQInfo->GammaLut[IPE_B_CHANNEL]);
    }
    return E_OK;
}

//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
void Sen_EnterSleepMode(IPL_SLEEP_LEVEL Level)
{
    StopPreviewDramData();
    pre_close();
    sie_close();
    if (SensorInfor.bSieMClkEn == ENABLE)
    {
        pll_disableClock(SIE_MCLK);
    }
    ipe_close();
    ime_close();
}

void Sen_LeaveSleepMode(IPL_SLEEP_LEVEL Level)
{
    IPL_OBJ IplObj;
    SIE_OPENOBJ SIEOpenObj;
    PRE_OPENOBJ PREOpenObj;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;

    SIEOpenObj.FP_SIEISR_CB = sie_isrCB;
    PREOpenObj.FP_PREISR_CB = pre_isrCB;
    IPEOpenObj.FP_IPEISR_CB = ipe_isrCB;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = ime_isrCB;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    if (SensorInfor.bSieMClkEn == ENABLE)
    {
        pll_enableClock(SIE_MCLK);
    }
    sie_open(&SIEOpenObj);
    pre_open(&PREOpenObj);
    ipe_open(&IPEOpenObj);
    ime_open(&IMEOpenObj);
    IplObj.uiCmd = NULL;
    Set2Preview(&IplObj);
}
//#NT#2009/10/08#Bowen Li -end


void Sen_FlowFunc(SEN_FLOW_MSG MsgID, SenCBInfo *SenInfo)
{
    switch(MsgID)
    {
        case AE_PRV_INIT:
            aaa_InitAE();
            break;

        case AE_PRV_SETTING:
            AeCap2Prv();
            break;

        case AE_CAP_SETTING:
            if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
            {
                AePrv2Cap();
                SetIPPFuncInfor(CAP_ISO, ISONUM0);
            }
            break;

        case AWB_PRV_INIT:
            aaa_InitRBgain();
            aaa_InitAWB();
            break;

        case AWB_PRV_SETTING:
            AwbCap2Prv();
            break;

        case AWB_CAP_SETTING:
            AwbPrv2Cap();
            break;

        case PREFLASH_FLOW:
            flash_PreFlashFlow();
            break;

        case IPE_CAP_SETTING:
        case IPE_D2D_SETTING:
        case IPE_PRV_SETTING:
            ipe_IQparam(MsgID, &SenInfo->IPEModeInfo->IQINFO);
            ipe_changeParam(&SenInfo->IPEModeInfo->IQINFO);
            break;

        case IPE_PRV_AE_SETTING:
        case IPE_PRV_AF_SETTING:
            ipe_IQparam(MsgID, &SenInfo->IPEModeInfo->IQINFO);
            ipe_changeParam(&SenInfo->IPEModeInfo->IQINFO);
            SetSaturation(gImageAlgInfo.Saturation);
            SetSharpness(gImageAlgInfo.Sharpness);
            break;

        case TG_INIT:
            debug_err(("TG_INIT  gImageAlgInfo.TvInFormat = %d\n\r",gImageAlgInfo.TvInFormat));
            //#NT#2011/08/25#Spark Chou -begin
            //#NT# DVI Input
            if ( gImageAlgInfo.TvInFormat == _TvInFormat_OFF)
                Sensor_config(SIE_PRE_Para_prv.ModeSel);
            else
                Sensor_config_TVP5150(SIE_PRE_Para_prv.ModeSel);
            //#NT#2011/08/25#Spark Chou -end
            break;

        case AFE_INIT:
            break;

        case PRE_PRV_SETTING:
            pre_PrvParameter(SenInfo->IppMgrInfo);
            break;

        case PRE_CAP_SETTING:
            pre_CapParameter(SenInfo->IppMgrInfo);
            break;

        case PRE_D2D_SETTING:
            pre_D2DParameter(SenInfo->IppMgrInfo);
            break;

        case SIE_PRE_GET_CAP_RAW:
            siepre_GetCapRaw(SenInfo->SiePreCapInfo);
            break;

        case SENSOR_MODE_SETTING:
//            Sensor_Mode_Transform(SenInfo->SiePrePrvInfo->ModeSel);
            Ae_Transform();
            aaa_AeLock(LOCK);
            break;

        case AE_PRV_GAIN_SETTING:
            AeSetAGCGain();
            break;

        case IPPFUNC_Switch:
            IPPFunc_OnOffSwitch();
            break;

        case AAFD_Switch:
            AAFD_LockSwitch(SenInfo->IPPImgMode);
            break;

//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
        case IPL_SLEEP:
            Sen_EnterSleepMode(SenInfo->IplSleepLevel);
            break;

        case IPL_WAKEUP:
            Sen_LeaveSleepMode(SenInfo->IplSleepLevel);
            break;
//#NT#2009/10/08#Bowen Li -end
        //#NT#2009/12/09#Jarkko Chang -begin
        case IME_Chrom_Prv_SETTING:
            ime_ChromPram(MsgID);
            break;
        case IME_Chrom_Prv_AE_SETTING:
            ime_ChromPram(MsgID);
            break;
        case IME_Chrom_Cap_SETTING:
            ime_ChromPram(MsgID);
            break;
        //#NT#2009/12/09#Jarkko Chang -end
        default:
            debug_err(("-E- MsgID(%d) > MaxID(%d)\r\n", MsgID, SEN_FLOW_MSG_NUM));
            break;
    }
}
#endif
