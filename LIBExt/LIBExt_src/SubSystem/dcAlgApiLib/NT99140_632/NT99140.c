
#include "sensor_model.h"
#if NT99140_632
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"

#define SEN_READ_ID 0x55
#define SEN_WRITE_ID 0x54

#define NT99140_LDO_ENABLED		1

//#NT#20090907#ethanlau -begin
_SEN_MODE _SEN_MODE_;
//#NT#20090907#ethanlau -end

//#NT#20120820#clifflin -begin
//#NT#Add sensor flip function
_SEN_FLIP SensorFlipSel = SEN_Flip_OFF;
//#NT#20120820#clifflin -end

//extern void        DelayMs(unsigned long ms);

const UINT32 Sensor_Para[][2] =
{
    // Mclk24M  Pclk 40M   720p  30Fps
#if NT99140_LDO_ENABLED
    {0x3109, 0x84},
#else
    {0x3109, 0x04},
#endif

    {0x32F0,0x01},    // Output_Format_Sel 0:YCbCr
    {0x3028,0x05},
    {0x3029,0x02},
    {0x302a,0x00},
    {0x3336,0x20},
    {0x3270,0x00},
    {0x3271,0x04},
    {0x3272,0x0E},
    {0x3273,0x28},
    {0x3274,0x3F},
    {0x3275,0x50},
    {0x3276,0x6E},
    {0x3277,0x88},
    {0x3278,0xA0},
    {0x3279,0xB3},
    {0x327A,0xD2},
    {0x327B,0xE8},
    {0x327C,0xF5},
    {0x327D,0xFF},
    {0x327E,0xFF},
    {0x3302,0x00},
    {0x3303,0x01},
    {0x3304,0x00},
    {0x3305,0xD3},
    {0x3306,0x00},
    {0x3307,0x2B},
    {0x3308,0x07},
    {0x3309,0xA4},
    {0x330A,0x07},
    {0x330B,0x2D},
    {0x330C,0x01},
    {0x330D,0x30},
    {0x330E,0x00},
    {0x330F,0xC9},
    {0x3310,0x07},
    {0x3311,0x4D},
    {0x3312,0x07},
    {0x3313,0xEA},
    {0x32DB,0x90},
    {0x3300,0x70},
    {0x3301,0x40},
    {0x3024,0x00},

    {0x3040,0x04},    // Calibration Control
    //{0x3040,0x02},    // Calibration Control
    {0x3041,0x02},      // Auto Calibration
    {0x3042,0xFF},      // ABLC_Thr_Top
    {0x3043,0x14},      // ABLC_Thr_Bottom
    
    {0x3052,0xC0},
        
    {0x3053,0x10},
    {0x3057,0x80},    // DeRowDefect_ThrH
    {0x3058,0x00},    // DeRowDefect_ThrL
    {0x3059,0x2F},    // DeRowDefect_Control
    {0x305f,0x44},
    //{0x305f,0x00},
    //{0x305f,0x88},

    {0x32b0,0x00},
    {0x32b1,0x90},
    {0x32BB,0x1b},
    {0x32bd,0x10},
    {0x32be,0x05},
    {0x32bf,0x49},
    {0x32c0,0x40},
    {0x32C3,0x0F},     // AGC_Min_Limit
    {0x32c5,0x24},
    {0x32cd,0x01},
    {0x32d3,0x00},
    {0x32f6,0x0c},   // Special_Effect_1
    {0x3118,0xF2},   // Reset Boost
    {0x3119,0xF2},   // TX Boost
    {0x311A,0x13},   // Row sel boost
    {0x3106,0x01},   // abs_en=0, anti-blooming_en = 1
    {0x3108,0x55},   // G_INT_0/1, BIAS_INT_0/1
    {0x3105,0x47},     // 70uA

    {0x3300,0x70},
    {0x3301,0x40},
    {0x3331,0x08},
    {0x3332,0x40},
    {0x32bf,0x49},
    {0x3200,0x3e},
    {0x3201,0x3f},
    {0x3012,0x01},
    {0x3013,0xB8},
    {0x3028,0x13},
    {0x3029,0x20},
    {0x302a,0x00},

    {0x3022,0x24},
    //{0x3022,0x27},// Rotate

    {0x3023,0x24},
    {0x3002,0x00},
    {0x3003,0x02},
    {0x3004,0x00},
    {0x3005,0x02},
    {0x3006,0x05},
    {0x3007,0x03},
    {0x3008,0x02},
    {0x3009,0xd3},
    {0x300a,0x07},
    {0x300b,0x14},
    {0x300c,0x02},
    {0x300d,0xE2},
    {0x300e,0x05},
    {0x300f,0x02},
    {0x3010,0x02},
    {0x3011,0xD2},
    {0x301d,0x24},
        
    {0x301E,0x12}, // Add by Iyen
        
    {0x32bb,0x1b},
    {0x32bc,0x40},
    {0x32c1,0x2b},
    {0x32c2,0x82},
    {0x32c8,0xdc},
    {0x32c9,0xb8},
    {0x32c4,0x00},
    {0x3201,0x0f}, // Disable AE, AWB
    {0x3021,0x06},
    {0x3021,0x16},
    //{0x3060,0x01},

    {0x32f0,0x5C},    // [3:2] = 3: B start
    
    {0x306A,0x02}, // Driving 4mA

    {0x3106,0x03},
        
    {0x3060,0x01}
};
//#NT#20111006#Iyen -begin
const UINT32 Sensor_Mode[][SEN_MODE_MAX]=
{
     //Addr, 30fps 30fps 30fps 30fps 15fps 10fps
    {0x300c, 0x02, 0x02, 0x02, 0x02, 0x05, 0x08},
    {0x300d, 0xE2, 0xE2, 0xE2, 0xE2, 0xC4, 0xA6},
    {0x3060, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}
};
//#NT#20111006#Iyen -end

extern void SensorReset(void);

void Sensor_config(UINT32 SenModeSel)
{           
    TimerDelayMs(10);
    Sensor_Init();
    Sensor_SetMode(SenModeSel);
}

void Sensor_Init(void)
{
    UINT32 idx, table_size;

    table_size = (sizeof(Sensor_Para)/sizeof(Sensor_Para[0]));
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg(Sensor_Para[idx][0], Sensor_Para[idx][1]);
    }
}

//#NT#20120820#clifflin -begin
//#NT#Add sensor flip function
/**
  Set Sensor flip type

  This function is used to set sensor flip type
  This function is used to set sensor flip type

  @param _SEN_FLIP SenFlipVal: sensor flip type
    Available values are below:
    SEN_Flip_OFF
    SEN_Flip_H
    SEN_Flip_V
    SEN_Flip_H_V
  @param BOOL bActive
    - @b TRUE: Flip output image immediatly
    - @b FALSE: Flip output image after next sensor initiation
*/
void Sensor_SetFlip(_SEN_FLIP SenFlipVal, BOOL bActive)
{
    SensorFlipSel = SenFlipVal;
    if(bActive == TRUE)
    {
        switch(SenFlipVal)
        {
        case SEN_Flip_OFF:
            Sensor_WriteReg(0x3022, 0x27);//(0x3022, Sensor_ReadReg(0x3022)&0xfc);

        break;
        case SEN_Flip_H:
            //Sensor_WriteReg(0x3022, (Sensor_ReadReg(0x3022)&0xfc)|0x02);

        break;
        case SEN_Flip_V:
            //Sensor_WriteReg(0x3022, (Sensor_ReadReg(0x3022)&0xfc)|0x01);

        break;
        case SEN_Flip_H_V:
            Sensor_WriteReg(0x3022, 0x24);//(0x3022, (Sensor_ReadReg(0x3022)&0xfc)|0x03);

        break;
        default:
        break;
        }

    }

}
//#NT#20120820#clifflin -end

//#NT#20111006#Iyen -begin
void Sensor_SetMode(UINT8 ucMode)
{
    UINT16 idx, table_size;
    UINT8 PrvRes;

    table_size = (sizeof(Sensor_Mode)/sizeof(Sensor_Mode[0]));

    switch(ucMode)
    {
        case MONITOR_MODE_2:
            PrvRes = SEN_MODE_720P_15;
            break;
        case MONITOR_MODE_3:
            PrvRes = SEN_MODE_720P_10;
            break;
        case MONITOR_MODE:
        default:
            PrvRes = SEN_MODE_720P_30;
            break;
    }

    for(idx=0; idx<table_size; idx++)
    {
        Sensor_WriteReg(Sensor_Mode[idx][0], Sensor_Mode[idx][PrvRes]);
    }    
}
//#NT#20111006#Iyen -end

//#NT#20090907#ethanlau -begin
_SEN_MODE Get_SensorMode(void)
{
    return _SEN_MODE_;
}
//#NT#20090907#ethanlau -end
void Sensor_WriteReg(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulReg2, ulData;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (addr>>8);
    ulReg2      = ((addr)&(0x000000ff));
    ulData      = ((value)&(0x000000ff));
    
    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("NT99140 Error transmit data (write addr)!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulData, 0, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

UINT32 Sensor_ReadReg(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData/*, ulData1, ulData2*/;

    i2c_lock();
    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1      =(addr>>8);
    ulReg2      =((addr)&(0x000000ff));
    ulData      =0;

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

//        erReturn=i2c_masterTransmit(ulReg1, 0, 1);
        erReturn=i2c_masterTransmit(ulReg1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }
        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);

        if (erReturn !=  I2C_SUCCESS/*ER_I2C_AL*/)
        {
            debug_err(("Error transmit addr before receive!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
//     ulData=((ulData1<<8)|(ulData2));
    i2c_unlock();
    return ulData;
}

void Sensor_WriteReg8bit(UINT32 addr, UINT32 value)
{
#if 0
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = ((addr)&(0x000000ff));
    ulData      = ((value)&(0x000000ff));
    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (write addr)!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }
        erReturn=i2c_masterTransmit(ulData, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
#endif
}

UINT32 Sensor_ReadReg8bit(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulData, ulData1, ulData2;
    i2c_lock();
    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1      = addr;
    ulData      =0;
    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data!!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);

        if (erReturn !=  I2C_SUCCESS/*ER_I2C_AL*/)
        {
            debug_err(("Error transmit addr before receive!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData1, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
    ulData=ulData1;
    i2c_unlock();
    return ulData;
}

#endif
