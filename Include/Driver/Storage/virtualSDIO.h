/**
    Header file for virtual SDIO module driver

    @file       virtualSDIO.h
    @ingroup    mICardSDIO

    @brief      Virtual SDIO Card driver header

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#ifndef _VSDIO_H
#define _VSDIO_H

#include "StrgDef.h"
#include "SdmmcDesc.h"

/**
    @addtogroup mICardSDIO
*/
//@{

/**
    SDIO ID

    Physical SDIO ID for virtual SDIO to mount on.

    @note For virtualSdio_setMapping()
*/
typedef enum
{
    SDIO_ID_1,                 ///< SDIO driver
    SDIO_ID_2,                 ///< SDIO2 driver

    ENUM_DUMMY4WORD(SDIO_ID)
} SDIO_ID;

//------------------------------------------------------------
// The general api for the SDMMC device driver
//------------------------------------------------------------

/*
    Public OS level functions
*/

// Functions exported from virtual SDIO
extern ER                   virtualSdio_open(void);
extern ER                   virtualSdio_close(void);
extern BOOL                 virtualSdio_isOpened(void);
extern ER                   virtualSdio_lock(void);
extern ER                   virtualSdio_unlock(void);

// Functions exported from virtual SDIO2
extern ER                   virtualSdio2_open(void);
extern ER                   virtualSdio2_close(void);
extern BOOL                 virtualSdio2_isOpened(void);
extern ER                   virtualSdio2_lock(void);
extern ER                   virtualSdio2_unlock(void);

//#NT#2012/03/04#HH Chuang -begin
//#NT#Add virtual SDIO driver
// Functions exported from virtual SDIO3
extern ER                   virtualSdio3_open(void);
extern ER                   virtualSdio3_close(void);
extern BOOL                 virtualSdio3_isOpened(void);
extern ER                   virtualSdio3_lock(void);
extern ER                   virtualSdio3_unlock(void);
//#NT#2012/03/04#HH Chuang -end

/*
    Public driver specific operating functions
*/

// Functions exported from virtual SDIO
extern PSTRG_TAB            virtualSdio_getStorageObject(void);
extern void                 virtualSdio_setFormatParam(INT8* pOemID, INT8* pVolumeLab, BOOL bRandomVolID, UINT32 uiVolID);
extern ER                   virtualSdio_setMapping(SDIO_ID sdioID, UINT32 uiBaseLBA, UINT32 uiSecCnt);

// Functions exported from virtual SDIO2
extern PSTRG_TAB            virtualSdio2_getStorageObject(void);
extern void                 virtualSdio2_setFormatParam(INT8* pOemID, INT8* pVolumeLab, BOOL bRandomVolID, UINT32 uiVolID);
extern ER                   virtualSdio2_setMapping(SDIO_ID sdioID, UINT32 uiBaseLBA, UINT32 uiSecCnt);

//#NT#2012/03/04#HH Chuang -begin
//#NT#Add virtual SDIO driver
extern PSTRG_TAB            virtualSdio3_getStorageObject(void);
extern void                 virtualSdio3_setFormatParam(INT8* pOemID, INT8* pVolumeLab, BOOL bRandomVolID, UINT32 uiVolID);
extern ER                   virtualSdio3_setMapping(SDIO_ID sdioID, UINT32 uiBaseLBA, UINT32 uiSecCnt);
//#NT#2012/03/04#HH Chuang -end

//@}

#endif // _SDIO_H





