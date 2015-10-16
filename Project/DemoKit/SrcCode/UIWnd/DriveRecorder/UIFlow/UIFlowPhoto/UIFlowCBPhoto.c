#include "Type.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "Debug.h"
#include "NVTUserCommand.h"
#include "SystemClock.h"
#include "ImageAlgInfor.h"
#include "KeyScanTsk.h"
#include "PlaySoundTsk.h"
#include "UIMenuPhotoSettings.h"
#include "UIMenuSetupSettings.h"
#include "UISystemStatusSettings.h"
#include "UIPhotoObj.h"
#include "FilesysTsk.h"
#include "UIMenuWndSetup.h"
#include "ide.h"
#include "PhotoDisplayTsk.h"

#define CBPhoto_Debug   1

#if CBPhoto_Debug
#define CBPhotoMG   debug_err
#else
#define CBPhotoMG   debug_ind
#endif

/**
  FlowPhoto_ImageCB

  Image (capture) process call back function

  @param uiEventID,pInfo
  @return void
*/
void FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo)
{
    switch (uiEventID)
    {
        case ALGMSG_PREVIEW:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_PREVIEW\r\n"));
            SysClk_SetClk(SPEED_ID_PHOTO);
            break;

        case ALGMSG_FOCUS:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_FOCUS\r\n"));
            break;

        case ALGMSG_FOCUSEND:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_FOCUSEND\r\n"));
            break;

        case ALGMSG_CHGOZOOMSTEP:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_CHGOZOOMSTEP (%d)\r\n", pInfo->OzoomIndex));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_OZOOMSTEPCHG, 0);
            break;

        case ALGMSG_CHGDZOOMSTEP:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_CHGDZOOMSTEP (%d) Ratio (%d)\r\n", pInfo->DzoomIndex, pInfo->DzoomRealRatio));
            Set_AlgMsgInfo(pInfo);
            if(Get_DZoomResetting() == FALSE)
            {
                Ux_PostEvent(NVTEVT_CB_DZOOMSTEPCHG, 0);
            }
            else
            {
                /* Ignore the msg while process dzoom reset action!! And clear the dzoom reset flag! */
                Set_DZoomResetting(FALSE);
            }
            break;

        case ALGMSG_PREFLASH:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_PREFLASH\r\n"));
            break;

        case ALGMSG_PREFLASHEND:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_PREFLASHEND\r\n"));
            break;

        case ALGMSG_FIRSTRAW:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_FIRSTRAW\r\n"));
            SysClk_SetClk(SPEED_ID_CAPTURE);
            break;

        case ALGMSG_JPEGOK:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_JPEGOK\r\n"));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_JPGOK, 0);
            break;

        case ALGMSG_VIDEO_RECORDING:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_VIDEO_RECORDING\r\n"));
            SysClk_SetClk(SPEED_ID_VIDEO);
            break;

        case ALGMSG_PREVIEWSTABLE:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_PREVIEWSTABLE\r\n"));
            if (IPL_GetMode() != IPL_MODE_CAP)
            {
                //#NT#2010/09/28#ethanlau -begin
                PhotoDisplay_SetMode(DISPLAY_MODE_PREVIEW, TRUE);
                //#NT#2010/09/28#ethanlau -end
            }
            if(!ide_get_enable_video(IDE_VIDEOID_1))
            {
                ide_enable_video(IDE_VIDEOID_1);
            }
            break;

        case ALGMSG_JPGEXIFMODIFY:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_JPGEXIFMODIFY\r\n"));
            break;

        case ALGMSG_LENSJAM:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_LENSJAM\r\n"));
            break;

        case ALGMSG_SUSKEY:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_SUSKEY\r\n"));
            KeyScan_Suspend();
            break;

        case ALGMSG_SUSPHOTO:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_SUSPHOTO\r\n"));
            //sus_tsk(PHOTOTSK_ID);
            break;

        case ALGMSG_SUSFLOWPRV:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_SUSFLOWPRV\r\n"));
            //sus_tsk(FLOWPVIEWTSK_ID);
            break;

        case ALGMSG_RSMKEY:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_RSMKEY\r\n"));
            KeyScan_Resume();
            break;

        case ALGMSG_RSMPHOTO:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_RSMPHOTO\r\n"));
            //rsm_tsk(PHOTOTSK_ID);
            break;

        case ALGMSG_RSMFLOWPRV:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_RSMFLOWPRV\r\n"));
            //rsm_tsk(FLOWPVIEWTSK_ID);
            break;

        case ALGMSG_SOUND_SHUTTER2:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_SOUND_SHUTTER2\r\n"));
            KeyScan_PlaySound(PLAYSOUND_SOUND_KEY_SHUTTER2);
            break;

        case ALGMSG_FDEND:
            debug_ind(("FlowPhoto_ImageCB: ALGMSG_FDEND\r\n"));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_FDEND, 0);
            break;

        case ALGMSG_SDEND:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_SDEND\r\n"));
            Ux_PostEvent(NVTEVT_CB_SDEND, 0);
            break;

        case ALGMSG_CAPEND:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_CAPEND\r\n"));
            Ux_PostEvent(NVTEVT_CB_CAPTUREEND, 0);
            break;

        case ALGMSG_CAPFSTOK:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_CAPFSTOK\r\n"));
            Ux_PostEvent(NVTEVT_CB_CAPFSTOK, 0);
            break;

        case ALGMSG_SMARTSCENE:
            CBPhotoMG(("FlowPhoto_ImageCB: ALGMSG_SMARTSCENE\r\n"));
            Set_AlgMsgInfo(pInfo);
            Ux_PostEvent(NVTEVT_CB_SMARTSCENE, 0);
            break;

        default:
            break;
    }
}


