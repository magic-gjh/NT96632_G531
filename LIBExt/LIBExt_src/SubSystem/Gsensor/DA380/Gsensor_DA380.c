#include "stdio.h"
#include "Gsensor.h"
#include "spi.h"
#include "Gsensor_DA380.h"

#define abs(x) (((x) < 0) ? -(x) : (x))
#define IDC_SADDR_G_Sensor_W 0x4e
#define IDC_SADDR_G_Sensor_R 0x4f
static int g_GSensorSensitivity;

void DA380_OpenInterface(void)
{

	gpio_setDir(GPIO_GSENSOR_CLK, GPIO_DIR_OUTPUT);
	gpio_setDir(GPIO_GSENSOR_DAT, GPIO_DIR_OUTPUT);

	gpio_setPin(GPIO_GSENSOR_CLK);
	gpio_setPin(GPIO_GSENSOR_DAT);

	I2C_Init();
}

void DA380_CloseInterface(void)
{

}

/*return value: 0: is ok    other: is failed*/
int  i2c_read_byte_data( unsigned char addr, unsigned char *data)
{
	int ret = 0;
	
	*data = i2c_read(addr,IDC_SADDR_G_Sensor_W);
	return ret;
}

/*return value: 0: is ok    other: is failed*/
int    i2c_write_byte_data( unsigned char addr, unsigned char data){
		
		int ret = 0;
		
		//idc_write (IDC_STYPE_G_Sensor,IDC_SADDR_G_Sensor_W,addr ,&data,1);
		i2c_write(addr, data,IDC_SADDR_G_Sensor_W);
	
		return 0;
}

/*return value: 0: is count    other: is failed*/
int     i2c_read_block_data( unsigned char base_addr, unsigned char count, unsigned char *data){
	int i = 0;
		
	for(i = 0; i < count;i++)
	{
				if(i2c_read_byte_data(base_addr+i,(data+i)))
				{
					return -1;		
				}
	}	
		
	return count;
}

int mir3da_register_read( unsigned char addr, unsigned char *data){
    int     res = 0;

    res = i2c_read_byte_data(addr, data);
    if(res != 0) {
          return res;
    }	

    return res;
}

int mir3da_register_write( unsigned char addr, unsigned char data){
    int     res = 0;

    res = i2c_write_byte_data(addr, data);
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_read_continuously( unsigned char addr, unsigned char count, unsigned char *data)
{
    int     res = 0;

    res = (count==i2c_read_block_data(addr, count, data)) ? 0 : 1;
    if(res != 0) {
         return res;
    }

    return res;
}

int mir3da_register_mask_write(unsigned char addr, unsigned char mask, unsigned char data){
    int     res = 0;
    unsigned char      tmp_data;

    res = mir3da_register_read(addr, &tmp_data);
    if(res) {
        return res;
    }

    tmp_data &= ~mask; 
    tmp_data |= data & mask;
    res = mir3da_register_write(addr, tmp_data);

    return res;
}

/*return value: 0: is ok    other: is failed*/
BOOL DA380_GsensorInit(void)
{
	int res = 0;
	unsigned char data=0;
	mir3da_register_read(NSA_REG_WHO_AM_I,&data);
	if(data != 0x13){
	       debug_msg("------mir3da read chip id  error= %x-----\r\n",data); 
		return -1;
	}

	debug_msg("------mir3da chip id = %x-----\r\n",data); 

	res =  mir3da_register_mask_write(NSA_REG_SPI_I2C, 0x24, 0x24);

	delay(5);

	res |= mir3da_register_mask_write(NSA_REG_G_RANGE, 0x03, 0x01);
	res |= mir3da_register_mask_write(NSA_REG_G_RANGE, 0x0C, 0x00);
	res |= mir3da_register_mask_write(NSA_REG_POWERMODE_BW, 0xFF, 0x5E);
	res |= mir3da_register_mask_write(NSA_REG_ODR_AXIS_DISABLE, 0xFF, 0x06);
	
	res |= mir3da_register_mask_write(NSA_REG_INT_PIN_CONFIG, 0x0F, 0x05);//set int_pin level
	res |= mir3da_register_mask_write(NSA_REG_INT_LATCH, 0x8F, 0x86);//clear latch and set latch mode
	
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x83);
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0x69);
	res |= mir3da_register_mask_write(NSA_REG_ENGINEERING_MODE, 0xFF, 0xBD);
	res |= mir3da_register_mask_write(NSA_REG_SWAP_POLARITY, 0xFF, 0x00);

	return res;	    	
}

int mir3da_set_enable(char enable)
{
		int res = 0;
		if(enable)
		res = mir3da_register_mask_write(NSA_REG_POWERMODE_BW,0xC0,0x40);
		else	
		res = mir3da_register_mask_write(NSA_REG_POWERMODE_BW,0xC0,0x80);
	
	return res;	
}

void DA380_ReadInterupt(void)
{
	int   res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x03);
	res = mir3da_register_write(NSA_REG_ACTIVE_DURATION,0x03 );
	res = mir3da_register_write(NSA_REG_ACTIVE_THRESHOLD,0x1B );	
	res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x04 );
	//res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x04 );

}

int mir3da_close_interrupt(int num){
	int   res = 0;

	res = mir3da_register_write(NSA_REG_INTERRUPT_SETTINGS1,0x00 );
			
	switch(num){

		case 0:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING1,0x00 );
			break;

		case 1:
			res = mir3da_register_write(NSA_REG_INTERRUPT_MAPPING3,0x00 );
			break;
	}

	return res;
}
#if 0
/*return value: 0: is ok    other: is failed*/
void DA380_GetGsensorData(Gsensor_Data *GS_Data)
{
    unsigned char    tmp_data[6] = {0};

    if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
       // return 0;
    }
    
    GS_Data->Axis.Xacc = ((UINT32)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
    GS_Data->Axis.Yacc = ((UINT32)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
    GS_Data->Axis.Zacc = ((UINT32)(tmp_data[5] << 8 | tmp_data[4]))>> 4;

     debug_msg("oringnal x y z %d %d %d\r\n",GS_Data->Axis.Xacc,GS_Data->Axis.Yacc,GS_Data->Axis.Zacc); 	

    //return 0;
}
#else
/*return value: 0: is ok    other: is failed*/
void DA380_GetGsensorData(Gsensor_Data *GS_Data)
{
    short   tmp_data[3] = {0};

    if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, (unsigned char *)tmp_data) != 0) {
       // return 0;
    }
    
    GS_Data->Axis.Xacc = (unsigned short)(tmp_data[0] >> 2)&0x3fff;
    GS_Data->Axis.Yacc = (unsigned short)(tmp_data[1] >> 2)&0x3fff;
    GS_Data->Axis.Zacc = (unsigned short)(tmp_data[2] >> 2)&0x3fff;

     //debug_msg("oringnal x y z %d %d %d\r\n",GS_Data->Axis.Xacc,GS_Data->Axis.Yacc,GS_Data->Axis.Zacc); 	

    //return 0;
}
#endif
/*
int mir3da_read_int_status(void)
{
	char data = 0;

	mir3da_register_read(NSA_REG_MOTION_FLAG,&data);
	if(data&0x04)
		return 1;

	return 0;
}*/

static short threhold[4] = {300,600,1200,10000};//value 10000 means turn off collision check
/*char mir3da_check_collision(int level)
{
		static short prev_x = 0,prev_y = 0,prev_z = 0;
		short x = 0, y = 0, z = 0;
		static char b_is_first = 0;
		char is_collision = 0;
		
		if(mir3da_read_data(&x,&y,&z))
			return -1;
			
		if(b_is_first == 0)
		{
				prev_x = x;
				prev_y = y;
				prev_z = z;
				
				b_is_first = 1;
				
				return 0;					
		}	
		
		if((abs(x - prev_x) > threhold[level])||(abs(y - prev_y) > threhold[level])||(abs(z - prev_z) > threhold[level]))
				is_collision = 1;
				
		prev_x = x;
		prev_y = y;
		prev_z = z;
		
		return is_collision;
}*/


void DA380_SetSensitivity(UINT32 level)
{
	switch(level)
	{
		case 0:
			g_GSensorSensitivity = 5000;
			debug_msg("GSensorSensitivity --off--\r\n");
			break;	
		case 1:
			g_GSensorSensitivity = 1500;
			debug_msg("GSensorSensitivity --low--\r\n");

			break;
		case 2:
			g_GSensorSensitivity = 1200;			
			debug_msg("GSensorSensitivity --meddile--\r\n");

			break;
		case 3:	
			g_GSensorSensitivity = 800;
			debug_msg("GSensorSensitivity --high--\r\n");

			break;
		default:
			break;
	}
}
void DA380_GetGsensorStatus(UINT32 status,UINT32 *uiData)
{
}

GSENSOR_FUNC DA380FP = {
    DA380_GsensorInit,        // GsensorInit
    DA380_GetGsensorData,     // GetGsensorData
    DA380_GetGsensorStatus,   // GetGsensorStatus
    DA380_OpenInterface,      // OpenInterface
    DA380_CloseInterface,     // closeInterface
    DA380_SetSensitivity, //set sensitivity
    DA380_ReadInterupt, //read interupt when interupte happended
};

