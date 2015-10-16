#include     "sensor_model.h"
#if ZT3150_630
volatile unsigned long cap_cnt;
extern volatile unsigned int ime_fc;
extern volatile unsigned int ipe_fc;
extern volatile unsigned int ipe_vac;
UINT32 currSIETime = 0, prevSIETime = 0;
UINT32 currPRETime = 0, prevPRETime = 0;
UINT32 currIPETime = 0, prevIPETime = 0;
UINT32 currIMETime = 0, prevIMETime = 0;

extern UINT32 ShowFPS;
UINT32 uiLines = 0;
void sie_isrCB(UINT32 IntStatus)
{
//#NT#2010/10/27#Jarkko Chang -begin
//#NT#modify for ca setting
    UINT32 HRatio, VRatio, Ratio;

    if (IntStatus & SIE_INT_LNEND )
    {
        uiLines ++;
    }
    if (IntStatus & SIE_INT_FLDEND)
    {
        if ( ShowFPS == TRUE )
        {
            Perf_Open();
            currSIETime = Perf_GetCurrent();
            debug_err(("SIE %d ms\r\n", (currSIETime-prevSIETime)/1000));
            prevSIETime = currSIETime;
        }
        /*
        if (Get_DZoomFlag() & DZOOM_SET_CA)
        {
//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for ca process
            if (pre_checkFunctionEnable(PRE_CA_EN))
            {
                Sen_GetCASubRatio(SIE_PRE_Para_prv.VGAType, GetIPPSizeInfor(_CAHSub), GetIPPSizeInfor(_CAVSub), &HRatio, &VRatio);
                Ratio = Sen_GetIntlvRatio(SIE_PRE_Para_prv.VGAType);
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
//#NT#2009/10/16#Jarkko Chang -end
            Clr_DZoomFlag(DZOOM_SET_CA);
        }
        */
        if (Get_DZoomFlag() & DZOOM_SET_SIEPRE)
        {
            sie_Set2Dzoom(&SIE_PRE_Para_prv);
            Clr_DZoomFlag(DZOOM_SET_SIEPRE);
            Set_DZoomFlag(DZOOM_SET_IPEIME);
//            Set_DZoomFlag(DZOOM_SET_CA);
        }
//#NT#2011/09/08#Spark Chou -begin
//#NT#
        if ( GetIPPFuncInfor(DVI_MODE_EN) == ENABLE && DVISkipFrmEn == ENABLE )
        {
        }
        else
        {
          //  sie_SensorDataEnable(ON);
          //  sie_Load();
        }
//#NT#2011/09/08#Spark Chou -end
    }
//#NT#2010/10/27#Jarkko Chang -end
    if (IntStatus & SIE_INT_BP1)
    {
        if(cap_cnt>0)
        {
            cap_cnt--;
        }
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_SIE, IntStatus);
}

void pre_isrCB(UINT32 IntStatus)
{
    if (IntStatus & PRE_INT_FLDEND)
    {
        if (Get_DZoomFlag() & DZOOM_SET_IPEIME)
        {
            VideoDzoomSet();
            ime_clrIntFlag();
            Clr_DZoomFlag(DZOOM_SET_IPEIME);
            Set_DZoomFlag(DZOOM_IDLE);
        }
    }
    if ( IntStatus & (PRE_INTE_OVFL1|PRE_INTE_OVFL2) )
    {
        debug_err(("^RPRE overflow\r\n"));
    }
    if (IntStatus & PRE_INT_FRMEND)
    {
        if ( ShowFPS == TRUE )
        {
            currPRETime = Perf_GetCurrent();
            debug_err(("PRE %d ms\r\n", (currPRETime-prevPRETime)/1000));
            prevPRETime = currPRETime;
        }
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_PRE, IntStatus);
}


void ime_isrCB(UINT32 IntStatus)
{
    UINT   ready_FB,enc_FB;
        if(IntStatus & Ime_INT_FRM)
        {
/*
        if ( ShowFPS == TRUE )
        {
            currIMETime = Perf_GetCurrent();
            debug_err(("ime %d ms\r\n", (currIMETime-prevIMETime)/1000));
            prevIMETime = currIMETime;
        }
        */
            if(gAlgMsg.Alg_Mode==MODE_Video_Recording && IME_D2_Para.buf_num > 3)
            {
                ready_FB=(ime_fc-1)%IME_D2_Para.buf_num;
                enc_FB=(ime_fc+IME_D2_Para.buf_num-3)%IME_D2_Para.buf_num;
                switch(ready_FB)
                {
                    case 0:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb0_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr0_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 1:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb1_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr1_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 2:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb2_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr2_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 3:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb3_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr3_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 4:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb4_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr4_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 5:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb5_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr5_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 6:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb6_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr6_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                    case 7:
                        ime_setPath2Scale_to_DMAReg(IME_D2_Para.y7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.y7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cb7_addr,//unsigned int    D_Addr,
                                            IME_D2_Para.cr7_addr);//unsigned int    D_Addr,
                        IME_D2_Para.done_buf = enc_FB;
                    break;
                }
            }
        }
    Sen_IsrEventProc(SEN_ISR_PHOTO_IME, IntStatus);
}

//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
void ipe_isrCB(UINT32 IntStatus)
{
    if(IntStatus & IPE_INT_FRM)
    {
        /*
        if ( ShowFPS == TRUE )
        {
            currIPETime = Perf_GetCurrent();
            debug_err(("IPE %d ms\r\n", (currIPETime-prevIPETime)/1000));
            prevIPETime = currIPETime;
        }
        */
    }

    Sen_IsrEventProc(SEN_ISR_PHOTO_IPE, IntStatus);
    Sen_IsrEventProc(SEN_ISR_PHOTODIS_IPE, IntStatus);
}

void dis_isrCB(UINT32 IntStatus)
{
    Sen_IsrEventProc(SEN_ISR_PHOTODIS_DIS, IntStatus);
}
//#NT#2009/11/27#Connie Yu -end
#endif
