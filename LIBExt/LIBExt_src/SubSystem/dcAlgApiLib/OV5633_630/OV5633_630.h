#ifndef _OV5633_H
#define _OV5633_H

// Sensor function
// 0 for index, don't use
typedef enum {
    SEN_MODE_ADDR = 0,
    SEN_MODE_VGA = MONITOR_MODE,
    SEN_MODE_1280_960 = MONITOR_MODE_2,
    SEN_MODE_FULL = MONITOR_MODE_3,
    SEN_MODE_FULL_FREX = STILL_MODE,
    SEN_MODE_MAX = 5
}_SEN_MODE;

//#NT#20090907#ethanlau -begin
extern _SEN_MODE Get_SensorMode(void);
//#NT#20090907#ethanlau -end
extern void Sensor_SetMode(UINT8 ucMode);
extern void Sensor_Init(void);
extern void Sensor_config(UINT32 SenModeSel);
extern void Sensor_WriteReg(UINT32 addr, UINT32 value);
extern UINT32 Sensor_ReadReg(UINT32 addr);
extern void Sensor_WriteReg8bit(UINT32 addr, UINT32 value);
extern void Sensor_TG_CFG(UINT32 addr, UINT32 value);
extern void Sensor_AFE_CFG(UINT32 addr, UINT32 value);
#endif