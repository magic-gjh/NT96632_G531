
#include "sensor_model.h"
#if NT99141_632
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"

#define SEN_READ_ID 0x55
#define SEN_WRITE_ID 0x54

#define NT99141_LDO_ENABLED		1

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
#if NT99141_LDO_ENABLED
0x3109, 0x84,
#else
0x3109, 0x04,
#endif

0x3040, 0x04,
0x3041, 0x02,
0x3042, 0xFF,
0x3043, 0x08,
0x3052, 0xE0,
0x305F, 0x33,
0x3106, 0x03,

0x3053, 0x10,

0x3100, 0x07,
0x3108, 0x00,
0x3110, 0x22,
0x3111, 0x57,
0x3112, 0x22,
0x3113, 0x55,
0x3114, 0x05,
0x3135, 0x00,

0x3200, 0x00,
0x3201, 0x08,
0x32BF, 0x60,
0x32C0, 0x58,
0x32C1, 0x58,
0x32C2, 0x58,
0x32C3, 0x00,
0x32C4, 0x20,
0x32C5, 0x10,
0x32C6, 0x18,
0x32C7, 0x00,
0x32C8, 0x78,
0x32C9, 0x58,
0x32CA, 0x68,
0x32CB, 0x68,
0x32CC, 0x70,
0x32CD, 0x70,
0x32DB, 0x6E,
0x3028, 0x27,
0x3029, 0x20,
0x302A, 0x04,
0x3022, 0x24,     //Rotate
//0x3022, 0x27,    
0x3023, 0x24,
0x3002, 0x00,
0x3003, 0x02,
0x3004, 0x00,
0x3005, 0x02,
0x3006, 0x05,
0x3007, 0x05,
0x3008, 0x02,
0x3009, 0xD5,
0x300A, 0x07, 
0x300B, 0x13, 
0x300C, 0x02,
0x300D, 0xe0,
0x300E, 0x05,
0x300F, 0x04,
0x3010, 0x02,
0x3011, 0xD4,
0x301E, 0x12,
0x32B8, 0x48,
0x32B9, 0x38,
0x32BB, 0x87,
0x32BC, 0x40,
0x32BD, 0x44,
0x32BE, 0x3C,
0x3021, 0x06,
0x32f0, 0x50,
0x3024, 0x00,

// NR
{0x3363, 0x07},
{0x333F, 0x07},	
{0x3360, 0x08}, // IQ_PARAM_0	
{0x3361, 0x18}, // IQ_PARAM_1	
{0x3362, 0x30}, // IQ_PARAM_2	
{0x336D, 0x20}, // DPC_0	
{0x336E, 0x1C},	// DPC_1
{0x336F, 0x18},	// DPC_2
{0x3370, 0x12},	// DPC_3
{0x3371, 0x10}, // NRW_0	
{0x3372, 0x20},	// NRW_1
{0x3373, 0x30},	// NRW_2
{0x3374, 0x3F},	// NRW_3
{0x3375, 0x04}, // NTHR_0	
{0x3376, 0x06},	// NTHR_1
{0x3377, 0x08},	// NTHR_2
{0x3378, 0x0A},	// NTHR_3
{0x3379, 0x10}, // COMPMAX_0	
{0x337A, 0x10},	// COMPMAX_1
{0x337B, 0x10},	// COMPMAX_2
{0x337C, 0x10},	// COMPMAX_3
	
{0x3021, 0x06}, 
{0x32F0, 0x80}, // Output: 0x50 for Raw, 0x80 for NR-Raw
{0x3024, 0x00},
// Exposure
{0x3012, 0x03},
{0x3013, 0x20},
{0x301D, 0x00},	

{0x3060, 0x01},
};

const UINT32 Sensor_Para_Flip[][2] =
{
    // Mclk24M  Pclk 40M   720p  30Fps
#if NT99141_LDO_ENABLED
0x3109, 0x84,
#else
0x3109, 0x04,
#endif

0x3040, 0x04,
0x3041, 0x02,
0x3042, 0xFF,
0x3043, 0x08,
0x3052, 0xE0,
0x305F, 0x33,
0x3106, 0x03,

0x3053, 0x10,

0x3100, 0x07,
0x3108, 0x00,
0x3110, 0x22,
0x3111, 0x57,
0x3112, 0x22,
0x3113, 0x55,
0x3114, 0x05,
0x3135, 0x00,

0x3200, 0x00,
0x3201, 0x08,
0x32BF, 0x60,
0x32C0, 0x58,
0x32C1, 0x58,
0x32C2, 0x58,
0x32C3, 0x00,
0x32C4, 0x20,
0x32C5, 0x10,
0x32C6, 0x18,
0x32C7, 0x00,
0x32C8, 0x78,
0x32C9, 0x58,
0x32CA, 0x68,
0x32CB, 0x68,
0x32CC, 0x70,
0x32CD, 0x70,
0x32DB, 0x6E,
0x3028, 0x27,
0x3029, 0x20,
0x302A, 0x04,
0x3022, 0x24,     //Rotate
//0x3022, 0x27,    
0x3023, 0x24,
0x3002, 0x00,
0x3003, 0x02,
0x3004, 0x00,
0x3005, 0x02,
0x3006, 0x05,
0x3007, 0x05,
0x3008, 0x02,
0x3009, 0xD5,
0x300A, 0x07, 
0x300B, 0x13, 
0x300C, 0x02,
0x300D, 0xe0,
0x300E, 0x05,
0x300F, 0x04,
0x3010, 0x02,
0x3011, 0xD4,
0x301E, 0x12,
0x32B8, 0x48,
0x32B9, 0x38,
0x32BB, 0x87,
0x32BC, 0x40,
0x32BD, 0x44,
0x32BE, 0x3C,
0x3021, 0x06,
0x32f0, 0x50,
0x3024, 0x00,

// NR
{0x3363, 0x07},
{0x333F, 0x07},	
{0x3360, 0x08}, // IQ_PARAM_0	
{0x3361, 0x18}, // IQ_PARAM_1	
{0x3362, 0x30}, // IQ_PARAM_2	
{0x336D, 0x20}, // DPC_0	
{0x336E, 0x1C},	// DPC_1
{0x336F, 0x18},	// DPC_2
{0x3370, 0x12},	// DPC_3
{0x3371, 0x10}, // NRW_0	
{0x3372, 0x20},	// NRW_1
{0x3373, 0x30},	// NRW_2
{0x3374, 0x3F},	// NRW_3
{0x3375, 0x04}, // NTHR_0	
{0x3376, 0x06},	// NTHR_1
{0x3377, 0x08},	// NTHR_2
{0x3378, 0x0A},	// NTHR_3
{0x3379, 0x10}, // COMPMAX_0	
{0x337A, 0x10},	// COMPMAX_1
{0x337B, 0x10},	// COMPMAX_2
{0x337C, 0x10},	// COMPMAX_3
	
{0x3021, 0x06}, 
{0x32F0, 0x80}, // Output: 0x50 for Raw, 0x80 for NR-Raw
{0x3024, 0x00},
// Exposure
{0x3012, 0x03},
{0x3013, 0x20},
{0x301D, 0x00},	

{0x3060, 0x01},
};

const UINT32 Sensor_Mode[][SEN_MODE_MAX]=
{
     //Addr, 30fps 30fps 30fps 30fps 15fps 10fps
    {0x300c, 0x02, 0x02, 0x02, 0x02, 0x05, 0x08},
    {0x300d, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xA0},
    {0x3060, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}
};

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
        if(SensorFlipSel == SEN_Flip_H_V){    
        Sensor_WriteReg(Sensor_Para_Flip[idx][0], Sensor_Para_Flip[idx][1]);}
        else{		
        Sensor_WriteReg(Sensor_Para[idx][0], Sensor_Para[idx][1]);}			
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
            Sensor_WriteReg(0x3022, 0x27);//Sensor_ReadReg(0x3022)&0xfc);

        break;
        case SEN_Flip_H:
            //Sensor_WriteReg(0x3022, (Sensor_ReadReg(0x3022)&0xfc)|0x02);

        break;
        case SEN_Flip_V:
            //Sensor_WriteReg(0x3022, (Sensor_ReadReg(0x3022)&0xfc)|0x01);

        break;
        case SEN_Flip_H_V:
            Sensor_WriteReg(0x3022, 0x24);//(Sensor_ReadReg(0x3022)&0xfc)|0x03);

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
    //debug_err(("NT99141 write addr(%x, %x)\r\n",addr,value));
    
    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("NT99141 Error transmit data (write addr)!!\r\n"));
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
