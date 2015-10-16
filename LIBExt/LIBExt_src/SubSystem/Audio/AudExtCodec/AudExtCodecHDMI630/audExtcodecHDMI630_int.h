/*
    NT96630 embedded HDMI extended audio codec internal header file

    NT96630 embedded HDMI extended audio codec internal header file

    @file       audExtcodecHDMI630_int.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _AUDEXTCODECHDMI630_INT_H
#define _AUDEXTCODECHDMI630_INT_H

#include "Audio.h"
#include "AudioCodec.h"
#include "Type.h"

/**
    @addtogroup mISYSAud
*/
//@{

void    audcodecHDMI_setDevObj(PAUDIO_DEVICE_OBJ pAudDevObj);
void    audcodecHDMI_init(PAUDIO_SETTING pAudio);
void    audcodecHDMI_setRecordSource(AUDIO_RECSRC RecSrc);
void    audcodecHDMI_setOutput(AUDIO_OUTPUT Output);
void    audcodecHDMI_setSamplingRate(AUDIO_SR SamplingRate);
void    audcodecHDMI_setChannel(AUDIO_CH Channel);
void    audcodecHDMI_setVolume(AUDIO_VOL Vol);
void    audcodecHDMI_setGain(AUDIO_GAIN Gain);
void    audcodecHDMI_setEffect(AUDIO_EFFECT Effect);
void    audcodecHDMI_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);
void    audcodecHDMI_stop(void);
void    audcodecHDMI_record(void);
void    audcodecHDMI_playback(void);
void    audcodecHDMI_setFormat(AUDIO_I2SFMT I2SFmt);
void    audcodecHDMI_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio);
BOOL    audcodecHDMI_sendCommand(UINT32 uiReg, UINT32 uiData);
BOOL    audcodecHDMI_readData(UINT32 uiReg, UINT32 *puiData);
void    audcodecHDMI_setMixer(AUDIO_MIXER MixerType);

//@}

#endif
