
#include "stdio.h"
#include "Gsensor.h"
#include "spi.h"
#include "DMARD07.h"



//void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc)
//{
  //  memcpy(pGsensorFunc,&DMARD07FP,sizeof(GSENSOR_FUNC));
//}


void DMARD07_WriteReg(UINT32 addr, UINT32 value)
{
	i2c_write((unsigned char)addr, (unsigned char)value,DMARD07_DEV_WRITE_ADDRESS);
}

unsigned char DMARD07_ReadReg(UINT32 addr)
{
	unsigned char value;
	
	value = i2c_read((unsigned char)addr,DMARD07_DEV_WRITE_ADDRESS);
	debug_err(("DMARD07_ReadReg(0x%02X) = 0x%02X\r\n",addr,value));
	return value;
}

static UINT32 DMARD07_GetDev(void)
{
	UINT32 ReadValue;

	ReadValue = DMARD07_ReadReg(WHO_AM_I);
    return ReadValue;
}

BOOL DMARD07_GsensorInit(void)
{
 UINT32 i, erReturn = 0;


    // Get LIS331DL device ID
    erReturn = DMARD07_GetDev();
    if (erReturn != DMARD07_Dev)
    {
        debug_msg("DMARD07 Dev:%x\r\n",erReturn);
        return FALSE;
    }
	DMARD07_ReadReg(0x53); //sw reset

	// init setting
	// Low Power:32Hz, XYZ enable 
	DMARD07_WriteReg(0x44, 0x47);
	// xy enable
	//DMARD07_WriteReg(0x44, 0x46);
	DMARD07_ReadReg(0x44);
	
	// 2G mode, High Pass Filter for INT1, Low pass filter for data
	//DMARD07_WriteReg(0x45, 0x24);
	// 2G mode, High Pass Filter for INT1, High pass filter for data
//	DMARD07_WriteReg(0x45, 0x14);
	// 2G mode, Low Pass Filter for INT1, High pass filter for data
	//DMARD07_WriteReg(0x45, 0x10);
	
	// 4G mode, High Pass Filter for INT1, High pass filter for data
	DMARD07_WriteReg(0x45, 0x54);	
	//  High-pass Filter Cutoff for 0.6 Hz 
	DMARD07_WriteReg(0x46, 0x00);
	
	// No latch, INT SRC1 enable, active 1 
	DMARD07_WriteReg(0x47, 0x04);
	// No latch, INT SRC1 enable, active 0
	//DMARD07_WriteReg(0x47, 0x44);
	// latch int 1, int 2, Int SRC1 enable
	//DMARD07_WriteReg(0x47, 0x34);
	// latch int 1
	//DMARD07_WriteReg(0x47, 0x24);
	
	DMARD07_WriteReg(0x48, 0x00);
	
	//  OR all interrupt events, High-G XYZ 
	// xyz +G
	//DMARD07_WriteReg(0x4A, 0x2A);
	// xyz -G
	DMARD07_WriteReg(0x4A, 0x15);	
	// xy +-G
	//DMARD07_WriteReg(0x4A, 0x3c);
	// xy +G
	//DMARD07_WriteReg(0x4A, 0x28);
	// xy -G
	//DMARD07_WriteReg(0x4A, 0x14);
	// xyz +-G
	//DMARD07_WriteReg(0x4A, 0x3f);
	
	// Threshold = 755.9 mg 
	DMARD07_WriteReg(0x4C, 0x30);
	//  Duration = 47.1 ms 
	DMARD07_WriteReg(0x4D, 0x10);	

}


void DMARD07_GetGsensorData(Gsensor_Data *GS_Data)
{
}

void DMARD07_GetGsensorStatus(UINT32 status,UINT32 *uiData)
{
}

void DMARD07_SetSensitivity(UINT32 level)
{
 	UINT32 buf[2];	 

	switch(level)
	{
		case 0:
			DMARD07_WriteReg(0x44, 0x00);	
			DMARD07_WriteReg(0x47, 0x04);			
			debug_err(("GSensorSensitivity()==================>0\r\n"));
			break;	
		case 1:
			//DMARD07_WriteReg(0x44, 0x00);
			
			// Threshold = 755.9 mg 
			DMARD07_WriteReg(0x4C, 0x50);
			//  Duration = 1000/32hz=31.25 ms 
			DMARD07_WriteReg(0x4D, 0x03);				
			DMARD07_WriteReg(0x44, 0x47);
			DMARD07_WriteReg(0x47, 0x04);
			
			debug_err(("GSensorSensitivity()==================>1\r\n"));
			break;
		case 2:
			//DMARD07_WriteReg(0x44, 0x00);			
			
			// Threshold = 755.9 mg 
			DMARD07_WriteReg(0x4C, 0x40);
			//  Duration = 1000/32hz=31.25 ms 
			DMARD07_WriteReg(0x4D, 0x03);
			DMARD07_WriteReg(0x44, 0x47);
			DMARD07_WriteReg(0x47, 0x04);
			
			debug_err(("GSensorSensitivity()==================>2\r\n"));
			break;
		case 3:
			//DMARD07_WriteReg(0x44, 0x00);			
			
			// Threshold = 755.9 mg 
			DMARD07_WriteReg(0x4C, 0x20);
			//  Duration = 1000/32hz=31.25 ms 
			DMARD07_WriteReg(0x4D, 0x03);
			DMARD07_WriteReg(0x44, 0x47);
			DMARD07_WriteReg(0x47, 0x04);
			
			debug_err(("GSensorSensitivity()==================>3\r\n"));
			break;
		default:
			break;
	}
}

void DMARD07_ReadInterupt()
{
      UINT32 Val;   
      Val = DMARD07_ReadReg(0x4b);		
}

static BOOL bPreSpiOpen = FALSE;

void DMARD07_OpenInterface(void)
{

	gpio_setDir(GPIO_GSENSOR_CLK, GPIO_DIR_OUTPUT);
	gpio_setDir(GPIO_GSENSOR_DAT, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_GSENSOR_CLK);
	gpio_setPin(GPIO_GSENSOR_DAT);

	I2C_Init();
}

void DMARD07_CloseInterface(void)
{

}


GSENSOR_FUNC DMARD07FP = {
    DMARD07_GsensorInit,        // GsensorInit
    DMARD07_GetGsensorData,     // GetGsensorData
    DMARD07_GetGsensorStatus,   // GetGsensorStatus
    DMARD07_OpenInterface,      // OpenInterface
    DMARD07_CloseInterface,     // closeInterface
    DMARD07_SetSensitivity, //set sensitivity
    DMARD07_ReadInterupt, //read interupt when interupte happended
};


