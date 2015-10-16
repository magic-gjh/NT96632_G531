#include "Debug.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "NVTUserCommand.h"
#include "SysCfg.h"
#include "MediaPlayApi.h"

void PlayFlowMoviePlayCBFunc(UINT32 event_id)
{
    switch (event_id)
    {
    case MEDIAPLAY_EVENT_FINISH:
        Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0);
        break;

    case MEDIAPLAY_EVENT_ONE_SECOND:
        Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 0);
        break;

    case MEDIAPLAY_EVENT_START:
        Ux_PostEvent(NVTEVT_CB_MOVIE_START, 0);
        break;

    case MEDIAPLAY_EVENT_ONE_VIDEOFRAME:
        //Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_VIDEOFRAME, 0);
        break;

    default:
        debug_err(("PlayFlowMoviePlayCBFunc: MOVH264 unknown eventid=%d\r\n", event_id));
    }
}

