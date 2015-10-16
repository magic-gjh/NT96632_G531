#ifndef _CAL_COMMON_630_H
#define _CAL_COMMON_630_H

#include "Cal_Alg.h"

typedef unsigned char   UB;         /* Unsigned 8-bit integer       */
typedef unsigned short  UH;         /* Unsigned 16-bit integer      */
typedef unsigned long   UW;         /* Unsigned 32-bit integer      */
#define  CAL_BLOCK        1
//----------------------------------------------------------------------------------------------------
//#define NandBlockSize        16000
#define AdjustDataNandAdd0 0xfb    //After 4M
#define AdjustDataNandAdd1 0xfc
#define AdjustDataNandAdd2 0xfd
#define AdjustDataNandAdd3 0xfe
//----------------------------------------------------------------------------------------------------
//#NT#2008/07/08#Bowen Li -begin
//#NT#move to cal_api.h
#if 0
#define _Iris_Test_Status       0x10
#define _Mshutter_Test_Status   0x11
#define _FAE_Test_Status        0x12
#define _Zoom_SpeedTest_Status  0x14
#define _Zoom_StepMissTest_Status  0x15
#define _Zoom_Stop2PR_Status  0x16// 0x15//JJ0228
#define _Focus_StepMissTest_Status  0x16
#define _FF_Status 0x81
#define _NF_Status 0x86
#define _Focus_Test_Status 0x87
#define _Focus_Offset_Status 0x88//JJ0325

#define _RTC_Test_Status        0x31
#define _EmptyVolt_Status        0x32
#define _Burnin_Status          0x33
#define _Language_Status       0x34 //ricky@0401/2008

#define _ISO_Status 0x20//JJ0212 0x19//JJ0210 0x18
#define _Mshutter_Status 0x1B//J0215 0x1A//JJ0201 0x19
#define _FTP_Status 0x21
#define _Prv_W_Status 0x41
#define _Prv_B_Status 0x42
#define _Cap_W_Status 0x45
#define _Cap_B_Status 0x46
#define _Dzoom_W_Status 0x4A
#define _Dzoom_B_Status 0x4B
#define _AWB_Status 0x52//JJ0201 0x51
#define _Flash_Status 0x61
#define _OB_Status 0x71
#define _Shading_Status 0x81
#endif
//#NT#2008/07/08#YBowen Li -end
//----------------------------------------------------------------------------------------------------
#define Prv_Mode 0
#define Cap_Mode 1
#define Dzoom_Mode 2
#define Status_White 0
#define Status_Black 1
//----------------------------------------------------------------------------------------------------
#define ERROR_OK 0
#define ERRORCODE_ISO_MIN 11
#define ERRORCODE_ISO_MAX 12
#define ERRORCODE_IRIS_NOTSWITCH 15
#define ERRORCODE_MSHUTTER_STG1_FAIL 10
#define ERRORCODE_MSHUTTER_STG2_FAIL 11
#define ERRORCODE_MSHUTTER_STG3_FAIL 12
#define ERRORCODE_MSHUTTER_STG4_FAIL 13
#define ERRORCODE_MSHUTTER_STG5_FAIL 14
#define ERRORCODE_MSHUTTER_STG6_FAIL 15
#define ERRORCODE_MSHUTTER_MIN 16
#define ERRORCODE_MSHUTTER_MAX 17
#define ERRORCODE_MSHUTTER_NOTOPEN 18
#define ERRORCODE_MSHUTTER_NOTCLOSE 19
#define ERRORCODE_ZOOM_TOOSLOW 20
#define ERRORCODE_ZOOM_STEPMISS 23
#define ERRORCODE_FOCUS_STEPMISS 24
#define ERRORCODE_ZOOM_STOP2PR_MAX 25//JJ0228
#define ERRORCODE_ZOOM_STOP2PR_MIN 26
#define ERRORCODE_FF_2Far 41
#define ERRORCODE_FF_2Near 42
#define ERRORCODE_FOCUSTEST_NG 43

#define ERRORCODE_FTP_MIN 21//flash trigger point
#define ERRORCODE_FTP_MAX 22
#define ERRORCODE_DPPW_MAX 31//defect pixels of preview white over the max
#define ERRORCODE_DPPB_MAX 32//defect pixels of preview black over the max
#define ERRORCODE_DPCW_MAX 33//defect pixels of capture white over the max
#define ERRORCODE_DPCB_MAX 34//defect pixels of capture black over the max
#define ERRORCODE_DPDW_MAX 35//defect pixels of dzoom white over the max
#define ERRORCODE_DPDB_MAX 36//defect pixels of dzoom black over the max
#define ERRORCODE_DP_CTNE_PRV 37
#define ERRORCODE_DP_CTNE_CAP 38
#define ERRORCODE_DP_CTNE_DZOOM 39
//#define ERRORCODE_FF_Status1 43
//#define ERRORCODE_FF_Status2 44
#define ERRORCODE_NF_2Far 46
#define ERRORCODE_NF_2Near 47
#define ERRORCODE_NF_Status1 48
#define ERRORCODE_NF_Status2 49
#define ERRORCODE_AWBR_MIN 51
#define ERRORCODE_AWBR_MAX 52
#define ERRORCODE_AWBB_MIN 53
#define ERRORCODE_AWBB_MAX 54
#define ERRORCODE_FYAVGP_MIN 61
#define ERRORCODE_FYAVGP_MAX 62
#define ERRORCODE_FYVAR_MAX     63
//#define ERRORCODE_FAGCP_MIN 61
//#define ERRORCODE_FAGCP_MAX 62
//#define ERRORCODE_FAGCC_MIN 63
//#define ERRORCODE_FAGCC_MAX 64
#define ERRORCODE_FAGC_BRIGHT 66
#define ERRORCODE_OB_MAX    71

//DP max num expansion
#define ERRORCODE_DPFIELD_MAX 72//defect pixels of certain field over the max

#define ERRORCODE_VIGXCENT_MIN  73
#define ERRORCODE_VIGXCENT_MAX  74

#define ERRORCODE_VIGYCENT_MIN  75
#define ERRORCODE_VIGYCENT_MAX  76

#define ERRORCODE_VIGGAIN_MAX   77

#define ERRORCODE_VIGSHIFT_MAX   78

//----------------------------------------------------------------------------------------------------
/*
#define DP_Prv_MAX 500//200
#define DP_Cap_MAX 3000//500
#define DP_Dzoom_MAX 200
*/
//----------------------------------------------------------------------------------------------------
#define CAL_TRUE    1
#define CAL_FALSE    0

#define DP_SEARCH_TILL_END      1

#define CAL_SAVE_RAW                0
#define CAL_SAVE_ADJUST             0
//----------------------------------------------------------------------------------------------------
typedef struct _NandAdjust_Struct{
    UINT Agc_ISOMin;
    UINT Mshutter;
    AWB_Struct AWB;
    UINT FlashTriggerPoint;
    UINT Flash_Y_Avg;
    UINT OB_Min;
    UINT Far_Focus[15];
    UINT Near_Focus[15];
    UINT Shading_Status; //Added 20040605
    UINT Shading_Gain[4];
}NandAdjust_Struct;

typedef struct _Skip_Struct{
    UINT SkipXL;
    UINT SkipXR;
    UINT SkipYU;
    UINT SkipYD;
}Skip_Struct;

typedef struct _Block_Struct{
    UINT Block_X;
    UINT Block_Y;
}Block_Struct;

typedef struct _Sensor_Cal_Struct{
    UINT            DefaultAgcISOMin;
    UINT            DefaultMshutter;
    UINT            DefaultAWBRgain;
    UINT            DefaultAWBBgain;
    UINT            DefaultFlashTriggerPoint;
    UINT            DefaultMinOB;
    UINT            DefaultFlashYAvg;
    UINT            DarkLimitPrv;
    UINT            DarkLimitCap;
    UINT            BrightLimitPrv;
    UINT            BrightLimitCap;
    Block_Struct    BlockPrv;
    Skip_Struct        SkipPrv;
    Skip_Struct        SkipPrv2;
    Skip_Struct        SkipPrv3;
    Block_Struct    BlockCap;
    Skip_Struct        SkipCap;
    UINT            MaxContDPPrv;
    UINT            MaxContDPCap;
    UINT            MaxContDPDzoom;
    UB                FlashYTarget;
    UB                FLashYMargin;
}Sensor_Cal_Struct;

void SetCalFlag(unsigned int value);
unsigned int GetCalFlag(void);
void DP_PrvCfg(unsigned int addr,unsigned int Xsize,unsigned int Ysize);
UB DefectPixel_Cal(UB mode,UB environment);
UB DefectPixel_Cap(UB status);

UINT8 DP_Search(void);
UW DFP_Bayer_Search(UW BayAdd, UH width, UH height, UH LineOffset, UW DPadd, UH OffsetX, UH OffsetY, UH StartCnt, UH MaxCnt,UB Threshold, UH Block_X, UH Block_Y);
UW DFP_VGA_Search(UW RGGBAddr, UH width, UH height, UH LineOffset, UW DPadd, UH OffsetX, UH OffsetY, UH StartCnt, UH MaxCnt,UB Threshold, UH Block_X, UH Block_Y);
void DFP_GetOnePicPrv(unsigned int addr,unsigned int Xsize,unsigned int Ysize);
void DFP_GetOnePicCap(unsigned int addr,unsigned int Xsize,unsigned int Ysize, unsigned char environment);

void SortDPdata(UW* Data,UH Num);
void SortDPdata2Fld(UINT32* Data,UINT16 Num);
BOOL CheckContinueDP(UW* Data, UH Num, UH mode);

void DP_Correction(UW BayAdd, UH CenterX, UH CenterY, UH Window_w, UH Window_h, UINT32 DPAddr);
void SaveRawData(UINT RawAdd,UINT ImWidth,UINT ImHeight,UINT Code);

UB FlashlightAEAdjust(void);
extern volatile Adjust_Struct AAA_Adjust;
extern Sensor_Cal_Struct CalInfor;
extern unsigned int guiCalFlags;


extern unsigned char HwDpCapFlag;

extern void Cal_AlgShowString(char *pStr, unsigned short X, unsigned short Y);
extern void Cal_AlgClearScreen(void);
extern void Cal_AlgDisplayData(UH x,UH y,UW Data);
extern void Cal_AlgDisplayErrorCode(UH x,UH y,UB code);

extern void CAL_PassAdjustData(void);


#endif// _CAL_COMMON_433_H
