/*
    IPE module driver

    NT96630 IPE driver extern header file.

    @file       Ipe_lib.h
    @ingroup    mIIPPIPE
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPE_LIB_H
#define _IPE_LIB_H

#define IPE_DEBUG   0
#define IPE_DEBUG_INFO   0

#define GetIpeVACnt ipe_getVACnt

/** \addtogroup mIIPPIPE */
//@{

/**
    @name Define IPE function enable.
*/
//@{
#define IPE_CGAIN_EN                   0x00000001  //bit 0
#define IPE_NR_EN                      0x00000002  //bit 1
#define IPE_RLPF_EN                    0x00000004  //bit 2
#define IPE_GLPF_EN                    0x00000008  //bit 3
#define IPE_BLPF_EN                    0x00000010  //bit 4
#define IPE_CCR_EN                     0x00000020  //bit 5
#define IPE_CTRL_EN                    0x00000080  //bit 7
#define IPE_HADJ_EN                    0x00000100  //bit 8
#define IPE_CADJ_EN                    0x00000400  //bit 10
#define IPE_CADJ_YENH_EN               0x00000800  //bit 11
#define IPE_CADJ_YLPF_EN               0x00001000  //bit 12
#define IPE_CADJ_YCON_EN               0x00002000  //bit 13
#define IPE_CADJ_CLPF_EN               0x00004000  //bit 14
#define IPE_CADJ_CCON_EN               0x00008000  //bit 15
#define IPE_VACC_EN                    0x00010000  //bit 16
#define IPE_PREGAMMA_EN                0x00040000  //bit 18
#define IPE_CHROMATH_EN                0x00080000  //bit 19
#define IPE_EDGETH_EN                0x00100000  //bit 20
//@}

/**
    @name Define IPE interrput enable.
*/
//@{
#define IPE_INTE_ST                    0x00000001  //bit 0
#define IPE_INTE_HOR                   0x00000002  //bit 1
#define IPE_INTE_FRM                   0x00000004  //bit 2
#define IPE_INTE_BP1                   0x00000020  //bit 5
#define IPE_INTE_DMAEND                0x00000040  //bit 6
#define IPE_INTE_IPESTART              0x00000080  //bit 7
#define IPE_INTE_CTHOUTEND              0x00000100  //bit 8
#define IPE_INTE_ETHOUTEND              0x00000200  //bit 9
#define IPE_INTE_SYNCDBG               0x00000400  //bit 10
#define IPE_INTE_ALL                   0x000007e7
//@}

/**
    @name Define IPE interrput status.
*/
//@{
#define IPE_INT_ST                     0x00000001  //bit 0
#define IPE_INT_HOR                    0x00000002  //bit 1
#define IPE_INT_FRM                    0x00000004  //bit 2
#define IPE_INT_BP1                    0x00000020  //bit 5
#define IPE_INT_DMAEND                 0x00000040  //bit 6
#define IPE_INT_IPESTART               0x00000080  //bit 7
#define IPE_INT_CTHOUTEND               0x00000100  //bit 8
#define IPE_INT_ETHOUTEND               0x00000200  //bit 9
#define IPE_INT_SYNCDBG                0x00000400  //bit 10
#define IPE_INT_ALL                    0x000007e7
//@}

/**
    @name Define IPE engine value.
*/
//@{
#define IPE_HSIZE_IODIFF     16

#define VAC_WIN8                        8
#define VAC_WIN16                       16
#define IPE_VAC_HNF_MINIMUN             2
#define IPE_VACC_VALUE_MSK              0x0003ffff
#define IPE_VACC_NUM_MSK                0xfffc0000
#define IPE_MAXVAWORD      512//16*16*2

#define IPE_MAX_COLOR_OFFSET    255
#define IPE_MAX_COLOR_GAIN      1023
//@}

/**
    @name Define IPE function setting selection.
*/
//@{
#define IPE_SET_NONE               0x00000000
#define IPE_SET_GAMMA               0x00000001
#define IPE_SET_NR               0x00000002
#define IPE_SET_EDGE               0x00000004
#define IPE_SET_COLOR               0x00000008
#define IPE_SET_COLOR_GAIN               0x00000010
#define IPE_SET_COLOREFFECT               0x00000020
#define IPE_SET_INVERSEEFFECT               0x00000040
#define IPE_SET_CTH               0x00000400
#define IPE_SET_ETH               0x00000800
#define IPE_SET_VA               0x80000000
#define IPE_SET_ALL               0xffffffff
//@}

/**
    @name Enum IPE input control.
*/
//@{
typedef enum
{
    IPE_PATR = 0,
    IPE_PATGR = 1,
    IPE_PATGB = 2,
    IPE_PATB = 3
}IPE_CfaPatEnum;

typedef enum
{
    IPE_PACK_12bit = 0,
    IPE_PACK_10bit = 1,
    IPE_PACK_08bit = 2,
    IPE_PACK_16bit = 3
}IPE_PackBitEnum;

typedef enum
{
    PPBINI_0 = 0,
    PPBINI_1 = 1,
    PPBINI_PRE = 2
}IPE_InPPBIniEnum;

typedef enum
{
    SYNC_CROP_END = 0,
    SYNC_AUTO = 1
}IPE_InSyncSelEnum;
//@}

/**
    @name Enum IPE operation mode.
*/
//@{
typedef enum
{
    SINGLE_STRIPE_MODE = 0,
    AUTO_SLICE_MODE = 1,
    AUTO_FRAME_MODE = 2,
    DVI_MODE = 3
}IPE_StripeEnum;

typedef enum
{
    IPE_LINEX3 = 0,
    IPE_LINEX5 = 1,
    IPE_LINEX9 = 2,
    IPE_LINEXRESERVED = 3
}IPE_LbufEnum;
//@}

/**
    @name Enum IPE DVI.
*/
//@{
typedef enum
{
    DVI_CBYCRY = 0,
    DVI_CRYCBY = 1,
    DVI_YCRYCB = 2,
    DVI_YCBYCR = 3
}IPE_DVIFormatInEnum;

typedef enum
{
    DVI_HSUB1 = 0,
    DVI_HSUB2 = 1,
    DVI_HSUB4 = 2
}IPE_DVIHSubOutEnum;
//@}

/**
    @name Enum IPE output control.
*/
//@{
typedef enum
{
    IPE_YCC444 = 0,
    IPE_YCC422 = 1,
    IPE_YCC420 = 2
}IPE_OmatEnum;

typedef enum
{
    YCC_DROP_RIGHT = 0,
    YCC_DROP_LEFT = 1
}IPE_SubHSelEnum;

typedef enum
{
    LOWPASS = 0,
    MEDIAN = 1
}IPE_CbCrLMFSelEnum;

typedef enum
{
    YDATA_TYPE = 0,
    CBDATA_TYPE = 1,
    CRDATA_TYPE = 2,
    VADATA_TYPE = 3,
    POST_NR_DATA_TYPE = 4
}IPE_DramOutDataTypeEnum;
//@}

/**
    @name Enum IPE color channel.
*/
//@{
typedef enum
{
    IPE_R_CHANNEL = 0,
    IPE_G_CHANNEL = 1,
    IPE_B_CHANNEL = 2
}IPE_ChannelEnum;
//@}

/**
    @name Enum IPE overlap enable.
*/
//@{
typedef enum
{
    IPE_OLAP_OFF = 0,
    IPE_OLAP_ON = 1
}IPE_OlapEnum;
//@}

/**
    @name Enum IPE chroma threshold param.
*/
//@{
typedef enum
{
    IPE_CHROMA_UPDATE_AUTO = 0,
    IPE_CHROMA_UPDATE_MANUAL = 1
}IPE_CthModeEnum;

typedef enum
{
    IPE_CHROMA_PRECOMP = 0,
    IPE_CHROMA_POSTCOMP = 1
}IPE_CthSrcEnum;
//@}

/**
    @name Enum IPE VA param.
*/
//@{
typedef enum
{
    VDET_EVAR = 0,
    VDET_DIFF = 1,
    EEXT_ABS = 2
}IPE_VASrcEnum;

typedef enum
{
    WINDOWS_8X8 = 0,
    WINDOWS_16X16 = 1
}IPE_VANumEnum;

typedef enum
{
    COMBINED_HV = 0,
    SEPARATE_HV = 1
}IPE_VAHVEnum;

typedef enum
{
    VACC_AUTO_UPDATED = 0,
    VACC_MAN_UPDATED = 1
}IPE_VAModeEnum;
//@}

/**
    @name Enum IPE edge param.
*/
//@{
typedef enum
{
    HVOPT_ABSAVG = 0,
    HVOPT_ABSMAX = 1,
    HVOPT_AVGABS = 2
}IPE_EthHVOptEnum;

typedef enum
{
    HVOPT_H = 0,
    HVOPT_MAX = 1,
    HVOPT_AVG = 2,
    HVOPT_V = 3
}IPE_EimgHVOptEnum;
//@}

/**
    @name Enum IPE wait frame end selection.
*/
//@{
typedef enum
{
    IPE_NOCLRFLG = 0,
    IPE_CLRFLG = 1
}IPE_FlgClrEnum;
//@}

/**
    @name Enum IPE stripe mode.
*/
//@{
typedef enum
{
    IPE_SSTP = 0,
    IPE_MSTP = 1,
    IPE_AUTOSTP = 2,
    IPE_DVIMODE = 3
}IPE_SripeModeEnum;
//@}

/**
    @name Enum IPE destinstion.
*/
//@{
typedef enum
{
    IPE_DEST_IME = 0,
    IPE_DEST_DRAM = 1
}IPE_DestEnum;
//@}

/**
    @name Enum IPE function setting mode.
*/
//@{
typedef enum
{
    IPE_FUNC_ENABLE = 0,
    IPE_FUNC_DISABLE = 1,
    IPE_FUNC_SET = 2,
    IPE_FUNC_NOUPDATE = 3
}IPE_FuncUpdateEnum;
//@}

/**
    @name Enum IPE AF window.
*/
//@{
typedef enum
{
    WIN8x8_HV_VALUE,
    WIN8x8_HV_NUM,
    WIN8x8_H_VALUE,
    WIN8x8_H_NUM,
    WIN8x8_V_VALUE,
    WIN8x8_V_NUM,
    WIN8x8_WEIGHT_VALUE,
    WIN8x8_WEIGHT_NUM,
    WIN16x16_HV_VALUE,
    WIN16x16_HV_NUM,
    WIN16x16_H_VALUE,
    WIN16x16_H_NUM,
    WIN16x16_V_VALUE,
    WIN16x16_V_NUM,
    WIN16x16_WEIGHT_VALUE,
    WIN16x16_WEIGHT_NUM
}AF_VASelEum;
//@}

/**
    @name Struct IPE size.
*/
//@{
typedef struct _STR_STRIPE_INFOR{
    UINT32    n;
    UINT32    l;
    UINT32    m;
    IPE_OlapEnum    olap;
}STR_STRIPE_INFOR;

typedef struct _STR_IMG_SIZE{
    UINT32    hsize;
    UINT32    vsize;
}STR_IMG_SIZE;
//@}

/**
    @name Struct IPE threshold.
*/
//@{
typedef struct _STR_TH_INFOR{
    UINT32    th_low;
    UINT32    th_high;
}STR_TH_INFOR;
//@}

/**
    @name Struct IPE VA.
*/
//@{
typedef struct _STR_VA_INFOR{
    IPE_VASrcEnum    vacc_src;
    UINT32    vacc_gain;
    UINT32    vacc_sqr;
    UINT32    stx;
    UINT32    sty;
    IPE_VAModeEnum    vacc_mode;
    IPE_VANumEnum    vacc_num;
    IPE_VAHVEnum    vacc_hv;
    UINT32    hnf;
    UINT32    hsz;
    UINT32    vsz;
    UINT32    va_th;
}STR_VA_INFOR;
//@}

/**
    @name Struct IPE color channel.
*/
//@{
typedef struct _STR_COLOR_INFOR{
    UINT32 ChR;
    UINT32 ChGr;
    UINT32 ChGb;
    UINT32 ChB;
}STR_COLOR_INFOR;
//@}

/**
    @name Struct IPE edge.
*/
//@{
typedef struct _STR_IPE_EDGE_ENHMAP_PARA{
    UINT32     EdgEnhance;
    UINT32     EthrA;
    UINT32     EthrB;
    UINT32     EtabA;
    UINT32     EtabB;
}STR_IPE_EDGE_ENHMAP_PARA;


typedef struct _STR_IPE_EDGE_KER_PARA{
    UINT32     Coeff0;
    INT16     Coeff1;
    INT16     Coeff2;
    INT16     Coeff3;
    INT16     Coeff4;
    INT16     Coeff5;
    INT16     Coeff6;
    INT16     Coeff7;
    INT16     Coeff8;
    UINT32     EdgeDiv;
    BOOL     EdgeSym;
}STR_IPE_EDGE_KER_PARA;
//@}

/**
    @name Struct IPE NR.
*/
//@{
typedef struct _STR_IPE_NOVANR2_PARA{
    UINT32      RBW;
    UINT32      GW;
    UINT32      GBAL;
    UINT32      OUTL;
    UINT32      OutlRbw;
    UINT32      OutlGw;
    UINT32      Honly;
    UINT32      Nsndir;
    UINT32      Nsvsb;
    UINT32      NsGain;
    UINT32      Nsmar;
    UINT32      Lpfw;
    UINT32      Sumsel;
    UINT32      Dthr;
    UINT16      Nthr[4];
}STR_IPE_NOVANR2_PARA;
//@}

/**
    @name Struct IPE CG CC.
*/
//@{
typedef enum
{
    IPE_CG_2_8 = 0,
    IPE_CG_3_7 = 1
}IPE_CGRangeEnum;

typedef struct _STR_IPE_COLOR_PARA{
    STR_COLOR_INFOR ColorOffset;
    STR_COLOR_INFOR ColorGain;
    IPE_CGRangeEnum CGRange;
    INT16            *ColorCorrect;
    UINT8   ColorSupWeight;
}STR_IPE_COLOR_PARA;
//@}

/**
    @name Struct IPE color effect.
*/
//@{
typedef struct
{
    UINT8 CbOfs;
    UINT8 CrOfs;
    UINT8 YCon;
    UINT8 CCon;
    INT16 IntOfs;
    INT16 SatOfs;
} IPE_COLOR_EFFECT_PARAM;
//@}

/**
    @name Struct IPE inv effect.
*/
//@{
typedef struct
{
    UINT32 Inv;
    UINT32 Hinv;
} IPE_INVERSE_EFFECT_PARAM;
//@}

/**
    @name Struct IPE chroma threshold.
*/
//@{
typedef struct
{
    IPE_CthModeEnum Mode;
    IPE_CthSrcEnum Src;
    STR_TH_INFOR *Cbth;
    STR_TH_INFOR *Crth;
} IPE_CTH_PARAM;
//@}

/**
    @name Struct IPE edge threshold.
*/
//@{
typedef struct
{
    IPE_EthHVOptEnum HVopt;
    UINT32 Eth;
} IPE_ETH_PARAM;
//@}

/**
    @name Struct IPE open object.
*/
//@{
typedef struct _IPE_OPENOBJ{
    void (*FP_IPEISR_CB)(UINT32 intstatus);
    UINT32 IPE_CLOCKSEL;
} IPE_OPENOBJ,*PIPE_OPENOBJ;
//@}

/**
    @name Struct IPE size info.
*/
//@{
typedef struct _IPE_SIZEINFO{
    IPE_SripeModeEnum StripeMode;
    STR_IMG_SIZE INSIZE;
    STR_STRIPE_INFOR HSTRIPE;
    STR_STRIPE_INFOR VSTRIPE;
    IPE_LbufEnum LbufNum;
    IPE_CfaPatEnum CfaPat;
    UINT32 LofsIn;
    UINT32 LofsOut;
    UINT32 LofsCth;
    UINT32 LofsEth;
} IPE_SIZEINFO,*PIPE_SIZEINFO;
//@}

/**
    @name Struct IPE input info.
*/
//@{
typedef struct _IPE_INPUTINFO{
    UINT32 PPBEn;
    IPE_InPPBIniEnum PPBIni;
    IPE_InSyncSelEnum SyncSel;
    UINT32 Adr0;
    UINT32 Adr1;
    IPE_PackBitEnum PackBit;
    //#NT#2011/08/24#YH Lee -begin
    //#NT#Add IPE DVI mode
    IPE_DVIFormatInEnum DVIFormat;
    IPE_DVIHSubOutEnum DVIHsub;
    //#NT#2011/08/24#YH Lee -end
} IPE_INPUTINFO,*PIPE_INPUTINFO;
//@}

/**
    @name Struct IPE output info.
*/
//@{
typedef struct _IPE_OUTPUTINFO{
    IPE_DramOutDataTypeEnum OutSrcSel;
    IPE_OmatEnum YCFormat;
    IPE_SubHSelEnum SubHSel;
    UINT32 Adr0;
    UINT32 AdrCth;
    UINT32 AdrEth0;
    UINT32 AdrEth1;
    UINT32 AdrEth2;
} IPE_OUTPUTINFO,*PIPE_OUTPUTINFO;
//@}

/**
    @name Struct IPE IQ info.
*/
//@{
typedef struct _IPE_IQINFO{
    UINT32 FuncSel;
    IPE_FuncUpdateEnum FuncUpdateSel;
    UINT32 ParamUpdateSel;
    STR_IPE_NOVANR2_PARA *NR_PARAM;
    UINT8 *NRStab;
    UINT8 *NRDtab;
    STR_IPE_EDGE_ENHMAP_PARA *EDGE_PARAM;
    STR_IPE_EDGE_KER_PARA  *EdgeKerH;
    STR_IPE_EDGE_KER_PARA  *EdgeKerV;
    UINT8  *Emap;
    UINT8  *Estab;
    STR_IPE_COLOR_PARA COLOR_PARAM;
    UINT8 *FStab;
    UINT8 *FDtab;
    IPE_COLOR_EFFECT_PARAM *COLOR_EFFECT_PARAM;
    UINT8 *HueTab;
    UINT8 *SatTab;
    UINT8 *IntTab;
    UINT8 *DDSTab;
    IPE_INVERSE_EFFECT_PARAM *INVERSE_EFFECT_PARAM;
    IPE_CTH_PARAM *CTH_PARAM;
    IPE_ETH_PARAM *ETH_PARAM;
    STR_VA_INFOR VA_PARAM;
    UINT32 *GammaLut[3];
} IPE_IQINFO,*PIPE_IQINFO;
//@}

/**
    @name Struct IPE mode info.
*/
//@{
typedef struct _IPE_MODEINFO{
    IPE_DestEnum OutDest;
    UINT32 IntEn;
    IPE_SIZEINFO SIZEINFO;
    IPE_INPUTINFO ININFO;
    IPE_OUTPUTINFO OUTINFO;
    IPE_IQINFO IQINFO;
} IPE_MODEINFO,*PIPE_MODEINFO;
//@}

/**
    @name Struct IPE AF info.
*/
//@{
typedef struct _AF_IPEINFO{
    IPE_VASrcEnum    vacc_src;
    UINT32    vacc_gain;
    UINT32    vacc_sqr;
    IPE_VANumEnum    vacc_num;
    UINT32    va_th;
    AF_VASelEum vacc_outsel;
    UINT32 TotalWeight;
    UINT32 HdirWeight;
} AF_IPEINFO,*PAF_IPEINFO;
//@}

/**
    @name Struct IPE extern info.
*/
//@{
typedef struct IPE_PARAM_STRUCT
{
    UINT32 InSizeH;
    UINT32 InSizeV;
    UINT32 InLineOfs;
    IPE_PackBitEnum BitDepth;
    IPE_CfaPatEnum CfaPat;
    UINT32 Add0;
    UINT32 Add1;
    UINT32 VAAddr;
//#NT#2010/04/30#Jarkko Chang -begin
//#NT#add for stripe mode change
    UINT32 StripeMode;
//#NT#2010/04/30#Jarkko Chang -end
    STR_VA_INFOR VaccInfo;
    STR_STRIPE_INFOR HStripe;
    STR_STRIPE_INFOR VStripe;
    IPE_OmatEnum OutFormat;
    IPE_DramOutDataTypeEnum OutSrcSel;
    IPE_DVIFormatInEnum DVIFormat;
    IPE_DVIHSubOutEnum DVIHsub;
} IPE_PARAM;
//@}

extern void ipe_clrFrameEnd(void);
extern void ipe_waitFrameEnd(IPE_FlgClrEnum ClrFlgSel);
extern void ipe_clrDMAEnd(void);
extern void ipe_waitDMAEnd(IPE_FlgClrEnum ClrFlgSel);
extern UINT ipe_getVACnt(void);
extern ER ipe_open(IPE_OPENOBJ *PIPE_OpenInfo);
extern ER ipe_close(void);
extern ER ipe_changeSize(IPE_SIZEINFO *HVsize);
extern ER ipe_changeParam(IPE_IQINFO *IQinfo);
extern ER ipe_changeInterrupt(UINT32 IntEn);
extern ER ipe_changeInput(IPE_INPUTINFO *ININFO);
extern ER ipe_changeOutput(IPE_OUTPUTINFO *OUTINFO);
extern ER ipe_enableDMAOut(BOOL Enable);
extern ER ipe_setMode(IPE_MODEINFO *ModeInfo);
extern ER ipe_start(void);
extern ER ipe_pause(void);
extern ER ipe_reset(BOOL ResetEn);

extern UINT32 ipe_getVersion(void);
extern UINT32 ipe_getIntStatus(void);
extern void ipe_getVA_Infor(STR_VA_INFOR *VA_Infor);
extern UINT32 ipe_getCFAPat(void);
extern STR_STRIPE_INFOR ipe_getHstripe(void);
extern STR_STRIPE_INFOR ipe_getVstripe(void);
extern UINT32 ipe_getLbufnum(void);
extern BOOL ipe_checkFunctionEnable(UW ipe_Function);
extern UINT32 ipe_getDebugStatus(void);
extern void ipe_clearInt(unsigned long intr);
extern BOOL ipe_checkBusy(void);
extern UINT32 ipe_getEdgeThCnt(void);
//#NT#2010/01/22#Connie Yu -begin
//#NT#DIS update
extern UINT32 ipe_getIntEnable(void);
extern UINT32 ipe_getPPBEOutID(void);
//#NT#2010/01/22#Connie Yu -end
//@}

#endif
