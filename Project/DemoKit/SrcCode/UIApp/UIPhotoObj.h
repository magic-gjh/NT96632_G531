#ifndef UI_PHOTOOBJ_H
#define UI_PHOTOOBJ_H

#include "Type.h"
#include "UIFramework.h"
#include "ImageAlgInfor.h"
#include "NVTUserCommand.h"
#include "UIMenuPhotoSettings.h"


//-----------------------------------------------------------------------------
// Global Definitions
//-----------------------------------------------------------------------------
// Zoom control
#define UI_ZOOM_CTRL_STOP               0
#define UI_ZOOM_CTRL_IN                 1
#define UI_ZOOM_CTRL_OUT                2
#define UI_ZOOM_CTRL_RESET_DZOOM        3
#define UI_DZOOM_IDX_MIN                10

// DZoom string
#define MAX_DZOOM_STR_LEN               16

//-----------------------------------------------------------------------------
// Type Define
//-----------------------------------------------------------------------------
// Current zoom interface
typedef enum
{
    ZOOM_IF_OPTICAL = 0,
    ZOOM_IF_DIGITAL,
    ZOOM_IF_TOTAL_NUM
} ZOOM_IF;

// Scene mode info from IQ
typedef struct _SCENE_MODE_INFO
{
    UINT32      Scene;
    UINT32      Flash;
    UINT32      EV;
    UINT32      Sharpness;
} SCENE_MODE_INFO, *PSCENE_MODE_INFO;

//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern VControl UIPhotoObjCtrl;

extern AlgMsgInfo *Get_AlgMsgInfo(void);
extern void     Set_AlgMsgInfo(AlgMsgInfo *pInfo);

extern UINT32   Get_FlashValue(UINT32 uiIndex);
extern UINT32   Get_SizeValue(UINT32 uiIndex);
extern const CHAR *Get_SizeString(UINT32 uiIndex);
extern UINT32   Get_QualityValue(UINT32 uiIndex);
extern UINT32   Get_ColorValue(UINT32 uiIndex);
extern UINT32   Get_SceneModeValue(UINT32 uiIndex);
extern UINT32   Get_SceneModeFromValue(_IMGSceneMode_sel uiVal);
extern UINT32   Get_SceneRtnFlash(UINT32 uiIndex);
extern UINT32   Get_SceneRtnEV(UINT32 uiIndex);
extern UINT32   Get_SceneRtnSharpness(UINT32 uiIndex);
extern UINT32   Get_EVValue(UINT32 uiIndex);
extern UINT32   Get_MeteringValue(UINT32 uiIndex);
extern UINT32   Get_WBValue(UINT32 uiIndex);
extern UINT32   Get_ISOValue(UINT32 uiIndex);
extern UINT32   Get_SaturationValue(UINT32 uiIndex);
extern UINT32   Get_SharpnessValue(UINT32 uiIndex);
extern UINT32   Get_AntiShakingValue(UINT32 uiIndex);
extern UINT32   Get_QuickReviewValue(UINT32 uiIndex);
extern CHAR     *Get_SelftimerCntString(UINT32 uiCount);

extern void     Set_CurrentZoomIF(UINT32 uiIndex);
extern UINT32   Get_CurrentZoomIF(void);
extern void     Set_DZoomResetting(BOOL bReset);
extern BOOL     Get_DZoomResetting(void);
extern CHAR     *Get_DZoomRatioString(AlgMsgInfo *pInfo);

extern void     UI_ZoomControl(UINT32 uiZoomCtrl, BOOL bDZoomEnable);




#endif
