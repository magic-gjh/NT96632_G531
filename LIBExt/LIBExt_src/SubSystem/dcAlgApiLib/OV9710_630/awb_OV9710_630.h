#ifndef _AWB_OV9710_630_H_
#define _AWB_OV9710_630_H_

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
    Modify_RB = 100,     //Modify RGain,BGain of the referenced block
    Special_case = 101,
    Restart = 102
}Whitepoint;

///////////////////////////////////////////////////////////////////////
// awb parameter
#define Thr_LowBright2 20
#define Thr_HighBright2 200
#define Outdoor_LV 90
#define Sunlight_LV 130

//LV4~LV13 
#define Thr_GR_L2 84//83
#define Thr_GR_H2 164//215
#define Thr_GB_L2 126//92
#define Thr_GB_H2 213//280
#define Thr_RB_L2 80//50
#define Thr_RB_H2 254//306
#define Thr_GainSum_L2 284//238
#define Thr_GainSum_H2 334//403
#define Thr_GainMux_L2 179//170
#define Thr_GainMux_H2 267//320//269
#define Thr_GainDiff_L2 20
//LV10~LV13
#define Thr_GR_L3 130//147
#define Thr_GR_H3 164//215
#define Thr_GB_L3 126//92
#define Thr_GB_H3 186//204//140//jack lan
#define Thr_RB_L3 80//50
#define Thr_RB_H3 156//88
#define Thr_GainSum_L3 284//238
#define Thr_GainSum_H3 334//376
#define Thr_GainMux_L3 199//170
#define Thr_GainMux_H3 267//320//269
#define Thr_GainDiff_L3 20
//over LV13
#define Thr_GR_L4  148//158
#define Thr_GR_H4  164//218
#define Thr_GB_L4  126//90
#define Thr_GB_H4  139//140
#define Thr_RB_L4  80//45
#define Thr_RB_H4  93//80
#define Thr_GainSum_L4 284//250
#define Thr_GainSum_H4 296//370
#define Thr_GainDiff_L4 15


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

