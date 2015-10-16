
#include "stdio.h"
#include "Gsensor.h"
#include "spi.h"
#include "GMA301.h"


//void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc)
//{
 //   memcpy(pGsensorFunc,&GMA301FP,sizeof(GSENSOR_FUNC));
//}

void GMA301_WriteReg(UINT32 addr, UINT32 value)
{
	i2c_write((unsigned char)addr, (unsigned char)value,GMA301_DEV_WRITE_ADDRESS);
}

unsigned char GMA301_ReadReg(UINT32 addr)
{
	unsigned char value;
	
	value = i2c_read((unsigned char)addr,GMA301_DEV_WRITE_ADDRESS);
	//debug_err(("GMA301_ReadReg(0x%02X) = 0x%02X\r\n",addr,value));
	return value;
}

static UINT32 GMA301_GetDev(void)
{
	UINT32 ReadValue;

	ReadValue = GMA301_ReadReg(WHO_AM_I);
    return ReadValue;
}

BOOL GMA301_GsensorInit(void)
{
 UINT32 i, erReturn = 0;


    // Get LIS331DL device ID
    erReturn = GMA301_GetDev();
    if (erReturn != GMA301_Dev)
   {
        debug_msg("GMA301 Dev:%x\r\n",erReturn);
        return FALSE;
    }
		//init
		GMA301_WriteReg(0x21,0x52);//
		GMA301_WriteReg(0x00,0x02);//
		GMA301_WriteReg(0x00,0x12);//
		GMA301_WriteReg(0x00,0x02);//
		GMA301_WriteReg(0x00,0x82);//
		GMA301_WriteReg(0x00,0x02);//
		//GMA301_WriteReg(0x1F,0x28);//
		//GMA301_WriteReg(0x0C,0x8F);//
		//GMA301_WriteReg(0x00,0x06);//
		GMA301_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
		//interrupt setup
		GMA301_WriteReg(0x11,0x40);//IIC 0X07 for no pullup //0x06 High active  0x40 low active
		//GMA301_WriteReg(0x11,0x06);//IIC 0X06 for no pullup 
		//set Gsensor Level 
		//0x08-->0.5G 
		//0X10-->1G
		GMA301_WriteReg(0x38,0XFF);//
		GMA301_WriteReg(0x39,0X40);//10 1g 20 2g 30 3g 40 4g 50 5g 60 6g  (8 : 0.5g)
		
		//GMA301_WriteReg(0x0F,0x00);//
		//GMA301_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
		GMA301_WriteReg(0x1F,0x28);//To disable micro motion interrupt.

		//set active.
		GMA301_WriteReg(0x0C,0x8F);//
		GMA301_WriteReg(0x00,0x06);//
		delay(20); //2014-12-18 added 20ms delay for G-sensor active.
		
		//TimerDelayMs(10);
		//Latched reference data of micro motion.
		
		GMA301_ReadReg(0x12);
		GMA301_ReadReg(0x13);
		GMA301_ReadReg(0x14);
		GMA301_ReadReg(0x15);
		GMA301_ReadReg(0x16);
		GMA301_ReadReg(0x17);
		GMA301_ReadReg(0x18);
		GMA301_ReadReg(0x19);
		
		//GMA301_ReadReg(0x1A);
		//GMA301_ReadReg(0x1B);

		//GMA301_WriteReg(0x1F,0x28);//To enable micro motion interrupt.
		//delay(100); //2014_0819 added 1ms delay for micro motion setup itself.

		//GMA301_WriteReg(0x0B,0x40);

		//GMA301_WriteReg(0x1F,0x38);//2014-12-19 To enable micro motion interrupt.
		GMA301_WriteReg(0x1F,0x18);//2014-12-19 To enable micro motion interrupt.
		delay(1); //2014-12-18 added 1ms delay for micro motion setup itself.
		
		//Sensitivity parameter adjust
		GMA301_WriteReg(0x0D,0x44);//2014-12-18 Set output data rate to 25HZ. ( 0x70, 0x60, 0x50, 0x40 only)
		GMA301_WriteReg(0x0F,0x05);//2014-12-19 4 tap.( 0x00~0x05 only )
		
		GMA301_ReadReg(0x1C);//2014-12-18 Clear interrupt flag.
		GMA301_ReadReg(0x1D);

		GMA301_WriteReg(0x0E,0x1C);//To enable interrupt.//parking monitor
		//GSensor_I2C_ReadReg_2B(0x1C,value);

}


void GMA301_GetGsensorData(Gsensor_Data *GS_Data)
{
       INT16 data_hi,data_low;
	 INT16 x,y,z;
	 UINT32 value;
	 
	value=GMA301_ReadReg(0x12);
	if(value!=0x55)
	{
		return;
	}
	GMA301_ReadReg(0x13);
	
	data_low=GMA301_ReadReg(0x14);
	data_hi=GMA301_ReadReg(0x15);
	
      if(0)//((data_hi & 0xf0 == 0xf0) ) 
	{
		x = data_hi << 8 |data_low;
		x = 0xffff - x + 1;
		x &= 0x1ff;
		x = -x;
	}
	else
	{
		x = data_hi << 8 |data_low;
	}
	
	data_low=GMA301_ReadReg(0x16);
	data_hi=GMA301_ReadReg(0x17);
      if(0)//((data_hi & 0xf0 == 0xf0) ) 
	{
		y=data_hi<<8|data_low;	
		y = 0xffff - y + 1;
		y &= 0x1ff;		
		y = -y;
	}
	else
	{
		y=data_hi<<8|data_low;	
	}	
	
	data_low=GMA301_ReadReg(0x18);
	data_hi=GMA301_ReadReg(0x19);
      if(0)//((data_hi & 0xf0 == 0xf0) ) 
	{
		z=data_hi<<8|data_low;	
		z = 0xffff - z + 1;
		z &= 0x1ff;		
		z = -z;
	}
	else
	{
		z=data_hi<<8|data_low;	
	}		
	//debug_msg("x-y-z: %d,%d,%d\r\n",x,y,z);
	GS_Data->Axis.Xacc=x;
	GS_Data->Axis.Yacc=y;
	GS_Data->Axis.Zacc=z;	

	//return TRUE;
}

void GMA301_GetGsensorStatus(UINT32 status,UINT32 *uiData)
{
}

void GMA301_SetSensitivity(UINT32 level)
{
 	UINT32 buf[2];	 

	switch(level)
	{
		case 0:
			GMA301_WriteReg(0x0E,0x00);//2014-12-18 To disable interrupt.//parking monitor
			GMA301_ReadReg(0x1C);//2014-12-18 Clear interrupt flag.
			GMA301_ReadReg(0x1D);
			
			debug_err(("GSensorSensitivity --off--\r\n"));
			break;	
		case 1:
			GMA301_WriteReg(0x39,0x60);//2014-12-18 set 7g
			GMA301_WriteReg(0x0E,0x1C);//To enable interrupt.//parking monitor
			
			debug_err(("GSensorSensitivity --low--\r\n"));

			break;
		case 2:
			GMA301_WriteReg(0x39,0x18);//2014-12-18 set 4g
			GMA301_WriteReg(0x0E,0x1C);//To enable interrupt.//parking monitor			
			debug_err(("GSensorSensitivity --meddile--\r\n"));

			break;
		case 3:
			GMA301_WriteReg(0x39,0x08);//2014-12-18 set 0.5g
			GMA301_WriteReg(0x0E,0x1C);//To enable interrupt.//parking monitor			
			debug_err(("GSensorSensitivity --high--\r\n"));

			break;
		default:
			break;
	}
}

void GMA301_ReadInterupt()
{
	GMA301_WriteReg(0x0E,0x00);//0x1C//0x00 // 0x00:disable interrupt
	GMA301_WriteReg(0x1F,0x28);//To disable micro motion interrupt.
	//Latched reference data of micro motion.
	GMA301_ReadReg(0x12);
	GMA301_ReadReg(0x13);
	GMA301_ReadReg(0x14);
	GMA301_ReadReg(0x15);
	GMA301_ReadReg(0x16);
	GMA301_ReadReg(0x17);
	GMA301_ReadReg(0x18);
	GMA301_ReadReg(0x19);

	GMA301_WriteReg(0x1F,0x18);//2014-12-19 To enable micro motion interrupt.
	delay(1);
	GMA301_ReadReg(0x1C);//clr int
	GMA301_ReadReg(0x1D);
	GMA301_WriteReg(0x0E,0x1C);//To enable Z, Y, and X interrupt.
}

static BOOL bPreSpiOpen = FALSE;

void GMA301_OpenInterface(void)
{

	gpio_setDir(GPIO_GSENSOR_CLK, GPIO_DIR_OUTPUT);
	gpio_setDir(GPIO_GSENSOR_DAT, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_GSENSOR_CLK);
	gpio_setPin(GPIO_GSENSOR_DAT);

	I2C_Init();
}

void GMA301_CloseInterface(void)
{

}


GSENSOR_FUNC GMA301FP = {
    GMA301_GsensorInit,        // GsensorInit
    GMA301_GetGsensorData,     // GetGsensorData
    GMA301_GetGsensorStatus,   // GetGsensorStatus
    GMA301_OpenInterface,      // OpenInterface
    GMA301_CloseInterface,     // closeInterface
    GMA301_SetSensitivity, //set sensitivity
    GMA301_ReadInterupt, //read interupt when interupte happended
};


