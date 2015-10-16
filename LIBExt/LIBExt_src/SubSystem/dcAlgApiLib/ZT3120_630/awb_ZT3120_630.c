#include    "sensor_model.h"
#include "Cal_Alg.h"
#if ZT3120_630

UINT Rgain=256,Bgain=256,Ggain=256,OldRgain=256,OldBgain=256,OldGgain=256;
UINT gAWB_counter=0;
UINT32    R_Acc[256]={0}, G_Acc[256]={0}, B_Acc[256]={0};
STR_AAA_CA_PARA CA_para;

extern INT32 abs(INT32 i);

volatile unsigned int awb_fc=0;
unsigned int gTmpCounter=0;
UINT8 R_acc[64], G_acc[64], B_acc[64];
unsigned char  gAWBWeight[64];
//#NT#2009/10/27#Jarkko Chang -begin
//#NT#add for csc
unsigned char  gCapAWBWeight[64];
//#NT#2009/10/27#Jarkko Chang -end

unsigned int ExpValue;
unsigned char PhotoAWBEnter=0;
unsigned int Rcfg=256,Bcfg=256;
unsigned int Calibration_Rg=256,Calibration_Bg=256;
unsigned int Golden_Rg=256,Golden_Bg=256;
int K_Rgain=100,K_Bgain=100;
unsigned int Golden_Rg_Min=353,Golden_Rg_Max=381,Golden_Bg_Min=281,Golden_Bg_Max=298;
extern AE_STRUCT AE_Param;
//extern volatile Adjust_Struct   *AAA_Adjust_Info;


UINT32  gOutdoor_LV = Outdoor_LV;
UINT32  gNight_LV = Night_LV;
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
#if Awb_Cal_On
UINT8 R_acc1[64], G_acc1[64], B_acc1[64];
UINT8 cal_Rgain,cal_Bgain;
#endif

#define DIGITAL_GAIN_ON 0

#if DIGITAL_GAIN_ON
UINT DigitalGain = 384;
UINT RgainTmp=256,BgainTmp=256,GgainTmp=256;
#define NIGHT_EV_VALUE 640
#define NIGHT_EV_VALUE_HIGH_THES 1280
#endif

unsigned short MwbTbl[7][2]={
    { 256, 256}, // Auto
    { 406, 315}, // Daylight (corresponded to Premier light box: B light )
    { 350, 354}, // Cloudy   (corresponded to Premier light box: C light )
    { 242, 528}, // Tungsten (corresponded to Premier light box: A light )
    { 310, 421}, // Fluorescent1 (corresponded to Premier light box: FL1 light )
    { 310, 421}, // Fluorescent2 (corresponded to Premier light box: FL2 light )
    { 417, 256}, // Sunset
};
unsigned short CustomerTbl[3][2]={
    { 256, 256},
    { 256, 256},
    { 256, 256},
};

void AWB_ChangeRate(UB mode)
{
    switch(mode)
    {
        case AWB_RATE_LOWLIGHT:
            aaa_SetAwbRate(4);
            break;
        case AWB_RATE_STARTUP:
            aaa_SetAwbRate(4);
            break;
        case AWB_RATE_NORMAL:
        default:
            aaa_SetAwbRate(4);
            break;
    }
}
unsigned int Saturation(unsigned int RR,unsigned int GG,unsigned int BB)
{
    unsigned int Max,Min;
    if(RR>GG)
    {
        if(GG>BB)
        {
            Max=RR;
            Min=BB;
        }
        else
        {
            if(RR>BB)
            {
                Max=RR;
                Min=GG;
            }
            else
            {
                Max=BB;
                Min=GG;
            }
        }
    }
    else
    {
        if(RR>BB)
        {
            Max=GG;
            Min=BB;
        }
        else
        {
            if(GG>BB)
            {
                Max=GG;
                Min=RR;
            }
            else
            {
                Max=BB;
                Min=RR;
            }
        }
    }
    //$$
    if((Max+Min)<1)
       return(100*(Max-Min)/(1+Max+Min));
    return(100*(Max-Min)/(Max+Min));
}

/**
    Initialize AWB
*/
void aaa_InitRBgain(void)
{
    if(AE__GetLV()<gOutdoor_LV)
    {
        Rgain =369;
        Ggain =256;
        Bgain =357;
    }
    else
    {
        Rgain =360;
        Ggain =269;
        Bgain =340;
    }
    PhotoAWBEnter=1;

    #if Awb_Cal_On
    if((AAA_Adjust.AWB.Rgain==0)||(AAA_Adjust.AWB.Bgain==0))
    {
        K_Rgain=100;
        K_Bgain=100;
        return;
    }

    Calibration_Rg=AAA_Adjust.AWB.Rgain;
    Calibration_Bg=AAA_Adjust.AWB.Bgain;

    //debug_msg("Calibration_Rg=%d,Calibration_Bg=%d\r\n",Calibration_Rg,Calibration_Bg);

    //temperally way for average golden sample value, need to change after release final calibration fw.
    if(1)//(AAA_Adjust_Info->FTP_Status==0x21/*_FTP_Status*/)
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
    else
    {
        K_Rgain=100;
        K_Bgain=100;
    }

    /*
    if((Calibration_Rg>Golden_Rg_Max) || (Calibration_Rg<Golden_Rg_Min))
        K_Rgain=(100*Golden_Rg)/Calibration_Rg;
    else
        K_Rgain=100;
    if((Calibration_Bg>Golden_Bg_Max) || (Calibration_Bg<Golden_Bg_Min))
        K_Bgain=(100*Golden_Bg)/Calibration_Bg;
    else
        K_Bgain=100;
    */
    //K_Rgain=(100*Calibration_Rg)/Golden_Rg;
    //K_Bgain=(100*Calibration_Bg)/Golden_Bg;

    /*
    if((Calibration_Rg<K_Rmin)&&(Calibration_Rg>305))
        K_Rgain=256+20*(K_Rmin-Calibration_Rg)/(K_Rmin-305);
    else
        K_Rgain=256;

    if((Calibration_Rg>K_Rmax)&&(Calibration_Rg<435))
        K_Rgain=256-20*(Calibration_Rg-K_Rmax)/(435-K_Rmax);
    else
        K_Rgain=256;

    if((Calibration_Bg<K_Bmin)&&(Calibration_Bg>245))
        K_Bgain=256+20*(K_Bmin-Calibration_Bg)/(K_Bmin-245);
    else
        K_Bgain=256;

    if((Calibration_Bg>K_Bmax)&&(Calibration_Bg<375))
        K_Bgain=256-20*(Calibration_Bg-K_Bmax)/(375-K_Bmax);
    else
        K_Bgain=256;
    */
#endif
}

void aaa_InitAWB(void)
{
    aaa_SetAwbRate(4);
//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca output
    if (pre_checkFunctionEnable(PRE_CG_EN))
    {
	    aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
	}
	else
	{
	    aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
	}
//#NT#2009/10/16#Jarkko Chang -end
}

/**
    AWB Algorithm Sample
*/
void aaa_CAprocess(void)
{
//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for ca process
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
//    aaa_Get_CA_RSLT(&CA_para);
    if (pre_checkFunctionEnable(PRE_CA_EN))
    {
        aaa_GetCAResult(&CA_para, 8, SENSOR_OUT_PROGRESSIVE);
    }
    else
    {
        aaa_GetCAResult(&CA_para, gImageAlgInfo.PrePrvBitDepth, SENSOR_OUT_PROGRESSIVE);
    }
//#NT#2009/10/16#Jarkko Chang -end
}

//#NT#2009/10/27#Jarkko Chang -begin
//#NT#add for csc
/*
void aaa_CapAWBprocess(UINT RawAdd, UINT startpixel)
{
    UINT i,j,k,m,x,y; //,Tungsten=0,office=0,daywhite=0;
    UINT R_total=1, G_total=1, B_total=1,W_total=0;
    UINT NewRgain,NewGgain,NewBgain;
    UINT R_cal,G_cal,B_cal;
    UINT R_Avg,G_Avg,B_Avg;
    UINT BlockWidth, BlockHeight;
    UINT BlockSize_w=256,BlockSize_v = 192;
    UINT _tmpR,_tmpB;
    UINT Rg_tmp,Bg_tmp,Rg_temp,Bg_temp;
    if(gImageAlgInfo.WBMode != _IMGWBMode_Auto)
    {
        return ;
    }
    else
    {
        R_cal=0;G_cal=0;B_cal=0;
        AWB_LV=AE__GetLV();
        //debug_err(("Capture AWB [%x]\r\n", RawAdd));
        BlockWidth = (GetIPPSizeInfor(_PreCropCapHsize) / BlockSize_w);//8
        BlockHeight = (GetIPPSizeInfor(_PreCropCapVsize) / BlockSize_v);//8
        for(i=0;i<BlockWidth*BlockHeight;i++)
        {
            y = i / BlockWidth;
            x = i - (y * BlockWidth);

            CalculateRawY( RawAdd,
                           BlockSize_w * x, BlockSize_v * y,
                           BlockSize_w, BlockSize_v,
                           8,
                           startpixel,
                           &R_cal,&G_cal,&B_cal);


            if(R_cal==0)
                R_cal = 1;
            if(B_cal==0)
                B_cal = 1;
            if(G_cal==0)
                G_cal = 1;
#if Awb_Cal_On
            R_acc1[i]=(R_cal*K_Rgain)/100;
            G_acc1[i]=G_cal;
            B_acc1[i]=(B_cal*K_Bgain)/100;

            //debug_msg("R_acc1[%2d],G_acc1[%2d],B_acc1[%2d]] (%5d,%5d)=%5d,%5d,%5d\r\n",i,i,i, K_Rgain,K_Bgain,R_acc1[i], G_acc1[i], B_acc1[i]);
#endif
            G_acc[i] = G_cal;
            B_acc[i] = B_cal;
            R_acc[i] = R_cal;

            gCapAWBWeight[i]=AwbCheckWhite(R_acc1[i],G_acc1[i],B_acc1[i]);
            //debug_err(("Capture AWB[%d-%d] [%d-%d %dx%d] %d %d %d\r\n", x, y, BlockWidth * x, BlockHeight * y, BlockWidth, BlockHeight, R_cal, G_cal, B_cal));
#if 0
            ide_enable_video(IDE_VIDEOID_1);
            sprintf(message,"%d %d  ", Calibration_Rg,Calibration_Bg);
    SenFP_ShowOSDString(message,20,20,2);
     sprintf(message,"%d %d  ", K_Rgain,K_Bgain);
    SenFP_ShowOSDString(message,20,80,2);
#endif
            if(gCapAWBWeight[i]==1)
            {


                R_total+=R_cal;
                G_total+=G_cal;
                B_total+=B_cal;
                W_total++;
            }
        }

        if(W_total > 0)
        {
            R_Avg = R_total/W_total;
            G_Avg = G_total/W_total;
            B_Avg = B_total/W_total;

            Rg_tmp=100*G_Avg/R_Avg;
            Bg_tmp=100*G_Avg/B_Avg;

            for(i=0;i<BlockWidth*BlockHeight;i++)
            {
                if(gCapAWBWeight[i]==1)
                {
                    Rg_temp = 100*G_acc[i]/R_acc[i];
                    Bg_temp = 100*G_acc[i]/B_acc[i];
                    if(abs(Rg_tmp-Rg_temp)>17)
                    {
                        R_total-=R_acc[i];
                        G_total-=G_acc[i];
                        B_total-=B_acc[i];
                        W_total--;
                        gCapAWBWeight[i]=OutofRange;
                        continue;
                    }
                    else if(abs(Bg_tmp- Bg_temp)>17)
                    {
                        R_total-=R_acc[i];
                        G_total-=G_acc[i];
                        B_total-=B_acc[i];
                        W_total--;
                        gCapAWBWeight[i]=OutofRange;
                    }

                    if(gCapAWBWeight[i]==1)
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
                                gCapAWBWeight[i] = Modify_RB;
                            }
                        }
                    }
                    #if 0
                    if((G_acc[i]>MaxGValue)&&(gAWBWeight[i])>0)
                    {
                        MaxGValue=G_acc[i];
                        MaxGIndex=i;
                        MaxGFlag=1;
                    }
                    #endif
                }
            }
        }

        if(W_total > 0)
        {
            if(R_total<1)
                R_total=1;
            if(G_total<1)
                G_total=1;
            if(B_total<1)
                B_total=1;

            NewRgain = (G_total<<8)/R_total;
            NewGgain = 256;
            NewBgain = (G_total<<8)/B_total;
#if 1
            if(NewRgain < 256)
            {
                NewGgain = (NewGgain*256+(NewRgain-1))/NewRgain;
                NewBgain = (NewBgain*256+(NewRgain-1))/NewRgain;
                NewRgain = 256;
            }
            else if(NewBgain < 256)
            {
                NewGgain = (NewGgain*256+(NewBgain-1))/NewBgain;
                NewRgain = (NewRgain*256+(NewBgain-1))/NewBgain;
                NewBgain = 256;
            }


            if(0)//(NewBgain > (88+NewRgain))
            {   //foxconn want a little bit red in low color temperature
                NewRgain = NewRgain*110/100;
                NewBgain = NewBgain *96/100;
            }
            //NewRgain = NewRgain*98/100;
            //NewBgain = NewBgain*102/100;
#endif
            Rgain = NewRgain;
            Bgain = NewBgain;
            Ggain = NewGgain;//(NewGgain + OldGgain*2)/3;
        }

        //debug_err(("%d %d %d %d\r\n", R_total[0], G_total[0], B_total[0], W_total[0]));
        //debug_err(("%d %d %d %d\r\n", R_total[1], G_total[1], B_total[1], W_total[1]));
        //debug_err(("%d %d %d %d\r\n", R_total[2], G_total[2], B_total[2], W_total[2]));
        //debug_err(("%d %d %d %d\r\n", R_total[3], G_total[3], B_total[3], W_total[3]));
    }
    debug_err(("%d   %d  %d\r\n", Rgain,Bgain, W_total));
}
//#NT#2009/10/27#Jarkko Chang -end
*/

void aaa_AWBprocess(void)
{
    UINT i,W;
    UINT R_total=1, G_total=1, B_total=1,W_total=0;
    UINT NewRgain,NewBgain,NewGgain;
    UINT R_cal,G_cal,B_cal;
    UINT R_Avg,G_Avg,B_Avg;
    UINT Rg_tmp,Bg_tmp,Rg_temp,Bg_temp;
    UINT _tmpR,_tmpB;
    BOOL _flag1 =0, _flag2 = 0,_flag3 = 0;
    UINT _tmp=0;

return ;
    if(gImageAlgInfo.WBMode != _IMGWBMode_Auto)
    {
        aaa_MWBprocess(gImageAlgInfo.WBMode);
        return;
    }



    gAWB_counter++;
    aaa_CAprocess();
    AWB_LV=AE__GetLV();
RESTART:
    for(i=0;i<CA_Window_Num;i++)
    {
#if Awb_Cal_On
            R_cal=((CA_para.R_Data[i])*K_Rgain)/100;
            G_cal=(CA_para.G_Data[i]);
            B_cal=((CA_para.B_Data[i])*K_Bgain)/100;


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
            gAWBWeight[i]=AwbCheckWhite(R_cal,G_cal,B_cal);
#endif
        if(gAWBWeight[i]==1)
        {
            R_total+=R_cal;
            G_total+=G_cal;
            B_total+=B_cal;
            W_total+=gAWBWeight[i];
        }
    }
#if 1
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
#endif
 //  if(W_total > 5)
  if(W_total > 5)
    {
        if(R_total<1)
            R_total=1;
        if(B_total<1)
            B_total=1;
        NewRgain = (G_total<<8)/R_total;
        NewGgain = 256;
        NewBgain = (G_total<<8)/B_total;
#if 0
        if(NewRgain<1)
            NewRgain=1;
        if(NewBgain<1)
            NewBgain=1;

        if(NewRgain < 256)
        {
            NewGgain = (NewGgain*256+(NewRgain-1))/NewRgain;
            NewBgain = (NewBgain*256+(NewRgain-1))/NewRgain;
            NewRgain = 256;
        }
        else if(NewBgain < 256)
        {
            NewGgain = (NewGgain*256+(NewBgain-1))/NewBgain;
            NewRgain = (NewRgain*256+(NewBgain-1))/NewBgain;
            NewBgain = 256;
        }

        //under a light enviroment
        if(NewBgain > (88+NewRgain))
        {
            NewRgain = NewRgain*113/100;
            NewBgain = NewBgain *96/100;
        }
        else
        {
            NewRgain = NewRgain*102/100;
            NewBgain = NewBgain*103/100;
        }
        // Let sky more blue
        if(AWB_LV > Sunlight_LV)
        {
            NewRgain = NewRgain*97/100;
            NewBgain = NewBgain * 103/100;
        }
#endif
        if(PhotoAWBEnter==1)
        {
            OldRgain=NewRgain;
            OldBgain=NewBgain;
            OldGgain=NewGgain;
            PhotoAWBEnter=0;
        }

        Rgain = (NewRgain+OldRgain*7)/8;
        Bgain = (NewBgain+OldBgain*7)/8;
        Ggain = NewGgain;

#if MASSAGE_ON_DIS_GAIN
        if(Rgain != OldRgain || Bgain != OldBgain)
            _flag1 = 1;
#endif

        OldRgain=Rgain;
        OldBgain=Bgain;
        OldGgain=Ggain;

        if(_flag3 == TRUE)
            gAWBWeight[0] = Restart;

//            Ggain=250;

//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca output
        //digital gain
//#NT#2009/12/02#JJ Huang -begin
//        Rgain = (Rgain *255)/245;
//        Ggain = (Ggain *255)/245;
//        Bgain = (Bgain *255)/245;
//#NT#2009/12/02#JJ Huang -end
/*
        debug_err(("awb R,B gain = %d,%d\r\n",Rgain,Bgain));

        sprintf(message, "%d, %d, %d",Rgain,Ggain,Bgain);
        if(SensorInfor.FP_ShowOSDString != NULL && SensorInfor.FP_ClearOSD != NULL)
        {
            SensorInfor.FP_ClearOSD();
            SensorInfor.FP_ShowOSDString (message,20,20,4);
        }


*/

#if DIGITAL_GAIN_ON
      //debug_err(("AE__GetLV() = %d\n\r",AE__GetLV()));
        if(AE_Param.EV_Value <= NIGHT_EV_VALUE)
       {
  		        Rgain =390;
        		Ggain =256;
        		Bgain =351;


		       RgainTmp=(Rgain*DigitalGain)/256;
        		GgainTmp=(Ggain*DigitalGain)/256;
        		BgainTmp=(Bgain*DigitalGain)/256;

			Rgain =RgainTmp;
        		Ggain =GgainTmp;
        		Bgain =BgainTmp;
	        if (pre_checkFunctionEnable(PRE_CG_EN))
	        {
	           aaa_SetPREColorgain(RgainTmp, GgainTmp,GgainTmp, BgainTmp);
	        }
	        else
	        {
	             aaa_SetIPEColorgain(RgainTmp, GgainTmp,GgainTmp, BgainTmp);
	        }
		 //aaa_AeLock(LOCK);
		// aaa_AwbLock(LOCK);

        }else  if(AE_Param.EV_Value > NIGHT_EV_VALUE_HIGH_THES)
       {
                        Rgain = OldRgain;
        		  Bgain = OldBgain;
        		 Ggain = OldGgain;
	   	  if (pre_checkFunctionEnable(PRE_CG_EN))
	        {
	           aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
	        }
	        else
	        {
	             aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
	        }
	}
#else

        if (pre_checkFunctionEnable(PRE_CG_EN))
        {
           aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
        }
        else
        {
             aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
        }
#endif


//#NT#2009/10/16#Jarkko Chang -end
    }
    else
    {
#if MASSAGE_ON_DIS_GAIN
        _flag1 = 0;
#endif
        //If outdoor without gray point and R > B
        if(AWB_LV >= gOutdoor_LV)
        {
            Rgain = OldRgain;
            Bgain = OldBgain;
            Ggain = OldGgain;
            if(Rgain < Bgain*95/100)
            {
                Rgain = Rgain * 102/95;
                Bgain = Bgain * 85/100;
            }
            if(Rgain < Bgain)
            {
                //Rgain = Rgain * 102/95;
                Bgain = Bgain * 85/100;
            }
            gAWBWeight[0] = Special_case;
#if 0
    if(SensorInfor.FP_ShowOSDString != NULL && SensorInfor.FP_ClearOSD != NULL)
    {
//    SenFP_ClearOSD();
        sprintf(message,"Special case");
//    SenFP_ShowOSDString(message,20,100,4);
        SensorInfor.FP_ClearOSD();
        SensorInfor.FP_ShowOSDString (message,20,100,4);
    }
#endif
        }
	#if 1
        else if( (AWB_LV <= (gNight_LV+20) ) && (W_total < 2) )

         {
           		 Rgain =235;
        		Ggain =256;
        		Bgain =540;

           /* Rgain = OldRgain;
            Bgain = OldBgain;
            Ggain = OldGgain;
            if(Rgain > Bgain*95/100)
            {
                Rgain = Rgain * 85/100;
                Bgain = Bgain * 102/95;
            }
            if(Rgain > Bgain)
            {
                //Rgain = Rgain * 102/95;
                Rgain = Rgain * 85/100;
            }*/

            gAWBWeight[0] = Special_case;

        }
	#endif

        if(_flag3 == TRUE)
        {
            gAWBWeight[0] = Restart;
            gOutdoor_LV = Outdoor_LV;
        }
//        Ggain=250;

//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca output
#if DIGITAL_GAIN_ON
      //debug_err(("AE__GetLV() = %d\n\r",AE__GetLV()));
        if(AE_Param.EV_Value <= NIGHT_EV_VALUE)
       {
  		        Rgain =390;
        		Ggain =256;
        		Bgain =351;


		       RgainTmp=(Rgain*DigitalGain)/256;
        		GgainTmp=(Ggain*DigitalGain)/256;
        		BgainTmp=(Bgain*DigitalGain)/256;

			Rgain =RgainTmp;
        		Ggain =GgainTmp;
        		Bgain =BgainTmp;
	        if (pre_checkFunctionEnable(PRE_CG_EN))
	        {
	           aaa_SetPREColorgain(RgainTmp, GgainTmp,GgainTmp, BgainTmp);
	        }
	        else
	        {
	             aaa_SetIPEColorgain(RgainTmp, GgainTmp,GgainTmp, BgainTmp);
	        }
		 //aaa_AeLock(LOCK);
		// aaa_AwbLock(LOCK);

        }else  if(AE_Param.EV_Value > NIGHT_EV_VALUE_HIGH_THES)
       {
                        Rgain = OldRgain;
        		  Bgain = OldBgain;
        		 Ggain = OldGgain;
	   	  if (pre_checkFunctionEnable(PRE_CG_EN))
	        {
	           aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
	        }
	        else
	        {
	             aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
	        }
	}
#else
  		        Rgain =390;
        		Ggain =256;
        		Bgain =351;
        if (pre_checkFunctionEnable(PRE_CG_EN))
        {
           aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
        }
        else
        {
             aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
        }
#endif

//#NT#2009/10/16#Jarkko Chang -end
    }

  /*     if(AE_Param.EV_Value < AE_LOWLIGHT_EV_THRES)
    {
        AWB_ChangeRate(AE_RATE_LOWLIGHT);
    }
    else
    {
        AWB_ChangeRate(AE_RATE_NORMAL);
    }*/

#if MASSAGE_ON_DIS_LV
        if(AWB_LV != Old_AWB_LV)
            _flag2 = 1;
        Old_AWB_LV = AWB_LV;
#endif

#if MASSAGE_ON_DIS_GAIN
        if(_flag1 ==1)
        {
            if(SensorInfor.FP_ShowOSDString != NULL && SensorInfor.FP_ClearOSD != NULL)
            {
                sprintf(message,"%d %d,%d %d,%d %d", OldRgain, OldBgain,NewRgain,NewBgain,Rgain, Bgain);
                SensorInfor.FP_ClearOSD();
                SensorInfor.FP_ShowOSDString (message,20,60,4);
            }
            _flag1 = 0;
        }
#endif
#if MASSAGE_ON_DIS_LV
        if(_flag2 ==1)
        {
            if(SensorInfor.FP_ShowOSDString != NULL && SensorInfor.FP_ClearOSD != NULL)
            {
                sprintf(message,"LV:%d",AWB_LV);
                SensorInfor.FP_ClearOSD();
                SensorInfor.FP_ShowOSDString (message,20,100,4);
            }
            _flag2 = 0;
        }
#endif

#if 0
    if(SensorInfor.FP_ShowOSDString != NULL && SensorInfor.FP_ClearOSD != NULL)
    {
        sprintf(message,"W:%d R:%d B:%d", W_total, Rgain, Bgain);
        SensorInfor.FP_ClearOSD();
        SensorInfor.FP_ShowOSDString (message,20,60,4);
    }
    if((gAWB_counter%5)==0)
    {
        debug_err(("AWB:Rgain=%3d,Ggain=%3d,Bgain=%3d\r\n",
                    Rgain,
                    Ggain,
                    Bgain));
        debug_err(("----------------------------\r\n"));
    }
#endif
}

/**
    Check if a point (R,G,B) white
    @param R inputR
    @param G inputG
    @param B inputB
    @return 1:white 0:not white
*/
INT8 AwbCheckWhite(UINT R,UINT G,UINT B)
{
    UINT    _GainSum,_RBGainSum,_RBGainDelta,_DiffRBGain;
    UINT    _Saturation;
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

#if 0
    if((rgain<125)&&(bgain<115))
        return R_BGain_lo;

    if( (rgain>135 && bgain > 127) )
        return R_BGain_hi;
#endif
    _GainSum = rgain + bgain;
    _RBGainSum = (100*(R*R+B*B)/(R*B));
    _RBGainDelta = abs(_GainSum-_RBGainSum);
    _Saturation = Saturation(R,G,B);
    _DiffRBGain = (abs(rgain-bgain));
    if(AWB_LV<=gNight_LV)
   {
        if(rgain<Thr_GR_L1)
            return Rgain_lo;
        else if(rgain>Thr_GR_H1)
            return Rgain_hi;
        else if(bgain<Thr_GB_L1)
            return Bgain_lo;
        else if(bgain>Thr_GB_H1)
            return Bgain_hi;
        else if(((((int)R*100)<Thr_RB_L1*(int)B)) )
            return RBGain_lo;
        else if( ((int)R*100)>Thr_RB_H1*(int)B )
            return RBGain_hi;
        else if(_GainSum <Thr_GainSum_L1)
            return GainSum_lo;
        else if(_GainSum > Thr_GainSum_H1)
            return GainSum_hi;
        else if(_Saturation <Thr_S_L1)
            return S_lo;
        else if(_Saturation > Thr_S_H1)
            return S_hi;
        else if(_RBGainSum <Thr_RBSum_L1)
            return RBSum_lo;
        else if(_RBGainSum > Thr_RBSum_H1)
            return RBSum_hi;
        else if(_RBGainDelta <Thr_Delta_L1)
            return Delta_lo;
        else if(_RBGainDelta > Thr_Delta_H1)
            return Delta_hi;
    }
   else if(AWB_LV<gOutdoor_LV)
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
        else if(_Saturation <Thr_S_L2)
            return S_lo;
        else if(_Saturation > Thr_S_H2)
            return S_hi;
        else if(_RBGainSum <Thr_RBSum_L2)
            return RBSum_lo;
        else if(_RBGainSum > Thr_RBSum_H2)
            return RBSum_hi;
        else if(_RBGainDelta <Thr_Delta_L2)
            return Delta_lo;
        else if(_RBGainDelta > Thr_Delta_H2)
            return Delta_hi;
    }
    else
    {
        if(rgain<Thr_GR_L3)
            return Rgain_lo;
        else if(rgain>Thr_GR_H3)
            return Rgain_hi;
        else if(bgain<Thr_GB_L3)
            return Bgain_lo;
        else if(bgain>Thr_GB_H3)
            return Bgain_hi;
        else if(((((int)R*100)<Thr_RB_L3*(int)B)) )
            return RBGain_lo;
        else if( ((int)R*100)>Thr_RB_H3*(int)B )
            return RBGain_hi;
        else if(_GainSum <Thr_GainSum_L3)
            return GainSum_lo;
        else if(_GainSum > Thr_GainSum_H2)
            return GainSum_hi;
        else if(_Saturation <Thr_S_L3)
            return S_lo;
        else if(_Saturation > Thr_S_H3)
            return S_hi;
        else if(_RBGainSum <Thr_RBSum_L3)
            return RBSum_lo;
        else if(_RBGainSum > Thr_RBSum_H2)
            return RBSum_hi;
        else if(_RBGainDelta <Thr_Delta_L3)
            return Delta_lo;
        else if(_RBGainDelta > Thr_Delta_H3 )
            return Delta_hi;
    }
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
//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca output
    if (pre_checkFunctionEnable(PRE_CG_EN))
    {
	    aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
	}
	else
	{
	    aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
	}
//#NT#2009/10/16#Jarkko Chang -end
}

void AwbPrv2Cap(void)
{
    //assign awb gain
    //Rgain = 256;
    //Bgain = 256;
}

void AwbCap2Prv(void)
{
//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca output
    if (pre_checkFunctionEnable(PRE_CG_EN))
    {
	    aaa_SetPREColorgain(Rgain, Ggain, Ggain, Bgain);
	}
	else
	{
	    aaa_SetIPEColorgain(Rgain, Ggain, Ggain, Bgain);
	}
//#NT#2009/10/16#Jarkko Chang -end
}
#endif
