/**
    Public header file for remote controller module.

    This file is the header file that define the API and data type for remote
    controller module.

    @file       remote.h
    @ingroup    mIIORemote
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _REMOTE_H
#define _REMOTE_H

#include "Type.h"

/**
    @addtogroup mIIORemote
*/
//@{

/**
    Remote controller ISR callback function prototype
*/
typedef void (*ISRCallbackFunc)(UINT32 uiCallID, UINT32 uiParam);

/**
    Remote controller returned pattern structure
*/
typedef struct
{
    UINT32  uiLow;      ///< Low word of received data pattern (bit0 ~ bit31)
    UINT32  uiHigh;     ///< High word of received data pattern (bit32 ~ bit63)
} T_REMOTE_PATTERN;

/**
    Remote controller interrupt status
*/
typedef enum
{
    REMOTE_INT_RD,      ///< Data ready status ID
    REMOTE_INT_ERR,     ///< Timeout error status ID
    REMOTE_INT_WAKEUP,  ///< Wakeup interrupt status ID
    ENUM_DUMMY4WORD(REMOTE_INTSTS_ID)
} REMOTE_INTSTS_ID;

/**
    Remote controller threshold select
*/
typedef enum
{
    REMOTE_SPACE_DET,   ///< Detect the space cycle count as threshold
    REMOTE_PULSE_DET,   ///< Detect the pulse cycle count as threshold
    ENUM_DUMMY4WORD(REMOTE_TH_SEL)
} REMOTE_TH_SEL;

/**
    Remote controller data order
*/
typedef enum
{
    REMOTE_DATA_LSB,    ///< Data bit order is LSB first
    REMOTE_DATA_MSB,    ///< Data bit order is MSB first
    ENUM_DUMMY4WORD(REMOTE_DATA_ORDER)
} REMOTE_DATA_ORDER;

// Remote driver APIs
extern ER remote_open(ISRCallbackFunc pEventHdl);
extern ER remote_close(void);
extern ER remote_setEnable(BOOL bEn);
extern ER remote_setHdrFilterEn(BOOL bEn);
extern ER remote_setRepDetEn(BOOL bEn);
extern ER remote_setRdIntEn(BOOL bEn);
extern ER remote_setErrIntEn(BOOL bEn);
extern ER remote_setWakeupIntEn(BOOL bEn);
extern ER remote_setThresholdSel(REMOTE_TH_SEL sel);
extern ER remote_setInvSignal(BOOL bInv);
extern ER remote_setDataOrder(REMOTE_DATA_ORDER order);
extern ER remote_setGsr(UINT32 count);
extern ER remote_setLogicTh(UINT32 count);
extern ER remote_setDataLen(UINT32 len);
extern ER remote_setWakeupData(UINT32 low, UINT32 high);
extern ER remote_setPowerDownMode(UINT32 uiMode);

//@}

#endif
