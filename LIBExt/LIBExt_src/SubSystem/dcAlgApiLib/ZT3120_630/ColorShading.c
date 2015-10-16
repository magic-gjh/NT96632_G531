//#include "sensor_model.h"
#include "ColorShading.h"
#include "filesystsk.h"
#include "pll.h"
#include "ime_lib.h"
#include    "grph.h"

//#define CONST_CS_PARTI_BURST    5
#define CONST_CS_FIELDNUM_MAX 8

#define CONST_CS_ONE_RESET   0xffffffff
#define CONST_CS_ZERO_RESET  0x00000000

#define DEFINE_DONT_WAIT_NAND_READING 1//Maple@JJ070409 Performance 0

#define ClrShade_ind //debug_err
#define ClrShade_err    debug_err


#define CS_TEST_CODE    0

//Maple@JJ070411 Performance: move to .h

typedef struct
{
    UINT32   RawBayAdd[CONST_CS_BURSTPIC_MAX];
    UINT32   GainBayAdd;
    UINT32   GainTblBufAdd;
    UINT32   CurrTblAdd;
    UINT32   LastTblAdd;
    UINT16   LineCntBuf[CONST_CS_FIELDNUM_MAX];
    UINT16   LineCntMax;
    UINT16   LinePre;
    UINT16   BayerHSize;
    UINT16   BayerVSize;
    UINT16   TableHSize;
    UINT16   TableVSize;
    UINT8    CurrTblInd;
    UINT8    LastTblInd;
    COLORSHADING_STAGE    Stage;
    COLORSHADING_STAGESTATUS    StageStatus;
    UINT8    StageArg;
    UINT8    CapFldNum;
    UINT8    RawBitDepth;
    UINT8    PreTarget;
    UINT8    BayPattern;
    UINT8    BurstDivision;
    UINT8    BurstPicNum;
    COLORSHADING_CAPMODE CapMode;
    BOOL     Enable;
} COLORSHADING_CTRL, *PCOLORSHADING_CTRL;


static COLORSHADING_CTRL g_ColorShadingCtrl;

static UINT8 CS_INTERN_DoStage(COLORSHADING_STAGE stage);
static void CS_INTERN_Reset(void);
static void CS_INTERN_Initial(void);
static void CS_INTERN_GetTable(void);
static void CS_INTERN_GetBayer(void);
static void CS_INTERN_DoLine(void);
static void CS_INTERN_DoRegion(void);
static void CS_INTERN_DoPartial(void);


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
//This region: external API for parameter setup

UINT8 ColorShading_SetRawBayAdd(UINT32 input)
{
    g_ColorShadingCtrl.RawBayAdd[0] = input;
}

UINT8 ColorShading_SetRawBayAdd_ByIndex(UINT32 input, UINT8 index)
{
    g_ColorShadingCtrl.RawBayAdd[index] = input;
}

UINT8 ColorShading_SetGainBayAdd(UINT32 input)
{
    g_ColorShadingCtrl.GainBayAdd = input;
}

UINT8 ColorShading_SetGainTblBufAdd(UINT32 input)
{
    g_ColorShadingCtrl.GainTblBufAdd = input;
}

UINT8 ColorShading_SetLinePre(UINT32 input)
{
    g_ColorShadingCtrl.LinePre = input;
}

UINT8 ColorShading_SetBayerHSize(UINT32 input)
{
    g_ColorShadingCtrl.BayerHSize = input;
}

UINT8 ColorShading_SetBayerVSize(UINT32 input)
{
    g_ColorShadingCtrl.BayerVSize = input;
}

UINT8 ColorShading_SetTableHSize(UINT32 input)
{
    g_ColorShadingCtrl.TableHSize = input;
}

UINT8 ColorShading_SetTableVSize(UINT32 input)
{
    g_ColorShadingCtrl.TableVSize = input;
}

UINT8 ColorShading_SetCurrTblInd(UINT32 input)
{
    g_ColorShadingCtrl.CurrTblInd = input;
}

UINT8 ColorShading_SetStageArg(UINT32 input)
{
    g_ColorShadingCtrl.StageArg = input;
}

UINT8 ColorShading_SetCapFldNum(UINT32 input)
{
    g_ColorShadingCtrl.CapFldNum = input;
}

UINT8 ColorShading_SetRawBitDepth(UINT32 input)
{
    g_ColorShadingCtrl.RawBitDepth = input;
}

UINT8 ColorShading_SetBayPattern(UINT32 input)
{
    g_ColorShadingCtrl.BayPattern = input;
}

UINT8 ColorShading_SetBurstDivision(UINT32 input)
{
    g_ColorShadingCtrl.BurstDivision = input;
}

UINT8 ColorShading_SetBurstPicNum(UINT32 input)
{
    g_ColorShadingCtrl.BurstPicNum = input;
}

UINT8 ColorShading_SetCapMode(COLORSHADING_CAPMODE input)
{
    g_ColorShadingCtrl.CapMode = input;
}

UINT8 ColorShading_SetEnable(UINT32 input)
{
    g_ColorShadingCtrl.Enable = input;
}

UINT8 ColorShading_SetUp(PCOLORSHADING_PARAM pColorShadingParam)
{
    UINT32 i;

    for(i=0; i<CONST_CS_BURSTPIC_MAX; i++)
    {
        g_ColorShadingCtrl.RawBayAdd[i] = pColorShadingParam->RawBayAdd[i];
    }

    g_ColorShadingCtrl.GainBayAdd   = pColorShadingParam->GainBayAdd;
    g_ColorShadingCtrl.GainTblBufAdd= pColorShadingParam->GainTblBufAdd;
    g_ColorShadingCtrl.LinePre      = pColorShadingParam->LinePre;
    g_ColorShadingCtrl.BayerHSize   = pColorShadingParam->BayerHSize;
    g_ColorShadingCtrl.BayerVSize   = pColorShadingParam->BayerVSize;
    g_ColorShadingCtrl.TableHSize   = pColorShadingParam->TableHSize;
    g_ColorShadingCtrl.TableVSize   = pColorShadingParam->TableVSize;
    g_ColorShadingCtrl.CurrTblInd   = pColorShadingParam->CurrTblInd;
    g_ColorShadingCtrl.CapFldNum    = pColorShadingParam->CapFldNum;
    g_ColorShadingCtrl.RawBitDepth  = pColorShadingParam->RawBitDepth;
    g_ColorShadingCtrl.BayPattern   = pColorShadingParam->BayPattern;
    g_ColorShadingCtrl.BurstDivision= pColorShadingParam->BurstDivision;
    g_ColorShadingCtrl.BurstPicNum  = pColorShadingParam->BurstPicNum;
    g_ColorShadingCtrl.CapMode      = pColorShadingParam->CapMode;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
//This region: external API for paremeter check

BOOL ColorShading_GetEnable(void)
{
    return g_ColorShadingCtrl.Enable;
}

BOOL ColorShading_GetEnable_CheckCapMode(COLORSHADING_CAPMODE capMode)
{
    if(capMode != g_ColorShadingCtrl.CapMode)
        return FALSE;
    else
        return g_ColorShadingCtrl.Enable;
}

UINT32 ColorShading_GetCurrTblAdd(void)
{
    return g_ColorShadingCtrl.CurrTblAdd;
}

UINT32 ColorShading_GetGainBayAdd(void)
{
    return g_ColorShadingCtrl.GainBayAdd;
}


//Maple@JJ070411 Performance
UINT32 ColorShading_GetStage(void)
{
    return g_ColorShadingCtrl.Stage;
}

UINT32 ColorShading_GetStageStatus(void)
{
    return g_ColorShadingCtrl.StageStatus;
}
//~Maple@JJ070411 Performance

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
//This region: external API for commands

UINT8 ColorShading_Reset()
{
    g_ColorShadingCtrl.Stage = COLORSHADING_STAGE_0_RESET;
    g_ColorShadingCtrl.StageStatus = STATUS_START;

    ColorShading_Proceed(COLORSHADING_STAGE_0_RESET, FALSE);
}

UINT8 ColorShading_Initial()
{
    g_ColorShadingCtrl.Stage = COLORSHADING_STAGE_1_INITIAL;
    g_ColorShadingCtrl.StageStatus = STATUS_START;

    ColorShading_Proceed(COLORSHADING_STAGE_1_INITIAL, FALSE);
}



UINT8 ColorShading_Proceed(COLORSHADING_STAGE target, BOOL Wait)
{
#define AUOT_CHECK_ENABLE 0

#if AUOT_CHECK_ENABLE
    if(!g_ColorShadingCtrl.Enable)
    {
        return 0;
    }
#endif//AUOT_CHECK_ENABLE

    ClrShade_ind(("Prcd %d %d %d ",target,g_ColorShadingCtrl.PreTarget,Wait));

    if(target==COLORSHADING_STAGE_JUST_PROCEED)
    {
        target = g_ColorShadingCtrl.PreTarget;
    }
    else
    {
        g_ColorShadingCtrl.PreTarget = target;
    }

    if(target<g_ColorShadingCtrl.Stage)
    {//current stage beyond target
        ClrShade_ind(("\r\n"));
    }
    else//(target>=g_ColorShadingCtrl.Stage)
    {
        do
        {
            if(target==g_ColorShadingCtrl.Stage)
            {
                if(g_ColorShadingCtrl.StageStatus==STATUS_FINISH)//target achieved
                {
                    break;
                }
                else
                {
                    CS_INTERN_DoStage(g_ColorShadingCtrl.Stage);
                }
            }
            else//(target>g_ColorShadingCtrl.Stage)
            {
                if(g_ColorShadingCtrl.StageStatus==STATUS_FINISH)//target achieved
                {
                    g_ColorShadingCtrl.Stage ++ ;
                    g_ColorShadingCtrl.StageStatus = STATUS_START;
                }
                CS_INTERN_DoStage(g_ColorShadingCtrl.Stage);
            }
            ClrShade_ind(("\r\n"));
       } while(Wait);
    }

    return 0;
}

UINT8 ColorShading_ResetLastTblInd(void)
{
    g_ColorShadingCtrl.LastTblInd = CONST_CS_ONE_RESET;
}

UINT32 ColorShading_Debug(UINT32 input)
{
    //Do everything you want here, to debug.
    return 0;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
//This region: static Function

UINT8 CS_INTERN_DoStage(COLORSHADING_STAGE stage)
{
    ClrShade_err(("DS %d %d", stage, g_ColorShadingCtrl.StageStatus));

    switch(g_ColorShadingCtrl.CapMode)
    {
        case COLORSHADING_CAPMODE_SINGLE:
            switch(stage)
            {
                case COLORSHADING_STAGE_0_RESET:
                    CS_INTERN_Reset();
                    break;
                case COLORSHADING_STAGE_1_INITIAL:
                    CS_INTERN_Initial();
                    break;
                case COLORSHADING_STAGE_2_GET_TABLE:
                    CS_INTERN_GetTable();
                    break;
                case COLORSHADING_STAGE_3_GET_BAYER:
                    CS_INTERN_GetBayer();
                    break;
                case COLORSHADING_STAGE_4_DO_LINE:
                    CS_INTERN_DoLine();
                    break;
                case COLORSHADING_STAGE_5_DO_REGION:
                    debug_msg("bbbbbbbbbbbbbbbbbb\r\n");
                    CS_INTERN_DoRegion();
                    break;
                case COLORSHADING_STAGE_6_END:
                    break;
                default:

                    break;
            }
            break;

        case COLORSHADING_CAPMODE_BURST:
            switch(stage)
            {
                case COLORSHADING_STAGE_0_RESET:
                    CS_INTERN_Reset();
                    break;
                case COLORSHADING_STAGE_A_INITIAL:
                    CS_INTERN_Initial();
                    break;
                case COLORSHADING_STAGE_B_GET_TABLE:
                    CS_INTERN_GetTable();
                    break;
                case COLORSHADING_STAGE_C_DO_PARTIAL:
                    CS_INTERN_DoPartial();
                    break;
                case COLORSHADING_STAGE_D_END:
                    break;
                default:

                    break;
            }
            break;
        default:
//Maple@JJ@20070420 CS: Do Reset for all mode, even non-defined mode
            switch(stage)
            {
                case COLORSHADING_STAGE_0_RESET:
                    CS_INTERN_Reset();
                    break;
                default:
                    ClrShade_err(("CS: Undefined CapMode\r\n"));
                    break;
            }
//~Maple@JJ@20070420 CS: Do Reset for all mode, even non-defined mode
            break;
    }
}


void CS_INTERN_Reset(void)  // 盢计耴箂
{
    UINT8 i;

    for(i=0; i<CONST_CS_BURSTPIC_MAX; i++)
    {
        g_ColorShadingCtrl.RawBayAdd[i] =CONST_CS_ZERO_RESET;
    }
    g_ColorShadingCtrl.GainBayAdd       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.GainTblBufAdd    =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.CurrTblAdd       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.LastTblAdd       =CONST_CS_ZERO_RESET;
    for(i=0; i<CONST_CS_FIELDNUM_MAX; i++)
    {
        g_ColorShadingCtrl.LineCntBuf[i]=CONST_CS_ZERO_RESET;
    }
    g_ColorShadingCtrl.LineCntMax       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.LinePre          =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.BayerHSize       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.BayerVSize       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.TableHSize       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.TableVSize       =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.CurrTblInd       =CONST_CS_ZERO_RESET;//this two must be different
    g_ColorShadingCtrl.LastTblInd       =CONST_CS_ONE_RESET;//this two must be different
    g_ColorShadingCtrl.Stage            =CONST_CS_ZERO_RESET;
    //g_ColorShadingCtrl.StageStatus      =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.StageArg         =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.CapFldNum        =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.RawBitDepth      =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.BurstDivision    =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.BurstPicNum      =CONST_CS_ZERO_RESET;
    g_ColorShadingCtrl.CapMode          =CONST_CS_ONE_RESET;
    g_ColorShadingCtrl.Enable           =FALSE;

    g_ColorShadingCtrl.StageStatus      =STATUS_FINISH;
}

void CS_INTERN_Initial(void)  //盢篈耴箂
{
    //Do something to initialize if necessary
    g_ColorShadingCtrl.StageArg         =COLORSHADING_STAGE_0_RESET;

    g_ColorShadingCtrl.StageStatus      =STATUS_FINISH;

    if(g_ColorShadingCtrl.CapMode == COLORSHADING_CAPMODE_BURST)
    {
        g_ColorShadingCtrl.LastTblInd = CONST_CS_ONE_RESET;
    }
}

void CS_INTERN_GetTable(void)
{
    UINT32 TmpBuffStartByte;
    UINT32 uiTmp;

    if(g_ColorShadingCtrl.StageStatus==STATUS_START)
    {
#if 1
        //Do not care about: g_ColorShadingCtrl.CurrTblInd, g_ColorShadingCtrl.LastTblInd
        //JJ: ぃノ眔Gain Table, 场砞﹚GainTblBufAdd,竒ΤGain Table
        g_ColorShadingCtrl.CurrTblAdd = g_ColorShadingCtrl.GainTblBufAdd;//GainTable竚碞GainTableBuffer秨繷
        g_ColorShadingCtrl.LastTblAdd = g_ColorShadingCtrl.CurrTblAdd;
        g_ColorShadingCtrl.StageArg=0x0;//把计单, Ωぃノ弄Gain Table
        g_ColorShadingCtrl.StageStatus = STATUS_FINISH; //spark add
#else
        if(g_ColorShadingCtrl.CurrTblInd==g_ColorShadingCtrl.LastTblInd)
        {//check if this table is already loaded
            ClrShade_ind(("Use old"));
            g_ColorShadingCtrl.CurrTblAdd = g_ColorShadingCtrl.LastTblAdd;
            g_ColorShadingCtrl.StageArg=0x0;//Maple@JJ070409 CS
        }
        else
        {
            ClrShade_ind(("Get new"));

            {//get gain-table
#if CS_TEST_CODE
                memset(g_ColorShadingCtrl.GainTblBufAdd, 0x0, 5*0x4000/*5 nand blks*/);
#endif
//Maple@JJ070411 Performance
#if DEFINE_DONT_WAIT_NAND_READING
                g_ColorShadingCtrl.StageArg=0x1;//Maple@JJ070409 CS
                TmpBuffStartByte = ReadColorShadingDataByTblIndex_NoWait(g_ColorShadingCtrl.GainTblBufAdd, g_ColorShadingCtrl.CurrTblInd);
                if(TmpBuffStartByte==0xffffffff)
                {
                    ClrShade_err(("CS: GetTbl Delay\r\n"));
              return;//StageStatus must not increase
                }
#else
                TmpBuffStartByte = ReadColorShadingDataByTblIndex(g_ColorShadingCtrl.GainTblBufAdd, g_ColorShadingCtrl.CurrTblInd);
#endif
//~Maple@JJ070411 Performance
                g_ColorShadingCtrl.CurrTblAdd = g_ColorShadingCtrl.GainTblBufAdd+TmpBuffStartByte;
            }

            {//record
                g_ColorShadingCtrl.LastTblInd = g_ColorShadingCtrl.CurrTblInd ;
                g_ColorShadingCtrl.LastTblAdd = g_ColorShadingCtrl.CurrTblAdd;
            }
        }
        g_ColorShadingCtrl.StageStatus ++;
#endif
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_1)
    {
//Maple@JJ0403 CS
#if DEFINE_DONT_WAIT_NAND_READING
        //Maple@JJ070409 CS
        if(g_ColorShadingCtrl.StageArg)
        {
            TimerDelayMs(1);//Maple@JJ070409 Performance: release CPU to FileSysTsk
            uiTmp = 0x1; //ReadColorShadingDataByTblIndex_CheckEnd();
        }
        else
        {
            uiTmp = 0x1;//TRUE
        }
        //~Maple@JJ070409 CS
        if(uiTmp)
//Maple@JJ0328 Performance
        {
            ClrShade_ind(("done"));
            g_ColorShadingCtrl.StageStatus = STATUS_FINISH;
        }
        else
        {
            ClrShade_ind(("return %x", uiTmp));
        }
#else
        ClrShade_ind(("done"));
        g_ColorShadingCtrl.StageStatus = STATUS_FINISH;
#endif
//~Maple@JJ0403 CS
    }

}

void CS_INTERN_GetBayer(void)
{
    IME_OPENOBJ IMEOpenObj;
    IME_MODE_PRAM OneIme;

    if(g_ColorShadingCtrl.StageStatus==STATUS_START)
    {
        ClrShade_ind(("tbl2Byr"));
        ipe_reset(ON);
        ipe_reset(OFF);
        pll_enableSystemReset(IME_RSTN);
        pll_disableSystemReset(IME_RSTN);

        OneIme.op_mode = _D2D;
        OneIme.in_src = IMEALG_DMA2IME;

        OneIme.format.in_format      = IME_IN_FMT_422;
        OneIme.in_size.h_size        = g_ColorShadingCtrl.TableHSize;
        OneIme.in_size.v_size        = g_ColorShadingCtrl.TableVSize;
        OneIme.line_ofs.in_lineofs   = g_ColorShadingCtrl.TableHSize;
        OneIme.in_addr.y_addr        = g_ColorShadingCtrl.CurrTblAdd;
        OneIme.in_addr.cb_addr       = g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize);
        OneIme.in_addr.cr_addr       = g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize*2);

        OneIme.ycc_format.in_format  = 0;//pScale->in_pack;

        OneIme.path_num = IMEALG_1PATH;

        OneIme.format.out1_format    = IME_OUT_FMT_422_COSITED;
        OneIme.p1_out_size.h_size    = g_ColorShadingCtrl.BayerHSize;
        OneIme.p1_out_size.v_size    = g_ColorShadingCtrl.BayerVSize;

        OneIme.line_ofs.out1_lineofs = g_ColorShadingCtrl.BayerHSize;
        OneIme.p1_out0_addr.y_addr   = g_ColorShadingCtrl.GainBayAdd;
        OneIme.p1_out0_addr.cb_addr  = g_ColorShadingCtrl.GainBayAdd;
        OneIme.p1_out0_addr.cr_addr  = g_ColorShadingCtrl.GainBayAdd;

        OneIme.format.out1_format= IMEALG_BAYER;
        
        OneIme.ycc_format.out1_format= IMEALG_YCBCR_PLANAR;
        OneIme.ycc_format.out2_format= IMEALG_YCBCR_PLANAR;

        IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
        IMEOpenObj.FP_IMEISR_CB = NULL;
        ime_open(&IMEOpenObj);
        ime_setMode(&OneIme);
        ime_clrIntFlag();
        ime_setStart();
        ime_waitFlagFrameEnd();
        ime_setStop();
        ime_close();

/*
        ime_DMAScale2Bayer_NoWait(  g_ColorShadingCtrl.CurrTblAdd,
                                    g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize),
                                    g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize*2),
                                    g_ColorShadingCtrl.TableHSize,
                                    0,
                                    g_ColorShadingCtrl.TableHSize,
                                    g_ColorShadingCtrl.TableVSize,
                                    0,
                                    g_ColorShadingCtrl.GainBayAdd,
                                    g_ColorShadingCtrl.BayerHSize,
                                    g_ColorShadingCtrl.BayerHSize,
                                    g_ColorShadingCtrl.BayerVSize,
                                    g_ColorShadingCtrl.BayPattern);
*/
        g_ColorShadingCtrl.StageStatus ++;
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_1)
    {
        ime_waitFlagFrameEnd();
        ClrShade_ind(("done"));
        g_ColorShadingCtrl.StageStatus = STATUS_FINISH;
    }
}

void CS_INTERN_DoLine(void)
{
    UINT8 i;
    UINT16 FldLineCnt, FldCnt, TotalLineCnt;
    UINT32 RawByrAdd, GainByrAdd;

    if(g_ColorShadingCtrl.StageStatus==STATUS_START)
    {
        ClrShade_ind(("init"));

        for(i=0; i<CONST_CS_FIELDNUM_MAX; i++)
        {
            g_ColorShadingCtrl.LineCntBuf[i] = 0;
        }

        g_ColorShadingCtrl.StageStatus ++;
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_1)
    {
        if(g_ColorShadingCtrl.StageArg<g_ColorShadingCtrl.CapFldNum)
        {
            FldCnt = g_ColorShadingCtrl.StageArg;
            FldLineCnt = g_ColorShadingCtrl.LineCntBuf[FldCnt];

            ClrShade_ind(("f%d l%3d", FldCnt,FldLineCnt));
            if(FldLineCnt<g_ColorShadingCtrl.LinePre)
            {
                TotalLineCnt = (FldLineCnt*g_ColorShadingCtrl.CapFldNum)+FldCnt;

                RawByrAdd = g_ColorShadingCtrl.RawBayAdd[0] +
                            (TotalLineCnt*g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)/8;
                GainByrAdd= g_ColorShadingCtrl.GainBayAdd  +   TotalLineCnt*g_ColorShadingCtrl.BayerHSize;

        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(GainByrAdd, g_ColorShadingCtrl.BayerHSize, 1,((g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)>>3));
        grph_setImg2(RawByrAdd,g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth/8);
        grph_setAOP2(g_ColorShadingCtrl.RawBitDepth,GRPH_DST_2,GRPH_AOP_MULTIPLY_DIV,6,(GainByrAdd &0x3)<<3,(RawByrAdd &0x3)<<3);
        grph_disableDMA();
        grph_close();
/*
                Graph_Multi(GainByrAdd,
                            RawByrAdd,
                            g_ColorShadingCtrl.BayerHSize,
                            1,
                            g_ColorShadingCtrl.BayerHSize,
                            g_ColorShadingCtrl.RawBitDepth,
                            (GainByrAdd &0x3)<<3,
                            (RawByrAdd &0x3)<<3);
*/
                g_ColorShadingCtrl.LineCntBuf[FldCnt]++;
            }
        }
        else if(g_ColorShadingCtrl.StageArg==g_ColorShadingCtrl.CapFldNum)
        {
            g_ColorShadingCtrl.StageStatus ++ ;
        }
        else
        {
            //You should never be here.
        }
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_2)
    {
        g_ColorShadingCtrl.LineCntMax = 0;

        for(i=0; i<g_ColorShadingCtrl.CapFldNum; i++)
        {
            if(g_ColorShadingCtrl.LineCntBuf[i] > g_ColorShadingCtrl.LineCntMax)
            {
                g_ColorShadingCtrl.LineCntMax = g_ColorShadingCtrl.LineCntBuf[i];
            }
        }

//Ginger@JJ@070813 ColorShading
        ClrShade_err(("L "));
        for(i=0; i<g_ColorShadingCtrl.CapFldNum; i++)
        {
            ClrShade_err(("%3d ", g_ColorShadingCtrl.LineCntBuf[i]));
        }
        ClrShade_err(("max%3d\r\n", g_ColorShadingCtrl.LineCntMax));
//~Ginger@JJ@070813 ColorShading

        g_ColorShadingCtrl.StageStatus++;
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_3)
    {


        for(FldCnt=0; FldCnt<g_ColorShadingCtrl.CapFldNum; FldCnt++)
        {
            FldLineCnt = g_ColorShadingCtrl.LineCntBuf[FldCnt];
            if(FldLineCnt < g_ColorShadingCtrl.LineCntMax)
            {
                break;
            }
        }

        ClrShade_ind(("f%d l%3d", FldCnt,FldLineCnt));

        if(FldCnt<g_ColorShadingCtrl.CapFldNum)
        {
            TotalLineCnt = (FldLineCnt*g_ColorShadingCtrl.CapFldNum)+FldCnt;

            RawByrAdd = g_ColorShadingCtrl.RawBayAdd[0] +
                        (TotalLineCnt*g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)/8;
            GainByrAdd= g_ColorShadingCtrl.GainBayAdd  +   TotalLineCnt*g_ColorShadingCtrl.BayerHSize;

        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(GainByrAdd, g_ColorShadingCtrl.BayerHSize, 1,((g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)>>3));
        grph_setImg2(RawByrAdd,g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth/8);
        grph_setAOP2(g_ColorShadingCtrl.RawBitDepth,GRPH_DST_2,GRPH_AOP_MULTIPLY_DIV,6,(GainByrAdd &0x3)<<3,(RawByrAdd &0x3)<<3);
        grph_disableDMA();
        grph_close();
/*
            Graph_Multi(GainByrAdd,
                        RawByrAdd,
                        g_ColorShadingCtrl.BayerHSize,
                        1,
                        g_ColorShadingCtrl.BayerHSize,
                        g_ColorShadingCtrl.RawBitDepth,
                        (GainByrAdd &0x3)<<3,
                        (RawByrAdd &0x3)<<3);
*/
            g_ColorShadingCtrl.LineCntBuf[FldCnt]++;
        }
        else
        {
            g_ColorShadingCtrl.StageStatus++;
        }

    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_4)
    {
        //Add "check-if-finished" code here
        {
            g_ColorShadingCtrl.StageStatus = STATUS_FINISH;
        }
    }
}

void CS_INTERN_DoRegion(void)
{
    UINT8 i;
    UINT16 TotalLineCnt;
    UINT32 RawByrAdd, GainByrAdd;

    if(g_ColorShadingCtrl.StageStatus==STATUS_START)
    {
        debug_msg("aaaaaaaaaaaa\r\n");
        TotalLineCnt = (g_ColorShadingCtrl.LineCntMax*g_ColorShadingCtrl.CapFldNum);

        ClrShade_ind(("TotalLineCnt%3d", TotalLineCnt));

        RawByrAdd = g_ColorShadingCtrl.RawBayAdd[0] +
                    (TotalLineCnt*g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)/8;
        GainByrAdd= g_ColorShadingCtrl.GainBayAdd  +   TotalLineCnt*g_ColorShadingCtrl.BayerHSize;

//Maple@JJ0328 Performance
        //Graph_Multi(GainByrAdd,
        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(GainByrAdd, g_ColorShadingCtrl.BayerHSize, g_ColorShadingCtrl.BayerVSize-TotalLineCnt,((g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)>>3));
        grph_setImg2(RawByrAdd,g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth/8);
        grph_setAOP2(g_ColorShadingCtrl.RawBitDepth,GRPH_DST_2,GRPH_AOP_MULTIPLY_DIV,6,(GainByrAdd &0x3)<<3,(RawByrAdd &0x3)<<3);
        grph_disableDMA();
        grph_close();
/*        
        Graph_Multi_NoWait(GainByrAdd,
//~Maple@JJ0328 Performance
                    RawByrAdd,
                    g_ColorShadingCtrl.BayerHSize,
                    g_ColorShadingCtrl.BayerVSize-TotalLineCnt,
                    g_ColorShadingCtrl.BayerHSize,
                    g_ColorShadingCtrl.RawBitDepth,
                    (GainByrAdd &0x3)<<3,
                    (RawByrAdd &0x3)<<3);
*/
        g_ColorShadingCtrl.StageStatus++;
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_1)
    {
        //Add "check-if-finished" code here
//Maple@JJ0328 Performance
//        if(Graph_Multi_CheckEnd())
//~Maple@JJ0328 Performance
        {
            ClrShade_ind(("done"));
            g_ColorShadingCtrl.StageStatus = STATUS_FINISH;
        }
    }
}

extern void CmdTsk_DoCommand_Extern(CHAR *pcCmdStr);


void CS_INTERN_DoPartial(void)
{
    UINT32 i, j, uiTmp;
    UINT16  CS_ByrVstart, CS_ByrVend, CS_ByrSkipTop, CS_ByrSkipButt;
    UINT16  CS_TblVstart, CS_TblVend, CS_TblSkipTop, CS_TblSkipButt;
    IME_OPENOBJ IMEOpenObj;
    IME_MODE_PRAM OneIme;

    if(g_ColorShadingCtrl.StageStatus==STATUS_START)
    {
        for(i=0; i<g_ColorShadingCtrl.BurstDivision; i++)
        {
            CS_TblSkipButt = 0;
            CS_TblVstart = (g_ColorShadingCtrl.TableVSize*i)/g_ColorShadingCtrl.BurstDivision;
            CS_TblVend   = (g_ColorShadingCtrl.TableVSize*(i+1))/g_ColorShadingCtrl.BurstDivision;
            CS_ByrVstart = (g_ColorShadingCtrl.BayerVSize*i)/g_ColorShadingCtrl.BurstDivision;
            CS_ByrVend   = (g_ColorShadingCtrl.BayerVSize*(i+1))/g_ColorShadingCtrl.BurstDivision;

            if(CS_ByrVstart%2)
                CS_ByrVstart--;
            if(CS_ByrVend%2)
                CS_ByrVend--;

            if(i!=0)
            {
                CS_TblSkipTop  = 1;
                CS_ByrSkipTop  = (CS_ByrVend-CS_ByrVstart)/(CS_TblVend-CS_TblVstart);
            }
            else
            {
                CS_TblSkipTop  = 0;
                CS_ByrSkipTop  = 0;
            }
            if(i!=(g_ColorShadingCtrl.BurstDivision-1))
            {
                CS_TblSkipButt = 1;
                CS_ByrSkipButt  = (CS_ByrVend-CS_ByrVstart)/(CS_TblVend-CS_TblVstart);
            }
            else
            {
                CS_TblSkipButt = 0;
                CS_ByrSkipButt  = 0;
            }

            if(CS_ByrSkipTop%2)
                CS_ByrSkipTop--;//CS_ByrSkipTop++;
            //if(CS_TblVstart%2)
                //CS_TblVstart++;


            ClrShade_ind(("CS_ByrSkipxxx %d %d\r\n", CS_ByrSkipTop, CS_ByrSkipButt));
            ClrShade_ind(("CS_TblVxxx %d %d\r\n", CS_TblVstart, CS_TblVend));
            ClrShade_ind(("CS_ByrVxxx %d %d\r\n", CS_ByrVstart, CS_ByrVend));

            uiTmp=(g_ColorShadingCtrl.TableHSize*(CS_TblVstart-CS_TblSkipTop));


            OneIme.op_mode = _D2D;
            OneIme.in_src = IMEALG_DMA2IME;

            OneIme.format.in_format      = IME_IN_FMT_422;
            OneIme.in_size.h_size        = g_ColorShadingCtrl.TableHSize;
            OneIme.in_size.v_size        = (CS_TblVend+CS_TblSkipButt)-(CS_TblVstart-CS_TblSkipTop);
            OneIme.line_ofs.in_lineofs   = g_ColorShadingCtrl.TableHSize;
            OneIme.in_addr.y_addr        = uiTmp + g_ColorShadingCtrl.CurrTblAdd;
            OneIme.in_addr.cb_addr       = uiTmp + g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize);
            OneIme.in_addr.cr_addr       = uiTmp + g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize*2);

            OneIme.ycc_format.in_format  = 0;//pScale->in_pack;

            OneIme.path_num = IMEALG_1PATH;

            OneIme.format.out1_format    = IME_OUT_FMT_422_COSITED;
            OneIme.p1_out_size.h_size    = g_ColorShadingCtrl.BayerHSize;
            OneIme.p1_out_size.v_size    = (CS_ByrVend+CS_ByrSkipButt)-(CS_ByrVstart-CS_ByrSkipTop);

            OneIme.line_ofs.out1_lineofs = g_ColorShadingCtrl.BayerHSize;
            OneIme.p1_out0_addr.y_addr   = g_ColorShadingCtrl.GainBayAdd;
            OneIme.p1_out0_addr.cb_addr  = g_ColorShadingCtrl.GainBayAdd;
            OneIme.p1_out0_addr.cr_addr  = g_ColorShadingCtrl.GainBayAdd;

            OneIme.format.out1_format= IMEALG_BAYER;
            
            OneIme.ycc_format.out1_format= IMEALG_YCBCR_PLANAR;
            OneIme.ycc_format.out2_format= IMEALG_YCBCR_PLANAR;

            IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
            IMEOpenObj.FP_IMEISR_CB = NULL;
            ime_open(&IMEOpenObj);
            ime_setMode(&OneIme);
            ime_clrIntFlag();
            ime_setStart();
            ime_waitFlagFrameEnd();
            ime_setStop();
            ime_close();

/*            
            ime_DMAScale2Bayer( uiTmp + g_ColorShadingCtrl.CurrTblAdd,
                                uiTmp + g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize),
                                uiTmp + g_ColorShadingCtrl.CurrTblAdd+(g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize*2),
                                g_ColorShadingCtrl.TableHSize,  0,  g_ColorShadingCtrl.TableHSize,
                                (CS_TblVend+CS_TblSkipButt)-(CS_TblVstart-CS_TblSkipTop),
                                0,
                                g_ColorShadingCtrl.GainBayAdd,
                                g_ColorShadingCtrl.BayerHSize,
                                g_ColorShadingCtrl.BayerHSize,
                                (CS_ByrVend+CS_ByrSkipButt)-(CS_ByrVstart-CS_ByrSkipTop),
                                g_ColorShadingCtrl.BayPattern);
            */

#if 0//JJtest
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*) g_ColorShadingCtrl.GainBayAdd+(g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop) ,
                    g_ColorShadingCtrl.BayerHSize*(CS_ByrVend-CS_ByrVstart),
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
#endif
            for(j=0; j<g_ColorShadingCtrl.BurstPicNum; j++)
            {
//debug_err(("GainBayADd %08x  Param-1 %08x\r\n", g_ColorShadingCtrl.GainBayAdd, (g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop)));
//debug_err(("RawBayAdd[%d] %08x  Param-1 %08x\r\n", j, g_ColorShadingCtrl.RawBayAdd[j], (g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop)));
//debug_err(("CS_ByrVstart %d\r\n", CS_ByrVstart));

        grph_open();
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(g_ColorShadingCtrl.GainBayAdd+(g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop), g_ColorShadingCtrl.BayerHSize, CS_ByrVend-CS_ByrVstart,((g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth)>>3));
        grph_setImg2(g_ColorShadingCtrl.RawBayAdd[j]+(g_ColorShadingCtrl.BayerHSize*CS_ByrVstart),g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.RawBitDepth/8);
        grph_setAOP2(g_ColorShadingCtrl.RawBitDepth,GRPH_DST_2,GRPH_AOP_MULTIPLY_DIV,6,((g_ColorShadingCtrl.GainBayAdd+(g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop))&0x3)<<3,((g_ColorShadingCtrl.RawBayAdd[j]+(g_ColorShadingCtrl.BayerHSize*CS_ByrVstart))&0x3)<<3);
        grph_disableDMA();
        grph_close();
/*
                Graph_Multi(g_ColorShadingCtrl.GainBayAdd+(g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop),
                            g_ColorShadingCtrl.RawBayAdd[j]+(g_ColorShadingCtrl.BayerHSize*CS_ByrVstart),
                            g_ColorShadingCtrl.BayerHSize,
                            CS_ByrVend-CS_ByrVstart,
                            g_ColorShadingCtrl.BayerHSize,
                            g_ColorShadingCtrl.RawBitDepth,
                            ((g_ColorShadingCtrl.GainBayAdd+(g_ColorShadingCtrl.BayerHSize*CS_ByrSkipTop))&0x3)<<3,
                            ((g_ColorShadingCtrl.RawBayAdd[j]+(g_ColorShadingCtrl.BayerHSize*CS_ByrVstart))&0x3)<<3);
*/
//CmdTsk_DoCommand_Extern("r 97110000\r\n");

#if 0//JJtest
        if(i>2)
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*) g_ColorShadingCtrl.RawBayAdd[j],
                    g_ColorShadingCtrl.BayerHSize*g_ColorShadingCtrl.BayerVSize,
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
#endif
            }
        }
#if 0//JJtest
        FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*) g_ColorShadingCtrl.CurrTblAdd ,
                    g_ColorShadingCtrl.TableHSize*g_ColorShadingCtrl.TableVSize*3,
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
#endif

        g_ColorShadingCtrl.StageStatus++;
    }
    else if(g_ColorShadingCtrl.StageStatus==STATUS_1)
    {
        //Add "check-if-finished" code here
        {
            ClrShade_ind(("done"));
            g_ColorShadingCtrl.StageStatus = STATUS_FINISH;
        }
    }
}


//__align(4) unsigned char ColorShadingTbl[3][ColorShadingTbl_V][ColorShadingTbl_H];


void SaveGainTable(unsigned long Add,unsigned long size, unsigned char byte) //<--- 喷靡ノ, 干纕弄
{
    HNVT_FILE *pFile;
    char Data[7];

    pFile = fs_fopen("A:\\Data444.raw","w");
    if(pFile == NULL)
        return ;

    fs_fwrite((UB*)Add, byte, size, pFile);
    fs_fclose(pFile);
}


void LoadGainTable(unsigned long Add,unsigned long size, unsigned char byte)
{
    HNVT_FILE *pFile;
    char Data[7];

    pFile = fs_fopen("A:\\Data444.raw","r");
    if(pFile == NULL)
        return ;

    fs_fread((UB*)Add, byte, size, pFile);
    fs_fclose(pFile);
}


void ColorShading_Cal(unsigned char* ColorShadingTbl, unsigned long Add, unsigned long Width,unsigned long Height,unsigned short MaxTbl_H,unsigned short MaxTbl_V,unsigned char StartPixel)
{
    unsigned short i,j,k;
    unsigned short Hori_Step,Vert_Step,Max_R=0,Max_G=0,Max_B=0,Data_R,Data_G,Data_B,offsetH,offsetV;
    unsigned short StartH,StartV,SizeH,SizeV,temp;
    unsigned long DataEnd;

    unsigned char endl[2]={0X0D,0X0A},byte;
    unsigned char *buff;
    unsigned short MaxValue;

    if(CS_GainRes<8)
    {
        MaxValue = 0xff;
        byte = 1;
    }
    else
    {
        MaxValue = 0xffff;
        byte = 2;
    }

    DataEnd = Add+Width*Height;
    MaxTbl_H = ColorShadingTbl_H;
    MaxTbl_V = ColorShadingTbl_V;

    Hori_Step = Width/(MaxTbl_H-1);
    Vert_Step = Height/(MaxTbl_V-1);

    if(Hori_Step%2)
        Hori_Step--;
    if(Vert_Step%2)
        Vert_Step--;

/*
    if(Hori_Step%4)
        offsetH=Hori_Step/2+1;
    else
        offsetH=Hori_Step/2;

    if(Vert_Step%4)
        offsetV=Vert_Step/2+1;
    else
        offsetV=Vert_Step/2;
*/
    //Hori_Step = 8;
    //Vert_Step = 8;

    for(j=0;j<(MaxTbl_V);j++)
    {
        if(j==0)
        {
            SizeV = Vert_Step/2;
            StartV = 0;
        }
        else if(j==(MaxTbl_V-1))
        {
            SizeV = Vert_Step/2;
            StartV = Height-SizeV;
        }
        else
        {
            SizeV = Vert_Step;
            StartV = (Height*j/(MaxTbl_V-1))-(SizeV/2);//Vert_Step*j;//-offsetV;
        }
        if(StartV%2)
            StartV--;

        for(i=0;i<(MaxTbl_H);i++)
        {
            if(i==0)
            {
                SizeH = Hori_Step/2;
                StartH = 0;
            }
            else if(i == (MaxTbl_H-1))
            {
                SizeH = Hori_Step/2;
                StartH = Width-SizeH;
            }
            else
            {
                SizeH = Hori_Step;
                StartH = (Width*i/(MaxTbl_H-1))-(SizeH/2);//Hori_Step*i;//-offsetH;
            }
            if(StartH%2)
                StartH--;
            CalculateRawY(Add, StartH, StartV, SizeH, SizeV, 1, StartPixel, &Data_R,&Data_G,&Data_B);
            if(Data_R>Max_R)
                Max_R = Data_R;
            if(Data_G>Max_G)
                Max_G = Data_G;
            if(Data_B>Max_B)
                Max_B = Data_B;
        }
    }
    for(j=0;j<(MaxTbl_V);j++)
    {
        if(j==0)
        {
            SizeV = Vert_Step/2;
            StartV = 0;
        }
        else if(j==(MaxTbl_V-1))
        {
            SizeV = Vert_Step/2;
            StartV = Height-SizeV;
        }
        else
        {
            SizeV = Vert_Step;
            StartV = (Height*j/(MaxTbl_V-1))-(SizeV/2);//Vert_Step*j;//-offsetV;
        }
        if(StartV%2)
            StartV--;

        for(i=0;i<(MaxTbl_H);i++)
        {
            if(i==0)
            {
                SizeH = Hori_Step/2;
                StartH = 0;
            }
            else if(i == (MaxTbl_H-1))
            {
                SizeH = Hori_Step/2;
                StartH = Width-SizeH;
            }
            else
            {
                SizeH = Hori_Step;
                StartH = (Width*i/(MaxTbl_H-1))-(SizeH/2);//Hori_Step*i;//-offsetH;
            }
            if(StartH%2)
                StartH--;
            CalculateRawY(Add, StartH, StartV, SizeH, SizeV, 1, StartPixel, &Data_R,&Data_G,&Data_B);

            temp = (Max_R<<CS_GainRes)/Data_R;
            /*
            if(temp>=MaxValue)
                ColorShadingTbl[0][j][i] = MaxValue;
            else
                ColorShadingTbl[0][j][i] = temp;

            temp = (Max_G<<CS_GainRes)/Data_G;
            if(temp>=MaxValue)
                ColorShadingTbl[1][j][i] = MaxValue;
            else
                ColorShadingTbl[1][j][i] = temp;

            temp = (Max_B<<CS_GainRes)/Data_B;
            if(temp>=MaxValue)
                ColorShadingTbl[2][j][i] = MaxValue;
            else
                ColorShadingTbl[2][j][i] = temp;
*/
            if(temp>=MaxValue)
                ColorShadingTbl[j * MaxTbl_H + i] = MaxValue;
            else
                ColorShadingTbl[j * MaxTbl_H + i] = temp;

            temp = (Max_G<<CS_GainRes)/Data_G;
            if(temp>=MaxValue)
                ColorShadingTbl[MaxTbl_H * MaxTbl_V + j * MaxTbl_H + i] = MaxValue;
            else
                ColorShadingTbl[MaxTbl_H * MaxTbl_V + j * MaxTbl_H + i] = temp;

            temp = (Max_B<<CS_GainRes)/Data_B;
            if(temp>=MaxValue)
                ColorShadingTbl[MaxTbl_H * MaxTbl_V * 2 + j * MaxTbl_H + i] = MaxValue;
            else
                ColorShadingTbl[MaxTbl_H * MaxTbl_V * 2 + j * MaxTbl_H + i] = temp;

        }
    }
#if 0
    for(j=0;j<(MaxTbl_V);j++)
    {
        for(i=2;i<(MaxTbl_H-2);i++)
        {
            for(k=0;k<3;k++)
            {
                if((ColorShadingTbl[k][j][i]>ColorShadingTbl[k][j][i-1] && ColorShadingTbl[k][j][i]>ColorShadingTbl[k][j][i+1])
                    ||(ColorShadingTbl[k][j][i]<ColorShadingTbl[k][j][i-1] && ColorShadingTbl[k][j][i]<ColorShadingTbl[k][j][i+1]))
                {
                    ColorShadingTbl[k][j][i] = (ColorShadingTbl[k][j][i-2]+ColorShadingTbl[k][j][i-1]+ColorShadingTbl[k][j][i+1]+ColorShadingTbl[k][j][i+2])/4;
                }
            }
        }
    }
#endif
    //SaveGainTable((unsigned long)ColorShadingTbl,ColorShadingTbl_H*ColorShadingTbl_V*3,byte);

/*
    for(j=1;j<(MaxTbl_V-1);j++)
    {
        for(i=1;i<(MaxTbl_H-1);i++)
        {
            for(k=0;k<3;k++)
            {
                temp = 0;
                temp += ColorShadingTbl[k][j-1][i-1];
                temp += ColorShadingTbl[k][j][i-1];
                temp += ColorShadingTbl[k][j+1][i-1];
                temp += ColorShadingTbl[k][j-1][i];
                //temp += ColorShadingTbl[k][j][i];
                temp += ColorShadingTbl[k][j+1][i];
                temp += ColorShadingTbl[k][j-1][i+1];
                temp += ColorShadingTbl[k][j][i+1];
                temp += ColorShadingTbl[k][j+1][i+1];
                ColorShadingTbl[k][j][i] = (unsigned char)(((temp>>3)+ColorShadingTbl[k][j][i])>>1);
            }
        }
    }

    for(j=0;j<64;j++)
    {
        for(i=0;i<64;i+=4)
        {
            WeightingTable[j][i/4][0] = (64-i)*(64-j);
            WeightingTable[j][i/4][1] = i*(64-j);
            WeightingTable[j][i/4][2] = (64-i)*j;
            WeightingTable[j][i/4][3] = i*j;
        }
    }

    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            for(k=0;k<3;k++)
            {
                 AAA_Adjust.Color_Table[k][j][i] = ColorShadingTbl[k][j][i];
            }
        }
    }
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
            &ColorShadingTbl[0][0][0],//    UB *pBuf,
            ColorShadingTbl_H*ColorShadingTbl_V*3,//JJ_UI_FileFmt
            0,//UW Pos,
            1,//BOOL CloseFile,
            FST_FMT_RAW,//UW FileFormat,
            FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
*/
/*
    buff = (UB*)DataEnd;

    //buff += sprintf((char *)buff,"%s",endqty);
    buff += sprintf((char *)buff,"%s",endl);
    //buff += sprintf((char *)buff,"%s\t  1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t11\t12%s", endl, endl);
    //*buff++= '\t';
    //for(  i = 0; i < 80; i++ )
    //    *buff++ = '-' ;
    //buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"MaxR=%d MaxG=%d MaxB=%d%s%s",Max_R,Max_G,Max_B,endl,endl);

    buff += sprintf((char *)buff,"R data: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%d\t",Color_Tbl[0][j][i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"G data: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%d\t",Color_Tbl[1][j][i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }
    buff += sprintf((char *)buff,"%s", endl);

    buff += sprintf((char *)buff,"B data: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%d\t",Color_Tbl[2][j][i]);
        }
        buff += sprintf((char *)buff,"%s", endl);
    }
    buff += sprintf((char *)buff,"%s", endl);

    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                (UB*)DataEnd,// UB *pBuf,
                (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                0,//UW Pos,
                1,//BOOL CloseFile,
                FST_FMT_RAW,//UW FileFormat,
                FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
    buff = (UB*)DataEnd;


    //buff += sprintf((char *)buff,"R gain: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%x",ColorShadingTbl[0][j][i]);
            buff += sprintf((char *)buff,"%s", endl);
        }
    }
    //buff += sprintf((char *)buff,"%s", endl);
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                (UB*)DataEnd,// UB *pBuf,
                (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                0,//UW Pos,
                1,//BOOL CloseFile,
                FST_FMT_RAW,//UW FileFormat,
                FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
    buff = (UB*)DataEnd;

    //buff += sprintf((char *)buff,"G gain: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%x",ColorShadingTbl[1][j][i]);
            buff += sprintf((char *)buff,"%s", endl);
        }
    }
    //buff += sprintf((char *)buff,"%s", endl);
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                (UB*)DataEnd,// UB *pBuf,
                (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                0,//UW Pos,
                1,//BOOL CloseFile,
                FST_FMT_RAW,//UW FileFormat,
                FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
    buff = (UB*)DataEnd;

    //buff += sprintf((char *)buff,"B gain: %s",endl);
    for(j=0;j<MaxTbl_V;j++)
    {
        for(i=0;i<MaxTbl_H;i++)
        {
            buff += sprintf((char *)buff,"%x",ColorShadingTbl[2][j][i]);
            buff += sprintf((char *)buff,"%s", endl);
        }
    }

    //buff += sprintf((char *)buff,"%s", endl);
    //buff += sprintf((char *)buff,"%s", endl);

    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)DataEnd,// UB *pBuf,
                    (buff-(UB*)DataEnd),//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
*/
}


void ColorShadingCorrection(UB *ColorShadingTbl,unsigned long Add, unsigned long Width,unsigned long Height,unsigned char StartPixel)
{
    unsigned long i,j,gain0,gain1,OriData,TargData,w,count;
    unsigned long Data[4];
    unsigned long Hori_Step,Vert_Step,Gain_LU,Gain_RU,Gain_LD,Gain_RD,Tbl_H,Tbl_V,Dist_H,Dist_V;
    unsigned short Weig_LU,Weig_RU,Weig_LD,Weig_RD;
    unsigned short Area;
    unsigned char Tbl_Sel[4],Tbl_0,Tbl_1;
    unsigned short MaxTbl_H,MaxTbl_V;

    MaxTbl_H = ColorShadingTbl_H;
    MaxTbl_V = ColorShadingTbl_V;

    if(StartPixel == Color_R)
    {
        Tbl_Sel[0] = 0;
        Tbl_Sel[1] = 1;
        Tbl_Sel[2] = 1;
        Tbl_Sel[3] = 2;
    }
    else if(StartPixel == Color_Gr)
    {
        Tbl_Sel[0] = 1;
        Tbl_Sel[1] = 0;
        Tbl_Sel[2] = 2;
        Tbl_Sel[3] = 1;
    }
    else if(StartPixel == Color_Gb)
    {
        Tbl_Sel[0] = 1;
        Tbl_Sel[1] = 2;
        Tbl_Sel[2] = 0;
        Tbl_Sel[3] = 1;
    }
    else if(StartPixel == Color_B)
    {
        Tbl_Sel[0] = 2;
        Tbl_Sel[1] = 1;
        Tbl_Sel[2] = 1;
        Tbl_Sel[3] = 0;
    }

    Hori_Step = Width/(MaxTbl_H-1);
    Vert_Step = Height/(MaxTbl_V-1);
    Area = Hori_Step*Vert_Step;

    for(j=0;j<Height;j++)
    {
        Tbl_V = j/Vert_Step;
        Dist_V = j%Vert_Step;
        if(j%2)
        {
            Tbl_0 = Tbl_Sel[2];
            Tbl_1 = Tbl_Sel[3];
        }
        else
        {
            Tbl_0 = Tbl_Sel[0];
            Tbl_1 = Tbl_Sel[1];
        }
        Tbl_H = 0;
        Dist_H = 0;
        for(i=0;i<Width;i+=4)
        {
            OriData = *(UW*)Add;
            Data[0]= OriData&0xff;
            Data[1]=(OriData>>8)&0xff;
            Data[2]=(OriData>>16)&0xff;
            Data[3]=(OriData>>24)&0xff;

            Tbl_H = i/Hori_Step;
            Dist_H = i%Hori_Step;
            Weig_LU = (Hori_Step-Dist_H)*(Vert_Step-Dist_V);
            Weig_RU = (Dist_H)*(Vert_Step-Dist_V);
            Weig_LD = (Hori_Step-Dist_H)*Dist_V;
            Weig_RD = Dist_H*Dist_V;

            //Gain_LU = ColorShadingTbl[Tbl_0][Tbl_V][Tbl_H];//GetColorShadingGain(Tbl_0,Tbl_H,Tbl_V);
            //Gain_RU = ColorShadingTbl[Tbl_0][Tbl_V][Tbl_H+1];//GetColorShadingGain(Tbl_0,Tbl_H+1,Tbl_V);
            //Gain_LD = ColorShadingTbl[Tbl_0][Tbl_V+1][Tbl_H];//GetColorShadingGain(Tbl_0,Tbl_H,Tbl_V+1);
            //Gain_RD = ColorShadingTbl[Tbl_0][Tbl_V+1][Tbl_H+1];//GetColorShadingGain(Tbl_0,Tbl_H+1,Tbl_V+1);
            Gain_LU = ColorShadingTbl[Tbl_0*ColorShadingTbl_H*ColorShadingTbl_V+(Tbl_V*ColorShadingTbl_H)+Tbl_H];
            Gain_RU = ColorShadingTbl[Tbl_0*ColorShadingTbl_H*ColorShadingTbl_V+(Tbl_V*ColorShadingTbl_H)+Tbl_H+1];
            Gain_LD = ColorShadingTbl[Tbl_0*ColorShadingTbl_H*ColorShadingTbl_V+((Tbl_V+1)*ColorShadingTbl_H)+Tbl_H];
            Gain_RD = ColorShadingTbl[Tbl_0*ColorShadingTbl_H*ColorShadingTbl_V+((Tbl_V+1)*ColorShadingTbl_H)+Tbl_H+1];

            gain0 = (Gain_LU*Weig_LU+Gain_RU*Weig_RU+Gain_LD*Weig_LD+Gain_RD*Weig_RD)/Area;

            //Gain_LU = ColorShadingTbl[Tbl_1][Tbl_V][Tbl_H];//GetColorShadingGain(Tbl_1,Tbl_H,Tbl_V);
            //Gain_RU = ColorShadingTbl[Tbl_1][Tbl_V][Tbl_H+1];//GetColorShadingGain(Tbl_1,Tbl_H+1,Tbl_V);
            //Gain_LD = ColorShadingTbl[Tbl_1][Tbl_V+1][Tbl_H];//GetColorShadingGain(Tbl_1,Tbl_H,Tbl_V+1);
            //Gain_RD = ColorShadingTbl[Tbl_1][Tbl_V+1][Tbl_H+1];//GetColorShadingGain(Tbl_1,Tbl_H+1,Tbl_V+1);
            Gain_LU = ColorShadingTbl[Tbl_1*ColorShadingTbl_H*ColorShadingTbl_V+(Tbl_V*ColorShadingTbl_H)+Tbl_H];
            Gain_RU = ColorShadingTbl[Tbl_1*ColorShadingTbl_H*ColorShadingTbl_V+(Tbl_V*ColorShadingTbl_H)+Tbl_H+1];
            Gain_LD = ColorShadingTbl[Tbl_1*ColorShadingTbl_H*ColorShadingTbl_V+((Tbl_V+1)*ColorShadingTbl_H)+Tbl_H];
            Gain_RD = ColorShadingTbl[Tbl_1*ColorShadingTbl_H*ColorShadingTbl_V+((Tbl_V+1)*ColorShadingTbl_H)+Tbl_H+1];

            gain1 = (Gain_LU*Weig_LU+Gain_RU*Weig_RU+Gain_LD*Weig_LD+Gain_RD*Weig_RD)/Area;
            Data[0] = ((Data[0]*gain0)>>CS_GainRes);
            if(Data[0]>0xff)
                Data[0] = 0xff;
            //w+=1;
            Data[1] = ((Data[1]*gain1)>>CS_GainRes);
            if(Data[1]>0xff)
                Data[1] = 0xff;
            //w+=1;
            Data[2] = ((Data[2]*gain0)>>CS_GainRes);
            if(Data[2]>0xff)
                Data[2] = 0xff;
            //w+=1;
            Data[3] = ((Data[3]*gain1)>>CS_GainRes);
            if(Data[3]>0xff)
                Data[3] = 0xff;
            TargData = (Data[3]<<24) |(Data[2]<<16)|(Data[1]<<8)|(Data[0]);

            *(UW*)Add = TargData;
            Add+=4;
        }
    }
}



