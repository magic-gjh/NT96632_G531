/*
    NT96630 embedded HDMI extended audio codec driver

    This file is the driver for NT96630 embedded HDMI extended audio codec.

    @file       audExtcodecHDMI630.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#include <string.h>
#include "audExtcodecHDMI630_int.h"
#include "hdmitx.h"
//#NT#2010/04/20#Chris Hsu[0010194] -begin
//#NT#Fix recording muted audio when HDMI is pluged in
#include "audio_int.h"
//#NT#2010/04/20#Chris Hsu[0010194] -end

/**
    @addtogroup mISYSAud
*/
//@{

static BOOL                 bIsMute         = FALSE;
static AUDIO_CODEC_FUNC     CodecHDMI630FP  =
{
    // Set Device Object
    NULL,
    // Init
    NULL,
    // Set Record Source
    NULL,
    // Set Output
    NULL,
    // Set Sampling rate
    NULL,
    // Set Channel
    NULL,
    // Set Volume (Playback)
    audcodecHDMI_setVolume,
    // Set Gain (Record)
    NULL,
    // Set Effect
    NULL,
    // Set Feature
    NULL,
    //#NT#2010/04/20#Chris Hsu[0010194] -begin
    //#NT#Fix recording muted audio when HDMI is pluged in
    // Stop
    (FP)AUDIO_CODEC_FUNC_USE_DEFAULT,
    // Record
    (FP)AUDIO_CODEC_FUNC_USE_DEFAULT,
    //#NT#2010/04/20#Chris Hsu[0010194] -end
    // Playback
    NULL,
    // Set I2S Format
    NULL,
    // Set I2S clock ratio
    NULL,
    // Send command
    NULL,
    // Read data
    NULL,
    // Set Mixer
    NULL,
};

/**
    Get audio codec function pointer for NT96630 embedded HDMI audio codec

    This function will return audio code function pointer for NT96630 embedded
    HDMI audio codec.

    @parm[out]  pAudCodecFunc       Audio codec function pointer
    @return void
*/
void audcodecHDMI_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc)
{
    memcpy((void *)pAudCodecFunc, (const void *)&CodecHDMI630FP, sizeof(AUDIO_CODEC_FUNC));
}

/*
    Set the audio device object pointer of audio module

    This function will set the audio device object pointer

    @param[in] pAudDevObj   Audio device object
    @return void
*/
#if 0
void audcodecHDMI_setDevObj(PAUDIO_DEVICE_OBJ pAudDevObj)
{
}
#endif

/*
    Initialize API for audio module

    This function will initialize audio codec
    It will set the following parameters if required:\n
        (1) Sampling Rate\n
        (2) Channel\n
        (3) Record Source\n
        (4) I2S Clock Ratio\n
        (5) I2S Format\n
        (6) Master/Slave Mode\n
        (7) Output Path\n
        (8) Other Codec related settings (Power on/down, Control interface...)

    @param[in] pAudio   Audio setting
    @return void
*/
#if 0
void audcodecHDMI_init(PAUDIO_SETTING pAudio)
{
}
#endif

/*
    Set record source API for audio module

    This function will set record source

    @param[in] RecSrc   Record Source
    @return void
*/
#if 0
void audcodecHDMI_setRecordSource(AUDIO_RECSRC RecSrc)
{
}
#endif

/*
    Set output path API for audio module

    This function will set output path

    @param[in] Output   Output Path
    @return void
*/
#if 0
void audcodecHDMI_setOutput(AUDIO_OUTPUT Output)
{
}
#endif

/*
    Set sampling rate API for audio module

    This function will set sampling rate

    @param[in] SamplingRate     Sampling Rate
    @return void
*/
#if 0
void audcodecHDMI_setSamplingRate(AUDIO_SR SamplingRate)
{
}
#endif

/*
    Set channel API for audio module

    This function will set channel

    @param[in] Channel  Channel mode
    @return void
*/
#if 0
void audcodecHDMI_setChannel(AUDIO_CH Channel)
{
}
#endif

/*
    Set playback volume API for audio module

    This function will set playback volume

    @param[in] Vol      Playback volume
    @return void
*/
void audcodecHDMI_setVolume(AUDIO_VOL Vol)
{
    if (Vol == AUDIO_VOL_MUTE)
    {
        // Already mute
        if (bIsMute == TRUE)
        {
            return;
        }

        bIsMute = TRUE;
    }
    else
    {
        // Already unmute
        if (bIsMute == FALSE)
        {
            return;
        }

        bIsMute = FALSE;
    }

    if (hdmitx_isOpened() == FALSE)
    {
        hdmitx_open();
        hdmitx_setAudMute(bIsMute);
        hdmitx_close();
    }
    else
    {
        hdmitx_setAudMute(bIsMute);
    }
}

/*
    Set record gain API for audio module

    This function will set record gain

    @param[in] Gain     Record gain
    @return void
*/
#if 0
void audcodecHDMI_setGain(AUDIO_GAIN Gain)
{
}
#endif

/*
    Set playback sound effect API for audio module

    This function will set playback sound effect

    @param[in] Effect   Playback sound effect
    @return void
*/
#if 0
void audcodecHDMI_setEffect(AUDIO_EFFECT Effect)
{
}
#endif

/*
    Set special feature

    This function will set the special feature to the audio codec.

    @param[in] Feature  Audio feature
    @param[in] bEnable  Enable/Disable the feature
        - @b TRUE:  Enable
        - @b FALSE: Disable
    @return void
*/
#if 0
void audcodecHDMI_setFeature(AUDIO_FEATURE Feature, BOOL bEnable)
{
}
#endif

/*
    Stop API for audio module

    This function will be called when audio stop

    @return void
*/
#if 0
void audcodecHDMI_stop(void)
{
}
#endif

/*
    Record API for audio module

    This function will be called when start to record

    @return void
*/
#if 0
void audcodecHDMI_record(void)
{
}
#endif

/*
    Playback API for audio module

    This function will be called when start to playback

    @return void
*/
#if 0
void audcodecHDMI_playback(void)
{
}
#endif

/*
    Mixer API for audio module

    This function will set mixer for output path

    @param[in] MixerType    Mixer type
    @return void
*/
#if 0
void audcodecHDMI_setMixer(AUDIO_MIXER MixerType)
{
}
#endif

/*
    Set I2S Format API for audio module

    This function will set the I2S format

    @param[in] I2SFmt   I2S Format
    @return void
*/
#if 0
void audcodecHDMI_setFormat(AUDIO_I2SFMT I2SFmt)
{
}
#endif

/*
    Set I2S Clock Ratio API for audio module

    This function will set I2S clock ratio

    @param[in] I2SCLKRatio  I2S clock ratio
    @return void
*/
#if 0
void audcodecHDMI_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio)
{
}
#endif

/*
    Send command to audio codec

    This function will send command to audio codec's register

    @param[in] uiReg    Audio codec register index
    @param[in] uiData   Register data
    @return Send command status
        - @b TRUE:      Register index is correct and data is sent
        - @b FALSE:     Send command to register fail
*/
#if 0
BOOL audcodecHDMI_sendCommand(UINT32 uiReg, UINT32 uiData)
{
}
#endif

/*
    Read data from audio codec

    This function will read data from audio codec

    @param[in] uiReg        Audio codec register index
    @param[out] puiData     Register data
    @return Read data status
        - @b TRUE:          Register index is correct and data is read from codec
        - @b FALSE:         Read data from register fail
*/
#if 0
BOOL audcodecHDMI_readData(UINT32 uiReg, UINT32 *puiData)
{
}
#endif

//@}
