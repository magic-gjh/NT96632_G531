#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIMenuPhotoSettings.h"
#include "UIMenuPlaySettings.h"
#include "UIMenuMovieSettings.h"
#include "UISystemStatusSettings.h"
#if (UI_STYLE == UI_STYLE_DVPLUS)
#include "UIMenuWndPhotoStill.h"
#endif
#include "UIMenuWndMovieVideo.h"
#include "Debug.h"
#include "PlaybackTsk.h"
#include "PrimaryTsk.h"
#include "SieCapTsk.h"
#include "UIResource.h"
#include "FilesysTsk.h"
#include "KeyScanTsk.h"
#include "GlobalVar.h"
#include "UIPhotoObj.h"
#include "UIPlayObj.h"
#include "UIMovieObj.h"
#include "UISystemObj.h"
#include "SysCfg.h"
#include "DemoKit_String.h"
#include "AppInitPlayback.h"
#include "Audio.h"
#include "PhotoDisplayTsk.h"
#include "Alg_IPLMode.h"

//#NT#2009/09/21#Chris Chung -begin
INT32 OnForceToLiveviewMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
//#NT#2009/09/21#Chris Chung -end


static void Sys_SetPhotoDispByImageSize(void)
{
    UINT32 uiCurrMode = Primary_GetCurrentMode();
    IPL_OBJ IplObj;

    if (uiCurrMode == PRIMARY_MODE_MOVIE || uiCurrMode == PRIMARY_MODE_PHOTO)
    {
        PHOTODISPLAY_APPOBJ PhotoDisplayObj;
        PhotoDisplay_Close();
        PhotoDisplayObj.uiDisplayFBWidth     = g_LCDSize.uiWidth;
        PhotoDisplayObj.uiDisplayFBHeight    = g_LCDSize.uiHeight;
        PhotoDisplayObj.uiDisplayOutWidth    = g_LCDSize.uiWinWidth;
        PhotoDisplayObj.uiDisplayOutHeight   = g_LCDSize.uiWinHeight;
        PhotoDisplay_Open(&PhotoDisplayObj);

        if (uiCurrMode == PRIMARY_MODE_MOVIE)
        {
            IplObj.Mode     = IPL_MODE_VIDEO;        
        }
        else
        {
            IplObj.Mode     = IPL_MODE_PREVIEW;        
        }
        IplObj.uiImeOutWidth1 = g_LCDSize.uiWinWidth;
        IplObj.uiImeOutHeight1 = g_LCDSize.uiWinHeight;
        IplObj.uiCmd    = IPL_CMD_CHG_SIZE_IME1;
        IPL_SetCmd(&IplObj);

        if (uiCurrMode == PRIMARY_MODE_MOVIE)
        {
            PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
            Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIESIZE,1,Get_MovieSizeIndex());
        }
        else
        {
            PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, TRUE);
            Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SIZE,1,Get_SizeIndex());
        }
    }
}

//#NT#2010/10/25#Philex Lin -begin
// for Ramdisk utilization
int SysGetCaptureAddr(UINT32 *addr)
{
    int err = get_blk((VP)addr, POOL_ID_CAPTURE);
    if (err == E_OK)
        rel_blk(POOL_ID_CAPTURE, (VP)*addr);
    //debug_err(("SysGetCaptureAddr(), addr = 0x%x\r\n", *addr));
    return (err);
}

#if (_RAM_DISK_ == ENABLE)
int SysGetRamDiskAddr(UINT32 *addr)
{
    int err = get_blk((VP)addr, POOL_ID_FTABLE);
    if (err == E_OK)
        rel_blk(POOL_ID_FTABLE, (VP)*addr);


#if 0
    int err = SysGetCaptureAddr(addr);
    if (err == E_OK)
        *addr += (POOL_SIZE_CAPTURE - POOL_SIZE_RAM_DISK);
    //debug_err(("SysGetRamDiskAddr(), addr = 0x%x\r\n", *addr));
    return (err);
#endif
}
#endif
//#NT#2010/10/25#Philex Lin -end

///////////////////////////////////////////////////////////////////
UINT32 Get_BeepValue(UINT32 uiIndex)
{
    //#NT#2009/06/15#Ks Hung -begin
    switch(uiIndex)
    {
        case BEEP_OFF:
            return FALSE;
        case BEEP_ON:
            return TRUE;
    }
    //#NT#2009/06/15#Ks Hung -end
    return FALSE;
}

UINT32 Get_VolumeValue(UINT32 uiIndex)
{
    switch(uiIndex)
    {
        case VOLUME_LVL_OFF:
            return AUDIO_VOL_MUTE;
        case VOLUME_LVL_1:
            return AUDIO_VOL_3;
        case VOLUME_LVL_2:
            return AUDIO_VOL_5;
        case VOLUME_LVL_3:
            return AUDIO_VOL_7;
    }
    return AUDIO_VOL_5;
}

UINT32 Get_PowerOffValue(UINT32 uiIndex)
{
    //#NT#2009/06/15#Ks Hung -begin
    switch(uiIndex)
    {
        case POWEROFF_2MIN:
            return 120;
        case POWEROFF_5MIN:
            return 300;
        case POWEROFF_10MIN:
            return 600;
        case POWEROFF_DISABLED:
            return KEYSCAN_AUTOPOWEROFF_DISABLED;
    }
    //#NT#2009/06/15#Ks Hung -end
    return 0;
}


UINT32 Get_LCDOffValue(UINT32 uiIndex)
{
    switch(Get_LcdOffIndex())
    {
        case LCDOFF_1MIN:
            return 60;
			
        case LCDOFF_3MIN:
            return 180;
            
        case LCDOFF_5MIN:
            return 300;
			
        case LCDOFF_OFF:
            return KEYSCAN_AUTOPOWEROFF_DISABLED;
    }
    return KEYSCAN_AUTOPOWEROFF_DISABLED;
}


UINT32 Get_FreqValue(UINT32 uiIndex)
{
    //#NT#2009/06/15#Ks Hung -begin
    switch(uiIndex)
    {
        case FREQ_50HZ:
            return _POWERFreq_50Hz;
        case FREQ_60HZ:
            return _POWERFreq_60Hz;
    }
    //#NT#2009/06/15#Ks Hung -end
    return _POWERFreq_60Hz;
}

UINT32 Get_LanguageValue(UINT32 uiIndex)
{
    switch(uiIndex)
    {
        case ENGLISH:
            return (UINT32)&gDemoKit_String_EN;
        case GERMAN:
            return (UINT32)&gDemoKit_String_DE;
        case FRANCE:
            return (UINT32)&gDemoKit_String_FR;
        case SPANISH:
            return (UINT32)&gDemoKit_String_ES;
        case ITALIAN:
            return (UINT32)&gDemoKit_String_IT;
        case PORTUGUESE:
            return (UINT32)&gDemoKit_String_PO;
        case TCHINESE:
            return (UINT32)&gDemoKit_String_TC;
        case SCHINESE:
            return (UINT32)&gDemoKit_String_SC;
        case JAPANESE:
            return (UINT32)&gDemoKit_String_JP;
	 case RUSSIAN:
	      return (UINT32)&gDemoKit_String_RU;
    }
    return (UINT32)&gDemoKit_String_EN;
}
///////////////////////////////////////////////////////////////////////

INT32 OnExe_Format(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_BackgroundPostEvent(NVTEVT_BKW_FORMAT);
    return NVTEVT_CONSUME;
}

INT32 OnExe_Beep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    Set_BeepIndex(uiSelect);
    KeyScan_EnableKeyTone(Get_BeepValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_Volume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AUDIO_DEVICE_OBJ AudioDevObj;
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    Set_VolumeIndex(uiSelect);

    if (aud_getLockStatus() == TASK_LOCKED)
    {
        aud_setVolume(Get_VolumeValue(uiSelect));
    }
    else
    {
        aud_getDeviceObject(&AudioDevObj);
        aud_open(&AudioDevObj);
        aud_setVolume(Get_VolumeValue(uiSelect));
        aud_close();
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_Language(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    Set_LanguageIndex(uiSelect);
    debug_err(("[OnExe_Language][selected = %d] %x \r\n", uiSelect, Get_LanguageValue(uiSelect)));
    GxGfx_SetStringTable((const STRING_TABLE*)Get_LanguageValue(uiSelect));

    return NVTEVT_CONSUME;
}

INT32 OnExe_TVMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0,uiOldMode;;
    if(paramNum>0)
        uiSelect= paramArray[0];
	
    uiOldMode = Get_TVModeIndex();
    Set_TVModeIndex(uiSelect);
    //debug_ind(("[OnExe_TVMode][selected = %d] %x \r\n", uiSelect, Get_TVModeValue(uiSelect)));
    //GxGfx_SetStringTable((const STRING_TABLE*)Get_TVModeValue(uiSelect));

    if ((KeyScan_IsTVPlugIn() == TRUE) && (uiSelect != uiOldMode))
    {
        KeyScan_ChangeTVMode();
        Sys_SetPhotoDispByImageSize();
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_PowerOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
    Set_PowerOffIndex(uiSelect);
    KeyScan_SetAutoPoweroffTime(Get_PowerOffValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_SysReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    SysReset_MenuInfo();

    return NVTEVT_CONSUME;
}

INT32 OnExe_NumReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    if((PB_ReadCurrFileSysInfo(PLAYFILEINFO_MAXDIRID) == MIN_DCF_DIR_NUM) && (PB_ReadCurrFileSysInfo(PLAYFILEINFO_DIRNUMS) == 0) && (PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILENUMS) == 0))
        Set_FilesysDirId(MIN_DCF_DIR_NUM);
    else
        Set_FilesysDirId((PB_ReadCurrFileSysInfo(PLAYFILEINFO_MAXDIRID)+1));

    Set_FilesysFileId(MIN_DCF_FILE_NUM);
    debug_msg("OnExe_NumReset(), DirNum = %d, FileNum = %d\r\n", Get_FilesysDirId(), Get_FilesysFileId());
    FilesysSetWriteNextFileId(((UINT16)Get_FilesysDirId()), ((UINT16)Get_FilesysFileId()), TRUE);
    //Save DirNumber & FileNumber
    Save_MenuInfo();

    return NVTEVT_CONSUME;
}

INT32 OnExe_Freq(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
    Set_FreqIndex(uiSelect);
    ImgCapture_SetParameter(_Frequency,(UINT8)Get_FreqValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_DelayPowerOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
	
    if(paramNum>0)
        uiSelect= paramArray[0];
	
    Set_DelayPowerOffIndex(uiSelect);
	
    return NVTEVT_CONSUME;
}

INT32 OnExe_LcdOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
	
    if(paramNum>0)
        uiSelect= paramArray[0];
	
    Set_LcdOffIndex(uiSelect);

    KeyScan_SetAutoLCDoffTime(Get_LCDOffValue(uiSelect));
	
    return NVTEVT_CONSUME;
}

INT32 OnExe_Rotate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];
    Set_ImageRotateIndex(uiSelect);
	
    switch(Get_ImageRotateIndex())
    {
	   case IMAGE_ROTATE_ON:
		UI_SetParameter(_ImageFlip,_Flip_H_V);   		
	   break;
	   case IMAGE_ROTATE_OFF:
		UI_SetParameter(_ImageFlip,_Flip_Off);   		
	   break;	   
    }
    return NVTEVT_CONSUME;
}



INT32 OnExe_Bright(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    Set_BrightIndex(uiSelect);
    //SetLCDBright(uiSelect);   //#NewUx porting: Temporarily marked for compile error
    return NVTEVT_CONSUME;
}

INT32 OnExe_Display(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    Set_DisplayIndex(uhSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_DateFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
    Set_DateFormatIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_USB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    switch(uiSelect)
    {
   //case USB_MODE_PICTBRIDGE:
        //Primary_ChangeMode(PRIMARY_MODE_USBSICD);
        //break;
    case USB_MODE_PCC:
        Primary_ChangeMode(PRIMARY_MODE_USBPCC);
        break;
    default:
        Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
        break;
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_Opening(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];

    Set_OpeningIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnChangeDscMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiType = DSCMODE_CHGTO_NEXT;
    UINT32 uiCurrMode = Primary_GetCurrentMode();
    UINT32 uiChangeTo = 0;

    if(paramNum>0)
        uiType = paramArray[0];

    switch(uiType)
    {
        case DSCMODE_CHGTO_NEXT:
            switch(uiCurrMode)
            {
                case PRIMARY_MODE_MOVIE:
                    uiChangeTo = PRIMARY_MODE_PHOTO;
                    break;

                case PRIMARY_MODE_PLAYBACK:
                   if(KeyScan_IsHDMIPlugIn())//Allow only playback mode when HDMI plugged in
                    {
                        //OnForceToLiveviewMode(pCtrl, paramNum, paramArray);
                        return NVTEVT_CONSUME;                        
                    }
                    uiChangeTo = PRIMARY_MODE_MOVIE;
			break;
                case PRIMARY_MODE_PHOTO:
                    uiChangeTo = PRIMARY_MODE_PLAYBACK;
                    break;					
                default:
                    uiChangeTo = PRIMARY_MODE_MOVIE;
                    break;
            }
            break;

        case DSCMODE_CHGTO_PREV:
            if((uiCurrMode == PRIMARY_MODE_PLAYBACK) && (KeyScan_IsHDMIPlugIn()||KeyScan_IsTVPlugIn))   //Allow only playback mode when HDMI plugged in
            {
                return NVTEVT_CONSUME;
            }
            uiChangeTo = Primary_GetPreviousMode();
            break;

        case DSCMODE_CHGTO_CURR:
            uiChangeTo = uiCurrMode;
            break;

        default:
            debug_err(("UISystemObj: Unknown change type 0x%x\r\n", uiType));
            break;
    }
    Primary_ChangeMode(uiChangeTo);

    return NVTEVT_CONSUME;
}

INT32 OnForceToLiveviewMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiCurrMode = Primary_GetCurrentMode();
    UINT32 uiChangeTo = 0;
    VControl *pCloseWnd = NULL;

    switch(uiCurrMode)
    {
        case PRIMARY_MODE_PHOTO:
        case PRIMARY_MODE_MOVIE:
            /* Close all the window upper than root */
            Ux_GeWindowByIndex(&pCloseWnd, UX_2NDLAYER_WND_IDX);
            if(pCloseWnd)
            {
                Ux_CloseWindow(pCloseWnd, 0);
            }
            break;

        case PRIMARY_MODE_PLAYBACK:
            if(!KeyScan_IsHDMIPlugIn())   //Allow only playback mode when HDMI plugged in
            {
                #if (_PLAYBACK_FILESHOW_ == _PLAYBACK_FILESHOW_SEPA_)
                /* Switch to DC/DV liveview mode based on DC/DV playback mode */
                switch(AppInit_ModePlayback_Get_Mode())
                {
                    case APP_PLAYBACK_BR_JPG:
                        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
                        break;
                    case APP_PLAYBACK_BR_VIDEO:
                    default:
                        Primary_ChangeMode(PRIMARY_MODE_MOVIE);
                        break;
                }
                #else
                /* Switch to DC/DV liveview mode based on the previous mode */
                switch(Primary_GetPreviousMode())
                {
                    case PRIMARY_MODE_PHOTO:
                        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
                        break;
                    default:
                        Primary_ChangeMode(PRIMARY_MODE_MOVIE);
                        break;
                }
                #endif
            }
            break;

        default:
            debug_err(("UISystemObj: Unknown current DSC mode 0x%x\r\n", uiCurrMode));
            break;
    }

    return NVTEVT_CONSUME;
}

INT32 OnForceToPlaybackMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (_PLAYBACK_FILESHOW_ == _PLAYBACK_FILESHOW_SEPA_)
    //#NT#2009/09/01#Chris Chung -begin
    //#NT#add for HDV5D4 playback key
    switch(Primary_GetCurrentMode())
    {
    case PRIMARY_MODE_PHOTO:
        AppInit_ModePlayback_Set_Mode(APP_PLAYBACK_BR_JPG);
        break;

    case PRIMARY_MODE_PLAYBACK:
        switch(AppInit_ModePlayback_Get_Mode())
        {
        case APP_PLAYBACK_BR_VIDEO:
            AppInit_ModePlayback_Set_Mode(APP_PLAYBACK_BR_JPG);
            break;
        case APP_PLAYBACK_BR_JPG:
        default:
            AppInit_ModePlayback_Set_Mode(APP_PLAYBACK_BR_VIDEO);
        }
        break;

    case PRIMARY_MODE_MOVIE:
    default:
        AppInit_ModePlayback_Set_Mode(APP_PLAYBACK_BR_VIDEO);
    }
    //#NT#2009/09/01#Chris Chung -end
    Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
    #else
    if (Primary_GetCurrentMode() != PRIMARY_MODE_PLAYBACK)
    {
        AppInit_ModePlayback_Set_Mode(APP_PLAYBACK_FLOW);
        Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
    }
    #endif
    return NVTEVT_CONSUME;
}

INT32 OnSaveTPCal(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (_TouchPanel_ != DISABLE)
    UINT32 uiSelect = 0;
    if(paramNum>0)
        uiSelect= paramArray[0];
    Set_TPCalFlag(uiSelect);
    Save_MenuInfo();
    #endif

    return NVTEVT_CONSUME;
}
////////////////////////////////////////////////////////////

EVENT_ENTRY UISystemObjCtrlCmdMap[] =
{
    {NVTEVT_EXE_FORMAT,                 OnExe_Format},
    {NVTEVT_EXE_BEEP,                   OnExe_Beep},
    {NVTEVT_EXE_VOLUME,                 OnExe_Volume},
    {NVTEVT_EXE_LANGUAGE,               OnExe_Language},
    {NVTEVT_EXE_TVMODE,                 OnExe_TVMode},
    {NVTEVT_EXE_POWEROFF,               OnExe_PowerOff},
    {NVTEVT_EXE_SYSRESET,               OnExe_SysReset},
    {NVTEVT_EXE_NUMRESET,               OnExe_NumReset},
    {NVTEVT_EXE_FREQ,                   OnExe_Freq},
    {NVTEVT_EXE_LCDOFF,                   OnExe_LcdOff},       
    {NVTEVT_EXE_DELAYPOWEROFF,               OnExe_DelayPowerOff},
    {NVTEVT_EXE_IMAGEROTATE,               OnExe_Rotate},        
    {NVTEVT_EXE_BRIGHT,                 OnExe_Bright},
    {NVTEVT_EXE_DISPLAY,                OnExe_Display},
    {NVTEVT_EXE_DATEFORMAT,             OnExe_DateFormat},
    {NVTEVT_EXE_USB,                    OnExe_USB},
    {NVTEVT_EXE_OPENING,                OnExe_Opening},
    {NVTEVT_CHANGE_DSCMODE,             OnChangeDscMode},
    {NVTEVT_FORCETO_LIVEVIEW_MODE,      OnForceToLiveviewMode},
    {NVTEVT_FORCETO_PLAYBACK_MODE,      OnForceToPlaybackMode},
    {NVTEVT_SAVE_TP_CAL,                OnSaveTPCal},
    {NVTEVT_NULL,                       0},  //End of Command Map
};

VControl UISystemObjCtrl={
    "UISystemObj",
    UISystemObjCtrlCmdMap,
    0,
    CTRL_SYS,   // CTRL_SYS
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};
