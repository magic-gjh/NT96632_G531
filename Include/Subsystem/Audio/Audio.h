/**
    Header file for Audio module

    This file is the header file that define the API and data type
    for Audio module.

    @file       Audio.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _AUDIO_H
#define _AUDIO_H

#include "Type.h"

/**
    @addtogroup mISYSAud
*/
//@{

/**
    @name   Audio event

    Definition for audio event
*/
//@{
#define AUDIO_EVENT_TCLATCH             0x00000001      ///< Time code latch event
#define AUDIO_EVENT_TCHIT               0x00000002      ///< Time code hit event
#define AUDIO_EVENT_DMADONE             0x00000004      ///< DMA finish transfering one buffer event
#define AUDIO_EVENT_BUF_FULL            0x00000008      ///< Buffer queue full event
#define AUDIO_EVENT_BUF_EMPTY           0x00000010      ///< Buffer queue empty event
#define AUDIO_EVENT_DONEBUF_FULL        0x00000020      ///< Done buffer queue full event
#define AUDIO_EVENT_FIFO_ERROR          0x00000100      ///< FIFO Error
//@}

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
/**
    Audio codec selection
*/
typedef enum
{
    AUDIO_CODECSEL_DEFAULT,     ///< Default audio codec (By linking library)
    AUDIO_CODECSEL_EXT_0,       ///< Extended audio codec 0
    ENUM_DUMMY4WORD(AUDIO_CODECSEL)
} AUDIO_CODECSEL;
//#NT#2010/01/29#Chris Hsu -end

/**
    Audio format
*/
typedef enum
{
    AUDIO_FMT_I2S,              ///< I2S
    AUDIO_FMT_AC97,             ///< AC97
    ENUM_DUMMY4WORD(AUDIO_FMT)
} AUDIO_FMT;

/**
    Audio sampling rate
*/
typedef enum
{
    AUDIO_SR_8000   = 8000,     ///< 8 KHz
    AUDIO_SR_11025  = 11025,    ///< 11.025 KHz
    AUDIO_SR_12000  = 12000,    ///< 12 KHz
    AUDIO_SR_16000  = 16000,    ///< 16 KHz
    AUDIO_SR_22050  = 22050,    ///< 22.05 KHz
    AUDIO_SR_24000  = 24000,    ///< 24 KHz
    AUDIO_SR_32000  = 32000,    ///< 32 KHz
    AUDIO_SR_44100  = 44100,    ///< 44.1 KHz
    AUDIO_SR_48000  = 48000,    ///< 48 KHz
    ENUM_DUMMY4WORD(AUDIO_SR)
} AUDIO_SR;

/**
    Audio channel
*/
typedef enum
{
    AUDIO_CH_LEFT,              ///< Left
    AUDIO_CH_RIGHT,             ///< Right
    AUDIO_CH_STEREO,            ///< Stereo
    AUDIO_CH_MONO,              ///< Mono two channel
    ENUM_DUMMY4WORD(AUDIO_CH)
} AUDIO_CH;

/**
    Audio volume (playback)
*/
typedef enum
{
    AUDIO_VOL_MUTE,             ///< Volume mute
    AUDIO_VOL_0,                ///< Volume 0
    AUDIO_VOL_1,                ///< Volume 1
    AUDIO_VOL_2,                ///< Volume 2
    AUDIO_VOL_3,                ///< Volume 3
    AUDIO_VOL_4,                ///< Volume 4
    AUDIO_VOL_5,                ///< Volume 5
    AUDIO_VOL_6,                ///< Volume 6
    AUDIO_VOL_7,                ///< Volume 7
    AUDIO_VOL_UNMUTE,           ///< Volume unmute
    ENUM_DUMMY4WORD(AUDIO_VOL)
} AUDIO_VOL;

/**
    Audio gain (record)
*/
typedef enum
{
    AUDIO_GAIN_MUTE,            ///< Gain mute
    AUDIO_GAIN_0,               ///< Gain 0
    AUDIO_GAIN_1,               ///< Gain 1
    AUDIO_GAIN_2,               ///< Gain 2
    AUDIO_GAIN_3,               ///< Gain 3
    AUDIO_GAIN_4,               ///< Gain 4
    AUDIO_GAIN_5,               ///< Gain 5
    AUDIO_GAIN_6,               ///< Gain 6
    AUDIO_GAIN_7,               ///< Gain 7
    ENUM_DUMMY4WORD(AUDIO_GAIN)
} AUDIO_GAIN;

/**
    Audio record source (input path)
*/
typedef enum
{
    AUDIO_RECSRC_MIC,           ///< Microphone
    AUDIO_RECSRC_CD,            ///< CD In
    AUDIO_RECSRC_VIDEO,         ///< Video In
    AUDIO_RECSRC_AUX,           ///< Aux In
    AUDIO_RECSRC_LINE,          ///< Line In
    AUDIO_RECSRC_STEREO_MIX,    ///< Stereo Mix
    AUDIO_RECSRC_MONO_MIX,      ///< Mono Mix
    AUDIO_RECSRC_PHONE,         ///< Phone In
    ENUM_DUMMY4WORD(AUDIO_RECSRC)
} AUDIO_RECSRC;

/**
    Audio output path
*/
typedef enum
{
    AUDIO_OUTPUT_SPK,           ///< Speaker
    AUDIO_OUTPUT_HP,            ///< Headphone
    AUDIO_OUTPUT_LINE,          ///< Line Out
    AUDIO_OUTPUT_MONO,          ///< Mono Out
    AUDIO_OUTPUT_NONE,          ///< No output path
    ENUM_DUMMY4WORD(AUDIO_OUTPUT)
} AUDIO_OUTPUT;

/**
    Audio effect
*/
typedef enum
{
    AUDIO_EFFECT_NONE,          ///< No special audio effect
    AUDIO_EFFECT_3D_HALF,       ///< 50% 3D effect
    AUDIO_EFFECT_3D_FULL,       ///< 100% 3D effect
    AUDIO_EFFECT_ROCK,          ///< Rock
    AUDIO_EFFECT_POP,           ///< POP
    AUDIO_EFFECT_JAZZ,          ///< Jazz
    AUDIO_EFFECT_CLASSICAL,     ///< Classical
    AUDIO_EFFECT_DNACE,         ///< Dance
    AUDIO_EFFECT_HEAVY,         ///< Heavy
    AUDIO_EFFECT_DISCO,         ///< Disco
    AUDIO_EFFECT_SOFT,          ///< Soft
    AUDIO_EFFECT_LIVE,          ///< Live
    AUDIO_EFFECT_HALL,          ///< Hall
    ENUM_DUMMY4WORD(AUDIO_EFFECT)
} AUDIO_EFFECT;

/**
    Audio output mixer type
*/
typedef enum
{
    AUDIO_MIXER_NORMAL,          ///< Normal Mixer : R=R, L=L
    AUDIO_MIXER_RLAVG,           ///< Average Mixer : R=(R+L)/2, L=(R+L)/2
    ENUM_DUMMY4WORD(AUDIO_MIXER)
} AUDIO_MIXER;

/**
    I2S clock ratio
*/
typedef enum
{
    AUDIO_I2SCLKR_256FS_32BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_256FS_64BIT,  ///< SystemClk = 256 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_384FS_32BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_384FS_48BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 48 BitClk
    AUDIO_I2SCLKR_384FS_96BIT,  ///< SystemClk = 384 * FrameSync, FrameSync = 96 BitClk
    AUDIO_I2SCLKR_512FS_32BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_512FS_64BIT,  ///< SystemClk = 512 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_512FS_128BIT, ///< SystemClk = 512 * FrameSync, FrameSync = 128 BitClk
    AUDIO_I2SCLKR_768FS_32BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_768FS_48BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 48 BitClk
    AUDIO_I2SCLKR_768FS_64BIT,  ///< SystemClk = 768 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_768FS_192BIT, ///< SystemClk = 768 * FrameSync, FrameSync = 192 BitClk
    AUDIO_I2SCLKR_1024FS_32BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 32 BitClk
    AUDIO_I2SCLKR_1024FS_64BIT, ///< SystemClk = 1024 * FrameSync, FrameSync = 64 BitClk
    AUDIO_I2SCLKR_1024FS_128BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 128 BitClk
    AUDIO_I2SCLKR_1024FS_256BIT,///< SystemClk = 1024 * FrameSync, FrameSync = 256 BitClk
    ENUM_DUMMY4WORD(AUDIO_I2SCLKR)
} AUDIO_I2SCLKR;

/**
    Audio clock rate
*/
typedef enum
{
    AUDIO_CLK_24576,            ///< 24.576 MHz
    AUDIO_CLK_12288,            ///< 12.288 MHz
    AUDIO_CLK_8192,             ///< 8.192 MHz
    AUDIO_CLK_6144,             ///< 6.144 MHz
    AUDIO_CLK_4096,             ///< 4.096 MHz
    AUDIO_CLK_3072,             ///< 3.072 MHz
    AUDIO_CLK_2048,             ///< 2.048 MHz
    AUDIO_CLK_11289,            ///< 11.289 MHz
    AUDIO_CLK_5644,             ///< 5.644 MHz
    AUDIO_CLK_2822,             ///< 2.822 MHz
    ENUM_DUMMY4WORD(AUDIO_CLK)
} AUDIO_CLK;

/**
    I2S format
*/
typedef enum
{
    AUDIO_I2SFMT_STANDARD,      ///< I2S Standard
    AUDIO_I2SFMT_LIKE_MSB,      ///< I2S Like, MSB justified
    AUDIO_I2SFMT_LIKE_LSB,      ///< I2S Like, LSB justified
    ENUM_DUMMY4WORD(AUDIO_I2SFMT)
} AUDIO_I2SFMT;

/**
    I2s control interface
*/
typedef enum
{
    AUDIO_I2SCTRL_SIF,          ///< Use SIF to control I2S codec
    AUDIO_I2SCTRL_I2C,          ///< Use I2C to control I2S codec
    AUDIO_I2SCTRL_GPIO_SIF,     ///< Use GPIO to simulate SIF to control I2S codec
    AUDIO_I2SCTRL_GPIO_I2C,     ///< Use GPIO to simulate I2C to control I2S codec
    AUDIO_I2SCTRL_NONE,         ///< Not a I2S codec
    ENUM_DUMMY4WORD(AUDIO_I2SCTRL)
} AUDIO_I2SCTRL;

/**
    Audio codec feature
*/
typedef enum
{
    AUDIO_FEATURE_ALC,                  ///< Auto level control (Recording path)
    AUDIO_FEATURE_MICBOOST,             ///< Microphone boost
    AUDIO_FEATURE_NG_WITH_MICBOOST,     ///< Noise gate function with microphone boost (internal or external)
    AUDIO_FEATURE_PLAYBACK_PCM_EXPAND,  ///< PCM data expand.Audio controller will duplicate mono PCM data(DRAM data) and playback to stereo channel.
    AUDIO_FEATURE_RECORD_PCM_EXPAND,    ///< PCM data expand.Audio controller will record mono channel PCM data and duplicate to DRAM.
    //#NT#2010/02/08#Cliff Lin -begin
    //#NT#Added for support the selected path power always on
    AUDIO_FEATURE_HP_PWR_ALWAYSON,      ///< Head phone power always on
    AUDIO_FEATURE_SPK_PWR_ALWAYSON,     ///< Speaker power always on
    AUDIO_FEATURE_LINE_PWR_ALWAYSON,    ///< LineOut power always on
    AUDIO_FEATURE_OUTPATH_ONLYPWR2ON,   ///< Only allow Output path power to on
    //#NT#2010/02/08#Cliff Lin -end
    ENUM_DUMMY4WORD(AUDIO_FEATURE)
} AUDIO_FEATURE;

/**
    I2S setting
*/
typedef struct
{
    AUDIO_I2SCLKR       ClkRatio;       ///< Clock Ratio
    AUDIO_I2SFMT        I2SFmt;         ///< I2S Standard or I2S Like MSB/LSB
    BOOL                bMaster;        ///< Audio Controller Master(TRUE) Mode or Slave(FALSE) Mode
} AUDIO_I2S_SETTING;

/**
    Audio clock setting
*/
typedef struct
{
    BOOL                bClkExt;        ///< Clock Source, External(TRUE), Internal(FALSE)
    AUDIO_CLK           Clk;            ///< Clock Rate, valid when clock source is internal
} AUDIO_CLK_SETTING;

/**
    Audio setting
*/
typedef struct
{
    AUDIO_CLK_SETTING   Clock;          ///< Audio clock setting
    AUDIO_I2S_SETTING   I2S;            ///< I2S Setting
    AUDIO_FMT           Fmt;            ///< AUDIO_FMT_I2S or AUDIO_FMT_AC97
    AUDIO_SR            SamplingRate;   ///< Sampling Rate
    AUDIO_CH            Channel;        ///< AUDIO_CH_LEFT, AUDIO_CH_RIGHT or AUDIO_CH_STEREO
    AUDIO_RECSRC        RecSrc;         ///< Record Source
    AUDIO_OUTPUT        Output;         ///< Output Path
    BOOL                bEmbedded;      ///< Embedded audio codec or not
} AUDIO_SETTING, *PAUDIO_SETTING;

/**
    Audio buffer queue
*/
typedef struct _AUDIO_BUF_QUEUE
{
    UINT32                  uiAddress;  ///< Buffer Starting Address (Word Alignment)
    UINT32                  uiSize;     ///< Buffer Size (Word Alignment)
    UINT32                  uiValidSize;///< Valid PCM data size (Byte)
    struct _AUDIO_BUF_QUEUE *pNext;     ///< Next queue element
} AUDIO_BUF_QUEUE, *PAUDIO_BUF_QUEUE;

/**
    Audio object
*/
typedef struct
{
    void            (*pEventHandler)(UINT32 EventID);   ///< EventHandler function pointer, set to NULL if you don't want to handle audio event
    UINT32          uiGPIOColdReset;                    ///< GPIO pin number of AC97 Cold Reset pin
    AUDIO_I2SCTRL   uiI2SCtrl;                          ///< I2S Control Interface. Could be one of the following: AUDIO_I2SCTRL_SIF, AUDIO_I2SCTRL_I2C, AUDIO_I2SCTRL_GPIO_SIF, AUDIO_I2SCTRL_GPIO_I2C
    UINT32          uiChannel;                          ///< SIF channel number to control I2S codec, only valid when uiI2SCtrl == AUDIO_I2SCTRL_SIF, ADC channel for ADC recording
    UINT32          uiGPIOData;                         ///< GPIO pin number for SIF DATA / I2C SDA, only valid when uiI2SCtrl == AUDIO_I2SCTRL_GPIO_SIF or AUDIO_I2SCTRL_GPIO_I2C
    UINT32          uiGPIOClk;                          ///< GPIO pin number for SIF CLK / I2C SCL, only valid when uiI2SCtrl == AUDIO_I2SCTRL_GPIO_SIF or AUDIO_I2SCTRL_GPIO_I2C
    UINT32          uiGPIOCS;                           ///< GPIO pin number for SIF CS, only valid when uiI2SCtrl == AUDIO_I2SCTRL_GPIO_SIF
    UINT32          uiADCZero;                          ///< ADC value of PCM data = 0
} AUDIO_DEVICE_OBJ, *PAUDIO_DEVICE_OBJ;

//#NT#2009/11/05#Cliff Lin -begin
//#NT#Add for resampling
/**
    Audio re-sampling information
*/
typedef struct
{
    BOOL            bResample;          ///< Enable/Disable playback re-sampling function
    UINT32          uiDstBufAddr;       ///< buffer address for re-sampling
    UINT32          uiDstBufSize;       ///< buffer size for re-sampling, must be target sampling rate * 4 * 4
    AUDIO_SR        AudioTargetSR;      ///< Target Sampling rate after re-sampling
} AUDIO_RESAMPLE_INFO, *PAUDIO_RESAMPLE_INFO;
//#NT#2009/11/05#Cliff Lin -end

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
typedef struct
{
    void    (*setDevObj)(PAUDIO_DEVICE_OBJ pAudDevObj);
    void    (*init)(PAUDIO_SETTING pAudio);
    void    (*setRecordSource)(AUDIO_RECSRC RecSrc);
    void    (*setOutput)(AUDIO_OUTPUT Output);
    void    (*setSamplingRate)(AUDIO_SR SamplingRate);
    void    (*setChannel)(AUDIO_CH Channel);
    void    (*setVolume)(AUDIO_VOL Vol);
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
//#NT#2010/01/29#Chris Hsu -end

// API
extern ER       aud_open(PAUDIO_DEVICE_OBJ pDevObj);
extern ER       aud_close(void);
extern ID       aud_getLockStatus(void);
extern void     aud_getDeviceObject(PAUDIO_DEVICE_OBJ pDevObj);

extern void     aud_init(PAUDIO_SETTING pAudio);

extern void     aud_setRecordSource(AUDIO_RECSRC RecSrc);
extern void     aud_setOutput(AUDIO_OUTPUT Output);
extern void     aud_setSamplingRate(AUDIO_SR SamplingRate);
extern void     aud_setChannel(AUDIO_CH Channel);
extern void     aud_setVolume(AUDIO_VOL Vol);
extern void     aud_setGain(AUDIO_GAIN Gain);
extern void     aud_setEffect(AUDIO_EFFECT Effect);
extern void     aud_setMixer(AUDIO_MIXER MixerType);
extern void     aud_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);

extern void     aud_setI2SFormat(AUDIO_I2SFMT I2SFmt);
extern void     aud_setI2SClockRatio(AUDIO_I2SCLKR I2SCLKRatio);

extern void             aud_resetBufferQueue(void);
extern BOOL             aud_isBufferQueueFull(void);
extern BOOL             aud_addBufferToQueue(PAUDIO_BUF_QUEUE pAudioBufQueue);
extern PAUDIO_BUF_QUEUE aud_getDoneBufferFromQueue(void);
extern PAUDIO_BUF_QUEUE aud_getCurrentBufferFromQueue(void);

extern void     aud_stop(void);
extern void     aud_record(BOOL bAVSync, BOOL bTCLatchEvent, BOOL bTCHitEvent);
extern void     aud_playback(BOOL bTCLatchEvent, BOOL bTCHitEvent);
extern void     aud_pause(void);
extern void     aud_resume(void);

extern void     aud_setTimecodeTrigger(UINT32 uiTrigger);
extern void     aud_setTimecodeOffset(UINT32 uiOffset);
extern UINT32   aud_getTimecodeValue(void);

extern BOOL     aud_isPlaybackMode(void);
extern BOOL     aud_isBusy(void);

extern void     aud_sendAC97Command(BOOL bWriteCmd, UINT32 uiRegIdx, UINT32 uiData);
extern void     aud_readAC97Data(UINT32 *puiRegIdx, UINT32 *puiData);
extern void     aud_coldresetAC97(void);
extern void     aud_warmresetAC97(void);

extern BOOL     aud_sendI2SCommand(UINT32 uiRegIdx, UINT32 uiData);
extern BOOL     aud_readI2SData(UINT32 uiRegIdx, UINT32 *puiData);

extern void     aud_enableEvent(UINT32 uiEvent);
extern void     aud_disableEvent(UINT32 uiEvent);
//#NT#2009/11/05#Cliff Lin -begin
//#NT#Add for resampling
extern BOOL     aud_setResampleInfo(PAUDIO_RESAMPLE_INFO pResampleInfo);
//#NT#2009/11/05#Cliff Lin -end
//#NT#2009/12/21#Cliff Lin -begin
//#NT#Add for Audio clock control
extern BOOL     aud_setI2SClockCtrl(BOOL bAudClkAlwaysON);
//#NT#2009/12/21#Cliff Lin -end

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
extern void     aud_setExtCodec(AUDIO_CODECSEL AudioCodec, PAUDIO_CODEC_FUNC pAudioCodecFunc);
extern void     aud_switchCodec(AUDIO_CODECSEL AudioCodec);
//#NT#2010/01/29#Chris Hsu -end

//@}

#endif
