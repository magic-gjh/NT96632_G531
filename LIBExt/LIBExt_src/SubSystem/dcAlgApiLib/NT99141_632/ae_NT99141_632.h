#ifndef __AE_NT99141_632_H
#define __AE_NT99141_632_H

typedef struct AEPARAM_STRUCT
{
    UINT32 EV_Value;
    UINT32 ExpoTime;
    UINT32 StableCnt;
    UINT32 Pre_ExpoFrame;
    UINT32 AePgaGain_int;
    UINT32 AePgaGain_fra;
    UINT32 ExpoFrame;
    UINT32 ExpoLine;
    UINT32 PreExpoLine;
    UINT32 Sub_n;
    UINT32 Sub_m;
    UINT32 ISO_Gain;
    UINT32 AeUseIris;
    UINT32 ExpoLine_Cap;
    UINT32 ExpoTime_Cap;
    UINT32 ISO_Gain_Cap;
} AE_STRUCT;

typedef struct AEDebug_STRUCT
{
    UINT8   AE_ExpectY;
    UINT8   AE_AddY;
    UINT32  AE_CapExpoTime;
    UINT32  AE_FlashISO;
} AE_Debug_msg;
//////////////////////////////////
//AE Definition
//////////////////////////////////
#define DEBUG_AE_INFO   ENABLE//ENABLE//DISABLE

#define Max_AE_Idx  51//47


//#define PRV_MAX_FRAME             3
//#define SHUTTER_DELAY     PREVIEW_VSYNC_TOTAL-2;
//#define MIN_AGC 210
//#define AGC_Ev  75
#define SHOW_AE_MSG 0
//#define Agc_ISO50 470//550//570//430//327
//#NT#20090907#ethanlau -begin
//move to aaa_aeprocess
//#define SENSOR_PRE_PCLK     24000000
//#define SENSOR_CAP_PCLK     24000000
//#define SENSOR_PRE_PCLK_M   (SENSOR_PRE_PCLK/1000000)
//#define SENSOR_CAP_PCLK_M   (SENSOR_CAP_PCLK/1000000)
//#NT#20090907#ethanlau -end
#define Flag_SetExpoTime    1
#define Flag_SetPgaGain     2
#define Flag_SetIris        4
#define Flag_AeReset        8
#define Flag_AeCompByExpo   16

#define AE_EV           0
#define AE_EXP          1
#define AE_ISO          2
#define AE_IRIS         3
#define AE_ADJ_TARGET   4

#define AE_ADJ_EXP      1
#define AE_ADJ_GAIN     2
#define AE_ADJ_IRIS     0x80

#define EV_MAX			17

#define NormalExpectY          125
#define YConRange               10   //3
#define AE_LOWLIGHT_EV_THRES    500 //250//640//320

#define AE_RATE_NORMAL          0
#define AE_RATE_STARTUP         1
#define AE_RATE_LOWLIGHT        2
#define AE_RATE_FAST            3

#define _AE_REDUCE_OVER_EXPO        1
#define _AE_HIST_METER_ENABLE		0

// For high contrast in low light environment
#if _AE_REDUCE_OVER_EXPO
#define _AE_REDUCE_OVER_EXPO_LHC    1
#else
#define _AE_REDUCE_OVER_EXPO_LHC    0
#endif

#define OverExpo_Outdoor_EV         20480
#define OverExpo_Max_AddY_Outdoor   (NormalExpectY-110)
#define OverExpo_Outdoor_Th_H       45
#define OverExpo_Outdoor_Th_L       (OverExpo_Outdoor_Th_H/3)

#define OverExpo_Indoor_EV          10240
#define OverExpo_Max_AddY_Indoor    25
#define OverExpo_Indoor_Th_H        45
#define OverExpo_Indoor_Th_L        (OverExpo_Outdoor_Th_H/3)

#define OverExpo_Lowlight_EV        320//740//4934
#define OverExpo_Max_AddY_Lowlight  (NormalExpectY-60)
#define OverExpo_Lowlight_Th_H      10//6
#define OverExpo_Lowlight_Th_L      (OverExpo_Lowlight_Th_H/3)

#define Default_AddStep             2

#define F_Number					42
#define EV_Value_MIN    231 //6x
//#define EV_Value_MIN    198 //7x
//#define EV_Value_MIN    173 //8x
//#define EV_Value_MIN    138 //10x
//#define EV_Value_MIN    115 //12x
#define EV_Value_MIN_NightScene EV_Value_MIN
#define EV_Value_MAX    ((1<<EV_MAX)*10)
#define EV_INITIAL      5120

#define AGC_ISO50       1500//1900//3700//1300

#define AE_MIN_GAIN_EN  DISABLE

#define ISO_Gain_Min    75
#define ISO_Gain_Max    (ISO_Gain_Min*12)

#define AGC_ISOMin      4//0
#define AGC_GAP         16
#define GAIN_STEP       625

#define ISO_Gain_Range  (ISO_Gain_Max - ISO_Gain_Min)

#define ISO_MAX_RATIO   (ISO_Gain_Max*100/ISO_Gain_Min)

#define MinISO(AGC_CalRst,ptr)          \
{                                       \
    *ptr = ISOBASE*AGC_ISO50/AGC_CalRst;\
}

extern AE_Debug_msg AE_Debug;
extern volatile UINT8 AGC_DelayCnt;
extern AE_STRUCT AE_Param;

UINT32 GetAEAdjustRatio(UINT32 YAvg);
UINT32 RatioLog2( UINT32 Devider, UINT32 Devident);
void AE_ChangeRate(UB mode);
void Allot_AeSet(UINT32 CurrentEV);
void Dynamic_IpePara(void);
//#NT#20090706#ETHAN LAU -begin
extern void aaa_InitAE(UINT32 para_mun,...);
//#NT#20090706#ETHAN LAU -end
void init_ae(void);
void SetPreExpLine(UINT32 Expline);
UINT32 GetPreExpLine(UINT8 ShutterIdx);
void AE__SetExptime(UINT8 ShutterIdx);
extern void Ae_SetPgaGain(void);
void SetPreAGC(int intGain, int fraGain);
extern void SharpTG_CFG(int SEDA, int value);
//extern void SharpAFE_CFG(UINT32 addr, UINT32 value);
extern void aaa_AEprocess(void);
void AeConverge(UINT32 YAvg);
void AeSet(void);
UINT32 Get_AE_Idx(void);
extern void AePrv2Cap(void);
extern void AeCap2Prv(void);
//#NT#20090706
extern void AeSetAGCGain(void);
extern UINT32 Get_ExpoFrame(void);
extern void WaitAeStable(UINT16 count);
//#NT#2010/09/16#ethanlau -begin
extern void Ae_FastConverge2TargetLumi(UINT32* _gamma,_IMGSieBitDepth_sel _bit,UINT _targetY);
//#NT#2010/09/16#ethanlau -end
extern void aaa_InitAEParameter(void);
extern void AeISOGain2TGValue(UINT32 ISO_Gain);
extern void ISO_AE(UINT8 ISO_Speed);
extern UINT32 gFlagCap;

extern UINT32 nSensorPCLK, nSensorPxlNum, nAE_Maxline;
extern UINT8 nAE_ShutterIdx;
extern UINT32 YAverage;
extern UINT32 gAGC_ISO50;

extern void AeCapSet(void);
extern BOOL Is_SensorGRBulb(void);
extern UINT32 GetSensorGRBulb_IntTime(void);
extern UINT32 AE__GetLV(void);
extern UINT32 AE__GetNYAdj(void);
extern BOOL Is_SensorGRERS(void);
extern UINT AE_GetHistHighCnt(void);
extern BOOL AE_GetFlagHFED(void);
extern void Ae_Transform(UINT32 para_num,...);

typedef enum _AE_HIST_RATIO_
{
	AE_HIST_RATIO_DAYLIGHT = 0,
	AE_HIST_RATIO_NIGHT,
} AE_HIST_RATIO;

#define AE_HISTMETER_LEVEL		16
#define AE_HISTMETER_BLK_X		32
#define AE_HISTMETER_BLK_Y		30
#define AE_HISTMETER_WINDOW_X	(IMAGE_SIZE_H/AE_HISTMETER_BLK_X)//32
#define AE_HISTMETER_WINDOW_Y	(IMAGE_SIZE_V/AE_HISTMETER_BLK_Y)//18
#define AE_HISTMETER_BLK_NUM    (AE_HISTMETER_WINDOW_X*AE_HISTMETER_WINDOW_Y)
#define AE_HISTMETER_RANGE_GAP	0
#define AE_HISTMETER_RANGE_L	AE_HISTMETER_RANGE_GAP
#define AE_HISTMETER_RANGE_H	(AE_HISTMETER_WINDOW_Y-AE_HISTMETER_RANGE_GAP)

#define AE_ANTI_OVEREXPO_LV0	0 // Over Exposure OFF
#define AE_ANTI_OVEREXPO_LV1	1
#define AE_ANTI_OVEREXPO_LV2	2
#define AE_ANTI_OVEREXPO_LV3	3
#define AE_ANTI_OVEREXPO_LV4	4
#define AE_ANTI_OVEREXPO_LV5	5
#define AE_ANTI_OVEREXPO_LEVEL	AE_ANTI_OVEREXPO_LV4

#endif
