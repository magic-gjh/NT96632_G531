/**
    Header file for SIF module.

    This file is the header file that define the API and data type
    for SIF module.

    @file       sif.h
    @ingroup    mIIOSIF
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _SIF_H
#define _SIF_H

#include "Type.h"

/**
    @addtogroup mIIOSIF
*/
//@{

/**
    @name   SIF maximum / minimum value

    Definition for SIF maximum / minimum configuration value
*/
//@{
#define SIF_MAX_SENDUR          0x0F    ///< Maximum SEN duration
#define SIF_MAX_TXSZ            0x60    ///< Maximum transmit size
#define SIF_MIN_TXSZ            0x01    ///< Minimum transmit size
#define SIF_MAX_CLKDIV          0x3FF   ///< Maximum clock divider
#define SIF_MIN_CLKDIV          0x01    ///< Minimum clock divider
#define SIF_MAX_SENH            0x07    ///< Maximum SEN hold time
#define SIF_MAX_SENS            0x0F    ///< Maximum SEN Setup time
#define SIF_MAX_DELAY           0x0F    ///< Maximum SEN Delay time

//@}


/**
    SIF channel ID
*/
typedef enum
{
    SIF_CH0,                ///< SIF channel 0
    SIF_CH1,                ///< SIF channel 1
    SIF_CH2,                ///< SIF channel 2
    SIF_CH3,                ///< SIF channel 3
    SIF_CH4,                ///< SIF channel 4
    SIF_CH5,                ///< SIF channel 5
    ENUM_DUMMY4WORD(SIF_CH)
} SIF_CH;

#define SIF_TOTAL_CHANNEL       6

/**
    SIF sync source ID
*/
typedef enum
{
    SIF_SYNCSRC_CCD_TG,     ///< SIF Sync Source CCD TG
    SIF_SYNCSRC_CCD_AFE,    ///< SIF Sync Source CCD AFE
    SIF_SYNCSRC_LCD,        ///< SIF Sync Source LCD
    ENUM_DUMMY4WORD(SIF_SYNCSRC)
} SIF_SYNCSRC;

/**
    SIF bus mode ID
*/
typedef enum
{
    SIF_MODE_0000,          ///< SIF bus mode 0000
    SIF_MODE_0001,          ///< SIF bus mode 0001
    SIF_MODE_0010,          ///< SIF bus mode 0010
    SIF_MODE_0011,          ///< SIF bus mode 0011
    SIF_MODE_0100,          ///< SIF bus mode 0100
    SIF_MODE_0101,          ///< SIF bus mode 0101
    SIF_MODE_0110,          ///< SIF bus mode 0110
    SIF_MODE_0111,          ///< SIF bus mode 0111
    SIF_MODE_1000,          ///< SIF bus mode 1000
    SIF_MODE_1001,          ///< SIF bus mode 1001
    SIF_MODE_1010,          ///< SIF bus mode 1010
    SIF_MODE_1011,          ///< SIF bus mode 1011
    SIF_MODE_1100,          ///< SIF bus mode 1100
    SIF_MODE_1101,          ///< SIF bus mode 1101
    SIF_MODE_1110,          ///< SIF bus mode 1110
    SIF_MODE_1111,          ///< SIF bus mode 1111
    ENUM_DUMMY4WORD(SIF_MODE)
} SIF_MODE;

/**
    SIF channel setting
*/
typedef struct
{
    SIF_MODE            uiMode;         ///< Bus mode. SIF_MODE_0000 ~ SIF_MODE_1111
    BOOL                bSyncEn;        ///< Enable / Disable sync function
                                        ///< - @b TRUE: Enable
                                        ///< - @b FALSE: Disable
    SIF_SYNCSRC         uiSyncSrc;      ///< Select sync source
                                        ///< @note Value only when bSyncEn = TRUE
    UINT32              uiSenDuration;  ///< SEN duration for SIF_MODE_1000 ~ SIF_MODE_1111
                                        ///< @note Valid value: 0x01 ~ 0x0F
    UINT32              uiLength;       ///< Transmit size
                                        ///< @note Valid value: 0x01 ~ 0x60
    UINT32              uiClkDiv;       ///< Clock divider
                                        ///<
                                        ///< Set SIF bus clock. SIF bus clock = APB clock / (2 * (uiClkDiv + 1))
                                        ///< @note Valid value: 0x001 ~ 0x3FF
    BOOL                bMSB;           ///< MSB or LSB shifted out first
                                        ///< - @b TRUE: MSB shifted out first
                                        ///< - @b FALSE: LSB shifted out first
    UINT32              uiSenH;         ///< SEN hold time, hold for X half SIF bus clock
                                        ///< @note Valid value: 0x00 ~ 0x07

} SIF_CTRL, *PSIF_CTRL;

//SIF driver APIs
extern ER       sif_open(void);
extern ER       sif_close(void);
extern BOOL     sif_isOpened(void);
extern ID       sif_getLockStatus(void);
extern void     sif_setControl(SIF_CH uiCh, PSIF_CTRL pSifCtrl);
extern void     sif_setSens(SIF_CH uiCh,UINT32 uiSens);
extern void     sif_send(SIF_CH uiCh, UINT32 uiData0, UINT32 uiData1, UINT32 uiData2);
extern void     sif_sendMSB(SIF_CH uiCh, UINT32 uiData2);
extern void     sif_sendLSB(SIF_CH uiCh, UINT32 uiData0);
extern void     sif_waitTransferEnd(void);
extern BOOL     sif_sendDMA(SIF_CH uiCh, UINT8 *pTxData, UINT32 uiTxWords);
extern void     sif_setDelay(SIF_CH uiCh,UINT32 uiDelay);
extern void     sif_pauseDMA(void);
extern void     sif_resumeDMA(void);
extern BOOL     sif_waitDMATransferEnd(void);

//@}

#endif
