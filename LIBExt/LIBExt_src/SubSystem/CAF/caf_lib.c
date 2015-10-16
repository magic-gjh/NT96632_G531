#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Type.h"
#include "debug.h"
#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "oscommon.h"
#include "ipe_lib.h"
#include "Lens.h"
//#NT#2010/02/22#Jarkko Chang -begin
#include "Caf_lib.h"
//#NT#2010/02/22#Jarkko Chang -end
BOOL FocusTrack_Enable = FALSE;
BOOL RT_AF_Direction;
unsigned long LastAF_Value;
unsigned char RT_AF_Status=0,RT_AF_Step;

unsigned short TargetIdx;
unsigned short RTAF_BestIdx;
unsigned short Recode_FocusStep[100];
unsigned char Index_Cnt=0;
unsigned long Tracking_StableCnt=0;
UINT32 cafHist[64], AvgY;

BOOL RtAFLockFlag = FALSE;
BOOL InitialFlag = TRUE;
BOOL VAEnableFlag = FALSE;
BOOL FocusRun=FALSE;
BOOL Inv_Direction=FALSE;
BOOL Resum_TrackingFocus;
INT16 RTAF_MinIndex,RTAF_MaxIndex;
UINT32 VACnt;

//extern INT8 Focus_TempComp_Far[13];
//extern INT16 Focus_Zoom_Tbl_Dist[12];
//extern INT16 Focus_Zoom_Tbl[13][12];
//extern INT16 Get_FocusPosition(void);
//extern void Focus_Go2(INT16 NewPosition);
extern UINT32 AeGetStableCount(void);
extern void AF_AFPrepare(BOOL bOP);
extern void AF_GetCurrentRange(INT16* piStartIdx, INT16* piEndIdx, INT8* piStep, INT16 *iDefaultIdx);
extern int TimerDelayMs(unsigned long ms);
//#NT#2010/02/10#Jarkko Chang -begin
//#NT#modify for caf
//extern UINT32 User_VABegin(UINT8 *Param);
extern ER User_VABegin(AF_IPEINFO *AFInfo);
extern int  User_VAEnd(AF_IPEINFO *AFInfo);
//extern int  User_VAEnd(UINT8 *Param);
//#NT#2010/02/10#Jarkko Chang -end
extern BOOL IsFocusInitFinish(void);
//extern double GetPreciseEV();
extern UINT16 caf_GetFocusDist(void);

//#NT#2010/02/22#Jarkko Chang -begin
static FPCALLBACK_GETSearchRange g_fpGetSearchRange = NULL;

void CAF_RegGetRTAF_SearchRangeCB(FPCALLBACK_GETSearchRange getSearchRange)
{
    g_fpGetSearchRange = getSearchRange;
}
//#NT#2010/02/22#Jarkko Chang -end

void RTAF_SearchRange(INT16 MinIndex, INT16 MaxIndex)
{
	RTAF_MinIndex = MinIndex;
	RTAF_MaxIndex = MaxIndex;
}

BOOL IsTrackingEn(void)
{
	return FocusTrack_Enable;
}

void SetTrackingFocusEnable(BOOL TrackingEn)
{
	FocusTrack_Enable = TrackingEn;
}

unsigned long GetTrackingStableCnt(void)
{
	return Tracking_StableCnt;
}

UINT32 caf_ChkHist(UINT32 Percent, UINT32 Step)
{
    UINT32 i, Avg, Center, BlockW, BlockH;


    BlockW = GetIPPSizeInfor(_ImeOut1Hsize) / 8 * 2;
    BlockH = GetIPPSizeInfor(_ImeOut1Vsize) / 8 * 2;
    Center = AvgY / 4;
    if (Center > Step)
    {
        Center -= Step;
    }
    else
    {
        Center = 0;
    }

    Avg = 0;
    for (i = 0; i < (Step * 2); i ++)
    {
        Avg += cafHist[Center];
        Center ++;
        if (Center >= 64)
        {
            break;
        }
    }
    Avg = (Avg * 100) / (BlockH * BlockW);
    //sprintf(message,"%d", Avg);
    //SenFP_ShowOSDString(message,20,50,1);
    if (Avg >= Percent)
    {
        return 1;
    }
    return 0;
}

UINT32 caf_calHist(void)
{
    UINT32 i, j, BlockW, BlockH, StartX, StartY, Avg;
    UINT8 *Addr;

    GetActImgFBAddr_Path1(&FB_para1);
    BlockW = GetIPPSizeInfor(_ImeOut1Hsize) / 8 * 2;
    BlockH = GetIPPSizeInfor(_ImeOut1Vsize) / 8 * 2;
    StartX = GetIPPSizeInfor(_ImeOut1Hsize) / 2 - (BlockW / 2);
    StartY = GetIPPSizeInfor(_ImeOut1Vsize) / 2 - (BlockH / 2);
    Addr = (UINT8 *)(FB_para1.y_addr + (StartY * GetIPPSizeInfor(_ImeOut1LineOffset)) + StartX);
    for (i = 0; i < 64; i++)
    {
        cafHist[i] = 0;
    }

    Avg = 0;
    for (j = 0; j < BlockH; j ++)
    {
        for (i = 0; i < BlockW; i ++)
        {
            Avg += Addr[i];
            cafHist[(Addr[i] / 4)] ++;
        }
        Addr += GetIPPSizeInfor(_ImeOut1LineOffset);
    }
    Avg = Avg / (BlockW * BlockH);

#if 0
    debug_err(("_____________________________________ %d\r\n", Avg));
    for (i = 0; i < 8; i ++)
    {
        debug_err(("%d %d %d %d %d %d %d %d\r\n",
                    cafHist[i*8+0], cafHist[i*8+1], cafHist[i*8+2], cafHist[i*8+3],
                    cafHist[i*8+4], cafHist[i*8+5], cafHist[i*8+6], cafHist[i*8+7]
                    ));
    }
#endif
    return Avg;
}

UINT32 caf_SceneChk(void)
{
    INT32 i;
    UINT32 HighMax, LowMax, tempValue;
    UINT32 StartIdx;

    StartIdx = AvgY / 4;
    LowMax = StartIdx;
    HighMax = StartIdx;

    tempValue = 0;
    for (i = StartIdx; i > 0; i --)
    {
        if (cafHist[i] > tempValue)
        {
            tempValue = cafHist[i];
            LowMax = i;
        }
    }


    tempValue = 0;
    for (i = StartIdx; i < 64; i ++)
    {
        if (cafHist[i] > tempValue)
        {
            tempValue = cafHist[i];
            HighMax = i;
        }
    }

    if (((HighMax >= 62) && (HighMax <= 63)) && (LowMax >= (StartIdx - 8)))
    {
        return 1;
    }
    return 0;
}

UINT32 DampCnt = 0;
UINT32 caf_Algorithm(void)
{
    UINT32 AF_Value, CafStatus;
    INT16 NextIdx;

    CafStatus = FALSE;
    if (0)//caf_GetFocusDist() > 50)
    {
        RT_AF_Step = 3 + (Lens_Zoom_GetSection() - 1)/2;
    }
    else
    {
        if (RT_AF_Direction == 1)
        {
            RT_AF_Step = 6 + (Lens_Zoom_GetSection() - 1)/2;
        }
        else
        {
            RT_AF_Step = 9 + (Lens_Zoom_GetSection() - 1)/2;
        }
    }
//    debug_err(("RT_AF_Status = %d,%d\r\n", RT_AF_Status,RT_AF_Step));

    switch(RT_AF_Status)
    {
        case 0://first set status and get current contrast
            Resum_TrackingFocus=IsTrackingEn();
            AF_Value=(aaa_GetVariation(27) + aaa_GetVariation(28) + aaa_GetVariation(35) + aaa_GetVariation(36));
            if (0)//caf_GetFocusDist() < 50)
            {
                if (AF_Value < (LastAF_Value * 10 / 100))
                {
                    Lens_Focus_Goto(RTAF_MinIndex);
                }
                else if (caf_SceneChk())
                {
                    Lens_Focus_Goto(RTAF_MinIndex);
                    sie_waitVD(3);
                    LastAF_Value = (aaa_GetVariation(27) + aaa_GetVariation(28) + aaa_GetVariation(35) + aaa_GetVariation(36));
                    RT_AF_Status = 6;
                }
            }

            if(!Inv_Direction)
            {
                if(Lens_Focus_GetCurrentPosition()<((RTAF_MinIndex+RTAF_MaxIndex)/2))
                {
                    RT_AF_Direction=1;
                }
                else
                {
                    RT_AF_Direction=0;
                }
            }
            else
            {
                Inv_Direction=FALSE;
            }
            if(RT_AF_Direction==0)
                NextIdx=Lens_Focus_GetCurrentPosition()-RT_AF_Step;
            else
                NextIdx=Lens_Focus_GetCurrentPosition()+RT_AF_Step;
            Lens_Focus_Goto(NextIdx);
            RT_AF_Status++;
//            debug_err(("RT_AF_Direction=%d\r\n",RT_AF_Direction));
            break;

        case 1:
            if(RT_AF_Direction==0)
                NextIdx=Lens_Focus_GetCurrentPosition()-RT_AF_Step;
            else
                NextIdx=Lens_Focus_GetCurrentPosition()+RT_AF_Step;

            Lens_Focus_Goto(NextIdx);
            VACnt = ipe_getVACnt();
            Index_Cnt ++;
            RT_AF_Status++;
//            debug_err(("RT_AF_Direction=%d\r\n",RT_AF_Direction));
            break;

        case 2:
            //debug_err(("\r\n%d\r\n",RT_AF_Status));
            if (VACnt == ipe_getVACnt())
            {
                return CafStatus;
            }
            AF_Value=(aaa_GetVariation(27) + aaa_GetVariation(28) + aaa_GetVariation(35) + aaa_GetVariation(36));
            LastAF_Value=AF_Value;

            if(RT_AF_Direction==0)
                NextIdx=Lens_Focus_GetCurrentPosition()-RT_AF_Step;
            else
                NextIdx=Lens_Focus_GetCurrentPosition()+RT_AF_Step;

            //debug_err(("\r\n%.8d %.8d %d %d %d %.2d\r\n", AF_Value, LastAF_Value, Lens_Focus_GetCurrentPosition(), NextIdx, RT_AF_Direction, RT_AF_Step));
            Lens_Focus_Goto(NextIdx);
            VACnt = ipe_getVACnt();
            RT_AF_Status++;
            Index_Cnt ++;
            break;

        case 3:
            //debug_err(("\r\n%d\r\n",RT_AF_Status));
            if (VACnt == ipe_getVACnt())
            {
                return CafStatus;
            }
            AF_Value=(aaa_GetVariation(27) + aaa_GetVariation(28) + aaa_GetVariation(35) + aaa_GetVariation(36));
            DampCnt ++;
            if (DampCnt > 3)
            {
                RTAF_BestIdx = Index_Cnt;
                RT_AF_Status = 5;
                DampCnt = 0;
            }
            else
            {
                if(AF_Value<LastAF_Value)
                {
                    if(RT_AF_Direction==0)
                        NextIdx=Lens_Focus_GetCurrentPosition()-RT_AF_Step;
                    else
                        NextIdx=Lens_Focus_GetCurrentPosition()+RT_AF_Step;
                    RT_AF_Direction=!RT_AF_Direction;
                    Inv_Direction = TRUE;
                    RT_AF_Status=0;
                    Index_Cnt=0;
                    //debug_err(("\r\n%.8d %.8d %d %d %d %.2d\r\n", AF_Value, LastAF_Value, Lens_Focus_GetCurrentPosition(), NextIdx, RT_AF_Direction, RT_AF_Step));
                    break;
                }
            }
            LastAF_Value=AF_Value;

            if(RT_AF_Direction==0)
                NextIdx=Lens_Focus_GetCurrentPosition()-RT_AF_Step;
            else
                NextIdx=Lens_Focus_GetCurrentPosition()+RT_AF_Step;

            //debug_err(("\r\n%d %.8d %.8d %d %d %d %.2d\r\n",RT_AF_Status, AF_Value, LastAF_Value, Lens_Focus_GetCurrentPosition(), NextIdx, RT_AF_Direction, RT_AF_Step));
            Lens_Focus_Goto(NextIdx);
            VACnt = ipe_getVACnt();
            DampCnt = 0;
            RT_AF_Status++;
            Index_Cnt ++;
            break;

        case 4:
            //debug_err(("\r\n%d\r\n",RT_AF_Status));
            if (VACnt == ipe_getVACnt())
            {
                return CafStatus;
            }
            AF_Value=(aaa_GetVariation(27) + aaa_GetVariation(28) + aaa_GetVariation(35) + aaa_GetVariation(36));
            if(AF_Value<LastAF_Value)
            {
                RTAF_BestIdx=Index_Cnt;
                RT_AF_Status++;
            }
            else if((Lens_Focus_GetCurrentPosition()>=RTAF_MaxIndex)||(Lens_Focus_GetCurrentPosition()<=RTAF_MinIndex))
            {
                RT_AF_Status++;
                RTAF_BestIdx=Index_Cnt;
            }
            else
            {
                if(RT_AF_Direction==0)
                    NextIdx=Lens_Focus_GetCurrentPosition()-RT_AF_Step;
                else
                    NextIdx=Lens_Focus_GetCurrentPosition()+RT_AF_Step;

                if(NextIdx>=RTAF_MaxIndex)
                    NextIdx=RTAF_MaxIndex;
                else if(NextIdx<=RTAF_MinIndex)
                    NextIdx=RTAF_MinIndex;

                //debug_err(("\r\n%.8d %.8d %d %d %d %.2d\r\n", AF_Value, LastAF_Value, Lens_Focus_GetCurrentPosition(), NextIdx, RT_AF_Direction, RT_AF_Step));
                RTAF_BestIdx=Index_Cnt;
                LastAF_Value=AF_Value;
                Lens_Focus_Goto(NextIdx);
                VACnt = ipe_getVACnt();
                Index_Cnt ++;
            }
            break;

        case 5:
            RT_AF_Status ++;
            Lens_Focus_Goto(Recode_FocusStep[RTAF_BestIdx-1]);
            VACnt = ipe_getVACnt();
            break;

        case 6:
            if (VACnt == ipe_getVACnt())
            {
                return CafStatus;
            }

            AF_Value=(aaa_GetVariation(27) + aaa_GetVariation(28) + aaa_GetVariation(35) + aaa_GetVariation(36));
//#NT#2009/02/21#Bowen Li -begin
            if ((AF_Value > LastAF_Value * 95 / 100) || (AF_Value < LastAF_Value * 105 / 100))
//#NT#2009/02/21#Bowen Li -end
            {
                VACnt = ipe_getVACnt();
                CafStatus = 1;
            }
            else
            {
                RT_AF_Status=0;
                Index_Cnt = 0;
            }
            break;
    }
    Recode_FocusStep[Index_Cnt] = Lens_Focus_GetCurrentPosition();
    return CafStatus;
}

void caf_setInitFlag(BOOL flag)
{
    InitialFlag = flag;
}

BOOL caf_GetVAFlag(void)
{
    return VAEnableFlag;
}

//#NT#2010/02/10#Jarkko Chang -begin
//#NT#modify for caf
void caf_VAPprepare(BOOL flag)
{
    UINT8  UserParam[20];
    AF_IPEINFO gAFIPEInfo;

    if (VAEnableFlag == flag)
    {
        return;
    }

    if (flag == TRUE)
    {
/*
        UserParam[0]= 7;// Total Parameter number
        UserParam[1]= VDET_EVAR;// vacc_src
        UserParam[2]= 0; // vacc_sqr
        UserParam[3]= 0; // vacc_gain
        UserParam[4]= WINDOWS_8X8;//vacc_num
        UserParam[5]= 16;//va_th
        UserParam[6]= WIN8x8_HV_VALUE;//VAOutputSel
        debug_msg("----->before User_VABegin in caf_VAPprepare\r\n");
        User_VABegin(UserParam);
*/
        gAFIPEInfo.vacc_src = VDET_EVAR;
        gAFIPEInfo.vacc_sqr = 0;
        gAFIPEInfo.vacc_gain = 0;
        gAFIPEInfo.vacc_num = WINDOWS_8X8;
        gAFIPEInfo.va_th = 16;
        gAFIPEInfo.vacc_outsel = WIN8x8_HV_VALUE;
        User_VABegin(&gAFIPEInfo);

    }
    else
    {
//        User_VAEnd(UserParam);
        User_VAEnd(&gAFIPEInfo);
    }
    VAEnableFlag = flag;
}
//#NT#2010/02/10#Jarkko Chang -end

void caf_Lock(BOOL flag)
{
    RtAFLockFlag = flag;
}

void caf_initial(void)
{
    INT16 StartIdx, EndIdx, DefaultIdx;
    INT8 Step;
    RT_AF_Status = 0;
    Index_Cnt = 0;
    if (VAEnableFlag == TRUE)
    {
        caf_VAPprepare(FALSE);
    }
    caf_VAPprepare(TRUE);
    /*
    AF_GetCurrentRange(&StartIdx, &EndIdx, &Step, &DefaultIdx);
    StartIdx += Focus_TempComp_Far[Lens_Zoom_GetSection() - 1];
    RTAF_SearchRange(StartIdx, EndIdx);
    */
//#NT#2010/02/22#Jarkko Chang -begin
#if 1
    if(g_fpGetSearchRange == NULL)
    {
        debug_err(("Get SearchRange FP is null!!\r\n"));
        StartIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 2);
        EndIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);
        RTAF_SearchRange(StartIdx,EndIdx);
    }
    else
    {
        g_fpGetSearchRange();
    }
#else
    if(CalLens_IsAdjust())
    {
        RTAF_SearchRange(CalLens_GetNearFocusStep(Lens_Zoom_GetSection()), CalLens_GetFarFocusStep(Lens_Zoom_GetSection()));
    }
    else
    {
        StartIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 2);
        EndIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);
        RTAF_SearchRange(StartIdx,EndIdx);
    }
#endif
//#NT#2010/02/22#Jarkko Chang -end
}

BOOL caf_ChkVAValue(UINT32 Threshold)
{
    //debug_err(("LastAF_Value = %d\r\n", LastAF_Value));
    if ((LastAF_Value) < Threshold)
    {
        return 1;
    }
    return 0;
}

void caf_AutoFocusProcess(void)
{
    static double PrevEV = 99999;
    UINT32 CurEV;

    if (RtAFLockFlag == FALSE)
    {
//        CurEV = GetPreciseEV();
        CurEV = AE__GetEV();
        if ((CurEV > (PrevEV/1.1)) && (CurEV < (PrevEV*1.1)))
        {
        }
        else
        {
            PrevEV = CurEV;
            InitialFlag = TRUE;
            AvgY = caf_calHist();
        }
        if(Lens_Module_GetState( ) == LENS_IDLE)
//        if (IsFocusInitFinish())
        {
            if (InitialFlag == TRUE)
            {
                InitialFlag = FALSE;
                caf_initial();
            }
            else
            {
                //#NT#2009/03/01#JJ Huang -begin
//                flash_SuspendCharge();
//                FocusPowerOnOff(ON);
                Lens_OnOff(ON);
                caf_Algorithm();
//                FocusPowerOnOff(OFF);
                Lens_OnOff(OFF);
//                flash_ResumeCharge();
                //#NT#2009/03/01#JJ Huang -end
            }
        }
        else
        {
            InitialFlag = TRUE;
        }
    }
}

UINT16 caf_GetFocusDist(void)
{
#if 0
    UINT32 i;
    UINT32 uiAval, uiBval, uiAintvl, uiBintvl;

    for (i = 0; i < 12; i ++)
    {
        if (Lens_Focus_GetCurrentPosition() < Focus_Zoom_Tbl[Lens_Zoom_GetSection()-1][i])
            break;
    }
    if (i == 0)
    {
        return Focus_Zoom_Tbl_Dist[0];
    }
    else if (i == 12)
    {
        return Focus_Zoom_Tbl_Dist[11];
    }
    uiAval = Focus_Zoom_Tbl_Dist[i-1];
    uiBval = Focus_Zoom_Tbl_Dist[i];
    uiAintvl = Lens_Focus_GetCurrentPosition() - Focus_Zoom_Tbl[Lens_Zoom_GetSection()-1][i-1];
    uiBintvl = Focus_Zoom_Tbl[Lens_Zoom_GetSection()-1][i] - Lens_Focus_GetCurrentPosition();
    return (UINT16)((uiAval*uiBintvl + uiBval*uiAintvl)/(uiAintvl+uiBintvl));
#endif
}


