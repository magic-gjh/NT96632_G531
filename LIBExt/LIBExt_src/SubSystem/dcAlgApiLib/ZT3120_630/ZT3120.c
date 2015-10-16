
#include "sensor_model.h"
#if ZT3120_630
#include    "sif.h"
#include    "utility.h"
#include    "pll.h"
#include    "i2c.h"
#include    "cal_api.h"

#define ZT3120_CMD_DONE     0
#define ZT3120_CMD_RUNNING  1
#define ZT3120_CMD_ERROR    2


#define SEN_READ_ID 0x05
#define SEN_WRITE_ID 0x04

//#define SEN_READ_ID 0x07
//#define SEN_WRITE_ID 0x06

void fnZT3120MailBoxCommand(
	unsigned char ucPara1,
	unsigned char ucPara2,
	unsigned char ucPara3,
	unsigned char ucCmd);
unsigned char fnZT3120MailboxStatus();
unsigned char fnD2SetOperationMode(
	unsigned char ucMode,
	unsigned char ucResolution);

void Sensor_config(UINT32 SenModeSel)
{
    debug_err(("ZT3120 Sensor_config...\r\n"));

    TimerDelayMs(100);
    debug_err(("0x2b = %d\r\n", Sensor_ReadReg(0x2b)));
	while (fnZT3120MailboxStatus() == ZT3120_CMD_RUNNING)
	{
        TimerDelayMs(100);
		;
	}

    //.......
    Sensor_Init();
    //.......
    Sensor_SetMode(SenModeSel);
  	Sensor_WriteReg(0x0007, 15);
  	Sensor_WriteReg(0x002b, 8);

    debug_err(("Sensor_config end...\r\n"));
}

void Sensor_Init(void)
{

}

void Sensor_SetMode(UINT8 ucMode)
{
    if ( ucMode == MONITOR_MODE_DVI1 )
    {
        if ( gImageAlgInfo.Frequency == _POWERFreq_60Hz )
            fnD2SetOperationMode(0x01, 0x03);
        else
            fnD2SetOperationMode(0x01, 0x04);
    }
    if ( ucMode == MONITOR_MODE_DVI2 )
    {
        if ( gImageAlgInfo.Frequency == _POWERFreq_60Hz )
            fnD2SetOperationMode(0x02, 0x03);
        else
            fnD2SetOperationMode(0x02, 0x04);
    }
    if ( ucMode == MONITOR_MODE_DVI3 )
    {
        if ( gImageAlgInfo.Frequency == _POWERFreq_60Hz )
            fnD2SetOperationMode(0x03, 0x03);
        else
            fnD2SetOperationMode(0x03, 0x04);
    }

    else
        debug_err(("^Rinvalidate sensor mode...\r\n"));
    debug_err(("sensor set mode %d finished...\r\n",ucMode));
}


void fnZT3120MailBoxCommand(
	unsigned char ucPara1,
	unsigned char ucPara2,
	unsigned char ucPara3,
	unsigned char ucCmd)
{
  	Sensor_WriteReg(0x0081, ucPara1);
	Sensor_WriteReg(0x0082, ucPara2);
	Sensor_WriteReg(0x0083, ucPara3);
	Sensor_WriteReg(0x0080, ucCmd);
}

unsigned char fnZT3120MailboxStatus()
{
	//unsigned char ucCmdStatus = Sensor_ReadReg(0x0080);
    UINT32 ucCmdStatus = Sensor_ReadReg(0x0080);

	// If ZT3120 cannot execute the command successfully, set bit 6.
	if (ucCmdStatus & 0x40)
		return ZT3120_CMD_ERROR;
	// If ZT3120 have executed the command successfully, set bit 7.
	if (ucCmdStatus & 0x80)
		return ZT3120_CMD_DONE;
	// If ZT3120 is executing the command, bit 6 and 7 are not set.
	return ZT3120_CMD_RUNNING;
}

unsigned char fnD2SetOperationMode(
	unsigned char ucMode,
	unsigned char ucResolution)
{
	unsigned char ucCmdStatus = ZT3120_CMD_RUNNING;

	//fnZT3120MailBoxCommand(0x01, ucMode, ucResolution, 0x00);
	fnZT3120MailBoxCommand(ucMode, ucResolution, 0x00, 0x01);
	TimerDelayMs(100);
	while (ucCmdStatus == ZT3120_CMD_RUNNING)
	{
		ucCmdStatus = fnZT3120MailboxStatus();
	}
	return ucCmdStatus;
}

void Sensor_WriteReg(UINT32 addr, UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulReg2, ulData1, ulData2;

    i2c_lock();

    ulWriteAddr = SEN_WRITE_ID;
    ulReg1      = (addr >> 8) & 0xff;
    ulReg2      = addr & 0xff;

    //ulData1     = (value>>8);
    ulData1     = ((value)&(0x000000ff));

    do
    {
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);


        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Sensor_WriteReg: Error transmit data (write addr)!! %d\r\n", erReturn));
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

        erReturn=i2c_masterTransmit(ulData1, 0, 1);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (data1) !!\r\n"));
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
    ulReg1      = (addr >> 8) & 0xff;
    ulReg2      =addr;

    ulData      =0;
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
            debug_err(("Sensor_ReadReg:Error transmit data!!\r\n"));
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

        //erReturn=i2c_masterReceive(&ulData1, 0, 0);
        erReturn=i2c_masterReceive(&ulData1, 1, 1);
        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error receive data1!!\r\n"));
            break;
        }
    } while(erReturn != I2C_SUCCESS);
    if ( erReturn==I2C_SUCCESS )
        ulData=ulData1;
    i2c_unlock();
    return ulData;
}
#endif
