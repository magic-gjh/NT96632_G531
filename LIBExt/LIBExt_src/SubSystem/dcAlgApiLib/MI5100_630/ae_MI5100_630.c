#include "sensor_model.h"
//#NT#20090706#ETHAN LAU -begin
#include <stdarg.h>
//#NT#20090706#ETHAN LAU -end
#if MI5100_630
#include "sif.h"
#include "uart.h"
#include "pll.h"
#include "Fd_lib.h"

extern UINT32 uiAE_TEST;
extern UINT32 uiAeIsoGain;
extern UINT32 uiAeExpoTime;

#define EN_15FPS 1

void aaa_GetCenterHist(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset, UINT32* Histogram);
UINT32 CenterHighCnt, CenterHist[64];
BOOL AE_UartMsg = FALSE;
BOOL AE_OsdMsg = FALSE;
extern UINT gpuiFbAdd[3];

//#NT#2009/11/14#Jarkko Chang -begin
UINT32 SENSOR_PRE_PCLK = 10;
UINT32 SENSOR_CAP_PCLK = 10;
//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
UINT32 SENSOR_PRE_PCLK_M = 960;//680;//(SENSOR_PRE_PCLK/1000000);
UINT32 SENSOR_CAP_PCLK_M = 960;//680;//(SENSOR_CAP_PCLK/1000000);
//#NT#2010/06/10#Jarkko Chang -end
//#NT#2009/11/14#Jarkko Chang -end

UINT EV=0,Old_EV=0;
UINT32 YAverage;
UINT32 gFlagCap=0,gAEcounter=0,gAELock=0;
UINT32 Hist[64];
UINT32 tmp=0;
UINT32 AE_Stable_Cnt=0;
UINT32 AE_Idx=0,g_Idx_f=0,g_Idx_n=0;
UINT FIXISO_mode = 0;
BOOL fixiso = FALSE;

UINT32 LogTbl[10] = {107,115,123,132,141,152,162,174,187,200};
UINT32 nMin_EV_Value = EV_Value_MIN;
UINT32 Last_EV=10,Damping_EV=10;
UINT32 Real_EV=10;
UINT32 nAE_Maxline;
INT32 LV=0;

UINT16 AE_Flag=0;
UINT16 Add_Y,Add_Step = Default_AddStep;
UINT16  Last_OverExpo_Max_AddY = OverExpo_Max_AddY_Indoor;
UINT16 EVCompTable[13] = {25,33,42,53,67,83,100,118,136,151,166,178,190};
INT16 NormalY_adj=0;
//UINT16 Ae_IrisZoomStep0[2] = {80, 320};

UINT8 Last_AEStatus,Damping;
UINT8 AE_Counter= 0;
UINT8 CurrentIdx;
volatile UINT8 AGC_DelayCnt = 0;
UINT8 nPreAGC_int=0x3;
UINT8 nPreAGC_fra=0x7F;

BOOL bAE_LowLightMode = FALSE;
BOOL gGR_ERS4LongExp=FALSE;

AE_STRUCT AE_Param;
AE_Debug_msg AE_Debug;
UINT32 Expo_CompensateRatio = 100;
extern volatile BOOL GRS_MODE;
//extern UINT32 ShutterCloseLeadTime;

//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
//#NT#20090703#ETHAN LAU -BEGIN
UINT32 BiningMode = _2xBining;
void SetAeBining(UINT32 Mode);
//#NT#20090703#ETHAN LAU -END
//#NT#2010/06/10#Jarkko Chang -end

const UINT8 AEWTable[8][8]={
        {1,1,1, 1, 1,1,1,1},
        {1,2,2, 2, 2,2,2,1},
        {1,2,3, 3, 3,3,2,1},
        {1,2,3,10,10,3,2,1},
        {1,2,4,10,10,4,2,1},
        {1,2,3, 4, 4,3,2,1},
        {1,2,2, 3, 3,2,2,1},
        {1,1,1, 1, 1,1,1,1},
        };
#if 1
const UINT8 AE_Window_CenterWeighted[8][8]={
#if 0
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 3, 6, 6, 3, 2, 1},
        {1, 2, 5,10,10, 5, 2, 1},
        {1, 2, 5,10,10, 5, 2, 1},
        {1, 2, 4, 6, 6, 4, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        };
#elif 1
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        };
#elif 1
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 3, 3, 3, 3, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
        {1, 2, 3, 3, 3, 3, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        };
#endif
#else
const UINT8 AE_Window_CenterWeighted[8][8]={
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 3, 5, 5, 3, 2, 1},
        {1, 2, 5, 8, 8, 5, 2, 1},
        {1, 2, 5, 8, 8, 5, 2, 1},
        {1, 2, 4, 6, 6, 4, 2, 1},
        {1, 2, 3, 4, 4, 3, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        };
#endif
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

const UINT8 AE_Window_Multi[8][8]={
    {3,3,2,1,1,2,3,3},
    {3,3,2,2,2,2,3,3},
    {2,2,3,3,3,3,2,2},
    {1,2,3,4,4,3,2,1},
    {1,2,3,4,4,3,2,1},
    {2,2,3,3,3,3,2,2},
    {3,3,2,2,2,2,3,3},
    {3,3,2,1,1,2,3,3}
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

#if EN_15FPS
//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
UINT32 AeCurve_AntiFlicker60[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{10240,    16666,        50,    0,          AE_ADJ_EXP}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN}, // 1/60
{2560,     33333,       100,    0,          AE_ADJ_GAIN}, // 1/30
{1280,     33333,       200,    0,          AE_ADJ_GAIN}, // 1/30
{640,      66666,       200,    0,          AE_ADJ_GAIN}, // 1/30
{160,     133332,       400,    0,          AE_ADJ_GAIN}, // 1/15
{  0,     133332,       800,    0,          AE_ADJ_GAIN}, // 1/15
};

UINT32 AeCurve_AntiFlicker60_vga[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{10240,    16666,        50,    0,          AE_ADJ_EXP}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN}, // 1/60
{2560,     16666,       200,    0,          AE_ADJ_GAIN}, // 1/30
{1280,     16666,       400,    0,          AE_ADJ_GAIN}, // 1/30
{640,      33333,       400,    0,          AE_ADJ_GAIN}, // 1/30
{320,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
{  0,      66666,       800,    0,          AE_ADJ_GAIN}, // 1/15
};

UINT32 AeCurve_AntiFlicker50[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{8533,     20000,        50,    0,          AE_ADJ_EXP}, // 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      80000,       200,    0,          AE_ADJ_GAIN},// 1/25
{133,     160000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,     160000,       800,    0,          AE_ADJ_GAIN},// 1/12.5
};

UINT32 AeCurve_AntiFlicker50_vga[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{8533,     20000,        50,    0,          AE_ADJ_EXP}, // 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/25
{1066,     20000,       400,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};
#else
//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
UINT32 AeCurve_AntiFlicker60[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
//{20480,     8333,        50,    0,          AE_ADJ_EXP},  // 1/120
{10240,    16666,        50,    0,          AE_ADJ_EXP}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN}, // 1/60
{2560,     33333,       100,    0,          AE_ADJ_GAIN}, // 1/30
{1280,     33333,       200,    0,          AE_ADJ_GAIN}, // 1/30
{640,      66666,       200,    0,          AE_ADJ_GAIN}, // 1/30
{320,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
{  0,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
};

UINT32 AeCurve_AntiFlicker60_vga[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
//{20480,     8333,        50,    0,          AE_ADJ_EXP},  // 1/120
{10240,    16666,        50,    0,          AE_ADJ_EXP}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN}, // 1/60
{2560,     16666,       200,    0,          AE_ADJ_GAIN}, // 1/30
{1280,     16666,       400,    0,          AE_ADJ_GAIN}, // 1/30
{640,      33333,       400,    0,          AE_ADJ_GAIN}, // 1/30
{320,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
{  0,      66666,       400,    0,          AE_ADJ_GAIN}, // 1/15
};

UINT32 AeCurve_AntiFlicker50[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
//{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     20000,        50,    0,          AE_ADJ_EXP}, // 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      80000,       200,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};

UINT32 AeCurve_AntiFlicker50_vga[8][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
//{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     20000,        50,    0,          AE_ADJ_EXP}, // 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/25
{1066,     20000,       400,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};
//#NT#2010/06/10#Jarkko Chang -end
#endif

//Anti-flicker:60Hz
#if 0
//ISO100
UINT AeCurve_AntiFlicker60[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{10240,     8333,       100,    0,          AE_ADJ_EXP}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/15
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/3.75
{160,     533333,       100,    0,          AE_ADJ_GAIN},// 1/1.875
{0,       533333,       100,    0,          AE_ADJ_GAIN},// 1/1.875
};
#elif 0
//ISO200
UINT AeCurve_AntiFlicker60[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{10240,     4166,       200,    0,          AE_ADJ_EXP}, // 1/120
{5120,      8333,       200,    0,          AE_ADJ_GAIN},// 1/60
{2560,     16666,       200,    0,          AE_ADJ_GAIN},// 1/30
{1280,     33333,       200,    0,          AE_ADJ_GAIN},// 1/15
{640,      66666,       200,    0,          AE_ADJ_GAIN},// 1/7.5
{320,     133333,       200,    0,          AE_ADJ_GAIN},// 1/3.75
{160,     266666,       200,    0,          AE_ADJ_GAIN},// 1/1.875
{0,       533333,       200,    0,          AE_ADJ_GAIN},// 1/1.875
};
#elif 0
//ISO400
UINT AeCurve_AntiFlicker60[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{10240,     2083,       400,    0,          AE_ADJ_EXP}, // 1/120
{5120,      4166,       400,    0,          AE_ADJ_GAIN},// 1/60
{2560,      8333,       400,    0,          AE_ADJ_GAIN},// 1/30
{1280,     16666,       400,    0,          AE_ADJ_GAIN},// 1/15
{640,      33333,       400,    0,          AE_ADJ_GAIN},// 1/7.5
{320,      66666,       400,    0,          AE_ADJ_GAIN},// 1/3.75
{160,     133333,       400,    0,          AE_ADJ_GAIN},// 1/1.875
{0,       266666,       400,    0,          AE_ADJ_GAIN},// 1/1.875
};
#elif 0
//ISO adjust
  //for iso using   lv = 9, exp = 1/30, for adjust iso50
UINT32 AeCurve_AntiFlicker60[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_GAIN},
{40960,    33333,        50,    0,          AE_ADJ_GAIN}, // 1/120
{20480,    33333,       100,    0,          AE_ADJ_GAIN}, // 1/120
{10240,    33333,       200,    0,          AE_ADJ_GAIN},// 1/120
{5120,     33333,       400,    0,          AE_ADJ_GAIN},// 1/120
{2560,     33333,       800,    0,          AE_ADJ_GAIN},// 1/60
{1280,     33333,       800,    0,          AE_ADJ_GAIN},// 1/30
{640,      33333,       800,    0,          AE_ADJ_GAIN},// 1/30
{  0,      33333,       800,    0,          AE_ADJ_GAIN},// 1/15
};
#endif

UINT AeCurve_ISO50_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP},  // 1/120
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/120
{5120,     16666,       100,    0,          AE_ADJ_GAIN},// 1/60
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30
{1280,     66666,       100,    0,          AE_ADJ_GAIN},// 1/15
{640,     133333,       100,    0,          AE_ADJ_GAIN},// 1/7.5
{320,     266666,       100,    0,          AE_ADJ_GAIN},// 1/3.75
{160,     533333,       100,    0,          AE_ADJ_GAIN},// 1/1.875
{0,       533333,       100,    0,          AE_ADJ_GAIN},// 1/1.875
};

UINT AeCurve_ISO50_50Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     20000,       100,    0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,    0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,    0,          AE_ADJ_GAIN},// 1/12.5
{533,     160000,       100,    0,          AE_ADJ_GAIN},// 1/6.25
{266,     320000,       100,    0,          AE_ADJ_GAIN},// 1/3.125
{133,     640000,       100,    0,          AE_ADJ_GAIN},// 1/1.5625
{0,       640000,       100,    0,          AE_ADJ_GAIN},// 1/1.5625
};

UINT AeCurve_ISO100_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,  0,          AE_ADJ_EXP},
{20480,     8333,        50,  0,          AE_ADJ_EXP},  // 1/120
{10240,     8333,       100,  0,          AE_ADJ_GAIN}, // 1/120
{5120,     16666,       100,  0,          AE_ADJ_GAIN},// 1/60
{2560,     33333,       100,  0,          AE_ADJ_GAIN},// 1/30
{1280,     66666,       100,  0,          AE_ADJ_GAIN},// 1/15
{640,     133333,       100,  0,          AE_ADJ_GAIN},// 1/7.5
{320,     266667,       100,  0,          AE_ADJ_GAIN},// 1/7.5
{160,     533333,       100,  0,          AE_ADJ_GAIN},// 1/7.5
{0,       533333,       100,  0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO100_50Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,  0,          AE_ADJ_EXP},
{17066,    10000,        50,  0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,  0,          AE_ADJ_GAIN}, // 1/100
{4266,     20000,       100,  0,          AE_ADJ_GAIN},// 1/50
{2133,     40000,       100,  0,          AE_ADJ_GAIN},// 1/25
{1066,     80000,       100,  0,          AE_ADJ_GAIN},// 1/12.5
{533,     160000,       100,  0,          AE_ADJ_GAIN},// 1/6.25
{266,     320000,       100,  0,          AE_ADJ_GAIN},// 1/6.25
{133,     640000,       100,  0,          AE_ADJ_GAIN},// 1/6.25
{0,       640000,       100,  0,          AE_ADJ_GAIN},// 1/6.25
};

UINT AeCurve_ISO200_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,        0,       50,   0,          AE_ADJ_EXP},
{20480,     8333,        50,  0,          AE_ADJ_EXP},  // 1/120
{10240,     8333,       100,  0,          AE_ADJ_GAIN}, // 1/240
{5120,      8333,       200,  0,          AE_ADJ_GAIN},// 1/120
{2560,     16666,       200,  0,          AE_ADJ_GAIN},// 1/60
{1280,     33333,       200,  0,          AE_ADJ_GAIN},// 1/30
{640,      66666,       200,  0,          AE_ADJ_GAIN},// 1/15
{320,     133333,       200,  0,          AE_ADJ_GAIN},// 1/7.5
{160,     266667,       200,  0,          AE_ADJ_GAIN},// 1/7.5
{0,         266667,       200,  0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO200_50Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,  0,          AE_ADJ_EXP},
{17066,    10000,        50,  0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,  0,          AE_ADJ_GAIN}, // 1/200
{4266,     10000,       200,  0,          AE_ADJ_GAIN},// 1/100
{2133,     20000,       200,  0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,  0,          AE_ADJ_GAIN},// 1/25
{533,      80000,       200,  0,          AE_ADJ_GAIN},// 1/12.5
{266,     160000,       200,  0,          AE_ADJ_GAIN},// 1/6.25
{133,     320000,       200,  0,          AE_ADJ_GAIN},// 1/6.25
{0,       320000,       200,  0,          AE_ADJ_GAIN},// 1/6.25
};

UINT AeCurve_ISO400_60Hz[11][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,  0,          AE_ADJ_EXP},
{40960,     4166,        50,  0,          AE_ADJ_EXP}, // 1/960
{20480,     4166,       100,  0,          AE_ADJ_GAIN}, // 1/960
{10240,     4166,       200,  0,          AE_ADJ_GAIN}, // 1/480
{5120,      4166,       400,  0,          AE_ADJ_GAIN},// 1/240
{2560,      8333,       400,  0,          AE_ADJ_GAIN},// 1/120
{1280,     16666,       400,  0,          AE_ADJ_GAIN},// 1/60
{640,      33333,       400,  0,          AE_ADJ_GAIN},// 1/30
{320,      66666,       400,  0,          AE_ADJ_GAIN},// 1/15
{160,     133333,       400,  0,          AE_ADJ_GAIN},// 1/7.5
{0,       133333,       400,  0,          AE_ADJ_GAIN},// 1/7.5
};

UINT AeCurve_ISO400_50Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,        0,       50,   0,          AE_ADJ_EXP},
{17066,    10000,        50,  0,          AE_ADJ_EXP}, // 1/100
{8533,      10000,       100,  0,          AE_ADJ_GAIN}, // 1/400
{4266,      10000,       200,  0,          AE_ADJ_GAIN},// 1/200
{2133,     10000,       400,  0,          AE_ADJ_GAIN},// 1/100
{1066,     20000,       400,  0,          AE_ADJ_GAIN},// 1/50
{533,      40000,       400,  0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,  0,          AE_ADJ_GAIN},// 1/12.5
{133,     160000,       400,  0,          AE_ADJ_GAIN},// 1/6.25
{0,         160000,       400,  0,          AE_ADJ_GAIN},// 1/6.25
};

UINT32 AeCurve_Landscape_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,     0,         AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{5120,     16666,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{1280,     33333,       200,    0,          AE_ADJ_GAIN},//
{640,      33333,       400,    0,          AE_ADJ_GAIN},//
{320,      66666,       400,    0,          AE_ADJ_GAIN},// 1/15*2
{160,      66666,       800,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     133333,       800,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_Landscape_50Hz[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};

UINT32 AeCurve_NightLandscape_60Hz[11][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{5120,     16666,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN}, // 1/60*2
{1280,     33333,       200,    0,          AE_ADJ_GAIN},//
{640,      33333,       200,    0,          AE_ADJ_GAIN},//
{320,      66666,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     133333,       200,    0,          AE_ADJ_GAIN},// 1/7.5*2
{ 80,     133333,       400,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     266666,       400,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_NightLandscape_50Hz[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};


UINT32 AeCurve_Action_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,     8333,       100,    0,          AE_ADJ_GAIN},//
{5120,      8333,       200,    0,          AE_ADJ_GAIN},//
{2560,      8333,       400,    0,          AE_ADJ_GAIN},// 1/30*2
{1280,     16666,       400,    0,          AE_ADJ_GAIN},// 1/15*2
{640,      33333,       400,    0,          AE_ADJ_GAIN},//
{320,      33333,       800,    0,          AE_ADJ_GAIN},// 1/15*2
{160,      66666,       800,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     133333,       800,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_Action_50Hz[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{2133,     10000,       400,    0,          AE_ADJ_GAIN},// 1/50
{1066,     20000,       400,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};

UINT32 AeCurve_Potrait_60Hz[10][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,    16666,        50,    0,          AE_ADJ_GAIN}, // 1/60*2
{5120,     33333,        50,    0,          AE_ADJ_GAIN},// 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30*2
{1280,     33333,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{640,      33333,       400,    0,          AE_ADJ_GAIN},// 1/15*2
{320,      33333,       800,    0,          AE_ADJ_GAIN},// 1/15*2
{160,      66666,       800,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     133333,       800,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_Potrait_50Hz[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};

UINT32 AeCurve_NightPotrait_60Hz[12][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{20480,     8333,        50,    0,          AE_ADJ_EXP}, // 1/60*2
{10240,    16666,        50,    0,          AE_ADJ_GAIN}, // 1/60*2
{5120,     33333,        50,    0,          AE_ADJ_GAIN},// 1/60*2
{2560,     33333,       100,    0,          AE_ADJ_GAIN},// 1/30*2
{1280,     33333,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{640,      66666,       200,    0,          AE_ADJ_GAIN},// 1/15*2
{320,     133333,       400,    0,          AE_ADJ_GAIN},// 1/15*2
{160,     266666,       400,    0,          AE_ADJ_GAIN},// 1/7.5*2
{ 80,     533333,       400,    0,          AE_ADJ_GAIN},// 1/7.5*2
{  0,     533333*2,     400,    0,          AE_ADJ_GAIN},// 1/7.5*2
};

UINT32 AeCurve_NightPotrait_50Hz[9][5]={
// EV       ExpoTime    ISO     IrisStep
{3000000,      0,        50,    0,          AE_ADJ_EXP},
{17066,    10000,        50,    0,          AE_ADJ_EXP}, // 1/100
{8533,     10000,       100,    0,          AE_ADJ_GAIN}, // 1/100
{4266,     10000,       200,    0,          AE_ADJ_GAIN},// 1/100
{2133,     20000,       200,    0,          AE_ADJ_GAIN},// 1/50
{1066,     40000,       200,    0,          AE_ADJ_GAIN},// 1/25
{533,      40000,       400,    0,          AE_ADJ_GAIN},// 1/25
{266,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
{  0,      80000,       400,    0,          AE_ADJ_GAIN},// 1/12.5
};
//#NT#2009/11/14#Jarkko Chang -end
UINT32 (*AeCurve)[5] = (UINT32(*)[5])AeCurve_AntiFlicker60;


UINT diffp, diffn, np, nn, contrast,CenAvg;
UINT16 ISO_Prv;
UINT CapExpLine_befConv;
UINT HighCnt;
UINT LowCnt;
BOOL f_HalfFrmExpDet=0; //Half frame exposure detection only
BOOL AE_FirstRun = FALSE;


INT32 InterpolateValue(UINT32 Index, UINT32 LowValue, UINT32 HighValue, UINT32 MinIndex, UINT32 MaxIndex)
{
    if ( Index < MinIndex)
        return LowValue;
    else if ( Index > MaxIndex)
        return HighValue;
    else
        return LowValue + ((INT32)((INT32)HighValue - (INT32)LowValue)) * ((INT32)(Index  - MinIndex)) / ((INT32)(MaxIndex - MinIndex));
}


void AEMode_AE(UINT8 nMode)
{
    static UINT8 nPreModeSetting = 0xFF;

    if(nMode == nPreModeSetting)
    return;

    //Assign AE table
    switch(nMode)
    {
        case _IMGAEMode_CenterWeighted:
//            aaa_SetAeWindow((UB*)AE_Window_CenterWeighted);
            aaa_SetAeWindow((UB*)AE_Window_Multi);
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
//#NT#2010/02/02#Jarkko Chang -begin
//#NT#add for fd ae
        case _IMGAEMode_FaceDetection:
            aaa_SetAeWindow((UB*)AE_Window_Adaptive);
            break;
//#NT#2010/02/02#Jarkko Chang -end
        default:
            aaa_SetAeWindow((UB*)AEWTable);
            break;
    }

    nPreModeSetting = nMode;
//    ResetAE();
}

#if 1
void ISO_AE(UINT8 ISO_Speed)
{
//#NT#2009/10/28#Jarkko Chang -begin
//#NT#add for fixed iso
    if(ISO_Speed == _IMGISO_Auto)
    {
        fixiso = FALSE;
        return;
    }
    switch(ISO_Speed)
    {
        case _IMGISO_100:
            AE_FIXISO(100);
            break;
        case _IMGISO_200:
            AE_FIXISO(200);
            break;
        case _IMGISO_400:
            AE_FIXISO(400);
            break;
        case _IMGISO_800:
            AE_FIXISO(800);
            break;
        case _IMGISO_1600:
            AE_FIXISO(1600);
            break;
        case _IMGISO_3200:
            AE_FIXISO(3200);
            break;
        default:
            AE_FIXISO(100);
            break;
    }
//#NT#2009/10/28#Jarkko Chang -end
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

//#NT#20090710#ETHAN LAU -BEGIN
void AE_FIXISO(UINT mode)
{
    FIXISO_mode = mode;
    fixiso =TRUE;
}
//#NT#20090710#ETHAN LAU -end
//#NT#2009/10/28#Jarkko Chang -begin
//#NT#modify for fixed iso setting
void AntiFlickerISO_AE(UINT8 nISO, UINT8 nFlicker)
{

    static UINT8 nPreFlickerSetting = 0xFF;
    static UINT8 nPreISOSetting = 0xFF;
    static UINT nVideoResolution = 0xFF;

//    if((nFlicker == nPreFlickerSetting)&&(nISO == nPreISOSetting)&&(nVideoResolution == GetIPPSizeInfor(_Vid_ResolutionIdx)))
//        return;
    //debug_err(("nISO=%d, nFlicker=%d\r\n",nISO,nFlicker));
    //debug_err(("nPreISOSetting=%d, nPreFlickerSetting=%d\r\n",nPreISOSetting,nPreFlickerSetting));
    nVideoResolution = GetIPPSizeInfor(_Vid_ResolutionIdx);

    if(nFlicker==_POWERFreq_50Hz)
    {
        switch(nISO)
        {
        nMin_EV_Value = EV_Value_MIN;
        case _IMGISO_Auto:
        case _IMGISO_100:
        case _IMGISO_200:
        case _IMGISO_400:
        case _IMGISO_800:
        case _IMGISO_1600:
        case _IMGISO_3200:
            if ((IPL_GetNextMode() == IPL_MODE_VIDEO) || (IPL_GetNextMode() == IPL_MODE_VIDEOREC))
            {
                if ( (nVideoResolution ==  _Resolution_320x240) || (nVideoResolution ==  _Resolution_640x480))
                {
                    AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50_vga;
                }
                else
                {
                    AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
                }
            }
            else
            {
                AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
            }
        break;
/*
            case _IMGISO_50:
                AeCurve = (UINT32(*)[5])AeCurve_ISO50_50Hz;
                nMin_EV_Value = ((1<<6)*10);
                break;

            case _IMGISO_100:
                AeCurve = (UINT32(*)[5])AeCurve_ISO100_50Hz;
                nMin_EV_Value = ((1<<5)*10);
                break;

            case _IMGISO_200:
                AeCurve = (UINT32(*)[5])AeCurve_ISO200_50Hz;
                nMin_EV_Value = ((1<<4)*10);
                break;

            case _IMGISO_400:
                AeCurve = (UINT32(*)[5])AeCurve_ISO400_50Hz;
                nMin_EV_Value = ((1<<3)*10);
                break;
*/
        default:
            AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
        break;
        }
    }
    else//if(nFlicker==_POWERFreq_60Hz)
    {
        nMin_EV_Value = EV_Value_MIN;
        switch(nISO)
        {
        case _IMGISO_Auto:
        case _IMGISO_100:
        case _IMGISO_200:
        case _IMGISO_400:
        case _IMGISO_800:
        case _IMGISO_1600:
        case _IMGISO_3200:
            if ((IPL_GetNextMode() == IPL_MODE_VIDEO) || (IPL_GetNextMode() == IPL_MODE_VIDEOREC))
            {
                if ( (nVideoResolution ==  _Resolution_320x240) || (nVideoResolution ==  _Resolution_640x480))
                {
                    AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60_vga;
                }
                else
                {
                    AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
                }
            }
            else
            {
                AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
            }
        break;
/*
            case _IMGISO_50:
                AeCurve = (UINT32(*)[5])AeCurve_ISO50_60Hz;
                nMin_EV_Value = ((1<<6)*10);
                break;

            case _IMGISO_100:
                AeCurve = (UINT32(*)[5])AeCurve_ISO100_60Hz;
                nMin_EV_Value = ((1<<5)*10);
                break;

            case _IMGISO_200:
                AeCurve = (UINT32(*)[5])AeCurve_ISO200_60Hz;
                nMin_EV_Value = ((1<<4)*10);
                break;

            case _IMGISO_400:
                AeCurve = (UINT32(*)[5])AeCurve_ISO400_60Hz;
                nMin_EV_Value = ((1<<3)*10);
                break;
*/
        default:
            AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
        break;
        }
    }

    nPreFlickerSetting = nFlicker;
    nPreISOSetting = nISO;

//    ResetAE();
}
//#NT#2009/10/28#Jarkko Chang -end


//#NT#20090703#ETHAN LAU -BEGIN
void SceneMode_AE(PImageAlgInfo Info)
{
    static UINT8 nPreFlickerSetting = 0xFF;
    static UINT8 nPreISOSetting = 0xFF;
    static UINT8 nPreSceneModeSetting = 0xFF;
    static UINT8 nPreAEModeSetting = 0xFF;
    static UINT nVideoResolution = 0xFF;

    nVideoResolution = GetIPPSizeInfor(_Vid_ResolutionIdx);


    if(Info->SceneMode == _SceneMode_Auto)
    {
        if(((Info->Frequency != nPreFlickerSetting)&&(Info->ISO!= nPreISOSetting)))
        {
            AntiFlickerISO_AE(Info->ISO, Info->Frequency);
        }
        if(Info->AEMode != nPreAEModeSetting)
        {
            AEMode_AE(Info->AEMode);
        }

        debug_msg("SceneMode_AE  %d,%d\r\n",Info->Frequency,Info->ISO);
        if(Info->Frequency==_POWERFreq_50Hz)
        {
            switch(Info->ISO)
            {
            nMin_EV_Value = EV_Value_MIN;
            case _IMGISO_100:
                        AeCurve = (UINT32(*)[5])AeCurve_ISO100_50Hz;
            break;
            case _IMGISO_200:
                        AeCurve = (UINT32(*)[5])AeCurve_ISO200_50Hz;
            break;
            case _IMGISO_400:
            case _IMGISO_800:
            case _IMGISO_1600:
            case _IMGISO_3200:
                        AeCurve = (UINT32(*)[5])AeCurve_ISO400_50Hz;
            break;
            case _IMGISO_Auto:
                        AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
            break;
            default:
                AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker50;
            break;
            }
        }
        else//if(nFlicker==_POWERFreq_60Hz)
        {
            nMin_EV_Value = EV_Value_MIN;
            switch(Info->ISO)
            {
            case _IMGISO_100:
                        AeCurve = (UINT32(*)[5])AeCurve_ISO100_60Hz;
            break;
            case _IMGISO_200:
                        AeCurve = (UINT32(*)[5])AeCurve_ISO200_60Hz;
            break;
            case _IMGISO_400:
            case _IMGISO_800:
            case _IMGISO_1600:
            case _IMGISO_3200:
                        AeCurve = (UINT32(*)[5])AeCurve_ISO400_60Hz;
            break;
            case _IMGISO_Auto:
                        AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
            break;
            default:
                AeCurve = (UINT32(*)[5])AeCurve_AntiFlicker60;
            break;
            }
        }
        
    }
    else
    {
        if(Info->AEMode != nPreAEModeSetting)
        {
            AEMode_AE(Info->AEMode);
        }
        if((Info->SceneMode == nPreSceneModeSetting))
            return;
        else
        {
            AntiFlickerISO_AE(Info->ISO, Info->Frequency);
        }

        if(gImageAlgInfo.Frequency == _POWERFreq_60Hz)
        {
            switch(Info->SceneMode)
            {
                case _SceneMode_Beach:
                case _SceneMode_Landscape:
                    AeCurve = (UINT32(*)[5])AeCurve_Landscape_60Hz;                
                break;
                case _SceneMode_NightPotrait:
                    AeCurve = (UINT32(*)[5])AeCurve_NightPotrait_60Hz;
                break;
                case _SceneMode_Sport:
                    AeCurve = (UINT32(*)[5])AeCurve_Action_60Hz;
                break;
                case _SceneMode_Potrait:
                case _SceneMode_Party:
                    AeCurve = (UINT32(*)[5])AeCurve_Potrait_60Hz;
                break;
                case _SceneMode_NightLandscape:
                    AeCurve = (UINT32(*)[5])AeCurve_NightLandscape_60Hz;
                break;

                    
                case _SceneMode_NightScene:
                case _SceneMode_AV:
                case _SceneMode_TV:
                case _SceneMode_Manual:
                case _SceneMode_SunSet:
                case _SceneMode_BackLight:
                case _SceneMode_Document:
                case _SceneMode_Firework:
                case _SceneMode_BMode:
                case _SceneMode_Program:
                default:
                break;
            }
        }
        else
        {
            switch(Info->SceneMode)
            {
                case _SceneMode_Beach:
                case _SceneMode_Landscape:
                    AeCurve = (UINT32(*)[5])AeCurve_Landscape_50Hz;                
                break;
                case _SceneMode_NightPotrait:
                    AeCurve = (UINT32(*)[5])AeCurve_NightPotrait_50Hz;
                break;
                case _SceneMode_Sport:
                    AeCurve = (UINT32(*)[5])AeCurve_Action_50Hz;
                break;
                case _SceneMode_Potrait:
                case _SceneMode_Party:
                    AeCurve = (UINT32(*)[5])AeCurve_Potrait_50Hz;
                break;
                case _SceneMode_NightLandscape:
                    AeCurve = (UINT32(*)[5])AeCurve_NightLandscape_50Hz;
                break;

                    
                case _SceneMode_NightScene:
                case _SceneMode_AV:
                case _SceneMode_TV:
                case _SceneMode_Manual:
                case _SceneMode_SunSet:
                case _SceneMode_BackLight:
                case _SceneMode_Document:
                case _SceneMode_Firework:
                case _SceneMode_BMode:
                case _SceneMode_Program:
                default:
                break;
            }
        }
    }

    Allot_AeSet(AE_Param.EV_Value);
    nPreFlickerSetting = Info->Frequency;
    nPreISOSetting = Info->ISO;
    nPreSceneModeSetting = Info->SceneMode;
    nPreAEModeSetting = Info->AEMode;

}
//#NT#20090703#ETHAN LAU -END
/**
    Initialize AE
*/
void aaa_InitAEParameter(void)
{
    if(AE_FirstRun)
        return;
    AE_Param.EV_Value = EV_INITIAL;
    AntiFlickerISO_AE(gImageAlgInfo.ISO, gImageAlgInfo.Frequency);
    AE_FirstRun = TRUE;
}


void aaa_InitAE(void)
{
    AE_ChangeRate(AE_RATE_NORMAL);
    aaa_AeLock(UNLOCK);
    AE_Flag=0;
    aaa_InitAEParameter();
    if(!Is_AELock())
    {
        Allot_AeSet(AE_Param.EV_Value);
        Ae_SetPgaGain();
    }
}
void SetPreExpLine(UINT32 Expline)
{
    if((Expline<1)&&(Expline>=0xFFFFF))
        return;
//#NT#2009/11/14#Jarkko Chang -begin
//    if(Expline < 16)
//       Expline = 16;
//#NT#2009/11/14#Jarkko Chang -end

    Sensor_WriteReg(0x08, ((Expline >> 16) & 0x000F));
    Sensor_WriteReg(0x09, ((Expline      ) & 0xFFFF));
    Sensor_WriteReg(0x0C, 0);
}

void AeSetAGCGain(void)
{
     SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
}

void SetPreAGC(int intGain, int fraGain)
{
  UH temp, AGCtemp;

    temp = 0;
    while(fraGain > 63)
    {
        fraGain -= 32;
        temp++;
    }

    if(temp != 0)
        AGCtemp = (((1 << temp) - 1) << 6) + fraGain;
    else
        AGCtemp = fraGain;

//	debug_err(("AGC = 0x%X\r\n", AGCtemp));

//#NT#2009/11/14#Jarkko Chang -begin
//    if(AGCtemp < 8)
//       AGCtemp = 8;
//#NT#2009/11/14#Jarkko Chang -end

#if 0
	sprintf(message, "AGCtemp = %d", AGCtemp);
    SensorInfor.FP_ShowOSDString(message, 50, 140, 15);
#endif
	Sensor_WriteReg(0x35, AGCtemp);
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
    SetPreExpLine(AE_GetCapExpLineBefConv());
    SetPreAGC(AE_Param.AePgaGain_int, AE_Param.AePgaGain_fra);
}

void aaa_AEprocess(void)
{
//#NT#2010/02/02#Jarkko Chang -begin
//#NT#add for fd ae
    UINT32 block_w,block_h;
    UINT32 face_location[20][2];
    UINT32 i,j,k,size_x,size_y,fd_num;
    FACE    Face_rslt[FACE_NUM_MAX];
    FDAPP_INSIZEINFOR   in_size;

    GetActImgFBAddr_Path1(&FB_para1);
//#NT#2009/12/09#Jarkko Chang -begin
    SenFlowFunc(IME_Chrom_Prv_AE_SETTING, &gSenCBInfo);
//#NT#2009/12/09#Jarkko Chang -end

    gAEcounter++;//=Get_SieFrameCounter();
    nAE_Maxline = GetIPPSizeInfor(_VDTotal_Preview)-5;

    //#NT#20090703#ETHAN LAU -BEGIN
#if 0
    AEMode_AE(gImageAlgInfo.AEMode);
#else
    if(Is_FDLock())
    {
        AEMode_AE(gImageAlgInfo.AEMode);
    }
    else
    {
        fd_GetRsltFace(Face_rslt, ABS_COORD);
        fd_getInputSize(&in_size);
        fd_num = fde_GetRsltNum(FDE_RSLT_NONOVERLAP);
        memset(&AE_Window_Adaptive,1,sizeof(UINT8)*64);
//        block_w = (GetIPPSizeInfor(_ImeOut1Hsize)>>3);
//        block_h = GetIPPSizeInfor(_ImeOut1Vsize)>>3;
        block_w = in_size.width>>3;
        block_h = in_size.height>>3;
        if(fd_num == 0)
        {
            AEMode_AE(gImageAlgInfo.AEMode);
        }
        else
        {
            for(j=0;j<fd_num;j++)
            {
                for(i=0;i<CA_WINDOW_X;i++)
                {
                    if(i*block_w >Face_rslt[j].stx)
                    {
                        face_location[j][0] = i-1;
                        break;
                    }
                }
                for(i=0;i<CA_WINDOW_X;i++)
                {
                    if(i*block_h > Face_rslt[j].sty)
                    {
                        face_location[j][1] = i-1;
                        break;
                    }
                }
                size_x = Face_rslt[j].width / block_w + 1;
                size_y = Face_rslt[j].height / block_h + 1;
                for(i=face_location[j][0];i<CA_WINDOW_X;i++)
                {
                    if((i - face_location[j][0])<size_x)
                    {
                        for(k=face_location[j][1];k<CA_WINDOW_Y;k++)
                        {
                            if((k - face_location[j][1])<size_y)
                            {
                                if(AE_Window_Adaptive[i][k] == 1)
                                    AE_Window_Adaptive[k][i]+=10;
                            }
                        }
                    }
                }
            }
#if 0
            for(i=0;i<CA_WINDOW_X;i++)
            {
                debug_err(("%d %d %d %d %d %d %d %d\r\n",AE_Window_Adaptive[i][0],AE_Window_Adaptive[i][1],
                                                         AE_Window_Adaptive[i][2],AE_Window_Adaptive[i][3],
                                                         AE_Window_Adaptive[i][4],AE_Window_Adaptive[i][5],
                                                         AE_Window_Adaptive[i][6],AE_Window_Adaptive[i][7]));
            }
            debug_err(("\r\n"));
#endif
            AEMode_AE(_IMGAEMode_FaceDetection);
        }
    }
#endif
//#NT#2010/02/02#Jarkko Chang -end    
    AntiFlickerISO_AE(gImageAlgInfo.ISO, gImageAlgInfo.Frequency);
//    SceneMode_AE(&gImageAlgInfo);
    //#NT#20090703#ETHAN LAU -END


    //center histogram
    aaa_GetCenterHist(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset), (UW*)CenterHist);

    YAverage=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
    aaa_Read_HIST_RSLT((UW*)Hist);
    AeConverge(YAverage);
    //SenFlowFunc(IPE_PRV_AE_SETTING, &gSenCBInfo);
    if(AE_UartMsg)
    {
        if((gAEcounter%5)==0)
        {
            debug_err(("StbCnt = %d, YAverage=%3d, LV=%3d, EV_Value=%8d, ISO_Gain(AGC)=%3d(%4x), ExpoTime=%7d, AE_ExpectY=%3d, ExpoLine=%d\r\n",
                        AE_Param.StableCnt,
                        YAverage,
                        LV,
                        AE_Param.EV_Value,
                        AE_Param.ISO_Gain,
                        AE_Param.AePgaGain_fra,
                        AE_Param.ExpoTime,
                        AE_Debug.AE_ExpectY,
                        AE_Param.ExpoLine));
            debug_err(("----------------------------\r\n"));
            gAEcounter=0;
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

        SenFlowFunc(IPE_PRV_AE_SETTING, &gSenCBInfo);
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
    UINT32 AeRatio,EV_Value;

    AeRatio = GetAEAdjustRatio(YAvg);
    if(AeRatio> (100-YConRange_roughly) && AeRatio< (100+YConRange_roughly) && !(AE_Flag & Flag_AeReset))
    {
        AE_Param.StableCnt++;
        if(AeRatio> (100-YConRange) && AeRatio< (100+YConRange))
        {
            return;
        }
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

    EV_Value = AE_Param.EV_Value*AeRatio/100; //smoothing
    EV_Value = (EV_Value+AE_Param.EV_Value)/2;
//    EV_Value = (EV_Value*3+AE_Param.EV_Value)/4;

    if(EV_Value>=EV_Value_MAX)
        EV_Value=EV_Value_MAX;
    else
    {
//#NT#2009/11/30#Jarkko Chang -begin
//#NT#modofy for hd500c3+mi5120 iso gain issue
//        if (gImageAlgInfo.SceneMode == _SceneMode_NightLandscape)
        if ((gImageAlgInfo.SceneMode == _SceneMode_NightLandscape) || (gImageAlgInfo.AEEVValue > _IMGEVValue_P3))
        {
            if(EV_Value <= EV_Value_Night_MIN)
            {
               EV_Value = EV_Value_Night_MIN;
            }
        }
        else
        {
            if(EV_Value <= nMin_EV_Value)
            {
               EV_Value = nMin_EV_Value;
            }
        }
//#NT#2009/11/30#Jarkko Chang -end
    }

    if(0)//(EV_Value < AE_LOWLIGHT_EV_THRES)
    {
        AE_ChangeRate(AE_RATE_LOWLIGHT);
        bAE_LowLightMode = TRUE;
    }
    else
    {
        AE_ChangeRate(AE_RATE_NORMAL);
        bAE_LowLightMode = FALSE;
    }
/*
    if((YAverage > 195)&&(bAE_LowLightMode))
        EV_Value = 640;
*/
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

        //debug_err(("Gain=0x%x",AE_Param.AePgaGain_fra));
        AE_Flag &= (~Flag_SetPgaGain);
        nPreAGC_int = AE_Param.AePgaGain_int;
        nPreAGC_fra = AE_Param.AePgaGain_fra;
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
    Ae_SetExpoTime();
}

void AeSet_isr(void)
{
     if(AE_Flag & Flag_SetPgaGain)
    {
        AE_Flag &= (~Flag_SetPgaGain);
        SenFlowFunc(AE_PRV_GAIN_SETTING, &gSenCBInfo);
    }
}
UINT32 Get_AE_Idx(void)
{
    return AE_Idx;
}

void AeCap2Prv(void)
{
    //AE_Flag |= Flag_SetPgaGain;
    Allot_AeSet(AE_Param.EV_Value);
    //AeISOGain2TGValue(AE_Param.ISO_Gain);
    //AeExpoTime2TGValue(AE_Param.ExpoTime, GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview));
    //SetPreExpLine(AE_Param.ExpoLine);
    Ae_SetPgaGain();
    nAE_Maxline = GetIPPSizeInfor(_VDTotal_Preview)-5;
}
//#NT#20091008#ethanlau -begin
void AntiShakingMech(_IMGStillAntiShaking level)
{
    SetAntishaking_RawbiningEnalbe(FALSE);
    debug_msg("level=%d\r\n",level);
    switch(level)
    {
        case _Level1:
        case _Level2:
            if(AE_Param.ISO_Gain_Cap< (MaxISO>>1) )
            {
                AE_Param.ISO_Gain_Cap =AE_Param.ISO_Gain_Cap<<1;
                AE_Param.ExpoTime_Cap=AE_Param.ExpoTime_Cap>>1;
                //AeISOGain2TGValue(AE_Param.ISO_Gain_Cap);
            }
            else
                SetAntishaking_RawbiningEnalbe(TRUE);
        break;
        case _Level0:
        default:
        break;
    }
}
//#NT#20091008#ethanlau -end

//#NT#20090708#ETHAN LAU -BEGIN
void AePrv2Cap(void)
{
    UINT32 nPrev_Expline, nCap_Expline, ShutterOffset,  ExpoTime,tmp,_ratio;
    INT32 GR_rst_end, GR_shutter_start, GR_read_start,ShutterTime;
    UINT32 TargetY, OriY_Pregam, TarY_Pregam;
    
    nAE_Maxline = GetIPPSizeInfor(_VDTotal_Captrue)-5;

    //if (Sen_GetCalMode() == DISABLE)
    {
        SceneMode_AE(&gImageAlgInfo);
    }
    
    if(GRS_MODE)//GRS AePrv2Cap
    {
        if(Flash_Param.FlshEna)
        {
            ExpoTime=AE_Param.ExpoTime*BiningMode;
            SetPreExpLine(GetIPPSizeInfor(_VDTotal_Captrue));//To reduce dark gradient current. Recommended by Micron FAE
        }
        else
        {
            Ae_SetPgaGain();
            ExpoTime=AE_Param.ExpoTime*BiningMode;
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

        debug_ind(("AePrv2Cap ShutterOffset=%d\r\n", ShutterOffset));

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


        debug_ind(("AePrv2Cap ExpoTime=%d\r\n", ExpoTime));
        debug_ind(("AePrv2Cap ShutterTime=%d\r\n", ShutterTime));
        debug_ind(("AePrv2Cap GR_read_start=%d\r\n", GR_read_start));
        debug_ind(("AePrv2Cap GR_shutter_start=%d\r\n",GR_shutter_start));

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
        SetIPPFuncInfor(_StillAntiShakingIdx, gImageAlgInfo.StillAntiShaking);
        AE_Param.ExpoTime_Cap = AE_Param.ExpoTime*BiningMode;
        AE_Param.ISO_Gain_Cap = AE_Param.ISO_Gain;

        if( (Flash_Param.FlshEna) && (AE_Param.EV_Value<Flash_Param.FTP) )
            nCap_Expline =  GetIPPSizeInfor(_VDTotal_Captrue);
        else
        {
            if (gImageAlgInfo.DriveMode == _IMGDriveMode_Single)
            {
                //AE_Param.ExpoTime_Cap = AE_Param.ExpoTime;

                TargetY = NormalExpectY*EVCompTable[gImageAlgInfo.AEEVValue - 4]/100;

                if((AE_Param.EV_Value <= (nMin_EV_Value*1.2))&&(YAverage<TargetY/2))
                {
                    if(YAverage >18)
                    {
                        OriY_Pregam = GammaInverse(YAverage);
                    }
                    else
                    {
                        OriY_Pregam = (YAverage+3)*4/2;
                    }
                    if(OriY_Pregam == 0)
                        OriY_Pregam = 1;
                    TarY_Pregam = GammaInverse(TargetY);

    //                if (OriY_Pregam < TarY_Pregam/2)
    //                    TarY_Pregam = TarY_Pregam/2;
                    
    //                nCap_Expline = nCap_Expline*TarY_Pregam/OriY_Pregam;
                    AE_Param.ExpoTime_Cap = AE_Param.ExpoTime_Cap*TarY_Pregam/OriY_Pregam;
                }
            }
        }

        if(GetIPPFuncInfor(_StillAntiShakingIdx)!= _Level0)
        {
            AntiShakingMech(GetIPPFuncInfor(_StillAntiShakingIdx));
        }


        if(AE_Param.ExpoTime_Cap > 500000)
            AE_Param.ExpoTime_Cap = 500000;
        
        AeExpoTime2TGValue(AE_Param.ExpoTime_Cap,GetIPPSizeInfor(_VDTotal_Captrue),GetIPPSizeInfor(_HDTotal_Captrue));
        nCap_Expline = AE_Param.ExpoLine;

        AE_Debug.AE_CapISO =AE_Param.ISO_Gain_Cap * Expo_CompensateRatio / 100;
        AeISOGain2TGValue(AE_Debug.AE_CapISO);
        
//        ISO_Prv=AE_Param.ISO_Gain;
        CapExpLine_befConv= nCap_Expline;
//        AE_Param.ExpoLine= nCap_Expline;
//        SetIPPFuncInfor(_StillAntiShakingIdx, gImageAlgInfo.StillAntiShaking);
//        debug_msg("GetIPPFuncInfor(_StillAntiShakingIdx)=%d\r\n",GetIPPFuncInfor(_StillAntiShakingIdx));
#if 0
        debug_err(("AePrv2Cap Flash_Param.Ena = %d\r\n", Flash_Param.FlshEna));
        ExpoTime = (nCap_Expline*GetIPPSizeInfor(_HDTotal_Captrue)*SENSOR_CAP_PCLK)/SENSOR_CAP_PCLK_M;
      debug_err(("AePrv2Cap :preL=%d capL=%d iso=%d exptime=%d\r\n",nPrev_Expline,nCap_Expline,AE_Param.ISO_Gain,ExpoTime));
#endif
 //       AE_Param.ExpoTime = (nCap_Expline*GetIPPSizeInfor(_HDTotal_Captrue)*SENSOR_CAP_PCLK)/SENSOR_CAP_PCLK_M;
//        AE_Param.ExpoTime= AE_Debug.AE_CapExpoTime;
//#NT#2009/10/20#Jarkko Chang -begin
//#NT#add for jpg exif using

            if  (fixiso)
            {
                AE_Param.ISO_Gain_Cap = FIXISO_mode;
            }
            else
            {
                AE_Param.ISO_Gain_Cap = AE_Debug.AE_CapISO;
            }
            AE_Param.ExpoTime_Cap =  (nCap_Expline*GetIPPSizeInfor(_HDTotal_Captrue)*SENSOR_CAP_PCLK)/SENSOR_CAP_PCLK_M;
//#NT#2009/10/20#Jarkko Chang -end
    }
}
//#NT#20090708#ETHAN LAU -end


void Ae_Transform(void)
{
    AeISOGain2TGValue(AE_Param.ISO_Gain);
    AeExpoTime2TGValue(AE_Param.ExpoTime, GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview));
    SetPreExpLine(AE_Param.ExpoLine);
    Ae_SetPgaGain();
}

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

void AeExpoTime2TGValue(UINT32 Time, UINT32 VDTotal, UINT32 HDTotal)
{
    UINT32 LineTime;
    UINT32 ExpLine;
    UINT32 ExpLineTime;
//    ExpLineTime = (HDTotal*SENSOR_PRE_PCLK)/ SENSOR_PRE_PCLK_M;
//        ExpLineTime = HDTotal/60;
    ExpLine = (Time*SENSOR_PRE_PCLK_M) / (HDTotal*SENSOR_PRE_PCLK);
//    ExpLine = (Time*SENSOR_PRE_PCLK_M) / HDTotal;
//    ExpLine = (Time*60) / (HDTotal);
    if(ExpLine > 0xFFFF)
       ExpLine = 0xFFFF;

    if(ExpLine < 0x0001)
       ExpLine = 0x0001;

    AE_Param.ExpoLine = ExpLine;
//    LineTime = (ExpLine*HDTotal)/SENSOR_PRE_PCLK_M;
    LineTime = (ExpLine*HDTotal*SENSOR_PRE_PCLK)/SENSOR_PRE_PCLK_M;
    
    if(LineTime<Time)
        Expo_CompensateRatio = Time*100/LineTime;
    else
        Expo_CompensateRatio = 100;
}

void AeISOGain2TGValue(UINT32 ISO_Gain)
{
    UINT32 Gain_factor, Calcul_AGC_Ratio;

    Calcul_AGC_Ratio = 100*ISO_Gain/AeCurve[0][AE_ISO];

    Gain_factor = ((Calcul_AGC_Ratio-100)*100+(GAIN_STEP>>1))/GAIN_STEP + AGC_ISOMin;
    
    if(Gain_factor > 0x7F)
        Gain_factor = 0x7F;

    AE_Param.AePgaGain_fra = Gain_factor;
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
    UINT32  OverExpo_Max_AddY;//,temp;

    LV=10+RatioLog2(AE_Param.EV_Value,20);

#if(SENSOR_GRS_MODE == 1)
       GRS_MODE = TRUE;
 #else
        GRS_MODE = FALSE;
#endif

    if(YAvg==0)
        YAvg=1;

    //debug_err(("LV = %2d, EV = %5d\r\n", LV, AE_Param.EV_Value));

#if _AE_REDUCE_OVER_EXPO
    //if((gImageAlgInfo.AEEVValue==_IMGEVValue_00) && (gImageAlgInfo.AEMode!=_IMGAEMode_Spot))
    if(gImageAlgInfo.AEEVValue==_IMGEVValue_00)
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
                OverExpo_Max_AddY = InterpolateValue(AE__GetLV(), OverExpo_Max_AddY_Night, OverExpo_Max_AddY_Indoor, 70, 100);
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

    if(gImageAlgInfo.AEEVValue != _IMGEVValue_00)
    {
        ExpectY = (NormalExpectY*EVCompTable[gImageAlgInfo.AEEVValue - 4])/100;
    }
    else
    {
        ExpectY = NormalExpectY;
    }

    if(((ExpectY>Add_Y)||(!bAE_LowLightMode))&&(AE_Counter > 3))
        ExpectY-=Add_Y;
    Ratio = (YAvg * 100) / ExpectY;

    Real_EV = AE_Param.EV_Value*Ratio/100;
//    debug_msg("-->Real EV = %d\r\n",Real_EV);
#if 1
    if(Ratio<60)
        Ratio = 60;
    else if(Ratio>170)
        Ratio = 170;
#else   //orig
    if(Ratio<50)
        Ratio = 50;
    else if(Ratio>200)
        Ratio = 200;
#endif
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
}
//#NT#20090703#ETHAN LAU -BEGIN
void SetAeBining(UINT32 Mode)
{
    if(!Is_AELock())
        debug_err(("AE is unlock~~"));
    BiningMode = Mode;
    //debug_msg("!!!!BiningMode=%d\r\n",BiningMode);
}
//#NT#20090703#ETHAN LAU -END


void Allot_AeSet(UINT32 CurrentEV)
{
    UINT8 i;
    UINT32 Calcul_Time = 0, Calcul_Gain = 0;
    UINT32 FnRatio;

    //FnRatio = Ae_IrisZoomStep0[0];
    FnRatio = 100;

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

    #if 0
    if(!(AeCurve[i][AE_ADJ_TARGET] & AE_ADJ_IRIS))
    {
        if(AE_Param.AeUseIris != AeCurve[i][AE_IRIS])
        {
            AE_Flag |= Flag_SetIris;
        }
        AE_Param.AeUseIris = AeCurve[i][AE_IRIS];
    }
    #endif


    if(AeCurve[i][AE_ADJ_TARGET] & AE_ADJ_EXP)
    {
        Calcul_Gain = AeCurve[i][AE_ISO];

        if(Calcul_Gain > MaxISO)
        {
            Calcul_Gain = MaxISO;
        }

        Calcul_Time = (1000000*FnRatio)/(CurrentEV*Calcul_Gain/100);
    }
    else// if(AeCurve[i][AE_ADJ_TARGET] & AE_ADJ_GAIN)
    {
        Calcul_Time = AeCurve[i][AE_EXP];
        Calcul_Gain = (1000000*FnRatio)/(CurrentEV*Calcul_Time/100);
    }

    if(Calcul_Gain > MaxISO)
    {
        Calcul_Gain = MaxISO;
    }
//#NT#2009/11/14#Jarkko Chang -begin
    else if(Calcul_Gain < 50)
    {
        Calcul_Gain = 50;
    }
//#NT#2009/11/14#Jarkko Chang -end

    if(Calcul_Time < 250)
    {
        Calcul_Time = 250;
    }
    //Prv binning
    AE_Param.ISO_Gain = Calcul_Gain;
	//#NT#2010/06/10#Jarkko Chang -begin
    //#NT#modify for pclk 96Mhz using
    AE_Param.ExpoTime = Calcul_Time/BiningMode;
    //#NT#2010/06/10#Jarkko Chang -end
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


    AeExpoTime2TGValue(AE_Param.ExpoTime,GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview));
    AE_Flag |= Flag_SetExpoTime;
    if(AE_Param.ExpoLine<1)
        AE_Param.ExpoLine=1;
    AE_Param.ISO_Gain = AE_Param.ISO_Gain*Expo_CompensateRatio/100;

    AeISOGain2TGValue(AE_Param.ISO_Gain);
    AE_Flag |= Flag_SetPgaGain;

    AeSet();
    AGC_DelayCnt = 0;
}

#if 1
void WaitAeStable(UINT16 count)
{
    UINT16 VD_count = 0;
    AE_Param.StableCnt = 0;
    //#NT#2011/01/19#Jarkko Chang -begin
    //#NT# always wait VD for awb stable
    /*
    if((AE_Param.EV_Value <= nMin_EV_Value) || (AE_Param.EV_Value >= EV_Value_MAX))
    {
        return;
    }
    */
    //#NT#2011/01/19#Jarkko Chang -end
    while(AE_Param.StableCnt <= count)
    {
        sie_waitVD(1);
        if(VD_count++ > count * 3)
        {
            break;
        }
    }
}
#endif

BOOL Is_SensorGRERS(void)
{
    return gGR_ERS4LongExp;
}

UINT32 AE__GetLV(void)
{
    return LV;
}

//#NT#2010/03/12#Jarkko Chang -begin
UINT32 AE__GetEV(void)
{
//    return AE_Param.EV_Value;
    return Last_EV;
}

UINT32 GetAeStableCnt(void)
{
     return AE_Param.StableCnt;
}

BOOL GetAeConvergeState(void)
{
    if((AE_Param.EV_Value <= nMin_EV_Value) || (AE_Param.EV_Value >= EV_Value_MAX))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

UINT32 AE__GetRealEV(void)
{
    return Real_EV;
}
//#NT#2010/03/12#Jarkko Chang -end

UINT32 AE__GetNYAdj(void)
{
    return NormalY_adj;
}

UINT16 AE_GetPrvISO(void)
{
    return ISO_Prv;
}

UINT AE_GetCapExpLineBefConv(void)
{
    return CapExpLine_befConv;
}
UINT AE_GetHistHighCnt(void)
{
    return HighCnt;
}

void AE_GetHist(UINT32 *Histogram)
{
    UINT32 i;

    for(i=0;i<64;i++)
    {
        Histogram[i]=Hist[i];
    }
}

UINT AE_GetHistLowCnt(void)
{
    return LowCnt;
}

BOOL AE_GetFlagHFED(void)
{
    return f_HalfFrmExpDet;
}

//#NT#2009/09/23#Jarkko Chang -begin
/*************************** calibration function ***************************/
void AeSetAGCGainStep(UINT32 Step)
{
    AE_Param.AePgaGain_int = 0;
    AE_Param.AePgaGain_fra= Step;
    SetPreAGC(AE_Param.AePgaGain_int,AE_Param.AePgaGain_fra);
}

void AeSetAGCGainISO(UINT32 ISO)
{
    AE_Param.ISO_Gain= ISO;
    AeISOGain2TGValue(ISO);
    SetPreAGC(AE_Param.AePgaGain_int,AE_Param.AePgaGain_fra);
}

void AeSetPrvExpoLine(UINT32 ExpoLine)
{
    AE_Param.ExpoLine = ExpoLine;
    SetPreExpLine(ExpoLine);
    AE_Param.ExpoTime = (ExpoLine*GetIPPSizeInfor(_HDTotal_Preview)*SENSOR_PRE_PCLK)/SENSOR_PRE_PCLK_M;
}

void AeSetPrvExpoTime(UINT32 ExpoTime)
{
    AeExpoTime2TGValue(ExpoTime, GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview));
    SetPreExpLine(AE_Param.ExpoLine);
   AE_Param.ExpoTime =  ExpoTime;
}

void AeSetCapExpoTime(UINT32 ExpoTime)
{
    AeExpoTime2TGValue(ExpoTime, GetIPPSizeInfor(_VDTotal_Captrue), GetIPPSizeInfor(_HDTotal_Captrue));
   AE_Param.ExpoTime_Cap =  ExpoTime;
    SetPreExpLine(AE_Param.ExpoLine);
}
//#NT#2009/09/23#Jarkko Chang -end
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
