/**
    Header file for Gsensor module

    This file is the header file that define the API and data type
    for Gsensor module.

    @file       Gsensor.h
    @ingroup    mISYSGsensor
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _GSENSOR_H
#define _GSENSOR_H

#include "Type.h"
#include "I2C.h"
#include "string.h"
#include "debug.h"
#include "OSCommon.h"
#include "Gpio.h"

/**
    @addtogroup mISYSGsensor
*/
//@{

#define GSENSOR_ERR     1
#if GSENSOR_ERR
#define GSENSOR_err(...)    debug_msg("GSensor:"__VA_ARGS__)
#else
#define GSENSOR_err(...)
#endif

#define GSENSOR_DBG     1
#if GSENSOR_DBG
#define GSENSOR_dbg(EXP,...)    if (!(EXP)) debug_msg("GSensor:"__VA_ARGS__)
#else
#define GSENSOR_dbg(EXP,...)
#endif

#define FLGGS_IDLE      (0x00000001)
#define FLGGS_INT       (0x00000002)



#define ERROR_CODE_TRUE			0
#define ERROR_CODE_FALSE		1
#define ERROR_CODE_WRITE_ADDR	10
#define ERROR_CODE_WRITE_DATA	20
#define ERROR_CODE_READ_ADDR	30
#define ERROR_CODE_READ_DATA	40
#define ERROR_CODE_START_BIT	50
#define ERROR_CODE_APROCESS		60
#define ERROR_CODE_DENY			70

#define GPIO_GSENSOR_DAT             C_GPIO_38
#define GPIO_GSENSOR_CLK              C_GPIO_39

#define I2C_SCL_OUT				gpio_setDir(GPIO_GSENSOR_CLK, GPIO_DIR_OUTPUT)
#define I2C_SCL_IN				gpio_setDir(GPIO_GSENSOR_CLK, GPIO_DIR_INPUT)
#define I2C_SCL_HIGH			gpio_setPin(GPIO_GSENSOR_CLK)
#define I2C_SCL_LOW				gpio_clearPin(GPIO_GSENSOR_CLK)
#define I2C_SCL_DETECT			gpio_getPin(GPIO_GSENSOR_CLK)

#define I2C_SDA_OUT				gpio_setDir(GPIO_GSENSOR_DAT, GPIO_DIR_OUTPUT)
#define I2C_SDA_IN				gpio_setDir(GPIO_GSENSOR_DAT, GPIO_DIR_INPUT)
#define I2C_SDA_HIGH 			gpio_setPin(GPIO_GSENSOR_DAT)
#define I2C_SDA_LOW  			gpio_clearPin(GPIO_GSENSOR_DAT)
#define I2C_SDA_DETECT			gpio_getPin(GPIO_GSENSOR_DAT)


// for axis data
typedef enum
{
    GSENSOR_AXIS_X,
    GSENSOR_AXIS_Y,
    GSENSOR_AXIS_Z,
    ENUM_DUMMY4WORD(GSENSOR_AXIS)
} GSENSOR_AXIS;


// for get status
typedef enum
{
    GSENSOR_ORIENT_0,
    GSENSOR_ORIENT_90,
    GSENSOR_ORIENT_180,
    GSENSOR_ORIENT_270,
    ENUM_DUMMY4WORD(GSENSOR_ORIENT)
} GSENSOR_ORIENT;

typedef enum
{
    GSENSOR_SHAKE_LEFT,
    GSENSOR_SHAKE_RIGHT,
    ENUM_DUMMY4WORD(GSENSOR_SHAKE)
} GSENSOR_SHAKE;

typedef struct
{
   UINT16      Xacc;
   UINT16      Yacc;
   UINT16      Zacc;
} AXIS_DATA;

// for get status
typedef enum
{
    ORIENT_0,
    ORIENT_90,
    ORIENT_180,
    ORIENT_270,
    ENUM_DUMMY4WORD(ORIENT)
} ORIENT;

typedef enum
{
    SHAKE_L,
    SHAKE_R,
    ENUM_DUMMY4WORD(SHAKE)
} SHAKE;

typedef struct {
    AXIS_DATA   Axis;
    ORIENT      Ori;
    SHAKE       SH;
} Gsensor_Data,*pGsensor_Data;

// for set feature
// 1. orientation detect time interval
// 2. shake detect time interval
// 3. shake threshold
typedef enum
{
    GSENSOR_FEATURE_ORIENT,
    GSENSOR_FEATURE_SHAKE_TIME,
    GSENSOR_FEATURE_SHAKE_THRESHOLD,
    ENUM_DUMMY4WORD(GSENSOR_FEATURE)
} GSENSOR_FEATURE;

typedef struct
{
    BOOL        (*GsensorInit)(void);
    void        (*GetGsensorData)(Gsensor_Data *GS_Data);
    void        (*GetGsensorStatus)(UINT32 status,UINT32 *uiData);
    void        (*GsensorOpenInterface)(void);
    void        (*GsensorCloseInterface)(void);
    void        (*GsensorSetSensitivity)(UINT32 level);
    void        (*GsensorReadInterupt)(void);
} GSENSOR_FUNC, *PGSENSOR_FUNC;

typedef struct
{
    UINT32      RegAddr;         //Register address
    UINT32      Data;            //Data
}GS_I2C_CMD;

typedef enum
{
    GSENSOR_NONE,
    GSENSOR_DMARD07,
    GSENSOR_GMA301,
    GSENSOR_DA380,
    GSENSOR_MAX
} GSENSOR_TYPE;

#define DMARD07_DEV_WRITE_ADDRESS	0x38
#define GMA301_DEV_WRITE_ADDRESS	0x30
#define DA380_DEV_WRITE_ADDRESS     0x4e

extern void I2C_Init();
extern void g_i2c_start(void);
extern void g_i2c_stop(void);
extern unsigned char g_i2c_ack_detect(void);
extern void g_i2c_ack_send(void);
extern unsigned char g_i2c_write_byte(unsigned char data);
extern unsigned char g_i2c_read_byte(void);
extern unsigned char i2c_write(unsigned char sub_addr, unsigned char buff,unsigned char device_addr);
extern unsigned char i2c_read(unsigned char sub_addr,unsigned char device_addr);

extern BOOL gsensor_open(void);
extern BOOL gsensor_close(void);
extern BOOL gsensor_Init(void);
extern void gsensor_GetData(Gsensor_Data *GS_Data);
extern void gsensor_GetStatus(UINT32 status,UINT32 *uiData);
extern void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc);
extern void gsensor_OpenInterface(void);
extern void gsensor_CloseInterface(void);
extern void LIS331DL_WriteData(UINT32 *buf);
extern void LIS331DL_ReadData(UINT32 *buf);
extern void gsensor_SetSensitivity(UINT32 level);
extern void gsensor_ReadSensitivity(void);
extern UINT32 gsensor_gettype(void);
//@}
extern GSENSOR_FUNC GsensorNoneFP;
extern GSENSOR_FUNC DMARD07FP;
extern GSENSOR_FUNC GMA301FP;
extern GSENSOR_FUNC DA380FP;

#endif
