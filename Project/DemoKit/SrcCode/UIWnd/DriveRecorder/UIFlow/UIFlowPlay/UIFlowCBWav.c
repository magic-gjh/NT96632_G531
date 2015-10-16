#include "Type.h"
#include "UIFramework.h"
#include "WavStudioTsk.h"
#include "NVTUserCommand.h"
void FlowCommon_WAVStudioCB(UINT32 uiEventID)
{
    switch(uiEventID)
    {
        #if 0
        case WAVSTUDIO_EVENT_RECORDING:
            Ux_PostEvent(NVTCMD_CALLBACK, 1, NVTEVT_MEMO_RECORD);
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_RECORDING\r\n"));
            break;
        case WAVSTUDIO_EVENT_PLAYING:
            //UIFrameworkSendCommand(NVTCMD_CALLBACK, 1, NVTEVT_MEMO_PLAY);
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_PLAYING\r\n"));
            break;
        case WAVSTUDIO_EVENT_STOPPED:
            Ux_PostEvent(NVTCMD_CALLBACK, 1, NVTEVT_MEMO_STOP);
            debug_err(("CALLBACK:WAVSTUDIO_EVENT_STOPPED \r\n"));
            break;
        case WAVSTUDIO_EVENT_PAUSED :
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_PAUSED\r\n"));
            break;
        case WAVSTUDIO_EVENT_RESUMED:
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_RESUMED\r\n"));
            break;
        case WAVSTUDIO_EVENT_SAVING:
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_SAVING\r\n"));
            break;
        case WAVSTUDIO_EVENT_DURATION:
            Ux_PostEvent(NVTCMD_CALLBACK, 1, NVTEVT_MEMO_DURATION);
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_DURATION\r\n"));
            break;
        case WAVSTUDIO_EVENT_DISKFULL:
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_DISKFULL\r\n"));
            break;
        case WAVSTUDIO_EVENT_BUFFULL:
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_BUFFULL\r\n"));
            break;
        case WAVSTUDIO_EVENT_INVALIDFILE:
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_INVALIDFILE\r\n"));
            break;
        case WAVSTUDIO_EVENT_RWERROR:
            debug_err(("CALLBACK: WAVSTUDIO_EVENT_RWERROR\r\n"));
            break;
        default:
            debug_err(("CALLBACK EVENT = %x\r\n",uiEventID));
            break;
        #endif
    }
}
