#include "sensor_model.h"
#include "ime.h"


void SetPrvParams(IPP_PARAM *IppParam_Prv)
{
//#NT#2009/11/02#Bowen Li -begin
//#NT#modify CA function for 2 & 3 line
    UINT32 HRatio, VRatio, Ratio;

    SetSensorMode(IppParam_Prv->Sie_Struct->ModeSel);

    memset(&IppParam_Prv->Sie_Struct->SieFieldNum, 0, sizeof(STR_FLDNUM));
    IppParam_Prv->Sie_Struct->VDTotal = GetIPPSizeInfor(_VDTotal_Preview);
    IppParam_Prv->Sie_Struct->VDSync = GetIPPSizeInfor(_VDSync_Preview);
    IppParam_Prv->Sie_Struct->HDActX = GetIPPSizeInfor(_HDActX_Preview);
    IppParam_Prv->Sie_Struct->HDActY[0] = GetIPPSizeInfor(_HDActY_Preview);
    IppParam_Prv->Sie_Struct->HDTotal = GetIPPSizeInfor(_HDTotal_Preview);
    IppParam_Prv->Sie_Struct->HDSync = GetIPPSizeInfor(_HDSync_Preview);
    IppParam_Prv->Sie_Struct->RawCenterX = GetIPPSizeInfor(_SieXcenter);
    IppParam_Prv->Sie_Struct->RawCenterY = GetIPPSizeInfor(_SieYcenter);
    IppParam_Prv->Sie_Struct->PreCropSizeH = GetIPPSizeInfor(_PreCropHsize);
    IppParam_Prv->Sie_Struct->PreCropSizeV = GetIPPSizeInfor(_PreCropVsize);
    IppParam_Prv->Sie_Struct->PreOutSizeH = GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF; //?????
    IppParam_Prv->Sie_Struct->PreOutLineOfs = GetIPPSizeInfor(_PreOutLineOffset);
    IppParam_Prv->Sie_Struct->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_PRV_RAW_1);
    IppParam_Prv->Sie_Struct->PreOutAdd1 = ippbuf_GetBuf(IPPBUF_PRV_RAW_2);
    IppParam_Prv->Sie_Struct->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1);
    IppParam_Prv->Sie_Struct->SieOutAdd1 = ippbuf_GetBuf(IPPBUF_PRV_SIERAW_2);
    IppParam_Prv->Sie_Struct->SieObOfs = GetIPPSizeInfor(_OBOffset);
    if (IppParam_Prv->Sie_Struct->AutoObEn == ENABLE)
    {
        IppParam_Prv->Sie_Struct->SieObParam.X0 = GetIPPSizeInfor(_OBWinX0);
        IppParam_Prv->Sie_Struct->SieObParam.X1 = GetIPPSizeInfor(_OBWinX1);
        IppParam_Prv->Sie_Struct->SieObParam.Y0 = GetIPPSizeInfor(_OBWinY0);
        IppParam_Prv->Sie_Struct->SieObParam.Y1 = GetIPPSizeInfor(_OBWinY1);
        IppParam_Prv->Sie_Struct->SieObParam.SubSample = GetIPPSizeInfor(_OBSubSample);
        IppParam_Prv->Sie_Struct->SieObParam.Thres = GetIPPSizeInfor(_OBThreshold);
        IppParam_Prv->Sie_Struct->SieObGain = GetIPPSizeInfor(_OBGain);
    }

    if (IppParam_Prv->Sie_Struct->PreFuncEn & PRE_CA_EN)
    {
        Sen_GetCASubRatio(IppParam_Prv->Sie_Struct->VGAType, GetIPPSizeInfor(_CAHSub), GetIPPSizeInfor(_CAVSub), &HRatio, &VRatio);
        Ratio = Sen_GetIntlvRatio(IppParam_Prv->Sie_Struct->VGAType);
        SetIPPSizeInfor(_CAHsize, (GetIPPSizeInfor(_PreOutHsize) * Ratio)/HRatio);
        SetIPPSizeInfor(_CAVsize, (GetIPPSizeInfor(_PreOutVsize) / Ratio)/VRatio);
        SetIPPSizeInfor(_CALineOffset, GetIPPSizeInfor(_CAHsize));
    }
    else
    {
        SetIPPSizeInfor(_CAHsize, GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF);
        SetIPPSizeInfor(_CAVsize, GetIPPSizeInfor(_IpeOutVsize));
        SetIPPSizeInfor(_CALineOffset, GetIPPSizeInfor(_PreOutLineOffset));
    }
//#NT#2009/11/02#Bowen Li -end
    if (gImageAlgInfo.SieBitDepth == _SieBitDepth_8)
    {
        IppParam_Prv->Sie_Struct->SieBitDepth = SIE_OUT_PACK_8BIT;
        IppParam_Prv->Sie_Struct->PreInBitDepth = PRE_INPACK_8BIT;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_12)
    {
        IppParam_Prv->Sie_Struct->SieBitDepth = SIE_OUT_PACK_12BIT;
        IppParam_Prv->Sie_Struct->PreInBitDepth = PRE_INPACK_12BIT;
    }
    else if (gImageAlgInfo.SieBitDepth == _SieBitDepth_16)
    {
        IppParam_Prv->Sie_Struct->SieBitDepth = SIE_OUT_PACK_16BIT;
        IppParam_Prv->Sie_Struct->PreInBitDepth = PRE_INPACK_16BIT;
    }
    else
    {
        debug_err(("Prv SieBitDepth error\r\n"));
    }

    if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_8)
    {
        IppParam_Prv->Sie_Struct->PreBitDepth = PRE_OUTPACK_8BIT;
    }
    else if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_10)
    {
        IppParam_Prv->Sie_Struct->PreBitDepth = PRE_OUTPACK_10BIT;
    }
    else if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_12)
    {
        IppParam_Prv->Sie_Struct->PreBitDepth = PRE_OUTPACK_12BIT;
    }
    else if (gImageAlgInfo.PrePrvBitDepth == _PreBitDepth_16)
    {
        IppParam_Prv->Sie_Struct->PreBitDepth = PRE_OUTPACK_16BIT;
    }
    else
    {
        debug_err(("Prv PreBitDepth error\r\n"));
    }

    IppParam_Prv->Ipe_Struct->VAAddr = ippbuf_GetBuf(IPPBUF_PRV_VA);
    IppParam_Prv->Ipe_Struct->Add0 = IppParam_Prv->Sie_Struct->PreOutAdd0;
    IppParam_Prv->Ipe_Struct->Add1 = IppParam_Prv->Sie_Struct->PreOutAdd1;
    IppParam_Prv->Ipe_Struct->BitDepth = IppParam_Prv->Sie_Struct->PreBitDepth;
    IppParam_Prv->Ipe_Struct->InSizeH = IppParam_Prv->Sie_Struct->PreOutSizeH;
    IppParam_Prv->Ipe_Struct->InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    IppParam_Prv->Ipe_Struct->InLineOfs = IppParam_Prv->Sie_Struct->PreOutLineOfs;

    IppParam_Prv->Ime_Struct->y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    IppParam_Prv->Ime_Struct->cb0_addr=(IppParam_Prv->Ime_Struct->y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IppParam_Prv->Ime_Struct->cr0_addr=(IppParam_Prv->Ime_Struct->y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IppParam_Prv->Ime_Struct->y1_addr=IppParam_Prv->Ime_Struct->y0_addr+2*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IppParam_Prv->Ime_Struct->cb1_addr=(IppParam_Prv->Ime_Struct->y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IppParam_Prv->Ime_Struct->cr1_addr=(IppParam_Prv->Ime_Struct->y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IppParam_Prv->Ime_Struct->y2_addr=IppParam_Prv->Ime_Struct->y0_addr+4*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IppParam_Prv->Ime_Struct->cb2_addr=(IppParam_Prv->Ime_Struct->y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IppParam_Prv->Ime_Struct->cr2_addr=(IppParam_Prv->Ime_Struct->y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);

    IppParam_Prv->Ime_Struct->in_h=GetIPPSizeInfor(_IpeOutHsize);
    IppParam_Prv->Ime_Struct->in_v=GetIPPSizeInfor(_IpeOutVsize);
    IppParam_Prv->Ime_Struct->out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IppParam_Prv->Ime_Struct->out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IppParam_Prv->Ime_Struct->in_format=IME_IN_FMT_422;
    IppParam_Prv->Ime_Struct->out_format=IME_OUT_FMT_422_COSITED;
    //#NT#20091119#ethanlau -begin
    //IppParam_Prv->Ime_Struct->out_YCC_pack = IMEALG_CBCR_PACK;
    //#NT#20091119#ethanlau -end
    IppParam_Prv->Ime_Struct->out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);

    //#NT#2011/08/25#Spark Chou -begin
    //#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        IppParam_Prv->Ipe_Struct->DVIFormat = IppParam_Prv->Ipe_Struct->DVIFormat;
        IppParam_Prv->Ipe_Struct->DVIHsub = IppParam_Prv->Ipe_Struct->DVIHsub;
        IppParam_Prv->Ipe_Struct->InSizeH = (IppParam_Prv->Ipe_Struct->InLineOfs >> 1);
        IppParam_Prv->Ime_Struct->in_h=IppParam_Prv->Ipe_Struct->InSizeH;
    }
    //#NT#2011/08/25#Spark Chou -end
}

UINT32 Set2Prv_Common(IPP_PARAM *IppParam_Prv)
{
    static BOOL FirstFlg = TRUE;

    SetPrvParams(IppParam_Prv);

//#NT#20090902#ethanlau -begin
//move to below
#if 0
    if(IppParam_Prv->Sie_Struct->SieMaster == 0)
    {
        SenFlowFunc(TG_INIT, &gSenCBInfo);
        SenFlowFunc(AFE_INIT, &gSenCBInfo);
    }
#endif
//#NT#20090902#ethanlau -end
    if (FirstFlg == TRUE)
    {
        IMEReset();
        IPEReset();
        SIEReset();
    }

    //#NT#2012/01/03#Jarkko Chang -begin
    //#NT# modify for DVI MST using
    ime_Set2Preview(IppParam_Prv->Ime_Struct);
    ime_to_ipe_Stripe_trans(&IppParam_Prv->Ipe_Struct->HStripe,&IppParam_Prv->Ipe_Struct->VStripe);

    if (IppParam_Prv->Ime_Struct->ime_sten == IMEALG_MULTI)
//    if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
    {
        IppParam_Prv->Ipe_Struct->HStripe.olap = IPE_OLAP_ON;
        IppParam_Prv->Ipe_Struct->VStripe.olap = IPE_OLAP_ON;
    }
    else
    {
        IppParam_Prv->Ipe_Struct->HStripe.olap = IPE_OLAP_OFF;
        IppParam_Prv->Ipe_Struct->VStripe.olap = IPE_OLAP_OFF;
    }
    //#NT#2012/01/03#Jarkko Chang -end

    ipe_Set2Preview(IppParam_Prv->Ipe_Struct);

    sie_Set2Preview(IppParam_Prv->Sie_Struct);

//#NT#20090902#ethanlau -begin
    if(IppParam_Prv->Sie_Struct->SieMaster == 1)
    {
        SenFlowFunc(TG_INIT, &gSenCBInfo);
        SenFlowFunc(AFE_INIT, &gSenCBInfo);
    }
    else //if(IppParam_Prv->Sie_Struct->SieMaster == 0)
    {
        SenFlowFunc(TG_INIT, &gSenCBInfo);
        SenFlowFunc(AFE_INIT, &gSenCBInfo);
    }
//#NT#20090902#ethanlau -end
if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
{
    if(FirstFlg == TRUE)
    {
        SenFlowFunc(AE_PRV_INIT, &gSenCBInfo);
        SenFlowFunc(AWB_PRV_INIT, &gSenCBInfo);
    }
    else
    {
        SenFlowFunc(AE_PRV_SETTING, &gSenCBInfo);
        SenFlowFunc(AWB_PRV_SETTING, &gSenCBInfo);
    }
}
    //#NT#2010/01/06#Jarkko Chang -begin
    //#NT#drop sensor first frame
    sie_waitVD(2);
    //#NT#2010/01/06#Jarkko Chang -end

    sie_start();
    pre_start();
    FirstFlg = FALSE;
    return 0;

}

