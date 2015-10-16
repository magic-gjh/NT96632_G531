/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CommandTsk.c
    @ingroup

    @brief      Excute command via UART interface.
                Excute command and output message via UART interface.

    @note       Nothing.

    @version    V1.00.000
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "kernel.h"
#include "SysCfg.h"
#include "Top.h"
#include "UART.h"
#include "FileSysTsk.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "SystemInfoGet.h"
#include "KerVerInfo.h"
#include "AppVerInfo.h"
#include "Driver.h"
#include "ImageAlgInfor.h"
#include "ReadNand.h"
#include "Nand.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UISystemStatusSettings.h"
#include "SystemClock.h"
#include "SieCapTsk.h"
#include "PhotoTsk.h"
#include "PlaybackTsk.h"
#include "GlobalVar.h"
#include "PStore.h"
#include "PSDrv.h"
#include "ide.h"
#include "TestDRAM.h"
#include "sdio.h"
#include "sdmmc.h"
#include "GPIOMapping.h"
#include "adc.h"
#include "pwm.h"
#include "BuildDateInfo.h"
#include "Lens.h"
#include "AF_Lib.h"
#include "UICustomer.h"
#if(_GPS_RECEIVER_ == ENABLE)
#include "GPS.h"
#endif
#include "fde_lib.h"
#include "ime.h"
#include "hdmitx.h"
#include "Audio.h"
#include "MP3Api.h"
//#NT#2010/10/27#Connie Yu -begin
//#NT#movie DIS update
#include "PhotoDisplayTsk.h"
#include "PhotoDisTsk.h"
//#NT#2010/10/27#Connie Yu -end
#include "RawEncAPI.h"//2010/01/07 Meg
#include "UserFolder.h"

#include "BuildBmpInfo.h"

#include "aacEnc.h"//2012/02/23 Meg
#include "aacdec.h"//2012/02/23 Meg
#include "UIMenuMovieSettings.h"
#if (_COMMANDTSK_MODE_ == ENABLE)

#define _NAVI_COMUNICATION_PRO_    DISABLE//ENABLE //

#define CmdTsk_OutputMsg(msg)           debug_msg msg

//--------FOR NAVI TO DVR COMUNICATION ------------------------

#define CMD_HEADER    6
#define Byte_CMD     0
#define Byte_SA     1
#define Byte_DA     2
#define Byte_SN     3
#define Byte_CTL     4
#define Byte_SIZE     5

#define NAVI_CMD    0x01
#define SA_NAVI     0x55
#define SA_DVR       0xDD
#define DA_NAVI      0X55
#define DA_DVR        0XDD

#define CMD_GPS_INFO 0x10
#define CMD_CTL_TOUCHKEY  0X20

enum Command
{
	COMMAND_NULL = 0,
	COMMAND_GPS_INFO,			
	COMMAND_TOUCH_KEY,	
	COMMAND_MAX
};

static UINT32 CommandIndex = COMMAND_NULL;
BOOL bStarMovie = FALSE;

UINT32 SP_1[12]={0};
UINT32 Speed[2]={0};


//--------FOR NAVI TO DVR COMUNICATION ------------------------

#if(_NAVI_COMUNICATION_PRO_==ENABLE)
#define _UART_MONITOR_CMD_SIZE          256
#else
#define _UART_MONITOR_CMD_SIZE          64
#endif
#define COMMAND_ERROR                   "bad command or invalid parameters\r\n"

//----- Static funcs & variables define here  ------------------
//#NT#2009/09/09#Niven Cho -begin
//#NT#Marked., Move to CommandTsk.h for USB Debug System
//static void CmdTsk_DoCommand(CHAR *pcCmdStr);
#include "CommandTsk.h"
#include "MediaRecAPI.h" //2012/02/23 Meg Lin

//#NT#2009/09/09#Niven Cho -end
static void CmdTsk_ReadMemory(CHAR *para_str);
static void CmdTsk_WriteMemory(CHAR *para_str);
static void PutHexData(UINT8 SourceDat[],UINT32 Length);

//----- External funcs & variables declaration here  ------------------
extern void     sen_cmd(CHAR *pcCmdStr);
extern INT      zoom_index;
extern void     DPS_PrintFx(UB CapItem);

extern UINT32   VersionNumber, ReleaseDate;
extern BOOL    gUIAviIsRecording;

UINT32 GPSTime_Y;
UINT32 GPSTime_M;
UINT32 GPSTime_D;
UINT32 GPSTime_H;
UINT32 GPSTime_Mi;
UINT32 GPSTime_S;
UINT32 GPSDataCounter = 0;

UINT32 a2i(char data)
{
        switch(data)
        {
            case 48:
                return 0;
            break;
            case 49:
                return 1;
            break;
            case 50:
                return 2;
            break;
            case 51:
                return 3;
            break;
            case 52:
                return 4;
            break;
            case 53:
                return 5;
            break;
            case 54:
                return 6;
            break;
            case 55:
                return 7;
            break;
            case 56:
                return 8;
            break;
            case 57:
                return 9;
            break;
            default:
            break;

        }
}
void PutHexData(UINT8 SourceDat[],UINT32 Length)
{
	UINT32 i = 0;
      debug_err(("data :"));
	for(i=0;i<Length;i++)
	{
		uart2_putChar(SourceDat[i]);		
		debug_msg("0x%02x ",SourceDat[i]);
		
	}
      debug_err(("\r\n"));	
	
}

//----- CommandTsk  ------------------
void CommandTsk(void)
{
    CHAR    pcCommandLine[_UART_MONITOR_CMD_SIZE];
    CHAR    cLen;
    static UINT32 Step=0;
    static UINT32 Cmd_counter=0xff;
    UINT32 CheckSum,CheckSum_Cur;
    UINT32 DataSize;
    UINT32 i;
    static UINT8 string_buf[32];
    static UINT8 sp_buf[64];
	
    UINT8 Cmd_Return[5];
	
    /* start up messages */
    CmdTsk_OutputMsg(("\r\n=============================================================\r\n"));
    CmdTsk_OutputMsg(("  Novatek NT96630\r\n"));
    CmdTsk_OutputMsg(("  Copyright (c) 2010 Novatek Microelectronic Corp.\r\n"));
    CmdTsk_OutputMsg(("  Kernel      ver: %s, build: %s\r\n", Ker_GetVerInfo(), Ker_GetBuildDate()));
    CmdTsk_OutputMsg(("  Driver      ver: %s, build: %s\r\n", drv_getVerInfo(), drv_getBuildDate()));
    CmdTsk_OutputMsg(("  Application ver: %s, build: %s\r\n", App_GetVerInfo(), App_GetBuildDate()));
    strncpy(pcCommandLine, (char *)&VersionNumber, 8);
    pcCommandLine[8] = '\0';
    CmdTsk_OutputMsg(("  Project     ver: %s, ", pcCommandLine));
    strncpy(pcCommandLine, (char *)&ReleaseDate, 8);
    pcCommandLine[8] = '\0';
    CmdTsk_OutputMsg(("build: %s\r\n", pcCommandLine));
    CmdTsk_OutputMsg(("-------------------------------------------------------------\r\n"));
    CmdTsk_OutputMsg(("  Firmware    ver: %s\r\n", (CHAR *)GetVersionString()));
    CmdTsk_OutputMsg(("  Firmware  build: %s\r\n", (CHAR *)Prj_GetBuildDateStd()));
    CmdTsk_OutputMsg(("=============================================================\r\n\r\n"));

    while(1)
    {
		
#if(_NAVI_COMUNICATION_PRO_ == ENABLE)
        CmdTsk_OutputMsg((">> "));
        cLen = _UART_MONITOR_CMD_SIZE;
      if(Step == 0)
      	{
       	uart2_getChar(pcCommandLine); 
		if(*pcCommandLine == NAVI_CMD){
		   string_buf[0] = NAVI_CMD;
		   Step = 1;				   
		   //debug_msg("Step 1---\r\n");					   
		   }
		else{
				//Speed[0]=0;
				//Speed[1]=0;	
			}
      	}
	if(Step == 1)
	{
       	uart2_getChar(pcCommandLine); 
		if(*pcCommandLine == SA_NAVI){
		   string_buf[1] = SA_NAVI;			
		   Step = 2;}
		   else{
		   Step = 0;}
	}
	if(Step == 2)
	{
       	uart2_getChar(pcCommandLine); 
		if(*pcCommandLine == DA_DVR){
		   string_buf[2] = DA_DVR;						
		   Step = 3;}
		   else{
		   Step = 0;}		   
	}		
		
	if(Step == 3)
	{
	      //debug_msg("-------------------step 3 cmdcounter :%d %d\r\n",Cmd_counter,*pcCommandLine);
       	uart2_getChar(pcCommandLine); 
		//if(*pcCommandLine != Cmd_counter){
		   string_buf[3]= Cmd_counter = *pcCommandLine;
		   Step = 4;
		 //}
		//else{
		   //Step = 0;
		//}			
	}		
	if(Step == 4)
	{
       	uart2_getChar(pcCommandLine); 
		
		switch(*pcCommandLine)
		{
		   case CMD_GPS_INFO:
		   	CommandIndex = COMMAND_GPS_INFO;
		       string_buf[4] = CMD_GPS_INFO;					
		   	Step = 5;
		   	break;		
		   case CMD_CTL_TOUCHKEY:
		   	CommandIndex = COMMAND_TOUCH_KEY;	
			//debug_msg("---------------------Step: 4 :%x\r\n",*pcCommandLine);					   			
		       string_buf[4] = CMD_CTL_TOUCHKEY;								
		   	Step = 5;
		   	break;
		   default:		   	
		   	CommandIndex = COMMAND_NULL;
		   	Step = 0;
		     break;
		}			
	}		
	if(Step == 5)
	{
       	uart2_getChar(pcCommandLine); 
		DataSize = *pcCommandLine;
	       string_buf[5] = DataSize;								
	   	Step = 6;		
	}
	if(Step == 6)
	{
			//debug_msg("-----------------------Step: 6 CommandIndex = %x\r\n",CommandIndex);					   
	
	      if(CommandIndex == COMMAND_GPS_INFO)
	      	{
			for(i=0;i<DataSize;i++)
			{		
	       		uart2_getChar(pcCommandLine); 
			       //debug_msg("GPS--DataSize[%d] : 0x%02x 0x%02x\r\n",i,DataSize,*pcCommandLine);			
				sp_buf[i] = *pcCommandLine;
			}
			GPSTime_Y=sp_buf[13];
			GPSTime_M=sp_buf[14];
			GPSTime_D=sp_buf[15];
			GPSTime_H=sp_buf[16];
			GPSTime_Mi=sp_buf[17];	
			GPSTime_S=sp_buf[18];
			Speed[0]=sp_buf[19];
			Speed[1]=sp_buf[20];					
			Step = 7;
		}
		else if(CommandIndex == COMMAND_TOUCH_KEY){	
       		uart2_getChar(pcCommandLine); 
			//debug_msg("KEY--DataSize[%d] : 0x%02x 0x%02x\r\n",i,DataSize,*pcCommandLine);			
			string_buf[6] = *pcCommandLine;

	             uart2_getChar(pcCommandLine); 
		       CheckSum_Cur = *pcCommandLine;
		        //debug_msg("checksum_cur : 0x%02x \r\n",CheckSum_Cur);					
		       Step = 7;
			}
	}
	
	if(Step == 7)
	{
		
		for(i=0;i<(CMD_HEADER+DataSize);i++)
		{
		      //debug_msg("string_buf : 0x%02x \r\n",string_buf[i]);					
			CheckSum+= string_buf[i];
		}
		
		CheckSum = (CheckSum&0xff);
		//debug_msg("checksum_cal : 0x%02x \r\n",CheckSum);			
			//debug_msg("Step: 7 CommandIndex = %x\r\n",CommandIndex);			
		if (1)//(CheckSum == CheckSum_Cur)
		{
			switch(CommandIndex)
			{
				case COMMAND_GPS_INFO:
					if(sp_buf[0] == 1) // gps 
					{
					      GPSDataCounter++;
						  
						for(i=0;i<12;i++)
						{
							SP_1[i] = a2i(sp_buf[1+i]);
							//debug_msg("SP[%d]:0x%02x\r\n",i,SP_1[i]);
						}					
						Cmd_Return[0]=0x06;
						Cmd_Return[1]=0xdd;
						Cmd_Return[2]=0x55;
						Cmd_Return[3]=Cmd_counter;
						Cmd_Return[4]=0x00;					
						PutHexData(Cmd_Return,sizeof(Cmd_Return));	
						Cmd_counter = 0;	
						Step = 0;								
					}
					else
					{
						for(i=0;i<12;i++)
						{
							SP_1[i] = 0;					
						}
						Cmd_Return[0]=0x06;
						Cmd_Return[1]=0xdd;
						Cmd_Return[2]=0x55;
						Cmd_Return[3]=Cmd_counter;
						Cmd_Return[4]=0x00;					
						PutHexData(Cmd_Return,sizeof(Cmd_Return));	
						Cmd_counter = 0;	
						Step = 0;		
						Speed[0]=0;
						Speed[1]=0;	
									
					}
					break;
				case COMMAND_TOUCH_KEY:
							if(string_buf[Byte_SIZE+1] == 0x00)  //key enter
                                             {
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								if(bStarMovie == FALSE){
            							Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;		
								Step = 0;
									}
								else
								{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x01;
								PutHexData(Cmd_Return,sizeof(Cmd_Return));
								Step = 0;
								
								}
							}														
							else if(string_buf[Byte_SIZE+1] == 0x01) //key up
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
							
            							Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;			
								Step = 0;
								
							}
							else if(string_buf[Byte_SIZE+1] == 0x02) //key down
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
							
            							Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;		
								Step = 0;
								
							}
							else if(string_buf[Byte_SIZE+1] == 0x03) //key mode
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
						
            							Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;			
								Step = 0;
								
							}
							else if(string_buf[Byte_SIZE+1] == 0x04) //key shutter2
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
            							Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;		
								Step = 0;
								
							}														
							else if(string_buf[Byte_SIZE+1] == 0x05) //key mute
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
							   if(gUIAviIsRecording == TRUE){
							    switch(Get_MovieAudioIndex())
							    {
							        case MOVIE_AUDIO_OFF:
							            aud_setGain(AUDIO_GAIN_6);			
							            Set_MovieAudioIndex(MOVIE_AUDIO_ON);
							            break;
							        case MOVIE_AUDIO_ON:
							            aud_setGain(AUDIO_GAIN_MUTE);
							            Set_MovieAudioIndex(MOVIE_AUDIO_OFF);			
							            break;
							    }									
				            	             Ux_PostEvent(NVTEVT_OSD_REFLASH, 0);
							   	}
            							//Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;		
								Step = 0;
								
							}
							
							else if(string_buf[Byte_SIZE+1] == 0x07) //key reserve
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;		
								Step = 0;
								
							}								

							else if(string_buf[Byte_SIZE+1] == 0x08) //key reserve
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;		
								Step = 0;
								
							}																									
					break;

			}
		}
	      else
	      	{
			CmdTsk_OutputMsg(("check sum error %x != %x\r\n",CheckSum,CheckSum_Cur));
			Cmd_Return[0]=0x06;
			Cmd_Return[1]=0xdd;
			Cmd_Return[2]=0x55;
			Cmd_Return[3]=Cmd_counter;
			Cmd_Return[4]=0x01;	
			PutHexData(Cmd_Return,sizeof(Cmd_Return));	
			Step = 0;
		}
	}
#else
        CmdTsk_OutputMsg(("> "));

        cLen = _UART_MONITOR_CMD_SIZE;
        uart_getString(pcCommandLine, &cLen);
        CmdTsk_OutputMsg(("\r\n"));
        if (cLen)
        {
            CmdTsk_DoCommand(pcCommandLine);
	 	//NAVIToDVR_DoCommand(pcCommandLine);            
        }
#endif		
    }
}

#define BAT_ADC_MAX 832 //4.0V
#define BAT_ADC_36V 749 //3.6V
#define BAT_ADC_35V 728 //3.5V
#define BAT_ADC_349V 726  //3.49V
#define BAT_ADC_348V 724  //3.48V
#define BAT_ADC_347V 722  //3.47V
#define BAT_ADC_346V 720  //3.46V
#define BAT_ADC_345V 718  //3.45V
#define BAT_ADC_343V 714  //3.43V
#define BAT_ADC_341V 710  //3.41V
#define BAT_ADC_34V 707 //3.4V
UINT32  uirecordBat_PoolAddr = 0;
UINT32  uirecordBat_offset = 0;
BOOL    brecordBat_init_flag = FALSE;
static char        filepath1[]="A:\\batt36v.log";
static char        filepath2[]="A:\\batt35v.log";
static char        filepath3[]="A:\\batt349v.log";
static char        filepath4[]="A:\\batt348v.log";
static char        filepath5[]="A:\\batt347v.log";
static char        filepath6[]="A:\\batt346v.log";
static char        filepath7[]="A:\\batt345v.log";
static char        filepath8[]="A:\\batt343v.log";
static char        filepath9[]="A:\\batt341v.log";
static char        filepath10[]="A:\\batt34v.log";
UINT32 battery_detect_level = BAT_ADC_36V;
void save_BAT()
{
    HNVT_FILE   *pFile;
    UINT32      filesize,fileOffset;
    ER          fstatus;
    static char        *filepath;
    switch(battery_detect_level)
    {
        case BAT_ADC_36V:
        filepath = filepath1;
        break;
        case BAT_ADC_35V:
        filepath = filepath2;
        //pFile = fs_fopen(filepath2, "R");
        break;
        case BAT_ADC_349V:
        filepath = filepath3;
        break;
        case BAT_ADC_348V:
        filepath = filepath4;
        break;
        case BAT_ADC_347V:
        filepath = filepath5;
        break;
        case BAT_ADC_346V:
        filepath = filepath6;
        break;
        case BAT_ADC_345V:
        filepath = filepath7;
        break;
        case BAT_ADC_343V:
        filepath = filepath8;
        break;
        case BAT_ADC_341V:
        filepath = filepath9;
        break;
        case BAT_ADC_34V:
        filepath = filepath10;
        //pFile = fs_fopen(filepath3, "R");
        break;
    }
    pFile = fs_fopen(filepath, "R");
    if (pFile != NULL)
    {
        fileOffset =  pFile->fileSize;
        fs_fclose(pFile);
    }
    else
    {
        fileOffset = 0;
    }
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    filesize = uirecordBat_offset;
    fstatus = FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, filepath, (UINT8 *)uirecordBat_PoolAddr, &filesize, fileOffset, FST_TIME_INFINITE);
    if(fstatus)
    {
        debug_err(("write error\r\n"));
    }
    debug_err(("save_BAT\n\r"));
    switch(battery_detect_level)
    {
        case BAT_ADC_36V:
        //debug_err(("below BAT_ADC_3.6V \n\r"));
        battery_detect_level = BAT_ADC_35V;
        break;
        case BAT_ADC_35V:
        //debug_err(("below BAT_ADC_3.5V \n\r"));
        battery_detect_level = BAT_ADC_348V;
        break;
        case BAT_ADC_349V:
        //debug_err(("below BAT_ADC_3.49V \n\r"));
        battery_detect_level = BAT_ADC_348V;
        break;
        case BAT_ADC_348V:
        //debug_err(("below BAT_ADC_3.48V \n\r"));
        battery_detect_level = BAT_ADC_345V;
        break;
        case BAT_ADC_347V:
        //debug_err(("below BAT_ADC_3.47V \n\r"));
        battery_detect_level = BAT_ADC_346V;
        break;
        case BAT_ADC_346V:
        //debug_err(("below BAT_ADC_3.46V \n\r"));
        battery_detect_level = BAT_ADC_345V;
        break;
        case BAT_ADC_345V:
        //debug_err(("below BAT_ADC_3.45V \n\r"));
        battery_detect_level = BAT_ADC_343V;
        break;
        case BAT_ADC_343V:
        //debug_err(("below BAT_ADC_3.43V \n\r"));
        battery_detect_level = BAT_ADC_341V;
        break;
        case BAT_ADC_341V:
        //debug_err(("below BAT_ADC_3.41V \n\r"));
        battery_detect_level = BAT_ADC_34V;
        break;
        case BAT_ADC_34V:
        //debug_err(("below BAT_ADC_3.4V \n\r"));
        battery_detect_level = BAT_ADC_36V;
        break;
    }
    return;
}
static char temp_record_bat_buf[50];
void init_BAT()
{
    get_blk((VP*)&uirecordBat_PoolAddr, POOL_ID_SICD);
    rel_blk(POOL_ID_SICD, (VP)uirecordBat_PoolAddr);
    brecordBat_init_flag = TRUE;
    uirecordBat_offset = 0;
    memset(temp_record_bat_buf,0x00,sizeof(temp_record_bat_buf));

    return;
}
#define BATTERY_DET_COUNT           8
void test_BAT(UINT32 time_second,BOOL save_file)
{
    UINT32 bat_adc = 0;
    float  CurV;
    UINT32 len;
    UINT32 cnt = 0;
    for(cnt =0;cnt<BATTERY_DET_COUNT;cnt++)
    {
        bat_adc = bat_adc + VolDet_GetBatteryADC();
    }
    bat_adc = bat_adc >> 3;
    CurV = (float)(bat_adc)*4.0/BAT_ADC_MAX;

    if(brecordBat_init_flag == FALSE)
    {
        init_BAT();
    }
    if(save_file == TRUE)
    {
        save_BAT();
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
        return;
    }
    debug_msg("T=(%d),CurAD=(%d),CurV=(%1.3f),Lv=(%d)\r\n",time_second,bat_adc,CurV,battery_detect_level);
    sprintf(temp_record_bat_buf,"%d,%1.3f\r\n",bat_adc,CurV);
    //debug_err(("uirecordBat_offset = %d\n\r",uirecordBat_offset));
    len = strlen(temp_record_bat_buf);
    memcpy((char*)(uirecordBat_PoolAddr+uirecordBat_offset),temp_record_bat_buf,len);
    uirecordBat_offset = uirecordBat_offset + len;
    if(bat_adc < battery_detect_level)
    {
        switch(battery_detect_level)
        {
            case BAT_ADC_36V:
            debug_err(("below BAT_ADC_3.6V \n\r"));
            //battery_detect_level = BAT_ADC_35V;
            break;
            case BAT_ADC_35V:
            debug_err(("below BAT_ADC_3.5V \n\r"));
            //battery_detect_level = BAT_ADC_34V;
            break;
            case BAT_ADC_349V:
            debug_err(("below BAT_ADC_3.49V \n\r"));
            //battery_detect_level = BAT_ADC_345V;
            break;
            case BAT_ADC_348V:
            debug_err(("below BAT_ADC_3.48V \n\r"));
            //battery_detect_level = BAT_ADC_345V;
            break;
            case BAT_ADC_347V:
            debug_err(("below BAT_ADC_3.47V \n\r"));
            //battery_detect_level = BAT_ADC_345V;
            break;
            case BAT_ADC_346V:
            debug_err(("below BAT_ADC_3.46  V \n\r"));
            //battery_detect_level = BAT_ADC_345V;
            break;
            case BAT_ADC_345V:
            debug_err(("below BAT_ADC_3.45V \n\r"));
            //battery_detect_level = BAT_ADC_345V;
            break;
            case BAT_ADC_343V:
            debug_err(("below BAT_ADC_3.43V \n\r"));
            //battery_detect_level = BAT_ADC_341V;
            break;
            case BAT_ADC_341V:
            debug_err(("below BAT_ADC_3.41V \n\r"));
            //battery_detect_level = BAT_ADC_34V;
            break;
            case BAT_ADC_34V:
            debug_err(("below BAT_ADC_3.4V \n\r"));
            //battery_detect_level = BAT_ADC_36V;
            break;
        }
        //stop video record
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
        return;
    }
}

void testMP3(void)
{
#if (_MP3_MODE_ == ENABLE)
    MP3PLAYAPI_INITPARAM    MP3InitParam;
    UINT32                  uiPoolAddr;
    UINT32                  uiTypeNow;
    UINT32                  uiTotalFiles, uiFileIndex, uiTotalSec, uiNowOrder;

    uiTotalFiles = MP3PlayFileSys_GetMp3TotalFiles();
    MP3PlayFileSys_SetNowOrder(uiTotalFiles - 1);

    uiFileIndex = MP3PlayFileSys_GetFileIndex(MP3PlayFileSys_GetNowOrder());
    debug_msg("uiFileIndex = %ld\r\n", uiFileIndex);
    MP3PlayFile_SetNowIndexToRead(uiFileIndex);

    uiTypeNow = MP3PlayFileSys_GetType(uiFileIndex);

    if(uiTypeNow != SEARCHTYPE_MP3)
    {
        debug_msg("Not MP3 file!\r\n");
        return;
    }

    MP3PlayFile_ReadToNextFile(READFILE_CURR);
    MP3PlayClose();

    get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    MP3InitParam.Mp3PlayBufAddr = (uiPoolAddr + POOL_SIZE_CAPTURE - POOL_SIZE_MP3FONT - POOL_SIZE_MP3);
    MP3InitParam.Mp3PlayBufSize = POOL_SIZE_MP3;
    MP3InitParam.CallbackFunc = NULL;

    uiNowOrder = MP3PlayFileSys_GetNowOrder();
    uiTotalSec = MP3Play_GetActualPlayingSeconds(uiNowOrder, MP3InitParam.Mp3PlayBufAddr, MP3InitParam.Mp3PlayBufSize);
    debug_msg("uiNowOrder = %ld, uiTotalFiles = %ld, uiTotalSec = %ld\r\n", uiNowOrder, uiTotalFiles, uiTotalSec);

    MP3PlayOpen(&MP3InitParam);
    MP3PlayApi_SetDefaultVolume(AUDIO_VOL_7);
    MP3Play_StartPlaying();
#endif
}


#include "UIMovieObj.h"
#include "UISystemObj.h"
#include "UIPlayObj.h"
#include "AppInitPlayback.h"
#include "GPIOMapping.h"
/**
  Excute command received from UART

  Excute command received from UART.
  [Command Task internal API]

  @param CHAR *pcCmdStr: Command String
  @return void
*/
//#NT#2009/09/09#Niven Cho -begin
//#NT#Modified., Remove static for USB Debug System
UINT32 gTestAACSR = 0;//2012/02/23 Meg Lin

void CmdTsk_DoCommand(CHAR *pcCmdStr)
//#NT#2009/09/09#Niven Cho -end
{
    unsigned char item, data;
    char *pBuf;
    MEDIAREC_AACTEST_OBJ aacObj;//2012/02/23 Meg Lin

    switch (*pcCmdStr)
    {
    // Help
    case 'h':
    case '?':
        if ((!strcmp(pcCmdStr, "h")) || (!strcmp(pcCmdStr, "?")) || (!strcmp(pcCmdStr, "help")))
        {
            CmdTsk_OutputMsg(("h   : display help message\r\n"));
            CmdTsk_OutputMsg(("?   : display help message\r\n"));
            CmdTsk_OutputMsg(("r 0xHex_Address [0xHex_Length]: read contents of memory\r\n"));
            CmdTsk_OutputMsg(("w 0xHex_Address 0xHex_Data : write data to memory\r\n"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'A'
    case 'a':
        if (!strncmp(pcCmdStr, "asen ", 5))
        {
            sen_cmd((pcCmdStr+5));
        }
        else if (!strncmp(pcCmdStr, "aac1",4))
        {
            aacObj.uiPCMAddr =0x2000000;
            aacObj.uiPCMSize =0x200000;
            aacObj.uiAACBSAddr=aacObj.uiPCMAddr+aacObj.uiPCMSize;
            aacObj.uiAACBSSize=0x100000;
            aacObj.uiAudSampleRate =AUDIO_SR_32000;
            gTestAACSR = aacObj.uiAudSampleRate;
            aacObj.uiAudBitRate=48000;

            MediaAACSetAudio(&aacObj);

        }
        else if (!strncmp(pcCmdStr, "aacstop",7))
        {
            MediaAACSetAudioStop();
        }
        else if (!strncmp(pcCmdStr, "aacd",4))
        {
            test_Daac(1, gTestAACSR);

            //test_Daac(1, 24000);
        }

        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'B'
    case 'b':
        get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
        rel_blk(POOL_ID_CAPTURE, pBuf);

        if(pBuf == NULL)
        {
            CmdTsk_OutputMsg(("Benchmark : get buffer fail\r\n"));
            break;
        }
        if (!strncmp(pcCmdStr,"benchmark card", 14))
        {
            CmdTsk_OutputMsg(("Benchmark Card...\r\n"));
            if (FilesysBenchmark(Primary_GetStorageObj(PRIMARY_STGTYPE_CARD), (UB *)pBuf, POOL_SIZE_CAPTURE) == E_OK)
            {
                CmdTsk_OutputMsg(("Benchmark done\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Benchmark fail\r\n"));
            }
        }
        else if (!strncmp(pcCmdStr,"benchmark nand", 14))
        {
            CmdTsk_OutputMsg(("Benchmark Nand...\r\n"));
            if (FilesysBenchmark(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), (UB *)pBuf, POOL_SIZE_CAPTURE) == E_OK)
            {
                CmdTsk_OutputMsg(("Benchmark done\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Benchmark fail\r\n"));
            }
        }
#if (OSD_DUMP == OSD_DUMP_ENABLE)
        else if (!strncmp(pcCmdStr,"bmp make", 8))
        {
          char filename[20];

            sscanf(pcCmdStr+9,"%s",filename);
            BMP_MakeFile(filename);
        }
#endif
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'C'
    case 'c':
        if (!strncmp(pcCmdStr,"chknand all", 11))
        {
            UINT32 errblock;
            CmdTsk_OutputMsg(("Read check for all NAND: \r\n"));
            errblock = nand_readCheckAll(TRUE);
            CmdTsk_OutputMsg(("nand_readCheckAll report %d bad block!\r\n", errblock));
        }
        else if (!strncmp(pcCmdStr,"chknand erase", 13))
        {
            UINT32 errblock;
            CmdTsk_OutputMsg(("Erase check NAND FAT area: \r\n"));
            errblock = nand_eraseCheckFAT();
            CmdTsk_OutputMsg(("nand_eraseCheckFAT report %d bad block!\r\n", errblock));
        }
        //this match should be placed at the end of all of the aboves
        else if (!strncmp(pcCmdStr,"chknand", 7))
        {
            UINT32 errblock;
            CmdTsk_OutputMsg(("Read check for NAND read error: \r\n"));
            errblock = nand_readCheckAll(FALSE);
            CmdTsk_OutputMsg(("nand_readCheckAll report %d bad block!\r\n", errblock));
        }
        else if (!strncmp(pcCmdStr,"card out", 8))
        {
            PSTRG_TAB storageObj;
            storageObj = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);
            storageObj->Close();
            CmdTsk_OutputMsg(("Remove card\r\n"));
        }
        else if (!strncmp(pcCmdStr,"card in", 7))
        {
            PSTRG_TAB storageObj;
            storageObj = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);
            storageObj->Open();
            CmdTsk_OutputMsg(("Insert card\r\n"));
        }
        else if (!strncmp(pcCmdStr, "config save", 11))
        {
            CmdTsk_OutputMsg(("Saving menu info to flash......\r\n"));
            Save_MenuInfo();
            CmdTsk_OutputMsg(("Saving completed!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "config reset", 12))
        {
            CmdTsk_OutputMsg(("Reset menu info to default......\r\n"));
            SysReset_MenuInfo();
            CmdTsk_OutputMsg(("Resetting completed!\r\n"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'D'
    case 'd':
        //#NT#2008/03/20#Chris Hsu -begin
        //#NT#Replace with debug_dumpKernelStatus() API
        if (!strncmp(pcCmdStr,"dump ker", 8))
        {
            debug_dumpKernelStatus();
        }
        //#NT#2008/03/20#Chris Hsu -end
        else if (!strncmp(pcCmdStr,"dump nand", 9))
        {
            //#NT#2009/01/06#Chris Hsu -begin
            //#NT#Modify for new ReadNand_Total() API
            UINT32 uiMB;

            // Dump all main data
            if (sscanf(pcCmdStr + 9, "%d", &uiMB) == 0)
            {
                ReadNand_Total(READNAND_MDATASIZE_ALL);
            }
            // Only dump X MB main data
            else
            {
                ReadNand_Total(uiMB << 20);
            }
            //#NT#2009/01/06#Chris Hsu -end
        }
        //#NT#2009/01/21#HH Chuang -begin
        //#NT#add printing sd card information
        else if (!strncmp(pcCmdStr, "dcf create", 10))
        {
            //USERFolder_FolderCreate();
        }
        else if (!strncmp(pcCmdStr, "dcf set", 7))
        {
            USERFolder_set();
        }
        else if (!strncmp(pcCmdStr, "dump sdinfo", 11))
        {
#if (_STORAGECARD_ == _STORAGECARD_SDMMC_)
            sdmmc_printCardInformation();
#elif (_STORAGECARD_ == _STORAGECARD_SDIO_)
            sdio_printCardInformation();
#elif (_STORAGECARD_ == _STORAGECARD_SDIO2_)
            sdio2_printCardInformation();
#endif
        }
        //#NT#2009/01/21#HH Chuang -end
        else if (!strncmp(pcCmdStr, "dramtest", 8))
        {
            UINT32 uiTestAddrStart, uiTestSize;

            uiTestAddrStart = FRE_MEM;
            uiTestSize = (END_MEM - FRE_MEM);

            CmdTsk_OutputMsg((">>>>>>DRAM test start..... (Addr: 0x%.8X ~ 0x%.8X)\r\n", uiTestAddrStart, (uiTestAddrStart + uiTestSize)));
            TestDRAM_WriteReadWord(uiTestAddrStart, uiTestSize);
            TestDRAM_WriteReadHalfWord(uiTestAddrStart, uiTestSize);
            TestDRAM_WriteReadByte(uiTestAddrStart, uiTestSize);
            TestDRAM_WriteReadWordRandom(uiTestAddrStart, uiTestSize);
            CmdTsk_OutputMsg((">>>>>>DRAM test completed.\r\n"));
        }
        //#NT#2010/10/28#Connie Yu -begin
        else if (!strncmp(pcCmdStr, "disv on", 7))
        {
            PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, TRUE);
            PhotoDis_setMode(PHOTODIS_MODE_START, TRUE);
        }
        else if (!strncmp(pcCmdStr, "disv off", 8))
        {
            PhotoDis_setMode(PHOTODIS_MODE_IDLE, TRUE);
            PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
        }
        else if (!strncmp(pcCmdStr, "db on", 5))
        {
            PhotoDis_setFlagVideoBlend(ENABLE);
        }
        else if (!strncmp(pcCmdStr, "db off", 6))
        {
            PhotoDis_setFlagVideoBlend(DISABLE);
        }
        //#NT#2010/10/28#Connie Yu -end
        else if (!strncmp(pcCmdStr, "dp1", 3))
        {
            dis_setDebugPrint(ENABLE);
        }
        else if (!strncmp(pcCmdStr, "dp0", 3))
        {
            dis_setDebugPrint(DISABLE);
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;
    case 'e':
        //#NT#2009/05/27#KS Hung -begin
        if (!strncmp(pcCmdStr,"ehdmi720p", 9))
        {
            //pll_setPLL2(0x63,0x02);//PLL2=297MHz
            //gVID=HDMI_1280X720P60;
            InitTVHDMI(HDMI_1280X720P60,HDMI_AUDIO_NO_CHANGE);
            OpenTVHDMI(&g_LCDCtrlObj,&g_LCDSize);
            KeyScan_ScaleDisplay();
        }
        else if (!strncmp(pcCmdStr,"ehdmi576p", 9))
        {
            //pll_setPLL2(0x5A,0x02);//PLL2=270MHz
            //gVID=HDMI_720X576P50;
            InitTVHDMI(HDMI_720X576P50,HDMI_AUDIO_NO_CHANGE);
            OpenTVHDMI(&g_LCDCtrlObj,&g_LCDSize);
            KeyScan_ScaleDisplay();
        }
        else if (!strncmp(pcCmdStr,"ehdmi480p", 9))
        {
            //pll_setPLL2(0x5A,0x02);//PLL2=270MHz
            //gVID=HDMI_720X480P60;
            InitTVHDMI(HDMI_720X480P60,HDMI_AUDIO_NO_CHANGE);
            OpenTVHDMI(&g_LCDCtrlObj,&g_LCDSize);
            KeyScan_ScaleDisplay();
        }
        else if (!strncmp(pcCmdStr,"ehdmi1080i", 10))
        {
            //pll_setPLL2(0x63,0x02);//PLL2=297MHz
            //gVID=HDMI_1920X1080I60;
            InitTVHDMI(HDMI_1920X1080I60,HDMI_AUDIO_NO_CHANGE);
            OpenTVHDMI(&g_LCDCtrlObj,&g_LCDSize);
            KeyScan_ScaleDisplay();
        }
        else if (!strncmp(pcCmdStr,"elcd", 4))
        {
            CloseTVHDMI();
            OpenPanel(&g_LCDCtrlObj, &g_LCDSize);
            KeyScan_ScaleDisplay();
            //KeyScan_EnableTV(FALSE);
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        //#NT#2009/05/27#KS Hung -end
    break;
    // Command start with 'F'
    case 'f':
        if (!strncmp(pcCmdStr,"format card", 11))
        {
            CmdTsk_OutputMsg(("Format Card...\r\n"));
            if (FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_CARD), TRUE) == E_OK)
            {
                CmdTsk_OutputMsg(("Format Done.\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Format Fail\r\n"));
            }
        }
        else if (!strncmp(pcCmdStr,"format nand", 11))
        {
            CmdTsk_OutputMsg(("Format Nand...\r\n"));
            if (FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE) == E_OK)
            {
                CmdTsk_OutputMsg(("Format Done.\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Format Fail\r\n"));
            }
        }
        else if (!strncmp(pcCmdStr,"format nand2", 12))
        {
            CmdTsk_OutputMsg(("Format Nand...\r\n"));
            if (FilesysFormat(nand2_getStorageObject(), FALSE) == E_OK)
            {
                CmdTsk_OutputMsg(("Format nand2 Done.\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Format nand2 Fail\r\n"));
            }
        }
        else if(!strncmp(pcCmdStr,"flashon", 7))
        {
            GPIOMap_TriggerFlash(ON);
            TimerDelayMs(100);
            GPIOMap_TriggerFlash(OFF);
            debug_msg("Flash Trigger  \r\n");
        }
        else if(!strncmp(pcCmdStr,"flashcharge", 11))
        {
            GPIOMap_ChargeFlash(TRUE,TRUE);
            while(1)
            {
                TimerDelayMs(100);
                if(GPIOMap_IsFlashReady())
                {
                    debug_msg("Flash Charge ready\r\n");
                    GPIOMap_ChargeFlash(FALSE,FALSE);
                    break;
                }
                debug_msg("...");
            }
            debug_msg("Flash Charge");
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;
        //#NT#2009/03/18#Photon Lin -begin
        //#GPS feature
        // Command start with 'G'
    case 'g':
        if (!strncmp(pcCmdStr, "gps open", 8))
        {
        //#NT#2009/05/23#Jarkko Chang -begin
        //#tmp mark for pass compiler
//            GPSRec_Open(UART_BAUDRATE_4800);
        //#NT#2009/05/23#Jarkko Chang -end
        }
        else if (!strncmp(pcCmdStr, "gps close", 9))
        {
        //#NT#2009/05/23#Jarkko Chang -begin
        //#tmp mark for pass compiler
//            GPSRec_Close();
        //#NT#2009/05/23#Jarkko Chang -end
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;
        //#NT#2009/03/18#Photon Lin -end
    // Command start with 'I'
#if (_SYSINFO_MODE_ == ENABLE)
    case 'i':
        if (!strncmp(pcCmdStr, "info sys", 8))
        {
            SYSInfo_GetSystemInfo();
        }
        //#NT#2009/04/21#Jacky Huang -begin
        //#NT#Add for read/write audio codec register API
        else if (!strncmp(pcCmdStr,"i2sw ", 5))
        {
            UINT32 regIdx=0,value=0;
            sscanf((pcCmdStr+5), "%x %x\r\n", &regIdx, &value);

            aud_sendI2SCommand(regIdx,value);
            CmdTsk_OutputMsg(("Write regIdx=0x%x value=0x%x.\r\n",regIdx,value));
        }
        else if (!strncmp(pcCmdStr,"i2sr ", 5))
        {
            UINT32 regIdx=0,value=0;

            sscanf((pcCmdStr+5), "%x\r\n", &regIdx);

            aud_readI2SData(regIdx,&value);
            CmdTsk_OutputMsg(("Read regIdx=0x%x value=0x%x.\r\n",regIdx,value));
        }
       //#NT#2009/04/21#Jacky Huang -end
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;
#endif

    // Command start with 'K'
    case 'k':
        if (!strncmp(pcCmdStr, "kdel", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_DEL);
            Ux_PostEvent(NVTEVT_KEY_DEL, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Del Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kdis", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_DISPLAY);
            Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_DISPLAY);
            //Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Display Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ku", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_UP);
            Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Up Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kd", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_DOWN);
            Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Down Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "krr", 3))
        {
            Ux_PostEvent(NVTEVT_KEY_RIGHT, 1, NVTEVT_KEY_RELEASE);
            CmdTsk_OutputMsg(("Right Key release\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kr", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_RIGHT);
            Ux_PostEvent(NVTEVT_KEY_RIGHT, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Right Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "klr", 3))
        {
            Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_RELEASE);
            CmdTsk_OutputMsg(("Left Key release\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kl", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_LEFT);
            Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Left Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ke", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_ENTER);
            Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Enter Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kmd", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_MODE);
            Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Mode Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "km", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_MENU);
            Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Menu Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kpb", 3))
        {
            Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Playback Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzir", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_RELEASED | FLGKEY_ZOOMIN);
            Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_RELEASE);
            CmdTsk_OutputMsg(("Zoom In Key release\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzi", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_ZOOMIN);
            Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Zoom In Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzor", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_RELEASED | FLGKEY_ZOOMOUT);
            Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_RELEASE);
            CmdTsk_OutputMsg(("Zoom Out Key release\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzo", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_ZOOMOUT);
            Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Zoom Out Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ks1", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_SHUTTER1);
            Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Shutter1 Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ks2", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
            Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Shutter2 Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ks", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_SHUTTER);
            Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Shutter Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kie1enc", 7))
        {
            PB_SetImgEnhCurrFile(PB_IMGENH_BLACKWHITE, FALSE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
            CmdTsk_OutputMsg(("Re-encode B&W image!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kie2enc", 7))
        {
            PB_SetImgEnhCurrFile(PB_IMGENH_SEPIA, FALSE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
            CmdTsk_OutputMsg(("Re-encode Sepia image!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kie3enc", 7))
        {
            PB_SetImgEnhCurrFile(PB_IMGENH_NEGATIVE, FALSE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
            CmdTsk_OutputMsg(("Re-encode Negative image!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kie1", 4))
        {
            PB_SetImgEnhDisplay(PB_IMGENH_BLACKWHITE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
            CmdTsk_OutputMsg(("Apply B&W effect!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kie2", 4))
        {
            PB_SetImgEnhDisplay(PB_IMGENH_SEPIA);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
            CmdTsk_OutputMsg(("Apply Sepia effect!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kie3", 4))
        {
            PB_SetImgEnhDisplay(PB_IMGENH_NEGATIVE);
            PB_WaitCommandFinish(PB_WAIT_INFINITE);
            CmdTsk_OutputMsg(("Apply Negative effect!\r\n"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }

        break;

    // Command start with 'L'
    case 'l':
        if (!strncmp(pcCmdStr,"li",2))
        {
            //LensInit();
            CmdTsk_OutputMsg(("Lens Init\r\n"));
        }
        if (!strncmp(pcCmdStr,"lr",2))
        {
            //LensRetract();
            CmdTsk_OutputMsg(("Lens retract\r\n"));
        }
        else if (!strncmp(pcCmdStr,"laf",3))
        {
            AF_Process();
            CmdTsk_OutputMsg(("AF\r\n"));
        }
        else if(!strncmp(pcCmdStr,"lm ",3))
        {
            sscanf(pcCmdStr+4,"%d %d \n",&item,& data);
            UI_SetParameter(item,data);
        }
        else if(!strncmp(pcCmdStr,"la ",3))
        {
            sscanf(pcCmdStr+4,"%d\n",& data);
            Photo_Aperture_Move(data);
        }
        else if (!strncmp(pcCmdStr, "lens ", 5))
        {
            Lens_Cmd((pcCmdStr+5));
        }        
        break;

    // Command start with 'M'
    case 'm':
        if (!strncmp(pcCmdStr,"mode photo", 10))
        {
            CmdTsk_OutputMsg(("Photo Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        }
        else if (!strncmp(pcCmdStr,"mode setup", 10))
        {
            CmdTsk_OutputMsg(("Setup Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_SETUP);
        }
        else if (!strncmp(pcCmdStr,"mode playback", 13))
        {
            CmdTsk_OutputMsg(("Playback Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
        }
        else if (!strncmp(pcCmdStr,"mode msdc", 9))
        {
            CmdTsk_OutputMsg(("MSDC Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
        }
        else if (!strncmp(pcCmdStr,"mode sicd", 9))
        {
            CmdTsk_OutputMsg(("SICD Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_USBSICD);
        }
        else if (!strncmp(pcCmdStr,"mode pcc", 8))
        {
            CmdTsk_OutputMsg(("PCC Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_USBPCC);
        }
        else if (!strncmp(pcCmdStr,"mode movie", 10))
        {
            CmdTsk_OutputMsg(("Movie Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_MOVIE);
        }
        else if (!strncmp(pcCmdStr,"mode eng", 8))
        {
            CmdTsk_OutputMsg(("Engineering Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_ENGINEER);
        }
        else if (!strncmp(pcCmdStr,"mode mp3", 8))
        {
            CmdTsk_OutputMsg(("MP3 Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_MUSIC);
        }
        //#NT#2008/09/02#JR Huang -begin
        //#arrange the test program to reduce the FW size
        /**
        An example of mov test program.
        @code

        else if (!strncmp(pcCmdStr,"movplay test1", 13))
        {
            int i=0;

            CmdTsk_OutputMsg(("Movie playback test mode one\r\n"));

            while(i<20000)
            {
                CmdTsk_OutputMsg(("test %d time\r\n",i));

                Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
                TimerDelayMs(2000);
                CmdTsk_OutputMsg(("test one movie playing\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                CmdTsk_OutputMsg(("forward speed up\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(4000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                CmdTsk_OutputMsg(("backward speed up\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_LEFT);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(4000);
                set_flg(FLG_ID_KEY, FLGKEY_UP);
                TimerDelayMs(2000);
                CmdTsk_OutputMsg(("close playback\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_DOWN);
                TimerDelayMs(1000);
                CmdTsk_OutputMsg(("test two movie playing\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(2000);
                CmdTsk_OutputMsg(("change mode\r\n"));
                Primary_ChangeMode(PRIMARY_MODE_MOVIE);
                TimerDelayMs(500);
                i++;
            }
        }
        else if (!strncmp(pcCmdStr,"movplay test2", 13))
        {
            int i=0;

            CmdTsk_OutputMsg(("Movie playback test mode two\r\n"));
            CmdTsk_OutputMsg(("repeat quickly enter 'enter key'\r\n"));

            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
            TimerDelayMs(1000);

            while(i<20000)
            {
                CmdTsk_OutputMsg(("test %d time\r\n",i));

                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(200);
                set_flg(FLG_ID_KEY, FLGKEY_DOWN);
                TimerDelayMs(500);
                i++;
            }
        }
        else if (!strncmp(pcCmdStr,"movplay test3", 13))
        {
            int i=0;

            CmdTsk_OutputMsg(("Movie playback test mode three\r\n"));

            while(i<20000)
            {
                CmdTsk_OutputMsg(("test %d time\r\n",i));

                Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
                TimerDelayMs(2000);
                CmdTsk_OutputMsg(("test one movie playing\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(2000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(3000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(4000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(5000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(6000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(7000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(8000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(9000);
                set_flg(FLG_ID_KEY, FLGKEY_ENTER);
                TimerDelayMs(1000);

                i++;
            }
        }
        else if (!strncmp(pcCmdStr,"movrec test1", 12))
        {
            int i=0;

            CmdTsk_OutputMsg(("Movie recording test mode one\r\n"));

            while(i<12000)
            {
                CmdTsk_OutputMsg(("test %d time\r\n",i));

                Primary_ChangeMode(PRIMARY_MODE_MOVIE);
                TimerDelayMs(500);
                CmdTsk_OutputMsg(("test one movie recording\r\n"));
                CmdTsk_OutputMsg(("test open stop\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
                TimerDelayMs(2500);
                set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
                TimerDelayMs(1000);
                 CmdTsk_OutputMsg(("test two movie recording\r\n"));
                set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
                TimerDelayMs(2500);
                 CmdTsk_OutputMsg(("test change mode\r\n"));
                Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
                TimerDelayMs(1500);
                i++;
            }
        }
        else if (!strncmp(pcCmdStr,"movrec test2", 12))
        {
            int i=0;

            CmdTsk_OutputMsg(("Movie recording test mode two\r\n"));
            CmdTsk_OutputMsg(("repeat quickly enter 'enter key'\r\n"));

            Primary_ChangeMode(PRIMARY_MODE_MOVIE);
            TimerDelayMs(1000);

            while(i<20000)
            {
                CmdTsk_OutputMsg(("test %d time\r\n",i));

                set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
                TimerDelayMs(200);
                set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
                TimerDelayMs(1000);
                i++;
            }
        }
        @endcode
        */
        //#NT#2008/09/02#JR Huang -end
        else if (!strncmp(pcCmdStr,"mp3 play", 8))
        {
            CmdTsk_OutputMsg((">>>>>>>mp3 play start-------------------------\r\n"));
            testMP3();
            CmdTsk_OutputMsg((">>>>>>>mp3 play end-------------------------\r\n"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    case 'p':
        if (!strncmp(pcCmdStr, "ps format", 9))
        {
            PSTORE                  PStoreStruct;

            PStoreStruct.uiSignature  = 0x55AA55AA;
            PStoreStruct.uiPSVersion  = 0x0001;
            PStoreStruct.uiPStoreSize = SysInit_GetPStoreSize();
            PStore_EnablePS();
            PStore_Format(&PStoreStruct);
        }
        break;

    // Command start with 'R'
    case 'r':
        if (!strncmp(pcCmdStr, "r ", 2))
        {
            CmdTsk_ReadMemory((pcCmdStr+2));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'S'
    case 's':
        if (!strncmp(pcCmdStr,"s2",2))//Single capture
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_ImgSize, _IMGSize_9M);
            ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_SiePath,_PostGamma);
            ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
            ImgCapture_StartCapture();
            CmdTsk_OutputMsg(("Single Capture \r\n"));
//#NT#2008/08/07#Adrian Jiang -end

        }
        else if (!strncmp(pcCmdStr,"s3",2))//Burst Capture 3
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_ImgSize, _IMGSize_5M);
            ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_SiePath,_PostGamma);
            ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Burst);
            ImgCapture_SetParameter(_BurstNum,_BurstNum_3);
            ImgCapture_StartCapture();
            CmdTsk_OutputMsg(("Burst Capture 3 pic\r\n"));
//#NT#2008/08/07#Adrian Jiang -end
        }
        else if (!strncmp(pcCmdStr,"s4",2))//InfBurst
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_ImgSize, _IMGSize_5M);
            ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_SiePath,_PostGamma);
            ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_InfBurst);
            ImgCapture_StartCapture();
            CmdTsk_OutputMsg(("InfBurst\r\n"));
//#NT#2008/08/07#Adrian Jiang -end
        }
        else if (!strncmp(pcCmdStr,"ss",2))//Stop Burst Capture
        {
            ImgCapture_StopBurstCapture();
            CmdTsk_OutputMsg(("StopBurst\r\n"));
        }
        else if (!strncmp(pcCmdStr,"set logo",8))
        {
          PLAY_RESIZE_LOGO LogoData = {0};
          PS_SECNAME_ID secnameId;
          INT32 err;
          ER    erReturn = E_OK;
          UINT8       *BgBuf;
          PSTORE_SECTION_HANDLE* pSecHdl;

              LogoData.uiJpegSize = PLAY_LOGO_QVGA;
              err = PB_ReSizeForLogo(&LogoData);
              if (err==E_OK)
              {
                debug_msg("address=0x%x,buffer=0x%x\r\n",LogoData.uiJpegAddress,LogoData.uiJpegSize);

                get_blk((void *)&BgBuf,  POOL_ID_SICD);
                rel_blk(POOL_ID_SICD, BgBuf);

                *BgBuf = LO_BYTE(LO_WORD(LogoData.uiJpegSize));
                *(BgBuf+1) = HI_BYTE(LO_WORD(LogoData.uiJpegSize));
                *(BgBuf+2) = LO_BYTE(HI_WORD(LogoData.uiJpegSize));
                *(BgBuf+3) = HI_BYTE(HI_WORD(LogoData.uiJpegSize));
                memcpy((void *)(BgBuf+4),(void *)(LogoData.uiJpegAddress),LogoData.uiJpegSize);

                PStore_EnablePS();
                secnameId = PS_BG_LOGO;
                pSecHdl = PStore_OpenSection(secnameId, PS_RDWR | PS_CREATE);
                if (pSecHdl == E_PS_SECHDLER)
                {
                    debug_err(("Section open fail\r\n"));
                }

                erReturn = PStore_WriteSection(BgBuf, 0, (LogoData.uiJpegSize+4), pSecHdl);

                if (erReturn != E_PS_OK)
                    {
                        debug_err(("PStore program error\r\n"));
                    }

              } else {
                debug_msg("ReSize logo failed\r\n");
              }
        }
        else
            CmdTsk_OutputMsg((COMMAND_ERROR));
        break;

    // Command start with 'T'
    case 't':
        {
//#NT#2007/04/17#Stephen Wang -begin
//#NT#Add rtc test utility
            if (!strncmp(pcCmdStr, "time", 4))
            {
                if (strlen(pcCmdStr) > 6)
                {
                    if (!strncmp(pcCmdStr+5, "set", 3))
                    {
                        int year,month,day;
                        int hour,minute,second;
                        sscanf(pcCmdStr+9, "%d/%d/%d %d:%d:%d", &year,&month,&day,&hour,&minute,&second);
                        rtc_setDate(year,month,day);
                        rtc_setTime(hour,minute,second);
                        break;
                    }
                }
                {
                    RTC_DATE date;
                    RTC_TIME time;
                    date = rtc_getDate();
                    time = rtc_getTime();
                    CmdTsk_OutputMsg(("%d/%d/%d %d:%d:%d\r\n",date.s.year,date.s.month,date.s.day,time.s.hour,time.s.minute,time.s.second));
                }
            }
//#NT#2007/04/17#Stephen Wang -end
#if 0   //this is for internal test only since it will corrupt NAND data
            else if (!strncmp(pcCmdStr, "tno", 3))
            {
                nand_open();
            }
            else if (!strncmp(pcCmdStr, "tnc", 3))
            {
                nand_close();
            }
            else if (!strncmp(pcCmdStr, "tnr", 3))
            {
                UINT32 block = atoi(pcCmdStr+4);

                get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
                rel_blk(POOL_ID_CAPTURE, pBuf);

                // Read NAND data to DRAM
                nand_readReservedBlock((INT8 *)pBuf, block, 1);
            }
            else if (!strncmp(pcCmdStr, "tnw", 3))
            {
                UINT32 block = atoi(pcCmdStr+4);
                int i;

                get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
                rel_blk(POOL_ID_CAPTURE, pBuf);

                for (i=0; i<16384; i++)
                {
                    pBuf[i] = i;
                }
                // Read NAND data to DRAM
                nand_writeReservedBlock((INT8 *)pBuf, block, 1);
            }
#endif
//#NT#2007/05/10#Stephen Wang -begin
            //Stress test for clock change between photo and playback mode
            else if (!strncmp(pcCmdStr, "tclk", 4))
            {
                UINT32 loopcnt;
                int i;

                if (strlen(pcCmdStr) > 5)
                {
                    loopcnt = atoi(pcCmdStr+5);
                    for (i=0; i<loopcnt; i++)
                    {
                        CmdTsk_OutputMsg(("TestClk:%d\r\n",i));
                        SysClk_SetClk(SPEED_ID_PHOTO);
                        SysClk_SetClk(SPEED_ID_PLAYBACK);
                    }
                }

            }
//#NT#2007/05/10#Stephen Wang -end
        }
        break;

    // Command start with 'W'
    case 'w':
        if (!strncmp(pcCmdStr,"w ",2))
        {
            CmdTsk_WriteMemory((pcCmdStr+2));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    default:
        CmdTsk_OutputMsg((COMMAND_ERROR));
        break;
    }
} /* DoCommand */

/**
  Read memory and output to UART

  Read memory and output to UART.
  [Command Task internal API]

  @param CHAR *para_str: Command String
  @return void
*/
static void CmdTsk_ReadMemory(CHAR *para_str)
{
    CHAR    str_output[_UART_MONITOR_CMD_SIZE],out_ch;
    UINT32  i=0,j;
    UINT32  addr,length,str_len;
    CHAR    arg_num;

    arg_num=sscanf(para_str, "%x %x %s\n", &addr, &length, str_output);

    if (arg_num==1)
        length=256;

    addr&=0xFFFFFFFC;

    if ((arg_num==1) || (arg_num==2))
    {
        CmdTsk_OutputMsg(("addr = 0x%08X, length = 0x%08X\r\n", addr, length));
        for (i=0;i<length;)
        {

            str_len=sprintf(str_output,"%08lX : %08lX %08lX %08lX %08lX  ",addr, *((UINT32 *)addr),
                      *((UINT32 *)(addr+4)),*((UINT32 *)(addr+8)), *((UINT32 *)(addr+12)));
            for (j=0;j<16;j++)
            {
                out_ch = *((char *)(addr+j));
                if ((out_ch<0x20) || (out_ch>=0x80))
                    str_len+=sprintf(str_output+str_len,".");
                else
                    str_len+=sprintf(str_output+str_len,"%c",out_ch);
            }
            CmdTsk_OutputMsg((str_output));
            CmdTsk_OutputMsg(("\r\n"));
            i+=16;
            addr+=16;
        }
    }
    else
    {
        CmdTsk_OutputMsg((COMMAND_ERROR));
    }
}

/**
  Wriet memory according to command received from UART

  Wriet memory according to command received from UART.
  [Command Task internal API]

  @param CHAR *para_str: Command String
  @return void
*/
static void CmdTsk_WriteMemory(CHAR *para_str)
{
    UINT32  addr, data;
    CHAR    arg_num, str_input[_UART_MONITOR_CMD_SIZE];

    arg_num=sscanf(para_str, "%x %x %s\n", &addr, &data, str_input);
    if (arg_num==2)
    {
        addr &= 0xFFFFFFFC;
        *((int *) addr)=data;
        CmdTsk_OutputMsg(("addr = 0x%08X, data = 0x%08X\r\n", addr, data));
    }
    else
    {
        CmdTsk_OutputMsg((COMMAND_ERROR));
    }
}
int test_Daac(UINT32 ch, UINT32 bR)
{
	AudioProperty property;
    int frame = 0;
    UINT32 len, ms;
    UINT32 addr = 0x3000000, readaddr, temptime=0;
    UINT32 outaddr = 0x3200000;
	short *samples;             /* Input PCM buffer */
	unsigned char *frames;      /* Output Bitstream buffer*/
    AudioError sta;
	void *aacdecoder;
    UINT32 usedAAC, outputRaw;

	//infile = fopen("LC8k.pcm", "rb");
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    len = MediaAACGetBSLen();
    addr = (UINT32)0x3000000;
    FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, "A:\\aac0220.aac", (UINT8 *)addr, &len, 0, FST_TIME_INFINITE);



	//?? property.start = inbuf;
	//?? property.length = 4096;
    property.CodingType = 0;	//Raw Data:0, AAC Header:1

	/* Raw Data test */
	property.SampleRate = bR;
	property.Channel = 1;

	/* Create the AAC Encoder object */
	aacdecoder = AACD_Create(AACDCreateObj);

	if(AACD_Configure(aacdecoder, &property) != AUDIO_ERROR_NONE)
	{
	   debug_msg("AAC Encoder Configure Failure!\n");
	   //exit(1);
	   return;
	}
	sta = AACE_STATUS_OK;
	samples = (short *)outaddr;
	frames = (unsigned char *)addr;
    readaddr = addr;
    Perf_Open();

    Perf_Mark();
    while(readaddr < (addr+len))
    {
        sta = AACD_Run(aacdecoder, frames, samples);
        if (sta != AUDIO_ERROR_NONE)
        {
	        debug_msg("AACE_Run failure %d!!\n", sta);
            break;
        }
	    //  else
	    //  debug_msg("AACE_Run OK!!\n");
        frame++;
        usedAAC= AACD_GetBitstreamLength(aacdecoder);
        outputRaw = AACD_GetDataLength(aacdecoder);//*property.Channel;
        //debug_msg("used = %d, outsize=%d \n", usedAAC, outputRaw);

        readaddr += usedAAC;
        frames += usedAAC;
        samples += (outputRaw/2);
    }
    temptime = Perf_GetDuration()/1000;
    Perf_Close();

    debug_msg("total time = %d ms, frame=%d!\r\n", temptime, frame);

	debug_msg("Sample Rate = %d, Bit Rate = %d, Channel = %d\n",
	       AACD_GetSampleRate(aacdecoder),
	       AACD_GetBitRate(aacdecoder),
	       AACD_GetChannel(aacdecoder));

	/* Delete the AAC Encoder object */
	//AACE_Delete(aaceobj, AACEDeleteObj);
	FilesysCloseActFile();
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
#if 1
            FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,   //UW Cmd,
                        (UB*)outaddr,  //UB *pBuf,
                        (UINT32)samples-outaddr,            //UW BufSize,
                        0,                      //UW Pos,
                        1,                      //BOOL CloseFile,
                        FST_FMT_RAW,            //UW FileFormat,
                        FST_TIME_INFINITE);     //UW TimeOut
#endif
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    debug_msg("file write OK!\r\n");

}

void NAVIToDVR_DoCommandString(CHAR * pcCmdStr)
{
    static UINT32 Cmd_counter;
    UINT32 CheckSum;
    UINT32 DataSize;
    UINT32 i;

    UINT8 Cmd_Return[6];

    switch (*pcCmdStr)
    {
    	 case NAVI_CMD:
		if((pcCmdStr[Byte_SA] == SA_NAVI)&&(pcCmdStr[Byte_DA] == SA_DVR))
		{
			if(Cmd_counter !=pcCmdStr[Byte_SN])
			{
				DataSize = pcCmdStr[Byte_SIZE];
				for(i=0;i<(CMD_HEADER+DataSize);i++)
				{
					CheckSum+= pcCmdStr[i];
				}
				
				CheckSum = (CheckSum&0xff);
				
				if(CheckSum == pcCmdStr[CMD_HEADER+DataSize])
				{
				switch(pcCmdStr[Byte_CTL])
				 {
				 	case CMD_GPS_INFO:
						if(pcCmdStr[Byte_SIZE] == 0x012)
						{
							
						}
						else
						{
							CmdTsk_OutputMsg(("key cmd error...\r\n"));
							Cmd_Return[0]=0x06;
							Cmd_Return[1]=0xdd;
							Cmd_Return[2]=0x55;
							Cmd_Return[3]=Cmd_counter;
							Cmd_Return[4]=0x01;	
							Cmd_Return[5]=0x00;					 							
							PutHexData(Cmd_Return,sizeof(Cmd_Return));
						}
						break;
					case CMD_CTL_TOUCHKEY:
						if(pcCmdStr[Byte_SIZE] == 0x01)
						{
							if(pcCmdStr[Byte_SIZE+1] == 0x00)  //key enter
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;								
							}
							else if(pcCmdStr[Byte_SIZE+1] == 0x01) //key up
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;									
							}
							else if(pcCmdStr[Byte_SIZE+1] == 0x02) //key down
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;								
							}
							else if(pcCmdStr[Byte_SIZE+1] == 0x03) //key mode
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;								
							}
							else if(pcCmdStr[Byte_SIZE+1] == 0x04) //key shutter2
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;								
							}
							else if(pcCmdStr[Byte_SIZE+1] == 0x05) //key mute
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;								
							}
							else if(pcCmdStr[Byte_SIZE+1] == 0x06) //key menu
							{
								Cmd_Return[0]=0x06;
								Cmd_Return[1]=0xdd;
								Cmd_Return[2]=0x55;
								Cmd_Return[3]=Cmd_counter;
								Cmd_Return[4]=0x00;
								Cmd_Return[5]=0x00;										
            							Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
								PutHexData(Cmd_Return,sizeof(Cmd_Return));	
								Cmd_counter = 0;								
							}							
						}
						else
						{
							CmdTsk_OutputMsg(("key cmd error...\r\n"));
							Cmd_Return[0]=0x06;
							Cmd_Return[1]=0xdd;
							Cmd_Return[2]=0x55;
							Cmd_Return[3]=Cmd_counter;
							Cmd_Return[4]=0x01;		
							Cmd_Return[5]=0x00;									
							PutHexData(Cmd_Return,sizeof(Cmd_Return));
						}
						break;
					default:
						break;
				}
				}
				else
				{
					CmdTsk_OutputMsg(("check sum error %x != %x\r\n",CheckSum,pcCmdStr[CMD_HEADER+DataSize+1]));
					Cmd_Return[0]=0x06;
					Cmd_Return[1]=0xdd;
					Cmd_Return[2]=0x55;
					Cmd_Return[3]=Cmd_counter;
					Cmd_Return[4]=0x01;	
					Cmd_Return[5]=0x00;							
					PutHexData(Cmd_Return,sizeof(Cmd_Return));						
				}

			}
		   else
		   {
			 CmdTsk_OutputMsg(("send cmd repeat Cmd_counter : %d SN : %d \r\n",Cmd_counter,pcCmdStr[Byte_SN]));
			Cmd_Return[0]=0x06;
			Cmd_Return[1]=0xdd;
			Cmd_Return[2]=0x55;
			Cmd_Return[3]=Cmd_counter;
			Cmd_Return[4]=0x01;					 
			Cmd_Return[5]=0x00;					 			
			 PutHexData(Cmd_Return,sizeof(Cmd_Return));									 
		   }
		}
	break;
	default:
	       CmdTsk_OutputMsg(("wrong cmd : %s\r\n",pcCmdStr));		
	break;		   
    }
}

void NAVIToDVR_DoCommandChar(CHAR * pcCmdStr)
{

}
#endif
