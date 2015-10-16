#ifndef UI_MENUPLAYSETTING_H
#define UI_MENUPLAYSETTING_H

#include "Type.h"



//-----------------------------------------------------------------------------
// SlideShow Interval
//-----------------------------------------------------------------------------
typedef enum
{
    SLIDE_3SEC = 0,
    SLIDE_5SEC,
    SLIDE_10SEC,
    SLIDE_SETTING_MAX
} SLIDE_SETTING;

//-----------------------------------------------------------------------------
// PlayMov Speed
//-----------------------------------------------------------------------------
typedef enum
{
    PLAYMOV_SPEED_BWD_8X = 0,   // 8X backward play speed
    PLAYMOV_SPEED_BWD_4X,       // 4X backward play speed
    PLAYMOV_SPEED_BWD_2X,       // 2X backward play speed
    PLAYMOV_SPEED_FWD_1X,       // Normal play speed
    PLAYMOV_SPEED_FWD_2X,       // 2X forward play speed
    PLAYMOV_SPEED_FWD_4X,       // 4X forward play speed
    PLAYMOV_SPEED_FWD_8X,       // 8X forward play speed
    PLAYMOV_SPEED_SETTING_MAX
} PLAYMOV_SPEED;

//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern void     Set_SlideIndex(UINT32 uiIndex);
extern UINT32   Get_SlideIndex(void);

extern void     Set_DPOFPrtIndex(UINT32 uiIndex);
extern UINT32   Get_DPOFPrtIndex(void);

extern void     Set_PlayMovSpeedIndex(UINT32 uiIndex);
extern UINT32   Get_PlayMovSpeedIndex(void);

extern void Set_MenuItem_Copy_Status(UINT32 uiStatus);
extern void Set_MenuItem_Slide_Status(UINT32 uiStatus);
extern void Set_MenuItem_DPOF_Status(UINT32 uiStatus);
extern void Set_MenuItem_Delete_Status(UINT32 uiStatus);
extern void Set_MenuItem_Protect_Status(UINT32 uiStatus);
extern void Set_MenuItem_Thumbnail_Status(UINT32 uiStatus);


#endif
