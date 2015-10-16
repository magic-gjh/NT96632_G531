/*
    Appication initialization for Movie record

    Get memory and set parameters for Movie record.

    @file       AppInitMovie.c
    @ingroup    mIPRJAPCfg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "SysCfg.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "AppInitMovie.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
#include "PhotoDisTsk.h"
#include "UIMovieObj.h"
#include "UIMenuMovieSettings.h"
#include "SieCapTsk.h"
#include "ImageAlgInfor.h"
#include "VideoEncode.h"
#include "MediaRecAPI.h"
#include "dai.h"
#include "GenDateImprintData.h"
#if (_MOVIE_PIM_MODE_ == ENABLE)
#include "RawEncApi.h"
#endif

/**
    @addtogroup mIPRJAPCfg
*/
//@{

#define MEDIA_BITRATE_W1280_F30  1250*1024//1250K
#define MEDIA_BITRATE_W640_F30   450*1024 //450K
#define MEDIA_BITRATE_W320_F30   200*1024 //200K

extern void     FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo);
extern void     FlowMovie_RecCB(UINT32 uiEventID, UINT32 uiSeconds);
extern void     FlowMovie_RawEncCB(UINT32 uiEventID);
extern void     FlowMovie_DrawStrCB(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth);
extern void     FlowMovie_SetDIS(UINT32 event, UINT32 value);

static UINT32   uiVideoFileFmt;

void Appinit_ModeMovie_GetVideoFmt(UINT32 *p1)
{
    *p1 = uiVideoFileFmt;
}

/**
    Initialize application for Movie mode

    Initialize application for Movie mode.

    @return void
*/
void AppInit_ModeMovie(void)
{
    MEDIAREC_OBJ    mediaRecObj;
    PHOTO_APPOBJ    PhotoObj;
    PHOTODISPLAY_APPOBJ PhotoDisplayObj;
    UINT32          uiPimBufSize = 0;   // Picture In Movie buffer size
    UINT32          drawSize = 0;       // date stamp drawing buffer size
    UINT32          maxWidth, maxHeight;
    UINT32          uiPoolAddr;
    VIDEO_ENCODER   *pObj;
#if (_MOVIE_PIM_MODE_ == ENABLE)
    RAW_ENC_OBJ     RawEncObj;
#endif
    //#NT#2010/10/27#Connie Yu -begin
    //#NT#movie DIS update
    PHOTODIS_APPOBJ     PhotoDisObj = {0};
    //#NT#2010/10/27#Connie Yu -end

    // Get memory for Photo task
    get_blk((VP*)&(uiPoolAddr), POOL_ID_IQ);
    rel_blk(POOL_ID_IQ, (VP)uiPoolAddr);

    memset(&PhotoObj,0x00,sizeof(PhotoObj));

    PhotoObj.uiMemoryAddr           = uiPoolAddr;
    PhotoObj.uiMemorySize           = POOL_SIZE_IQ;

    PhotoObj.uiDisplayFBWidth       = g_LCDSize.uiWidth;
    PhotoObj.uiDisplayFBHeight      = g_LCDSize.uiHeight;
    PhotoObj.CallBackFunc           = (FPALGCALLBACK)FlowPhoto_ImageCB;

    // Open Photo task for AE & AWB
    if (Photo_Open(&PhotoObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo task\r\n"));
    }

    // Register AE & AWB function pointer
    Photo_RegisterAE((FPPHOTOAE)aaa_AEprocess);
    Photo_RegisterAWB((FPPHOTOAWB)aaa_AWBprocess);

    PhotoDisplayObj.uiDisplayFBWidth       = g_LCDSize.uiWidth;
    PhotoDisplayObj.uiDisplayFBHeight      = g_LCDSize.uiHeight;
    PhotoDisplayObj.uiDisplayOutWidth      = g_LCDSize.uiWinWidth;
    PhotoDisplayObj.uiDisplayOutHeight     = g_LCDSize.uiWinHeight;

    // Open PhotoDisplay task
    if (PhotoDisplay_Open(&PhotoDisplayObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo display task\r\n"));
    }

    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    // Open Photo DIS task
    //#NT#2010/10/27#Connie Yu -begin
    //#NT#movie DIS update
    PhotoDisObj.uiDisplayFBWidth   = g_LCDSize.uiWidth;
    PhotoDisObj.uiDisplayFBHeight  = g_LCDSize.uiHeight;
    PhotoDisObj.uiDisplayOutWidth      = g_LCDSize.uiWinWidth;
    PhotoDisObj.uiDisplayOutHeight     = g_LCDSize.uiWinHeight;
    if (PhotoDis_Open(&PhotoDisObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo DIS task\r\n"));
    }
    //#NT#2010/10/27#Connie Yu -end
    PhotoDis_registerDIS((FPPHOTODIS)dis_accumUpdateProcess);
    //#NT#2009/11/27#Connie Yu -end

    // Get memory for encoder task
    get_blk((VP*)&(uiPoolAddr), POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

#if (_MOVIE_PIM_MODE_ == ENABLE)
    RawEncObj.uiMemAddr     = uiPoolAddr;
    RawEncObj.uiMemSize     = POOL_SIZE_PIM;
    RawEncObj.uiMaxWidth    = 1920;
    RawEncObj.uiMaxHeight   = 1080;
    RawEncObj.uiInputWidth  = 640;
    RawEncObj.uiInputHeight = 480;
    RawEncObj.uiEncQ        = 80;//2010/01/04 Meg
    RawEncObj.uiRawFormat   = RAW_FORMAT_420_UVPACK;
    RawEncObj.CallBackFunc  = FlowMovie_RawEncCB;//2010/01/07 Meg
    uiPimBufSize            = RawEncObj.uiMemSize;
    RawEnc_Open(&RawEncObj);
#endif

#if (_MOVIE_DATE_STAMP_ == ENABLE)
    maxWidth  = (1920*120)/100;                         // suppose max video size = 1920 x 1080, x1.2 for DIS
    maxHeight = MovieDateImprint_GetMaxFontHeight();    // max height of date stamp fonts
    drawSize  = maxWidth*maxHeight*2;                   // *2 for Y and uv
    MovieDateImprint_SetDrawAddr(uiPoolAddr + uiPimBufSize);
    //MovieDateImprint_SetDrawType(DATEDRAW_TYPE_420UV);
#endif

    mediaRecObj.uiMemAddr         = uiPoolAddr + uiPimBufSize + drawSize;
    mediaRecObj.uiMemSize         = POOL_SIZE_CAPTURE - uiPimBufSize - drawSize;
    mediaRecObj.CallBackFunc      = FlowMovie_RecCB;
#if (_MOVIE_DATE_STAMP_ == ENABLE)
    mediaRecObj.CallbackDrawFunc  = FlowMovie_DrawStrCB;
#else
    mediaRecObj.CallbackDrawFunc  = NULL;
#endif

#if(_GPS_RECEIVER_ == ENABLE)
    Set_MovieGPSIndex(MOVIE_GPS_ON);
#else
    Set_MovieGPSIndex(MOVIE_GPS_OFF);
#endif

#if (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
    mediaRecObj.CallbackOpenDISFunc = NULL;//2010/11/01 Meg
#else
    mediaRecObj.CallbackOpenDISFunc = FlowMovie_SetDIS;//NULL;//2010/11/01 Meg
#endif
    mediaRecObj.ucRecFormat       = MEDIAREC_AUD_VID_BOTH;//FAST_FWD:ucVidFrameRate should be 2
    mediaRecObj.ucSaveType        = MEDIAREC_SAVETYPE_ONE_SEC;

    // Set video parameters
    mediaRecObj.ucVidCodec        = MEDIAREC_ENC_H264;
#if (_MOVIE_DATE_STAMP_ == ENABLE)
    if (mediaRecObj.ucVidCodec == MEDIAREC_ENC_JPG)
        MovieDateImprint_SetDrawType(DATEDRAW_TYPE_422);
    else//H264
        MovieDateImprint_SetDrawType(DATEDRAW_TYPE_420UV);
#endif
    //mediaRecObj.ucYCbCrFormat     = MEDIAREC_JPEG_FORMAT_420;
    mediaRecObj.ucVidFrameRate    = MEDIAREC_VRATE_30;
    mediaRecObj.ucRecQuality      = MEDIAREC_VQUALITY_NORMAL;
    mediaRecObj.ucCBthreshold     = MEDIAREC_VRATE_30;
    mediaRecObj.uiWidth           = 1280;
    mediaRecObj.uiHeight          = 720;
    mediaRecObj.uiMaxWidth        = 1440;
    mediaRecObj.uiMaxHeight       = 1080;
    // Set audio parameters
#if (_MOVIE_FMT_ == _MOVIE_FMT_MOVH264_)
    mediaRecObj.FileInfo.filetype = MEDIAREC_MOV; // MOV format, audio can be PCM or AAC
    //mediaRecObj.ucAudCodec        = MEDIAREC_ENC_AAC;
    mediaRecObj.ucAudCodec        = MEDIAREC_ENC_PCM;
#else //_MOVIE_FMT_AVIH264_
    mediaRecObj.FileInfo.filetype = MEDIAREC_AVI; // AVI format, audio should be PCM
    mediaRecObj.ucAudCodec        = MEDIAREC_ENC_PCM;
#endif
    mediaRecObj.uiAudSampleRate   = AUDIO_SR_32000;//for HDMI
    mediaRecObj.uiAudBits         = 16;
    mediaRecObj.uiAudChannels     = 1;
    mediaRecObj.audioCh           = AUDIO_CH_RIGHT;//2009/12/22 Meg, set
    mediaRecObj.QVinfo.ucQVEnable           = FALSE;
    mediaRecObj.UserData.ucUserDataEmbed    = FALSE;
    mediaRecObj.BRCtrl.ucEnableBRCtrl       = TRUE;
    mediaRecObj.BRCtrl.uiTargetBitrate      = MEDIA_BITRATE_W1280_F30;
    mediaRecObj.FileLimit.uiMaxRecSecs      = 60;
    mediaRecObj.FileLimit.uiMaxRecFileSize  = 0;
    mediaRecObj.FileInfo.filetype           = MEDIAREC_AVI;//2010/12/15 Meg Lin
    mediaRecObj.ucSeamlessSec               = 600;//2010/12/15 Meg Lin
    if (mediaRecObj.ucVidCodec == MEDIAREC_ENC_H264)
    {
        pObj = MP_H264Enc_getVideoEncodeObject();
        MediaRec_SetVideoEncObj(pObj);
    }
    else
    {
        pObj = MP_MjpgEnc_getVideoEncodeObject();
        MediaRec_SetVideoEncObj(pObj);
    }

    //reset user data
    MediaRec_SetUserData(0, 0);
    MediaRec_SetFre1Data(0, 0);//2010/01/29 Meg Lin

    // set video file type
    uiVideoFileFmt = mediaRecObj.FileInfo.filetype;

    // Open encoder task
    if (MediaRec_Open(&mediaRecObj) != E_OK)
    {
        debug_err(("Error opening MovMjpg task\r\n"));
    }
}

//@}
