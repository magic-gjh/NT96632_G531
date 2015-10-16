#include    "sensor_model.h"
#include    "cal_api.h"
#if    OV9710_DUAL_630

UINT Rgain=396,Bgain=355,Ggain=256,OldRgain=396,OldBgain=355,OldGgain=256;
UINT CapRgain=396,CapBgain=355,CapGgain=256;
UINT gAWB_counter=0;
UINT32    R_Acc[256]={0}, G_Acc[256]={0}, B_Acc[256]={0};
STR_AAA_CA_PARA CA_para;

extern INT32 abs(INT32 i);

volatile unsigned int awb_fc=0;
unsigned int gTmpCounter=0;
UINT8 R_acc[64], G_acc[64], B_acc[64];
UINT8 CapR_acc[64], CapG_acc[64], CapB_acc[64];
unsigned char  gAWBWeight[64];
unsigned char  gCapAWBWeight[64];
unsigned int ExpValue;
unsigned char PhotoAWBEnter=0;
unsigned int Rcfg=256,Bcfg=256;
unsigned int Calibration_Rg=256,Calibration_Bg=256;
unsigned int Golden_Rg=396,Golden_Bg=355;
int K_Rgain=100,K_Bgain=100;
unsigned int Golden_Rg_Min=400,Golden_Rg_Max=450,Golden_Bg_Min=308,Golden_Bg_Max=334;
extern AE_STRUCT AE_Param;
//extern volatile Adjust_Struct   *AAA_Adjust_Info;
BOOL AWB_DebugMsg = FALSE;

UINT32  gOutdoor_LV = Outdoor_LV;
INT32 AWB_LV,Old_AWB_LV;
UINT16 Thr_GR_L_M;
UINT16 Thr_GR_H_M;
UINT16 Thr_GB_L_M;
UINT16 Thr_GB_H_M;
UINT16 Thr_RB_L_M;
UINT16 Thr_RB_H_M;
UINT16 Thr_Saturation_L_M;
UINT16 Thr_Saturation_H_M;
UINT16 Thr_GainSum_L_M;
UINT16 Thr_GainSum_H_M;
extern UINT32 nMin_EV_Value;
#if Awb_Cal_On
UINT8 R_acc1[64], G_acc1[64], B_acc1[64];
UINT8 cal_Rgain,cal_Bgain;
#endif

//#NT#2010/09/23#ethanlau -begin
typedef enum {
    _indoor=0,
    _outdoor=1
}environ;
typedef struct _Coordinate
{
    UINT32 Startx;
    UINT32 Starty;
    UINT32 Endx;
    UINT32 Endy;
}Coordinate,*pCoordinate;
BOOL UniformSceneChk(UINT32 *G,UINT32 *B,pCoordinate _coor,UINT32 diff);
UINT AwbCheckWhite(UINT R,UINT G,UINT B,environ _environ);
//#NT#2010/09/23#ethanlau -end

unsigned short MwbTbl[7][2]={
    { 256, 256}, // Auto
    { 480, 324}, // Daylight (corresponded to Premier light box: B light )
    { 448, 261}, // Cloudy   (corresponded to Premier light box: C light )
    { 238, 482}, // Tungsten (corresponded to Premier light box: A light )
    { 398, 329}, // Fluorescent1 (corresponded to Premier light box: FL1 light )
    { 256, 256}, // Fluorescent2 (corresponded to Premier light box: FL2 light )
    { 256, 256}, // Sunset
};
unsigned short CustomerTbl[3][2]={
    { 256, 256},
    { 256, 256},
    { 256, 256},
};

/**
    Initialize AWB
*/
void aaa_InitRBgain(void)
{

    Cal_Struct *CalInfo;

    CalInfo = Cal_GetCalibrationData();
    AWB_LV = AE__GetLV();
    PhotoAWBEnter = 1;

    if(AWB_LV<110)
    {
        Rgain =317;
        Bgain =390;
    }
    else
    {
        Rgain =390;
        Bgain =337;
    }

#if Awb_Cal_On
    if(CalInfo->CalStatus_AWB != _AWB_Status)
    {
        K_Rgain=100;
        K_Bgain=100;
        debug_err(("WARING:AWB calibration is not done yet!\r\n"));
        return;
    }

    Calibration_Rg=CalInfo->Cal_AWBStatistic.Rgain;
    Calibration_Bg=CalInfo->Cal_AWBStatistic.Bgain;

    //debug_msg("Calibration_Rg=%d,Calibration_Bg=%d\r\n",Calibration_Rg,Calibration_Bg);

    //temperally way for average golden sample value, need to change after release final calibration fw.
    //if(AAA_Adjust_Info->FTP_Status==0x21/*_FTP_Status*/)
    {
// Golden sample is not choosen yet.
//        Golden_Rg=AAA_Adjust_Info->AWB_Prv.Rgain;
//        Golden_Bg=AAA_Adjust_Info->AWB_Prv.Bgain;
//        debug_msg("AWB Golden Sample : GS_Rg = %d, GS_Bg = %d\r\n",Golden_Rg,Golden_Bg);
        Golden_Rg_Min=(Golden_Rg*100)/106;
        Golden_Rg_Max=(Golden_Rg*106)/100;
        Golden_Bg_Min=(Golden_Bg*100)/104;
        Golden_Bg_Max=(Golden_Bg*104)/100;

        if(Calibration_Rg>Golden_Rg_Max)
            K_Rgain=100*Calibration_Rg/Golden_Rg_Max;
        else if(Calibration_Rg<Golden_Rg_Min)
            K_Rgain=100*Calibration_Rg/Golden_Rg_Min;
        else
            K_Rgain=100;

        if(Calibration_Bg>Golden_Bg_Max)
            K_Bgain=100*Calibration_Bg/Golden_Bg_Max;
        else if(Calibration_Bg<Golden_Bg_Min)
            K_Bgain=100*Calibration_Bg/Golden_Bg_Min;
        else
            K_Bgain=100;
    }

#endif
}

void aaa_InitAWB(void)
{
    aaa_SetAwbRate(4);
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for ca process
//    aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
    aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
//#NT#2009/08/31#Jarkko Chang -end
}

/**
    AWB Algorithm Sample
*/
void aaa_CAprocess(void)
{
    if (pre_checkFunctionEnable(PRE_CA_EN))
    {
        CA_para.DramAddr = ippbuf_GetBuf(IPPBUF_CA);
    }
    else
    {
        if (pre_ReadCurrentOutBufferID() == 1)
        {
            CA_para.DramAddr = ippbuf_GetBuf(IPPBUF_PRV_RAW_1);
        }
        else
        {
            CA_para.DramAddr = ippbuf_GetBuf(IPPBUF_PRV_RAW_2);
        }
    }

    //#NT#2011/03/02#ethanlau -begin
    //#NT#fix V-flip bug,this addr is for ca calulate, not for engine output addr
    //CA_para.DramAddr += Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreOutHsize), 8);
    CA_para.DramAddr += Sen_GetIPEAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_CAHsize), GetIPPSizeInfor(_CALineOffset), 8);
    //#NT#2011/03/02#ethanlau -end
    CA_para.DramLofs = GetIPPSizeInfor(_CALineOffset);
    CA_para.ImgHsize = GetIPPSizeInfor(_CAHsize);
    CA_para.ImgVsize = GetIPPSizeInfor(_CAVsize);
    CA_para.R_Data = R_Acc;
    CA_para.G_Data = G_Acc;
    CA_para.B_Data = B_Acc;
    CA_para.Win_x = CA_WINDOW_X;
    CA_para.Win_y = CA_WINDOW_Y;

    CA_para.Cfapat = Sen_GetIPECfaPat(CFAPAT_PRV, gImageAlgInfo.ImageFlip);
//#NT#2009/11/02#Bowen Li -begin
//#NT#modify CA function for 2 & 3 line
    if (pre_checkFunctionEnable(PRE_CA_EN))
    {
        aaa_GetCAResult(&CA_para, 8, SIE_PRE_Para_prv.VGAType);
    }
    else
    {
        aaa_GetCAResult(&CA_para, gImageAlgInfo.PrePrvBitDepth, SIE_PRE_Para_prv.VGAType);
    }
//#NT#2009/10/16#Jarkko Chang -end
//#NT#2009/11/02#Bowen Li -end
}

void Sensor_AWBEn(UINT32 enable);

void aaa_AWBprocess(void)
{
    UINT i;
    UINT R_total=1, G_total=1, B_total=1,W_total=0;
    UINT NewRgain,NewBgain,NewGgain;
    UINT R_cal,G_cal,B_cal;
    UINT Rg_tmp,Bg_tmp,Rg_temp,Bg_temp;
    UINT _tmpR,_tmpB;
    BOOL _flag1=FALSE, _flag2=FALSE;
    environ _condition;
    static environ _precondition = _indoor;
    Coordinate _coor;
    static UINT stbcount = 0;
    aaa_SetAwbRate(2);

    if(gImageAlgInfo.WBMode != _IMGWBMode_Auto)
    {
        Sensor_AWBEn(FALSE);
        aaa_MWBprocess(gImageAlgInfo.WBMode);
        return;
    }

    //#NT#2012/02/13#Spark Chou -begin
    //#NT#ISP AWB
    Sensor_AWBEn(TRUE);
    Rgain=Bgain=Ggain=256;
    OldRgain=OldBgain=OldGgain=256;
    aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
    //#NT#2012/02/13#Spark Chou -end
    gAWB_counter++;
    aaa_CAprocess();
    for(i=0;i<CA_Window_Num;i++)
    {
        R_cal=(CA_para.R_Data[i]);
        G_cal=(CA_para.G_Data[i]);
        B_cal=(CA_para.B_Data[i]);
        if(R_cal<1)
            R_cal=1;
        if(G_cal<1)
            G_cal=1;
        if(B_cal<1)
            B_cal=1;
        R_acc[i] = R_cal;
        G_acc[i] = G_cal;
        B_acc[i] = B_cal;
    }
    return ;

    gAWB_counter++;
    aaa_CAprocess();
    AWB_LV=AE__GetLV();

    if(AWB_LV > (gOutdoor_LV+15) )
        _condition = _outdoor;
    else if(AWB_LV<gOutdoor_LV)
        _condition = _indoor;
    else
        _condition = _precondition;
    _precondition = _condition;

#if 1//for lab uniform A light
    _coor.Startx = 1;
    _coor.Starty = 1;
    _coor.Endx = 6;
    _coor.Endy = 6;
    if(UniformSceneChk(CA_para.G_Data,CA_para.B_Data,&_coor, 3))
        stbcount++;
    else
        stbcount = 0;
    if(stbcount > 9)
        _condition = _indoor;
    else
        _condition = _precondition;
#endif

    for(i=0;i<CA_Window_Num;i++)
    {
#if Awb_Cal_On
            R_cal=((CA_para.R_Data[i] )*K_Rgain)/100;
            G_cal=(CA_para.G_Data[i] );
            B_cal=((CA_para.B_Data[i] )*K_Bgain)/100;


#else
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for ca process
            R_cal=(CA_para.R_Data[i]);
            G_cal=(CA_para.G_Data[i]);
            B_cal=(CA_para.B_Data[i]);
//#NT#2009/08/31#Jarkko Chang -end
#endif
        if(R_cal<1)
            R_cal=1;
        if(G_cal<1)
            G_cal=1;
        if(B_cal<1)
            B_cal=1;
        R_acc[i] = R_cal;
        G_acc[i] = G_cal;
        B_acc[i] = B_cal;

#if AwbGrayWorld
            gAWBWeight[i]=1;
#else
            gAWBWeight[i]=AwbCheckWhite(R_cal,G_cal,B_cal,_condition);
#endif
        if(gAWBWeight[i]==1)
        {
            R_total+=R_cal;
            G_total+=G_cal;
            B_total+=B_cal;
            W_total+=gAWBWeight[i];
        }
    }

    if(W_total)
    {
        Rg_tmp=100*G_total/R_total;
        Bg_tmp=100*G_total/B_total;

        for(i=0;i<CA_Window_Num;i++)
        {
            if(gAWBWeight[i]==1)
            {
                Rg_temp = 100*G_acc[i]/R_acc[i];
                Bg_temp = 100*G_acc[i]/B_acc[i];
                if(abs(Rg_tmp-Rg_temp)>16)
                {
                    R_total-=R_acc[i];
                    G_total-=G_acc[i];
                    B_total-=B_acc[i];
                    W_total--;
                    gAWBWeight[i]=OutofRange;
                    continue;
                }
                else if(abs(Bg_tmp- Bg_temp)>16)
                {
                    R_total-=R_acc[i];
                    G_total-=G_acc[i];
                    B_total-=B_acc[i];
                    W_total--;
                    gAWBWeight[i]=OutofRange;
                }

                if(gAWBWeight[i]==1)
                {
                    if(AWB_LV > gOutdoor_LV-1)//avoid grass&durt in outdoor
                    {
                        if(Rg_temp <= (Bg_temp*95/100))
                        {
                            R_total -= R_acc[i];
                            B_total -= B_acc[i];
                            _tmpR = R_acc[i]*95/100;
                            _tmpB = B_acc[i]*100/85;
                            R_total += _tmpR;
                            B_total += _tmpB;
                            gAWBWeight[i] = Modify_RB;
                        }
                    }
                }
            }
        }
    }

    if(W_total > 5)
    {
        if(R_total<1)
            R_total=1;
        if(B_total<1)
            B_total=1;
        NewRgain = (G_total<<8)/R_total;
        NewGgain = 256;
        NewBgain = (G_total<<8)/B_total;

        if(NewRgain<1)
            NewRgain=1;
        if(NewBgain<1)
            NewBgain=1;

        if(PhotoAWBEnter==1)
        {
            PhotoAWBEnter=0;
        }
        //if((AE_Param.StableCnt>1) || (AE_Param.EV_Value==nMin_EV_Value) )
        {
            Rgain = (NewRgain+OldRgain*4)/5;
            Bgain = (NewBgain+OldBgain*4)/5;
            Ggain = NewGgain;

#if MASSAGE_ON_DIS_GAIN
        if(Rgain != OldRgain || Bgain != OldBgain)
            _flag1 = 1;
#endif

        OldRgain=Rgain;
        OldBgain=Bgain;
        OldGgain=Ggain;

//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for ca process
//        aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
            aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
            //debug_msg("PRE RGBGAIN: %d %d %d\r\n",Rgain,Ggain,Bgain);
#if 0
            SensorInfor.FP_ClearOSD();
            sprintf(message,"%3d %3d %3d 0x%4x",
                    Rgain,
                    Ggain,
                    Bgain);
            SensorInfor.FP_ShowOSDString(message,20,20,4);
#endif
        }
//#NT#2009/08/31#Jarkko Chang -end
    }
    else
    {
#if MASSAGE_ON_DIS_GAIN
        _flag1 = 0;
#endif

//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for ca process
        //aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
        //if((AE_Param.StableCnt>1) || (AE_Param.EV_Value==nMin_EV_Value) )
            aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
//#NT#2009/08/31#Jarkko Chang -end
    }

#if MASSAGE_ON_DIS_LV
        if(AWB_LV != Old_AWB_LV)
            _flag2 = 1;
        Old_AWB_LV = AWB_LV;
#endif

#if MASSAGE_ON_DIS_GAIN
        if(_flag1 ==1)
        {
            SenFP_ClearOSD();
            sprintf(message,"%d %d,%d %d,%d %d", OldRgain, OldBgain,NewRgain,NewBgain,Rgain, Bgain);
            SenFP_ShowOSDString(message,20,60,4);
            _flag1 = 0;
        }
#endif
#if MASSAGE_ON_DIS_LV
        if(_flag2 ==1)
        {
            SenFP_ClearOSD();
            sprintf(message,"LV:%d",AWB_LV);
            SenFP_ShowOSDString(message,20,100,4);
            _flag2 = 0;
        }
#endif

    if(AWB_DebugMsg)
    {
        if((gAWB_counter%5)==0)
        {
            debug_err(("AWB:Rgain=%3d,Ggain=%3d,Bgain=%3d\r\n",
                        Rgain,
                        Ggain,
                        Bgain));
            debug_err(("----------------------------\r\n"));
        }
    }
}

/**
    Check if a point (R,G,B) white
    @param R inputR
    @param G inputG
    @param B inputB
    @return 1:white 0:not white
*/
UINT AwbCheckWhite(UINT R,UINT G,UINT B,environ _environ)
{
    UINT    _GainSum,_GainMux;
    UINT    rgain,bgain;

    if(G<Thr_LowBright2)
    {
        return Bright_lo;
    }

    if(G>Thr_HighBright2)
    {
        return Bright_hi;
    }

    rgain=100*G/R;
    bgain=100*G/B;
    _GainMux = rgain*bgain/100;

    _GainSum = rgain + bgain;
    if(_environ==_indoor)
    {
        if(rgain<Thr_GR_L2)
            return Rgain_lo;
        else if(rgain>Thr_GR_H2)
            return Rgain_hi;
        else if(bgain<Thr_GB_L2)
            return Bgain_lo;
        else if(bgain>Thr_GB_H2)
            return Bgain_hi;
        else if(((((int)R*100)<Thr_RB_L2*(int)B)) )
            return RBGain_lo;
        else if( ((int)R*100)>Thr_RB_H2*(int)B )
            return RBGain_hi;
        else if(_GainSum <Thr_GainSum_L2)
            return GainSum_lo;
        else if(_GainSum > Thr_GainSum_H2)
            return GainSum_hi;
        else if(abs(rgain-bgain) < Thr_GainDiff_L2)
            return GainDiff_min;
        else if(_GainMux > Thr_GainMux_H2)
            return GainMux_max;
        else if(_GainMux < Thr_GainMux_L2)
            return GainMux_min;
    }
    else// if(_environ==_outdoor)
    {
        if(rgain<Thr_GR_L3)
            return Rgain_lo;
        else if(rgain>Thr_GR_H3)
            return Rgain_hi;
        else if(bgain<Thr_GB_L3 )
            return Bgain_lo;
        else if(bgain>Thr_GB_H3)
            return Bgain_hi;
        else if(((((int)R*100)<Thr_RB_L3*(int)B)) )
            return RBGain_lo;
        else if( ((int)R*100)>Thr_RB_H3*(int)B )
            return RBGain_hi;
        else if(_GainSum <Thr_GainSum_L3)
            return GainSum_lo;
        else if(_GainSum > Thr_GainSum_H3)
            return GainSum_hi;
        else if(abs(rgain-bgain) < Thr_GainDiff_L3)
            return GainDiff_min;
        else if(_GainMux > Thr_GainMux_H3)
            return GainMux_max;
        else if(_GainMux < Thr_GainMux_L3)
            return GainMux_min;
    }
#if 0
    else
    {
        if(rgain<Thr_GR_L4)
            return Rgain_lo;
        else if(rgain>Thr_GR_H4)
            return Rgain_hi;
        else if(bgain<Thr_GB_L4 )
            return Bgain_lo;
        else if(bgain>Thr_GB_H4)
            return Bgain_hi;
        else if(((((int)R*100)<Thr_RB_L4*(int)B)) )
            return RBGain_lo;
        else if( ((int)R*100)>Thr_RB_H4*(int)B )
            return RBGain_hi;
        else if(_GainSum <Thr_GainSum_L4)
            return GainSum_lo;
        else if(_GainSum > Thr_GainSum_H4)
            return GainSum_hi;
        else if(abs(rgain-bgain) < Thr_GainDiff_L4)
            return GainDiff_min;
    }
#endif
    return OK;
}


/**
    Modify WB Gain Manually
    @param Rin Gain of R
    @param GRin Gain of GR
    @param GBin Gain of GB
    @param Bin Gain of B
*/
void aaa_MWBprocess(UINT8 mode)
{
    #if 1
    switch (mode)
    {
        case _IMGWBMode_Daylight:
#if Awb_Cal_On
            Rgain = (MwbTbl[1][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (MwbTbl[1][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = MwbTbl[1][0];
            Bgain = MwbTbl[1][1];
#endif
        break;
        case _IMGWBMode_Cloudy:
#if Awb_Cal_On
            Rgain = (MwbTbl[2][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (MwbTbl[2][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = MwbTbl[2][0];
            Bgain = MwbTbl[2][1];
#endif
        break;
        case _IMGWBMode_Tungsten:
#if Awb_Cal_On
            Rgain = (MwbTbl[3][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (MwbTbl[3][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = MwbTbl[3][0];
            Bgain = MwbTbl[3][1];
#endif
        break;
        case _IMGWBMode_Fluorescent1:
#if Awb_Cal_On
            Rgain = (MwbTbl[4][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (MwbTbl[4][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = MwbTbl[4][0];
            Bgain = MwbTbl[4][1];
#endif
        break;
        case _IMGWBMode_Fluorescent2:
#if Awb_Cal_On
            Rgain = (MwbTbl[5][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (MwbTbl[5][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = MwbTbl[5][0];
            Bgain = MwbTbl[5][1];
#endif
        break;
        case _IMGWBMode_Sunset:
#if Awb_Cal_On
            Rgain = (MwbTbl[6][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (MwbTbl[6][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = MwbTbl[6][0];
            Bgain = MwbTbl[6][1];
#endif
        break;
        case _IMGWBMode_Customer1:
#if Awb_Cal_On
            Rgain = (CustomerTbl[0][0]*Calibration_Rg)/Golden_Rg;
            Bgain = (CustomerTbl[0][1]*Calibration_Bg)/Golden_Bg;
#else
            Rgain = CustomerTbl[0][0];
            Bgain = CustomerTbl[0][1];
#endif
        break;
        case _IMGWBMode_Customer2:
            Rgain = CustomerTbl[1][0];
            Bgain = CustomerTbl[1][1];
        break;
        case _IMGWBMode_Customer3:
            Rgain = CustomerTbl[2][0];
            Bgain = CustomerTbl[2][1];
        break;
    }
#else

    switch (mode)
    {
        case _IMGWBMode_Daylight:
            Thr_GR_L_M=115;
            Thr_GR_H_M=142;
            Thr_GB_L_M=100;
            Thr_GB_H_M=130;
            Thr_RB_L_M=80;
            Thr_RB_H_M=98;
            Thr_Saturation_L_M=8;
            Thr_Saturation_H_M=17;
            Thr_GainSum_L_M=210;
            Thr_GainSum_H_M=280 ;
            aaa_MSel_Autoprocess(_6000k);
        break;
        case _IMGWBMode_Cloudy:
            Thr_GR_L_M=125;
            Thr_GR_H_M=156;
            Thr_GB_L_M=88;
            Thr_GB_H_M=109;
            Thr_RB_L_M=62;
            Thr_RB_H_M=79;
            Thr_Saturation_L_M=14;
            Thr_Saturation_H_M=22;
            Thr_GainSum_L_M=210;
            Thr_GainSum_H_M=270;
            aaa_MSel_Autoprocess(_7500k);
        break;
        case _IMGWBMode_Tungsten:
            Thr_GR_L_M=94;
            Thr_GR_H_M=115;
            Thr_GB_L_M=130;
            Thr_GB_H_M=159;
            Thr_RB_L_M=120;
            Thr_RB_H_M=157;
            Thr_Saturation_L_M=14;
            Thr_Saturation_H_M=24;
            Thr_GainSum_L_M=210;
            Thr_GainSum_H_M=280;
            aaa_MSel_Autoprocess(_3500k);
        break;
        case _IMGWBMode_Fluorescent1:
            Thr_GR_L_M=116;
            Thr_GR_H_M=144;
            Thr_GB_L_M=121;
            Thr_GB_H_M=152;
            Thr_RB_L_M=94;
            Thr_RB_H_M=117;
            Thr_Saturation_L_M=12;
            Thr_Saturation_H_M=19;
            Thr_GainSum_L_M=220;
            Thr_GainSum_H_M=310;
            aaa_MSel_Autoprocess(_4000k);
        break;
        case _IMGWBMode_Fluorescent2:
            Thr_GR_L_M=116;
            Thr_GR_H_M=145;
            Thr_GB_L_M=109;
            Thr_GB_H_M=138;
            Thr_RB_L_M=83;
            Thr_RB_H_M=103;
            Thr_Saturation_L_M=10;
            Thr_Saturation_H_M=17;
            Thr_GainSum_L_M=210;
            Thr_GainSum_H_M=290;
            aaa_MSel_Autoprocess(_5000k);
        break;
        case _IMGWBMode_Sunset:
#if Awb_Cal_On
#else
#endif
        break;
        case _IMGWBMode_Customer1:
#if Awb_Cal_On
#else
#endif
        break;
        case _IMGWBMode_Customer2:
        break;
        case _IMGWBMode_Customer3:
        break;
    }
    #endif
    //debug_msg("MWBGain:R=%d,B=%d\r\n",Rgain,Bgain);
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for ca process
//    aaa_SetIPEColorgain(Rgain, 256, 256, Bgain);
    Ggain = 256;
    if(Rgain < 256)
    {
        Bgain = Bgain*256/Rgain;
        Ggain = Ggain*256/Rgain;
        Rgain = 256;
    }
    else if(Bgain < 256)
    {
        Rgain = Rgain*256/Bgain;
        Ggain = Ggain*256/Bgain;
        Bgain = 256;
    }
    aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
//#NT#2009/08/31#Jarkko Chang -end
}

void AwbPrv2Cap(void)
{
    UINT i,j,k,m,x,y; //,Tungsten=0,office=0,daywhite=0;
    UINT R_total=0, G_total=0, B_total=0,W_total=0;
    UINT NewRgain,NewGgain,NewBgain;
    unsigned short R_cal,G_cal,B_cal;
    UINT R_Avg,G_Avg,B_Avg;
    UINT Rg_tmp,Bg_tmp;
    UINT BlockWidth, BlockHeight;
    UINT BlockSize = 8;

    if(gImageAlgInfo.WBMode != _IMGWBMode_Auto)
    {
        CapRgain = Rgain;
        CapBgain = Bgain;
        CapGgain = Ggain;
        return ;
    }
    else
    {
        R_cal=0;G_cal=0;B_cal=0;

        AWB_LV=AE__GetLV();
        //debug_err(("Capture AWB [%x]\r\n", RawAdd));
        BlockWidth = ((GetIPPSizeInfor(_PreCropCapHsize) / BlockSize)>>1)<<1;
        BlockHeight = ((GetIPPSizeInfor(_PreCropCapVsize) / BlockSize)>>1)<<1;
        for(i=0;i<BlockSize*BlockSize;i++)
        {
            y = i / BlockSize;
            x = i - (y * BlockSize);

            CalculateRawY( ippbuf_GetBuf(IPPBUF_CAP_RAW_DARKFRAME),
                           BlockWidth * x, BlockHeight * y,
                           BlockWidth, BlockHeight,
                           8,
                           SIE_PRE_Para_d2d.PreCfaPat,
                           &R_cal,&G_cal,&B_cal);

            if(R_cal==0)
                R_cal = 1;
            if(B_cal==0)
                B_cal = 1;
            if(G_cal==0)
                G_cal = 1;

            CapR_acc[i] = R_cal;
            CapB_acc[i] = B_cal;
            CapG_acc[i] = G_cal;

            //if ( Flash_Param.FlshEna && isChangeFlashExpoSetting())
            //    gCapAWBWeight[i]=AwbCheckWhite_flash(R_cal,G_cal,B_cal);
            //else

            gCapAWBWeight[i]=AwbCheckWhite(R_cal,G_cal,B_cal,_indoor);


            if(gCapAWBWeight[i]==1)
            {

                R_total+=R_cal;
                G_total+=G_cal;
                B_total+=B_cal;
                W_total++;
            }
        }

        //debug_err(("%d %d %d %d\r\n", R_total[0], G_total[0], B_total[0], W_total[0]));
        //debug_err(("%d %d %d %d\r\n", R_total[1], G_total[1], B_total[1], W_total[1]));
        //debug_err(("%d %d %d %d\r\n", R_total[2], G_total[2], B_total[2], W_total[2]));
        //debug_err(("%d %d %d %d\r\n", R_total[3], G_total[3], B_total[3], W_total[3]));


        if (W_total>1)
        {

            NewRgain = (G_total<<8)/(R_total);//((G_total[0]+G_total[1]+G_total[2]+G_total[3])<<8)/(R_total[0]+R_total[1]+R_total[2]+R_total[3]);
            NewGgain = 256;
            NewBgain = (G_total<<8)/(B_total);

            //NewRgain=  NewRgain*AWBCompensate[4][0]/AWBCompensate[4][1];
            //NewBgain= NewBgain*AWBCompensate[4][2]/AWBCompensate[4][1];
            CapRgain=  NewRgain; //*AWBCompensate[x][0]/AWBCompensate[x][1];
            CapBgain=  NewBgain;
            CapGgain=  NewGgain;
            //debug_err(("Capture AWB c %d %d\r\n", Rgain, Bgain));
        }
        else
        {
            CapRgain = Rgain;
            CapBgain = Bgain;
            CapGgain = Ggain;
        }
    }
    debug_err(("%d   %d  %d\r\n", CapRgain,CapBgain, W_total));
}

void AwbCap2Prv(void)
{
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#modify for ca process
//    aaa_SetIPEColorgain(Rgain, 256, 256, Bgain);
    aaa_SetPREColorgain(Rgain, 256, 256, Bgain);
//#NT#2009/08/31#Jarkko Chang -end
}

BOOL UniformSceneChk(UINT32 *G,UINT32 *B,pCoordinate _coor, UINT32 diff)
{
    UINT32 i,j;
    UINT avg=0,count=0,tmp,diff_avg=0;
    UINT32  Bgain[CA_Window_Num];
    for(i=_coor->Starty;i<_coor->Endy;i++)
        for(j=_coor->Startx;j<_coor->Endx;j++)
        {
            tmp = i*CA_WINDOW_X+j;
            if(B[tmp]<1)
                B[tmp]=1;
            Bgain[tmp] = G[tmp]*100/B[tmp];
            avg+=Bgain[tmp];
            count++;
        }
    avg = (avg + count-1)/count;
    for(i=_coor->Starty;i<_coor->Endy;i++)
        for(j=_coor->Startx;j<_coor->Endx;j++)
        {
            tmp = i*CA_WINDOW_X+j;
            diff_avg = diff_avg + abs(avg - Bgain[tmp]);
        }
    diff_avg = (diff_avg + count-1)/count;
    //debug_msg("UniformSceneChk - diff_avg=%d\r\n",diff_avg);
    if(diff_avg < diff)
        return TRUE;
    else
        return FALSE;
}

#endif
