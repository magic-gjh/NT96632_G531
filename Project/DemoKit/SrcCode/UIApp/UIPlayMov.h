#ifndef UI_PLAYMOV_H
#define UI_PLAYMOV_H

#include "UIPlayObj.h"


//-----------------------------------------------------------------------------
// Global Definitions
//-----------------------------------------------------------------------------
#define UI_MEDIAPLAY_EVENT_FINISH            1   ///< Finish
#define UI_MEDIAPLAY_EVENT_ONE_SECOND        2   ///< One second arrives
#define UI_MEDIAPLAY_EVENT_ONE_VIDEOFRAME    3   ///< Decode one video-frame
#define UI_MEDIAPLAY_EVENT_START             4   ///< Start mjpgMov playback
#define UI_MEDIAPLAY_EVENT_STOPATEND         5




//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern UINT32   PlayMov_GetSpeedValue(UINT32 uiIndex);
extern void     PlayMov_MediaCallBack(UINT32 uiEventID);
extern void     PlayMov_PauseMeida(void);
extern void     PlayMov_ResumeMeida(void);
extern void     PlayMov_StopMeida(void);
extern void     PlayMov_PlayMedia(MovPlayCb movPlayCB);
extern void     PlayMov_FwdMedia(UINT32 uiIndex);
extern void     PlayMov_BwdMedia(UINT32 uiIndex);
extern void     PlayMov_StopFwdMedia(void);
extern void     PlayMov_StopBwdMedia(void);

#endif
