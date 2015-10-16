#include    <stdio.h>
#include    <string.h>
#include    "kernel.h"
#include    "..\Cal_Header.h" // There is too many Cal_Header.h under Calibration/.
#include    "Cal_MI5100_630.h"
#include    "cal_common_630.h"
#include    "ImageAlgInfor.h"
#include    "KeyScanTsk.h"
#include    "filesystsk.h"
#include    "SysCfg.h"
#include    "PrimaryTsk.h"
#include    "KeyScanTsk.h"
#include    "AppInit.h"
#include    "SieCapTsk.h"
#include    "aaa_lib.h"
#include    "PhotoTsk.h"
#include    "Lens.h"
#include    "Cal_Alg.h"
#include    "interrupt.h"
#include    "DMA.h"
#include    "Cal_Alg.h"
#include    "Cal_Api.h"
#include    "pre_lib.h"


#if (_CALIBRATION_MODE_ == ENABLE)
//extern SIZE_INFOR SIF_Cap_Size;
INT8 gS0[] = "0.DP Search";
INT8 gS1[] = "1.AWB Adjust";
INT8 gS2[] = "2.ISO Adjust";
INT8 gS3[] = "3.Mshutter Adjust";
INT8 gS4[] = "4.Flash Y";
INT8 gS5[] = "5.Read Status";
INT8 gS6[] = "6.Default";
INT8 gS7[] = "7.Burning System";
INT8 gS8[] = "8.Burning Flash";
INT8 gS9[] = "9.Clear calibration Data";
INT8 gCalStr[5][10];
UINT32 CalExpTime=36, CalAgc=0,CalExpTimePxl=0xffff;
UINT8 CalExpoFrame=0;
UINT32 CalNandAdd[4]={AdjustDataNandAdd0,AdjustDataNandAdd1,AdjustDataNandAdd2,AdjustDataNandAdd3};
const UINT8 AE_Window_EqualWeighted[8][8]={
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

__align(4)unsigned long CalGammaLUT_T1[33]={
  0, 24, 48, 69, 87,101,113,123,
131,139,147,154,160,166,172,178,
183,189,194,198,203,207,211,215,
218,223,227,231,236,241,245,251,
255};

__align(4)UINT8 CalIpeGamma_S1[20]={
   0,   13,   26,   41,   56,   73,   91,  110,
 128,  146,  163,  179,  195,  211,  226,  241,
 255,    0,    0,    0,
};

IPL_OBJ IplObj;
UINT32 Cal_Mshutter=1000;//Default_Mshutter;
UINT8 LT_FirstTime;
UINT8 L1_item=0,L2_item=0,Level=1,RunFinish=0;
UINT8 LT_DisplayWin=0;
//UINT8 MShutterAdjFlag = FALSE;

extern UINT gCapRawAddr;
extern UINT YPreFlash;
extern BOOL AUTO_OB;
extern BOOL CCD_VGA_OUTPUT;//JJ 20050908
extern volatile unsigned long cap_cnt;
extern UINT g_Idx_n;
extern volatile Adjust_Struct AAA_Adjust;

//extern void Sensor_AFE_CFG(UINT32 addr, UINT32 value);
//extern void Sensor_TG_CFG(UINT32 addr, UINT32 value);
extern void Sensor_Cap2Prv(void);
extern void Init_Sensor_TG(void);
//JJ marked: Not necessary//extern  void Lens_Motor_CFG(UINT32 addr, UINT32 value);
extern  void flash_ConfigMainFlash(UINT16 uiDelayLine, UINT32 uiOnTimeUs);
extern    void flash_TriggerFlash(void);
extern  BOOL GetSubNoiseFlag (void);
extern  void Sensor_SetMode(unsigned char ucMode);

void ShowCalItem(char item);
void CalibrationItem(char item);
UINT32 WaitAeStable(UINT32 ExpectY, UINT32 ISOValue);
UINT32 WaitAeStable_WithTimeOut(UINT32 ExpectY, UINT32 ISOValue, UINT8 MaxCnd);
UINT32 WaitAeStableISO(UINT32 ExpectY, UINT32 ExposureLine);
UINT CalculateRawY2(UINT Add, UINT lineOffset, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT sample,unsigned char StartPixel,unsigned short *R,unsigned short *G,unsigned short *B);

UINT32 CAL_DataAvg(UINT32 addr, UINT16 Hstart, UINT16 Vstart, UINT16 Hsize, UINT16 Vsize, UINT16 lineOffset)
{
    UINT32 sum, cnt, addrTmp, dataTmp;
    UINT16 i, j;

    for(j=0; j<Vsize; j++)
    {
        for(i=0; i<Hsize; i++)
        {
            addrTmp = addr + (Vstart+j)*lineOffset + (Hstart+i);
            dataTmp = *(volatile UINT8*)(addrTmp);

            sum += dataTmp;
            cnt ++;
        }
    }

    return sum/cnt;
}


#define MIN(A,B)  (((A) < (B)) ? (A) : (B))
void SysInfo_UpdatePresetWB(UINT16 *wbR, UINT16 *wbG, UINT16 *wbB)
{
    UINT16 min;

    *wbR = (256*WB_COLOR_CALB_RED+(*wbR)/2) / (*wbR);
    *wbG = (256*WB_COLOR_CALB_GREEN+(*wbG)/2) / (*wbG);
    *wbB = (256*WB_COLOR_CALB_BLUE+(*wbB)/2) / (*wbB);

    min = MIN(MIN(*wbR, *wbG), *wbB);

    *wbR = (256* (*wbR)+min/2)/min;
    *wbG = (256* (*wbG)+min/2)/min;
    *wbB = (256* (*wbB)+min/2)/min;
}


void CAL_PassAdjustData(void)
{
#if 1
    debug_err(("CAL_PassAdjustData\r\n"));

    {//ISO
        UINT32 AGC_ISO50;

        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetISOAdjustData(&iISO100);

        if(AAA_Adjust.ISO_Status==_ISO_Status)
            AGC_ISO50 = (INT32)AAA_Adjust.Agc_ISOMin;
        else
            AGC_ISO50 = 0;//Default_Agc_ISO100;

        Cal_SetCalibrationData(_Cal_ISO, (UINT*)&AGC_ISO50);//JJ_433CAL: need review// IQ_ADJ_SetISOAdjustData(iISO100);
    }

    {//DP
        UINT32 uiDPTblPrv, uiDPTblCap;
        UINT32 Prv_DP[3],Cap_DP;

        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetDPAdjustData(&uiDPTblPrv, &uiDPTblCap);

        if(AAA_Adjust.DP_Status.Prv_White==_Prv_W_Status && AAA_Adjust.DP_Status.Prv_Black==_Prv_B_Status)
            uiDPTblPrv = (UINT32)&AAA_Adjust.Defect_Pixel[0];
        else
            uiDPTblPrv = 0;
        if(AAA_Adjust.DP_Status.Cap_White==_Cap_W_Status && AAA_Adjust.DP_Status.Cap_Black==_Cap_B_Status)
            uiDPTblCap = (UINT32)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX];
        else
            uiDPTblCap = 0;

        //#NT#2008/09/04#JJ Huang -Begin
        //#NT#Preview DP Status information
        Cal_SetCalibrationData(_CalStatus_PrvDP_W, (UINT*)&(AAA_Adjust.DP_Status.Prv_White));
        Cal_SetCalibrationData(_CalStatus_PrvDP_B, (UINT*)&(AAA_Adjust.DP_Status.Prv_Black));
        Cal_SetCalibrationData(_CalStatus_CapDP_W, (UINT*)&(AAA_Adjust.DP_Status.Cap_White));
        Cal_SetCalibrationData(_CalStatus_CapDP_B, (UINT*)&(AAA_Adjust.DP_Status.Cap_Black));
        //#NT#2008/09/04#JJ Huang -end
        Prv_DP[0] = AAA_Adjust.DP_Cap_Num[0] + AAA_Adjust.DP_Cap_Num[2];
        Prv_DP[1] = AAA_Adjust.DP_Cap_Num[3];
        Prv_DP[2] = AAA_Adjust.DP_Cap_Num[4];
        Cap_DP = AAA_Adjust.DP_Cap_Num[1] + AAA_Adjust.DP_Cap_Num[5];

        Cal_SetCalibrationData(_Cal_DP_PreviewNum, (UINT*)Prv_DP);
        Cal_SetCalibrationData(_Cal_DP_CaptureNum, (UINT*)&Cap_DP);
        Cal_SetCalibrationData(_Cal_DP_PreviewAddr, (UINT*)&uiDPTblPrv);
        Cal_SetCalibrationData(_Cal_DP_CaptureAddr, (UINT*)&uiDPTblCap);
    }

    {//AWB
        //UINT16 uiRvalue, uiGvalue, uiBvalue;
        UINT32 Gain[2];
        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetAWBAdjustData(&uiRvalue, &uiGvalue, &uiBvalue);

        if(AAA_Adjust.AWB_Status==_AWB_Status)
        {
#if 0
            uiRvalue = (UINT32)(0x10000)/AAA_Adjust.AWB.Rgain;
            uiGvalue = 0x100;
            uiBvalue = (UINT32)(0x10000)/AAA_Adjust.AWB.Bgain;
            SysInfo_UpdatePresetWB(&uiRvalue, &uiGvalue, &uiBvalue);
#else
            Gain[0] = AAA_Adjust.AWB.Rgain;
            Gain[1] = AAA_Adjust.AWB.Bgain;
#endif
        }
        else
        {
#if 0
            uiRvalue = 0x100;//(UINT32)(0x10000)/Default_AWB_Rgain;
            uiGvalue = 0x100;
            uiBvalue = 0x100;//(UINT32)(0x10000)/Default_AWB_Bgain;
#else
            Gain[0] = 256;
            Gain[1] = 256;
#endif
        }

        //debug_err(("AWB Data %3d %3d %3d\r\n", uiRvalue, uiGvalue, uiBvalue));
        Cal_SetCalibrationData(_Cal_AWB_Statistic, (UINT*)Gain);
        Cal_SetCalibrationData(_CalStatus_AWB, (UINT*)&(AAA_Adjust.AWB_Status));
    }
#if 0
    {//Mshtr
        #define CAL_MSHTR_DELAY_COMPENSATION 50//JJ0226 131
        UINT32 uiDelayTimeA, uiDelayTimeB;

        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetDPAdjustData(&uiDelayTimeA, &uiDelayTimeB);

        if(AAA_Adjust.Mshutter_Status[0]==_Mshutter_Status)
            uiDelayTimeA = AAA_Adjust.Mshutter[0];
        else
        {
            AAA_Adjust.Mshutter[0] = Default_Mshutter;
            uiDelayTimeA = Default_Mshutter;
        }
        if(AAA_Adjust.Mshutter_Status[1]==_Mshutter_Status)
            uiDelayTimeB = AAA_Adjust.Mshutter[1];
        else
        {
            AAA_Adjust.Mshutter[1] = Default_Mshutter2;
            uiDelayTimeB = Default_Mshutter2;
        }
        Cal_SetCalibrationData(_Cal_Mshutter, (UINT*)&uiDelayTimeA);//JJ_433CAL: need review//IQ_ADJ_SetMshtrAdjustData(uiDelayTimeA-CAL_MSHTR_DELAY_COMPENSATION, uiDelayTimeB-CAL_MSHTR_DELAY_COMPENSATION);
    }

    {//Flashlight
        UINT32 uiMagnitude;
        UINT32 uiRvalue, uiGvalue, uiBvalue;

        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetFlashAdjustData(&uiMagnitude, &uiRvalue, &uiGvalue, &uiBvalue);

        if(AAA_Adjust.FlashAE_Status==_Flash_Status)
        {
            uiMagnitude = AAA_Adjust.Flash_Y_Avg;
        }
        else
        {
            uiMagnitude = Default_Flash_Y_Avg;
        }


        if(AAA_Adjust.FlashAWB_Status==_Flash_Status)
        {
            uiRvalue = (UINT32)(0x10000)/AAA_Adjust.AWB_Flash.Rgain;
            uiGvalue = 0x100;
            uiBvalue = (UINT32)(0x10000)/AAA_Adjust.AWB_Flash.Bgain;
        }
        else
        {
            uiRvalue = (UINT32)(0x10000)/Default_Flash_AWB_Rgain;
            uiGvalue = 0x100;
            uiBvalue = (UINT32)(0x10000)/Default_Flash_AWB_Bgain;
        }

        Cal_SetCalibrationData(_Cal_Flash_AWB, (UINT*)&uiRvalue);//JJ_433CAL: need review//IQ_ADJ_SetFlashAdjustData(uiMagnitude, uiRvalue, uiGvalue, uiBvalue);
    }

    {//Focus
        UINT8 i;
        UINT32 puiFarFocusArray[15], puiNearFocusArray[15];

        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetFocusAdjustData(puiFarFocusArray, puiNearFocusArray);

        debug_err(("Cal. Status FF = %d NF = %d\r\n", AAA_Adjust.FFocus_Status, AAA_Adjust.NFocus_Status));
        if(AAA_Adjust.FFocus_Status==_FF_Status && AAA_Adjust.NFocus_Status==_NF_Status)
        {
            for(i=0; i<7; i++)
            {
                puiFarFocusArray[i] = AAA_Adjust.Far_Focus[i];
                puiNearFocusArray[i] = AAA_Adjust.Near_Focus[i];
                //debug_err(("[%2d] FF = %3d NF = %3d\r\n", i, puiFarFocusArray[i], puiNearFocusArray[i]));
            }
        }
        else
        {
            for(i=0; i<7; i++)
            {
                puiFarFocusArray[i] = Focus_Zoom_Tbl_Default[i][0];
                puiNearFocusArray[i] = Focus_Zoom_Tbl_Default[i][6];
                //debug_err(("[%2d] FF = %3d NF = %3d\r\n", i, puiFarFocusArray[i], puiNearFocusArray[i]));
            }
        }

        Cal_SetCalibrationData(_Cal_Focus_Far, (UINT*)puiFarFocusArray);//JJ_433CAL: need review//IQ_ADJ_SetFocusAdjustData(puiFarFocusArray, puiNearFocusArray);
        Cal_SetCalibrationData(_Cal_Focus_Near, (UINT*)puiNearFocusArray);//JJ_433CAL: need review

        if(AAA_Adjust.uiFocusOffsetStatus!=_Focus_Offset_Status)
        {//JJ0325 for AF offset
            AAA_Adjust.iFocusOffset[0] = 0;
            AAA_Adjust.iFocusOffset[1] = 0;
        }
    }

    {//Zoom
        UINT32 uiZoomStop2PR;

        //JJ_433CAL: no get api//Cal_GetCalibrationData(??) IQ_ADJ_GetZoomAdjustData(&uiZoomStop2PR);

        if(AAA_Adjust.Zoom_Stop2PR_Status==_Zoom_Stop2PR_Status)
        {
            uiZoomStop2PR = AAA_Adjust.uiZoomStop2PR;

        }
        else
        {
            uiZoomStop2PR = Default_Zoom_Stop2PR;
        }

        Cal_SetCalibrationData(_Cal_Zoom_Stop2PR, (UINT*)&uiZoomStop2PR);//JJ_433CAL: need review//IQ_ADJ_SetZoomAdjustData(uiZoomStop2PR);
    }
#endif
#endif
}

static UINT16 Cal_PanelMsgXPos(UINT8 Xindex)
{//3//8 columns
    if(Xindex>7)
        Xindex=7;
    return 10+30*Xindex;
}
static UINT16 Cal_PanelMsgYPos(UINT8 Yindex)
{//8 rows
    if(Yindex>7)
        Yindex=7;
    return 5+30*Yindex;
}

static UINT16 uiCalPanelMsgYPos[] = {10,180};

UINT8 IrisSwitchTest(void)
{
}

UINT8 MshutterSwitchTest(void)
{
}

UINT8 AFETest(void)
{
}

UINT8 ZoomSpeedTest(void)
{
}

UINT8 ZoomStepMissTest(void)
{
}

UINT8 ZoomPRAdjust(void)
{
}

UINT8 FocusStepMissTest(void)
{
}

UINT8 FarFocusAdjust(void)
{
}

void FocusOffsetAdjust(void)
{
}

UINT8 AFTest(void)
{
}


//20080114 Add:
UINT MShutterCapture(UINT16 InLine)
{
    UINT Lum;
    UINT16 R,G,B;
    Cal_Mshutter=(InLine*GetHDTotalPrv()*1000)/PixelClkInKHz;

    //Photo_Shutter_Move(OPEN);
    //Aperture_Go2(iAperture);

    StopPreviewDramData();

    sie_waitVD(1);

    SetIPPSizeInfor(_PreCropHsize, IMAGE_SIZE_H);//JJ_433CAL:need review// SetSieInHsize(IMAGE_SIZE_H);
    SetIPPSizeInfor(_PreCropVsize, IMAGE_SIZE_V);//JJ_433CAL:need review// SetSieInVsize(IMAGE_SIZE_V);

    SensorIF_SetOB(FALSE, 0);
    //JJ_433CAL: No set2Capture in Calibration Code//CalSie_Set2Capture(gCapRawAddr, GetSieCapXCenter(), GetSieCapYCenter(),                        GetSieInHsize(), GetSieInVsize(), 2, 0);

    Sensor_Cap2Prv();

//    Lum = CalculateRawY2(gCapRawAddr,GetSieInHsize(),0,0,GetSieInHsize(),GetSieInVsize(),2,0,&R,&G,&B);
    return G;
}

void MShutterExpTimeSet(UINT16 uiExpTimeUs)
{
    UINT32 uiExpLn, uiExpPxl, uiTmp;

    uiExpLn = (uiExpTimeUs*PixelClkInKHz/1000)/GetHDTotalPrv();
    uiTmp = uiExpTimeUs - (uiExpLn*GetHDTotalPrv()*1000/PixelClkInKHz);
    uiExpPxl = uiTmp*PixelClkInKHz/1000;

    CalSetExpTime2(uiExpLn, uiExpPxl);
    CalAeSet();
}



UINT8 MshutterAdjust(void)
{
    //#NT#2009/09/01#KS Hung -begin
    FLGPTN  uiFlag1;
    UINT BlackYavg, Yavg, rtStatus;
    //#NT#2009/09/01#KS Hung -end
    UINT8 ErrorCode;
    UINT16 RGB[3];
    UINT32 ExposureTime, AGCGain, MShutterTime, Interval;

    StopPreviewDramData();
    Set2Preview();


    Photo_LockFD(LOCK);
    Photo_LockAE(LOCK);
    Photo_LockAWB(LOCK);
    wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_2NDIDLE, TWF_ORW);
//#NT#2008/07/08#Bowen Li -begin
//#NT#fix bug(status)
    rtStatus = _Mshutter_Status;
//#NT#2008/07/08#Bowen Li -end
    ErrorCode = ERROR_OK;
    ExposureTime = 30000;
    AGCGain = 100;
    MShutterTime = 31500;
    AeSetAGCGainStep(AGCGain);
    AeSetCapExpoTime(ExposureTime);
    ShutterSet(CLOSE);
    CalCaptureOne(gCapRawAddr, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize), 0, 0x80, MShutterTime);
    BlackYavg = CalculateRawY(gCapRawAddr, 0, 0, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize), 8, 3, &RGB[0], &RGB[1], &RGB[2]);
    debug_err(("BlackYavg = %d\r\n", BlackYavg));
    ShutterSet(OPEN);

    MShutterTime = 27000;
    ExposureTime = 33333 - MShutterTime;
    Interval = ExposureTime - 20;
    while(1)
    {
        AeSetAGCGainStep(AGCGain);
        AeSetCapExpoTime(ExposureTime);

        CalCaptureOne(gCapRawAddr, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize), 0, 0x80, MShutterTime);
        Yavg = CalculateRawY(gCapRawAddr, 0, 0, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize), 8, 3, &RGB[0], &RGB[1], &RGB[2]);
        debug_err(("Yavg = %d\r\n", Yavg));

        Interval = (Interval + 1) / 2;
        if (Yavg > (Yavg + 0))
        {
            if (ExposureTime == 0)
            {
                    break;
            }
            if (ExposureTime < Interval)
            {
                ExposureTime = 0;
            }
                else
            {
                ExposureTime -= Interval;
            }
        }
        else
        {
            ExposureTime += Interval;
        }
        if (Interval == 1)
        {
            break;
        }
    }
    Cal_SetCalibrationData(_Cal_Mshutter, (UINT *)(MShutterTime + ExposureTime));
    Cal_SetCalibrationData(_CalStatus_Mshutter, &rtStatus);

}

UINT8 DP_Adjust_White(void)
{
    //#NT#2009/09/01#KS Hung -begin
    FLGPTN  uiFlag1,uiFlag;
    //#NT#2009/09/01#KS Hung -end
    UW DP_Cnt_CW = 0, DP_Cnt_CD = 0, DP_Cnt_Cap = 0;
    UW DP_Cnt_PW = 0, DP_Cnt_PD = 0, DP_Cnt_Prv = 0;
    UINT32 CapRawHSize, CapRawVSize, PrvRawHSize, PrvRawVSize;
    UINT32 RawAddr;
    UINT8 ErrorCode=ERROR_OK;
    UINT uiTmp;
    UINT32 i;

    debug_err(("DP White Adjust... \r\n"));
    debug_err(("FACE CAM. TO LIGHT BOX... \r\n"));
    debug_err(("THEN PRESS LEFT... \r\n"));
    Cal_AlgShowString("FACE CAM. TO LIGHT BOX",20,80);
    Cal_AlgShowString("THEN PRESS LEFT",20,105);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);

    memset((void*)(&AAA_Adjust.Defect_Pixel[0]), 0x0, (DP_Prv_MAX+DP_Cap_MAX)*4);
    Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_PREVIEW);

    Photo_LockAE(LOCK);
    Photo_LockAWB(LOCK);
    wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_2NDIDLE, TWF_ORW);
    AeSetPrvExpoLine(MaxExpLine_640x480);
    AeSetAGCGainISO(50);

    //disable DP function
    uiTmp = 0;

    Cal_SetCalibrationData(_CalStatus_CapDP_B, &uiTmp);
    Cal_SetCalibrationData(_CalStatus_CapDP_W, &uiTmp);
    Cal_SetCalibrationData(_CalStatus_PrvDP_B, &uiTmp);
    Cal_SetCalibrationData(_CalStatus_PrvDP_W, &uiTmp);

    AAA_Adjust.DP_Status.Prv_White = AAA_Adjust.DP_Status.Prv_Black = 0;
    AAA_Adjust.DP_Status.Cap_White = AAA_Adjust.DP_Status.Cap_Black = 0;
    DP_Cnt_PW = DP_Cnt_CW = 0;

    RawAddr = ippbuf_GetBuf(IPPBUF_PRV_END);
    debug_err(("PRV_RawAddr = 0x%x\r\n",RawAddr));

    Cal_AlgShowString("Prev", Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(0));
    Cal_AlgShowString("Dark", Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(0));
    Cal_AlgShowString("White",Cal_PanelMsgXPos(6),Cal_PanelMsgYPos(0));
    Cal_AlgShowString("FREE", Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(1));
    Cal_AlgShowString("BAD",  Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(2));
    sprintf(message, "%d", DP_Prv_MAX);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(1));
    sprintf(message, "%d", DP_Cap_MAX);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(1));
    Cal_AlgShowString("--",   Cal_PanelMsgXPos(6),Cal_PanelMsgYPos(1));

    CalPreviewOne(  RawAddr,
                    _SieBitDepth_8,
                    &PrvRawHSize,
                    &PrvRawVSize,
                    0x0);
    debug_err(("Prv SIE size = %dx%d\r\n", PrvRawHSize, PrvRawVSize));

#if 0
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
    (UB*)RawAddr,//   UB *pBuf,
    PrvRawHSize*PrvRawVSize,//JJ_UI_FileFmt
    0,//UW Pos,
    1,//BOOL CloseFile,
    FST_FMT_RAW,//UW FileFormat,
    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif

    // 3-2 DP calibration main process
    //Preview White
    debug_err(("PRV WHITE...\r\n"));
    //Cal_AlgShowString("PRV WHITE...1sec", 20, 40);
    DP_Cnt_PW = DFP_Bayer_Search(
                RawAddr+(CalInfor.SkipPrv.SkipYU*PrvRawHSize+CalInfor.SkipPrv.SkipXL),
                PrvRawHSize-CalInfor.SkipPrv.SkipXL-CalInfor.SkipPrv.SkipXR,
                PrvRawVSize-CalInfor.SkipPrv.SkipYU-CalInfor.SkipPrv.SkipYD,
                PrvRawHSize, //HSize = Line-offset
                (UW)&(AAA_Adjust.Defect_Pixel[0]),
                0/*no-crop-offset*/+CalInfor.SkipPrv.SkipXL,
                0/*no-crop-offset*/+CalInfor.SkipPrv.SkipYU,
                0,
                DP_Prv_MAX,
                CalInfor.BrightLimitPrv,
                CalInfor.BlockPrv.Block_X,
                CalInfor.BlockPrv.Block_Y);

    sprintf(message, "%d", DP_Cnt_PW);
    Cal_AlgShowString(message, Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(2));

    if(DP_Cnt_PW >= DP_Prv_MAX)
    {
        ErrorCode = ERRORCODE_DPPW_MAX;
        AAA_Adjust.DP_Status.Prv_White=0;
    }
    else
    {
        AAA_Adjust.DP_Status.Prv_White=_Prv_W_Status;
    }
    Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_PREVIEW);

    if(ErrorCode==0 || DP_SEARCH_TILL_END)
    {
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);

        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_PREVIEW;
        IPL_SetCmd(&IplObj);

        Photo_LockAE(LOCK);
        Photo_LockAWB(LOCK);
        wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_2NDIDLE, TWF_ORW);
        //Capture White
        debug_err(("CAP WHITE...\r\n"));
        //Cal_AlgShowString("CAP DARK...10sec", 20, 130);

        CalCaptureOne(  RawAddr,
                        GetIPPSizeInfor(_PreCropCapHsize),
                        GetIPPSizeInfor(_PreCropCapVsize),
                        0,
                        0xa0,
                        31500);

        CapRawHSize = GetIPPSizeInfor(_PreCropCapHsize);
        CapRawVSize = GetIPPSizeInfor(_PreCropCapVsize);
        debug_err(("Cap %dx%d\r\n", CapRawHSize, CapRawVSize));

        debug_err(("Crop ini %dx%d\r\n", pre_GetCropHIni(), pre_GetCropVIni()));

#if 0
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
        (UB*)RawAddr,//   UB *pBuf,
        CapRawHSize*CapRawVSize,//JJ_UI_FileFmt
        0,//UW Pos,
        1,//BOOL CloseFile,
        FST_FMT_RAW,//UW FileFormat,
        FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif

        DP_Cnt_CW = DFP_Bayer_Search(
                    RawAddr+(CalInfor.SkipCap.SkipYU*CapRawHSize+CalInfor.SkipCap.SkipXL),
                    CapRawHSize-CalInfor.SkipCap.SkipXL-CalInfor.SkipCap.SkipXR,
                    CapRawVSize-CalInfor.SkipCap.SkipYU-CalInfor.SkipCap.SkipYD,
                    CapRawHSize,
                    (UW)(AAA_Adjust.Defect_Pixel)+(DP_Prv_MAX<<2),
                    pre_GetCropHIni()+CalInfor.SkipCap.SkipXL,
                    pre_GetCropVIni()*SensorCapTotalFld+CalInfor.SkipCap.SkipYU,
                    0,
                    DP_Cap_MAX,
                    CalInfor.BrightLimitCap,
                    CalInfor.BlockCap.Block_X,
                    CalInfor.BlockCap.Block_Y);

        sprintf(message, "%d", DP_Cnt_CW);
        Cal_AlgShowString(message, Cal_PanelMsgXPos(6),Cal_PanelMsgYPos(2));



        if(DP_Cnt_CW >= DP_Cap_MAX)
        {
            ErrorCode = ERRORCODE_DPCW_MAX;
            AAA_Adjust.DP_Status.Cap_White=0;
        }
        else
        {
            AAA_Adjust.DP_Status.Cap_White=_Cap_W_Status;
        }
    }

    //summary
    DP_Cnt_Prv = DP_Cnt_PW;
    DP_Cnt_Cap = DP_Cnt_CW;
    AAA_Adjust.DP_Cap_Num[0] = DP_Cnt_PW;
    AAA_Adjust.DP_Cap_Num[1] = DP_Cnt_CW;
    debug_err(("Cal_DP_WHITE: DFP White = %d, %d\r\n\r\n", DP_Cnt_PW, DP_Cnt_CW));
    debug_err(("ErrorCode = %d, DP Num Prv = %d, Cap = %d\r\n", ErrorCode, DP_Cnt_Prv, DP_Cnt_Cap));

    WriteCalData();

    #if CAL_SAVE_ADJUST
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)AAA_Adjust.Defect_Pixel,//   UB *pBuf,
                    (DP_Prv_MAX+DP_Cap_MAX)*4,//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
    #endif

    //Cal_AlgClearScreen();
    //Cal_AlgShowString("DP_WHITE NUM:", 20,40);
    //sprintf(message, "PRV %d CAP %d", DP_Cnt_PW, DP_Cnt_CW);
    //Cal_AlgShowString(message, 20, 70);

    Cal_AlgShowString("DP White",Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(5));
    if((ErrorCode == ERROR_OK))
    {
        Cal_AlgShowString("PASS",Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(5));
    }
    else
    {
        Cal_AlgShowString("NG",Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(5));
    }

    //3-3 set down
    AeCap2Prv();
//    Set2Preview();
    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_PREVIEW;
    IPL_SetCmd(&IplObj);

    return ErrorCode;

}

UINT8 DP_Adjust_Dark(void)
{
    UINT32  WaitFlag;
    FLGPTN uiFlag;
    #define DP_D_EXPOTIME_PRV   60000
    #define DP_D_AGCGAIN_PRV    500//ISO400
    #define DP_D_EXPOTIME_CAP   60000
    #define DP_D_AGCGAIN_CAP    300

    //#NT#2009/09/01#KS Hung -begin
    FLGPTN uiFlag1;
    //#NT#2009/09/01#KS Hung -end
    UINT32 DP_Cnt_CW = 0, DP_Cnt_CD = 0, DP_Cnt_Cap = 0;
    UINT32 DP_Cnt_PW = 0, DP_Cnt_PD = 0, DP_Cnt_Prv = 0;
    //UINT32 DP_Cnt_PD1 = 0,DP_Cnt_PD2 = 0;
    UINT32 RawAddr;
    UINT16 CapRawHSize, CapRawVSize, PrvRawHSize, PrvRawVSize;
    UINT8 ErrorCode = ERROR_OK;
    UINT uiTmp;
    UINT8 i;

    debug_err(("DP Dark Adjust... \r\n"));
    debug_err(("CLOSE THE LIGHT... \r\n"));
    debug_err(("THEN PRESS LEFT... \r\n"));
    Cal_AlgClearScreen();
    Cal_AlgShowString("CLOSE THE LIGHT",20,80);
    Cal_AlgShowString("THEN PRESS LEFT",20,105);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    Cal_AlgClearScreen();
    Cal_AlgShowString("PROCESSIN... \r\n",20,80);


    //disable DP function
    uiTmp = 0;
#if 0
//#NT#2008/07/09#JJ Huang -begin
//#NT#Changed with Calibration-Data-Interface
    Cal_SetCalibrationData(_CalStatus_CapDP_B, &uiTmp);
    Cal_SetCalibrationData(_CalStatus_PrvDP_B, &uiTmp);

    Cal_SetCalibrationData(_CalStatus_CapDP_W, &uiTmp);
    Cal_SetCalibrationData(_CalStatus_PrvDP_W, &uiTmp);
#endif
//#NT#2008/07/09#JJ Huang -end

    AAA_Adjust.DP_Status.Prv_Black = 0;
    AAA_Adjust.DP_Status.Cap_Black = 0;
    DP_Cnt_PD = DP_Cnt_CD = 0;
#if 1
    DP_Cnt_PW = AAA_Adjust.DP_Cap_Num[0];
    DP_Cnt_CW = AAA_Adjust.DP_Cap_Num[1];



    if(AAA_Adjust.DP_Status.Cap_White != _Cap_W_Status)
    {
        if(DP_Cap_MAX <= DP_Cnt_CW)
            ErrorCode = ERRORCODE_DPCW_MAX;
        else
            ErrorCode = ERRORCODE_DP_CTNE_CAP;
    }
    if(AAA_Adjust.DP_Status.Prv_White != _Prv_W_Status)
    {
        if(DP_Prv_MAX <= DP_Cnt_PW)
            ErrorCode = ERRORCODE_DPPW_MAX;
        else
            ErrorCode = ERRORCODE_DP_CTNE_CAP;
    }
#endif
    Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_PREVIEW);

    if(ErrorCode==0 || DP_SEARCH_TILL_END)
    {
        //Preview Dark
//////////////////////////////////////////////////////////////////
        Photo_LockAE(LOCK);
        Photo_LockAWB(LOCK);
        wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_2NDIDLE, TWF_ORW);
        AeSetPrvExpoLine(MaxExpLine_2560x960);
        AeSetAGCGainISO(800);
        RawAddr = ippbuf_GetBuf(IPPBUF_PRV_END);

        CalPreviewOne(  RawAddr,
                        _SieBitDepth_8,
                        &PrvRawHSize,
                        &PrvRawVSize,
                        0x0);
#if 0
        debug_msg("Prv %dx%d\r\n", PrvRawHSize, PrvRawVSize);
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
        (UB*)RawAddr,//   UB *pBuf,
        PrvRawHSize*PrvRawVSize,//JJ_UI_FileFmt
        0,//UW Pos,
        1,//BOOL CloseFile,
        FST_FMT_RAW,//UW FileFormat,
        FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif
        debug_msg("PRV DARK...\r\n");

        //Cal_AlgShowString("PRV DARK...1sec", 20, 100);
        DP_Cnt_PD = DFP_Bayer_Search(
                    RawAddr+(CalInfor.SkipPrv.SkipYU*PrvRawHSize+CalInfor.SkipPrv.SkipXL),
                    PrvRawHSize-CalInfor.SkipPrv.SkipXL-CalInfor.SkipPrv.SkipXR,
                    PrvRawVSize-CalInfor.SkipPrv.SkipYU-CalInfor.SkipPrv.SkipYD,
                    PrvRawHSize, //HSize = Line-offset
                    (UW)&(AAA_Adjust.Defect_Pixel[0]),
                    0/*no-crop-offset*/+CalInfor.SkipPrv.SkipXL,
                    0/*no-crop-offset*/+CalInfor.SkipPrv.SkipYU,
                    DP_Cnt_PW,
                    DP_Prv_MAX,
                    CalInfor.DarkLimitPrv,
                    CalInfor.BlockPrv.Block_X,
                    CalInfor.BlockPrv.Block_Y);

        sprintf(message, "%d", DP_Cnt_PW+DP_Cnt_PD);
        Cal_AlgShowString(message, Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(2));
        if((DP_Cnt_PW+DP_Cnt_PD) >= (DP_Prv_MAX))//+DP_Prv_MAX_2+DP_Prv_MAX_3))
        {
            ErrorCode = ERRORCODE_DPPB_MAX;
            AAA_Adjust.DP_Status.Prv_Black=0;
        }
        else
        {
            AAA_Adjust.DP_Status.Prv_Black=_Prv_B_Status;
            AAA_Adjust.DP_Status.Prv_White=_Prv_W_Status;
        }
    }
    Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_PREVIEW);
    if(ErrorCode==0 || DP_SEARCH_TILL_END)
    {
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);

        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_PREVIEW;
        IPL_SetCmd(&IplObj);

        Photo_LockAE(LOCK);
        Photo_LockAWB(LOCK);
        wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_2NDIDLE, TWF_ORW);
//        AeSetPrvExpoLine(MaxCapExpLine);
//        AeSetAGCGainISO(300);

        //Capture Dark
        debug_err(("CAP DARK...\r\n"));
        //Cal_AlgShowString("CAP DARK...10sec", 20, 130);

        RawAddr = ippbuf_GetBuf(IPPBUF_PRV_END);

        CalCaptureOne(  RawAddr,
                        GetIPPSizeInfor(_PreCropCapHsize),
                        GetIPPSizeInfor(_PreCropCapVsize),
                        0,
                        0x0,
                        0);
    CapRawHSize = GetIPPSizeInfor(_PreCropCapHsize);
    CapRawVSize = GetIPPSizeInfor(_PreCropCapVsize);


#if 0
    debug_err(("Cap %dx%d\r\n", CapRawHSize, CapRawVSize));
    debug_err(("Crop ini %dx%d\r\n", pre_GetCropHIni(), pre_GetCropVIni()));
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
    (UB*)RawAddr,//   UB *pBuf,
    CapRawHSize*CapRawVSize,//JJ_UI_FileFmt
    0,//UW Pos,
    1,//BOOL CloseFile,
    FST_FMT_RAW,//UW FileFormat,
    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif

        DP_Cnt_CD = DFP_Bayer_Search(
                    RawAddr+(CalInfor.SkipCap.SkipYU*CapRawHSize+CalInfor.SkipCap.SkipXL),
                    CapRawHSize-CalInfor.SkipCap.SkipXL-CalInfor.SkipCap.SkipXR,
                    CapRawVSize-CalInfor.SkipCap.SkipYU-CalInfor.SkipCap.SkipYD,
                    CapRawHSize,
                    (UW)(AAA_Adjust.Defect_Pixel)+(DP_Prv_MAX),
                    pre_GetCropHIni()+CalInfor.SkipCap.SkipXL,
                    pre_GetCropVIni()*SensorCapTotalFld+CalInfor.SkipCap.SkipYU,
                    DP_Cnt_CW,
                    DP_Cap_MAX,//+DP_Prv_MAX_2+DP_Prv_MAX_3,
                    CalInfor.DarkLimitCap,
                    CalInfor.BlockCap.Block_X,
                    CalInfor.BlockCap.Block_Y);

        sprintf(message, "%d", DP_Cnt_CD);
        Cal_AlgShowString(message, Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(2));
        sprintf(message, "%d", DP_Cnt_CW);
        Cal_AlgShowString(message, Cal_PanelMsgXPos(6),Cal_PanelMsgYPos(2));
        if(DP_Cnt_CW+DP_Cnt_CD >= DP_Cap_MAX)
        {
            ErrorCode = ERRORCODE_DPCB_MAX;
            AAA_Adjust.DP_Status.Cap_Black=0;
        }
        else
        {
            AAA_Adjust.DP_Status.Cap_Black=_Cap_B_Status;
            AAA_Adjust.DP_Status.Cap_White = _Cap_W_Status;
        }
    }
    //summary
    DP_Cnt_Prv = DP_Cnt_PW+DP_Cnt_PD;
    DP_Cnt_Cap = DP_Cnt_CW+DP_Cnt_CD;
    AAA_Adjust.DP_Cap_Num[0] = DP_Cnt_PW;
    AAA_Adjust.DP_Cap_Num[1] = DP_Cnt_CW;
    AAA_Adjust.DP_Cap_Num[2] = DP_Cnt_PD;
    AAA_Adjust.DP_Cap_Num[3] = 0;
    AAA_Adjust.DP_Cap_Num[4] = 0;
    AAA_Adjust.DP_Cap_Num[5] = DP_Cnt_CD;
    debug_err(("Cal_DP_DARK: DFP White = %d, %d\r\n\r\n", DP_Cnt_PW, DP_Cnt_CW));
    debug_err(("Cal_DP_DARK: DFP Dark = %d %d %d, %d\r\n\r\n", DP_Cnt_PD,0,0, DP_Cnt_CD));
    debug_err(("ErrorCode = %d, DP Num Prv = %d, Cap = %d\r\n", ErrorCode, DP_Cnt_Prv, DP_Cnt_Cap));

    if(ErrorCode==0)
    {
        //sorting and recording
        SortDPdata((UW*)&AAA_Adjust.Defect_Pixel[0],            DP_Cnt_Prv);
        SortDPdata((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],   DP_Cnt_Cap);

        //SortDPdata2Fld((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],    DP_Cnt_Cap);

        AAA_Adjust.Defect_Pixel[DP_Cnt_Prv] = 0xFFFFFFFF;
        AAA_Adjust.Defect_Pixel[DP_Prv_MAX+DP_Cnt_Cap] = 0xFFFFFFFF;

        /*
        if(CheckContinueDP((UW*)&AAA_Adjust.Defect_Pixel[0],DP_Cnt_Prv,Prv_Mode))
        {
            ErrorCode=ERRORCODE_DP_CTNE_PRV;
            AAA_Adjust.DP_Status.Prv_White=((DP_Cnt_Prv>>8)&0xff);
            AAA_Adjust.DP_Status.Prv_Black=(DP_Cnt_Prv&0xff);
        }
        if(CheckContinueDP((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],DP_Cnt_Cap,Cap_Mode))
        {
            ErrorCode=ERRORCODE_DP_CTNE_CAP;
            AAA_Adjust.DP_Status.Cap_White=((DP_Cnt_Cap>>8)&0xff);
            AAA_Adjust.DP_Status.Cap_Black=(DP_Cnt_Cap&0xff);
        }
        if(HwDpCapFlag)
        {
            SortDPdata2Fld((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],DP_Cnt_Cap);
        }
        debug_ind(("DP Num: %d %d %d %d\r\n",
        AAA_Adjust.DP_Cap_Num[0],
        AAA_Adjust.DP_Cap_Num[1],
        AAA_Adjust.DP_Cap_Num[2],
        AAA_Adjust.DP_Cap_Num[3]));
        */
    }
    debug_msg("DPSTATUS: %d %d %d %d\r\n",AAA_Adjust.DP_Status.Prv_Black
        ,AAA_Adjust.DP_Status.Prv_White
        ,AAA_Adjust.DP_Status.Cap_Black
        ,AAA_Adjust.DP_Status.Cap_White);
    WriteCalData();

    debug_msg("DP Num read from nand: %d %d %d %d %d %d\r\n",
        AAA_Adjust.DP_Cap_Num[0],
        AAA_Adjust.DP_Cap_Num[1],
        AAA_Adjust.DP_Cap_Num[2],
        AAA_Adjust.DP_Cap_Num[3],
        AAA_Adjust.DP_Cap_Num[4],
        AAA_Adjust.DP_Cap_Num[5]);
    #if CAL_SAVE_ADJUST
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)AAA_Adjust.Defect_Pixel,//   UB *pBuf,
                    (DP_Prv_MAX/*+DP_Prv_MAX_2+DP_Prv_MAX_3*/+DP_Cap_MAX)*4,//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
    #endif

    //Cal_AlgClearScreen();
    //Cal_AlgShowString("DP DARK NUM:", 20,40);
    //sprintf(message, "PRV %d CAP %d", DP_Cnt_PD, DP_Cnt_CD);
    //Cal_AlgShowString(message, 20, 70);
    //Cal_AlgShowString("DP TOTAL NUM:", 20,100);
    //sprintf(message, "PRV %d CAP %d", DP_Cnt_Prv, DP_Cnt_Cap);
    //Cal_AlgShowString(message, 20, 130);

    Cal_AlgShowString("DP Dark",Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(6));
    if((ErrorCode == ERROR_OK))
    {
        Cal_AlgShowString("PASS",Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(6));
    }
    else
    {
        Cal_AlgShowString("NG",Cal_PanelMsgXPos(4),Cal_PanelMsgYPos(6));
    }

    //3-3 set down
    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_PREVIEW;
    IPL_SetCmd(&IplObj);
    Cal_AlgClearScreen();
    Cal_AlgShowString("CLOSE THE LIGHT",20,80);
    Cal_AlgShowString("THEN PRESS LEFT",20,105);
    clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);

    return ErrorCode;
}

UB Lens_Shading_Table_adjust(UINT Addr,UINT VIG_X, UINT VIG_Y)
{
}

UB LensShading_Adjust(void)
{
}


UB OBAdjust(void)
{
}


UINT8 MshutterLasting(void)
{
}


UINT8 NearFocusAdjust(void)
{
}

UINT8 WriteAdjustData2File(INT8 state)
{
}

UINT8 ReadCalStatus(void)
{
    //##if one item fail,return FALSE 2007/07/27@Janice Hunag
    UINT8 result = TRUE;
    FLGPTN uiFlag;
    Cal_AlgClearScreen();

    if( (AAA_Adjust.DP_Status.Prv_White==_Prv_W_Status)&&(AAA_Adjust.DP_Status.Cap_White==_Cap_W_Status) )
    {
        Cal_AlgShowString("0 DP White PASS", 10, 60);
    }
    else
    {
        Cal_AlgShowString("0 DP White NG", 10, 60);
        result = FALSE;
    }

    if(AAA_Adjust.AWB_Status==_AWB_Status)
    {
        Cal_AlgShowString("1 AWB PASS", 10, 80);
    }
    else
    {
        Cal_AlgShowString("1 AWB NG", 10, 80);
        result = FALSE;
    }
#if 0
    if(AAA_Adjust.Mshutter_Status[0]==_Mshutter_Status&& AAA_Adjust.Mshutter_Status[1]==_Mshutter_Status)
    {
        Cal_AlgShowString("2 Mshutter PASS", 10, 100);
    }
    else
    {
        Cal_AlgShowString("2 Mshutter NG", 10, 100);
        result = FALSE;
    }
#endif
    //temporary using ISO_status for BurnIn
    if(AAA_Adjust.ISO_Status==_ISO_Status)
    {
        Cal_AlgShowString("3 ISO PASS", 10, 120);
    }
    else
    {
        Cal_AlgShowString("3 ISO NG", 10, 120);
        result = FALSE;
    }

    if( (AAA_Adjust.DP_Status.Prv_Black==_Prv_B_Status)&&(AAA_Adjust.DP_Status.Cap_Black==_Cap_B_Status) )
    {
        Cal_AlgShowString("4 DP Black PASS", 10, 140);
    }
    else
    {
        Cal_AlgShowString("4 DP Black NG", 10, 140);
        result = FALSE;
    }

    /*
    if(AAA_Adjust.FlashAE_Status==_Flash_Status)
    {
        Cal_AlgShowString("4 FLASH PASS", 10, 140);
    }
    else
    {
        Cal_AlgShowString("4 FLASH NG", 10, 140);
    }

    if(AAA_Adjust.FFocus_Status==_FF_Status)
    {
        Cal_AlgShowString("5 FFOCUS PASS", 10, 160);
    }
    else
    {
        Cal_AlgShowString("5 FFOCUS NG", 10, 160);
    }

    if(AAA_Adjust.NFocus_Status==_NF_Status)
    {
        Cal_AlgShowString("6 NFOCUS PASS", 10, 180);
    }
    else
    {
        Cal_AlgShowString("6 NFOCUS NG", 10, 180);
    }
    */
    Cal_AlgShowString("PRESS left key to continue",Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(8));

    clr_flg(FLG_ID_KEY,FLGKEY_LEFT);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    clr_flg(FLG_ID_KEY,FLGKEY_LEFT);
    result = ERROR_OK;
    return result;
}

#if 0
UINT8 DP_Adjust(void)
{
    //UINT uiFlag=0;
    UINT16 DP_Cnt_PW, DP_Cnt_PD, DP_Cnt_CW, DP_Cnt_CD, DP_Cnt_Prv, DP_Cnt_Cap;
    UINT32 RawAddr;
    UINT8 ErrorCode;
    UINT32 i;

    for(i=0; i<(DP_Prv_MAX+DP_Cap_MAX); i++)
        AAA_Adjust.Defect_Pixel[i] = 0x0;

    //3-1

    //Cal_AlgShowString("FACE CAM. TO LIGHT BOX",20,80);
    //Cal_AlgShowString("THEN PRESS LEFT",20,110);
    //clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    //wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);

    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
    ImgCapture_SetParameter(_SiePath,_PostGamma);
    Set2Preview();
    Photo_LockAE(1);
    Photo_LockAWB(1);

    //setting Bright AE
    //Fixed AE setting
    WaitAeConverge();
    SetRBgain_White();
    //CalSetExpTime(33);
    //CalSetAgc(9);
    //CalAeSet();
    //aaa_SetColorgain(GetCFAPattern(),380, 256, 256, 269);


    Cal_AlgClearScreen();
    Get_Sensor_CalInfor();
    HwDpCapFlag = 1;

    SetDPAdd(0,0);//not doing Blm. while calibration
    ErrorCode = ERROR_OK;
    Get_Sensor_CalInfor();
    AAA_Adjust.DP_Status.Prv_White = AAA_Adjust.DP_Status.Prv_Black = 0;
    AAA_Adjust.DP_Status.Cap_White = AAA_Adjust.DP_Status.Cap_Black = 0;
    DP_Cnt_PW = DP_Cnt_PD = DP_Cnt_CW = DP_Cnt_CD = 0;
    RawAddr=gCapRawAddr;

    // 3-2 DP calibration main process
    //Preview White
    debug_ind(("PRV WHITE...\r\n"));
    Cal_AlgShowString("PRV WHITE...1sec", 20, 40);
    DFP_GetOnePicPrv(RawAddr, GetSieInHsize(), GetSieInVsize());

    DP_Cnt_PW = DFP_Bayer_Search(
                RawAddr+(CalInfor.SkipPrv.SkipYU*GetSieInHsize()+CalInfor.SkipPrv.SkipXL),
                GetSieInHsize()-CalInfor.SkipPrv.SkipXL-CalInfor.SkipPrv.SkipXR,
                GetSieInVsize()-CalInfor.SkipPrv.SkipYU-CalInfor.SkipPrv.SkipYD,
                GetSieInHsize(),
                (UW)&(AAA_Adjust.Defect_Pixel[0]),
                sie_GetCropH_Offset()+CalInfor.SkipPrv.SkipXL,
                sie_GetCropV_Offset()*SensorCapTotalFld+CalInfor.SkipPrv.SkipYU,
                0,
                DP_Prv_MAX,
                CalInfor.BrightLimitPrv,
                CalInfor.BlockPrv.Block_X,
                CalInfor.BlockPrv.Block_Y);

    if(DP_Cnt_PW >= DP_Prv_MAX)
    {
        ErrorCode = ERRORCODE_DPPW_MAX;
    }
    else
    {
        AAA_Adjust.DP_Status.Prv_White=_Prv_W_Status;
    }

    if(ErrorCode==0 || DP_SEARCH_TILL_END)
    {
        //Capture White
        debug_ind(("CAP WHITE...\r\n"));
        Cal_AlgShowString("CAP WHITE...10sec", 20, 70);
        Cal_Mshutter=Default_Mshutter;
        DFP_GetOnePicCap(RawAddr, IMAGE_SIZE_H, IMAGE_SIZE_V, Status_White);

        DP_Cnt_CW = DFP_Bayer_Search(
                    RawAddr+(CalInfor.SkipCap.SkipYU*IMAGE_SIZE_H+CalInfor.SkipCap.SkipXL),
                    IMAGE_SIZE_H-CalInfor.SkipCap.SkipXL-CalInfor.SkipCap.SkipXR,
                    IMAGE_SIZE_V-CalInfor.SkipCap.SkipYU-CalInfor.SkipCap.SkipYD,
                    IMAGE_SIZE_H,
                    (UW)(AAA_Adjust.Defect_Pixel)+(DP_Prv_MAX<<2),
                    sie_GetCropH_Offset()+CalInfor.SkipCap.SkipXL,
                    sie_GetCropV_Offset()*SensorCapTotalFld+CalInfor.SkipCap.SkipYU,
                    0,
                    DP_Cap_MAX,
                    CalInfor.BrightLimitCap,
                    CalInfor.BlockCap.Block_X,
                    CalInfor.BlockCap.Block_Y);

        if(DP_Cnt_CW >= DP_Cap_MAX)
        {
            ErrorCode = ERRORCODE_DPCW_MAX;
        }
        else
        {
            AAA_Adjust.DP_Status.Cap_White=_Cap_W_Status;
        }
    }

    //Cal_AlgShowString("CLOSE SHUTTER",20,100);
    //Cal_AlgShowString("THEN PRESS LEFT",20,130);
    //clr_flg(FLG_ID_KEY, FLGKEY_LEFT);
    //wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    Photo_Shutter_Move(CLOSE);

    //setting Dark AE/AWB
    //Fixed AE/AWB setting
    Photo_LockAE(1);
    Photo_LockAWB(1);
    SetRBgain_Dark();
    //aaa_SetColorgain(GetCFAPattern(),512, 256, 256, 512);
    //CalSetExpTime(MaxExpLine); // 1/30 s
    CalSetExpTime(727); // 1/20 s
    CalSetAgc(Agc_ISO100); //ISO100
    //CalSetAgc(Agc_ISO100+2*Gain_gap); //ISO400
    CalAeSet();


    //DFP_DarkAEPrv();
    //SensorDelayVD(2);

    if(ErrorCode==0 || DP_SEARCH_TILL_END)
    {
        //Preview Dark
        debug_ind(("PRV DARK...\r\n"));
        Cal_AlgShowString("PRV DARK...1sec", 20, 100);
        DFP_GetOnePicPrv(RawAddr, GetSieInHsize(), GetSieInVsize());
        debug_err(("PreviewCrop:%x %x\r\n", sie_GetCropH_Offset(), sie_GetCropV_Offset()));

        DP_Cnt_PD = DFP_Bayer_Search(
                    RawAddr+(CalInfor.SkipPrv.SkipYU*GetSieInHsize()+CalInfor.SkipPrv.SkipXL),
                    GetSieInHsize()-CalInfor.SkipPrv.SkipXL-CalInfor.SkipPrv.SkipXR,
                    GetSieInVsize()-CalInfor.SkipPrv.SkipYU-CalInfor.SkipPrv.SkipYD,
                    GetSieInHsize(),
                    (UW)&(AAA_Adjust.Defect_Pixel[0]),
                    sie_GetCropH_Offset()+CalInfor.SkipPrv.SkipXL,
                    sie_GetCropV_Offset()*SensorCapTotalFld+CalInfor.SkipPrv.SkipYU,
                    DP_Cnt_PW,
                    DP_Prv_MAX,
                    0,//CalInfor.DarkLimitPrv,
                    CalInfor.BlockPrv.Block_X,
                    CalInfor.BlockPrv.Block_Y);

        if(DP_Cnt_PW+DP_Cnt_PD >= DP_Prv_MAX)
        {
            ErrorCode = ERRORCODE_DPPB_MAX;
        }
        else
        {
            AAA_Adjust.DP_Status.Prv_Black=_Prv_B_Status;
        }
    }

    if(ErrorCode==0 || DP_SEARCH_TILL_END)
    {
        //Capture Dark
        debug_ind(("CAP DARK...\r\n"));
        Cal_AlgShowString("CAP DARK...10sec", 20, 130);
        Cal_Mshutter=Default_Mshutter;
        DFP_GetOnePicCap(RawAddr, IMAGE_SIZE_H, IMAGE_SIZE_V, Status_Black);
        debug_err(("CaptureCrop:%x %x\r\n", sie_GetCropH_Offset(), sie_GetCropV_Offset()));

        DP_Cnt_CD = DFP_Bayer_Search(
                    RawAddr+(CalInfor.SkipCap.SkipYU*IMAGE_SIZE_H+CalInfor.SkipCap.SkipXL),
                    IMAGE_SIZE_H-CalInfor.SkipCap.SkipXL-CalInfor.SkipCap.SkipXR,
                    IMAGE_SIZE_V-CalInfor.SkipCap.SkipYU-CalInfor.SkipCap.SkipYD,
                    IMAGE_SIZE_H,
                    (UW)(AAA_Adjust.Defect_Pixel)+(DP_Prv_MAX<<2),
                    sie_GetCropH_Offset()+CalInfor.SkipCap.SkipXL,
                    sie_GetCropV_Offset()*SensorCapTotalFld+CalInfor.SkipCap.SkipYU,
                    DP_Cnt_CW,
                    DP_Cap_MAX,
                    0,//CalInfor.DarkLimitCap,
                    CalInfor.BlockCap.Block_X,
                    CalInfor.BlockCap.Block_Y);

        if(DP_Cnt_CW+DP_Cnt_CD >= DP_Cap_MAX)
        {
            ErrorCode = ERRORCODE_DPCB_MAX;
        }
        else
        {
            AAA_Adjust.DP_Status.Cap_Black=_Cap_B_Status;
        }
    }
    Photo_Shutter_Move(OPEN);

    //summary
    DP_Cnt_Prv = DP_Cnt_PW+DP_Cnt_PD;
    DP_Cnt_Cap = DP_Cnt_CW+DP_Cnt_CD;
    debug_ind(("Cal_DP: DFP White = %d, %d\r\n\r\n", DP_Cnt_PW, DP_Cnt_CW));
    debug_ind(("Cal_DP: DFP Dark = %d, %d\r\n\r\n", DP_Cnt_PD, DP_Cnt_CD));
    debug_ind(("ErrorCode = %d, DP Num Prv = %d, Cap = %d\r\n", ErrorCode, DP_Cnt_Prv, DP_Cnt_Cap));

    if(ErrorCode==0)
    {
        //sorting and recording
        SortDPdata((UW*)&AAA_Adjust.Defect_Pixel[0],            DP_Cnt_Prv);
        SortDPdata((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],    DP_Cnt_Cap);

        AAA_Adjust.Defect_Pixel[DP_Cnt_Prv] = 0xFFFFFFFF;
        AAA_Adjust.Defect_Pixel[DP_Prv_MAX+DP_Cnt_Cap] = 0xFFFFFFFF;

        /*
        if(CheckContinueDP((UW*)&AAA_Adjust.Defect_Pixel[0],DP_Cnt_Prv,Prv_Mode))
        {
            ErrorCode=ERRORCODE_DP_CTNE_PRV;
            AAA_Adjust.DP_Status.Prv_White=((DP_Cnt_Prv>>8)&0xff);
            AAA_Adjust.DP_Status.Prv_Black=(DP_Cnt_Prv&0xff);
        }
        if(CheckContinueDP((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],DP_Cnt_Cap,Cap_Mode))
        {
            ErrorCode=ERRORCODE_DP_CTNE_CAP;
            AAA_Adjust.DP_Status.Cap_White=((DP_Cnt_Cap>>8)&0xff);
            AAA_Adjust.DP_Status.Cap_Black=(DP_Cnt_Cap&0xff);
        }
        if(HwDpCapFlag)
        {
            SortDPdata2Fld((UW*)&AAA_Adjust.Defect_Pixel[DP_Prv_MAX],DP_Cnt_Cap);
        }
        debug_ind(("DP Num: %d %d %d %d\r\n",
        AAA_Adjust.DP_Cap_Num[0],
        AAA_Adjust.DP_Cap_Num[1],
        AAA_Adjust.DP_Cap_Num[2],
        AAA_Adjust.DP_Cap_Num[3]));
        */
#if 1
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                        (UB*)AAA_Adjust.Defect_Pixel,//   UB *pBuf,
                        (DP_Prv_MAX+DP_Cap_MAX)*4,//JJ_UI_FileFmt
                        0,//UW Pos,
                        1,//BOOL CloseFile,
                        FST_FMT_RAW,//UW FileFormat,
                        FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif

        WriteCalData();
#if 1
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                        (UB*)AAA_Adjust.Defect_Pixel,//   UB *pBuf,
                        (DP_Prv_MAX+DP_Cap_MAX)*4,//JJ_UI_FileFmt
                        0,//UW Pos,
                        1,//BOOL CloseFile,
                        FST_FMT_RAW,//UW FileFormat,
                        FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif
    }

    //3-3 set down
    //Photo_LockAE(0);
    Set2Preview();

    Cal_AlgClearScreen();
    Cal_AlgShowString("DP NUM:", 20,70);
    sprintf(message, "PRV %d CAP %d", DP_Cnt_Prv, DP_Cnt_Cap);
    Cal_AlgShowString(message, 20, 100);

    if(ErrorCode==0)
    {
        Cal_AlgShowString("OK", 20, 130);
    }
    else
    {
        sprintf(message, "ERRCODE: %d", ErrorCode);
        Cal_AlgShowString(message, 20, 130);
    }
    return ErrorCode;
}
#else
UINT8 DP_Adjust(void)
{
    UINT8 ErrorCode;

    ErrorCode=DP_Adjust_White();
    ErrorCode=DP_Adjust_Dark();

    return ErrorCode;
}
#endif


void DFP_DarkAEPrv(void)
{
#if 0//Mark Useless Code
    //Fixed AE setting for preview dark environment
    SetPreExpLine(MaxExpLine);
    SetPara_PgaGain(AgcGain_max);//#NT#2007/10/23#JJ Huang  //SetPreAGC(AgcGain_max);
    SensorDelayVD(2);
#endif//Mark Useless Code
}

void DFP_DarkAECap(void)
{
    //Fixed AE setting for capture dark environment
}

//modified to be general for different start pixels in bayer pattern
UINT CalculateRawY(UINT Add, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT sample,unsigned char StartPixel,unsigned short *R,unsigned short *G,unsigned short *B)
{
    UINT SumR,SumGr,SumGb,SumB,AvgR,AvgG,AvgB,Data[4];
    UINT i,j,Y,count=0;
    unsigned char Pixel_R,Pixel_Gr,Pixel_Gb,Pixel_B;

    Data[0]=0;
    Data[1]=0;
    Data[2]=0;
    Data[3]=0;
    if(StartPixel == 0)// R
    {
        Pixel_R = 0;
        Pixel_Gr = 1;
        Pixel_Gb = 2;
        Pixel_B = 3;
    }
    else if (StartPixel == 1) // Gr
    {
        Pixel_Gr = 0;
        Pixel_R = 1;
        Pixel_B = 2;
        Pixel_Gb = 3;
    }
    else if (StartPixel == 2) // Gb
    {
        Pixel_Gb = 0;
        Pixel_B = 1;
        Pixel_R = 2;
        Pixel_Gr = 3;
    }
    else if (StartPixel == 3) // B
    {
        Pixel_B = 0;
        Pixel_Gb = 1;
        Pixel_Gr = 2;
        Pixel_R = 3;
    }

    for(j=StartY;j<(StartY+SizeY);j+=(sample*2))
    {
        for(i=StartX;i<(StartX+SizeX);i+=(sample*2))
        {
            Data[Pixel_Gb]+=*((UB*)(Add+i+j*GetIPPSizeInfor(_PreOutLineOffset)));
            Data[Pixel_B]+=*((UB*)(Add+i+1+j*GetIPPSizeInfor(_PreOutLineOffset)));
            Data[Pixel_R]+=*((UB*)(Add+i+(j+1)*GetIPPSizeInfor(_PreOutLineOffset)));
            Data[Pixel_Gr]+=*((UB*)(Add+i+1+(j+1)*GetIPPSizeInfor(_PreOutLineOffset)));
            count++;
        }
    }
    if((StartX+StartY)%2==0)
    {
        if(StartX%2==0)
        {
            SumGb=Data[0];
            SumB=Data[1];
            SumR=Data[2];
            SumGr=Data[3];
        }
        else
        {
            SumGr=Data[0];
            SumR=Data[1];
            SumB=Data[2];
            SumGb=Data[3];
        }
    }
    else
    {
        if(StartX%2==0)
        {
            SumR=Data[0];
            SumGr=Data[1];
            SumGb=Data[2];
            SumB=Data[3];
        }
        else
        {
            SumB=Data[0];
            SumGb=Data[1];
            SumGr=Data[2];
            SumR=Data[3];
        }
    }
    AvgR=SumR/count;
    AvgB=SumB/count;
    AvgG=(SumGr+SumGb)/(count*2);
    *R=AvgR;
    *G=AvgG;
    *B=AvgB;

    Y=(77*AvgR+150*AvgG+29*AvgB)>>8;

    return Y;
}

//modified to be general for different start pixels in bayer pattern
UINT CalculateRawY2(UINT Add, UINT lineOffset, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT sample,unsigned char StartPixel,unsigned short *R,unsigned short *G,unsigned short *B)
{
    UINT SumR,SumGr,SumGb,SumB,AvgR,AvgG,AvgB,Data[4];
    UINT i,j,Y,count=0;
    unsigned char Pixel_R,Pixel_Gr,Pixel_Gb,Pixel_B;

    Data[0]=0;
    Data[1]=0;
    Data[2]=0;
    Data[3]=0;
    if(StartPixel == 0)// R
    {
        Pixel_R = 0;
        Pixel_Gr = 1;
        Pixel_Gb = 2;
        Pixel_B = 3;
    }
    else if (StartPixel == 1) // Gr
    {
        Pixel_Gr = 0;
        Pixel_R = 1;
        Pixel_B = 2;
        Pixel_Gb = 3;
    }
    else if (StartPixel == 2) // Gb
    {
        Pixel_Gb = 0;
        Pixel_B = 1;
        Pixel_R = 2;
        Pixel_Gr = 3;
    }
    else if (StartPixel == 3) // B
    {
        Pixel_B = 0;
        Pixel_Gb = 1;
        Pixel_Gr = 2;
        Pixel_R = 3;
    }
    for(j=StartY;j<(StartY+SizeY);j+=(sample*2))
    {
        for(i=StartX;i<(StartX+SizeX);i+=(sample*2))
        {
            Data[Pixel_Gb]+=*((UB*)(Add+i+j*lineOffset));
            Data[Pixel_B]+=*((UB*)(Add+i+1+j*lineOffset));
            Data[Pixel_R]+=*((UB*)(Add+i+(j+1)*lineOffset));
            Data[Pixel_Gr]+=*((UB*)(Add+i+1+(j+1)*lineOffset));
            count++;
        }
    }
    if((StartX+StartY)%2==0)
    {
        if(StartX%2==0)
        {
            SumGb=Data[0];
            SumB=Data[1];
            SumR=Data[2];
            SumGr=Data[3];
        }
        else
        {
            SumGr=Data[0];
            SumR=Data[1];
            SumB=Data[2];
            SumGb=Data[3];
        }
    }
    else
    {
        if(StartX%2==0)
        {
            SumR=Data[0];
            SumGr=Data[1];
            SumGb=Data[2];
            SumB=Data[3];
        }
        else
        {
            SumB=Data[0];
            SumGb=Data[1];
            SumGr=Data[2];
            SumR=Data[3];
        }
    }
    AvgR=SumR/count;
    AvgB=SumB/count;
    AvgG=(SumGr+SumGb)/(count*2);
    *R=AvgR;
    *G=AvgG;
    *B=AvgB;

    Y=(77*AvgR+150*AvgG+29*AvgB)>>8;

    return Y;
}
//modified to be general for different start pixels in bayer pattern
//Further modified for fixed-point return value
UINT CalculateRawY3(UINT Add, UINT lineOffset, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT sample,unsigned char StartPixel,unsigned short *R,unsigned short *G,unsigned short *B, UINT8 uiFractionalBit)
{
    UINT SumR,SumGr,SumGb,SumB,AvgR,AvgG,AvgB,Data[4];
    UINT i,j,Y,count=0;
    unsigned char Pixel_R,Pixel_Gr,Pixel_Gb,Pixel_B;

    Data[0]=0;
    Data[1]=0;
    Data[2]=0;
    Data[3]=0;
    if(StartPixel == 0)// R
    {
        Pixel_R = 0;
        Pixel_Gr = 1;
        Pixel_Gb = 2;
        Pixel_B = 3;
    }
    else if (StartPixel == 1) // Gr
    {
        Pixel_Gr = 0;
        Pixel_R = 1;
        Pixel_B = 2;
        Pixel_Gb = 3;
    }
    else if (StartPixel == 2) // Gb
    {
        Pixel_Gb = 0;
        Pixel_B = 1;
        Pixel_R = 2;
        Pixel_Gr = 3;
    }
    else if (StartPixel == 3) // B
    {
        Pixel_B = 0;
        Pixel_Gb = 1;
        Pixel_Gr = 2;
        Pixel_R = 3;
    }
    for(j=StartY;j<(StartY+SizeY);j+=(sample*2))
    {
        for(i=StartX;i<(StartX+SizeX);i+=(sample*2))
        {
            Data[Pixel_Gb]+=*((UB*)(Add+i+j*lineOffset));
            Data[Pixel_B]+=*((UB*)(Add+i+1+j*lineOffset));
            Data[Pixel_R]+=*((UB*)(Add+i+(j+1)*lineOffset));
            Data[Pixel_Gr]+=*((UB*)(Add+i+1+(j+1)*lineOffset));
            count++;
        }
    }
    if((StartX+StartY)%2==0)
    {
        if(StartX%2==0)
        {
            SumGb=Data[0];
            SumB=Data[1];
            SumR=Data[2];
            SumGr=Data[3];
        }
        else
        {
            SumGr=Data[0];
            SumR=Data[1];
            SumB=Data[2];
            SumGb=Data[3];
        }
    }
    else
    {
        if(StartX%2==0)
        {
            SumR=Data[0];
            SumGr=Data[1];
            SumGb=Data[2];
            SumB=Data[3];
        }
        else
        {
            SumB=Data[0];
            SumGb=Data[1];
            SumGr=Data[2];
            SumR=Data[3];
        }
    }
    AvgR=(SumR<<uiFractionalBit)/count;
    AvgB=(SumB<<uiFractionalBit)/count;
    AvgG=((SumGr+SumGb)<<uiFractionalBit)/(count*2);
    *R=AvgR;
    *G=AvgG;
    *B=AvgB;

    Y=(77*AvgR+150*AvgG+29*AvgB)>>8;

    return Y;
}

void WaitAeConverge(void)
{
    UINT32 NormalY = 200;
    UINT32 Yavg;
    INT32 EL, oldEL;
    UINT32 stableCnt, Count;

    EL=36;
    stableCnt = Count = 0;
    while(stableCnt<3)
    {
        oldEL=EL;
        GetActImgFBAddr_Path1(&FB_para1);
        aaa_SetAeWindow((UB*)AE_Window_EqualWeighted);
        Yavg=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));//JJ_433CAL: need review
        debug_err(("Yavg=%d, EL=%d\r\n",Yavg,EL));

        if (Yavg == 0)
        {
            Yavg = 1;
        }
        //1/4EV convergence
        if((Yavg>=NormalY*119/100)||(Yavg<=NormalY*100/119))
        {
            EL = (oldEL*NormalY)/Yavg;
            if (EL > MaxExpLine_1280x480)
            {
                EL = MaxExpLine_1280x480;
            }
            else if (EL < MinExpLine)
            {
                EL = MinExpLine;
            }
        }
        else if((Yavg>=NormalY*109/100)&&(EL>MinExpLine))
        {
            EL--;
        }
        else if((Yavg<=NormalY*100/109)&&(EL<MaxExpLine_1280x480))
        {
            EL++;
        }
        AeSetAGCGainISO(50);   //ISO50
        AeSetPrvExpoLine(EL);

        if ((Yavg<NormalY*109/100) && (Yavg>NormalY*100/109))
        {
            stableCnt++;
        }
        else
        {
            if (oldEL == EL)
            {
                Count ++;
            }
            else
            {
                Count = 0;
            }
            if (Count > 40)
            {
                debug_err(("Ae Converge fail(WaitAeConverge)\r\n"));
                break;
            }
        }
        sie_waitVD(5);
    }
}

UINT32 WaitAeStable_WithTimeOut(UINT32 ExpectY, UINT32 ISOValue, UINT8 MaxCnt)
{
    UINT32 Yavg;
    INT32 EL, oldEL;
    UINT8 stableCnt, Count, i;
    UINT32 ExpMaxCnt=0, MinCnt=0;

    EL=36;
    stableCnt = Count = 0, i=0;
    while(stableCnt<3)
    {
        oldEL=EL;
        GetActImgFBAddr_Path1(&FB_para1);
        aaa_SetAeWindow((UB*)AE_Window_EqualWeighted);
        Yavg=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));//JJ_433CAL: need review

sprintf(message, "%2d Y%3d L%3d G%3d", i++, Yavg, EL, ISOValue);
Cal_AlgShowString(message,Cal_PanelMsgXPos(1),Cal_PanelMsgYPos(7));

        debug_err(("Yavg=%3d, EL=%3d, ISOValue=%3d\r\n", Yavg, EL, ISOValue));
        if (Yavg == 0)
        {
            Yavg = 1;
        }
        //1/4EV convergence
        if((Yavg>=ExpectY*119/100)||(Yavg<=ExpectY*100/119))
        {
            EL = (oldEL*ExpectY)/Yavg;
            if (EL >= MaxExpLine_640x480)
            {
                EL = MaxExpLine_640x480;
                ExpMaxCnt++;
                if(ExpMaxCnt++>2)
                {
                    debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
                    break;
                }
            }
            else if (EL <= MinExpLine)
            {
                EL = MinExpLine;
                MinCnt++;
                if(MinCnt++>2)
                {
                    debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
                    break;
                }
            }
        }
        else if((Yavg>=ExpectY*109/100)&&(EL>MinExpLine))
        {
            EL--;
        }
        else if((Yavg<=ExpectY*100/109)&&(EL<MaxExpLine_640x480))
        {
            EL++;
        }


        CalSetExpTime(EL);
        CalSetAgc(ISOValue);
        CalAeSet();
        if ((Yavg<ExpectY*109/100) && (Yavg>ExpectY*100/109))
        {
            stableCnt++;
        }
        else
        {
            if (oldEL == EL)
            {
                Count ++;
            }
            else
            {
                Count = 0;
            }
            if (Count > MaxCnt)
            {
                debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
                break;
            }
        }

        //#NT#2009/09/01#KS Hung -begin
        //#NT#No this function
        //SensorDelayVD(5);
        //#NT#2009/09/01#KS Hung -end
        debug_ind(("Yavg=%d, EL=%d\r\n",Yavg,EL));
    }
    return Yavg;
}
#if 0
UINT32 WaitAeStable(UINT32 ExpectY, UINT32 ISOValue)
{
    UINT32 Yavg;
    INT32 EL, oldEL;
    UINT8 stableCnt, Count, i;
    UINT32 MaxCnt=0, MinCnt=0;

    EL=36;
    stableCnt = Count = 0, i=0;
    while(stableCnt<3)
    {
        oldEL=EL;
        GetActImgFBAddr_Path1(&FB_para1);
        aaa_SetAeWindow((UB*)AE_Window_EqualWeighted);
        Yavg=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));//JJ_433CAL: need review

sprintf(message, "%2d Y%3d L%3d G%3d", i++, Yavg, EL, ISOValue);
Cal_AlgShowString(message,Cal_PanelMsgXPos(1),Cal_PanelMsgYPos(7));

        debug_err(("Yavg=%3d, EL=%3d, ISOValue=%3d\r\n", Yavg, EL, ISOValue));
        if (Yavg == 0)
        {
            Yavg = 1;
        }
        //1/4EV convergence
        if((Yavg>=ExpectY*119/100)||(Yavg<=ExpectY*100/119))
        {
            EL = (oldEL*ExpectY)/Yavg;
            if (EL >= MaxExpLine_640x480)
            {
                EL = MaxExpLine_640x480;
                MaxCnt++;
                if(MaxCnt++>2)
                {
                    debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
                    break;
                }
            }
            else if (EL <= MinExpLine)
            {
                EL = MinExpLine;
                MinCnt++;
                if(MinCnt++>2)
                {
                    debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
                    break;
                }
            }
        }
        else if((Yavg>=ExpectY*109/100)&&(EL>MinExpLine))
        {
            EL--;
        }
        else if((Yavg<=ExpectY*100/109)&&(EL<MaxExpLine_640x480))
        {
            EL++;
        }


        CalSetExpTime(EL);
        CalSetAgc(ISOValue);
        CalAeSet();
        if ((Yavg<ExpectY*109/100) && (Yavg>ExpectY*100/109))
        {
            stableCnt++;
        }
        else
        {
            if (oldEL == EL)
            {
                Count ++;
            }
            else
            {
                Count = 0;
            }
            if (Count > 20)
            {
                debug_err(("Ae Converge fail(WaitAeStable)\r\n"));
                break;
            }
        }

        //#NT#2009/09/01#KS Hung -begin
        //#NT#No this function
        //SensorDelayVD(5);
        //#NT#2009/09/01#KS Hung -end
        debug_ind(("Yavg=%d, EL=%d\r\n",Yavg,EL));
    }
    return Yavg;
}
#endif
UINT32 WaitAeStableISO(UINT32 ExpectY, UINT32 ExposureLine)
{
    UINT32 Yavg;
    UINT32 ISO, OldISO;
    UINT8 stableCnt, Count;

    ISO=200;//Agc_ISO50;
    stableCnt = 0;
    while(stableCnt<3)
    {
        OldISO = ISO;
        GetActImgFBAddr_Path1(&FB_para1);
        aaa_SetAeWindow((UB*)AE_Window_EqualWeighted);
        Yavg=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));//JJ_433CAL: need review

        //1/4EV convergence
        if((Yavg>=ExpectY*109/100)&&(ISO>AgcGain_min))
        {
            ISO--;
        }
        else if((Yavg<=ExpectY*100/109)&&(ISO<AgcGain_max))
        {
            ISO++;
        }

        if((Yavg>=ExpectY*119/100) && (ISO > AgcGain_min))
        {
            ISO -= 4;
        }
        else if((Yavg<=ExpectY*100/119) && (ISO<AgcGain_max))
        {
            ISO += 4;
        }

        CalSetExpTime(ExposureLine);
        CalSetAgc(ISO);
        CalAeSet();

        if ((Yavg<ExpectY*105/100) && (Yavg>ExpectY*100/105))
        {
            stableCnt++;
        }
        else
        {
            if (OldISO == ISO)
            {
                Count ++;
            }
            else
            {
                Count = 0;
            }
            if (Count > 20)
            {
                debug_err(("Ae Converge fail(WaitAeStableISO)\r\n"));
                break;
            }
        }

        sie_waitVD(3);
        debug_err(("Yavg=%d, ISO=%d\r\n",Yavg,ISO));
    }
    return ISO;
}


void SetRBgain_White(void)
{
}
void SetRBgain_Dark(void)
{
    aaa_SetColorgain(GetCFAPattern(),2*256, 256, 256, 2*256);
}
//AE for calibration use only
void CalSetExpTime(UINT32 Line)
{
    CalExpTime = Line;
}

void CalSetExpTime2(UINT32 Line, UINT32 Pixel)
{
    CalExpTime = Line;
    CalExpTimePxl = Pixel;
}


void CalSetExpTimeInUs(UINT32 uiExpTimeUs)
{
    UINT32 ExpLine, ExpPixel;
    UINT64 ui64Tmp;
    UINT32 HDTotal;

    HDTotal = GetHDTotalPrv();

    ui64Tmp = uiExpTimeUs;
    ui64Tmp *= PixelClkInKHz;
    ui64Tmp /= (1000);

    ExpLine = (ui64Tmp)/HDTotal;

    ExpPixel = ui64Tmp - (ExpLine*HDTotal);

    debug_err(("Cal ExpTime %4d lines, %4d pxls\r\n", ExpLine, ExpPixel));

    ExpLine++;//one more line is needed for ExpPixel
    CalSetExpTime2(ExpLine, ExpPixel);
}

void CalSetAgc(UINT32 Agc)
{
    CalAgc = Agc;
}

void CalAeSet(void)
{
//JJ_433Cal: use new api
/*
    SetPreVDLength(GetVDTotalPrv());
    SetPreExpLine(CalExpTime);
    SetPreExpHighPrecision_Cal(CalExpTimePxl);
    SetPara_PgaGain(CalAgc);//#NT#2007/10/23#JJ Huang SetPreAGC(CalAgc);
    //debug_err(("CalAgc = %d, CalExpTime = %d\r\n", CalAgc, CalExpTime));
*/
}

#define CAL_AE_PRV2CAP_MODIFY 1//JJ0403 for AWB Adjust Lum-Accuracy

#if CAL_AE_PRV2CAP_MODIFY//JJ0403 for AWB Adjust Lum-Accuracy
BOOL bCalAePrv2CapTranByExpT = FALSE;
#endif
void CalAePrv2Cap(UINT32 ExpTime,UINT32 ExpTimePxl,UINT32 AgcGain)
{
#if CAL_AE_PRV2CAP_MODIFY//JJ0403 for AWB Adjust Lum-Accuracy
    if(bCalAePrv2CapTranByExpT)
        ExpTime = ExpTime * 4;
    else
        AgcGain=AgcGain+(171*2);
#else
    AgcGain=AgcGain+(171*2);
#endif
    if(ExpTime == 0)
    {
       ExpTime = 1;
    }

    //JJ_433CAL: use new api// SetCapExpLine(ExpTime);
    //JJ_433CAL: use new api// SetCapExpHighPrecision(ExpTimePxl);
    //JJ_433CAL: use new api// SetPara_PgaGain(AgcGain);
    debug_err(("Cap ExpT: %d,%d; G %d\r\n", ExpTime, ExpTimePxl, AgcGain));
}

void CalSetExpTimeIn1HD(UINT32 ExposureTime)
{
#if 0//Mark Useless Code
    UINT32 n;
    float temp;
    temp = ExposureTime;
    temp = (temp - 1.66667/*60T*/) / 0.4444/*16T*/;
    n = temp;
#endif//Mark Useless Code
}
//----------------------------------------------------------------------------------------------------
//Capture for calibration use only
UINT8 CalGet_ExpoFrame(void)
{
#if 0
    return 0;
#else
    if(!aaa_GetAeCalibration())
        CalExpoFrame = (CalExpTime)/(MaxExpLine_640x480+1);//(CalExpTime<<1)/(MaxExpLine+1);
    else
        CalExpoFrame = (CalExpTime)/(MaxExpLine_640x480+1);
    debug_err(("Cap ExpF: %d\r\n", CalExpoFrame));
    return CalExpoFrame;
#endif
}

//for DP calibration
void CalSie_Set2Preview(unsigned long    PreviewCenterH,
                        unsigned long    PreviewCenterV,
                        unsigned long    Sie_InHsize,
                        unsigned long    Sie_InVsize,
                        unsigned long    Sie_OutHsize,
                        BOOL ToDram, // 0: to IPE, 1: to DRAM
                        unsigned int    D_Addr)
{
#if 0// No set2Preview in Calibration Code
    #if 1
    unsigned int    Sie_OutVsize;
    unsigned short Img_Width;

    debug_err(("JJ test 1113\r\n"));
    debug_err(("CalSie_Set2Preview(%d,%d,%d,%d,%d,%d,0x%x)\r\n", PreviewCenterH, PreviewCenterV, Sie_InHsize, Sie_InVsize, Sie_OutHsize, ToDram, D_Addr));

    //if(CCD_VGA_OUTPUT)
    //    Sie_OutVsize=Sie_InVsize*2;
    //else
        Sie_OutVsize=Sie_InVsize;
    sie_SetSensorSync(GetHDSyncPrv(), GetHDTotalPrv(), GetVDSyncPrv()*2, GetVDTotalPrv()*2);
    sie_Reset(1);

//No Need Anymore in 433// #if 0//Padding_Prv
//No Need Anymore in 433//     sie_SetInterleave_LineInterval(0x28);
//No Need Anymore in 433// #else
//No Need Anymore in 433//     sie_SetInterleave_LineInterval(0x0f);
//No Need Anymore in 433// #endif

    sie_SetBreakPoint(100, 200);
    sie_SetLoadTiming(1);
    sie_SyncPhase.Master=0;
    sie_SyncPhase.HD_Phase=1;
    sie_SyncPhase.VD_Phase=0;//JJ0307 follow SensorLib 1;
    sie_SyncPhase.Data_Phase=1;//JJ0307 follow SensorLib 0;
    sie_SetSyncPhase(sie_SyncPhase);

    sie_FieldAct.VH_Act = GetACTStartXPrv();
    sie_FieldAct.VV_Act1 = GetACTStartYPrv();
    sie_FieldAct.VV_Act2 = GetACTStartYPrv();
    sie_FieldAct.VV_Act3 = GetACTStartYPrv();
    sie_FieldAct.VV_Act4 = GetACTStartYPrv();
    sie_FieldAct.VV_Act5 = GetACTStartYPrv();
    sie_SetFieldAct(sie_FieldAct);
#if 0//Padding_Prv
        pre_FunctionEnable(ENABLE,PRE_PADDING_EN);
        Img_Width = Sie_OutHsize-16;
#else
        pre_FunctionEnable(DISABLE,PRE_PADDING_EN);
        Img_Width = Sie_OutHsize;
#endif
    sie_SetActWindow((PreviewCenterH-Sie_InHsize/2-GetACTStartXPrv()),(PreviewCenterV-Sie_InVsize/2-GetACTStartYPrv()),
                        Sie_InHsize,Sie_InVsize);

    sie_SensorData_Enable(0);
    pre_FunctionEnable(ENABLE,PRE_GAMMA_EN);
    pre_FunctionEnable(DISABLE,PRE_CG_EN|SIE_HSCL_EN);

    //------------------Lens shading setting------------------//
    //if (aaa_GetVIGCalibration())
    //{
    //    sie_FunctionEnable(ENABLE,SIE_VIG_EN);
    //    sie_Set_VIG_LUT_432((UB*)Shading_Table, (UB*)Shading_Table, (UB*)Shading_Table);
    //    sie_Set_VIG_Position_432((PreviewCenterH-Sie_InHsize/2-ACT_STRX_Prv) + (Sie_InHsize>>1),
    //                    (PreviewCenterV-Sie_InVsize/2-ACT_STRY_Prv) + (12*(Sie_InVsize)>>1), 11, 0);
    //    sie_Set_VIG_Param_432(81, 9, 9);
    // }
    //else
    {
        pre_FunctionEnable(DISABLE,PRE_VIG_EN);
    }
    //------------------Lens shading setting end--------------//

    //if(CCD_VGA_OUTPUT)
    //{
    //     sie_FunctionEnable(ENABLE,SIE_HINTLV_EN);
    //    sie_FunctionEnable(DISABLE,SIE_HINTLV_SEL);
    //    pre_HScale(Sie_InHsize/2,Img_Width);
    //}
    //else
    {
        pre_FunctionEnable(DISABLE,PRE_HINTLV_EN);
        pre_HScale(Sie_InHsize,Img_Width);
    }
    sie_SetLineOffset(Sie_OutHsize);


    //if(GetDPAdd_Prv()!=0)
    //{
    //    sie_Set_DefectPixel(GetDPAdd_Prv(), GetDPAdd_Cap());
    //    sie_FunctionEnable(ENABLE,SIE_BLM_EN);
    //}
    //else
    //{
    //    sie_FunctionEnable(DISABLE,SIE_BLM_EN);
    //}

    //if(AUTO_OB)
    //{
    //    sie_OB_Param.X0=100;
    //    sie_OB_Param.X1=2800;
    //    sie_OB_Param.Y0=0;
    //    sie_OB_Param.Y1=0;
    //    sie_OB_Param.SubSample=4;
    //    sie_OB_Param.Thres=0x80;
    //    sie_Set_OB_Window(sie_OB_Param);
    //    sie_SetOBGainOffset(0x00,32);
    //    sie_FunctionEnable(ENABLE,SIE_OB_AVER_EN|SIE_OB_EN);
    //}
    //else
    {
        sie_SetOBGainOffset(SensorIF_GetOB(),32);
        sie_FunctionEnable(DISABLE,SIE_OB_AVER_EN|SIE_OB_EN);
    }

    pre_SetOutputSize(Sie_OutHsize);

    //sie_Set_ColorOffset(0,0,0,0);
    // no such func in 433// sie_Set_ColorCut(0,0);

    if(ToDram)
    {
        sie_SetOutputStartAdd(D_Addr); // output address 1
        sie_SetOutputStartAdd1(D_Addr+Sie_OutHsize*Sie_InVsize);//JJ_Temp_For_AO Get_DzoomPrvMaxBuffer()); // output address 2
        sie_SetLineOffset(Sie_OutHsize);
        //no such func in 433// sie_Set_FrameSwap(SIE_FRAME_MODE_SWAP);
        //no such func in 433// sie_Set_FrameAddInc(SIE_FRAME_INC_AUTO);

        #if ((PRV_USING_IPE_GAMMA == ENABLE) && (PRV_USING_IPE_COLOR_GAIN == ENABLE))
        pre_SetPre2DramPath(0); //JJ_433CAL: no such definition
        #elif ((PRV_USING_IPE_GAMMA == ENABLE) && (PRV_USING_IPE_COLOR_GAIN == DISABLE))
        pre_SetPre2DramPath(1); //JJ_433CAL: no such definition
        #else
        pre_SetPre2DramPath(2); //JJ_433CAL: no such definition
        #endif

    }
    else
    {
        debug_err(("NG in 433\r\n"));
    }
    pre_SetGamma(CalGammaLUT_T1, CalGammaLUT_T1, CalGammaLUT_T1);
    sie_IntrptEnable(DISABLE,SIE_INT_ALL);
    dis_int(_INT_SIE);
    def_int(_INT_SIE, sie_isr);
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_IntrptEnable(ENABLE,SIE_INTE_FLDEND|SIE_INT_BP1);
    ena_int(_INT_SIE);
    sie_Load();

    def_int(_INT_IPE, ipe_isr);
    ena_int(_INT_IPE);
    sie_Reset(0);
    #else
    unsigned int    Sie_OutVsize;
    unsigned short  Img_Width;

    Sie_OutVsize=Sie_InVsize;
    sie_SetSensorSync(GetHDSyncPrv(),GetHDTotalPrv(),GetVDSyncPrv()*2,GetVDTotalPrv()*2);
    sie_Reset(1);

    sie_SetBreakPoint(20, 128);
    sie_SetLoadTiming(1);
    //sie_HDVD_PhaseInverse(1,1);
    sie_SyncPhase.Master=0;
    sie_SyncPhase.HD_Phase=1;
    sie_SyncPhase.VD_Phase=1;
    sie_SyncPhase.Data_Phase=1;
    sie_SetSyncPhase(sie_SyncPhase);

    sie_FieldAct.VH_Act = GetACTStartXPrv();
    sie_FieldAct.VV_Act = GetACTStartYPrv();
    sie_SetFieldAct(sie_FieldAct);

    sie_FunctionEnable(DISABLE,SIE_PDDING_EN);
    Img_Width = Sie_OutHsize;

    sie_SetActWindow( (PreviewCenterH-Sie_InHsize/2-GetACTStartXPrv()),
                        (PreviewCenterV-Sie_InVsize/2-GetACTStartYPrv()),
                        Sie_InHsize,Sie_InVsize);

    sie_SensorData_Enable(0);
    sie_FunctionEnable(DISABLE,SIE_BLM_EN|SIE_CA_EN);
    sie_FunctionEnable(ENABLE,SIE_CG_EN|SIE_GAMMA_EN|SIE_HSCL_EN|SIE_LOAD_TIMING);
    pre_HScale(Sie_InHsize,Img_Width);
    sie_SetLineOffset(Sie_OutHsize);

    if(0)//AUTO_OB)
    {
        sie_OB_Param.X0=0;
        sie_OB_Param.X1=512;
        sie_OB_Param.Y0=0;
        sie_OB_Param.Y1=1;
        sie_OB_Param.SubSample=2;
        sie_OB_Param.Thres=0x200;
        sie_Set_OB_Window(sie_OB_Param);
        sie_SetOBGainOffset(0x00);
        sie_FunctionEnable(ENABLE,SIE_OB_AVER_EN|SIE_OB_EN);
    }
    else
    {
        sie_SetOBGainOffset(SensorIF_GetOB());
        sie_FunctionEnable(DISABLE,SIE_OB_AVER_EN|SIE_OB_EN);
    }
    sie_Set_Gamma(CalGammaLUT_T1);
    pre_SetOutputSize(Sie_OutHsize);
    sie_Set_ColorOffset(0,0,0,0);

    //#NT#2007/10/23#JJ Huang sie_Set_DramSao(1, D_Addr);
    //#NT#2007/10/23#JJ Huang sie_Set_DramSaoBufnum(0);
    sie_SetLineOffset(Sie_OutHsize);

    sie_FunctionEnable(ENABLE,SIE_PRVW_OEN);
    //#NT#2007/10/23#JJ Huang sie_Set_OutPath(1);// 1: to DRAM
    if(gImageAlgInfo.SiePath == _PreColorGain)
    {
        pre_SetPre2DramPath(SIE_DRAM_PATH_PRE_COLORGAIN);//#NT#2007/10/23#JJ Huang         sie_Set_OutSrc(SIE_DRAM_PATH_PRE_COLORGAIN);
    }
    else if(gImageAlgInfo.SiePath == _PreGamma)
    {
        pre_SetPre2DramPath(SIE_DRAM_PATH_PRE_GAMMA);//#NT#2007/10/23#JJ Huang         sie_Set_OutSrc(SIE_DRAM_PATH_PRE_GAMMA);
    }
    else if(gImageAlgInfo.SiePath == _PostGamma)
    {
        pre_SetPre2DramPath(SIE_DRAM_PATH_POST_GAMMA);//#NT#2007/10/23#JJ Huang         sie_Set_OutSrc(SIE_DRAM_PATH_POST_GAMMA);
    }
    else
    {
        debug_err(("Non-defined value for gImageAlgInfo.SiePath\r\n"));
        pre_SetPre2DramPath(SIE_DRAM_PATH_POST_GAMMA);//#NT#2007/10/23#JJ Huang         sie_Set_OutSrc(SIE_DRAM_PATH_POST_GAMMA);
    }

    sie_IntrptEnable(DISABLE,SIE_INT_ALL);
    dis_int(_INT_SIE);
    def_int(_INT_SIE, sie_isr);
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_IntrptEnable(ENABLE,SIE_INTE_FLDEND|SIE_INT_BP1);
    ena_int(_INT_SIE);
    sie_Load();

    def_int(_INT_IPE, ipe_isr);
    ena_int(_INT_IPE);
    sie_Reset(0);
    #endif
#endif// No set2Preview in Calibration Code
}

void CalSie_Set2Capture(UINT32 CapAdd, UINT16 CapCenterH, UINT16 CapCenterV, UINT16 ImageHSize, UINT16 ImageVSize, UINT8 SiePath, BOOL autoOB)
{
#if 0// No set2Capture in Calibration Code
    UINT uiFlag;
    UINT8 capdone, steps;
    unsigned long Img_Width;
    unsigned short ExpoFrame;
    unsigned short Line,Pixel2;
    UINT32 uiPriority0, uiPriority1, FieldVSize;
    //AWB_INFO awbInfo;

    dma_getPriority(&uiPriority0, &uiPriority1);
    uiPriority0 |= ((unsigned long)(P0_SIER0|P0_SIER1));
    dma_setPriority(uiPriority0, uiPriority1);
    //---------------------------------------------------------------------//
    Lens_OnOff(1);
    *(unsigned long volatile *)(0x98100004)|=(0x3<<10);
#if Padding_Cap
    Img_Width = ImageHSize-16;
#else
    Img_Width = ImageHSize;
#endif
    sie_SensorData_Enable(0);

    sie_SetSensorSync(GetHDSyncPrv(),GetHDTotalPrv(),GetVDSyncPrv()*2,GetVDTotalPrv()*2);
    Sensor_TG_CFG(0x4000, 3/*PREVIEW_MODE*/);

    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_SetBreakPoint(100, 200);
    sie_FunctionEnable(DISABLE, SIE_CROP_EN);
    sie_FunctionEnable(DISABLE, SIE_PRVW_OEN);

    sie_IntrptEnable(DISABLE,SIE_INTE_ALL);
    dis_int(_INT_SIE);
    def_int(_INT_SIE, sie_isr);
    sie_ClrIntrStatus(SIE_INT_ALL);
    while (!(sie_ReadIntrStatus() & SIE_INT_FLDEND));
    sie_ClrIntrStatus(SIE_INT_ALL);

    sie_IntrptEnable(ENABLE, SIE_INTE_FLDEND|SIE_INTE_BP1|SIE_INTE_BP2);
    ena_int(_INT_SIE);
    capdone = 0;

    CalAePrv2Cap(CalExpTime, CalExpTimePxl, CalAgc);
    ExpoFrame = CalGet_ExpoFrame();
    steps = (ExpoFrame > 0) ? 0 : 2;
    FilesysPause();
    FilesysWaitPaused();
    cap_cnt = 8 + ExpoFrame;
    clr_flg(FLG_ID_PHOTO, FLGPHOTO_BP1);
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //8

    if (ExpoFrame > 0)
    {
        #if 0//(PREVIEW_MODE == MONITOR_MODE_A)
        //set OFDC1 to high
        Sensor_TG_CFG(0x7c42, 0x9800);
        #elif 1//(PREVIEW_MODE == MONITOR_MODE_C)
        //set OFDC1 & OFDC2 to high
        Sensor_TG_CFG(0x7c42, 0xD81F);//JJ0222 0xD800);
        #endif

        #if 0//(PREVIEW_MODE == MONITOR_MODE_A)
        //disable V7A & V9A
        Sensor_TG_CFG(0x7c51, 0x8000);
        Sensor_TG_CFG(0x7c52, 0x0002);
        Sensor_TG_CFG(0x7c49, 0x7fff);
        Sensor_TG_CFG(0x7c4a, 0xfffd);
        #elif 1//(PREVIEW_MODE == MONITOR_MODE_C)
        //disable V5 & V9A & &V9B
        Sensor_TG_CFG(0x7c51, 0x2000);
        Sensor_TG_CFG(0x7c52, 0x0006);
        Sensor_TG_CFG(0x7c49, 0xdfff);
        Sensor_TG_CFG(0x7c4a, 0xfff9);
        #endif

#if 1//(USING_AO_AE_AWB == ENABLE)
        flash_ConfigMainFlash(aaa_GetSubLine() + 3, 10/*AEParam.StrobeTime*/);
#else
        flash_ConfigMainFlash(GetVDTotalPrv()-10, 10);
#endif
        flash_TriggerFlash();


        wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);
        //set exposure line
        Sensor_TG_CFG(0x4002/*SUBCK_NUM*/, 0x0000);
        SetCapExpHighPrecision(0);
    }

    while(cap_cnt > 7)  //7
    {
        wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);
    }
    if (ExpoFrame == 0)
    {
        #if 0//(PREVIEW_MODE == MONITOR_MODE_A)
        //set OFDC1 to high
        Sensor_TG_CFG(0x7c42, 0x9800);
        #elif 1//(PREVIEW_MODE == MONITOR_MODE_C)
        //set OFDC1 & OFDC2 to high
        Sensor_TG_CFG(0x7c42, 0xD81F);//JJ0222 0xD800);
        #endif
        Sensor_TG_CFG(0x7c51, 0x2000);
        Sensor_TG_CFG(0x7c52, 0x0006);
        Sensor_TG_CFG(0x7c49, 0xdfff);
        Sensor_TG_CFG(0x7c4a, 0xfff9);
#if 1//(USING_AO_AE_AWB == ENABLE)
        flash_ConfigMainFlash(aaa_GetSubLine() + 3, 10/*AEParam.StrobeTime*/);
#else
        flash_ConfigMainFlash(GetVDTotalPrv()-10, 10);
#endif
        flash_TriggerFlash();
    }
    Time2_Line_Pixel2_v2(Cal_Mshutter,PixelClkInKHz,&Line,&Pixel2);
    sie_SetBreakPoint(100, Line);


    //exposure field, may take flash light in this field
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //6
    ShutterCloseFlag = TRUE;
    //set exposure line
    Sensor_TG_CFG(0x4002/*SUBCK_NUM*/, 0x0000);
    SetCapExpHighPrecision(0);

    Sensor_TG_CFG(0x7c51, 0xf800);
    Sensor_TG_CFG(0x7c52, 0xfe1f);

    Sensor_TG_CFG(0x7c49, 0xffff);
    Sensor_TG_CFG(0x7c4a, 0xffff);

    Sensor_TG_CFG(0x4000, 0/*CAPTURE_MODE*/);
    sie_CaptureSet(CapAdd,CapCenterH,CapCenterV,Img_Width,ImageVSize);
    sie_Cap2Dram();

    //1 still field
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //5
    FilesysResume();

    //2 still field
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //4
#if 0//(CAP_RECAL_AWB == ENABLE)
    if (gImageAlgInfo.SieBitDepth == _SieBitDepth_16)
    {
        AEAlgInfo.FrameBufInfo.BufAddr = CapAdd + (4 * ImageHSize * gImageAlgInfo.SieBitDepth >> 3);
        AEAlgInfo.FrameBufInfo.ImgWidth = ImageHSize;
        AEAlgInfo.FrameBufInfo.ImgHight = FieldVSize;
        AEAlgInfo.FrameBufInfo.ImgLineOffset = (5 * ImageHSize * gImageAlgInfo.SieBitDepth >> 3);
        aaa_AWBStillEvaluation(&AEAlgInfo, &awbInfo);
        AWBParam.CapRgain = awbInfo.Rgain;
        AWBParam.CapGgain = awbInfo.Ggain;
        AWBParam.CapBgain = awbInfo.Bgain;
    }
    debug_ind(("%d %d %d \r\n",AWBParam.CurRgain,AWBParam.CurGgain,AWBParam.CurBgain));
#endif
    //3 still field
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //3

    //4 still field
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //2

    //5 still field
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //1
    Sensor_TG_CFG(0x4000, 3/*PREVIEW_MODE*/);

    //preview mode
    wai_flg(&uiFlag, FLG_ID_PHOTO, FLGPHOTO_BP1, TWF_CLR|TWF_ORW);      //0
    //OFDC1 & OFDC2 low
    Sensor_TG_CFG(0x7c42, 0x181F);//JJ0222 0x1800);
    shutter_move(OPEN);
#endif// No set2Capture in Calibration Code
}

void CalSetSiePath(UINT8 path)
{
    if(path==0)
    {
        pre_SetPre2DramPath(0);//JJ_433CAL: no such definition     sie_Set_OutSrc(SIE_DRAM_PATH_PRE_COLORGAIN);
    }
    else if(path==1)
    {
        pre_SetPre2DramPath(1);//JJ_433CAL: no such definition       sie_Set_OutSrc(SIE_DRAM_PATH_PRE_GAMMA);
    }
    else if(path==2)
    {
        pre_SetPre2DramPath(2);//JJ_433CAL: no such definition        sie_Set_OutSrc(SIE_DRAM_PATH_POST_GAMMA);
    }
    else
    {
        debug_err(("Non-defined value for Calibration SiePath\r\n"));
        pre_SetPre2DramPath(2);//JJ_433CAL: no such definition      sie_Set_OutSrc(SIE_DRAM_PATH_POST_GAMMA);
    }
}

void Get_Sensor_CalInfor(void)
{
    CalInfor.DefaultAgcISOMin=Default_Agc_ISOMin;
    CalInfor.DefaultMshutter=Default_Mshutter;
    CalInfor.DefaultAWBRgain=Default_AWB_Rgain;
    CalInfor.DefaultAWBBgain=Default_AWB_Bgain;
    CalInfor.DefaultFlashTriggerPoint=Default_FlashTriggerPoint;
    CalInfor.DefaultMinOB=Default_Min_OB;
    CalInfor.DefaultFlashYAvg=Default_Flash_Y_Avg;
    CalInfor.DarkLimitPrv=Dark_Limit_Prv;
    CalInfor.DarkLimitCap=Dark_Limit_Cap;
    CalInfor.BrightLimitPrv=Bright_Limit_Prv;
    CalInfor.BrightLimitCap=Bright_Limit_Cap;
    CalInfor.BlockPrv.Block_X=Block_X_Prv;
    CalInfor.BlockPrv.Block_Y=Block_Y_Prv;
    CalInfor.BlockCap.Block_X=Block_X_Cap;
    CalInfor.BlockCap.Block_Y=Block_Y_Cap;

    CalInfor.SkipPrv.SkipXL=SkipXL_Prv;
    CalInfor.SkipPrv.SkipXR=SkipXR_Prv;
    CalInfor.SkipPrv.SkipYU=SkipYU_Prv;
    CalInfor.SkipPrv.SkipYD=SkipYD_Prv;

    CalInfor.SkipPrv2.SkipXL=SkipXL_Prv2;
    CalInfor.SkipPrv2.SkipXR=SkipXR_Prv2;
    CalInfor.SkipPrv2.SkipYU=SkipYU_Prv2;
    CalInfor.SkipPrv2.SkipYD=SkipYD_Prv2;

    CalInfor.SkipPrv3.SkipXL=SkipXL_Prv3;
    CalInfor.SkipPrv3.SkipXR=SkipXR_Prv3;
    CalInfor.SkipPrv3.SkipYU=SkipYU_Prv3;
    CalInfor.SkipPrv3.SkipYD=SkipYD_Prv3;

    CalInfor.SkipCap.SkipXL=SkipXL_Cap;
    CalInfor.SkipCap.SkipXR=SkipXR_Cap;
    CalInfor.SkipCap.SkipYU=SkipYU_Cap;
    CalInfor.SkipCap.SkipYD=SkipYD_Cap;
    CalInfor.MaxContDPPrv=Max_Continue_DP_Prv;
    CalInfor.MaxContDPCap=Max_Continue_DP_Cap;
    CalInfor.FlashYTarget=FlashYAvgTarget;
    CalInfor.FLashYMargin=FLashYAvgMargin;
}

void WriteDPdata(void)
{
    char DPfile[]="A:\\DP_Data.txt\0";

    HNVT_FILE     *pFile;

    pFile = fs_fopen(DPfile,"w");
    if(pFile == NULL)
    {
        debug_err(("File open fail !!"));
    }
    fs_fwrite((void *)&AAA_Adjust.DP_Status, 1, sizeof(AAA_Adjust.DP_Status), pFile);
    fs_fwrite((void *)AAA_Adjust.Defect_Pixel, 1, (DP_Prv_MAX+DP_Cap_MAX)*4, pFile);
    fs_fclose(pFile);
}

UINT8 StainTest(void)
{
    UINT8 ErrorCode=ERROR_OK;

    Cal_AlgShowString("Stain Test is supposed",         Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(0));
    Cal_AlgShowString("to run with main code",         Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(1));

    return ErrorCode;
}

UINT8 TVResolutionTest(void)
{
    UINT8 ErrorCode=ERROR_OK;

    Cal_AlgShowString("TV-resolution is supposed",         Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(0));
    Cal_AlgShowString("to run with main code",         Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(1));

    return ErrorCode;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
UB ISOAdjust(void)
{
    //#NT#2009/09/01#KS Hung -begin
    FLGPTN uiFlag1,uiFlag;
    UINT   Yavg;
    //#NT#2009/09/01#KS Hung -end
    UINT8 ErrorCode;
    UINT16 R, G, B;
    UINT ExpTime, AGCGain, Interval, NormalExpectY, rtStatus;
    Cal_AlgClearScreen();
    debug_err(("ISO Adjust.\r\n"));
    Cal_AlgShowString("LV MACHINE WITH LV 9!\r\n", Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(4));
    Cal_AlgShowString("Press Left to continue\r\n", Cal_PanelMsgXPos(0),Cal_PanelMsgYPos(6));
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    clr_flg(FLG_ID_KEY,FLGKEY_LEFT);
    Cal_AlgClearScreen();
    Photo_SetImageResolution(_Resolution_1440x1080, IPL_MODE_PREVIEW);
    Photo_LockAE(LOCK);
    Photo_LockAWB(LOCK);
    wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_2NDIDLE, TWF_ORW);

    rtStatus = _ISO_Status;
    ErrorCode = ERROR_OK;

//lv9 , 1/30s ,iso 50 , no bining
    ExpTime = 33333;
    AGCGain = 1500;
    Interval = 60;
    NormalExpectY = 125;
    while(1)
    {
        AeSetAGCGainStep(AGCGain);
        AeSetPrvExpoTime(ExpTime);
        sie_waitVD(6);
        Yavg = CalculateRawY(ippbuf_GetBuf(IPPBUF_PRV_RAW_1), 0, 0, GetIPPSizeInfor(_PreCropHsize), GetIPPSizeInfor(_PreCropVsize), 8, 1, &R, &G, &B);
        debug_err(("%d %d %d\r\n", Yavg, ExpTime, AGCGain));
        if (Yavg > NormalExpectY*103/100)
        {
            AGCGain -= Interval;
        }
        else if(Yavg < NormalExpectY*97/100)
        {
            AGCGain += Interval;
        }
        else
        {
            if(AGCGain<1500)
                AGCGain= 1500;
            break;
        }

        if(Interval <=3)
        {
            break;
        }
        Interval = Interval-3;
    }

    if(AGCGain<Agc_ISO50_Min)
        ErrorCode=ERRORCODE_ISO_MIN;
    else if(AGCGain>Agc_ISO50_Max)
        ErrorCode=ERRORCODE_ISO_MAX;

    debug_err(("Yavg = %d, ExpT = %d, AGC = %d,Err = %d\r\n", Yavg, ExpTime, AGCGain, ErrorCode));
    Cal_SetCalibrationData(_Cal_ISO, &AGCGain);
    Cal_SetCalibrationData(_CalStatus_ISO, &rtStatus);

    if(ErrorCode == ERROR_OK)
    {
        AAA_Adjust.ISO_Status =_ISO_Status;
        AAA_Adjust.Agc_ISOMin=AGCGain;
    }
    WriteCalData();

    if((ErrorCode == ERROR_OK))
    {
        Cal_AlgShowString("PASS",Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(4));
    }
    else
    {
        sprintf(message, "NG %d", ErrorCode);
        Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(4));
    }

    Cal_AlgShowString("PRESS left key to continue",Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(8));

    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    clr_flg(FLG_ID_KEY,FLGKEY_LEFT);
    debug_msg("Read after Cal: AGCGain  %d\r\n", AAA_Adjust.Agc_ISOMin);// as a nand R/W test msg

    return ErrorCode;
}

UB AWB_Adjust(void)
{
    UINT uiFlag1, rtStatus, LumY;
    UINT8 ErrorCode;
    FLGPTN uiFlag;
    UINT16 RGB[3], Rgain,Bgain;
    UINT32 ExpoTime, AGCGain, Mshutter, RawAddr;

    debug_msg("AWB_Adjust\r\n");
    Cal_AlgClearScreen();
    Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_PREVIEW);
    Photo_LockAWB(LOCK);
    Photo_LockAE(LOCK);
    SetIPPFuncInfor(FQV_EN, DISABLE);

    RawAddr = gCapRawAddr;
    rtStatus = _AWB_Status;
    ErrorCode = ERROR_OK;
    WaitAeConverge();

    //#NT#2009/09/01#KS Hung -begin
    //#NT#argument number is error
    //Photo_WaitPhotoIdle(TRUE);//wai_flg(&uiFlag1, FLG_ID_PHOTO, FLGPHOTO_IDLE, TWF_ORW);
    //#NT#2009/09/01#KS Hung -end
    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);

    RawAddr = ippbuf_GetBuf(IPPBUF_PRV_END);
    CalCaptureOne(RawAddr, GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize), PATH_PRE_CLOLRGAIN, 0x50, Mshutter);

#if 0
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*)RawAddr,
                    GetIPPSizeInfor(_PreCropCapHsize)*GetIPPSizeInfor(_PreCropCapVsize),
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
    debug_err(("CalCaptureOne %d x %d\r\n",GetIPPSizeInfor(_PreCropCapHsize), GetIPPSizeInfor(_PreCropCapVsize)));
#endif

    LumY = CalculateRawY(RawAddr, GetIPPSizeInfor(_PreCropCapHsize)/4,
                         GetIPPSizeInfor(_PreCropCapVsize)/4,
                         GetIPPSizeInfor(_PreCropCapHsize)/2,
                         GetIPPSizeInfor(_PreCropCapVsize)/2,
                         8, 1, &RGB[0], &RGB[1], &RGB[2]);
#if 0
    sprintf(message, "%d",LumY);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(6));
    debug_err(("R = %d G = %d B = %d\r\n", RGB[0], RGB[1], RGB[2]));
    sprintf(message, "%d",RGB[0]);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(0));
    sprintf(message, "%d",RGB[1]);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(2));
    sprintf(message, "%d",RGB[2]);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(4));
#endif
    Rgain = RGB[1]*256/RGB[0];
    Bgain = RGB[1]*256/RGB[2];

    if(Rgain <= Rgain_Min)
    {
        ErrorCode = ERRORCODE_AWBR_MIN;
    }
    else if(Rgain >= Rgain_Max)
    {
        ErrorCode = ERRORCODE_AWBR_MAX;
    }
    if(Bgain <= Bgain_Min)
    {
        ErrorCode = ERRORCODE_AWBB_MIN;
    }
    else if(Bgain >= Bgain_Max)
    {
        ErrorCode = ERRORCODE_AWBB_MAX;
    }

    AAA_Adjust.AWB.Rgain = RGB[1]*256/RGB[0];
    AAA_Adjust.AWB.Bgain = RGB[1]*256/RGB[2];
    if((ErrorCode == ERROR_OK))
    {
        AAA_Adjust.AWB_Status = rtStatus;
        debug_msg("AAA_Adjust.AWB_Status = %d\r\n",AAA_Adjust.AWB_Status);
    }

    //Move to CAL_PassAdjustData
    //Cal_SetCalibrationData(_Cal_AWB_Statistic, (UINT *)RGB);
    //Cal_SetCalibrationData(_CalStatus_AWB, &rtStatus);

    debug_err(("After Cal: AAA_Adjust.AWB  %3d  %3d\r\n", AAA_Adjust.AWB.Rgain, AAA_Adjust.AWB.Bgain));// as a nand R/W test msg

    sprintf(message, "AWB RB gain :%d %d",AAA_Adjust.AWB.Rgain,AAA_Adjust.AWB.Bgain);
    Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(0));
    TimerDelayMs(2000);
    AeCap2Prv();
    Cal_AlgClearScreen();
    IplObj.uiCmd = IPL_CMD_CHG_MODE;
    IplObj.Mode = IPL_MODE_PREVIEW;
    IPL_SetCmd(&IplObj);

    WriteCalData();

    if((ErrorCode == ERROR_OK))
    {
        Cal_AlgShowString("PASS",Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(4));
    }
    else
    {
        sprintf(message, "NG %d", ErrorCode);
        Cal_AlgShowString(message,Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(4));
    }
    Cal_AlgShowString("PRESS left key to continue",Cal_PanelMsgXPos(2),Cal_PanelMsgYPos(8));

    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_LEFT, TWF_ORW|TWF_CLR);
    clr_flg(FLG_ID_KEY,FLGKEY_LEFT);
    debug_msg("Read after Cal: AAA_Adjust.AWB  %3d  %3d\r\n", AAA_Adjust.AWB.Rgain, AAA_Adjust.AWB.Bgain);// as a nand R/W test msg
    return ErrorCode;
}


UINT8 FlashlightAEAdjust(void)
{
}

UINT8 FlashlightAWBAdjust(void)
{
}

UINT8 FlashlightRedEyeTest(void)
{
}

UINT8 FlashlightGnoShots(void)
{
}

//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
UINT32 SNRTest(void)
{
    SenSNR_EnableSNRMeasure(TRUE);
    return E_OK;
}
//#NT#2010/12/15#JJ Huang -end

#endif
