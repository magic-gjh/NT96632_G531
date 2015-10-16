#include     "sensor_model.h"
#if OV9710_DUAL_630
volatile unsigned long cap_cnt;
extern volatile unsigned int ime_fc;
extern volatile unsigned int ipe_fc;
extern volatile unsigned int ipe_vac;

UINT32 currTime = 0;
UINT32 prevTime = 0;
extern UINT32 ShowFPS;

extern UINT32 gDynaGammaSetting;
UINT32 IQ_GammaLUT[130];

INT32 InterpolateIpeValue(UINT32 Index, UINT32 LowValue, UINT32 HighValue, UINT32 MinIndex, UINT32 MaxIndex)
{
    if ( Index < MinIndex)
        return LowValue;
    else if ( Index > MaxIndex)
        return HighValue;
    else
        return LowValue + ((INT32)((INT32)HighValue - (INT32)LowValue)) * ((INT32)(Index  - MinIndex)) / ((INT32)(MaxIndex - MinIndex));
}


//#NT#2009/11/02#Bowen Li -begin
//#NT#modify CA function for 2 & 3 line
void sie_isrCB(UINT32 IntStatus)
{
    UINT32 HRatio, VRatio, Ratio;
    if (IntStatus & SIE_INT_FLDEND)
    {
        if ( ShowFPS == TRUE )
        {
            currTime = Perf_GetCurrent();
            debug_err(("%d ms\r\n", (currTime-prevTime)/1000));
            prevTime = currTime;
        }
        if (Get_DZoomFlag() & DZOOM_SET_CA)
        {
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
            Clr_DZoomFlag(DZOOM_SET_CA);
        }
        if (Get_DZoomFlag() & DZOOM_SET_SIEPRE)
        {
            sie_Set2Dzoom(&SIE_PRE_Para_prv);
            Clr_DZoomFlag(DZOOM_SET_SIEPRE);
            Set_DZoomFlag(DZOOM_SET_IPEIME);
            Set_DZoomFlag(DZOOM_SET_CA);
        }
    }

    if (IntStatus & SIE_INT_BP1)
    {
        if(cap_cnt>0)
        {
            cap_cnt--;
        }
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_SIE, IntStatus);
}
//#NT#2009/11/02#Bowen Li -end

void pre_isrCB(UINT32 IntStatus)
{
    int i;
    if (IntStatus & PRE_INT_FLDEND)
    {
        if (Get_DZoomFlag() & DZOOM_SET_IPEIME)
        {
            VideoDzoomSet();
            ime_clrIntFlag();
            Clr_DZoomFlag(DZOOM_SET_IPEIME);
            Set_DZoomFlag(DZOOM_IDLE);
        }
#if 0
        if ( gDynaGammaSetting == TRUE )
        {
            for (i=0; i< 130; i++ )
            {
                IQ_GammaLUT[i] = InterpolateIpeValue(AE_Param.EV_Value, GammaLUT_128Tab_Video_Night[i], GammaLUT_128Tab_Video_Normal[i], 320, 1280);
            }
            CPU_WriteGammaTab(IQ_GammaLUT, IQ_GammaLUT, IQ_GammaLUT);
        }
#endif
    }
    Sen_IsrEventProc(SEN_ISR_PHOTO_PRE, IntStatus);
}

void ime_isrCB(UINT32 IntStatus)
{
    UINT   ready_FB,enc_FB,_currentY;
        if(IntStatus & Ime_INT_FRM)
        {
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
    Sen_IsrEventProc(SEN_ISR_PHOTO_IPE, IntStatus);
    Sen_IsrEventProc(SEN_ISR_PHOTODIS_IPE, IntStatus);
}

void dis_isrCB(UINT32 IntStatus)
{
    Sen_IsrEventProc(SEN_ISR_PHOTODIS_DIS, IntStatus);
}
//#NT#2009/11/27#Connie Yu -end
#endif
