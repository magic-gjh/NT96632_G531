#include    "sensor_model.h"
#include    "interrupt.h"


/*******************************************
Note :
    a.  HD_Division is not equal to job_req->hd_DivisionNum.Because Hardware Register settings is HD_Division-1
    HD_Division is used for calculation, but job_req->hd_DivisionNum is used for register settings.

    b.  SIE  Act_width & Act_height description and PRE crop parameter descriptions are below drawings.

    Horizontal (HD sognal) :
             __________________________________________   ____________________________________________
           _/                                          \_/                                            \

                ^              ^
                |              |
        Act_startX          CenterX

                |<--------------------------->|  (Act_width = HD_Total_Prv - ACT_STRX_Prv

                ^
                |__
                    Pre cropping start pos( Pre_hcrop_ini = 0 )

                  |<--------------->| ( Pre_hcropping_size = RawHsize)

                |<--->| ( Pre_hcrop_ini = (CenterX - Act_startX)-Pre_hcropping_size/2

----------------------------------------------------------------------------------------------------------

        Vertical (HD sognal) :  SensorCapTotalFld = N , RawSize = RawHsize*RawVsize
             __________   __________   __________                         __________   __________   __________
           _/          \_/          \_/          \        ~~~~~         _/          \_/          \_/


                          ^                                    ^
                          |                                    |
                        Act_startY                           CenterY                (Act_height = VD_Total_Prv - ACT_STRY_Prv - LAST_ONE_LINE_OMITT

                              |<------------------------------------------------------------------>|

                          ^
                          |__
                              Pre cropping start( Pre_vcrop_ini = 0 )

                           |<--------------------------------------------->| (Pre_vcropping_size = RawVsize/N )


                            |<---------->| ( Pre_vcrop_ini = (CenterY - Act_startY)- Pre_vcropping_size/2


*********************************************/
IME_FBADDR_PRAM IME_D1_Para,IME_D2_Para;
SIE_PRE_PARAM SIE_PRE_Para_prv;
SIE_PRE_PARAM SIE_PRE_Para_cap;
SIE_PRE_PARAM SIE_PRE_Para_d2d;
IPE_PARAM IPE_Para;
IPP_PARAM Ipp_Param;
STR_SIE_REG SIE_Param;
STR_PRE_REG PRE_Param;
IPE_MODEINFO IPEModeInfo;
IME_MODE_PRAM IMEParam;
//#NT#2009/06/25#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
STR_IME_STRIPE_INFOR    GetStripe;
STR_STRIPE_INFOR    IpeHstipe,IpeVstipe;
//#NT#2009/06/25#Jarkko Chang -end

struct PUBLIC_STR_JOBREQS  IPPMgrReq;


int Sensor_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if (JobType == PREVIEW_JOB)
    {
        SIE_Param.REG_HDVD_Sync.sie_hd_sync = job_req->SIE_PRE_PRV->HDSync;
        SIE_Param.REG_HDVD_Sync.sie_hd_total = job_req->SIE_PRE_PRV->HDTotal;
        SIE_Param.REG_HDVD_Sync.sie_vd_sync = job_req->SIE_PRE_PRV->VDSync;
        SIE_Param.REG_HDVD_Sync.sie_vd_total = job_req->SIE_PRE_PRV->VDTotal;
        SIE_Param.REG_sie_FieldAct.VH_Act = job_req->SIE_PRE_PRV->HDActX;
        SIE_Param.REG_sie_FieldAct.VV_Act1 = job_req->SIE_PRE_PRV->HDActY[0];
        SIE_Param.REG_RemapFieldNum = job_req->SIE_PRE_PRV->SieFieldNum;
    }
    else if (JobType == CAPTURE_JOB)
    {
        SIE_Param.REG_RemapFieldNum = job_req->SIE_PRE_CAP->SieFieldNum;
        PRE_Param.REG_RemapFieldNum = SIE_Param.REG_RemapFieldNum;

        SIE_Param.REG_sie_FieldAct.VH_Act = job_req->SIE_PRE_CAP->HDActX;
        SIE_Param.REG_sie_FieldAct.VV_Act1 = job_req->SIE_PRE_CAP->HDActY[0];
        SIE_Param.REG_sie_FieldAct.VV_Act2 = job_req->SIE_PRE_CAP->HDActY[1];
        SIE_Param.REG_sie_FieldAct.VV_Act3 = job_req->SIE_PRE_CAP->HDActY[2];
        SIE_Param.REG_sie_FieldAct.VV_Act4 = job_req->SIE_PRE_CAP->HDActY[3];
        SIE_Param.REG_sie_FieldAct.VV_Act5 = job_req->SIE_PRE_CAP->HDActY[4];
        SIE_Param.REG_sie_FieldAct.VV_Act6 = job_req->SIE_PRE_CAP->HDActY[5];
        SIE_Param.REG_sie_FieldAct.VV_Act7 = job_req->SIE_PRE_CAP->HDActY[6];
        SIE_Param.REG_sie_FieldAct.VV_Act8 = job_req->SIE_PRE_CAP->HDActY[7];
        SIE_Param.REG_HDVD_Sync.sie_hd_sync = job_req->SIE_PRE_CAP->HDSync;
        SIE_Param.REG_HDVD_Sync.sie_hd_total = job_req->SIE_PRE_CAP->HDTotal;
        SIE_Param.REG_HDVD_Sync.sie_vd_sync = job_req->SIE_PRE_CAP->VDSync;
        SIE_Param.REG_HDVD_Sync.sie_vd_total = job_req->SIE_PRE_CAP->VDTotal;

     }else if (JobType == PRE_DRAM_RAWIMGIN_JOB) {
    }else {
        return IPP_FAIL;
     }

    return IPP_OK;

}

int Sensor_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int Sensor_ExecIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int Sensor_EndIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int SIE_BeginIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType) {

    UINT32 Sie_PreHsize,Sie_PreVsize;

    if (JobType == PREVIEW_JOB)
    {
        Sie_PreHsize = SIE_Param.REG_HDVD_Sync.sie_hd_total - SIE_Param.REG_sie_FieldAct.VH_Act;
        Sie_PreHsize = (Sie_PreHsize / 4) * 4;
        Sie_PreVsize = SIE_Param.REG_HDVD_Sync.sie_vd_total - SIE_Param.REG_sie_FieldAct.VV_Act1 - LAST_ONE_LINE_OMITT;
        SIE_Param.REG_Sie_OutSize.Hsize = Sie_PreHsize;
        SIE_Param.REG_Sie_OutSize.Vsize = Sie_PreVsize;


        if (job_req->SIE_PRE_PRV->SieOutDest == OUT_PRE_DEST)
        {
            SIE_Param.REG_sie_output_dest = OUT_PRE_DEST;
        }
        else if (job_req->SIE_PRE_PRV->SieOutDest == OUT_DRAM_DEST)
        {
            SIE_Param.REG_sie_output_dest = OUT_DRAM_DEST;
            SIE_Param.REG_sie_pre_pack_bus = job_req->SIE_PRE_PRV->SieBitDepth;
            SIE_Param.REG_siepre_ppb0_addr = job_req->SIE_PRE_PRV->SieOutAdd0;

            if (job_req->SIE_PRE_PRV->SieBitDepth == SIE_OUT_PACK_8BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize;
            }
            else if (job_req->SIE_PRE_PRV->SieBitDepth == SIE_OUT_PACK_12BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = (Sie_PreHsize*12)/8;
            }
            else if (job_req->SIE_PRE_PRV->SieBitDepth == SIE_OUT_PACK_16BIT )
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize*2;
            }
            else
            {
                debug_err(("ERROR\r\n"));
            }

            job_req->SIE_PRE_PRV->SieOutLineOfs = SIE_Param.REG_Sie_OutSize.lineoft;
            job_req->SIE_PRE_PRV->SieOutSizeV = Sie_PreVsize;
            SIE_Param.REG_Sie_OutSize.Vsize = Sie_PreVsize;

        }else if (job_req->SIE_PRE_PRV->SieOutDest == OUT_PINGPONGBUF_DEST)
        {
            SIE_Param.REG_sie_output_dest = OUT_PINGPONGBUF_DEST;
            SIE_Param.REG_sie_pre_pack_bus = job_req->SIE_PRE_PRV->SieBitDepth;
            SIE_Param.REG_siepre_ppb0_addr = job_req->SIE_PRE_PRV->SieOutAdd0;
            SIE_Param.REG_siepre_ppb1_addr = job_req->SIE_PRE_PRV->SieOutAdd1;

            if (job_req->SIE_PRE_PRV->SieBitDepth == SIE_OUT_PACK_8BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize;
            }
            else if (job_req->SIE_PRE_PRV->SieBitDepth == SIE_OUT_PACK_12BIT )
            {
                SIE_Param.REG_Sie_OutSize.lineoft = (Sie_PreHsize*12)/8;
            }
            else if (job_req->SIE_PRE_PRV->SieBitDepth == SIE_OUT_PACK_16BIT )
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize*2;
            }
            else
            {
                debug_err(("ERROR\r\n"));
            }

            job_req->SIE_PRE_PRV->SieOutLineOfs = SIE_Param.REG_Sie_OutSize.lineoft;
            job_req->SIE_PRE_PRV->SieOutSizeV = Sie_PreVsize;
            SIE_Param.REG_Sie_OutSize.Vsize = Sie_PreVsize;
        }

        SIE_Param.AutoObEn = job_req->SIE_PRE_PRV->AutoObEn;
        SIE_Param.REG_sie_OB_Param = job_req->SIE_PRE_PRV->SieObParam;
        SIE_Param.OBGain = job_req->SIE_PRE_PRV->SieObGain;
        SIE_Param.OBOfs = job_req->SIE_PRE_PRV->SieObOfs;

        SIE_Param.REG_sie_intrp_en = job_req->SIE_PRE_PRV->SieIntEn;
        SIE_Param.REG_sie_intrp_bp1 = job_req->SIE_PRE_PRV->SieBp1;
        SIE_Param.REG_sie_intrp_bp2 = job_req->SIE_PRE_PRV->SieBp2;
        SIE_Param.REG_sie_SyncPhase.Master = job_req->SIE_PRE_PRV->SieMaster;
        SIE_Param.REG_sie_SyncPhase.VD_Phase = job_req->SIE_PRE_PRV->SieVDPhase;
        SIE_Param.REG_sie_SyncPhase.HD_Phase = job_req->SIE_PRE_PRV->SieHDPhase;
        SIE_Param.REG_sie_SyncPhase.Data_Phase= job_req->SIE_PRE_PRV->SieDataPhase;
        SIE_Param.REG_sie_hd_inv = job_req->SIE_PRE_PRV->SieHDInv;
        SIE_Param.REG_sie_vd_inv = job_req->SIE_PRE_PRV->SieVDInv;
    }else if (JobType == CAPTURE_JOB)
    {
        Sie_PreHsize = job_req->SIE_PRE_CAP->HDTotal- job_req->SIE_PRE_CAP->HDActX;
        Sie_PreHsize = ((Sie_PreHsize)/4)*4;
        Sie_PreVsize = job_req->SIE_PRE_CAP->VDTotal- job_req->SIE_PRE_CAP->HDActY[0] - LAST_ONE_LINE_OMITT;

        SIE_Param.REG_Sie_OutSize.Hsize = Sie_PreHsize;
        SIE_Param.REG_Sie_OutSize.Vsize = Sie_PreVsize;
        SIE_Param.REG_sie_intrp_en = job_req->SIE_PRE_CAP->SieIntEn;
        SIE_Param.REG_sie_intrp_bp1 = job_req->SIE_PRE_CAP->SieBp1;
        SIE_Param.REG_sie_intrp_bp2 = job_req->SIE_PRE_CAP->SieBp2;
        SIE_Param.REG_sie_intrp_en = job_req->SIE_PRE_CAP->SieIntEn;
        SIE_Param.REG_sie_vd_inv = job_req->SIE_PRE_CAP->SieVDInv;
        SIE_Param.REG_sie_hd_inv = job_req->SIE_PRE_CAP->SieHDInv;
        if (job_req->SIE_PRE_CAP->SieOutDest == OUT_PRE_DEST)
        {
            SIE_Param.REG_sie_output_dest = OUT_PRE_DEST;
        }
        else if (job_req->SIE_PRE_CAP->SieOutDest == OUT_DRAM_DEST)
        {
            SIE_Param.REG_sie_output_dest = OUT_DRAM_DEST;
            SIE_Param.REG_sie_pre_pack_bus = job_req->SIE_PRE_CAP->SieBitDepth;
            SIE_Param.REG_siepre_ppb0_addr = job_req->SIE_PRE_CAP->SieOutAdd0;

            if (job_req->SIE_PRE_CAP->SieBitDepth == SIE_OUT_PACK_8BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize;
            }
            else if (job_req->SIE_PRE_CAP->SieBitDepth == SIE_OUT_PACK_12BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = (Sie_PreHsize*12)/8;
            }
            else if (job_req->SIE_PRE_CAP->SieBitDepth == SIE_OUT_PACK_16BIT )
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize*2;
            }
            else
            {
                debug_err(("ERROR\r\n"));
            }

            job_req->SIE_PRE_CAP->SieOutLineOfs = SIE_Param.REG_Sie_OutSize.lineoft;
            job_req->SIE_PRE_CAP->SieOutSizeV = Sie_PreVsize;
            SIE_Param.REG_Sie_OutSize.Vsize = Sie_PreVsize;

        }
        else if (job_req->SIE_PRE_CAP->SieOutDest == OUT_PINGPONGBUF_DEST)
        {
            SIE_Param.REG_sie_output_dest = OUT_PINGPONGBUF_DEST;
            SIE_Param.REG_sie_pre_pack_bus = job_req->SIE_PRE_CAP->SieBitDepth;
            SIE_Param.REG_siepre_ppb0_addr = job_req->SIE_PRE_CAP->SieOutAdd0;
            SIE_Param.REG_siepre_ppb1_addr = job_req->SIE_PRE_CAP->SieOutAdd1;


            if (job_req->SIE_PRE_CAP->SieBitDepth == SIE_OUT_PACK_8BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize;
            }
            else if (job_req->SIE_PRE_CAP->SieBitDepth == SIE_OUT_PACK_12BIT)
            {
                SIE_Param.REG_Sie_OutSize.lineoft = (Sie_PreHsize*12)/8;
            }
            else if (job_req->SIE_PRE_CAP->SieBitDepth == SIE_OUT_PACK_16BIT )
            {
                SIE_Param.REG_Sie_OutSize.lineoft = Sie_PreHsize*2;
            }
            else
            {
                debug_err(("ERROR\r\n"));
            }

            job_req->SIE_PRE_CAP->SieOutSizeV = Sie_PreVsize;
            job_req->SIE_PRE_CAP->SieOutLineOfs = SIE_Param.REG_Sie_OutSize.lineoft;
            SIE_Param.REG_siepre_ppb0_addr = job_req->SIE_PRE_CAP->SieOutAdd0;
            SIE_Param.REG_siepre_ppb1_addr = job_req->SIE_PRE_CAP->SieOutAdd1;
            SIE_Param.REG_Sie_OutSize.Vsize = Sie_PreVsize;
        }
        SIE_Param.AutoObEn = job_req->SIE_PRE_CAP->AutoObEn;
        SIE_Param.REG_sie_OB_Param = job_req->SIE_PRE_CAP->SieObParam;
        SIE_Param.OBGain = job_req->SIE_PRE_CAP->SieObGain;
        SIE_Param.OBOfs = job_req->SIE_PRE_CAP->SieObOfs;
    }
    else if (JobType == PRE_DRAM_RAWIMGIN_JOB)
    {
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}


int SIE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType) {


    if (JobType == PREVIEW_JOB )
    {

    }else if (JobType == CAPTURE_JOB ) {


    }else if (JobType == PRE_DRAM_RAWIMGIN_JOB ) {
    }else{

        return IPP_FAIL;
    }

        return IPP_OK;
}

int SIE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{

    if (JobType == PREVIEW_JOB)
    {
        SIE_Param.SIE_Operation = SIE_ENGINE_SET2PRV;
        sie_setMode(&SIE_Param);

    }
    else if (JobType == CAPTURE_JOB)
    {
        SIE_Param.SIE_Operation = SIE_ENGINE_SET2CAP;
        sie_setMode(&SIE_Param);
    }
    else if (JobType == PRE_DRAM_RAWIMGIN_JOB)
    {
    }
    else
    {
        return IPP_FAIL;
    }

    return IPP_OK;
}

int SIE_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return TRUE;
}


int PRE_BeginIPP(struct  PUBLIC_STR_JOBREQS * job_req, JobTypeEnum JobType)
{
    UINT32 AddrOfs;
    UINT32 pre_hcrop_ini, pre_hcrop_width;
    UINT32 pre_vcrop_ini, pre_vcrop_height;

    AddrOfs = 0;
    if (JobType == PREVIEW_JOB)
    {
        PRE_Param.PRE_VGAType = job_req->SIE_PRE_PRV->VGAType;
        PRE_Param.REG_pre_input_pack_bus = job_req->SIE_PRE_PRV->PreInBitDepth;
        PRE_Param.REG_pre_output_pack_bus = job_req->SIE_PRE_PRV->PreBitDepth;
        PRE_Param.REG_pre_intrp_en = job_req->SIE_PRE_PRV->PreIntEn;
        PRE_Param.REG_pre_input_Size.Hsize = SIE_Param.REG_Sie_OutSize.Hsize;
        PRE_Param.REG_pre_input_Size.Vsize = SIE_Param.REG_Sie_OutSize.Vsize;
        PRE_Param.REG_pre_input_Size.lineoft = SIE_Param.REG_Sie_OutSize.lineoft;
        PRE_Param.REG_pre_input_pack_bus = SIE_Param.REG_sie_pre_pack_bus;
        PRE_Param.REG_pre_in_ppb0_addr = SIE_Param.REG_siepre_ppb0_addr;
        PRE_Param.REG_pre_in_ppb1_addr = SIE_Param.REG_siepre_ppb1_addr;
        PRE_Param.REG_pre_input_Size.lineoft=SIE_Param.REG_Sie_OutSize.lineoft;

        PRE_Param.REG_RemapFieldNum = SIE_Param.REG_RemapFieldNum;

        //vertical size
        if (PRE_Param.PRE_VGAType == SENSOR_OUT_TWO_LINE)
        {
            job_req->PRV_INFO.PreOutVsize = job_req->SIE_PRE_PRV->PreCropSizeV * TWO_LINE;
        }
        else if (PRE_Param.PRE_VGAType == SENSOR_OUT_THREE_LINE)
        {
            job_req->PRV_INFO.PreOutVsize = job_req->SIE_PRE_PRV->PreCropSizeV * THREE_LINE;
        }
        else
        {
            job_req->PRV_INFO.PreOutVsize = job_req->SIE_PRE_PRV->PreCropSizeV;
        }

        //horizontal size
        PRE_Param.PRE_EnFunction &= ~PRE_PADDING_EN;
        if (job_req->SIE_PRE_PRV->IsPadding)
        {
            job_req->SIE_PRE_PRV->PreOutSizeH =  job_req->SIE_PRE_PRV->PreOutSizeH - PADDING_REPEATING_PIX;
            PRE_Param.PRE_EnFunction |= PRE_PADDING_EN;

        }

        if(SIE_Param.REG_sie_output_dest == OUT_DRAM_DEST)
            PRE_Param.REG_pre_input_source = PATH_DRAM_INPUT;
        else if(SIE_Param.REG_sie_output_dest == OUT_PRE_DEST)
            PRE_Param.REG_pre_input_source = PATH_SIE_DIRECT_INPUT;
        else if(SIE_Param.REG_sie_output_dest == OUT_PINGPONGBUF_DEST)
            PRE_Param.REG_pre_input_source = PATH_SIE_PINGPONG_INPUT;
        else
            return IPP_FAIL;


        pre_hcrop_width = job_req->SIE_PRE_PRV->PreCropSizeH;
        pre_vcrop_height = job_req->SIE_PRE_PRV->PreCropSizeV;
        pre_hcrop_ini = (job_req->SIE_PRE_PRV->RawCenterX - SIE_Param.REG_sie_FieldAct.VH_Act - pre_hcrop_width / 2);
        pre_vcrop_ini = (job_req->SIE_PRE_PRV->RawCenterY - SIE_Param.REG_sie_FieldAct.VV_Act1 - pre_vcrop_height / 2);

        PRE_Param.REG_pre_hv_width = pre_hcrop_width;
        PRE_Param.REG_pre_hv_ini = pre_hcrop_ini;
        PRE_Param.REG_pre_vv_height = pre_vcrop_height;
        PRE_Param.REG_pre_vv_ini = pre_vcrop_ini;
        PRE_Param.REG_pre_output_size = job_req->SIE_PRE_PRV->PreOutSizeH;
        PRE_Param.REG_pre_output_lineoft = job_req->SIE_PRE_PRV->PreOutLineOfs;
        PRE_Param.REG_pre_out_ppb0_addr = job_req->SIE_PRE_PRV->PreOutAdd0;
        PRE_Param.REG_pre_out_ppb1_addr = job_req->SIE_PRE_PRV->PreOutAdd1;
        PRE_Param.REG_pre_output_path = job_req->SIE_PRE_PRV->PreSrcSel;
        PRE_Param.REG_pre_in_ppb2_addr = job_req->SIE_PRE_PRV->SiePreAddr2;
        PRE_Param.REG_pre_CfaPat = job_req->SIE_PRE_PRV->PreCfaPat;
        PRE_Param.REG_pre_CfaPat2 = job_req->SIE_PRE_PRV->PreCfaPat2;

        AddrOfs = Sen_GetPREAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreOutVsize), PRE_Param.REG_pre_output_lineoft, gImageAlgInfo.PrePrvBitDepth);
        PRE_Param.REG_pre_out_ppb0_addr += AddrOfs;
        PRE_Param.REG_pre_out_ppb1_addr += AddrOfs;
    }
    else if (JobType == CAPTURE_JOB )
    {
        PRE_Param.REG_pre_input_pack_bus = job_req->SIE_PRE_CAP->PreInBitDepth;
        PRE_Param.REG_pre_output_pack_bus = job_req->SIE_PRE_CAP->PreBitDepth;
        PRE_Param.REG_pre_intrp_en = job_req->SIE_PRE_CAP->PreIntEn;
        PRE_Param.REG_pre_input_Size.lineoft = SIE_Param.REG_Sie_OutSize.lineoft;

        if(SIE_Param.REG_sie_output_dest == OUT_DRAM_DEST)
            PRE_Param.REG_pre_input_source = PATH_DRAM_INPUT;
        else if(SIE_Param.REG_sie_output_dest == OUT_PRE_DEST)
            PRE_Param.REG_pre_input_source = PATH_SIE_DIRECT_INPUT;
        else if(SIE_Param.REG_sie_output_dest == OUT_PINGPONGBUF_DEST)
            PRE_Param.REG_pre_input_source = PATH_SIE_PINGPONG_INPUT;
        else
            return IPP_FAIL;

        pre_hcrop_width = job_req->SIE_PRE_CAP->PreCropSizeH;
        pre_vcrop_height = (job_req->SIE_PRE_CAP->PreCropSizeV + job_req->SIE_PRE_CAP->SieFieldNum.TotalFld)/(job_req->SIE_PRE_CAP->SieFieldNum.TotalFld + 1);
        pre_vcrop_height = (pre_vcrop_height + 3) / 4 * 4;
        PRE_Param.REG_pre_input_Size.Hsize = SIE_Param.REG_Sie_OutSize.Hsize;
        PRE_Param.REG_pre_input_Size.Vsize = SIE_Param.REG_Sie_OutSize.Vsize;

        pre_hcrop_ini = (job_req->SIE_PRE_CAP->RawCenterX - job_req->SIE_PRE_CAP->HDActX - pre_hcrop_width/2);
        pre_vcrop_ini = (job_req->SIE_PRE_CAP->RawCenterY - job_req->SIE_PRE_CAP->HDActY[0] - pre_vcrop_height/2);

        PRE_Param.REG_pre_hv_width = pre_hcrop_width;
        PRE_Param.REG_pre_hv_ini = pre_hcrop_ini;

        PRE_Param.REG_pre_vv_height = pre_vcrop_height;
        PRE_Param.REG_pre_vv_ini = pre_vcrop_ini;

        PRE_Param.REG_pre_output_size = pre_hcrop_width;
        PRE_Param.REG_pre_output_lineoft = job_req->SIE_PRE_CAP->PreOutLineOfs;
        PRE_Param.REG_pre_out_ppb0_addr =  job_req->SIE_PRE_CAP->PreOutAdd0;
        PRE_Param.REG_pre_out_ppb1_addr = job_req->SIE_PRE_CAP->PreOutAdd1;
        PRE_Param.REG_pre_output_path = job_req->SIE_PRE_CAP->PreSrcSel;

        PRE_Param.REG_pre_in_ppb0_addr = SIE_Param.REG_siepre_ppb0_addr;
        PRE_Param.REG_pre_in_ppb1_addr = SIE_Param.REG_siepre_ppb1_addr;
        PRE_Param.REG_pre_in_ppb2_addr = job_req->SIE_PRE_CAP->SiePreAddr2;
        PRE_Param.REG_pre_input_pack_bus = SIE_Param.REG_sie_pre_pack_bus;
        PRE_Param.REG_pre_CfaPat = job_req->SIE_PRE_CAP->PreCfaPat;
        PRE_Param.REG_pre_CfaPat2 = job_req->SIE_PRE_CAP->PreCfaPat2;

        AddrOfs = Sen_GetPREAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreCropCapVsize), PRE_Param.REG_pre_output_lineoft, gImageAlgInfo.PreCapBitDepth);
        PRE_Param.REG_pre_out_ppb0_addr += AddrOfs;
        PRE_Param.REG_pre_out_ppb1_addr += AddrOfs;

    }
    else if (JobType == PRE_DRAM_RAWIMGIN_JOB )
    {
        job_req->FunType = job_req->SIE_PRE_D2D->PreFuncEn;
        PRE_Param.PRE_EnFunction =  job_req->SIE_PRE_D2D->PreFuncEn;
        PRE_Param.REG_pre_CfaPat = job_req->SIE_PRE_D2D->PreCfaPat;
        PRE_Param.REG_pre_CfaPat2 = job_req->SIE_PRE_D2D->PreCfaPat2;

        PRE_Param.REG_RemapFieldNum.TotalFld = 1;
        PRE_Param.REG_pre_output_path = job_req->SIE_PRE_D2D->PreSrcSel;

        PRE_Param.REG_pre_input_pack_bus = job_req->SIE_PRE_D2D->PreInBitDepth;
        PRE_Param.REG_pre_input_Size.Hsize = job_req->SIE_PRE_D2D->PreDramInSizeH;
        PRE_Param.REG_pre_input_Size.Vsize = job_req->SIE_PRE_D2D->PreDramInSizeV;
        PRE_Param.REG_pre_input_Size.lineoft = job_req->SIE_PRE_D2D->PreInLineOfs;

        PRE_Param.REG_pre_hv_width = job_req->SIE_PRE_D2D->PreDramInSizeH;
        PRE_Param.REG_pre_vv_height = job_req->SIE_PRE_D2D->PreDramInSizeV;

        PRE_Param.REG_pre_in_ppb0_addr = job_req->SIE_PRE_D2D->SiePreAddr0;
        PRE_Param.REG_pre_in_ppb1_addr = job_req->SIE_PRE_D2D->SiePreAddr1;

        PRE_Param.REG_pre_output_pack_bus = job_req->SIE_PRE_D2D->PreBitDepth;
        PRE_Param.REG_pre_out_ppb0_addr = job_req->SIE_PRE_D2D->PreOutAdd0;
        PRE_Param.REG_pre_output_size = job_req->SIE_PRE_D2D->PreDramInSizeH;
        PRE_Param.REG_pre_output_lineoft = job_req->SIE_PRE_D2D->PreOutLineOfs;
    }else{

        return IPP_FAIL;
    }

    return IPP_OK;
}


int PRE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if (JobType == PREVIEW_JOB )
    {
    }
    else if (JobType == CAPTURE_JOB )
    {
    }
    else if (JobType == PRE_DRAM_RAWIMGIN_JOB )
    {
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int PRE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if (JobType == PREVIEW_JOB )
    {
        PRE_Param.PRE_Operation = PRE_ENGINE_SET2PRV;
        PRE_Param.PRE_EnFunction = job_req->SIE_PRE_PRV->PreFuncEn;
        PRE_Param.PRE_EnFunction |= Sen_GetPREFlipParam(gImageAlgInfo.ImageFlip);

        if (PRE_Param.PRE_EnFunction & PRE_VIG_EN)
        {
            PRE_Param.pre_vigline_setting = job_req->pre_vigline_setting;
            PRE_Param.pre_vig_setting = job_req->pre_vig_setting;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GEO_EN)
        {
            PRE_Param.pre_geo_setting = job_req->pre_geo_setting;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GAINCTRL_EN)
        {
            PRE_Param.pre_gainctrl_A = job_req->pre_gainctrl_A;
            PRE_Param.pre_gainctrl_B = job_req->pre_gainctrl_B;
        }
        if (PRE_Param.PRE_EnFunction & PRE_CG_EN)
        {
            PRE_Param.pre_cg_ofs = job_req->pre_cg_ofs;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GAMMA_EN)
        {
            PRE_Param.pre_gamma_r = job_req->pre_gamma_r;
            PRE_Param.pre_gamma_g = job_req->pre_gamma_g;
            PRE_Param.pre_gamma_b = job_req->pre_gamma_b;
        }
        if (PRE_Param.PRE_EnFunction & PRE_VACC_EN)
        {
            PRE_Param.VaccIn = &job_req->SIE_PRE_PRV->VaccIn;
        }
        if (PRE_Param.PRE_EnFunction & PRE_DF_SUB_EN)
        {
            PRE_Param.pre_dfs_setting = job_req->pre_dfs_setting;
        }
        //#NT#2009/10/16#Jarkko Chang -begin
        //#NT#modify for ca flow
        if (PRE_Param.PRE_EnFunction & PRE_CA_EN)
        {
//#NT#2009/11/02#Bowen Li -begin
//#NT#modify CA function for 2 & 3 line
            PRE_Param.pre_CA_OutputAdd = job_req->pre_ca_setting.pre_CA_OutputAdd;
            PRE_Param.pre_CA_OutputPtah = job_req->pre_ca_setting.pre_CA_OutputPtah;
            PRE_Param.pre_CA_Hsub = job_req->pre_ca_setting.pre_CA_Hsub;
            PRE_Param.pre_CA_Vsub = job_req->pre_ca_setting.pre_CA_Vsub;
            PRE_Param.pre_CA_OutputLineofs = job_req->pre_ca_setting.pre_CA_OutputLineofs;
//#NT#2009/11/02#Bowen Li -end
        }
        //#NT#2009/10/16#Jarkko Chang -end
        pre_setMode(&PRE_Param);

    }
    else if (JobType == CAPTURE_JOB )
    {
        PRE_Param.PRE_Operation = PRE_ENGINE_SET2CAP;
        PRE_Param.REG_pre_bp1 = job_req->SIE_PRE_CAP->PreBp1;
        PRE_Param.PRE_EnFunction = job_req->SIE_PRE_CAP->PreFuncEn;
        PRE_Param.PRE_EnFunction |= Sen_GetPREFlipParam(gImageAlgInfo.ImageFlip);
        if (PRE_Param.PRE_EnFunction & PRE_VIG_EN)
        {
            PRE_Param.pre_vigline_setting = job_req->pre_vigline_setting;
            PRE_Param.pre_vig_setting = job_req->pre_vig_setting;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GEO_EN)
        {
            PRE_Param.pre_geo_setting = job_req->pre_geo_setting;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GAINCTRL_EN)
        {
            PRE_Param.pre_gainctrl_A = job_req->pre_gainctrl_A;
            PRE_Param.pre_gainctrl_B = job_req->pre_gainctrl_B;
        }
        if (PRE_Param.PRE_EnFunction & PRE_CG_EN)
        {
            PRE_Param.pre_cg_ofs = job_req->pre_cg_ofs;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GAMMA_EN)
        {
            PRE_Param.pre_gamma_r = job_req->pre_gamma_r;
            PRE_Param.pre_gamma_g = job_req->pre_gamma_g;
            PRE_Param.pre_gamma_b = job_req->pre_gamma_b;
        }
        if (PRE_Param.PRE_EnFunction & PRE_VACC_EN)
        {
            PRE_Param.VaccIn = &job_req->SIE_PRE_CAP->VaccIn;
        }
        if (PRE_Param.PRE_EnFunction & PRE_DF_SUB_EN)
        {
            PRE_Param.pre_dfs_setting = job_req->pre_dfs_setting;
        }
        pre_setMode(&PRE_Param);

    }
    else if (JobType == PRE_DRAM_RAWIMGIN_JOB )
    {
        PRE_Param.PRE_Operation = PRE_ENGINE_SET2D2D;
        PRE_Param.PRE_EnFunction = job_req->SIE_PRE_D2D->PreFuncEn;
        if (PRE_Param.PRE_EnFunction & PRE_VIG_EN)
        {
            PRE_Param.pre_vigline_setting = job_req->pre_vigline_setting;
            PRE_Param.pre_vig_setting = job_req->pre_vig_setting;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GEO_EN)
        {
            PRE_Param.pre_geo_setting = job_req->pre_geo_setting;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GAINCTRL_EN)
        {
            PRE_Param.pre_gainctrl_A = job_req->pre_gainctrl_A;
            PRE_Param.pre_gainctrl_B = job_req->pre_gainctrl_B;
        }
        if (PRE_Param.PRE_EnFunction & PRE_CG_EN)
        {
            PRE_Param.pre_cg_ofs = job_req->pre_cg_ofs;
        }
        if (PRE_Param.PRE_EnFunction & PRE_GAMMA_EN)
        {
            PRE_Param.pre_gamma_r = job_req->pre_gamma_r;
            PRE_Param.pre_gamma_g = job_req->pre_gamma_g;
            PRE_Param.pre_gamma_b = job_req->pre_gamma_b;
        }
        if (PRE_Param.PRE_EnFunction & PRE_VACC_EN)
        {
            PRE_Param.VaccIn = &job_req->SIE_PRE_D2D->VaccIn;
        }
        if (PRE_Param.PRE_EnFunction & PRE_DF_SUB_EN)
        {
            PRE_Param.pre_dfs_setting = job_req->pre_dfs_setting;
        }
        pre_setMode(&PRE_Param);
        pre_start();
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int PRE_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    unsigned int PreIntStatus;
    if (JobType == PRE_DRAM_RAWIMGIN_JOB )
    {
        PreIntStatus=pre_ReadIntrStatus();
        while(!(PreIntStatus & PRE_INT_FLDEND))
        PreIntStatus=pre_ReadIntrStatus();
        pre_pause();

        return IPP_OK;
    }
    else
        return IPP_OK;
}

int IPE_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    UINT32 AddrOfs;

    AddrOfs = 0;
    if (JobType == PREVIEW_JOB)
    {
        IPEModeInfo.OutDest = IPE_DEST_IME;
        IPEModeInfo.IntEn = IPE_INTE_DMAEND|IPE_INTE_FRM;
        IPEModeInfo.SIZEINFO.StripeMode = job_req->IpeParam->StripeMode;
        IPEModeInfo.SIZEINFO.HSTRIPE = job_req->IpeParam->HStripe;
        IPEModeInfo.SIZEINFO.VSTRIPE = job_req->IpeParam->VStripe;
        //#NT#2010/05/28#ethanlau -begin
        if(IPEModeInfo.SIZEINFO.StripeMode==IPE_MSTP)
            IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX9;
        //#NT#2011/08/25#Spark Chou -begin
        //#NT# DVI Input
        else if (IPEModeInfo.SIZEINFO.StripeMode==IPE_DVIMODE)
            IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEXRESERVED;
        //#NT#2011/08/25#Spark Chou -end
        else //if(IPEModeInfo.SIZEINFO.StripeMode==IPE_SSTP)
            IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX5;

        //#NT#2012/01/03#Jarkko Chang -begin
        //#NT# modify DVI MST setting
        if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
        {
            IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEXRESERVED;
        }
        //#NT#2012/01/03#Jarkko Chang -end
        //#NT#2010/05/28#ethanlau -end
        IPEModeInfo.SIZEINFO.CfaPat = Sen_GetIPECfaPat(job_req->IpeParam->CfaPat, gImageAlgInfo.ImageFlip);
        IPEModeInfo.SIZEINFO.INSIZE.hsize = job_req->IpeParam->InSizeH;
        IPEModeInfo.SIZEINFO.INSIZE.vsize = job_req->IpeParam->InSizeV;
        IPEModeInfo.SIZEINFO.LofsIn = job_req->IpeParam->InLineOfs;
        IPEModeInfo.ININFO.Adr0 = job_req->IpeParam->Add0;
        IPEModeInfo.ININFO.Adr1 = job_req->IpeParam->Add1;
        IPEModeInfo.ININFO.PackBit = job_req->IpeParam->BitDepth;
        IPEModeInfo.ININFO.PPBEn = ENABLE;
        IPEModeInfo.ININFO.PPBIni = PPBINI_PRE;
        IPEModeInfo.ININFO.SyncSel = SYNC_CROP_END;
        //#NT#2011/08/25#Spark Chou -begin
        //#NT# DVI Input
        IPEModeInfo.ININFO.DVIFormat = job_req->IpeParam->DVIFormat;
        IPEModeInfo.ININFO.DVIHsub = job_req->IpeParam->DVIHsub;
        //#NT#2011/08/25#Spark Chou -end
        IPEModeInfo.OUTINFO.YCFormat = IPE_YCC422;
        IPEModeInfo.OUTINFO.SubHSel = YCC_DROP_RIGHT;
        IPEModeInfo.OUTINFO.Adr0 = job_req->IpeParam->VAAddr;

        AddrOfs = Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreOutHsize), IPEModeInfo.SIZEINFO.LofsIn, gImageAlgInfo.PrePrvBitDepth);
        IPEModeInfo.ININFO.Adr0 += AddrOfs;
        IPEModeInfo.ININFO.Adr1 += AddrOfs;
    }
    else if (JobType == CAPTURE_JOB )
    {
        IPEModeInfo.OutDest = IPE_DEST_IME;
        IPEModeInfo.IntEn = 0;
        IPEModeInfo.SIZEINFO.StripeMode = IPE_MSTP;
        IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX9;
        IPEModeInfo.SIZEINFO.CfaPat = Sen_GetIPECfaPat(job_req->IpeParam->CfaPat, gImageAlgInfo.ImageFlip);
        IPEModeInfo.SIZEINFO.HSTRIPE = job_req->IpeParam->HStripe;
        IPEModeInfo.SIZEINFO.VSTRIPE = job_req->IpeParam->VStripe;
        IPEModeInfo.SIZEINFO.LofsIn = job_req->IpeParam->InLineOfs;

        IPEModeInfo.ININFO.Adr0 = job_req->IpeParam->Add0;
        IPEModeInfo.ININFO.PackBit = job_req->IpeParam->BitDepth;
        IPEModeInfo.ININFO.PPBEn = DISABLE;
        IPEModeInfo.ININFO.PPBIni = PPBINI_0;
        IPEModeInfo.ININFO.SyncSel = SYNC_CROP_END;
        IPEModeInfo.OUTINFO.YCFormat = job_req->IpeParam->OutFormat;
        IPEModeInfo.OUTINFO.SubHSel = YCC_DROP_RIGHT;
        AddrOfs = Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreCropCapHsize), IPEModeInfo.SIZEINFO.LofsIn, gImageAlgInfo.PreCapBitDepth);
        IPEModeInfo.ININFO.Adr0 += AddrOfs;
    }
    else if (JobType == IPE_DRAM_RAWIMGIN_JOB )
    {
        IPEModeInfo.OutDest = IPE_DEST_DRAM;
        IPEModeInfo.IntEn = IPE_INTE_FRM;
        IPEModeInfo.SIZEINFO.StripeMode = IPE_AUTOSTP;
        IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX9;
        IPEModeInfo.SIZEINFO.CfaPat = job_req->IpeParam->CfaPat;
        IPEModeInfo.SIZEINFO.INSIZE.hsize = job_req->IpeParam->InSizeH;
        IPEModeInfo.SIZEINFO.INSIZE.vsize = job_req->IpeParam->InSizeV;
        IPEModeInfo.SIZEINFO.LofsIn = job_req->IpeParam->InLineOfs;
        IPEModeInfo.ININFO.Adr0 = job_req->IpeParam->Add0;
        IPEModeInfo.ININFO.Adr1 = job_req->IpeParam->Add0;
        IPEModeInfo.ININFO.PackBit = job_req->IpeParam->BitDepth;
        IPEModeInfo.ININFO.PPBEn = DISABLE;
        IPEModeInfo.ININFO.PPBIni = PPBINI_0;
        IPEModeInfo.ININFO.SyncSel = SYNC_CROP_END;
        IPEModeInfo.OUTINFO.YCFormat = IPE_YCC422;
        IPEModeInfo.OUTINFO.SubHSel = YCC_DROP_RIGHT;
        IPEModeInfo.OUTINFO.Adr0 = job_req->IpeParam->VAAddr;
    }
    else if ((JobType == IPE_IME_FRAME_1PATH_JOB ) || (JobType == IPE_IME_FRAME_2PATH_JOB ))
    {
        IPEModeInfo.OutDest = IPE_DEST_IME;
        IPEModeInfo.IntEn = 0;
        IPEModeInfo.SIZEINFO.StripeMode = IPE_MSTP;
        IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEX9;
        IPEModeInfo.SIZEINFO.CfaPat = Sen_GetIPECfaPat(job_req->IpeParam->CfaPat, gImageAlgInfo.ImageFlip);
        IPEModeInfo.SIZEINFO.HSTRIPE = job_req->IpeParam->HStripe;
        IPEModeInfo.SIZEINFO.VSTRIPE = job_req->IpeParam->VStripe;
        IPEModeInfo.SIZEINFO.LofsIn = job_req->IpeParam->InLineOfs;

        IPEModeInfo.ININFO.Adr0 = job_req->IpeParam->Add0;
        IPEModeInfo.ININFO.PackBit = job_req->IpeParam->BitDepth;
        IPEModeInfo.ININFO.PPBEn = DISABLE;
        IPEModeInfo.ININFO.PPBIni = 0;
        IPEModeInfo.ININFO.SyncSel = 0;
        IPEModeInfo.OUTINFO.YCFormat = job_req->IpeParam->OutFormat;
        IPEModeInfo.OUTINFO.SubHSel = 0;
        AddrOfs = Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreCropCapHsize), IPEModeInfo.SIZEINFO.LofsIn, gImageAlgInfo.PreCapBitDepth);
        IPEModeInfo.ININFO.Adr0 += AddrOfs;
    }
//#NT#2011/09/06#Spark Chou -begin
    //#NT#
    else if ( JobType == IPE_IME_FRAME_DVI_JOB )
    {
        IPEModeInfo.OutDest = IPE_DEST_IME;
        IPEModeInfo.IntEn = 0;
        //#NT#2012/01/03#Jarkko Chang -begin
        //#NT# modify DVI MST setting
//        IPEModeInfo.SIZEINFO.StripeMode = IPE_DVIMODE;
        //IPEModeInfo.SIZEINFO.StripeMode = IPE_MSTP;
        IPEModeInfo.SIZEINFO.StripeMode = job_req->IpeParam->StripeMode;
        //#NT#2012/01/03#Jarkko Chang -end
        IPEModeInfo.SIZEINFO.LbufNum = IPE_LINEXRESERVED;
        IPEModeInfo.SIZEINFO.CfaPat = Sen_GetIPECfaPat(job_req->IpeParam->CfaPat, gImageAlgInfo.ImageFlip);
        IPEModeInfo.SIZEINFO.HSTRIPE = job_req->IpeParam->HStripe;
        IPEModeInfo.SIZEINFO.VSTRIPE = job_req->IpeParam->VStripe;
        IPEModeInfo.SIZEINFO.LofsIn = job_req->IpeParam->InLineOfs;

        IPEModeInfo.ININFO.DVIFormat = job_req->IpeParam->DVIFormat;
        IPEModeInfo.ININFO.DVIHsub = job_req->IpeParam->DVIHsub;

        IPEModeInfo.ININFO.Adr0 = job_req->IpeParam->Add0;
        IPEModeInfo.ININFO.PackBit = job_req->IpeParam->BitDepth;
        IPEModeInfo.ININFO.PPBEn = DISABLE;
        IPEModeInfo.ININFO.PPBIni = 0;
        IPEModeInfo.ININFO.SyncSel = 0;
        IPEModeInfo.OUTINFO.YCFormat = job_req->IpeParam->OutFormat;
        IPEModeInfo.OUTINFO.SubHSel = 0;
        AddrOfs = Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreCropCapHsize), IPEModeInfo.SIZEINFO.LofsIn, gImageAlgInfo.PreCapBitDepth);
        IPEModeInfo.ININFO.Adr0 += AddrOfs;
    }
    //#NT#2011/09/06#Spark Chou -end
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}


int IPE_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType) {
    return IPP_OK;
}

int IPE_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if (JobType == PREVIEW_JOB)
    {
        ipe_setMode(&IPEModeInfo);
        ipe_start();
    }
    else if (JobType == CAPTURE_JOB )
    {
        ipe_setMode(&IPEModeInfo);
    }
    else if (JobType == IPE_DRAM_RAWIMGIN_JOB )
    {
        ipe_setMode(&IPEModeInfo);
        ipe_start();
    }
    else if ((JobType == IPE_IME_FRAME_1PATH_JOB ) || (JobType == IPE_IME_FRAME_2PATH_JOB ) ||
             (JobType == IPE_IME_FRAME_DVI_JOB))
    {
        ipe_setMode(&IPEModeInfo);
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int IPE_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}

int IME_BeginIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if (JobType == PREVIEW_JOB)
    {
        IMEParam.op_mode = _AUTO_1PATH;
        if ((ime_getOPState() == _AUTO_1PATH) || (ime_getOPState() == _CHANGESIZE_1PATH))
        {
            IMEParam.op_mode = _CHANGESIZE_1PATH;
        }
        IMEParam.in_size.h_size = job_req->ImeParam1->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1->in_v;
        IMEParam.format.in_format = job_req->ImeParam1->in_format;
        IMEParam.format.out1_format = job_req->ImeParam1->out_format;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1->out_v;
        IMEParam.line_ofs.out1_lineofs = job_req->ImeParam1->out_lineoffset;

//#NT#2009/06/30#Jarkko Chang -begin
//#NT#modify for 720p display ratio
        IMEParam.p1_out0_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out0_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out0_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out1_addr.y_addr = job_req->ImeParam1->y1_addr;
        IMEParam.p1_out1_addr.cb_addr = job_req->ImeParam1->cb1_addr;
        IMEParam.p1_out1_addr.cr_addr = job_req->ImeParam1->cr1_addr;
        IMEParam.p1_out2_addr.y_addr = job_req->ImeParam1->y2_addr;
        IMEParam.p1_out2_addr.cb_addr = job_req->ImeParam1->cb2_addr;
        IMEParam.p1_out2_addr.cr_addr = job_req->ImeParam1->cr2_addr;
//#NT#2009/06/30#Jarkko Chang -end
        IMEParam.stripe_mode = job_req->ImeParam1->ime_sten;

        IMEParam.ycc_format.in_format = job_req->ImeParam1->in_YCC_pack;
        IMEParam.ycc_format.out1_format = job_req->ImeParam1->out_YCC_pack;
    }
    else if (JobType == VIDEO_JOB)
    {
        IMEParam.op_mode = _CHANGESIZE_2PATH;
        IMEParam.in_size.h_size = job_req->ImeParam1->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1->in_v;
        IMEParam.format.in_format = job_req->ImeParam1->in_format;

        IMEParam.format.out1_format = job_req->ImeParam1->out_format;
        IMEParam.line_ofs.out1_lineofs = job_req->ImeParam1->out_lineoffset;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1->out_v;
//#NT#2009/06/30#Jarkko Chang -begin
//#NT#modify for 720p display ratio
        IMEParam.p1_out0_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out0_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out0_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out1_addr.y_addr = job_req->ImeParam1->y1_addr;
        IMEParam.p1_out1_addr.cb_addr = job_req->ImeParam1->cb1_addr;
        IMEParam.p1_out1_addr.cr_addr = job_req->ImeParam1->cr1_addr;
        IMEParam.p1_out2_addr.y_addr = job_req->ImeParam1->y2_addr;
        IMEParam.p1_out2_addr.cb_addr = job_req->ImeParam1->cb2_addr;
        IMEParam.p1_out2_addr.cr_addr = job_req->ImeParam1->cr2_addr;
//#NT#2009/06/30#Jarkko Chang -end
        IMEParam.stripe_mode = job_req->ImeParam1->ime_sten;

        IMEParam.ycc_format.in_format = job_req->ImeParam1->in_YCC_pack;
        IMEParam.ycc_format.out1_format = job_req->ImeParam1->out_YCC_pack;

        IMEParam.format.out2_format = job_req->ImeParam2->out_format;
        IMEParam.line_ofs.out2_lineofs = job_req->ImeParam2->out_lineoffset;
        IMEParam.p2_out_size.h_size = job_req->ImeParam2->out_h;
        IMEParam.p2_out_size.v_size = job_req->ImeParam2->out_v;
        IMEParam.p2_out0_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out0_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out0_addr.cr_addr = job_req->ImeParam2->cr0_addr;
        IMEParam.p2_out1_addr.y_addr = job_req->ImeParam2->y1_addr;
        IMEParam.p2_out1_addr.cb_addr = job_req->ImeParam2->cb1_addr;
        IMEParam.p2_out1_addr.cr_addr = job_req->ImeParam2->cr1_addr;
        IMEParam.p2_out2_addr.y_addr = job_req->ImeParam2->y2_addr;
        IMEParam.p2_out2_addr.cb_addr = job_req->ImeParam2->cb2_addr;
        IMEParam.p2_out2_addr.cr_addr = job_req->ImeParam2->cr2_addr;

        IMEParam.ycc_format.out2_format = job_req->ImeParam2->out_YCC_pack;
    }
    else if (JobType == CAPTURE_JOB )
    {
        IMEParam.op_mode = _SLICE_2PATH;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.in_size.h_size = job_req->ImeParam1->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1->in_v;
        IMEParam.format.in_format = job_req->ImeParam1->in_format;

        IMEParam.format.out1_format = job_req->ImeParam1->out_format;
        IMEParam.line_ofs.out1_lineofs = job_req->ImeParam1->out_lineoffset;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1->out_v;
        IMEParam.p1_out0_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out0_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out0_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out1_addr.y_addr = job_req->ImeParam1->y1_addr;
        IMEParam.p1_out1_addr.cb_addr = job_req->ImeParam1->cb1_addr;
        IMEParam.p1_out1_addr.cr_addr = job_req->ImeParam1->cr1_addr;
        IMEParam.p1_out2_addr.y_addr = job_req->ImeParam1->y2_addr;
        IMEParam.p1_out2_addr.cb_addr = job_req->ImeParam1->cb2_addr;
        IMEParam.p1_out2_addr.cr_addr = job_req->ImeParam1->cr2_addr;

        IMEParam.format.out2_format = job_req->ImeParam2->out_format;
        IMEParam.line_ofs.out2_lineofs = job_req->ImeParam2->out_lineoffset;
        IMEParam.p2_out_size.h_size = job_req->ImeParam2->out_h;
        IMEParam.p2_out_size.v_size = job_req->ImeParam2->out_v;
        IMEParam.p2_out0_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out0_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out0_addr.cr_addr = job_req->ImeParam2->cr0_addr;
        IMEParam.p2_out1_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out1_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out1_addr.cr_addr = job_req->ImeParam2->cr0_addr;
        IMEParam.p2_out2_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out2_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out2_addr.cr_addr = job_req->ImeParam2->cr0_addr;
    }
    else if (JobType == IPE_IME_FRAME_2PATH_JOB )
    {
        IMEParam.op_mode = _SLICE_2PATH;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.in_size.h_size = job_req->ImeParam1->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1->in_v;
        IMEParam.format.in_format = job_req->ImeParam1->in_format;

        IMEParam.format.out1_format = job_req->ImeParam1->out_format;
        IMEParam.line_ofs.out1_lineofs = job_req->ImeParam1->out_lineoffset;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1->out_v;
        IMEParam.p1_out0_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out0_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out0_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out1_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out1_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out1_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out2_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out2_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out2_addr.cr_addr = job_req->ImeParam1->cr0_addr;

        IMEParam.format.out2_format = job_req->ImeParam2->out_format;
        IMEParam.line_ofs.out2_lineofs = job_req->ImeParam2->out_lineoffset;
        IMEParam.p2_out_size.h_size = job_req->ImeParam2->out_h;
        IMEParam.p2_out_size.v_size = job_req->ImeParam2->out_v;
        IMEParam.p2_out0_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out0_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out0_addr.cr_addr = job_req->ImeParam2->cr0_addr;
        IMEParam.p2_out1_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out1_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out1_addr.cr_addr = job_req->ImeParam2->cr0_addr;
        IMEParam.p2_out2_addr.y_addr = job_req->ImeParam2->y0_addr;
        IMEParam.p2_out2_addr.cb_addr = job_req->ImeParam2->cb0_addr;
        IMEParam.p2_out2_addr.cr_addr = job_req->ImeParam2->cr0_addr;
    }
    else if (JobType == IPE_IME_FRAME_1PATH_JOB )
    {
        IMEParam.op_mode = _SLICE_1PATH;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.in_size.h_size = job_req->ImeParam1->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1->in_v;
        IMEParam.format.in_format = job_req->ImeParam1->in_format;

        IMEParam.format.out1_format = job_req->ImeParam1->out_format;
        IMEParam.line_ofs.out1_lineofs = job_req->ImeParam1->out_lineoffset;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1->out_v;
        IMEParam.p1_out0_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out0_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out0_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out1_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out1_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out1_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out2_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out2_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out2_addr.cr_addr = job_req->ImeParam1->cr0_addr;
    }
    //#NT#2011/09/06#Spark Chou -begin
    //#NT#
    else if (JobType == IPE_IME_FRAME_DVI_JOB )
    {
        IMEParam.op_mode = _DVI_1PATH;;
        IMEParam.in_src = IMEALG_IPE2IME;
        IMEParam.in_size.h_size = job_req->ImeParam1->in_h;
        IMEParam.in_size.v_size = job_req->ImeParam1->in_v;
        IMEParam.format.in_format = job_req->ImeParam1->in_format;

        IMEParam.format.out1_format = job_req->ImeParam1->out_format;
        IMEParam.line_ofs.out1_lineofs = job_req->ImeParam1->out_lineoffset;
        IMEParam.p1_out_size.h_size = job_req->ImeParam1->out_h;
        IMEParam.p1_out_size.v_size = job_req->ImeParam1->out_v;
        IMEParam.p1_out0_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out0_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out0_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out1_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out1_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out1_addr.cr_addr = job_req->ImeParam1->cr0_addr;
        IMEParam.p1_out2_addr.y_addr = job_req->ImeParam1->y0_addr;
        IMEParam.p1_out2_addr.cb_addr = job_req->ImeParam1->cb0_addr;
        IMEParam.p1_out2_addr.cr_addr = job_req->ImeParam1->cr0_addr;
    }
    //#NT#2011/09/06#Spark Chou -end
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}


int IME_AllocatResourceIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}

int IME_ExecIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    if (JobType == PREVIEW_JOB)
    {
        ime_setMode(&IMEParam);
        ime_setStart();
    }
    else if (JobType == VIDEO_JOB )
    {
        ime_setMode(&IMEParam);
        ime_setStart();
    }
    else if (JobType == CAPTURE_JOB )
    {
        ime_setMode(&IMEParam);
        ime_setSliceH(GetIPPSizeInfor(_CapIMESliceMode));
    }
    else if ((JobType == IPE_IME_FRAME_1PATH_JOB ) || (JobType == IPE_IME_FRAME_2PATH_JOB ) || (JobType == IPE_IME_FRAME_DVI_JOB))
    {
        ime_setMode(&IMEParam);
    }
    else
    {
        return IPP_FAIL;
    }
    return IPP_OK;
}

int IME_EndIPP(struct  PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType)
{
    return IPP_OK;
}

