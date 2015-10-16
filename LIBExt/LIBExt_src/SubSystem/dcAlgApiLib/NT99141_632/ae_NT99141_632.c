#include "sensor_model.h"
//#NT#20090706#ETHAN LAU -begin
#include <stdarg.h>
//#NT#20090706#ETHAN LAU -end
#if NT99141_632
#include "sif.h"
#include "uart.h"
#include "pll.h"
#include "ErrorNo.h"
#include "cal_api.h"
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
#include "sen_snr.h"
//#NT#2010/12/15#JJ Huang -end


extern UINT32 IQ_GammaLUT[130];

extern UINT32 uiAE_TEST;
extern UINT32 uiAeIsoGain;
extern UINT32 uiAeExpoTime;

BOOL bDynaGammaTrigger = FALSE;
BOOL AE_UartMsg = FALSE;
BOOL AE_OsdMsg = FALSE;

extern UINT gpuiFbAdd[3];
//#NT20091123#ethanlau -begin
UINT32 MINISO=50;
//#NT#20091123#ethanlau -end

//#NT#20090907#ethanlau -begin
UINT32 SENSOR_PRE_PCLK = 40000000;
UINT32 SENSOR_CAP_PCLK = 40000000;
UINT32 SENSOR_PRE_PCLK_M = 40;//(SENSOR_PRE_PCLK/1000000);
UINT32 SENSOR_CAP_PCLK_M = 40;//(SENSOR_CAP_PCLK/1000000);
BOOL AEDUMMYLINE =TRUE;
UINT32 A1=66000000,A2=66000000,A3=69000000;
//#NT#2011/06/21#Jarkko Chang -begin
//#NT# add for 1440x1080 15fps using
void AE_PclkParaSet(void);
//#NT#2011/06/21#Jarkko Chang -end
BOOL GetDelayFlag(void);
void SetDelayFlag(BOOL flag);
void SetExpotimeFlag(BOOL flag);
BOOL GetExpotimeFlag(void);

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
UINT16  Last_OverExpo_Th_H = 0xFFFF;//OverExpo_Outdoor_Th_H;
UINT16  Last_OverExpo_Th_L = 0;//OverExpo_Outdoor_Th_L;

UINT16 EVCompTable[13] = {25,33,42,50,67,83,100,118,136,154,166,178,190};
INT16 NormalY_adj=0;
//UINT16 Ae_IrisZoomStep0[2] = {80, 320};

UINT8 Last_AEStatus,Damping;
UINT8 AE_Counter= 0;
UINT8 CurrentIdx;

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
        {1,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,1},
        {1,2,3,4,4,3,2,1},
        {1,2,4,4,4,4,2,1},
        {1,2,4,4,4,4,2,1},
        {1,2,3,4,4,3,2,1},
        {1,2,3,3,3,3,2,1},
        {1,1,1,1,1,1,1,1},
        };
const UINT8 AE_Window_CenterWeighted[8][8]={
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 3, 5, 5, 3, 2, 1},
        {1, 2, 5,10,10, 5, 2, 1},
        {1, 2, 5, 8, 8, 5, 2, 1},
        {1, 2, 4, 6, 6, 4, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
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
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0},
        {0,0,0,1,1,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
        };

#if 1
// 25FPS
UINT32 AeCurve_AntiFlicker50[12][5]={
{  5598,     10000,      75,    0,    AE_ADJ_EXP},
{  2780,     10000,     150,    0,    AE_ADJ_GAIN},
{  1389,     20000,     150,    0,    AE_ADJ_GAIN},
{   464,     30000,     300,    0,    AE_ADJ_GAIN},
{   347,     40000,     300,    0,    AE_ADJ_GAIN},
{   231,     40000,     450,    0,    AE_ADJ_GAIN},
{   198,     40000,     525,    0,    AE_ADJ_GAIN},
{   173,     40000,     600,    0,    AE_ADJ_GAIN},
{   138,     40000,     750,    0,    AE_ADJ_GAIN},
{   115,     40000,     900,    0,    AE_ADJ_GAIN},
{     0,     40000,     900,    0,    AE_ADJ_GAIN},
{     0,     40000,     900,    0,    AE_ADJ_GAIN},
};

UINT32 AeCurve_AntiFlicker60[12][5]={
{  6718,      8333,      75,    0,    AE_ADJ_EXP},
{  3336,      8333,     150,    0,    AE_ADJ_GAIN},
{  1667,     16666,     150,    0,    AE_ADJ_GAIN},
{  1111,     24999,     150,    0,    AE_ADJ_GAIN},
{   464,     33332,     300,    0,    AE_ADJ_GAIN},
{   347,     41665,     300,    0,    AE_ADJ_GAIN},
{   222,     41665,     450,    0,    AE_ADJ_GAIN},
{   190,     41665,     525,    0,    AE_ADJ_GAIN},
{   166,     41665,     600,    0,    AE_ADJ_GAIN},
{   133,     41665,     750,    0,    AE_ADJ_GAIN},
{   110,     41665,     900,    0,    AE_ADJ_GAIN},
{     0,     41665,     900,    0,    AE_ADJ_GAIN},
};
#endif

#if 0
// 30FPS
UINT32 AeCurve_AntiFlicker50[10][5]={
{  5598,     10000,      75,    0,    AE_ADJ_EXP},
{  2780,     10000,     150,    0,    AE_ADJ_GAIN},
{  1389,     20000,     150,    0,    AE_ADJ_GAIN},
{   464,     30000,     300,    0,    AE_ADJ_GAIN},
{   309,     30000,     450,    0,    AE_ADJ_GAIN},
{   231,     30000,     600,    0,    AE_ADJ_GAIN},
{   185,     30000,     750,    0,    AE_ADJ_GAIN},
{   154,     30000,     900,    0,    AE_ADJ_GAIN},
{     0,     30000,     900,    0,    AE_ADJ_GAIN},
{     0,     30000,     900,    0,    AE_ADJ_GAIN},
};

UINT32 AeCurve_AntiFlicker60[10][5]={
{  6718,      8333,      75,    0,    AE_ADJ_EXP},
{  3336,      8333,     150,    0,    AE_ADJ_GAIN},
{  1667,     16666,     150,    0,    AE_ADJ_GAIN},
{  1111,     24999,     150,    0,    AE_ADJ_GAIN},
{   464,     33332,     300,    0,    AE_ADJ_GAIN},
{   278,     33332,     450,    0,    AE_ADJ_GAIN},
{   208,     33332,     600,    0,    AE_ADJ_GAIN},
{   166,     33332,     750,    0,    AE_ADJ_GAIN},
{   138,     33332,     900,    0,    AE_ADJ_GAIN},
{     0,     33332,     900,    0,    AE_ADJ_GAIN},
};
#endif

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

UINT32 AeCurve_NormalCapture60[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,        0,       100,  0,          AE_ADJ_EXP},
{10240,       8333,       100,  0,          AE_ADJ_EXP}, // 1/60*2
{5120,        8333,       200,  0,          AE_ADJ_GAIN},// 1/60*2
{2560,       16666,       200,  0,          AE_ADJ_GAIN},// 1/30*2
{1280,       33333,       200,  0,          AE_ADJ_GAIN},// 1/15*2
{640,        66666,       200,  0,          AE_ADJ_GAIN},// 1/15*2
{320,        66666,       400,  0,          AE_ADJ_GAIN},// 1/7.5*2
{160,       133333,       400,  0,          AE_ADJ_GAIN},// 1/7.5*2
{    0,     266666,       400,  0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_NormalCapture50[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,         100,  0,          AE_ADJ_EXP},
{8533,       10000,       100,  0,          AE_ADJ_EXP}, // 1/50*2
{4266,       10000,       200,  0,          AE_ADJ_GAIN},// 1/50*2
{2133,       20000,       200,  0,          AE_ADJ_GAIN},// 1/25*2
{1066,       40000,       200,  0,          AE_ADJ_GAIN},// 1/12.5*2
{533,        80000,       200,  0,          AE_ADJ_GAIN},// 1/12.5*2
{266,        80000,       400,  0,          AE_ADJ_GAIN},// 1/6.25*2
{133,       160000,       400,  0,          AE_ADJ_GAIN},// 1/6.25*2
{0,     320000,       400,  0,          AE_ADJ_GAIN},// 1/6.25*2
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

UINT AeCurve_AntiShaking60[19][5]={
{  6721,      8333,      75,    0,    AE_ADJ_EXP},
{  3360,      8333,     150,    0,    AE_ADJ_GAIN},
{  1680,     16666,     150,    0,    AE_ADJ_GAIN},
{  1120,     24999,     150,    0,    AE_ADJ_GAIN},
{   840,     33332,     150,    0,    AE_ADJ_GAIN},
{   672,     41665,     150,    0,    AE_ADJ_GAIN},
{   560,     49998,     150,    0,    AE_ADJ_GAIN},
{   480,     58331,     150,    0,    AE_ADJ_GAIN},
{   420,     66664,     150,    0,    AE_ADJ_GAIN},
{   373,     74997,     150,    0,    AE_ADJ_GAIN},
{   336,     83330,     150,    0,    AE_ADJ_GAIN},
{   305,     91663,     150,    0,    AE_ADJ_GAIN},
{   280,     99996,     150,    0,    AE_ADJ_GAIN},
{   258,    108329,     150,    0,    AE_ADJ_GAIN},
{   240,    116662,     150,    0,    AE_ADJ_GAIN},
{   224,    124995,     150,    0,    AE_ADJ_GAIN},
{   105,    133328,     300,    0,    AE_ADJ_GAIN},
{    35,    133328,     900,    0,    AE_ADJ_GAIN},
{     0,    133328,     900,    0,    AE_ADJ_GAIN},
};

UINT AeCurve_AntiShaking50[19][5]={
{  5600,     10000,      75,    0,    AE_ADJ_EXP},
{  2800,     10000,     150,    0,    AE_ADJ_GAIN},
{  1400,     20000,     150,    0,    AE_ADJ_GAIN},
{   840,     30000,     150,    0,    AE_ADJ_GAIN},
{   700,     40000,     150,    0,    AE_ADJ_GAIN},
{   560,     50000,     150,    0,    AE_ADJ_GAIN},
{   420,     60000,     150,    0,    AE_ADJ_GAIN},
{   400,     70000,     150,    0,    AE_ADJ_GAIN},
{   350,     80000,     150,    0,    AE_ADJ_GAIN},
{   311,     90000,     150,    0,    AE_ADJ_GAIN},
{   280,    100000,     150,    0,    AE_ADJ_GAIN},
{   254,    110000,     150,    0,    AE_ADJ_GAIN},
{   233,    120000,     150,    0,    AE_ADJ_GAIN},
{   107,    130000,     300,    0,    AE_ADJ_GAIN},
{    35,    130000,     900,    0,    AE_ADJ_GAIN},
{     0,    130000,     900,    0,    AE_ADJ_GAIN},
{     0,    130000,     900,    0,    AE_ADJ_GAIN},
{     0,    130000,     900,    0,    AE_ADJ_GAIN},
{     0,    130000,     900,    0,    AE_ADJ_GAIN},
};

UINT32 (*AeCurve)[5] = (UINT32(*)[5])AeCurve_AntiFlicker60;


UINT diffp, diffn, np, nn, contrast,CenAvg;
UINT HighCnt;
BOOL f_HalfFrmExpDet=0; //Half frame exposure detection only

void AEMode_AE(UINT8 nMode)
{
    static UINT8 nPreModeSetting = 0xFF;
    if(nMode == nPreModeSetting)
    return;

#if 0
    aaa_SetAeWindow((UB*)AE_Window_CenterWeighted);
#else
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
#endif

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

#if 0
    AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
#else
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
#endif

    nPreFlickerSetting = nFlicker;
    ResetAE();
}



void AntiFlickerISO_AE(UINT8 nISO, UINT8 nFlicker)
{
    if(nFlicker==_POWERFreq_50Hz)
    {
#if 0
		AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
#else
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
#endif
    }
    else//if(nFlicker==_POWERFreq_60Hz)
    {
#if 0
		AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
#else
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
#endif		
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
		if(Info->AEEVValue <= _IMGEVValue_00)
		{
	        nMin_EV_Value = EV_Value_MIN;
		}
		else
		{
			nMin_EV_Value = ((EV_Value_MIN*100)/EVCompTable[gImageAlgInfo.AEEVValue - 4]);
		}

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
UINT16 AE_HIST_CHANGE_EV_L, AE_HIST_CHANGE_EV_H;
	
void aaa_InitAEParameter(void)
{
    PImageAlgInfo pimginfo;
	UINT8 i;
    pimginfo = UI_GetParameter();
    if(!AE_FirstRun)
        return;
    AE_Param.EV_Value = EV_INITIAL;
    //#NT#20090707#ETHAN LAU -begin
    AE_CONTROL(pimginfo);
    //AntiFlickerISO_AE(gImageAlgInfo.ISO, gImageAlgInfo.Frequency);
    //#NT#20090707#ETHAN LAU -end
    AE_FirstRun = FALSE;

	AE_HIST_CHANGE_EV_H = AeCurve_AntiFlicker50[0][AE_EV]-1;

	for(i=0; i<(sizeof(AeCurve_AntiFlicker50)/sizeof(AeCurve_AntiFlicker50[0])); i++)
	{
		if(AeCurve_AntiFlicker50[i][AE_EXP]>=30000)
		{
			AE_HIST_CHANGE_EV_L = AeCurve_AntiFlicker50[i][AE_EV];
			break;
		}
	}
}

//#NT#20090706#ETHAN LAU -begin
void aaa_InitAE(UINT32 para_num,...)
{
    UINT32 ptr,tmp;
    va_list marker;
    Cal_Struct *CalInfo;
    //debug_msg("NT99141 aaa_InitAE ...\r\n");	
    CalInfo = Cal_GetCalibrationData();
    nAE_Maxline = GetIPPSizeInfor(_HDTotal_Preview);
    va_start(marker, para_num);
    ptr = va_arg( marker, UINT32);
    va_end(marker);
    AE_ChangeRate(AE_RATE_NORMAL);
    AE_Flag=0;
    aaa_InitAEParameter();
    Allot_AeSet(AE_Param.EV_Value);
    Ae_SetPgaGain();

    aaa_AeLock(LOCK);
}

//#NT#20090706#ETHAN LAU -end

void SetPreExpLine(UINT32 Expline)
{
    UINT32 Line;

    Line = Expline;

    if (Line > 0xffff)
    {
        Line = 0xffff;
    }

    if (Line < 4)
    {
        Line = 4;
    }
	
    Sensor_WriteReg(0x3012, (UINT8)((Line>>8)&0xFF));
    Sensor_WriteReg(0x3013, (UINT8)(Line&0xFF));
    //Sensor_WriteReg(0x3060, 0x02);
}

void AeSetAGCGain(void)
{
#if 0
    if(AE_Flag & Flag_SetPgaGain)
    {
        SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
        SetPreExpLine1(AE_Param.ExpoLine);
        AE_Flag &= ~Flag_SetPgaGain;
    }
#endif
}

void SetPreAGC(int intGain, int fraGain)
{
    UINT16 Temp;
    UINT16 AGC = 0;

    Temp = (intGain >> 4);

    if(Temp > 0)
        AGC = (((1 << Temp) - 1) << 4);

    Temp = (intGain & 0x0F);
    AGC = AGC + Temp;

    if(AGC > 0x7F)
    {
        debug_err(("register overflow\r\n"));
        AGC = 0x7F;
    }

    //debug_err(("AGC = %X\r\n", AGC));

#if AE_MIN_GAIN_EN
    if(AGC < 0x10)
        AGC = 0x10;
#endif
/*
    if(AGC > 0x1F)
        AGC = 0x1F;
*/
/*
    SenFP_ClearOSD();
    sprintf(message,"%02X", AGC);
    SenFP_ShowOSDString(message,20,100,4);
*/
    Sensor_WriteReg(0x301D, (UINT8)AGC);
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
    nAE_Maxline = GetIPPSizeInfor(_HDTotal_Captrue);
    SetPreExpLine(AE_Param.ExpoLine_Cap);
    SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
}

//#NT#2011/06/21#Jarkko Chang -begin
//#NT# add for 1440x1080 15fps using
void AE_PclkParaSet(void)
{
    UINT32 vid_resolution;
    vid_resolution = GetIPPSizeInfor(_Vid_ResolutionIdx);

    switch(vid_resolution)
    {
        case _Resolution_1440x1080:
            SENSOR_PRE_PCLK = 20500000;
            SENSOR_PRE_PCLK_M = (SENSOR_PRE_PCLK/1000000);
        break;
        default:
            SENSOR_PRE_PCLK = 41000000;
            SENSOR_PRE_PCLK_M = (SENSOR_PRE_PCLK/1000000);
        break;
    }
}
//#NT#2011/06/21#Jarkko Chang -end

#if _AE_HIST_METER_ENABLE

UINT32 AE_HistMeter_Cnt[AE_HISTMETER_LEVEL];

UINT32 AE_HistMeter_Weight[2][AE_HISTMETER_LEVEL] = {
#if (AE_ANTI_OVEREXPO_LEVEL==AE_ANTI_OVEREXPO_LV0)
{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, // Day light
{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, // Night
#elif (AE_ANTI_OVEREXPO_LEVEL==AE_ANTI_OVEREXPO_LV1)
{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  90,  80,  70,  60}, // Day light
{  30,  50,  75,  90, 100, 100, 100, 100,  90,  70,  48,  24,  24,  20,  20,  20}, // Night
#elif (AE_ANTI_OVEREXPO_LEVEL==AE_ANTI_OVEREXPO_LV2)
{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  80,  64,  52,  40}, // Day light
{  30,  50,  75,  90, 100, 100, 100, 100,  75,  50,  24,  12,  12,  10,  10,  10}, // Night
#elif (AE_ANTI_OVEREXPO_LEVEL==AE_ANTI_OVEREXPO_LV3)
{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  80,  64,  52,  40}, // Day light
{  30,  50,  75,  90, 100, 100, 100, 100,  75,  50,  24,  12,  12,  10,  10,  10}, // Night
#elif (AE_ANTI_OVEREXPO_LEVEL==AE_ANTI_OVEREXPO_LV4)
{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  70,  50,  30,  20,  10}, // Day light
{  30,  50,  75,  90, 100, 100, 100,  80,  64,  32,  16,   8,   8,   5,   5,   5}, // Night
#elif (AE_ANTI_OVEREXPO_LEVEL==AE_ANTI_OVEREXPO_LV5)
{ 100, 100, 100, 100, 100, 100, 100, 100, 100,  80,  50,  35,  25,  15,  10,   5}, // Day light
{  30,  50,  75,  90, 100, 100, 100,  90,  48,  24,  10,   6,   4,   2,   2,   2}, // Night
#endif
};

UINT32 aaa_EstimateY_HistMeter(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset)
{
    UINT16 i, j, level;
    UINT32 Ytotal=0,Wtotal=0,Yavg;
	UINT8 AeHistRatioSel = 0;

    if(AE_Param.EV_Value >= AE_HIST_CHANGE_EV_H)
    {
		AeHistRatioSel = AE_HIST_RATIO_DAYLIGHT;
	}
    else if(AE_Param.EV_Value <= AE_HIST_CHANGE_EV_L)
    {
		AeHistRatioSel = AE_HIST_RATIO_NIGHT;
    }   	

	for(i=0; i<AE_HISTMETER_LEVEL; i++)
		AE_HistMeter_Cnt[i] = 0;
		
    for(j=0;j<AE_HISTMETER_WINDOW_Y;j++)
    {
        for(i=0;i<AE_HISTMETER_WINDOW_X;i++)
        {
            Yavg = aaa_BlockAverageY(YAddr,((i*h_size)/AE_HISTMETER_WINDOW_X),((j*v_size)/AE_HISTMETER_WINDOW_Y),(h_size/AE_HISTMETER_WINDOW_X),(v_size/AE_HISTMETER_WINDOW_Y),offset);
			level = (Yavg/AE_HISTMETER_LEVEL);
            Ytotal += (UINT32)Yavg;
			AE_HistMeter_Cnt[level]++;
        }
    }

	for(i=0; i<AE_HISTMETER_LEVEL; i++)
	{
		Wtotal += ((AE_HistMeter_Cnt[i]*AE_HistMeter_Weight[AeHistRatioSel][i])/100);
	}

#if 0
    SenFP_ClearOSD();
    sprintf(message,"%d, %d, %d, %d", AE_HistMeter_Cnt[0], AE_HistMeter_Cnt[1], AE_HistMeter_Cnt[2], AE_HistMeter_Cnt[3]);
    SenFP_ShowOSDString(message,20,20,4);
    sprintf(message,"%d, %d, %d, %d", AE_HistMeter_Cnt[4], AE_HistMeter_Cnt[5], AE_HistMeter_Cnt[6], AE_HistMeter_Cnt[7]);
    SenFP_ShowOSDString(message,20,50,4);
    sprintf(message,"%d, %d, %d, %d", AE_HistMeter_Cnt[8], AE_HistMeter_Cnt[9], AE_HistMeter_Cnt[10], AE_HistMeter_Cnt[11]);
    SenFP_ShowOSDString(message,20,80,4);
    sprintf(message,"%d, %d, %d, %d", AE_HistMeter_Cnt[12], AE_HistMeter_Cnt[13], AE_HistMeter_Cnt[14], AE_HistMeter_Cnt[15]);
    SenFP_ShowOSDString(message,20,110,4);
    sprintf(message,"%d, %d, %d", Ytotal, Wtotal, (Ytotal/Wtotal));
    SenFP_ShowOSDString(message,20,140,4);
    //sprintf(message,"%d, %d", AE_HIST_CHANGE_EV_L, AE_HIST_CHANGE_EV_H);
    //SenFP_ShowOSDString(message,20,170,4);
#endif

    return (Ytotal/Wtotal);
}
#endif

#define AE_WINDOW_X     8
#define AE_WINDOW_Y     8
#define AE_WINDOW_NUM   (AE_WINDOW_X*AE_WINDOW_Y)
extern STR_AAA_CA_PARA CA_para;
UINT32 aaa_EstimateY(void)
{
    UINT32 i, idx1, idx2;
    UINT32 Y_Data[AE_WINDOW_NUM], Yavg, Yweight, Yval;
    UINT32 R_cal, G_cal, B_cal;
    
    aaa_CAprocess();

    Yavg = 0;
    Yweight = 0;
    for(i=0; i<AE_WINDOW_NUM; i++)
    {
        R_cal = ((CA_para.R_Data[i]*Rgain)>>8);
        G_cal = ((CA_para.G_Data[i]*Ggain)>>8);
        B_cal = ((CA_para.B_Data[i]*Bgain)>>8);

        Y_Data[i] = (UINT32)((R_cal * 77) + (G_cal * 150) + (B_cal * 29)) >> 8;

		if(Y_Data[i] > 255)
			Y_Data[i] = 255;
        
        idx1 = (Y_Data[i]>>1);
        idx2 = idx1+1;

        Yval = (((GammaLUT_128Tab_Video_Normal[idx2]>>2) - (GammaLUT_128Tab_Video_Normal[idx1]>>2))/((idx2<<1) - (idx1<<1)))*(Y_Data[i]-(idx1<<1))+(GammaLUT_128Tab_Video_Normal[idx1]>>2);
        //Yval = Y_Data[i];

        Yavg += (Yval * AEWTable[i>>3][i&0x07]);
        Yweight += AEWTable[i>>3][i&0x07];
    }

    return (Yavg/Yweight);
}

void aaa_AEprocess(void)
{
    UINT32 OB_AVG_R, OB_AVG_Gr, OB_AVG_Gb, OB_AVG_B;
    UINT32 RegVal_H, RegVal_L, RegVal;
    PImageAlgInfo pimginfo;
    
    pimginfo = UI_GetParameter();
    GetActImgFBAddr_Path1(&FB_para1);

    gAEcounter++;//=Get_SieFrameCounter();

    bDynaGammaTrigger = TRUE;

    AE_CONTROL(pimginfo);

#if _AE_HIST_METER_ENABLE
    YAverage=aaa_EstimateY_HistMeter(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
#else

#if DYNAMIC_GAMMA_EN
	YAverage = aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    YAverage = aaa_EstimateY();
#else
    YAverage=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
#endif

#endif
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
                        AE_Param.AePgaGain_int,
                        AE_Param.ExpoTime,
                        AE_Debug.AE_AddY,
                        AE_Debug.AE_ExpectY,
                        AE_Param.ExpoLine,
                        nAE_Maxline,
                        AE_Param.StableCnt));
            debug_err(("----------------------------\r\n"));
            gAEcounter=0;
        }

    }
    
    AE_Counter++;
    //prevTime = Perf_GetCurrent();

    if(AE_OsdMsg)
    {
        SenFP_ClearOSD();

#if 0
        sprintf(message,"%d", ((prevTime-currTime)/1000));
        SenFP_ShowOSDString(message,20,20,4);
#endif

#if 0
        sprintf(message,"%d", gAlgMsg.Alg_Mode);
        SenFP_ShowOSDString(message,20,20,4);
#endif

#if 1
        sprintf(message,"%d, %d, %d", YAverage, AE_Param.EV_Value, nMin_EV_Value);
        SenFP_ShowOSDString(message,20,20,4);
        sprintf(message,"%d, %d", AE_Param.ExpoTime, AE_Param.ExpoLine);
        SenFP_ShowOSDString(message,20,50,4);
        sprintf(message,"%d, %02X", AE_Param.ISO_Gain, Sensor_ReadReg(0x301D));
        SenFP_ShowOSDString(message,20,80,4);
#endif

#if 0
        sprintf(message,"%d, %d, %d", IQ_GammaLUT[0], IQ_GammaLUT[1], IQ_GammaLUT[2]);
        SenFP_ShowOSDString(message,20,20,4);
#endif

#if 0
        sprintf(message,"%d, %d, %d", YAverage, AE_Debug.AE_ExpectY, AE_Param.EV_Value);
        SenFP_ShowOSDString(message,20,20,4);
        sprintf(message,"%d, %d", AE_Debug.AE_ExpectY, HighCnt);
		SenFP_ShowOSDString(message,20,50,4);
        sprintf(message,"%d, %d", Add_Y, Add_Step);
		SenFP_ShowOSDString(message,20,80,4);
#endif        

#if 0
        sprintf(message,"%d, %d, %d", YAverage, AE_Debug.AE_ExpectY, AE_Param.EV_Value);
        SenFP_ShowOSDString(message,20,20,4);
        sprintf(message,"%d, %d, %d", AE_Param.ExpoTime, AE_Param.ISO_Gain, AE_Param.AePgaGain_int);
        SenFP_ShowOSDString(message,20,50,4);
		sprintf(message,"%d, %d", nMin_EV_Value, Add_Y);
        SenFP_ShowOSDString(message,20,80,4);
#endif        

#if 0
        sprintf(message,"%02X, %02X", Sensor_ReadReg(0x307E), Sensor_ReadReg(0x307F));
        SenFP_ShowOSDString(message,20,20,4);
#endif        

#if 0
        sprintf(message,"%d, %d, %d", YAverage, AE_Debug.AE_ExpectY, AE_Debug.AE_AddY);
        SenFP_ShowOSDString(message,20,20,4);
        sprintf(message,"%d, %d, %d", AE_Param.EV_Value, Last_AEStatus, Add_Step);
        SenFP_ShowOSDString(message,20,50,4);                
        sprintf(message,"%d", HighCnt);
        SenFP_ShowOSDString(message,20,80,4);                                
        sprintf(message,"%d, %d, %d, %d", Hist[63], Hist[62], Hist[61], Hist[60]);
        SenFP_ShowOSDString(message,20,110,4);
        sprintf(message,"%d, %d, %d, %d", Hist[59], Hist[58], Hist[57], Hist[56]);
        SenFP_ShowOSDString(message,20,140,4);
#endif
    }

    SenFlowFunc(IPE_PRV_AE_SETTING, &gSenCBInfo);
    SenFlowFunc(IME_Chrom_Prv_AE_SETTING, &gSenCBInfo);

    // For avoid AE flash every 30 second
    if(AE_Counter > 250)
       AE_Counter = 250; 
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
    //EV_Value = (EV_Value*3+AE_Param.EV_Value)/4;

    if(EV_Value>=EV_Value_MAX)
        EV_Value=EV_Value_MAX;
    else
    {
        if(EV_Value <= nMin_EV_Value)
        {
           EV_Value = nMin_EV_Value;
        }
    }
    AE_Param.EV_Value=EV_Value;
    Allot_AeSet(EV_Value);
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
	Ae_SetPgaGain();
	Ae_SetExpoTime();
	Sensor_WriteReg(0x3060, 0x02);
}

void AeSet_isr(void)
{
    SenFlowFunc(AE_PRV_GAIN_SETTING, &gSenCBInfo);
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
    Allot_AeSet(AE_Param.EV_Value);
    //#NT#2010/05/10#ethanlau -end
    AE_Param.ExpoLine = AeExpoTime2TGValue(AE_Param.ExpoTime,GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M);
    AeISOGain2TGValue(AE_Param.ISO_Gain);
    SetPreExpLine(AE_Param.ExpoLine);
    SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
}


void SceneMode_AE(UINT8 scene)
{
#if 1
    if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)
        AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
    else
        AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;

    nMin_EV_Value = EV_Value_MIN;
#else
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
#endif
}

void AePrv2Cap(void)
{
#if 0
    SceneMode_AE(gImageAlgInfo.SceneMode);
    Allot_AeSet(AE_Param.EV_Value);
#else
    UINT32 CapISO=0,CapTime=0,_preiso,_preexpt,_preexpl;
    UINT16 TargetY, OriY_Pregam, TarY_Pregam;
    UINT32 nPrev_Expline, nCap_Expline, ShutterOffset,  ExpoTime,tmp,_ratio;
    INT32 GR_rst_end, GR_shutter_start, GR_read_start,ShutterTime;
    UINT32 weight = 0;
    PImageAlgInfo pimginfo;
    Cal_Struct *CalInfo;
    CalInfo = Cal_GetCalibrationData();
    pimginfo = UI_GetParameter();
    _preiso = AE_Param.ISO_Gain;
    _preexpt = AE_Param.ExpoTime;
    _preexpl = AE_Param.ExpoLine;

    //debug_err(("AePrv2Cap...%d, %d\r\n", GRS_MODE, Flash_Param.FlshEna));

#if 1
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

        //debug_err(("AePrv2Cap ShutterOffset=%d\r\n", ShutterOffset));

        GR_rst_end=0x600;//0x34c;//844;
        GR_read_start=ExpoTime*SENSOR_CAP_PCLK_M*2/512+GR_rst_end;//internal clock=PCLK*2
        //Set exposure setting

        if(GR_read_start>0xFFFF)
            GR_read_start=0xFFFF;

        if(GR_shutter_start>0xFFFF)
            GR_shutter_start=0xFFFF;
    }
    else//ERS AePrv2Cap
    {
        //debug_err(("ERS---%d\r\n", gImageAlgInfo.ISO));
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

            //CapTime = AE_Param.ExpoTime*GetIPPSizeInfor(_Binning);
            CapTime = AE_Param.ExpoTime;
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

            debug_err(("Cap Time, ISO = %d, %d\r\n", CapTime, CapISO));
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
            /*
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
            */

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
#if 0
        debug_err(("AePrv2Cap iso=%d capiso=%d\r\n",AE_Param.ISO_Gain,AE_Param.ISO_Gain_Cap));
        debug_err(("AePrv2Cap preT=%d capT=%d\r\n",AE_Param.ExpoTime,AE_Param.ExpoTime_Cap));
        debug_err(("AePrv2Cap preL=%d capL=%d\r\n",AE_Param.ExpoLine,AE_Param.ExpoLine_Cap));
#endif
    }
#endif
#endif
}
//#NT#20090708#ETHAN LAU -end

//#NT#20090706#ETHAN LAU -begin
void Ae_Transform(UINT32 para_num,...)
{
    //debug_msg("AE_Param.ExpoTime=%d,GetIPPSizeInfor(_HDTotal_Preview)=%d,SENSOR_PRE_PCLK_M=%d,nAE_Maxline=%d\r\n",AE_Param.ExpoTime,GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M,nAE_Maxline);
    AE_Param.ExpoLine = AeExpoTime2TGValue(AE_Param.ExpoTime,GetIPPSizeInfor(_VDTotal_Preview),GetIPPSizeInfor(_HDTotal_Preview),SENSOR_PRE_PCLK_M);
    AeISOGain2TGValue(AE_Param.ISO_Gain);
    AE_ChangeRate(AE_RATE_NORMAL);

    SetPreExpLine(AE_Param.ExpoLine);
    Ae_SetPgaGain();
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
    UINT32 Gain_factor, Calcul_AGC_Ratio;
    UINT16 shf;

    Calcul_AGC_Ratio = (UINT32)(((UINT32)100*(UINT32)ISO_Gain)/(UINT32)ISO_Gain_Min);

    shf = 0;

    while(Calcul_AGC_Ratio/((1<<(shf+1))*100))
        shf++;

    Gain_factor = ((Calcul_AGC_Ratio-((1<<shf)*100))*100)/(GAIN_STEP<<shf) + AGC_ISOMin + AGC_GAP*shf;

    if (Gain_factor > 0xff)
    {
        Gain_factor = 0xff;
    }
    AE_Param.AePgaGain_int = (unsigned int)Gain_factor;
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
    UINT32 Y_Diff;
    UINT32 OverExposureTh_H, OverExposureTh_L;
	
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
        if(AE_Param.EV_Value > OverExpo_Outdoor_EV)// Lv11
        {
            OverExpo_Max_AddY = OverExpo_Max_AddY_Outdoor;
            OverExposureTh_H = OverExpo_Outdoor_Th_H;
            OverExposureTh_L = OverExpo_Outdoor_Th_L;
        }
#if !_AE_REDUCE_OVER_EXPO_LHC
        else if(AE_Param.EV_Value <= OverExpo_Indoor_EV)
        {
            OverExpo_Max_AddY = OverExpo_Max_AddY_Indoor;
            OverExposureTh_H = OverExpo_Indoor_Th_H;
            OverExposureTh_L = OverExpo_Indoor_Th_L;
        }        
#else
        else if(AE_Param.EV_Value <= OverExpo_Lowlight_EV)
        {
            OverExpo_Max_AddY = OverExpo_Max_AddY_Lowlight;
            OverExposureTh_H = OverExpo_Lowlight_Th_H;
            OverExposureTh_L = OverExpo_Lowlight_Th_L;
        }
#endif        
        else
        {
            OverExpo_Max_AddY = Last_OverExpo_Max_AddY;
            OverExposureTh_H = Last_OverExpo_Th_H;
            OverExposureTh_L = Last_OverExpo_Th_L;
        }

        Last_OverExpo_Max_AddY = OverExpo_Max_AddY;
        Last_OverExpo_Th_H = OverExposureTh_H;
        Last_OverExpo_Th_L = OverExposureTh_L;
			
            HighCnt=(Hist[60]+Hist[61]+Hist[62]+Hist[63])
                +(Hist[56]+Hist[57]+Hist[58]+Hist[59])
                +(Hist[52]+Hist[53]+Hist[54]+Hist[55])/2;// >224  [224~239]+[240~225

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
	        else if(HighCnt>OverExposureTh_H)
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
            else if(HighCnt<OverExposureTh_L)
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

#if 1
    if(pimginfo->AEEVValue != _IMGEVValue_00)
    {
        ExpectY = (NormalExpectY*EVCompTable[gImageAlgInfo.AEEVValue - 4])/100;
    }
    else
    {
        ExpectY = NormalExpectY;
    }

    if((ExpectY>Add_Y)&&(AE_Counter > 3))
        ExpectY-=Add_Y;
        
#else
    ExpectY = NormalExpectY;
#endif

    Ratio = (YAvg * 100) / ExpectY;

    if(Ratio<30)
        Ratio = 30;
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
#if 1
    aaa_SetAeRate(3);
#else
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
            aaa_SetAeRate(4);
            break;
    }
#endif
}


void Allot_AeSet(UINT32 CurrentEV)
{
    UINT8 i;
    UINT32 Calcul_Time, Calcul_Gain;
    UINT32 FnRatio;
	
    //FnRatio = Ae_IrisZoomStep0[0];
    FnRatio = F_Number;

    // Search range
    for(i=0; i<20; i++)
    {
        //if((CurrentEV*100/FnRatio)>=AeCurve[i][AE_EV])
        if((CurrentEV)>=AeCurve[i][AE_EV])
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

    if(Calcul_Gain > ISO_Gain_Max)
    {
        Calcul_Gain = ISO_Gain_Max;
    }
    else if(Calcul_Gain < ISO_Gain_Min)
    {
        Calcul_Gain = ISO_Gain_Min;
    }
/*
    if(Calcul_Time < 250)
    {
        Calcul_Time = 250;
    }
*/
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
#endif

