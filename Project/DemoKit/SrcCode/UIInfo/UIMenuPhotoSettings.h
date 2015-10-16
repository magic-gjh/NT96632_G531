#ifndef UI_MENUPHOTOSETTING_H
#define UI_MENUPHOTOSETTING_H

#include "Type.h"



//-----------------------------------------------------------------------------
// Photo Selftimer Time
//-----------------------------------------------------------------------------
typedef enum
{
    SELFTIMER_OFF = 0,
    SELFTIMER_ON_2S,
    SELFTIMER_ON_5S,
    SELFTIMER_ON_10S,
    SELFTIMER_SETTING_MAX
} SELFTIMER_SETTIMG;

//-----------------------------------------------------------------------------
// Flash Type
//-----------------------------------------------------------------------------
typedef enum
{
    FLASH_AUTO = 0,
    FLASH_ON,
    FLASH_OFF,
    FLASH_SETTING_MAX
} FLASH_SETTING;

//-----------------------------------------------------------------------------
// Macro State
//-----------------------------------------------------------------------------
typedef enum
{
    MACRO_OFF = 0,
    MACRO_ON,
    MACRO_SETTING_MAX
} MACRO_SETTING;

//-----------------------------------------------------------------------------
// ACC 12V Status
//-----------------------------------------------------------------------------
typedef enum
{
    ACC12V_PLUGIN = 0,
    ACC12V_UNPLUG,
    ACC12V_SETTING_MAX
} ACC12V_SETTING;
//-----------------------------------------------------------------------------
// Capture Mode
//-----------------------------------------------------------------------------
typedef enum
{
    CAPTURE_MODE_SINGLE = 0,
    CAPTURE_MODE_SELFTIMER_2S,
    CAPTURE_MODE_SELFTIMER_5S,
    CAPTURE_MODE_SELFTIMER_10S,
    CAPTURE_MODE_CONTSHOT3,
    CAPTURE_MODE_SETTING_MAX
} CAPTURE_MODE_SETTING;

//-----------------------------------------------------------------------------
// Scene Mode
//-----------------------------------------------------------------------------
typedef enum
{
    SCENE_AUTO = 0,
    SCENE_SMART,
    SCENE_LANDSCAPE,
    SCENE_BACKLIGHT,
    SCENE_NIGHTSCENE,
    SCENE_PORTRAIT,
    SCENE_SPORTS,
    SCENE_SETTING_MAX
} SCENE_SETTING;

//-----------------------------------------------------------------------------
// Image Quality
//-----------------------------------------------------------------------------
typedef enum
{
    QUALITY_BEST = 0,
    QUALITY_FINE,
    QUALITY_NORMAL,
    QUALITY_SETTING_MAX
} QUALITY_SETTING;

//-----------------------------------------------------------------------------
// Image Size
//-----------------------------------------------------------------------------
typedef enum
{
    SIZE_12M = 0,
    SIZE_8M,
    //SIZE_7M_169,
    SIZE_5M,
    SIZE_4M,
    SIZE_3M,
    //SIZE_2M_169,
    SIZE_1M,
    SIZE_SETTING_MAX
} SIZE_SETTING;

//-----------------------------------------------------------------------------
// AF Window
//-----------------------------------------------------------------------------
typedef enum
{
    AFWINDOW_SINGLE = 0,
    AFWINDOW_MULTI,
    AFWINDOW_SETTING_MAX
} AFWINDOW_SETTING;

//-----------------------------------------------------------------------------
// AF Beam State
//-----------------------------------------------------------------------------
typedef enum
{
    AFBEAM_OFF = 0,
    AFBEAM_ON,
    AFBEAM_SETTING_MAX
} AFBEAM_SETTING;

//-----------------------------------------------------------------------------
// Color Mode
//-----------------------------------------------------------------------------
typedef enum
{
    COLOR_STANDARD = 0,
    COLOR_MONO,
    COLOR_SEPIA,
    COLOR_SETTING_MAX
} COLOR_SETTING;

//-----------------------------------------------------------------------------
// ISO
//-----------------------------------------------------------------------------
typedef enum
{
    ISO_AUTO = 0,
    ISO_100,
    ISO_200,
    ISO_400,
    ISO_SETTING_MAX
} ISO_SETTING;

//-----------------------------------------------------------------------------
// WB Type
//-----------------------------------------------------------------------------
typedef enum
{
    WB_AUTO = 0,
    WB_DAYLIGHT,
    WB_CLOUDY,
    WB_TUNGSTEN,
    WB_FLUORESCENT1,
    WB_SETTING_MAX
} WB_SETTING;

//-----------------------------------------------------------------------------
// Metering Type
//-----------------------------------------------------------------------------
typedef enum
{
    METERING_CENTER_WEIGHTED = 0,
    METERING_AVERAGE,
    METERING_SPOT,
    METERING_SETTING_MAX
} METERING_SETTING;

//-----------------------------------------------------------------------------
// EV Value
//-----------------------------------------------------------------------------
typedef enum
{
    EV_P20 = 0,
    EV_P16,
    EV_P13,
    EV_P10,
    EV_P06,
    EV_P03,
    EV_00,
    EV_N03,
    EV_N06,
    EV_N10,
    EV_N13,
    EV_N16,
    EV_N20,
    EV_SETTING_MAX
} EV_SETTING;

//-----------------------------------------------------------------------------
// Saturation Level
//-----------------------------------------------------------------------------
typedef enum
{
    SATURATION_HIGH = 0,
    SATURATION_NORNAL,
    SATURATION_LOW,
    SATURATION_SETTING_MAX
} SATURATION_SETTING;

//-----------------------------------------------------------------------------
// Sharpness Level
//-----------------------------------------------------------------------------
typedef enum
{
    SHARPNESS_HARD = 0,
    SHARPNESS_NORMAL,
    SHARPNESS_SOFT,
    SHARPNESS_SETTING_MAX
} SHARPNESS_SETTING;

//-----------------------------------------------------------------------------
// Contrast Level
//-----------------------------------------------------------------------------
typedef enum
{
    CONTRAST_HARD = 0,
    CONTRAST_NORMAL,
    CONTRAST_SOFT,
    CONTRAST_SETTING_MAX
} CONTRAST_SETTING;

//-----------------------------------------------------------------------------
// Face/Smile Detection State
//-----------------------------------------------------------------------------
typedef enum
{
    FD_OFF = 0,
    FD_ON,
    FD_SMILE,
    FD_SETTING_MAX
} FD_SETTIMG;

//-----------------------------------------------------------------------------
// AntiShaking State
//-----------------------------------------------------------------------------
typedef enum
{
    ANTISHAKING_OFF = 0,
    ANTISHAKING_ON,
    ANTISHAKING_SETTING_MAX
} ANTISHAKING_SETTING;

//-----------------------------------------------------------------------------
// Quick Review Time
//-----------------------------------------------------------------------------
typedef enum
{
    QREVIEW_0SEC = 0,
    QREVIEW_2SEC,
    QREVIEW_5SEC,
    QREVIEW_SETTING_MAX
} QREVIEW_SETTING;

//-----------------------------------------------------------------------------
// Histogram State
//-----------------------------------------------------------------------------
typedef enum
{
    HISTOGRAM_OFF = 0,
    HISTOGRAM_ON,
    HISTOGRAM_SETTING_MAX
} HISTOGRAM_SETTING;

//-----------------------------------------------------------------------------
// Date Imprint State
//-----------------------------------------------------------------------------
typedef enum
{
    DATEIMPRINT_OFF = 0,
    DATEIMPRINT_DATE,
    DATEIMPRINT_DATE_TIME,
    DATEIMPRINT_SETTING_MAX
} DATEIMPRINT_SETTING;

//-----------------------------------------------------------------------------
// Cont Shot Mode
//-----------------------------------------------------------------------------
typedef enum
{
    CONT_SHOT_OFF = 0,
    CONT_SHOT_3,
    CONT_SHOT_SETTING_MAX
} CONT_SHOT_SETTING;


//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern void     Set_SelftimerIndex(UINT32 uiIndex);
extern UINT32   Get_SelftimerIndex(void);

extern void     Set_FlashIndex(UINT32 uiIndex);
extern UINT32   Get_FlashIndex(void);

extern void     Set_MacroIndex(UINT32 uiIndex);
extern UINT32   Get_MacroIndex(void);

extern void     Set_CaptureModeIndex(UINT32 uiIndex);
extern UINT32   Get_CaptureModeIndex(void);

extern void     Set_SizeIndex(UINT32 uiIndex);
extern UINT32   Get_SizeIndex(void);

extern void     Set_QualityIndex(UINT32 uiIndex);
extern UINT32   Get_QualityIndex(void);

extern void     Set_ColorIndex(UINT32 uiIndex);
extern UINT32   Get_ColorIndex(void);

extern void     Set_ISOIndex(UINT32 uiIndex);
extern UINT32   Get_ISOIndex(void);

extern void     Set_WBIndex(UINT32 uiIndex);
extern UINT32   Get_WBIndex(void);

extern void     Set_MeteringIndex(UINT32 uiIndex);
extern UINT32   Get_MeteringIndex(void);

extern void     Set_SceneModeIndex(UINT32 uiIndex);
extern UINT32   Get_SceneModeIndex(void);

extern void     Set_RunningSceneIndex(UINT32 uiIndex);
extern UINT32   Get_RunningSceneIndex(void);

extern void     Set_EVIndex(UINT32 uiIndex);
extern UINT32   Get_EVIndex(void);

extern void     Set_SaturationIndex(UINT32 uiIndex);
extern UINT32   Get_SaturationIndex(void);

extern void     Set_SharpnessIndex(UINT32 uiIndex);
extern UINT32   Get_SharpnessIndex(void);

extern void     Set_FDIndex(UINT32 uiIndex);
extern UINT32   Get_FDIndex(void);

extern void     Set_AntiShakingIndex(UINT32 uiIndex);
extern UINT32   Get_AntiShakingIndex(void);

extern void     Set_QuickReviewIndex(UINT32 uiIndex);
extern UINT32   Get_QuickReviewIndex(void);

extern void     Set_DateImprintIndex(UINT32 uiIndex);
extern UINT32   Get_DateImprintIndex(void);

extern void     Set_ContShotIndex(UINT32 uiIndex);
extern UINT32   Get_ContShotIndex(void);

#endif