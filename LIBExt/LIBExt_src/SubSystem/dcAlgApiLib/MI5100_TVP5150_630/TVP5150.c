
#include "sensor_model.h"
#if MI5100_TVP5150_630
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"
#include    "cal_api.h"
#include "Top.h"


UINT32 Sensor_ReadReg_TVP5150(UINT32 addr);
void Sensor_WriteReg_TVP5150(UINT32 addr, UINT32 value);

//TV decoder: TI TVP5150AM1
const UINT32 TVP5150_Param[][2] =
{
    {0x01, 0x15},
    {0x03, 0x2D},
    {0x04, 0xC0},
    {0x07, 0x20},
    {0x08, 0x0C},
    {0x09, 0x90},
    {0x0C, 0x98},
    {0x0D, 0x40},
    {0x28, 0x00},

    {0x0F, 0x08},
    {0x1D, 0x02},
    {0x87, 0x01},
    {0x1E, 0x01},
    {0x1C, 0xFF},
    {0x0A, 0x80},
};

void Sensor_config_TVP5150(UINT32 SenModeSel)
{
    unsigned char idx, table_size;
    UINT32 value;
    table_size = (sizeof(TVP5150_Param)/(4*2));
    debug_err(("TVP5150 Sensor_config\r\n"));
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg_TVP5150(TVP5150_Param[idx][0], TVP5150_Param[idx][1]);
        TimerDelayMs(30);
    }
    TimerDelayMs(100);
}

void Sensor_Init_TVP5150(void)
{
}

void  Sensor_SetMode_TVP5150(UINT8 ucMode)
{
}

UINT32 GetCurretInputMode(void)
{
    UINT32 mode;

    mode=Sensor_ReadReg_TVP5150(0x8c);
    if(mode==0x81)
        return _TvInFormat_NTSC;
    else if(mode==0x83)
        return _TvInFormat_PAL;
    else
        return _TvInFormat_OFF; //_TvInFormat_NTSC;
}

void Sensor_WriteReg_TVP5150(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData1, ulData2;

    i2c_lock();

        erReturn=i2c_open();
    if(erReturn != E_OK)
    {
        debug_err(("Error open I2C driver!!\r\n"));
           //break;
    }

    pinmux_select_i2c(PINMUX_I2C);

    erReturn = i2c_initMaster(64, 10, 4);
    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
           //break;
    }
	    i2c_enable();

    ulWriteAddr = 0xb8;
    ulReg1      = addr;
    //ulData1     = (value>>8);
    ulData2     = ((value)&(0x000000ff));

  do
    {

        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (write addr)!!\r\n"));
           break;
        }

         //   debug_err((" transmit data (write addr) = %x\r\n",erReturn));

        erReturn=i2c_masterTransmit(ulReg1, 0, 0);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg) !!\r\n"));
            break;
        }
         //  debug_err((" Error transmit data (reg) = %x\r\n",erReturn));

   /*     erReturn=i2c_masterTransmit(ulData1, 0, 0);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }*/


        erReturn=i2c_masterTransmit(ulData2, 0, 1);

     //   debug_err((" Error transmit data (data2) = %x\r\n",erReturn));

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data2) !!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);
    i2c_reset();

    i2c_unlock();
 //debug_err(("tvp5150 Sensor_WriteReg end !!\r\n"));
}


UINT32 Sensor_ReadReg_TVP5150(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg, ulData, /*ulData1,*/ ulData2;

    i2c_lock();

    ulWriteAddr =0xB8;
    ulReadAddr  =0xB9;

    ulReg       =addr;
    ulData      =0;

    erReturn=i2c_open();
    if(erReturn != E_OK)
    {
        debug_err(("Error open I2C driver!!\r\n"));
        goto FUNC_END;
    }

    pinmux_select_i2c(PINMUX_I2C);

    erReturn = i2c_initMaster(64, 10, 4);
    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
        goto FUNC_END;
    }

    i2c_enable();
    //i2c_enableInt(I2C_INT_DT_EN);

    erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);
    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error transmit data (write addr)!! TVP5150AM1\r\n"));
        goto FUNC_END;
    }

    erReturn=i2c_masterTransmit(ulReg, 0, 0);
    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error transmit data!!\r\n"));
        goto FUNC_END;
    }

    i2c_enable();
    //i2c_enableInt(I2C_INT_DT_EN|I2C_INT_DR_EN);

    erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);
    if (erReturn !=  I2C_SUCCESS)
    {
        debug_err(("Error transmit addr before receive!!\r\n"));
        goto FUNC_END;
    }

    erReturn=i2c_masterReceive(&ulData2, 1, 1);
    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error receive data2!!\r\n"));
        goto FUNC_END;
    }

FUNC_END:

    i2c_reset();
    i2c_unlock();

    //ulData=((ulData1<<8)|(ulData2));
    ulData=ulData2;

    return ulData;
}



#endif
