/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2004.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: SIE Library Module                                                    *
* Description:                                                             *
* Author: Harry                                                                 *
****************************************************************************/

/** \addtogroup mIIPPSIE */
//@{

#ifndef _PRE_LIB_H
#define _PRE_LIB_H

#include "Type.h"
#include "sie_lib.h"

#define PRE_DEBUG   0
#define PRE_DEBUG_INFO  0

#define PRE_BLOCK_BUF_SIZE   (24 * 4) //byte

#define pre_Reset                   pre_reset
#define pre_RstSac                  pre_rstSac
#define pre_Load                    pre_load
#define pre_SetInputSize            pre_setInputSize
#define pre_SetVcropSize            pre_setVcropSize
#define pre_SetHcropSize            pre_setHcropSize
#define pre_SetOutputSize           pre_setOutputSize
#define pre_SetPre2DramPath         pre_setPre2DramPath
#define pre_SetFldNum               pre_setFldNum
#define pre_CheckFunctionEnable     pre_checkFunctionEnable
#define pre_CheckBusy               pre_checkBusy
#define pre_ReadIntrStatus          pre_readIntrStatus
#define pre_ClrIntrStatus           pre_clrIntrStatus
#define pre_SetInputPackMode        pre_setInputPackMode
#define pre_SetOutputPackMode       pre_setOutputPackMode
#define pre_SetOutputStartAdd       pre_setOutputStartAdd
#define pre_SetOutputStartAdd1      pre_setOutputStartAdd1
#define pre_SetInputLineOffset      pre_setInputLineOffset
#define pre_SetInputLineOffsetCh2   pre_setInputLineOffsetCh2
#define pre_SetBufferSize           pre_setBufferSize
#define pre_SetOutputLineOffset     pre_setOutputLineOffset
#define pre_SetInputStartAdd        pre_setInputStartAdd
#define pre_SetInputStartAdd1       pre_setInputStartAdd1
#define pre_SetInputStartAdd2       pre_setInputStartAdd2
#define pre_SetHScaling             pre_setHScaling
#define pre_SetHBining              pre_setHBining
#define pre_SetColorInfo            pre_setColorInfo
#define pre_GetColorInfo            pre_getColorInfo
#define pre_SetVaccParam            pre_setVaccParam
#define pre_GetVaccValue            pre_getVaccValue
#define pre_GetVaccNum              pre_getVaccNum
#define pre_GetCropHIni             pre_getCropHIni
#define pre_GetCropVIni             pre_getCropVIni
#define pre_ReadCurrentInAdd        pre_readCurrentInAdd
#define pre_GetSaiAdd               pre_getSaiAdd
#define pre_ReadCurrentInBufferID   pre_readCurrentInBufferID
#define pre_ReadCurrentOutAdd       pre_readCurrentOutAdd
#define pre_GetSaoAdd               pre_getSaoAdd
#define pre_ReadCurrentOutBufferID  pre_readCurrentOutBufferID
#define pre_SetBreakPoint           pre_setBreakPoint
#define pre_SetDfSubOp              pre_setDfSubOp
#define pre_SetDfSubTh              pre_setDfSubTh
#define pre_SetVigPosition          pre_setVigPosition
#define pre_SetVigParam             pre_setVigParam
#define pre_SetVigLut               pre_setVigLut
#define pre_SetGeoPosition          pre_setGeoPosition
#define pre_SetGeoParamNormGain     pre_setGeoParamNormGain
#define pre_SetGeoParamDist         pre_setGeoParamDist
#define pre_SetGeoParamCorrGain     pre_setGeoParamCorrGain
#define pre_SetGeo                  pre_setGeo
#define pre_SetGamma                pre_setGamma
#define pre_SetCfa                  pre_setCfa
#define pre_SetLinearShadingParam   pre_setLinearShadingParam
#define pre_SetLinearShadingLut     pre_setLinearShadingLut
#define pre_ClearAllMode            pre_clearAllMode
#define pre_WaitFlagFieldEnd        pre_waitFlagFieldEnd
#define pre_SetGainCtrlA            pre_setGainCtrlA
#define pre_SetGainCtrlB            pre_setGainCtrlB

#define PRE_SRC_SEL                         0x00000003  //bit 0:1
#define PRE_BLM_EN                          0x00000100  //bit 8
#define PRE_DF_EMBED_EN                     0x00000200  //bit 9
#define PRE_HSCL_EN                         0x00000400  //bit 10
#define PRE_CG_EN                           0x00000800  //bit 11
#define PRE_GAINCTRL_EN                     0x00001000  //bit 12
#define PRE_GAMMA_EN                        0x00002000  //bit 13
#define PRE_GEO_EN                          0x00004000  //bit 14
#define PRE_VACC_EN                         0x00008000  //bit 15
#define PRE_HINTLV_EN                       0x00010000  //bit 16
#define PRE_HINTLV_SEL                      0x00020000  //bit 17
#define PRE_PADDING_EN                      0x00040000  //bit 18
#define PRE_HFLIP_EN                        0x00200000  //bit 21
#define PRE_VFLIP_EN                        0x00400000  //bit 22
#define PRE_VIG_EN                          0x00800000  //bit 23
#define PRE_CA_EN                           0x01000000  //bit 24
#define PRE_HINTLV_OUTEN                    0x02000000  //bit 25
#define PRE_DF_SUB_EN                       0x08000000  //bit 27
#define PRE_DF_SUB_OP_SEL                   0x30000000  //bit 28:29
#define PRE_DP_CNTRST_EN                    0x80000000  //bit 31


#define PRE_INTE_FLDEND                     0x00000004  //bit 2
#define PRE_INTE_LNEDN                      0x00000008  //bit 3
#define PRE_INTE_LN16END                    0x00000010  //bit 4
#define PRE_INTE_BP1                        0x00000020  //bit 5
#define PRE_INTE_OVFL1                      0x00000080  //bit 7
#define PRE_INTE_DPCF                       0x00001000  //bit 12
#define PRE_INTE_DRAM1_END                  0x00002000  //bit 13
#define PRE_INTE_VA                         0x00008000  //bit 15
#define PRE_INTE_CROPST                     0x00010000  //bit 16
#define PRE_INTE_CROPEND                    0x00020000  //bit 17
#define PRE_INTE_DRAM2_FAIL                 0x00080000  //bit 19
#define PRE_INTE_FRMEND                     0x00100000  //bit 20
#define PRE_INTE_OVFL2                      0x00200000  //bit 21
#define PRE_INTE_ALL                        0x0003ffff

#define PRE_INT_FLDEND                      0x00000004  //bit 2
#define PRE_INT_LNEDN                       0x00000008  //bit 3
#define PRE_INT_LN16END                     0x00000010  //bit 4
#define PRE_INT_BP1                         0x00000020  //bit 5
#define PRE_INT_OVFL1                       0x00000080  //bit 7
#define PRE_INT_DPCF                        0x00001000  //bit 12
#define PRE_INT_DRAM1_END                   0x00002000  //bit 13
#define PRE_INT_VA                          0x00008000  //bit 15
#define PRE_INT_CROPST                      0x00010000  //bit 16
#define PRE_INT_CROPEND                     0x00020000  //bit 17
#define PRE_INT_GAMFAIL                     0x00040000  //bit 18
#define PRE_INT_DRAMI2_FAIL                 0x00080000  //bit 19
#define PRE_INT_FRMEND                      0x00100000  //bit 20
#define PRE_INT_OVFL2                       0x00200000  //bit 21
#define PRE_INT_ALL                         0x003fffff

typedef struct{
    UH    Hsize; ///< H active
    UH    Vsize; ///< V field1 active
    UH    lineoft;
} STR_PREIN_SIZE;


typedef enum
{
    PPB_8LINES,
    PPB_16LINES,
    PPB_32LINES,
    PPB_64LINES,
    PPB_96LINES,
    PPB_128LINES,
    PPB_192LINES,
    PPB_FULL_HEIGHT
}PRE_PPB_HeightEnum;

typedef enum
{
    PRE_INPACK_12BIT,
    PRE_INNOT_USED1,
    PRE_INPACK_8BIT,
    PRE_INPACK_16BIT
}PRE_InPackBitSelEnum;

typedef enum
{
    PRE_OUTPACK_12BIT,
    PRE_OUTPACK_10BIT,
    PRE_OUTPACK_8BIT,
    PRE_OUTPACK_16BIT
}PRE_OutPackBitSelEnum;

typedef enum
{
    PATH_DRAM_INPUT,
    PATH_SIE_DIRECT_INPUT,
    PATH_SIE_PINGPONG_INPUT
}PRE_InputSourceSelEnum;

typedef enum
{
    PATH_PRE_CLOLRGAIN,
    PATH_PRE_GAMMA,
    PATH_POST_GAMMA
}PRE_OutputPathEnum;

typedef enum
{
    SENSOR_OUT_PROGRESSIVE,
    SENSOR_OUT_TWO_LINE,
    SENSOR_OUT_THREE_LINE
} PRE_VGAFormatEnum;

typedef enum
{
    PRE_ENGINE_IDLE = 0,
    PRE_ENGINE_READY = 1,
    PRE_ENGINE_PAUSE= 2,
    PRE_ENGINE_RUN = 3
} PRE_ENGINE_STATUS;

typedef enum
{
    PRE_ENGINE_OPEN = 0,
    PRE_ENGINE_CLOSE = 1,
    PRE_ENGINE_SET2READY = 2,
    PRE_ENGINE_SET2PRV = 3,
    PRE_ENGINE_SET2CAP = 4,
    PRE_ENGINE_SET2PAUSE = 5,
    PRE_ENGINE_SET2RUN = 6,
    PRE_ENGINE_SET2D2D = 7
} PRE_ENGINE_OPERATION;


typedef enum
{
    PreCfa_R = 0,
    PreCfa_Gr = 1,
    PreCfa_Gb = 2,
    PreCfa_B = 3
}PreCfa;

//PRE CG information
typedef struct _STR_PRE_CG_INFOR{
    UH OfsR;
    UH OfsGr;
    UH OfsGb;
    UH OfsB;
    UH GainR;
    UH GainGr;
    UH GainGb;
    UH GainB;
    UH GainSel;
}STR_PRE_CG_INFOR;

//PRE VIG information
typedef struct _STR_PRE_VIG_INFOR{
    UH VIG_XR;
    UH VIG_YR;
    UH VIG_XGR;
    UH VIG_YGR;
    UH VIG_XGB;
    UH VIG_YGB;
    UH VIG_XB;
    UH VIG_YB;
    UB VIG_MUL;
    UH VIG_XDIV;
    UH VIG_YDIV;
    UB VIG_YDIST;
    UB VIG_T;
    UB *VIG_R_LUT;
    UB *VIG_G_LUT;
    UB *VIG_B_LUT;
}STR_PRE_VIG_INFOR;

//PRE Linear VIG information
typedef struct _STR_PRE_VIGLINE_INFOR{
    UB Xcf;
    UB Ycf;
    UB Scale;
    UH Dist_T;
    UH XSub;
    UB XInv;
    UB *VIG_R2_LUT;
    UB *VIG_G2_LUT;
    UB *VIG_B2_LUT;
}STR_PRE_VIGLINE_INFOR;

//PRE GEO information
typedef struct _STR_PRE_GEO_INFOR{
    UH GEO_CENTX;
    UH GEO_CENTY;
    UW GEO_CENTXS;
    UW GEO_CENTYS;
    UH GEO_NORMFACT;
    UH GEO_NORMBIT;
    UH GEO_RLUTGAIN;
    UH GEO_GLUTGAIN;
    UH GEO_BLUTGAIN;
    UH GEO_DISTSEL;
    UH GEO_XDIST;
    UH GEO_YDIST;
    UH GEO_CORREN;
    UH GEO_CORRNORM;
    UH GEO_RCORRGAIN;
    UH GEO_GRCORRGAIN;
    UH GEO_GBCORRGAIN;
    UH GEO_BCORRGAIN;
    UH *GEO_Lut;
}STR_PRE_GEO_INFOR;

typedef struct _STR_PRE_VA_INFOR{
    unsigned long    stx;///<VA starting x
    unsigned long    sty;///<VA starting y
    unsigned long    vacc_mode;///<VA mode
    unsigned long    vnf;///<VA
    unsigned long    hnf;///<VA hnf
    unsigned long    nnf;///<VA hnf
    unsigned long    hsz;///<VA hsz
    unsigned long    vsz;///<VA vsz
    unsigned long    va_th;///<VA threshold
}STR_PRE_VA_INFOR;

//PRE GEO information
typedef struct _STR_PRE_DFS_INFOR{
    UW DFS_OP;
    UW DFS_TH;
    UW DFS_THSRC;
}STR_PRE_DFS_INFOR;

typedef struct _STR_PRE_GAINCTRL_INFOR{
    UW MapA;
    UW MapB;
    UH GainA;
    UH GainB;
    UH Switch;
    UH OffsetA;
    UH OffsetB;
}STR_PRE_GAINCTRL_INFOR;

typedef struct _PRE_OPENOBJ{
    void (*FP_PREISR_CB)(UINT32 intstatus);
} PRE_OPENOBJ;

//#NT#2009/10/13#Jarkko Chang -begin
typedef struct _STR_PRE_CA_INFOR{
    UINT32    pre_CA_OutputAdd;
    UINT32    pre_CA_OutputPtah;
    UINT32    pre_CA_Hsub;
    UINT32    pre_CA_Vsub;
    UINT32    pre_CA_OutputLineofs;
}STR_PRE_CA_INFOR;
//#NT#2009/10/13#Jarkko Chang -end

//#NT#2009/10/29#Chris Yang -begin
//#NT# Add to support ECS
typedef struct _STR_PRE_ECS_INFOR{
    UINT32 pre_input2_dram_addr;
    UINT32 pre_input2_lineoft;
    UINT32 pre_map_Hsize;
    UINT32 pre_map_Vsize;
    UINT32 pre_map_Hscale;
    UINT32 pre_map_Vscale;
    UINT32 pre_map_shift;
} STR_PRE_ECS_INFOR, *PSTR_PRE_ECS_INFOR;
//#NT#2009/10/29#Chris Yang -end

typedef enum
{
    PreCA_Subsample_1_2 = 0,
    PreCA_Subsample_1_4 = 1,
    PreCA_Subsample_1_8 = 2
}PreCA_SubsampleRate;


typedef struct {
    PRE_ENGINE_OPERATION            PRE_Operation;
    UINT32                          PRE_EnFunction;
    PRE_VGAFormatEnum               PRE_VGAType;
    PRE_PPB_HeightEnum              REG_ppb_buf_height;
    PRE_InPackBitSelEnum            REG_pre_input_pack_bus;
    PRE_OutPackBitSelEnum           REG_pre_output_pack_bus;
    PRE_InputSourceSelEnum          REG_pre_input_source;
    PRE_OutputPathEnum              REG_pre_output_path;
    STR_FLDNUM                      REG_RemapFieldNum;
    STR_DIV                         REG_hd_DivisionSeq;
    UINT32                          REG_hd_DivisionNum;
    STR_PREIN_SIZE                  REG_pre_input_Size;
    UINT32                          REG_pre_vv_ini;
    UINT32                          REG_pre_vv_height;
    UINT32                          REG_pre_hv_ini;
    UINT32                          REG_pre_hv_width;
    UINT32                          REG_pre_in_ppb0_addr;
    UINT32                          REG_pre_in_ppb1_addr;
    UINT32                          REG_pre_in_ppb2_addr;
    UINT32                          REG_pre_input_lineoft;
    UINT32                          REG_pre_out_ppb0_addr;
    UINT32                          REG_pre_out_ppb1_addr;
    UINT32                          REG_pre_output_size;
    UINT32                          REG_pre_output_lineoft;
    UINT32                          REG_pre_intrp_en;
    UINT32                          REG_pre_bp1;
    UINT32                          REG_pre_CfaPat;
    UINT32                          REG_pre_CfaPat2;
    STR_PRE_GAINCTRL_INFOR          pre_gainctrl_A;
    STR_PRE_GAINCTRL_INFOR          pre_gainctrl_B;
    STR_PRE_CG_INFOR                pre_cg_ofs;
    STR_PRE_VIG_INFOR               pre_vig_setting;
    STR_PRE_VIGLINE_INFOR           pre_vigline_setting;
    STR_PRE_GEO_INFOR               pre_geo_setting;
    STR_PRE_DFS_INFOR               pre_dfs_setting;
//#NT#2009/10/29#Chris Yang -begin
//#NT# Add to support ECS
    STR_PRE_ECS_INFOR               pre_ecs_setting;
//#NT#2009/10/29#Chris Yang -end
    UINT32                          *pre_gamma_r;
    UINT32                          *pre_gamma_g;
    UINT32                          *pre_gamma_b;
    STR_PRE_VA_INFOR                *VaccIn;
    UINT32                          pre_CA_OutputAdd;
    UINT32                          pre_CA_OutputLineofs;
    PRE_OutputPathEnum              pre_CA_OutputPtah;
    PreCA_SubsampleRate             pre_CA_Hsub;
    PreCA_SubsampleRate             pre_CA_Vsub;
}STR_PRE_REG;



extern ER pre_open(PRE_OPENOBJ *pObjCB);
extern ER pre_close(void);
extern ER pre_pause(void);
extern ER pre_start(void);
extern ER pre_setMode(STR_PRE_REG *PRE_PARAM);
extern void pre_load(void);
extern void pre_en(BOOL reset);
extern void pre_reset(BOOL reset);
extern void pre_setInputSize(UW HSIZEIN, UW VSIZEIN);
extern void pre_setVcropSize(UW VV_INI, UW VV_HEIGHT);
extern void pre_setHcropSize(UW HV_INI, UW HV_WIDTH);
extern void pre_setOutputSize(UW HSIZEOUT );
extern void pre_setFldNum(STR_FLDNUM        pre_FieldNum);
extern void pre_IntrptEnable(BOOL Int_enable, UW pre_Intrpt);
extern void pre_FunctionEnable(BOOL Fnc_enable,UW pre_Function);
extern BOOL pre_checkFunctionEnable(UW pre_Function);
extern BOOL pre_checkBusy(void);
extern UW pre_readIntrStatus(void);
extern void pre_clrIntrStatus(UW status);
extern void pre_Cap2Dram(void);
extern void pre_setInputPackMode(PRE_InPackBitSelEnum mode);
extern void pre_setOutputPackMode(PRE_OutPackBitSelEnum mode);
extern void pre_setOutputStartAdd(UW Add);
extern void pre_Division(UB mode);
extern void pre_setDfSubOp(UW op_sel);
extern void pre_setDfSubTh(UW DFS_TH,UW DFS_THSRC);
extern UW pre_readCurrentOutBufferID(void);

extern void pre_SieDirectPre(PRE_InputSourceSelEnum path_sel);
extern void pre_setInputLineOffset(UW LineOffset);
extern void pre_setInputLineOffsetCh2(UW LineOffset);
extern void pre_setPre2DramPath(PRE_OutputPathEnum Path);
extern void pre_setOutputLineOffset(UW LineOffset);
extern void pre_setInputStartAdd(UW Add);
extern void pre_setInputStartAdd1(UW Add);
extern void pre_setInputStartAdd2(UW Add);
extern void pre_setVigPosition(UH VIG_XR, UH VIG_YR, UH VIG_XGR, UH VIG_YGR, UH VIG_XGB, UH VIG_YGB, UH VIG_XB, UH VIG_YB);
extern void pre_setVigParam(UB VIG_MUL, UH VIG_XDIV, UH VIG_YDIV, UB VIG_YDIST, UB VIG_T); // 0xf8
extern void pre_setVigLut(UB *VIG_R_LUT,UB *VIG_G_LUT,UB *VIG_B_LUT);
extern void pre_setGamma(UW *pre_Gamma_R,UW *pre_Gamma_G,UW *pre_Gamma_B);
extern void pre_setColorInfo(STR_PRE_CG_INFOR *CG_Info);
extern void pre_getColorInfo(STR_PRE_CG_INFOR *CG_Info);
extern void pre_setVaccParam(STR_PRE_VA_INFOR *VaccInfo);
extern void pre_getVaccValue(UW *VaccValue);
extern void pre_getVaccNum(UW *VaccNum);
extern UW pre_readCurrentInBufferID(void);
extern UW pre_getSaoAdd(UW BufID);
extern UW pre_getCropHIni(void);
extern UW pre_getCropVIni(void);


extern void pre_setCfa(PreCfa VIG_CFA,PreCfa Gamma_CFA);
extern void pre_setLinearShadingParam(UB Xcf,UB Ycf,UB Scale,UH Dist_T,UH XSub, UB XInv);
extern void pre_setLinearShadingLut(UB *VIG_R2_LUT,UB *VIG_G2_LUT,UB *VIG_B2_LUT);
extern void pre_setGeoPosition(UH GEO_CENTX, UH GEO_CENTY, UW GEO_CENTXS, UW GEO_CENTYS);
extern void pre_setGeoParamNormGain(UH GEO_NORMFACT, UH GEO_NORMBIT, UH GEO_RLUTGAIN, UH GEO_GLUTGAIN, UH GEO_BLUTGAIN); //
extern void pre_setGeoParamDist(UH GEO_DISTSEL, UH GEO_XDIST, UH GEO_YDIST);
extern void pre_setGeoParamCorrGain(UH GEO_CORREN, UH GEO_CORRNORM, UH GEO_RCORRGAIN, UH GEO_GRCORRGAIN, UH GEO_GBCORRGAIN, UH GEO_BCORRGAIN); //
extern void pre_setGeo(UH *pre_Geo_Lut);
extern void pre_setBreakPoint(UW BP1);
extern void pre_HScale(unsigned short InputH, unsigned short OutputH);
extern void pre_OutputPpbEn(UW PPB_En);
extern void pre_setOutputStartAdd1(UW Add);
extern void pre_rstSac(void);
extern void pre_clearAllMode(void);
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add new API for Dzoom
extern void pre_clrFieldEnd(void);
extern void pre_waitFieldEnd(BOOL bClrFlag);
#if 0
//#NT#2009/10/30#Harry Tsai -begin
//#NT#Sync to NT96450 driver
extern void pre_waitFlagFieldEnd(BOOL bClrFlag);
extern void pre_clrFieldEnd(void);
//#NT#2009/10/30#Harry Tsai -end
#endif
//#NT#2009/11/27#Connie Yu -end
extern void pre_setHBining(UB pixel_add, UB value_div);
void pre_setGainCtrlA(STR_PRE_GAINCTRL_INFOR *GainCtrl);
void pre_setGainCtrlB(STR_PRE_GAINCTRL_INFOR *GainCtrl);
extern void pre_setCAOutputAdd(UW CA_Add);
extern void pre_setCAOutputParam(PRE_OutputPathEnum CA_Path, UW H_Sub, UW V_Sub, UW DramLineOfs);

//#NT#2009/10/29#Chris Yang -begin
//#NT# Add to support ECS
extern void pre_setECSSize(UINT32 hSize, UINT32 vSize);
extern void pre_setECSMap(UINT32 map_hscl, UINT32 map_vscl, UINT32 map_shift);
//#NT#2009/10/29#Chris Yang -end

//#NT#2009/11/26#Linkin Huang -begin
//#NT# add function declaration
extern ER pre_enableFunc(UINT32 Function, STR_PRE_REG *PRE_PARAM);
//#NT#2009/11/26#Linkin Huang -end

#endif
//@}
