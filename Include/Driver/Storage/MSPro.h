/**
    MS / MS Pro Card driver header file.

    @file       MSPro.h
    @ingroup    mICardMS
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _MSPro_H
#define _MSPro_H

#include "StrgDef.h"

/**
    @addtogroup mICardMS
*/
//@{

/**
    @name MS power control function prototype


*/
//@{
typedef void    (*MS_POWER_CTRL_HDLFP)(BOOL);   ///< power control function prototype
//@}

/**
    @name MS card insert/protect detection function prototype

    MS card insert/protect detection call back prototype
*/
//@{
typedef BOOL    (*MS_DETECT_HDL)(void);         ///< card insert/protect detection function prototype
//@}

/**
    MS initialization infomation

    @note For mspro_init()
*/
typedef struct
{
    MS_DETECT_HDL       pChkInsertHdl;          ///< MS card insert detection handler
    MS_DETECT_HDL       pChkProtectHdl;         ///< MS card write-protect detection handler
    FP                  pInitPadHdl;            ///< MS card IO pad init handler
    MS_POWER_CTRL_HDLFP pPowerCtrlHdl;          ///< MS card power control handler
} MS_INIT_INFO, *PMS_INIT_INFO;

//------------------------------------------------------------
// The general api for the MS/MS_Pro device driver
//------------------------------------------------------------
/** Exported object for filesystem. */
//extern STRG_TAB gMSProObj;

/*
    Public OS level functions
*/
extern void         mspro_attach(void);
extern void         mspro_detach(void);
extern void         mspro_init(PMS_INIT_INFO pInitInfo);
extern ER           mspro_open(void);
extern ER           mspro_close(void);
extern BOOL         mspro_isOpened(void);
extern ER           mspro_lock(void);
extern ER           mspro_unlock(void);

/*
    Public driver specific operating functions
*/
//extern ER mspro_writeSectors(CHAR *pcBuf, ULONG ulLBAddr, ULONG ulSctCnt);
//extern ER mspro_readSectors(CHAR *pcBuf, ULONG ulLBAddr, ULONG ulSctCnt);
//extern ER           mspro_format(void);
extern void         mspro_setFormatType(int ftype);

extern PSTRG_TAB    mspro_getStorageObject(void);

//
// Customized API
//

/*
    Enable card power.

    @param bOn TRUE=turn on card power, FALSE=turn off card power
*/
//void MSCardUser_EnableCardPower(BOOL bOn);

/*
    Check card insertion state.

    @return TRUE=card inserted, FALSE=card removed
*/
//BOOL MSCardUser_CheckCardInserted(void);

/*
    Initialize card I/O pad.
*/
//void MSCardUser_InitCardIO(void);

//@}

#endif //_MSPro_H
