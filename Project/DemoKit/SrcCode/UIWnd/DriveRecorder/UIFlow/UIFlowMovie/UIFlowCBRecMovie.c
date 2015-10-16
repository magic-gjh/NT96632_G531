#include "UIFramework.h"
#include "UIGraphics.h"

#include "SysCfg.h"
#include "Debug.h"
#include "NVTUserCommand.h"
#include "AviRecApi.h"
#include "UIFlowWndMovie.h"
#include "UIMenuMovieSettings.h"
#include "MediaRecApi.h"
#include "GenDateImprintData.h"
#if (_MOVIE_PIM_MODE_ == ENABLE)
#include "RawEncApi.h"
#endif
#include "PhotoDisTsk.h"
#include "KeyScanTsk.h"

extern BOOL gbMoviFileLock;

void FlowMovie_RecCB(UINT32 uiEventID, UINT32 uiSeconds)
{
 static  char    szFilename[32];
 static  UINT16  uhFolderId, uhFileId;

    //debug_err(("FlowMovie_RecCB:uiEventID=%d,uiSeconds=%d\r\n",uiEventID,uiSeconds));
    switch (uiEventID)
    {
    case MEDIAREC_EVENT_RESULT_OVERTIME:
        Ux_PostEvent(NVTEVT_CB_MOVIE_OVERTIME, 0);
        debug_err(("FlowMovie_RecCB() result overtime: %d\r\n", uiEventID));
        break;

    case MEDIAREC_EVENT_RESULT_FULL:
    case AVIREC_EVENT_RESULT_OVERWRITE:
        debug_err(("--FlowMovie_RecCB() result full: %d\r\n", uiEventID));
		
        Ux_PostEvent(NVTEVT_CB_MOVIE_FULL, 0);	
        break;

    case MEDIAREC_EVENT_RESULT_HW_ERR:
//    case MEDIAREC_EVENT_RESULT_SLOW:
    case MEDIAREC_EVENT_RESULT_NOTREADY:
    case AVIREC_EVENT_RESULT_INVALIDFILE:
    case MEDIAREC_EVENT_RESULT_WRITE_ERR:
        Ux_PostEvent(NVTEVT_CB_MOVIE_WR_ERROR, 0);
        debug_err(("FlowMovie_RecCB() result error: %d\r\n", uiEventID));
        break;

    case MEDIAREC_EVENT_THRESHOLD:
        debug_ind(("FlowMovie_RecCB() uiSeconds = %d\r\n", uiSeconds));
        Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 1,  uiSeconds);
        break;

    case MEDIAREC_EVENT_RESULT_NORMAL:
        #if (_GPS_RECEIVER_ == ENABLE && _GPS_RECEIVER_LOG_ == ENABLE)
        GPSRec_LogGPSData();
        #endif
        debug_ind(("FlowMovie_RecCB() NVTEVT_CB_MOVIE_FINISH \r\n"));

        Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0);
        break;

    case MEDIAREC_EVENT_CUTFILE_END:
        debug_msg("%s,MEDIAREC_EVENT_CUTFILE_END\r\n",__func__);	
        FlowDCF_UpdateName(FALSE);
        #if (_GPS_RECEIVER_ == ENABLE&& _GPS_RECEIVER_LOG_ == ENABLE)
        GPSRec_LogGPSData();
        #endif
        Ux_PostEvent(NVTEVT_CB_MOVIE_CULT_NORMAL, 0);
		
        break;

//#NT#2011/2/9#Philex - being
// get cyclic recording deleted file's DID/FID,
// and delete the corresponding gps log file
    case MEDIAREC_EVENT_DELETE_DID:
        debug_msg("MEDIAREC_EVENT_DELETE_DID\r\n");
        break;

    case MEDIAREC_EVENT_DELETE_FID:
        debug_msg("MEDIAREC_EVENT_DELETE_FID\r\n");
        break;
//#NT#2011/2/9#Philex - end

    case MEDIAREC_EVENT_RESULT_SLOW:
    case MEDIAREC_EVENT_RESULT_CUT_FAIL:
        //#NT#2011/5/25#Philex - begin
        // recording fisished because card write speed is to slow
        // re-start recording again if this condition comes true
        debug_err(("MEDIAREC_EVENT_RESULT_CUT_FAIL\r\n"));
        Ux_PostEvent(NVTEVT_KEY_ENTER, 3, NVTEVT_KEY_PRESS, FLGKEY_PRESSED,UIFlowWndMovie_Restart_Rec);
        //Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 1,UIFlowWndMovie_Restart_Rec);
        //#NT#2011/5/25#Philex - end
        break;

    default:
        break;
    }
}

#if (_MOVIE_PIM_MODE_ == ENABLE)
/**
    Callback function for Raw Encoding Task

    Callback function for Raw Encoding Task. Since Raw Encoding Task can only
    handle one picture at a time. UI has to wait for Raw Encoding Task to finish
    its job to capture next picture. The callback function will inform UI that
    JPEG encoding is finished.

    @param[in] uiEventID    Event ID from Raw Encoding Task
    @return void
*/
void FlowMovie_RawEncCB(UINT32 uiEventID)
{
    debug_ind(("%s, ID %d\r\n", __func__, uiEventID));

    switch (uiEventID)
    {
    case RAWENC_EVENT_RESULT_OK:
        debug_ind(("RawEnc write file OK\r\n"));
        // Inform media recording task about picture file size
        MediaRec_MinusFilesize(RawEnc_GetRawFilesize());
        break;

    case RAWENC_EVENT_RESULT_ERR:
        debug_err(("RawEnc encode error\r\n"));
        break;

    case RAWENC_EVENT_RESULT_WRITE_ERR:
        debug_err(("RawEnc write file error\r\n"));
        break;
    }

    // Inform UI for picture in movie is done
    UIFlowWndMovie_DrawPIM(FALSE);
    Ux_PostEvent(NVTEVT_REDRAW, 0);
    UIFlowWndMovie_SetPIMReady(TRUE);
}
#endif 

#if (_MOVIE_DATE_STAMP_ == ENABLE)
void FlowMovie_DrawStrCB(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    MovieDateImprint_CopyData(yAddr, cbAddr, crAddr, lineY, imgWidth);
}
#endif

void FlowMovie_SetDIS(UINT32 event, UINT32 value)
{
    if (Get_MovieDISIndex() == MOVIE_DIS_ON)
    {
        ide_disableVideo(IDE_VIDEOID_1);

        if (event == MEDIAREC_IPLMODE_PREVIEW)
        {
            PhotoDis_setMode(PHOTODIS_MODE_IDLE, TRUE);
            PhotoDisplay_SetMode(DISPLAY_MODE_VIDEO, TRUE);
        }
        else
        {
            PhotoDisplay_SetMode(DISPLAY_MODE_DISVIEW, TRUE);
            PhotoDis_setMode(PHOTODIS_MODE_START, TRUE);
        }

        sie_waitVD(4);
        ide_enableVideo(IDE_VIDEOID_1);
    }
}
