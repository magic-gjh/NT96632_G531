
#include "type.h"
#include "debug.h"

#include "sensor_model.h"
#include "Ime.h"
#include "OSCommon.h"
#include "interrupt.h"
#include "Timer.h"
#include "pll.h"
#include "JpgEnc.h"
#include "JPEG.h"
#include "Grph.h"
#include "Filesystsk.h"
#include "Top.h"

#include "utility.h"
#include  "Memory.h"

#include "ImageQualitySim.h"

//#NT#2009/11/23#Linkin Huang -begin
//#NT# add for Simulation tool
//BOOL SimToolEn = FALSE;
//#NT#2009/11/23#Linkin Huang -end

//#NT#2010/06/01/#Likin Huang -begin
//#NT# define message flag
#define SimMsgFlag  0
//#NT#2010/06/01#Linkin Huang -end

//===================================================================================================

typedef struct
{
  UINT32 BinAddr;
  UINT32 inRawAddr;
  UINT32 outRawAddr;
  UINT32 YCCAddr;
  UINT32 JpegAddr;
  UINT32 TmpBufAddr;  
} WORKIN_BUFFER_INFO;

typedef struct
{
  UINT32 rawWidth;
  UINT32 rawHeight;
  UINT32 rawLineOffset;
  UINT32 rawBitDepth;

  UINT32 outWidth;
  UINT32 outHeight;
  UINT32 outLineOffset;  
} BUFFER_ASIGNER_INFO;

typedef struct
{
    UINT32 bay_addr;
    UINT32 jpg_addr;
    UINT32 pingpong_addr;
    UINT32 bay_hsize;
    UINT32 bay_vsize;
    UINT32 jpg_hsize;
    UINT32 jpg_vsize;
    UINT32 jpg_filesize;
    UINT32 bay_packbit;
    UINT32 bay_startpix;
    UINT32 CF_addr;
} SIM_SLICEINFO_PRAM;

typedef struct
{
  UINT32 hSize;
  UINT32 vSize;
  UINT32 LineOffset;
  UINT8   cfa;
  UINT32 sourceAddr;
  UINT32 distAddr;
  UINT   ipe_GaEn;
  UINT32 *chrSmLUT;
  UINT32 *rGaLUT;
  UINT32 *gGaLUT;
  UINT32 *bGaLUT;
} CHROMA_CURVE_INFO;


typedef struct
{
    UINT32  src_skp;            ///< source skip select
    UINT32  src_shf;            ///< source shift select
    UINT32  binning;            ///< binning mode select
    UINT32  gau_length;         ///< lpf length1
    UINT32  edgdet_length;      ///< lpf length2
    UINT32  edgenh_length;      ///< lpf length3
    UINT32  xorg0_length;       ///< lpf length4
    UINT32  xorg1_length;       ///< lpf length5
    UINT32  xmod_length;        ///< lpf length6
    UINT32  avg_mask;           ///< average mask
    UINT32  avg_norm;           ///< average normalize
    UINT32  lpf_weighting;      ///< lpf out weighting
    UINT32  out_mux;            ///< final output mux
    UINT32  edgdet_mapa;        ///< edge detect mapa
    UINT32  edgdet_mapb;        ///< edge detect mapb
    UINT32  edgdet_mapc;        ///< edge detect mapc
    UINT32  edgdet_mapn;        ///< edge detect mapn
    UINT32  edgdet_shift;       ///< edge detect result shift
    UINT32  edgdet_mux;         ///< edge detect result mux
    UINT32  edgdet_threshold;   ///< edge detect threshold
    UINT32  edgenh_mapa;        ///< edge enhance mapa
    UINT32  edgenh_mapb;        ///< edge enhance mapb
    UINT32  edgenh_mapc;        ///< edge enhance mapc
    UINT32  edgenh_mapn;        ///< edge enhance mapn
    UINT32  edgenh_shift;       ///< edge enhance shift
    UINT32  edgenh_mux;         ///< edge enhance output mux
    UINT32  edgenh_mul;         ///< edge enhance multiple
    UINT32  repeat;             ///< chroma filter repeat num
} INDEP_FDE_CHR_FILT_PRAM;

typedef struct
{
  UINT32 BayerHsize;
  UINT32 BayerVsize;
  UINT32 BayerAddr;
  UINT8  PreBitDepth;
  UINT8  PreCfa;

  UINT32 ImeOutHsize;
  UINT32 ImeOutVsize;
  UINT32 ImeOutLineOffset;

  UINT32 SLC_BUF_ADDR;
  
  UINT32 JpgOutAddr;
  UINT32 JpegSize;
  UINT32 JPEGQuality;
} INDEP_SLICE_MODE_INFO;

typedef struct
{
  UINT32 Pre_CropHsize;
  UINT32 Pre_CropVsize;
  UINT8  BitDepth;
  
  STR_STRIPE_INFOR Hstripe;
  STR_STRIPE_INFOR Vstripe;
  
  UINT32 InAddr0;
  UINT32 cfapat;
  UINT32 ipe_omat;
} INDEP_IPE_SLICE_CFG_INFO;


typedef struct
{
    UINT GainCtrlA_MapA;
    UINT GainCtrlA_MapB;
    UINT GainCtrlB_MapA;
    UINT GainCtrlB_MapB;
    UINT GainCtrlA_GainA;
    UINT GainCtrlA_GainB;
    UINT GainCtrlA_Switch;
    UINT GainCtrlA_OffsetA;
    UINT GainCtrlA_OffsetB;
    UINT GainCtrlB_GainA;
    UINT GainCtrlB_GainB;
    UINT GainCtrlB_Switch;
    UINT GainCtrlB_OffsetA;
    UINT GainCtrlB_OffsetB;
    UINT GainCtrlEnable;
} IndepGainCtrl_Param;// *PIndepGainCtrl_Param;


typedef struct
{
    UINT8  CG_EN;
    UINT8  GainCtrl_En;
    UINT8  GAMMA_EN;
    UINT8  GEO_EN;
    UINT8  VIG_EN;
    UINT8  CfaPat;
    SIE_PRE_PARAM SiePreParam;
    unsigned long *GammaRLUT_T128;
    unsigned long *GammaGLUT_T128;
    unsigned long *GammaBLUT_T128;
    UINT cgainoffset[8];
    IndepGainCtrl_Param gc_Param;
    STR_PRE_VIG_INFOR pre_vig_setting;
    STR_PRE_VIGLINE_INFOR pre_vigline_setting;
    STR_PRE_GEO_INFOR pre_geo_setting;
} Indep_PRED2D_Pram;// *PIndep_PRED2D_Pram;

//=======================================================================================================================================

ER Indep_SliceMode_EventHandle(UINT32 uiJpgStatus, UINT32 uiImeStatus);
unsigned int get_regbits(unsigned int reg, int bp, int bn);
void ParseIPEReg(UINT ipeAddr, UINT *bayWidth, UINT *bayHeight, UINT *ipeCFA, UINT *packBitDepth);
void ParseChrReg(UINT chrAddr);
void ParseChrSmReg(UINT chrAddr);
void ParseGammaReg(UINT chrAddr);
void ParseGainOffsetReg(UINT chrAddr, UINT *cgoff);
void ParseGainCtrlReg(UINT chrAddr, IndepGainCtrl_Param *gctrlParam);
void ParseOutSizeReg(UINT Addr, UINT *outW, UINT *outH, UINT *jpgQ);
void ParseLSCReg(UINT Addr, STR_PRE_VIG_INFOR *lscReg, UINT *lsc_en);
void ParseGeoReg(UINT Addr, STR_PRE_GEO_INFOR *geoH, STR_PRE_GEO_INFOR *geoV, UINT *geo_en);
void ParseGeoCORRReg(UINT Addr, UINT *Hgeo_en, UINT *HCORR_en, UINT *HNorm, UINT *Vgeo_en, UINT *VCORR_en, UINT *VNorm);
void ParseAPPReg(UINT Addr, UINT *nrRAW);
void ParseChrCurYReg(UINT chrAddr);

void GetPREGammaValue(UINT RegAddr, unsigned long *GammaRLUT_T128, unsigned long *GammaGLUT_T128, unsigned long *GammaBLUT_T128);    



void SaveReg2File(UINT32 RegAddr, UINT32 RegLength);
void RegMemCpy(UINT32 InAddr, UINT32 OutAddr, UINT32 Num);


void Indep_ChromaCurve(CHROMA_CURVE_INFO *ChromaCurveInfo);
void Indep_pre_Dram2Dram(STR_PRE_REG SiePreParam, BOOL isToOpenPRE);
void Indep_GetIMEJPGAddr(UINT StartAddr,IME_FBADDR_PRAM* TmpAddr,UINT IME_OutHeight,UINT IME_OutWidth,UB OutFormat,UB OPPBnum);
void Indep_ime_to_ipe_Stripe_trans(STR_STRIPE_INFOR *Hstripe,STR_STRIPE_INFOR *Vstripe);
void Indep_jpg_SliceCfg(UINT JpgYAddr,UINT JpgCbAddr,UINT JpgCrAddr,UINT ImeHsize,UINT ImeVsize,UINT JpgOutputAddr,UINT JpgBufSize, UINT8 JpgQuality);
void Indep_ipe_SliceCfg(INDEP_IPE_SLICE_CFG_INFO *ipe_slice_cfg);
void Indep_ime_SliceCfg(IME_FBADDR_PRAM *pIME_Para1,IME_FBADDR_PRAM *pIME_Para2, JobTypeEnum Job);
ER Indep_SliceMode_Process(INDEP_SLICE_MODE_INFO *SliceModeInfo);

void BufferAsigner(BUFFER_ASIGNER_INFO AsignerInfo);

//===================================================================================================
// globel variable
WORKIN_BUFFER_INFO  WorkinBuffers;

unsigned int IndepJpegSize = 0;
unsigned int IndepJpegAddr = 0;

SIM_SLICEINFO_PRAM SliceModeParam;
INDEP_FDE_CHR_FILT_PRAM IndepChrParam;

unsigned int IndepChrFilterEnable, IndepChrSmCbEnable, IndepChrSmCrEnable, IndepChrSmYEnable;
unsigned int PREGammaEnable = 0;
unsigned int PREGeoEnable = 0;
unsigned int IPENRRawEnable = 0;
UINT8 ipePreGammaEnable = 0;

__align(4)UW  IndepGammaRLUT_T129[130]={
     0,    8,   16,  24,   32,   40,   48,  56,   64,   72,   80,  88,   96,  104,  112,  120,
 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224,  232,  240,  248,
 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 344, 352,  360,  368,  376,
 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480,  488,  496,  504,
 512, 520, 528, 536, 544, 552, 560, 568, 576, 584, 592, 600, 608,  616,  624,  632,
 640, 648, 656, 664, 672, 680, 688, 696, 704, 712, 720, 728, 736,  744,  752,  760,
 768, 776, 784, 792, 800, 808, 816, 824, 832, 840, 848, 856, 864,  872,  880,  888,
 896, 904, 912, 920, 928, 936, 944, 952, 960, 968, 976, 984, 992,1000, 1008,1016,
1023,1023
};
__align(4)UW  IndepGammaGLUT_T129[130]={
     0,    8,   16,  24,   32,   40,   48,  56,   64,   72,   80,  88,   96,  104,  112,  120,
 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224,  232,  240,  248,
 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 344, 352,  360,  368,  376,
 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480,  488,  496,  504,
 512, 520, 528, 536, 544, 552, 560, 568, 576, 584, 592, 600, 608,  616,  624,  632,
 640, 648, 656, 664, 672, 680, 688, 696, 704, 712, 720, 728, 736,  744,  752,  760,
 768, 776, 784, 792, 800, 808, 816, 824, 832, 840, 848, 856, 864,  872,  880,  888,
 896, 904, 912, 920, 928, 936, 944, 952, 960, 968, 976, 984, 992,1000, 1008,1016,
1023,1023
};
__align(4)UW  IndepGammaBLUT_T129[130]={
     0,    8,   16,  24,   32,   40,   48,  56,   64,   72,   80,  88,   96,  104,  112,  120,
 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224,  232,  240,  248,
 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 344, 352,  360,  368,  376,
 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480,  488,  496,  504,
 512, 520, 528, 536, 544, 552, 560, 568, 576, 584, 592, 600, 608,  616,  624,  632,
 640, 648, 656, 664, 672, 680, 688, 696, 704, 712, 720, 728, 736,  744,  752,  760,
 768, 776, 784, 792, 800, 808, 816, 824, 832, 840, 848, 856, 864,  872,  880,  888,
 896, 904, 912, 920, 928, 936, 944, 952, 960, 968, 976, 984, 992,1000, 1008,1016,
1023,1023
};

__align(4)UW  IndepChrSmYLUT[130]={
     0,    8,   16,  24,   32,   40,   48,  56,   64,   72,   80,  88,   96,  104,  112,  120,
 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224,  232,  240,  248,
 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 344, 352,  360,  368,  376,
 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480,  488,  496,  504,
 512, 520, 528, 536, 544, 552, 560, 568, 576, 584, 592, 600, 608,  616,  624,  632,
 640, 648, 656, 664, 672, 680, 688, 696, 704, 712, 720, 728, 736,  744,  752,  760,
 768, 776, 784, 792, 800, 808, 816, 824, 832, 840, 848, 856, 864,  872,  880,  888,
 896, 904, 912, 920, 928, 936, 944, 952, 960, 968, 976, 984, 992,1000, 1008,1016,
1023,1023
};

__align(4)UW  IndepChrSmCbLUT[130]={
     0,    8,   16,  24,   32,   40,   48,  56,   64,   72,   80,  88,   96,  104,  112,  120,
 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224,  232,  240,  248,
 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 344, 352,  360,  368,  376,
 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480,  488,  496,  504,
 512, 520, 528, 536, 544, 552, 560, 568, 576, 584, 592, 600, 608,  616,  624,  632,
 640, 648, 656, 664, 672, 680, 688, 696, 704, 712, 720, 728, 736,  744,  752,  760,
 768, 776, 784, 792, 800, 808, 816, 824, 832, 840, 848, 856, 864,  872,  880,  888,
 896, 904, 912, 920, 928, 936, 944, 952, 960, 968, 976, 984, 992,1000, 1008,1016,
1023,1023
};

__align(4)UW  IndepChrSmCrLUT[130]={
     0,    8,   16,  24,   32,   40,   48,  56,   64,   72,   80,  88,   96,  104,  112,  120,
 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224,  232,  240,  248,
 256, 264, 272, 280, 288, 296, 304, 312, 320, 328, 336, 344, 352,  360,  368,  376,
 384, 392, 400, 408, 416, 424, 432, 440, 448, 456, 464, 472, 480,  488,  496,  504,
 512, 520, 528, 536, 544, 552, 560, 568, 576, 584, 592, 600, 608,  616,  624,  632,
 640, 648, 656, 664, 672, 680, 688, 696, 704, 712, 720, 728, 736,  744,  752,  760,
 768, 776, 784, 792, 800, 808, 816, 824, 832, 840, 848, 856, 864,  872,  880,  888,
 896, 904, 912, 920, 928, 936, 944, 952, 960, 968, 976, 984, 992,1000, 1008,1016,
1023,1023
};

__align(4) unsigned char shadingLineTable[20] = {
0,0,0,0,
0,0,0,0,
0,0,0,0,
0,0,0,0,
0,0,0,0,
};

STR_PRE_VIG_INFOR vig_infor;
STR_PRE_GEO_INFOR GeoH_infor,GeoV_infor;
IndepGainCtrl_Param GainCtrlParam;

UINT GeoH_en, GeoV_en, GeoH_Corr, GeoV_Corr, GeoH_Norm, GeoV_Norm;
UB vigR[17], vigG[17], vigB[17];
UH GEOH_Lut[65],GEOV_Lut[65];



//===================================================================================================
//APIs

ER Indep_SliceMode_EventHandle(UINT32 uiJpgStatus, UINT32 uiImeStatus)
{
    if (uiJpgStatus & _JPEG_Buf_End)
    {
        while((uiImeStatus & Ime_INT_FRM) == 0)
        {
            //TrgImeSlice();
            ime_triggerSlice();
            uiImeStatus = ime_waitDone();
        }
        return E_BOVR;
    }
    return E_OK;
}

unsigned int get_regbits(unsigned int reg, int bp, int bn)
{
    return((reg >> bp) & ((1 << bn) - 1));
}

void ParseIPEReg(UINT ipeAddr, UINT *bayWidth, UINT *bayHeight, UINT *ipeCFA, UINT *packBitDepth)
{
    UINT memTmp;
    int hn, hl, hm, holap, vn, vl, vm, volap;

    //0x04
    memTmp=*(UINT *)(ipeAddr+0x04);
    *ipeCFA=get_regbits(memTmp, 0, 2);
    *packBitDepth=get_regbits(memTmp, 6, 2);

    //0x0c
    memTmp=*(UINT *)(ipeAddr+0x0c);
    hn=get_regbits(memTmp, 0, 6);
    hl=get_regbits(memTmp, 6, 6);
    hm=get_regbits(memTmp, 12, 4);
    holap=get_regbits(memTmp, 16, 1);

    //0x10
    memTmp=*(UINT *)(ipeAddr+0x10);
    vn=get_regbits(memTmp, 0, 12);
    vl=get_regbits(memTmp, 12, 12);
    vm=get_regbits(memTmp, 24, 6);
    volap=get_regbits(memTmp, 30, 1);

    *bayWidth=hm*16*(hn-2)+hl*16;
    *bayHeight=vl+1+(vn+1-12)*vm;
}


void ParseChrReg(UINT chrAddr)
{
    UINT memTmp;

    chrAddr += 32;

    // 0
    memTmp=*(UINT *)(chrAddr+560);
    IndepChrFilterEnable=get_regbits(memTmp, 0, 1);
    IndepChrParam.src_skp=get_regbits(memTmp, 16, 1);

    // 1
    memTmp=*(UINT *)(chrAddr+564);
    IndepChrParam.src_shf=get_regbits(memTmp, 0, 1);
    IndepChrParam.binning=get_regbits(memTmp, 16, 1);

    // 2
    memTmp=*(UINT *)(chrAddr+568);
    IndepChrParam.gau_length=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgdet_length=get_regbits(memTmp, 16, 16);

    // 3
    memTmp=*(UINT *)(chrAddr+572);
    IndepChrParam.edgenh_length=get_regbits(memTmp, 0, 16);
    IndepChrParam.xorg0_length=get_regbits(memTmp, 16, 16);

    // 4
    memTmp=*(UINT *)(chrAddr+576);
    IndepChrParam.xorg1_length=get_regbits(memTmp, 0, 16);
    IndepChrParam.xmod_length=get_regbits(memTmp, 16, 16);

    // 5
    memTmp=*(UINT *)(chrAddr+580);
    IndepChrParam.avg_norm=get_regbits(memTmp, 0, 16);
    IndepChrParam.out_mux=get_regbits(memTmp, 16, 16);

    // 6
    memTmp=*(UINT *)(chrAddr+584);
    IndepChrParam.edgdet_shift=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgdet_mux=get_regbits(memTmp, 16, 16);

    // 7
    memTmp=*(UINT *)(chrAddr+588);
    IndepChrParam.lpf_weighting=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgdet_mapa=get_regbits(memTmp, 16, 16);

    //8
    memTmp=*(UINT *)(chrAddr+592);
    IndepChrParam.edgdet_mapb=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgdet_mapc=get_regbits(memTmp, 16, 16);

    //9
    memTmp=*(UINT *)(chrAddr+596);
    IndepChrParam.edgdet_mapn=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgdet_shift=get_regbits(memTmp, 16, 16);

    // 10
    memTmp=*(UINT *)(chrAddr+600);
    IndepChrParam.edgdet_threshold=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgenh_mapa=get_regbits(memTmp, 16, 16);

    // 11
    memTmp=*(UINT *)(chrAddr+604);
    IndepChrParam.edgenh_mapb=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgenh_mapc=get_regbits(memTmp, 16, 16);

    // 12
    memTmp=*(UINT *)(chrAddr+608);
    IndepChrParam.edgenh_mapn=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgenh_shift=get_regbits(memTmp, 16, 16);

    // 13
    memTmp=*(UINT *)(chrAddr+612);
    IndepChrParam.edgenh_mux=get_regbits(memTmp, 0, 16);
    IndepChrParam.edgenh_mul=get_regbits(memTmp, 16, 16);

    // 14
    memTmp=*(UINT *)(chrAddr+616);
    IndepChrParam.repeat=get_regbits(memTmp, 0, 16);

    // 15
    memTmp=*(UINT *)(chrAddr+620);
    IndepChrParam.avg_mask=get_regbits(memTmp, 0, 17);
}


void ParseChrSmReg(UINT chrAddr)
{
    UINT memTmp;
    int i,j;

    chrAddr += 32;

    j=0;
    for(i=0; i<127; i+=2)
    {
        //Cb
        memTmp=*(UINT *)(chrAddr+664+j);
        IndepChrSmCbLUT[i]=get_regbits(memTmp, 0, 16);
        IndepChrSmCbLUT[i+1]=get_regbits(memTmp, 16, 16);
        //Cr
        memTmp=*(UINT *)(chrAddr+920+j);
        IndepChrSmCrLUT[i]=get_regbits(memTmp, 0, 16);
        IndepChrSmCrLUT[i+1]=get_regbits(memTmp, 16, 16);
        j+=4;
    }
    memTmp=*(UINT *)(chrAddr+1176);
    IndepChrSmCbLUT[i]=get_regbits(memTmp, 0, 16);
    memTmp=*(UINT *)(chrAddr+1180);
    IndepChrSmCrLUT[i]=get_regbits(memTmp, 0, 16);

    memTmp=*(UINT *)(chrAddr+1184);
    IndepChrSmCbEnable=get_regbits(memTmp, 0, 16);
    IndepChrSmCrEnable=get_regbits(memTmp, 16, 16);
}

void ParseGammaReg(UINT chrAddr)
{
    UINT memTmp;
    int i,j;

    chrAddr += 32;

    j=0;
    for(i=0; i<127; i+=2)
    {
        //R
        memTmp=*(UINT *)(chrAddr+1224+j);
        IndepGammaRLUT_T129[i]=get_regbits(memTmp, 0, 16);
        IndepGammaRLUT_T129[i+1]=get_regbits(memTmp, 16, 16);
        //G
        memTmp=*(UINT *)(chrAddr+1480+j);
        IndepGammaGLUT_T129[i]=get_regbits(memTmp, 0, 16);
        IndepGammaGLUT_T129[i+1]=get_regbits(memTmp, 16, 16);
        //B
        memTmp=*(UINT *)(chrAddr+1736+j);
        IndepGammaBLUT_T129[i]=get_regbits(memTmp, 0, 16);
        IndepGammaBLUT_T129[i+1]=get_regbits(memTmp, 16, 16);
        j+=4;
    }
    memTmp=*(UINT *)(chrAddr+1992);
    IndepGammaRLUT_T129[i]=get_regbits(memTmp, 0, 16);
    memTmp=*(UINT *)(chrAddr+1996);
    IndepGammaGLUT_T129[i]=get_regbits(memTmp, 0, 16);
    memTmp=*(UINT *)(chrAddr+2000);
    IndepGammaBLUT_T129[i]=get_regbits(memTmp, 0, 16);
    memTmp=*(UINT *)(chrAddr+2004);
    PREGammaEnable=get_regbits(memTmp, 0, 1);
}

void ParseGainOffsetReg(UINT chrAddr, UINT *cgoff)
{
    UINT memTmp;

    chrAddr += 32;

    // 0
    memTmp=*(UINT *)(chrAddr+2044);
    cgoff[0]=get_regbits(memTmp, 0, 16);
    cgoff[1]=get_regbits(memTmp, 16, 16);
    // 1
    memTmp=*(UINT *)(chrAddr+2048);
    cgoff[2]=get_regbits(memTmp, 0, 16);
    cgoff[3]=get_regbits(memTmp, 16, 16);
    // 2
    memTmp=*(UINT *)(chrAddr+2052);
    cgoff[4]=get_regbits(memTmp, 0, 16);
    cgoff[5]=get_regbits(memTmp, 16, 16);
    // 3
    memTmp=*(UINT *)(chrAddr+2056);
    cgoff[6]=get_regbits(memTmp, 0, 16);
    cgoff[7]=get_regbits(memTmp, 16, 16);
}

void ParseGainCtrlReg(UINT chrAddr, IndepGainCtrl_Param *gctrlParam)
{
    UINT memTmp;

    chrAddr += 32;

    // 0
    memTmp=*(UINT *)(chrAddr+2100);
    gctrlParam->GainCtrlA_MapA=get_regbits(memTmp, 0, 8);
    gctrlParam->GainCtrlA_MapB=get_regbits(memTmp, 8, 8);
    gctrlParam->GainCtrlB_MapA=get_regbits(memTmp, 16, 8);
    gctrlParam->GainCtrlB_MapB=get_regbits(memTmp, 24, 8);
    // 1
    memTmp=*(UINT *)(chrAddr+2104);
    gctrlParam->GainCtrlA_GainA=get_regbits(memTmp, 0, 10);
    gctrlParam->GainCtrlA_GainB=get_regbits(memTmp, 16, 10);
    gctrlParam->GainCtrlA_Switch=get_regbits(memTmp, 28, 1);
    // 2
    memTmp=*(UINT *)(chrAddr+2108);
    gctrlParam->GainCtrlA_OffsetA=get_regbits(memTmp, 0, 12);
    gctrlParam->GainCtrlA_OffsetB=get_regbits(memTmp, 16, 12);
    // 3
    memTmp=*(UINT *)(chrAddr+2112);
    gctrlParam->GainCtrlB_GainA=get_regbits(memTmp, 0, 10);
    gctrlParam->GainCtrlB_GainB=get_regbits(memTmp, 16, 10);
    gctrlParam->GainCtrlB_Switch=get_regbits(memTmp, 28, 1);
    // 4
    memTmp=*(UINT *)(chrAddr+2116);
    gctrlParam->GainCtrlB_OffsetA=get_regbits(memTmp, 0, 12);
    gctrlParam->GainCtrlB_OffsetB=get_regbits(memTmp, 16, 12);
    gctrlParam->GainCtrlEnable=get_regbits(memTmp, 28, 1);
}

void ParseOutSizeReg(UINT Addr, UINT *outW, UINT *outH, UINT *jpgQ)
{
    UINT memTmp;

    Addr += 32;

    // 0
    memTmp=*(UINT *)(Addr+2160);
    *outW=get_regbits(memTmp, 0, 13);
    *outH=get_regbits(memTmp, 13, 12);
    *jpgQ=get_regbits(memTmp, 25, 7);
}

void ParseLSCReg(UINT Addr, STR_PRE_VIG_INFOR *lscReg, UINT *lsc_en)
{
    UINT memTmp;

    Addr += 32;
    
    // 0
    memTmp=*(UINT *)(Addr+2204);
    vigR[0]=get_regbits(memTmp, 0, 8);
    vigR[1]=get_regbits(memTmp, 8, 8);
    vigR[2]=get_regbits(memTmp, 16, 8);
    vigR[3]=get_regbits(memTmp, 24, 8);
    // 1
    memTmp=*(UINT *)(Addr+2208);
    vigR[4]=get_regbits(memTmp, 0, 8);
    vigR[5]=get_regbits(memTmp, 8, 8);
    vigR[6]=get_regbits(memTmp, 16, 8);
    vigR[7]=get_regbits(memTmp, 24, 8);
    // 2
    memTmp=*(UINT *)(Addr+2212);
    vigR[8]=get_regbits(memTmp, 0, 8);
    vigR[9]=get_regbits(memTmp, 8, 8);
    vigR[10]=get_regbits(memTmp, 16, 8);
    vigR[11]=get_regbits(memTmp, 24, 8);
    // 3
    memTmp=*(UINT *)(Addr+2216);
    vigR[12]=get_regbits(memTmp, 0, 8);
    vigR[13]=get_regbits(memTmp, 8, 8);
    vigR[14]=get_regbits(memTmp, 16, 8);
    vigR[15]=get_regbits(memTmp, 24, 8);
    // 4
    memTmp=*(UINT *)(Addr+2220);
    vigR[16]=get_regbits(memTmp, 0, 8);
    // 5
    memTmp=*(UINT *)(Addr+2224);
    lscReg->VIG_XR=get_regbits(memTmp, 0, 14);
    lscReg->VIG_YR=get_regbits(memTmp, 16, 14);
    // 6
    memTmp=*(UINT *)(Addr+2228);
    lscReg->VIG_XGR=get_regbits(memTmp, 0, 14);
    lscReg->VIG_YGR=get_regbits(memTmp, 16, 14);
    // 7
    memTmp=*(UINT *)(Addr+2232);
    lscReg->VIG_XGB=get_regbits(memTmp, 0, 14);
    lscReg->VIG_YGB=get_regbits(memTmp, 16, 14);
    // 8
    memTmp=*(UINT *)(Addr+2236);
    lscReg->VIG_XB=get_regbits(memTmp, 0, 14);
    lscReg->VIG_YB=get_regbits(memTmp, 16, 14);
    // 9
    memTmp=*(UINT *)(Addr+2240);
    lscReg->VIG_YDIST=get_regbits(memTmp, 0, 4);
    lscReg->VIG_MUL=get_regbits(memTmp, 4, 8);
    lscReg->VIG_T=get_regbits(memTmp, 12, 8);
    lscReg->VIG_XDIV=get_regbits(memTmp, 20, 6);
    lscReg->VIG_YDIV=get_regbits(memTmp, 26, 6);
    // 10
    memTmp=*(UINT *)(Addr+2244);
    vigG[0]=get_regbits(memTmp, 0, 8);
    vigG[1]=get_regbits(memTmp, 8, 8);
    vigG[2]=get_regbits(memTmp, 16, 8);
    vigG[3]=get_regbits(memTmp, 24, 8);
    // 11
    memTmp=*(UINT *)(Addr+2248);
    vigG[4]=get_regbits(memTmp, 0, 8);
    vigG[5]=get_regbits(memTmp, 8, 8);
    vigG[6]=get_regbits(memTmp, 16, 8);
    vigG[7]=get_regbits(memTmp, 24, 8);
    // 12
    memTmp=*(UINT *)(Addr+2252);
    vigG[8]=get_regbits(memTmp, 0, 8);
    vigG[9]=get_regbits(memTmp, 8, 8);
    vigG[10]=get_regbits(memTmp, 16, 8);
    vigG[11]=get_regbits(memTmp, 24, 8);
    // 13
    memTmp=*(UINT *)(Addr+2256);
    vigG[12]=get_regbits(memTmp, 0, 8);
    vigG[13]=get_regbits(memTmp, 8, 8);
    vigG[14]=get_regbits(memTmp, 16, 8);
    vigG[15]=get_regbits(memTmp, 24, 8);
    // 14
    memTmp=*(UINT *)(Addr+2260);
    vigG[16]=get_regbits(memTmp, 0, 8);
    // 15
    memTmp=*(UINT *)(Addr+2264);
    vigB[0]=get_regbits(memTmp, 0, 8);
    vigB[1]=get_regbits(memTmp, 8, 8);
    vigB[2]=get_regbits(memTmp, 16, 8);
    vigB[3]=get_regbits(memTmp, 24, 8);
    // 16
    memTmp=*(UINT *)(Addr+2268);
    vigB[4]=get_regbits(memTmp, 0, 8);
    vigB[5]=get_regbits(memTmp, 8, 8);
    vigB[6]=get_regbits(memTmp, 16, 8);
    vigB[7]=get_regbits(memTmp, 24, 8);
    // 17
    memTmp=*(UINT *)(Addr+2272);
    vigB[8]=get_regbits(memTmp, 0, 8);
    vigB[9]=get_regbits(memTmp, 8, 8);
    vigB[10]=get_regbits(memTmp, 16, 8);
    vigB[11]=get_regbits(memTmp, 24, 8);
    // 18
    memTmp=*(UINT *)(Addr+2276);
    vigB[12]=get_regbits(memTmp, 0, 8);
    vigB[13]=get_regbits(memTmp, 8, 8);
    vigB[14]=get_regbits(memTmp, 16, 8);
    vigB[15]=get_regbits(memTmp, 24, 8);
    // 19
    memTmp=*(UINT *)(Addr+2280);
    vigB[16]=get_regbits(memTmp, 0, 8);
    // 20
    memTmp=*(UINT *)(Addr+2284);
    *lsc_en=get_regbits(memTmp, 0, 1);
    lscReg->VIG_R_LUT=vigR;
    lscReg->VIG_G_LUT=vigG;
    lscReg->VIG_B_LUT=vigB;
}


void ParseGeoReg(UINT Addr, STR_PRE_GEO_INFOR *geoH, STR_PRE_GEO_INFOR *geoV, UINT *geo_en)
{
    UINT memTmp;
    int i,j;

    Addr += 32;

    //Horizontal
    // 0
    memTmp=*(UINT *)(Addr+2328);
    geoH->GEO_CENTX=get_regbits(memTmp, 0, 13);
    geoH->GEO_CENTY=get_regbits(memTmp, 16, 13);
    // 1
    memTmp=*(UINT *)(Addr+2332);
    geoH->GEO_CENTXS=get_regbits(memTmp, 0, 26);
    // 2
    memTmp=*(UINT *)(Addr+2336);
    geoH->GEO_CENTYS=get_regbits(memTmp, 0, 26);
    // 3
    memTmp=*(UINT *)(Addr+2340);
    geoH->GEO_NORMFACT=get_regbits(memTmp, 0, 8);
    geoH->GEO_NORMBIT=get_regbits(memTmp, 8, 5);
    // 4
    memTmp=*(UINT *)(Addr+2344);
    geoH->GEO_RLUTGAIN=get_regbits(memTmp, 0, 12);
    geoH->GEO_GLUTGAIN=get_regbits(memTmp, 16, 12);
    // 5
    memTmp=*(UINT *)(Addr+2348);
    geoH->GEO_BLUTGAIN=get_regbits(memTmp, 0, 12);
    // 6
    memTmp=*(UINT *)(Addr+2352);
    geoH->GEO_XDIST=get_regbits(memTmp, 0, 8);
    geoH->GEO_YDIST=get_regbits(memTmp, 8, 8);
    geoH->GEO_DISTSEL=get_regbits(memTmp, 16, 1);
    // 7
    j=0;
    for(i=0; i<63; i+=2)
    {
        memTmp=*(UINT *)(Addr+2356+j);
        GEOH_Lut[i]=get_regbits(memTmp, 0, 16);
        GEOH_Lut[i+1]=get_regbits(memTmp, 16, 16);
        j+=4;
    }
    memTmp=*(UINT *)(Addr+2484);
    GEOH_Lut[64]=get_regbits(memTmp, 0, 16);
    *geo_en=get_regbits(memTmp, 28, 1);
    geoH->GEO_Lut=GEOH_Lut;

    //Vertical
    // 0
    memTmp=*(UINT *)(Addr+2488);
    geoV->GEO_CENTX=get_regbits(memTmp, 0, 13);
    geoV->GEO_CENTY=get_regbits(memTmp, 16, 13);
    // 1
    memTmp=*(UINT *)(Addr+2492);
    geoV->GEO_CENTXS=get_regbits(memTmp, 0, 26);
    // 2
    memTmp=*(UINT *)(Addr+2496);
    geoV->GEO_CENTYS=get_regbits(memTmp, 0, 26);
    // 3
    memTmp=*(UINT *)(Addr+2500);
    geoV->GEO_NORMFACT=get_regbits(memTmp, 0, 8);
    geoV->GEO_NORMBIT=get_regbits(memTmp, 8, 5);
    // 4
    memTmp=*(UINT *)(Addr+2504);
    geoV->GEO_RLUTGAIN=get_regbits(memTmp, 0, 12);
    geoV->GEO_GLUTGAIN=get_regbits(memTmp, 16, 12);
    // 5
    memTmp=*(UINT *)(Addr+2508);
    geoV->GEO_BLUTGAIN=get_regbits(memTmp, 0, 12);
    // 6
    memTmp=*(UINT *)(Addr+2512);
    geoV->GEO_XDIST=get_regbits(memTmp, 0, 8);
    geoV->GEO_YDIST=get_regbits(memTmp, 8, 8);
    geoV->GEO_DISTSEL=get_regbits(memTmp, 16, 1);
    // 7
    j=0;
    for(i=0; i<63; i+=2)
    {
        memTmp=*(UINT *)(Addr+2516+j);
        GEOV_Lut[i]=get_regbits(memTmp, 0, 16);
        GEOV_Lut[i+1]=get_regbits(memTmp, 16, 16);
        j+=4;
    }
    memTmp=*(UINT *)(Addr+2644);
    GEOV_Lut[64]=get_regbits(memTmp, 0, 16);
    geoV->GEO_Lut=GEOV_Lut;
}

void ParseGeoCORRReg(UINT Addr, UINT *Hgeo_en, UINT *HCORR_en, UINT *HNorm, UINT *Vgeo_en, UINT *VCORR_en, UINT *VNorm)
{
    UINT memTmp;

    Addr += 32;

    // 0
    memTmp=*(UINT *)(Addr+2648);
    *Hgeo_en = get_regbits(memTmp, 0, 1);
    *HCORR_en = get_regbits(memTmp, 1, 1);
    *HNorm = get_regbits(memTmp, 2, 2);

    *Vgeo_en = get_regbits(memTmp, 4, 1);
    *VCORR_en = get_regbits(memTmp, 5, 1);
    *VNorm = get_regbits(memTmp, 6, 2);
}

void ParseAPPReg(UINT Addr, UINT *nrRAW)
{
    UINT memTmp;

    Addr += 32;

    // 0
    memTmp=*(UINT *)(Addr+2688);
    *nrRAW=get_regbits(memTmp, 0, 1);
}

//#NT#2008/12/08#Chris Yang -begin
//#NT#Add for Chroma curve on Y
void ParseChrCurYReg(UINT chrAddr)
{
    UINT memTmp;
    int i,j;

    chrAddr += 32;

    j=0;
    for(i=0; i<127; i+=2)
    {
        //Y
        memTmp=*(UINT *)(chrAddr+2768+j);
        IndepChrSmYLUT[i]=get_regbits(memTmp, 0, 16);
        IndepChrSmYLUT[i+1]=get_regbits(memTmp, 16, 16);
        j+=4;
    }
    memTmp=*(UINT *)(chrAddr+3024);
    IndepChrSmYLUT[i]=get_regbits(memTmp, 0, 16);
    IndepChrSmYEnable=get_regbits(memTmp, 16, 16);
}
//#NT#2008/12/08#Chris Yang -end


void GetPREGammaValue(UINT RegAddr, unsigned long *GammaRLUT_T128, unsigned long *GammaGLUT_T128, unsigned long *GammaBLUT_T128)
{
    UINT memTmp;
    int i,j;

    j=0;
    for(i=0; i<127; i+=2)
    {
        //R
        memTmp=*(UINT *)(RegAddr+j);
        GammaRLUT_T128[i]=get_regbits(memTmp, 0, 16);
        GammaRLUT_T128[i+1]=get_regbits(memTmp, 16, 16);
        //G
        memTmp=*(UINT *)(RegAddr+256+j);
        GammaGLUT_T128[i]=get_regbits(memTmp, 0, 16);
        GammaGLUT_T128[i+1]=get_regbits(memTmp, 16, 16);
        //B
        memTmp=*(UINT *)(RegAddr+512+j);
        GammaBLUT_T128[i]=get_regbits(memTmp, 0, 16);
        GammaBLUT_T128[i+1]=get_regbits(memTmp, 16, 16);
        j+=4;
    }
    memTmp=*(UINT *)(RegAddr+768);
    GammaBLUT_T128[i]=get_regbits(memTmp, 0, 10);
    GammaGLUT_T128[i]=get_regbits(memTmp, 10, 10);
    GammaRLUT_T128[i]=get_regbits(memTmp, 20, 10);
}


void Indep_ime_to_ipe_Stripe_trans(STR_STRIPE_INFOR *Hstripe,STR_STRIPE_INFOR *Vstripe)
{
    STR_IME_STRIPE_INFOR    indGetStripe = {0};
    
    //Get IME stripe information
    indGetStripe  = ime_getHstripe();
    Hstripe->n    = indGetStripe.n;
    Hstripe->l    = indGetStripe.l;
    Hstripe->m    = indGetStripe.m;
    Hstripe->olap = IPE_OLAP_ON;
    
    indGetStripe  = ime_getVstripe();
    Vstripe->n    = indGetStripe.n;
    Vstripe->l    = indGetStripe.l;
    Vstripe->m    = indGetStripe.m;
    Vstripe->olap = IPE_OLAP_ON;

    //IME to IPE stripe convert
    Hstripe->n += 1;
    Hstripe->l += 1;
    Vstripe->n += 8;
    
}
//#NT#2009/06/25#Jarkko Chang -end

void Indep_jpg_SliceCfg(UINT JpgYAddr,UINT JpgCbAddr,UINT JpgCrAddr,UINT ImeHsize,UINT ImeVsize,UINT JpgOutputAddr,UINT JpgBufSize, UINT8 JpgQuality)
{
    UINT        JPG_BSize;
    JPGCmpParam    JPGParam;
    char        ER_Code;

    //Address to be setup in 1st time trigger
    JPGParam.RawImageAddr[0] = JpgYAddr;            //Raw Image address Y
    JPGParam.RawImageAddr[1] = JpgCbAddr;           //Raw Image address CB
    JPGParam.RawImageAddr[2] = JpgCrAddr;           //Raw Image address CR
    JPGParam.JPGBSAddr = JpgOutputAddr;             //Compress Bitstream location,
    JPG_BSize = JpgBufSize;                         //assume 3 M buffer size..
    JPGParam.JPGBSBufSize = &JPG_BSize;
    JPGParam.Width = ImeHsize;
    JPGParam.Height =ImeVsize;
    JPGParam.Mode  = JPEG_FORMAT_422 | JPG_Enc_SrcSliceMode;
    JPGParam.bOuputJpgDC = FALSE;
    //#NT#2010/02/24#Meg Lin -begin
    //#NT#add lineoffset
    JPGParam.LineOffsetY = JPGParam.Width;
    JPGParam.LineOffsetCbCr = JPGParam.Width/2;
    //#NT#2010/02/24#Meg Lin -end 
    ER_Code = JpegEncOnePic(&JPGParam, JpgQuality);
}

void Indep_ChromaCurve(CHROMA_CURVE_INFO *ChromaCurveInfo)
{
    //STR_FLDNUM pre_FieldNum;
    //unsigned long PreIntStatus;
    IPE_IQINFO IQInfo;
    STR_PRE_REG PRE_D2D_Param = {0};


    IQInfo.FuncUpdateSel = IPE_FUNC_DISABLE;
    IQInfo.FuncSel = (IPE_PREGAMMA_EN | 0x80000000);
    IQInfo.ParamUpdateSel = 0;
    ipe_changeParam(&IQInfo);
    //ipe_pause();
    //ipe_start();


    //PRE D2D
    // -begin- PRE basic flow
    PRE_D2D_Param.PRE_Operation  = PRE_ENGINE_SET2D2D;
    PRE_D2D_Param.REG_pre_intrp_en |= PRE_INT_FLDEND;
    PRE_D2D_Param.PRE_EnFunction = 0x0;

    // Field number
    PRE_D2D_Param.REG_RemapFieldNum.Fld1 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld2 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld3 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld4 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld5 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld6 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld7 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld8 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.TotalFld = 0;

    // cfa
    PRE_D2D_Param.REG_pre_CfaPat  = ChromaCurveInfo->cfa;
    PRE_D2D_Param.REG_pre_CfaPat2 = ChromaCurveInfo->cfa;

    // input flow info.    
    PRE_D2D_Param.REG_pre_input_Size.Hsize   = ChromaCurveInfo->hSize;
    PRE_D2D_Param.REG_pre_input_Size.Vsize   = ChromaCurveInfo->vSize;
    PRE_D2D_Param.REG_pre_input_Size.lineoft = ChromaCurveInfo->LineOffset;
    PRE_D2D_Param.REG_pre_in_ppb0_addr       = ChromaCurveInfo->sourceAddr;
    PRE_D2D_Param.REG_pre_input_pack_bus     = PRE_INPACK_8BIT; // 8Bits
    PRE_D2D_Param.REG_pre_input_source       = PATH_DRAM_INPUT;

    // output flow info.
    PRE_D2D_Param.REG_pre_hv_ini          = 0;
    PRE_D2D_Param.REG_pre_hv_width        = ChromaCurveInfo->hSize;
    PRE_D2D_Param.REG_pre_vv_ini          = 0;
    PRE_D2D_Param.REG_pre_vv_height       = ChromaCurveInfo->vSize;    
    PRE_D2D_Param.REG_pre_output_size     = ChromaCurveInfo->hSize;
    PRE_D2D_Param.REG_pre_output_lineoft  = ChromaCurveInfo->LineOffset;
    PRE_D2D_Param.REG_pre_out_ppb0_addr   = ChromaCurveInfo->distAddr;
    PRE_D2D_Param.REG_pre_output_pack_bus = PRE_INPACK_8BIT;  // 8-bits
    // -end- PRE basic flow        

    // gamma table  
    PRE_D2D_Param.PRE_EnFunction     |= PRE_GAMMA_EN;    
    PRE_D2D_Param.REG_pre_output_path = PATH_POST_GAMMA;
    PRE_D2D_Param.pre_gamma_r         = ChromaCurveInfo->chrSmLUT;
    PRE_D2D_Param.pre_gamma_g         = ChromaCurveInfo->chrSmLUT;
    PRE_D2D_Param.pre_gamma_b         = ChromaCurveInfo->chrSmLUT;
    Indep_pre_Dram2Dram(PRE_D2D_Param, TRUE);
    
    

    // change back gamma table  
    PRE_D2D_Param.PRE_EnFunction = 0x0;  
    PRE_D2D_Param.REG_pre_output_path = PATH_POST_GAMMA;
    PRE_D2D_Param.pre_gamma_r         = ChromaCurveInfo->rGaLUT;
    PRE_D2D_Param.pre_gamma_g         = ChromaCurveInfo->gGaLUT;
    PRE_D2D_Param.pre_gamma_b         = ChromaCurveInfo->bGaLUT;
    pre_enableFunc(PRE_GAMMA_EN, &PRE_D2D_Param);


    if(ChromaCurveInfo->ipe_GaEn == 1)
        IQInfo.FuncUpdateSel = IPE_FUNC_ENABLE;
    else
        IQInfo.FuncUpdateSel = IPE_FUNC_DISABLE;
    IQInfo.FuncSel = (IPE_PREGAMMA_EN | 0x00000000);
    IQInfo.ParamUpdateSel = 0;
    ipe_changeParam(&IQInfo);
    //ipe_pause();
    //ipe_start();
    
}

void Indep_ipe_SliceCfg(INDEP_IPE_SLICE_CFG_INFO *ipe_slice_cfg)
{    
    debug_msg("IPE CFA pattern : %d\r\n", ipe_slice_cfg->cfapat);
    debug_msg("IPE Input Bit Depth : %d\r\n", ipe_slice_cfg->BitDepth);
    
    switch(ipe_slice_cfg->BitDepth)
    {
        case _PreBitDepth_10 :
            IPE_Para.BitDepth = IPE_PACK_10bit;
            IPE_Para.InLineOfs = ipe_slice_cfg->Pre_CropHsize * _PreBitDepth_10 / 8;
            break;

        case _PreBitDepth_12 :
            IPE_Para.BitDepth = IPE_PACK_12bit;
            IPE_Para.InLineOfs = ipe_slice_cfg->Pre_CropHsize * _PreBitDepth_12 / 8;
            break;

        case _PreBitDepth_16 :
            IPE_Para.BitDepth = IPE_PACK_16bit;
            IPE_Para.InLineOfs = ipe_slice_cfg->Pre_CropHsize * _PreBitDepth_16 / 8;
            break;

        case _PreBitDepth_8 :
            IPE_Para.BitDepth = IPE_PACK_08bit;
            IPE_Para.InLineOfs = ipe_slice_cfg->Pre_CropHsize;
            break;

        default:
            IPE_Para.BitDepth = IPE_PACK_08bit;
            IPE_Para.InLineOfs = ipe_slice_cfg->Pre_CropHsize;
            break;
            
    }

    //IPPMgrReq.IpeParam = &IPE_Para;    

    IPEModeInfo.OutDest               = IPE_DEST_IME;
    IPEModeInfo.IntEn                 = 0;
    IPEModeInfo.SIZEINFO.StripeMode   = IPE_MSTP;
    IPEModeInfo.SIZEINFO.INSIZE.hsize = ipe_slice_cfg->Pre_CropHsize;
    IPEModeInfo.SIZEINFO.INSIZE.vsize = ipe_slice_cfg->Pre_CropVsize;
    IPEModeInfo.SIZEINFO.HSTRIPE      = ipe_slice_cfg->Hstripe;
    IPEModeInfo.SIZEINFO.VSTRIPE      = ipe_slice_cfg->Vstripe;
    IPEModeInfo.SIZEINFO.LofsIn       = IPE_Para.InLineOfs;
    IPEModeInfo.SIZEINFO.LbufNum      = IPE_LINEX9;
    IPEModeInfo.SIZEINFO.CfaPat       = ipe_slice_cfg->cfapat;
    IPEModeInfo.ININFO.Adr0           = ipe_slice_cfg->InAddr0;
    IPEModeInfo.ININFO.PackBit        = IPE_Para.BitDepth;
    IPEModeInfo.ININFO.PPBEn          = DISABLE;
    IPEModeInfo.ININFO.PPBIni         = PPBINI_0;
    IPEModeInfo.ININFO.SyncSel        = SYNC_CROP_END;
    IPEModeInfo.OUTINFO.YCFormat      = ipe_slice_cfg->ipe_omat;
    IPEModeInfo.OUTINFO.SubHSel       = YCC_DROP_RIGHT;

    ipe_setMode(&IPEModeInfo);
    
    //==================================================================
    // user define parameters
    //SenFlowFunc(IPE_CAP_SETTING, &gSenCBInfo);
    RegMemCpy(GetIndepAddr(), 0x9a100000, 0x208);
    //ipePreGammaEnable = get_regbits(*(UINT *)(0x9a100008), 18, 1);
    *((volatile UINT32*)(0x9a100000)) = 0x00000004;
    ipe_reset(ENABLE);
    ipe_reset(DISABLE);

    //==================================================================

    
    
}

void Indep_GetIMEJPGAddr(UINT StartAddr,IME_FBADDR_PRAM* TmpAddr,UINT IME_OutHeight,UINT IME_OutWidth,UB OutFormat,UB OPPBnum)
{
    UINT Slc_YSize,Slc_CSize;

    Slc_YSize=IME_OutWidth*IME_OutHeight;
    switch(OutFormat)
    {
        case 0://444
        Slc_CSize=Slc_YSize;
        break;
        case 1://422
        Slc_CSize=Slc_YSize/2;
        break;
        case 2://422
        Slc_CSize=Slc_YSize/2;
        break;
        case 3://420
        Slc_CSize=Slc_YSize/4;
        break;
        case 4://420
        Slc_CSize=Slc_YSize/4;
        break;
        default://420
        Slc_CSize=Slc_YSize/4;
        break;
    }

    if(OPPBnum==3)
    {
        TmpAddr->y0_addr= StartAddr;
        TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
        TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;

        TmpAddr->y1_addr= TmpAddr->cr0_addr+Slc_CSize;
        TmpAddr->cb1_addr=TmpAddr->y1_addr+Slc_YSize;
        TmpAddr->cr1_addr=TmpAddr->cb1_addr+Slc_CSize;

        TmpAddr->y2_addr= TmpAddr->cr1_addr+Slc_CSize;
        TmpAddr->cb2_addr=TmpAddr->y2_addr+Slc_YSize;
        TmpAddr->cr2_addr=TmpAddr->cb2_addr+Slc_CSize;
    }
    else if(OPPBnum==1)
    {
        TmpAddr->y0_addr= StartAddr;
        TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
        TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;
    }
    TmpAddr->out_format=OutFormat;

}

void Indep_ime_SliceCfg(IME_FBADDR_PRAM *pIME_Para1,IME_FBADDR_PRAM *pIME_Para2, JobTypeEnum Job)
{
    //IPPMgrReq.ImeParam1 = pIME_Para1;
    //IPPMgrReq.ImeParam2 = pIME_Para2;

    IMEParam.op_mode               = _SLICE_1PATH;
    IMEParam.in_src                = IMEALG_IPE2IME;
    IMEParam.in_size.h_size        = pIME_Para1->in_h;
    IMEParam.in_size.v_size        = pIME_Para1->in_v;
    IMEParam.format.in_format      = pIME_Para1->in_format;

    IMEParam.format.out1_format    = pIME_Para1->out_format;
    IMEParam.line_ofs.out1_lineofs = pIME_Para1->out_lineoffset;
    IMEParam.p1_out_size.h_size    = pIME_Para1->out_h;
    IMEParam.p1_out_size.v_size    = pIME_Para1->out_v;
    
    IMEParam.p1_out0_addr.y_addr   = pIME_Para1->y0_addr;
    IMEParam.p1_out0_addr.cb_addr  = pIME_Para1->cb0_addr;
    IMEParam.p1_out0_addr.cr_addr  = pIME_Para1->cr0_addr;
    
    IMEParam.p1_out1_addr.y_addr   = pIME_Para1->y1_addr;
    IMEParam.p1_out1_addr.cb_addr  = pIME_Para1->cb1_addr;
    IMEParam.p1_out1_addr.cr_addr  = pIME_Para1->cr1_addr;
    
    IMEParam.p1_out2_addr.y_addr   = pIME_Para1->y2_addr;
    IMEParam.p1_out2_addr.cb_addr  = pIME_Para1->cb2_addr;
    IMEParam.p1_out2_addr.cr_addr  = pIME_Para1->cr2_addr;


    ime_setMode(&IMEParam);
    ime_setSliceH(GetIPPSizeInfor(_CapIMESliceMode));
}


UINT IndepSliceGetJPGAddr(void)
{
    return IndepJpegAddr;
}

UINT IndepSliceGetJPGSize(void)
{
    return IndepJpegSize;
}


UINT32 GetIndepAddr(void)
{
  UINT8   *pBuffer;
  UINT32 StartableAddr = 0x0;

  get_blk((void *)&pBuffer,  11);
  rel_blk(11, pBuffer);
  StartableAddr = (UINT32)((UINT32*)pBuffer);

  if((StartableAddr % 0x00001000) != 0)
        StartableAddr = StartableAddr + (0x00001000 - (StartableAddr % 0x00001000));

  return StartableAddr;
}

void SaveReg2File(UINT32 RegAddr, UINT32 RegLength)
{
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
    (UB*)RegAddr,//   UB *pBuf,
    RegLength,
    0,//UW Pos,
    1,//BOOL CloseFile,
    FST_FMT_RAW,//UW FileFormat,
    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
}

void RegMemCpy(UINT32 InAddr, UINT32 OutAddr, UINT32 Num)
{
    UINT32 i = 0;    
    memcpy((UINT32 *)(OutAddr+0x08), (UINT32 *)(InAddr+0x08), 4);
    //memcpy((UINT32 *)(OutAddr+0x50), (UINT32 *)(InAddr+0x50), 472);

    for(i = 0x50; i <= 0x228; i+=4)
    {
      *((volatile UINT32*)(OutAddr + i)) = *((volatile UINT32*)(InAddr + i));
      debug_msg("IPE Register(0x%08x) : 0x%08x\r\n", (OutAddr + i), *((volatile UINT32*)(OutAddr + i)));
    }
}


void Indep_pre_Dram2Dram(STR_PRE_REG SiePreParam, BOOL isToOpenPRE)
{
    PRE_OPENOBJ PREOpenObj;
    
    PREOpenObj.FP_PREISR_CB = NULL;
    if(isToOpenPRE == TRUE)    
        pre_open(&PREOpenObj);

    pre_setMode(&SiePreParam);
    
    //clear frame-end flag
    pre_clrFieldEnd();
    //==================================================================================
    // recover IQ parameter setting

    //==================================================================================
    pre_start();
    
    // wait pre frame-end
    pre_waitFieldEnd(FALSE);
    pre_pause();

    if(isToOpenPRE == TRUE)    
        pre_close();
}


ER Indep_SliceMode_Process(INDEP_SLICE_MODE_INFO *SliceModeInfo)
{
    UINT JpgYAddr, JpgCbAddr, JpgCrAddr, CounterMax;
    UINT IPEOutForm;
    UINT SliceAddr;
    UINT Ime_Counter, SliceHeight, ProcHeight;
    //UINT32  ChrFiltIdx, ,P2Addr, IPECFAPat;
    UINT32 uiImeStatus, uiJpgStatus;
    //UINT32 BlackV;
    STR_STRIPE_INFOR ImeHstipe,ImeVstipe;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;

    //#NT#2009/11/02#Linkin Huang -begin
    //#NT# added by Linkin
    CHROMA_CURVE_INFO ChmaCurveInfo;
    IME_FBADDR_PRAM LocaL_IMEJPGAddrP1 = {0};
    INDEP_IPE_SLICE_CFG_INFO IPE_SliceCfg = {0};
    //#NT#2009/11/02#Linkin Huang -end

    IPEOpenObj.FP_IPEISR_CB = NULL;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    ipe_open(&IPEOpenObj);
    ime_open(&IMEOpenObj);

    uiImeStatus = 0;
    uiJpgStatus = 0;
    //reset ime & jpg

    //calculate slice height & slice number
    Ime_Counter = 0;
    SliceHeight = ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode));  // IME slice height : 64-lines
    CounterMax=(SliceModeInfo->ImeOutVsize + SliceHeight - 1) / SliceHeight;

    //initial slice parameters
    SliceAddr = SliceModeInfo->SLC_BUF_ADDR;
    Indep_GetIMEJPGAddr(SliceAddr, &LocaL_IMEJPGAddrP1, SliceHeight, SliceModeInfo->ImeOutHsize, IMEALG_OUTYUV422_COS, 3);
    
    LocaL_IMEJPGAddrP1.in_h           = SliceModeInfo->BayerHsize - 16;
    LocaL_IMEJPGAddrP1.in_v           = SliceModeInfo->BayerVsize;
    LocaL_IMEJPGAddrP1.out_h          = SliceModeInfo->ImeOutHsize;
    LocaL_IMEJPGAddrP1.out_v          = SliceModeInfo->ImeOutVsize;
    LocaL_IMEJPGAddrP1.in_format      = IMEALG_INYUV422;
    LocaL_IMEJPGAddrP1.out_format     = IMEALG_OUTYUV422_COS;
    LocaL_IMEJPGAddrP1.out_lineoffset = SliceModeInfo->ImeOutLineOffset;
    LocaL_IMEJPGAddrP1.in_YCC_pack    = IMEALG_YCBCR_PLANAR;
    LocaL_IMEJPGAddrP1.out_YCC_pack   = IMEALG_YCBCR_PLANAR;

    Indep_ime_SliceCfg(&LocaL_IMEJPGAddrP1, NULL, CAPTURE_JOB);
    IPEOutForm = IPE_YCC422;
    
    
/* // marked by Linkin
    //ChrFiltIdx = GetIPPFuncInfor(CAP_ISO);
    //Local_ChromaPara = (UINT(*)[28])Get_ChromaInfo();
    //SetIPPFuncInfor(CHRFILT_NUM, Local_ChromaPara[ChrFiltIdx][27]);
*/

    Indep_ime_to_ipe_Stripe_trans(&ImeHstipe, &ImeVstipe);

    //#NT#2009/11/06#Linkin Huang -begin
    //#NT# added by Linkin
    IPE_SliceCfg.Pre_CropHsize = SliceModeInfo->BayerHsize;
    IPE_SliceCfg.Pre_CropVsize = SliceModeInfo->BayerVsize;
    IPE_SliceCfg.BitDepth      = SliceModeInfo->PreBitDepth;
    IPE_SliceCfg.Hstripe       = ImeHstipe;
    IPE_SliceCfg.Vstripe       = ImeVstipe;
    IPE_SliceCfg.InAddr0       = SliceModeInfo->BayerAddr;
    IPE_SliceCfg.cfapat        = SliceModeInfo->PreCfa;
    IPE_SliceCfg.ipe_omat      = IPEOutForm;
    //#NT#2009/11/06#Linkin Huang -end
    Indep_ipe_SliceCfg(&IPE_SliceCfg);
    ime_setStart();
    ime_triggerSlice();
    ipe_start();
    
    jpeg_setSliceGoDisable();
    jpg_SliceCfg(LocaL_IMEJPGAddrP1.y0_addr, LocaL_IMEJPGAddrP1.cb0_addr, LocaL_IMEJPGAddrP1.cr0_addr,
                SliceModeInfo->ImeOutHsize, SliceModeInfo->ImeOutVsize, SliceModeInfo->JpgOutAddr, GetJpgBufSize(), SliceModeInfo->JPEGQuality);
    
    for (Ime_Counter = 0; Ime_Counter < CounterMax; Ime_Counter++)
    {        
        /************* step 1 wait ime slice end & get address *************/
        while(1)
        {
            uiImeStatus = ime_waitDone();
            if (Ime_Counter == (CounterMax - 1))
            {
                if (uiImeStatus & Ime_INT_FRM)
                {
                    break;
                }
            }
            else
            {
                if (uiImeStatus & Ime_INT_SLICE_DN1)
                {
                    break;
                }
            }
        }

        //get ime output address
        switch(Ime_Counter % SLICE_BUF_NUM)
        {
            case 0:
                JpgYAddr  = LocaL_IMEJPGAddrP1.y0_addr;
                JpgCbAddr = LocaL_IMEJPGAddrP1.cb0_addr;
                JpgCrAddr = LocaL_IMEJPGAddrP1.cr0_addr;
                break;
            case 1:
                JpgYAddr  = LocaL_IMEJPGAddrP1.y1_addr;
                JpgCbAddr = LocaL_IMEJPGAddrP1.cb1_addr;
                JpgCrAddr = LocaL_IMEJPGAddrP1.cr1_addr;
                break;
            case 2:
                JpgYAddr  = LocaL_IMEJPGAddrP1.y2_addr;
                JpgCbAddr = LocaL_IMEJPGAddrP1.cb2_addr;
                JpgCrAddr = LocaL_IMEJPGAddrP1.cr2_addr;
                break;
            default:
                JpgYAddr = JpgCbAddr = JpgCrAddr = 0;
                break;
        }
        ProcHeight = SliceHeight;
        if ((Ime_Counter * Ime_Counter) > SliceModeInfo->ImeOutVsize)
        {
            ProcHeight = (Ime_Counter * Ime_Counter) - SliceModeInfo->ImeOutVsize;
        }

        /*============== step 2 post processer =================*/
/*         
        ChrFiltIdx = GetIPPFuncInfor(CAP_ISO);
        Local_ChromaPara = (UINT(*)[28])Get_ChromaInfo();
        SetIPPFuncInfor(CHRFILT_NUM, Local_ChromaPara[ChrFiltIdx][27]);

        //#NT#2009/11/12#Linkin Huang -begin
        //#NT# chroma filter is removed within NT96630
        if(GetIPPFuncInfor(CHRFILT_EN))
        {
            ChromaFilter(JpgCbAddr, JpgCrAddr, ippbuf_GetBuf(IPPBUF_CAP_CHROMA_FILTER),
                        (IMEJPGAddrP1.out_h/2), (IMEJPGAddrP1.out_h/2), ProcHeight, ChromaPara[ChrFiltIdx]);
        }
         //#NT#2009/11/12#Linkin Huang -end
*/

        //chroma curve
        if(IndepChrSmYEnable == 1)
        {
            ChmaCurveInfo.hSize      = LocaL_IMEJPGAddrP1.out_h;
            ChmaCurveInfo.vSize      = ProcHeight;
            ChmaCurveInfo.cfa        = SliceModeInfo->PreCfa;
            ChmaCurveInfo.LineOffset = ChmaCurveInfo.hSize;
            ChmaCurveInfo.sourceAddr = JpgYAddr;
            ChmaCurveInfo.distAddr   = JpgYAddr;            
            ChmaCurveInfo.ipe_GaEn   = ipePreGammaEnable;
            ChmaCurveInfo.chrSmLUT   = IndepChrSmCbLUT;
            ChmaCurveInfo.rGaLUT     = IndepGammaRLUT_T129;
            ChmaCurveInfo.gGaLUT     = IndepGammaGLUT_T129;
            ChmaCurveInfo.bGaLUT     = IndepGammaBLUT_T129;
            
            Indep_ChromaCurve(&ChmaCurveInfo);
        }
        
        if(IndepChrSmCbEnable == 1)
        {
            ChmaCurveInfo.hSize      = LocaL_IMEJPGAddrP1.out_h / 2;
            ChmaCurveInfo.vSize      = ProcHeight;
            ChmaCurveInfo.cfa        = SliceModeInfo->PreCfa;
            ChmaCurveInfo.LineOffset = ChmaCurveInfo.hSize;
            ChmaCurveInfo.sourceAddr = JpgCbAddr;
            ChmaCurveInfo.distAddr   = JpgCbAddr;            
            ChmaCurveInfo.ipe_GaEn   = ipePreGammaEnable;
            ChmaCurveInfo.chrSmLUT   = IndepChrSmCbLUT;
            ChmaCurveInfo.rGaLUT     = IndepGammaRLUT_T129;
            ChmaCurveInfo.gGaLUT     = IndepGammaGLUT_T129;
            ChmaCurveInfo.bGaLUT     = IndepGammaBLUT_T129;
            
            Indep_ChromaCurve(&ChmaCurveInfo);            
        }
        
        if(IndepChrSmCrEnable == 1)
        {  
            ChmaCurveInfo.hSize      = LocaL_IMEJPGAddrP1.out_h / 2;
            ChmaCurveInfo.vSize      = ProcHeight;
            ChmaCurveInfo.cfa        = SliceModeInfo->PreCfa;
            ChmaCurveInfo.LineOffset = ChmaCurveInfo.hSize;
            ChmaCurveInfo.sourceAddr = JpgCrAddr;
            ChmaCurveInfo.distAddr   = JpgCrAddr;            
            ChmaCurveInfo.ipe_GaEn   = ipePreGammaEnable;
            ChmaCurveInfo.chrSmLUT   = IndepChrSmCbLUT;
            ChmaCurveInfo.rGaLUT     = IndepGammaRLUT_T129;
            ChmaCurveInfo.gGaLUT     = IndepGammaGLUT_T129;
            ChmaCurveInfo.bGaLUT     = IndepGammaBLUT_T129;
            
            Indep_ChromaCurve(&ChmaCurveInfo);
        }
        

        /************* step 3 encode to jpeg & trigger next slice *************/
        if (Ime_Counter == 0)
        {
            //jpeg slice start
            jpeg_setSliceEncAddr(ProcHeight, JpgYAddr, JpgCbAddr, JpgCrAddr);
            jpeg_setStartEncode();
            ime_triggerSlice();
        }
        else if (Ime_Counter == (CounterMax - 1))
        {
            //jpeg slice end
            while(1)
            {
                uiJpgStatus = jpeg_waitDone();
                if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                {
                    break;
                }
            }
            if (Indep_SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
            {
            
                ipe_pause();
                ipe_close();
                ime_setStop();
                ime_close();                
                jpeg_setEndEncode();
                jpeg_close();
                return E_BOVR;
            }            
            jpeg_setSliceEncAddr(0, JpgYAddr, JpgCbAddr, JpgCrAddr);
        }
        else
        {
            //jpeg slice
            while(1)
            {
                uiJpgStatus = jpeg_waitDone();
                if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End))
                {
                    break;
                }
            }
            if (Indep_SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
            {
                ipe_pause();
                ipe_close();
                ime_setStop();
                ime_close();
                jpeg_setEndEncode();
                jpeg_close();
                return E_BOVR;
            }
            jpeg_setSliceEncAddr(ProcHeight, JpgYAddr, JpgCbAddr, JpgCrAddr);
            ime_triggerSlice();
        }
    }

    ipe_pause();
    ipe_close();
    ime_setStop();
    ime_close();

    //wait jpeg encode finish
    while(1)
    {
        uiJpgStatus = jpeg_waitDone();
        if (uiJpgStatus & (_JPEG_Frame_End | _JPEG_Buf_End))
        {
            break;
        }
    }
    if (Indep_SliceMode_EventHandle(uiJpgStatus, uiImeStatus) == E_BOVR)
    {
        jpeg_setEndEncode();
        jpeg_close();
        return E_BOVR;
    }
    jpeg_setEndEncode();
    jpeg_close();
    SliceModeInfo->JpegSize = jpeg_getComSize() +  JPGHeaderSize;
    IndepJpegSize = SliceModeInfo->JpegSize;
    return E_OK;
}




UINT32 Trigger_IndepSlice630(void)
{
    UINT imgWidth, imgHeight, imgCFA, imgBitDepth;
    UINT bitdeph;
    UINT cgof[8];
    UINT sliceH;
    UINT lsc_Enable;
    UINT imeOutHSize, imeOutVSize, jpgQualityValue;

    UINT32 PREOutAddr = 0x0;
    UINT32 GraphOutAddr1 = 0x0;

    //#NT#2009/10/30#Linkin Huang -begin
    //#NT# added by Linkin
    STR_PRE_REG PRE_D2D_Param = {0};
    //PRE_OutputPathEnum PreOutSource;
    INDEP_SLICE_MODE_INFO indSliceModeInfo = {0};
    //#NT#2009/10/30 Linkin Huang -end
    //sliceH = ImeSliceModeToVSize(2);  // IME slice height : 64-line

    IMEReset();
    IPEReset();
    SIEReset();

    SetIPPSizeInfor(_CapIMESliceMode, SLICE_MODE);
    sliceH = ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode));
    
    ParseIPEReg(GetIndepAddr(), &imgWidth, &imgHeight, &imgCFA, &imgBitDepth);
    switch(imgBitDepth)
    {
        case 0:
            bitdeph = 12;
            break;
        case 1:
        case 2:
            bitdeph = 8;
            break;
        case 3:
            bitdeph = 16;
            break;
        default:
            bitdeph = 8;
            break;
    }
    debug_msg("Image width : %d\r\n", imgWidth);
    debug_msg("Image height : %d\r\n", imgHeight);
    debug_msg("Image CFA : %d\r\n", imgCFA);
    debug_msg("Image Pixel Depth : %d\r\n", imgBitDepth);
    
    SliceModeParam.bay_hsize     = imgWidth;
    SliceModeParam.bay_vsize     = imgHeight;    
    SliceModeParam.bay_addr      = GetIndepAddr()+5000*sliceH*6+0x100000+8192;
    SliceModeParam.bay_packbit   = bitdeph;
    SliceModeParam.bay_startpix  = imgCFA;
    
    SliceModeParam.jpg_addr      = GetIndepAddr()+5000*sliceH*2*3;
    SliceModeParam.jpg_filesize  = 0x800000;
    SliceModeParam.jpg_hsize     = imgWidth-16;
    SliceModeParam.jpg_vsize     = imgHeight;
    SliceModeParam.pingpong_addr = GetIndepAddr();
    SliceModeParam.CF_addr       = SliceModeParam.bay_addr + (imgWidth * imgHeight * bitdeph/8);    

    //ParseChrReg(GetIndepAddr());  // marked by Linkin

    ParseChrSmReg(GetIndepAddr());

    ParseGammaReg(GetIndepAddr());

    ParseGainOffsetReg(GetIndepAddr(), cgof);

    ParseGainCtrlReg(GetIndepAddr(), &GainCtrlParam);

    ParseOutSizeReg(GetIndepAddr(), &imeOutHSize, &imeOutVSize, &jpgQualityValue);

    if((imeOutHSize == 0) || (imeOutHSize > 32767))
        imeOutHSize = imgWidth;
    
    if((imeOutVSize==0) || (imeOutVSize>4096))
        imeOutVSize = imgHeight;
    
    if(jpgQualityValue>100)
        jpgQualityValue=95;

    SliceModeParam.jpg_hsize = imeOutHSize;
    SliceModeParam.jpg_vsize = imeOutVSize;
    //JPEG_Quality[0] = jpgQualityValue;

    ParseLSCReg(GetIndepAddr(), &vig_infor, &lsc_Enable);
    ParseGeoReg(GetIndepAddr(), &GeoH_infor, &GeoV_infor, &PREGeoEnable);

    ParseGeoCORRReg(GetIndepAddr(), &GeoH_en, &GeoH_Corr, &GeoH_Norm, &GeoV_en, &GeoV_Corr, &GeoV_Norm);

    ParseAPPReg(GetIndepAddr(), &IPENRRawEnable);

//#NT#2008/12/08#Chris Yang -begin
//#NT#Add for Sim tool
    ParseChrCurYReg(GetIndepAddr());
//#NT#2008/12/08#Chris Yang -end
//======================================================================================================================================================

    //#NT#2010/06/01/#Likin Huang -begin
    //#NT# debug message of sim flow
    #if SimMsgFlag
      debug_msg("PRE start ...\r\n");
    #endif
    //#NT#2010/06/01/#Likin Huang -begin
    
    //PRE D2D
    // -begin- PRE basic flow
    PRE_D2D_Param.PRE_Operation  = PRE_ENGINE_SET2D2D;
    PRE_D2D_Param.REG_pre_intrp_en |= PRE_INT_FLDEND;
    PRE_D2D_Param.PRE_EnFunction = 0x0;

    // Field number
    PRE_D2D_Param.REG_RemapFieldNum.Fld1 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld2 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld3 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld4 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld5 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld6 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld7 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.Fld8 = 0;
    PRE_D2D_Param.REG_RemapFieldNum.TotalFld = 0;

    // cfa
    PRE_D2D_Param.REG_pre_CfaPat = imgCFA;
    PRE_D2D_Param.REG_pre_CfaPat2 = imgCFA;

    // input flow info.    
    PRE_D2D_Param.REG_pre_input_Size.Hsize = imgWidth;
    PRE_D2D_Param.REG_pre_input_Size.Vsize = imgHeight;
    PRE_D2D_Param.REG_pre_input_Size.lineoft = imgWidth * bitdeph / 8;
    PRE_D2D_Param.REG_pre_in_ppb0_addr = SliceModeParam.bay_addr;
    PRE_D2D_Param.REG_pre_input_pack_bus = imgBitDepth;
    PRE_D2D_Param.REG_pre_input_source = PATH_DRAM_INPUT;

    PREOutAddr = SliceModeParam.bay_addr - PRE_D2D_Param.REG_pre_output_lineoft;

    // output flow info.
    PRE_D2D_Param.REG_pre_hv_ini = 0;
    PRE_D2D_Param.REG_pre_hv_width = imgWidth;
    PRE_D2D_Param.REG_pre_vv_ini = 0;
    PRE_D2D_Param.REG_pre_vv_height = imgHeight;    
    PRE_D2D_Param.REG_pre_output_size = imgWidth;
    PRE_D2D_Param.REG_pre_output_lineoft = imgWidth * bitdeph / 8;
    PRE_D2D_Param.REG_pre_out_ppb0_addr = PREOutAddr;
    PRE_D2D_Param.REG_pre_output_pack_bus = imgBitDepth;
    // -end- PRE basic flow    
    

    // color gain
    PRE_D2D_Param.PRE_EnFunction |= PRE_CG_EN;
    
    PRE_D2D_Param.REG_pre_output_path = PATH_PRE_GAMMA;

    
    PRE_D2D_Param.pre_cg_ofs.GainR   = cgof[0];
    PRE_D2D_Param.pre_cg_ofs.GainGr  = cgof[1];
    PRE_D2D_Param.pre_cg_ofs.GainGb  = cgof[2];
    PRE_D2D_Param.pre_cg_ofs.GainB   = cgof[3];
    PRE_D2D_Param.pre_cg_ofs.OfsR    = cgof[4];
    PRE_D2D_Param.pre_cg_ofs.OfsGr   = cgof[5];
    PRE_D2D_Param.pre_cg_ofs.OfsGb   = cgof[6];
    PRE_D2D_Param.pre_cg_ofs.OfsB    = cgof[7];       
    PRE_D2D_Param.pre_cg_ofs.GainSel = 0;    

    //Gain Control
    if(GainCtrlParam.GainCtrlEnable == TRUE)
    {
        PRE_D2D_Param.PRE_EnFunction |= PRE_GAINCTRL_EN;
    
        PRE_D2D_Param.pre_gainctrl_A.MapA    = GainCtrlParam.GainCtrlA_MapA;
        PRE_D2D_Param.pre_gainctrl_A.MapB    = GainCtrlParam.GainCtrlA_MapB;
        PRE_D2D_Param.pre_gainctrl_A.GainA   = GainCtrlParam.GainCtrlA_GainA;
        PRE_D2D_Param.pre_gainctrl_A.GainB   = GainCtrlParam.GainCtrlA_GainB;
        PRE_D2D_Param.pre_gainctrl_A.Switch  = GainCtrlParam.GainCtrlA_Switch;
        PRE_D2D_Param.pre_gainctrl_A.OffsetA = GainCtrlParam.GainCtrlA_OffsetA;
        PRE_D2D_Param.pre_gainctrl_A.OffsetB = GainCtrlParam.GainCtrlA_OffsetB;
    
        PRE_D2D_Param.pre_gainctrl_B.MapA    = GainCtrlParam.GainCtrlB_MapA;
        PRE_D2D_Param.pre_gainctrl_B.MapB    = GainCtrlParam.GainCtrlB_MapB;
        PRE_D2D_Param.pre_gainctrl_B.GainA   = GainCtrlParam.GainCtrlB_GainA;
        PRE_D2D_Param.pre_gainctrl_B.GainB   = GainCtrlParam.GainCtrlB_GainB;
        PRE_D2D_Param.pre_gainctrl_B.Switch  = GainCtrlParam.GainCtrlB_Switch;
        PRE_D2D_Param.pre_gainctrl_B.OffsetA = GainCtrlParam.GainCtrlB_OffsetA;
        PRE_D2D_Param.pre_gainctrl_B.OffsetB = GainCtrlParam.GainCtrlB_OffsetB;
    }

    // gamma table
    if(PREGammaEnable == TRUE)
    {
      PRE_D2D_Param.PRE_EnFunction |= PRE_GAMMA_EN;
    
      PRE_D2D_Param.REG_pre_output_path = PATH_POST_GAMMA;
      PRE_D2D_Param.pre_gamma_r = IndepGammaRLUT_T129;
      PRE_D2D_Param.pre_gamma_g = IndepGammaGLUT_T129;
      PRE_D2D_Param.pre_gamma_b = IndepGammaBLUT_T129;
    }
    else
    {
      PRE_D2D_Param.PRE_EnFunction &= ~PRE_GAMMA_EN;
      PRE_D2D_Param.REG_pre_output_path = PATH_PRE_GAMMA;
    }
    

    //Geo H
    if(GeoH_en == TRUE)
    {
        PRE_D2D_Param.PRE_EnFunction |= PRE_GEO_EN;  
    
        PRE_D2D_Param.pre_geo_setting.GEO_CENTX    = GeoH_infor.GEO_CENTX;
        PRE_D2D_Param.pre_geo_setting.GEO_CENTY    = GeoH_infor.GEO_CENTY;
        PRE_D2D_Param.pre_geo_setting.GEO_CENTXS   = GeoH_infor.GEO_CENTXS;
        PRE_D2D_Param.pre_geo_setting.GEO_CENTYS   = GeoH_infor.GEO_CENTYS;
        PRE_D2D_Param.pre_geo_setting.GEO_NORMFACT = GeoH_infor.GEO_NORMFACT;
        PRE_D2D_Param.pre_geo_setting.GEO_NORMBIT  = GeoH_infor.GEO_NORMBIT;
        PRE_D2D_Param.pre_geo_setting.GEO_RLUTGAIN = GeoH_infor.GEO_RLUTGAIN;
        PRE_D2D_Param.pre_geo_setting.GEO_GLUTGAIN = GeoH_infor.GEO_GLUTGAIN;
        PRE_D2D_Param.pre_geo_setting.GEO_BLUTGAIN = GeoH_infor.GEO_BLUTGAIN;
        PRE_D2D_Param.pre_geo_setting.GEO_XDIST    = GeoH_infor.GEO_XDIST;
        PRE_D2D_Param.pre_geo_setting.GEO_YDIST    = GeoH_infor.GEO_YDIST;
        PRE_D2D_Param.pre_geo_setting.GEO_DISTSEL  = GeoH_infor.GEO_DISTSEL;
        PRE_D2D_Param.pre_geo_setting.GEO_Lut      = GeoH_infor.GEO_Lut;
        PRE_D2D_Param.pre_geo_setting.GEO_CORREN   = GeoH_Corr;
        PRE_D2D_Param.pre_geo_setting.GEO_CORRNORM = GeoH_Norm;
    }

    //VIG
    if(lsc_Enable == TRUE)
    {
        PRE_D2D_Param.PRE_EnFunction |= PRE_VIG_EN;
        
        PRE_D2D_Param.pre_vig_setting.VIG_XR    = vig_infor.VIG_XR;
        PRE_D2D_Param.pre_vig_setting.VIG_XGR   = vig_infor.VIG_XGR;
        PRE_D2D_Param.pre_vig_setting.VIG_XGB   = vig_infor.VIG_XGB;
        PRE_D2D_Param.pre_vig_setting.VIG_XB    = vig_infor.VIG_XB;
        PRE_D2D_Param.pre_vig_setting.VIG_YR    = vig_infor.VIG_YR;
        PRE_D2D_Param.pre_vig_setting.VIG_YGR   = vig_infor.VIG_YGR;
        PRE_D2D_Param.pre_vig_setting.VIG_YGB   = vig_infor.VIG_YGB;
        PRE_D2D_Param.pre_vig_setting.VIG_YB    = vig_infor.VIG_YB;
        PRE_D2D_Param.pre_vig_setting.VIG_MUL   = vig_infor.VIG_MUL;
        PRE_D2D_Param.pre_vig_setting.VIG_T     = vig_infor.VIG_T;
        PRE_D2D_Param.pre_vig_setting.VIG_XDIV  = vig_infor.VIG_XDIV;
        PRE_D2D_Param.pre_vig_setting.VIG_YDIV  = vig_infor.VIG_YDIV;
        PRE_D2D_Param.pre_vig_setting.VIG_YDIST = vig_infor.VIG_YDIST;
        PRE_D2D_Param.pre_vig_setting.VIG_R_LUT = vig_infor.VIG_R_LUT;
        PRE_D2D_Param.pre_vig_setting.VIG_G_LUT = vig_infor.VIG_G_LUT;
        PRE_D2D_Param.pre_vig_setting.VIG_B_LUT = vig_infor.VIG_B_LUT;
        
        //Line VIG
        PRE_D2D_Param.pre_vigline_setting.VIG_R2_LUT = shadingLineTable;
        PRE_D2D_Param.pre_vigline_setting.VIG_G2_LUT = shadingLineTable;
        PRE_D2D_Param.pre_vigline_setting.VIG_B2_LUT = shadingLineTable;
    }    
    Indep_pre_Dram2Dram(PRE_D2D_Param, TRUE);

    GraphOutAddr1 = PREOutAddr + 
                    (PRE_D2D_Param.REG_pre_output_lineoft * imgHeight) +
                    PRE_D2D_Param.REG_pre_output_lineoft*4;

    if(GeoV_en)  //preD2DParam.GEO_EN==1
    {
        //Step1:Rotate 90 for image processing
        grph_open();

        grph_setImg1(PREOutAddr, (imgWidth*bitdeph/8), imgHeight, (imgWidth*bitdeph/8));
        grph_setImg2(GraphOutAddr1, (imgHeight*bitdeph/8));
        if(bitdeph == 8)
            grph_setGOP(0, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);
        else
            grph_setGOP(1, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);

        // Step2: perform PRE Dram2Dram
        // -begin- PRE basic flow
        PRE_D2D_Param.PRE_Operation  = PRE_ENGINE_SET2D2D;
        PRE_D2D_Param.REG_pre_intrp_en |= PRE_INT_FLDEND;
        PRE_D2D_Param.PRE_EnFunction = 0x0;
    
        // Field number
        PRE_D2D_Param.REG_RemapFieldNum.Fld1 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld2 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld3 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld4 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld5 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld6 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld7 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.Fld8 = 0;
        PRE_D2D_Param.REG_RemapFieldNum.TotalFld = 0;
    
        // cfa
        PRE_D2D_Param.REG_pre_CfaPat = imgCFA;
        PRE_D2D_Param.REG_pre_CfaPat2 = imgCFA;
    
        // input flow info.    
        PRE_D2D_Param.REG_pre_input_Size.Hsize = imgHeight;
        PRE_D2D_Param.REG_pre_input_Size.Vsize = imgWidth;
        PRE_D2D_Param.REG_pre_input_Size.lineoft = imgHeight * bitdeph / 8;
        PRE_D2D_Param.REG_pre_in_ppb0_addr = GraphOutAddr1;
        PRE_D2D_Param.REG_pre_input_pack_bus = imgBitDepth;
        PRE_D2D_Param.REG_pre_input_source = PATH_DRAM_INPUT;
    
        // output flow info.
        PRE_D2D_Param.REG_pre_hv_ini = 0;
        PRE_D2D_Param.REG_pre_hv_width = imgHeight;
        PRE_D2D_Param.REG_pre_vv_ini = 0;
        PRE_D2D_Param.REG_pre_vv_height = imgWidth;    
        PRE_D2D_Param.REG_pre_output_size = imgHeight;
        PRE_D2D_Param.REG_pre_output_lineoft = imgHeight * bitdeph / 8;
        PRE_D2D_Param.REG_pre_out_ppb0_addr = GraphOutAddr1 - PRE_D2D_Param.REG_pre_output_lineoft ;
        PRE_D2D_Param.REG_pre_output_pack_bus = imgBitDepth;
        // -end- PRE basic flow 

        PRE_D2D_Param.PRE_EnFunction |= PRE_GEO_EN;
        
        PRE_D2D_Param.pre_geo_setting.GEO_CENTX    = GeoV_infor.GEO_CENTX;
        PRE_D2D_Param.pre_geo_setting.GEO_CENTY    = GeoV_infor.GEO_CENTY;
        PRE_D2D_Param.pre_geo_setting.GEO_CENTXS   = GeoV_infor.GEO_CENTXS;
        PRE_D2D_Param.pre_geo_setting.GEO_CENTYS   = GeoV_infor.GEO_CENTYS;
        PRE_D2D_Param.pre_geo_setting.GEO_NORMFACT = GeoV_infor.GEO_NORMFACT;
        PRE_D2D_Param.pre_geo_setting.GEO_NORMBIT  = GeoV_infor.GEO_NORMBIT;
        PRE_D2D_Param.pre_geo_setting.GEO_RLUTGAIN = GeoV_infor.GEO_RLUTGAIN;
        PRE_D2D_Param.pre_geo_setting.GEO_GLUTGAIN = GeoV_infor.GEO_GLUTGAIN;
        PRE_D2D_Param.pre_geo_setting.GEO_BLUTGAIN = GeoV_infor.GEO_BLUTGAIN;
        PRE_D2D_Param.pre_geo_setting.GEO_XDIST    = GeoV_infor.GEO_XDIST;
        PRE_D2D_Param.pre_geo_setting.GEO_YDIST    = GeoV_infor.GEO_YDIST;
        PRE_D2D_Param.pre_geo_setting.GEO_DISTSEL  = GeoV_infor.GEO_DISTSEL;
        PRE_D2D_Param.pre_geo_setting.GEO_Lut      = GeoV_infor.GEO_Lut;
        PRE_D2D_Param.pre_geo_setting.GEO_CORREN   = GeoV_Corr;
        PRE_D2D_Param.pre_geo_setting.GEO_CORRNORM = GeoV_Norm;
        Indep_pre_Dram2Dram(PRE_D2D_Param, TRUE);

        //Step3:Rotate image back.
        grph_setImg1(PRE_D2D_Param.REG_pre_out_ppb0_addr, imgHeight*bitdeph/8, imgWidth, imgHeight*bitdeph/8);
        grph_setImg2(PREOutAddr, imgWidth*bitdeph/8);
        if(bitdeph==8)
            grph_setGOP(0, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);
        else
            grph_setGOP(1, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);

        grph_close();
    }

    //#NT#2010/06/01/#Likin Huang -begin
    //#NT# debug message of sim flow
    #if SimMsgFlag
      debug_msg("PRE done ...\r\n");
    #endif
    //#NT#2010/06/01/#Likin Huang -end
    
    //#NT#2009/11/05#Linkin Huang -begin
    //#NT# added by Linkin
    indSliceModeInfo.BayerAddr        = PREOutAddr;
    indSliceModeInfo.BayerHsize       = SliceModeParam.bay_hsize;
    indSliceModeInfo.BayerVsize       = SliceModeParam.bay_vsize;
    indSliceModeInfo.PreBitDepth      = bitdeph;
    indSliceModeInfo.PreCfa           = imgCFA;
    indSliceModeInfo.ImeOutHsize      = SliceModeParam.jpg_hsize;
    indSliceModeInfo.ImeOutVsize      = SliceModeParam.jpg_vsize;
    indSliceModeInfo.ImeOutLineOffset = indSliceModeInfo.ImeOutHsize;
    indSliceModeInfo.JPEGQuality      = jpgQualityValue;
    indSliceModeInfo.JpgOutAddr       = SliceModeParam.jpg_addr;
    indSliceModeInfo.SLC_BUF_ADDR     = SliceModeParam.pingpong_addr; 
    IndepJpegAddr                     = indSliceModeInfo.JpgOutAddr;
    //#NT#2009/11/05#Linkin Huang -end

    //#NT#2010/06/01/#Likin Huang -begin
    //#NT# debug message of sim flow
    #if SimMsgFlag
      debug_msg("IPE-IME start ...\r\n");
    #endif
    //#NT#2010/06/01/#Likin Huang -end
    
    Indep_SliceMode_Process(&indSliceModeInfo);

    //#NT#2010/06/01/#Likin Huang -begin
    //#NT# debug message of sim flow
    #if SimMsgFlag
      debug_msg("IPE-IME done ...\r\n");
    #endif
    //#NT#2010/06/01/#Likin Huang -end

    debug_msg("JPEG size : %d\r\n", indSliceModeInfo.JpegSize);
    debug_msg("JPEG out Addr : 0x%08x\r\n", indSliceModeInfo.JpgOutAddr);


    return E_OK;
}




void SimToolTest(void)
{
  //SIE_OPENOBJ SIEOpenObj;
  //PRE_OPENOBJ PREOpenObj;
  UINT32 BIN_ADDR = 0x0, RAW_ADDR = 0x0;
  UINT32 FileSizeBin = 3060, FileSizeImg = 4000*2992;
  
  BIN_ADDR = GetIndepAddr();
  RAW_ADDR = GetIndepAddr()+5000*64*6+0x100000+8192;  
  
  //SIEOpenObj.FP_SIEISR_CB = sie_isrCB;
  //PREOpenObj.FP_PREISR_CB = pre_isrCB;

  //sie_open(&SIEOpenObj);
  //pre_open(&PREOpenObj);    
  StopPreviewDramData();
            pre_close();
            sie_close();
            /*
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            if (SensorInfor.bSieMClkEn == ENABLE)
            {
                pll_disableClock(SIE_MCLK);
            }
//#NT#2009/10/08#Bowen Li -end
*/
            ipe_close();
            ime_close();

  FilesysWaitCmdFinish(FST_TIME_INFINITE);
  if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, "A:\\test.bin", (UINT8 *)BIN_ADDR, &FileSizeBin, 0, FST_TIME_INFINITE))
  {
   debug_msg("Bin file reading, fail ...\r\n");
  }
  else
  {
   debug_msg("Bin file reading, success ...\r\n");
  }

  FilesysWaitCmdFinish(FST_TIME_INFINITE);
  if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, "A:\\img.raw", (UINT8 *)RAW_ADDR, &FileSizeImg, 0, FST_TIME_INFINITE))
  {
   debug_msg("Bin file reading, fail ...\r\n");
  }
  else
  {
   debug_msg("Bin file reading, success ...at 0x%08x\r\n", RAW_ADDR);
  }

  Trigger_IndepSlice630();

  //debug_msg("Buffer addr : 0x%08x\r\n",GetIndepAddr()); 
  
}



/*    
void IndepChromaCurve(UINT32 hSize, UINT32 vSize, UINT32 sourceAddr, UINT32 distAddr, UINT ipe_GaEn, UINT32 *chrSmLUT)
{
    STR_FLDNUM pre_FieldNum;
    unsigned long PreIntStatus;
    __align(4)UW gammaR[130], gammaG[130], gammaB[130];

    pre_FieldNum.Fld1=0;
    pre_FieldNum.Fld2=0;
    pre_FieldNum.Fld3=0;
    pre_FieldNum.Fld4=0;
    pre_FieldNum.Fld5=0;
    pre_FieldNum.Fld6=0;
    pre_FieldNum.Fld7=0;
    pre_FieldNum.Fld8=0;
    pre_FieldNum.TotalFld=0;

    ipe_DisableFeature(IPE_PREGAMMA_EN);
    ipe_Stop();
    ipe_Start();

    GetPREGammaValue(0x9A001400, gammaR, gammaG, gammaB);
    dis_int(_INT_PRE);
    pll_setClockRate(PRE_CLKSRC, PRE_CLKSRC_PRE);
    pll_setClockRate(PRE_CLKSEL, PRE_CLK_96M);
    pre_ClearAllMode();
    pre_Reset(1);
    pre_en(0);

    pre_SetFldNum(pre_FieldNum);
    pre_SetPre2DramPath(POST_GAMMA);
    pre_SieDirectPre(DRAM_INPUT);
    pre_SetInputSize(hSize/2, vSize);
    pre_SetInputPackMode(2);
    pre_SetInputStartAdd(sourceAddr);
    pre_SetInputLineOffset(hSize/2);

    pre_SetHcropSize(0, hSize/2);
    pre_SetVcropSize(0, vSize);

    pre_FunctionEnable(0, PRE_HSCL_EN);
    pre_SetOutputSize(hSize/2);
    pre_SetOutputPackMode(2);
    pre_SetOutputStartAdd(distAddr);
    pre_SetOutputLineOffset(hSize/2);
    pre_OutputPpbEn(0);

    pre_RstSac();
    pre_ClrIntrStatus(PRE_INT_ALL);

    pre_SetGamma(chrSmLUT, chrSmLUT, chrSmLUT);

    //Start operation
    pre_FunctionEnable(0, PRE_VIG_EN);
    pre_FunctionEnable(0, PRE_GEO_EN);
    pre_FunctionEnable(0, PRE_BLM_EN);
    pre_FunctionEnable(0, PRE_DF_EMBED_EN);
    pre_FunctionEnable(0, PRE_CG_EN);
    pre_FunctionEnable(1, PRE_GAMMA_EN);
    pre_FunctionEnable(0, PRE_VACC_EN);
    pre_FunctionEnable(0, PRE_DF_SUB_EN);

    pre_Reset(0);
    pre_Load();
    pre_en(1);

    PreIntStatus=pre_ReadIntrStatus();
    while(!(PreIntStatus & PRE_INT_FLDEND))
        PreIntStatus=pre_ReadIntrStatus();

    pre_SetGamma(gammaR, gammaG, gammaB);
    pre_FunctionEnable(0, PRE_GAMMA_EN);
    pre_Reset(1);
    pre_Reset(0);
    if(ipe_GaEn==1)
        ipe_EnableFeature(IPE_PREGAMMA_EN);
    else
        ipe_DisableFeature(IPE_PREGAMMA_EN);
    ipe_Stop();
    ipe_Start();
}
*/

/*
void IndepChromaFilter(UINT CbAddr, UINT CrAddr, UINT bufferAddr, UINT LineOffset, UINT Hsize, UINT Vsize, UINT ISO_Value)
{
    FDE_CHR_PRAM chroma_info;
    UINT32 bufferAddr2;
    UINT32 uiFlag, i;

    bufferAddr2= bufferAddr + Hsize*Vsize;
    chroma_info.filt_pram = &IndepChrParam;//(FDE_CHR_FILT_PRAM *)&CHR_Param[ISO_Value];

    grph_open();
    grph_swReset();
    grph_enableDMA();

    FDE_ChrOpen();
    for(i = 0; i < chroma_info.filt_pram->repeat; i ++)
    {
        //Cb Horizontal filtering
        chroma_info.in_addr = CbAddr;
        chroma_info.out_addr = bufferAddr;
        chroma_info.width = Hsize;
        chroma_info.height = Vsize;
        chroma_info.lineoffset = LineOffset;
        fde_ChromaFilter(&chroma_info);
        wai_flg((UINT *)&uiFlag, FLG_ID_INT, FLGINT_FD, TWF_ORW | TWF_CLR);

        //Cr Horizontal filtering
        chroma_info.in_addr = CrAddr;
        chroma_info.out_addr = bufferAddr2;
        chroma_info.width = Hsize;
        chroma_info.height = Vsize;
        chroma_info.lineoffset = LineOffset;
        fde_ChromaFilter(&chroma_info);

        //Cb rotate
        grph_setImg1(bufferAddr, LineOffset, Vsize, Hsize);
        grph_setImg2(CbAddr, Vsize);
        grph_setGOP(0, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);

        wai_flg((UINT *)&uiFlag, FLG_ID_INT, FLGINT_FD, TWF_ORW | TWF_CLR);
        //Cb Verticle filtering
        chroma_info.in_addr = CbAddr;
        chroma_info.out_addr = bufferAddr;
        chroma_info.width = Vsize;
        chroma_info.height = Hsize;
        chroma_info.lineoffset = Vsize;
        fde_ChromaFilter(&chroma_info);

        //Cr rotate
        grph_setImg1(bufferAddr2, LineOffset, Vsize, Hsize);
        grph_setImg2(CrAddr, Vsize);
        grph_setGOP(0, GRPH_DST_2, FALSE, GRPH_GOP_ROT_90);

        wai_flg((UINT *)&uiFlag, FLG_ID_INT, FLGINT_FD, TWF_ORW | TWF_CLR);
        //Cr Verticle filtering
        chroma_info.in_addr = CrAddr;
        chroma_info.out_addr = bufferAddr2;
        chroma_info.width = Vsize;
        chroma_info.height = Hsize;
        chroma_info.lineoffset = Vsize;
        fde_ChromaFilter(&chroma_info);

        //Cb rotate
        grph_setImg1(bufferAddr, Vsize, Hsize, Vsize);
        grph_setImg2(CbAddr, LineOffset);
        grph_setGOP(0, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);

        wai_flg((UINT *)&uiFlag, FLG_ID_INT, FLGINT_FD, TWF_ORW | TWF_CLR);
        //Cr rotate
        grph_setImg1(bufferAddr2, Vsize, Hsize, Vsize);
        grph_setImg2(CrAddr, LineOffset);
        grph_setGOP(0, GRPH_DST_2, FALSE, GRPH_GOP_ROT_270);
    }
    grph_disableDMA();
    grph_close();
    fde_ChrReset();
    FDE_ChrClose();
}
*/

/*
void IndepIPENRRawOut(UINT OutAddr, UINT LineOffset)
{
    ipe_SetDmaOutAddr(OutAddr, POST_NR_DATA_TYPE ,  LineOffset);
}
*/


