/*
    kernel initialization for user defined task, event...


    @file       KerInit.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "Kernel.h"
#include "SysCfg.h"

// Data in SysCfg.c
extern  T_CTSK                  user_systasktabl[];
extern  SYSTEM_EVF_TABLE        user_sysevftabl[];
extern  T_CSEM                  user_syssemtabl[];
extern  SYSTEM_MBX_TABLE        user_sysmbxtabl[];
extern  T_CMPF                  user_sysfpltabl[];
//======================================================================
// Module Task Stack Memory Allocation
//======================================================================
// Playback
#if (_PLAYBACK_MODE_ == ENABLE)
__align(4) UCHAR pucStkPlayback[STKSIZE_PLAYBACK];
#endif

// IQ
#if (_IQ_MODE_ == ENABLE)
__align(4) UCHAR pucStk1stPhoto[STKSIZE_1STPHOTO];
__align(4) UCHAR pucStk2ndPhoto[STKSIZE_2NDPHOTO];
__align(4) UCHAR pucStk3rdPhoto[STKSIZE_3RDPHOTO];
__align(4) UCHAR pucStkImgSlice[STKSIZE_IMGSLICETSK];
__align(4) UCHAR pucStkSieCap[STKSIZE_SIECAPTSK];
__align(4) UCHAR pucStkDisplayPhoto[STKSIZE_DISPLAYPHOTO];
__align(4) UCHAR pucStkLensCtrlTask[STKSIZE_LENSCTRL];
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
__align(4) UCHAR pucStkDisPhoto[STKSIZE_DISPHOTO];
//#NT#2009/11/27#Connie Yu -end
#endif

// USB
#if ((_USBMSDC_MODE_ == ENABLE) || \
     (_USBSICD_MODE_ == ENABLE) || \
     (_USBPCC_MODE_ == ENABLE))
__align(4) UCHAR pucStkUsbCtrl[STKSIZE_USBCTRL];
__align(4) UCHAR pucStkUsbSetup[STKSIZE_USBSETUP];
#endif

#if (_USBMSDC_MODE_ == ENABLE)
__align(4) UCHAR pucStkUsbStrgVnd[STKSIZE_USBSTRGVND];
#endif

#if (_USBPCC_MODE_ == ENABLE)
__align(4) UCHAR pucStkUsbPcCamVideo[STKSIZE_USBPCCAMVIDEO];
#endif

#if (_USBSICD_MODE_ == ENABLE)
__align(4) UCHAR pucStkUsbSICDVnd[STKSIZE_USBSICDVND];
__align(4) UCHAR pucStkUsbSICDEP3[STKSIZE_USBSICDEP3];
#endif

// MP3
#if (_MP3_MODE_ == ENABLE)
__align(4) UCHAR pucStkMP3Play[STKSIZE_MP3PLAY];
__align(4) UCHAR pucStkMP3PlayFileRead[STKSIZE_MP3PLAYFILESYS];
#endif

// WAV
#if (_WAVSTUDIO_MODE_ == ENABLE)
__align(4) UCHAR pucStkWavStudio[STKSIZE_WAVSTUDIO];
#endif

// Media Framework
__align(4) UCHAR pucStkMediaRec[STKSIZE_MEDIAREC];
__align(4) UCHAR pucStkMediaPlay[STKSIZE_MEDIAPLAY];
__align(4) UCHAR pucStkMediaVideoRec[STKSIZE_MEDIAVIDEOREC];
__align(4) UCHAR pucStkMediaAudioEnc[STKSIZE_MEDIAAUDIOENC];
__align(4) UCHAR pucStkMediaVideoPlay[STKSIZE_MEDIAVIDEOPLAY];
__align(4) UCHAR pucStkMediaAudioPlay[STKSIZE_MEDIAAUDIOPLAY];
__align(4) UCHAR pucStkMediaRecFS[STKSIZE_MEDIARECFS];//2010/12/15 Meg Lin
__align(4) UCHAR pucStkMediaRecCmd[STKSIZE_MEDIARECCMD];//2010/12/15 Meg Lin

// Picture In Movie
#if (_MOVIE_PIM_MODE_ == ENABLE)
__align(4) UCHAR pucStkRawEnc[STKSIZE_RAWENC];
#endif

// GPS
#if(_GPS_RECEIVER_ == ENABLE)
__align(4) UCHAR pucStkGPSReceive[STKSIZE_GPSRECEIVE];
#endif

//======================================================================
// Module Task Configration Infomation
//======================================================================
// Playback
#if (_PLAYBACK_MODE_ == ENABLE)
extern void PlaybackTsk(void);

TASKINFO PlaybackAPPCfg =
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{PLAYBAKTSK_ID,        PRI_PLAYBACK,       STKSIZE_PLAYBACK,       PlaybackTsk,        pucStkPlayback},        "Playback"};
#endif

// IQ
#if (_IQ_MODE_ == ENABLE)
extern void Photo1stTsk(void);
extern void Photo2ndTsk(void);
extern void Photo3rdTsk(void);
extern void ImgSliceTsk(void);
extern void SieCapTsk(void);
extern void PhotoDisplayTsk(void);
extern void LensCtrlTsk(void);
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
extern void PhotoDisTsk(void);
//#NT#2009/11/27#Connie Yu -end

TASKINFO    IQTaskAPPCfg[] =
{
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{PHOTO1STTSK_ID,       PRI_1STPHOTO,       STKSIZE_1STPHOTO,       Photo1stTsk,        pucStk1stPhoto},        "Photo1st"},
    {{PHOTO2NDTSK_ID,       PRI_2NDPHOTO,       STKSIZE_2NDPHOTO,       Photo2ndTsk,        pucStk2ndPhoto},        "Photo2nd"},
    {{PHOTO3RDTSK_ID,       PRI_3RDPHOTO,       STKSIZE_3RDPHOTO,       Photo3rdTsk,        pucStk3rdPhoto},        "Photo3rd"},
    {{IMGSLICETSK_ID,       PRI_IMGSLICE,       STKSIZE_IMGSLICETSK,    ImgSliceTsk,        pucStkImgSlice},        "ImgSlice"},
    {{SIECAPTSK_ID,         PRI_SIECAP,         STKSIZE_SIECAPTSK,      SieCapTsk,          pucStkSieCap},          "SieCap"},
    {{PHOTODISPLAYTSK_ID,   PRI_DISPLAYPHOTO,   STKSIZE_DISPLAYPHOTO,   PhotoDisplayTsk,    pucStkDisplayPhoto},    "PhotoDisp"},
    {{LENSCTRLTSK_ID,       PRI_LENSCTRL,       STKSIZE_LENSCTRL,       LensCtrlTsk,        pucStkLensCtrlTask},    "LensCtrl"},
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    {{PHOTODISTSK_ID,       PRI_DISPHOTO,       STKSIZE_DISPHOTO,       PhotoDisTsk,        pucStkDisPhoto},        "PhotoDis"},
    //#NT#2009/11/27#Connie Yu -end
};
#endif

// USB
#if (_USBMSDC_MODE_ == ENABLE)
extern void USBMSDC_StrgTsk(void);
#endif

#if (_USBPCC_MODE_ == ENABLE)
extern void USBPccVideoTsk(void);
#endif

#if (_USBSICD_MODE_ == ENABLE)
extern void UsicdTsk(void);
extern void UsicdEP3Tsk(void);
#endif

#if ((_USBMSDC_MODE_ == ENABLE) || \
     (_USBSICD_MODE_ == ENABLE) || \
     (_USBPCC_MODE_ == ENABLE))
extern void USBCtlStartTsk(void);
extern void USBSetupHalTsk(void);

TASKINFO    USBSubsystemCfg[] =
{
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{USBCTRLTSK_ID,        PRI_USBCTRL,        STKSIZE_USBCTRL,        USBCtlStartTsk,     pucStkUsbCtrl},         "USBCtlStart"},
    {{USBSETUPTSK_ID,       PRI_USBSETUP,       STKSIZE_USBSETUP,       USBSetupHalTsk,     pucStkUsbSetup},        "USBSetupHal"},
    {{USBSTRGTSK_ID,        PRI_USBSTRG,        STKSIZE_USBSTRGVND,     USBMSDC_StrgTsk,    pucStkUsbStrgVnd},      "USBMSDC_Strg"},
    {{USBPCCVIDEOTSK_ID,    PRI_USBPCCVIDEO,    STKSIZE_USBPCCAMVIDEO,  USBPccVideoTsk,     pucStkUsbPcCamVideo},   "USBPccVideo"},
    {{USBSICDTSK_ID,        PRI_USBSICD,        STKSIZE_USBSICDVND,     UsicdTsk,           pucStkUsbSICDVnd},      "USBSICD"},
    {{USBSICDEP3TSK_ID,     PRI_USBSICDEP3,     STKSIZE_USBSICDEP3,     UsicdEP3Tsk,        pucStkUsbSICDEP3},      "USBSICDEP3"},
};
#endif

//Media Framework
extern void MediaRecTsk(void);
extern void MediaPlayTsk(void);
extern void MediaVideoRecTsk(void);
extern void MediaAudioEncTsk(void);
extern void MediaVideoPlayTsk(void);
extern void MediaAudioPlayTsk(void);
extern void MediaRecFSTsk(void);//2010/12/15 Meg Lin
extern void MediaRecCmdTsk(void);//2010/12/15 Meg Lin
extern void MediaAudioAACTestTsk(void);//2012/12/23 Meg Lin
//2010/12/15 Meg Lin
TASKINFO    MediaFrameworkCfg[] =
{
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{MEDIARECTSK_ID,       PRI_MEDIAREC,       STKSIZE_MEDIAREC,       MediaRecTsk,        pucStkMediaRec},        "MediaRec"},
    {{MEDIARECFSTSK_ID,     PRI_MEDIARECFS,     STKSIZE_MEDIARECFS,     MediaRecFSTsk,      pucStkMediaRecFS},      "MediaRecFS"},
    {{MEDIARECCMDTSK_ID,    PRI_MEDIARECCMD,    STKSIZE_MEDIARECCMD,    MediaRecCmdTsk,     pucStkMediaRecCmd},     "MediaRecCmd"},
    {{MEDIAVIDEORECTSK_ID,  PRI_MEDIAVIDEOREC,  STKSIZE_MEDIAVIDEOREC,  MediaVideoRecTsk,   pucStkMediaVideoRec},   "MediaVideoRec"},
    {{MEDIAAUDIOENCTSK_ID,  PRI_MEDIAAUDIOENC,  STKSIZE_MEDIAAUDIOENC,  MediaAudioEncTsk,   pucStkMediaAudioEnc},   "MediaAudioEnc"},//2010/04/26 Meg Lin
    {{MEDIAPLAYTSK_ID,      PRI_MEDIAPLAY,      STKSIZE_MEDIAPLAY,      MediaPlayTsk,       pucStkMediaPlay},       "MediaPlay"},
    {{MEDIAVIDEOPLAYTSK_ID, PRI_MEDIAVIDEOPLAY, STKSIZE_MEDIAVIDEOPLAY, MediaVideoPlayTsk,  pucStkMediaVideoPlay},  "MediaVideoPlay"},
    {{MEDIAAUDIOPLAYTSK_ID, PRI_MEDIAAUDIOPLAY, STKSIZE_MEDIAAUDIOPLAY, MediaAudioPlayTsk,  pucStkMediaAudioPlay},  "MediaAudioPlay"},
    {{MEDIARECAACTESTTSK_ID,  PRI_MEDIAAUDIOENC,  STKSIZE_MEDIAAUDIOENC,  MediaAudioAACTestTsk,   pucStkMediaAudioEnc},   "MediaAudioAACTestTsk"},//2012/12/23 Meg Lin

};

// Picture In Movie
#if (_MOVIE_PIM_MODE_ == ENABLE)
extern void RawEncTsk(void);

TASKINFO    RawEncCfg =
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{RAWENC_ID,            PRI_RAWENC,         STKSIZE_RAWENC,         RawEncTsk,          pucStkRawEnc},          "RawEnc"};
#endif

// MP3
#if (_MP3_MODE_ == ENABLE)
extern void MP3PlayTsk(void);
extern void MP3PlayFileReadTsk(void);

TASKINFO    MP3AppCfg[] =
{
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{MP3PLAYTSK_ID,        PRI_MP3PLAY,        STKSIZE_MP3PLAY,        MP3PlayTsk,         pucStkMP3Play},         "MP3Play"},
    {{MP3PLAYFILETSK_ID,    PRI_MP3PLAYFILE,    STKSIZE_MP3PLAYFILESYS, MP3PlayFileReadTsk, pucStkMP3PlayFileRead}, "MP3PlayFileRead"},
};
#endif

// WAV
#if (_WAVSTUDIO_MODE_ == ENABLE)
extern void WavStudioTsk(void);

TASKINFO    WavAppCfg =
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{WAVSTUDIOTSK_ID,      PRI_WAVSTUDIO,      STKSIZE_WAVSTUDIO,      WavStudioTsk,       pucStkWavStudio},       "WavStudio"};
#endif

// GPS
#if (_GPS_RECEIVER_ == ENABLE)
extern void GPSRecTsk(void);

TASKINFO    GPSReceiverAppCfg =
//  Task ID                 Priority            Stack size              Function pointer    Stack pointer           Function name
    {{GPSRECEIVE_ID,        PRI_GPSRECEIVE,     STKSIZE_GPSRECEIVE,     GPSRecTsk,          pucStkGPSReceive},      "GPSRecTsk"};
#endif

void KerInitModules(void)
{
    UINT32 i;

    //======================================================================
    // Module Configrations
    //======================================================================
    // Playback
#if (_PLAYBACK_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&PlaybackAPPCfg);
#endif

    // IQ
#if (_IQ_MODE_ == ENABLE)
    for (i=0; i<(sizeof(IQTaskAPPCfg)/sizeof(TASKINFO)); i++)
    {
        OSCommon_CfgModuleTask(&(IQTaskAPPCfg[i]));
    }
#endif

    // USB
#if ((_USBMSDC_MODE_ == ENABLE) || \
     (_USBSICD_MODE_ == ENABLE) || \
     (_USBPCC_MODE_ == ENABLE))
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[0]));
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[1]));
#endif

#if (_USBMSDC_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[2]));
#endif

#if (_USBPCC_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[3]));
#endif

#if (_USBSICD_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[4]));
    OSCommon_CfgModuleTask(&(USBSubsystemCfg[5]));
#endif

    // Media Framework
    for (i=0; i<sizeof(MediaFrameworkCfg)/sizeof(TASKINFO); i++)
    {
        OSCommon_CfgModuleTask(&(MediaFrameworkCfg[i]));
    }

    // Picture In Movie
#if (_MOVIE_PIM_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&(RawEncCfg));
#endif

    // MP3
#if (_MP3_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&(MP3AppCfg[0]));
    OSCommon_CfgModuleTask(&(MP3AppCfg[1]));
#endif

    // WAV
#if (_WAVSTUDIO_MODE_ == ENABLE)
    OSCommon_CfgModuleTask(&(WavAppCfg));
#endif

    // GPS
#if(_GPS_RECEIVER_ == ENABLE)
    OSCommon_CfgModuleTask(&(GPSReceiverAppCfg));
#endif
}

void KerInitUser(void)
{
    UINT32  i;
    ER      er;

    // Define task
    for (i=0; i<USER_TSK_NUM; i++)
    {
        if ((er = icr_tsk((T_CTSK *)&user_systasktabl[i])) != E_OK)
        {
            debug_err(("Define task %ld (tbl[%ld]) error [%ld]\r\n", user_systasktabl[i].tskid, i, er));
        }

        // Set stack bottom (lower address) memory to STACK_CHECK_TAG
        // We can check the value for stack overflow situation
        if (user_systasktabl[i].stksz)
        {
            *(UINT32 *)(user_systasktabl[i].istack) = STACK_CHECK_TAG;
        }
    }

    // Define flag
    for (i=0; i<USER_EVF_NUM; i++)
    {
        if ((er = icr_flg(user_sysevftabl[i].evfid)) != E_OK)
        {
            debug_err(("Define flag %ld (tbl[%ld]) error [%ld]\r\n", user_sysevftabl[i].evfid, i, er));
        }
    }

    // Define semaphore
    for (i=0; i<USER_SEM_NUM; i++)
    {
        if ((er = icr_sem((T_CSEM *)&user_syssemtabl[i])) != E_OK)
        {
            debug_err(("Define semaphore %ld (tbl[%ld]) error [%ld]\r\n", user_syssemtabl[i].semid, i, er));
        }
    }

    // Define mailbox
    for (i=0; i<USER_MBX_NUM; i++)
    {
        if ((er = icr_mbx(user_sysmbxtabl[i].mbxid)) != E_OK)
        {
            debug_err(("Define mailbox %ld (tbl[%ld]) error [%ld]\r\n", user_sysmbxtabl[i].mbxid, i, er));
        }
    }

    // Define fixed memory pool
    for (i=0; i<USER_FPL_NUM; i++)
    {
        if ((er = icr_mpf((T_CMPF *)&user_sysfpltabl[i])) != E_OK)
        {
            debug_err(("Define fixed memory pool %ld (tbl[%ld]) error [%ld]\r\n", user_sysfpltabl[i].mpfid, i, er));
        }
    }
}
