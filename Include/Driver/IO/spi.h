/**
    Header file of SPI module driver

    This file is the header file of SPI module

    @file       SPI.h
    @ingroup    mIIOSPI
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _SPI_H
#define _SPI_H

#include "Type.h"

/**
    @addtogroup mIIOSPI
*/
//@{

/**
    @name SPI max transfer count definition. (unit: word)

    @note For spi_writeReadData()
*/
//@{
#define SPI_MAX_TRANSFER_CNT    (0xFFFC)    ///< max transfer count for spi_writeReadData()
//@}

/**
    SPI mode

    @note For SPI_INIT_INFO, PSPI_INIT_INFO, and spi_init()
*/
typedef enum
{
    SPI_MODE_0,                 ///< MODE 0
    SPI_MODE_1,                 ///< MODE 1
    SPI_MODE_2,                 ///< MODE 2
    SPI_MODE_3,                 ///< MODE 3
    ENUM_DUMMY4WORD(SPI_MODE)
} SPI_MODE;

/**
    SPI pinmux selection

    @note For SPI_INIT_INFO, PSPI_INIT_INFO, and spi_init()
*/
typedef enum
{
    SPI_PINMUX_STORAGE,         ///< Pinmux to storage interface
    SPI_PINMUX_SERIAL,          ///< Pinmux to serial interface
    ENUM_DUMMY4WORD(SPI_PINMUX)
} SPI_PINMUX;

/**
    SPI transfer length

    @note For spi_setTransferLen()
*/
typedef enum
{
    SPI_TRANSFER_LEN_1BYTE,     ///< 1 byte
    SPI_TRANSFER_LEN_2BYTES,    ///< 2 bytes
    SPI_TRANSFER_LEN_3BYTES,    ///< 3 bytes
    SPI_TRANSFER_LEN_4BYTES,    ///< 4 bytes
    ENUM_DUMMY4WORD(SPI_TRANSFER_LEN)
} SPI_TRANSFER_LEN;

/**
    SPI initialization infomation

    @note For spi_init()
*/
typedef struct
{
    BOOL        bCSActiveLow;   ///< CS polarity
                                ///< - @b TRUE: CS is active LOW
                                ///< - @b FALSE: CS is active HIGH
    BOOL        bMasterMode;    ///< Master mode
                                ///< - @b TRUE: master mode
                                ///< - @b FALSE: slave mode
    UINT32      uiClkDiv;       ///< Clock divider: 0 ~ 255. SPI clock frequency = 48Mhz / (2 * (uiClkDiv+1))
    SPI_MODE    spiMODE;        ///< SPI MODE select
                                ///< - @b SPI_MODE_0: mode 0
                                ///< - @b SPI_MODE_1: mode 1
                                ///< - @b SPI_MODE_2: mode 2
                                ///< - @b SPI_MODE_3: mode 3
    BOOL        bLSB;           ///< LSB mode select (bit order)
                                ///< - @b TRUE: LSB mode
                                ///< - @b FALSE: MSB mode
    SPI_PINMUX  pinmuxChannel;  ///< SPI pinmux channel
                                ///< - @b SPI_PINMUX_STORAGE
                                ///< - @b SPI_PINMUX_SERIAL
} SPI_INIT_INFO, *PSPI_INIT_INFO;


extern void spi_attach(void);
extern void spi_detach(void);
extern ER   spi_open(void);
extern ER   spi_close(void);
extern BOOL spi_isOpened(void);
extern ER   spi_init(PSPI_INIT_INFO pSpiInitInfo);

extern ER   spi_setTransferLen(SPI_TRANSFER_LEN length);
extern void spi_setCSActive(BOOL bCSActive);
extern ER   spi_writeReadData(UINT32 uiWordCount, UINT32* pTxBuf, UINT32* pRxBuf, BOOL bDmaMode);
extern ER   spi_waitDataDone(void);
extern ER   spi_writeSingle(UINT32 uiTxWord);
extern ER   spi_readSingle(UINT32* pRxWord);
extern ER   spi_writeReadSingle(UINT32 uiTxWord, UINT32* pRxWord);

extern ER   spi_enBitMatch(UINT32 uiCmd, UINT32 uiBitPosition, BOOL bWaitValue);
extern ER   spi_waitBitMatch(UINT32 uiTimeoutMs);

//@}

#endif
