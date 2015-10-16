#ifndef _AWB_NT99141_632_H_
#define _AWB_NT99141_632_H_

///////////////////////////////////////////////
//AWB Header
//////////////////////////////////////////////

#define AWB_EXPERIMENT  0
#define MASSAGE_ON_DIS_GAIN 0
#define MASSAGE_ON_DIS_LV 0
#define Calibration_Test 0
#define Awb_Cal_On 0
#define AwbGrayWorld 0
#define AWB_NEWALG 0
#define SMD 1
#define GM  0
#define foxconn_awbtest 0

#define CCDADJUST_R         303
#define CCDADJUST_B         308

typedef enum {
    _3500k=0,
    _4000k,
    _5000k,
    _6000k,
    _7500k,
    _low=5,
    _middle,
    _high,
    _fail=8
}ColorTemperature;

typedef enum {
    OK = 1,
    Rgain_hi = 10,
    Rgain_lo = 11,
    Bgain_hi = 20,
    Bgain_lo = 21,
    RBGain_hi = 30,
    RBGain_lo = 31,
    GainSum_hi = 40,
    GainSum_lo = 41,
    Bright_lo = 50,
    Bright_hi = 51,
    R_BGain_hi = 60,
    R_BGain_lo = 61,
    OutofRange = 62,
    GainDiff_min = 70,
    GainDiff_max = 71,
    GainMux_min = 80,
    GainMux_max = 81,
    Sat_min = 90,
    Sat_max = 91,    
    Modify_RB = 100,     //Modify RGain,BGain of the referenced block
    Special_case = 101,
    Restart = 102
}Whitepoint;

///////////////////////////////////////////////////////////////////////
// awb parameter
#define Thr_LowBright2 10
#define Thr_HighBright2 140//100//200
#define Outdoor_LV 90
#define Sunlight_LV 130

//LV4~LV10
#define Thr_GR_L2       38//50//81
#define Thr_GR_H2       206//167
#define Thr_GB_L2       49//114
#define Thr_GB_H2       270//187
#define Thr_RB_L2       37//73
#define Thr_RB_H2       450//233//211
#define Thr_GainSum_L2  141//259
#define Thr_GainSum_H2  405//329
#define Thr_GainMux_L2  46//149
#define Thr_GainMux_H2  313//267
#define Thr_GainDiff_L2 0//30
#define Thr_Sat_L2      10
#define Thr_Sat_H2      52//43

//LV10~LV13
#define Thr_GR_L3       38
#define Thr_GR_H3       175//148
#define Thr_GB_L3       49
#define Thr_GB_H3       179//139
#define Thr_RB_L3       37
#define Thr_RB_H3       450
#define Thr_GainSum_L3  141
#define Thr_GainSum_H3  355
#define Thr_GainMux_L3  46
#define Thr_GainMux_H3  313
#define Thr_GainDiff_L3 0
//over LV13
#define Thr_GR_L4       Thr_GR_L2
#define Thr_GR_H4       Thr_GR_H2
#define Thr_GB_L4       Thr_GB_L2
#define Thr_GB_H4       Thr_GB_H2
#define Thr_RB_L4       Thr_RB_L2
#define Thr_RB_H4       Thr_RB_H2
#define Thr_GainSum_L4  Thr_GainSum_L2
#define Thr_GainSum_H4  Thr_GainSum_H2
#define Thr_GainDiff_L4 Thr_GainDiff_L2


unsigned int AwbCheckWhiteYCC(unsigned char rr,unsigned char gg,unsigned char bb);
void RGBToYCbCr(int rr,int gg,int bb,float *y_ycc,float *cb_ycc,float *cr_ycc);

//extern data
extern UINT Rgain,Ggain,Bgain,OldRgain,OldGgain,OldBgain,CapRgain,CapGgain,CapBgain;
extern unsigned char            gAWBWeight[64];
extern unsigned char            gCapAWBWeight[64];
extern UB R_acc[64], G_acc[64], B_acc[64];
extern UB CapR_acc[64], CapG_acc[64], CapB_acc[64];
extern unsigned int Calibration_Rg,Calibration_Bg;
extern unsigned int Rcfg,Bcfg;
#if Awb_Cal_On
extern UB R_acc1[64],G_acc1[64],B_acc1[64];
extern UB cal_Rgain,cal_Bgain;
#endif

//extern function
extern void aaa_InitAWB(void);
extern void aaa_AWBprocess(void);
extern void AwbPrv2Cap(void);
extern void AwbCap2Prv(void);
extern void aaa_InitRBgain(void);
void RGBToYCbCr(int rr,int gg,int bb,float *y_ycc,float *cb_ycc,float *cr_ycc);
unsigned int AwbCheckWhiteYCC(unsigned char rr,unsigned char gg,unsigned char bb);
unsigned int SumRgBg(unsigned int RR,unsigned int GG,unsigned int BB);
unsigned int SumRBGain(unsigned int RR,unsigned int GG,unsigned int BB);
unsigned int DiffRgBg(unsigned int RR,unsigned int GG,unsigned int BB);

#endif

