#include "Type.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "Debug.h"
#include "NVTUserCommand.h"
#include "KeyScanTsk.h"
#include "MP3Api.h"



/**
  FlowMP3_PlayCB

  MP3 play process call back function

  @param uiEventID, uiParam
  @return void
*/
void FlowMP3_PlayCB(UINT32 uiEventID, UINT32 uiParam)
{
    switch (uiEventID)
    {
        case MP3CALLBACK_ONESECOND:
            debug_err(("FlowMP3_PlayCB: MP3CALLBACK_ONESECOND (%ld)\r\n", uiParam));
            Ux_PostEvent(NVTEVT_CB_MP3PLAY_ONESECOND, 1, uiParam);
            break;

        case MP3CALLBACK_FINISH:
            debug_err(("FlowMP3_PlayCB: MP3CALLBACK_FINISH\r\n"));
            Ux_PostEvent(NVTEVT_CB_MP3PLAY_FINISH, 0);
            break;

        default:
            break;
    }
}

