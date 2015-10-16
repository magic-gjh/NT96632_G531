/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanMisc.c
    @ingroup    mIPRJAPKeyIO

    @brief      Miscellaneous KeyScan functions
                Miscellaneous KeyScan functions

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Kernel.h"
#include "KeyScanInt.h"
#include "KeyScanTsk.h"
#include "FileSysTsk.h"
#include "Timer.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UISystemStatusSettings.h"
#include "UIMenuPhotoSettings.h"

static BOOL     bBusyLED = FALSE;

/**
  Detect system is busy or not

  Detect system is busy or not.
  The following contidions indicate that system is busy:
    (1) Flash is charging
    (2) FileSystem is reading/writing
  If system is busy then toggle red LED
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetBusy(void)
{
#if (_FLASHLIGHT_ != _FLASHLIGHT_NONE_)
    if (KeyScan_IsFlashCharging() == TRUE)
    {
        Ux_PostEvent(NVTEVT_FLASHCHARGING, 1, NVTEVT_FLASHCHARGING_PROC);
        if (bBusyLED == FALSE)
        {
            bBusyLED = TRUE;
            KeyScan_EnableLEDToggle(KEYSCAN_LED_FCS, TRUE);
        }
    }
    else
    {
        if (bBusyLED == TRUE)
        {
            bBusyLED = FALSE;
            KeyScan_EnableLEDToggle(KEYSCAN_LED_FCS, FALSE);
            Ux_PostEvent(NVTEVT_FLASHCHARGING, 1, NVTEVT_FLASHCHARGING_END);
        }
    }
#endif
}

/**
  Suspend KeyScan task

  Suspend KeyScan task. The KeyScan timer interrupt will be disabled.
  All key event will be disabled.

  @param void
  @return void
*/
void KeyScan_Suspend(void)
{
    timer_pausePlay(g_uiKeyScanTimerID, _TIMER_PAUSE);

    //#NT#2008/06/11#Steven Wang -begin
    //clr_flg(FLG_ID_INT2, FLGINT2_TIMER0 << g_uiKeyScanTimerID);
    //#NT#2008/06/11#Steven Wang -end
}

/**
  Resume KeyScan task

  Resume KeyScan task.

  @param void
  @return void
*/
void KeyScan_Resume(void)
{
    timer_pausePlay(g_uiKeyScanTimerID, _TIMER_PLAY);
}

/**
  Detect Macro h/w switch is ON or OFF

  Detect Macro h/w switch is ON or OFF

  @param void
  @return void
*/
void KeyScan_DetMacro(void)
{
    static UINT32   uiMacroPrevSts = 0xFFFFFFFF;
    UINT32          uiMacroCurSts;

    if(GPIOMap_DetMacro())
    {
        uiMacroCurSts =  MACRO_ON ;
    }
    else
    {
        uiMacroCurSts =  MACRO_OFF ;
    }

    //debug_err(("uiMacroPrevSts %x uiMacroCurSts %x  \r\n", uiMacroPrevSts, uiMacroCurSts));

    if(uiMacroPrevSts != uiMacroCurSts)
    {
        Set_MacroIndex(uiMacroCurSts);

        if (Ux_IsInitMessage())
        {
            Ux_PostEvent(NVTEVT_MACRO_CHANGE, 0);
            uiMacroPrevSts = uiMacroCurSts;
        }
        else
        {
            //clear init stat for detect change
            uiMacroPrevSts = 0xFFFFFFFF;
        }
    }
}

void KeyScan_SetTime(void)
{
    HNVT_FILE   *pFile;
    HNVT_FILE   *pFile2;

    CHAR uiTimeFileName[] = "A:\\TIMESET.txt";
     CHAR datetime[32];
    int i = 0;
    UINT32  Year,Month,Day,Hour,Minute,Second;
    BOOL settimeDone = FALSE;

    if (GPIOMap_DetStrgCard() == FALSE)
    {
        debug_err(("No card detected for opening %s\r\n", uiTimeFileName));
        return ;
    }

    // wait file system finish
    FilesysWaitInitFinish(FST_TIME_INFINITE);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    pFile = fs_fopen((char *)uiTimeFileName, "R");
    if(pFile->fileSize < 1)
     return;

    if(pFile != NULL)
    {
        fs_fread((void *)datetime, 1, pFile->fileSize,pFile);
        
        if (datetime[11]== ' ')
            datetime[11]= '0';
        for(i=0; i<19; i++)
        {
               if (datetime[i]>='0'&&datetime[i]<='9')
                   datetime[i] = datetime[i] - '0';
                //debug_err(("datetime[%d] :%d \r\n",i,datetime[i]));
        }
        Year = datetime[0] * 1000 + datetime[1] * 100 + datetime[2] * 10 + datetime[3];
        if((datetime[4]>= 0) && (datetime[4]<= 9))
        {
                debug_err(("KeyScan_SetTime error format  !!\r\n"));
                return;
        }
        Month = datetime[5] *10 + datetime[6];
        if((datetime[7]>= 0) && (datetime[7]<= 9))
        {
                debug_err(("KeyScan_SetTime error format  !!\r\n"));
                return;
        }
        Day = datetime[8] * 10 + datetime[9];
        if((datetime[10]>= 0) && (datetime[10]<= 9))
        {
                debug_err(("KeyScan_SetTime error format  !!\r\n"));
                return;
        }
        Hour = datetime[11] * 10 + datetime[12];
        if((datetime[13]>= 0) && (datetime[13]<= 9))
        {
                debug_err(("KeyScan_SetTime error format  !!\r\n"));
                return;
        }
        Minute = datetime[14] * 10 + datetime[15];
        if((datetime[16]>= 0) && (datetime[16]<= 9))
        {
                debug_err(("KeyScan_SetTime error format  !!\r\n"));
                return;
        }
        Second = datetime[17] * 10 + datetime[18];
        if((Year > 2050) || (Year < 2010))
            Year = 2010;
        if((Month > 12) || (Month < 1))
            Month = 1;
        if((Day > 31) || (Day < 1))
            Day = 1;
        if((Hour > 23) || (Hour < 1))
            Hour = 0;
        if((Minute > 59) || (Minute < 1))
            Minute = 1;
        if((Second > 59) || (Second < 1))
            Second = 1;
       
       rtc_setDate(Year, Month, Day);
       rtc_setTime( Hour, Minute, Second);
       settimeDone = TRUE;
    }
    else
    {
        debug_err(("open file error \r\n"));
        return;
    }
   debug_err(("KeyScan_SetTime finish !!\r\n"));

   fs_fclose(pFile);
   if(settimeDone)
    {
        FilesysDeleteByName(uiTimeFileName,FALSE);
     }


}
//@}
