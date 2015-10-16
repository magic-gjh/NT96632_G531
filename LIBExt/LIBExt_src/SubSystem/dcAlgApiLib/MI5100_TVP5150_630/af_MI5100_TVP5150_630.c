#include "sensor_model.h"
#if MI5100_TVP5150_630
#include "utility.h"

AF_IPEINFO gAFIPEInfo;
UINT32 VaccValueArray[5];

void Make_AF_Table(void)
{
}


UINT8 AF_Process(void)
{
    UINT16    AF_Step;
    UINT16    ScanIdx,MaxIdx=0;
    UINT16    StartIdx,EndIdx,NextIdx;
    UINT16    MaxValue=0, Value, Value2;
    UINT32  ContrastCnt;
    UINT8   Select_Win=4;
    UINT8   Status;
    //UINT    uiFlag;


    Select_Win=31;

    StartIdx=284;
    EndIdx=485;
    MaxIdx=StartIdx;
    Lens_Focus_Goto(StartIdx);

    gAFIPEInfo.vacc_src = VDET_EVAR;
    gAFIPEInfo.vacc_sqr = 0;
    gAFIPEInfo.vacc_gain = 0;
    gAFIPEInfo.vacc_num = WINDOWS_8X8;
    gAFIPEInfo.va_th = 16;
    gAFIPEInfo.vacc_outsel = WIN8x8_HV_VALUE;
    User_VABegin(&gAFIPEInfo);
        
    AF_Step= 1;
    Lens_Focus_Goto(StartIdx+AF_Step);
    ContrastCnt = GetIpeVACnt();
    Status = AFStatus_OK;
    for(ScanIdx = StartIdx; ScanIdx <= (EndIdx+AF_Step-1); ScanIdx+=AF_Step)
    {
        if(0)//(IS_SensorPreviewVGA())
        {
            NextIdx = ScanIdx + 3*AF_Step;
        }
        else
        {
            NextIdx = ScanIdx + 2*AF_Step;
        }
        sie_waitVD(1);
        Lens_Focus_Goto(NextIdx);
        //wai_flg((UINT*)&uiFlag, FLG_ID_PHOTO, FLGPHOTO_AF_VA, TWF_CLR|TWF_ORW);
        //clr_flg(FLG_ID_PHOTO, FLGPHOTO_AF_VA);
        ContrastCnt = GetIpeVACnt();
        Value=(aaa_GetVariation(30)+aaa_GetVariation(31)+aaa_GetVariation(32)+aaa_GetVariation(33))/4;
        pre_GetVaccValue(VaccValueArray);
        Value2 = (VaccValueArray[0]+VaccValueArray[1]+VaccValueArray[2]+VaccValueArray[3]+VaccValueArray[4])/5;
        debug_err(("AF step %d PRE Value = %d\r\n", ScanIdx, Value2));
        debug_err(("AF step %d IPE value = %d\r\n", ScanIdx, Value));

        if(Value>MaxValue)
        {
            MaxIdx=ScanIdx;
            MaxValue=Value;
        }

        if (((ScanIdx - MaxIdx) >= 2) && (MaxValue > ((Value*11)/10)))
        {
            Status = AFStatus_OK;
            break;
        }

    }
    if ((MaxIdx >= EndIdx) || (MaxValue < 350))
    {
        Status = AFStatus_FAIL;
    }
    Lens_Focus_Goto(MaxIdx);

    User_VAEnd(&gAFIPEInfo);

    return Status;
}

void AF_Release(void)
{

}
UINT16 AF_FullRangeSearch(UINT16 StartIdx, UINT16 EndIdx, UINT8 IntervalStep)
{
    UINT16    AF_Step=1;
    UINT16    ScanIdx, MaxIdx=0;
    UINT16    NextIdx;
    UINT16    MaxValue=0, Value;
    UINT32    ContrastCnt;

    if(IntervalStep != 0)
    {
        AF_Step=IntervalStep;
    }
    else
    {
        AF_Step=1;
    }
    aaa_InitAF(GetIPPSizeInfor(_IpeOutHsize), GetIPPSizeInfor(_IpeOutVsize));
    Lens_Focus_Goto(StartIdx-AF_Step);
    sie_waitVD(1);
    Lens_Focus_Goto(StartIdx);
    sie_waitVD(1);
    ContrastCnt = GetIpeVACnt();
    Lens_Focus_Goto(StartIdx+AF_Step);
    if(0)//(IS_SensorPreviewVGA())
    {
        sie_waitVD(1);
        ContrastCnt = GetIpeVACnt();
        Lens_Focus_Goto(StartIdx+AF_Step*2);
    }
    for(ScanIdx = StartIdx; ScanIdx <= (EndIdx+AF_Step-1); ScanIdx += AF_Step)
    {
        if(0)//(IS_SensorPreviewVGA())
        {
            NextIdx = ScanIdx + 3*AF_Step;
        }
        else
        {
            NextIdx = ScanIdx + 2*AF_Step;
        }
        sie_waitVD(1);
        ContrastCnt = GetIpeVACnt();
        Lens_Focus_Goto(NextIdx);
        while(ContrastCnt == GetIpeVACnt())
        {
            TimerDelayMs(1);
        }
        Value = aaa_GetVariation(4);
        if(Value > MaxValue)
        {
            MaxIdx = ScanIdx;
            MaxValue = Value;
        }
    }
    Lens_Focus_Goto(MaxIdx);
    return MaxIdx;
}



#endif
