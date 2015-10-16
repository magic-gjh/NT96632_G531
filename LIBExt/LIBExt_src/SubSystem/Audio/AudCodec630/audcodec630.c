/*
    NT96630 embedded audio codec driver

    This file is the driver for NT96630 embedded audio codec.
    The I2S clock ratio must be "AUDIO_I2SCLKR_256FS_32BIT" or
    "AUDIO_I2SCLKR_256FS_64BIT".The I2S format must be "AUDIO_I2SFMT_LIKE_LSB".
    Default disable ALC, enable MIC BOOST.

    @file       audcodec630.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#include <string.h>
#include "OSCommon.h"
#include "eac.h"
#include "pll.h"
#include "Utility.h"
//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
#include "AudioCodec.h"
//#NT#2010/01/29#Chris Hsu -end
#include "audio_int.h"
#include "audcodec630_int.h"

/**
    @addtogroup mISYSAud
*/
//@{

static AUDIO_OUTPUT         ADC630Output    = AUDIO_OUTPUT_HP;
static AUDIO_CH             ADC630Ch        = AUDIO_CH_RIGHT;
static AUDIO_GAIN           ADC630Gain      = AUDIO_GAIN_2;
//#NT#SA4   -begin
// add Get volume info
static AUDIO_VOL            ADCVolume       = AUDIO_VOL_7;
//#NT#SA4   -begin
//#NT#2010/02/02#Chris Hsu -begin
//#NT#Fine tune set sampling rate function, only set to HW when sampling rate is changed
static AUDIO_SR             ADC630SR        = 0;
//#NT#2010/02/02#Chris Hsu -end

//#NT#2010/01/06#Cliff Lin -begin
//#NT#Added for Support unmute function
static BOOL                 bADC630VolMute  = FALSE;
static AUDIO_VOL            ADC630UnMuteVol = AUDIO_GAIN_4;
//#NT#2010/01/06#Cliff Lin -end

//#NT#2010/02/08#Cliff Lin -begin
//#NT#Added for support the selected path clock always on
static BOOL                 bOnlyOutPathPwr2On  = FALSE;
static BOOL                 bHpPwrAlwaysOn  = FALSE;
static BOOL                 bSpkPwrAlwaysOn  = FALSE;
static BOOL                 bLinePwrAlwaysOn  = FALSE;
static void audcodec_setPreAssignPwr(AUDIO_OUTPUT Output);
//#NT#2010/02/08#Cliff Lin -end

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
static AUDIO_CODEC_FUNC     ADC630FP        =
{
    // Set Device Object
    NULL,
    // Init
    audcodec_init,
    // Set Record Source
    NULL,
    // Set Output
    audcodec_setOutput,
    // Set Sampling rate
    audcodec_setSamplingRate,
    // Set Channel
    audcodec_setChannel,
    // Set Volume (Playback)
    audcodec_setVolume,
    // Set Gain (Record)
    audcodec_setGain,
    // Set Effect
    NULL,
    // Set Feature
    audcodec_setFeature,
    // Stop
    audcodec_stop,
    // Record
    audcodec_record,
    // Playback
    audcodec_playback,
    // Set I2S Format
    NULL,
    // Set I2S clock ratio
    NULL,
    // Send command
    audcodec_sendCommand,
    // Read data
    audcodec_readData,
    // Set Mixer
    audcodec_setMixer,
};

/**
    Get audio codec function pointer for NT96630 embedded audio codec

    This function will return audio code function pointer for NT96630 embedded
    audio codec.

    @parm[out]  pAudCodecFunc       Audio codec function pointer
    @return void
*/
void audcodec_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc)
{
    memcpy((void *)pAudCodecFunc, (const void *)&ADC630FP, sizeof(AUDIO_CODEC_FUNC));
}

/*
    Set the audio device object pointer of audio module

    This function will set the audio device object pointer

    @param[in] pAudDevObj   Audio device object
    @return void
*/
#if 0
void audcodec_setDevObj(PAUDIO_DEVICE_OBJ pAudDevObj)
{
}
#endif
//#NT#2010/01/29#Chris Hsu -end

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
void audcodec_init(PAUDIO_SETTING pAudio)
{
    // Set default register value
    eac_init();

    // Set Sampling Rate
    audcodec_setSamplingRate(pAudio->SamplingRate);

    // Set Channel
    audcodec_setChannel(pAudio->Channel);

    // Set Output path
    audcodec_setOutput(pAudio->Output);
}

/*
    Set record source API for audio module

    This function will set record source

    @param[in] RecSrc   Record Source
    @return void
*/
#if 0
void audcodec_setRecordSource(AUDIO_RECSRC RecSrc)
{
}
#endif


/*
    Set pre-assigned output path power

    Set pre-assigned output path power

    @param[in] Output   Output Path
    @return void
*/
static void audcodec_setPreAssignPwr(AUDIO_OUTPUT Output)
{

    BOOL bSetDACPwrEn = FALSE;

    //#NT#2010/02/08#Cliff Lin -begin
    //#NT#Added for support the selected path clock always on
    ADC630Output = Output;

    if(bHpPwrAlwaysOn || bSpkPwrAlwaysOn || bLinePwrAlwaysOn)
    {
        if(bHpPwrAlwaysOn && ((bOnlyOutPathPwr2On) || (Output == AUDIO_OUTPUT_HP)))
        {
            bSetDACPwrEn = TRUE;
            eac_setDACOutput(EAC_OUTPUT_HP,TRUE);
        }
        else
        {
            eac_setDACOutput(EAC_OUTPUT_HP,FALSE);
        }
        if(bSpkPwrAlwaysOn && ((bOnlyOutPathPwr2On) || (Output == AUDIO_OUTPUT_SPK)))
        {
            bSetDACPwrEn = TRUE;
            eac_setDACOutput(EAC_OUTPUT_SPK,TRUE);
        }
        else
        {
            eac_setDACOutput(EAC_OUTPUT_SPK,FALSE);
        }
        if(bLinePwrAlwaysOn && ((bOnlyOutPathPwr2On) || (Output == AUDIO_OUTPUT_LINE)))
        {
            eac_setDACOutput(EAC_OUTPUT_LINE,TRUE);
            bSetDACPwrEn = TRUE;
        }
        else
        {
            eac_setDACOutput(EAC_OUTPUT_LINE,FALSE);
        }
    }

    if(bSetDACPwrEn == TRUE)
    {
        // Enable DAC power
        eac_setDACPwr(TRUE);
        // Disable DAC reset
        eac_setDACReset(FALSE);
    }
    else
    {
        // Enable DAC power
        eac_setDACPwr(FALSE);
        // Disable DAC reset
        eac_setDACReset(TRUE);
    }
}

/*
    Set output path API for audio module

    This function will set output path

    @param[in] Output   Output Path
    @return void
*/
void audcodec_setOutput(AUDIO_OUTPUT Output)
{
    BOOL bSetDACPwrEn = FALSE;

    // In playing
    if (eac_isDACEnable() == TRUE)
    {
        // Disable previous output path
        audcodec_stop();

        //#NT#2010/02/08#Cliff Lin -begin
        //#NT#Added for support the selected path clock always on
        audcodec_setPreAssignPwr(Output);
        //#NT#2010/02/08#Cliff Lin -end

        // Enable new output path
        audcodec_playback();
    }
    else
    {
        //#NT#2010/02/08#Cliff Lin -begin
        //#NT#Added for support the selected path clock always on
        audcodec_setPreAssignPwr(Output);
        //#NT#2010/02/08#Cliff Lin -end
    }
}

/*
    Set sampling rate API for audio module

    This function will set sampling rate

    @param[in] SamplingRate     Sampling Rate
    @return void
*/
void audcodec_setSamplingRate(AUDIO_SR SamplingRate)
{
    //#NT#2010/02/02#Chris Hsu -begin
    //#NT#Fine tune set sampling rate function, only set to HW when sampling rate is changed
    if (SamplingRate == ADC630SR)
    {
        return;
    }

    ADC630SR = SamplingRate;
    //#NT#2010/02/02#Chris Hsu -end

    switch(SamplingRate)
    {
    // 8 KHz
    case AUDIO_SR_8000:
        // 2.048 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_2048) <<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 11.025 KHz
    case AUDIO_SR_11025:
        // 2.822 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_2822)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 12 KHz
    case AUDIO_SR_12000:
        // 3.072 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_3072)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 16 KHz
    case AUDIO_SR_16000:
        // 4.096 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_4096)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 22.05 KHz
    case AUDIO_SR_22050:
        // 5.644 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_5644)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 24 KHz
    case AUDIO_SR_24000:
        // 6.144 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_6144)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 32 KHz
    case AUDIO_SR_32000:
        // 8.192 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_8192)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 44.1 KHz
    case AUDIO_SR_44100:
        // 11.289 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_11289)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;

    // 48 KHz
    default:
        // 12.288 MHZ MCLK
        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (AUDIO_CLK_12288)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));
        break;
    }

    aud_setI2SClockRatio(AUDIO_I2SCLKR_256FS_32BIT);
}

/*
    Set channel API for audio module

    This function will set channel

    @param[in] Channel  Channel mode
    @return void
*/
void audcodec_setChannel(AUDIO_CH Channel)
{
    // Save channel
    ADC630Ch = Channel;
}

/*
    Set playback volume API for audio module

    This function will set playback volume

    @param[in] Vol      Playback volume
    @return void
*/
void audcodec_setVolume(AUDIO_VOL Vol)
{
    UINT32 uiData;

    //#NT#2010/01/06#Cliff Lin -begin
    //#NT#Added for Support unmute function
    if ((Vol > AUDIO_VOL_7) && (Vol != AUDIO_VOL_UNMUTE))
    {
    //#NT#2010/01/06#Cliff Lin -end
        Vol = AUDIO_VOL_7;
    }

    if (Vol == AUDIO_VOL_MUTE)
    {
        // DAC digital mute
        eac_setDigitalVolume(0x00000000);

        //#NT#2010/01/06#Cliff Lin -begin
        //#NT#Added for Support unmute function
        bADC630VolMute = TRUE;
        //#NT#2010/01/06#Cliff Lin -end
    }
    else
    {
        //#NT#2010/01/06#Cliff Lin -begin
        //#NT#Added for Support unmute function
        if((Vol == AUDIO_VOL_UNMUTE) && (bADC630VolMute == TRUE))
        {
            Vol = ADC630UnMuteVol;
        }

        ADC630UnMuteVol = Vol;
        bADC630VolMute = FALSE;
        //#NT#2010/01/06#Cliff Lin -end

        //eac_setDigitalVolume(0x000000F6);
        eac_setDigitalVolume(0x000000FF);

        uiData = 3 + ((Vol - AUDIO_VOL_0) << 2);
        uiData &= 0X000000FF;

        eac_setAnalogVolume(EAC_OUTPUT_SPK, uiData);
        eac_setAnalogVolume(EAC_OUTPUT_HP, uiData);
        eac_setAnalogVolume(EAC_OUTPUT_LINE, uiData);
    }
    //#NT#SA4   -begin
    // add Get volume info
    ADCVolume = Vol;
    //#NT#SA4   -begin
}

//#NT#SA4   -begin
// add Get volume info
/**
  Get PCM Volume API for audio module

  This function is to Get PCM Volume API for audio module

  @param void
  @return AUDIO_VOL *Vol:Volume info
*/
void audcodec_getVolume(AUDIO_VOL *Vol)
{
     *Vol = ADCVolume;
}
//#NT#SA4   -end


/*
    Set record gain API for audio module

    This function will set record gain

    @param[in] Gain     Record gain
    @return void
*/
void audcodec_setGain(AUDIO_GAIN Gain)
{
    UINT32 uiData;

    if (Gain > AUDIO_GAIN_7)
    {
        Gain = AUDIO_GAIN_7;
    }

    ADC630Gain = Gain;

    if (Gain == AUDIO_GAIN_MUTE)
    {
        // ADC digital mute
        eac_setDigitalGain(0x00);
    }
    else
    {
        eac_setDigitalGain(0x000000C3);

        // Set ALC target gain
        if (eac_isALCEnable() == TRUE)
        {
            uiData = 1 + ((Gain - AUDIO_GAIN_0) << 1);
            eac_setALC_Target(uiData);
        }
        // Set ADC PGA gain
        else
        {
            uiData = 2 + ((Gain - AUDIO_GAIN_0) << 2);
            eac_setAnalogGain(uiData);
        }
    }
}

/*
    Set playback sound effect API for audio module

    This function will set playback sound effect

    @param[in] Effect   Playback sound effect
    @return void
*/
#if 0
void audcodec_setEffect(AUDIO_EFFECT Effect)
{
}
#endif

/*
    Set special feature

    This function will set the special feature to the audio codec.

    @param[in] Feature  Audio feature
        - @b AUDIO_FEATURE_ALC
        - @b AUDIO_FEATURE_MICBOOST
        - @b AUDIO_FEATURE_NG_WITH_MICBOOST
        - @b AUDIO_FEATURE_OUTPATH_ONLYPWR2ON
        - @b AUDIO_FEATURE_HP_PWR_ALWAYSON
        - @b AUDIO_FEATURE_SPK_PWR_ALWAYSON
        - @b AUDIO_FEATURE_LINE_PWR_ALWAYSON

    @param[in] bEnable  Enable/Disable the feature
        - @b TRUE:  Enable
        - @b FALSE: Disable
    @return void
*/
void audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable)
{
    switch (Feature)
    {
    case AUDIO_FEATURE_ALC:
        // Enable/Disable ALC
        eac_setALC(bEnable);

        // Set gain
        audcodec_setGain(ADC630Gain);
        break;

    case AUDIO_FEATURE_MICBOOST:
        // Enable/Disable microphone boost
        eac_setBoostA(bEnable);

        // Modify Noise Gate Threshold
        if (bEnable == TRUE)
        {
//            eac_setALC_NoiseThd(0x05);
            eac_setALC_NoiseThd(0x07);
        }
        else
        {
            eac_setALC_NoiseThd(0x1E);
        }
        break;

    case AUDIO_FEATURE_NG_WITH_MICBOOST:
        eac_setBoostD(bEnable);
        break;

    //#NT#2010/02/08#Cliff Lin -start
    //#NT#Added for support the selected path clock always on
    case AUDIO_FEATURE_OUTPATH_ONLYPWR2ON:
        bOnlyOutPathPwr2On  = bEnable;
        if ((bEnable == TRUE) && (bHpPwrAlwaysOn || bSpkPwrAlwaysOn || bLinePwrAlwaysOn))
        {
            if(bHpPwrAlwaysOn)
            {
                eac_setDACOutput(EAC_OUTPUT_HP,FALSE);
            }
            if(bSpkPwrAlwaysOn)
            {
                eac_setDACOutput(EAC_OUTPUT_SPK,FALSE);
            }
            if(bLinePwrAlwaysOn)
            {
                eac_setDACOutput(EAC_OUTPUT_LINE,FALSE);
            }
            // Enable DAC power
            eac_setDACPwr(TRUE);

            // Disable DAC reset
            eac_setDACReset(FALSE);
        }
        break;

    case AUDIO_FEATURE_HP_PWR_ALWAYSON:
        if(bOnlyOutPathPwr2On)
        {
            if(bEnable)
            {
                bHpPwrAlwaysOn  = bEnable;
            }
        }
        else
        {
            bHpPwrAlwaysOn  = bEnable;
        }
        break;

    case AUDIO_FEATURE_SPK_PWR_ALWAYSON:
        if(bOnlyOutPathPwr2On)
        {
            if(bEnable)
            {
                bSpkPwrAlwaysOn  = bEnable;
            }
        }
        else
        {
            bSpkPwrAlwaysOn  = bEnable;
        }
        break;

    case AUDIO_FEATURE_LINE_PWR_ALWAYSON:
        if(bOnlyOutPathPwr2On)
        {
            if(bEnable)
            {
                bLinePwrAlwaysOn  = bEnable;
            }
        }
        else
        {
            bLinePwrAlwaysOn  = bEnable;
        }
        break;
    //#NT#2010/02/08#Cliff Lin -end
    default:
        debug_wrn(("Unsupported feature\r\n"));
        break;
    }
}

/*
    Stop API for audio module

    This function will be called when audio stop

    @return void
*/
void audcodec_stop(void)
{
    //#NT#2010/02/08#Cliff Lin -begin
    //#NT#Added for support the selected path clock always on
    // disable DAC SPK output path
    if(!bSpkPwrAlwaysOn)
    {
        eac_setDACOutput(EAC_OUTPUT_SPK,FALSE);
    }
    // disable DAC LINE output path
    if(!bLinePwrAlwaysOn)
    {
        eac_setDACOutput(EAC_OUTPUT_LINE,FALSE);
    }
    // disable DAC HP output path
    if(!bHpPwrAlwaysOn)
    {
        eac_setDACOutput(EAC_OUTPUT_HP,FALSE);
    }
    if(!(eac_chkDACOutputEn(EAC_OUTPUT_SPK) || eac_chkDACOutputEn(EAC_OUTPUT_LINE)
       || eac_chkDACOutputEn(EAC_OUTPUT_HP)))
    {
        // enable DAC reset
        eac_setDACReset(TRUE);

        // disable DAC power
        eac_setDACPwr(FALSE);
    }
    //#NT#2010/02/08#Cliff Lin -end
//#NT#SA4/2009/11/2 - begin
//fixed 爆音問題
    switch (ADC630Output)
    {
    // Speaker
    case AUDIO_OUTPUT_SPK:
    default:
        // enable DAC reset
        eac_setDACReset(TRUE);
        // disable DAC power
        eac_setDACPwr(FALSE);
        eac_setDACOutput(EAC_OUTPUT_SPK,FALSE);
        break;

    // Headphone
    case AUDIO_OUTPUT_HP:
        //eac_setDACPwr(FALSE);
        //eac_setDACOutput(EAC_OUTPUT_HP,FALSE);
        break;

    // Lineout
    case AUDIO_OUTPUT_LINE:
        break;

    // None
    case AUDIO_OUTPUT_NONE:
         // enable DAC reset
        eac_setDACReset(TRUE);
        eac_setDACPwr(FALSE);
        eac_setDACOutput(EAC_OUTPUT_LINE,FALSE);
        eac_setDACOutput(EAC_OUTPUT_SPK,FALSE);
        eac_setDACOutput(EAC_OUTPUT_HP,FALSE);
        break;
    }

//#NT#SA4/2009/11/2 - end
    // enable ADC reset
    eac_setADCReset(TRUE);
    // disable ADC power
    eac_setADCPwr(FALSE);
}

/*
    Record API for audio module

    This function will be called when start to record

    @return void
*/
void audcodec_record(void)
{
    /*
    switch (ADC630Ch)
    {
    case AUDIO_CH_LEFT:
        break;

    case AUDIO_CH_RIGHT:
        break;

    case AUDIO_CH_STEREO:
        break;

    default:
        break;
    }
    */

    // enable ADC power
    eac_setADCPwr(TRUE);

    // disable ADC reset
    eac_setADCReset(FALSE);

    // Delay 200ms to avoid popup noise
    TimerDelayMs(200);
}

/*
    Playback API for audio module

    This function will be called when start to playback

    @return void
*/
void audcodec_playback(void)
{
    switch (ADC630Output)
    {
    // Speaker
//    case AUDIO_OUTPUT_SPK:
    default:
//#NT#SA4/2009/11/2 - begin
//fixed 爆音問題
         // enable DAC reset
        eac_setDACReset(TRUE);
        eac_setDACPwr(FALSE);
        eac_setDACOutput(EAC_OUTPUT_LINE,FALSE);
        eac_setDACOutput(EAC_OUTPUT_HP,FALSE);
        eac_setDACOutput(EAC_OUTPUT_SPK,TRUE);
//#NT#2009/11/2# SA4 - end
        break;

    // Headphone
    case AUDIO_OUTPUT_HP:
        eac_setDACOutput(EAC_OUTPUT_HP, TRUE);
        break;

    // Lineout
    case AUDIO_OUTPUT_LINE:
        eac_setDACOutput(EAC_OUTPUT_LINE,TRUE);
        break;

    // None
    case AUDIO_OUTPUT_NONE:
        return;
    }

    // Enable DAC power
    eac_setDACPwr(TRUE);

    // Disable DAC reset
    eac_setDACReset(FALSE);

    // Patch for Audio codec stable time
    TimerDelayMs(60);
}

/*
    Mixer API for audio module

    This function will set mixer for output path

    @param[in] MixerType    Mixer type
    @return void
*/
void audcodec_setMixer(AUDIO_MIXER MixerType)
{
    switch (MixerType)
    {
    case AUDIO_MIXER_NORMAL:
    case AUDIO_MIXER_RLAVG:
        eac_setDACMixer(MixerType);
        break;

    default:
        debug_wrn(("Unsupported type\r\n"));
        break;
    }
}

/*
    Set I2S Format API for audio module

    This function will set the I2S format

    @param[in] I2SFmt   I2S Format
    @return void
*/
#if 0
void audcodec_setFormat(AUDIO_I2SFMT I2SFmt)
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
void audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio)
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
BOOL audcodec_sendCommand(UINT32 uiReg, UINT32 uiData)
{
    return eac_sendCommand(uiReg, uiData);
}

/*
    Read data from audio codec

    This function will read data from audio codec

    @param[in] uiReg        Audio codec register index
    @param[out] puiData     Register data
    @return Read data status
        - @b TRUE:          Register index is correct and data is read from codec
        - @b FALSE:         Read data from register fail
*/
BOOL audcodec_readData(UINT32 uiReg, UINT32 *puiData)
{
    return eac_readData(uiReg, puiData);
}

//@}
