#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "SysCfg.h"
#include "debug.h"
#include "SieCapTsk.h"
#include "AviRecAPI.h"
#include "UIMovieObj.h"
#include "AviPlayAPI.h"
#include "PlaybackTsk.h"
#include "UIPhotoObj.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "PhotoDisTsk.h"
#include "MediaRecAPI.h"
#include "UISystemStatusSettings.h"
#include "UIMenuSetupSettings.h"
#include "GenDateImprintData.h"
#if (_MOVIE_PIM_MODE_ == ENABLE)
#include "RawEncApi.h"
#endif
#include "KeyScanTsk.h"
extern BOOL gbKscanTVPlug;

extern void PlayFlowMoviePlayCBFunc(UINT32 event_id);

static UINT32 uiMovieSizeTable[MOVIE_SIZE_SETTING_MAX][4]=
{   //Width,Height,FR,TBR
    //{1920,  1080,   10,     1024*1024   },      // 1080P(16:9), 10fps, 1MB/sec
    //{1440,  1080,   15,     1024*1024   },      // 1080P(4:3), 15fps, 1MB/sec
    {1280,  720,    30,     800*1024    },      // 720P, 30fps, 800KB/sec
    //{ 848,  480,    30,     400*1024    },      // WVGA, 30fps, 400KB/sec
    { 720,  480,    30,     400*1024    },      // WVGA, 30fps, 400KB/sec    
    { 640,  480,    30,     300*1024    },      // WVGA, 30fps, 400KB/sec    
    { 320,  240,    30,     200*1024    }       // QVGA, 30fps, 200KB/sec
};

static UINT32 uiMovieQuaTable[3]=
{
    AVIREC_QUALITY_BEST,
    AVIREC_QUALITY_NORMAL,
    AVIREC_QUALITY_DRAFT
};

UINT32 GetMovieQualityValue(UINT16 uhIndex)
{
    return uiMovieQuaTable[uhIndex];
}

UINT32 GetMovieSizeWidth(UINT32 ubIndex)
{
    return uiMovieSizeTable[ubIndex][0];
}

UINT32 GetMovieSizeHeight(UINT32 ubIndex)
{
    return uiMovieSizeTable[ubIndex][1];
}

UINT32 GetMovieFrameRate(UINT32 ubIndex)
{
    return uiMovieSizeTable[ubIndex][2];
}

UINT32 GetMovieTargetBitrate(UINT16 ubIndex)
{
    return uiMovieSizeTable[ubIndex][3];
}

void SetMovieTargetBitrate(UINT16 ubIndex)
{
    return;
}

INT32 OnExe_MovieSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    BOOL bDontSaveFlag = FALSE;
	
   AlgMsgInfo *pAlgMsgInfo = Get_AlgMsgInfo();

    if(paramNum>0)
        uhSelect= paramArray[0];
	
    if(paramNum>1)
        bDontSaveFlag= paramArray[1];
		
    debug_msg("OnExe_MovieSize,uhSelect=%d\n\r",uhSelect);
    if(uhSelect != Get_MovieSizeIndex())
    {
             Photo_DZoom_GotoStep(UI_DZOOM_IDX_MIN);    
		pAlgMsgInfo->DzoomIndex = UI_DZOOM_IDX_MIN;
    }
    if(bDontSaveFlag == FALSE)
    Set_MovieSizeIndex(uhSelect);

    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_WIDTH, GetMovieSizeWidth(uhSelect), 0, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_HEIGHT, GetMovieSizeHeight(uhSelect), 0, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FRAMERATE, GetMovieFrameRate(uhSelect), 0, 0);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_CBTHRESHOLD, GetMovieFrameRate(uhSelect), 0, 0);
    SetMovieTargetBitrate(uhSelect);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_TARGETRATE, GetMovieTargetBitrate(uhSelect), 0, 0);

#if (_MOVIE_PIM_MODE_ == ENABLE)
    RawEnc_ChangeParameter(INPUT_WIDTH, GetMovieSizeWidth(uhSelect), 0, 0);
    RawEnc_ChangeParameter(INPUT_HEIGHT, GetMovieSizeHeight(uhSelect), 0, 0);
#endif

    switch(uhSelect)
    {
    //case MOVIE_SIZE_1080P_HD:
        //Photo_SetImageResolution(_Resolution_1920x1080, IPL_MODE_VIDEO);
        //break;
   // case MOVIE_SIZE_1080P:
        //Photo_SetImageResolution(_Resolution_1440x1080, IPL_MODE_VIDEO);
       // break;
    case MOVIE_SIZE_720P:
        Photo_SetImageResolution(_Resolution_1280x720, IPL_MODE_VIDEO);
        break;
    case MOVIE_SIZE_WVGA:
        Photo_SetImageResolution(_Resolution_848x480, IPL_MODE_VIDEO);
        //Photo_SetImageResolution(_Resolution_720x480, IPL_MODE_VIDEO);		
        break;
    case MOVIE_SIZE_VGA:
        //Photo_SetImageResolution(_Resolution_848x480, IPL_MODE_VIDEO);
        Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_VIDEO);		
        break;		
    case MOVIE_SIZE_QVGA:
        Photo_SetImageResolution(_Resolution_320x240, IPL_MODE_VIDEO);
        break;
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/03/23#Lily Kao -begin
    AVIREC_VIDEO_OBJ      AVIEncObj;
    //#NT#2009/03/23#Lily Kao -end

    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    //#NT#2009/03/23#Lily Kao -begin
    AVIRec_GetVideo(&AVIEncObj);
    Set_MovieQualityIndex(uhSelect);
    AVIEncObj.uiRecQuality = GetMovieQualityValue(uhSelect);
    //#NT#2009/03/24#Lily Kao -begin
    AVIEncObj.uiTargetBitrate   = GetMovieTargetBitrate(Get_MovieSizeIndex());
    //#NT#2009/03/24#Lily Kao -end
    AVIRec_SetVideo(&AVIEncObj);
    //#NT#2009/03/23#Lily Kao -end

    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    //#NT#2009/03/16#Lily Kao -begin
    //#Color setting is different in photo and movie mode
    UINT32 bSet = 1;
    //#NT#2009/03/16#Lily Kao -end

    if(paramNum>0)
        uhSelect= paramArray[0];

    //#NT#2009/03/16#Lily Kao -begin
    //#Color setting is different in photo and movie mode
    if(paramNum == 2)
        bSet = paramArray[1];
    if (bSet)
    {
        Set_MovieColorIndex(uhSelect);
    }
    //#NT#2009/03/16#Lily Kao -end

    ImgCapture_SetParameter(_ImageEffect,(UINT8)Get_ColorValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieSelftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    Set_MovieSelftimerIndex(uhSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieDIS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0, uiValue;

    if(paramNum>0)
        uiSelect= paramArray[0];

    Set_MovieDISIndex(uiSelect);
    uiValue = (uiSelect == MOVIE_DIS_ON) ? _DISMode_On : _DISMode_Off;
    ImgCapture_SetParameter(_DISMode, uiValue);

    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_MovieDateImprint: uiSelect %d\r\n", uiSelect));

    Set_MovieDateImprintIndex(uiSelect);
    return NVTEVT_CONSUME;
}

extern  GPSDATA gpsdata;
INT32 OnExe_MovieGPS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_MovieGPS: uiSelect %d\r\n", uiSelect));

    Set_MovieGPSIndex(uiSelect);

    if (Get_MovieGPSIndex()==MOVIE_GPS_ON)
    {
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_GPSON, 1, 0, 0);
        MediaRec_SetGPSData((UINT32)&gpsdata,sizeof(GPSDATA));
    } else {
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_GPSON, 0, 0, 0);
    }

    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_err(("OnExe_MovieCyclicRec: uiSelect %d\r\n", uiSelect));

    Set_MovieCyclicRecIndex(uiSelect);
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_CUTSEC,Get_MovieCyclicRecTime(),0,0);
    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieGodBless(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_MovieCyclicRec: uiSelect %d\r\n", uiSelect));

    Set_MovieGodBlessIndex(uiSelect);
    return NVTEVT_CONSUME;
}


INT32 OnExe_MovieGsensor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_ind(("OnExe_MovieCyclicRec: uiSelect %d\r\n", uiSelect));

    Set_MovieGsensorIndex(uiSelect);
    gsensor_SetSensitivity(uiSelect);	
    return NVTEVT_CONSUME;
}


INT32 OnExe_MovieGolfShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_err(("OnExe_MovieGolfShot: uiSelect %d\r\n", uiSelect));

    Set_MovieGolfShotIndex(uiSelect);
    return NVTEVT_CONSUME;
}

static void OnExe_MovieRecordSetting(void)
{
#if (_MOVIE_DATE_STAMP_ == ENABLE)
    {
        UINT32 uiDateFormat;

        // Get date format
        switch (Get_DateFormatIndex())
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

        // Generate date imprint
        switch (Get_MovieDateImprintIndex())
        {
        case MOVIE_DATEIMPRINT_ON:
            {
                /*
                COLOR_YCBCR MovieStampColorBg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp background color
                COLOR_YCBCR MovieStampColorFr = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp frame color
                COLOR_YCBCR MovieStampColorFg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp foreground color
                */
                COLOR_YCBCR MovieStampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
                COLOR_YCBCR MovieStampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
                COLOR_YCBCR MovieStampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

                // use STAMP_AUTO to set position automatically
                MovieDateImprint_Setup(
                    STAMP_ON |
                    STAMP_AUTO |
                    STAMP_DATE_TIME |
                    STAMP_BOTTOM_LEFT |
                    STAMP_POS_END |
                    uiDateFormat);
                MovieDateImprint_SetColor(&MovieStampColorBg, &MovieStampColorFr, &MovieStampColorFg);
                // use STAMP_MANUAL to set color
                MovieDateImprint_Setup(
                    STAMP_ON |
                    STAMP_MANUAL |
                    STAMP_DATE_TIME |
                    STAMP_BG_TRANSPARENT |
                    uiDateFormat);
            }
            break;

        case MOVIE_DATEIMPRINT_OFF:
        default:
            MovieDateImprint_Setup(STAMP_OFF);
            break;
        }
    }
#endif
}

INT32 OnExe_MovieFlashRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
 UINT32 uiSelect = 0;

    if(paramNum>0)
        uiSelect= paramArray[0];

    debug_err(("OnExe_MovieFlashRec: uiSelect %d\r\n", uiSelect));

    Set_MovieFlashRecIndex(uiSelect);
    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
 UINT32 uhSelect = 0;

    if(paramNum != 0)
        uhSelect= paramArray[0];

    debug_ind(("OnExe_MovieAudio\n\r"));

    Set_MovieAudioIndex(uhSelect);
    switch(uhSelect)
    {
        case MOVIE_AUDIO_OFF:
            aud_setGain(AUDIO_GAIN_MUTE);
            break;
        case MOVIE_AUDIO_ON:
        default:
            aud_setGain(AUDIO_GAIN_6);
            break;
    }
    return NVTEVT_CONSUME;
}

INT32 OnExe_MovieMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum != 0)
        uhSelect= paramArray[0];

    debug_ind(("OnExe_MovieMotionDet\n\r"));
    Set_MovieMotionDetIndex(uhSelect);
    return NVTEVT_CONSUME;
}

static BOOL bzFlashToggle = FALSE;
INT32 OnExe_MovieStartFlashRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    if (Get_MovieFlashRecIndex()==MOVIE_FLASHREC_ON)
    {
        OnExe_MovieRecordSetting();
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_GOLFSHOT_ON, 0, 0, 0);
#if (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
        // force enable file recovery in power lost
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_PWROFFPT, 1, 0, 0);
#endif
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FLASH_ON, 1, 0, 0);
#if (_MOVIE_REC_SEAMLESS_ == ENABLE)
        if (Get_MovieCyclicRecIndex() != MOVIE_CYCLIC_REC_OFF)
            MediaRec_ChangeParameter(MEDIAREC_RECPARAM_ENDTYPE, MEDIAREC_ENDTYPE_CUTOVERLAP, 0, 0);
        else
            MediaRec_ChangeParameter(MEDIAREC_RECPARAM_ENDTYPE, MEDIAREC_ENDTYPE_CUT_TILLCARDFULL, 0, 0);
#endif
        MediaRec_Record();
        bzFlashToggle = TRUE;
    }

    return NVTEVT_CONSUME;
}

INT32 UIRecMovie_Start(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{


    if (bzFlashToggle==TRUE && Get_MovieFlashRecIndex()==MOVIE_FLASHREC_ON)
    {
        bzFlashToggle=FALSE;
        MediaRec_PreRecordStartWriting();
        return NVTEVT_CONSUME;
    }

    // record setting
    OnExe_MovieRecordSetting();

    // update DCF Folder/File name
    FlowDCF_UpdateName(FALSE);

    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_CUTSEC,Get_MovieCyclicRecTime(),0,0);
    // disable Golf_shot/Flash recording
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_GOLFSHOT_ON, 0, 0, 0);
#if (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
        // force enable file recovery in power lost
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_PWROFFPT, 1, 0, 0);
#endif
    MediaRec_ChangeParameter(MEDIAREC_RECPARAM_FLASH_ON, 0, 0, 0);
#if (_MOVIE_REC_SEAMLESS_ == ENABLE)
    if (Get_MovieCyclicRecIndex() != MOVIE_CYCLIC_REC_OFF)
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_ENDTYPE, MEDIAREC_ENDTYPE_CUTOVERLAP, 0, 0);
    else {
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_ENDTYPE, MEDIAREC_ENDTYPE_CUT_TILLCARDFULL, 0, 0);
        MediaRec_ChangeParameter(MEDIAREC_RECPARAM_CUTSEC, 1500, 0, 0); // 25 min
    }
#endif
    if(gbKscanTVPlug==FALSE){
    KeyScan_EnableLEDToggle(KEYSCAN_LED_GREEN, TRUE);
    KeyScan_TurnOffLED(KEYSCAN_LED_GREEN);
    }
    if (Get_MovieGPSIndex()==MOVIE_GPS_ON)
    {
        //GPSRec_EnableGPSLog(TRUE);
    }
    MediaRec_Record();
    return NVTEVT_CONSUME;
}

INT32 UIRecMovie_Stop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(gbKscanTVPlug == FALSE){
    KeyScan_EnableLEDToggle(KEYSCAN_LED_GREEN, FALSE);
    KeyScan_TurnOnLED(KEYSCAN_LED_GREEN);
    KeyScan_TurnOffLED(KEYSCAN_LED_RED);
    	}
    MediaRec_Stop(1);
    if (Get_MovieGPSIndex()==MOVIE_GPS_ON)
    {
        GPSRec_EnableGPSLog(FALSE);
    }
    return NVTEVT_CONSUME;
}

UINT32 UIRecMovie_GetData(RECMOVIE_GET_DATATYPE dataType)
{
    UINT32 retV = 0;
    switch(dataType)
    {
        case RECMOVIE_MAXSECOND:
            //retV = MediaRec_Disk2Second();//AVIRec_Disk2Second();
            MediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_INDEXBUF,0);
            retV = MediaRec_Disk2SecondWithType(MEDIAREC_COUNTTYPE_FS,0);//AVIRec_Disk2Second();
            break;
        default:
            break;
    }
    //debug_err(("[UIRecMovie_GetData]retV = 0x%x\n\r",retV));
    return retV;
}

INT32 UIPlayMovie_Init(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    static AVIPLAY_OBJ aviPlayObj;
    PLAY_OBJ    playObj;
    INT32       retV;

    AVIPlay_Close();
    PB_Close(PB_WAIT_INFINITE);
    memset((void *)&playObj, 0, sizeof(PLAY_OBJ));
    memset((void *)&aviPlayObj, 0, sizeof(AVIPLAY_OBJ));
    aviPlayObj.CallBackFunc = PlayFlowMoviePlayCBFunc;
    aviPlayObj.bHWVideo = TRUE;
    aviPlayObj.bHWAudio = FALSE;
    aviPlayObj.bPlayAllFiles = FALSE ;
    retV = AVIPlay_Open(&aviPlayObj);
    return retV;
}

INT32 UIPlayMovie_Close(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 retV;
    static PLAY_OBJ playObj;
    memset((void *)&playObj,0,sizeof(PLAY_OBJ));

    AVIPlay_Stop();
    AVIPlay_Close();
    playObj.ui1stPlayMode = PLAYMODE_AVI;
    retV = PB_Open(&playObj);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);
    return retV;
}

INT32 UIPlayMovie_ForewardPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_FWD();
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_BackwardPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_BWD();
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_ForewardStepPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_FWDStep();
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_BackwardStepPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_BWDStep();
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_PausePlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_Pause();
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_SetSpeedL1(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_SpeedUp(UIPLAYAVI_SPEED_X1_IDX);
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_SetSpeedL2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_SpeedUp(UIPLAYAVI_SPEED_X2_IDX);
    return NVTEVT_CONSUME;
}

INT32 UIPlayMovie_SetSpeedL4(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    AVIPlay_SpeedUp(UIPLAYAVI_SPEED_X4_IDX);
    return NVTEVT_CONSUME;
}

UINT32 UIPlayMovie_GetData(PLAYMOVIE_GET_DATATYPE dataType)
{
    PAVIHEAD_INFO puiMovieInfo = 0;
    UINT32 retV = 0;
    switch(dataType)
    {
        case PLAYMOVIE_DIRID:
            retV = PB_GetCurFileDirId();
            break;
        case PLAYMOVIE_FILEID:
            retV = PB_GetCurFileNameId();
            break;
        case PLAYMOVIE_TOTAL_FRAME:
            retV = AVIPlay_GetCurrTotalVideoFrame();
            break;
        case PLAYMOVIE_TOTAL_SECOND:
            puiMovieInfo = PB_GetCurrAVIInfo();
            retV = puiMovieInfo->uiToltalSecs;
            break;
        case PLAYMOVIE_CURR_FRAME:
            retV = AVIPlay_GetCurrCurrVideoFrame();
            break;
        case PLAYMOVIE_CURR_SECOND:
            retV = AVIPlay_GetCurrPlaySecs();
            break;
        case PLAYMOVIE_LOCKSTATUS:
            retV = PB_GetFILELock();
            break;
        case PLAYMOVIE_FILENUM:
            retV = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILENUMS);
            break;
        default:
            debug_msg("UIPlayMovie_GetData:Unknown type=%d\r\n", dataType);
            break;
    }
    return retV;
}

void UIPlayMovie_SetData(PLAYMOVIE_SET_DATATYPE dataType)
{
    switch(dataType)
    {
        case PLAYMOVIE_SPEEDL1:
            AVIPlay_SpeedUp(UIPLAYAVI_SPEED_X1_IDX);
            break;
        case PLAYMOVIE_SPEEDL2:
            AVIPlay_SpeedUp(UIPLAYAVI_SPEED_X2_IDX);
            break;
        case PLAYMOVIE_SPEEDL4:
            AVIPlay_SpeedUp(UIPLAYAVI_SPEED_X4_IDX);
            break;
        default:
            debug_msg("UIPlayMovie_SetData:Unknown type=%d\r\n", dataType);
            break;
    }
    return;
}


INT32 Get_GSensorSensitivity(void)
{
    switch(Get_MovieGsensorIndex())
    {
        case MOVIE_GSENSOR_HIGH:
            return 640;//128;// 4G
        case MOVIE_GSENSOR_MED:
            return 800;//256;// 5G
        case MOVIE_GSENSOR_LOW:
            return 960;//384; //6G 
        case MOVIE_GSENSOR_OFF:
            return 9000;//no use
    }
}

////////////////////////////////////////////////////////////

EVENT_ENTRY UIMovieObjCtrlCmdMap[] =
{
    {NVTEVT_EXE_MOVIESIZE,          OnExe_MovieSize},
    {NVTEVT_EXE_QUALITY,            OnExe_MovieQuality},
    {NVTEVT_EXE_COLOR,              OnExe_MovieColor},
    {NVTEVT_EXE_SELFTIMER,          OnExe_MovieSelftimer},
    {NVTEVT_EXE_MOVIEDIS,           OnExe_MovieDIS},
    {NVTEVT_EXE_DATEIMPRINT,        OnExe_MovieDateImprint},
    {NVTEVT_EXE_GPS,                OnExe_MovieGPS},
    {NVTEVT_EXE_CYCLIC_REC,         OnExe_MovieCyclicRec},
    {NVTEVT_EXE_GOD_BLESS,          OnExe_MovieGodBless},
    {NVTEVT_EXE_GSENSOR,          OnExe_MovieGsensor},    
    {NVTEVT_EXE_GOLF_SHOT,          OnExe_MovieGolfShot},
    {NVTEVT_EXE_FLASH_REC,          OnExe_MovieFlashRec},
    {NVTEVT_EXE_AUDIO_SET,          OnExe_MovieAudio},
    {NVTEVT_EXE_MOTION_DET,         OnExe_MovieMotionDet},
    {NVTEVT_EXE_START_FLASH_REC,    OnExe_MovieStartFlashRec},
    {NVTEVT_START_RECAVI,           UIRecMovie_Start},
    {NVTEVT_STOP_RECAVI,            UIRecMovie_Stop},
    {NVTEVT_INIT_PLAYAVI,           UIPlayMovie_Init},
    {NVTEVT_CLOSE_PLAYAVI,          UIPlayMovie_Close},
    {NVTEVT_FWDPLAY_PLAYAVI,        UIPlayMovie_ForewardPlay},
    {NVTEVT_BWDPLAY_PLAYAVI,        UIPlayMovie_BackwardPlay},
    {NVTEVT_FWDSTEPPLAY_PLAYAVI,    UIPlayMovie_ForewardStepPlay},
    {NVTEVT_BWDSTEPPLAY_PLAYAVI,    UIPlayMovie_BackwardStepPlay},
    {NVTEVT_PAUSE_PLAYAVI,          UIPlayMovie_PausePlay},
    {NVTEVT_NULL,                   0},  //End of Command Map
};

VControl UIMovieObjCtrl={
    "UIMovieObj",
    UIMovieObjCtrlCmdMap,
    0,
    CTRL_SYS,   // CTRL_SYS
    0,  //data
    0,
    0,
    {0,0,0,0},
    0,
    0
};

