/**
    @file       Sdmmc.h
    @ingroup    mICardSD

    @brief      SD/MMC Card driver header

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

*/

/*
    @addtogroup mICardSD
@{
*/

#ifndef _SDMMC_H
#define _SDMMC_H

#include "StrgDef.h"
// Memory Card Structure definition
#include "SdmmcDesc.h"

//#NT#detect card by call back routine installed from upper layer
typedef BOOL (*SDMMC_CARD_DETECT_CB)(void);

//------------------------------------------------------------
// The general api for the SDMMC device driver
//------------------------------------------------------------
//#NT#2007/03/15#Ethan Wang -begin
extern PSTRG_TAB    sdmmc_getStorageObject(void);
//#NT#2007/03/15#Ethan Wang -end

// -- Card detect functions --
extern UINT     sdmmc_getCardExist(void);
extern UINT     sdmmc_getCardWriteProt(void);
extern void     sdmmc_setDetectCardExistHdl(SDMMC_CARD_DETECT_CB pHdl);
extern void     sdmmc_setDetectCardProtectHdl(SDMMC_CARD_DETECT_CB pHdl);

// -- Get card register functions --
extern UINT32               sdmmc_getOCRRegister(void);
extern PSD_CID_STRUCT       sdmmc_getCIDRegister(void);
extern PMMC_CID_STRUCT      sdmmc_getCIDRegisterMMC(void);
extern PSDMMC_CSD_STRUCT    sdmmc_getCSDRegister(void);
//#NT#2007/02/12#Ethan Wang -begin
extern PSD_SCR_STRUCT       sdmmc_getSCRRegister(void);
extern PSD_STATUS_STRUCT    sdmmc_getSSRRegister(void);
//#NT#2007/02/12#Ethan Wang -end

// -- Get card info functions --
extern PSDMMC_INFO_STRUCT   sdmmc_getCardInformation(void);
extern void sdmmc_printCardInformation(void);

#endif // _SDMMC_H

/**
@}
*/
