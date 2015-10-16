/**
    Compact Flash / ATA  controller header

    Compact Flash / ATA  controller header

    @file       cf.h
    @ingroup    mICardCF
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


#ifndef _CF_H
#define _CF_H

#include "StrgDef.h"

/** \addtogroup mICardCF */
//@{

/**
    CF operation mode index

    CF operation mode index, used for cf_init().

    @name   CF operation mode index
*/
//@{
#define CF_MEM_MAP_MODE              0x00       ///< Memory Mapping mode(for CF card only)
#define CF_IO_16_MODE                0x01       ///< IO mode(for CF card only)
#define CF_IO_PRICH_MODE             0x02       ///< IO primary channel mode(for CF card only)
#define CF_IO_SECCH_MODE             0x03       ///< IO secondary channel mode(for CF card only)
#define CF_MEM_WINDOW_MODE           0x04       ///< Memory window mode(for CF card only)
#define CF_TRUE_IDE_MODE             0x05       ///< True IDE mode(for CF card or ATA device)
//@}


/**
    Controller attached device type

    CF controller attached device type, used for cf_init().

    @name   Controller attached device type
*/
//@{
#define CFC_CARDTYPE_CF     (0x01)              ///< CF card type.
#define CFC_CARDTYPE_ATA    (0x02)              ///< ATA device.
//@}


/**
    CF card information structure

    CF basic information, used by cfcard_getCardInformation().
*/
//@{
typedef struct
{
    UINT16     uiDefault_Cylinder;          ///< Cylinder number
    UINT8      uiDefault_Head;              ///< Head number
    UINT16     uiDefault_Sector;            ///< Default sector count
    UINT32     uiTotal_Sector;              ///< Total sector number
    UINT32     uiTotal_LBA_Sector;          ///< Total LBA sector number
    UINT8      uiDMA_Supported;             ///< Support for DMA transfer
    UINT8      uiLBA_Supported;             ///< Support for LBA addressing
    UINT8      uiUDMA_Supported;            ///< Support for Ultra DMA transfer
    UINT8      uiUDMA_SupportedMode;        ///< Ultra-DMA supported mode
    UINT8      uiPIO_SupportedMode;         ///< PIO supported mode
    UINT8      uiLongAddr_Supported;        ///< Support for 48 bit addressing feature
    UINT8      uiPioBlockSize;              ///< PIO block size
    UINT8      uiHostProtectArea_Supported; ///< Support for host protected area feature

} CFCardInfoStruct, *PCFCardInfoStruct;
//@}


//------------------------------------------------------------
// The general api for the CF device driver
//------------------------------------------------------------
// -- Exported object for filesystem --
extern PSTRG_TAB cf_getStorageObject(void);

// -- General API --
extern ER   cf_init(UINT8 uiCardType, UINT32 uiMode, UINT32 uiCfClk);
extern ER   cf_open(void);
extern ER   cf_close(void);
extern ER   cf_format(void);
extern ER   cf_writeSectors(CHAR *pcBufPtr, UINT32 uiStartSec, UINT32 uiSectorCnt);
extern ER   cf_readSectors(CHAR *pcBufPtr, UINT32 uiStartSec, UINT32 uiSectorCnt);
extern ER   cf_eraseSectors(UINT32 uiStartSec, UINT32 uiSectorCnt);

// -- Get card info functions --
extern PCFCardInfoStruct    cfcard_getCardInformation(void);

#endif  // _CF_H

/**
@}
*/

