/**
    Header file for SDIO module driver

    @file       SDIO.h
    @ingroup    mICardSDIO

    @brief      SDIO Card driver header

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


#ifndef _SDIO_H
#define _SDIO_H

#include "StrgDef.h"
#include "SdmmcDesc.h"

/**
    @addtogroup mICardSDIO
*/
//@{

/**
    @name SDIO card detect function prototype

    SDIO card insert/protect detection call back prototype

    @note for sdio_setDetectCardExistHdl(), sdio2_setDetectCardExistHdl(), sdio_setDetectCardProtectHdl(), sdio2_setDetectCardProtectHdl()
*/
//@{
typedef BOOL (*SDIO_CARD_DETECT_CB)(void);  ///< card insert/detect function prototype
//@}

/**
    @name SDIO IO pad control function prototype

    SDIO IO pad control call back prototype

    @note for sdio_setCardIOHdl(), sdio2_setCardIOHdl()
*/
//@{
typedef void (*SDIO_SET_CARDIO_CB)(void);   ///< SDIO IO pad call back function prototype
//@}

//#NT#2010/11/10#HH Chuang -begin
//#NT#support pinmux assignment by project
/**
    SDIO pinmux location

    @note For PSDMMC_HOST_CONFIG_STRUCT, SDMMC_HOST_CONFIG_STRUCT
*/
typedef enum
{
    SDIO_PINMUX_LOCATION_NORMAL,    ///< normal pinmux location
    SDIO_PINMUX_LOCATION_2ND,       ///< 2nd pinmux location
    ENUM_DUMMY4WORD(SDIO_PINMUX_LOCATION)
} SDIO_PINMUX_LOCATION;
//#NT#2010/11/10#HH Chuang -end

/**
    @name SDIO source clock definition

    @note for sdio_setHSClk() and sdio2_setHSClk()
*/
//@{
#define SDIO_SRC_CLK_20MHZ      0   ///< Source clock 20Mhz
#define SDIO_SRC_CLK_24MHZ      1   ///< Source clock 24Mhz
#define SDIO_SRC_CLK_40MHZ      2   ///< Source clock 40Mhz (not available in SDIO2)
#define SDIO_SRC_CLK_48MHZ      3   ///< Source clock 48Mhz
#define SDIO_SRC_CLK_60MHZ      4   ///< Source clock 60Mhz (not available in SDIO2)
//@}

//------------------------------------------------------------
// Definitions for exported interface
//------------------------------------------------------------

/**
    @name SDIO IO RW flag definition.

    @note for SDIO_CMD52_STRUCT, SDIO_CMD53_STRUCT
    (for sdio_sendIOCMD52(), sdio2_sendIOCMD52(), sdio_sendIOCMD53(), sdio2_sendIOCMD53() )
*/
//@{
#define SDIO_IO_R                           0x0         ///< IO Read flag
#define SDIO_IO_W                           0x1         ///< IO Write flag
//@}

/**
    @name SDIO IO RAW (Read After Write) definition.

    @note for SDIO_CMD52_STRUCT
    (for sdio_sendIOCMD52(), sdio2_sendIOCMD52() )
*/
//@{
#define SDIO_IO_DIS_RAW                     0x0         ///< Disable IO RAW function
#define SDIO_IO_EN_RAW                      0x1         ///< Enable IO RAW function
//@}

/**
    @name SDIO IO byte/block access definition.

    @note for SDIO_CMD53_STRUCT
    (for sdio_sendIOCMD53(), sdio2_sendIOCMD53() )
*/
//@{
#define SDIO_IO_BYTEMODE                    0x0         ///< IO byte access mode
#define SDIO_IO_BLOCKMODE                   0x1         ///< IO block access mode
//@}

/**
    @name SDIO IO OP code definition.

    @note for SDIO_CMD53_STRUCT
    (for sdio_sendIOCMD53(), sdio2_sendIOCMD53() )
*/
//@{
#define SDIO_IO_OPCODE_FIX_ADDR             0           ///< OP code: multi byte R/W to fixed address
#define SDIO_IO_OPCODE_INCREMENT_ADDR       1           ///< OP code: multi byte R/W to incrementing address
//@}

/**
    @name SDIO IO function select definition.

    @note for SDIO_CMD52_STRUCT, SDIO_CMD53_STRUCT
    (for sdio_sendIOCMD52(), sdio2_sendIOCMD52() )
*/
//@{
#define SDIO_IO_FUNNUM0                     0           ///< Function 0 on IO device
#define SDIO_IO_FUNNUM1                     1           ///< Function 1 on IO device
#define SDIO_IO_FUNNUM2                     2           ///< Function 2 on IO device
#define SDIO_IO_FUNNUM3                     3           ///< Function 3 on IO device
#define SDIO_IO_FUNNUM4                     4           ///< Function 4 on IO device
#define SDIO_IO_FUNNUM5                     5           ///< Function 5 on IO device
#define SDIO_IO_FUNNUM6                     6           ///< Function 6 on IO device
#define SDIO_IO_FUNNUM7                     7           ///< Function 7 on IO device
//@}

//#NT#2009/12/16#SA4(HH Chuang) -begin
// return card type for upper layer
/**
    SDIO card type

    SDIO card type definition

    @note for sdio_getCardType() and sdio2_getCardType()
*/
typedef enum
{
    SDIO_CARD_UNKNOWN,          ///< unknown card type
    SDIO_CARD_SD,               ///< SD card
    SDIO_CARD_SDHC,             ///< SDHC card
    SDIO_CARD_SDXC,             ///< SDXC card
    SDIO_CARD_MMC,              ///< MMC card
    SDIO_CARD_SDIO,             ///< SDIO card
    ENUM_DUMMY4WORD(SDIO_CARD_TYPE)
} SDIO_CARD_TYPE;

// Backward Compatible
#define SDIO_CARD_TYPE_SD       (SDIO_CARD_SD)
#define SDIO_CARD_TYPE_MMC      (SDIO_CARD_MMC)
#define SDIO_CARD_TYPE_UNKNOWN  (SDIO_CARD_UNKNOWN)
//#NT#2012/06/25#HH Chuang -begin
//#NT#Improve compatability with some non-standard CMD17 behavior
/**
    SDIO configuration identifier

    @note For sdio_setConfig()
*/
typedef enum
{
    SDIO_CONFIG_ID_DIS_CMD17_RANGE_CHECK,   ///< Disable Out_Of_Range Check after CMD17 read last block on card
                                            ///< - @b TRUE: disable check. Can work with some non-standard card
                                            ///< - @b FALSE: enable check. Follow SD spec. (default value)

    SDIO_SET_INFO_VER = 0x00010001,         ///< START Tag for sdio_setInfo()
    SDIO_INFOR_END_TAG = 0x444E4549,        ///< END Tag for sdio_setInfo() ('I''E''N''D')
    ENUM_DUMMY4WORD(SDIO_CONFIG_ID)
} SDIO_CONFIG_ID;
//#NT#2012/06/25#HH Chuang -end
//#NT#2009/12/16#SA4(HH Chuang) -end
/**
    SDIO IO device CMD52 request structure.

    Structure definition for IO device CMD52 operation.

    @note for sdio_sendIOCMD52(), sdio2_sendIOCMD52()

    For more detail descriptions, please refer to SDIO spec.
*/
typedef struct
{
    INT8    *pData;                         ///< pointer to read/write buffer address
    UINT32  Reg_Addr;                       ///< register address in SDIO IO device
    UINT8   RAW;                            ///< Read After Write flag
                                            ///< - @b SDIO_IO_DIS_RAW: disable RAW function
                                            ///< - @b SDIO_IO_EN_RAW: enable RAW function
    UINT8   Function_Num;                   ///< function number in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM0: function 0 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM1: function 1 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM2: function 2 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM3: function 3 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM4: function 4 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM5: function 5 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM6: function 6 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM7: function 7 in SDIO IO device
    UINT8   RW_flag;                        ///< R/W flag
                                            ///< - @b SDIO_IO_R: IO read
                                            ///< - @b SDIO_IO_W: IO write
    UINT16  ByteCount;                      ///< Transfer byte count
    UINT16  Stuff;                          ///< Write Data/Stuff bits
} SDIO_CMD52_STRUCT, *PSDIO_CMD52_STRUCT;

/**
    SDIO IO device CMD53 request structure.

    Structure definition for IO device CMD53 operation.

    @note for sdio_sendIOCMD53(), sdio2_sendIOCMD53()

    For more detail descriptions, please refer to SDIO spec.
*/
typedef struct
{
    INT8    *pData;                         ///< pointer to read/write buffer address
    UINT32  Reg_Addr;                       ///< register address in SDIO IO device
    UINT8   OpCode;                         ///< IO OP code:
                                            ///< - @b SDIO_IO_OPCODE_FIX_ADDR: R/W to fixed address
                                            ///< - @b SDIO_IO_OPCODE_INCREMENT_ADDR: R/W to incrementing address
    UINT8   Block;                          ///< Block/Byte IO mode
                                            ///< - @b SDIO_IO_BYTEMODE: byte access mode
                                            ///< - @b SDIO_IO_BLOCKMODE: block access mode
    UINT8   Function_Num;                   ///< function number in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM0: function 0 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM1: function 1 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM2: function 2 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM3: function 3 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM4: function 4 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM5: function 5 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM6: function 6 in SDIO IO device
                                            ///< - @b SDIO_IO_FUNNUM7: function 7 in SDIO IO device
    UINT8   RW_flag;                        ///< R/W flag
                                            ///< - @b SDIO_IO_R: IO read
                                            ///< - @b SDIO_IO_W: IO write
    UINT16  ByteCount;                      ///< Transfer byte count
    UINT8   Stuff;                          ///< Write Data/Stuff bits
} SDIO_CMD53_STRUCT, *PSDIO_CMD53_STRUCT;

//#NT#2011/05/04#HH Chuang -begin
//#NT#detect SDHC and SDXC
/**
    SD card information structure
*/
typedef struct
{
    SDIO_CARD_TYPE cardType;                ///< card type
    UINT16  uiVersion;                      ///< SD/MMC spec version
    UINT32  uiWriteRate;                    ///< Write performance. unit: MB/s
} SDIO_MISC_INFORMATION, *PSDIO_MISC_INFORMATION;
//#NT#2011/05/04#HH Chuang -end

//------------------------------------------------------------
// The general api for the SDMMC device driver
//------------------------------------------------------------

/*
    Public OS level functions
*/

// Functions exported from SDIO
extern void                 sdio_init(void);
extern ER                   sdio_open(void);
extern ER                   sdio_close(void);
extern BOOL                 sdio_isOpened(void);
extern ER                   sdio_lock(void);
extern ER                   sdio_unlock(void);

// Functions exported from SDIO2
extern void                 sdio2_init(void);
extern ER                   sdio2_open(void);
extern ER                   sdio2_close(void);
extern BOOL                 sdio2_isOpened(void);
extern ER                   sdio2_lock(void);
extern ER                   sdio2_unlock(void);

/*
    Public driver specific operating functions
*/

// Functions exported from SDIO
extern PSTRG_TAB            sdio_getStorageObject(void);
extern UINT32               sdio_getCardExist(void);
extern UINT32               sdio_getCardWriteProt(void);
//#NT#2012/06/25#HH Chuang -begin
//#NT#Improve compatability with some non-standard CMD17 behavior
extern ER                   sdio_setConfig(SDIO_CONFIG_ID configID, UINT32 configContext);
//#NT#2012/06/25#HH Chuang -end
extern void                 sdio_setDetectCardExistHdl(SDIO_CARD_DETECT_CB pHdl);
extern void                 sdio_setDetectCardProtectHdl(SDIO_CARD_DETECT_CB pHdl);
extern void                 sdio_setCardIOHdl(SDIO_SET_CARDIO_CB pHdl);
extern void                 sdio_setDriveSink(UINT32 normalDriveSink, UINT32 hsDriveSink);
extern void                 sdio_setHSClk(UINT32 clkSrc, UINT32 clkDiv);
extern void                 sdio_setConfiguration(PSDMMC_HOST_CONFIG_STRUCT pSdioConfig);
//#NT#support pinmux assignment by project
extern void                 sdio_getConfiguration(PSDMMC_HOST_CONFIG_STRUCT pSdioConfig);
//#NT#2010/11/10#HH Chuang -end
extern void                 sdio_setFormatParam(INT8* pOemID, INT8* pVolumeLab, BOOL bRandomVolID, UINT32 uiVolID);
extern void                 sdio_printCardInformation(void);
extern PSDMMC_INFO_STRUCT   sdio_getCardInformation(void);
//#NT#2009/12/16#SA4(HH Chuang) -begin
// return card type for upper layer
extern SDIO_CARD_TYPE       sdio_getCardType(void);
//#NT#2009/12/16#SA4(HH Chuang) -end
//#NT#2011/05/04#HH Chuang -begin
//#NT#detect SDHC and SDXC
extern PSDIO_MISC_INFORMATION sdio_getMiscInfo(void);
//#NT#2010/05/04#HH Chuang -end

extern void                 sdio_setIOEnable(BOOL IOEn);
extern ER                   sdio_sendIOCMD52(PSDIO_CMD52_STRUCT pSDIO_CMD52);
extern ER                   sdio_sendIOCMD53(PSDIO_CMD53_STRUCT pSDIO_CMD53);

// Functions exported from SDIO2
extern PSTRG_TAB            sdio2_getStorageObject(void);
extern UINT32               sdio2_getCardExist(void);
extern UINT32               sdio2_getCardWriteProt(void);
//#NT#2012/06/25#HH Chuang -begin
//#NT#Improve compatability with some non-standard CMD17 behavior
extern ER                   sdio2_setConfig(SDIO_CONFIG_ID configID, UINT32 configContext);
//#NT#2012/06/25#HH Chuang -end
extern void                 sdio2_setDetectCardExistHdl(SDIO_CARD_DETECT_CB pHdl);
extern void                 sdio2_setDetectCardProtectHdl(SDIO_CARD_DETECT_CB pHdl);
extern void                 sdio2_setCardIOHdl(SDIO_SET_CARDIO_CB pHdl);
extern void                 sdio2_setDriveSink(UINT32 normalDriveSink, UINT32 hsDriveSink);
extern void                 sdio2_setHSClk(UINT32 clkSrc, UINT32 clkDiv);
extern void                 sdio2_setConfiguration(PSDMMC_HOST_CONFIG_STRUCT pSdioConfig);
//#NT#2010/11/10#HH Chuang -begin
//#NT#support pinmux assignment by project
extern void                 sdio2_getConfiguration(PSDMMC_HOST_CONFIG_STRUCT pSdioConfig);
//#NT#2010/11/10#HH Chuang -end
extern void                 sdio2_setFormatParam(INT8* pOemID, INT8* pVolumeLab, BOOL bRandomVolID, UINT32 uiVolID);
extern void                 sdio2_printCardInformation(void);
extern PSDMMC_INFO_STRUCT   sdio2_getCardInformation(void);
//#NT#2009/12/16#SA4(HH Chuang) -begin
// return card type for upper layer
extern SDIO_CARD_TYPE       sdio2_getCardType(void);
//#NT#2009/12/16#SA4(HH Chuang) -end
//#NT#2011/05/04#HH Chuang -begin
//#NT#detect SDHC and SDXC
extern PSDIO_MISC_INFORMATION sdio2_getMiscInfo(void);
//#NT#2011/05/04#HH Chuang -end

extern void                 sdio2_setIOEnable(BOOL IOEn);
extern ER                   sdio2_sendIOCMD52(PSDIO_CMD52_STRUCT pSDIO_CMD52);
extern ER                   sdio2_sendIOCMD53(PSDIO_CMD53_STRUCT pSDIO_CMD53);

//@}

#endif // _SDIO_H




