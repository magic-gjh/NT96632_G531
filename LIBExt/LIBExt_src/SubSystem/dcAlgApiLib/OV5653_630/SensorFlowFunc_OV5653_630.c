#include    "sensor_model.h"
#if OV5653_630
#include "ErrorNo.h"

SenCBInfo gSenCBInfo = {0, &IPPMgrReq, &SIE_PRE_Para_cap, &SIE_PRE_Para_prv, &IPEModeInfo};
//#NT#20091123#ethanlau -begin
//#NT#2011/01/27#JJ Huang -begin
#define ISO_BASE 50
#define ISO0_THRES 80
#define ISO1_THRES ((100+200)/2)
#define ISO2_THRES ((200+400)/2)
//#NT#2011/01/27#JJ Huang -end

//#NT#20091207#ethanlau -begin
#define ChromaEnableThres (NormalExpectY*(100-(2*YConRange))/100)
//#NT#20091207#ethanlau -end
UINT32 LinearItpl_RBW_0=0 ;
UINT32 LinearItpl_GW_0=0 ;
UINT32 LinearItpl_RBW_1=0 ;
UINT32 LinearItpl_GW_1=0 ;
UINT32 LinearItpl_RBW_2=0 ;
UINT32 LinearItpl_GW_2=0 ;
BOOL Firstflag = FALSE;
//#NT#20091123#ethanlau -end

//#NT#20091127#ethanlau -begin
IPE_ISONumEnum GetISOIdx(UINT32 ISO)
{
//#NT#2011/01/27#JJ Huang -begin
    if(IPL_GetMode()== IPL_MODE_CAP)
    {
        if(AE_Param.ISO_Gain_Cap<ISO1_THRES)
            return ISONUM3;
        else if(AE_Param.ISO_Gain_Cap<=ISO2_THRES)
            return ISONUM4;
        else
            return ISONUM5;
    }
    else
    {
        if(AE_Param.ISO_Gain<ISO1_THRES)
            return ISONUM0;
        else if(AE_Param.ISO_Gain<=ISO2_THRES)
            return ISONUM1;
        else
            return ISONUM2;
    }
//#NT#2011/01/27#JJ Huang -end
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
        //#NT#2011/01/18#JJ Huang -begin
        //#NT# For Cap IME Chroma Filter/Curve
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
        //#NT#2011/01/18#JJ Huang -end
            break;
        default:
            debug_err(("ime_ChromPram() param mode selection error!\r\n"));
            return E_SYS;
            break;
    }

    ime_setChromaFilter(&ime_filt_para);
    return E_OK;

}
//#NT#20091207#ethanlau -end
ER ipe_IQparam(UINT32 ParamMode, IPE_IQINFO *IQInfo)
{
    IPE_ISONumEnum ISO_Value;
    UINT32 ratio;
//#NT#20091123#ethanlau -begin
    UINT32 *LinearItpl_rbw,*LinearItpl_gw,up,low;
    BOOL bInterpolation = FALSE;
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    if(!Firstflag)
    {
        LinearItpl_RBW_0 = IPE_ISOParam[ISONUM0].NRParamSel->RBW;
        LinearItpl_GW_0 = IPE_ISOParam[ISONUM0].NRParamSel->GW;
        LinearItpl_RBW_1 = IPE_ISOParam[ISONUM1].NRParamSel->RBW;
        LinearItpl_GW_1 = IPE_ISOParam[ISONUM1].NRParamSel->GW;
        LinearItpl_RBW_2 = IPE_ISOParam[ISONUM2].NRParamSel->RBW;
        LinearItpl_GW_2 = IPE_ISOParam[ISONUM2].NRParamSel->GW;
        Firstflag = TRUE;
    }
//#NT#2010/11/15#JJ Huang -begin
#if 1
//#NT#2011/01/27#JJ Huang -begin
    if(ParamMode == IPE_CAP_SETTING)
    {
        if(AE_Param.ISO_Gain_Cap<ISO1_THRES)
            ISO_Value =ISONUM3;
        else if(AE_Param.ISO_Gain_Cap<=ISO2_THRES)
            ISO_Value =ISONUM4;
        else
            ISO_Value =ISONUM5;
    }
    else
    {
        if(AE_Param.ISO_Gain<ISO1_THRES)
            ISO_Value =ISONUM0;
        else if(AE_Param.ISO_Gain<=ISO2_THRES)
            ISO_Value =ISONUM1;
        else
            ISO_Value =ISONUM2;
    }
//#NT#2011/01/27#JJ Huang -end
#else
    if(AE_Param.ISO_Gain <= ISO0_THRES)
        ISO_Value = ISONUM0;
    else if(AE_Param.ISO_Gain <= ISO1_THRES)
        ISO_Value = ISONUM1;
    else if(AE_Param.ISO_Gain <= ISO2_THRES)
        ISO_Value = ISONUM2;
    else
        ISO_Value = ISONUM3;
    if(ParamMode == IPE_CAP_SETTING)
    {
        if(AE_Param.ISO_Gain_Cap<100)
            ISO_Value =ISONUM4;
        else if(AE_Param.ISO_Gain_Cap<=200)
            ISO_Value =ISONUM2;
        else
            ISO_Value =ISONUM3;
    }
#endif
//#NT#2010/11/15#JJ Huang -end

//#NT#20091123#ethanlau -end
    switch(ParamMode)
    {
//#NT#2009/06/29#Jarkko Chang -begin
//#NT#prepare for 96630 preview using
        case IPE_PRV_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;
#if 0             
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

//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for pre ca process
//                                         IPE_CGAIN_EN |
//#NT#2009/08/19#Jarkko#[0006137] -begin
//                                         IPE_EDGETH_EN |
//#NT#2009/08/19#Jarkko#[0006137] -end
                                         IPE_NR_EN ;
//                                         IPE_PREGAMMA_EN;
#endif
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
                                         IPE_NR_EN
                                         ;


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
//#NT#2009/08/31#Jarkko Chang -end
            IQInfo->GammaLut[IPE_R_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_G_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_B_CHANNEL] = GammaLUT_128Tab;

            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#20091127#ethanlau -begin
            IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //#NT#20091127#ethanlau -end
            IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
             //#NT#20091127#ethanlau -begin
            //IQInfo->COLOR_EFFECT_PARAM->SatOfs= Saturation[pimginfo->Saturation];
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
//#NT#2011/01/27#JJ Huang -begin
            IQInfo->COLOR_PARAM.ColorSupWeight=33;//22;
//#NT#2011/01/27#JJ Huang -end
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
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for pre ca process
//                                         IPE_CGAIN_EN |
//#NT#2009/08/19#Jarkko#[0006137] -begin
//                                         IPE_EDGETH_EN |
//#NT#2009/08/19#Jarkko#[0006137] -end
                                         IPE_NR_EN ;
//                                         IPE_PREGAMMA_EN;

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
//#NT#2009/08/31#Jarkko Chang -end
            IQInfo->GammaLut[IPE_R_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_G_CHANNEL] = GammaLUT_128Tab;
            IQInfo->GammaLut[IPE_B_CHANNEL] = GammaLUT_128Tab;

            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#20091127#ethanlau -begin
            IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //#NT#20091127#ethanlau -end
            IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
            //#NT#20091127#ethanlau -begin
            //IQInfo->COLOR_EFFECT_PARAM->SatOfs= Saturation[pimginfo->Saturation];
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
//#NT#2011/01/27#JJ Huang -begin
            IQInfo->COLOR_PARAM.ColorSupWeight=33;//22;
//#NT#2011/01/27#JJ Huang -end
        break;
        case IPE_PRV_AE_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;

            IQInfo->ParamUpdateSel = IPE_SET_NR | IPE_SET_EDGE | IPE_SET_COLOR | IPE_SET_COLOREFFECT;

            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
//#NT#20091123#ethanlau -begin
//#NT#2011/01/27#JJ Huang -begin
            #if 0 // The interpolation might be not-linear; please try bi-directional interpolation
            if(ISO_Value == ISONUM0)
            {
                LinearItpl_rbw = &LinearItpl_RBW_0;
                LinearItpl_gw = &LinearItpl_RBW_0;
                low = ISO_BASE;
                up = ISO1_THRES;
                bInterpolation = TRUE;
            }
            else if(ISO_Value == ISONUM1)
            {
                LinearItpl_rbw = &LinearItpl_RBW_1;
                LinearItpl_gw = &LinearItpl_RBW_1;
                low = ISO1_THRES;
                up = ISO2_THRES;
                bInterpolation = TRUE;
            }
            /*
            else if(ISO_Value == ISONUM2)
            {
                LinearItpl_rbw = &LinearItpl_RBW_2;
                LinearItpl_gw = &LinearItpl_RBW_2;
                low = ISO_THRES;
                up = ISO2_THRES;
                bInterpolation = TRUE;
            }
            */
            else
                bInterpolation = FALSE;
            if(bInterpolation)
            {
                ratio = 100*(AE_Param.ISO_Gain - low)/(up - low+1);
                IQInfo->NR_PARAM->RBW = ratio*(*LinearItpl_rbw)/100 + (*LinearItpl_rbw);
                if(IQInfo->NR_PARAM->RBW > 63)
                    IQInfo->NR_PARAM->RBW = 63;
                IQInfo->NR_PARAM->GW = ratio*(*LinearItpl_gw)/100 + (*LinearItpl_gw);
                if(IQInfo->NR_PARAM->GW > 63)
                    IQInfo->NR_PARAM->GW = 63;
                //debug_msg("ratio=%d,IQInfo->NR_PARAM.RBW=%d ,IQInfo->NR_PARAM.GW=%d\r\n",ratio,IQInfo->NR_PARAM->RBW,IQInfo->NR_PARAM->GW);
            }
            #endif
//#NT#2011/01/27#JJ Huang -end
//#NT#20091123#ethanlau -end
            IQInfo->NRStab = IPE_ISOParam[ISO_Value].NRStabSel;
            IQInfo->NRDtab = IPE_ISOParam[ISO_Value].NRDtabSel;

            IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            //#NT#20091127#ethanlau -begin
            IQInfo->EDGE_PARAM->EdgEnhance = Sharpness[ISO_Value][pimginfo->Sharpness];
            //#NT#20091127#ethanlau -end
            //#NT#2010/09/17#ethanlau -begin
            IQInfo->COLOR_EFFECT_PARAM = &ColorEffectTable[pimginfo->ImageEffect];
            IQInfo->COLOR_EFFECT_PARAM->SatOfs= Saturation[pimginfo->Saturation];
            //#NT#2010/09/17#ethanlau -end

            IQInfo->EdgeKerH = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->EdgeKerV = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Emap = IPE_ISOParam[ISO_Value].EmapSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;

            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_PARAM.ColorOffset.ChR=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGr=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChGb=0;
            IQInfo->COLOR_PARAM.ColorOffset.ChB=0;
            IQInfo->COLOR_PARAM.ColorGain.ChR=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGr=256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb=256;
            IQInfo->COLOR_PARAM.ColorGain.ChB=256;
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
//#NT#2011/01/27#JJ Huang -begin
            IQInfo->COLOR_PARAM.ColorSupWeight=33;//22;
//#NT#2011/01/27#JJ Huang -end
            //#NT#2011/03/02#ethanlau -begin
            //#NT#add for dis
            if(dis_isEnabled())
            {
                IQInfo->EdgeKerH = &DIS_Ker;
                IQInfo->EdgeKerV = &DIS_Ker;
                IQInfo->ETH_PARAM = &DIS_EthParam;
                IQInfo->FuncUpdateSel = IPE_FUNC_ENABLE;
                IQInfo->FuncSel |= IPE_EDGETH_EN;
                IQInfo->ParamUpdateSel |= IPE_SET_ETH;
            }
            //#NT#2011/03/02#ethanlau -end
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
    //Move Gamma setting out from IPE driver
    if(IQInfo->ParamUpdateSel & IPE_SET_GAMMA)
    {
        CPU_WriteGammaTab(IQInfo->GammaLut[IPE_R_CHANNEL], IQInfo->GammaLut[IPE_G_CHANNEL], IQInfo->GammaLut[IPE_B_CHANNEL]);
    }
    return E_OK;
}


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
            //AeCap2Prv();
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
        //#NT#20091207#ethanlau -begin
        case IME_Chrom_Prv_SETTING:
            ime_ChromPram(MsgID);
            break;
        case IME_Chrom_Prv_AE_SETTING:
            ime_ChromPram(MsgID);
            break;
        case IME_Chrom_Cap_SETTING:
            ime_ChromPram(MsgID);
            break;
        //#NT#20091207#ethanlau -end
        //#NT#2010/09/27#ethanlau -begin
        case IPL_SLEEP:
        case IPL_WAKEUP:
            break;
        //#NT#2010/09/27#ethanlau -end
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
