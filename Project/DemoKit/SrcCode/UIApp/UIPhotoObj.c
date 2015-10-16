#include <stdio.h>
#include <string.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIPhotoObj.h"
#include "ImageAlgInfor.h"
#include "SieCapTsk.h"
#include "UIMenuPhotoSettings.h"
#include "UIMenuSetupSettings.h"
#include "UISystemStatusSettings.h"
#include "KeyScanTsk.h"
#include "PhotoTsk.h"
#include "GenDateImprintData.h"
#include "Debug.h"



static UINT32 g_uiCurrentZoomIF     = ZOOM_IF_OPTICAL;
static BOOL g_bDZoomResetting       = FALSE;
static CHAR g_cDZoomRatioStr[MAX_DZOOM_STR_LEN] = {0};
static CHAR g_cSelftimerCntStr[8]   = {0};
static AlgMsgInfo g_AlgMsgInfo      = 0;

// Must in the same order of those in enum SIZE_SETTING
static const CHAR *g_pcSizeCfgTable[SIZE_SETTING_MAX] =
{
    "12M",      // 10M
    "8M",       // 8M
    "5M",     // 7MHD
    "4M",       // 5M
    "3M",       // 3M
    "1.3M",     // 2MHD
};

//#NT#2010/11/29#philex Lin - begin
// selected Scene mode will decide Flash/EV/WB/Sharpness parameter's values,
// so disable EV/WB/Sharpness setting in DVPlus project
#if 0
// Must refer to gSceneModeInfo
static SCENE_MODE_INFO g_SceneInfoTbl[SCENE_SETTING_MAX] =
{   //Scene             //Flash         //EV        //Sharpness
    { SCENE_AUTO,       FLASH_AUTO,     EV_00,      SHARPNESS_NORMAL    },
    { SCENE_SMART,      FLASH_AUTO,     EV_00,      SHARPNESS_NORMAL    },
    { SCENE_LANDSCAPE,  FLASH_OFF,      EV_00,      SHARPNESS_HARD      },
    { SCENE_BACKLIGHT,  FLASH_ON,       EV_00,      SHARPNESS_NORMAL    },
    { SCENE_NIGHTSCENE, FLASH_ON,       EV_00,      SHARPNESS_NORMAL    },
    { SCENE_PORTRAIT,   FLASH_AUTO,     EV_P03,     SHARPNESS_SOFT      },
    { SCENE_SPORTS,     FLASH_OFF,      EV_00,      SHARPNESS_NORMAL    }
};
#endif
//#NT#2010/11/29#philex Lin - end

INT32 OnExe_Flash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 OnExe_EV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 OnExe_Sharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//-----------------------------------------------------------------------------
// Photo Flow Object APIs
//-----------------------------------------------------------------------------
AlgMsgInfo *Get_AlgMsgInfo(void)
{
    return (void *)&g_AlgMsgInfo;
}
void Set_AlgMsgInfo(AlgMsgInfo *pInfo)
{
    memcpy((void *)&g_AlgMsgInfo, pInfo, sizeof(AlgMsgInfo));
}

UINT32 Get_FlashValue(UINT32 uiIndex)
{
    _IMGFlashMode_sel val;

    switch(uiIndex)
    {
        case FLASH_AUTO:
            val = _IMGFlashMode_Auto;
            break;
        case FLASH_ON:
            val = _IMGFlashMode_ForceON;
            break;
        case FLASH_OFF:
            val = _IMGFlashMode_ForceOFF;
            break;
        default:
            val = _IMGFlashMode_Auto;
            break;
    }
    return val;
}

UINT32 Get_SizeValue(UINT32 uiIndex)
{
    _IMGSize_sel val;

    switch(uiIndex)
    {
        case SIZE_12M:
            val = _IMGSize_12M;
            break;
        case SIZE_8M:
            val = _IMGSize_8M;
            break;
        //case SIZE_7M_169:
            //val = _IMGSize_7M_16x9;
           // break;
        case SIZE_5M:
            val = _IMGSize_5M;
            break;
        case SIZE_4M:
            val = _IMGSize_4M;
            break;			
        case SIZE_3M:
            val = _IMGSize_3M;
            break;
        case SIZE_1M:
            val = _IMGSize_1M;
            break;
        default:
            val = _IMGSize_5M;
            break;
    }
    return val;
}

const CHAR *Get_SizeString(UINT32 uiIndex)
{
    if(uiIndex < SIZE_SETTING_MAX)
        return g_pcSizeCfgTable[uiIndex];
    else
        return NULL;
}

UINT32 Get_QualityValue(UINT32 uiIndex)
{
    _IMGQuality_sel val;

    switch(uiIndex)
    {
        case QUALITY_BEST:
            val = _IMGQuality_Best;
            break;
        case QUALITY_FINE:
            val = _IMGQuality_Fine;
            break;
        case QUALITY_NORMAL:
            val = _IMGQuality_Normal;
            break;
        default:
            val = _IMGQuality_Best;
            break;
    }
    return val;
}

UINT32 Get_ColorValue(UINT32 uiIndex)
{
    _IMGEffect_sel val;

    switch(uiIndex)
    {
        case COLOR_STANDARD:
            val = _IMGEffect_Off;
            break;
        case COLOR_MONO:
            val = _IMGEffect_BW;
            break;
        case COLOR_SEPIA:
            val = _IMGEffect_Sepia;
            break;
        default:
            val = _IMGEffect_Off;
            break;
    }
    return val;
}

UINT32 Get_SceneModeValue(UINT32 uiIndex)
{
    _IMGSceneMode_sel val;

    switch(uiIndex)
    {
        case SCENE_AUTO:
            val = _SceneMode_Auto;
            break;
        case SCENE_SMART:
            val = _SceneMode_Smart;
            break;
        case SCENE_LANDSCAPE:
            val = _SceneMode_Landscape;
            break;
        case SCENE_BACKLIGHT:
            val = _SceneMode_BackLight;
            break;
        case SCENE_NIGHTSCENE:
            val = _SceneMode_NightScene;
            break;
        case SCENE_PORTRAIT:
            val = _SceneMode_Potrait;
            break;
        case SCENE_SPORTS:
            val = _SceneMode_Sport;
            break;
        default:
            val = _SceneMode_Auto;
            break;
    }
    return val;
}

UINT32 Get_SceneModeFromValue(_IMGSceneMode_sel uiVal)
{
    UINT32 uiIndex;

    switch(uiVal)
    {
        case _SceneMode_Auto:
            uiIndex = SCENE_AUTO;
            break;
        case _SceneMode_Landscape:
            uiIndex = SCENE_LANDSCAPE;
            break;
        case _SceneMode_BackLight:
            uiIndex = SCENE_BACKLIGHT;
            break;
        case _SceneMode_NightScene:
            uiIndex = SCENE_NIGHTSCENE;
            break;
        case _SceneMode_Potrait:
            uiIndex = SCENE_PORTRAIT;
            break;
        case _SceneMode_Sport:
            uiIndex = SCENE_SPORTS;
            break;
        default:
            uiIndex = SCENE_AUTO;
            break;
    }
    return uiIndex;
}

//#NT#2010/11/29#philex Lin - begin
// selected Scene mode will decide Flash/EV/WB/Sharpness parameter's values,
// so disable EV/WB/Sharpness setting in DVPlus project
#if 0
UINT32 Get_SceneRtnFlash(UINT32 uiIndex)
{
    UINT32 uiFlashIndex = FLASH_AUTO;
    UINT32 i;

    for(i=0; i < SCENE_SETTING_MAX; i++)
    {
        if(g_SceneInfoTbl[i].Scene == uiIndex)
        {
            uiFlashIndex = g_SceneInfoTbl[i].Flash;
            break;
        }
    }
    return uiFlashIndex;
}

UINT32 Get_SceneRtnEV(UINT32 uiIndex)
{
    UINT32 uiEVIndex = EV_00;
    UINT32 i;

    for(i=0; i < SCENE_SETTING_MAX; i++)
    {
        if(g_SceneInfoTbl[i].Scene == uiIndex)
        {
            uiEVIndex = g_SceneInfoTbl[i].EV;
            break;
        }
    }
    return uiEVIndex;
}

UINT32 Get_SceneRtnSharpness(UINT32 uiIndex)
{
    UINT32 uiSharpnessIndex = SHARPNESS_NORMAL;
    UINT32 i;

    for(i=0; i < SCENE_SETTING_MAX; i++)
    {
        if(g_SceneInfoTbl[i].Scene == uiIndex)
        {
            uiSharpnessIndex = g_SceneInfoTbl[i].Sharpness;
            break;
        }
    }
    return uiSharpnessIndex;
}
#endif

UINT32 Get_EVValue(UINT32 uiIndex)
{
    _IMGEVValue_sel val;

    switch(uiIndex)
    {
        case EV_N20:
            val = _IMGEVValue_N6;
            break;
        case EV_N16:
            val = _IMGEVValue_N5;
            break;
        case EV_N13:
            val = _IMGEVValue_N4;
            break;
        case EV_N10:
            val = _IMGEVValue_N3;
            break;
        case EV_N06:
            val = _IMGEVValue_N2;
            break;
        case EV_N03:
            val = _IMGEVValue_N1;
            break;
        case EV_00:
            val = _IMGEVValue_00;
            break;
        case EV_P03:
            val = _IMGEVValue_P1;
            break;
        case EV_P06:
            val = _IMGEVValue_P2;
            break;
        case EV_P10:
            val = _IMGEVValue_P3;
            break;
        case EV_P13:
            val = _IMGEVValue_P4;
            break;
        case EV_P16:
            val = _IMGEVValue_P5;
            break;
        case EV_P20:
            val = _IMGEVValue_P6;
            break;
        default:
            val = _IMGEVValue_00;
            break;
    }
    return val;
}

UINT32 Get_MeteringValue(UINT32 uiIndex)
{
    _IMGAEMode_sel val;

    switch(uiIndex)
    {
        case METERING_CENTER_WEIGHTED:
            val = _IMGAEMode_CenterWeighted;
            break;
        case METERING_AVERAGE:
            val = _IMGAEMode_Matrix;
            break;
        case METERING_SPOT:
            val = _IMGAEMode_Spot;
            break;
        default:
            val = _IMGAEMode_CenterWeighted;
            break;
    }
    return val;
}

UINT32 Get_WBValue(UINT32 uiIndex)
{
    _IMGWBMode_sel val;

    switch(uiIndex)
    {
        case WB_AUTO:
            val = _IMGWBMode_Auto;
            break;
        case WB_DAYLIGHT:
            val = _IMGWBMode_Daylight;
            break;
        case WB_CLOUDY:
            val = _IMGWBMode_Cloudy;
            break;
        case WB_TUNGSTEN:
            val = _IMGWBMode_Tungsten;
            break;
        case WB_FLUORESCENT1:
            val = _IMGWBMode_Fluorescent1;
            break;
        default:
            val = _IMGWBMode_Auto;
            break;
    }
    return val;
}

UINT32 Get_ISOValue(UINT32 uiIndex)
{
    _IMGISO_sel val;

    switch(uiIndex)
    {
        case ISO_AUTO:
            val = _IMGISO_Auto;
            break;
        case ISO_100:
            val = _IMGISO_100;
            break;
        case ISO_200:
            val = _IMGISO_200;
            break;
        case ISO_400:
            val = _IMGISO_400;
            break;
        default:
            val = _IMGISO_Auto;
            break;
    }
    return val;
}

UINT32 Get_SaturationValue(UINT32 uiIndex)
{
    _IMGSaturation_sel val;

    switch(uiIndex)
    {
        case SATURATION_HIGH:
            val = _IMGSaturation_P1;
            break;
        case SATURATION_NORNAL:
            val = _IMGSaturation_Normal;
            break;
        case SATURATION_LOW:
            val = _IMGSaturation_N1;
            break;
        default:
            val = _IMGSaturation_Normal;
            break;
    }
    return val;
}

UINT32 Get_SharpnessValue(UINT32 uiIndex)
{
    _IMGSharpness_sel val;

    switch(uiIndex)
    {
        case SHARPNESS_HARD:
            val = _IMGSharp_P1;
            break;
        case SHARPNESS_NORMAL:
            val = _IMGSharp_Normal;
            break;
        case SHARPNESS_SOFT:
            val = _IMGSharp_N1;
            break;
        default:
            val = _IMGSaturation_Normal;
            break;
    }
    return val;
}

UINT32 Get_AntiShakingValue(UINT32 uiIndex)
{
    _IMGStillAntiShaking val;

    switch(uiIndex)
    {
        case ANTISHAKING_OFF:
            val = _Level0;
            break;
        case ANTISHAKING_ON:
            val = _Level2;
            break;
        default:
            val = _Level0;
            break;
    }
    return val;
}

UINT32 Get_QuickReviewValue(UINT32 uiIndex)
{
    UINT32 val;

    switch(uiIndex)
    {
        case QREVIEW_0SEC:
            val = 0;
            break;
        case QREVIEW_2SEC:
            val = 2;
            break;
        case QREVIEW_5SEC:
            val = 5;
            break;
        default:
            val = 0;
            break;
    }
    return val;
}

CHAR *Get_SelftimerCntString(UINT32 uiCount)
{
    sprintf(g_cSelftimerCntStr, "%ld", uiCount);
    return g_cSelftimerCntStr;
}

void Set_CurrentZoomIF(UINT32 uiIndex)
{
    g_uiCurrentZoomIF = uiIndex;
}
UINT32 Get_CurrentZoomIF(void)
{
    return g_uiCurrentZoomIF;
}

void Set_DZoomResetting(BOOL bReset)
{
    g_bDZoomResetting = bReset;
}
BOOL Get_DZoomResetting(void)
{
    return g_bDZoomResetting;
}

CHAR *Get_DZoomRatioString(AlgMsgInfo *pInfo)
{
    UINT32 m, n;
    m = (pInfo->DzoomRealRatio)/10;
    n = (pInfo->DzoomRealRatio)%10;
    sprintf(g_cDZoomRatioStr, "x%ld.%ld", m, n);
    return g_cDZoomRatioStr;
}

void UI_ZoomControl(UINT32 uiZoomCtrl, BOOL bDZoomEnable)
{
    static BOOL bIsDigital = FALSE;

    switch(uiZoomCtrl)
    {
        case UI_ZOOM_CTRL_STOP:
            if(bIsDigital == FALSE)
            {
                debug_err(("UI_ZoomControl: Photo_OpticalZoom_Stop\r\n"));
                if(Photo_OpticalZoom_Stop() != 0)
                {
                    debug_err(("UI_ZoomControl: Photo lens error!!!\r\n"));
                    //Rsvd
                }
            }
            else
            {
                debug_err(("UI_ZoomControl: Photo_DZoom_Stop\r\n"));
                Photo_DZoom_Stop();
            }
            break;

        case UI_ZOOM_CTRL_IN:
            if(Photo_OpticalZoom_GetSection() >= Photo_OpticalZoom_GetMaxSection())
            {
                if(bDZoomEnable)
                {
                    debug_err(("UI_ZoomControl: Photo_DZoom_In\r\n"));
                    Photo_DZoom_In();
                    bIsDigital = TRUE;
                    Set_CurrentZoomIF(ZOOM_IF_DIGITAL);
                }
            }
            else
            {
                debug_err(("UI_ZoomControl: Photo_OpticalZoom_In\r\n"));
                Photo_OpticalZoom_In();
                bIsDigital = FALSE;
                Set_CurrentZoomIF(ZOOM_IF_OPTICAL);
            }
            break;

        case UI_ZOOM_CTRL_OUT:
            if(Photo_GetDZoomIdx() <= UI_DZOOM_IDX_MIN)
            {
                debug_err(("UI_ZoomControl: Photo_OpticalZoom_Out\r\n"));
                Photo_OpticalZoom_Out();
                bIsDigital = FALSE;
                Set_CurrentZoomIF(ZOOM_IF_OPTICAL);
            }
            else
            {
                debug_err(("UI_ZoomControl: Photo_DZoom_Out\r\n"));
                Photo_DZoom_Out();
                bIsDigital = TRUE;
                Set_CurrentZoomIF(ZOOM_IF_DIGITAL);
            }
            break;

        case UI_ZOOM_CTRL_RESET_DZOOM:
            if((bIsDigital == TRUE) && (Photo_GetDZoomIdx() > UI_DZOOM_IDX_MIN))
            {
                debug_err(("UI_ZoomControl: Photo_DZoom_GotoStep(UI_DZOOM_IDX_MIN)\r\n"));
                Set_DZoomResetting(TRUE);
                Photo_DZoom_GotoStep(UI_DZOOM_IDX_MIN);
            }
            bIsDigital = FALSE;
            Set_CurrentZoomIF(ZOOM_IF_OPTICAL);
            break;

        default:
            debug_err(("UI_ZoomControl: Unknown zoom control 0x%x\r\n", uiZoomCtrl));
            break;
    }
}

//-----------------------------------------------------------------------------
// Photo Flow Event Handler
//-----------------------------------------------------------------------------
INT32 OnCapture(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiDateFormat = 0;

    debug_err(("UIPhotoObj: OnCapture\r\n"));

    /* Get date format */
    switch(Get_DateFormatIndex())
    {
        case DATE_FORMAT_DMY:
            uiDateFormat = STAMP_DD_MM_YY;
            break;

        case DATE_FORMAT_MDY:
            uiDateFormat = STAMP_MM_DD_YY;
            break;

        case DATE_FORMAT_YMD:
        default:
            uiDateFormat = STAMP_YY_MM_DD;
            break;
    }

    /* Generate date imprint */
    switch(Get_DateImprintIndex())
    {
        case DATEIMPRINT_DATE:
            DateImprint_Setup(
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE |
            STAMP_BOTTOM_RIGHT |
            uiDateFormat);
            break;

        case DATEIMPRINT_DATE_TIME:
            DateImprint_Setup(
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
            STAMP_BOTTOM_RIGHT |
            uiDateFormat);
            break;

        case DATEIMPRINT_OFF:
        default:
            DateImprint_Setup(STAMP_OFF);
            break;
    }

    /* Set other necessary IQ parameters */
    if(Get_ContShotIndex() == CONT_SHOT_OFF)
    {
        /* Single capture */
        ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
        ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
        ImgCapture_SetParameter(_SiePath, _PostGamma);
        ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
        ImgCapture_SetParameter(_BurstNum, _BurstNum_1);
    }
    else
    {
        /* Continue capture 3 */
        ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
        ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
        ImgCapture_SetParameter(_SiePath, _PostGamma);
        ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Burst);
        ImgCapture_SetParameter(_BurstNum, _BurstNum_3);
    }

    /* update DCF Folder/File name */
    FlowDCF_UpdateName(TRUE);

    /* Start capture */
    ImgCapture_StartCapture();

    return NVTEVT_CONSUME;
}

INT32 OnCaptureStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    debug_err(("UIPhotoObj: OnCaptureStop\r\n"));

    /* Now only be used in cont shot cases! */
    ImgCapture_StopBurstCapture();

    return NVTEVT_CONSUME;
}

INT32 OnZoomIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Assume digital zoom is always enabled */
    UI_ZoomControl(UI_ZOOM_CTRL_IN, TRUE);
    return NVTEVT_CONSUME;
}

INT32 OnZoomOut(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Assume digital zoom is always enabled */
    UI_ZoomControl(UI_ZOOM_CTRL_OUT, TRUE);
    return NVTEVT_CONSUME;
}

INT32 OnZoomStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    /* Assume digital zoom is always enabled */
    UI_ZoomControl(UI_ZOOM_CTRL_STOP, TRUE);
    return NVTEVT_CONSUME;
}

INT32 OnSmartSceneCheck(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    _IMGSceneMode_sel rtnScene = _SceneMode_Auto;
    UINT32 uiSceneIndex = 0;
    UINT32 uiRtnFlash, uiRtnEV, uiRtnSharpness;

    if(paramNum > 0)
        rtnScene = paramArray[0];

    uiSceneIndex = Get_SceneModeFromValue(rtnScene);
    debug_ind(("OnSmartSceneCheck: rtnScene %d, uiSceneIndex %d\r\n", rtnScene, uiSceneIndex));

    Set_RunningSceneIndex(uiSceneIndex);

//#NT#2010/11/29#philex Lin - begin
// selected Scene mode will decide Flash/EV/WB/Sharpness parameter's values,
// so disable EV/WB/Sharpness setting in DVPlus project
    /* Scene info related configs (flash/ev/sharpness) */
#if 0
    #if (_FLASHLIGHT_ != _FLASHLIGHT_NONE_)
    uiRtnFlash = Get_SceneRtnFlash(uiSceneIndex);
    OnExe_Flash(pCtrl, 1, &uiRtnFlash);
    #endif

    uiRtnEV = Get_SceneRtnEV(uiSceneIndex);
    OnExe_EV(pCtrl, 1, &uiRtnEV);

    uiRtnSharpness = Get_SceneRtnSharpness(uiSceneIndex);
    OnExe_Sharpness(pCtrl, 1, &uiRtnSharpness);
#endif
//#NT#2010/11/29#philex Lin - end

    return NVTEVT_CONSUME;
}

INT32 OnExe_Selftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum > 0)
        uiSelect = paramArray[0];

    debug_ind(("OnExe_Selftimer: uiSelect %d\r\n", uiSelect));

    Set_SelftimerIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_Flash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum > 0)
        uiSelect = paramArray[0];

    debug_ind(("OnExe_Flash: uiSelect %d\r\n", uiSelect));

    Set_FlashIndex(uiSelect);

    if(uiSelect != FLASH_OFF)
        KeyScan_EnableFlashCharging(TRUE);
    else
        KeyScan_EnableFlashCharging(FALSE);

    ImgCapture_SetParameter(_FlashMode, (UINT8)Get_FlashValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_CaptureMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum > 0)
        uiSelect = paramArray[0];

    debug_ind(("OnExe_CaptureMode: uiSelect %d\r\n", uiSelect));

    Set_CaptureModeIndex(uiSelect);

    /* Capture Mode configs Selftimer & ContShot settings */
    switch(uiSelect)
    {
        case CAPTURE_MODE_SELFTIMER_2S:
            Set_SelftimerIndex(SELFTIMER_ON_2S);
            Set_ContShotIndex(CONT_SHOT_OFF);
            break;

        case CAPTURE_MODE_SELFTIMER_5S:
            Set_SelftimerIndex(SELFTIMER_ON_5S);
            Set_ContShotIndex(CONT_SHOT_OFF);
            break;

        case CAPTURE_MODE_SELFTIMER_10S:
            Set_SelftimerIndex(SELFTIMER_ON_10S);
            Set_ContShotIndex(CONT_SHOT_OFF);
            break;

        case CAPTURE_MODE_CONTSHOT3:
            Set_SelftimerIndex(SELFTIMER_OFF);
            Set_ContShotIndex(CONT_SHOT_3);
            break;

        case CAPTURE_MODE_SINGLE:
        default:
            Set_SelftimerIndex(SELFTIMER_OFF);
            Set_ContShotIndex(CONT_SHOT_OFF);
            break;
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_Quality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_Quality: uiSelect %d\r\n", uiSelect));

    Set_QualityIndex(uiSelect);
    ImgCapture_SetParameter(_Quality,(UINT8)Get_QualityValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_Size(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_Size: uiSelect %d\r\n", uiSelect));

    Set_SizeIndex(uiSelect);
    ImgCapture_SetParameter(_ImgSize, (UINT8)Get_SizeValue(uiSelect));
    switch (uiSelect)
    {
       case SIZE_12M:
       case SIZE_8M:
       case SIZE_5M:
       case SIZE_4M:	   	
       case SIZE_3M:
	case SIZE_1M:
         Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_PREVIEW);
         break;
       //case SIZE_7M_169:
       //case SIZE_2M_169:
        //Spark
         //Photo_SetImageResolution(_Resolution_3648x2048,IPL_MODE_PREVIEW);
         //Photo_SetImageResolution(_Resolution_640x360,IPL_MODE_PREVIEW);

         //break;
    }
    return NVTEVT_CONSUME;
}

INT32 OnExe_SceneMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiRtnFlash, uiRtnEV, uiRtnSharpness;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_SceneMode: uiSelect %d\r\n", uiSelect));

    /* Scene mode */
    Set_SceneModeIndex(uiSelect);
    Set_RunningSceneIndex(uiSelect);
    ImgCapture_SetParameter(_SceneMode, (UINT8)Get_SceneModeValue(uiSelect));

//#NT#2010/11/29#philex Lin - begin
// selected Scene mode will decide Flash/EV/WB/Sharpness parameter's values,
// so disable EV/WB/Sharpness setting in DVPlus project
    /* Scene info related configs (flash/ev/sharpness) */
#if 0
    if(uiSelect != SCENE_AUTO)
    {
        #if (_FLASHLIGHT_ != _FLASHLIGHT_NONE_)
        uiRtnFlash = Get_SceneRtnFlash(uiSelect);
        OnExe_Flash(pCtrl, 1, &uiRtnFlash);
        #endif

        uiRtnEV = Get_SceneRtnEV(uiSelect);
        OnExe_EV(pCtrl, 1, &uiRtnEV);

        uiRtnSharpness = Get_SceneRtnSharpness(uiSelect);
        OnExe_Sharpness(pCtrl, 1, &uiRtnSharpness);
    }
#endif
//#NT#2010/11/29#philex Lin - end

    return NVTEVT_CONSUME;
}

INT32 OnExe_EV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_EV: uiSelect %d\r\n", uiSelect));

    Set_EVIndex(uiSelect);
    ImgCapture_SetParameter(_AEEV,(UINT8)Get_EVValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_Metering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_Metering: uiSelect %d\r\n", uiSelect));

    Set_MeteringIndex(uiSelect);
    ImgCapture_SetParameter(_AEMode,(UINT8)Get_MeteringValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_WB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_WB: uiSelect %d\r\n", uiSelect));

    Set_WBIndex(uiSelect);
    ImgCapture_SetParameter(_WBMode,(UINT8)Get_WBValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_ISO(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_ISO: uiSelect %d\r\n", uiSelect));

    Set_ISOIndex(uiSelect);
    ImgCapture_SetParameter(_ISOIndex,(UINT8)Get_ISOValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_Color(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_Color: uiSelect %d\r\n", uiSelect));

    Set_ColorIndex(uiSelect);
    //ImgCapture_SetParameter(_ImageEffect,(UINT8)Get_ColorValue(uiSelect));
    UI_SetParameter(_ImageEffect,Get_ColorValue(uiSelect));
	
    return NVTEVT_CONSUME;
}

INT32 OnExe_Saturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_Saturation: uiSelect %d\r\n", uiSelect));

    Set_SaturationIndex(uiSelect);
    ImgCapture_SetParameter(_Saturation,(UINT8)Get_SaturationValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_Sharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_Sharpness: uiSelect %d\r\n", uiSelect));

    Set_SharpnessIndex(uiSelect);
    ImgCapture_SetParameter(_Sharpness,(UINT8)Get_SharpnessValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_FD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_FD: uiSelect %d\r\n", uiSelect));

    Set_FDIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_AntiShaking(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_AntiShaking: uiSelect %d\r\n", uiSelect));

    Set_AntiShakingIndex(uiSelect);
    ImgCapture_SetParameter(_StillAntiShaking,(UINT8)Get_AntiShakingValue(uiSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_QuickReview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_QuickReview: uiSelect %d\r\n", uiSelect));

    Set_QuickReviewIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_DateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_DateImprint: uiSelect %d\r\n", uiSelect));

    Set_DateImprintIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_ContShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_ContShot: uiSelect %d\r\n", uiSelect));

    Set_ContShotIndex(uiSelect);
    return NVTEVT_CONSUME;
}

EVENT_ENTRY UIPhotoObjCtrlCmdMap[] =
{
    {NVTEVT_CAPTURE,                OnCapture},
    {NVTEVT_CAPTURE_STOP,           OnCaptureStop},
    {NVTEVT_ZOOMIN,                 OnZoomIn},
    {NVTEVT_ZOOMOUT,                OnZoomOut},
    {NVTEVT_ZOOMSTOP,               OnZoomStop},
    {NVTEVT_SMARTSCENECHECK,        OnSmartSceneCheck},
    {NVTEVT_EXE_SELFTIMER,          OnExe_Selftimer},
    {NVTEVT_EXE_FLASH,              OnExe_Flash},
    {NVTEVT_EXE_CAPTUREMODE,        OnExe_CaptureMode},
    {NVTEVT_EXE_QUALITY,            OnExe_Quality},
    {NVTEVT_EXE_SIZE,               OnExe_Size},
    {NVTEVT_EXE_SCENEMODE,          OnExe_SceneMode},
    {NVTEVT_EXE_EV,                 OnExe_EV},
    {NVTEVT_EXE_WB,                 OnExe_WB},
    {NVTEVT_EXE_METERING,           OnExe_Metering},
    {NVTEVT_EXE_ISO,                OnExe_ISO},
    {NVTEVT_EXE_COLOR,              OnExe_Color},
    {NVTEVT_EXE_SATURATION,         OnExe_Saturation},
    {NVTEVT_EXE_SHARPNESS,          OnExe_Sharpness},
    {NVTEVT_EXE_FD,                 OnExe_FD},
    {NVTEVT_EXE_ANTISHAKING,        OnExe_AntiShaking},
    {NVTEVT_EXE_QUICKREVIEW,        OnExe_QuickReview},
    {NVTEVT_EXE_DATEIMPRINT,        OnExe_DateImprint},
    {NVTEVT_EXE_CONTSHOT,           OnExe_ContShot},
    {NVTEVT_NULL,                   0},  //End of Command Map
};

VControl UIPhotoObjCtrl={
    "UIPhotoObj",
    UIPhotoObjCtrlCmdMap,
    0,
    CTRL_SYS,
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};



