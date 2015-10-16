
#ifndef _CAL_DUMMY_630_H_
#define _CAL_DUMMY_630_H_

/*-----------------------------------*/

#define Default_Agc_ISO50          1500
#define Default_Mshutter            27283//JJ0215 26752//JJ0201 27283//26900
#define Default_Mshutter2           28591//JJ0215 27950//JJ0201 28591//27900
#define Default_AWB_Rgain           410//Jarkko0917
#define Default_AWB_Bgain           325//Jarkko0917
#define Default_Flash_Y_Avg         70
#define Default_Flash_AWB_Rgain     407//Jarkko0917
#define Default_Flash_AWB_Bgain     225//Jarkko

#define Default_Agc_ISOMin          0
#define Default_FlashTriggerPoint   188562  //Lv8.5
#define Default_Min_OB              0x50
#define Default_FlashAgc_Prv        100
#define Default_FlashAgc_Cap        100
#define Default_Shading_Gain        1000
#define Default_FFidx               (156-40)

/*---------Defect Pixel define---------------------*/
//Need not to change
#define Prv_Offset_X            1
#define Prv_Offset_Y            2

//Can modify if nessecary
#define Max_Continue_DP_Cap     4
#define Max_Continue_DP_Prv     2
#define Max_Continue_DP_Dzoom   2

#define Bright_Limit_Prv    25//30//15//15//30//35
#define Bright_Limit_Cap    40//55//JJ0311 110//24//24//30//35
#define Dark_Limit_Prv      20//30//10//15//30//35
#define Dark_Limit_Cap      25//40//15//24//30//35

#define Block_X_Prv         16
#define Block_Y_Prv         4
#define Block_X_Cap         32
#define Block_Y_Cap         32

//Have to modify if change sensor
#define SkipXL_Prv          0//2   // Skip the pixels in left
#define SkipXR_Prv        964// Skip the pixels in right
#define SkipYU_Prv          0//2   // Skip the pixels in top
#define SkipYD_Prv          3//2  // Skip the pixels in button

#define SkipXL_Prv2          0//2   // Skip the pixels in left
#define SkipXR_Prv2        840// Skip the pixels in right
#define SkipYU_Prv2          0//2   // Skip the pixels in top
#define SkipYD_Prv2        295// Skip the pixels in button

#define SkipXL_Prv3          0//2   // Skip the pixels in left
#define SkipXR_Prv3       2320// Skip the pixels in right
#define SkipYU_Prv3          0//2   // Skip the pixels in top
#define SkipYD_Prv3         14// Skip the pixels in button

#define SkipXL_Cap          0
#define SkipXR_Cap          0//2
#define SkipYU_Cap          0//JJ_Temp_For_AO //2
#define SkipYD_Cap          0
#define SkipYU_Field_Cap    1
#define SkipYD_Field_Cap    1
//PS: 必須為2之倍數

#define DP_Prv_StartAdd     0x80ff0000
#define DP_Cap_StartAdd     0x80ff1000

/*--------AWB define----------*/
//Golden-Sample value:
#define WB_COLOR_CALB_RED          222//JJ0201 211
#define WB_COLOR_CALB_GREEN        256
#define WB_COLOR_CALB_BLUE         104//JJ0201 114
#define Rgain_Min           256
#define Rgain_Max           1024
#define Bgain_Min           256
#define Bgain_Max           1024

//Golden-Sample value:
#define FLASH_WB_COLOR_CALB_RED          109//JJ0217 111//JJ0201 211
#define FLASH_WB_COLOR_CALB_GREEN        256
#define FLASH_WB_COLOR_CALB_BLUE         196//JJ0217 199//JJ0201 114
#define Flash_Rgain_Min           256
#define Flash_Rgain_Max           1024
#define Flash_Bgain_Min           256
#define Flash_Bgain_Max           1024

/*------------Flash define---------*/
#define FTP_Min         66667   //LV8
#define FTP_Max         266667  //LV10

#define FlashYAvgTarget 120      //實驗室灰牆
#define FLashYAvgMargin 20

/*------------ISO define-----------*/
#define DGain_ISO50_Min    70
#define DGain_ISO50_Max    150
//#NT#2007/04/17#YC Peng -begin
//#modify ISO max/min
#define Agc_ISO50_Min      (Default_Agc_ISO50)//JJ0228
#define Agc_ISO50_Max      (Default_Agc_ISO50<<3)
//#NT#2007/04/17#YC Peng -end
#define DGmin_Min           1
#define DGmin_Max           1000

/*---------Mshutter define-----------*/
#define Mshutter_Min        (Default_Mshutter-1000)//JJ0201 26800//106096
#define Mshutter_Max        (Default_Mshutter2+1000)//JJ0201 29100//107000

/*------------OB define------------*/
#define OB_DFP_Threshold    30
#define OB_Threshold        32

//#NT#2007/04/17#YC Peng -begin
//#modify lens shading define
/*---------Lens Shading define------------*/
#define Shading_Gain_Max    150
#define VigGain_Max         255
#define VigXcentRat_Min     90
#define VigXcentRat_Max     110//100
#define VigYcentRat_Min     90
#define VigYcentRat_Max     110//100
#define VigCntrConr_Th      55
#define VigConrConr_Th      15
//#NT#2007/04/17#YC Peng -end



extern UINT CalculateRawY(UINT Add, UINT StartX,UINT StartY, UINT SizeX, UINT SizeY,UINT sample,unsigned char StartPixel,unsigned short *R,unsigned short *G,unsigned short *B);
extern UINT CalculateCAsub(USHORT *R_acc,USHORT *G_acc,USHORT *B_acc,USHORT *acc_num);
extern UINT8 MshutterAdjust(void);
extern UINT8 MshutterLasting(void);
extern UINT8 OBAdjust(void);
extern UINT8 LensShading_Adjust(void);
extern UINT8 ISOAdjust(void);
extern UINT8 FarFocusAdjust(void);
extern UINT8 NearFocusAdjust(void);
extern UINT8 AWB_Adjust(void);
extern UINT8 DP_Adjust(void);
extern UINT8 DP_Adjust_White(void);
extern UINT8 DP_Adjust_Dark(void);
extern UINT8 FlashlightAEAdjust(void);
extern UINT8 WriteAdjustData2File(INT8 state);
extern UINT8 ReadCalStatus(void);
extern void CalSetSiePath(UINT8 path);
extern void LT_UIFunction(void);
extern void Get_Sensor_CalInfor(void);
extern void DFP_DarkAEPrv(void);
extern void DFP_DarkAECap(void);

extern UINT8 MshutterSwitchTest(void);
extern UINT8 IrisSwitchTest(void);
extern UINT8 AFETest(void);
extern UINT8 ZoomSpeedTest(void);
extern UINT8 ZoomStepMissTest(void);
extern UINT8 ZoomPRAdjust(void);//JJ0228
extern UINT8 FocusStepMissTest(void);
extern UINT8 AFTest(void);
extern UINT8 StainTest(void);
extern UINT8 TVResolutionTest(void);
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
extern UINT32 SNRTest(void);
//#NT#2010/12/15#JJ Huang -end

#endif

