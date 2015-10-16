/*
    IME module driver

    NT96630 IME driver extern header file.

    @file       ime_lib.h
    @ingroup    mIIPPIME
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IME_LIB_H
#define _IME_LIB_H

/**
    @addtogroup mIIPPIME
*/
//@{

/**
    Define calculate slice mode vertical size.
*/
//@{
#define ImeSliceModeToVSize(a)      (1 << (a + 4))  ///< calculate slice mode vertical size
//@}

/**
    Define IME in/out format.
*/
//@{
#define IME_IN_FMT_444              0       ///< input format YCC444
#define IME_IN_FMT_422              1       ///< input format YCC422
#define IME_IN_FMT_420              2       ///< input format YCC420
#define IME_OUT_FMT_444             0       ///< output format YCC444
#define IME_OUT_FMT_422_COSITED     1       ///< output format YCC422 cosited
#define IME_OUT_FMT_422_CENTERED    2       ///< output format YCC422 centered
#define IME_OUT_FMT_420_COSITED     3       ///< output format YCC420 cosited
#define IME_OUT_FMT_420_CENTERED    4       ///< output format YCC420 centered
//@}

/**
    Define IME interrput enable.

    Interrupts are enabled by writing "1s" to respective bits.
*/
//@{
#define Ime_INTE_ST                       0x00000001 ///< stripe done
#define Ime_INTE_HOR                      0x00000002 ///< horizontal
#define Ime_INTE_FRM                      0x00000004 ///< frame done
#define Ime_INTE_V1OB                     0x00000008 ///< path1 vertical not in sync
#define Ime_INTE_H1OB                     0x00000010 ///< path1 horizontal not in sync
#define Ime_INTE_V2OB                     0x00000020 ///< path2 vertical not in sync
#define Ime_INTE_H2OB                     0x00000040 ///< path2 horizontal not in sync
#define Ime_INTE_V1O16LN                  0x00000080 ///< path1 16 line done
#define Ime_INTE_V2O16LN                  0x00000100 ///< path2 16 line done
#define Ime_INTE_SLICE_DN1                0x00000200 ///< path1 slice done
#define Ime_INTE_SLICE_DN2                0x00000400 ///< path2 slice done
#define Ime_INTE_ALL                      0x000007ff ///< all interrupt
//@}

/**
    Define IME interrput status.

    Interrupts are cleared by writing "1s" to respective bits.
*/
//@{
#define Ime_INT_ST                        0x00000001 ///< stripe done
#define Ime_INT_HOR                       0x00000002 ///< horizontal
#define Ime_INT_FRM                       0x00000004 ///< frame done
#define Ime_INT_V1OB                      0x00000008 ///< path1 vertical not in sync
#define Ime_INT_H1OB                      0x00000010 ///< path1 horizontal not in sync
#define Ime_INT_V2OB                      0x00000020 ///< path2 vertical not in sync
#define Ime_INT_H2OB                      0x00000040 ///< path2 horizontal not in sync
#define Ime_INT_V1O16LN                   0x00000080 ///< path1 16 line done
#define Ime_INT_V2O16LN                   0x00000100 ///< path2 16 line done
#define Ime_INT_SLICE_DN1                 0x00000200 ///< path1 slice done
#define Ime_INT_SLICE_DN2                 0x00000400 ///< path2 slice done
#define Ime_INT_ALL                       0x000007ff ///< all interrupt
//@}

typedef enum
{
    IMEALG_DISABLE  = 0,        ///< disable function
    IMEALG_ENABLE   = 1,        ///< enable function
    ENUM_DUMMY4WORD(_IME_FUN_EN)
} _IME_FUN_EN;

typedef enum
{
    IME_CHANNEL_Y   = 0,        ///< Y channel
    IME_CHANNEL_CB  = 1,        ///< CB channel
    IME_CHANNEL_CR  = 2,        ///< CR channel
    ENUM_DUMMY4WORD(_IME_CHANNEL)
} _IME_CHANNEL;

typedef enum
{
    IME_BUFNUM_0    = 0,        ///< 1 buffer
    IME_BUFNUM_1    = 1,        ///< 2 buffer
    IME_BUFNUM_2    = 2,        ///< 3 buffer
    ENUM_DUMMY4WORD(_IME_BUFNUM)
} _IME_BUFNUM;

typedef enum
{
    _AUTO_1PATH         = 0,    ///< auto run 1 path
    _AUTO_2PATH         = 1,    ///< auto run 2 path
    _D2D                = 2,    ///< Dram to dram
    _SLICE_1PATH        = 3,    ///< slice mode 1 path
    _SLICE_2PATH        = 4,    ///< slice mode 2 path
    _START              = 5,    ///< start IME
    _STOP               = 6,    ///< stop IME
    _RESET              = 7,    ///< reset IME
    _CHANGESIZE_1PATH   = 8,    ///< change path1 size
    _CHANGESIZE_2PATH   = 9,    ///< change path1 and path2 size
    _CHANGECLOCK        = 10,   ///< change IME clock
    _CHANGEPATH         = 11,   ///< change path number
    _CHANGEBUFFER       = 12,   ///< change buffer information
    _NONE               = 13,   ///< none
    _OPEN               = 14,   ///< open IME
    _CLOSE              = 15,   ///< close IME
    _DVI_1PATH          = 16,   ///< DVI Mode
    ENUM_DUMMY4WORD(_IME_MODE_SEL)
} _IME_MODE_SEL;

typedef enum
{
    IMEALG_BICUBIC  = 0,        ///< bicubic interpolation
    IMEALG_BILINEAR = 1,        ///< bilinear interpolation
    IMEALG_NEAREST  = 2,        ///< nearest interpolation
    ENUM_DUMMY4WORD(_IMEALGSCALEMODE)
} _IMEALGSCALEMODE;

typedef enum
{
    IMEALG_2PATH    = 0,        ///< enable two paths
    IMEALG_1PATH    = 1,        ///< enable one path
    IMEALG_1PATHY   = 2,        ///< enable Y path only
    ENUM_DUMMY4WORD(_IMEALGSETPATH)
} _IMEALGSETPATH;

typedef enum
{
    IMEALG_INYUV444 = 0,        ///< input format YUV444
    IMEALG_INYUV422 = 1,        ///< input format YUV422
    IMEALG_INYUV420 = 2,        ///< input format YUV420
    IMEALG_INRGB444 = 3,        ///< input format RGB444
    ENUM_DUMMY4WORD(_IMEALGSETIMAT)
} _IMEALGSETIMAT;

typedef enum
{
    IMEALG_OUTYUV444        = 0,    ///< output format YUV444
    IMEALG_OUTYUV422_COS    = 1,    ///< output format YUV422 cosited
    IMEALG_OUTYUV422_CEN    = 2,    ///< output format YUV422 centered
    IMEALG_OUTYUV420_COS    = 3,    ///< output format YUV420 cosited
    IMEALG_OUTYUV420_CEN    = 4,    ///< output format YUV420 centered
    IMEALG_BAYER            = 5,    ///< output format Bayer
    IMEALG_OUTRGB444        = 6,    ///< output format RGB444
    ENUM_DUMMY4WORD(_IMEALGSETOMAT)
} _IMEALGSETOMAT;

typedef enum
{
    IMEALG_SINGLE   = 0,            ///< single stripe
    IMEALG_MULTI    = 1,            ///< multi stripe
    ENUM_DUMMY4WORD(_IMEALGSETSTRIPE)
} _IMEALGSETSTRIPE;

typedef enum
{
    IMEALG_16       = 0,            ///< slice height 16
    IMEALG_32       = 1,            ///< slice height 32
    IMEALG_64       = 2,            ///< slice height 64
    IMEALG_128      = 3,            ///< slice height 128
    IMEALG_256      = 4,            ///< slice height 256
    IMEALG_512      = 5,            ///< slice height 512
    IMEALG_1024     = 6,            ///< slice height 1024
    IMEALG_2048     = 7,            ///< slice height 2048
    ENUM_DUMMY4WORD(_IMEALG_SLICE_H)
} _IMEALG_SLICE_H;

typedef enum
{
    IMEALG_IPE2IME = 0,             ///< source IPE
    IMEALG_DMA2IME = 1,             ///< source DRAM
    ENUM_DUMMY4WORD(_IMEALG_SRC)
} _IMEALG_SRC;

typedef enum
{
    IMEALG_IPPBUFFER_DISABLE    = 0,    ///< disable input ping-pong buffer
    IMEALG_IPPBUFFER_ENABLE     = 1,    ///< enable input ping-pong buffer
    ENUM_DUMMY4WORD(_IMEALG_IPPE)
} _IMEALG_IPPE;

typedef enum
{
   IMEALG_1BUFFER = 0,                  ///< 1 output ping-pong buffer
   IMEALG_2BUFFER = 1,                  ///< 2 output ping-pong buffer
   IMEALG_3BUFFER = 2,                  ///< 3 output ping-pong buffer
   ENUM_DUMMY4WORD(_IMEALG_OPPBE)
} _IMEALG_OPPBE;


typedef enum
{
    IMEALG_RGB2YCC = 0,                 ///< color space transform RGB to YCC
    IMEALG_YCC2RGB = 1,                 ///< color space transform YCC to RGB
    ENUM_DUMMY4WORD(_IMEALG_ICST_OPT)
} _IMEALG_ICST_OPT;

typedef enum
{
   IMEALG_CLK60     = 0,                ///< IME clock 60Mhz
   //IMEALG_CLK48     = 1,                ///< IME clock 48
   //IMEALG_CLK40     = 2,                ///< IME clock 40Mhz
   //IMEALG_CLK30     = 3,                ///< IME clock 30Mhz
   IMEALG_CLK80     = 4,                ///< IME clock 80Mhz
   IMEALG_CLK96     = 5,                ///< IME clock 96Mhz
   IMEALG_CLK120    = 6,                ///< IME clock 120Mhz
   ENUM_DUMMY4WORD(_IMEALG_CLOCK)
} _IMEALG_CLOCK;

typedef enum
{
   IMEALG_HAVGSIZE2  = 0,               ///< H averaging filter size 2
   IMEALG_HAVGSIZE4  = 1,               ///< H averaging filter size 4
   IMEALG_HAVGSIZE8  = 2,               ///< H averaging filter size 8
   IMEALG_HAVGSIZE16 = 3,               ///< H averaging filter size 16
   ENUM_DUMMY4WORD(_IMEALG_HAVGSIZE)
} _IMEALG_HAVGSIZE;
typedef enum
{
    IMEALG_YCBCR_PLANAR = 0,
    IMEALG_CBCR_PACK = 1
} _IMEALG_YCC_FORMAT;

typedef void (*IMEIntCB)(UINT32);

typedef struct _IME_YCC_FORMAT
{
    _IMEALG_YCC_FORMAT in_format;       ///< input UVpacking
    _IMEALG_YCC_FORMAT out1_format;     ///< output path1 UVpacking
    _IMEALG_YCC_FORMAT out2_format;     ///< output path2 UVpacking
}IME_YCC_FORMAT;

typedef struct _STR_YCBCR_ADDR
{
    UINT32 y_addr;      ///< Y starting address
    UINT32 cb_addr;     ///< Cb starting address
    UINT32 cr_addr;     ///< Cr starting address
} STR_YCBCR_ADDR,*pSTR_YCBCR_ADDR;

typedef struct _STR_SCALE_SIZE_INFO
{
    UINT32 in_h;        ///< input horizontal size
    UINT32 in_v;        ///< input vertical size
    UINT32 out_h;       ///< output horizontal size
    UINT32 out_v;       ///< output vertical size
} STR_SCALE_SIZE_INFO,*pSTR_SCALE_SIZE_INFO;

typedef struct _STR_IME_STRIPE_INFOR{
    UINT32    n;        ///< stripe size in each stripe
    UINT32    l;        ///< stripe size of last stripe
    UINT32    m;        ///< stripe number
}STR_IME_STRIPE_INFOR;

typedef struct _IME_OPENOBJ{
    void (*FP_IMEISR_CB)(UINT32 intstatus); ///< isr callback function
    UINT32 IME_CLOCKSEL;                    ///< clock selection
} IME_OPENOBJ,*PIME_OPENOBJ;

/**
    Structure For ime_DMAScale_ext Usage
*/
typedef struct _STR_SCALE_PRAM_EXT
{
    UINT32  in_p1_addr;             ///< input plane 1 starting address
    UINT32  in_p2_addr;             ///< input plane 2 starting address
    UINT32  in_p3_addr;             ///< input plane 3 starting address
    UINT32  out_p1_addr;            ///< output plane 1 starting address
    UINT32  out_p2_addr;            ///< output plane 2 starting address
    UINT32  out_p3_addr;            ///< output plane 3 starting address
    UINT32  in_format;              ///< input format 0:YUV444 1:YUV422 2:YUV420 3:RGB444
    UINT32  out_format;             ///< output format 0:YUV444 1:YUV422_cosited 2:YUV422_centered 3:YUV420_cosited 4:YUV420_centered 5:RGB444
    UINT32  in_h;                   ///< input horizontal size
    UINT32  in_v;                   ///< input vertical size
    UINT32  in_lineoffset;          ///< input line offset
    UINT32  out_h;                  ///< output horizontal size
    UINT32  out_v;                  ///< output vertical size
    UINT32  out_lineoffset;         ///< output line offset
    BOOL    in_pack;                ///< input uv packing enable 0:Cb/Cr separate 1:Cb/Cr packing
    BOOL    out_p1_pack;            ///< output path1 uv packing enable  0:Cb/Cr separate 1:Cb/Cr packing
    BOOL    out_p2_pack;            ///< output path2 uv packing enable  0:Cb/Cr separate 1:Cb/Cr packing
    _IMEALG_SRC    src;             ///< IME source
} STR_SCALE_PRAM_EXT;

typedef struct _IME_SIZE_INFO
{
    UINT32 h_size;                  ///< horizontal size
    UINT32 v_size;                  ///< vertical size
    UINT32 crop_stx;                ///< crop horizontal start
    UINT32 crop_sty;                ///< crop vertical start
}IME_SIZE_INFO;

typedef struct _IME_LINEOFS_INFO
{
    UINT32 in_lineofs;              ///< input line offset
    UINT32 out1_lineofs;            ///< output path1 line offset
    UINT32 out2_lineofs;            ///< output path2 line offset
}IME_LINEOFS_INFO;

typedef struct _IME_FORMAT_INFO
{
    _IMEALGSETIMAT in_format;       ///< input format
    _IMEALGSETOMAT out1_format;     ///< output path1 format
    _IMEALGSETOMAT out2_format;     ///< output path2 format
}IME_FORMAT_INFO;

typedef struct _IME_YCC_ADDR
{
    UINT32 y_addr;                  ///< y address
    UINT32 cb_addr;                 ///< cb address
    UINT32 cr_addr;                 ///< cr address
}IME_YCC_ADDR;

typedef struct _IME_FILT_PARA
{
    _IME_FUN_EN         havg_en;    ///< horizontal averaging function enable
    _IMEALG_HAVGSIZE    havg_size;  ///< horizontal averaging filter size
    UINT32              havg_coeff; ///< horizontal averaging coefficient
    _IME_FUN_EN         crv_en;     ///< curve mapping function enable
    UINT32              crv_delta;  ///< curve mapping delta
    UINT32              crv_center; ///< curve mapping center
}IME_FILT_PARA;

typedef struct _STR_SCALE_PRAM_D2D
{
    IME_YCC_ADDR    in_addr;        ///< input starting address
    IME_YCC_ADDR    p1_out0_addr;   ///< output path1 buffer 0 starting address
    IME_YCC_ADDR    p1_out1_addr;   ///< output path1 buffer 1 starting address
    IME_YCC_ADDR    p1_out2_addr;   ///< output path1 buffer 2 starting address
    IME_YCC_ADDR    p2_out0_addr;   ///< output path2 buffer 0 starting address
    IME_YCC_ADDR    p2_out1_addr;   ///< output path2 buffer 1 starting address
    IME_YCC_ADDR    p2_out2_addr;   ///< output path2 buffer 2 starting address
    IME_SIZE_INFO   in_size;        ///< input size
    IME_SIZE_INFO   p1_out_size;    ///< path1 output size
    IME_SIZE_INFO   p2_out_size;    ///< path2 output size
    IME_FORMAT_INFO format;         ///< input/output format
    IME_LINEOFS_INFO line_ofs;      ///< input/output lineoffset
    IME_FILT_PARA   filt_para;      ///< filter parameter
    _IMEALG_SRC     src;            ///< IME source
    IME_YCC_FORMAT  ycc_format;     ///< UVpacking format
    _IMEALGSETPATH  path_sel;       ///< IME path selection
    _IMEALGSCALEMODE bil;           ///< IME interpolation method selection
} STR_SCALE_PRAM_D2D,*pSTR_SCALE_PRAM_D2D;

typedef struct _IME_MODE_PRAM
{
    _IME_MODE_SEL       op_mode;        ///< operation mode
    UINT8               wait_mode;      ///< wait mode
    _IMEALG_SRC         in_src;         ///< IME source
    _IMEALG_CLOCK       clk;            ///< clock selection
    _IMEALGSETPATH      path_num;       ///< path number selection
    UINT8               int_mode;       ///<
    _IMEALGSETSTRIPE    stripe_mode;    ///< stripe mode selection
    _IMEALG_IPPE        ippb_en;        ///< input ping-pong buffer enable
    _IMEALG_OPPBE       oppb1_num;      ///< output path1 ping-pong buffer number
    _IMEALG_OPPBE       oppb2_num;      ///< output path2 ping-pong buffer number
    _IME_FUN_EN         icst_en;        ///< color space transform enable
    _IMEALG_ICST_OPT    icst_op;        ///< color space transform operation selection
    IME_YCC_ADDR        in_addr;        ///< input starting address
    IME_YCC_ADDR        p1_out0_addr;   ///< output path1 buffer 0 starting address
    IME_YCC_ADDR        p1_out1_addr;   ///< output path1 buffer 1 starting address
    IME_YCC_ADDR        p1_out2_addr;   ///< output path1 buffer 2 starting address
    IME_YCC_ADDR        p2_out0_addr;   ///< output path2 buffer 0 starting address
    IME_YCC_ADDR        p2_out1_addr;   ///< output path2 buffer 1 starting address
    IME_YCC_ADDR        p2_out2_addr;   ///< output path2 buffer 2 starting address
    IME_SIZE_INFO       in_size;        ///< input size
    IME_SIZE_INFO       p1_out_size;    ///< path1 output size
    IME_SIZE_INFO       p2_out_size;    ///< path2 output size
    IME_FORMAT_INFO     format;         ///< input/output format
    IME_LINEOFS_INFO    line_ofs;       ///< input/output lineoffset
    IME_YCC_FORMAT      ycc_format;     ///< UVpacking format
    IME_FILT_PARA       filt_para;      ///< filter parameter
    _IMEALGSCALEMODE    bil;            ///< interpolation method selection
}IME_MODE_PRAM,*pIME_MODE_PRAM;

typedef struct _IME_FBADDR_INFO
{
    UINT32 y0_addr;                 ///< buffer 0 y address
    UINT32 cb0_addr;                ///< buffer 0 cb address
    UINT32 cr0_addr;                ///< buffer 0 cr address
    UINT32 y1_addr;                 ///< buffer 1 y address
    UINT32 cb1_addr;                ///< buffer 1 cb address
    UINT32 cr1_addr;                ///< buffer 1 cr address
    UINT32 y2_addr;                 ///< buffer 2 y address
    UINT32 cb2_addr;                ///< buffer 2 cb address
    UINT32 cr2_addr;                ///< buffer 2 cr address
    UINT32 in_h;                    ///< input horizontal size
    UINT32 in_v;                    ///< input vertical size
    UINT32 out_h;                   ///< output horizontal size
    UINT32 out_v;                   ///< output vertical size
    _IMEALGSETIMAT in_format;       ///< input format
    _IMEALGSETOMAT out_format;      ///< output format
    UINT32 out_lineoffset;          ///< output lineoffset
    UINT32 ime_sten;                ///< stripe mode selection
    UINT32 out_addr_offset;
    UINT32 src;                     ///< IME source
    IME_YCC_FORMAT ycc_format;      ///< UVpacking format
    IME_YCC_ADDR inYCCAddr;         ///< input address
    UINT32 in_lineoffset;           ///< input lineoffset
    IME_FILT_PARA   filt_para;      ///< filter parameter
} IME_FBADDR_INFO,*pIME_FBADDR_INFO;

//Fuction Declaration
//#NT#2009/12/03#Yuppie Lin -begin
//#NT#Remove private function
//extern void ime_reset(_IME_FUN_EN reset);
//extern void ime_start(_IME_FUN_EN b_en);
//#NT#2009/12/03#Yuppie Lin -end
//#NT#2009/12/03#Yuppie Lin -begin
//#NT#add reset function
extern ER ime_HWreset(void);
//#NT#2009/12/03#Yuppie Lin -end
extern void ime_Slice_Mode1_En(_IME_FUN_EN en);
extern void ime_setSliceH(_IMEALG_SLICE_H slice_h);
extern void ime_waitFlagFrameEnd(void);
extern void ime_clearFrameEnd(void);
extern void ime_InterruptEnable(UINT32 interrupt_bits);
extern void ime_StatustClear(UINT32 int_status_bits);
extern UINT32 ime_getInterruptStatus(void);
extern STR_IME_STRIPE_INFOR ime_getHstripe(void);
extern STR_IME_STRIPE_INFOR ime_getVstripe(void);
extern UINT32 ime_getOutputPath1Add(_IME_BUFNUM Bufnum, _IME_CHANNEL Channel);
extern UINT32 ime_getOutputPath2Add(_IME_BUFNUM Bufnum, _IME_CHANNEL Channel);
extern UINT32 ime_getActOutputPath1Add(_IME_CHANNEL Channel);
extern UINT32 ime_getActOutputPath2Add(_IME_CHANNEL Channel);
//#NT#2009/12/03#Yuppie Lin -begin
//#NT#Remove private function
//extern UINT8 ime_getOutputFormat(void);
extern UINT8 ime_getPath1CurrentBufferIndex(void);
extern UINT8 ime_getPath2CurrentBufferIndex(void);
//extern UINT32 ime_getSliceStatus(void);
//extern UINT32 ime_getStartStatus(void);
//#NT#2009/12/03#Yuppie Lin -end
extern ER ime_setMode(pIME_MODE_PRAM imeModeParam);
extern ER ime_open(IME_OPENOBJ *PIME_OpenInfo);
extern BOOL ime_isOpened(void);
extern ER ime_close(void);
extern ER ime_setReset(void);
extern ER ime_setStop(void);
extern ER ime_setStart(void);
extern ER ime_clrIntFlag(void);
extern UINT32 ime_waitDone(void);
extern _IME_MODE_SEL ime_getOPState(void);
//#NT#2009/11/20#Jarkko Chang -begin
extern void ime_setOutput2Format(_IMEALG_YCC_FORMAT pth2_fmt);
////#NT#2009/11/20#Jarkko Chang -end
//#NT#2009/12/03#Yuppie Lin -begin
//#NT#Add chroma filter
extern ER ime_setChromaFilter(IME_FILT_PARA *param);
//#NT#2009/12/03#Yuppie Lin -end
//
// Public API for IME wrapper function
//
extern void ime_DMAScale_ext(STR_SCALE_PRAM_EXT scale_info);
extern void ime_triggerSlice(void);
extern UINT32 ime_getSliceHeight(void);
extern void ime_setIOFormat(_IMEALG_YCC_FORMAT in_fmt, _IMEALG_YCC_FORMAT pth1_fmt, _IMEALG_YCC_FORMAT pth2_fmt);
extern ER   ime_setPath2Scale_to_DMAReg(UINT32 S0_2Y,UINT32 S0_2CB,UINT32 S0_2CR,
                                 UINT32 S1_2Y,UINT32 S1_2CB,UINT32 S1_2CR,
                                 UINT32 S2_2Y,UINT32 S2_2CB,UINT32 S2_2CR);
//#NT#2010/01/29#Bowen Li -begin
//#NT#Modified free run control flow
extern void ime_freeRun(_IME_FUN_EN f_en);
//#NT#2010/01/29#Bowen Li -end

//#NT#2012/02/01#Yuppie Lin -begin
//#NT# export filter setting
extern void ime_Path1HFiltMode(_IME_FUN_EN en);
extern void ime_Path1HFiltCoef(UINT32 coef);
extern void ime_Path1VFiltMode(_IME_FUN_EN en);
extern void ime_Path1VFiltCoef(UINT32 coef);
//#NT#2012/02/01#Yuppie Lin -end
#endif
