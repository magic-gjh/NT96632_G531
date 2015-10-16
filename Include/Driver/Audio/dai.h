/**
    Header file for DAI module

    This file is the header file that define the API and data type
    for DAI module

    @file       DAI.h
    @ingroup    mIAudDAI
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/



#ifndef _DAI_H
#define _DAI_H

#include "Type.h"

/** \addtogroup mIAudDAI */
//@{

/**
     DAI ISR callback function prototype
*/
typedef void (*DAICallbackFunc)(void);


// Interrupt Control Register
#define DAI_DISABLE_ALL_INT           0x00000000
#define DAI_STS_READ_INT_EN           0x00000001
#define DAI_CMD_WRITE_INT_EN          0x00000002

#define DAI_TXFIFO_THOD_INT_EN        0x00000010
#define DAI_TXFIFO_EMPTY_INT_EN       0x00000020
#define DAI_TXFIFO_ERROR_INT_EN       0x00000040
#define DAI_TXDMADONE_INT_EN          0x00000080
#define DAI_TXTCHIT_INT_EN            0x00000100

#define DAI_RXFIFO_THOD_INT_EN        0x00010000
#define DAI_RXFIFO_FULL_INT_EN        0x00020000
#define DAI_RXFIFO_ERROR_INT_EN       0x00040000
#define DAI_RXDMADONE_INT_EN          0x00080000
#define DAI_RXTCHIT_INT_EN            0x00100000
#define DAI_RXTCLATCH_INT_EN          0x10000000

// Interrupt Status Register
#define DAI_AC97_STSREAD_INT_STS      0x00000001
#define DAI_AC97_CMDWRITE_INT_STS     0x00000002

#define DAI_TXFIFO_THOD_INT_STS       0x00000010
#define DAI_TXFIFO_EMPTY_INT_STS      0x00000020
#define DAI_TXFIFO_ERROR_INT_STS      0x00000040
#define DAI_TXDMADONE_INT_STS         0x00000080
#define DAI_TXTCHIT_INT_STS           0x00000100

#define DAI_RXFIFO_THOD_INT_STS       0x00010000
#define DAI_RXFIFO_FULL_INT_STS       0x00020000
#define DAI_RXFIFO_ERROR_INT_STS      0x00040000
#define DAI_RXDMADONE_INT_STS         0x00080000
#define DAI_RXTCHIT_INT_STS           0x00100000
#define DAI_RXTCLATCH_INT_STS         0x10000000

#define DAI_CLEAR_ALL_INT_STS         0x001F01F3


// DAI format
typedef enum
{
    I2S,              //I2S
    AC97,             // AC97
    GPIO
} FRAME_SEL;

/**
     DAI I2S clock ratio
*/
typedef enum
{
    DAI_I2SCLKR_256FS_32BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_256FS_64BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_384FS_32BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_384FS_48BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 48 BitClk
    DAI_I2SCLKR_384FS_96BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 96 BitClk
    DAI_I2SCLKR_512FS_32BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_512FS_64BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_512FS_128BIT, ///< SystemClk = 512 * FrameSync, FrameSync = 128 BitClk
    DAI_I2SCLKR_768FS_32BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_768FS_48BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 48 BitClk
    DAI_I2SCLKR_768FS_64BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_768FS_192BIT, ///< SystemClk = 768 * FrameSync, FrameSync = 192 BitClk
    DAI_I2SCLKR_1024FS_32BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 32 BitClk
    DAI_I2SCLKR_1024FS_64BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 64 BitClk
    DAI_I2SCLKR_1024FS_128BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 128 BitClk
    DAI_I2SCLKR_1024FS_256BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 256 BitClk
    ENUM_DUMMY4WORD(DAI_I2SCLKR)
} DAI_I2SCLKR;

/**
     DAI I2S format
*/
typedef enum
{
    DAI_I2SFMT_STANDARD,        ///< I2S Standard
    DAI_I2SFMT_LIKE_MSB=2,      ///< I2S Like, MSB justified
    DAI_I2SFMT_LIKE_LSB,        ///< I2S Like, LSB justified
    ENUM_DUMMY4WORD(DAI_I2SFMT)
} DAI_I2SFMT;

/**
     DAI channel
*/
typedef enum
{
    DAI_CH_STEREO = 1,        ///< Stereo
    DAI_CH_MONO_LEFT,         ///< Left
    DAI_CH_MONO_RIGHT,        ///< Right
    ENUM_DUMMY4WORD(DAI_CH)

} DAI_CH;
/**
     DAI DRAM sound format
*/
typedef enum
{
    DAI_DRAMPCM_STEREO,       ///< Stereo
    DAI_DRAMPCM_MONO,         ///< Mono
    ENUM_DUMMY4WORD(DAI_DRAMSOUNDFMT)

} DAI_DRAMSOUNDFMT;

/**
     DAI DRAM PCM data Length
*/
typedef enum
{
    DAI_DRAMPCM_8,             ///< 8bits
    DAI_DRAMPCM_16,            ///< 16bits
    ENUM_DUMMY4WORD(DAI_DRAMPCMLEN)

} DAI_DRAMPCMLEN;
/**
     DAI DRAM PCM data format
*/
typedef enum
{
    DAI_DRAMPCM_LITTLE,        ///< Little endian
    DAI_DRAMPCM_BIG,           ///< Big Endian
    ENUM_DUMMY4WORD(DAI_DRAMPCMFMT)

} DAI_DRAMPCMFMT;

/**
     DAI access mode
*/
typedef enum
{
    DAI_ACCESS_PIO,            ///< PIO access mode
    DAI_ACCESS_DMA,            ///< DMA access mode
    ENUM_DUMMY4WORD(DAI_ACCESSMODE)

} DAI_ACCESSMODE;
/**
     DAI operation mode
*/
typedef enum
{
    DAI_OP_SLAVE,              ///< Slave mode
    DAI_OP_MASTER,             ///< Master mode
    ENUM_DUMMY4WORD(DAI_OPMODE)

} DAI_OPMODE;
/**
     Embedded codec clock source
*/
typedef enum
{
    DAI_CODESCK_INT,           ///< internal clock
    DAI_CODESCK_EXT,           ///< external clock
    ENUM_DUMMY4WORD(DAI_CODESCK)

} DAI_CODESCK;
/**
     DAI frame type
*/
typedef enum
{
    DAI_I2S,                  ///< I2S
    DAI_AC97,                 ///< AC97
    ENUM_DUMMY4WORD(DAI_FRAME)

} DAI_FRAME;

/**
     DAI transfer mode
*/
typedef enum
{
    DAI_TX,                  ///< transmit mode
    DAI_RX,                  ///< receive mode
    ENUM_DUMMY4WORD(DAI_TXRX_OP)

} DAI_TXRX_OP;

// APIs in digital DAI interface
extern void aud_isr(void);
extern void dai_open(DAICallbackFunc pIsrHdl);
extern void dai_close(void);
extern void dai_setI2SClkRatio(DAI_I2SCLKR I2SCLKRatio);
extern void dai_setI2SFmt(DAI_I2SFMT I2SFmt);
extern void dai_setI2SOpMode(DAI_OPMODE I2SOpMode);
extern void dai_setCodecClkSrc(DAI_CODESCK CodecClkSrc);
extern void dai_setFrame(DAI_FRAME Frame);
extern DAI_FRAME dai_getFrame(void);
extern void dai_enableAVSync(BOOL bAvSyncEn);
extern void dai_enableDMA(DAI_TXRX_OP TxRx_OP,BOOL bDmaEn);
extern void dai_enableDAI(DAI_TXRX_OP TxRx_OP, BOOL bEn);
extern BOOL dai_isDMAEnable(DAI_TXRX_OP TxRx_OP);
extern BOOL dai_isDAIEnable(DAI_TXRX_OP TxRx_OP);
extern void dai_clrFIFO(DAI_TXRX_OP TxRx_OP);
extern BOOL dai_isTxmode(void);
extern BOOL dai_isRxmode(void);
extern void dai_setFIFOThod(DAI_TXRX_OP TxRx_OP, UINT32 uiFIFOThod);
extern void dai_setCh(DAI_TXRX_OP TxRx_OP, DAI_CH DAICh);
extern void dai_setDram_SoundFmt(DAI_TXRX_OP TxRx_OP, DAI_DRAMSOUNDFMT SoundFmt);
extern void dai_setDRAM_PCMFmt(DAI_TXRX_OP TxRx_OP, DAI_DRAMPCMFMT PCMFmt);
extern void dai_setDRAM_PCMLen(DAI_TXRX_OP TxRx_OP, DAI_DRAMPCMLEN PCMLen);
extern void dai_setAccessMode(DAI_TXRX_OP TxRx_OP,DAI_ACCESSMODE AccessMode);
extern BOOL dai_isBusy(void);
extern void dai_setIntCtrl(UINT32 uiIntCtrl);
extern UINT32 dai_getIntCtrl(void);
extern void dai_clrIntStatus(UINT32 uiIntSts);
extern UINT32 dai_getIntStatus(void);
extern void dai_setTimecodeTrigger(DAI_TXRX_OP TxRx_OP, UINT32 uiTrigger);
extern UINT32 dai_getTimecodeTrigger(DAI_TXRX_OP TxRx_OP);
extern void dai_setTimecodeOffset(DAI_TXRX_OP TxRx_OP, UINT32 uiOffset);
extern UINT32 dai_getTimecodeValue(DAI_TXRX_OP TxRx_OP);
extern void dai_setDMAPara(DAI_TXRX_OP TxRx_OP, UINT32 uiStadr, UINT32 uiBufSz, BOOL bStop );
extern UINT32 dai_getDMACurrAdr(DAI_TXRX_OP TxRx_OP);
extern void dai_setFlg(void );
extern void dai_clrFlg(void );
extern void dai_sendAC97Command(BOOL bWriteCmd, UINT32 uiRegIdx, UINT32 uiData);
extern void dai_readAC97Data(UINT32 *puiRegIdx, UINT32 *puiData);
extern void dai_warmresetAC97(void);
extern void dai_setLoopBack(BOOL bEn);
//@}
#endif



