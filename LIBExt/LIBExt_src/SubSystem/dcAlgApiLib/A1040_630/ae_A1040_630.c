#include "sensor_model.h"
//#NT#20090706#ETHAN LAU -begin
#include <stdarg.h>
//#NT#20090706#ETHAN LAU -end
#if A1040_630
#include "sif.h"
#include "uart.h"
#include "pll.h"
#include "ErrorNo.h"
#include "cal_api.h"
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
#include "sen_snr.h"
//#NT#2010/12/15#JJ Huang -end

extern UINT32 uiAE_TEST;
extern UINT32 uiAeIsoGain;
extern UINT32 uiAeExpoTime;

void aaa_GetCenterHist(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset, UINT32* Histogram);
UINT32 CenterHighCnt, CenterHist[64];

extern UINT gpuiFbAdd[3];
//#NT20091123#ethanlau -begin
UINT32 MINISO=50;
//#NT#20091123#ethanlau -end

//#NT#20090907#ethanlau -begin
UINT32 SENSOR_PRE_PCLK = 48000000;
UINT32 SENSOR_CAP_PCLK = 48000000;
UINT32 SENSOR_PRE_PCLK_M = 48;//(SENSOR_PRE_PCLK/1000000);
UINT32 SENSOR_CAP_PCLK_M = 48;//(SENSOR_CAP_PCLK/1000000);
//BOOL AEDUMMYLINE =TRUE;
_SEN_MODE Prv_SenMode;
//UINT32 A1=66000000,A2=66000000,A3=69000000;
void AE_PclkParaSet(_SEN_MODE mode);
//BOOL GetDelayFlag(void);
//void SetDelayFlag(BOOL flag);
//void SetExpotimeFlag(BOOL flag);
//BOOL GetExpotimeFlag(void);

void SetSmartSceneRst(_IMGSceneMode_sel scenerst);
UINT32 AeExpoTime2TGValue(UINT32 Time, UINT32 VDTotal, UINT32 HDTotal,UINT32 Pclk);
//#NT#20090907#ethanlau -end

UINT EV=0,Old_EV=0;
UINT32 YAverage;
UINT32 gFlagCap=0,gAEcounter=0,gAELock=0;
UINT32 Hist[64];
UINT32 tmp=0;
UINT32 AE_Stable_Cnt=0;
UINT32 AE_Idx=0,g_Idx_f=0,g_Idx_n=0;

UINT32 LogTbl[10] = {107,115,123,132,141,152,162,174,187,200};
UINT32 nMin_EV_Value = EV_Value_MIN;
UINT32 Last_EV=10,Damping_EV=10;
UINT32 nAE_Maxline;
INT32 LV=0;

UINT16 AE_Flag=0;
UINT16 Add_Y,Add_Step = Default_AddStep;
UINT16  Last_OverExpo_Max_AddY = OverExpo_Max_AddY_Indoor;
UINT16 EVCompTable[13] = {25,33,42,50,67,83,100,118,136,154,166,178,190};
INT16 NormalY_adj=0;
//UINT16 Ae_IrisZoomStep0[2] = {80, 320};

UINT8 Last_AEStatus,Damping;
UINT8 AE_Counter= 0;
UINT8 CurrentIdx;


UINT32 OverExpoMaxAddy[3];
UINT32 OverExpoTh[3];

BOOL gGR_ERS4LongExp=FALSE;

AE_STRUCT AE_Param;
AE_Debug_msg AE_Debug;
UINT32 Expo_CompensateRatio = 100;
extern volatile BOOL GRS_MODE;
//extern UINT32 ShutterCloseLeadTime;
//#NT#2010/09/24#ethanlau -begin
typedef struct _Coordinate
{
    UINT32 Startx;
    UINT32 Starty;
    UINT32 Endx;
    UINT32 Endy;
}Coordinate,*pCoordinate;
//#NT#2010/09/24#ethanlau -end

const UINT8 AEWTable[8][8]={
        {0,0,0, 0, 0,0,0,0},
        {0,1,1, 1, 1,1,1,0},
        {0,1,3, 3, 3,3,1,0},
        {0,1,3, 5, 5,3,1,0},
        {0,1,3, 5, 5,3,1,0},
        {0,1,3, 3, 3,3,1,0},
        {0,1,1, 1, 1,1,1,0},
        {0,0,0, 0, 0,0,0,0},
        };
const UINT8 AE_Window_CenterWeighted[8][8]={
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
        {1, 2, 4, 4, 4, 4, 2, 1},
        {1, 2, 4, 4, 4, 4, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
        {1, 2, 3, 3, 3, 3, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        };

UINT8 AE_Window_Adaptive[8][8]={
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1},
        {1, 1,  1,  1,  1,  1, 1, 1}
};

const UINT8 AE_Window_Spot[8][8]={
        {0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,0},
        {0,0,3,3,3,3,0,0},
        {0,0,3,6,6,3,0,0},
        {0,0,3,6,6,3,0,0},
        {0,0,3,5,5,3,0,0},
        {0,0,2,3,3,2,0,0},
        {0,0,0,1,1,0,0,0}
        };
UINT32 AeCurve_Landscape[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{5120,     33333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{1280,     33333,       200,    0,          AE_ADJ_GAIN},//
{640,      66666,       200,    0,          AE_ADJ_GAIN},//
{320,     133333,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_NightLandscape[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{5120,     33333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{2560,     66666,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{1280,    133333,        50,    0,          AE_ADJ_EXP},//
{640,     266666,        50,    0,          AE_ADJ_EXP},//
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     533333,       100,    0,          AE_ADJ_GAIN},// 1/7.5*2
{ 80,     533333*2,     100,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     533333*4,     100,    0,          AE_ADJ_GAIN},// 1/7.5*2
};


UINT32 AeCurve_Action[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{40960,     4167,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{20480,     4167,       100,    0,          AE_ADJ_EXP}, // 1/60*2
{2560,     16666,       200,    0,          AE_ADJ_EXP},// 1/30*2
{1280,     33333,       200,    0,          AE_ADJ_EXP},// 1/15*2
{640,      66666,       200,    0,          AE_ADJ_EXP},//
{320,     133333,       200,    0,          AE_ADJ_EXP},// 1/15*2
{160,     266666,       200,    0,          AE_ADJ_EXP},// 1/7.5*2
{  0,     133333,       800,    0,          AE_ADJ_EXP},// 1/7.5*2
};

UINT32 AeCurve_Potrait[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30*2
{1280,     33333,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{640,      66666,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{320,     133333,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_NightPotrait[12][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN},// 1/60*2
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30*2
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     533333,       100,    0,          AE_ADJ_GAIN},// 1/7.5*2
{ 80,     533333*2,     100,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     533333*2,     100,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_NightPotrait60[12][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30*2
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     533333,       100,    0,          AE_ADJ_GAIN},// 1/7.5*2
{ 80,     533333*2,     100,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     533333*4,     100,    0,          AE_ADJ_GAIN},// 1/7.5*2
};


UINT32 AeCurve_NightPotrait50[12][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     20000,        50,    0,          AE_ADJ_GAIN},// 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,    0,          AE_ADJ_GAIN},// 1/25
{533,     160000,       100,    0,          AE_ADJ_GAIN},// 1/25
{266,     320000,       100,    0,          AE_ADJ_GAIN}, // 1/12.5
{133,     640000,       100,    0,          AE_ADJ_GAIN}, // 1/6.25
{67,    640000*2,       100,    0,          AE_ADJ_GAIN}, // 1/6.25
{0,     640000*4,       100,    0,          AE_ADJ_EXP}, // 1/6.25
};

UINT32 AeCurve_NormalCapture60[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/120
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/60
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/30
{640,      66666,       200,    0,          AE_ADJ_GAIN},// 1/30
{320,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
{160,     133333,       400,    0,          AE_ADJ_GAIN}, // 1/7.5
{0,       266666,       400,    0,          AE_ADJ_EXP}, // 1/7.5
};

UINT32 AeCurve_NormalCapture50[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     20000,       100,    0,          AE_ADJ_GAIN},// 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,    0,          AE_ADJ_GAIN},// 1/25
{533,      80000,       200,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN}, // 1/12.5
{133,     160000,       400,    0,          AE_ADJ_GAIN}, // 1/6.25
{0,       320000,       400,    0,          AE_ADJ_EXP}, // 1/6.25
};

UINT AeCurve_AntiFlicker60[10][5]={
// EV       ExpoTime    ISO     IrisStep
#if 0
{3000000,      0,           50,    0,          AE_ADJ_EXP},
{20480,     4166,        100,    0,          AE_ADJ_EXP},  // 1/120
{10240,     4166,        200,    0,          AE_ADJ_GAIN}, // 1/120
{5120,       8333,        200,    0,          AE_ADJ_GAIN}, // 1/120
{2560,     16666,        200,    0,          AE_ADJ_GAIN}, // 1/60
{1280,     33333,        200,    0,          AE_ADJ_GAIN}, // 1/30
{640,       33333,        400,    0,          AE_ADJ_GAIN}, // 1/30
{320,       33333,        800,    0,          AE_ADJ_GAIN}, // 1/30
{160,       33333,      1600,    0,          AE_ADJ_GAIN}, // 1/30
{0,           66666,      1600,    0,           AE_ADJ_EXP},  // 1/30
#endif
{3000000,      0,           50,    0,          AE_ADJ_EXP},
{20480,     4166,        100,    0,          AE_ADJ_EXP},  // 1/120
{10240,     4166,        200,    0,          AE_ADJ_GAIN}, // 1/120
{5120,       8333,        200,    0,          AE_ADJ_GAIN}, // 1/120
{2560,     16666,        200,    0,          AE_ADJ_GAIN}, // 1/60
{1280,     33333,        200,    0,          AE_ADJ_GAIN}, // 1/30
{640,       33333,        400,    0,          AE_ADJ_GAIN}, // 1/30
{320,       33333,        800,    0,          AE_ADJ_GAIN}, // 1/30
{160,       33333,      1600,    0,          AE_ADJ_GAIN}, // 1/30
{0,         33333,      3200,    0,           AE_ADJ_GAIN},  // 1/30
};

UINT AeCurve_AntiFlicker50[10][5]={
// EV       ExpoTime    ISO     IrisStep
#if 0
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,     5000,      100,    0,          AE_ADJ_EXP}, // 1/100
{8533,       5000,       200,    0,          AE_ADJ_GAIN},// 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/50
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      40000,       800,    0,          AE_ADJ_GAIN},// 1/25
{133,      40000,      1600,    0,          AE_ADJ_GAIN},// 1/25
{0,        80000,      1600,    0,          AE_ADJ_EXP}, // 1/25
#endif
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,     5000,      100,    0,          AE_ADJ_EXP}, // 1/100
{8533,       5000,       200,    0,          AE_ADJ_GAIN},// 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/50
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      40000,       800,    0,          AE_ADJ_GAIN},// 1/25
{133,      40000,      1600,    0,          AE_ADJ_GAIN},// 1/25
{0,        40000,      3200,    0,          AE_ADJ_GAIN}, // 1/25
};

UINT AeCurve_IROPEN_60[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/120
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/120
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/60
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/30
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/30
{320,     266666,       100,    0,          AE_ADJ_GAIN}, // 1/15
{160,     133333,       400,    0,          AE_ADJ_GAIN}, // 1/7.5
{0,       133333,       800,    0,          AE_ADJ_EXP}, // 1/7.5
};

UINT AeCurve_IROPEN_50[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN},// 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,    0,          AE_ADJ_GAIN},// 1/25
{533,     160000,       100,    0,          AE_ADJ_GAIN},// 1/25
{266,     320000,       100,    0,          AE_ADJ_GAIN}, // 1/12.5
{133,     160000,       400,    0,          AE_ADJ_GAIN}, // 1/6.25
{0,       160000,       800,    0,          AE_ADJ_EXP}, // 1/6.25
};


//#NT#2010/08/11#ethanlau -begin
//#NT#
UINT AeCurve_ISO50_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP},// 1/120
{10240,     8333,       100,    0,          AE_ADJ_GAIN},// 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/15
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{160,   2*266666,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{0,     2*266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO50_50Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP},// 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN},// 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,    0,          AE_ADJ_GAIN},// 1/12.5
{533,     160000,       100,    0,          AE_ADJ_GAIN},// 1/6.25
{266,     320000,       100,    0,          AE_ADJ_GAIN},// 1/3.125
{133,     640000,       100,    0,          AE_ADJ_GAIN},// 1/1.5625
{0,      1280000,       100,    0,          AE_ADJ_GAIN},// 1/1.5625
};


UINT AeCurve_ISO100_60Hz[12][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{327680,     520,        50,    0,          AE_ADJ_EXP},
{163840,     520,       100,    0,          AE_ADJ_GAIN},
{20480,     4166,       100,    0,          AE_ADJ_EXP},// 1/120
{10240,     8333,       100,    0,          AE_ADJ_GAIN},// 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/15
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{160,   2*266666,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{0,     4*266666,       100,    0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO100_50Hz[12][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{273056,     625,        50,    0,          AE_ADJ_EXP},
{136528,     625,       100,    0,          AE_ADJ_GAIN},
{17033,     5000,       100,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,    0,          AE_ADJ_GAIN},// 1/12.5
{533,     160000,       100,    0,          AE_ADJ_GAIN},// 1/6.25
{266,     320000,       100,    0,          AE_ADJ_GAIN},// 1/6.25
{133,     640000,       100,    0,          AE_ADJ_GAIN},// 1/6.25
{0,      1280000,       100,    0,          AE_ADJ_GAIN},// 1/6.25
};

UINT AeCurve_ISO200_60Hz[11][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{327680,     520,        50,    0,          AE_ADJ_EXP},
{163840,     520,       100,    0,          AE_ADJ_GAIN},
{10240,     8333,       100,    0,          AE_ADJ_EXP},// 1/120
{5120,      8333,       200,    0,          AE_ADJ_GAIN},// 1/120
{2560,     16666,       200,    0,          AE_ADJ_GAIN},// 1/60
{1280,     33333,       200,    0,          AE_ADJ_GAIN},// 1/30
{640,      66666,       200,    0,          AE_ADJ_GAIN},// 1/15
{320,     133333,       200,    0,          AE_ADJ_GAIN},// 1/7.5
{160,     266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5
{0,     2*266666,       200,    0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO200_50Hz[11][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{273024,     625,        50,    0,          AE_ADJ_EXP},
{136512,     625,       100,    0,          AE_ADJ_GAIN},
{8532,     10000,       100,    0,          AE_ADJ_EXP},// 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      80000,       200,    0,          AE_ADJ_GAIN},// 1/12.5
{266,     160000,       200,    0,          AE_ADJ_GAIN},// 1/6.25
{133,     320000,       200,    0,          AE_ADJ_GAIN},// 1/6.25
{0,       640000,       200,    0,          AE_ADJ_GAIN},// 1/6.25
};

UINT AeCurve_ISO400_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{327680,     520,        50,    0,          AE_ADJ_EXP},
{81920,      520,       200,    0,          AE_ADJ_GAIN},
{5120,      8333,       200,    0,          AE_ADJ_EXP},// 1/120
{2560,      8333,       400,    0,          AE_ADJ_GAIN},// 1/120
{1280,     16666,       400,    0,          AE_ADJ_GAIN},// 1/60
{640,      33333,       400,    0,          AE_ADJ_GAIN},// 1/30
{320,      66666,       400,    0,          AE_ADJ_GAIN},// 1/15
{160,     133333,       400,    0,          AE_ADJ_GAIN},// 1/7.5
{0,       133333,       400,    0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO400_50Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{273024,     625,        50,    0,          AE_ADJ_EXP},
{136512,     625,       100,    0,          AE_ADJ_GAIN},
{8532,     10000,       100,    0,          AE_ADJ_EXP},
{2133,     10000,       400,    0,          AE_ADJ_GAIN},// 1/100
{1066,     20000,       400,    0,          AE_ADJ_GAIN},// 1/50
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{133,     160000,       400,    0,          AE_ADJ_GAIN},// 1/6.25
{0,       160000,       400,    0,          AE_ADJ_GAIN},// 1/6.25
};

UINT AeCurve_AntiShaking60[10][5]={
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/120
{5120,      8333,       200,    0,          AE_ADJ_GAIN},// 1/120
{2560,     16666,       200,    0,          AE_ADJ_GAIN},// 1/60
{1280,     16666,       400,    0,          AE_ADJ_GAIN},// 1/30
{640,      33333,       400,    0,          AE_ADJ_GAIN},// 1/30
{320,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
{160,     133333,       400,    0,          AE_ADJ_GAIN}, // 1/7.5
{0,       266666,       400,    0,          AE_ADJ_EXP}, // 1/7.5
};

UINT AeCurve_AntiShaking50[10][5]={
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/50
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/25
{1066,     20000,       400,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN}, // 1/12.5
{133,     160000,       400,    0,          AE_ADJ_GAIN}, // 1/6.25
{0,       320000,       400,    0,          AE_ADJ_EXP}, // 1/6.25
};

UINT32 (*AeCurve)[5] = (UINT32(*)[5])AeCurve_AntiFlicker60;


UINT diffp, diffn, np, nn, contrast,CenAvg;
UINT HighCnt;
BOOL f_HalfFrmExpDet=0; //Half frame exposure detection only

//#NT20091026#ethanlau -begin
UINT32 GetAeMaxExptline(void)
{
//    return Sensor_ReadReg(0xC83A);
     return Sensor_ReadReg(0xC812);
}
//#NT#20091026#ethanlau -end

void AEMode_AE(UINT8 nMode)
{
    static UINT8 nPreModeSetting = 0xFF;
    if(nMode == nPreModeSetting)
    return;

    //Assign AE table
    switch(nMode)
    {
        case _IMGAEMode_CenterWeighted:
            aaa_SetAeWindow((UB*)AE_Window_CenterWeighted);
            //aaa_SetAeWindow((UB*)AE_Window_Adaptive);
            break;

        case _IMGAEMode_Spot:
            aaa_SetAeWindow((UB*)AE_Window_Spot);
            break;

        case _IMGAEMode_Matrix:
            aaa_SetAeWindow((UB*)AEWTable);
            break;

        case _IMGAEMode_Evaluative:
            aaa_SetAeWindow((UB*)AEWTable);
            break;
        case _IMGAEMode_FaceDetection:
            aaa_SetAeWindow((UB*)AE_Window_Adaptive);
            break;
        default:
            aaa_SetAeWindow((UB*)AEWTable);
            break;
    }

    nPreModeSetting = nMode;
    ResetAE();
}

#if 1
void ISO_AE(UINT8 ISO_Speed)
{
/*
    if(ISO_Speed == _IMGISO_Auto)
    {
        return;
    }
    switch(ISO_Speed)
    {
        case _IMGISO_100:
            AeCurve = (UINT32(*)[5])AeCurve_ISO100;
            break;

        case _IMGISO_200:
            AeCurve = (UINT32(*)[5])AeCurve_ISO200;
            break;

        case _IMGISO_400:
            AeCurve = (UINT32(*)[5])AeCurve_ISO400;
            break;

        case _IMGISO_800:
            AeCurve = (UINT32(*)[5])AeCurve_ISO800;
            break;

        default:
            AeCurve = (UINT32(*)[5])AeCurve_ISO100;
            break;
    }
    ResetAE();
*/
}
#endif

void AntiFlicker_AE(UINT8 nFlicker)
{
    static UINT8 nPreFlickerSetting = 0xFF;

    if(nFlicker == nPreFlickerSetting)
        return;

    switch(nFlicker)
    {
        case _POWERFreq_50Hz:
            AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
            break;
        case _POWERFreq_60Hz:
            AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
            break;
        default:
            AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
            break;
    }

    nPreFlickerSetting = nFlicker;
    ResetAE();
}



void AntiFlickerISO_AE(UINT8 nISO, UINT8 nFlicker)
{
    if(nFlicker==_POWERFreq_50Hz)
    {
        switch(nISO)
        {
            case _IMGISO_50:
                AeCurve = (UINT32(*)[5])AeCurve_ISO50_50Hz;
                break;

            case _IMGISO_100:
                AeCurve = (UINT32(*)[5])AeCurve_ISO100_50Hz;
                break;

            case _IMGISO_200:
                AeCurve = (UINT32(*)[5])AeCurve_ISO200_50Hz;
                break;

            case _IMGISO_400:
            case _IMGISO_800:
                AeCurve = (UINT32(*)[5])AeCurve_ISO400_50Hz;
                break;
        }
    }
    else//if(nFlicker==_POWERFreq_60Hz)
    {
        switch(nISO)
        {
            case _IMGISO_50:
                AeCurve = (UINT32(*)[5])AeCurve_ISO50_60Hz;
                break;

            case _IMGISO_100:
                AeCurve = (UINT32(*)[5])AeCurve_ISO100_60Hz;
                break;

            case _IMGISO_200:
                AeCurve = (UINT32(*)[5])AeCurve_ISO200_60Hz;
                break;

            case _IMGISO_400:
            case _IMGISO_800:
                AeCurve = (UINT32(*)[5])AeCurve_ISO400_60Hz;
                break;
        }
    }
}

//#NT#2010/09/24#ethanlau -begin
//#NT#_coor:    inner block coordinate
//    _ratio:   return TRUE if inner ave lumi > ratio*reminder ave lumi.(unit:%)
BOOL CheckBackLight(pCoordinate _coor,UINT _ratio)
{
    UINT32 i,j,count_out=0,count_inner=0,count=0;
    for(i=0;i<_coor->Starty;i++)
        for(j=0;j<CA_WINDOW_X;j++)
        {
            count_out+=aaa_Get_AE_BlockValue(j,i);
            count++;
            count_out+=aaa_Get_AE_BlockValue(j,CA_WINDOW_Y-1-i);
            count++;
        }
    for(i=0;i<_coor->Startx;i++)
        for(j=0;j<CA_WINDOW_Y;j++)
        {
            count_out+=aaa_Get_AE_BlockValue(i,j);
            count++;
            count_out+=aaa_Get_AE_BlockValue(CA_WINDOW_X-1-i,j);
            count++;
        }
    count_out /= count;

    count=0;
    for(i=_coor->Starty;i<=_coor->Endy;i++)
        for(j=_coor->Startx;j<=_coor->Endx;j++)
        {
            count_inner+=aaa_Get_AE_BlockValue(j,i);
            count++;
        }
    count_inner /= count;

    //debug_msg("CheckBackLight - inner = %d, out = %d\r\n",count_inner,count_out);
    if((count_out*_ratio) > (count_inner*100))
        return TRUE;
    else
        return FALSE;
}
//#NT#2010/09/24#ethanlau -end

//#NT#20091112#ethanlau -begin
//smart scene result
static _IMGSceneMode_sel SceneRslt;
void SetSmartSceneRst(_IMGSceneMode_sel scenerst)
{
    SceneRslt = scenerst;
    //add callback here
    if(scenerst!=0xff)
    {
        gAlgMsg.SmartSceneRst = SceneRslt;
        g_fpAlgExport(ALGMSG_SMARTSCENE, &gAlgMsg);
    }
}
void SceneModeLiveview_AE(_IMGSceneMode_sel scene)
{
    UINT32 block_w,block_h;
    UINT32 face_location[20][2];
    UINT32 i,j,k,size_x,size_y;
    Coordinate _coor;
    static UINT _count=0;
    static UINT _PreSmartSceneRslt=_SceneMode_Smart;
    switch(scene)
    {
        case _SceneMode_NightScene:
        case _SceneMode_NightPotrait:
        case _SceneMode_NightLandscape:
            nMin_EV_Value = EV_Value_MIN_NightScene;
            break;
        case _SceneMode_BackLight:
        case _SceneMode_Potrait:
            if(Is_FDLock())
            {
                AEMode_AE(_IMGAEMode_Spot);
                //debug_msg("WARING : Potrait need to unlock FD\r\n");
            }
            else
            {
                memset(&AE_Window_Adaptive,1,sizeof(UINT8)*64);
                block_w = GetIPPSizeInfor(_ImeOut1Hsize)>>3;
                block_h = GetIPPSizeInfor(_ImeOut1Vsize)>>3;
                if(gAlgMsg.FaceDetectNum == 0)
                    AEMode_AE(_IMGAEMode_CenterWeighted);
                else
                {
                    for(j=0;j<gAlgMsg.FaceDetectNum;j++)
                    {
                        for(i=0;i<CA_WINDOW_X;i++)
                        {
                            if(i*block_w > gAlgMsg.FaceDetectRect[j].stx)
                            {
                                face_location[j][0] = i-1;
                                break;
                            }
                        }
                        for(i=0;i<CA_WINDOW_X;i++)
                        {
                            if(i*block_h > gAlgMsg.FaceDetectRect[j].sty)
                            {
                                face_location[j][1] = i-1;
                                break;
                            }
                        }
                        size_x = gAlgMsg.FaceDetectRect[j].width / block_w + 1;
                        size_y = gAlgMsg.FaceDetectRect[j].height / block_h + 1;
                        for(i=face_location[j][0];i<CA_WINDOW_X;i++)
                        {
                            if((i - face_location[j][0])<size_x)
                            {
                                for(k=face_location[j][1];k<CA_WINDOW_Y;k++)
                                {
                                    if((k - face_location[j][1])<size_y)
                                    {
                                        if(AE_Window_Adaptive[i][k] == 1)
                                            AE_Window_Adaptive[i][k]+=2;
                                    }
                                }
                            }
                        }
                    }
                }
                AEMode_AE(_IMGAEMode_FaceDetection);
            }
            //nMin_EV_Value = EV_Value_MIN;
            break;
        case _SceneMode_Sport:
            AEMode_AE(_IMGAEMode_Matrix);
            nMin_EV_Value = EV_Value_MIN;
        break;
        case _SceneMode_Smart:
            _coor.Startx = _coor.Starty = 2;
            _coor.Endx = _coor.Endy = 5;
            if(gAlgMsg.FaceDetectNum!=0)
            {
                if(_PreSmartSceneRslt != _SceneMode_Potrait)
                    _count=0;
                _count++;
                if(_count>=5)
                    SetSmartSceneRst(_SceneMode_Potrait);
                _PreSmartSceneRslt = _SceneMode_Potrait;
            }
            else if(((AE_Param.EV_Value==EV_Value_MIN) && (AE_Param.StableCnt<1))||(AE_Param.EV_Value<EV_Value_MIN))
            {
                if(_PreSmartSceneRslt != _SceneMode_NightScene)
                    _count=0;
                _count++;
                if(_count>=5)
                    SetSmartSceneRst(_SceneMode_NightScene);
                _PreSmartSceneRslt = _SceneMode_NightScene;
            }
            else if(CheckBackLight(&_coor,70))
            {
                if(_PreSmartSceneRslt != _SceneMode_BackLight)
                    _count=0;
                _count++;
                if(_count>=5)
                    SetSmartSceneRst(_SceneMode_BackLight);
                _PreSmartSceneRslt = _SceneMode_BackLight;
            }
            else
            {
                if(_PreSmartSceneRslt != _SceneMode_Smart)
                    _count=0;
                _count++;
                if(_count>=5)
                    SetSmartSceneRst(_SceneMode_Smart);
                _PreSmartSceneRslt = _SceneMode_Smart;
            }
        break;
        case _SceneMode_Landscape:
        case _SceneMode_AV:
        case _SceneMode_TV:
        case _SceneMode_Manual:
        case _SceneMode_Party:
        case _SceneMode_SunSet:
        case _SceneMode_Document:
        case _SceneMode_Firework:
        case _SceneMode_Beach:
        case _SceneMode_BMode:
        case _SceneMode_Program:
        default:
            nMin_EV_Value = EV_Value_MIN;
        break;
    }
}
//#NT#20091112#ethanlau -end
//#NT#20090703#ETHAN LAU -BEGIN
//include meter,filcker,liveview smartscene
void AE_CONTROL(PImageAlgInfo Info)
{
    static UINT8 nPreFlickerSetting = 0xFF;
    static UINT8 nPreISOSetting = 0xFF;
    static UINT8 nPreSceneModeSetting = 0xFF;
    //static UINT8 nPreAEModeSetting = 0xFF;
    static UINT8 nPreSensormode = 0xFF;


    if(Info->SceneMode == _SceneMode_Auto)
    {
        nMin_EV_Value = EV_Value_MIN;
        //226-->LV4.5
        //273-->LV4.75
        //320-->LV5
        //407-->LV5.25
        //453-->LV5.5
        //547-->LV5.75
        //640-->LV6
        //nMin_EV_Value = 453;
        if((Info->Frequency != nPreFlickerSetting)||(nPreSensormode!= Get_SensorMode()))
        {
            AntiFlicker_AE(Info->Frequency);
        }
        //#NT#2010/09/21#ethanlau -begin
        //#NT#remove , cause when scene chg from Potrait to others,meter setting whill be return directly
        //if(Info->AEMode != nPreAEModeSetting)
        //#NT#2010/09/21#ethanlau -end
        {
            AEMode_AE(Info->AEMode);
        }
    }
    else
    {
        //#NT#2010/09/21#ethanlau -begin
        //#NT#remove , cause when scene chg from Potrait to others,meter setting whill be return directly
        //if(Info->AEMode != nPreAEModeSetting)
        //#NT#2010/09/21#ethanlau -end
        {
            AEMode_AE(Info->AEMode);
        }
        if((Info->Frequency != nPreFlickerSetting)||(nPreSensormode!= Get_SensorMode()))
        {
            AntiFlicker_AE(Info->Frequency);
        }
        if(Info->SceneMode!= _SceneMode_Smart)
        {
            SetSmartSceneRst (0XFF);
            SceneModeLiveview_AE(Info->SceneMode);
        }
        else //if(Info->SceneMode== _SceneMode_Smart)
        {
            SceneModeLiveview_AE(_SceneMode_Smart);
            SceneModeLiveview_AE(SceneRslt);
        }

    }

    nPreFlickerSetting = Info->Frequency;
    nPreISOSetting = Info->ISO;
    nPreSceneModeSetting = Info->SceneMode;
    //nPreAEModeSetting = Info->AEMode;
    nPreSensormode = Get_SensorMode();
}
//#NT#20090703#ETHAN LAU -END
/**
    Initialize AE
*/
BOOL AE_FirstRun=TRUE;
void aaa_InitAEParameter(void)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    if(!AE_FirstRun)
        return;
    AE_Param.EV_Value = EV_INITIAL;
    //#NT#20090707#ETHAN LAU -begin
    AE_CONTROL(pimginfo);
    //AntiFlickerISO_AE(gImageAlgInfo.ISO, gImageAlgInfo.Frequency);
    //#NT#20090707#ETHAN LAU -end
    AE_FirstRun = FALSE;
}

//#NT#20090706#ETHAN LAU -begin
void aaa_InitAE(UINT32 para_num,...)
{
    UINT32 ptr,tmp;
    va_list marker;
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();
    nAE_Maxline = GetAeMaxExptline();
    va_start(marker, para_num);
    ptr = va_arg( marker, UINT32);
    va_end(marker);
    AE_ChangeRate(AE_RATE_NORMAL);
    AE_Flag=0;
    aaa_InitAEParameter();
    AE_PclkParaSet(Get_SensorMode());
    Allot_AeSet(AE_Param.EV_Value);
    Ae_SetPgaGain();

    //#NT#2010/09/09#ethanlau -begin
    //#NT#set min iso
    if(CalInfo->CalStatus_ISO == _ISO_Status)
        tmp = CalInfo->Cal_ISO_Data[0];
    else
        tmp = AGC_ISO50 + ADJ_ISO;
    MinISO(tmp,&MINISO);
    //#NT#2010/09/09#ethanlau -end
    aaa_AeLock(LOCK);
}

//#NT#20090706#ETHAN LAU -end

void SetPreExpLine(UINT32 Expline)
{
    if(Expline > nAE_Maxline-2)
    {
        Sensor_WriteReg(0x300A, Expline+2); //frame_length_lines
        Sensor_WriteReg(0x3012, Expline); //coarse integration time
    }
    else
    {
        Sensor_WriteReg(0x300A, nAE_Maxline); //frame_length_lines
        Sensor_WriteReg(0x3012, Expline); //coarse integration time
    }
}

void AeSetAGCGain(void)
{
    Ae_SetPgaGain();
}

void SetPreAGC(int intGain, int fraGain)
{
    Sensor_WriteReg(0x305E, fraGain);   //GLOBAL GAIN
}

UINT32 Get_ExpoFrame(void)
{
    return g_Idx_f;
}

UINT8 Calc_CapExpoFrame(void)
{

    return g_Idx_f;
}

void AeCapSet(void)
{
    nAE_Maxline = GetAeMaxExptline();
    SetPreExpLine(AE_Param.ExpoLine_Cap);
    SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
}
void AE_PclkParaSet(_SEN_MODE mode)
{
    switch(mode)
    {
        case SEN_MODE_FULL:
        case SEN_MODE_720P_30:
        case SEN_MODE_1280_960:
        case SEN_MODE_1280_480:
        case SEN_MODE_2560_960:
        case SEN_MODE_VGA:
        default:
            SENSOR_PRE_PCLK = 48000000;
            SENSOR_PRE_PCLK_M = (SENSOR_PRE_PCLK/1000000);
        break;
    }
}

BOOL AE_UartMsg = FALSE;
BOOL AE_OsdMsg = FALSE;
void aaa_AEprocess(void)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    GetActImgFBAddr_Path1(&FB_para1);

    gAEcounter++;//=Get_SieFrameCounter();
    AE_PclkParaSet(Get_SensorMode());

    AE_CONTROL(pimginfo);

    //center histogram
    aaa_GetCenterHist(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset), (UW*)CenterHist);

    //global histogram
    YAverage=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    aaa_Read_HIST_RSLT((UW*)Hist);
    AeConverge(YAverage);
    if(AE_UartMsg)
    {
        if((gAEcounter%5)==0)
        {
            debug_err(("YAverage=%3d, LV=%3d, EV_Value=%8d, ISO_Gain(AGC)=%3d(0x%4x), ExpoTime=%7d, AE_AddY=%3d, AE_ExpectY=%3d, ExpoLine=%d(%d), Stable=%d\r\n",
                        YAverage,
                        LV,
                        AE_Param.EV_Value,
                        AE_Param.ISO_Gain,
                        AE_Param.AePgaGain_fra,
                        AE_Param.ExpoTime,
                        AE_Debug.AE_AddY,
                        AE_Debug.AE_ExpectY,
                        AE_Param.ExpoLine,
                        nAE_Maxline,
                        AE_Param.StableCnt));
            debug_err(("----------------------------\r\n"));
            gAEcounter=0;
            AddVideoIQInfo(message);
            AddVideoIQInfo( "AWB %d,  %d   %d\r\n", Rgain, Ggain, Bgain);
        }
    }
    if(AE_OsdMsg)
    {
        SenFP_ClearOSD();
//        sprintf(message,"%d, %d, %d", YAverage, LV, AE_Param.EV_Value);
//        SenFP_ShowOSDString(message,20,70,4);
        sprintf(message,"%d, %d, %d", AE_Param.ExpoTime, AE_Param.ISO_Gain, AE_Param.AePgaGain_int);
        SenFP_ShowOSDString(message,20,20,4);
    }


    //dynamic iq setting after ae stable
    //if( (AE_Param.StableCnt>1) ||(AE_Param.EV_Value ==nMin_EV_Value))
    {
        SenFlowFunc(IPE_PRV_AE_SETTING, &gSenCBInfo);
        //SenFlowFunc(IME_Chrom_Prv_AE_SETTING, &gSenCBInfo);
    }
    if ( AE_Counter < 255 )
        AE_Counter++;
}

// return (2^(i/4))*256, 所以使用Power3db前必須把return除256
int Power3db(int i)
{
    int j, temp=256;
    for (j=0; j<i; j++)
        temp = (temp *304)>>8;
    return temp;
}

void AeConverge(UINT32 YAvg)
{
#if 0
    UINT32 AeRatio,EV_Value;

    AeRatio = GetAEAdjustRatio(YAvg);
    if(AeRatio> (100-YConRange) && AeRatio< (100+YConRange) && !(AE_Flag & Flag_AeReset))
    {
        AE_Param.StableCnt++;
        return;
    }
/*
    else if((bAE_LowLightMode)&&(AeRatio>(100-YConRange-20))&&(AeRatio<(100+YConRange))&&(!(AE_Flag & Flag_AeReset)))
    {
        AE_Param.StableCnt++;
        return;
    }
*/
    else
    {
        AE_Flag &= (~Flag_AeReset);
        AE_Param.StableCnt = 0;
    }
    if(AE_UartMsg)
        debug_msg(" %d \r\n",AeRatio);
    if( (AeRatio<130)||(AeRatio>70) )
    {
        EV_Value = AE_Param.EV_Value*AeRatio/100;
        EV_Value = (EV_Value*6+AE_Param.EV_Value*4)/10;
    }
    else
    {
        EV_Value = AE_Param.EV_Value*AeRatio/100;
        EV_Value = EV_Value;
    }
    if(EV_Value>=EV_Value_MAX)
        EV_Value=EV_Value_MAX;
    else
    {
        if(EV_Value <= nMin_EV_Value)
           EV_Value = nMin_EV_Value;
    }
/*
    if(0)//(EV_Value < AE_LOWLIGHT_EV_THRES)
    {
        AE_ChangeRate(AE_RATE_LOWLIGHT);
    }
    else
    {
        AE_ChangeRate(AE_RATE_NORMAL);
    }
*/
    AE_Param.EV_Value=EV_Value;

    Allot_AeSet(EV_Value);
#else //A1040
    unsigned long AeRatio,EV_Value;

    AeRatio = GetAEAdjustRatio(YAvg);
    if(AeRatio> (100-YConRange) && AeRatio< (100+YConRange) && !(AE_Flag & Flag_AeReset))
    {
        AE_Param.StableCnt++;
        return;
    }
    else
    {
        AE_Flag &= (~Flag_AeReset);
        AE_Param.StableCnt = 0;
    }

    EV_Value = AE_Param.EV_Value*AeRatio/100; //smoothing
    EV_Value = (EV_Value*3+AE_Param.EV_Value)/4;

    if(EV_Value>=EV_Value_MAX)
    {
        EV_Value=EV_Value_MAX;
    }
    else
    {
        if(EV_Value <= nMin_EV_Value)
        {
           EV_Value = nMin_EV_Value;
        }
        /*
        if(EV_Value <= 340) //226 --> LV4.5; 340 --> LV4.75
        {
           EV_Value = 340;
        }
        */
    }
    AE_Param.EV_Value=EV_Value;
    Allot_AeSet(EV_Value);
#endif
}

void Ae_SetExpoTime(void)
{
    if(AE_Flag & Flag_SetExpoTime)
    {
        SetPreExpLine(AE_Param.ExpoLine);
        AE_Flag &= (~Flag_SetExpoTime);
    }
}

void Ae_test(UINT16 i)
{
    aaa_AeLock(1);
    SetPreExpLine(i);
    //SetPreAGC(0,i);
}


void Ae_SetPgaGain(void)
{
    if(AE_Flag & Flag_SetPgaGain)
    {
        SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
        AE_Flag &= (~Flag_SetPgaGain);
    }
}

void Ae_SetIris(void)
{
     if(AE_Flag & Flag_SetIris)
    {
        AE_Flag &= (~Flag_SetIris);
    }
}

void AeSet(void)
{
#if 0
    //#NT#2010/09/10#ethanlau -begin
    //#NT#time and agc must set in the same frameend
    //Ae_SetExpoTime();
    //#NT#2010/09/10#ethanlau -end
#else//A1040
    Ae_SetExpoTime();
    Ae_SetPgaGain();
#endif
}

void AeSet_isr(void)
{
    //#NT#20091106#ethanlau -begin
    //use sensor setting to let agc and aec setting in the same vd
    SenFlowFunc(AE_PRV_GAIN_SETTING, &gSenCBInfo);
    Ae_SetExpoTime();
    //#NT#20091106#ethanlau -end
}
//#NT#20091027#ethanlau -end
UINT32 Get_AE_Idx(void)
{
    return AE_Idx;
}

//#NT#20091008#ethanlau -begin
void AntiShakingMech(_IMGStillAntiShaking level,_POWERFreq_sel _flicker)
{
    SetAntishaking_RawbiningEnalbe(FALSE);
    switch(level)
    {
        case _Level1:
        case _Level2:
            switch(_flicker)
            {
                case _POWERFreq_50Hz:
                    AeCurve = (UINT32(*)[5])AeCurve_AntiShaking50;
                    break;
                case _POWERFreq_60Hz:
                default:
                    AeCurve = (UINT32(*)[5])AeCurve_AntiShaking60;
                    break;
            }
        break;
        case _Level0:
        default:
        break;
    }
}
//#NT#20091008#ethanlau -end

void AeCap2Prv(void)
{
    //#NT#2010/05/10#ethanlau -begin
    //#NT#increase set2preview vd duration
    if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)
        AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
    else
        AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
    AE_PclkParaSet(Get_SensorMode());
    Allot_AeSet(AE_Param.EV_Value);
    //#NT#2010/05/10#ethanlau -end
    AE_Param.ExpoLine = AeExpoTime2TGValue(AE_Param.ExpoTime,GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M);
    AeISOGain2TGValue(AE_Param.ISO_Gain);
    SetPreExpLine(AE_Param.ExpoLine);
    SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
}


void SceneMode_AE(UINT8 scene)
{
    switch(scene)
    {
        case _SceneMode_Landscape:
        case _SceneMode_SunSet:
            AeCurve = (UINT32(*)[5])AeCurve_Landscape;
            nMin_EV_Value = EV_Value_MIN;
            break;
        case _SceneMode_NightLandscape:
            AeCurve = (UINT32(*)[5])AeCurve_NightLandscape;
            nMin_EV_Value = EV_Value_MIN_NightScene;
            break;
        case _SceneMode_NightPotrait:
            if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)
                AeCurve = (UINT32(*)[5])AeCurve_NightPotrait60;
            else
                AeCurve = (UINT32(*)[5])AeCurve_NightPotrait50;
            nMin_EV_Value = EV_Value_MIN_NightScene;
            break;
        case _SceneMode_Concert:
        case _SceneMode_Sport:
            if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)
                AeCurve = (UINT32(*)[5])AeCurve_ISO400_60Hz;
            else
                AeCurve = (UINT32(*)[5])AeCurve_ISO400_50Hz;
            nMin_EV_Value = EV_Value_MIN;
            break;
        case _SceneMode_NightScene:
            nMin_EV_Value = EV_Value_MIN_NightScene;
            if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)

                AeCurve = (UINT32(*)[5])AeCurve_NormalCapture60;
            else
                AeCurve = (UINT32(*)[5])AeCurve_NormalCapture50;
            break;
        case _SceneMode_Potrait:
        case _SceneMode_Party:
        case _SceneMode_AV:
        case _SceneMode_TV:
        case _SceneMode_Manual:
        case _SceneMode_BackLight:
        case _SceneMode_Document:
        case _SceneMode_Firework:
        case _SceneMode_Beach:
        case _SceneMode_BMode:
        case _SceneMode_Program:
        default:
            nMin_EV_Value = EV_Value_MIN;
            if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)
                AeCurve = (UINT32(*)[5])AeCurve_NormalCapture60;
            else
                AeCurve = (UINT32(*)[5])AeCurve_NormalCapture50;
            break;
    }

}

void AePrv2Cap(void)
{
    UINT32 CapISO=0,CapTime=0,_preiso,_preexpt,_preexpl;
    UINT16 TargetY, OriY_Pregam, TarY_Pregam;
    UINT32 nPrev_Expline, nCap_Expline, ShutterOffset,  ExpoTime,tmp,_ratio;
    INT32 GR_rst_end, GR_shutter_start, GR_read_start,ShutterTime;
    UINT32 weight = 0;
    PImageAlgInfo pimginfo;
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();
    pimginfo = UI_GetParameter();
    //#RP#20110704#eric -begin
    //#Fix capture 過曝
    //AE_PclkParaSet(SEN_MODE_FULL);
    AE_PclkParaSet(SEN_MODE_1280_960);
    //#RP#20110704#eric -end
    _preiso = AE_Param.ISO_Gain;
    _preexpt = AE_Param.ExpoTime;
    _preexpl = AE_Param.ExpoLine;
    if(GRS_MODE)//GRS AePrv2Cap
    {
        if(Flash_Param.FlshEna)
        {
            ExpoTime=AE_Param.ExpoTime*GetIPPSizeInfor(_Binning);
            SetPreExpLine(GetIPPSizeInfor(_VDTotal_Captrue));//To reduce dark gradient current. Recommended by Micron FAE
        }
        else
        {
            Ae_SetPgaGain();
            ExpoTime=AE_Param.ExpoTime*GetIPPSizeInfor(_Binning);
            SetPreExpLine(100);//To reduce dark gradient current. Recommended by Micron FAE
        }

        //compensate shutter lag
/*
        if(GetMshutterDelay())
            ShutterOffset=GetMshutterDelay();
        else
            ShutterOffset=3350;//1000us default offset
*/
        if(ShutterOffset <2500)
            ShutterOffset = 3350;
        else if(ShutterOffset >4500)
            ShutterOffset = 3350;

        debug_err(("AePrv2Cap ShutterOffset=%d\r\n", ShutterOffset));

        GR_rst_end=0x600;//0x34c;//844;
        GR_read_start=ExpoTime*SENSOR_CAP_PCLK_M*2/512+GR_rst_end;//internal clock=PCLK*2
/*
        ShutterCloseLeadTime = ShutterOffset*SENSOR_CAP_PCLK_M*2/512;
        if(GR_read_start > ShutterCloseLeadTime)
            GR_shutter_start = GR_read_start - ShutterCloseLeadTime;
        else
            GR_shutter_start = 0;
*/
/*
        if(ExpoTime > ShutterOffset)
        {
            ShutterTime=ExpoTime-ShutterOffset;
            GR_shutter_start=ShutterTime*SENSOR_CAP_PCLK_M*2/512+GR_rst_end;//internal clock=PCLK*2
        }
        else
        {
            //ShutterTime=ExpoTime;
            //ShutterCloseLeadTime = ExpoTime-80;
            ShutterTime=0;
            ShutterCloseLeadTime = ShutterOffset-ExpoTime;
            GR_shutter_start=GR_rst_end-ShutterCloseLeadTime*SENSOR_CAP_PCLK_M*2/512;//internal clock=PCLK*2
        }
*/
        //Set exposure setting


        debug_err(("AePrv2Cap ExpoTime=%d\r\n", ExpoTime));
        debug_err(("AePrv2Cap ShutterTime=%d\r\n", ShutterTime));
        debug_err(("AePrv2Cap GR_read_start=%d\r\n", GR_read_start));
        debug_err(("AePrv2Cap GR_shutter_start=%d\r\n",GR_shutter_start));

        if(GR_read_start>0xFFFF)
            GR_read_start=0xFFFF;

        if(GR_shutter_start>0xFFFF)
            GR_shutter_start=0xFFFF;

//        Sensor_WriteReg(0x3162, GR_rst_end);
//        Sensor_WriteReg(0x3164, GR_shutter_start);
//        Sensor_WriteReg(0x3166, GR_read_start);
    }
    else//ERS AePrv2Cap
    {
        //debug_msg("Flash_Param.FlshEna= %d\r\n",Flash_Param.FlshEna);
        if(Flash_Param.FlshEna<Flash_Enable)
        {
            SceneMode_AE(gImageAlgInfo.SceneMode);
            if(gImageAlgInfo.ISO != _IMGISO_Auto)
                AntiFlickerISO_AE(gImageAlgInfo.ISO,gImageAlgInfo.Frequency);
            if(GetIPPFuncInfor(_StillAntiShakingIdx)!= _Level0)
            {
                AntiShakingMech(GetIPPFuncInfor(_StillAntiShakingIdx),gImageAlgInfo.Frequency);
            }
            Allot_AeSet(AE_Param.EV_Value);

            CapTime = AE_Param.ExpoTime*GetIPPSizeInfor(_Binning);
            CapISO = AE_Param.ISO_Gain;
            //#NT#2010/12/15#JJ Huang -begin
            //#NT#For SNR Measure: to have accurate ISO setting
            if(SenSNR_IsSNRMeasureEnable())
            {// AE Control (with fixed Fno!!)
                static UINT32 uiCapISOSelection=0xffffffff;
                static UINT32 uiCapTimeXISO=0;
                if(uiCapTimeXISO==0)
                { // 1st time
                    uiCapTimeXISO = CapTime*CapISO;
                    uiCapTimeXISO = uiCapTimeXISO*100/50;//AE Target Compensation
                }

                if(gImageAlgInfo.ISO==_IMGISO_50)
                {
                    CapTime = uiCapTimeXISO/50;
                    CapISO = 50;
                }
                else if(gImageAlgInfo.ISO==_IMGISO_100)
                {
                    CapTime = uiCapTimeXISO/100;
                    CapISO = 100;
                }
                else if(gImageAlgInfo.ISO==_IMGISO_200)
                {
                    CapTime = uiCapTimeXISO/200;
                    CapISO = 200;
                }
                else if(gImageAlgInfo.ISO==_IMGISO_400)
                {
                    CapTime = uiCapTimeXISO/400;
                    CapISO = 400;
                }
                else if(gImageAlgInfo.ISO==_IMGISO_800)
                {
                    CapTime = uiCapTimeXISO/800;
                    CapISO = 800;
                }
                else if(gImageAlgInfo.ISO==_IMGISO_1600)
                {
                    CapTime = uiCapTimeXISO/1600;
                    CapISO = 1600;
                }
                if(SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_MEAN) > (SenSNR_UNITY*103/100) ||
                    SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_MEAN) < (SenSNR_UNITY*97/100) )
                {
                    CapTime = (CapTime)*SenSNR_UNITY/SenSNR_GetSNRData(SenSNR_DATA_WEIGHTED_MEAN);
                }
                uiCapTimeXISO = CapTime*CapISO;

            }
            //#NT#2010/12/15#JJ Huang -end
        }
        if(Flash_Param.FlshEna>=Flash_Enable)
        {
            //ers
            ExpoTime = GetIPPSizeInfor(_VDTotal_Captrue)*GetIPPSizeInfor(_HDTotal_Captrue)/SENSOR_CAP_PCLK_M;
        }
        else
        {
            ExpoTime=CapTime;
            TargetY = NormalExpectY*EVCompTable[gImageAlgInfo.AEEVValue - 4]/100;
            if((AE_Param.EV_Value <= (nMin_EV_Value*12/10))&&(YAverage<TargetY))
            {
                OriY_Pregam = GammaInverse((YAverage<<2),GammaLUT_128Tab,_SieBitDepth_10);
                OriY_Pregam = OriY_Pregam / (1<<(_SieBitDepth_10 - _SieBitDepth_8));
                if(OriY_Pregam == 0)
                    OriY_Pregam = 1;
                TarY_Pregam = GammaInverse((TargetY<<2),GammaLUT_128Tab,_SieBitDepth_10);
                TarY_Pregam = TarY_Pregam / (1<<(_SieBitDepth_10 - _SieBitDepth_8));
                //ExpoTime = ExpoTime*TarY_Pregam/OriY_Pregam * 110/100;//adjust cause VIG
                ExpoTime = ExpoTime*TarY_Pregam/OriY_Pregam;
                if(gImageAlgInfo.Frequency==_POWERFreq_50Hz)
                {
                    if(ExpoTime >320000)
                        ExpoTime = 320000;
                }
                else
                {
                    if(ExpoTime >266666)
                        ExpoTime = 266666;
                }
                //SetIPPFuncInfor(PRED2D_EN,ENABLE);
            }

            AE_Param.ExpoTime_Cap = ExpoTime;
            if(Flash_Param.FlshEna< Flash_Enable)
                AE_Param.ISO_Gain_Cap = CapISO;

            if(Flash_Param.FlshEna>= Flash_Enable)
                AE_Param.ExpoLine_Cap =  nAE_Maxline;//SCR
            else
                AE_Param.ExpoLine_Cap = AeExpoTime2TGValue(AE_Param.ExpoTime_Cap,GetIPPSizeInfor(_VDTotal_Captrue),GetIPPSizeInfor(_HDTotal_Captrue),SENSOR_CAP_PCLK_M);
            //#NT#2010/12/15#JJ Huang -begin
            //#NT#For SNR Measure: to have accurate ISO setting
            if(SenSNR_IsSNRMeasureEnable())
            {
                if(gImageAlgInfo.ISO<_IMGISO_50 || gImageAlgInfo.ISO>_IMGISO_1600)
                {
                    AE_Param.ISO_Gain_Cap = AE_Param.ISO_Gain_Cap*Expo_CompensateRatio/100;
                }
            }
            //#NT#2010/12/15#JJ Huang -end
            AeISOGain2TGValue(AE_Param.ISO_Gain_Cap);
        }
        //debug_err(("AePrv2Cap Flash_Param.Ena = %d\r\n", Flash_Param.FlshEna));
        AE_Param.ISO_Gain = _preiso;
        AE_Param.ExpoTime = _preexpt;
        AE_Param.ExpoLine = _preexpl;
        debug_err(("AePrv2Cap iso=%d capiso=%d\r\n",AE_Param.ISO_Gain,AE_Param.ISO_Gain_Cap));
        debug_err(("AePrv2Cap preT=%d capT=%d\r\n",AE_Param.ExpoTime,AE_Param.ExpoTime_Cap));
        debug_err(("AePrv2Cap preL=%d capL=%d\r\n",AE_Param.ExpoLine,AE_Param.ExpoLine_Cap));
    }
}
//#NT#20090708#ETHAN LAU -end

//#NT#20090706#ETHAN LAU -begin
void Ae_Transform(UINT32 para_num,...)
{
    UINT32 ptr;
    va_list marker;
    va_start(marker, para_num);
    ptr = va_arg( marker, UINT32);
    va_end(marker);
#if 1
    switch(Get_SensorMode())
    {
        case SEN_MODE_2560_960:
        case SEN_MODE_720P_30:
        case SEN_MODE_1280_480:
        case SEN_MODE_VGA:
            SENSOR_PRE_PCLK = 48000000;
            SENSOR_PRE_PCLK_M = (SENSOR_PRE_PCLK/1000000);
        break;
    }
#endif
    nAE_Maxline = GetAeMaxExptline();
    //debug_msg("AE_Param.ExpoTime=%d,GetIPPSizeInfor(_HDTotal_Preview)=%d,SENSOR_PRE_PCLK_M=%d,nAE_Maxline=%d\r\n",AE_Param.ExpoTime,GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M,nAE_Maxline);
    AE_Param.ExpoLine = AeExpoTime2TGValue(AE_Param.ExpoTime,GetIPPSizeInfor(_VDTotal_Preview),GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M);
    AeISOGain2TGValue(AE_Param.ISO_Gain);

    AE_ChangeRate(AE_RATE_NORMAL);

    SetPreExpLine(AE_Param.ExpoLine);
    Ae_SetPgaGain();
    Prv_SenMode = Get_SensorMode();
}

//#NT#20090706#ETHAN LAU -end
void ResetAE(void)
{
    AE_Flag |= Flag_AeReset;
    AE_Param.StableCnt = 0;
}

UINT32 EstimateY(void)
{
    YAverage=aaa_EstimateY2(gpuiFbAdd[0],GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    return YAverage;
}

UINT32 AeExpoTime2TGValue(UINT32 Time, UINT32 VDTotal, UINT32 HDTotal,UINT32 Pclk)
{
    UINT32 ExpLine,linetime;

    ExpLine = (Time*Pclk)/HDTotal;

    //debug_err(("ExpoTime=%d ExpoLine=%d\r\n",Time,ExpLine));

    if(ExpLine > 0xFFFF)
       ExpLine = 0xFFFF;

    if(ExpLine > 1004 && ExpLine < 1007)
       ExpLine = 1004;

    if(ExpLine < 0x0001)
       ExpLine = 0x0001;

    //AE_Param.ExpoLine = ExpLine;

    if(ExpLine != 0xFFFF)
    {
        linetime = ExpLine*HDTotal/(Pclk);
    }
    else
    {
        linetime = Time;
    }

    //debug_err(("\r\nlinetime=%d\r\n",linetime));

    if(linetime<Time)
    {
        Expo_CompensateRatio = Time*100/linetime;
    }
    else
    {
        Expo_CompensateRatio = 100;
    }
    return ExpLine;
}

UINT32 ADJ_ISO1 = 100;
UINT32 gAGC_ISO50;//  =AGC_ISO50+ADJ_ISO;

void AeISOGain2TGValue(UINT32 ISO_Gain)
{
    UINT32 Cal_gain, intgain, fragain;

    Cal_gain = AGC_ISO50;

    Cal_gain= Cal_gain*100*ISO_Gain/50/100;

    //upper and lower bound
    if(Cal_gain<200)
    {
        Cal_gain=200;
    }
    if(Cal_gain>=MaxISO*10)
    {
        Cal_gain=MaxISO*10-1;
    }

    //convert desire gain to register setting
    if(Cal_gain<2000)//Agc 0x~2x
    {
        Cal_gain = (Cal_gain * 32 )/1000 ;
        Cal_gain |= 0x1000;
    }
    else if(Cal_gain<4000)//Agc 2x~4x
    {
        Cal_gain/=2;

        Cal_gain = (Cal_gain * 32 )/1000 ;
        Cal_gain |= 0x1040;
    }
    else if(Cal_gain<8000)//Agc 4x~8x
    {
        Cal_gain/=4;

        Cal_gain = (Cal_gain * 32 )/1000 ;
        Cal_gain |= 0x1080;
    }
    else if(Cal_gain<16000) //Agc 8x~16x
    {
        Cal_gain/=8;

        Cal_gain = (Cal_gain * 32 )/1000 ;
        Cal_gain |= 0x10C0;
    }
    else if(Cal_gain<32000) //Agc 16x~32x
    {
        Cal_gain/=16;

        Cal_gain = (Cal_gain * 32 )/1000 ;
        Cal_gain |= 0x20C0;
    }
    else
    {
        Cal_gain= 0x1020;//0x12FF;
    }
    intgain=0;
    fragain=Cal_gain;

    AE_Param.AePgaGain_int=intgain;
    AE_Param.AePgaGain_fra=fragain;
}

void SetPara_PgaGain(UINT32 PgaGain)
{
}

void AE__SetExptime(UINT8 ShutterIdx)
{
}

UINT32 GetAEAdjustRatio(UINT32 YAvg)
{
    UINT32   Ratio;
    UW              ExpectY;
    //UINT          HighCnt;//,i;//,LowCnt;
    BOOL            DoOverExp;
    UINT32  OverExpo_Max_AddY;
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    LV=RatioLog2(AE_Param.EV_Value,10);

    if(YAvg==0)
        YAvg=1;

    //debug_err(("LV = %2d, EV = %5d\r\n", LV, AE_Param.EV_Value));

#if _AE_REDUCE_OVER_EXPO
    //if((gImageAlgInfo.AEEVValue==_IMGEVValue_00) && (gImageAlgInfo.AEMode!=_IMGAEMode_Spot))
    if(pimginfo->AEEVValue==_IMGEVValue_00)
        DoOverExp=1;
    else
        DoOverExp=0;

        if(DoOverExp)
        {
            //if(AE_Param.EV_Value>40960)// Lv12
            if(AE_Param.EV_Value>20480)// Lv11
                OverExpo_Max_AddY=OverExpo_Max_AddY_Outdoor;
        else if(AE_Param.EV_Value<10240) //Lv10
            //#NT#2011/10/05#Spark Chou -begin
            //#NT# Dynamic range
            OverExpo_Max_AddY = InterpolateValue(AE_Param.EV_Value, OverExpo_Max_AddY_Night, OverExpo_Max_AddY_Indoor, 1280, (1280<<2));
            //#NT#2011/10/05#Spark Chou -end
        else
                OverExpo_Max_AddY=Last_OverExpo_Max_AddY;
            Last_OverExpo_Max_AddY=OverExpo_Max_AddY;
            HighCnt=(Hist[60]+Hist[61]+Hist[62]+Hist[63])
                +(Hist[56]+Hist[57]+Hist[58]+Hist[59])
            +(Hist[52]+Hist[53]+Hist[54]+Hist[55])/2;// >224  [224~239]+[240~225

        CenterHighCnt =   (CenterHist[60]+CenterHist[61]+CenterHist[62]+CenterHist[63])
                    +(CenterHist[56]+CenterHist[57]+CenterHist[58]+CenterHist[59])
                    +(CenterHist[52]+CenterHist[53]+CenterHist[54]+CenterHist[55])/2;
        if(0)//(AE_Hist[15]>(75*300) && ((AE_Hist[0]+AE_Hist[1]+AE_Hist[2])>(75*250)))
        {
            if(Add_Y>=Add_Step)
                    Add_Y-=Add_Step;
                else
                    Add_Y=0;
                //if(Last_AEStatus!=2)
                if(Last_AEStatus==1)
                    Damping++;
                Last_AEStatus=2;
        }

        else if(HighCnt>OverExpo_High_Th*5 || CenterHighCnt > OverExpo_Center_High_Th*5)
        {
            if(Add_Y<=OverExpo_Max_AddY-Add_Step)
                Add_Y+=Add_Step;
            else
                    Add_Y=OverExpo_Max_AddY;
                //if(Last_AEStatus!=1)
                if(Last_AEStatus==2)
                    Damping++;
                Last_AEStatus=1;
            }
            else if(HighCnt<OverExpo_Low_Th*5)
            {
                if(Add_Y>=Add_Step)
                    Add_Y-=Add_Step;
                else
                    Add_Y=0;
                //if(Last_AEStatus!=2)
                if(Last_AEStatus==1)
                    Damping++;
                Last_AEStatus=2;
            }
            else
            {
                if(Damping && (Last_AEStatus==0))
                    Damping--;
                Last_AEStatus=0;
            }

            if(((AE_Param.EV_Value/Last_EV)>3) ||((Last_EV/AE_Param.EV_Value)>3) ||((AE_Param.EV_Value/Damping_EV)>3) ||((Damping_EV/AE_Param.EV_Value)>3) )
            {
                Add_Step=Default_AddStep;
                Damping=0;
                Damping_EV=AE_Param.EV_Value;
            }
            else if((Damping>3) && (Add_Step>0))
        {
            if ( Add_Step > 1 )
                Add_Step--;
                Damping_EV=AE_Param.EV_Value;
            }

            Last_EV=AE_Param.EV_Value;

            //debug_err(("hc %d addY %d s %d d%d dev %d\r\n",HighCnt,Add_Y,Last_AEStatus,Damping,Damping_EV));

        }
        else
            Add_Y=0;
#else
            Add_Y=0;
#endif

    if(pimginfo->AEEVValue != _IMGEVValue_00)
    {
        ExpectY = (NormalExpectY*EVCompTable[gImageAlgInfo.AEEVValue - 4])/100;
    }
    else
    {
        ExpectY = NormalExpectY;
    }

    if((ExpectY>Add_Y)&&(AE_Counter > 3))
    {
        ExpectY-=Add_Y;
        AE_Flag |= Flag_AeReset;
    }
    Ratio = (YAvg * 100) / ExpectY;

    if(Ratio<33)
        Ratio = 33;
    else if(Ratio>300)
        Ratio = 300;

    AE_Debug.AE_AddY = Add_Y;
    AE_Debug.AE_ExpectY = ExpectY;

    return Ratio;
}

UINT32 RatioLog2( UINT32 Devider, UINT32 Devident )
{
    UINT32 Ratio;
    UINT32 idx,Ret;

    if(Devident==0)
        return 1;

    Ret = 0;
    Ratio = (Devider * 100) / Devident;
    while( Ratio >= 200 ) {
        Ratio /= 2;
        Ret += 10;
    }

    for( idx = 0; idx < 10; idx++ )
        if( Ratio < LogTbl[idx] )break;

    return Ret + idx;
}

void AE_ChangeRate(UB mode)
{
    //debug_msg("\r\n AE_ChangeRate - %d %d\r\n",mode,Get_SensorMode());
    switch(mode)
    {
        case AE_RATE_LOWLIGHT:
            aaa_SetAeRate(4);
            break;
        case AE_RATE_STARTUP:
            aaa_SetAeRate(3);
            break;
        case AE_RATE_NORMAL:
        default:
            //#NT#20091026#ethanlau-begin
            if((Get_SensorMode() == SEN_MODE_1280_480)||(Get_SensorMode() ==SEN_MODE_VGA))
                aaa_SetAeRate(5);
            else
                aaa_SetAeRate(3);
            //#NT#20091026#ethanlau-end
            break;
    }
}


void Allot_AeSet(UINT32 CurrentEV)
{
    UINT8 i;
    UINT32 Calcul_Time, Calcul_Gain;
    UINT32 FnRatio;

    FnRatio = 85;

    // Search range
    for(i=0; i<20; i++)
    {
        if((CurrentEV*85/FnRatio)>=AeCurve[i][AE_EV])
        //if((CurrentEV)>=AeCurve[i][AE_EV])
        {
            CurrentIdx = i;
            break;
        }
    }

    #if 1
    if(!(AeCurve[i][AE_ADJ_TARGET] & AE_ADJ_IRIS))
    {
        if(AE_Param.AeUseIris != AeCurve[i][AE_IRIS])
        {
            AE_Flag |= Flag_SetIris;
        }
        AE_Param.AeUseIris = AeCurve[i][AE_IRIS];
    }
    #endif

    //FnRatio = Ae_IrisZoomStep0[AE_Param.AeUseIris]*100/80;
    if(AeCurve[i][AE_ADJ_TARGET] & AE_ADJ_EXP)
    {
        Calcul_Gain = AeCurve[i][AE_ISO];
        Calcul_Time = (1000000*FnRatio)/(CurrentEV*Calcul_Gain/100);
    }
    else if(AeCurve[i][AE_ADJ_TARGET] & AE_ADJ_GAIN)
    {
        Calcul_Time = AeCurve[i][AE_EXP];
        Calcul_Gain = (1000000*FnRatio)/(CurrentEV*Calcul_Time/100);
    }

    if(Calcul_Gain > MaxISO)
    {
        Calcul_Gain = MaxISO;
    }
    if(Calcul_Gain < 50)
    {
        Calcul_Gain = 50;
    }

    AE_Param.ISO_Gain = Calcul_Gain;
    AE_Param.ExpoTime = Calcul_Time;

    if ( uiAE_TEST == TRUE )
    {
        AE_Param.ISO_Gain = uiAeIsoGain;
        AE_Param.ExpoTime = uiAeExpoTime;
    }
    else
    {
        uiAeIsoGain = AE_Param.ISO_Gain;
        uiAeExpoTime = AE_Param.ExpoTime;
    }

    AE_Param.ExpoLine = AeExpoTime2TGValue(AE_Param.ExpoTime,GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M);
    AE_Flag |= Flag_SetExpoTime;

    AE_Param.ISO_Gain = AE_Param.ISO_Gain*Expo_CompensateRatio/100;

    AeISOGain2TGValue(AE_Param.ISO_Gain);
    AE_Flag |= Flag_SetPgaGain;

    AeSet();
}

void WaitAeStable(UINT16 count)
{
    UINT16 VD_count = 0;
    AE_Param.StableCnt = 0;
    if((AE_Param.EV_Value <= nMin_EV_Value) || (AE_Param.EV_Value >= EV_Value_MAX))
    {
        return;
    }

    while(AE_Param.StableCnt <= count)
    {
        sie_waitVD(1);
        if(VD_count++ > count * 3)
        {
            debug_err(("- WaitAeStable : Timeout!\r\n"));
            break;
        }
    }
}
//#NT#2010/09/16#ethanlau -begin
//#NT# _bit:bits of gamma in
//#NT# if _targetY==0, converge to ae expectY
//#NT# else converge to _targetY
void Ae_FastConverge2TargetLumi(UINT32* _gamma,_IMGSieBitDepth_sel _bit,UINT _targetY)
{
    UINT32 _currentY,ratio,i;
    UINT32 _pregammaY,_pregammaExpectY;
    aaa_AeLock(LOCK);
    if(_targetY==0)
        _targetY = (NormalExpectY*EVCompTable[gImageAlgInfo.AEEVValue - 4])/100;

    GetActImgFBAddr_Path1(&FB_para1);
    _currentY=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    //debug_msg("evvalue: %d iso: %d time: %d(%d) Y: %d\r\n",AE_Param.EV_Value,AE_Param.ISO_Gain,AE_Param.ExpoTime,AE_Param.ExpoLine,_currentY);

    _pregammaY = GammaInverse((_currentY<<2),_gamma,_bit);
    _pregammaExpectY = GammaInverse((_targetY<<2),_gamma,_bit);
    _pregammaY = _pregammaY/(1<<(_bit - _SieBitDepth_8));
    _pregammaExpectY = _pregammaExpectY/(1<<(_bit - _SieBitDepth_8));
    //debug_msg("Ae_FastConverge2TargetLumi - %d %d %d %d\r\n",_pregammaY,_pregammaExpectY,_currentY,_targetY);
    if (_pregammaY==0)
        _pregammaY=1;
    ratio = _pregammaExpectY*100/_pregammaY;

    AE_Param.EV_Value = AE_Param.EV_Value*100/ratio;
    if(AE_Param.EV_Value < nMin_EV_Value)
        AE_Param.EV_Value=nMin_EV_Value;
    Allot_AeSet(AE_Param.EV_Value);
    //debug_msg("Ae_FastConverge2TargetLumi - %d %d %d %d\r\n",AE_Param.ISO_Gain,AE_Param.ExpoTime,AE_Param.EV_Value,ratio);
    AeSet_isr();
    sie_waitVD(1);
    sie_waitVD(2);
    //GetActImgFBAddr_Path1(&FB_para1);
    //_currentY=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    //debug_msg("- Ae_FastConverge2TargetLumi: ratio _currentY ISO = %d %d %d\r\n",ratio,_currentY,AE_Param.ISO_Gain);
    //debug_msg("agc: 0x%x line: 0x%x 0x%x 0x%x\r\n",Sensor_ReadReg(0x350B),Sensor_ReadReg(0x3500),Sensor_ReadReg(0x3501),Sensor_ReadReg(0x3502));
    aaa_AeLock(UNLOCK);
}
//#NT#2010/09/16#ethanlau -end
BOOL Is_SensorGRERS(void)
{
    return gGR_ERS4LongExp;
}

UINT32 AE__GetLV(void)
{
    return LV;
}

UINT32 AE__GetNYAdj(void)
{
    return NormalY_adj;
}

UINT AE_GetHistHighCnt(void)
{
    return HighCnt;
}
BOOL AE_GetFlagHFED(void)
{
    return f_HalfFrmExpDet;
}

/*************************** calibration function ***************************/

void AeAGCStep(UINT32 step)
{
    UINT32 Cal_gain, intgain, fragain=0;
    UINT32 count=0,decimal;

    gAGC_ISO50= step;

    Cal_gain=gAGC_ISO50;
    Cal_gain = Cal_gain;// iso 50

    while(1)
    {
        if(Cal_gain>=2*AGC_ISO50)
        {
            Cal_gain = Cal_gain>>1;
            count++;
        }
        else
            break;
    }
    if(count==4)
        fragain = 0x7f;
    else
    {
        decimal = 16*(Cal_gain-AGC_ISO50)/AGC_ISO50;
        while(count>0)
            fragain += (0x10*(1<<(--count)));
        fragain+=decimal;
    }

    intgain=0;

    //debug_err(("fragain=%x,decimal=%d,count=%d\r\n",fragain,decimal,tmp));

    AE_Param.AePgaGain_int=intgain;
    AE_Param.AePgaGain_fra=fragain;
}

void AeSetAGCGainStep(UINT32 Step)
{
    AeAGCStep(Step);
    SetPreAGC(AE_Param.AePgaGain_int,AE_Param.AePgaGain_fra);
}

void AeSetAGCGainISO(UINT32 ISO)
{
    AE_Param.ISO_Gain = ISO;
    AeISOGain2TGValue(ISO);
    SetPreAGC(AE_Param.AePgaGain_int,AE_Param.AePgaGain_fra);
}

void AeSetPrvExpoLine(UINT32 ExpoLine)
{
    AE_Param.ExpoLine = ExpoLine;
    SetPreExpLine(ExpoLine);
}

void AeSetPrvExpoTime(UINT32 ExpoTime)
{
    AE_Param.ExpoLine = AeExpoTime2TGValue(ExpoTime, GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M);
    SetPreExpLine(AE_Param.ExpoLine);
}

void AeSetCapExpoTime(UINT32 ExpoTime)
{
    AE_Param.ExpoLine_Cap = AeExpoTime2TGValue(ExpoTime, GetIPPSizeInfor(_VDTotal_Captrue), GetIPPSizeInfor(_HDTotal_Captrue),SENSOR_CAP_PCLK_M);
    SetPreExpLine(AE_Param.ExpoLine_Cap);
}
//#NT#2011/10/05#Spark Chou -begin
//#NT# Dynamic range
extern UINT32     AE_Hist[256];
void aaa_GetCenterHist(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset, UINT32* Histogram)
{
    UINT16 i,j;
    UINT32 hist_total,temp;
    for(i=0;i<256;i++)
    {
        AE_Hist[i] = 0;
    }

    for(i=3;i<=5;i++)
    {
        for(j=2;j<=5;j++)
        {
            aaa_BlockAverageY(YAddr,((j*h_size)/CA_WINDOW_X),((i*v_size)/CA_WINDOW_Y),(h_size/CA_WINDOW_X),(v_size/CA_WINDOW_Y),offset);
        }
    }
    hist_total =0;
    for(i=0;i<256;i++)
    {
        hist_total+=AE_Hist[i];
    }
    if (hist_total == 0) hist_total = 1;
    for(i=0;i<64;i++)
    {
        temp=1024*AE_Hist[i]/hist_total;

        *(Histogram+i)=(UINT32)temp;
    }
}
//#NT#2011/10/05#Spark Chou -end
#endif

