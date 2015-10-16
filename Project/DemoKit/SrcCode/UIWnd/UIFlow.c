#include "UIFlow.h"
#include "debug.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "UIFlowWndPhoto.h"
#include "UIFlowWndMovie.h"
#include "UIMenuWndUSB.h"
#include "UIPhotoObj.h"
#include "UIMovieObj.h"
#include "UIPlayObj.h"
#include "UISystemObj.h"
#include "UIFlowWndPrint.h"
#include "UIPrintObj.h"
#include "UIFlowWndPlay.h"
#include "UIFlowWndMP3.h"
#include "UIFlowWndWrnMsg.h"
#include "UIFlowWndWrnStrgHotPlug.h"
#include "UIMenuWndCalibration.h"
#include "UIMenuWndPCB.h"
#include "ide.h"
//-----------------------------------------------------------------------------
// Photo Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowPhoto_Open(void)
{
    Ux_SetSysObj(&UIPhotoObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_OpenWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
    debug_ind(("FlowPhoto_Open()\r\n"));
    ide_enable_video(IDE_VIDEOID_1);
    return E_OK;
}

ER FlowPhoto_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndPhotoCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Movie Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowMovie_Open(void)
{
    Ux_SetSysObj(&UIMovieObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_OpenWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
    //ide_enable_video(IDE_VIDEOID_1);
    debug_ind(("FlowMovie_Open()\r\n"));
    return E_OK;
}

ER FlowMovie_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndMovieCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Playback Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowPB_Open(void)
{
    Ux_SetSysObj(&UIPlayObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_PLAYINIT,0);
    Ux_OpenWindow((VControl *)(&UIFlowWndPlayCtrl), 0);
    debug_ind(("FlowPB_Open()\r\n"));
    return E_OK;
}

ER FlowPB_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndPlayCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// MP3 Mode Main Flow Window
//-----------------------------------------------------------------------------
ER FlowMP3_Open(void)
{
    //Ux_SetSysObj(&UIMP3ObjCtrl);
    Ux_SetSysDefCmd(UISystemObjCtrlCmdMap);
    Ux_OpenWindow((VControl *)(&UIFlowWndMP3Ctrl), 0);
    debug_ind(("FlowMP3_Open()\r\n"));
    return E_OK;
}

ER FlowMP3_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndMP3Ctrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Battery Empty Notice Window
//-----------------------------------------------------------------------------
ER FlowWrnBatteryEmpty_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIFlowWndWrnMsgCtrl), 2, FLOWWRNMSG_ISSUE_BATTERY_LOW, FLOWWRNMSG_TIMER_KEEP);
    debug_ind(("FlowWrnBatteryEmpty_Open()\r\n"));
    return E_OK;
}

ER FlowWrnBatteryEmpty_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndWrnMsgCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Storage Card Hot Plug/Unplug Notice Window
//-----------------------------------------------------------------------------
ER FlowWrnStrgHotPlug_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIFlowWndWrnStrgHotPlugCtrl), 0);
    debug_ind(("FlowWrnStrgHotPlug_Open()\r\n"));
    return E_OK;
}

ER FlowWrnStrgHotPlug_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndWrnStrgHotPlugCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Update DCF Folder/File name Utility
//-----------------------------------------------------------------------------
void FlowDCF_UpdateName(BOOL IsJPG)
{
 CHAR FolderName[6]="MEDIA";
 CHAR FileName[5]="PTDC";
 char DCFpathname[0x20];
 RTC_DATE  curDate;
 RTC_TIME  curTime;
 UINT32    uiVideoFmt;

    curDate = rtc_getDate();
    curTime = rtc_getTime();
    sprintf(FolderName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);
    sprintf(FileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysGetDCFPath((UB *)DCFpathname,TRUE);
    debug_msg("FlowDCF_UpdateName : %s ,%s-----\r\n",DCFpathname,FolderName);
	
    if (strncmp(DCFpathname, FolderName, 5))
    {
        FilesysSetDCFPath((UB *)FolderName, TRUE);
    }
    if (IsJPG==TRUE)
    {
        FilesysSetDCFPath((UB *)FileName, FALSE);
    } else {
        Appinit_ModeMovie_GetVideoFmt(&uiVideoFmt);
        if (uiVideoFmt==MEDIAREC_AVI)
            FilesysSetDCFFilename((INT8 *)FileName, FST_FMT_AVI);
        else
            FilesysSetDCFFilename((INT8 *)FileName, FST_FMT_MOV);
    }
}

//-----------------------------------------------------------------------------
// USB Menu Window
//-----------------------------------------------------------------------------
ER MenuUSB_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIMenuWndUSBCtrl), 0);
    debug_ind(("MenuUSB_Open()\r\n"));
    return E_OK;
}

ER MenuUSB_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIMenuWndUSBCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Printer Menu Window
//-----------------------------------------------------------------------------
ER MenuPrint_Open(void)
{
    Ux_SetSysObj(&UIPrintObjCtrl);
    Ux_OpenWindow((VControl *)(&UIFlowWndPrintCtrl), 0);
    debug_ind(("MenuPrint_Open()\r\n"));
    return E_OK;
}

ER MenuPrint_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIFlowWndPrintCtrl), 0);
    return E_OK;
}

//-----------------------------------------------------------------------------
// Calibration Menu Window
//-----------------------------------------------------------------------------
#if (_CALIBRATION_MODE_ == ENABLE)
ER MenuCalibration_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIMenuWndCalibrationCtrl), 0);
    debug_ind(("MenuCalibration_Open()\r\n"));
    return E_OK;
}

ER MenuCalibration_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIMenuWndCalibrationCtrl), 0);
    return E_OK;
}
#endif

//-----------------------------------------------------------------------------
// PCB Menu Window
//-----------------------------------------------------------------------------
ER MenuPCB_Open(void)
{
    Ux_OpenWindow((VControl *)(&UIMenuWndPCBCtrl), 0);
    debug_ind(("MenuPCB_Open()\r\n"));
    return E_OK;
}

ER MenuPCB_Close(void)
{
    Ux_CloseWindowClear((VControl *)(&UIMenuWndPCBCtrl), 0);
    return E_OK;
}

