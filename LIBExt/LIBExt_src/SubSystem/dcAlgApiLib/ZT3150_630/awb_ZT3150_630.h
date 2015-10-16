#ifndef _AWB_ZT3150_630_H_
#define _AWB_ZT3150_630_H_

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
    S_hi = 70,
    S_lo = 71,
    RBSum_hi = 80,
    RBSum_lo = 81,
    Delta_hi = 90,
    Delta_lo = 91,
    DiffGain_hi = 100,
    DiffGain_lo = 101,
    Modify_RB = 110,     //Modify RGain,BGain of the referenced block
    Special_case = 111,
    Restart = 112
}Whitepoint;

///////////////////////////////////////////////////////////////////////
// awb parameter
#define Thr_LowBright2 5//20
#define Thr_HighBright2 200
#define Outdoor_LV 90
#define Sunlight_LV 130
#define Night_LV 40

#define AWB_RATE_NORMAL          0
#define AWB_RATE_STARTUP         1
#define AWB_RATE_LOWLIGHT    2

//<LV4 //HD500C6
#define Thr_GR_L1 53//86
#define Thr_GR_H1 123//194//186
#define Thr_GB_L1 125
#define Thr_GB_H1 220
#define Thr_RB_L1 130//59
#define Thr_RB_H1 180
#define Thr_GainSum_L1 208//276//290
#define Thr_GainSum_H1 342//378
#define Thr_S_L1		15//18//19
#define Thr_S_H1		38
#define Thr_RBSum_L1 206
#define Thr_RBSum_H1 236
#define Thr_Delta_L1 0//12//20
#define Thr_Delta_H1 22//150//166//150//132//125//110
#define Thr_DiffGain_L1 0//4//25//34
#define Thr_DiffGain_H1 146

//LV4~LV9 //HD500C6
#define Thr_GR_L2 53//86
#define Thr_GR_H2 198//194//186
#define Thr_GB_L2 112
#define Thr_GB_H2 237
#define Thr_RB_L2 58//59
#define Thr_RB_H2 261
#define Thr_GainSum_L2 258//276//290
#define Thr_GainSum_H2 342//378
#define Thr_S_L2		14//18//19
#define Thr_S_H2		46
#define Thr_RBSum_L2 198
#define Thr_RBSum_H2 300
#define Thr_Delta_L2 10//12//20
#define Thr_Delta_H2 144//150//166//150//132//125//110
#define Thr_DiffGain_L2 0//4//25//34
#define Thr_DiffGain_H2 146

//LV9 up
#if 1   //0607
#define Thr_GR_L3 102//132
#define Thr_GR_H3 190//188
#define Thr_GB_L3 110//112
#define Thr_GB_H3 188//162
#define Thr_RB_L3 59
#define Thr_RB_H3 164//112
#define Thr_GainSum_L3 250//262
#define Thr_GainSum_H3 346
#define Thr_S_L3		12//13
#define Thr_S_H3		34
#define Thr_RBSum_L3 196
#define Thr_RBSum_H3 225
#define Thr_Delta_L3 46//66
#define Thr_Delta_H3 140
#define Thr_DiffGain_L3 0
#define Thr_DiffGain_H3 72
#else   //orig
#define Thr_GR_L3 132//152
#define Thr_GR_H3 190//188
#define Thr_GB_L3 110//112
#define Thr_GB_H3 162//156//142
#define Thr_RB_L3 59
#define Thr_RB_H3 112//106//94
#define Thr_GainSum_L3 262//276
#define Thr_GainSum_H3 346//318
#define Thr_S_L3		13//19
#define Thr_S_H3		34
#define Thr_RBSum_L3 196
#define Thr_RBSum_H3 225
#define Thr_Delta_L3 66//70
#define Thr_Delta_H3 140//112
#define Thr_DiffGain_L3 0
#define Thr_DiffGain_H3 72
#endif
/*
//over LV13
#define Thr_GR_L4  125
#define Thr_GR_H4  155
#define Thr_GB_L4  92
#define Thr_GB_H4  125
#define Thr_RB_L4  60
#define Thr_RB_H4  96
#define Thr_GainSum_L4 230
#define Thr_GainSum_H4 320
*/


char AwbCheckWhite(unsigned int R,unsigned int G,unsigned int B);
char AwbCheckWhite1(unsigned int R,unsigned int G,unsigned int B);
unsigned int AwbCheckWhiteYCC(unsigned char rr,unsigned char gg,unsigned char bb);
unsigned int Saturation(unsigned int RR,unsigned int GG,unsigned int BB);
void RGBToYCbCr(int rr,int gg,int bb,float *y_ycc,float *cb_ycc,float *cr_ycc);

//extern data
extern UINT Rgain,Ggain,Bgain,OldRgain,OldGgain,OldBgain;
extern unsigned char            gAWBWeight[64];
extern UB R_acc[64], G_acc[64], B_acc[64];
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
char AwbCheckWhite(unsigned int R,unsigned int G,unsigned int B);
unsigned int Saturation(unsigned int RR,unsigned int GG,unsigned int BB);
void RGBToYCbCr(int rr,int gg,int bb,float *y_ycc,float *cb_ycc,float *cr_ycc);
unsigned int AwbCheckWhiteYCC(unsigned char rr,unsigned char gg,unsigned char bb);
unsigned int SumRgBg(unsigned int RR,unsigned int GG,unsigned int BB);
unsigned int SumRBGain(unsigned int RR,unsigned int GG,unsigned int BB);
unsigned int DiffRgBg(unsigned int RR,unsigned int GG,unsigned int BB);

#endif

