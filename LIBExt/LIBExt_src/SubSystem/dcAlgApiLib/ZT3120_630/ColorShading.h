#ifndef _COLORSHADING_H_
#define _COLORSHADING_H_

#include "Type.h"
#include "Debug.h"


#define ColorShadingTbl_H 65//80
#define ColorShadingTbl_V 49//60
#define Color_R 0
#define Color_Gr 1
#define Color_Gb 2
#define Color_B 3


#define CS_GainRes 6// The compensation table is 8-bit, and this define is 2.6(xx.xxxxxx), the max gain is (2^2 - 2^-6 = 3.98)
//#define CS_BlockX 160
//#define CS_BlockY 120
#define CS_TblSetNum 22//(2*11)
#define CONST_CS_BURSTPIC_MAX  3


//Maple@JJ070411 Performance
typedef enum _colorshadingstagestatus_
{
    STATUS_START=0x0,
    STATUS_1=0x1,
    STATUS_2=0x2,
    STATUS_3=0x3,
    STATUS_4=0x4,
    STATUS_5=0x5,
    STATUS_6=0x6,
    STATUS_7=0x7,
    STATUS_8=0x8,
    STATUS_FINISH=0xff,
} COLORSHADING_STAGESTATUS;
//~Maple@JJ070411 Performance

typedef enum _colorshadingstage_
{
    //for all mode
    COLORSHADING_STAGE_0_RESET      =0,
    COLORSHADING_STAGE_JUST_PROCEED =0x7,      // 不管做到哪個stage, 繼續做下去
    //for CapMode = COLORSHADING_CAPMODE_SINGLE
    COLORSHADING_STAGE_1_INITIAL    =1,
    COLORSHADING_STAGE_2_GET_TABLE  =2,       // 到NAND讀取gain table
    COLORSHADING_STAGE_3_GET_BAYER  =3,       // 經過IME後, 把gain table轉成gain Raw, 再將capture Raw與gain Raw相乘
    COLORSHADING_STAGE_4_DO_LINE    =4,       // 一條一條去作相乘
    COLORSHADING_STAGE_5_DO_REGION  =5,       // 這時候Raw已經吐完, 直接把下半部的Raw整塊作相乘
    COLORSHADING_STAGE_6_END        =6,
    //for CapMode = COLORSHADING_CAPMODE_BURST
    COLORSHADING_STAGE_A_INITIAL    =1,
    COLORSHADING_STAGE_B_GET_TABLE  =2,
    COLORSHADING_STAGE_C_DO_PARTIAL =3,       // 取gain table的1/4, 跟Dram中的3張Raw的前1/4作相乘 (size只夠放3張raw, 若再加整張table會不夠)
    COLORSHADING_STAGE_D_END        =4,
} COLORSHADING_STAGE;

typedef enum _colorshadingcapmode_
{
    COLORSHADING_CAPMODE_SINGLE=0,
    COLORSHADING_CAPMODE_BURST,
} COLORSHADING_CAPMODE;

typedef struct
{
    UINT32   RawBayAdd[CONST_CS_BURSTPIC_MAX];
    UINT32   GainBayAdd;
    UINT32   GainTblBufAdd;
//    UINT32   CurrTblAdd;
//    UINT32   LastTblAdd;
//    UINT16   LineCntBuf[CONST_CS_FIELDNUM_MAX];
//    UINT16   LineCntMax;
    UINT16   LinePre;
    UINT16   BayerHSize;
    UINT16   BayerVSize;
    UINT16   TableHSize;
    UINT16   TableVSize;
    UINT8    CurrTblInd;
//    UINT8    LastTblInd;
//    COLORSHADING_STAGE    Stage;
//    COLORSHADING_STAGESTATUS    StageStatus;
//    UINT8    StageArg;
    UINT8    CapFldNum;
    UINT8    RawBitDepth;
//    UINT8    PreTarget;
    UINT8    BayPattern;
    UINT8    BurstDivision;
    UINT8    BurstPicNum;
    COLORSHADING_CAPMODE CapMode;
//    BOOL     Enable;
} COLORSHADING_PARAM, *PCOLORSHADING_PARAM;



/** APIs to set up parameters **/
UINT8 ColorShading_SetRawBayAdd(UINT32 input);
UINT8 ColorShading_SetRawBayAdd_ByIndex(UINT32 input, UINT8 index);
UINT8 ColorShading_SetGainBayAdd(UINT32 input);
UINT8 ColorShading_SetGainTblBufAdd(UINT32 input);
UINT8 ColorShading_SetLinePre(UINT32 input);
UINT8 ColorShading_SetBayerHSize(UINT32 input);
UINT8 ColorShading_SetBayerVSize(UINT32 input);
UINT8 ColorShading_SetTableHSize(UINT32 input);
UINT8 ColorShading_SetTableVSize(UINT32 input);
UINT8 ColorShading_SetCurrTblInd(UINT32 input);
UINT8 ColorShading_SetStageArg(UINT32 input);
UINT8 ColorShading_SetCapFldNum(UINT32 input);
UINT8 ColorShading_SetRawBitDepth(UINT32 input);
UINT8 ColorShading_SetBayPattern(UINT32 input);
UINT8 ColorShading_SetBurstDivision(UINT32 input);
UINT8 ColorShading_SetCapMode(COLORSHADING_CAPMODE input);
UINT8 ColorShading_SetEnable(UINT32 input);
UINT8 ColorShading_SetUp(PCOLORSHADING_PARAM pColorShadingParam);

/** APIs to get parameters **/
BOOL ColorShading_GetEnable(void);
BOOL ColorShading_GetEnable_CheckCapMode(COLORSHADING_CAPMODE capMode);
UINT32 ColorShading_GetCurrTblAdd(void);
//Maple@JJ070411 Performance
UINT32 ColorShading_GetStage(void);
UINT32 ColorShading_GetStageStatus(void);
//~Maple@JJ070411 Performance

/** APIs to command **/
UINT8 ColorShading_Reset();
UINT8 ColorShading_Initial();
UINT8 ColorShading_Proceed(COLORSHADING_STAGE target, BOOL Wait);
UINT32 ColorShading_Debug(UINT32 input);
UINT8 ColorShading_ResetLastTblInd(void);


void LoadGainTable(unsigned long Add,unsigned long size, unsigned char byte);
void ColorShading_Cal(unsigned char* ColorShadingTbl, unsigned long Add, unsigned long Width,unsigned long Height,unsigned short MaxTbl_H,unsigned short MaxTbl_V,unsigned char StartPixel);

#endif//_COLORSHADING_H_

