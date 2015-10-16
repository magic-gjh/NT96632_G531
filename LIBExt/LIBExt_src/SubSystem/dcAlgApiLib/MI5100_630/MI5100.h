#ifndef _MI5100_H
#define _MI5100_H

// Sensor function
// 0 for index, don't use
enum {
    SEN_MODE_ADDR = 0,
    SEN_MODE_VGA,
    SEN_MODE_720P,
    SEN_MODE_1440_1080,
    SEN_MODE_FULL,
    SEN_MODE_MAX
};

extern void Sensor_SetMode(UINT8 ucMode);
extern void Sensor_Init(void);
extern void Sensor_config(UINT32 SenModeSel);
extern void Sensor_WriteReg(UINT32 addr, UINT32 value);
extern UINT32 Sensor_ReadReg(UINT32 addr);
extern void Sensor_WriteReg8bit(UINT32 addr, UINT32 value);
extern void Sensor_TG_CFG(UINT32 addr, UINT32 value);
extern void Sensor_AFE_CFG(UINT32 addr, UINT32 value);
#endif