
/*
    Gsensor module driver

    This file is the driver of gsensor module

    @file       Gsensor.c
    @ingroup    mISYSGsensor
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "OSCommon.h"
#include "Gsensor.h"
#include "I2C.h"
#include "top.h"

static UINT32 uiGsensorTpye=GSENSOR_NONE;


#if 0 
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

void delay(UINT32 times) 
{
    for(;times > 0; times--)
        ;
}

#define I2C_DELAY delay(400)
#define I2C_DELAY_LONG delay(2000)



void I2C_Init()
{
	debug_err(("<<=====================I2C_Init=======================>>\r\n"));

	I2C_SCL_OUT;

	I2C_SDA_OUT;

	I2C_SCL_HIGH;
	
	I2C_DELAY;
	
	I2C_SDA_HIGH;
	
	I2C_DELAY;

}

void g_i2c_start(void)
{

	DEBUG_MSG(("<<=====================_i2c_start=======================>>\r\n"));
	I2C_SDA_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_DELAY;
	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_DELAY;
}

void g_i2c_stop(void)
{
	
	DEBUG_MSG(("<<=====================_i2c_stop=======================>>\r\n"));
	
	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_SDA_HIGH;
	I2C_DELAY_LONG;
}

unsigned char g_i2c_ack_detect(void)
{
	int i = 0;
	DEBUG_MSG(("<<=====================_i2c_ack_detect=======================>>\r\n"));

	I2C_SDA_HIGH;
	//I2C_DELAY;
	delay(20);
	I2C_SDA_IN; // SDA Input Mode
	I2C_SCL_HIGH;

	//I2C_DELAY;
	while(i<400)
	{
		i++;
		if (I2C_SDA_DETECT)
		{
			delay(2);
		}
		else
		{
			break;
		}
	}
	if (I2C_SDA_DETECT)
	{
		I2C_SDA_OUT;
		DEBUG_MSG(("<<=====================Ack Error!=======================>>\r\n"));
		return ERROR_CODE_FALSE; // false
	}

	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_SDA_OUT;
	return ERROR_CODE_TRUE; // true
}

void g_i2c_ack_send(void)
{

	DEBUG_MSG(("<<=====================_i2c_ack_send=======================>>\r\n"));

	I2C_SDA_LOW;
	I2C_DELAY;
	I2C_SCL_HIGH;
	I2C_DELAY;
	I2C_SCL_LOW;
	I2C_DELAY;
}
unsigned char g_i2c_write_byte(unsigned char data)
{
	unsigned char i;

	DEBUG_MSG(("<<=====================_i2c_write_byte=======================>>\r\n"));
	I2C_DELAY;
	for(i = 0; i< 8; i++)
	{
		if( (data << i) & 0x80) I2C_SDA_HIGH;
		else I2C_SDA_LOW;
		I2C_DELAY;
		I2C_SCL_HIGH;
		I2C_DELAY;
		I2C_SCL_LOW;
		I2C_DELAY;
	}

	if(g_i2c_ack_detect())
	{
		DEBUG_MSG(("<<=====================Acknoledge Error=======================>>\r\n"));
		return ERROR_CODE_FALSE;
	}
	return ERROR_CODE_TRUE;
}

unsigned char g_i2c_read_byte(void)
{
	unsigned char i, data;

	DEBUG_MSG(("<<=====================_i2c_read_byte=======================>>\r\n"));

	data = 0;
	//I2C_SDA_HIGH;
	I2C_DELAY;
	I2C_SDA_IN;
	for(i = 0; i< 8; i++)
	{
		data <<= 1;
		I2C_DELAY;
		I2C_SCL_HIGH;
		I2C_DELAY;
		if (I2C_SDA_DETECT) data |= 0x01;
		I2C_SCL_LOW;
		I2C_DELAY;
	}
	I2C_SDA_OUT;
	//g_i2c_ack_send();
	return data;
}

unsigned char i2c_write(unsigned char sub_addr, unsigned char buff,unsigned char device_addr)
{
	unsigned char i;
	//unsigned char device_addr = GMA301_DEV_WRITE_ADDRESS;

	DEBUG_MSG(("<<=====================_i2c_write=======================>>\r\n"));

	g_i2c_start();
	I2C_DELAY;
	if(g_i2c_write_byte(device_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	if(g_i2c_write_byte(sub_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_WRITE_ADDR;
	}
	if(g_i2c_write_byte(buff)) 
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Data=======================>>\r\n"));
		return ERROR_CODE_WRITE_DATA;
	}

	I2C_DELAY;
	g_i2c_stop();
	I2C_DELAY_LONG;
	return ERROR_CODE_TRUE;
}

unsigned char i2c_read(unsigned char sub_addr,unsigned char device_addr)
{
	unsigned char i;
	unsigned char buff;
	//unsigned char device_addr = GMA301_DEV_WRITE_ADDRESS;

	DEBUG_MSG(("<<=====================_i2c_read=======================>>\r\n"));	

	g_i2c_start();
	I2C_DELAY;
	if(g_i2c_write_byte(device_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	if(g_i2c_write_byte(sub_addr))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	g_i2c_start();
	I2C_DELAY;
	if(g_i2c_write_byte(device_addr+1))
	{
		g_i2c_stop();
		DEBUG_MSG(("<<=====================Write Error - Addr=======================>>\r\n"));
		return ERROR_CODE_READ_ADDR;
	}
	
	buff = g_i2c_read_byte();

	I2C_DELAY;
	g_i2c_stop();
	I2C_DELAY_LONG;
	return buff;
}



static BOOL   bGsensorOpened = FALSE;

static GSENSOR_FUNC     GsensorFunc = {
    NULL, // GsensorInit
    NULL, // GetGsensorData
    NULL, // GetGsensorStatus
    NULL, // OpenGsensorInterface
    NULL, // CloseGsensorInterface
};


BOOL gsensor_open(void)
{
   unsigned char value;

   if (bGsensorOpened==TRUE)
   {
     debug_msg("Gsensor open already\r\n");
     return TRUE;
   }

   bGsensorOpened=TRUE;

   // hook Gsensor functions
   //gsensor_getFP(&GsensorFunc);

   // open Gsensor interface
   gsensor_OpenInterface();
	
   value = i2c_read(0x0f,DMARD07_DEV_WRITE_ADDRESS);
   value = i2c_read(0x0f,DMARD07_DEV_WRITE_ADDRESS);
   value = i2c_read(0x0f,DMARD07_DEV_WRITE_ADDRESS);
   
   debug_msg("Gsnesor open value:%d \r\n",value);
   
   if(value==0x07)
   {
      uiGsensorTpye=GSENSOR_DMARD07;
    	memcpy(&GsensorFunc,&DMARD07FP,sizeof(GSENSOR_FUNC));
   }
   else 
   {
	   value = i2c_read(0x12,GMA301_DEV_WRITE_ADDRESS);
	   value = i2c_read(0x12,GMA301_DEV_WRITE_ADDRESS);
	   value = i2c_read(0x12,GMA301_DEV_WRITE_ADDRESS);
	   
         debug_msg("Gsnesor open value:%d \r\n",value);	   
	   if(value==0x55)
	   {
	      uiGsensorTpye=GSENSOR_GMA301;	   
	    	memcpy(&GsensorFunc,&GMA301FP,sizeof(GSENSOR_FUNC));
	   }   
	   else
	   {
   			
	        value = i2c_read(0x01,DA380_DEV_WRITE_ADDRESS);
	   		value = i2c_read(0x01,DA380_DEV_WRITE_ADDRESS);
	   		value = i2c_read(0x01,DA380_DEV_WRITE_ADDRESS);
			debug_msg("Gsnesor open value:%d \r\n",value);
			if(value==0x13)
	   		{  
	   		    debug_msg("magic_da380");
	      		uiGsensorTpye=GSENSOR_DA380;	   
	    		memcpy(&GsensorFunc,&DA380FP,sizeof(GSENSOR_FUNC));
	  		} 
			else
			{
      			uiGsensorTpye=GSENSOR_NONE;	   
	    		memcpy(&GsensorFunc,&GsensorNoneFP,sizeof(GSENSOR_FUNC));
			}
	   }
   }
   // Gsensor init
   gsensor_Init();
   debug_msg("Gsnesor open %d OK!!\r\n",uiGsensorTpye);

   return TRUE;
}

BOOL gsensor_close(void)
{

    if (bGsensorOpened==FALSE)
    {
        debug_msg("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    gsensor_CloseInterface();

    bGsensorOpened=FALSE;
    return TRUE;
}

BOOL gsensor_Init(void)
{
    debug_msg("magic_gsensor_Init1\r\n");
    if (GsensorFunc.GsensorInit != NULL)
    {
    	debug_msg("magic_gsensor_Init2\r\n");
        return GsensorFunc.GsensorInit();
    }
}

void gsensor_GetData(Gsensor_Data *GS_Data)
{

    if (GsensorFunc.GetGsensorData != NULL)
    {
        GsensorFunc.GetGsensorData(GS_Data);
    }
}

void gsensor_GetStatus(UINT32 status,UINT32 *uiData)
{

    if (GsensorFunc.GetGsensorStatus!= NULL)
    {
        GsensorFunc.GetGsensorStatus(status,uiData);
    }
}

void gsensor_OpenInterface(void)
{
   // if (GsensorFunc.GsensorOpenInterface != NULL)
    //{
  //      GsensorFunc.GsensorOpenInterface();
   // }
	gpio_setDir(GPIO_GSENSOR_CLK, GPIO_DIR_OUTPUT);
	gpio_setDir(GPIO_GSENSOR_DAT, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_GSENSOR_CLK);
	gpio_setPin(GPIO_GSENSOR_DAT);

	I2C_Init();   
}

void gsensor_CloseInterface(void)
{
    if (GsensorFunc.GsensorCloseInterface != NULL)
    {
        GsensorFunc.GsensorCloseInterface();
    } else {
        debug_msg("%s is NULL\r\n",__func__);
    }
}

void gsensor_SetSensitivity(UINT32 level)
{
    if (GsensorFunc.GsensorSetSensitivity != NULL)
    {
        GsensorFunc.GsensorSetSensitivity(level);
    }
}

void gsensor_ReadSensitivity(void)
{
    if (GsensorFunc.GsensorReadInterupt != NULL)
    {
        GsensorFunc.GsensorReadInterupt();
    }
}

UINT32 gsensor_gettype(void)
{
	return uiGsensorTpye;
}

