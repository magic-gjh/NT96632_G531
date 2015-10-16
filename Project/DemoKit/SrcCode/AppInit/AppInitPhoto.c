/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitPhoto.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for Photo mode
                Get memory and set parameters for Photo mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#if(_WAVSTUDIO_MODE_ == ENABLE)
#include "WavStudioTsk.h"
#endif
#include "UIFlow.h"
#include "GlobalVar.h"
#include "KeyScanTsk.h"
#include "SieCapTsk.h"
#include "PhotoTsk.h"
#include "PhotoDisplayTsk.h"
//#NT#2010/01/22#Connie Yu -begin
//#NT#DIS update
#include "PhotoDisTsk.h"
//#NT#2010/01/22#Connie Yu -end
//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for MSDCExt with NVT
#if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
#include "MsdcNvtCb_Photo.h"
#endif
//#NT#2009/09/09#Niven Cho -end
extern void     FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo);
#if (_WAVSTUDIO_MODE_ == ENABLE)
extern void FlowCommon_WAVStudioCB(UINT32 uiEventID);
#endif
#if (_WAV_SINGLE_USERDATA_ == ENABLE)
// Data in AppInitPlayback.c, must check the size
extern UINT8    g_uiWAVHeaderUserData[180];
#endif

/**
  Initialize application for Photo mode

  Initialize application for Photo mode.

  @param void
  @return void
*/
void AppInit_ModePhoto(void)
{
    UINT32              uiPoolAddr;
    PHOTO_APPOBJ        PhotoObj;
    CAPTURE_APPOBJ      CaptureObj;
    PHOTODISPLAY_APPOBJ PhotoDisplayObj;
    //#NT#2010/10/27#Connie Yu -begin
    //#NT#movie DIS update
    PHOTODIS_APPOBJ     PhotoDisObj = {0};
    //#NT#2010/10/27#Connie Yu -end
#if(_WAVSTUDIO_MODE_ == ENABLE)
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FMT       WavFormat;
#endif

    // Get memory for Capture task
    get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    CaptureObj.uiMemoryAddr         = uiPoolAddr;
    CaptureObj.uiMemorySize         = POOL_SIZE_CAPTURE - POOL_SIZE_WAV;
    CaptureObj.CallBackFunc         = (FPALGCALLBACK)FlowPhoto_ImageCB;
    //#NT#2009/07/30#Niven Cho -begin
    //#NT#Added., for MSDCExt with NVT
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    CaptureObj.CallBackFunc         = MsdcNvt_HookImageCb(CaptureObj.CallBackFunc);
    #endif
    //#NT#2009/07/30#Niven Cho -end

    // Open Capture task
    if (ImgCapture_Open(&CaptureObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for capture task\r\n"));
    }

#if(_WAVSTUDIO_MODE_ == ENABLE)
    //#NT#2008/04/28#Chris Hsu -begin
    //#NT#Add sample code for bits per sample and user data
    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;
    WavFormat.AudioSamplingRate = AUDIO_SR_11025;
#if (_VM_FORMAT_ == _VM_FORMAT_PCM8_)
    // PCM 8 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_8;
#elif (_VM_FORMAT_ == _VM_FORMAT_PCM16_)
    // PCM 16 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_16;
#elif (_VM_FORMAT_ == _VM_FORMAT_ADPCM_)
    // ADPCM
    WavFormat.AudioCodec        = AUDIOCODEC_IMA_ADPCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_ADPCM;
#elif (_VM_FORMAT_ == _VM_FORMAT_ASFPCM_)
    // ASF PCM
    #if (WAVFILE_ASF_ENABLE == ENABLE)
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.FileFormat        = WAVFILE_ASF;
    #endif
#endif
    WavStudio_SetFormat(&WavFormat);

    // Set maximum recording time
    WavStudio_SetMaxRecTime(10);

    // Set user data
#if (_WAV_SINGLE_USERDATA_ == ENABLE)
    WavStudio_SetUserData((UINT32)&g_uiWAVHeaderUserData, (UINT32)sizeof(g_uiWAVHeaderUserData));
#else
    WavStudio_SetUserData(0, 0);
#endif

    // Set WAV application object
    WavObj.uiMemoryAddr         = uiPoolAddr + POOL_SIZE_CAPTURE - POOL_SIZE_WAV;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    WavObj.WavStudioCB          = (WAVSTUDIO_CB)FlowCommon_WAVStudioCB;
    //#NT#2008/04/28#Chris Hsu -end

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for wav studio task\r\n"));
    }
#endif

    // Get memory for Photo task
    get_blk((VP *)&uiPoolAddr, POOL_ID_IQ);
    rel_blk(POOL_ID_IQ, (VP)uiPoolAddr);

    memset(&PhotoObj,0x00,sizeof(PhotoObj));

    PhotoObj.uiMemoryAddr           = uiPoolAddr;
    PhotoObj.uiMemorySize           = POOL_SIZE_IQ;

    PhotoObj.uiDisplayFBWidth       = g_LCDSize.uiWidth;
    PhotoObj.uiDisplayFBHeight      = g_LCDSize.uiHeight;

    PhotoObj.CallBackFunc           = (FPALGCALLBACK)FlowPhoto_ImageCB;
    //#NT#2009/07/30#Niven Cho -begin
    //#NT#Added., for MSDCExt with NVT
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    PhotoObj.CallBackFunc           = MsdcNvt_HookImageCb(PhotoObj.CallBackFunc);
    #endif
    //#NT#2009/07/30#Niven Cho -end

    /*
    if (KeyScan_IsTVPlugIn() == TRUE)
    {
        PhotoObj.uiDisplayFBWidth   = g_LCDSize.uiWidth >> 1;
        PhotoObj.uiDisplayFBHeight  = g_LCDSize.uiHeight >> 1;
    }
    else
    {
        PhotoObj.uiDisplayFBWidth   = g_LCDSize.uiWidth;
        PhotoObj.uiDisplayFBHeight  = g_LCDSize.uiHeight;
    }
    */

    // Open Photo task
    if (Photo_Open(&PhotoObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo task\r\n"));
    }

    PhotoDisplayObj.uiDisplayFBWidth    = g_LCDSize.uiWidth;
    PhotoDisplayObj.uiDisplayFBHeight   = g_LCDSize.uiHeight;
    PhotoDisplayObj.uiDisplayOutWidth   = g_LCDSize.uiWinWidth;
    PhotoDisplayObj.uiDisplayOutHeight  = g_LCDSize.uiWinHeight;

    // Open PhotoDisplay task
    if (PhotoDisplay_Open(&PhotoDisplayObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo display task\r\n"));
    }

    // Register AE/AWB/FD/SD function pointer
    Photo_RegisterAE((FPPHOTOAE)aaa_AEprocess);
    Photo_RegisterAWB((FPPHOTOAWB)aaa_AWBprocess);
    #if (_FD_FUNCTION_ == ENABLE)
    Photo_RegisterFD((FPPHOTOFD)fd_FDprocess);
    #endif
    #if (_SD_FUNCTION_ == ENABLE)
    Photo_RegisterSD((FPPHOTOSD)sd_SmileDetectionProcess);
    #endif

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
}

//@}
