
#include "sensor_model.h"
#if A1040_630
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"

#define SEN_READ_ID 0x91
#define SEN_WRITE_ID 0x90

//#NT#20090907#ethanlau -begin
_SEN_MODE _SEN_MODE_;
//#NT#20090907#ethanlau -end

extern void        DelayMs(unsigned long ms);

const UINT32 Sensor_Param[][2] =
{
{0x301A, 0x0234},             // RESET_REGISTER
//  POLL  COMMAND_REGISTER::HOST_COMMAND_1 =>  0x01, ..., 0x00 (10 reads)
{0xFFFF, 30},  // delay 10ms
{0x098E, 0x0000},             // LOGICAL_ADDRESS_ACCESS
{0xC97E, 0x01BA},             // CAM_SYSCTL_PLL_ENABLE
{0xC980, 0x0120},             // CAM_SYSCTL_PLL_DIVIDER_M_N
{0xC982, 0x0700},             // CAM_SYSCTL_PLL_DIVIDER_P
{0x098E, 0x0000},             // LOGICAL_ADDRESS_ACCESS
{0xC800, 0x0004},             // CAM_SENSOR_CFG_Y_ADDR_START
{0xC802, 0x0004},             // CAM_SENSOR_CFG_X_ADDR_START
{0xC804, 0x03CB},             // CAM_SENSOR_CFG_Y_ADDR_END
{0xC806, 0x050B},             // CAM_SENSOR_CFG_X_ADDR_END
{0xC808, 0x0234},
{0xC80A, 0x9340},             // CAM_SENSOR_CFG_PIXCLK
{0xC80C, 0x0001},             // CAM_SENSOR_CFG_ROW_SPEED
{0xC80E, 0x00DB},             // CAM_SENSOR_CFG_FINE_INTEG_TIME_MIN
{0xC810, 0x05B3},             // CAM_SENSOR_CFG_FINE_INTEG_TIME_MAX
{0xC812, 0x03EE},             // CAM_SENSOR_CFG_FRAME_LENGTH_LINES
{0xC814, 0x0636},             // CAM_SENSOR_CFG_LINE_LENGTH_PCK
{0xC816, 0x0060},             // CAM_SENSOR_CFG_FINE_CORRECTION
{0xC818, 0x03C3},             // CAM_SENSOR_CFG_CPIPE_LAST_ROW
{0xC834, 0x0000},             // CAM_SENSOR_CONTROL_READ_MODE

//{0xC85C, 0x0000},             // CAM_CROP_CROPMODE
{0xC854, 0x0000},
{0xC856, 0x0000},
{0xC858, 0x0500},
{0xC85A, 0x03C0},
{0xC85C, 0x0003},             // CAM_CROP_CROPMODE

{0xC868, 0x0500},             // CAM_OUTPUT_WIDTH
{0xC86A, 0x03C0},             // CAM_OUTPUT_HEIGHT

{0xE801, 0x0000},             // AUTO_BINNING_MODE
{0x098E, 0x0000},             // LOGICAL_ADDRESS_ACCESS
{0xC984, 0x8040},             // CAM_PORT_OUTPUT_CONTROL
{0x001E, 0x0777},             // PAD_SLEW

{0x098E, 0x2802},             // LOGICAL_ADDRESS_ACCESS
{0xA804, 0x0000},             // AE_TRACK_ALGO
{0xAC02, 0x0000},             // awb_mode
{0xBC02, 0x0000},             // ll_mode
{0xC909, 0x0000},             // CAM_AWB_AWBMODE
{0xC924, 0x0000},             // cam_ll_llmode
{0xC878, 0x0000},             // CAM_AET_AEMODE

{0x098E, 0x486C},             // LOGICAL_ADDRESS_ACCESS
{0xC86C, 0x0210},             // CAM_OUTPUT_FORMAT

{0x098E, 0x4834},             // LOGICAL_ADDRESS_ACCESS
{0xC834, 0x0003},             // CAM_SENSOR_CONTROL_READ_MODE

#if 0
{0x316A, 0x8270},
{0x316C, 0x8270},
{0x3ED0, 0x3605},
{0x3ED2, 0x77CF},
{0x316E, 0x8203},
{0x3180, 0x87FF},
{0x30D4, 0x6080},
{0xA802, 0x0008},
#else
//black sun, bobby 1018
{0x316A, 0x8270},
{0x316C, 0x8270},
{0x3ED0, 0x2305},
{0x3ED2, 0x77CF},
{0x316E, 0x8202},
{0x3180, 0x87FF},
{0x30D4, 0x6080},
{0xA802, 0x0008},
#endif

{0xDC00, 0x2831},             // SYSMGR_NEXT_STATE
{0x0080, 0x8002}              // COMMAND_REGISTER
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
//   Addr,      VGA,    720p_30 1280*960    1280*480        2560*960    Full
    {0x3011     ,0x10   ,0x17   ,0x10       ,0x0e           ,0x0E       ,0x10},
    {0x401d     ,0x08   ,0x08   ,0x08       ,0X08           ,0X08       ,0x08},
//#NT#2011/01/17#JJ Huang -begin
//#NT# For x4 Bining
    {0x3613     ,0xc4   ,0xc4   ,0xc4       ,0xc4           ,0xc4       ,0x44},
//#NT#2011/01/17#JJ Huang -end
    {0x3621     ,0xaf   ,0xaf   ,0xaf       ,0xaf           ,0x2f       ,0x2f},
    {0x3703     ,0x9a   ,0x9a   ,0xb0       ,0xb0           ,0xe6       ,0xe6},
    {0x3705     ,0xdc   ,0xda   ,0xda       ,0xda           ,0xda       ,0xda},
    {0x370a     ,0x81   ,0x80   ,0x80       ,0x80           ,0x80       ,0x80},
    {0x370c     ,0xc8   ,0xc5   ,0xc5       ,0xc5           ,0xa0       ,0xa0},
    {0x370d     ,0x42   ,0x42   ,0x42       ,0x42           ,0x4        ,0x4},
    {0x3713     ,0x2d   ,0x2d   ,0x2f       ,0x2f           ,0x2f       ,0x2f},
    {0x3803     ,0xa    ,0xe    ,0x8        ,0x08           ,0xe        ,0xe},
    {0x3804     ,0x2    ,0x5    ,0x5        ,0x5            ,0xa        ,0xA},
    {0x3805     ,0x80   ,0x00   ,0x00       ,0x00           ,0x00       ,0x20},
    {0x3806     ,0x1    ,0x2    ,0x3        ,0x1            ,0x3        ,0x7},
    {0x3807     ,0xe4   ,0xd0   ,0xc0       ,0xe0           ,0xc0       ,0x98},
    {0x3808     ,0x2    ,0x5    ,0x5        ,0x5            ,0xa        ,0xA},
    {0x3809     ,0x80   ,0x00   ,0x00       ,0x00           ,0x00       ,0x20},
    {0x380a     ,0x1    ,0x2    ,0x3        ,0x1            ,0x3        ,0x7},
    {0x380b     ,0xe4   ,0xd0   ,0xc0       ,0xe0           ,0xc0       ,0x98},
    {0x380C     ,0x9    ,0x8    ,0x8        ,0x8            ,0x13       ,0xc},
    {0x380D     ,0x90   ,0x60   ,0x98       ,0x98           ,0x20       ,0xb4},
    {0x380E     ,0x2    ,0x4    ,0x3        ,0x1            ,0x3        ,0x7},
    {0x380F     ,0x8e   ,0x2a   ,0xca       ,0xea           ,0xd0       ,0xb0},
    {0x3818     ,0xc2   ,0xc1   ,0xc1       ,0xc2           ,0xc1       ,0xc0},
    {0x381a     ,0x0    ,0x3c   ,0x3c       ,0x3c           ,0x3c       ,0x3c},
    {0x381c     ,0x20   ,0x30   ,0x20       ,0x20           ,0x20       ,0x20},
    {0x381d     ,0xa    ,0xee   ,0xa        ,0xa            ,0xa        ,0xa},
    {0x381e     ,0x1    ,0x5    ,0x1        ,0x1            ,0x1        ,0x1},
    {0x381f     ,0x20   ,0xc8   ,0x20       ,0x20           ,0x20       ,0x20},
    {0x3820     ,0x0    ,0x0    ,0x0        ,0x0            ,0x0        ,0x0},
    {0x3821     ,0x0    ,0x20   ,0x0        ,0x0            ,0x0        ,0x0},
    {0x3824     ,0x23   ,0x23   ,0x23       ,0x23           ,0x22       ,0x22},
    {0x3825     ,0x26   ,0x2A   ,0x7A       ,0x7A           ,0x52       ,0x52},
//    {0x3010     ,0x30   ,0x30   ,0x30       ,0x10           ,0x10       ,0x70},//vga:10=60fps, 30=30fps, 70=15fps
    {0x3010     ,0x10   ,0x30   ,0x20       ,0x10           ,0x10       ,0x20},//vga:10=60fps, 30=30fps, 70=15fps
                                                                                //5m :10=15fps, 30=7.5fps, 70=3.75fps                                                                                 //720p:10=60fps, 30=30fps, 70=15fps
    {0x5002     ,0x2    ,0x0    ,0x0        ,0x0            ,0x0        ,0x0},
    {0x5901     ,0x4    ,0x0    ,0x0        ,0x0            ,0x0        ,0x0},
    {0x3713     ,0x92   ,0x92   ,0x92       ,0x92           ,0x22       ,0x22},
    {0x3714     ,0x17   ,0x17   ,0x17       ,0x17           ,0x27       ,0x27},
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

void Sensor_config(UINT32 SenModeSel)
{
    debug_msg("\r\nSensor_config...\r\n");

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
        DelayMs(5);
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

#if 0
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
        }
    }
    if(ucMode == SEN_MODE_FULL)
    {
        Sensor_WriteReg(0x350d,0);
        Sensor_WriteReg(0x350c,0);
    }

    //#NT#2010/10/15#ethanlau -begin
    //#NT# for dv565 demo only
    _tmp = Sensor_ReadReg(0x3818);
    _tmp = _tmp|0x20;
    Sensor_WriteReg(0x3818,_tmp);
    //#NT#2010/10/15#ethanlau -end
#endif
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
