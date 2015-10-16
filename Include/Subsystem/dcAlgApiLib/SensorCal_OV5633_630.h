#ifndef _SENSORCAL_OV5653_630_H
#define _SENSORCAL_OV5653_630_H

#include "Type.h"
#include "NAND.h"
#include "sie_lib.h"
#include "pre_lib.h"
#include "aaa_lib.h"

//#NT#2009/09/18#Jarkko Chang -begin
//Exposure Control
#define AgcGain_ISO100  130
#define AgcGain_min     19
#define AgcGain_max     49
#define Agc_ISO50       (1500)
#define Max_AE_Idx      49
#define MaxExpLine_640x480      648
#define MaxExpLine_1280x480     485
#define MaxExpLine_1280x720     735
#define MaxExpLine_2560x960     971

#define MaxCapExpLine   1970
#define MinExpLine      2
#define MinCapExpLine   2
//#define Default_Zoom_Stop2PR        (78)//JJ0229
//#NT#2009/09/18#Jarkko Chang -end

#define SensorCapTotalFld       1
#define Field_Num1              0
#define Field_Num2              0
#define Field_Num3              0
#define Field_Num4              0
#define Field_Num5              0
#define Field_Num6              0
#define Field_Num7              0
#define Field_Num8              0

//#NT#2008/05/13#Adam Hsu -begin
#define IMAGE_SIZE_H            2576
#define IMAGE_SIZE_V            1940
//#NT#2008/05/13#Adam Hsu -end

//SIE Datasheet
#define PixelClk 24 //MHz
#define PixelClkInKHz 24000 //MHz


extern void aaa_SetColorgain(UB mode,UH R, UH Gr, UH Gb, UH B);
extern void ipe_SetGamma(unsigned char color,unsigned char *gamma17tbl);
extern void sie_CaptureSet(UW CapAdd, UH CapCenterH, UH CapCenterV,UH ImageHSize, UH ImageVSize);
extern void SetPreExpLine(INT Expline);
extern void SetCapExpLine(UINT Expline);
extern void SensorIF_SetOB(BOOL Auto_OB, unsigned short OB);
extern void AeSet(void);
extern void sie_Set2Capture(UINT32 CapAdd, UINT16 CapCenterH, UINT16 CapCenterV, UINT16 ImageHSize, UINT16 ImageVSize);
extern void sie_Set2Preview(UINT32 PreviewCenterH, UINT32 PreviewCenterV, UINT32 Sie_InHsize, UINT32 Sie_InVsize,
                                UINT32 Sie_OutHsize, BOOL ToDram, UINT D_Addr);

extern void sie_Cap2Preview(void);
extern void Make_AF_Table(void);
extern void flash_PreFlashFlow(void);
extern void Get_Sensor_CalInfor(void);
extern void Time2_Line_Pixel2(unsigned long Time, unsigned long PixelClock, unsigned short *M_Line, unsigned short *M_Pixel2);
extern UINT flash_Charge(void);
extern int sprintf(char * /*s*/, const char * /*format*/, ...);
extern INT8 AwbCheckWhite(UINT R,UINT G,UINT B);
extern unsigned int EstimateY(void);
extern ER WriteCalData(void);
extern ER ReadCalData(void);
extern UB ISOAdjust(void);
extern UB OBAdjust(void);
extern UB LensShading_Adjust(void);
extern UB MshutterAdjust(void);
extern UB DP_Adjust(void);
extern UB AWB_Adjust(void);
extern UB FlashlightAEAdjust(void);
extern UB FarFocusAdjust(void);
extern UB NearFocusAdjust(void);
extern UB WriteAdjustData2File(char state);
extern UB ReadCalStatus(void);
extern void WaitAeConverge(void);
extern unsigned int GetSieInHsize(void);
extern unsigned int GetSieInVsize(void);
extern int TimerDelayMs(unsigned long ms);
extern unsigned int GetPreImeOutHsize(void);
extern unsigned int GetPreImeOutVsize(void);
extern unsigned int GetPreImeOutOffset(void);
extern unsigned int GetSieXcenter(void);
extern unsigned int GetSieYcenter(void);


void SetRBgain_White(void);
void SetRBgain_Dark(void);
void CalSetExpTimeIn1HD(UINT32 ExposureTime);
void CalSetExpTime(UINT32 Line);
void CalSetExpTime2(UINT32 Line, UINT32 Pixel);
void CalSetAgc(UINT32 Agc);
void CalAeSet(void);
void CalAePrv2Cap(UINT32 ExpTime,UINT32 ExpTimePxl,UINT32 AgcGain);
UINT8 CalGet_ExpoFrame(void);
void CalSie_CaptureSet(UW CapAdd, UH CapCenterH, UH CapCenterV,UH ImageHSize, UH ImageVSize);
void CalSie_Set2Capture(UINT32 CapAdd, UINT16 CapCenterH, UINT16 CapCenterV, UINT16 ImageHSize, UINT16 ImageVSize, UINT8 SiePath, BOOL autoOB);
void CalSie_Set2Preview(UINT32 PreviewCenterH, UINT32 PreviewCenterV, UINT32 Sie_InHsize,UINT32 Sie_InVsize, UINT32 Sie_OutHsize, BOOL ToDram, UINT D_Addr);

extern void Set_ZoomBoundaryAutoCheck(BOOL en);
extern BOOL Lens_CheckFocusPI(void);
extern void FocusPI_OnOff(unsigned char on_off);
#endif

