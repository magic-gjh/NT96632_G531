/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Sensor Common Module                                               *
* Description:                                                             *
* Author: Adrian                                                           *
****************************************************************************/

/** \addtogroup SEN_COMMON_Lib */
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
#include "type.h"
#include "ipe_lib.h"
#include "math.h"
#include "debug.h"
#include "sen_snr.h"
//#NT#2010/12/15#JJ Huang -end
#if (SenSNR_DSP_PREPARE_ON)
#include "ImageAlgInfor.h"
#endif// (SenSNR_DSP_PREPARE_ON)

#define ss_max(a,b) (((a) > (b)) ? (a) : (b))
#define ss_min(a, b)  (((a) < (b)) ? (a) : (b))


//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
static BOOL gbSenSNR_Enable = FALSE;
static UINT32 puiSenSNR_RawMean[4]={0}, puiSenSNR_RawSNR[4]={0};
static UINT32 puiSenSNR_Uniformity[5]={0};
static UINT32 puiSenSNR_WeightedSNR, puiSenSNR_WeightedMEAN=SenSNR_UNITY, puiSenSNR_WeightedNonUniformity;

UINT32 SenSNR_EnableSNRMeasure(BOOL bEn)
{
    gbSenSNR_Enable = bEn;

    #if (SenSNR_DSP_PREPARE_ON)
    {
        static UINT32 uiPreCapBitDepthTemp;
        if(bEn)
        {
            uiPreCapBitDepthTemp = gImageAlgInfo.PreCapBitDepth;
            gImageAlgInfo.PreCapBitDepth = _PreBitDepth_16;
        }
        else
        {
            gImageAlgInfo.PreCapBitDepth = uiPreCapBitDepthTemp;
        }

    }
    #endif// (SenSNR_DSP_PREPARE_ON)
}

UINT32 SenSNR_IsSNRMeasureEnable(void)
{
    return gbSenSNR_Enable;
}

UINT32 SenSNR_GetSNRData(SenSNR_DATA uiIndex)
{
    UINT32 uiReturnVal;
    switch(uiIndex)
    {
            case SenSNR_DATA_MEAN_CH0:
                uiReturnVal = puiSenSNR_RawMean[0];
                break;
            case SenSNR_DATA_MEAN_CH1:
                uiReturnVal = puiSenSNR_RawMean[1];
                break;
            case SenSNR_DATA_MEAN_CH2:
                uiReturnVal = puiSenSNR_RawMean[2];
                break;
            case SenSNR_DATA_MEAN_CH3:
                uiReturnVal = puiSenSNR_RawMean[3];
                break;
            case SenSNR_DATA_SNR_CH0:
                uiReturnVal = puiSenSNR_RawSNR[0];
                break;
            case SenSNR_DATA_SNR_CH1:
                uiReturnVal = puiSenSNR_RawSNR[1];
                break;
            case SenSNR_DATA_SNR_CH2:
                uiReturnVal = puiSenSNR_RawSNR[2];
                break;
            case SenSNR_DATA_SNR_CH3:
                uiReturnVal = puiSenSNR_RawSNR[3];
                break;
            case SenSNR_DATA_UNIFORMITY_C0:
                uiReturnVal = puiSenSNR_Uniformity[0];
                break;
            case SenSNR_DATA_UNIFORMITY_C1:
                uiReturnVal = puiSenSNR_Uniformity[1];
                break;
            case SenSNR_DATA_UNIFORMITY_C2:
                uiReturnVal = puiSenSNR_Uniformity[2];
                break;
            case SenSNR_DATA_UNIFORMITY_C3:
                uiReturnVal = puiSenSNR_Uniformity[3];
                break;
            case SenSNR_DATA_UNIFORMITY_C4:
                uiReturnVal = puiSenSNR_Uniformity[4];
                break;
            case SenSNR_DATA_WEIGHTED_SNR:
                uiReturnVal = puiSenSNR_WeightedSNR;
                break;
            case SenSNR_DATA_WEIGHTED_MEAN:
                uiReturnVal = puiSenSNR_WeightedMEAN;
                break;
            case SenSNR_DATA_WEIGHTED_NONUNIFORMITY:
                uiReturnVal = puiSenSNR_WeightedNonUniformity;
                break;
            default:
                debug_err(("SenSNR_GetSNRData index %d not found !!\r\n", uiIndex));
                uiReturnVal = 0;
                break;
    }
    return uiReturnVal;
}


UINT32 SenSNR_Calculate_SNR(UINT32 RawAdd,UINT32 ImgHsize,UINT32 ImgVsize,UINT32 CfaPat,UINT32 BitDepth,UINT32 BlockH,UINT32 BlockV, UINT32 *puiMean, UINT32 *puiSNR)
{
    #define SSCS_CheckUniformity 1
    UINT32 i,j,k;
    UINT32 Data,Data_Sum,Data_Avg,Data_Cnt,Data_Sub;
    UINT32 StartX,StartY;
    double fSNR[4],fSTD[4];
    double fMEAN[4], fDIFF, fSUM[4];
    UINT32 uiUniformity[5]={0};
    UINT32 uiCornerCnt;

    debug_err(("\t\tSenSNR_Calculate_SNR:\r\n"));

    StartX = (ImgHsize - BlockH)/2;
    StartX&=0xfffffffe;// safe to be small, rather than to be big
    StartY = (ImgVsize - BlockV)/2;
    StartY&=0xfffffffe;// safe to be small, rather than to be big
    BlockH&=0xfffffffe;// safe to be small, rather than to be big
    BlockV&=0xfffffffe;// safe to be small, rather than to be big
    
    if(CfaPat==IPE_PATR)
    {
    }
    else if(CfaPat==IPE_PATGR)
    {
        StartX += 1;
    }
    else if(CfaPat==IPE_PATGB)
    {
        StartY += 1;
    }
    else if(CfaPat==IPE_PATB)
    {
        StartX += 1;
        StartY += 1;
    }

    #if SSCS_CheckUniformity
    for(uiCornerCnt=0; uiCornerCnt<5; uiCornerCnt++)
    {
        UINT32 uiStartXCorner, uiStartYCorner;

        if(uiCornerCnt==0)
        {
            uiStartXCorner = StartX-(BlockH);
            uiStartYCorner = StartY-(BlockV);
        }
        else if(uiCornerCnt==1)
        {
            uiStartXCorner = StartX+(BlockH);
            uiStartYCorner = StartY-(BlockV);
        }
        else if(uiCornerCnt==2)
        {
            uiStartXCorner = StartX-(BlockH);
            uiStartYCorner = StartY+(BlockV);
        }
        else if(uiCornerCnt==3)
        {
            uiStartXCorner = StartX+(BlockH);
            uiStartYCorner = StartY+(BlockV);
        }
        else if(uiCornerCnt==4)
        {
            uiStartXCorner = StartX;
            uiStartYCorner = StartY;
        }


        fSUM[0]=0;
        fSUM[1]=0;
        fSUM[2]=0;
        fSUM[3]=0;
        Data_Cnt = 0;
        debug_err(("\t\tA. %dx%d, %dx%d, %dx%d, %dbit\r\n", ImgHsize, ImgVsize, BlockH, BlockV, uiStartXCorner, uiStartYCorner, BitDepth));
        if(BitDepth==8)
        {
            for(j=uiStartYCorner;j<(uiStartYCorner+BlockV);j+=2)
            {
                for(i=(uiStartXCorner);i<(uiStartXCorner+BlockH);i+=2)
                {
                    #if 0 //debug only
                    debug_err(("\t\tx.\tData\t%f\t%f\t%f\t%f\r\n",
                             (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i)))),
                             (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+1)))),
                             (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize)))),
                             (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize+1))))       ));
                    #endif //debug only
                    fSUM[0] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i))));
                    fSUM[1] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i+1))));
                    fSUM[2] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize))));
                    fSUM[3] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize+1))));
                    Data_Cnt++;
                }
            }
        }
        else if(BitDepth==16)
        {
            for(j=uiStartYCorner;j<(uiStartYCorner+BlockV);j+=2)
            {
                for(i=(uiStartXCorner);i<(uiStartXCorner+BlockH);i+=2)
                {
                    #if 0 //debug only
                    debug_err(("\t\tx.\tData\t%f\t%f\t%f\t%f\r\n",
                             (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i)))),
                             (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+1)))),
                             (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize)))),
                             (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize+1))))       ));
                    #endif //debug only
                    fSUM[0] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i))));
                    fSUM[1] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+1))));
                    fSUM[2] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize))));
                    fSUM[3] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize+1))));
                    Data_Cnt++;
                }
            }
        }
        else
        {
            debug_err(("SenSNR_Calculate_SNR xxx %d not supported !\r\n", BitDepth));
        }

        fMEAN[0] = fSUM[0]/(double)Data_Cnt;
        fMEAN[1] = fSUM[1]/(double)Data_Cnt;
        fMEAN[2] = fSUM[2]/(double)Data_Cnt;
        fMEAN[3] = fSUM[3]/(double)Data_Cnt;

        uiUniformity[uiCornerCnt] = ((UINT32)(fMEAN[1]*SenSNR_UNITY)+(UINT32)(fMEAN[2]*SenSNR_UNITY))/2;
    }
    #endif//SSCS_CheckUniformity

    fSUM[0]=0;
    fSUM[1]=0;
    fSUM[2]=0;
    fSUM[3]=0;
    Data_Cnt = 0;
    debug_err(("\t\t0. %dx%d, %dx%d, %dx%d, %dbit\r\n", ImgHsize, ImgVsize, BlockH, BlockV, StartX, StartY, BitDepth));

    if(BitDepth==8)
    {
        for(j=StartY;j<(StartY+BlockV);j+=2)
        {
            for(i=(StartX);i<(StartX+BlockH);i+=2)
            {
                #if 0 //debug only
                debug_err(("\t\tx.\tData\t%f\t%f\t%f\t%f\r\n",
                         (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i)))),
                         (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+1)))),
                         (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize)))),
                         (float)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize+1))))       ));
                #endif //debug only
                fSUM[0] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i))));
                fSUM[1] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i+1))));
                fSUM[2] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize))));
                fSUM[3] += (double) (*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize+1))));
                Data_Cnt++;
            }
        }
    }
    else if(BitDepth==16)
    {
        for(j=StartY;j<(StartY+BlockV);j+=2)
        {
            for(i=(StartX);i<(StartX+BlockH);i+=2)
            {
                #if 0 //debug only
                debug_err(("\t\tx.\tData\t%f\t%f\t%f\t%f\r\n",
                         (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i)))),
                         (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+1)))),
                         (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize)))),
                         (float)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize+1))))       ));
                #endif //debug only
                fSUM[0] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i))));
                fSUM[1] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+1))));
                fSUM[2] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize))));
                fSUM[3] += (double) (*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize+1))));
                Data_Cnt++;
            }
        }
    }
    else
    {
        debug_err(("SenSNR_Calculate_SNR xxx %d not supported !\r\n", BitDepth));
    }

    fMEAN[0] = fSUM[0]/(double)Data_Cnt;
    fMEAN[1] = fSUM[1]/(double)Data_Cnt;
    fMEAN[2] = fSUM[2]/(double)Data_Cnt;
    fMEAN[3] = fSUM[3]/(double)Data_Cnt;

    debug_err(("\t\t1.\tSum\t%f\t%f\t%f\t%f\tCnt=%d\r\n",fSUM[0],fSUM[1],fSUM[2],fSUM[3], Data_Cnt));
    debug_err(("\t\t2.\tMean\t%f\t%f\t%f\t%f\r\n",fMEAN[0],fMEAN[1],fMEAN[2],fMEAN[3]));
 
    fSUM[0]=0;
    fSUM[1]=0;
    fSUM[2]=0;
    fSUM[3]=0;

    if(BitDepth==8)
    {
        for(j=StartY;j<(StartY+BlockV);j+=2)
        {
            for(i=(StartX);i<(StartX+BlockH);i+=2)
            {
                fDIFF = fMEAN[0] - (double)(*((UINT8 *)(RawAdd+(j*ImgHsize+i))));
                fSUM[0] += fDIFF*fDIFF;
                fDIFF = fMEAN[1] - (double)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+1))));
                fSUM[1] += fDIFF*fDIFF;
                fDIFF = fMEAN[2] - (double)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize))));
                fSUM[2] += fDIFF*fDIFF;
                fDIFF = fMEAN[3] - (double)(*((UINT8 *)(RawAdd+(j*ImgHsize+i+ImgHsize+1))));
                fSUM[3] += fDIFF*fDIFF;
            }
        }
     }
    else if(BitDepth==16)
    {
        for(j=StartY;j<(StartY+BlockV);j+=2)
        {
            for(i=(StartX);i<(StartX+BlockH);i+=2)
            {
                fDIFF = fMEAN[0] - (double)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i))));
                fSUM[0] += fDIFF*fDIFF;
                fDIFF = fMEAN[1] - (double)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+1))));
                fSUM[1] += fDIFF*fDIFF;
                fDIFF = fMEAN[2] - (double)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize))));
                fSUM[2] += fDIFF*fDIFF;
                fDIFF = fMEAN[3] - (double)(*((UINT16 *)(RawAdd+2*(j*ImgHsize+i+ImgHsize+1))));
                fSUM[3] += fDIFF*fDIFF;
            }
        }
     }
    else
    {
        debug_err(("SenSNR_Calculate_SNR xxx %d not supported !\r\n", BitDepth));
    }

    fSTD[0] = sqrt(fSUM[0]/(double)(Data_Cnt-1));
    fSTD[1] = sqrt(fSUM[1]/(double)(Data_Cnt-1));
    fSTD[2] = sqrt(fSUM[2]/(double)(Data_Cnt-1));
    fSTD[3] = sqrt(fSUM[3]/(double)(Data_Cnt-1));
 
    fSNR[0] = 20*log10((double)(fMEAN[0])/fSTD[0]);
    fSNR[1] = 20*log10((double)(fMEAN[1])/fSTD[1]);
    fSNR[2] = 20*log10((double)(fMEAN[2])/fSTD[2]);
    fSNR[3] = 20*log10((double)(fMEAN[3])/fSTD[3]);


    debug_err(("\t\t3.\tSTD\t%f\t%f\t%f\t%f\r\n",fSTD[0],fSTD[1],fSTD[2],fSTD[3]));
    debug_err(("\t\t3.\tSNR\t%f\t%f\t%f\t%f\r\n",fSNR[0],fSNR[1],fSNR[2],fSNR[3]));

    if(puiMean!=NULL)
    {
        puiMean[0] = (UINT32)(fMEAN[0]*SenSNR_UNITY);
        puiMean[1] = (UINT32)(fMEAN[1]*SenSNR_UNITY);
        puiMean[2] = (UINT32)(fMEAN[2]*SenSNR_UNITY);
        puiMean[3] = (UINT32)(fMEAN[3]*SenSNR_UNITY);
    }
    else
    {
        puiSenSNR_RawMean[0] = (UINT32)(fMEAN[0]*SenSNR_UNITY);
        puiSenSNR_RawMean[1] = (UINT32)(fMEAN[1]*SenSNR_UNITY);
        puiSenSNR_RawMean[2] = (UINT32)(fMEAN[2]*SenSNR_UNITY);
        puiSenSNR_RawMean[3] = (UINT32)(fMEAN[3]*SenSNR_UNITY);
    }
    if(puiSNR!=NULL)
    {
        puiSNR[0] = (UINT32)(fSNR[0]*SenSNR_UNITY);
        puiSNR[1] = (UINT32)(fSNR[1]*SenSNR_UNITY);
        puiSNR[2] = (UINT32)(fSNR[2]*SenSNR_UNITY);
        puiSNR[3] = (UINT32)(fSNR[3]*SenSNR_UNITY);
    }
    else
    {
        puiSenSNR_RawSNR[0] = (UINT32)(fSNR[0]*SenSNR_UNITY);
        puiSenSNR_RawSNR[1] = (UINT32)(fSNR[1]*SenSNR_UNITY);
        puiSenSNR_RawSNR[2] = (UINT32)(fSNR[2]*SenSNR_UNITY);
        puiSenSNR_RawSNR[3] = (UINT32)(fSNR[3]*SenSNR_UNITY);
    }
    puiSenSNR_Uniformity[0] = uiUniformity[0];
    puiSenSNR_Uniformity[1] = uiUniformity[1];
    puiSenSNR_Uniformity[2] = uiUniformity[2];
    puiSenSNR_Uniformity[3] = uiUniformity[3];
    puiSenSNR_Uniformity[4] = uiUniformity[4];

    {
        puiSenSNR_WeightedSNR = ((UINT32)(fSNR[1]*SenSNR_UNITY)+(UINT32)(fSNR[2]*SenSNR_UNITY))/2;
        #if 1// 1.000 as 50/255
        puiSenSNR_WeightedMEAN = (((UINT32)(fMEAN[1]*(SenSNR_UNITY*255/100))+(UINT32)(fMEAN[2]*(SenSNR_UNITY*255/100)))/2)/(1<<BitDepth);
        #elif 0// 1.000 as peak
        puiSenSNR_WeightedMEAN = (((UINT32)(fMEAN[1]*SenSNR_UNITY)+(UINT32)(fMEAN[2]*SenSNR_UNITY))/2)/(1<<BitDepth);
        #endif
        {
            UINT32 uiMaxTmp, uiMinTmp;
            uiMaxTmp = ss_max(uiUniformity[0], ss_max(uiUniformity[1], ss_max(uiUniformity[2], ss_max(uiUniformity[3], uiUniformity[4]))));
            uiMinTmp = ss_min(uiUniformity[0], ss_min(uiUniformity[1], ss_min(uiUniformity[2], ss_min(uiUniformity[3], uiUniformity[4]))));
            puiSenSNR_WeightedNonUniformity = ((uiMaxTmp-uiMinTmp)*2000)/(uiMaxTmp+uiMinTmp);
        }
    }

    //return (puiSNR[0]+puiSNR[1]+puiSNR[2]+puiSNR[3])/4;
    return puiSenSNR_WeightedSNR;
}
//#NT#2010/12/15#JJ Huang -end




#if 0 // old custoemr request
#define TargetY_Up 230 // 10bit
#define TargetY_Down 170 // 10bit
UINT8 Cal_SNR_Calc(void)
{
    UINT32 RawAdd,Hsize,Vsize,StartX,StartY;
    UINT32 ExpT,Lum,Lum_Down,Lum_Up;
    UINT32 i;
    UINT16 R,G,B,Tol=5;
    float SNR_Up,SNR_Down,SNR;
    char str[20];
 
    Cal_AlgClearScreen();
    ExpT = 20000;
    //ide_enable_video(IDE_VIDEOID_1);
    //Set2Preview();
    SetFlag_CalLensShadingEn(TRUE);
 
    Photo_LockAE(LOCK);
    Photo_LockAWB(LOCK);
    //aaa_SetAeCalibration(1);
    Lens_Aperture_Move(0);
 
    Lens_Shutter_Move(OPEN);
    ExpoTime2TGValue_Prv(ExpT); //F4.5, LV10, ISO100
    AeSetExpoTime_Prv();
    Cal_ISO2AgcValue_Prv(100);//90
    
    Hsize = 20;
    Vsize = 10;
    StartX = (640 -20)/2;
    StartY = (268 - 10)/2;
    while(1)
    {
        SensorDelayVD(4);
        if(sie_GetDramSao(1)==sie_Read_CurrentOutputAdd())
            RawAdd = sie_GetDramSao(2);
        else
            RawAdd = sie_GetDramSao(1);
        Lum = CalculateRawY_PRV(RawAdd,StartX,StartY,20,10,1,0,&R,&G,&B);
        Lum = Lum>>6;// 10bit
        debug_err(("Lum=%d, ExpoT=%d\r\n",Lum,ExpT));
        if((Lum > (TargetY_Down-Tol)) && (Lum < (TargetY_Down+Tol)))
            break;
 
        ExpT = ((ExpT*TargetY_Down/Lum)+ExpT)/2;        
        ExpoTime2TGValue_Prv(ExpT);
        AeSetExpoTime_Prv();
    }
 

    ExpoTime2TGValue_Cap(ExpT);
    Cal_ISO2AgcValue_Cap(80);
    StopPreviewDramData();
    SensorDelayVD(1);
    //ide_disable_video(IDE_VIDEOID_1);
    //sie_Set_ColorGain(256,256,256,256);
    ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_16);
    ImgCapture_SetParameter(_SiePath,_PreColorGain);
    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
 
    SetSieInHsize(SIF_Cap_Size.H_Size);
    SetSieInVsize(SIF_Cap_Size.V_Size);
    SensorDelayVD(1);
    CalSie_Set2Capture(gCapRawAddr, GetSieCapXCenter(), GetSieCapYCenter(),
                        GetSieInHsize(), GetSieInVsize());
 
    SensorDelayVD(1);
#if 0
         FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)gCapRawAddr,//   UB *pBuf,
                    GetSieInHsize()*GetSieInVsize()*gImageAlgInfo.SieBitDepth/8,//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
#endif
    //for calculating Y with respect to CFA pattern
    StartX = (SIF_Cap_Size.H_Size -84)/2;
    StartY = (SIF_Cap_Size.V_Size - 84)/2;
 
    Lum_Down = CalculateRawY(gCapRawAddr,StartX,StartY,84,84,1,0,&R,&G,&B)>>6;// 10bit
    debug_err(("Capture Lum=%d\r\n",Lum_Down));
    SNR_Down = Calculate_SNR(gCapRawAddr,GetSieInHsize(),GetSieInVsize(),84,84);
    //sie_Cap2Preview();
    Set2Preview();
 
    SetFlag_CalLensShadingEn(TRUE);
 
    Photo_LockAE(LOCK);
    Photo_LockAWB(LOCK);
    //aaa_SetAeCalibration(1);
    Lens_Aperture_Move(0);
 
    Lens_Shutter_Move(OPEN);
    ExpoTime2TGValue_Prv(ExpT); //F4.5, LV10, ISO100
    AeSetExpoTime_Prv();
    Cal_ISO2AgcValue_Prv(100);//90
    
    Hsize = 20;
    Vsize = 10;
    StartX = (640 -20)/2;
    StartY = (268 - 10)/2;
    while(1)
    {
        SensorDelayVD(4);
        if(sie_GetDramSao(1)==sie_Read_CurrentOutputAdd())
            RawAdd = sie_GetDramSao(2);
        else
            RawAdd = sie_GetDramSao(1);
        Lum = CalculateRawY_PRV(RawAdd,StartX,StartY,20,10,1,0,&R,&G,&B);
        Lum = Lum>>6;// 10bit
        debug_err(("Lum=%d, ExpoT=%d\r\n",Lum,ExpT));
        if((Lum > (TargetY_Up-Tol)) && (Lum < (TargetY_Up+Tol)))
            break;
 
        ExpT = ((ExpT*TargetY_Up/Lum)+ExpT)/2;        
        ExpoTime2TGValue_Prv(ExpT);
        AeSetExpoTime_Prv();
    }
 

    ExpoTime2TGValue_Cap(ExpT);
    Cal_ISO2AgcValue_Cap(80);
    StopPreviewDramData();
    SensorDelayVD(1);
    //ide_disable_video(IDE_VIDEOID_1);
    //sie_Set_ColorGain(256,256,256,256);
    ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_16);
    ImgCapture_SetParameter(_SiePath,_PreColorGain);
    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
 
    SetSieInHsize(SIF_Cap_Size.H_Size);
    SetSieInVsize(SIF_Cap_Size.V_Size);
    SensorDelayVD(1);
    CalSie_Set2Capture(gCapRawAddr, GetSieCapXCenter(), GetSieCapYCenter(),
                        GetSieInHsize(), GetSieInVsize());
 
    SensorDelayVD(1);
#if 0
         FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)gCapRawAddr,//   UB *pBuf,
                    GetSieInHsize()*GetSieInVsize()*gImageAlgInfo.SieBitDepth/8,//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
#endif
    //for calculating Y with respect to CFA pattern
    StartX = (SIF_Cap_Size.H_Size -84)/2;
    StartY = (SIF_Cap_Size.V_Size - 84)/2;
 
    Lum_Up = CalculateRawY(gCapRawAddr,StartX,StartY,84,84,1,0,&R,&G,&B)>>6;// 10bit
    debug_err(("Capture Lum=%d\r\n",Lum_Up));
    SNR_Up = Calculate_SNR(gCapRawAddr,GetSieInHsize(),GetSieInVsize(),84,84);
    //sie_Cap2Preview();
 

    SNR = (SNR_Up-SNR_Down)*(150-Lum_Down)/(Lum_Up-Lum_Down)+SNR_Down;
    debug_err(("Lum Down=%d, Up=%d,T=150\r\n",Lum_Down,Lum_Up));
    debug_err(("SNR: Down=%f, Up=%f, T=%f\r\n",SNR_Down,SNR_Up,SNR));
    Set2Preview();
    
    sprintf(str,"SNR is %f",SNR);   
    Cal_AlgShowString(str,100,80); //Cal_AlgShowString(str,50,50);
 
    {
        HNVT_FILE *pFile;
 
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        pFile = fs_fopen("A:\\SNR_Info.txt","aw");
 
        //sprintf(message, "A:\\SNR%s.txt",SysGetSerialNum());
        //pFile = fs_fopen(message,"w");
 
        if(pFile != NULL)
        {
            sprintf(message, "SN %s ",SysGetSerialNum());
            fs_fwrite(message, 1, strlen(message)+1, pFile);
 
            sprintf(message, "Image [1] Lum=%d, SNR=%f ",Lum_Down,SNR_Down);
            fs_fwrite(message, 1, strlen(message)+1, pFile);
            sprintf(message, "Image [2] Lum=%d, SNR=%f ",Lum_Up,SNR_Up);
            fs_fwrite(message, 1, strlen(message)+1, pFile);
            sprintf(message, "[Lum 150] SNR=%f\n",SNR);
            fs_fwrite(message, 1, strlen(message)+1, pFile);
            fs_fclose(pFile);
            //debug_err((message));
        }
 
    }
    return E_OK;
 
}
#endif 

