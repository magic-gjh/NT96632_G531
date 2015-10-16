/*
    NT96630 embedded audio codec internal header file

    @file       audcodec630_int.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _AUDCODEC630_INT_H
#define _AUDCODEC630_INT_H

#include "Audio.h"
#include "Type.h"

/**
   @  addtogroup mISYSAud
*/
//@{

void    audcodec_setDevObj(PAUDIO_DEVICE_OBJ pAudDevObj);
void    audcodec_init(PAUDIO_SETTING pAudio);
void    audcodec_setRecordSource(AUDIO_RECSRC RecSrc);
void    audcodec_setOutput(AUDIO_OUTPUT Output);
void    audcodec_setSamplingRate(AUDIO_SR SamplingRate);
void    audcodec_setChannel(AUDIO_CH Channel);
void    audcodec_setVolume(AUDIO_VOL Vol);
//#NT#SA4   -begin
// add Get volume info
void    audcodec_getVolume(AUDIO_VOL *Vol);
//#NT#SA4   -end
void    audcodec_setGain(AUDIO_GAIN Gain);
void    audcodec_setEffect(AUDIO_EFFECT Effect);
void    audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);
void    audcodec_stop(void);
void    audcodec_record(void);
void    audcodec_playback(void);
void    audcodec_setFormat(AUDIO_I2SFMT I2SFmt);
void    audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio);
BOOL    audcodec_sendCommand(UINT32 uiReg, UINT32 uiData);
BOOL    audcodec_readData(UINT32 uiReg, UINT32 *puiData);
void 		audcodec_setMixer(AUDIO_MIXER MixerType);

//@}

#endif
