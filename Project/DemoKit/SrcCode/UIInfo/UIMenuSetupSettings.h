#ifndef UI_MENUSETUPSETTING_H
#define UI_MENUSETUPSETTING_H

#include "Type.h"



//-----------------------------------------------------------------------------
// Beep Sound
//-----------------------------------------------------------------------------
typedef enum
{
    BEEP_OFF = 0,
    BEEP_ON,
    BEEP_SETTING_MAX
} BEEP_SETTING;

//-----------------------------------------------------------------------------
// Volume Level
//-----------------------------------------------------------------------------
typedef enum
{
    VOLUME_LVL_OFF = 0,
    VOLUME_LVL_1,
    VOLUME_LVL_2,
    VOLUME_LVL_3,
    VOLUME_SETTING_MAX
} VOLUME_SETTING;

//-----------------------------------------------------------------------------
// Language Suit
//-----------------------------------------------------------------------------
typedef enum
{
    ENGLISH = 0,
    GERMAN,
    FRANCE,
    SPANISH,
    ITALIAN,
    PORTUGUESE,
    TCHINESE,
    SCHINESE,
    JAPANESE,
    RUSSIAN,
    LANGUAGE_SETTING_MAX
} LANGUAGE_SETTING;

//-----------------------------------------------------------------------------
// TV Mode
//-----------------------------------------------------------------------------
typedef enum
{
    TV_MODE_NTSC = 0,
    TV_MODE_PAL,
    TV_MODE_SETTING_MAX
} TV_MODE_SETTING;

//-----------------------------------------------------------------------------
// Power Off Timer
//-----------------------------------------------------------------------------
typedef enum
{
    POWEROFF_DISABLED = 0,
    POWEROFF_2MIN,
    POWEROFF_5MIN,
    POWEROFF_10MIN,
    POWEROFF_SETTING_MAX
} POWEROFF_SETTING;

//-----------------------------------------------------------------------------
// USB Mode State
//-----------------------------------------------------------------------------
typedef enum
{
    USB_MODE_MSDC = 0,
    //USB_MODE_PICTBRIDGE,
    USB_MODE_PCC,
    USB_MODE_SETTING_MAX,
    USB_MODE_CHARGE // for future use
} USBMODE_SETTING;

//-----------------------------------------------------------------------------
// Date Format
//-----------------------------------------------------------------------------
typedef enum
{
    DATE_FORMAT_YMD = 0,
    DATE_FORMAT_DMY,
    DATE_FORMAT_MDY,
    DATE_FORMAT_SETTING_MAX
} DATE_FORMAT_SETTING;

//#NT#2009/10/21#KS Hung -begin
//-----------------------------------------------------------------------------
// Time Format
//-----------------------------------------------------------------------------
typedef enum
{
    TIME_FORMAT_12HOURS = 0,
    TIME_FORMAT_24HOURS,
    TIME_FORMAT_NONE,
    TIME_FORMAT_SETTING_MAX
} TIME_FORMAT_SETTING;
//#NT#2009/10/21#KS Hung -end

//-----------------------------------------------------------------------------
// Frequency
//-----------------------------------------------------------------------------
typedef enum
{
    FREQ_50HZ = 0,
    FREQ_60HZ,
    FREQ_SETTING_MAX
} FREQ_SETTING;

//-----------------------------------------------------------------------------
// LCD Brightness Adjust Level
//-----------------------------------------------------------------------------
#if 1
typedef enum
{
    LCDBRT_LVL_01=1,
    LCDBRT_LVL_02,
    LCDBRT_LVL_03,
    LCDBRT_LVL_04,
    LCDBRT_LVL_05,
    LCDBRT_LVL_06,
    LCDBRT_LVL_07,
    LCDBRT_LVL_08,
    LCDBRT_LVL_09,
    LCDBRT_LVL_SETTING_MAX
} LCDBRT_LVL_SETTING;
#else
typedef enum
{
    LCDBRT_STANDARD,
    LCDBRT_BRIGHT,
    LCDBRT_SETTING_MAX
} LCDBRT_SETTING;
#endif

//-----------------------------------------------------------------------------
// Display Type
//-----------------------------------------------------------------------------
typedef enum
{
    DISPLAY_NORMAL = 0,
    DISPLAY_ALL,
    DISPLAY_OFF,
    DISPLAY_SETTING_MAX
} DISPLAY_SETTING;

//-----------------------------------------------------------------------------
// OPENING
//-----------------------------------------------------------------------------
typedef enum
{
    OPENING_OFF = 0,
    OPENING_ON,
    OPENING_SETTING_MAX
} OPENING_SETTING;

//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern void Set_BeepIndex(UINT32 uiIndex);
extern UINT32 Get_BeepIndex(void);

extern void Set_VolumeIndex(UINT32 uiIndex);
extern UINT32 Get_VolumeIndex(void);

extern void Set_LanguageIndex(UINT32 uiIndex);
extern UINT32 Get_LanguageIndex(void);

extern void Set_TVModeIndex(UINT32 uiIndex);
extern UINT32 Get_TVModeIndex(void);

extern void Set_PowerOffIndex(UINT32 uiIndex);
extern UINT32 Get_PowerOffIndex(void);

extern void Set_FreqIndex(UINT32 uiIndex);
extern UINT32 Get_FreqIndex(void);

extern void Set_BrightIndex(UINT32 uiIndex);
extern UINT32 Get_BrightIndex(void);

extern void Set_DisplayIndex(UINT32 uiIndex);
extern UINT32 Get_DisplayIndex(void);

extern void Set_DateFormatIndex(UINT32 uiIndex);
extern UINT32 Get_DateFormatIndex(void);

extern void Set_FilesysDirId(UINT32 uiId);
extern UINT32 Get_FilesysDirId(void);

extern void Set_FilesysFileId(UINT32 uiId);
extern UINT32 Get_FilesysFileId(void);

extern void Set_OpeningIndex(UINT32 uiIndex);
extern UINT32 Get_OpeningIndex(void);

#if (_TouchPanel_ != DISABLE)
extern void Set_TPCalFlag(BOOL bTpCal);
extern BOOL Get_TPCalFlag(void);
#endif

extern void Set_ACCPowerOffIndex(UINT32 uiIndex);
extern UINT32 Get_ACCPowerOffIndex(void);
#endif
