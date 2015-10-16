#include "Type.h"
#include "SysCfg.h"
#include "UIMenuPhotoSettings.h"
#include "UICfgDefault.h"

static UINT32 guiSizeIndex          = UIDFT_PHOTO_SIZE;
static UINT32 guiQualityIndex       = UIDFT_QUALITY;
static UINT32 guiColorIndex         = UIDFT_COLOR;
static UINT32 guiISOIndex           = UIDFT_ISO;
static UINT32 guiEVIndex            = UIDFT_EV;
static UINT32 guiWBIndex            = UIDFT_WB;
static UINT32 guiMeteringIndex      = UIDFT_METERING;
static UINT32 guiSharpnessIndex     = UIDFT_SHARPNESS;
static UINT32 guiSaturationIndex    = UIDFT_SATURATION;
static UINT32 guiSceneModeIndex     = UIDFT_SCENE;
static UINT32 guiRunningSceneIndex  = UIDFT_SCENE;
static UINT32 guiQuickReviewIndex   = UIDFT_QREVIEW;
static UINT32 guiSelftimerIndex     = UIDFT_SELFTIMER;
static UINT32 guiFlashIndex         = UIDFT_FLASH;
static UINT32 guiMacroIndex         = UIDFT_MACRO;
static UINT32 guiCaptureModeIndex   = UIDFT_CAPTUREMODE;
static UINT32 guiDateImprintIndex   = UIDFT_DATEIMPRINT;
static UINT32 guiFDIndex            = UIDFT_FD;
static UINT32 guiAntiShakingIndex   = UIDFT_ANTISHAKING;
static UINT32 guiContShotIndex      = UIDFT_CONTSHOT;


//-----------------------------------------------------------------------------
// Photo Selftimer Time
//-----------------------------------------------------------------------------
void Set_SelftimerIndex(UINT32 uiIndex)
{
    guiSelftimerIndex = uiIndex;
}
UINT32 Get_SelftimerIndex(void)
{
    return guiSelftimerIndex;
}

//-----------------------------------------------------------------------------
// Flash Type
//-----------------------------------------------------------------------------
void Set_FlashIndex(UINT32 uiIndex)
{
    guiFlashIndex = uiIndex;
}
UINT32 Get_FlashIndex(void)
{
    return guiFlashIndex;
}

//-----------------------------------------------------------------------------
// Macro State
//-----------------------------------------------------------------------------
void Set_MacroIndex(UINT32 uiIndex)
{
    guiMacroIndex = uiIndex;
}
UINT32 Get_MacroIndex(void)
{
    return guiMacroIndex;
}

//-----------------------------------------------------------------------------
// Capture Mode
//-----------------------------------------------------------------------------
void Set_CaptureModeIndex(UINT32 uiIndex)
{
    guiCaptureModeIndex = uiIndex;
}
UINT32 Get_CaptureModeIndex(void)
{
    return guiCaptureModeIndex;
}

//-----------------------------------------------------------------------------
// Image Quality
//-----------------------------------------------------------------------------
void Set_QualityIndex(UINT32 uiIndex)
{
    guiQualityIndex = uiIndex;
}
UINT32 Get_QualityIndex(void)
{
    return guiQualityIndex;
}

//-----------------------------------------------------------------------------
// Image Size
//-----------------------------------------------------------------------------
void Set_SizeIndex(UINT32 uiIndex)
{
    guiSizeIndex = uiIndex;
}
UINT32 Get_SizeIndex(void)
{
    return guiSizeIndex;
}

//-----------------------------------------------------------------------------
// Color Mode
//-----------------------------------------------------------------------------
void Set_ColorIndex(UINT32 uiIndex)
{
    guiColorIndex = uiIndex;
}
UINT32 Get_ColorIndex(void)
{
    return guiColorIndex;
}

//-----------------------------------------------------------------------------
// ISO
//-----------------------------------------------------------------------------
void Set_ISOIndex(UINT32 uiIndex)
{
    guiISOIndex = uiIndex;
}
UINT32 Get_ISOIndex(void)
{
    return guiISOIndex;
}

//-----------------------------------------------------------------------------
// WB Type
//-----------------------------------------------------------------------------
void Set_WBIndex(UINT32 uiIndex)
{
    guiWBIndex = uiIndex;
}
UINT32 Get_WBIndex(void)
{
    return guiWBIndex;
}

//-----------------------------------------------------------------------------
// Metering Type
//-----------------------------------------------------------------------------
void Set_MeteringIndex(UINT32 uiIndex)
{
    guiMeteringIndex = uiIndex;
}
UINT32 Get_MeteringIndex(void)
{
    return guiMeteringIndex;
}

//-----------------------------------------------------------------------------
// Scene Mode
//-----------------------------------------------------------------------------
void Set_SceneModeIndex(UINT32 uiIndex)
{
    guiSceneModeIndex = uiIndex;
}
UINT32 Get_SceneModeIndex(void)
{
    return guiSceneModeIndex;
}

//-----------------------------------------------------------------------------
// Running Scene
//-----------------------------------------------------------------------------
void Set_RunningSceneIndex(UINT32 uiIndex)
{
    guiRunningSceneIndex = uiIndex;
}
UINT32 Get_RunningSceneIndex(void)
{
    return guiRunningSceneIndex;
}

//-----------------------------------------------------------------------------
// EV Value
//-----------------------------------------------------------------------------
void Set_EVIndex(UINT32 uiIndex)
{
    guiEVIndex = uiIndex;
}
UINT32 Get_EVIndex(void)
{
    return guiEVIndex;
}

//-----------------------------------------------------------------------------
// Saturation Level
//-----------------------------------------------------------------------------
void Set_SaturationIndex(UINT32 uiIndex)
{
    guiSaturationIndex = uiIndex;
}
UINT32 Get_SaturationIndex(void)
{
    return guiSaturationIndex;
}

//-----------------------------------------------------------------------------
// Sharpness Level
//-----------------------------------------------------------------------------
void Set_SharpnessIndex(UINT32 uiIndex)
{
    guiSharpnessIndex = uiIndex;
}
UINT32 Get_SharpnessIndex(void)
{
    return guiSharpnessIndex;
}

//-----------------------------------------------------------------------------
// Face/Smile Detection State
//-----------------------------------------------------------------------------
void Set_FDIndex(UINT32 uiIndex)
{
    guiFDIndex = uiIndex;
}
UINT32 Get_FDIndex(void)
{
    return guiFDIndex;
}

//-----------------------------------------------------------------------------
// AntiShaking State
//-----------------------------------------------------------------------------
void Set_AntiShakingIndex(UINT32 uiIndex)
{
    guiAntiShakingIndex = uiIndex;
}
UINT32 Get_AntiShakingIndex(void)
{
    return guiAntiShakingIndex;
}

//-----------------------------------------------------------------------------
// Quick Review Time
//-----------------------------------------------------------------------------
void Set_QuickReviewIndex(UINT32 uiIndex)
{
    guiQuickReviewIndex = uiIndex;
}
UINT32 Get_QuickReviewIndex(void)
{
    return guiQuickReviewIndex;
}

//-----------------------------------------------------------------------------
// Date Imprint State
//-----------------------------------------------------------------------------
void Set_DateImprintIndex(UINT32 uiIndex)
{
    guiDateImprintIndex = uiIndex;
}
UINT32 Get_DateImprintIndex(void)
{
    return guiDateImprintIndex;
}

//-----------------------------------------------------------------------------
// Cont Shot Mode
//-----------------------------------------------------------------------------
void Set_ContShotIndex(UINT32 uiIndex)
{
    guiContShotIndex = uiIndex;
}
UINT32 Get_ContShotIndex(void)
{
    return guiContShotIndex;
}

