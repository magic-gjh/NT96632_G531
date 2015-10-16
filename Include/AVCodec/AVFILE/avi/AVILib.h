/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AVILib.h
    @ingroup    mIAVAVI

    @brief      AVI utility functions


    @note       Nothing.

*/

#ifndef _AVILIB_H
#define _AVILIB_H

#define AVI_PARSEOK             0
#define AVI_ERRHEADER           (-1)
#define AVI_ERRDECODE           (-2)
#define AVI_ERRREAD             (-3)
#define AVI_ERRBIGFILE          (-4)

typedef struct
{
    UINT32      uiWavGroupID;           // 'RIFF'
    UINT32      uiWavTotalSize;         // audio size + 44 - 8
    UINT32      uiWavRiffType;          // 'WAVE'
    UINT32      uiWavFmtChunkID;        // 'fmt '
    UINT32      uiWavFmtChunkSize;      // 16
    INT16       sWavFormatTag;
    UINT16      usWavChannels;

    UINT32      uiWavSamplesPerSec;
    UINT32      uiWavAvgBytesPerSec;

    UINT16      usWavBlockAlign;
    UINT16      usWavBitsPerSample;
    UINT32      uiWavDatChunkID;
    UINT32      uiWavDatChunkSize;
} WAVEHEAD_INFO, *PWAVEHEAD_INFO;

typedef struct _AVIHEAD_INFO
{
//#NT#2007/04/04#KH Chang -begin
//#Add for keeping AVI file start address
    UINT32  uiFileStartAddr;        // AVI file start address
//#NT#2007/04/04#KH Chang -end
    UINT32  uiFirstVideoOffset;     // AVI first video frame offset
    UINT32  uiFirstAudioOffset;     // AVI first audio frame offset
    UINT32  uiIndexChunkOffset;     // AVI index chunk offset
    UINT32  uiMovListOffset;        // AVI MOVI List offset
    UINT32  uiUserDataOffset;       // AVI JUNK data offset
    UINT32  uiTotalFileSize;        // total file size
    UINT32  uiTotalFrames;          // total frames, video + audio
    UINT32  uiToltalSecs;           // total seconds
    UINT32  uiVidWidth;             // video width
    UINT32  uiVidHeight;            // video height
    UINT32  uiVidScale;             // video scale     (66666)
    UINT32  uiVidRate;              // video rate    (1000000)
    UINT32  uiAVIAudSampleRate;     // audio sample rate
    UINT32  uiAudBytesPerSec;       // audio bytes per second
    UINT32  uiAudWavTotalSamples;   // audio all samples
    UINT8   ucAudBitsPerSample;     // audio bits per sample
    UINT8   ucAudChannels;          // audio channels
    BOOL    bAudioSupport;          // with audio data
    BOOL    bVideoSupport;          // with video data
    //#NT#2009/07/14#Lily Kao -begin
//#NT#2009/09/24#SA4 -begin
    // only for USICD Use
//#NT#2009/09/24#SA4 -end
    UINT32  uiThumbWidth;           // AVI Thumbnail Width
    UINT32  uiThumbHeight;          // AVI Thumbnail Height
    UINT32  uiThumbSize;            // AVI Thumbnail Size
    UINT32  uiThumbOfs;             // thumbnail offset
    UINT32  videoOnlyByteSize;
    //#NT#2009/07/14#Lily Kao -end
} AVIHEAD_INFO, *PAVIHEAD_INFO;

//Functions
//#NT#2009/03/19#Photon Lin -begin
extern INT32 AVIUti_ParseHeader(UINT32 AVIStartAddr, AVIHEAD_INFO *pAviParams, UINT32 ReadAVISize);
//#NT#2009/03/19#Photon Lin -end
#endif

