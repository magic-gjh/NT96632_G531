#ifndef __AE_ZT3120_630_H
#define __AE_ZT3120_630_H

//#NT#2009/10/20#Jarkko Chang -begin
//#NT#add for jpg exif using
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
    UINT32 Sub_n;
    UINT32 Sub_m;
    UINT32 ISO_Gain;
    UINT32 AeUseIris;
    UINT32 ExpoTime_Cap;
    UINT32 ISO_Gain_Cap;
} AE_STRUCT;
//#NT#2009/10/20#Jarkko Chang -end

typedef struct AEDebug_STRUCT
{
    UINT8   AE_ExpectY;
    UINT8   AE_AddY;
    UINT32  AE_CapExpoTime;
    UINT32  AE_CapISO;
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

#define _AE_REDUCE_OVER_EXPO    1
#define NormalExpectY          110//122
#define YConRange_roughly       5
#define YConRange               3   //3
#define AE_LOWLIGHT_EV_THRES    500 //250//640//320
#define AE_RATE_NORMAL          0
#define AE_RATE_STARTUP         1
#define AE_RATE_LOWLIGHT    2

#define OverExpo_Max_AddY_Indoor    25  //5
#define OverExpo_Max_AddY_Outdoor   25  //40
#define OverExpo_Max_AddY_Night     90 //60
#define OverExpo_High_Th            4
#define OverExpo_Low_Th             2 //(OverExpo_High_Th/3)
#define OverExpo_Center_High_Th     2
#define Default_AddStep             5

#define EV_MIN          4
#define EV_MIN_Night    3
#define EV_MAX          18//15
#define EV_Value_MIN    ((1<<EV_MIN)*10)
#define EV_Value_Night_MIN    ((1<<EV_MIN_Night)*10)
#define EV_Value_MAX    ((1<<EV_MAX)*10)
//#NT#2009/10/22#Jarkko Chang -begin
#define EV_INITIAL      2560//5120

#define AGC_ISO50       10
#define ADJ_ISO         0
#define MaxISO          630

#define GAIN_STEP           1000
#define AGC_ISOMin          10
//#NT#2009/10/22#Jarkko Chang -end
extern AE_Debug_msg AE_Debug;
extern volatile UINT8 AGC_DelayCnt;
extern AE_STRUCT AE_Param;

UINT32 GetAEAdjustRatio(UINT32 YAvg);
UINT32 RatioLog2( UINT32 Devider, UINT32 Devident);
void AE_ChangeRate(UB mode);
void Allot_AeSet(UINT32 CurrentEV);
void Dynamic_IpePara(void);
extern void aaa_InitAE(void);
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
extern void Ae_Transform(void);
extern void AeSetAGCGain(void);
extern UINT32 Get_ExpoFrame(void);
extern void WaitAeStable(UINT16 count);
extern void aaa_InitAEParameter(void);
extern void AeISOGain2TGValue(UINT32 ISO_Gain);
extern void ISO_AE(UINT8 ISO_Speed);
extern UINT32 gFlagCap;

extern UINT32 nSensorPCLK, nSensorPxlNum, nAE_Maxline;
extern UINT8 nAE_ShutterIdx;
extern UINT32 YAverage;
extern UINT32 gAGC_ISO50;

extern void AeCapSet(void);
extern void AeExpoTime2TGValue(UINT32 Time, UINT32 VDTotal, UINT32 HDTotal);
extern BOOL Is_SensorGRBulb(void);
extern UINT32 GetSensorGRBulb_IntTime(void);
extern UINT32 AE__GetLV(void);
extern UINT32 AE__GetEV(void);
extern UINT32 AE__GetNYAdj(void);
extern UINT16 AE_GetPrvISO(void);
extern BOOL Is_SensorGRERS(void);
extern UINT AE_GetCapExpLineBefConv(void);
extern UINT AE_GetHistHighCnt(void);
extern UINT AE_GetHistLowCnt(void);
extern BOOL AE_GetFlagHFED(void);
//#NT#20090710#ETHAN LAU -begin
extern void AE_FIXISO(UINT mode);
//#NT#20090710#ETHAN LAU -end
//#NT#2010/03/12#Jarkko Chang -begin
extern UINT32 GetAeStableCnt(void);
extern BOOL GetAeConvergeState(void);
extern UINT32 AE__GetRealEV(void);
extern void AE_GetHist(UINT32 *Histogram);
//#NT#2010/03/12#Jarkko Chang -end

#endif
