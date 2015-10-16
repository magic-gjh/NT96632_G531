/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AviPlayManagerTsk.h
    @ingroup    mIAPPAVIMJPEG

    @brief      All functions for AVI playback mode header file

    @note

*/

#ifndef _AVIPLAYAPI_H
#define _AVIPLAYAPI_H

#include "Type.h"
#include "Audio.h"
#include "AppCommon.h"
//#NT#2007/03/09#Corey Ke -begin
//#Enable AVI task to play all files without DCF
#include "FileDB.h"
//#NT#2007/03/09#Corey Ke -end

#define AVIPLAYFORWARD                  0
#define AVIPLAYBACKWARD                 1

#define AVIPLAYSPEED1X                  0
#define AVIPLAYSPEED2X                  1
#define AVIPLAYSPEED4X                  2
//#NT#2007/05/14#KH Chang -begin
//#Add 5X speed up.
#define AVIPLAYSPEED5X                  3
#define AVIPLAYSPEED8X                  4
//#NT#2007/05/14#KH Chang -end

//#NT#2007/02/06#Corey Ke -begin
//#Support for video repeat one or all
#define AVI_PLAY_CURR                   0
#define AVI_PLAY_NEXT                   1
#define AVI_PLAY_PREV                   2
//#NT#2007/02/06#Corey Ke -end

//#NT#2007/03/20#KH Chang -begin
//#AVI playback task can display user selected video frame.
#define AVIPLAY_FIRST_FRAME             0
#define AVIPLAY_LAST_FRAME              0xFFFFFFFF
//#NT#2007/03/20#KH Chang -end

/**
@name Values: EVENT ID
@{
*/
#define AVIPLAY_EVENT_FINISH            1   ///< Finish
#define AVIPLAY_EVENT_ONE_SECOND        2   ///< One second arrives
#define AVIPLAY_EVENT_ONE_VIDEOFRAME    3   ///< Decode one video-frame
//#NT#2007/04/13#Chris Hsu -begin
//#Add start event
#define AVIPLAY_EVENT_START             4   ///< Start AVI playback
//#NT#2007/04/13#Chris Hsu -end

//#NT#2009/03/19#Photon Lin -begin
//#Add INIT event
#define AVIPLAY_EVENT_INIT_PROCESSING   5
#define AVIPLAY_EVENT_INIT_OK           6
//#NT#2009/03/19#Photon Lin -end

/**
@}
*/
//#NT#2007/03/29#KH Chang -begin
//#Unify AVI parser to AVIUtilities
/*
typedef struct _AVIHEAD_DEC_CFG{
    UINT32      TotalFileSize;              // total file size
    UINT32      headerlen;                  // length of header
    UINT16      *inbuf;                     // input buffer start address
    UINT32      video_scale;                // video scale     (66666)
    UINT32      video_rate;                 // video rate    (1000000)
    UINT32      audio_rate;                 // audio rate
    UINT32      audio_bytes_per_sec;        // audio bytes per second
    UINT32      AudioStartAddr;             // audio data (1st 01wb) start addr
    UINT32      VideoStartAddr;             // video data (1st 00dc) start addr
    UINT32      TotalFrames;                // Total frames, video + audio
//    UINT32      IndexTblOffset;             // offset of Index-table from "RIFF"
    UINT8       AudioSupport;               // support Audio format
    UINT8       VideoSupport;               // support Video format
    UINT8       IfIndexTbl;                 // if there is index table
    UINT8       Reserved;                   // reserved

    UINT16      audio_bits_per_sample;      // audio bits per sample
    UINT16      audio_channels;             // audio channels
    UINT32      audio_wav_TotalSamples;     // audio all samples
    UINT32      ToltalSecs;                 // total seconds. minzon 040915 add
} AVIHEAD_DEC_CFG;
*/
//#NT#2007/03/29#KH Chang -end
/**
    @brief AVI play open parameters

    Use this struct to set parameters for AVIPlay_Open()
*/
typedef struct _AVIPLAY_OBJ {
    UINT32      uiMemAddr;                          /** Data buffer starting address*/
    UINT32      uiMemSize;                          /** Data buffer size*/
    void        (*CallBackFunc)(UINT32 event_id);   /** Callback function */
    BOOL        bHWAudio;                          /** Hardware audio support : TRUE or FALSE */
    BOOL        bHWVideo;                          /** Hardware video support : TRUE or FALSE */
    /**
        Initial audio volume (Only effective when bHWAudio == TRUE)

        @note If not assigned, the volume is unpredictable.
    */
    AUDIO_VOL   initAudVol;
//#NT#2007/02/06#Corey Ke -begin
//#Support for video repeat one or all
    UINT8       uiPlayCommand;                       /** AVI_PLAY_CURR / AVI_PLAY_NEXT / AVI_PLAY_PREV */
//#NT#2007/02/06#Corey Ke -end
//#NT#2007/03/09#Corey Ke -begin
//#Enable AVI task to play all files without DCF
    BOOL        bPlayAllFiles;
    FILEDB_HANDLE AviFileHandle;
//#NT#2007/03/09#Corey Ke -end
//#NT#2007/05/14#KH Chang -begin
//#AVI playback with Background
    BOOL        bPlayWithBG;
//#NT#2007/05/14#KH Chang -end
} AVIPLAY_OBJ;

/** Pointer of AVIPLAY_OBJ */
typedef AVIPLAY_OBJ* PAVIPLAY_OBJ;

//---- Functions relesse -------------------------------
extern INT32            AVIPlay_Open(PAVIPLAY_OBJ    pAVIPlayObj);
extern INT32            AVIPlay_Close(void);
extern void             AVIPlayCopyDataTo(UINT32, UINT32, UINT32);
//#NT#2007/03/29#KH Chang -begin
//#Unify AVI parser to AVIUtilities
//extern AVIHEAD_DEC_CFG* AVIPlay_GetHeaderInfo(UINT32 inbuf);
//#NT#2007/03/29#KH Chang -end
extern void             AVIPlay_Stop(void);
extern void             AVIPlay_Pause(void);
extern void             AVIPlay_FWD(void);
extern void             AVIPlay_BWD(void);
extern void             AVIPlay_SpeedUp(UINT32 ucSpeedUpLevel);
extern void             AVIPlay_SetAudVolUp(void);
extern void             AVIPlay_SetAudVolDown(void);
extern void             AVIPlay_SetAudVol(AUDIO_VOL level);
extern void             AVIPlay_FWDStep(void);
extern void             AVIPlay_BWDStep(void);
//------------------------------------
extern AUDIO_VOL        AVIPlay_GetAudVol(void);
extern UINT32           AVIPlay_GetCurrPlaySecs(void);
extern UINT32           AVIPlay_GetTotalSecs(void);
extern UINT32           AVIPlay_GetSpeedLevel(void);
extern UINT32           AVIPlay_GetCurrPlayDir(void);

extern UINT32           AVIPlay_GetCurrTotalVideoFrame(void);
extern UINT32           AVIPlay_GetCurrCurrVideoFrame(void);

extern void             AVIPlay_SetIMEIDESize(PIMEIDE_ATTR pIMEIDEAttr);
//#NT#2007/03/20#KH Chang -begin
//#AVI playback can jump to any frame index when paused.
extern void             AVIPlay_SwitchFrame(UINT32 findex);
//#NT#2007/03/20#KH Chang -end

//#NT#2007/05/14#KH Chang -begin
//#AVI playback with Background
extern void AVIPLAY_Set1stVideoFrame(UINT16 uiDstLU_X, UINT16 uiDstLU_Y, UINT16 uiDstSZ_W, UINT16 uiDstSZ_H);
//#NT#2007/05/14#KH Chang -end

//#NT#2008/09/01#Corey Ke -begin
//#Update for TV plug in while AVI is playing
extern BOOL             AVIPLAY_GetAVIOpened(void);
//#NT#2008/09/01#Corey Ke -end
//#NT#2009/03/19#Photon Lin -begin
//External function declaration
extern void AVIPLAY_SetCommandAccess(BOOL bEn);
extern BOOL AVIPLAY_CheckCommandAccess(void);
//#NT#2009/03/19#Photon Lin -end
//---- data relesse ------------------------------------

//------------------------------------------------------

/**
@}
*/

#endif // _AVIPLAYAPI_H
