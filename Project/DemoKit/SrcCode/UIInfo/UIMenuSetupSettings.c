#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "SysCfg.h"
#include "UIMenuSetupSettings.h"
#include "UICfgDefault.h"

static UINT32 guiBeepIndex          = UIDFT_BEEP;
static UINT32 guiVolumeIndex        = UIDFT_VOLUME;
static UINT32 guiLanguageIndex      = UIDFT_LANGUAGE;
static UINT32 guiTVModeIndex        = UIDFT_TV_MODE;
static UINT32 guiPowerOffIndex      = UIDFT_POWEROFF;
static UINT32 guiFreqIndex          = UIDFT_FREQUENCY;
static UINT32 guiBrightIndex        = UIDFT_LCDBRIGHTNESS;
static UINT32 guiDisplayIndex       = UIDFT_DISPLAY;
static UINT32 guiDateFormatIndex    = UIDFT_DATE_FORMAT;
//#NT#2009/10/21#KS Hung -begin
static UINT32 guiTimeFormatIndex    = UIDFT_TIME_FORMAT;
//#NT#2009/10/21#KS Hung -end
static UINT32 guiOpeningIndex       = UIDFT_OPENING;

static UINT32 guiLcdOffIndex          = UIDFT_LCD_OFF;
static UINT32 guiDelayPowerOffIndex      = UIDFT_DELAYPOWEROFF;
static UINT32 guiImageRotateIndex      = UIDFT_IMAGEROTATE;
static UINT32 guiAccPowerOffIndex      = ACC_OFF_POWEROFF_FALSE;

static UINT32 guiFilesysDirNumber;
static UINT32 guiFilesysFileNumber;
static BOOL   g_bTpCalFlag;

//-----------------------------------------------------------------------------
// Beep Sound
//-----------------------------------------------------------------------------
void Set_BeepIndex(UINT32 uiIndex)
{
    guiBeepIndex = uiIndex;
}
UINT32 Get_BeepIndex(void)
{
    return guiBeepIndex;
}

//-----------------------------------------------------------------------------
// Volume Level
//-----------------------------------------------------------------------------
void Set_VolumeIndex(UINT32 uiIndex)
{
    guiVolumeIndex = uiIndex;
}
UINT32 Get_VolumeIndex(void)
{
    return guiVolumeIndex;
}

//-----------------------------------------------------------------------------
// Language Suit
//-----------------------------------------------------------------------------
void Set_LanguageIndex(UINT32 uiIndex)
{
    guiLanguageIndex = uiIndex;
}
UINT32 Get_LanguageIndex(void)
{
    return guiLanguageIndex;
}

//-----------------------------------------------------------------------------
// TV Mode
//-----------------------------------------------------------------------------
void Set_TVModeIndex(UINT32 uiIndex)
{
    guiTVModeIndex = uiIndex;
}
UINT32 Get_TVModeIndex(void)
{
    return guiTVModeIndex;
}

//-----------------------------------------------------------------------------
// Power Off Timer
//-----------------------------------------------------------------------------
void Set_PowerOffIndex(UINT32 uiIndex)
{
    guiPowerOffIndex = uiIndex;
}
UINT32 Get_PowerOffIndex(void)
{
    return guiPowerOffIndex;
}


//-----------------------------------------------------------------------------
// Power Off Timer
//-----------------------------------------------------------------------------
void Set_DelayPowerOffIndex(UINT32 uiIndex)
{
    guiDelayPowerOffIndex = uiIndex;
}
UINT32 Get_DelayPowerOffIndex(void)
{
    return guiDelayPowerOffIndex;
}

//LCD OFF
void Set_ImageRotateIndex(UINT32 uiIndex)
{
    guiImageRotateIndex = uiIndex;
}
UINT32 Get_ImageRotateIndex(void)
{
    return guiImageRotateIndex;
}


//LCD OFF
void Set_LcdOffIndex(UINT32 uiIndex)
{
    guiLcdOffIndex = uiIndex;
}
UINT32 Get_LcdOffIndex(void)
{
    return guiLcdOffIndex;
}

UINT32 Get_LcdOffTime()
{
	switch(guiLcdOffIndex)
	{
		case LCDOFF_1MIN:
		  return 60;
		case LCDOFF_3MIN:
		  return 180;
		case LCDOFF_5MIN:
		  return 300;
		case LCDOFF_OFF:
		  return 0;		  
	}
	return 60;
}


//-----------------------------------------------------------------------------
// Frequency
//-----------------------------------------------------------------------------
void Set_FreqIndex(UINT32 uiIndex)
{
    guiFreqIndex = uiIndex;
}
UINT32 Get_FreqIndex(void)
{
    return guiFreqIndex;
}

//-----------------------------------------------------------------------------
// LCD Brightness Adjust Level
//-----------------------------------------------------------------------------
void Set_BrightIndex(UINT32 uiIndex)
{
    guiBrightIndex = uiIndex;
}
UINT32 Get_BrightIndex(void)
{
    return guiBrightIndex;
}

//-----------------------------------------------------------------------------
// Display Type
//-----------------------------------------------------------------------------
void Set_DisplayIndex(UINT32 uiIndex)
{
    guiDisplayIndex = uiIndex;
}
UINT32 Get_DisplayIndex(void)
{
    return guiDisplayIndex;
}

//-----------------------------------------------------------------------------
// Date Format
//-----------------------------------------------------------------------------
void Set_DateFormatIndex(UINT32 uiIndex)
{
    guiDateFormatIndex = uiIndex;
}
UINT32 Get_DateFormatIndex(void)
{
    return guiDateFormatIndex;
}

//#NT#2009/10/21#KS Hung -begin
//-----------------------------------------------------------------------------
// Time Format
//-----------------------------------------------------------------------------
void Set_TimeFormatIndex(UINT32 uiIndex)
{
    guiTimeFormatIndex = uiIndex;
}
UINT32 Get_TimeFormatIndex(void)
{
    return guiTimeFormatIndex;
}
//#NT#2009/10/21#KS Hung -end

//-----------------------------------------------------------------------------
// Filesys Dir ID
//-----------------------------------------------------------------------------
void Set_FilesysDirId(UINT32 uiId)
{
    guiFilesysDirNumber = uiId;
}
UINT32 Get_FilesysDirId(void)
{
    return guiFilesysDirNumber;
}

//-----------------------------------------------------------------------------
// Filesys File ID
//-----------------------------------------------------------------------------
void Set_FilesysFileId(UINT32 uiId)
{
    guiFilesysFileNumber = uiId;
}
UINT32 Get_FilesysFileId(void)
{
    return guiFilesysFileNumber;
}

//-----------------------------------------------------------------------------
// Opening
//-----------------------------------------------------------------------------
void Set_OpeningIndex(UINT32 uiIndex)
{
    guiOpeningIndex = uiIndex;
}
UINT32 Get_OpeningIndex(void)
{
    return guiOpeningIndex;
}

//-----------------------------------------------------------------------------
// Touch Panel Calibration
//-----------------------------------------------------------------------------
#if (_TouchPanel_ != DISABLE)
void Set_TPCalFlag(BOOL bTpCal)
{
    g_bTpCalFlag = bTpCal;
}
BOOL Get_TPCalFlag(void)
{
    return g_bTpCalFlag;
}
#endif
//ACCOFF POWEROFF
void Set_ACCPowerOffIndex(UINT32 uiIndex)
{
    guiAccPowerOffIndex = uiIndex;
}
UINT32 Get_ACCPowerOffIndex(void)
{
    return guiAccPowerOffIndex;
}
