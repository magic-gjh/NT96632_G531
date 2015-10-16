/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       AviRecApi.h
    @ingroup    mIAPPAVIMJPEG

    @brief      Header file of AVI MJPEG Encode Application. External use.

    @note

*/

#ifndef _AVIRECAPI_H
#define _AVIRECAPI_H

#include "FilesysTsk.h"
#include "Audio.h"

/**
    @addtogroup mIAPPAVIMJPEG
@{
*/

/**
@name AviRec_Close parameters
@{
*/
#define     AVIREC_WAIT_END     1
#define     AVIREC_NO_WAIT      0
/**
@}
*/

/**
@name Values: AVI_REC_OBJ.uiRecFormat
@{
*/
/** @brief Video only */
#define AVIREC_VIDEO_ONLY       0x0001
/** @brief Audio+Video */
#define AVIREC_AUD_VID_BOTH     0x0002
/* @brief Audio only (Output is WAV file) */
//#define AVIREC_AUDIO_ONLY       0x0003
/**
@}
*/


//#NT#2009/03/19#Photon Lin -begin
//#Modify quality setting
//#NT#2009/04/20#Photon Lin -begin
//#Add new algorithm for rate control
/**
@name Values: AVI_REC_VIDEO_OBJ.uiRecQuality
@{
*/
/** @brief Video quality best */
#define AVIREC_QUALITY_BEST       1280//80
/** @brief Video quality normal */
#define AVIREC_QUALITY_NORMAL     2560//60
/** @brief Video quality draft */
#define AVIREC_QUALITY_DRAFT      4000//40
/**
@}
*/

//Each quality modification in rate control is 3
//#define AVI_Q_STEP                  3
// BPS tolerance, 30KB
//#define AVI_AVG_BPS_TOLERANCE       0x7800
//#NT#2009/04/20#Photon Lin -end
//Buffer control mechanism
#define AVIREC_QUEUE_ENABLE     0
#define AVIREC_QUEUE_THR_LV1    3
#define AVIREC_QUEUE_THR_LV2    6
#define AVIREC_QUEUE_THR_LV3    9
#define AVIREC_QUEUE_RATE_STEP    0x19000
//#NT#2009/03/19#Photon Lin -end
/**
@name Values: AVI_REC_VIDEO_OBJ.uiJpegFormat
@{
*/
/** @brief Jpeg compression format 444 */
#define AVIREC_JPEG_FORMAT_444      1
/** @brief Jpeg compression format 422 */
#define AVIREC_JPEG_FORMAT_422      2
/** @brief Jpeg compression format 420 */
#define AVIREC_JPEG_FORMAT_420      3
/**
@}
*/

/**
@name Values: Return of AVIRec_GetStatus()
@{
*/
/** @brief Avi record module has not been opened yet */
#define AVIREC_STATUS_NOT_OPENED            1
/** @brief Avi record module is opened but not recording */
#define AVIREC_STATUS_OPENED_NOT_RECORD     2
/** @brief Avi record module is recording (of course opened) */
#define AVIREC_STATUS_RECORDING             3
//#NT#2008/08/06#JR Huang -begin
//#arrange AVI play and recording flow
//#add stopping staus
#define AVIREC_STATUS_STOPPING              4
//#NT#2008/08/06#JR Huang -end
/**
@}
*/


/**
@name Values: AVI_REC_VIDEO_OBJ.uiRecQuality
@{
*/
/** @brief Avi record save file duration */

#define AVIREC_SAVEFILE_ONE_SECOND            1
#define AVIREC_SAVEFILE_HALF_SECOND           2
/**
@}
*/


/**
@name Values: EVENT ID
*/
//   below define AVI record EVENT ID
#define     AVIREC_EVENT_RESULT_NORMAL        1    ///< Finish normally
#define     AVIREC_EVENT_RESULT_HW_ERR        2    ///< Hardware error
#define     AVIREC_EVENT_RESULT_FULL          3    ///< Card full
#define     AVIREC_EVENT_RESULT_SLOW          4    ///< Write card too slowly
#define     AVIREC_EVENT_RESULT_NOTREADY      5    ///< Not ready
#define     AVIREC_EVENT_ONE_SECOND           6    ///< One second arrives
#define     AVIREC_EVENT_RESULT_OVERTIME      7    ///< Overtime
#define     AVIREC_EVENT_RESULT_OVERWRITE     8    ///< Buffer overwrite
#define     AVIREC_EVENT_RESULT_INVALIDFILE   9    ///< SAVE INVALIDFILE
#define     AVIREC_EVENT_RESULT_WRITE_ERR    10
/**
@}
*/

/**
    @brief AVI record callback function prototype
*/
typedef void (AviRecCallbackType)(UINT32 event_id, UINT32 Seconds);

/**
    @brief AVI record open parameters

    Use this struct to set parameters for AVIRec_Open()
*/
typedef struct _AVIREC_OBJ
{
    /** Memory buffer starting address */
    UINT32              uiMemAddr;
    /** Memory buffer size in bytes */
    UINT32              uiMemSize;
    /** Callback function poiter    */
    AviRecCallbackType  *CallBackFunc;
    /** @brief Output format

        AVIREC_AUD_VID_BOTH \n
        AVIREC_AUD_VIDEO_ONLY \n
    */
    UINT                uiRecFormat;
    /**
        @brief Jpeg compression format

        AVIREC_JPEG_FORMAT_444\n
        AVIREC_JPEG_FORMAT_422\n
        AVIREC_JPEG_FORMAT_420
    */
    UINT                uiJpegFormat;
    /** @brief Frame rate (frame/second) */
    UINT                    uiVidFrameRate;
    /** @brief Image Quality

        AVIREC_QUALITY_BEST\n
        AVIREC_QUALITY_NORMAL\n
        AVIREC_QUALITY_DRAFT
    */
    UINT                    uiRecQuality;
    /** @brief Video width */
    UINT                    uiWidth;
    /** @brief Video height */
    UINT                    uiHeight;

    /** @brief Audio sample rate (HZ) (Usually 8000 or 16000) */
    UINT                    uiAudSampleRate;
    /** @brief Audio sample bits (Usually 8 or 16) */
    UINT                    uiAudBits;
    /** @brief Audio record channel (Usually 1) */
    UINT                    uiAudChannels;
    UINT32                  uiAudioBufferAddr;
    UINT32                  uiSavefileduration;
    UINT32                  uiMaxRecSecs;
    UINT32                  uiTargetBitrate;
    UINT32                  uiRateCtrlSteps;
    UINT32                  uiRateCtrlStepSize;
    UINT32                  uiMaxRecFileSize;
    // Setup bUserDataEmbed = 1 to enable userdata embeded flow.
    /** @brief display width (for embeded thumbnail) */
    UINT                    uiDisplayWidth;
    /** @brief display height (for embeded thumbnail) */
    UINT                    uiDisplayHeight;
    /** @brief thumb width (for embeded thumbnail) */
    UINT                    uiThumbWidth;
    /** @brief thumb height (for embeded thumbnail) */
    UINT                    uiThumbHeight;
    /** @brief User data embed */
    BOOL                    bUserDataEmbed;   // 1: Userdata embeded  0: non_userdata
    BOOL                    bEnableBRCtrl;    // 1: Enable BitRate control
} AVIREC_OBJ;

/** pointer of AVIREC_OBJ */
typedef AVIREC_OBJ *PAVIREC_OBJ;

/**
    @brief AVI record video parameters

    Use this struct to set parameters for AVIRec_SetVideo()
*/
typedef struct _AVIREC_VIDEO_OBJ
{
    /** @brief Frame rate (frame/second) */
    UINT                    uiVidFrameRate;
    /** @brief Image Quality

        AVIREC_QUALITY_BEST\n
        AVIREC_QUALITY_NORMAL\n
        AVIREC_QUALITY_DRAFT
    */
    UINT                    uiRecQuality;
    /** @brief Video width */
    UINT                    uiWidth;
    /** @brief Video height */
    UINT                    uiHeight;
    UINT32                  uiTargetBitrate;
    /**
        @brief Jpeg compression format

        AVIREC_JPEG_FORMAT_444\n
        AVIREC_JPEG_FORMAT_422\n
        AVIREC_JPEG_FORMAT_420
    */
    UINT                uiJpegFormat;
} AVIREC_VIDEO_OBJ;

/** pointer of AVIREC_VIDEO_OBJ */
typedef AVIREC_VIDEO_OBJ *PAVIREC_VIDEO_OBJ;

//
// expose function declare
//
extern ER       AVIRec_Open(AVIREC_OBJ *param);
extern void     AVIRec_GetVideo(AVIREC_VIDEO_OBJ *param);
extern void     AVIRec_SetVideo(AVIREC_VIDEO_OBJ *param);
extern void     AVIRec_RegisterCB(AviRecCallbackType *CallBackFunc);
extern ER       AVIRec_Record(void);
extern ER       AVIRec_Stop(BOOL bWaitComplete);
extern ER       AVIRec_Close(UINT32 uiTimeOut);
extern UINT32   AVIRec_Disk2Second(void);
extern UINT32   AVIRec_GetStatus(void);
//#NT#2009/01/16#Photon Lin -begin
//#Add Pause/Resume mechanism
extern void     AVIRec_Pause(void);
extern void     AVIRec_Resume(void);
//#NT#2009/01/16#Photon Lin -end

extern void     AVIRec_SetQuickViewEnable(BOOL en);
extern UINT32   AVIRec_GetQuickViewMemNeedSize(void);
extern void     AVIRec_SetQuickViewMemAddr(UINT32 addr);
extern UINT32   AVIRec_GetQuickViewMemAddr(void);
extern UINT     AVIRec_GetQuickViewFrameSize(void);

extern UINT32   AVIRec_SetUserData(UINT8 *pUserData, UINT32 datalen);

extern void AVIRec_SetThumbFrameSrc(UINT32 Addr);
//#NT#2009/04/01#Photon Lin -begin
//#Add API for UI usage
extern UINT32 AVIRec_GetValidFileSize(void);
//#NT#2009/04/01#Photon Lin -end

/**
@}
*/

#endif //_AVIRECAPI_H
