/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2004.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: AAA Library Module                                                 *
* Description:                                                             *
* Author: YuppieLin                                                        *
****************************************************************************/

/** \addtogroup mIIPPAAA */
//@{

#ifndef _AAA_LIB433_H
#define _AAA_LIB433_H
#include "Type.h"
//#NT#2009/10/01#JJ Huang -begin
#include "pre_lib.h"
//#NT#2009/10/01#JJ Huang -end

#define AAA_FLAG_AE_ENABLE                  (0x00000001)
#define AAA_FLAG_AF_ENABLE                  (0x00000001<<1)
#define AAA_FLAG_AWB_ENABLE                 (0x00000001<<2)
#define AAA_FLAG_AE_CALIBRATION             (0x00000001<<3)
#define AAA_FLAG_AF_CALIBRATION             (0x00000001<<4)
#define AAA_FLAG_AWB_CALIBRATION            (0x00000001<<5)
#define AAA_FLAG_FLASH_CALIBRATION          (0x00000001<<6)
#define AAA_FLAG_MSH_CALIBRATION            (0x00000001<<7)
#define AAA_FLAG_OB_CALIBRATION             (0x00000001<<8)
#define AAA_FLAG_VIG_CALIBRATION            (0x00000001<<9)
#define AAA_FLAG_FD_ENABLE                  (0x00000001<<10)
//#NT#2008/08/28#Yuppie -begin
//#NT#Add SD process
#define AAA_FLAG_SD_ENABLE                  (0x00000001<<11)
//#NT#2008/08/28#Yuppie -end
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
#define AAA_FLAG_DIS_ENABLE                  (0x00000001<<12)
//#NT#2009/11/27#Connie Yu -end
#define AE_LOCK         (uiAAAFlags &= (~AAA_FLAG_AE_ENABLE))
#define AE_UNLOCK       (uiAAAFlags |=   AAA_FLAG_AE_ENABLE)
#define AF_UNLOCK       (uiAAAFlags |=   AAA_FLAG_AF_ENABLE)
#define AF_LOCK         (uiAAAFlags &= (~AAA_FLAG_AF_ENABLE))
#define AWB_UNLOCK      (uiAAAFlags |=   AAA_FLAG_AWB_ENABLE)
#define AWB_LOCK        (uiAAAFlags &= (~AAA_FLAG_AWB_ENABLE))
#define FD_LOCK         (uiAAAFlags &= (~AAA_FLAG_FD_ENABLE))
#define FD_UNLOCK       (uiAAAFlags |=   AAA_FLAG_FD_ENABLE)
//#NT#2008/08/28#Yuppie -begin
//#NT#Add SD process
#define SD_LOCK         (uiAAAFlags &= (~AAA_FLAG_SD_ENABLE))
#define SD_UNLOCK       (uiAAAFlags |=   AAA_FLAG_SD_ENABLE)
//#NT#2008/08/28#Yuppie -end
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
#define DIS_LOCK        (uiAAAFlags &= (~AAA_FLAG_DIS_ENABLE))
#define DIS_UNLOCK      (uiAAAFlags |=   AAA_FLAG_DIS_ENABLE)
//#NT#2009/11/27#Connie Yu -end
#define AE_CAL_ON       (uiAAAFlags |=   AAA_FLAG_AE_CALIBRATION)
#define AE_CAL_OFF      (uiAAAFlags &= (~AAA_FLAG_AE_CALIBRATION))
#define AF_CAL_ON       (uiAAAFlags |=   AAA_FLAG_AF_CALIBRATION)
#define AF_CAL_OFF      (uiAAAFlags &= (~AAA_FLAG_AF_CALIBRATION))
#define AWB_CAL_ON      (uiAAAFlags |=   AAA_FLAG_AWB_CALIBRATION)
#define AWB_CAL_OFF     (uiAAAFlags &= (~AAA_FLAG_AWB_CALIBRATION))
#define FLASH_CAL_ON    (uiAAAFlags |=   AAA_FLAG_FLASH_CALIBRATION)
#define FLASH_CAL_OFF   (uiAAAFlags &= (~AAA_FLAG_FLASH_CALIBRATION))
#define MSH_CAL_ON      (uiAAAFlags |=   AAA_FLAG_MSH_CALIBRATION)
#define MSH_CAL_OFF     (uiAAAFlags &= (~AAA_FLAG_MSH_CALIBRATION))
#define OB_CAL_ON       (uiAAAFlags |=   AAA_FLAG_OB_CALIBRATION)
#define OB_CAL_OFF      (uiAAAFlags &= (~AAA_FLAG_OB_CALIBRATION))
#define VIG_CAL_ON      (uiAAAFlags |=   AAA_FLAG_VIG_CALIBRATION)
#define VIG_CAL_OFF     (uiAAAFlags &= (~AAA_FLAG_VIG_CALIBRATION))

#define LCD_Y(Yaddr,offset,i,j)  *(volatile unsigned int *)(Yaddr+(offset*(j))+4*(i/4))

#define CA_WINDOW_X             8
#define CA_WINDOW_Y             8
#define CA_Window_Num           (CA_WINDOW_X*CA_WINDOW_Y)
#define VA_WINDOW_X             3
#define VA_WINDOW_Y             3
#define VA_Window_Num           (VA_WINDOW_X*VA_WINDOW_Y)

#define AWBScaleBit             8
#define AFScaleBit              8
#define LOCK                    1
#define UNLOCK                  0

//channel define for aaa_EstimateY()
#define CA_CHANNEL_R            0
#define CA_CHANNEL_G            1
#define CA_CHANNEL_B            2
#define CA_CHANNEL_Y            3

#define CA_SOURCE_PRECG         0
#define CA_SOURCE_POSTCG        1
#define CA_SOURCE_POSTGAMMA     2

#define VA_SOURCE_EVAR          0
#define VA_SOURCE_DIFF          1
#define VA_SOURCE_EEXT          2

#define STARTPIXEL_R            0x1b//R Gr Gb B : 8bit to present RGrGbB order 00 01 10 11 = 0x1b
#define STARTPIXEL_Gr           0x4e//Gr R B Gb : 8bit to present RGrGbB order
#define STARTPIXEL_Gb           0xb1
#define STARTPIXEL_B            0xe4

// define the color accumulation structure
typedef struct _STR_AAA_CA_PARA{
    UINT32 DramAddr;
    UINT32 DramLofs;
    UINT32 ImgHsize;
    UINT32 ImgVsize;
    UINT32 *R_Data;
    UINT32 *G_Data;
    UINT32 *B_Data;
    UINT32 Sub_rate;
    UINT32 Win_x;
    UINT32 Win_y;
    UINT32 Cfapat;
}STR_AAA_CA_PARA;
UINT32  aaa_BlockAverageY(UINT32 YAddr,UINT32 x_start,UINT32 y_start,UINT32 width,UINT32 height,UINT32 offset);
void    SetAwbScale(UINT32 Scale);
UINT32  GetAwbScale(void);
void    SetAfScale(UINT32 Scale);
UINT32  GetAfScale(void);

extern UINT32 regCFA_Pattern;
extern UINT32 VA_DRAM_Addr;
extern UINT32 VAOutputSel;
//#NT#2008/08/01#Lawrence Ho -begin
//#NT#New1  extern variable for set different H & V weight
extern UINT32 VAWeightBit;
extern UINT32 Weight_H;
//#NT#2008/08/01#Lawrence Ho -end
//#NT#2009/03/27#Yuppie -begin
//#NT#FD stability
extern UINT32  gFDcount;
//#NT#2009/03/27#Yuppie -end
// -------------------------------------------------
// The general api for AAA driver
//
extern void     aaa_InitISP(void);
// ---- AE ----
extern void     aaa_SetCA(UINT8 source, UINT16 stx, UINT16 sty, UINT16 hsize, UINT16 vsize);
extern void     aaa_Read_HIST_RSLT(UINT32 *Hist);
extern void     aaa_SetAeWindow(UINT8 *Window);
//#NT#2010/09/23#ethanlau -begin
extern UINT8*   aaa_GetAeWindow(void);
//#NT#2010/09/23#ethanlau -end
extern UINT32   aaa_EstimateY2(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset);//Estimate Y at IME output;
extern UINT32   aaa_EstimateY_Spot(UINT32 YAddr,UINT32 h_size,UINT32 v_size,UINT32 offset);//Estimate Y at IME output
extern void     aaa_SetAeRate(UINT8 rate);
extern UINT8    aaa_GetAeRate(void);
extern void     aaa_AeLock(UINT8 lock);
extern BOOL     Is_AELock(void);
//#NT#2009/12/18#YC Peng -begin
//#NT#Add for flash flow
extern UINT8    Block_Avg[CA_WINDOW_X][CA_WINDOW_Y];
//#NT#2009/12/18#YC Peng -end

// ---- AF ----
extern void aaa_InitAF(UINT16 Hsize, UINT16 Vsize);
extern void     aaa_SetVA(UINT8 source, UINT16 stx, UINT16 sty, UINT16 hsize, UINT16 vsize);
extern UINT32   aaa_GetVariation(UINT8 i);
//#NT#2009/10/01#JJ Huang -begin
extern void aaa_GetVaccValue(UINT32 *VaccValue);
extern void aaa_GetVaccNum(UINT32 *VaccNum);
//#NT#2009/10/01#JJ Huang -end
extern void     aaa_AfLock(UINT8 lock);
extern BOOL     Is_AFLock(void);

// ---- AWB ----
extern void     aaa_SetAwbRate(UINT8 rate);
extern UINT8    aaa_GetAwbRate(void);
extern void     aaa_SetColorgain(UINT8 mode,UINT16 R, UINT16 Gr, UINT16 Gb, UINT16 B);//mode 0:config at sie, 1:config at ipe
extern void     aaa_SetIPEColorgain(UINT16 R, UINT16 Gr, UINT16 Gb, UINT16 B);
//#NT#2009/10/16#Jarkko Chang -begin
extern void aaa_SetPREColorgain(UINT16 R_Gain, UINT16 Gr_Gain, UINT16 Gb_Gain, UINT16 B_Gain);
//#NT#2009/10/16#Jarkko Chang -end
//#NT#2008/10/02#Yuppie Lin -begin
//#NT#remove no use API
//extern void     aaa_GetColorgain(UINT8 pattern, UINT16 *CG);
//extern void     aaa_WBModify(UINT8 pattern,UINT16 Rin, UINT16 GRin, UINT16 GBin, UINT16 Bin);//additional modify WB manually
//#NT#2008/10/02#Yuppie Lin -end
extern void     aaa_AwbLock(UINT8 lock);
extern BOOL     Is_AWBLock(void);
extern void     aaa_Get_CA_RSLT(STR_AAA_CA_PARA * pCA_PARA);
//#NT#2009/10/01#JJ Huang -begin
extern void aaa_GetCAResult(STR_AAA_CA_PARA * pCA_PARA, UINT32 uiBitDepth, PRE_VGAFormatEnum Format);
//#NT#2009/10/01#JJ Huang -end
extern void aaa_SetCAsub(UINT32 outSA, UINT16 outLineOffs, UINT8 mode, UINT8 smpH, UINT8 smpV, UINT8 osrc, UINT16 hcnt, UINT16 vcnt);

// ---- FD ----
//#NT#2009/03/27#Yuppie -begin
//#NT#FD stability
extern void fde_FdLockCap(BOOL lock);
extern BOOL fde_IsFdLockCap(void);
//#NT#2009/03/27#Yuppie -end
//#NT#2008/09/19#Yuppie -begin
//#NT#add to prevent dzoom FD fail
extern void     aaa_FdReset(BOOL reset);
extern BOOL     aaa_GetFdReset(void);
//#NT#2008/09/19#Yuppie -end
extern void     aaa_SetFdRate(UINT8 rate);
extern UINT8    aaa_GetFdRate(void);
extern void     aaa_FdLock(UINT8 lock);
extern BOOL     Is_FDLock(void);
//#NT#2008/08/28#Yuppie -begin
//#NT#Add SD process
// ---- SD ----
extern void     aaa_SetSdRate(UINT8 rate);
extern UINT8    aaa_GetSdRate(void);
extern void     aaa_SdLock(UINT8 lock);
extern BOOL     Is_SDLock(void);
//#NT#2008/08/28#Yuppie -end
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
// ---- DIS ----
extern void     aaa_DisLock(UINT8 lock);
extern BOOL     aaa_IsDisLock(void);
//#NT#2009/11/27#Connie Yu -end
// ---- Calibration ----
extern void     aaa_SetAeCalibration(UINT8 cal);
extern UINT32   aaa_GetAeCalibration(void);
extern void     aaa_SetAfCalibration(UINT8 cal);
extern UINT32   aaa_GetAfCalibration(void);
extern void     aaa_SetAwbCalibration(UINT8 cal);
extern UINT32   aaa_GetAwbCalibration(void);
extern void     aaa_SetFlashCalibration(UINT8 cal);
extern UINT32   aaa_GetFlashCalibration(void);
extern void     aaa_SetMshCalibration(UINT8 cal);
extern UINT32   aaa_GetMshCalibration(void);
extern void     aaa_SetOBCalibration(UINT8 cal);
extern UINT32   aaa_GetOBCalibration(void);
extern void     aaa_SetVIGCalibration(UINT8 cal);
extern UINT32   aaa_GetVIGCalibration(void);

// ---- Option ----
extern void     aaa_Modify_CA_NormFactor(UINT8 factor);
extern UINT8    aaa_Get_AE_BlockValue(UINT8 i, UINT8 j);

extern void aaa_VAOutputSel(UINT32 VAOutputType, UINT32 DramOutAddr);
//#NT#2008/08/01#Lawrence Ho -begin
//#NT#New2 Customer require H and V with different weight
extern void aaa_SetVAWeight(UINT32 Total_VAWeightBit, UINT32 Hor_VAWeight);
//#NT#2008/08/01#Lawrence Ho -end

extern UINT32 aaa_MotionDetect(UINT8 *Block_Avg_Now, UINT8 Yavg, UINT8 WinNumX, UINT8 WinNumY);
#endif
//@}