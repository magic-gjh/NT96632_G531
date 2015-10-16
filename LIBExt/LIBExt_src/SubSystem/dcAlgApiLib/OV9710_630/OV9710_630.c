
#include "sensor_model.h"
#if OV9710_630
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"

#define SEN_READ_ID 0x61
#define SEN_WRITE_ID 0x60

//#NT#20090907#ethanlau -begin
_SEN_MODE _SEN_MODE_;
//#NT#20090907#ethanlau -end

//extern void        DelayMs(unsigned long ms);

const UINT32 Sensor_Param[][2] =
{
    {0x96, 0x41},
    {0xBC, 0x68},
    {0x12, 0x00},
    {0x3B, 0x00},
    {0x97, 0x00},
    {0x15, 0x00},
    {0x17, 0x25},
    {0x18, 0xA2},
    {0x19, 0x00},
    {0x1A, 0xCA},
    {0x03, 0x0A},
    {0x32, 0x07},
    {0x98, 0x00},
    {0x99, 0x00},
    {0x9A, 0x00},
    {0x57, 0x00},
    {0x58, 0xC8},
    {0x59, 0xA2},
    {0x4C, 0x13},
    {0x4B, 0x36},
    {0x3D, 0x3C},
    {0x3E, 0x03},
    {0x4E, 0x55},
    {0x4F, 0x55},
    {0x50, 0x55},
    {0x51, 0x55},
    {0x24, 0x55},
    {0x25, 0x48},
    {0x26, 0x92},
    {0x5C, 0x59},
    {0x5D, 0x00},
    {0x11, 0x00},
    {0x2A, 0x9B},
    {0x2B, 0x06},
    {0x2D, 0x00},
    {0x2E, 0x00},
    {0x41, 0x81},
    {0x13, 0x00},
    {0x14, 0x40},

    // General setting
    {0x1E, 0x07},
    {0x5F, 0x18},
    {0x69, 0x04},
    {0x65, 0x2A},
    {0x68, 0x0A},
    {0x39, 0x28},
    {0x4D, 0x90},
    {0xC1, 0x80},
    {0x2A, 0x98},
    {0x96, 0xC1},
    {0x6D, 0x02}
};
//#NT#2010/09/09#ethanlau -begin
//#NT#Binning
//if 0X3621==0XAF&&0X3613==0XC4  => 4X
//if 0X3621==0XAF||0X3613==0XC4  => 2X
//else                           => 1X
//mclk:
//1280x480=>IN:20MHZ OUT:69MHZ FPS:60
//720p=>IN:24MHZ OUT:69MHZ FPS:30
//XVGA:IN:24MHZ OUT:64MHZ FPS:30
//FULL:IN:24MHZ OUT:64MHZ FPS:10
//#NT#2010/09/09#ethanlau -end

const UINT32 Sensor_Mode[][5]=
{
    //Addr, 30fps, 20fps, 15fps, 10 fps
    {0x5c,  0x59,  0x52,  0x59,  0x59,},  //clk2 = clk1 x(32-0x5c[4:0])
    {0x5d,  0x00,  0x08,  0x04,  0x08,},  //clk3 = clk2 / (0x5d[3:2]+1)
};

void Sensor_config(UINT32 SenModeSel)
{
    Sensor_WriteReg8bit(0x12, 0x80);
    TimerDelayMs(10);
    Sensor_Init();
}

void Sensor_Init(void)
{
    UINT32 idx, table_size,tmp;

    table_size = (sizeof(Sensor_Param)/(4*2));
    debug_msg("sensor init...\r\n");
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg8bit(Sensor_Param[idx][0], Sensor_Param[idx][1]);
        TimerDelayMs(5);
    }
    debug_msg("sensor init finished...\r\n");

    Sensor_SetMode(0);
}


void Sensor_SetMode(UINT8 ucMode)
{
    UINT8 idx, table_size,_tmp;
 
    if(GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1440x1080 /*||
       GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x1080 */)
    {
        ucMode = 3; //15fps 
    }
    else if (GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x1080)
    {
        ucMode = 4; //12.5fps -> modify to 10fps
    }
//    else if (GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x720)
//    {
//        ucMode = 2; //20fps
//    }
    else 
    {
        ucMode = 1; //30fps
    }

    table_size = 2; //(sizeof(Sensor_Mode)/(4*2));
    for(idx=0;idx<table_size;idx++)
    {
		Sensor_WriteReg8bit(Sensor_Mode[idx][0], Sensor_Mode[idx][ucMode]);
        TimerDelayMs(10);
    }
    debug_msg("sensor set mode(%d) finished...\r\n",ucMode);
}

//#NT#20090907#ethanlau -begin
_SEN_MODE Get_SensorMode(void)
{
    return _SEN_MODE_;
}
//#NT#20090907#ethanlau -end
void Sensor_WriteReg(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulReg2, ulData1, ulData2;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (addr>>8);
    ulReg2      = ((addr)&(0x000000ff));
    ulData1     = (value>>8);
    ulData2     = ((value)&(0x000000ff));

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


        erReturn=i2c_masterTransmit(ulReg2, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }


        erReturn=i2c_masterTransmit(ulData1, 0, 0);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }


        erReturn=i2c_masterTransmit(ulData2, 0, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data2) !!\r\n"));
            break;
        }
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}

UINT32 Sensor_ReadReg(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulReg2, ulData, ulData1, ulData2;

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

        if (erReturn !=  I2C_SUCCESS)
        {
            debug_err(("Error transmit addr before receive!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData1, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }

        erReturn=i2c_masterReceive(&ulData2, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data2!!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
     ulData=((ulData1<<8)|(ulData2));
    i2c_unlock();
    return ulData;
}

void Sensor_WriteReg8bit(UINT32 addr, UINT32 value)
{
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
