/**
    Public header file for BMC module.

    This file is the header file that define the API and data type for BMC
    module.

    @file       bmc.h
    @ingroup    mIHALBMC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/


#ifndef _BMC_H
#define _BMC_H

#include "Type.h"

/**
    @addtogroup mIHALBMC
*/
//@{

/**
    Contant symbols for BMC chip select.
*/
typedef enum
{
    BMC_CS0,                    ///< BMC chip select 0
    BMC_CS1,                    ///< BMC chip select 1
    ENUM_DUMMY4WORD(BMC_CS)
}BMC_CS;

/**
    Contant symbols for BMC data bus mode.
*/
typedef enum
{
    BMC_DATA_BUS_MODE_8,        ///< Data bus 8 bits mode
    BMC_DATA_BUS_MODE_16,       ///< Data bus 16 bits mode
    ENUM_DUMMY4WORD(BMC_DATA_BUS_MODE)
}BMC_DATA_BUS_MODE;


/**
    Contant symbols for BMC DMA request control.
*/
typedef enum
{
    BMC_DMA_REQ_CTRL_FULL,      ///< Assert DMA request when FIFO is full/empty
    BMC_DMA_REQ_CTRL_HALF,      ///< Assert DMA request when FIFO is half full/empty
    ENUM_DUMMY4WORD(BMC_DMA_REQ_CTRL)
}BMC_DMA_REQ_CTRL;

/**
    Contant symbols for BMC DMA direction.
*/
typedef enum
{
    BMC_DMA_DIRECTION_WRITE,    ///< DMA from DRAM to device
    BMC_DMA_DIRECTION_READ,     ///< DMA from device to DRAM
    ENUM_DUMMY4WORD(BMC_DMA_DIRECTION)
}BMC_DMA_DIRECTION;

/**
    Contant symbols for BMC IOWait polarity.
*/
typedef enum
{
    BMC_CS1_IOWAIT_LOW_ACT,     ///< IOWait pin low active
    BMC_CS1_IOWAIT_HIGH_ACT,    ///< IOWait pin high active
    ENUM_DUMMY4WORD(BMC_CS1_IOWAIT_POLARITY)
}BMC_CS1_IOWAIT_POLARITY;

/**
   Contant symbols for PROM pin out mode setting.
*/
typedef enum
{
    BMC_PROM_PINMODE_STORAGE,   ///< Set BMC pin out to storage interface
    BMC_PROM_PINMODE_LCD,       ///< Set BMC pin out to LCD interface
    ENUM_DUMMY4WORD(BMC_PROM_PINMODE)
}BMC_PROM_PINMODE;


/**
   Contant symbols for PROM chip select setting.
*/
typedef enum
{
    BMC_PROM_CS0,               ///< Set to active BMC PROM chip select 0
    BMC_PROM_CS1,               ///< Set to active BMC PROM chip select 1
    BMC_PROM_CS0_CS1,           ///< Set to active BMC PROM chip select 0 and 1
    ENUM_DUMMY4WORD(BMC_PROM_CS)
}BMC_PROM_CS;


/**
    Contant symbols for PROM address width setting.
*/
typedef enum
{
    BMC_PROM_ADDR_WIDTH_256KB,  ///< Set address width as 256K bytes
    BMC_PROM_ADDR_WIDTH_512KB,  ///< Set address width as 512K bytes
    BMC_PROM_ADDR_WIDTH_1024KB, ///< Set address width as 1024K bytes
    BMC_PROM_ADDR_WIDTH_2048KB, ///< Set address width as 2048K bytes
    BMC_PROM_ADDR_WIDTH_4096KB, ///< Set address width as 4096K bytes
    BMC_PROM_ADDR_WIDTH_16B,    ///< Set address width as 16 bytes
    BMC_PROM_ADDR_WIDTH_256B,   ///< Set address width as 256 ytes
    BMC_PROM_ADDR_WIDTH_1B,     ///< Set address width as 1 byte
    ENUM_DUMMY4WORD(BMC_PROM_ADDR_WIDTH)
}BMC_PROM_ADDR_WIDTH;

/**
    Contant symbols for PROM one bit address setting.
*/
typedef enum
{
    BMC_PROM_ONEBITSEL_A0,   ///< Set address width as 256K bytes
    BMC_PROM_ONEBITSEL_A1,   ///< Set address width as 512K bytes
    BMC_PROM_ONEBITSEL_A2,   ///< Set address width as 1024K bytes
    BMC_PROM_ONEBITSEL_A3,   ///< Set address width as 2048K bytes
    BMC_PROM_ONEBITSEL_A11,  ///< Set address width as 4096K bytes
    BMC_PROM_ONEBITSEL_A12,  ///< Set address width as 16 bytes
    ENUM_DUMMY4WORD(BMC_PROM_ONEBITSEL)
}BMC_PROM_ONEBITSEL;

/**
    Contant symbols for BMC read timing.
*/
typedef struct
{
    UINT32  uiAOE;              ///< The time from ADDR to OEn valid, 0 ~ 127 HCLK
    UINT32  uiOEP;              ///< The time from ADDR to OEn invalid, 0 ~ 255 HCLK
    UINT32  uiRC;               ///< Read cycle time, 0 ~ 255 HCLK
} T_BMC_READ_TIMING;

/**
    Contant symbols for BMC write timing.
*/
typedef struct
{
    UINT32  uiAWE;              ///< The time from ADDR to WEn valid, 0 ~ 127 HCLK
    UINT32  uiWEP;              ///< The time from ADDR to WEn invalid, 0 ~ 255 HCLK
    UINT32  uiWC;               ///< Write cycle time, 0 ~ 255 HCLK
} T_BMC_WRITE_TIMING;

/**
    Contant symbols for BMC control setting.
*/
typedef struct
{
    BMC_DATA_BUS_MODE       dataBusMode;  ///< Data bus mode
    BOOL                    bIOWait;    ///< Enable/disable IOWait
    BMC_CS1_IOWAIT_POLARITY ioWaitPol;  ///< IOWait polarity setting
} T_BMC_CONTROL;

/**
    Contant symbols for BMC DMA control setting.
*/
typedef struct
{
    BOOL                bDMATrendEn;    ///< Enable/disable DMA transfer end interrupt
    BMC_DMA_REQ_CTRL    reqCtrl;        ///< DMA request control
} T_BMC_DMA_CONTROL;


// BMC driver APIs
extern ER bmc_open(void);
extern ER bmc_close(void);
extern ER bmc_waitDone(void);
extern ER bmc_setReadTiming(T_BMC_READ_TIMING *pTiming);
extern ER bmc_setWriteTiming(T_BMC_WRITE_TIMING *pTiming);
extern ER bmc_setReset(void);
extern ER bmc_setControl(T_BMC_CONTROL *pBMCCtrl);
extern ER bmc_setDMAControl(T_BMC_DMA_CONTROL *pDMACtrl);
extern ER bmc_startDMAWrite(UINT8 *pSrc, UINT32 uiDstOfs, UINT32 len);
extern ER bmc_startDMARead(UINT8 *pDst, UINT32 uiDstOfs, UINT32 len);
extern ER bmc_setChipSel(BMC_CS chipSel);
extern ER bmc_setPROMPinMode(BMC_PROM_PINMODE pinMode);
extern ER bmc_setPROMCS(BMC_PROM_CS cs);
extern ER bmc_setPROMAddrWidth(BMC_PROM_ADDR_WIDTH addrWidth);
extern ER bmc_setPROMOneBitAddr(BMC_PROM_ONEBITSEL sel);

//@}

#endif
