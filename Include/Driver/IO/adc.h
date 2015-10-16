/**

    Header file for ADC module.

    This file is the header file that define the API for ADC module.

    @file       adc.h
    @ingroup    mIIOADC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/



#ifndef _ADC_H
#define _ADC_H

#include "Type.h"


/**
    @addtogroup mIIOADC
*/
//@{


/**
    @name   ADC channel

    Definition for ADC channel
*/
//@{
#define ADC_CHANNEL_0           0             ///< ADC channel 0
#define ADC_CHANNEL_1           1             ///< ADC channel 1
#define ADC_CHANNEL_2           2             ///< ADC channel 2
#define ADC_CHANNEL_3           3             ///< ADC channel 3
#define ADC_CHANNEL_4           4             ///< ADC channel 0
#define ADC_CHANNEL_5           5             ///< ADC channel 1
#define ADC_CHANNEL_6           6             ///< ADC channel 2
#define ADC_CHANNEL_7           7             ///< ADC channel 3
//@}


#define ADC_CLOCK               6000000       // 6MHz

/**
    @name   ADC sampling divider range

    Definition for ADC sampling divider range
*/
//@{
#define ADC_SAMPDIV_MIN         0x0           ///< Minimum sampling divider value
#define ADC_SAMPDIV_MAX         0xF           ///< Maximum sampling divider value
//@}

/**
    @name   ADC channel sampling divider range

    Definition for ADC channel sampling divider range
*/
//@{
#define ADC_CHSAMPDIV_MIN       0x0           ///< Minimum channel sampling divider value
#define ADC_CHSAMPDIV_MAX       0xFF          ///< Maximum channel sampling divider value
//@}

/**
    @name   ADC channel external sampling counts

    Definition for ADC channel external sampling counts range
*/
//@{
#define ADC_EXTCNT_MIN          0x0           ///< Minimum channel external sampling counts value
#define ADC_EXTCNT_MAX          0xF           ///< Maximum channel external sampling counts value
//@}


/**
    @name ADC sampling times every sequence

    Definition ADC sampling times every sequence
*/
//@{
typedef enum
{
    ADC_SAMPAVG_1,
    ADC_SAMPAVG_2,
    ADC_SAMPAVG_4,
    ENUM_DUMMY4WORD(ADC_SAMPLEAVG)
} ADC_SAMPLEAVG;
//@}




/**
    @name   TP internal resistor range

    Definition for internal resistor range
*/
//@{
#define TP_INTRES_MIN         0x0            ///< Minimum internal resistor value
#define TP_INTRES_MAX         0x3F           ///< Maximum internal resistor value
//@}


/**
    @name   TP sampling cycle range

    Definition for TP sampling cycle range
*/
//@{
#define TP_SAMPCYC_MIN         0x0            ///< Minimum sampling cycle value
#define TP_SAMPCYC_MAX         0xFF           ///< Maximum sampling cycle value
//@}


/**
    @name   TP pen-down gsr range

    Definition for TP pen-down gsr range
*/
//@{
#define TP_PDGSR_MIN         0x0            ///< Minimum pen-down gsr value
#define TP_PDGSR_MAX         0xFF           ///< Maximum pen-down gsr value
//@}


/**
    @name TP operation mode

    Definition for TP operation
*/
//@{
typedef enum
{
    TP_MODE0,
    TP_MODE1,
    TP_MODE2,
    TP_MODE3,
    ENUM_DUMMY4WORD(TP_OPMODE)
} TP_OPMODE;
//@}

/**
    @name TP measurement mode

    Definition for TP measurement mode
*/
//@{
typedef enum
{
    TP_PD,
    TP_POS,
    TP_PRE,
    ENUM_DUMMY4WORD(TP_MSMODE)
} TP_MSMODE;
//@}


#define KEYSCAN_TP_PRESSURE_TH      245     // 255 - 10


// API

extern ER       adc_open(UINT32 uiChannel);
extern ER       adc_close(UINT32 uiChannel);
extern ID       adc_getLockStatus(UINT32 uiChannel);

extern void     adc_setControl(UINT32 uiSampDiv, BOOL bSeq);
extern void     adc_getControl(UINT32 *uiSampDiv, BOOL *bSeq);

extern void     adc_setMode(UINT32 uiChannel, BOOL bContMode, BOOL bIntEn, FP Isr);
extern void     adc_getMode(UINT32 uiChannel, BOOL *bContMode, BOOL *bIntEn);
extern void     adc_setChDivider(UINT32 uiChannel, UINT32 uiChSampDiv);
extern void     adc_setExtraSampCnt(UINT32 uiExtCnt);
extern void     adc_setSampleAvg(ADC_SAMPLEAVG SampAvg);

extern void     adc_enable(void);
extern void     adc_disable(void);
extern BOOL     adc_isEnabled(void);

extern void     adc_triggerOneShot(UINT32 uiChannel);

extern UINT32   adc_readData(UINT32 uiChannel);
extern BOOL     adc_isDataReady(UINT32 uiChannel);
extern void     adc_clearDataReady(UINT32 uiChannel);

extern UINT32   adc_readVoltage(UINT32 uiChannel);

extern void     adc_setTPControl(UINT32 uiGsr, UINT32 uiRes, UINT32 uiSampCyc);
//#NT#2011/01/25#Cliff Lin -begin
//#NT#modified for touch controller
extern BOOL     adc_setTPMode(TP_OPMODE Opmode, TP_MSMODE Msmode, BOOL bIntEn, FP TPIsr);
//#NT#2011/01/25#Cliff Lin -end
extern void     adc_startTP(void);
extern void     adc_stopTP(void);
extern void     adc_clearTPDataReady(void);
extern void     adc_getValidTPXY(UINT16 *puiX, UINT16 *puiY);
extern void     adc_readTPPressure(UINT16 *puiP1, UINT16 *puiP2);
extern void     adc_readTPXY(UINT16 *puiX, UINT16 *puiY);
extern void     adc_initTP(void *pSetting);
extern UINT32   adc_detTPGesture(UINT8 *pData);
//#NT#2011/01/25#Cliff Lin -begin
//#NT#modified for touch controller
extern BOOL     adc_isTPDataReady(void);
extern BOOL     adc_isTPPD(void);
extern void     adc_clearTPPDSts(void);
//#NT#2011/01/25#Cliff Lin -end

#endif

//@}
