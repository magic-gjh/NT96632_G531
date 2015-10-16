
#include "sensor_model.h"
#if MI5100_TVP5150_630
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"
#include    "cal_api.h"

//#NT#2008/09/30#YC Peng -begin
//#LSC PC calibration
#define NT96630_LC_ENABLE   1
void Sensor_SetLenc(void);
//#NT#2008/09/30#YC Peng -end

#define SEN_READ_ID 0xbb
#define SEN_WRITE_ID 0xba
//#define SEN_READ_ID 0x91
//#define SEN_WRITE_ID 0x90

const UINT32 Sensor_Param[][2] =
{
#if 0
    {0x08, 0x0000},
    {0x09, 0x00C8},//shutter lower
    {0x29, 0x0481},//RESERVED
    {0x3E, 0x0087},//RESERVED
    {0x3F, 0x0007},//RESERVED
    {0x41, 0x0003},//RESERVED
    {0x4B, 0x0000},
    {0x5F, 0x1C16},//RESERVED
    {0x73, 0x0300},//RESERVED
    {0x75, 0x4B08},//RESERVED
    {0x76, 0x664C},//RESERVED
    {0x78, 0xA567}//RESERVED
#else //orig
    {0x05, 0x0000},
//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
//    {0x06, 0x0019},
    {0x06, 0x008f},
//#NT#2010/06/10#Jarkko Chang -end
    {0x08, 0x0000},
    {0x09, 0x00C8},//shutter lower
    {0x0A, 0x8000},// For avoid timing issue, add by Iyen
//    {0x20, 0xC840},//Disable Column sum
//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
//    {0x20, 0x0840},//Disable Column sum
    {0x20, 0x0860},//Enable Column sum
    //{0x20, 0xc860},//Enable Column sum,flip H,V
//#NT#2010/06/10#Jarkko Chang -end
    {0x29, 0x0481},//RESERVED
    {0x3E, 0x0087},//RESERVED
    {0x3F, 0x0007},//RESERVED
    {0x41, 0x0003},//RESERVED
    {0x4B, 0x0028},// For avoid color not saturation issue, add by Iyen
    {0x5F, 0x1C16},//RESERVED
    {0x62, 0x0000},
    {0x2B, 0x0008},//Green1_Gain
    {0x2C, 0x0008},//Blue_Gain
    {0x2D, 0x0008},//Red_Gain
    {0x2E, 0x0008},//Green2_Gain
#endif
};

//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
const UINT32 MI5100_PLL[][2] =
{
#if 1   //in: 12Mhz out: 96Mhz

    {0x11, 0x1000},
    {0x12, 0x0001},
    {0x10, 0x0053},
#else   //in: 12Mhz out: 68Mhz
    {0x11, 0x1100},
    {0x12, 0x0002},
    {0x10, 0x0053},
#endif
};
//#NT#2010/06/10#Jarkko Chang -end

const UINT32 LSCTbl[][2] =
{
//#NT#2011/01/11#Jarkko Chang -begin
    {0xB7, 0x0052}, 	//  FX_RED
    {0xBB, 0x0037}, 	//  FY_RED
    {0xBF, 0x0DC3}, 	//  DF_DX_RED
    {0xC3, 0x0E2F}, 	//  DF_DY_RED
    {0xC7, 0xB9F1}, 	//  SECOND_DERIV_ZONE_0_RED
    {0xCB, 0x2009}, 	//  SECOND_DERIV_ZONE_1_RED
    {0xCF, 0x391B}, 	//  SECOND_DERIV_ZONE_2_RED
    {0xD3, 0x301E}, 	//  SECOND_DERIV_ZONE_3_RED
    {0xD7, 0x2C19}, 	//  SECOND_DERIV_ZONE_4_RED
    {0xDB, 0x3716}, 	//  SECOND_DERIV_ZONE_5_RED
    {0xDF, 0x3226}, 	//  SECOND_DERIV_ZONE_6_RED
    {0xE3, 0xA0C9}, 	//  SECOND_DERIV_ZONE_7_RED
    {0xE8, 0xE6D6}, 	//  CF_34_R
    {0xE7, 0xFFD0}, 	//  CF_12_R
    {0xB8, 0x0048}, 	//  FX_GREEN
    {0xBC, 0x0024}, 	//  FY_GREEN
    {0xC0, 0x0D0E}, 	//  DF_DX_GREEN
    {0xC4, 0x0E10}, 	//  DF_DY_GREEN
    {0xC8, 0xE80E}, 	//  SECOND_DERIV_ZONE_0_GREEN
    {0xCC, 0x2D0F}, 	//  SECOND_DERIV_ZONE_1_GREEN
    {0xD0, 0x1514}, 	//  SECOND_DERIV_ZONE_2_GREEN
    {0xD4, 0x3313}, 	//  SECOND_DERIV_ZONE_3_GREEN
    {0xD8, 0x0D0A}, 	//  SECOND_DERIV_ZONE_4_GREEN
    {0xDC, 0x2C16}, 	//  SECOND_DERIV_ZONE_5_GREEN
    {0xE0, 0x1118}, 	//  SECOND_DERIV_ZONE_6_GREEN
    {0xE4, 0xE5D9}, 	//  SECOND_DERIV_ZONE_7_GREEN
    {0xEA, 0x6D6B}, 	//  CF_34_G1
    {0xE9, 0xFFBF}, 	//  CF_12_G1
    {0xB9, 0x0045}, 	//  FX_GREEN2
    {0xBD, 0x0023}, 	//  FY_GREEN2
    {0xC1, 0x0DC5}, 	//  DF_DX_GREEN2
    {0xC5, 0x0D54}, 	//  DF_DY_GREEN2
    {0xC9, 0x1BF6}, 	//  SECOND_DERIV_ZONE_0_GREEN2
    {0xCD, 0x1F12}, 	//  SECOND_DERIV_ZONE_1_GREEN2
    {0xD1, 0x101C}, 	//  SECOND_DERIV_ZONE_2_GREEN2
    {0xD5, 0x320F}, 	//  SECOND_DERIV_ZONE_3_GREEN2
    {0xD9, 0x120C}, 	//  SECOND_DERIV_ZONE_4_GREEN2
    {0xDD, 0x2016}, 	//  SECOND_DERIV_ZONE_5_GREEN2
    {0xE1, 0x1D1E}, 	//  SECOND_DERIV_ZONE_6_GREEN2
    {0xE5, 0xD4DB}, 	//  SECOND_DERIV_ZONE_7_GREEN2
    {0xEC, 0x567F}, 	//  CF_34_G2
    {0xEB, 0xFFB2}, 	//  CF_12_G2
    {0xBA, 0x003E}, 	//  FX_BLUE
    {0xBE, 0x0016}, 	//  FY_BLUE
    {0xC2, 0x0D85}, 	//  DF_DX_BLUE
    {0xC6, 0x0ECA}, 	//  DF_DY_BLUE
    {0xCA, 0xE108}, 	//  SECOND_DERIV_ZONE_0_BLUE
    {0xCE, 0x3816}, 	//  SECOND_DERIV_ZONE_1_BLUE
    {0xD2, 0x0610}, 	//  SECOND_DERIV_ZONE_2_BLUE
    {0xD6, 0x2007}, 	//  SECOND_DERIV_ZONE_3_BLUE
    {0xDA, 0x0E05}, 	//  SECOND_DERIV_ZONE_4_BLUE
    {0xDE, 0x0315}, 	//  SECOND_DERIV_ZONE_5_BLUE
    {0xE2, 0x4A1F}, 	//  SECOND_DERIV_ZONE_6_BLUE
    {0xE6, 0x82FF}, 	//  SECOND_DERIV_ZONE_7_BLUE
    {0xEE, 0x7F7F}, 	//  CF_34_B
    {0xED, 0xDCC0}, 	//  CF_12_B
    {0xAE, 0x0020}, 	//  GLOBAL_OFFSET_FXY_FUNCTION
    {0xF, 0xA9F9}, 	//  LENS_CORRECTION_CONTROL
    {0xAF, 0x8000}, 	//  X2_FACTORS
    {0xEF, 0x0028}, 	//  CORNER_FACTOR_CONTROL
    {0xB0, 0x0C14}, 	//  ZONE_X1Y1
    {0xB1, 0x1A28}, 	//  ZONE_X2Y2
    {0xB2, 0x263C}, 	//  ZONE_X3Y3
    {0xB3, 0x4664}, 	//  ZONE_X4Y4
    {0xB4, 0x567A}, 	//  ZONE_X5Y5
    {0xB5, 0x688E}, 	//  ZONE_X6Y6
    {0xB6, 0x3450}, 	//  ZONE_CXCY
//#NT#2011/01/11#Jarkko Chang -end
};

//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
const UINT32 Sensor_Mode[][SEN_MODE_MAX]=
{
//   Addr,  VGA,    720P,   1440,   Full
#if 1
    {0x01,  0x0036, 0x0036, 0x0036, 0x0036},//row start
    {0x02,  0x0010, 0x0010, 0x0010, 0x0010},//column start
    {0x03,  0x079d, 0x079d, 0x079d, 0x079D},//row size
    {0x04,  0x0A2f, 0x0A2f, 0x0A2f, 0x0A2F},//column size
#if 0   //no binning
    {0x22,  0x0013, 0x0001, 0x0001, 0x0000},// Row binning(no binning)
    {0x23,  0x0013, 0x0001, 0x0001, 0x0000},// Column binning(no binning)
#else   //h/v binning
#if _HIGH_ISO_

    {0x22,  0x0013, 0x0011, 0x0011, 0x0000},// Row binning(no binning)
    {0x23,  0x0013, 0x0033, 0x0033, 0x0000},// Column binning
 #else
    {0x22,  0x0013, 0x0011, 0x0011, 0x0000},// Row binning(no binning)
    {0x23,  0x0013, 0x0011, 0x0011, 0x0000},// Column binning
 #endif
 
#endif
    {0x1E,  0x4406, 0x4406, 0x4406, 0x4406}
#else
    {0x01,  0x0016, 0x0036},//row start
    {0x02,  0x0010, 0x0010},//column start
    {0x03,  0x07ad, 0x079D},//row size
    {0x04,  0x0A2f, 0x0A2F},//column size
    {0x22,  0x0013, 0x0000},// Row binning 2X
    {0x23,  0x0013, 0x0000},// Column binning 2X
    {0x1E,  0x4406, 0x4406}
#endif
};
//#NT#2010/06/10#Jarkko Chang -end

void Sensor_config(UINT32 SenModeSel)
{
    debug_err(("Sensor_config...\r\n"));

    Sensor_WriteReg8bit(0x0100, 0x00);
    Sensor_WriteReg8bit(0x0104, 0x01);
    Sensor_Init();
    Sensor_WriteReg8bit(0x0104, 0x00);
    Sensor_WriteReg8bit(0x0100, 0x01);

    Sensor_SetMode(SenModeSel);
//    Sensor_SetMode(SEN_MODE_VGA);
}

//#NT#2009/06/30#YC Peng -begin
//#LSC PC calibration
void Sensor_SetLenc(void)
{
#if NT96630_LC_ENABLE
    UINT32 idx,tempReg, table_size;


    #if 1   //load default lsc parameters
    table_size = (sizeof(LSCTbl)/(4*2));
    debug_err(("Default LSC...\r\n"));
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg(LSCTbl[idx][0], LSCTbl[idx][1]);
    }
    #else
         Cal_Struct *CalInfo;

    CalInfo = Cal_GetCalibrationData();
    if (CalInfo->CalStatus_LSC == _LSC_Status)
    {
        debug_err(("Sensor_SetLenc\r\n"));
        for (idx = 0; idx < CalInfo->Cal_LSC_TableTotalCnt*2; idx += 2)
        {
            if( *((UINT16 *)CalInfo->Cal_LSC_Addr + idx) == 0x0F)
            {
                //enable lens shadding comp register
                tempReg=*((UINT16 *)CalInfo->Cal_LSC_Addr + idx + 1);
                tempReg=(tempReg|0x8000);
                Sensor_WriteReg(0x0F,tempReg);
            }
            else
            {
                Sensor_WriteReg(*((UINT16 *)CalInfo->Cal_LSC_Addr + idx), *((UINT16 *)CalInfo->Cal_LSC_Addr + idx + 1));
//                debug_err(("LSC Reg=0x%02x 0x%04x\r\n", *((UINT16 *)CalInfo->Cal_LSC_Addr + idx), *((UINT16 *)CalInfo->Cal_LSC_Addr + idx + 1)));
            }
        }
    }
	#endif
    debug_err(("Sensor_SetLenc finish\r\n"));
	
#endif
}
//#NT#2011/01/11#Jarkko Chang -end

void Sensor_Init(void)
{
    UINT32 idx, table_size;


    table_size = (sizeof(MI5100_PLL)/(4*2));

    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg(MI5100_PLL[idx][0], MI5100_PLL[idx][1]);
    }

    table_size = (sizeof(Sensor_Param)/(4*2));
    debug_err(("sensor init...\r\n"));
    for(idx=0;idx<table_size;idx++)
    {
        Sensor_WriteReg(Sensor_Param[idx][0], Sensor_Param[idx][1]);
    }
    Sensor_SetLenc();
  
    debug_ind(("sensor init finished...\r\n"));
}

void Sensor_SetMode(UINT8 ucMode)
{
    UINT8 idx, table_size;

    table_size = (sizeof(Sensor_Mode)/(4*SEN_MODE_MAX));
    for(idx=0; idx<table_size; idx++)
    {
        Sensor_WriteReg(Sensor_Mode[idx][SEN_MODE_ADDR], Sensor_Mode[idx][ucMode]);
    }
    debug_err(("sensor mi5100 set mode %d finished...\r\n",ucMode));
}

void Sensor_WriteReg(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData1, ulData2;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = addr;
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
    UINT32      ulWriteAddr, ulReadAddr, ulReg1, ulData, ulData1, ulData2;
    i2c_lock();
    ulWriteAddr =SEN_WRITE_ID;
    ulReadAddr  =SEN_READ_ID;
    ulReg1      =addr;
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
