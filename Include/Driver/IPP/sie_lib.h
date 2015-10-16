/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2004.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: SIE Library Module                                                    *
* Description:                                                             *
* Author: Harry                                                                 *
****************************************************************************/

/** \addtogroup mIIPPSIE */
//@{

#ifndef _SIE_LIB_H
#define _SIE_LIB_H

#include "Type.h"



#define  SlaveMode       0
#define  MasterMode      1
#define  Notinvert   0
#define  Invert      1

#define sie_Reset                       sie_reset
#define sie_Load                        sie_load
#define sie_RstSAC                      sie_rstSAC
#define sie_ReadPixelCounterStatus      sie_readPixelCounterStatus
#define sie_SetActWindow                sie_setActWindow
#define sie_SetSensorSync               sie_setSensorSync
#define sie_CheckFunctionEnable         sie_checkFunctionEnable
#define sie_ReadIntrStatus              sie_readIntrStatus
#define sie_ClrIntrStatus               sie_clrIntrStatus
#define sie_SetSyncPhase                sie_setSyncPhase
#define sie_SetBreakPoint               sie_setBreakPoint
#define sie_ReadCurrentOutputAdd        sie_readCurrentOutputAdd
#define sie_GetDramSao                  sie_getDramSao
#define sie_SetSie2DramBufNum           sie_setSie2DramBufNum
#define sie_SetOutputStartAdd           sie_setOutputStartAdd
#define sie_SetOutputStartAdd1          sie_setOutputStartAdd1
#define sie_SetLineOffset               sie_setLineOffset
#define sie_SetPackMode                 sie_setPackMode
#define sie_SetBufferSize               sie_setBufferSize
#define sie_SetSie2PrePath              sie_setSie2PrePath
#define sie_SetPingPong                 sie_setPingPong
#define sie_SetLoadTiming               sie_setLoadTiming
#define sie_SetFieldAct                 sie_setFieldAct
#define sie_SetOBWindow                 sie_setOBWindow
#define sie_GetOB                       sie_getOB
#define sie_SetOBGainOffset             sie_setOBGainOffset
#define sie_CheckBusy                   sie_checkBusy

// SIE Interrupt Enable Register
#define SIE_INTE_EFREND                 0x00000001  //bit 0
#define SIE_INTE_OFREND                 0x00000002  //bit 1
#define SIE_INTE_FLDEND                 0x00000004  //bit 2
#define SIE_INTE_LNEND                  0x00000008  //bit 3
#define SIE_INTE_LN16END                0x00000010  //bit 4
#define SIE_INTE_BP1                    0x00000020  //bit 5
#define SIE_INTE_BP2                    0x00000040  //bit 6
#define SIE_INTE_OVFL1                  0x00000080  //bit 7
#define SIE_INTE_FLSH                   0x00000400  //bit 10
#define SIE_INTE_MSH                    0x00000800  //bit 11
#define SIE_INTE_ACTST                  0x00001000  //bit 12
#define SIE_INTE_ACT_END                0x00002000  //bit 13
#define SIE_INTE_ALL                    0x00003fff

// SIE Status Register
#define SIE_INT_EFREND                  0x00000001  //bit 0
#define SIE_INT_OFREND                  0x00000002  //bit 1
#define SIE_INT_FLDEND                  0x00000004  //bit 2
#define SIE_INT_LNEND                   0x00000008  //bit 3
#define SIE_INT_LN16END                 0x00000010  //bit 4
#define SIE_INT_BP1                     0x00000020  //bit 5
#define SIE_INT_BP2                     0x00000040  //bit 6
#define SIE_INT_OVFL1                   0x00000080  //bit 7
#define SIE_INT_FLSH                    0x00000400  //bit 10
#define SIE_INT_MSH                     0x00000800  //bit 11
#define SIE_INT_ACTST                   0x00001000  //bit 12
#define SIE_INT_ACT_END                 0x00002000  //bit 13
#define SIE_INT_ALL                     0x00003fff


#define SIE_ACT_EN                      0x00000080  //bit 7
#define SIE_OB_AVER_EN                  0x00000100  //bit 8
#define SIE_OB_EN                       0x00000200  //bit 9
#define SIE_PRE_DEST                    0x00000400  //bit 10
//#NT#2011/11/07#YHLee -begin
//#NT#Add DVI API
#define SIE_DVI_EN                      0x00100000  //bit 20
//#define SIE_FIELD_IN                    0x02000000  //bit 25
//#define SIE_FIELD_INV                   0x04000000  //bit 26
#define SIE_HFLIP_EN                      0x10000000  //bit 28
#define SIE_VFLIP_EN                      0x20000000  //bit 29
//#NT#2011/11/07#YHLee -end

typedef struct{
    UH    sie_hd_sync; ///< H active
    UH    sie_vd_sync; ///< V field1 active
    UH    sie_hd_total; ///< V field2 active
    UH    sie_vd_total; ///< V field3 active
} STR_HDVD;

typedef struct{
    UH    Hsize; ///< H active
    UH    Vsize; ///< V field1 active
    UH    lineoft;
} STR_SIEOUT_SIZE;

typedef enum
{
    SIE_PPB_8LINES = 0,
    SIE_PPB_16LINES = 1,
    SIE_PPB_32LINES = 2,
    SIE_PPB_64LINES = 3,
    SIE_PPB_96LINES = 4,
    SIE_PPB_128LINES = 5,
    SIE_PPB_192LINES = 6,
    SIE_PPB_FULL_HEIGHT = 7
}SIE_PPB_HeightEnum;


typedef enum
{
    ONE_OUTPUT_BUF = 0,
    TWO_OUTPUT_BUF = 1
}SIE_OutBufNumEnum;

typedef enum
{
    OUT_DRAM_DEST = 0,// Only SIE output to DRAM
    OUT_PRE_DEST = 1,// SIE & PRE Direct Path (PRE output to DRAM)
    OUT_PINGPONGBUF_DEST = 2 // SIE & PRE PingPong Buf (SIE & PRE output to DRAM)
}SIE_OutDestEnum;

typedef enum
{
    SIE_OUT_PACK_12BIT = 0,
    SIE_OUT_NOT_USED1 = 1,
    SIE_OUT_PACK_8BIT = 2,
    SIE_OUT_PACK_16BIT = 3
}SIE_OutPackBitSelEnum;


typedef enum
{
    SieOut2DRAM = 0,
    SieOut2PRE = 1
}Sie2PrePathEnum;


typedef enum
{
    SieLoad_RisingEdge = 0,
    SieLoad_FallingEdge = 1
}SieLoadTimingEnum;

typedef enum
{
    VDLaggingHD = 0,
    VDLeadingHD = 1
}VDLeadingEnum;

typedef enum
{
    SieMS_LevelMode = 0,
    SieMs_PluseMode = 1
}SieMshtrModeEnum;

typedef enum
{
    SieMS_HighActive = 0,
    SieMS_LowActive = 1
}SieMshtrHighActEnum;

typedef enum
{
    SieMS_ActCurrentFrame = 0,
    SieMS_ActNextFrame = 1
}SieMshtrFrameDelayEnum;

///sie capture field sequence structure
typedef struct{
    UB    TotalFld; ///< 0: 1 field, 1: 2 field, 2: 3 field ¡K.7: 8 field
    UB    Fld1; ///< field num of the 1st field
    UB    Fld2; ///< field num of the 2nd field
    UB    Fld3; ///< field num of the 3rd field
    UB    Fld4; ///< field num of the 4th field
    UB    Fld5; ///< field num of the 5th field
    UB    Fld6; ///< field num of the 6th field
    UB    Fld7; ///< field num of the 7th field
    UB    Fld8; ///< field num of the 8th field
} STR_FLDNUM;
///sie division sequence structure
typedef struct{
    UH    DIV1;
    UH    DIV2;
    UH    DIV3;
    UH    DIV4;
    UH    DIV5;
} STR_DIV;

///sie sensor structure
typedef struct{
    BOOL Master; ///< VD generation 0:slave mode, 1:master mode
    BOOL VD_Phase; ///< VD latch 0: rising edge, 1:falling edge
    BOOL HD_Phase; ///< HD latch 0: rising edge, 1:falling edge
    BOOL Data_Phase; ///< Data latch 0: rising edge, 1:falling edge
} STR_PHASE;

///CCD active area starting position structure
typedef struct{
    UH    VH_Act; ///< H active
    UH    VV_Act1; ///< V field1 active
    UH    VV_Act2; ///< V field2 active
    UH    VV_Act3; ///< V field3 active
    UH    VV_Act4; ///< V field4 active
    UH    VV_Act5; ///< V field5 active
    UH    VV_Act6; ///< V field6 active
    UH    VV_Act7; ///< V field7 active
    UH    VV_Act8; ///< V field8 active
} STR_FLDACT;

///OB window and parameter structure
typedef struct{
    UH    X0; ///< left point
    UH    Y0; ///< top point
    UH    X1; ///< right point
    UH    Y1; ///< botton point
    UH    Thres; ///< if pixel greater than Thres, it will not take into account
    UH    SubSample; ///< H subsample 1/(1+subsample)
} STR_OB_Param;

typedef struct _SIE_OPENOBJ{
    void (*FP_SIEISR_CB)(UINT32 intstatus);
} SIE_OPENOBJ;

//#NT#2011/11/07#YHLee -begin
//#NT#Add DVI API
typedef enum
{
    DVI_CCIR_601 = 0,
    DVI_CCIR_656 = 1
} SIE_DVI_FMT;

typedef enum
{
    DVI_SD_MODE = 0, ///<8 bit data input
    DVI_HD_MODE = 1 ///<16 bit data input, please refer to TOP controller for DVI pin setting
} SIE_DVI_MODE;

typedef enum
{
    ACT_END_EAV_CODE = 0, ///< EAV code is recognized for the end of active region
    ACT_END_ACT_SET = 1 ///< ACT_WIDTH and ACT_HEIGHT settings are used to determine active region size
} SIE_DVI_656_ACT;

typedef struct _STR_SIE_DVI_INFO{
    SIE_DVI_FMT     DVIFormat; ///< Choose 601 or 656 input
    SIE_DVI_MODE    DVIMode; ///< Choose SD or HD input 
    SIE_DVI_656_ACT DVI656ActEnd; ///< Choose end of active recognition method
    BOOL            DVI656InYCSwap;
    BOOL            DVI656OutYCSwap;
    BOOL            DVI601FieldIn; ///< For interlaced even, odd field process
    BOOL            DVI601FieldInv; ///< For interlaced even, odd field process
} STR_SIE_DVI_INFO;
//#NT#2011/11/07#YHLee -end

typedef enum
{
    SIE_ENGINE_IDLE = 0,
    SIE_ENGINE_READY = 1,
    SIE_ENGINE_PAUSE = 2,
    SIE_ENGINE_RUN = 3
} SIE_ENGINE_STATUS;

typedef enum
{
    SIE_ENGINE_OPEN = 0,
    SIE_ENGINE_CLOSE = 1,
    SIE_ENGINE_SET2READY = 2,
    SIE_ENGINE_SET2PRV = 3,
    SIE_ENGINE_SET2CAP = 4,
    SIE_ENGINE_SET2PAUSE = 5,
    SIE_ENGINE_SET2RUN = 6
} SIE_ENGINE_OPERATION;

typedef struct STR_SIE_REG {
    SIE_ENGINE_OPERATION            SIE_Operation;
    SIE_PPB_HeightEnum              REG_ppb_buf_height;
    SIE_OutPackBitSelEnum           REG_sie_pre_pack_bus;
    SIE_OutBufNumEnum               REG_sie_outbuf_num;
    SIE_OutDestEnum                 REG_sie_output_dest;
    STR_FLDNUM                      REG_RemapFieldNum;
    STR_OB_Param                    REG_sie_OB_Param;
    STR_FLDACT                      REG_sie_FieldAct;
    STR_PHASE                       REG_sie_SyncPhase;
    STR_DIV                         REG_hd_DivisionSeq;
    STR_HDVD                        REG_HDVD_Sync;
    STR_SIEOUT_SIZE                 REG_Sie_OutSize;
    UINT32                          REG_hd_DivisionNum;
    UINT32                          REG_siepre_ppb0_addr;
    UINT32                          REG_siepre_ppb1_addr;
    UINT32                          REG_siepre_ppb2_addr;
    UINT32                          REG_sie_intrp_en;
    UINT32                          REG_sie_intrp_bp1;
    UINT32                          REG_sie_intrp_bp2;
    BOOL                            REG_sie_hd_inv;
    BOOL                            REG_sie_vd_inv;
    BOOL                            AutoObEn;
    UINT32                          OBOfs;
    UINT32                          OBGain;
}STR_SIE_REG;



extern STR_FLDNUM sie_FieldNum;
extern STR_PHASE sie_SyncPhase;
extern STR_FLDACT sie_FieldAct;
extern STR_OB_Param sie_OB_Param;


extern ER sie_open(SIE_OPENOBJ *pObjCB);
extern ER sie_close(void);
extern ER sie_pause(void);

extern ER sie_start(void);

extern ER sie_setMode(STR_SIE_REG *SIE_PARAM);
extern ER sie_changeSync(STR_HDVD HDVD_Sync,STR_FLDACT FldAct,STR_SIEOUT_SIZE SIE_Output);
extern void sie_waitVD(UINT32 count);
extern void sie_waitBP1(BOOL bClrFlag);
extern void sie_waitBP2(BOOL bClgFlag);
extern void sie_waitFlashDone(BOOL bClrFlag);


extern void sie_reset(BOOL reset); // or use define
BOOL sie_isEnabled(void);
extern void sie_load(void); // or use define
extern void sie_Cap2Dram(void); // or use define
extern void sie_CaptureFldnum(UB mode);
extern void sie_FlashStart(void); // or use define
extern void sie_MshtrStart(void); // or use define //1~6 : 0x00
extern void sie_setActWindow( UW SizeX, UW SizeY);// 0x30~0x34
extern void sie_setSensorSync(UW HD_Width, UW H_Total, UW VD_Width, UW V_Total);//V need to x2,UINT:half line // 0x08~ 0x1c
extern void sie_IntrptEnable(BOOL Int_enable, UW sie_Intrpt); // 0x10
extern void sie_FunctionEnable(BOOL Fnc_enable,UW sie_Function); // 0x04
BOOL sie_checkFunctionEnable(UW sie_Function); // 0x04
UW sie_readIntrStatus(void); // 0x14
extern void sie_clrIntrStatus(UW status); // 0x14
extern void sie_CaptureFldNum(UB mode);
extern void sie_setSyncPhase(STR_PHASE sie_SyncPhase); // 0x04
extern void sie_VDLeading(VDLeadingEnum Leading, UW Cycle); // 0x20
extern void sie_setBreakPoint(UH BP1, UH BP2); //0x0c
UW sie_readCurrentOutputAdd(void); // 0x2c
UW sie_getDramSao(unsigned char channel); // 0x38, 0x40
extern void sie_setOutputStartAdd(UW Add); // 0x38
extern void sie_setOutputStartAdd1(UW Add);
extern void sie_setLineOffset(UH LineOffset); // 0x3c
extern void sie_setPackMode(SIE_OutPackBitSelEnum mode); // 0: 12, 1: 10, 2: 8, 3: 16 bit  10 & 12 bit use MSB // 0x3c
extern void sie_setBufferSize(SIE_PPB_HeightEnum BufferSize);
extern void sie_Division(UB mode, STR_DIV Divisionnum);
extern void sie_setSie2PrePath(Sie2PrePathEnum Path);
extern void sie_setPingPong(SIE_OutBufNumEnum num);
extern void sie_setLoadTiming(SieLoadTimingEnum vd_edge); // 0: rising edge, 1: falling edge // 0x04
extern void sie_setFieldAct(STR_FLDACT FieldAct);
extern void sie_setOBWindow (STR_OB_Param sie_OB_Param ); // 0x60~0x68
extern void sie_getOB(UH *OB_Avg, UH *OB_Cnt); // 0x6c
extern void sie_setOBGainOffset(UH OB_Offset, UH OB_Gain); //0x70
extern void sie_FlashSetting(BOOL HighAct, UH LineDelay, UH Pixel2Delay, UW FlashWidth);// 0xd0~0xd4
extern void sie_MshtrSetting(SieMshtrModeEnum mode, SieMshtrHighActEnum HighAct, SieMshtrFrameDelayEnum FrameDelay,UW LineDelay, UW Pixel2Delay, UW MshtrWidth, UW CloseField, BOOL PinSwap);
extern void sie_SensorDataEnable(BOOL enable);
//#NT#2008/12/17#BowenLi -begin
//#NT#Add sie_HDVDPhaseInverse API
extern void sie_HDVDPhaseInverse(UB HD_Inv, UB VD_Inv);
//#NT#2008/12/17#BowenLi -end
extern void sie_setSie2DramBufNum(UB num);
extern volatile unsigned sie_fc;
UW sie_readPixelCounterStatus(void);
//#NT#2008/07/03#YuppieLin -begin
//#NT#Add patgen API
extern void sie_PatGen(UB Mode, UW Value);
//#NT#2008/07/03#YuppieLin -end
extern void sie_rstSAC(void);

//#NT#2009/03/31#Harry Tsai -begin
extern BOOL sie_checkBusy(void);
//#NT#2009/03/31#Harry Tsai -end

//#NT#2011/11/07#YHLee -begin
//#NT#Add DVI API
extern void sie_setDVI(STR_SIE_DVI_INFO *SIE_DVI_PARAM);
//#NT#2011/11/07#YHLee -end

#endif

//@}
