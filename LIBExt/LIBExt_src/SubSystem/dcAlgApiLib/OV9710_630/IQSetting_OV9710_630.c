/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2008.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: IQSetting_OV9710_630.c                                           *
* Description:     IQ setting of Alg                                      *
* Author: YH                                                           *
****************************************************************************/
#include "sensor_model.h"
//#NT#20091209#ethanlau -begin
//add for chromafilter
#include "grph.h"

//#NT#20091209#ethanlau -end
#if OV9710_630
__align(4) IPE_ISO_PARAM  IPE_ISOParam[Total_ISONum] =
{
    //Movie & preview
//                  NRSel,            NStabSel,            NDtabSel,            FStabSel,            FDtabsel,                EdgeSel,                     KerSel,                EmapSel,            EStabSel
    {IPE_NRParam[ISONUM0], IPE_nrStab[ISONUM0], IPE_nrDtab[ISONUM0], IPE_fmStab[ISONUM0], IPE_fmDtab[ISONUM0], IPE_EdgeParam[ISONUM0], IPE_edgeKernelDiv[ISONUM0], IPE_emap16tab[ISONUM0], IPE_emStab[ISONUM0]},
    {IPE_NRParam[ISONUM1], IPE_nrStab[ISONUM1], IPE_nrDtab[ISONUM1], IPE_fmStab[ISONUM1], IPE_fmDtab[ISONUM1], IPE_EdgeParam[ISONUM1], IPE_edgeKernelDiv[ISONUM1], IPE_emap16tab[ISONUM1], IPE_emStab[ISONUM1]},
    {IPE_NRParam[ISONUM2], IPE_nrStab[ISONUM2], IPE_nrDtab[ISONUM2], IPE_fmStab[ISONUM2], IPE_fmDtab[ISONUM2], IPE_EdgeParam[ISONUM2], IPE_edgeKernelDiv[ISONUM2], IPE_emap16tab[ISONUM2], IPE_emStab[ISONUM2]},
    {IPE_NRParam[ISONUM3], IPE_nrStab[ISONUM3], IPE_nrDtab[ISONUM3], IPE_fmStab[ISONUM3], IPE_fmDtab[ISONUM3], IPE_EdgeParam[ISONUM3], IPE_edgeKernelDiv[ISONUM3], IPE_emap16tab[ISONUM3], IPE_emStab[ISONUM3]},
    //Capture
//                  NRSel,            NStabSel,            NDtabSel,            FStabSel,            FDtabsel,                EdgeSel,                     KerSel,                EmapSel,            EStabSel
    {IPE_NRParam[ISONUM4], IPE_nrStab[ISONUM4], IPE_nrDtab[ISONUM4], IPE_fmStab[ISONUM4], IPE_fmDtab[ISONUM4], IPE_EdgeParam[ISONUM4], IPE_edgeKernelDiv[ISONUM4], IPE_emap16tab[ISONUM4], IPE_emStab[ISONUM4]},
    {IPE_NRParam[ISONUM5], IPE_nrStab[ISONUM5], IPE_nrDtab[ISONUM5], IPE_fmStab[ISONUM5], IPE_fmDtab[ISONUM5], IPE_EdgeParam[ISONUM5], IPE_edgeKernelDiv[ISONUM5], IPE_emap16tab[ISONUM5], IPE_emStab[ISONUM5]},
    {IPE_NRParam[ISONUM6], IPE_nrStab[ISONUM6], IPE_nrDtab[ISONUM6], IPE_fmStab[ISONUM6], IPE_fmDtab[ISONUM6], IPE_EdgeParam[ISONUM6], IPE_edgeKernelDiv[ISONUM6], IPE_emap16tab[ISONUM6], IPE_emStab[ISONUM6]},
    {IPE_NRParam[ISONUM7], IPE_nrStab[ISONUM7], IPE_nrDtab[ISONUM7], IPE_fmStab[ISONUM7], IPE_fmDtab[ISONUM7], IPE_EdgeParam[ISONUM7], IPE_edgeKernelDiv[ISONUM7], IPE_emap16tab[ISONUM7], IPE_emStab[ISONUM7]}
};

__align(4) STR_IPE_NOVANR2_PARA IPE_NRParam[Total_ISONum][16]=
{
//   RBW, GW, GBAL, OUTL, OutlRbw, OutlGw, Honly, Nsndir, Nsvsb, NsGain, Nsmar, Lpfw, Sumsel, Dthr,         Nthr[4]
    {16,   16,    0,    0,       0,      0,     0,      0,     0,     64,    24,    7,      0,   16, { 24,  18,  18,  24}},
    {16,   16,    0,    0,       0,      0,     0,      0,     0,     64,    24,    7,      0,   16, { 24,  18,  18,  24}},
    {32,   16,    0,    0,      32,     16,     0,      0,     0,     64,    24,    7,      0,   32, { 30,  20,  20,  30}},
    {48,   16,    0,    0,      60,     20,     0,      0,     0,     64,    24,    7,      0,   32, { 40,  30,  30,  40}},

//   RBW, GW, GBAL, OUTL, OutlRbw, OutlGw, Honly, Nsndir, Nsvsb, NsGain, Nsmar, Lpfw, Sumsel, Dthr,         Nthr[4]
    {16,   16,    0,    0,       0,      0,     0,      0,     2,     64,    24,    7,      0,   16, { 24,  18,  18,  24}},
    {16,   16,    0,    0,       0,      0,     0,      0,     2,     64,    24,    7,      0,   16, { 24,  18,  18,  24}},
    {32,   32,    0,    1,      32,     32,     0,      0,    15,     64,    36,    5,      0,   32, {128,  80,  80, 128}},
    {60,   60,    0,    1,      32,     32,     0,      0,    15,     64,    36,    3,      0,   59, {128,  80,  80, 128}}
};

__align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam[Total_ISONum][5]=
{
//  Enh, EthrA, EthrB, EtabA, EtabB
#if 0
    {  400,     12,     512,         0,        2},
    {  360,     16,     512,         1,        2},

    //{  200,     24,     512,         2,        2}, //ISO400~
    {  300,     16,     512,         1,        2},

    {  200,     32,     512,         2,        4},
#endif
    //0312
    {  400,     12,     512,         0,        2},
    {  360,     16,     512,         1,        2},
    {  280,     24,     512,         2,        3},
    {  200,     32,     512,         2,        4},


//  Enh, EthrA, EthrB, EtabA, EtabB
    {  134,      8,     512,         2,        6},
    {  100,      8,     512,         2,        6},
    {   80,     10,     512,         3,        6},
    {   80,     20,     512,         3,        6}
};

__align(4) STR_IPE_EDGE_KER_PARA IPE_AFedgeKernelDiv[11]=
    {  2, 0, 0, 0, -1, 0, 0,  0,  0, 1, 1};

__align(4) STR_IPE_EDGE_KER_PARA IPE_edgeKernelDiv[Total_ISONum][11]=
{
    {  8, 1, -1,  1, -1, 1, -1, -1, -1, 4, 1},
    {  8, 1, -1,  1, -1, 1, -1, -1, -1, 4, 1},
    {  8, 1, -1,  1, -1, 1, -1, -1, -1, 4, 1},
    {  8, 1, -1,  1, -1, 1, -1, -1, -1, 4, 1},

//    { 12, 8, 2, 0, -6, 0, 0, -4, -1, 4, 1},
    { 10, 8, 4, 0, -5, 0, 0, -4, -2, 4, 1},
    { 10, 8, 4, 0, -5, 0, 0, -4, -2, 4, 1},
    { 10, 8, 4, 0, -5, 0, 0, -4, -2, 4, 1},
    { 10, 8, 4, 0, -5, 0, 0, -4, -2, 4, 1}
};

 __align(4) UINT8 IPE_nrStab[Total_ISONum][16]=
{
     {255,255,232,202,170,135,108,69,43,22,13,3,0,0,0,0},
     {255,255,232,202,170,135,108,69,43,22,13,3,0,0,0,0},
     {255,255,232,202,170,135,108,69,43,22,13,3,0,0,0,0},
     {255,255,232,202,170,135,108,69,43,22,13,3,0,0,0,0},

     {255,255,232,202,170,135,108,69,43,22,13,3,0,0,0,0},
     {255,255,240,224,192,160,128,96,64,32,16,8,0,0,0,0},
     {255,255,240,224,192,160,128,96,64,32,16,8,0,0,0,0},
     {255,255,240,224,192,160,128,96,64,32,16,8,0,0,0,0}
};
 __align(4) UINT8 IPE_nrDtab[Total_ISONum][16]=
{
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},

     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0},
     {255,240,224,192,160,128,112,96,80,64,48,32,16,8,4,0}
};
 __align(4) UINT8 IPE_fmStab[Total_ISONum][16]=
{
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},

     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0},
     {128,96,64,32,16,8,4,2,0,0,0,0,0,0,0,0}
};
 __align(4) UINT8 IPE_fmDtab[Total_ISONum][16]=
{
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},

     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0},
     {96,64,32,16,8,4,2,0,0,0,0,0,0,0,0,0}
};
 __align(4) UINT8 IPE_emap16tab[Total_ISONum][16]=
{
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},

     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32},
     {32,64,96,128,160,196,228,255,255,228,196,160,128,96,64,32}
};
  __align(4) UINT8 IPE_emStab[Total_ISONum][16]=
 {
      {96,96,96,96,96,96,96,96,96,96,96,96, 96, 100, 104, 108},
      {96,96,96,96,96,96,96,96,96,96,96,96, 96, 100, 104, 108},
      {96,96,96,96,96,96,96,96,96,96,96,96, 96, 100, 104, 108},
      {96,96,96,96,96,96,96,96,96,96,96,96, 96, 100, 104, 108},

      {18,35,48,69,75,80,84,88,90,97,97,99,101, 99, 88, 73},
      {18,35,48,69,75,80,84,88,90,97,97,99,101, 99, 88, 73},
      {18,35,45,48,54,58,63,67,71,78,82,86, 93, 86, 80, 48},
      {18,35,45,48,54,58,63,67,71,78,82,86, 93, 86, 80, 48}
 };

__align(4) UINT CHR_Param[5][28];

__align(4)INT16 Indenity_CC[9] = {
    0x004D, 0x0096, 0x001D,
    0x07D5, 0x07AB, 0x0080,
    0x0080, 0x0795, 0x07EB
};

__align(4)INT16 Normal_CC[9] = {
    0x004C, 0x0096, 0x001D,
    0x07CC, 0x06F1, 0x0144,
    0x010D, 0x0726, 0x07CE
};


__align(4)UINT32 GammaLUT_128Tab_Video_Normal[130]={
#if 0
  0,  20,  40,  60,  80, 104, 124, 144, 164, 184,
204, 224, 240, 260, 276, 296, 312, 328, 344, 360,
376, 392, 408, 424, 440, 452, 464, 480, 492, 504,
520, 532, 544, 556, 564, 576, 584, 596, 608, 616,
628, 636, 648, 660, 668, 680, 688, 700, 708, 720,
728, 740, 744, 752, 760, 764, 772, 780, 788, 792,
800, 804, 808, 812, 816, 824, 828, 832, 836, 840,
844, 848, 852, 856, 860, 864, 868, 872, 876, 880,
884, 888, 892, 896, 904, 908, 912, 916, 920, 928,
932, 936, 936, 940, 944, 948, 948, 952, 956, 956,
960, 960, 964, 968, 968, 972, 976, 976, 980, 984,
984, 988, 988, 992, 996, 996, 1000, 1000, 1004, 1008,
1008, 1012, 1012, 1016, 1016, 1016, 1020, 1020, 1023, 1023
#endif
#if 0
//V737
0, 16, 36, 52, 72, 92, 112, 136, 164, 196,
228, 260, 284, 312, 328, 340, 356, 368, 380, 392,
404, 416, 428, 444, 456, 468, 480, 492, 504, 516,
528, 536, 548, 556, 564, 572, 580, 592, 600, 612,
620, 628, 636, 640, 648, 656, 664, 672, 680, 692,
700, 708, 716, 724, 732, 740, 748, 756, 764, 768,
776, 784, 788, 796, 800, 804, 808, 812, 816, 824,
828, 836, 840, 844, 848, 852, 856, 864, 868, 872,
876, 880, 884, 888, 896, 900, 904, 908, 912, 920,
924, 928, 928, 932, 936, 940, 940, 944, 948, 948,
952, 952, 956, 960, 960, 964, 968, 968, 972, 976,
976, 980, 980, 984, 988, 988, 996, 996, 1000, 1004,
1004, 1008, 1008, 1012, 1012, 1012, 1016, 1016, 1023,
#endif
#if 1
0, 32, 64, 92, 116, 140, 160, 184, 204, 224,
244, 264, 284, 308, 324, 340, 356, 368, 380, 392,
404, 416, 428, 444, 456, 468, 480, 492, 504, 516,
528, 536, 548, 556, 564, 572, 580, 592, 600, 612,
620, 628, 636, 640, 648, 656, 664, 672, 680, 692,
700, 708, 716, 724, 732, 740, 748, 756, 764, 768,
776, 784, 788, 796, 800, 804, 808, 812, 816, 824,
828, 836, 840, 844, 848, 852, 856, 864, 868, 872,
876, 880, 884, 888, 896, 900, 904, 908, 912, 920,
924, 928, 928, 932, 936, 940, 940, 944, 948, 948,
952, 952, 956, 960, 960, 964, 968, 968, 972, 976,
976, 980, 980, 984, 988, 988, 996, 996, 1000, 1004,
1004, 1008, 1008, 1012, 1012, 1012, 1016, 1016, 1023,
#endif
};


__align(4)UINT32 GammaLUT_128Tab_Video_Night[130]={
  0,  40,  80, 120, 160, 196, 220, 244, 268, 292,
316, 332, 348, 368, 384, 404, 416, 428, 440, 452,
464, 476, 488, 500, 512, 520, 532, 540, 548, 556,
564, 572, 580, 588, 596, 604, 612, 620, 624, 632,
636, 640, 644, 648, 656, 660, 664, 668, 672, 672,
676, 680, 684, 688, 688, 692, 696, 696, 700, 700,
700, 704, 708, 712, 716, 716, 720, 720, 724, 724,
724, 728, 728, 732, 732, 732, 736, 736, 740, 740,
740, 740, 740, 744, 744, 744, 748, 748, 748, 752,
756, 760, 760, 764, 768, 768, 772, 776, 780, 784,
788, 792, 796, 800, 808, 812, 816, 824, 828, 832,
840, 848, 852, 860, 868, 876, 884, 892, 904, 912,
920, 932, 940, 952, 964, 976, 992,1004,1023,1023
};



__align(4)UINT32 GammaLUT_128Tab_Capture[130]={
#if 0
 0 ,32 ,60 ,92 ,119 ,141 ,167 ,194 ,223 ,246 ,
266 ,290 ,307 ,328 ,345 ,363 ,377 ,394 ,408 ,424 ,
440 ,456 ,472 ,488 ,500 ,516 ,528 ,540 ,552 ,568 ,
580 ,588 ,600 ,612 ,624 ,632 ,644 ,656 ,664 ,672 ,
684 ,692 ,704 ,712 ,720 ,728 ,736 ,748 ,756 ,762 ,
768 ,776 ,784 ,792 ,796 ,804 ,812 ,816 ,824 ,832 ,
836 ,844 ,848 ,856 ,860 ,864 ,872 ,876 ,880 ,884 ,
888 ,892 ,896 ,900 ,904 ,907 ,909 ,911 ,913 ,916 ,
919 ,921 ,923 ,925 ,927 ,929 ,931 ,933 ,935 ,937 ,
939 ,941 ,943 ,945 ,947 ,949 ,951 ,953 ,955 ,957 ,
959 ,961 ,963 ,965 ,967 ,969 ,971 ,973 ,975 ,977 ,
979 ,981 ,983 ,985 ,987 ,989 ,991 ,993 ,995 ,997 ,
999 ,1001 ,1004 ,1007 ,1009 ,1011 ,1013 ,1016 ,1023 ,1023
#endif
  0,  20,  40,  60,  80, 104, 124, 144, 164, 184,
204, 224, 240, 260, 276, 296, 312, 328, 344, 360,
376, 392, 408, 424, 440, 452, 464, 480, 492, 504,
520, 532, 544, 556, 564, 576, 584, 596, 608, 616,
628, 636, 648, 660, 668, 680, 688, 700, 708, 720,
728, 740, 744, 752, 760, 764, 772, 780, 788, 792,
800, 804, 808, 812, 816, 824, 828, 832, 836, 840,
844, 848, 852, 856, 860, 864, 868, 872, 876, 880,
884, 888, 892, 896, 904, 908, 912, 916, 920, 928,
932, 936, 936, 940, 944, 948, 948, 952, 956, 956,
960, 960, 964, 968, 968, 972, 976, 976, 980, 984,
984, 988, 988, 992, 996, 996, 1000, 1000, 1004, 1008,
1008, 1012, 1012, 1016, 1016, 1016, 1020, 1020, 1023, 1023
 };


__align(4)UW  ChrCurveLUT[130]={
 0, 8, 16, 28, 36, 44, 52, 64, 72, 80,
88, 100, 108, 116, 124, 136, 144, 152, 160, 172,
180, 188, 196, 204, 216, 224, 232, 240, 248, 260,
268, 276, 284, 292, 300, 312, 320, 328, 336, 344,
352, 360, 368, 376, 388, 396, 404, 412, 420, 428,
436, 444, 452, 460, 468, 476, 484, 492, 496, 504,
508, 512, 512, 512, 512, 512, 512, 512, 516, 524,
532, 540, 548, 556, 564, 576, 584, 592, 600, 608,
620, 628, 636, 644, 656, 664, 672, 680, 688, 696,
708, 716, 724, 732, 740, 748, 756, 768, 776, 784,
792, 800, 808, 816, 824, 832, 840, 852, 860, 868,
876, 884, 892, 900, 908, 916, 924, 932, 940, 948,
960, 968, 976, 984, 992, 1000, 1008, 1016, 1023,
};

__align(4)UH PreGeoLUT[66]={
65535,    65535,    65531,    65524,
65516,    65504,    65490,    65473,
65454,    65433,    65409,    65382,
65353,    65322,    65288,    65252,
65213,    65172,    65129,    65083,
65036,    64986,    64933,    64879,
64823,    64764,    64703,    64641,
64576,    64509,    64441,    64370,
64298,    64224,    64148,    64071,
63992,    63911,    63828,    63744,
63659,    63572,    63484,    63394,
63303,    63210,    63117,    63022,
62925,    62828,    62730,    62630,
62530,    62428,    62326,    62222,
62118,    62013,    61907,    61800,
61692,    61584,    61475,    61365, 61255, 61255};

__align(4) unsigned char Shading_Table_PreF[20] = {
      0, 10, 20, 30,
     40, 50, 60, 70,
     80, 90,100,110,
    120,130,140,150,
    160,0,0,0
};

__align(4) unsigned char Shading_Table[20] = {
      0, 5, 10,15,
     20,25, 30,35,
     40,45, 50,55,
     60,65, 70,75,
     80,0,0,0
 };

//#NT#2009/08/12#Jarkko Chang -begin
//#NT#add for vig update
__align(4) unsigned char Shading_Table_R[20] = {
      0, 5, 10,15,
     20,25, 30,35,
     40,45, 50,55,
     60,65, 70,75,
     80,0,0,0
 };

__align(4) unsigned char Shading_Table_G[20] = {
      0, 5, 10,15,
     20,25, 30,35,
     40,45, 50,55,
     60,65, 70,75,
     80,0,0,0
 };

__align(4) unsigned char Shading_Table_B[20] = {
      0, 5, 10,15,
     20,25, 30,35,
     40,45, 50,55,
     60,65, 70,75,
     80,0,0,0
 };
//#NT#2009/08/12#Jarkko Chang -end

__align(4) unsigned char Shading_Table_Flash[20] = {
      0, 19, 39, 60,
     82,104,127,150,
    174,200,225,243,
    255,255,255,255,
    255,0,0,0
};


__align(4) unsigned char Shading_LineTable[20] = {
      0,0,0,0,
      0,0,0,0,
      0,0,0,0,
      0,0,0,0,
      0,0,0,0,
};

__align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1] = {
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Off,
    {128, 128, 128,   0, 0, 0},    //_IMGEffect_BW,
    {108, 146, 128,   0, 0, 0},    //_IMGEffect_Sepia,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Vivid,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Neutral,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_LowSharping,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Fasion1,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Fasion2,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_CustomManual,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Red,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Green,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Blue,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Yellow,
    {128, 128, 128, 128, 0, 0},    //_IMGEffect_Purple
};

__align(4) UINT32 Saturation[Max_Saturation+1] = {
//_IMGSaturation_P1,    _IMGSaturation_Normal,  _IMGSaturation_N1
                0xe0,     0x0,                  0x20
};

__align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1] = {
//    _IMGSharp_P1,_IMGSharp_Normal,_IMGSharp_N1
    {   0,        0,            0},   //video
    {   0,        0,            0},   //video
    {   0,        0,            0},   //video
    {   0,        0,            0},   //video

    {   0,        0,            0},
    {   0,        0,            0},
    {   0,        0,            0},
    {   0,        0,            0},
};

__align(4) IME_FILT_PARA IMECHROMAPARAM[Total_ISONum] =  {
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE, 5,0x80},
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE,5,0x80},
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE,5,0x80},
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE,5,0x80},

     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE, 5,0x80},
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE,5,0x80},
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE,5,0x80},
     {IMEALG_ENABLE,IMEALG_HAVGSIZE8,63,IMEALG_DISABLE,5,0x80},

};
//#NT#20091209#ethanlau -begin
__align(4) UINT32 CHRFILTPARAM[Total_ISONum][4] = {
    //reference grph.h
    //FLTSIZE,          FLTCOEF, CVMDELTA, CVMCENTER
    {GRPH_CFLT_FLTSIZE_2,0x10   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x10   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x10   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x20   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x10   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x20   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x10   , 0x10    , 0X80},
    {GRPH_CFLT_FLTSIZE_2,0x10   , 0x10    , 0X80}
};
//#NT#20091209#ethanlau -end

//#NT#2010/09/28#ethanlau -begin
//#NT#
__align(4) UINT8 IPE_ddsTab[8] = {
    16,32,32,32,32,32,32,32
};

__align(4) UINT8 IPE_hueTab[24] = {
    128,128,128,128,128,128,128,128,
    128,128,128,128,128,128,128,128,
    128,128,128,128,128,128,128,128
};

__align(4) UINT8 IPE_intTab[24] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0
};

__align(4) UINT8 IPE_satTab[24] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0
};
//#NT#2010/09/28#ethanlau -end
#endif