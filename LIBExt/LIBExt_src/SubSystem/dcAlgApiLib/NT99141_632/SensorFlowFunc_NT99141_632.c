#include    "sensor_model.h"
#if NT99141_632
#include "Pll.h"
#include "ErrorNo.h"
#include "ae_NT99141_632.h"

UINT32 gDynaGammaSetting = FALSE;
UINT16 gDynaGammaOutL=0, gDynaGammaOutH=1023;
UINT8 IPE_DynaIntTab[24];
SenCBInfo gSenCBInfo = {0, &IPPMgrReq, &SIE_PRE_Para_cap, &SIE_PRE_Para_prv, &IPEModeInfo};
//#NT#20091123#ethanlau -begin
//#NT#2010/11/15#JJ Huang -begin
#define ISO_BASE 50
#define ISO0_THRES 300
#define ISO1_THRES 450
#define ISO2_THRES 600

#define FMSTAB_EV_RANGE     100
#define FMSTAB_EV_START     (EV_Value_MIN+FMSTAB_EV_RANGE)

//#NT#20091207#ethanlau -begin
#define ChromaEnableThres NormalExpectY*(100+YConRange)/100
//#NT#20091207#ethanlau -end

BOOL Firstflag = FALSE;
//#NT#20091123#ethanlau -end

//#NT#20091127#ethanlau -begin
IPE_ISONumEnum GetISOIdx(UINT32 ISO)
{
     UINT32 ISO_Gain = AE_Param.ISO_Gain;
     UINT32 ISO_Value;
    if(IPL_GetMode()== IPL_MODE_CAP)
    {
        if (ISO_Gain < 200)
            ISO_Value = ISONUM4;
        else if (ISO_Gain < 400)
            ISO_Value = ISONUM5;
        else
            ISO_Value = ISONUM6;
    }
    else
    {
        if (ISO_Gain < 100)
            ISO_Value = ISONUM0;
        else if (ISO_Gain < 150)
            ISO_Value = ISONUM0;
        else if (ISO_Gain < 200)
            ISO_Value = ISONUM1;
        else
            ISO_Value = ISONUM2;
    }
    return ISO_Value;
}
//#NT#20091127#ethanlau -end
//#NT#20091207#ethanlau -begin
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
        case IME_Chrom_Cap_SETTING:
            break;
        default:
            debug_err(("ime_ChromPram() param mode selection error!\r\n"));
            return E_SYS;
            break;
    }

    ime_setChromaFilter(&ime_filt_para);
    return E_OK;

}

UINT32 GetInterpolationValue(UINT32 IQ_Value_L, UINT32 IQ_Value_H, UINT32 Ref_Diff, UINT32 Range_RefDiff)
{
    UINT32 IQ_Range;
    UINT32 InterpolationValue;

    IQ_Range = IQ_Value_H - IQ_Value_L;

    InterpolationValue = IQ_Value_L + ((IQ_Range * Ref_Diff) / Range_RefDiff);

    return InterpolationValue;
}

IPE_DYNAIQ_PARAM GetDynaIQ_Param(UINT16 IsoIdx, UINT32 IsoRatio)
{
    IPE_DYNAIQ_PARAM IQ_Param;
    UINT16 i;
    UINT32 Increase_IsoDiff, Decrease_IsoDiff;

    IQ_Param.IsoRatio = IsoRatio;

    Increase_IsoDiff = IQ_Param.IsoRatio - IPE_DynaIQ_Param[IsoIdx-1].IsoRatio;
    Decrease_IsoDiff = IPE_DynaIQ_Param[IsoIdx].IsoRatio - IQ_Param.IsoRatio;

    IQ_Param.Enhance = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx].Enhance, IPE_DynaIQ_Param[IsoIdx-1].Enhance, Decrease_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    IQ_Param.EthrA = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].EthrA, IPE_DynaIQ_Param[IsoIdx].EthrA, Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    IQ_Param.EthrB = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].EthrB, IPE_DynaIQ_Param[IsoIdx].EthrB, Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    IQ_Param.RBW = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].RBW, IPE_DynaIQ_Param[IsoIdx].RBW, Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    IQ_Param.GW = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].GW, IPE_DynaIQ_Param[IsoIdx].GW, Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    IQ_Param.Outl_RBW = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].Outl_RBW, IPE_DynaIQ_Param[IsoIdx].Outl_RBW, Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    IQ_Param.Outl_GW = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].Outl_GW, IPE_DynaIQ_Param[IsoIdx].Outl_GW, Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));

    for(i=0; i<4; i++)
    {
        IQ_Param.Nthr[i] = GetInterpolationValue(IPE_DynaIQ_Param[IsoIdx-1].Nthr[i], IPE_DynaIQ_Param[IsoIdx].Nthr[i], Increase_IsoDiff, (IPE_DynaIQ_Param[IsoIdx].IsoRatio-IPE_DynaIQ_Param[IsoIdx-1].IsoRatio));
    }

    return IQ_Param;
}

#define IPE_PARAM_DYNAMIC_EN    ENABLE

#define EEXT_CHANGE_TH_L    300
#define EEXT_CHANGE_TH_H    400

UINT8 FSTable[16], FDTable[16];
//INT16 DYNASAT[13] =  {-32,-31,-30,-28,-25,-15,-5,5,8,10,12,14,16};
INT16 DYNASAT[13] =  {0,0,0,0,0,0,0,5,8,10,12,14,16};
#define DYNAGAMMA_NORMAL_OUT_L	0
#define DYNAGAMMA_NORMAL_OUT_H	1023
#define DYNAGAMMA_NIGHT_OUT_L	80
#define DYNAGAMMA_NIGHT_OUT_H	1023

//#NT#20091207#ethanlau -end
ER ipe_IQparam(UINT32 ParamMode, IPE_IQINFO *IQInfo)
{
    IPE_ISONumEnum ISO_Value;
    UINT32 ratio;
//#NT#20091123#ethanlau -begin
    BOOL bInterpolation = FALSE;
    PImageAlgInfo pimginfo;
#if IPE_PARAM_DYNAMIC_EN
    UINT32 Increase_IsoDiff, Decrease_IsoDiff;
    UINT32 IsoGain, IsoRatio;
    UINT16 i, idx, StartIdx, EndIdx;
    IPE_ISONumEnum ISO_Value_L, ISO_Value_H;
    IPE_DYNAIQ_PARAM Ipe_Param;
    UINT32 EV_Diff;    
    static UINT8 EEXT_Type = 0x00;
#else
    UINT32 *LinearItpl_rbw,*LinearItpl_gw,up,low;
#endif

    pimginfo = UI_GetParameter();

    if(ParamMode == IPE_CAP_SETTING)
        IsoGain = AE_Param.ISO_Gain_Cap;
    else
        IsoGain = AE_Param.ISO_Gain;

#if 0
    SenFP_ClearOSD();
    sprintf(message,"%d", IsoGainDual);
    SenFP_ShowOSDString(message,20,70,4);
#endif

    if(!Firstflag)
    {
        // For preview & video
        Firstflag = TRUE;
    }

    if(ParamMode == IPE_CAP_SETTING)
    {
        ISO_Value = ISONUM4;
        StartIdx = (Total_ISONum>>1)+1;
        EndIdx = Total_ISONum;
    }
    else
    {
        ISO_Value = ISONUM0;
        StartIdx = 1;
        EndIdx = (Total_ISONum>>1);
    }

    // Set default value    
    IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
    IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;

    IsoRatio  = (IsoGain*100/ISO_Gain_Min);

    if(IsoRatio < 100)
        IsoRatio = 100;
    else if(IsoRatio > IPE_DynaIQ_Param[EndIdx-1].IsoRatio)
        IsoRatio = IPE_DynaIQ_Param[EndIdx-1].IsoRatio;

    for(idx=StartIdx; idx<EndIdx; idx++)
    {
        if(IsoRatio <= IPE_DynaIQ_Param[idx].IsoRatio)
            break;
    }

    if(IsoRatio >= EEXT_CHANGE_TH_H)
    {
        EEXT_Type = 0x01;
		//gDynaGammaOutL = DYNAGAMMA_NIGHT_OUT_L;
		//gDynaGammaOutH = DYNAGAMMA_NIGHT_OUT_H;
	}
    else if(IsoRatio <= EEXT_CHANGE_TH_L)
    {
        EEXT_Type = 0x00;
		//gDynaGammaOutL = DYNAGAMMA_NORMAL_OUT_L;
		//gDynaGammaOutH = DYNAGAMMA_NORMAL_OUT_H;
    }   

	if(IsoRatio > EEXT_CHANGE_TH_H)
	{
		gDynaGammaOutH = DYNAGAMMA_NORMAL_OUT_H;
		gDynaGammaOutL = DYNAGAMMA_NORMAL_OUT_L + ((IsoRatio-EEXT_CHANGE_TH_H)>>0);

		if(gDynaGammaOutL > DYNAGAMMA_NIGHT_OUT_L)
			gDynaGammaOutL = DYNAGAMMA_NIGHT_OUT_L;
	}
	else
	{
		gDynaGammaOutH = DYNAGAMMA_NORMAL_OUT_H;
		gDynaGammaOutL = DYNAGAMMA_NORMAL_OUT_L;
	}
    
    Ipe_Param = GetDynaIQ_Param(idx, IsoRatio);

    // Edge Parameter
    IQInfo->EDGE_PARAM->EdgEnhance = Ipe_Param.Enhance;
    IQInfo->EDGE_PARAM->EdgEnhance = ((IQInfo->EDGE_PARAM->EdgEnhance*Sharpness[ISO_Value][pimginfo->Sharpness]) / 100);

	if(EEXT_Type==0x01)
    	IQInfo->EDGE_PARAM->EthrA = (Ipe_Param.EthrA*100/150);//(Ipe_Param.EthrA>>3);
	else
		IQInfo->EDGE_PARAM->EthrA = Ipe_Param.EthrA;
	
    IQInfo->EDGE_PARAM->EthrB = Ipe_Param.EthrB;

    // NR Parameter
    IQInfo->NR_PARAM->RBW = Ipe_Param.RBW;
    IQInfo->NR_PARAM->GW = Ipe_Param.GW;
    IQInfo->NR_PARAM->OutlRbw = Ipe_Param.Outl_RBW;
    IQInfo->NR_PARAM->OutlGw = Ipe_Param.Outl_GW;
    
    for(i=0; i<4; i++)
    {
        IQInfo->NR_PARAM->Nthr[i] = Ipe_Param.Nthr[i];
    }

    // For fmStab
    if(AE_Param.EV_Value >= FMSTAB_EV_START)
    {
        EV_Diff = 0;
    }
    else
    {
        EV_Diff = FMSTAB_EV_START - AE_Param.EV_Value;
    }

#if 0
    IQInfo->FStab = IPE_fmStab[ISONUM0];
    IQInfo->FDtab = IPE_fmDtab[ISONUM0];
#else

    IQInfo->FStab = FSTable;
    IQInfo->FDtab = FDTable;

    for(i=0; i<16; i++)
    {
        IQInfo->FStab[i] = (UINT8)GetInterpolationValue((UINT32)IPE_fmStab[idx-1][i], (UINT32)IPE_fmStab[idx][i], EV_Diff, FMSTAB_EV_RANGE);
        IQInfo->FDtab[i] = (UINT8)GetInterpolationValue((UINT32)IPE_fmDtab[idx-1][i], (UINT32)IPE_fmDtab[idx][i], EV_Diff, FMSTAB_EV_RANGE);
    }
#endif    

/*
	for(i=0; i<24; i++)
		IPE_DynaIntTab[i] = (UINT8)(256-(gDynaGammaOutL>>1));
*/
	if(IPL_GetMode()==IPL_MODE_PCC)
	{
		EEXT_Type = 0x01;
	}

    if(ParamMode == IPE_CAP_SETTING)
    {
        IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
        IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;        
    }
    else
    {
        IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value+EEXT_Type].EdgeKerDivSel;
        IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value+EEXT_Type].EdgeKerDivSel;        
    }

//#NT#20091123#ethanlau -end
    switch(ParamMode)
    {
//#NT#2009/06/29#Jarkko Chang -begin
//#NT#prepare for 96630 preview using
        case IPE_PRV_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;
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
//                                         IPE_PREGAMMA_EN |
                                         IPE_NR_EN ;

            //#NT#2010/01/08#YH Lee -begin
            //#NT# Mod param update select, only used functions are updated
            //IQInfo->ParamUpdateSel = IPE_SET_ALL&~IPE_SET_GAMMA;
            //IQInfo->ParamUpdateSel = IPE_SET_ALL;
            IQInfo->ParamUpdateSel = IPE_SET_NR |
                                                      IPE_SET_EDGE |
                                                      IPE_SET_COLOR_GAIN |
                                                      IPE_SET_COLOR |
                                                      IPE_SET_COLOREFFECT;
            //#NT#2010/01/08#YH Lee -end
            IQInfo->GammaLut[IPE_R_CHANNEL] = GammaLUT_128Tab_Video_Normal;
            IQInfo->GammaLut[IPE_G_CHANNEL] = GammaLUT_128Tab_Video_Normal;
            IQInfo->GammaLut[IPE_B_CHANNEL] = GammaLUT_128Tab_Video_Normal;

            //IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            //IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#20091127#ethanlau -begin
            //IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //IQInfo->EDGE_PARAM->EdgEnhance = ((IQInfo->EDGE_PARAM->EdgEnhance*Sharpness[ISO_Value][pimginfo->Sharpness]) / 100);
            //#NT#20091127#ethanlau -end
            //IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            //IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            //IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            //IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
             //#NT#20091127#ethanlau -begin
    
            IQInfo->COLOR_EFFECT_PARAM->SatOfs = DYNASAT[(AE__GetLV()/10)];//Saturation[pimginfo->Saturation];
         
            //#NT#20091127#ethanlau -end
             //#NT#2010/11/09#JJ Huang -begin
            IQInfo->HueTab = IPE_hueTab;
            IQInfo->SatTab = IPE_satTab;
            IQInfo->IntTab = IPE_intTab;
            IQInfo->DDSTab = IPE_ddsTab;
             //#NT#2010/11/09#JJ Huang -end

            IQInfo->COLOR_PARAM.CGRange=IPE_CG_2_8;
            IQInfo->COLOR_PARAM.ColorGain.ChR=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGr=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb=256;
            IQInfo->COLOR_PARAM.ColorGain.ChB=256;
            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
            IQInfo->COLOR_PARAM.ColorSupWeight=22;
            gDynaGammaSetting = TRUE;
        break;
//#NT#2009/06/29#Jarkko Chang -end

        case IPE_CAP_SETTING :
        case IPE_D2D_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;

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
//                                         IPE_PREGAMMA_EN|
                                         IPE_NR_EN ;

            //#NT#2010/01/08#YH Lee -begin
            //#NT# Mod param update select, only used functions are updated
            //IQInfo->ParamUpdateSel = IPE_SET_ALL&~IPE_SET_GAMMA;
            //IQInfo->ParamUpdateSel = IPE_SET_ALL;
            IQInfo->ParamUpdateSel = IPE_SET_NR |
                                                      IPE_SET_EDGE |
                                                      IPE_SET_COLOR_GAIN |
                                                      IPE_SET_COLOR |
                                                      IPE_SET_COLOREFFECT;
            //#NT#2010/01/08#YH Lee -end
            IQInfo->GammaLut[IPE_R_CHANNEL] = GammaLUT_128Tab_Capture;
            IQInfo->GammaLut[IPE_G_CHANNEL] = GammaLUT_128Tab_Capture;
            IQInfo->GammaLut[IPE_B_CHANNEL] = GammaLUT_128Tab_Capture;

            //IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            //IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //IQInfo->EDGE_PARAM->EdgEnhance = ((IQInfo->EDGE_PARAM->EdgEnhance*Sharpness[ISO_Value][pimginfo->Sharpness]) / 100);
            //IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            //IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            //IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            //IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
            //#NT#20091127#ethanlau -begin
            IQInfo->COLOR_EFFECT_PARAM->SatOfs = DYNASAT[(AE__GetLV()/10)];//Saturation[pimginfo->Saturation];
            //#NT#20091127#ethanlau -end

             //#NT#2010/11/09#JJ Huang -begin
            IQInfo->HueTab = IPE_hueTab;
            IQInfo->SatTab = IPE_satTab;
            IQInfo->IntTab = IPE_intTab;
            IQInfo->DDSTab = IPE_ddsTab;
             //#NT#2010/11/09#JJ Huang -end            

            IQInfo->COLOR_PARAM.CGRange=IPE_CG_2_8;
            IQInfo->COLOR_PARAM.ColorGain.ChR=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGr=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb=256;
            IQInfo->COLOR_PARAM.ColorGain.ChB=256;
            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
            IQInfo->COLOR_PARAM.ColorSupWeight=22;
        break;
        case IPE_PRV_AE_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;

            IQInfo->ParamUpdateSel = IPE_SET_NR | IPE_SET_EDGE | IPE_SET_COLOR | IPE_SET_COLOREFFECT;

            //IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            //#NT#2010/09/17#ethanlau -begin
            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
            IQInfo->COLOR_EFFECT_PARAM->SatOfs= Saturation[pimginfo->Saturation];
            IQInfo->COLOR_EFFECT_PARAM->SatOfs = DYNASAT[(AE__GetLV()/10)];//Saturation[pimginfo->Saturation];
            //#NT#2010/09/17#ethanlau -end

            //IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            //IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            //IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            //IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

			//IQInfo->IntTab = IPE_DynaIntTab;

            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;
            IQInfo->COLOR_PARAM.ColorGain.ChR=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGr=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb=256;
            IQInfo->COLOR_PARAM.ColorGain.ChB=256;
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
            IQInfo->COLOR_PARAM.ColorSupWeight=22;
			gDynaGammaSetting = TRUE;
        break;
        case IPE_PRV_AF_SETTING :
//#NT#2010/11/01#Jarkko Chang -begin
            IQInfo->FuncUpdateSel = IPE_FUNC_ENABLE;
//#NT#2010/11/01#Jarkko Chang -end
            IQInfo->FuncSel |= IPE_VACC_EN;

            IQInfo->ParamUpdateSel = IPE_SET_EDGE | IPE_SET_VA;

            IQInfo->EdgeKerH = IPE_AFedgeKernelDiv;
            IQInfo->EdgeKerV = IPE_AFedgeKernelDiv;
        break;
        default :
            debug_err(("ipe_IQparam() param mode selection error!\r\n"));
            return E_SYS;
        break;
    }


#if 0
    if(1)//(ParamMode==IPE_CAP_SETTING)
    {
    SenFP_ClearOSD();
/*
    switch(ParamMode)
    {
        case IPE_PRV_SETTING:
            sprintf(message,"PRV");
            break;
        case IPE_PRV_AE_SETTING:
            sprintf(message,"PRV_AE");
            break;
        case IPE_CAP_SETTING:
            sprintf(message,"CAP");
            break;

    }
*/
    //SenFP_ShowOSDString(message,20,40,4);
#if 1
    sprintf(message,"%d, %d", IsoRatio, EEXT_Type);
    SenFP_ShowOSDString(message,20,70,4);
#endif

#if 0
    sprintf(message,"%d, %d", gDynaGammaOutL, gDynaGammaOutH);
    SenFP_ShowOSDString(message,20,70,4);
#endif	

#if 0
    sprintf(message,"%d", IPE_DynaIntTab[0]);
    SenFP_ShowOSDString(message,20,70,4);
#endif

#if 0
    sprintf(message,"%d, %d", IsoGainDual, Ipe_Param.IsoRatio);
    SenFP_ShowOSDString(message,20,70,4);
#endif

#if 0
    sprintf(message,"%d, %d, %d", IQInfo->NR_PARAM->RBW, IQInfo->NR_PARAM->GW, IQInfo->NR_PARAM->Nthr[0]);
    SenFP_ShowOSDString(message,20,100,4);
#endif

#if 0
    sprintf(message,"%d, %d, %d", IQInfo->EDGE_PARAM->EdgEnhance, IQInfo->EDGE_PARAM->EthrA, IQInfo->EDGE_PARAM->EthrB);
    SenFP_ShowOSDString(message,20,130,4);
#endif

#if 0
    sprintf(message,"%d, %d, %d, %d", IPE_fmStab[ISO_Value][0], IPE_fmStab[ISO_Value][1], IPE_fmStab[ISO_Value][2], IPE_fmStab[ISO_Value][3]);
    SenFP_ShowOSDString(message,20,90,4);
    sprintf(message,"%d, %d, %d, %d", IPE_fmStab[ISO_Value][4], IPE_fmStab[ISO_Value][5], IPE_fmStab[ISO_Value][6], IPE_fmStab[ISO_Value][7]);
    SenFP_ShowOSDString(message,20,110,4);
    sprintf(message,"%d, %d, %d, %d", IPE_fmStab[ISO_Value][8], IPE_fmStab[ISO_Value][9], IPE_fmStab[ISO_Value][10], IPE_fmStab[ISO_Value][11]);
    SenFP_ShowOSDString(message,20,130,4);
    sprintf(message,"%d, %d, %d, %d", IPE_fmStab[ISO_Value][12], IPE_fmStab[ISO_Value][13], IPE_fmStab[ISO_Value][14], IPE_fmStab[ISO_Value][15]);
    SenFP_ShowOSDString(message,20,150,4);
#endif

#if 0
    sprintf(message,"%d", AE_Param.EV_Value);
    SenFP_ShowOSDString(message,20,70,4);
    sprintf(message,"%d, %d, %d, %d", IQInfo->FStab[0], IQInfo->FStab[1], IQInfo->FStab[2], IQInfo->FStab[3]);
    SenFP_ShowOSDString(message,20,90,4);
    sprintf(message,"%d, %d, %d, %d", IQInfo->FStab[4], IQInfo->FStab[5], IQInfo->FStab[6], IQInfo->FStab[7]);
    SenFP_ShowOSDString(message,20,110,4);
    sprintf(message,"%d, %d, %d, %d", IQInfo->FStab[8], IQInfo->FStab[9], IQInfo->FStab[10], IQInfo->FStab[11]);
    SenFP_ShowOSDString(message,20,130,4);
    sprintf(message,"%d, %d, %d, %d", IQInfo->FStab[12], IQInfo->FStab[13], IQInfo->FStab[14], IQInfo->FStab[15]);
    SenFP_ShowOSDString(message,20,150,4);
#endif
    }
#endif
/*
    //Move Gamma setting out from IPE driver
    if(IQInfo->ParamUpdateSel & IPE_SET_GAMMA)
    {
        CPU_WriteGammaTab(IQInfo->GammaLut[IPE_R_CHANNEL], IQInfo->GammaLut[IPE_G_CHANNEL], IQInfo->GammaLut[IPE_B_CHANNEL]);
    }
*/    
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
    //#NT#20091126#ethanlau -begin
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    switch(MsgID)
    {
        case AE_PRV_INIT:
            //#NT#2010/09/09#ethanlau -begin
            aaa_InitAE(0);
            //#NT#2010/09/09#ethanlau -end
            break;

        case AE_PRV_SETTING:
            //#NT#2010/09/09#ethanlau -begin
            Ae_Transform(0);
            //#NT#2010/09/09#ethanlau -end
            AeCap2Prv();
            break;
        case AE_CAP2PRV_SETTING:
            AeCap2Prv();
            break;
        case AE_CAP_SETTING:
            AePrv2Cap();
            SetIPPFuncInfor(CAP_ISO, ISONUM0);
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
            //#NT#20090706#ETHAN LAU -BEGIN
            ipe_IQparam(MsgID, &SenInfo->IPEModeInfo->IQINFO);
            ipe_changeParam(&SenInfo->IPEModeInfo->IQINFO);
            break;
            //#NT#20090706#ETHAN LAU -END
        case IPE_PRV_AF_SETTING:
            ipe_IQparam(MsgID, &SenInfo->IPEModeInfo->IQINFO);
            ipe_changeParam(&SenInfo->IPEModeInfo->IQINFO);
            break;

        case TG_INIT:
            Sensor_config(SIE_PRE_Para_prv.ModeSel);
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
            Sensor_SetMode(SenInfo->SiePrePrvInfo->ModeSel);
            Ae_Transform(0);
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
        case IME_Chrom_Prv_SETTING:
            ime_ChromPram(MsgID);
            break;
        case IME_Chrom_Prv_AE_SETTING:
            ime_ChromPram(MsgID);
            break;
        case IME_Chrom_Cap_SETTING:
            ime_ChromPram(MsgID);
            break;
//#NT#20120820#clifflin -begin
//#NT#Add sensor flip function
        case SENSOR_FLIP_TYPE:
            Sensor_SetFlip(SenInfo->SENSORFlipType, FALSE);
            Sensor_config(SIE_PRE_Para_prv.ModeSel);//jack lan
			
            break;
//#NT#20120820#clifflin -end
        default:
            debug_err(("-E- MsgID(%d) > MaxID(%d)\r\n", MsgID, SEN_FLOW_MSG_NUM));
            break;
    }
}
//#NT#2010/09/10#ethanlau -begin
//#NT#_bit: gamma input bit
//#NT#return bits same with gamma in
UINT32 GammaInverse(UINT32 InputLum,UINT32* _gamma,_IMGSieBitDepth_sel _bit)
{
    UINT32 i,Y_Pregamma,_tblsize;
    UINT32 Weight;
    _tblsize = (1<<_bit)>>3;

    for(i=1; i<_tblsize; i++)
    {
        if(InputLum <_gamma[i])
        {
            break;
        }
    }
    Weight = (InputLum-_gamma[i-1])*100/(_gamma[i]-_gamma[i-1]);


    Y_Pregamma = (i-1)*8 + Weight*8/100;

    //debug_err(("in=%d, out=%d\r\n",InputLum,Y_Pregamma));

    return Y_Pregamma;
}
//#NT#InputLum: bits same with gamma in
//#NT#return bits same with gamma in
UINT32 GammaMap(UINT32 InputLum,UINT32* _gamma)
{
    UINT32 i,Y_Postgamma;
    UINT32 Weight;

    i = InputLum /8;

    Weight = (InputLum-i*8)*100/8;

    Y_Postgamma = _gamma[i] + Weight*(_gamma[i+1]-_gamma[i])/100;

    //debug_err(("in=%d, out=%d\r\n",InputLum,Y_Postgamma));

    return Y_Postgamma;
}
//#NT#2010/09/10#ethanlau -end
#endif
