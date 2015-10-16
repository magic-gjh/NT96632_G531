
#include "sensor_model.h"
#if OV9710_DUAL_630
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
extern unsigned char GPIO_i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char buff);
extern unsigned char GPIO_i2c_read(unsigned char device_addr, unsigned char sub_addr);

extern void GPIOMap_3010RSTNHigh(void);
extern void GPIOMap_3010RSTNLow(void);
extern UINT32 Get_SensorModeIndex(void);

void Sensor_AEEn(UINT32 enable);

const UINT32 Sensor_Param[][2] =
{

	#include "OV9712_1280x800.c"

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

const UINT32 B3010_ParamAB[][2] =
{
	#include "3010_1920x800.c"

};

const UINT32 B3010_ParamA[][2] =
{
//bypass mode

#if 1 ///1
    {0x00,0x02},
    {0xA8,0x3c},
    {0x88, 0x00},
    {0xA3, 0x01},
    {0xA2, 0x01},
    {0x00,0x02},
    {0x00,0x01},
#else ///2
    {0x00,0x02},
    {0xA8,0x3d},
    {0x88, 0x00},
    {0xA3, 0x01},
    {0xA2, 0x01},
    {0x00,0x02},
    {0x00,0x01},
#endif
};

const UINT32 B3010_ParamB[][2] =
{
//bypass mode

#if 0 ///1
    {0x00,0x02},
    {0xA8,0x3c},
    {0x88, 0x00},
    {0xA3, 0x01},
    {0xA2, 0x01},
    {0x00,0x02},
    {0x00,0x01},
#else ///2
    {0x00,0x02},
    {0xA8,0x3d},
    {0x88, 0x00},
    {0xA3, 0x01},
    {0xA2, 0x01},
    {0x00,0x02},
    {0x00,0x01},
#endif
};

const UINT32 Sensor_Mode[][5]=
{
    //Addr, 30fps, 20fps, 15fps, 12.5fps
    {0x5c,  0x59,  0x52,  0x59,  0x59,},  //clk2 = clk1 x(32-0x5c[4:0])
    {0x5d,  0x00,  0x08,  0x04,  0x0c,},  //clk3 = clk2 / (0x5d[3:2]+1)
};

void Bypass_Sensor_AB(void)
{
	UINT32 idx;
    UINT32 table_size;
    UINT32 value;
    value = Sensor_Read3010Reg8bit(0xa8);
    if ( value == 0x3f )
        return ;

	table_size = (sizeof(B3010_ParamAB)/(4*2));
	for(idx=0;idx<table_size;idx++)
	{
		Sensor_Write3010Reg8bit(0x7e, B3010_ParamAB[idx][0], B3010_ParamAB[idx][1]);
		TimerDelayMs(5);
	}
}


void Bypass_Sensor_A(void)
{
	UINT32 idx;
    UINT32 table_size;
    UINT32 value;
    value = Sensor_Read3010Reg8bit(0xa8);
    if ( value == 0x3c )
        return ;

	table_size = (sizeof(B3010_ParamA)/(4*2));
	for(idx=0;idx<table_size;idx++)
	{
    	Sensor_Write3010Reg8bit(0x7e, B3010_ParamA[idx][0], B3010_ParamA[idx][1]);
		TimerDelayMs(5);
	}
}

void Bypass_Sensor_B(void)
{
	UINT32 idx;
    UINT32 table_size;
    UINT32 value;
    value = Sensor_Read3010Reg8bit(0xa8);
    if ( value == 0x3d )
        return ;

	table_size = (sizeof(B3010_ParamB)/(4*2));
	for(idx=0;idx<table_size;idx++)
	{
    	Sensor_Write3010Reg8bit(0x7e, B3010_ParamB[idx][0], B3010_ParamB[idx][1]);
		TimerDelayMs(5);
	}
}

void Sensor_config(UINT32 SenModeSel)
{
    Sensor_Init();
}

void Sensor_Init(void)
{
    UINT32 idx, table_size,tmp;
    UINT32 i;

    debug_msg("sensor init...\r\n");

    //GPIOMap_3010RSTNHigh();
    GPIOMap_3010RSTNLow();
    TimerDelayMs(200);

    Sensor_WriteReg8bit(0x12, 0x80);
    TimerDelayMs(10);

    table_size = (sizeof(Sensor_Param)/(4*2));
    debug_msg("sensor init...\r\n");

    for(idx=0;idx<table_size;idx++)
    {
		Sensor_WriteReg8bit(Sensor_Param[idx][0], Sensor_Param[idx][1]);
        TimerDelayMs(10);
    }
    debug_msg("sensor init finished...\r\n");

    Sensor_SetMode(0);

    GPIOMap_3010RSTNHigh();
    TimerDelayMs(200);
    debug_msg("3010 init ...\r\n");

    switch(Get_SensorModeIndex())
    {
    	case 0:
            Sensor_AEEn(TRUE);
			Bypass_Sensor_AB();
			break;
    	case 1:
            Sensor_AEEn(FALSE);
			Bypass_Sensor_A();
			break;
    	case 2:
            Sensor_AEEn(FALSE);
			Bypass_Sensor_B();
			break;

    }
    debug_msg("3010 init finished...\r\n");
}

void Sensor_SetMode(UINT8 ucMode)
{
    UINT8 idx, table_size,_tmp;
#if 0
    //#NT#20090907#ethanlau -begin

    _SEN_MODE_ = ucMode;
    //#NT#20090907#ethanlau -end
    if(ucMode>=SEN_MODE_MAX)
    {
        debug_err(("no such sensor mode\r\n"));
        return;
    }

    debug_msg("sensor set mode(%d) finished...\r\n",ucMode);
#endif

    if(GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1440x1080 ||
       GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x1080 )
       //|| GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x720 )
    {
        ucMode = 3; //15fps
    }
    else if (GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x1080)
    {
        ucMode = 4; //12.5fps
    }
    else if (GetIPPSizeInfor(_Vid_ResolutionIdx) == _Resolution_1920x720)
    {
        ucMode = 2; //20fps
    }
    else  //1280x480
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

UINT32 SensorAWBEn = TRUE;
void Sensor_AWBEn(UINT32 enable)
{
    if ( enable == ENABLE && SensorAWBEn == FALSE )
    {
        debug_err(("OV9710 AWB Enable\r\n"));
        Sensor_WriteReg8bit(0x96, 0xf1);
        Sensor_WriteReg8bit(0x38, 0x10);

    }
    else if ( enable == DISABLE && SensorAWBEn == TRUE )
    {
        debug_err(("DSP AWB Enable\r\n"));

        Sensor_WriteReg8bit(0x96, 0xc1);
        Sensor_WriteReg8bit(0x38, 0x00);
    }
    SensorAWBEn = enable;
}

UINT32 SensorAEEn = TRUE;
void Sensor_AEEn(UINT32 enable)
{
    if ( enable == ENABLE )
    {
        Sensor_WriteReg8bit(0x13, 0x05);
    }
    else if ( enable == DISABLE )
    {
        Sensor_WriteReg8bit(0x13, 0x00);
    }
    SensorAEEn = enable;
}


void Sensor_Set3010I2CPath(UINT32 type)
{
    UINT32 value;
        value = Sensor_Read3010Reg8bit(0xb0);

    if ( type == 1 ) //set sensor A, SN0
    {
        value = (value | 0x10);
        value = (value | 0x84) & (~(0x01));
        value = (value | 0x02) & (~(0x48));
      	Sensor_Write3010Reg8bit(0x7e, 0xb0, value);
		TimerDelayMs(10);
    }
    else if ( type == 2 ) //set sensor B, SN1
    {
        value = (value | 0x10);
        value = (value | 0x48) & (~(0x02));
        value = (value | 0x01) & (~(0x84));
      	Sensor_Write3010Reg8bit(0x7e, 0xb0, value);
		TimerDelayMs(10);
    }
    else
    {
        value = (value & (~(0xD3))) | 0x0c;
      	Sensor_Write3010Reg8bit(0x7e, 0xb0, value);
        TimerDelayMs(10);
    }
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

void Sensor_Write3010Reg8bit(UINT32 ulWriteAddr, UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    //UINT32      ulWriteAddr, ulReg1, ulData;
    UINT32      ulReg1, ulData;

    i2c_lock();

    //ulWriteAddr = 0x7E;
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


UINT32 Sensor_Read3010Reg8bit(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulData, ulData1, ulData2;
    i2c_lock();
    ulWriteAddr =0x7e;
    ulReadAddr  =0x7f;
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

    //debug_err(("Sensor_Read3010Reg8bit () =========>>%d\r\n",ulData  ));
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

