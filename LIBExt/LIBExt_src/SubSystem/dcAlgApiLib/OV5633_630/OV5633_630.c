
#include "sensor_model.h"
#if OV5633_630
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"

#define SEN_READ_ID 0x6D
#define SEN_WRITE_ID 0x6C

//#NT#20090907#ethanlau -begin
_SEN_MODE _SEN_MODE_;
//#NT#20090907#ethanlau -end

extern void        DelayMs(unsigned long ms);

const UINT32 Sensor_Param[][2] =
{
    {0x30fa ,0x00},
    {0x300f ,0x00},
    {0x30b2 ,0x20},//~~~
    {0x3084 ,0x40},
    {0x3016 ,0x01},
    {0x308a ,0x25},
    {0x3013 ,0x00},
    {0x3015 ,0x03},
    {0x3065 ,0x50},
    {0x3068 ,0x08},
    {0x30bf ,0x02},
    {0x3069 ,0x00},
    {0x30ac ,0x05},
    {0x309d ,0x00},
    {0x309e ,0x24},
    {0x3098 ,0x5c},
    {0x3091 ,0x04},
    {0x3075 ,0x22},
    {0x3076 ,0x23},
    {0x3077 ,0x24},
    {0x3078 ,0x25},
    {0x30b5 ,0x0c},
    {0x3090 ,0x67},
    {0x30f9 ,0x11},
    {0x3311 ,0x80},
    {0x3312 ,0x1f},
    {0x3103 ,0x10},
    {0x305c ,0x01},
    {0x305d ,0x29},
    {0x305e ,0x00},
    {0x305f ,0xf7},
    {0x308d ,0x0b},
    {0x30ad ,0x20},
    {0x3072 ,0x0d},
    {0x308b ,0x82},
    {0x3317 ,0x9c},
    {0x3318 ,0x22},
    {0x3025 ,0x20},
    {0x3027 ,0x08},
    {0x3029 ,0x3f},
    {0x302b ,0xa3},
    {0x3319 ,0x22},
    {0x30a1 ,0xc4},
    {0x306a ,0x05},
    {0x3315 ,0x22},
    {0x30ae ,0x25},
    {0x3304 ,0x40},
    {0x3099 ,0x49},
    {0x30be ,0x50},
    {0x300e ,0xb0},
    {0x300f ,0x00},
    {0x3010 ,0x03},
    {0x3011 ,0x40},
    {0x30f8 ,0x00},
    {0x30f8 ,0x01},
    {0x30f8 ,0x09},
    {0x3083 ,0x00},
    {0x3086 ,0x24},
    {0x308c ,0x88},
    {0x30f9 ,0x13},
    {0x30fb ,0x81},
    {0x308a ,0x26},
    {0x3016 ,0x02},
    {0x3069 ,0x80},
    {0x309d ,0x04},
    {0x309e ,0x24},
    {0x3071 ,0x34},
    {0x331f ,0x22},
    {0x3098 ,0x54},
    {0x331e ,0x07},
    {0x3322 ,0x10},
    {0x3323 ,0x10},
    {0x331e ,0x05},
    {0x30af ,0x10},
    {0x304a ,0x00},
    {0x304d ,0x00},
    {0x304e ,0x22},
    {0x304d ,0xa0},
    {0x3058 ,0x00},
    {0x3059 ,0xff},
    {0x305a ,0x00},
    {0x30e9 ,0x04},
    {0x3090 ,0x67},
    {0x3084 ,0x44},
    {0x30e9 ,0x04},
    {0x30b5 ,0x1c},
    {0x331f ,0x22},
    {0x3020 ,0x02},
    {0x3021 ,0x04},
    {0x3022 ,0x07},
    {0x3023 ,0x80},
    {0x3024 ,0x00},
    {0x3025 ,0x2c},
    {0x3026 ,0x00},
    {0x3027 ,0x14},
    {0x3028 ,0x0a},
    {0x3029 ,0x2f},
    {0x302a ,0x07},
    {0x302b ,0xa3},
    {0x302c ,0x02},
    {0x302d ,0x80},
    {0x302e ,0x01},
    {0x302f ,0xe0},
    {0x30b3 ,0x09},
    {0x3301 ,0xc1},
    {0x3313 ,0xf1},
    {0x3314 ,0x05},
    {0x3315 ,0x04},
    {0x3316 ,0x01},
    {0x3317 ,0xe4},
    {0x3318 ,0x20},
    {0x300f ,0x01},
    {0x3022 ,0x08},
    {0x3023 ,0x10},
    {0x3300 ,0xd3},//{0x3300 ,0xfb},
    {0x3302 ,0xe0},
    {0x30ae ,0x15},
    {0x3304 ,0x4c},
    {0x30fa ,0x01},

    {0x300f ,0x31},
    {0x3018 ,0x3c},
    {0x3019 ,0x38},
    {0x301a ,0x82},
    {0x3000, 0x40},
    {0x3002, 0x03},
    {0x3003, 0xD0},
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

const UINT32 Sensor_Mode[][SEN_MODE_MAX]=
{
    //Addr,    VGA,   1280*960  Full
    { 0x3020 , 0x02 , 0x03 ,    0x07 } ,
    { 0x3021 , 0x04 , 0xe4 ,    0xbc } ,
    { 0x3022 , 0x0c , 0x07 ,    0x0c } ,
    { 0x3023 , 0x1c , 0xd6 ,    0xa0 } ,
    { 0x3024 , 0x00 , 0x00 ,    0x00 } ,
    { 0x3025 , 0x2c , 0x30 ,    0x20 } ,
    { 0x3026 , 0x00 , 0x00 ,    0x00 } ,
    { 0x3027 , 0x14 , 0x10 ,    0x08 } ,
    { 0x3028 , 0x0a , 0x0a ,    0x0a } ,
    { 0x3029 , 0x2f , 0x2f ,    0x3f } ,
    { 0x302a , 0x07 , 0x07 ,    0x07 } ,
    { 0x302b , 0xa3 , 0x97 ,    0xa3 } ,
    { 0x302c , 0x02 , 0x05 ,    0x0a } ,
    { 0x302d , 0x80 , 0x00 ,    0x20 } ,
    { 0x302e , 0x01 , 0x03 ,    0x07 } ,
    { 0x302f , 0xe0 , 0xc0 ,    0x98 } ,
    { 0x30b3 , 0x09 , 0x00 ,    0x00 } ,
    { 0x3301 , 0xc1 , 0xc0 ,    0xc0 } ,
    { 0x3313 , 0xf1 , 0xf0 ,    0xf0 } ,
    { 0x3314 , 0x05 , 0x05 ,    0x0a } ,
    { 0x3315 , 0x04 , 0x02 ,    0x22 } ,
    { 0x3316 , 0x01 , 0x03 ,    0x07 } ,
    { 0x3317 , 0xe4 , 0xc4 ,    0x9c } ,
    { 0x3318 , 0x20 , 0x22 ,    0x22 } ,
    { 0x3070 , 0x24 , 0x24 ,    0x24 } ,
    { 0x3092 , 0x00 , 0x00 ,    0x00 } ,
    { 0x30f8 , 0x09 , 0x05 ,    0x00 } ,
    { 0x3083 , 0x00 , 0x08 ,    0x08 } ,
    { 0x3086 , 0x24 , 0x00 ,    0x00 } ,
    { 0x308c , 0x88 , 0x80 ,    0x80 } ,
    { 0x30f9 , 0x13 , 0x11 ,    0x11 } ,
    { 0x30fb , 0x81 , 0x80 ,    0x80 } ,
    { 0x308a , 0x26 , 0x25 ,    0x25 } ,
    { 0x3016 , 0x02 , 0x01 ,    0x01 } ,
    { 0x300e , 0xb0 , 0xb6 ,    0xb5 } ,
    { 0x300f , 0x11 , 0x00 ,    0x0  } ,
    { 0x3010 , 0x03 , 0x07 ,    0x07 } ,
    { 0x3011 , 0x40 , 0x40 ,    0x40 } ,
    { 0x301e , 0x00 , 0x00 ,    0x00 } ,
    { 0x301f , 0x00 , 0x00 ,    0x00 } ,


};

const UINT32 OV5633_5M_FREX[][2] =
{
    {0x3020,0x07},
    {0x3021,0xbc},
    {0x3022,0x0c},
    {0x3023,0xa0},
    {0x3024,0x00},
    {0x3025,0x1e},
    {0x3026,0x00},
    {0x3027,0x08},
    {0x3028,0x0a},
    {0x3029,0x3f},
    {0x302a,0x07},
    {0x302b,0xa3},
    {0x302c,0x0a},
    {0x302d,0x20},
    {0x302e,0x07},
    {0x302f,0x98},
    {0x30b3,0x00},
    {0x3301,0xc0},
    {0x3313,0xf0},
    {0x3314,0x0a},
    {0x3315,0x22},
    {0x3316,0x07},
    {0x3317,0x9c},
    {0x3318,0x22},
    {0x3070,0x24},
    {0x3092,0x00},
    {0x30f8,0x00},
    {0x3013,0x00},
    {0x3002,0xff},
    {0x3003,0xff},
    {0x3089,0x0a},
    {0x30b1,0x3f},
    {0x30b2,0x02},
    {0x30b4,0x20},
    {0x30e4,0x01},
    {0x30e7,0x00},
    {0x30ec,0x28},
    {0x30f6,0xb0},
    {0x350e,0x44},
    {0x3071,0x20},
    {0x30e9,0x05},
    {0x3319,0x22},
    {0x3090,0x61},
    {0x30ac,0x04},
    {0x30ae,0x14},
    {0x30ad,0x20},
    {0x30bf,0x11},//改善高亮度十字線
    {0x30b4,0x00},
    {0x3506,0x30},
    {0x3501,0x01},
    {0x3300,0xd2},
    {0x30f9,0x11},
    {0x300e,0xb3},//{0x300f ,0x10}, //30 for 3.75fps PCLK=24MHz, 20 for 5fps PCLK=32MHz, 10 for 7.5fps PCLK=48MHZ, 00 for 15fps PCLK=96fps
    {0x300f,0x10},
    {0x3010,0x07},
    {0x3011,0x40},
    {0x301e,0x00},
    {0x301f,0x40},
};

void Sensor_config(UINT32 SenModeSel)
{
    debug_msg("\r\nSensor_config...\r\n");

    Sensor_WriteReg(0x3012, 0x80);
    TimerDelayMs(100);
    Sensor_Init();

    Sensor_SetMode(SenModeSel);
}

void Sensor_Init(void)
{
    UINT32 idx, table_size;

    table_size = (sizeof(Sensor_Param)/(4*2));
    debug_msg("sensor init...\r\n");
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg(Sensor_Param[idx][0], Sensor_Param[idx][1]);
    }
    debug_msg("sensor init finished...\r\n");
}


void Sensor_SetMode(UINT8 ucMode)
{
    UINT8 idx, table_size;
    //#NT#20090907#ethanlau -begin
    _SEN_MODE_ = ucMode;
    //#NT#20090907#ethanlau -end
    if(ucMode>=SEN_MODE_MAX)
    {
        debug_err(("no such sensor mode\r\n"));
        return;
    }

    if(ucMode == SEN_MODE_FULL_FREX)
    {
        table_size = (sizeof(OV5633_5M_FREX)/(4*2));
        for(idx=0; idx<table_size; idx++)
        {
            Sensor_WriteReg(OV5633_5M_FREX[idx][SEN_MODE_ADDR], OV5633_5M_FREX[idx][1]);
        }
    }
    else
    {
        table_size = (sizeof(Sensor_Mode)/(4*SEN_MODE_MAX));
        for(idx=0; idx<table_size; idx++)
            Sensor_WriteReg(Sensor_Mode[idx][SEN_MODE_ADDR], Sensor_Mode[idx][ucMode]);
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
    UINT32      ulWriteAddr, ulReg1,ulReg2, ulData1;//, ulData2;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (addr>>8);
    ulReg2      = ((addr)&(0x000000ff));
    ulData1     = value;//(value>>8);
    //ulData2     = ((value)&(0x000000ff));

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
            debug_err(("Error transmit data (reg1) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulReg2, 0, 0);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (reg2) !!\r\n"));
            break;
        }

        erReturn=i2c_masterTransmit(ulData1, 0, 1);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }

#if 0
        erReturn=i2c_masterTransmit(ulData2, 0, 1);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data2) !!\r\n"));
            break;
        }
#endif
    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}



UINT32 Sensor_ReadReg(UINT32 addr)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReadAddr, ulReg1,ulReg2, ulData, ulData1;//, ulData2;
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

        erReturn=i2c_masterReceive(&ulData1, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }
#if 0
        erReturn=i2c_masterReceive(&ulData2, 1, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data2!!\r\n"));
            break;
        }
#endif
    } while(erReturn != I2C_SUCCESS);
     ulData=ulData1;//((ulData1<<8)|(ulData2));
    i2c_unlock();
    return ulData;
}

void Sensor_WriteReg8bit(UINT32 addr, UINT32 value)
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

        erReturn=i2c_masterTransmit(ulData, 0, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
            break;
        }

    } while(erReturn != I2C_SUCCESS);

    i2c_unlock();
}
#endif
