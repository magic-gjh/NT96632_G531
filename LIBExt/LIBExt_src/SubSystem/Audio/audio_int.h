/*
    Audio internal header file

    This file is the header file that define register for Audio module

    @file       audio_int.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _AUDIO_INT_H
#define _AUDIO_INT_H

#include "Audio.h"

/**
    @addtogroup mISYSAud
*/
//@{

#define AUDIO_FIFO_DEPTH                0x00000008
#define AUDIO_TXTHOD_VALUE              0x00000007
#define AUDIO_RXTHOD_VALUE              0x00000007

#define AUDIO_BUF_BLKNUM                0x00000008//2011/01/24 Meg Lin
#define AUDIO_MAX_TC_TRIGGER            0xFFFFFFFF

// Support 1 default codec and 1 extended codec
//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
#define AUDIO_MAX_CODEC_NUM             2
//#NT#2010/01/29#Chris Hsu -end

//#NT#2010/04/20#Chris Hsu[0010194] -begin
//#NT#Fix recording muted audio when HDMI is pluged in
#define AUDIO_CODEC_FUNC_USE_DEFAULT    0xE0000000
//#NT#2010/04/20#Chris Hsu[0010194] -end

typedef struct
{
    UINT32  uiADCSampDiv;
    BOOL    bADCSeq;
    BOOL    bADCEn;
} AUDIO_ADC_STATUS;

//#NT#2009/12/22#Cliff Lin -begin
//#NT#Add for Audio operation mode
// Audio mode
typedef enum
{
    AUDIO_TX,              ///< At playback mode
    AUDIO_RX,              ///< At record   mode
    ENUM_DUMMY4WORD(AUDIO_MODE)
} AUDIO_MODE;
//#NT#2009/12/22#Cliff Lin -end

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
/*
typedef struct
{
    void    (*init)(PAUDIO_SETTING pAudio);
    void    (*setRecordSource)(AUDIO_RECSRC RecSrc);
    void    (*setOutput)(AUDIO_OUTPUT Output);
    void    (*setSamplingRate)(AUDIO_SR SamplingRate);
    void    (*setChannel)(AUDIO_CH Channel);
    void    (*setVolume)(AUDIO_VOL Vol);
//#NT#SA4   -begin
// add Get volume info
    void    (*getVolume)(AUDIO_VOL *Vol);
//#NT#SA4   -end
    void    (*setGain)(AUDIO_GAIN Gain);
    void    (*setEffect)(AUDIO_EFFECT Effect);
    void    (*setFeature)(AUDIO_FEATURE Feature, BOOL bEnable);
    void    (*stop)(void);
    void    (*record)(void);
    void    (*playback)(void);
    void    (*setFormat)(AUDIO_I2SFMT I2SFmt);
    void    (*setClockRatio)(AUDIO_I2SCLKR I2SCLKRatio);
    BOOL    (*sendCommand)(UINT32 uiReg, UINT32 uiData);
    BOOL    (*readData)(UINT32 uiReg, UINT32 *puiData);
    void    (*setMixer)(AUDIO_MIXER MixerType);
} AUDIO_CODEC_FUNC, *PAUDIO_CODEC_FUNC;
*/

/*
// APIs in audio codec
extern void audcodec_setFP(PAUDIO_CODEC_FUNC pAudCodecFunc);
extern void audcodec_setDeviceObject(PAUDIO_DEVICE_OBJ pAudDevObj);
*/

/*
// APIs for Audio.c internal usage
extern void aud_resample(UINT32 uiResample_InAddr, UINT32 uiResample_InSize, UINT32* uiResample_OutAddr, UINT32* uiResample_OutSize);
*/
//#NT#2010/01/29#Chris Hsu -end

//@}

#endif
