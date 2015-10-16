
#include "sensor_model.h"
#if OV5653_630
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
//    {0x3008 ,0x82},
//    {0x3008 ,0x42},
    {0x300f ,0x8e},
    {0x3011 ,0x10},
    {0x3017 ,0x7f},
    {0x3018 ,0xfc},
    {0x3103 ,0x93},
    {0x3500 ,0x00},
    {0x3501 ,0x3},//{0x3501 ,0x15},//
    {0x3502 ,0xf0},
    {0x350b ,0x4},
    {0x3503 ,0x37},//;17//agc active timing
    {0x350A ,0x0},
    {0x350B ,0x10},//{0x350B ,0x7f},//
    {0x3600 ,0x50},
    {0x3601 ,0x0d},
    {0x3603 ,0xa7},
    {0x3604 ,0x50},
    {0x3605 ,0x4},
    {0x3606 ,0x3f},
    {0x3612 ,0x1a},
    {0x3615 ,0x50},
    {0x3620 ,0x56},
    {0x3630 ,0x22},
    {0x3631 ,0x22},
    {0x3632 ,0x55},
    {0x3702 ,0x3a},
    {0x3704 ,0x18},
    {0x3706 ,0x41},
    {0x370b ,0x40},
    {0x370e ,0x0},
    {0x3710 ,0x28},
    {0x3712 ,0x13},
    {0x3800 ,0x2},
    {0x3801 ,0x54},
    {0x3810 ,0x40},
    {0x3815 ,0x82},
    {0x3830 ,0x50},
    {0x3836 ,0x41},
    {0x3a00 ,0x38},
    {0x3a18 ,0x0},
    {0x3a19 ,0xf8},
    {0x3a1a ,0x6},
    {0x3c01 ,0x80},
    {0x4000 ,0x1},//{0x4000 ,0x1},
    {0x401c ,0x48},
    {0x401d ,0x28}, //**
    {0x5000 ,0x06},
    {0x5001 ,0x0},
    {0x503d ,0x0},
    {0x5046 ,0x1},
    {0x3406 ,0x1},
    {0x3400 ,0x4},
    {0x3401 ,0x0},
    {0x3402 ,0x4},
    {0x3403 ,0x0},
    {0x3404 ,0x4},
    {0x3405 ,0x0},
    {0x3010 ,0x30},
    {0x3613 ,0xc4}, //**
    {0x3621 ,0xef},//[6] =0,Hor bin sum,[6]=1,hor skip
    {0x3703 ,0x9a},
    {0x3705 ,0xdc},
    {0x370a ,0x81},
    {0x370c ,0xc8},
    {0x370d ,0x42},
    {0x3713 ,0x2d},
    {0x3803 ,0xa},
    {0x3804 ,0x5},
    {0x3805 ,0x0},
    {0x3806 ,0x1},
    {0x3807 ,0xe4},//;e0
    {0x3808 ,0x2},
    {0x3809 ,0x80},
    {0x380a ,0x1},
    {0x380b ,0xe4},//;e0
    {0x380C ,0x9},//;8
    {0x380D ,0x90},//;38
    {0x380E ,0x2},
    {0x380F ,0x8e},//;F8
    {0x3818 ,0xc2},
    {0x381a ,0x0},
    {0x381c ,0x20},
    {0x381d ,0xa},
    {0x381e ,0x1},
    {0x381f ,0x20},
    {0x3820 ,0x0},
    {0x3821 ,0x0},
    {0x3824 ,0x23},
    {0x3825 ,0x26},
    {0x5002 ,0x2},
    {0x5901 ,0x4},
    {0x4700 ,0x4},//;Sync mode
    //{0x4708 ,0x7},//;Vsync, HREF, PCLK polarity, inverse signal
    {0x3008 ,0x2},
    {0x350d ,0x0},
    {0x350c ,0x0},
    {0x3604 ,0x40},
    {0x3631 ,0x36},
    {0x3632 ,0x5f},
    {0x3711 ,0x24},
    {0x3713 ,0x92},
    {0x3714 ,0x17},
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
//   Addr,      VGA,    720p_30 1280*960    1280*480        2560*960    2560*720  2560*1080   Full
    {0x3011     ,0x10   ,0x17   ,0x10       ,0x0e           ,0x0E       ,0x10      ,0x10      ,0x10},

    {0x401d     ,0x08   ,0x08   ,0x08       ,0X08           ,0X08       ,0X08      ,0x08      ,0x08},
    {0x3613     ,0xc4   ,0xc4   ,0xc4       ,0xc4           ,0xc4       ,0xc4      ,0x44      ,0x44},
    {0x3621     ,0xaf   ,0xaf   ,0xaf       ,0xaf           ,0x2f       ,0x2f      ,0x2f      ,0x2f},
    {0x3703     ,0x9a   ,0x9a   ,0xb0       ,0xb0           ,0xe6       ,0xe6      ,0xe6      ,0xe6},
    {0x3705     ,0xdc   ,0xda   ,0xda       ,0xda           ,0xda       ,0xda      ,0xda      ,0xda},
    {0x370a     ,0x81   ,0x80   ,0x80       ,0x80           ,0x80       ,0x80      ,0x80      ,0x80},
    {0x370c     ,0xc8   ,0xc5   ,0xc5       ,0xc5           ,0xa0       ,0xa0      ,0xa0      ,0xa0},
    {0x370d     ,0x42   ,0x42   ,0x42       ,0x42           ,0x4        ,0x4       ,0x4       ,0x4},
    {0x3713     ,0x2d   ,0x2d   ,0x2f       ,0x2f           ,0x2f       ,0x2f      ,0x2f      ,0x2f},
    {0x3803     ,0xa    ,0xe    ,0x8        ,0x08           ,0xe        ,0xe       ,0xe       ,0xe},
    {0x3804     ,0x2    ,0x5    ,0x5        ,0x5            ,0xa        ,0xa       ,0xA       ,0xA},
    {0x3805     ,0x80   ,0x00   ,0x00       ,0x00           ,0x00       ,0x00      ,0x20      ,0x20},
    {0x3806     ,0x1    ,0x2    ,0x3        ,0x1            ,0x3        ,0x2       ,0x5       ,0x7},
    {0x3807     ,0xe4   ,0xd0   ,0xc0       ,0xe0           ,0xc0       ,0xf8      ,0x90      ,0x98},

    {0x3808     ,0x2    ,0x5    ,0x5        ,0x5            ,0xa        ,0xa       ,0xA       ,0xA},
    {0x3809     ,0x80   ,0x00   ,0x00       ,0x00           ,0x00       ,0x00      ,0x20      ,0x20},
    {0x380a     ,0x1    ,0x2    ,0x3        ,0x1            ,0x3        ,0x2       ,0x5       ,0x7},
    {0x380b     ,0xe4   ,0xd0   ,0xc0       ,0xe0           ,0xc0       ,0xf8      ,0x90      ,0x98},

    {0x380C     ,0x9    ,0x8    ,0x8        ,0x8            ,0x13       ,0x13      ,0x0d       ,0xc},
    {0x380D     ,0x90   ,0x60   ,0x98       ,0x98           ,0x20       ,0x20      ,0xb4      ,0xb4},
    {0x380E     ,0x2    ,0x4    ,0x3        ,0x1            ,0x3        ,0x3       ,0x6       ,0x7},
    {0x380F     ,0x8e   ,0x2a   ,0xca       ,0xea           ,0xd0       ,0x1a      ,0xd0     ,0xb0},
    {0x3818     ,0xc2   ,0xc1   ,0xc1       ,0xc2           ,0xc1       ,0xc1      ,0xc0      ,0xc0},
    {0x381a     ,0x0    ,0x3c   ,0x3c       ,0x3c           ,0x3c       ,0x3c      ,0x3c      ,0x3c},

    {0x381c     ,0x20   ,0x30   ,0x20       ,0x20           ,0x20       ,0x30      ,0x31      ,0x20},
    {0x381d     ,0xa    ,0xee   ,0xa        ,0xa            ,0xa        ,0xda      ,0x16      ,0xa},
    {0x381e     ,0x1    ,0x5    ,0x1        ,0x1            ,0x1        ,0x6       ,0x5       ,0x1},
    {0x381f     ,0x20   ,0xc8   ,0x20       ,0x20           ,0x20       ,0x10      ,0x90      ,0x20},

    {0x3820     ,0x0    ,0x0    ,0x0        ,0x0            ,0x0        ,0x0       ,0x0       ,0x0},
    {0x3821     ,0x0    ,0x20   ,0x0        ,0x0            ,0x0        ,0x0       ,0x0       ,0x0},
    {0x3824     ,0x23   ,0x23   ,0x23       ,0x23           ,0x22       ,0x22      ,0x22      ,0x22},
    {0x3825     ,0x26   ,0x2A   ,0x7A       ,0x7A           ,0x52       ,0x52      ,0x52      ,0x52},
    {0x3010     ,0x10   ,0x30   ,0x20       ,0x10           ,0x10       ,0x10      ,0x10      ,0x20},//vga:10=60fps, 30=30fps, 70=15fps
                                                                                                      //5m :10=15fps, 30=7.5fps, 70=3.75fps                                                                                 //720p:10=60fps, 30=30fps, 70=15fps
    {0x5002     ,0x2    ,0x0    ,0x0        ,0x0            ,0x0        ,0x0       ,0x0       ,0x0},
    {0x5901     ,0x4    ,0x0    ,0x0        ,0x0            ,0x0        ,0x0       ,0x0       ,0x0},
    {0x3713     ,0x92   ,0x92   ,0x92       ,0x92           ,0x22       ,0x22      ,0x22      ,0x22},
    {0x3714     ,0x17   ,0x17   ,0x17       ,0x17           ,0x27       ,0x27      ,0x27      ,0x27},
};

const UINT32 Sensor_Mode_1280x480[][2]=
{
//   Addr,      1280x480
    {0x3200    ,0x0   },
    {0x3201    ,0xff  },
    {0x3202    ,0xff  },
    {0x3203    ,0xff  },
    {0x3212    ,0x0   },
    {0x3613    ,0x44  },
    {0x3621    ,0xaf  },
    {0x3703    ,0x98  },
    {0x3705    ,0xdc  },
    {0x370a    ,0x81  },
    {0x370c    ,0xc8  },
    {0x370d    ,0x42  },
    {0x3713    ,0x92  },
    {0x3714    ,0x17  },
    {0x3803    ,0x6   },
    {0x3804    ,0x5   },
    {0x3805    ,0x0   },
    {0x3806    ,0x1   },
    {0x3807    ,0xe0  },
    {0x3808    ,0x5   },
    {0x3809    ,0x0   },
    {0x380a    ,0x1   },
    {0x380b    ,0xe0  },
    {0x380C    ,0x8   },
    {0x380D    ,0xe7  },
    {0x380E    ,0x1   },
    {0x380F    ,0xee  },
    {0x3818    ,0xc2  },
    {0x381a    ,0x0   },
    {0x381c    ,0x20  },
    {0x381d    ,0xa   },
    {0x381e    ,0x1   },
    {0x381f    ,0x20  },
    {0x3820    ,0x0   },
    {0x3821    ,0x0   },
    {0x3824    ,0x23  },
    {0x3825    ,0x18  },
    {0x3010    ,0x0   },
    {0x3011    ,0x0e  },
    {0x5002    ,0x0   },
    {0x5901    ,0x0   },
    {0x3010    ,0x10  },
    {0x3212    ,0x10  },
    {0x3212    ,0xa0  },
};

UINT32 RegValue;
void Sensor_config(UINT32 SenModeSel)
{
    debug_msg("\r\nSensor_config...\r\n");

    Sensor_WriteReg(0x3008, 0x82);
    TimerDelayMs(100);
    Sensor_WriteReg(0x3008, 0x42);
    TimerDelayMs(100);
#if 0
    while(1)
    {
        DelayMs(1000);
    }
#endif
    Sensor_Init();
#if 0
    if(SenModeSel == _Vid_Prv_Resolution_1280x720 ||
       SenModeSel == _Vid_Prv_Resolution_1440x1080 ||
       SenModeSel == _Vid_Prv_Resolution_1280x480 ||
       SenModeSel == _Vid_Prv_Resolution_848x480)
        Sensor_Init_720P();
#endif
    Sensor_SetMode(SenModeSel);
}

void Sensor_Init(void)
{
    UINT32 idx, table_size,tmp;

    table_size = (sizeof(Sensor_Param)/(4*2));
    debug_msg("sensor init...\r\n");
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg(Sensor_Param[idx][0], Sensor_Param[idx][1]);
#if 1
        RegValue = Sensor_ReadReg(Sensor_Param[idx][0]);
        if ( RegValue != Sensor_Param[idx][1] )
        {
            debug_err(("^RReg[0x%04x] = %02x %02x\r\n", Sensor_Param[idx][0], Sensor_Param[idx][1], RegValue));
            debug_err(("Key [asen c] to continue\r\n"));


        }
#endif
    }

    /*
    // Marked if using internal LDO
    //#NT#2010/09/09#ethanlau -begin
    //#NT# For Mustek FDV350 external LDO setting
    tmp = Sensor_ReadReg(0x3030);
    tmp |= 0x0020;
    Sensor_WriteReg(0x3030,tmp);
    //#NT#2010/09/09#ethanlau -end
    */
    debug_msg("sensor init finished...\r\n");
}


void Sensor_SetMode(UINT8 ucMode)
{
    UINT8 idx, table_size,_tmp;
    //#NT#20090907#ethanlau -begin
    _SEN_MODE_ = ucMode;
    //#NT#20090907#ethanlau -end
    if(ucMode>=SEN_MODE_MAX)
    {
        debug_err(("no such sensor mode\r\n"));
        return;
    }

        debug_err(("^RSensor_SetMode %d\r\n", ucMode));

    if(ucMode == SEN_MODE_1280_480)
    {
        table_size = (sizeof(Sensor_Mode_1280x480)/(4*2));
        for(idx=0; idx<table_size; idx++)
        {
            Sensor_WriteReg(Sensor_Mode_1280x480[idx][SEN_MODE_ADDR], Sensor_Mode_1280x480[idx][1]);
        }
        //sensor mode is configed by grouping parameter,need to delay some time to let sensor setting worked.
        TimerDelayMs(300);
    }
    else
    {
        table_size = (sizeof(Sensor_Mode)/(4*SEN_MODE_MAX));
        for(idx=0; idx<table_size; idx++)
        {
            Sensor_WriteReg(Sensor_Mode[idx][SEN_MODE_ADDR], Sensor_Mode[idx][ucMode]);
#if 1
            RegValue = Sensor_ReadReg(Sensor_Mode[idx][SEN_MODE_ADDR]);
            if ( RegValue != Sensor_Mode[idx][ucMode] )
            {
                debug_err(("^RReg[0x%04x] = %02x %02x\r\n", Sensor_Mode[idx][SEN_MODE_ADDR], Sensor_Mode[idx][ucMode], RegValue));
                debug_err(("Key [asen c] to continue\r\n"));

            }
#endif
        }
    }
    if(ucMode == SEN_MODE_FULL)
    {
        Sensor_WriteReg(0x350d,0);
        Sensor_WriteReg(0x350c,0);
    }

    //#NT#2010/10/15#ethanlau -begin
    //#NT# for dv565 demo only
    //#NT#2011/01/13#JJ Huang -begin
    //#NT# OV5653 Sensor Flip Switch
    #ifdef OV5653_630_HVFlip
    //For dv565, HV-flip
    _tmp = Sensor_ReadReg(0x3818);
    _tmp = _tmp|0x20;
    Sensor_WriteReg(0x3818,_tmp);
    #else
    //For Normal(HJT/TPK/EVB), no-flip
    #endif
    //#NT#2011/01/13#JJ Huang -end
    //#NT#2010/10/15#ethanlau -end

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
