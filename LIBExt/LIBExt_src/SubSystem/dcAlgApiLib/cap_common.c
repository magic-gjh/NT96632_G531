#include    "sensor_model.h"

#include    "pll.h"
#include    "AlbumShot.h"
#include    "ime.h"
#include    "IDE.h"
#include    "JpgEnc.h"
#include    "Grph.h"
#include    "filesystsk.h"
#include    "CoupleShot.h"
#include    "PreShot.h"
#include    "ime_lib.h"


UINT (*ChromaPara)[5];

IME_FBADDR_PRAM IMEJPGAddrP1,IMEJPGAddrP2;
extern IPP_FUNC_INFOR gIPPFuncInfor;

void Exec_D2D(SIE_PRE_PARAM *SiePreInfo, UINT8 rot_en)
{
    UINT8 OpMode;
    ImageAlgInfo *AlgInfo;
    AlgInfo = UI_GetParameter();
    OpMode = 0;
    if (rot_en)
    {

        if (AlgInfo->PreCapBitDepth == _PreBitDepth_16)
        {
            OpMode = 1;
        }
        SIE_PRE_Para_d2d.SiePreAddr0 = SiePreInfo->PreOutAdd0;
        SIE_PRE_Para_d2d.PreInBitDepth = SiePreInfo->PreBitDepth;
        SIE_PRE_Para_d2d.PreInLineOfs = SiePreInfo->PreOutLineOfs;
        SIE_PRE_Para_d2d.PreDramInSizeH = SiePreInfo->PreCropSizeH;
        SIE_PRE_Para_d2d.PreDramInSizeV = SiePreInfo->PreCropSizeV;
        SIE_PRE_Para_d2d.PreSrcSel = PATH_POST_GAMMA;
        SIE_PRE_Para_d2d.PreOutAdd0 = SiePreInfo->SiePreAddr2;
        SIE_PRE_Para_d2d.PreBitDepth = SiePreInfo->PreBitDepth;
        SIE_PRE_Para_d2d.PreOutLineOfs = SiePreInfo->PreOutLineOfs;
        pre_Dram2Dram(&SIE_PRE_Para_d2d);

        //First. Rotate 90 for image processing
        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(SIE_PRE_Para_d2d.PreOutAdd0, SIE_PRE_Para_d2d.PreOutLineOfs, SIE_PRE_Para_d2d.PreDramInSizeV, SIE_PRE_Para_d2d.PreOutLineOfs);
        grph_setImg2(SIE_PRE_Para_d2d.SiePreAddr0, (SIE_PRE_Para_d2d.PreDramInSizeV * AlgInfo->PreCapBitDepth >> 3));
        grph_setGOP(OpMode, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);


        //Second. Setting Pre parameter according to rotated image.
        SIE_PRE_Para_d2d.PreInLineOfs = (SiePreInfo->PreCropSizeV * AlgInfo->PreCapBitDepth >> 3);
        SIE_PRE_Para_d2d.PreDramInSizeH = SiePreInfo->PreCropSizeV;
        SIE_PRE_Para_d2d.PreDramInSizeV = SiePreInfo->PreCropSizeH;
        SIE_PRE_Para_d2d.PreOutLineOfs = (SiePreInfo->PreCropSizeV * AlgInfo->PreCapBitDepth >> 3);
        pre_Dram2Dram(&SIE_PRE_Para_d2d);

        //Third. Rotate image back.
        grph_setImg1(SIE_PRE_Para_d2d.PreOutAdd0, SIE_PRE_Para_d2d.PreOutLineOfs, SIE_PRE_Para_d2d.PreDramInSizeV, SIE_PRE_Para_d2d.PreOutLineOfs);
        grph_setImg2(SIE_PRE_Para_d2d.SiePreAddr0, (SIE_PRE_Para_d2d.PreDramInSizeV * AlgInfo->PreCapBitDepth >> 3));
        grph_setGOP(OpMode, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);
        grph_disableDMA();
        grph_close();
    }
    else
    {
        SIE_PRE_Para_d2d.SiePreAddr0 = SiePreInfo->PreOutAdd0;
        SIE_PRE_Para_d2d.PreInBitDepth = SiePreInfo->PreBitDepth;
        SIE_PRE_Para_d2d.PreInLineOfs = SiePreInfo->PreOutLineOfs;
        SIE_PRE_Para_d2d.PreDramInSizeH = SiePreInfo->PreCropSizeH;
        SIE_PRE_Para_d2d.PreDramInSizeV = SiePreInfo->PreCropSizeV;
        SIE_PRE_Para_d2d.PreSrcSel = PATH_POST_GAMMA;
        SIE_PRE_Para_d2d.PreOutAdd0 = SiePreInfo->SiePreAddr2;
        SIE_PRE_Para_d2d.PreBitDepth = SiePreInfo->PreBitDepth;
        SIE_PRE_Para_d2d.PreOutLineOfs = SiePreInfo->PreOutLineOfs;
        pre_Dram2Dram(&SIE_PRE_Para_d2d);
    }
}

//#NT#2010/01/06#YC Peng -begin
//#NT#Modify capture AWB flow
//#NT#2009/12/18#YC Peng -begin
//#NT#Modify capture AWB flow
void Set2SieCap_Normal(SIE_PRE_PARAM *SiePreInfo, UINT32 BufIdx)
{
    //CAP AE/AWB parameter
    SenFlowFunc(AE_CAP_SETTING, &gSenCBInfo);

    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_NORMAL);
    SiePreInfo->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_SIERAW_1);
    SiePreInfo->SieOutAdd1 = ippbuf_GetBuf(IPPBUF_CAP_SIERAW_2);
    if (GetIPPFuncInfor(PRED2D_EN) && !(GetIPPFuncInfor(PRE_D2D_ROT_EN)))
    {
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
        SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1 + BufIdx);
    }
    else
    {
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1 + BufIdx);
        SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
    }
    CaptureOne(SiePreInfo);
    SenFlowFunc(AWB_CAP_SETTING, &gSenCBInfo);

    if (GetIPPFuncInfor(PRED2D_EN))
    {
        Exec_D2D(SiePreInfo,GetIPPFuncInfor(PRE_D2D_ROT_EN));
    }
}
//#NT#2009/12/18#YC Peng -end
//#NT#2010/01/06#YC Peng -end

void Set2SieCap_AlbumShot(SIE_PRE_PARAM *SiePreInfo)
{

    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING, &gSenCBInfo);

    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_ALBUMSHOT);
    SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
    CaptureOne(SiePreInfo);

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING, &gSenCBInfo);
    if (ASGetCapCounter() == 0)
    {
        ASCapParamInit();
    }
    ASImgProcess(ASGetCapCounter());
}


void Set2SieCap_CoupleShot(SIE_PRE_PARAM *SiePreInfo)
{
    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_COUPLESHOT);
    if(CSGetCapCounter()==COUPLESHOT_0)
    {
        CSParamInit();
        CSCopyPrvYCC();
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    }
    else if(CSGetCapCounter()==COUPLESHOT_1)
    {
        ide_disable_video(IDE_VIDEOID_2);
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_2);
    }

    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING, &gSenCBInfo);

    //Capture RAW
    CaptureOne(SiePreInfo);

    //merge couple shot img1 to img0
    if(CSGetCapCounter()==COUPLESHOT_1)
    {
        CSMergeCapRAW();
    }

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING, &gSenCBInfo);
}
void Set2SieCap_PreShot(SIE_PRE_PARAM *SiePreInfo)
{
    //assign preshot buffer address
    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_PRESHOT);
    if(PSGetCapCounter()==PRESHOT_0)
    {
        PSParamInit();
        PSCopyPrvYCC();
    }
    else if(PSGetCapCounter()==PRESHOT_1)
    {
        ide_disable_video(IDE_VIDEOID_2);
    }
    SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);

    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING, &gSenCBInfo);

    //Capture RAW
    CaptureOne(SiePreInfo);

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING, &gSenCBInfo);
}

//#NT#2009/12/04#YC Peng -begin
//#NT#Add drive mode AdjFocusShot for adjust FF lens
void Set2SieCap_AdjFocusShot(SIE_PRE_PARAM *SiePreInfo)
{
    //CAP AE parameter
    SenFlowFunc(AE_CAP_SETTING, &gSenCBInfo);

    ippbuf_ExecuteAssigner(IPPBUF_MODE_CAP_NORMAL);
    SiePreInfo->SieOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_SIERAW_1);
    SiePreInfo->SieOutAdd1 = ippbuf_GetBuf(IPPBUF_CAP_SIERAW_2);
    if (GetIPPFuncInfor(PRED2D_EN) && !(GetIPPFuncInfor(PRE_D2D_ROT_EN)))
    {
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
        SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    }
    else
    {
        SiePreInfo->PreOutAdd0 = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
        SiePreInfo->SiePreAddr2 = ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME);
    }
    CaptureOne(SiePreInfo);
    SenAdjFocusMode();

    //CAP AWB parameter
    SenFlowFunc(AWB_CAP_SETTING, &gSenCBInfo);
    if (GetIPPFuncInfor(PRED2D_EN))
    {
        Exec_D2D(SiePreInfo,GetIPPFuncInfor(PRE_D2D_ROT_EN));
    }
}
//#NT#2009/12/04#YC Peng -end

void Set2SieCap_Init(void)
{
    gSenCBInfo.IPPImgMode = IPPImg_BeforCap;
    SenFlowFunc(AAFD_Switch, &gSenCBInfo);

    if(g_fpAlgExport)
    {
        g_fpAlgExport((ALGMSG_SUSKEY), &gAlgMsg);
        g_fpAlgExport((ALGMSG_SUSFLOWPRV), &gAlgMsg);
        g_fpAlgExport((ALGMSG_SUSPHOTO), &gAlgMsg);
        //#NT#20090929#Bowen Li -begin
        //#NT#add new function for change clock
        //g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
        //#NT#20090929#Bowen Li -end
    }
}

void Set2SieCap_SetParam(void)
{
    IPL_OBJ IplObj;
    UINT CapInfo;

    Lens_OnOff(ON);
    ide_disable_video(IDE_VIDEOID_1);

    //assign cap parameter
    SetIPPSizeInfor(_FQVSampleRate, FQV_SAMPLE_RATE);
    SetIPPSizeInfor(_CapIMESliceMode, SLICE_MODE);
    SetIPPSizeInfor(_ThumbHsize, CAP_THUMB_H);
    SetIPPSizeInfor(_ThumbVsize, CAP_THUMB_V);
    SetIPPSizeInfor(_QVHsize, CAP_QV_H);
    SetIPPSizeInfor(_QVVsize, CAP_QV_V);
    SetIPPSizeInfor(_QVPreHsize, CAP_QV_PRE_H);
    SetIPPSizeInfor(_QVPreVsize, CAP_QV_PRE_V);
    SetIPPSizeInfor(_ScreenHsize, CAP_SCREEN_H);
    SetIPPSizeInfor(_ScreenVsize, CAP_SCREEN_V);
    SetIPPSizeInfor(_PreOutLineOffset, (GetIPPSizeInfor(_PreCropCapHsize) * gImageAlgInfo.PreCapBitDepth / 8));
    SetIPPSizeInfor(_ImeCapOutHsize, GetIPPSizeInfor(_JpegCapHsize));
    SetIPPSizeInfor(_ImeCapOutVsize, GetIPPSizeInfor(_JpegCapVsize));
    Get_CapInfo(Cap_SyncMode, &CapInfo);
    SetSensorMode(CapInfo);

    SetSiePreCapParam(&SIE_PRE_Para_cap);

    SenFlowFunc(IPPFUNC_Switch, &gSenCBInfo);

    SenFlowFunc(PREFLASH_FLOW, &gSenCBInfo);

    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_CAP;
    IPL_SetCmd(&IplObj);
}

void Set2SieCap(ImageAlgInfo *AlgInfo, UINT32 BufIdx)
{
    switch(AlgInfo->DriveMode)
    {
        //normal mode(single, burst, continue......)
        case _IMGDriveMode_BurstFlash:
        case _IMGDriveMode_Burst:
        case _IMGDriveMode_InfBurst:
        case _IMGDriveMode_AEBreaketing:
        case _IMGDriveMode_AWBBreaketing:
        case _IMGDriveMode_Single:
            if(g_fpAlgExport)
                g_fpAlgExport(ALGMSG_SOUND_SHUTTER2, &gAlgMsg);
            SetIPPFuncInfor(FQV_EN, ENABLE);
            //#NT#2010/11/11#ethanlau -begin
            SetIPPFuncInfor(_StillAntiShakingIdx, AlgInfo->StillAntiShaking);
            //#NT#2010/11/11#ethanlau -end
            Set2SieCap_Normal(&SIE_PRE_Para_cap, BufIdx);
            break;

        case _IMGDriveMode_PreShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_PreShot(&SIE_PRE_Para_cap);
            break;

        case _IMGDriveMode_CoupleShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_CoupleShot(&SIE_PRE_Para_cap);
            break;

        case _IMGDriveMode_AlbumShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_AlbumShot(&SIE_PRE_Para_cap);
            break;

        //#NT#2009/12/04#YC Peng -begin
        //#NT#Add drive mode AdjFocusShot for adjust FF lens
        case _IMGDriveMode_AdjFocusShot:
            SetIPPFuncInfor(FQV_EN, DISABLE);
            Set2SieCap_AdjFocusShot(&SIE_PRE_Para_cap);
            break;
        //#NT#2009/12/04#YC Peng -end

        default:
        debug_err(("-E- AlgInfo->DriveMode = %d\r\n", AlgInfo->DriveMode));
            break;
    }
}

ER SliceMode_EventHandle(UINT32 uiJpgStatus, UINT32 uiImeStatus)
{
    if (uiJpgStatus & _JPEG_Buf_End)
    {
        while((uiImeStatus & Ime_INT_FRM) == 0)
        {
            //TrgImeSlice();
            ime_triggerSlice();
            uiImeStatus = ime_waitDone();
        }
        return E_BOVR;
    }
    return E_OK;
}

ER DVIMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize)
{
    UINT32 BlackV;
    IPE_IME_FM_PARAM fmInfo;
    UINT JpgYAddr, JpgCbAddr, JpgCrAddr, CounterMax;
    UINT        JPG_BSize;
    JPGCmpParam    JPGParam;
    char        ER_Code;

//    IPE_Para.DVIFormat = DVI_FORMAT;
//    IPE_Para.DVIHsub = DVI_HSUB;


    //source
    fmInfo.BayerAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    fmInfo.BayerHsize = GetIPPSizeInfor(_PreCropCapHsize);
    fmInfo.BayerVsize = GetIPPSizeInfor(_PreCropCapVsize);

    //dest
    fmInfo.ImeOutPath = IMEALG_1PATH;
    fmInfo.OutputAddr1Y = ippbuf_GetBuf(IPPBUF_CAP_FRAME_Y);
    fmInfo.OutputAddr1Cb = ippbuf_GetBuf(IPPBUF_CAP_FRAME_CB);
    fmInfo.OutputAddr1Cr = ippbuf_GetBuf(IPPBUF_CAP_FRAME_CR);
    fmInfo.ImeHsize1 = GetIPPSizeInfor(_ImeCapOutHsize);
    fmInfo.ImeVsize1 = GetIPPSizeInfor(_ImeCapOutVsize);
    fmInfo.ImeHofst1 = fmInfo.ImeHsize1;
    fmInfo.ImeFmt1 = IME_OUT_FMT_422_COSITED;

    //adjust image ratio
    debug_err(("^RgImageAlgInfo.ImageRatio %d\r\n", gImageAlgInfo.ImageRatio));
    if (gImageAlgInfo.ImageRatio != _Ratio_4_3)
    {
        if ( gImageAlgInfo.ImageRatio == _Ratio_1_1 )
        {
            //get adjust ratio
            //fmInfo.ImeHsize1 = Adj_HSize(fmInfo.ImeHsize1, Get_HRatio(gImageAlgInfo.ImageRatio, _Ratio_1_1), 16);
            memset((void *)fmInfo.OutputAddr1Y, 0x0, (GetIPPSizeInfor(_ImeCapOutHsize) * fmInfo.ImeVsize1));
            memset((void *)fmInfo.OutputAddr1Cb, 0x80, ((GetIPPSizeInfor(_ImeCapOutHsize) / 2) * fmInfo.ImeVsize1));
            memset((void *)fmInfo.OutputAddr1Cr, 0x80, ((GetIPPSizeInfor(_ImeCapOutHsize) / 2) * fmInfo.ImeVsize1));
            fmInfo.OutputAddr1Y += ((GetIPPSizeInfor(_ImeCapOutHsize) - fmInfo.ImeHsize1) >> 1);
            fmInfo.OutputAddr1Cb += (((GetIPPSizeInfor(_ImeCapOutHsize) - fmInfo.ImeHsize1) >> 1) >> 1);
            fmInfo.OutputAddr1Cr += (((GetIPPSizeInfor(_ImeCapOutHsize) - fmInfo.ImeHsize1) >> 1) >> 1);
        }
        else
        {
            //get adjust ratio
            fmInfo.ImeVsize1 = Adj_VSize(fmInfo.ImeVsize1, Get_VRatio(gImageAlgInfo.ImageRatio, _Ratio_4_3), 4);
            BlackV = (GetIPPSizeInfor(_ImeCapOutVsize) - fmInfo.ImeVsize1) >> 1;
            //upper
            memset((void *)fmInfo.OutputAddr1Y, 0x0, (fmInfo.ImeHsize1 * BlackV));
            memset((void *)fmInfo.OutputAddr1Cb, 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));
            memset((void *)fmInfo.OutputAddr1Cr, 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));
            //lower
            memset((void *)(fmInfo.OutputAddr1Y + (fmInfo.ImeHsize1 * (BlackV + fmInfo.ImeVsize1))), 0x0, (fmInfo.ImeHsize1 * BlackV));
            memset((void *)(fmInfo.OutputAddr1Cb + ((fmInfo.ImeHsize1 / 2) * (BlackV + fmInfo.ImeVsize1))), 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));
            memset((void *)(fmInfo.OutputAddr1Cr + ((fmInfo.ImeHsize1 / 2) * (BlackV + fmInfo.ImeVsize1))), 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));

            fmInfo.OutputAddr1Y += (((GetIPPSizeInfor(_ImeCapOutVsize) - fmInfo.ImeVsize1) >> 1) * fmInfo.ImeHsize1);
            fmInfo.OutputAddr1Cb += ((((GetIPPSizeInfor(_ImeCapOutVsize) - fmInfo.ImeVsize1) >> 1) * fmInfo.ImeHsize1) >> 1);
            fmInfo.OutputAddr1Cr += ((((GetIPPSizeInfor(_ImeCapOutVsize) - fmInfo.ImeVsize1) >> 1) * fmInfo.ImeHsize1) >> 1);
        }
    }
    ipe_ime_Framemode(fmInfo);

    //Address to be setup in 1st time trigger
    JPGParam.RawImageAddr[0] = fmInfo.OutputAddr1Y;            //Raw Image address Y
    JPGParam.RawImageAddr[1] = fmInfo.OutputAddr1Cb;           //Raw Image address CB
    JPGParam.RawImageAddr[2] = fmInfo.OutputAddr1Cr;           //Raw Image address CR
    JPGParam.JPGBSAddr = JpgDAddr;                             //Compress Bitstream location,
    JPG_BSize = GetJpgBufSize();                                     //assume 3 M buffer size..
    JPGParam.JPGBSBufSize = &JPG_BSize;
    JPGParam.Width = fmInfo.ImeHsize1;
    JPGParam.Height =fmInfo.ImeVsize1;
    JPGParam.Mode  = JPEG_FORMAT_422;
    JPGParam.bOuputJpgDC = FALSE;
    JPGParam.LineOffsetY = JPGParam.Width;
    JPGParam.LineOffsetCbCr = JPGParam.Width/2;
    ER_Code = JpegEncOnePic(&JPGParam, JpgQuality);
    *JpgSize = JPG_BSize;

    //QV
    //dest
    fmInfo.ImeOutPath = IMEALG_1PATH;
    fmInfo.OutputAddr1Y = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    fmInfo.OutputAddr1Cb = fmInfo.OutputAddr1Y + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize));
    fmInfo.OutputAddr1Cr = fmInfo.OutputAddr1Y + (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 3 / 2);
    fmInfo.ImeHsize1 = GetIPPSizeInfor(_QVPreHsize);
    fmInfo.ImeVsize1 = GetIPPSizeInfor(_QVPreVsize);
    fmInfo.ImeHofst1 = fmInfo.ImeHsize1;
    fmInfo.ImeFmt1 = IME_OUT_FMT_422_COSITED;

    if (gImageAlgInfo.ImageRatio != _Ratio_4_3)
    {
        if ( gImageAlgInfo.ImageRatio == _Ratio_1_1 )
        {
            //get adjust ratio
            fmInfo.ImeHsize1 = Adj_HSize(fmInfo.ImeHsize1, Get_HRatio(gImageAlgInfo.ImageRatio, _Ratio_4_3), 16);
            debug_err(("fmInfo.ImeHsize1 = %d, GetIPPSizeInfor(_QVPreHsize) = %d\r\n", fmInfo.ImeHsize1, GetIPPSizeInfor(_QVPreHsize)));
            memset((void *)fmInfo.OutputAddr1Y, 0x0, (GetIPPSizeInfor(_QVPreHsize) * fmInfo.ImeVsize1));
            memset((void *)fmInfo.OutputAddr1Cb, 0x80, ((GetIPPSizeInfor(_QVPreHsize) / 2) * fmInfo.ImeVsize1));
            memset((void *)fmInfo.OutputAddr1Cr, 0x80, ((GetIPPSizeInfor(_QVPreHsize) / 2) * fmInfo.ImeVsize1));
            fmInfo.OutputAddr1Y += ((GetIPPSizeInfor(_QVPreHsize) - fmInfo.ImeHsize1) >> 1);
            fmInfo.OutputAddr1Cb += (((GetIPPSizeInfor(_QVPreHsize) - fmInfo.ImeHsize1) >> 1) >> 1);
            fmInfo.OutputAddr1Cr += (((GetIPPSizeInfor(_QVPreHsize) - fmInfo.ImeHsize1) >> 1) >> 1);
        }
        else
        {
            //get adjust ratio
            fmInfo.ImeVsize1 = Adj_VSize(fmInfo.ImeVsize1, Get_VRatio(gImageAlgInfo.ImageRatio, _Ratio_4_3), 4);
            BlackV = (GetIPPSizeInfor(_QVPreVsize) - fmInfo.ImeVsize1) >> 1;
            //upper
            memset((void *)fmInfo.OutputAddr1Y, 0x0, (fmInfo.ImeHsize1 * BlackV));
            memset((void *)fmInfo.OutputAddr1Cb, 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));
            memset((void *)fmInfo.OutputAddr1Cr, 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));
            //lower
            memset((void *)(fmInfo.OutputAddr1Y + (fmInfo.ImeHsize1 * (BlackV + fmInfo.ImeVsize1))), 0x0, (fmInfo.ImeHsize1 * BlackV));
            memset((void *)(fmInfo.OutputAddr1Cb + ((fmInfo.ImeHsize1 / 2) * (BlackV + fmInfo.ImeVsize1))), 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));
            memset((void *)(fmInfo.OutputAddr1Cr + ((fmInfo.ImeHsize1 / 2) * (BlackV + fmInfo.ImeVsize1))), 0x80, ((fmInfo.ImeHsize1 / 2) * BlackV));

            fmInfo.OutputAddr1Y += (((GetIPPSizeInfor(_QVPreVsize) - fmInfo.ImeVsize1) >> 1) * fmInfo.ImeHsize1);
            fmInfo.OutputAddr1Cb += ((((GetIPPSizeInfor(_QVPreVsize) - fmInfo.ImeVsize1) >> 1) * fmInfo.ImeHsize1) >> 1);
            fmInfo.OutputAddr1Cr += ((((GetIPPSizeInfor(_QVPreVsize) - fmInfo.ImeVsize1) >> 1) * fmInfo.ImeHsize1) >> 1);
        }
    }

    ipe_ime_Framemode(fmInfo);
    return E_OK;
}


ER SliceMode_Process(UINT BayerAddr, UINT JpgDAddr, UINT8 JpgQuality, UINT *JpgSize)
{
    UINT JpgYAddr, JpgCbAddr, JpgCrAddr, CounterMax;
    UINT IPEOutForm,IPECFAPat;
    UINT SliceAddr,P2Addr;
    UINT Ime_Counter, SliceHeight, ProcHeight;
    UINT32  ChrFiltIdx;
    UINT32 uiImeStatus, uiJpgStatus;
    UINT32 DataStampVStartPxl, DataStampHStartPxl;
    UINT32 DataStampVDoBlk, DataStampVDoBlkStartPxl;
    UINT32 DataStampVRemain, DataStampVPartialSize;
    UINT32 DataStampXPos, DataStampYPos;
    UINT32 BlackV;
    DS_STAMP_INFOR dsStamp, dsStampTmp;
    DS_BACKGROUND_INFOR dsBackgroundTmp;
    STR_STRIPE_INFOR ImeHstipe,ImeVstipe;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;
    UINT32 ipe_gamma_en;
    //#NT#2010/05/28#ethanlau -begin
    //#NT#add for 632 chromafilter
    IME_FILT_PARA  *Ime_Filter;
    Ime_Filter = Get_ImeFilterInfo();
    //#NT#2010/05/28#ethanlau -end

    IPEOpenObj.FP_IPEISR_CB = NULL;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    ipe_open(&IPEOpenObj);
    ime_open(&IMEOpenObj);

    uiImeStatus = 0;
    uiJpgStatus = 0;
    //reset ime & jpg

    //calculate slice height & slice number
    Ime_Counter = 0;
    SliceHeight = ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode));
    CounterMax=(GetIPPSizeInfor(_ImeCapOutVsize)+SliceHeight-1)/SliceHeight;

    //initial datastamp parameters
    datastamp_GetStampInfor(&dsStamp);
    datastamp_GetPosition(&DataStampXPos, &DataStampYPos);
    DataStampHStartPxl = (GetIPPSizeInfor(_ImeCapOutHsize)*DataStampXPos)>>StampStartRatioShift;
    DataStampHStartPxl &= 0xfffffff8;//needed to be word-aligned for Cb,Cr
    DataStampVStartPxl = (GetIPPSizeInfor(_ImeCapOutVsize)*DataStampYPos)>>StampStartRatioShift;
    DataStampVDoBlk = (DataStampVStartPxl)/SliceHeight;
    DataStampVRemain = dsStamp.Height;
    DataStampVDoBlkStartPxl = DataStampVStartPxl%SliceHeight;

    //initial slice parameters
    SliceAddr=ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF);
    GetIMEJPGAddr(SliceAddr,&IMEJPGAddrP1,SliceHeight,GetIPPSizeInfor(_ImeCapOutHsize),IMEALG_OUTYUV422_COS,3);
    IMEJPGAddrP1.in_h=GetIPPSizeInfor(_PreCropCapHsize)-16;
    IMEJPGAddrP1.in_v=GetIPPSizeInfor(_PreCropCapVsize);
    IMEJPGAddrP1.out_h=GetIPPSizeInfor(_ImeCapOutHsize);
    IMEJPGAddrP1.out_v=GetIPPSizeInfor(_ImeCapOutVsize);
    IMEJPGAddrP1.in_format=IMEALG_INYUV422;
    IMEJPGAddrP1.out_format = IMEALG_OUTYUV422_COS;
    IMEJPGAddrP1.out_lineoffset=GetIPPSizeInfor(_ImeCapOutHsize);
    //#NT#2009/08/12#Jarkko Chang -begin
    //#NT#add for cb/cr separate
    IMEJPGAddrP1.in_YCC_pack = IMEALG_YCBCR_PLANAR;
    IMEJPGAddrP1.out_YCC_pack = IMEALG_YCBCR_PLANAR;
    //#NT#2009/08/12#Jarkko Chang -end
    IMEJPGAddrP2.in_h=GetIPPSizeInfor(_PreCropCapHsize)-16;
    IMEJPGAddrP2.in_v=GetIPPSizeInfor(_PreCropCapVsize);
    IMEJPGAddrP2.out_h=GetIPPSizeInfor(_QVPreHsize);
    IMEJPGAddrP2.out_v=GetIPPSizeInfor(_QVPreVsize);
    IMEJPGAddrP2.in_format=IMEALG_INYUV422;
    IMEJPGAddrP2.out_format = IMEALG_OUTYUV422_COS;
    IMEJPGAddrP2.out_lineoffset=IMEJPGAddrP2.out_h;
    //#NT#2009/08/12#Jarkko Chang -begin
    //#NT#add for cb/cr separate
    IMEJPGAddrP2.in_YCC_pack = IMEALG_YCBCR_PLANAR;
    IMEJPGAddrP2.out_YCC_pack = IMEALG_YCBCR_PLANAR;
    //#NT#2009/08/12#Jarkko Chang -end
    P2Addr = ippbuf_GetBuf(IPPBUF_CAP_QV_PRE);
    GetIMEJPGAddr(P2Addr,&IMEJPGAddrP2,IMEJPGAddrP2.out_v,IMEJPGAddrP2.out_h,IMEALG_OUTYUV422_COS,1);

    if (gImageAlgInfo.ImageRatio != _Ratio_4_3)
    {
        if ( gImageAlgInfo.ImageRatio == _Ratio_1_1 )
        {
            //get adjust ratio
            IMEJPGAddrP2.out_h = Adj_HSize(IMEJPGAddrP2.out_h, Get_HRatio(gImageAlgInfo.ImageRatio, _Ratio_4_3), 16);

            memset((void *)IMEJPGAddrP2.y0_addr, 0x0, (GetIPPSizeInfor(_QVPreHsize) * IMEJPGAddrP2.out_v));
            memset((void *)IMEJPGAddrP2.cb0_addr, 0x80, ((GetIPPSizeInfor(_QVPreHsize) / 2) * IMEJPGAddrP2.out_v));
            memset((void *)IMEJPGAddrP2.cr0_addr, 0x80, ((GetIPPSizeInfor(_QVPreHsize) / 2) * IMEJPGAddrP2.out_v));

            IMEJPGAddrP2.y0_addr += (((GetIPPSizeInfor(_QVPreHsize) - IMEJPGAddrP2.out_h) >> 1));
            IMEJPGAddrP2.cb0_addr += ((((GetIPPSizeInfor(_QVPreHsize) - IMEJPGAddrP2.out_h) >> 1)) >> 1);
            IMEJPGAddrP2.cr0_addr += ((((GetIPPSizeInfor(_QVPreHsize) - IMEJPGAddrP2.out_h) >> 1)) >> 1);
        }
        else
        {
            //get adjust ratio
            IMEJPGAddrP2.out_v = Adj_VSize(IMEJPGAddrP2.out_v, Get_VRatio(gImageAlgInfo.ImageRatio, _Ratio_4_3), 4);
            BlackV = (GetIPPSizeInfor(_QVPreVsize) - IMEJPGAddrP2.out_v) >> 1;
            //upper
            memset((void *)IMEJPGAddrP2.y0_addr, 0x0, (IMEJPGAddrP2.out_h * BlackV));
            memset((void *)IMEJPGAddrP2.cb0_addr, 0x80, ((IMEJPGAddrP2.out_h / 2) * BlackV));
            memset((void *)IMEJPGAddrP2.cr0_addr, 0x80, ((IMEJPGAddrP2.out_h / 2) * BlackV));
            //lower
            memset((void *)(IMEJPGAddrP2.y0_addr + (IMEJPGAddrP2.out_h * (BlackV + IMEJPGAddrP2.out_v))), 0x0, (IMEJPGAddrP2.out_h * BlackV));
            memset((void *)(IMEJPGAddrP2.cb0_addr + ((IMEJPGAddrP2.out_h / 2) * (BlackV + IMEJPGAddrP2.out_v))), 0x80, ((IMEJPGAddrP2.out_h / 2) * BlackV));
            memset((void *)(IMEJPGAddrP2.cr0_addr + ((IMEJPGAddrP2.out_h / 2) * (BlackV + IMEJPGAddrP2.out_v))), 0x80, ((IMEJPGAddrP2.out_h / 2) * BlackV));

            IMEJPGAddrP2.y0_addr += (((GetIPPSizeInfor(_QVPreVsize) - IMEJPGAddrP2.out_v) >> 1) * IMEJPGAddrP2.out_h);
            IMEJPGAddrP2.cb0_addr += ((((GetIPPSizeInfor(_QVPreVsize) - IMEJPGAddrP2.out_v) >> 1) * IMEJPGAddrP2.out_h) >> 1);
            IMEJPGAddrP2.cr0_addr += ((((GetIPPSizeInfor(_QVPreVsize) - IMEJPGAddrP2.out_v) >> 1) * IMEJPGAddrP2.out_h) >> 1);
        }
    }

    ime_SliceCfg(&IMEJPGAddrP1,&IMEJPGAddrP2, CAPTURE_JOB);

    IPEOutForm=IPE_YCC422;
    Get_CapInfo(Cap_CFAPat, &IPECFAPat);
    ChrFiltIdx = GetIPPFuncInfor(CAP_ISO);
    ChromaPara = (UINT(*)[5])Get_ChromaInfo();
    //#NT#2010/05/28#ethanlau -begin
    //#NT#no use in 632
    SetIPPFuncInfor(CHRFILT_NUM, ChromaPara[ChrFiltIdx][4]);
    //#NT#2010/05/28#ethanlau -end
//#NT#2009/06/30#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
    ime_to_ipe_Stripe_trans(&ImeHstipe,&ImeVstipe);
//#NT#2009/06/30#Jarkko Chang -end
    ipe_SliceCfg(&ImeHstipe,&ImeVstipe,BayerAddr,IPECFAPat,IPEOutForm, CAPTURE_JOB);
    ime_setStart();
    //TrgImeSlice();
    ime_triggerSlice();
    ipe_start();

//#NT#2009/07/31#Jarkko Chang -begin
//#NT#add for jpg using
    jpeg_setSliceGoDisable();
//#NT#2009/07/31#Jarkko Chang -end

    jpg_SliceCfg(IMEJPGAddrP1.y0_addr,IMEJPGAddrP1.cb0_addr,IMEJPGAddrP1.cr0_addr,
                GetIPPSizeInfor(_ImeCapOutHsize),GetIPPSizeInfor(_ImeCapOutVsize),JpgDAddr,GetJpgBufSize(),JpgQuality);
    for (Ime_Counter = 0; Ime_Counter < CounterMax; Ime_Counter ++)
    {
        /************* step 1 wait ime slice end & get address *************/
        while(1)
        {
            uiImeStatus = ime_waitDone();
            if (Ime_Counter == (CounterMax - 1))
            {
                if (uiImeStatus & Ime_INT_FRM)
                {
                    break;
                }
            }
            else
            {
                if (uiImeStatus & Ime_INT_SLICE_DN1)
                {
                    break;
                }
            }
        }

        //get ime output address
        switch(Ime_Counter % SLICE_BUF_NUM)
        {
            case 0:
                JpgYAddr=IMEJPGAddrP1.y0_addr;JpgCbAddr=IMEJPGAddrP1.cb0_addr;JpgCrAddr=IMEJPGAddrP1.cr0_addr;
                break;
            case 1:
                JpgYAddr=IMEJPGAddrP1.y1_addr;JpgCbAddr=IMEJPGAddrP1.cb1_addr;JpgCrAddr=IMEJPGAddrP1.cr1_addr;
                break;
            case 2:
                JpgYAddr=IMEJPGAddrP1.y2_addr;JpgCbAddr=IMEJPGAddrP1.cb2_addr;JpgCrAddr=IMEJPGAddrP1.cr2_addr;
                break;
            default:
                JpgYAddr = JpgCbAddr = JpgCrAddr = 0;
                break;
        }
        ProcHeight = SliceHeight;
        if ((Ime_Counter * Ime_Counter) > GetIPPSizeInfor(_ImeCapOutVsize))
        {
            ProcHeight = (Ime_Counter * Ime_Counter) - GetIPPSizeInfor(_ImeCapOutVsize);
        }

        /************* step 2 post processer *************/
        //chroma filter
        if(GetIPPFuncInfor(CHRFILT_EN))
        {
            //#NT#2010/05/28#ethanlau -begin
            ChromaFilter(JpgCbAddr, JpgCrAddr, ippbuf_GetBuf(IPPBUF_CAP_CHROMA_FILTER),
                        (IMEJPGAddrP1.out_h/2), (IMEJPGAddrP1.out_h/2), ProcHeight, ChromaPara[ChrFiltIdx],&(Ime_Filter[ChrFiltIdx]));
            //#NT#2010/05/28#ethanlau -end
        }

        //chroma curve
        if(GetIPPFuncInfor(CHRCURVE_EN))
        {
            //#NT#20091126#ethanlau -begin
            if(IPEModeInfo.IQINFO.FuncSel & IPE_PREGAMMA_EN)
                ipe_gamma_en = ENABLE;
            else
                ipe_gamma_en = DISABLE;
            //#NT#20091126#ethanlau -end
            ChromaCurve(IMEJPGAddrP1.out_h, ProcHeight, JpgCbAddr, JpgCbAddr,ipe_gamma_en ,
                        (UINT32 *)GetIPPFuncInfor(CHRCBCURVE), (UINT32 *)GetIPPFuncInfor(CAPGAMMACURVE),
                        (UINT32 *)GetIPPFuncInfor(CAPGAMMACURVE), (UINT32 *)GetIPPFuncInfor(CAPGAMMACURVE));
            ChromaCurve(IMEJPGAddrP1.out_h, ProcHeight, JpgCrAddr, JpgCrAddr, ipe_gamma_en,
                        (UINT32 *)GetIPPFuncInfor(CHRCRCURVE), (UINT32 *)GetIPPFuncInfor(CAPGAMMACURVE),
                        (UINT32 *)GetIPPFuncInfor(CAPGAMMACURVE), (UINT32 *)GetIPPFuncInfor(CAPGAMMACURVE));
        }
        //datastamp
        if(Ime_Counter==DataStampVDoBlk && datastamp_IsEnable())
        {
            if (DataStampVRemain>(SliceHeight-DataStampVDoBlkStartPxl))
            {
                DataStampVPartialSize = (SliceHeight-DataStampVDoBlkStartPxl);
            }
            else
                DataStampVPartialSize = DataStampVRemain;

            dsBackgroundTmp.AddrY  = JpgYAddr+(GetIPPSizeInfor(_ImeCapOutHsize)*DataStampVDoBlkStartPxl+DataStampHStartPxl);
            dsBackgroundTmp.AddrCb = JpgCbAddr+(GetIPPSizeInfor(_ImeCapOutHsize)*DataStampVDoBlkStartPxl+DataStampHStartPxl)/2;
            dsBackgroundTmp.AddrCr = JpgCrAddr+(GetIPPSizeInfor(_ImeCapOutHsize)*DataStampVDoBlkStartPxl+DataStampHStartPxl)/2;
            dsBackgroundTmp.LineOffset = GetIPPSizeInfor(_ImeCapOutHsize);

            dsStampTmp.AddrY    = dsStamp.AddrY+((dsStamp.Height-DataStampVRemain)*dsStamp.Width);
            dsStampTmp.AddrCb   = dsStamp.AddrCb+((dsStamp.Height-DataStampVRemain)*dsStamp.Width/2);
            dsStampTmp.AddrCr   = dsStamp.AddrCr+((dsStamp.Height-DataStampVRemain)*dsStamp.Width/2);
            dsStampTmp.LineOffset   = dsStamp.LineOffset;
            dsStampTmp.Height   = DataStampVPartialSize;
            dsStampTmp.Width    = dsStamp.Width;
            dsStampTmp.ColorKeyY    = dsStamp.ColorKeyY;
            dsStampTmp.ColorKeyCb   = dsStamp.ColorKeyCb;
            dsStampTmp.ColorKeyCr   = dsStamp.ColorKeyCr;

            datastamp_ImprintYCC(   &dsBackgroundTmp,
                                    &dsStampTmp,
                                    ippbuf_GetBuf(IPPBUF_CAP_DATA_STAMP));

            DataStampVRemain -= DataStampVPartialSize;
            if(DataStampVRemain)
            {
                DataStampVDoBlkStartPxl = 0;
                DataStampVDoBlk ++;
            }
        }

        /************* step 3 encode to jpeg & trigger next slice *************/
        if (Ime_Counter == 0)
        {
            //jpeg slice start
            jpeg_setSliceEncAddr(ProcHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
            jpeg_setStartEncode();
            //TrgImeSlice();
            ime_triggerSlice();
        }
        else if (Ime_Counter == (CounterMax - 1))
        {
            //jpeg slice end
            while(1)
            {
                uiJpgStatus = jpeg_waitDone();
                if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                {
                    break;
                }
            }
            if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
            {
                ipe_pause();
                ipe_close();
                ime_setStop();
                ime_close();
                jpeg_setEndEncode();
                jpeg_close();
                return E_BOVR;
            }
            jpeg_setSliceEncAddr(0,JpgYAddr,JpgCbAddr,JpgCrAddr);
        }
        else
        {
            //jpeg slice
            while(1)
            {
                uiJpgStatus = jpeg_waitDone();
                if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                {
                    break;
                }
            }
            if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
            {
                ipe_pause();
                ipe_close();
                ime_setStop();
                ime_close();
                jpeg_setEndEncode();
                jpeg_close();
                return E_BOVR;
            }
            jpeg_setSliceEncAddr(ProcHeight,JpgYAddr,JpgCbAddr,JpgCrAddr);
            //TrgImeSlice();
            ime_triggerSlice();
        }
    }

    ipe_pause();
    ipe_close();
    ime_setStop();
    ime_close();

    //wait jpeg encode finish
    while(1)
    {
        uiJpgStatus = jpeg_waitDone();
        if (uiJpgStatus & (_JPEG_Frame_End | _JPEG_Buf_End))
        {
            break;
        }
    }
    if (SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
    {
        jpeg_setEndEncode();
        jpeg_close();
        return E_BOVR;
    }
    jpeg_setEndEncode();
    jpeg_close();
    *JpgSize = jpeg_getComSize() +  JPGHeaderSize;
    return E_OK;
}

UINT8 Sen_GetJpgQuality(void)
{
    return JPEG_Quality[gImageAlgInfo.Quality];
}

UINT32 Sen_GetBurstNum(void)
{
    return CapBufAlloc[gImageAlgInfo.BurstDriveNum][2];
}

UINT32 Sen_GetRawNum(void)
{
    return CapBufAlloc[gImageAlgInfo.BurstDriveNum][0];
}

UINT32 Sen_GetJpgNum(void)
{
    return CapBufAlloc[gImageAlgInfo.BurstDriveNum][1];
}

void ipe_ime_Framemode(IPE_IME_FM_PARAM fmInfo)
{
    UINT IPEOutForm, IPECFAPat;
    STR_STRIPE_INFOR ImeHstipe,ImeVstipe;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;

    IPEOpenObj.FP_IPEISR_CB = NULL;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    ipe_open(&IPEOpenObj);
    ime_open(&IMEOpenObj);

#if 0
    pll_enableSystemReset(IME_RSTN);
    pll_disableSystemReset(IME_RSTN);
#endif

    //Get address and set IME for path1 and path2
    GetIMEJPGAddr(fmInfo.OutputAddr1Y,&IMEJPGAddrP1,fmInfo.ImeVsize1,fmInfo.ImeHsize1,1,1);
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
        IMEJPGAddrP1.in_h=(fmInfo.BayerHsize>>1);
    else
        IMEJPGAddrP1.in_h=fmInfo.BayerHsize-16;
    IMEJPGAddrP1.in_v=fmInfo.BayerVsize;
    IMEJPGAddrP1.out_h=fmInfo.ImeHsize1;
    IMEJPGAddrP1.out_v=fmInfo.ImeVsize1;
    IMEJPGAddrP1.in_format=1;    //422
    IMEJPGAddrP1.out_lineoffset=fmInfo.ImeHofst1; //fmInfo.ImeHsize1;

    GetIMEJPGAddr(fmInfo.OutputAddr2Y,&IMEJPGAddrP2,fmInfo.ImeVsize2,fmInfo.ImeHsize2,1,1);
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
        IMEJPGAddrP2.in_h=(fmInfo.BayerHsize>>1);
    else
        IMEJPGAddrP2.in_h=fmInfo.BayerHsize-16;
    IMEJPGAddrP2.in_v=fmInfo.BayerVsize;
    IMEJPGAddrP2.out_h=fmInfo.ImeHsize2;
    IMEJPGAddrP2.out_v=fmInfo.ImeVsize2;
    IMEJPGAddrP2.in_format=1;    //422
    IMEJPGAddrP2.out_lineoffset=fmInfo.ImeHofst2;//fmInfo.ImeHsize2;

    IMEJPGAddrP1.y0_addr = fmInfo.OutputAddr1Y;
    IMEJPGAddrP1.cb0_addr = fmInfo.OutputAddr1Cb;
    IMEJPGAddrP1.cr0_addr = fmInfo.OutputAddr1Cr;

    if (fmInfo.ImeOutPath == IMEALG_2PATH)
    {
        IMEJPGAddrP2.y0_addr = fmInfo.OutputAddr2Y;
        IMEJPGAddrP2.cb0_addr = fmInfo.OutputAddr2Cb;
        IMEJPGAddrP2.cr0_addr = fmInfo.OutputAddr2Cr;
        ime_SliceCfg(&IMEJPGAddrP1,&IMEJPGAddrP2, IPE_IME_FRAME_2PATH_JOB);
    }
    else
    {
        if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
        {
            ime_SliceCfg(&IMEJPGAddrP1,&IMEJPGAddrP2, IPE_IME_FRAME_DVI_JOB);
        }
        else
        {
            ime_SliceCfg(&IMEJPGAddrP1,&IMEJPGAddrP2, IPE_IME_FRAME_1PATH_JOB);
        }
    }
    ime_Slice_Mode1_En(DISABLE);
    ime_Slice_Mode2_En(DISABLE);

    IPEOutForm=IPE_YCC422;
    IPECFAPat=CFAPAT_CAP;
//#NT#2009/06/30#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
    ime_to_ipe_Stripe_trans(&ImeHstipe,&ImeVstipe);

//#NT#2009/06/30#Jarkko Chang -end
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        ipe_SliceCfg(&ImeHstipe,&ImeVstipe,fmInfo.BayerAddr,IPECFAPat,IPEOutForm, IPE_IME_FRAME_DVI_JOB);
    }
    else
    {
        ipe_SliceCfg(&ImeHstipe,&ImeVstipe,fmInfo.BayerAddr,IPECFAPat,IPEOutForm, IPE_IME_FRAME_1PATH_JOB);
    }

    ime_clrIntFlag();
    ime_setStart();
    ipe_start();
    ime_waitFlagFrameEnd();

    ipe_pause();
    ipe_close();
    ime_setStop();
    ime_close();
}

