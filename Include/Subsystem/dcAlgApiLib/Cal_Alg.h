#ifndef _CAL_ALG_H
#define _CAL_ALG_H

#define DP_Prv_MAX 1000//500//200
#define DP_Cap_MAX 8240//((2048+16)*3+2048)
#define DP_Dzoom_MAX 200


typedef struct _DP_Struct{
    UINT Prv_White;
    UINT Prv_Black;
    UINT Cap_White;
    UINT Cap_Black;
    UINT Dzoom_White;
    UINT Dzoom_Black;
}DP_Struct;

typedef struct _AWB_Struct{
    UINT Rgain;
    UINT Bgain;
}AWB_Struct;

typedef struct _Adjust_Struct{
#if 1// 1: New Content, 0: Old Content
    UINT Iris_Test_Status;
    UINT MShtr_Test_Status;
    UINT FAE_Test_Status;
    UINT Zoom_SpeedTest_Status;
    UINT Zoom_StepMissTest_Status;
    UINT Focus_StepMissTest_Status;
    UINT NFocus_Status;
    UINT FFocus_Status;
    UINT Far_Focus[15];
    UINT Near_Focus[15];
    UINT Focus_Test_Status;
    UINT ISO_Status;
    UINT Agc_ISO_100;
    UINT Mshutter_Status[8];
    UINT FTP_Status;
    UINT AWB_Status;
    AWB_Struct AWB;
    AWB_Struct AWB_Prv;
    DP_Struct DP_Status;
    UINT Shading_Status;
    UINT ShadingXcenter;
    UINT ShadingYcenter;
    UINT ShadingDecayRatio;
    UINT FlashAE_Status;
    UINT Flash_Y_Avg;
    UINT FlashAWB_Status;
    AWB_Struct AWB_Flash;
    UINT OB_Status;
    UINT Agc_ISOMin;
    UINT Mshutter[8];
    UINT DP_Cap_Num[8];
    UW Defect_Pixel[DP_Prv_MAX+DP_Cap_MAX];
    UW DP_Dzoom[DP_Dzoom_MAX];
    UINT FlashTriggerPoint;
    UINT OB_Min;
    UINT Shading_Gain[5];
    UINT Cap_Field_Num;
    UINT Cap_Field_DP_Cnt[8];
    USHORT DP_Cap_Dark_Histo[255];
    USHORT Flash_ADC_Val[2][8];
    UINT Flash_Max_Width_Y;
//    UINT8 SerialNumber[CAL_SERIAL_LEN];
    UINT RTC_Test_Status;
    UINT EmptyVolt_Test_Status;
    UINT EmptyVolt;
    UINT EmptyVolt_Delta; //ricky@1211/2007
    UINT uiLanguageIndex;
    UINT uiTVModeIndex;
    UINT Burnin_Status;
    UINT Zoom_Stop2PR_Status;//JJ0228
    UINT uiZoomStop2PR;//JJ0228
    INT16 iFocusOffset[2];//JJ0324
    UINT uiFocusOffsetStatus;//JJ0324
    UINT Language_Status; //ricky@0401/2008
#else
    UINT Iris_Test_Status;
    UINT MShtr_Test_Status;
    UINT FAE_Test_Status;
    UINT ISO_Status;
    UINT Mshutter_Status[8];
    UINT FTP_Status;
    UINT AWB_Status;
    DP_Struct DP_Status;
    UINT Flash_Status;
    UINT OB_Status;
    UINT FFocus_Status;
    UINT NFocus_Status;
    UINT Agc_ISOMin;
    UINT Agc_ISO_100;
    UINT Mshutter[8];
    UINT DP_Cap_Num[8];
    UW Defect_Pixel[DP_Prv_MAX+DP_Cap_MAX];
    UW DP_Dzoom[DP_Dzoom_MAX];
    AWB_Struct AWB;
    AWB_Struct AWB_Prv;
    UINT FlashTriggerPoint;
    UINT Flash_Y_Avg;
    UINT OB_Min;
    UINT Far_Focus[15];
    UINT Near_Focus[15];
    UINT Shading_Status;
    UINT Shading_Gain[5];
    UINT Cap_Field_Num;
    UINT Cap_Field_DP_Cnt[8];
    USHORT DP_Cap_Dark_Histo[255];
    USHORT Flash_ADC_Val[2][8];
    UINT Flash_Max_Width_Y;
    UINT ShadingXcenter;
    UINT ShadingYcenter;
 #endif
 }Adjust_Struct;


ER WriteCalData(void);
ER ReadCalData(void);

#endif