/**

    Public header file for I2C module

    This file is the header file that define the API and data type for I2C
    module.

    @file       i2c.h
    @ingroup    mIIOI2C
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/



#ifndef _I2C_H
#define _I2C_H

#include "Type.h"
#if 0  //i2c driver will handle this part
#define I2C_INT_DT_EN     0x0100
#define I2C_INT_DR_EN     0x0200
#define I2C_INT_BERR_EN   0x0400
#define I2C_INT_STOP_EN   0x0800
#define I2C_INT_SAM_EN    0x1000
#define I2C_INT_AL_EN     0x2000
#endif

/** \addtogroup mIIOI2C */

//@{

/**
    I2C transfer status
*/
typedef enum
{
    I2C_SUCCESS,                        ///< Transfer success
    ER_I2C_DATA_ERROR,                  ///< System error
    ER_I2C_NAK,                         ///< Receive a nak
    ER_I2C_BUSY,                        ///< I2C busy between START and STOP
    ER_I2C_BUS_NOT_AVAILABLE,           ///< I2C bus is being used by anothers.
    ER_I2C_AL,                          ///< Lose arbitration
    ER_I2C_SYS_ERR,                     ///< Open DMA channel error
    ENUM_DUMMY4WORD(ER_I2C)
} ER_I2C;


/**
    I2C transfer data for PIO mode
*/
typedef struct
{
    UINT8  uiData0;      ///< I2C Data Register0
    UINT8  uiData1;      ///< I2C Data Register1
    UINT8  uiData2;      ///< I2C Data Register2
    UINT8  uiData3;      ///< I2C Data Register3
} I2C_DATA_REG, *PI2C_DATA_REG;


//I2C Driver API

ER  i2c_close(void);
ER  i2c_open(void);

void i2c_attach(void);
void i2c_detach(void);

BOOL i2c_isOpened(void);

ER  i2c_lock(void);
ER  i2c_unlock(void);

void i2c_reset(void);
void i2c_enable(void);
void i2c_disable(void);

#if 0 //i2c driver will handle this part
void i2c_enableInt(UINT32 IntCtrl);
void i2c_disableInt(UINT32 IntCtrl);
#endif
void i2c_getClkSetting(UINT32 *clkdiv, UINT32 *tsr, UINT32 *gsr);
void i2c_setSAR(UINT32 sar, BOOL tenBitMode);

ER_I2C i2c_setPIOSize(UINT32 size);
void i2c_setPIOAckGen(BOOL Ack1 , BOOL Ack2 , BOOL Ack3 ,BOOL Ack4);
void i2c_setPIOStartGen(BOOL Sta1 , BOOL Sta2 , BOOL Sta3 ,BOOL Sta4);
void i2c_setPIOStopGen(BOOL St1 , BOOL St2 , BOOL St3 ,BOOL St4);
ER_I2C i2c_masterTransmitB(I2C_DATA_REG uiData);
ER_I2C i2c_masterReceive4B(UINT32 *puiData);
ER_I2C i2c_masterTransmitDMA(UINT8 *DMA_Addr,UINT32 DMA_Trans_Len);
ER_I2C i2c_masterReceiveDMA(UINT8 *DMA_Addr,UINT32 DMA_Trans_Len);

UINT32 i2c_getStatus(void);
ER_I2C i2c_initMaster(UINT32 clkdiv, UINT32 tsr, UINT32 gsr);
ER_I2C i2c_masterReceive(UINT32 *puiData, BOOL bNak, BOOL bStop);
ER_I2C i2c_masterTransmit(UINT32 uiData, BOOL bStart, BOOL bStop);
//@}

#endif
