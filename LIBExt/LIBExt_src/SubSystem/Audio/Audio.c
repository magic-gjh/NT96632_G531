/*
    Audio module driver

    This file is the driver of audio module

    @file       Audio.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include <string.h>
#include "OSCommon.h"
#include "Interrupt.h"
//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
#include "AudioCodec.h"
//#NT#2010/01/29#Chris Hsu -end
#include "audio_int.h"
#include "Pll.h"
#include "Top.h"
#include "GPIO.h"
#include "Timer.h"
#include "SIF.h"
#include "I2C.h"
#include "Utility.h"
#include "dma.h"
#include "dai.h"

/**
    @addtogroup mISYSAud
*/
//@{

// Define
#define AUDIO_FIFOERRMSG_OPTION     ENABLE    //default :Enabled

//#NT#2009/12/22#Cliff Lin -begin
//#NT#Fix audio queue operation for audio resampling
// Static variable
static AUDIO_BUF_QUEUE      AudioRealBufferQueue[AUDIO_BUF_BLKNUM];
static PAUDIO_BUF_QUEUE     pAudioRealBufferFront   = NULL;
static PAUDIO_BUF_QUEUE     pAudioRealBufferRear    = NULL;
static PAUDIO_BUF_QUEUE     pAudioRealBufferDone    = NULL;

static AUDIO_BUF_QUEUE      AudioPseudoBufferQueue[AUDIO_BUF_BLKNUM];
static PAUDIO_BUF_QUEUE     pAudioPseudoBufferFront = NULL;
static PAUDIO_BUF_QUEUE     pAudioPseudoBufferRear  = NULL;
static PAUDIO_BUF_QUEUE     pAudioPseudoBufferDone  = NULL;
//#NT#2009/12/22#Cliff Lin -end

static ID                   AudioLockStatus     = NO_TASK_LOCKED;
static AUDIO_DEVICE_OBJ     AudioDeviceObj      =
{
    NULL,               // Event handler
    0,                  // GPIO pin number for Cold Reset
    AUDIO_I2SCTRL_SIF,  // I2S control interface
    2,                  // I2S SIF channel / ADC channel
    0,                  // GPIO pin number for Data
    0,                  // GPIO pin number for Clk
    0,                  // GPIO pin number for CS
    0                   // ADC value of PCM data = 0
};

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
static UINT32               uiAudioExtCodecNum  = AUDIO_CODECSEL_DEFAULT;
static AUDIO_CODECSEL       AudioSelectedCodec  = AUDIO_CODECSEL_DEFAULT;
static AUDIO_CODEC_FUNC     AudioCodecFunc[AUDIO_MAX_CODEC_NUM] =
{
    // Default codec
    {
        NULL,           // setDevObj
        NULL,           // init
        NULL,           // setRecordSource
        NULL,           // setOutput
        NULL,           // setSamplingRate
        NULL,           // setChannel
        NULL,           // setVolume
        NULL,           // setGain
        NULL,           // setEffect
        NULL,           // setFeature
        NULL,           // stop
        NULL,           // record
        NULL,           // playback
        NULL,           // setFormat
        NULL,           // setClockRatio
        NULL,           // sendCommand
        NULL,           // readData
        NULL            // setMixer
    },
    // Extended codec 0
    {
        NULL,           // setDevObj
        NULL,           // init
        NULL,           // setRecordSource
        NULL,           // setOutput
        NULL,           // setSamplingRate
        NULL,           // setChannel
        NULL,           // setVolume
        NULL,           // setGain
        NULL,           // setEffect
        NULL,           // setFeature
        NULL,           // stop
        NULL,           // record
        NULL,           // playback
        NULL,           // setFormat
        NULL,           // setClockRatio
        NULL,           // sendCommand
        NULL,           // readData
        NULL            // setMixer
    }
};
//#NT#2010/01/29#Chris Hsu -end

static AUDIO_CH             AudioCH             = AUDIO_CH_RIGHT;
//#NT#2009/12/08#Cliff Lin -begin
//#NT#Add for Audio expand function
static BOOL                 bAudioExpandPlay    = FALSE;
static BOOL                 bAudioExpandRec     = FALSE;
//#NT#2009/12/08#Cliff Lin -end

//#NT#2009/12/22#Cliff Lin -begin
//#NT#Add for Audio clock control
static BOOL                 bAudioClkEn         = FALSE;
//#NT#2009/12/22#Cliff Lin -end
//#NT#2009/12/22#Cliff Lin -begin
//#NT#Add for Audio operation mode. The operation mode will be changed
//#NT#when call aud_playback() or aud_record() function.
static AUDIO_MODE           AudioMode           = AUDIO_TX;
//#NT#2009/12/22#Cliff Lin -end
//#NT#2009/11/05#Cliff Lin -begin
//#NT#Add for resampling
static AUDIO_SR             AudioSR             = AUDIO_SR_8000;
static AUDIO_RESAMPLE_INFO  AudioResampleInfo   =
{
    FALSE,
    0,
    0,
    0
};
static UINT32               uiAudioResampleAddr[AUDIO_BUF_BLKNUM-1];
static UINT32               uiAudioResampleAddrIndex    = 0;
static UINT32               uiAudioResampleInOutRatio, uiAudioResampleKStart;
static INT16                Resample_factor[15][6]      =
{
    { 0,    0, 1024,   0,    0,  0},
    {12, -105,  603, 603, -105, 12},
    { 0,    0, 1024,   0,    0,  0},
    {16, -120,  818, 369,  -67,  6},
    { 6,  -67,  369, 818, -120, 16},
    { 0,    0, 1024,   0,    0,  0},
    {16, -112,  904, 258,  -47,  3},
    {12, -105,  603, 603, -105, 12},
    { 3,  -47,  258, 904, -112, 16},
    { 0,    0, 1024,   0,    0,  0},
    { 14, -90,  969, 157,  -28,  1},
    { 16,-120,  818, 369,  -67,  6},
    { 12,-105,  603, 603, -105, 12},
    { 6,  -67,  369, 818, -120, 16},
    { 1,  -28,  157, 969,  -90, 14}
};
static INT16                uiPre_Sample[6] = {0, 0, 0, 0, 0, 0};

/*
    Resampling PCM data.

    This function is used to resampling audio PCM data. (Audio internal API)

    @param[in] uiInAddr    The source buffer address of data to be resampling
    @param[in] uiInSize    The source buffer size in byte unit of data to be resampling
    @param[out] uiOutAddr  Re-sampling data output buffer address
    @param[out] uiOutSize  Re-sampling data output buffer size
    @return void
*/
static void aud_resample(UINT32 uiInAddr, UINT32 uiInSize, UINT32* uiOutAddr, UINT32* uiOutSize)
{
    UINT32 i, k, m, uiOutputAddr;
    INT32  iRreSample;

    uiOutputAddr    =
    *uiOutAddr      = uiAudioResampleAddr[uiAudioResampleAddrIndex];
    *uiOutSize      = uiAudioResampleInOutRatio*uiInSize;

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix resampling issue at stereo
    if(AudioCH == AUDIO_CH_STEREO)
    {
        for(i=0; i< uiInSize/4; i++)
        {

            if(i<5)
            {
                for(k=uiAudioResampleKStart; k< uiAudioResampleInOutRatio+uiAudioResampleKStart; k++)
                {
                    iRreSample = 0;
                    for(m=0; m<5-i; m++)
                    {
                        iRreSample +=uiPre_Sample[i+m]*Resample_factor[k][m]/1024;
                    }
                    for(m=5-i; m<6; m++)
                    {
                        iRreSample += (*((INT16*)(uiInAddr + (m - (5-i))*4))) * Resample_factor[k][m]/1024;
                    }
                    *((UINT16*)(uiOutputAddr+(i*uiAudioResampleInOutRatio+(k-uiAudioResampleKStart))*4)) = (INT16)iRreSample;
                    *((UINT16*)(uiOutputAddr+(i*uiAudioResampleInOutRatio+(k-uiAudioResampleKStart))*4+2)) = (INT16)iRreSample;
                }
            }
            else
            {
                for(k=uiAudioResampleKStart; k< uiAudioResampleInOutRatio+uiAudioResampleKStart; k++)
                {
                    iRreSample = 0;
                    for(m=0; m<6; m++)
                    {
                        iRreSample += (*((INT16*)(uiInAddr +((i-5)+m)*4 ))) * Resample_factor[k][m]/1024;
                    }
                    *((UINT16*)(uiOutputAddr+(i*uiAudioResampleInOutRatio+(k-uiAudioResampleKStart))*4)) = (INT16)iRreSample;
                    *((UINT16*)(uiOutputAddr+(i*uiAudioResampleInOutRatio+(k-uiAudioResampleKStart))*4+2)) = (INT16)iRreSample;
                }
                if(i >= ((uiInSize/4)-5) )
                {
                    uiPre_Sample[i-((uiInSize/4)-5)] = (*((INT16*)(uiInAddr+i*4)));
                }
            }
        }
    }
    //#NT#2009/12/22#Cliff Lin -end
    else //mono
    {
        for(i=0; i< uiInSize/2; i++)
        {
            if(i<5)
            {
                for(k=uiAudioResampleKStart; k< uiAudioResampleInOutRatio+uiAudioResampleKStart; k++)
                {
                    iRreSample = 0;
                    for(m=0; m<5-i; m++)
                    {
                        iRreSample += uiPre_Sample[i+m]*Resample_factor[k][m]/1024;
                    }
                    for(m=5-i; m<6; m++)
                    {
                        iRreSample += ((*((INT16*)(uiInAddr + (m - (5-i))*2))) * Resample_factor[k][m])/1024;
                    }
                    *((UINT16*)(uiOutputAddr+(i*uiAudioResampleInOutRatio+(k-uiAudioResampleKStart))*2)) = (INT16)iRreSample;
                }
            }
            else
            {
                for(k=uiAudioResampleKStart; k< uiAudioResampleInOutRatio+uiAudioResampleKStart; k++)
                {
                    iRreSample = 0;
                    for(m=0; m<6; m++)
                    {
                        iRreSample += ((*((INT16*)(uiInAddr +((i-5)+m)*2 ))) * Resample_factor[k][m])/1024;
                    }
                    *((UINT16*)(uiOutputAddr+(i*uiAudioResampleInOutRatio+(k-uiAudioResampleKStart))*2)) = (INT16)iRreSample;
                }
                if(i >= ((uiInSize/2)-5) )
                {
                    uiPre_Sample[i-((uiInSize/2)-5)] = (*((INT16*)(uiInAddr+i*2)));
                }
            }
        }
    }
    uiAudioResampleAddrIndex = (uiAudioResampleAddrIndex + 1) % (AUDIO_BUF_BLKNUM - 1);
}
//#NT#2009/11/05#Cliff Lin -end

/*
    Audio ISR

    Audio interrupt service routine

    @return void
*/
void aud_isrHandler(void)
{

    UINT32 uiAudioEventStatus = 0;
    UINT32 uiIntStatusReg = 0;

    // Get interrupt status
    uiIntStatusReg = dai_getIntStatus();

    // Clear interrupt status
    dai_clrIntStatus(uiIntStatusReg);

    // Only handle interrupts which are enabled
    uiIntStatusReg &= (dai_getIntCtrl());

    // Time Code Hit interrupt
    if (uiIntStatusReg & (DAI_TXTCHIT_INT_STS | DAI_RXTCHIT_INT_STS))
    {
        uiAudioEventStatus |= AUDIO_EVENT_TCHIT;

        //#NT#2009/12/22#Cliff Lin -begin
        //#NT#remove unnecessary code
        // Change timecode trigger value first
        //dai_setTimecodeTrigger(DAI_TX,(dai_getTimecodeTrigger(DAI_TX)) -1);
        //#NT#2009/12/22#Cliff Lin -end
    }

    // Time Code Latch interrupt
    if (uiIntStatusReg & DAI_RXTCLATCH_INT_STS)
    {
        uiAudioEventStatus |= AUDIO_EVENT_TCLATCH;
    }

    // DMA done interrupt
    if (uiIntStatusReg & (DAI_TXDMADONE_INT_STS | DAI_RXDMADONE_INT_STS))
    {
        //#NT#2009/12/22#Cliff Lin -begin
        //#NT#Fix audio queue operation for audio resampling
        uiAudioEventStatus |= AUDIO_EVENT_DMADONE;
        // Buffer Full or Empty
        if (pAudioRealBufferRear->pNext == pAudioRealBufferFront)
        {
            if (uiIntStatusReg & (DAI_TXDMADONE_INT_STS))
            {
                uiAudioEventStatus |= AUDIO_EVENT_BUF_EMPTY;
            }
            else
            {
                uiAudioEventStatus |= AUDIO_EVENT_BUF_FULL;
            }
        }
        else
        {
            // For pseudo operation using(upper layer using)
            // Save valid PCM data size
            pAudioPseudoBufferRear->uiValidSize   = pAudioPseudoBufferRear->uiSize;

            // Move to next queue
            pAudioPseudoBufferRear                = pAudioPseudoBufferRear->pNext;

            // For real operation using(audio driver operation using)
            // Save valid PCM data size
            pAudioRealBufferRear->uiValidSize   = pAudioRealBufferRear->uiSize;

            // Move to next queue
            pAudioRealBufferRear                = pAudioRealBufferRear->pNext;

            // Set new DMA starting address and size
            if (pAudioRealBufferRear->pNext != pAudioRealBufferFront)
            {
                if (uiIntStatusReg & (DAI_TXDMADONE_INT_STS))
                {
                    dai_setDMAPara(DAI_TX, pAudioRealBufferRear->pNext->uiAddress, pAudioRealBufferRear->pNext->uiSize, FALSE );
                }
                else
                {
                    dai_setDMAPara(DAI_RX, pAudioRealBufferRear->pNext->uiAddress, pAudioRealBufferRear->pNext->uiSize, FALSE );
                }
            }

        }
        // Check done queue
        if (pAudioRealBufferRear->pNext == pAudioRealBufferDone)
        {
            uiAudioEventStatus |= AUDIO_EVENT_DONEBUF_FULL;
        }
        //#NT#2009/12/22#Cliff Lin -end
    }

    if (uiIntStatusReg & (DAI_TXFIFO_ERROR_INT_STS|DAI_RXFIFO_ERROR_INT_STS))
    {
        uiAudioEventStatus |= AUDIO_EVENT_FIFO_ERROR;

#if (AUDIO_FIFOERRMSG_OPTION == ENABLE)
        if(uiIntStatusReg & DAI_TXFIFO_ERROR_INT_STS)
        {
            debug_err(("Audio Tx Under-Run Error\r\n"));
        }
        else
        {
            debug_err(("Audio Rx Over-Run Error\r\n"));
        }
#endif
    }

    // Call the event handler
    if (AudioDeviceObj.pEventHandler != NULL)
    {
        AudioDeviceObj.pEventHandler(uiAudioEventStatus);
    }
    // Set Flag
    else
    {
        dai_setFlg();
    }

    // If audio is stopped and closed in event handler, don't do the following jobs
    if (AudioLockStatus != NO_TASK_LOCKED)
    {
        // Time Code Hit interrupt status must be cleared after timecode
        // trigger value changed
        if (uiIntStatusReg & DAI_TXTCHIT_INT_STS)
        {
            dai_clrIntStatus(DAI_TXTCHIT_INT_STS);

            // Make sure timecode hit interrupt status is cleared
            while (dai_getIntStatus() & DAI_TXTCHIT_INT_STS)
            {
                ;
            }
        }
    }
}

/**
    Lock audio

    This function will lock audio. (Wait for semaphore)

    @return Lock audio status
        - @b E_OK:      Lock success
        - @b Others:    Lock fail
*/
static ER aud_lock(void)
{
    ER erReturn;

    erReturn = wai_sem(SEMID_DAI);

    if (erReturn != E_OK)
        return erReturn;

    AudioLockStatus = TASK_LOCKED;

    return E_OK;
}

/**
    Unlock audio

    This function will unlock audio. (Signal semaphore)

    @return Unlock audio status
        - @b E_OK:      Unlock success
        - @b Others:    Unlock fail
*/
static ER aud_unlock(void)
{
    AudioLockStatus = NO_TASK_LOCKED;
    return sig_sem(SEMID_DAI);
}

/**
    Check whether audio is opened or not

    If audio is opened this function will return TRUE. Otherwise, this function
    will return FALSE and output error message to UART.

    @return Audio open status
        - @b FALSE:     Audio is closed
        - @b TRUE:      Audio is opened
*/
static BOOL aud_isOpened(void)
{
    if (AudioLockStatus == NO_TASK_LOCKED)
    {
        debug_err(("Audio driver is not opened\r\n"));
        return FALSE;
    }

    return TRUE;
}

#if 0
#pragma mark -
#endif

/**
    Get the lock status of audio

    This function return the lock status of audio.

    @return Audio lock status
        - @b NO_TASK_LOCKED:    Audio is free, no application is using audio
        - @b TASK_LOCKED:       Audio is locked by some application
*/
ID aud_getLockStatus(void)
{
    return AudioLockStatus;
}

#if 0
/**
    Pre-initialize audio driver.

    Pre-initialize driver required hardware before driver open.

    @note Don't call this function in ISR.

    @return void
*/
void aud_attach(void)
{
    //sif_enableClock();
}

/**
    De-initialize audio driver.

    De-initialize driver required hardware after driver close.

    @note Don't call this function in ISR.

    @return void
*/
void aud_detach(void)
{
    //sif_disableClock();
}
#endif

/**
    Open audio driver

    This function should be called before calling any other functions,
    except the following:
        - aud_getDeviceObject()
        - aud_getLockStatus()
        - aud_isPlaybackMode()
        - aud_isBusy()
        - aud_setExtCodec()

    You have to set the callback function pointer (pEventHandler) in the
    Device Object. The parameter of callback function will be the Event Mask.
    The Event Mask is the combination of the following:
        - AUDIO_EVENT_TCLATCH
        - AUDIO_EVENT_TCHIT
        - AUDIO_EVENT_DMADONE
        - AUDIO_EVENT_BUF_FULL
        - AUDIO_EVENT_BUF_EMPTY
        - AUDIO_EVENT_DONEBUF_FULL
        - AUDIO_EVENT_FIFO_ERROR

    If you want to use another PIN for AC97 cold reset, you should set
    bUseAltColdResetPin to TRUE and set uiAGPIOColdReset to correct AGPIO
    pin number. And don't forget to change pinmux of that pin to GPIO by
    your application.\n

    If you want to use another SIF channel to control I2S codec (default is 2),
    you should set bUseAltSIFChannel to TRUE and set uiAltSIFChannel to
    the channel number.\n

    If you want to use GPIO to control I2S codec, you should set bUseGPIO
    to TRUE, bUserAltSIFChannel to FALSE and set uiAGPIOSIFCS, uiAGPIOSIFCLK
    and uiAGPIOSIFDATA to correct AGPIO pin number. Don't forget to change
    pinmux of those pins to GPIO by your application.

    @param[in] pDevObj  Audio Device Object pointer
    @return Open status
        - @b E_OK       Open success
        - @b Others     Open fail
*/
ER aud_open(PAUDIO_DEVICE_OBJ pDevObj)
{
    ER      erReturn;

    // Lock Audio
    erReturn = aud_lock();
    if (erReturn != E_OK)
    {
        return erReturn;
    }

    //enable DAI isr
    dai_open(&aud_isrHandler);

    // Enable Audio clock
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Add for Audio clock control
    // Audio clock is not always ON, turn on clock
    if (bAudioClkEn == FALSE)
    {
        pll_enableClock(DAI_CLK);
        pll_enableClock(ADO_SCLK);
    }
    //#NT#2009/12/22#Cliff Lin -end

    // Save Audio Device Object setting
    AudioDeviceObj.pEventHandler        = pDevObj->pEventHandler;
    AudioDeviceObj.uiGPIOColdReset      = pDevObj->uiGPIOColdReset;
    AudioDeviceObj.uiI2SCtrl            = pDevObj->uiI2SCtrl;
    AudioDeviceObj.uiChannel            = pDevObj->uiChannel;
    AudioDeviceObj.uiGPIOData           = pDevObj->uiGPIOData;
    AudioDeviceObj.uiGPIOClk            = pDevObj->uiGPIOClk;
    AudioDeviceObj.uiGPIOCS             = pDevObj->uiGPIOCS;

    // Open GPIO driver
    gpio_open();

    // Open SIF driver
    if (AudioDeviceObj.uiI2SCtrl == AUDIO_I2SCTRL_SIF)
    {
        sif_open();
    }

    // Open I2C driver
    if (AudioDeviceObj.uiI2SCtrl == AUDIO_I2SCTRL_I2C)
    {
        i2c_open();
        i2c_enable();
    }

    // Reset timecode offset (pause/resume will set timecode offset value,
    // to avoid some AP forget to reset the offset value, reset the offset value here
    dai_setTimecodeOffset(DAI_TX, 0);

    // Setup Audio DMA High priority
    dma_setChannelPriority(DMA_CH_DAI_PLY, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_DAI_REC, DMA_PRIORITY_HIGH);

    return E_OK;
}

/**
    Close audio driver

    Release audio driver and let other application use audio.

    @return Close status
        - @b E_OK:      Close success
        - @b Others:    Close fail
*/
ER aud_close(void)
{
    ER erReturn;

    // Return immediatelly if is not opened
    if (AudioLockStatus == NO_TASK_LOCKED)
    {
        return E_OK;
    }

    // Release interrupt
    dai_close();

    // Disable Audio clock
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Add for Audio clock control
    // Audio clock is not always ON, turn off clock
    if (bAudioClkEn == FALSE)
    {
        pll_disableClock(DAI_CLK);
        pll_disableClock(ADO_SCLK);
    }
    //#NT#2009/12/22#Cliff Lin -end

    // Close GPIO driver
    gpio_close();

    // Close SIF driver
    if (AudioDeviceObj.uiI2SCtrl == AUDIO_I2SCTRL_SIF)
    {
        sif_close();
    }

    // Close I2C driver
    if (AudioDeviceObj.uiI2SCtrl == AUDIO_I2SCTRL_I2C)
    {
        i2c_disable();
        i2c_close();
    }

    // Unlock Audio
    erReturn = aud_unlock();
    if (erReturn != E_OK)
    {
        return erReturn;
    }

    return E_OK;
}

#if 0
#pragma mark -
#endif

/**
    Get audio device object

    Get audio device object.

    @param[out] pDevObj     Current audio device object
    @return void
*/
void aud_getDeviceObject(PAUDIO_DEVICE_OBJ pDevObj)
{
    memcpy((void *)pDevObj, (const void *)&AudioDeviceObj, sizeof(AUDIO_DEVICE_OBJ));
}

/************************************************************************/
/*                                                                      */
/* Audio Configuration related API                                      */
/*                                                                      */
/************************************************************************/

/**
    Initialize audio driver

    Initialize the audio driver based on parameter.
    This function should be call once after power on.

    @param[in] pAudio   Audio setting pointer
    @return void
*/
void aud_init(PAUDIO_SETTING pAudio)
{
    UINT32 uiTCTrigger;
    UINT32 uiTCValue;
    UINT32 i;

    // Initialize codec function and set the audio device object pointer to codec
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    // Only default codec will call this function,
    // If project assign extend codec, default and extend codec function
    // pointer will be set in aud_setExtCodec()
    if (uiAudioExtCodecNum == AUDIO_CODECSEL_DEFAULT)
    {
        audcodec_getFP(&AudioCodecFunc[AUDIO_CODECSEL_DEFAULT]);
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setDevObj != NULL)
        {
            AudioCodecFunc[i].setDevObj(&AudioDeviceObj);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end

    // Initialize buffer queue
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    for (i=0; i<AUDIO_BUF_BLKNUM; i++)
    {
        AudioPseudoBufferQueue[i].pNext = &AudioPseudoBufferQueue[(i + 1) % AUDIO_BUF_BLKNUM];

        AudioRealBufferQueue[i].pNext = &AudioRealBufferQueue[(i + 1) % AUDIO_BUF_BLKNUM];
    }
    //#NT#2009/12/22#Cliff Lin -end

    // Avoid timecode hit interrupt
    // for Tx
    uiTCTrigger = dai_getTimecodeTrigger(DAI_TX);
    uiTCValue = dai_getTimecodeValue(DAI_TX);

    if (uiTCValue == uiTCTrigger)
    {
        dai_setTimecodeTrigger(DAI_TX, uiTCTrigger - 1);
    }

    // for Rx
    uiTCTrigger = dai_getTimecodeTrigger(DAI_RX);
    uiTCValue = dai_getTimecodeValue(DAI_RX);

    if (uiTCValue == uiTCTrigger)
    {
        dai_setTimecodeTrigger(DAI_RX, uiTCTrigger - 1);
    }

    // Set Access Mode to DMA
    dai_setAccessMode(DAI_TX, DAI_ACCESS_DMA);
    dai_setAccessMode(DAI_RX, DAI_ACCESS_DMA);

    // Set DRAM PCM length(16 bits)
    dai_setDRAM_PCMLen(DAI_TX, DAI_DRAMPCM_16);
    dai_setDRAM_PCMLen(DAI_RX, DAI_DRAMPCM_16);

    // Set DRAM PCM format(little edian)
    dai_setDRAM_PCMFmt(DAI_TX, DAI_DRAMPCM_LITTLE);
    dai_setDRAM_PCMFmt(DAI_RX, DAI_DRAMPCM_LITTLE);

    // Set Channel
    AudioCH = pAudio->Channel;
    switch (pAudio->Channel)
    {
    case AUDIO_CH_LEFT:
        dai_setDram_SoundFmt(DAI_TX, DAI_DRAMPCM_MONO);
        dai_setCh(DAI_TX, DAI_CH_MONO_LEFT);
        dai_setDram_SoundFmt(DAI_RX, DAI_DRAMPCM_MONO);
        dai_setCh(DAI_RX, DAI_CH_MONO_LEFT);
        break;

    case AUDIO_CH_RIGHT:
        dai_setDram_SoundFmt(DAI_TX, DAI_DRAMPCM_MONO);
        dai_setCh(DAI_TX, DAI_CH_MONO_RIGHT);
        dai_setDram_SoundFmt(DAI_RX, DAI_DRAMPCM_MONO);
        dai_setCh(DAI_RX, DAI_CH_MONO_RIGHT);
        break;

//    case AUDIO_CH_STEREO:
    default:
        dai_setDram_SoundFmt(DAI_TX, DAI_DRAMPCM_STEREO);
        dai_setCh(DAI_TX, DAI_CH_STEREO);
        dai_setDram_SoundFmt(DAI_RX, DAI_DRAMPCM_STEREO);
        dai_setCh(DAI_RX, DAI_CH_STEREO);
        break;
    }

    // Set FIFO Threshold to AUDIO_THOD_VALUE
    // for TX
    dai_setFIFOThod(DAI_TX, AUDIO_TXTHOD_VALUE);

    // for RX
    dai_setFIFOThod(DAI_RX, AUDIO_TXTHOD_VALUE);

    // Set Clock Source
    // External Clock
    if (pAudio->Clock.bClkExt == TRUE)
    {
        // Set Clock Source to external
        dai_setCodecClkSrc(DAI_CODESCK_EXT);

        // Set audio clock source to external in clkgen module
        pll_setClockRate(PLL_CLKSEL_ADO_CLKSRC, PLL_CLKSEL_ADO_CLKSRC_EXT);
    }
    // Internal clock
    else
    {
        // Set Clock Source to internal
        dai_setCodecClkSrc(DAI_CODESCK_INT);

        // Set audio clock source to internal in clkgen module
        pll_setClockRate(PLL_CLKSEL_ADO_CLKSRC, PLL_CLKSEL_ADO_CLKSRC_INT);

        // Set audio clock rate in CG module
//        pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX, (pAudio->Clock.Clk)<<(PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET));

        // Set audio clock source to internal in CG module
        pll_setClockRate(PLL_CLKSEL_ADO_INTMETHOD, PLL_CLKSEL_ADO_INTMETHOD_MUX);
    }


    // AC97
    if (pAudio->Fmt == AUDIO_FMT_AC97)
    {
        // Set frame format to AC97
        dai_setFrame(DAI_AC97);

        // Configure cold reset pin to output pin and pull high
        gpio_setDir(AudioDeviceObj.uiGPIOColdReset, GPIO_DIR_OUTPUT);
        gpio_setPin(AudioDeviceObj.uiGPIOColdReset);

        // DAI controller act as slave, don't need MCLK
        // External clock source don't require MCLK pin, set to GPIO
        if (pAudio->Clock.bClkExt == TRUE)
        {
            // Set pinmux to disable MCLK output
            pinmux_select_audmclk(PINMUX_GPIO);
        }
        else
        {
            // Set pinmux to enable MCLK output
            pinmux_select_audmclk(PINMUX_AUDIO_MCLK);
        }

        // Set AC97 PinMUX
        // Change PinMUX after Audio initialization to avoid selecting clock source fail problem
        pinmux_select_audio(PINMUX_AUDIO_AC97);
    }
    // I2S
    else
    {
        // Set frame format to I2S
        dai_setFrame(DAI_I2S);

        // Set Master/Slave mode
        // Master Mode
        if (pAudio->I2S.bMaster == TRUE)
        {
            // Embedded codec don't require MCLK pin, set to GPIO
            if (pAudio->bEmbedded == TRUE)
            {
                // Set pinmux to disable MCLK input/output
                pinmux_select_audmclk(PINMUX_GPIO);
            }
            else
            {
                // Set pinmux to enable MCLK input/output
                pinmux_select_audmclk(PINMUX_AUDIO_MCLK);
            }

            // Set Audio to master mode
            dai_setI2SOpMode(DAI_OP_MASTER);
        }
        // Slave Mode
        else
        {
            // Embedded codec or slave mode don't require MCLK pin, set to GPIO
            if (pAudio->Clock.bClkExt == TRUE ||
                pAudio->bEmbedded == TRUE)
            {
                // Set pinmux to disable MCLK output
                pinmux_select_audmclk(PINMUX_GPIO);
            }
            else
            {
                // Set pinmux to enable MCLK output
                pinmux_select_audmclk(PINMUX_AUDIO_MCLK);
            }

            // Set Audio to slave mode
            dai_setI2SOpMode(DAI_OP_SLAVE);
        }

        // Embedded codec don't require these pins, set to GPIO
        if (pAudio->bEmbedded == TRUE)
        {
            pinmux_select_audio(PINMUX_GPIO);
        }
        // Set I2S PinMUX
        // Change PinMUX after Audio initialization to avoid selecting clock source fail problem
        else
        {
            pinmux_select_audio(PINMUX_AUDIO_I2S);
        }

        // Set I2S Clock Ratio
        dai_setI2SClkRatio(pAudio->I2S.ClkRatio);

        // Set I2S format
        switch (pAudio->I2S.I2SFmt)
        {
        case AUDIO_I2SFMT_LIKE_MSB:
            dai_setI2SFmt(DAI_I2SFMT_LIKE_MSB);
            break;

        case AUDIO_I2SFMT_LIKE_LSB:
            dai_setI2SFmt(DAI_I2SFMT_LIKE_LSB);
            break;

//        case AUDIO_I2SFMT_STANDARD:
        default:
            dai_setI2SFmt(DAI_I2SFMT_STANDARD);
            break;
        }
    }

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    // Initialize Codec
    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].init != NULL)
        {
            AudioCodecFunc[i].init(pAudio);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

#if 0
#pragma mark -
#endif

/**
    Select record source

    This function select the record source.
    The result is codec dependent.

    @param[in] RecSrc   Record source, available values are:
        - @b AUDIO_RECSRC_MIC
        - @b AUDIO_RECSRC_CD
        - @b AUDIO_RECSRC_VIDEO
        - @b AUDIO_RECSRC_AUX
        - @b AUDIO_RECSRC_LINE
        - @b AUDIO_RECSRC_STEREO_MIX
        - @b AUDIO_RECSRC_MONO_MIX
        - @b AUDIO_RECSRC_PHONE
    @return void
*/
void aud_setRecordSource(AUDIO_RECSRC RecSrc)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setRecordSource != NULL)
        {
            AudioCodecFunc[i].setRecordSource(RecSrc);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Select output path

    This function select the output path.
    The result is codec dependent.

    @param[in] Output   Output path, available values are below:
        - @b AUDIO_OUTPUT_SPK
        - @b AUDIO_OUTPUT_HP
        - @b AUDIO_OUTPUT_LINE
        - @b AUDIO_OUTPUT_MONO
        - @b AUDIO_OUTPUT_NONE
    @return void
*/
void aud_setOutput(AUDIO_OUTPUT Output)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setOutput != NULL)
        {
            AudioCodecFunc[i].setOutput(Output);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Set sampling rate

    This function set the sampling rate.
    The result is codec dependent.

    @param[in] SamplingRate     Sampling rate, available values are below:
    - @b AUDIO_SR_8000
    - @b AUDIO_SR_11025
    - @b AUDIO_SR_12000
    - @b AUDIO_SR_16000
    - @b AUDIO_SR_22050
    - @b AUDIO_SR_24000
    - @b AUDIO_SR_32000
    - @b AUDIO_SR_44100
    - @b AUDIO_SR_48000
    @return void
*/
void aud_setSamplingRate(AUDIO_SR SamplingRate)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;
    //#NT#2010/01/29#Chris Hsu -end

    if (aud_isOpened() == FALSE)
    {
        return;
    }
    //#NT#2009/10/28#Cliff Lin -begin
    //#NT#Add for resampling
    AudioSR = SamplingRate;

    //#NT#2010/02/02#Chris Hsu -begin
    //#NT#Fix the bug that HDMI can't lock audio when playback if the
    //#NT#sampling rate is not equal to target sampling rate
    if (AudioResampleInfo.bResample == TRUE)
    {
        if (AudioResampleInfo.AudioTargetSR != AudioSR)
        {
            uiAudioResampleInOutRatio = AudioResampleInfo.AudioTargetSR / AudioSR;

            switch (uiAudioResampleInOutRatio)
            {
    //        case 1:
    //        case 2:
            default:
                uiAudioResampleKStart = 0;
                break;

            case 3:
                uiAudioResampleKStart = 2;
                break;

            case 4:
                uiAudioResampleKStart = 5;
                break;

            case 6:
                uiAudioResampleKStart = 9;
                break;
            }
        }
    }
    //#NT#2009/10/28#Cliff Lin -end
    // Set sampling rate immediately when re-sampling is disabled
    else
    {
        //#NT#2010/01/29#Chris Hsu -begin
        //#NT#Support multiple audio codec
        for (i=0; i<(uiAudioExtCodecNum + 1); i++)
        {
            if (AudioCodecFunc[i].setSamplingRate != NULL)
            {
                AudioCodecFunc[i].setSamplingRate(SamplingRate);
            }
        }
        //#NT#2010/01/29#Chris Hsu -end
    }
    //#NT#2010/02/02#Chris Hsu -end
}

/**
    Set channel mode

    This function set channel mode.
    The result is codec dependent.

    @param[in] Channel      Channel mode, available values are below:
        - @b AUDIO_CH_LEFT
        - @b AUDIO_CH_RIGHT
        - @b AUDIO_CH_STEREO
        - @b AUDIO_CH_MONO
    @return void
*/
void aud_setChannel(AUDIO_CH Channel)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }
    AudioCH = Channel;
    switch (Channel)
    {
    //#NT#2009/12/08#Cliff Lin -begin
    //#NT#Add for Audio expand function
    case AUDIO_CH_LEFT:
        dai_setDram_SoundFmt(DAI_TX ,DAI_DRAMPCM_MONO);
        if (bAudioExpandPlay == TRUE)
        {
            dai_setCh(DAI_TX ,DAI_CH_STEREO);
        }
        else
        {
            dai_setCh(DAI_TX ,DAI_CH_MONO_LEFT);
        }

        if (bAudioExpandRec == TRUE)
        {
            dai_setDram_SoundFmt(DAI_RX ,DAI_DRAMPCM_STEREO);
        }
        else
        {
            dai_setDram_SoundFmt(DAI_RX ,DAI_DRAMPCM_MONO);
        }
        dai_setCh(DAI_RX ,DAI_CH_MONO_LEFT);
        break;

    case AUDIO_CH_RIGHT:
        dai_setDram_SoundFmt(DAI_TX ,DAI_DRAMPCM_MONO);
        if (bAudioExpandPlay == TRUE)
        {
            dai_setCh(DAI_TX ,DAI_CH_STEREO);
        }
        else
        {
            dai_setCh(DAI_TX ,DAI_CH_MONO_RIGHT);
        }

        if (bAudioExpandRec == TRUE)
        {
            dai_setDram_SoundFmt(DAI_RX ,DAI_DRAMPCM_STEREO);
        }
        else
        {
            dai_setDram_SoundFmt(DAI_RX ,DAI_DRAMPCM_MONO);
        }
        dai_setCh(DAI_RX ,DAI_CH_MONO_RIGHT);
        break;
    //#NT#2009/12/08#Cliff Lin -end

    //case AUDIO_CH_STEREO:
    default:
        dai_setDram_SoundFmt(DAI_TX ,DAI_DRAMPCM_STEREO);
        dai_setCh(DAI_TX ,DAI_CH_STEREO);
        dai_setDram_SoundFmt(DAI_RX ,DAI_DRAMPCM_STEREO);
        dai_setCh(DAI_RX ,DAI_CH_STEREO);
        break;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setChannel != NULL)
        {
            AudioCodecFunc[i].setChannel(Channel);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Set playback volume

    This function set playback volume
    The result is codec dependent.

    @param[in] Vol      Playback volume, available values are below:
        - @b AUDIO_VOL_0
        - @b AUDIO_VOL_1
        - @b AUDIO_VOL_2
        - @b AUDIO_VOL_3
        - @b AUDIO_VOL_4
        - @b AUDIO_VOL_5
        - @b AUDIO_VOL_6
        - @b AUDIO_VOL_7
        - @b AUDIO_VOL_MUTE
        - @b AUDIO_VOL_UNMUTE
    @return void
*/
void aud_setVolume(AUDIO_VOL Vol)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setVolume != NULL)
        {
            AudioCodecFunc[i].setVolume(Vol);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Set record gain

    This function set record gain.
    The result is codec dependent.

    @param[in] Gain     Record gain, available values are below:
        - @b AUDIO_GAIN_0
        - @b AUDIO_GAIN_1
        - @b AUDIO_GAIN_2
        - @b AUDIO_GAIN_3
        - @b AUDIO_GAIN_4
        - @b AUDIO_GAIN_5
        - @b AUDIO_GAIN_6
        - @b AUDIO_GAIN_7
        - @b AUDIO_GAIN_MUTE
    @return void
*/
void aud_setGain(AUDIO_GAIN Gain)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setGain != NULL)
        {
            AudioCodecFunc[i].setGain(Gain);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Set sound effect

    This function set the sound effect when playback.
    The result is codec dependent.

    @param[in] Effect   Sound effect, available values are below:
        - @b AUDIO_EFFECT_NONE
        - @b AUDIO_EFFECT_3D_HALF
        - @b AUDIO_EFFECT_3D_FULL
        - @b AUDIO_EFFECT_ROCK
        - @b AUDIO_EFFECT_POP
        - @b AUDIO_EFFECT_JAZZ
        - @b AUDIO_EFFECT_CLASSICAL
        - @b AUDIO_EFFECT_DNACE
        - @b AUDIO_EFFECT_HEAVY
        - @b AUDIO_EFFECT_DISCO
        - @b AUDIO_EFFECT_SOFT
        - @b AUDIO_EFFECT_LIVE
        - @b AUDIO_EFFECT_HALL
    @return void
*/
void aud_setEffect(AUDIO_EFFECT Effect)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setEffect != NULL)
        {
            AudioCodecFunc[i].setEffect(Effect);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    set Mixer type

    This function is used to set channel mixer type for Playback.
    The result is codec dependent.

    @param [in] MixerType Mixer type
        - @b AUDIO_MIXER_NORMAL
        - @b AUDIO_MIXER_RLAVG
    @return void
*/
void aud_setMixer(AUDIO_MIXER MixerType)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setMixer != NULL)
        {
            AudioCodecFunc[i].setMixer(MixerType);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Set special feature

    This function set the special feature to the audio codec.
    You should call this function after aud_init() and audio at stop status.

    @param[in] Feature      Audio feature, available values are below:
        - @b AUDIO_FEATURE_ALC:                 Result is codec dependent
        - @b AUDIO_FEATURE_MICBOOST:            Result is codec dependent
        - @b AUDIO_FEATURE_NG_WITH_MICBOOST:    Result is codec dependent
        - @b AUDIO_FEATURE_PLAYBACK_PCM_EXPAND
        - @b AUDIO_FEATURE_RECORD_PCM_EXPAND
        - @b AUDIO_FEATURE_HP_PWR_ALWAYSON
        - @b AUDIO_FEATURE_SPK_PWR_ALWAYSON
        - @b AUDIO_FEATURE_LINE_PWR_ALWAYSON
        - @b AUDIO_FEATURE_OUTPATH_ONLYPWR2ON
    @param[in] bEnable      Enable / Disable the feature
        - @b TRUE:          Enable feature
        - @b FALSE:         Disable feature
    @return void
*/
void aud_setFeature(AUDIO_FEATURE Feature, BOOL bEnable)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;
    //#NT#2010/01/29#Chris Hsu -end

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    switch (Feature)
    {
    case AUDIO_FEATURE_PLAYBACK_PCM_EXPAND:
        bAudioExpandPlay = bEnable;
        break;

    case AUDIO_FEATURE_RECORD_PCM_EXPAND:
        bAudioExpandRec = bEnable;
        break;

    default:
        //#NT#2010/01/29#Chris Hsu -begin
        //#NT#Support multiple audio codec
        for (i=0; i<(uiAudioExtCodecNum + 1); i++)
        {
            if (AudioCodecFunc[i].setFeature != NULL)
            {
                AudioCodecFunc[i].setFeature(Feature, bEnable);
            }
        }
        //#NT#2010/01/29#Chris Hsu -end
        break;
    }
}

/**
    Set I2S format

    This function set I2S format
    The result is codec dependent.

    @param[in] I2SFmt       I2S format, available values are below:
        - @b AUDIO_I2SFMT_STANDARD
        - @b AUDIO_I2SFMT_LIKE_MSB
        - @b AUDIO_I2SFMT_LIKE_LSB
    @return void
*/
void aud_setI2SFormat(AUDIO_I2SFMT I2SFmt)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;
    //#NT#2010/01/29#Chris Hsu -end

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    switch (I2SFmt)
    {
    case AUDIO_I2SFMT_LIKE_MSB:
        dai_setI2SFmt(DAI_I2SFMT_LIKE_MSB);
        break;

    case AUDIO_I2SFMT_LIKE_LSB:
        dai_setI2SFmt(DAI_I2SFMT_LIKE_LSB);
        break;

//    case AUDIO_I2SFMT_STANDARD:
    default:
        dai_setI2SFmt(DAI_I2SFMT_STANDARD);
        break;
    }

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    for (i=0; i<(uiAudioExtCodecNum + 1); i++)
    {
        if (AudioCodecFunc[i].setFormat != NULL)
        {
            AudioCodecFunc[i].setFormat(I2SFmt);
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

/**
    Set I2S clock ratio

    This function set I2S clock ratio
    The result is codec dependent.

    @param[in] I2SCLKRatio      I2S Clock Ratio, available values are below:
        - @b AUDIO_I2SCLKR_256FS_32BIT
        - @b AUDIO_I2SCLKR_256FS_64BIT
        - @b AUDIO_I2SCLKR_384FS_32BIT
        - @b AUDIO_I2SCLKR_384FS_48BIT
        - @b AUDIO_I2SCLKR_384FS_96BIT
        - @b AUDIO_I2SCLKR_512FS_32BIT
        - @b AUDIO_I2SCLKR_512FS_64BIT
        - @b AUDIO_I2SCLKR_512FS_128BIT
        - @b AUDIO_I2SCLKR_768FS_32BIT
        - @b AUDIO_I2SCLKR_768FS_48BIT
        - @b AUDIO_I2SCLKR_768FS_64BIT
        - @b AUDIO_I2SCLKR_768FS_192BIT
        - @b AUDIO_I2SCLKR_1024FS_32BIT
        - @b AUDIO_I2SCLKR_1024FS_64BIT
        - @b AUDIO_I2SCLKR_1024FS_128BIT
        - @b AUDIO_I2SCLKR_1024FS_256BIT
    @return void
*/
void aud_setI2SClockRatio(AUDIO_I2SCLKR I2SCLKRatio)
{
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    // Check if audio format is I2S
    if (dai_getFrame() == DAI_I2S)
    {
        dai_setI2SClkRatio(I2SCLKRatio);

        for (i=0; i<(uiAudioExtCodecNum + 1); i++)
        {
            if (AudioCodecFunc[i].setClockRatio != NULL)
            {
                AudioCodecFunc[i].setClockRatio(I2SCLKRatio);
            }
        }
    }
    //#NT#2010/01/29#Chris Hsu -end
}

//#NT#2009/12/22#Cliff Lin -begin
//#NT#Add for Audio clock control
/**
    Set I2S clock control by open / close or always on

    This function is used to set I2S Clock always on or not

    @param[in] bAudClkAlwaysON      Enable or disable I2S clock always on feature
        - @b TRUE:      I2S clock will be always enabled.
        - @b FALSE:     I2S clock will be enabled or disabled by audio open / close API
    @return Operation status
        - @b TRUE:      This operation is success.
        - @b FALSE:     This operation is fail (audio driver is not opened).
*/
BOOL aud_setI2SClockCtrl(BOOL bAudClkAlwaysON)
{
    if (aud_isOpened() == FALSE)
    {
        return FALSE;
    }

    bAudioClkEn = bAudClkAlwaysON;
    return TRUE;
}
//#NT#2009/12/22#Cliff Lin -end

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio buffer related API                                             */
/*                                                                      */
/************************************************************************/

/**
    Reset Audio buffer queue

    This function reset the audio buffer queue.
    You should call this function before you add any buffer to queue.

    @return void
*/
void aud_resetBufferQueue(void)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    pAudioRealBufferFront   =
    pAudioRealBufferRear    =
    pAudioRealBufferDone    = &AudioRealBufferQueue[0];

    pAudioPseudoBufferFront =
    pAudioPseudoBufferRear  =
    pAudioPseudoBufferDone  = &AudioPseudoBufferQueue[0];

    uiAudioResampleAddrIndex = 0;
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Check Audio buffer queue is full or not

    This function check the audio buffer queue is full or not.

    @return Audio buffer full or not
        - @b TRUE:  buffer queue is full
        - @b FALSE: buffer queue is not full
*/
BOOL aud_isBufferQueueFull(void)
{
    if (aud_isOpened() == FALSE)
    {
        return FALSE;
    }
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    return (BOOL)(pAudioRealBufferFront->pNext == pAudioRealBufferRear);
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Add buffer to Audio buffer queue

    This function add buffer to Audio buffer queue.
    Please keep at least 3 buffers in the queue if you still want to add more buffers.

    @param[in] pAudioBufQueue       The pointer of queue structure
        You have to assign the uiAddress (Word alignment) and uiSize (Byte)
        value in the structure. Both uiAddress and uisize must be word alignment.
    @return Status
        - @b TRUE: buffer added
        - @b FALSE: buffer queue is full or audio is not opened
*/
BOOL aud_addBufferToQueue(PAUDIO_BUF_QUEUE pAudioBufQueue)
{
    UINT32 uiMaxBufSize;

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    if (aud_isOpened() == FALSE)
    {
        return FALSE;
    }
    // Queue is Full
    if (pAudioRealBufferFront->pNext == pAudioRealBufferRear)
    {
        return FALSE;
    }
    // For pseudo operation using(upper layer using)
    pAudioPseudoBufferFront->uiAddress    = pAudioBufQueue->uiAddress;
    pAudioPseudoBufferFront->uiSize       = pAudioBufQueue->uiSize;
    pAudioPseudoBufferFront               = pAudioPseudoBufferFront->pNext;

    // For real operation using(audio driver operation using)
    if ((AudioResampleInfo.bResample == TRUE) &&
        (AudioResampleInfo.AudioTargetSR != AudioSR) &&
        (dai_isDAIEnable(DAI_RX) == FALSE))
    {
        // Check audio buffer size
        if (AudioCH == AUDIO_CH_STEREO)
        {
            uiMaxBufSize = AudioSR << 2;
        }
        else
        {
            uiMaxBufSize = AudioSR << 1;
        }

        if (pAudioBufQueue->uiSize > uiMaxBufSize)
        {
            debug_err(("Exceed maximum re-sampling one buffer size\r\n"));
            return FALSE;
        }

        // Re-sampling
        aud_resample((UINT32)pAudioBufQueue->uiAddress, (UINT32)pAudioBufQueue->uiSize, &(pAudioRealBufferFront->uiAddress), &(pAudioRealBufferFront->uiSize));
    }
    else
    {
        pAudioRealBufferFront->uiAddress    = pAudioBufQueue->uiAddress;
        pAudioRealBufferFront->uiSize       = pAudioBufQueue->uiSize;
    }
    pAudioRealBufferFront                   = pAudioRealBufferFront->pNext;

    return TRUE;
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Get done buffer from Audio buffer queue

    This function get done buffer from Audio buffer queue.

    @return Done buffer or NULL if there is no available done buffer
*/
PAUDIO_BUF_QUEUE aud_getDoneBufferFromQueue(void)
{
    PAUDIO_BUF_QUEUE    pRetQueue;

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    if (aud_isOpened() == FALSE)
    {
        return NULL;
    }

    if (pAudioRealBufferDone == NULL)
    {
        return NULL;
    }

    // Done Queue is empty
    if (pAudioRealBufferDone == pAudioRealBufferRear)
    {
        // Audio is NOT stopped
        if ((dai_isDAIEnable(DAI_TX) == TRUE) ||
            (dai_isDAIEnable(DAI_RX) == TRUE))
        {
            return NULL;
        }
        // Audio is stopped
        else
        {
            pRetQueue               = pAudioPseudoBufferDone;
            pAudioPseudoBufferDone  = NULL;
            pAudioRealBufferDone    = NULL;
        }
    }
    else
    {
        pRetQueue                   = pAudioPseudoBufferDone;
        // Move done queue to next
        pAudioPseudoBufferDone      = pAudioPseudoBufferDone->pNext;
        pAudioRealBufferDone        = pAudioRealBufferDone->pNext;
    }
    // Return the queue
    return pRetQueue;
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Get current buffer from Audio buffer queue

    This function get current buffer that Audio is accessing from the buffer queue.

    @return Current buffer
*/
PAUDIO_BUF_QUEUE aud_getCurrentBufferFromQueue(void)
{
    if (aud_isOpened() == FALSE)
    {
        return NULL;
    }
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    if (AudioMode == AUDIO_RX)
    {
        pAudioPseudoBufferRear->uiValidSize =
        pAudioRealBufferRear->uiValidSize   = dai_getDMACurrAdr(DAI_RX) - pAudioRealBufferRear->uiAddress;
    }
    return pAudioPseudoBufferRear;
    //#NT#2009/12/22#Cliff Lin -end
}

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio Playback/Record Flow related API                               */
/*                                                                      */
/************************************************************************/

/**
    Stop playback/record

    This function stop playback or record

    @return void
*/
void aud_stop(void)
{
    //#NT#2010/02/02#Chris Hsu -begin
    //#NT#Fix the bug that HDMI can't lock audio when playback if the
    //#NT#sampling rate is not equal to target sampling rate
    UINT32 i;
    //#NT#2010/02/02#Chris Hsu -end

    // Prevent calling aud_stop() multiple times
    if (AudioLockStatus == NO_TASK_LOCKED || ((dai_isDMAEnable(DAI_TX) == FALSE) && (dai_isDMAEnable(DAI_RX) == FALSE)))
    {
        return;
    }

    // Disable Audio Interrupt
    dai_setIntCtrl(DAI_DISABLE_ALL_INT);

    // Clear all interrupt status, and interrupt flag
    dai_clrIntStatus(DAI_CLEAR_ALL_INT_STS);

    dai_clrFlg();

    // Diable Audio
    dai_enableDAI(DAI_TX, FALSE);
    dai_enableDAI(DAI_RX, FALSE);

    // Wait for Audio really stopped
    while (dai_isBusy() == TRUE)
    {
        ;
    }

    // Disable DMA
    dai_enableDMA(DAI_TX, FALSE);
    dai_enableDMA(DAI_RX, FALSE);

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    // Stop Codec
    if (AudioCodecFunc[AudioSelectedCodec].stop != NULL)
    {
        AudioCodecFunc[AudioSelectedCodec].stop();
    }
    //#NT#2010/01/29#Chris Hsu -end

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Use new define for audio operation mode
    if (AudioMode == AUDIO_RX)
    //#NT#2009/12/22#Cliff Lin -end
    {
        // Save valid PCM data size of last buffer in record mode
        //#NT#2009/12/22#Cliff Lin -begin
        //#NT#Fix audio queue operation for audio resampling
        pAudioPseudoBufferRear->uiValidSize =
        pAudioRealBufferRear->uiValidSize   = dai_getDMACurrAdr(DAI_RX) - pAudioRealBufferRear->uiAddress;
        //#NT#2009/12/22#Cliff Lin -end

        //#NT#2010/02/02#Chris Hsu -begin
        //#NT#Fix the bug that HDMI can't lock audio when playback if the
        //#NT#sampling rate is not equal to target sampling rate
        // Set to target sampling rate to let HDMI sink can lock audio
        if ((AudioResampleInfo.bResample == TRUE) &&
            (AudioResampleInfo.AudioTargetSR != AudioSR))
        {
            for (i=0; i<(uiAudioExtCodecNum + 1); i++)
            {
                if (AudioCodecFunc[i].setSamplingRate != NULL)
                {
                    AudioCodecFunc[i].setSamplingRate(AudioResampleInfo.AudioTargetSR);
                }
            }
        }
        //#NT#2010/02/02#Chris Hsu -end
    }
}

/**
    Start to record

    This function start to record.\n

    If bAVSync is TRUE, audio will start to record after receiving SIE VD signal,
    otherwise audio will start to record immediately.\n

    If bTCLatchEvent is TRUE, audio will issue AUDIO_EVENT_TCLATCH event.
    If bTCHitEvent is TRUE, audio will issue AUDIO_EVENT_TCHIT event.

    Audio will update the timecode value when receving VD signal from SIE or
    stopped. When internal timecode counter matches timecode trigger value,
    audio will issue AUDIO_EVENT_TCHIT event. Timecode counter is the audio
    sample count that audio record.

    The AUDIO_EVENT_DMADONE, AUDIO_EVENT_FIFO_ERROR, AUDIO_EVENT_BUF_FULL
    and AUDIO_EVENT_DONEBUF_FULL events will be always issue if condition occurs.

    @param[in] bAVSync          Enable Audio's AVSync function or not
        - @b TRUE:  Enable AVSync function
        - @b FALSE: Disable AVSync function
    @param[in] bTCLatchEvent    Issue timecode latch event or not
        - @b TRUE:  Issue timecode latch event
        - @b FALSE: Don't issue timecode latch event
    @param[in] bTCHitEvent      Issue timecode hit event or not
        - @b TRUE:  Issue timecode hit event
        - @b FALSE: Don't issue timecode hit event
    @return void
*/
void aud_record(BOOL bAVSync, BOOL bTCLatchEvent, BOOL bTCHitEvent)
{
    //#NT#2010/02/02#Chris Hsu -begin
    //#NT#Fix the bug that HDMI can't lock audio when playback if the
    //#NT#sampling rate is not equal to target sampling rate
    UINT32 uiIntCtrlReg, i;
    //#NT#2010/02/02#Chris Hsu -end

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    //#NT#2010/02/02#Chris Hsu -begin
    //#NT#Fix the bug that HDMI can't lock audio when playback if the
    //#NT#sampling rate is not equal to target sampling rate
    // If re-sampling is enabled, aud_setSamplingRate() won't set sampling rate,
    // We have to set correct sampling when start to record
    if ((AudioResampleInfo.bResample == TRUE) &&
        (AudioResampleInfo.AudioTargetSR != AudioSR))
    {
        for (i=0; i<(uiAudioExtCodecNum + 1); i++)
        {
            if (AudioCodecFunc[i].setSamplingRate != NULL)
            {
                AudioCodecFunc[i].setSamplingRate(AudioSR);
            }
        }
    }
    //#NT#2010/02/02#Chris Hsu -end

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    // Enable Codec to Record
    if (AudioCodecFunc[AudioSelectedCodec].record != NULL)
    {
        AudioCodecFunc[AudioSelectedCodec].record();
    }
    //#NT#2010/01/29#Chris Hsu -end

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix resampling issue at recording
    pAudioRealBufferRear    = pAudioPseudoBufferRear;
    pAudioRealBufferDone    = pAudioPseudoBufferDone;
    pAudioRealBufferFront   = pAudioPseudoBufferFront;
    //#NT#2009/12/22#Cliff Lin -end

    // Enable/Disable Audio's AVSync bit
    dai_enableAVSync(bAVSync);

    // Enable specific interrupt
    uiIntCtrlReg = DAI_RXDMADONE_INT_EN | DAI_RXFIFO_ERROR_INT_EN;

    if (bTCLatchEvent == TRUE)
    {
        uiIntCtrlReg |= DAI_RXTCLATCH_INT_EN;
    }

    if (bTCHitEvent == TRUE)
    {
        uiIntCtrlReg |= DAI_RXTCHIT_INT_EN;
    }
    //#NT#2009/12/22#Cliff Lin -begin
    // Set Audio operation mode to "receive" (Record) mode
    AudioMode = AUDIO_RX;
    //#NT#2009/12/22#Cliff Lin -end

    // Clear FIFO
    dai_clrFIFO(DAI_RX);

    // Clear interrupt status, and interrupt flag
    dai_clrIntStatus(DAI_CLEAR_ALL_INT_STS);

    dai_clrFlg();

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    // Set DMA starting address and size
    dai_setDMAPara(DAI_RX, pAudioRealBufferRear->uiAddress, pAudioRealBufferRear->uiSize, FALSE );

    // Enable DMA
    dai_enableDMA(DAI_RX, TRUE);

    // Enable Audio
    dai_enableDAI(DAI_RX, TRUE);

    // Wait for DMA transfer start
    while (dai_getDMACurrAdr(DAI_RX) == pAudioRealBufferRear->uiAddress)
    {
        ;
    }

    // Enable Interrupt
    dai_setIntCtrl(uiIntCtrlReg);

    // Set next DMA starting address and size
    if (pAudioRealBufferRear->pNext != pAudioRealBufferFront)
    {
        dai_setDMAPara(DAI_RX, pAudioRealBufferRear->pNext->uiAddress, pAudioRealBufferRear->pNext->uiSize, FALSE );
    }
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Start to playback

    This function start to playback.\n

    If bTCLatchEvent is TRUE, audio will issue AUDIO_EVENT_TCLATCH event.
    If bTCHitEvent is TRUE, audio will issue AUDIO_EVENT_TCHIT event.\n

    Audio will update the timecode value when receving VD signal from SIE or
    stopped. When internal timecode counter matches timecode trigger value,
    audio will issue AUDIO_EVENT_TCHIT event. Timecode counter is the audio
    sample count that audio playback.\n

    The AUDIO_EVENT_DMADONE, AUDIO_EVENT_FIFO_ERROR, AUDIO_EVENT_BUF_EMPTY
    and AUDIO_EVENT_DONEBUF_FULL events will be always issue if condition occurs.

    @param[in] bTCLatchEvent    Issue timecode latch event or not
        - @b TRUE:  Issue timecode latch event
        - @b FALSE: Don't issue timecode latch event
    @param[in] bTCHitEvent      Issue timecode hit event or not
        - @b TRUE:  Issue timecode hit event
        - @b FALSE: Don't issue timecode hit event
    @return void
*/
void aud_playback(BOOL bTCLatchEvent, BOOL bTCHitEvent)
{
    UINT32 uiIntCtrlReg;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    // Enable Codec to playback
    if (AudioCodecFunc[AudioSelectedCodec].playback != NULL)
    {
        AudioCodecFunc[AudioSelectedCodec].playback();
    }
    //#NT#2010/01/29#Chris Hsu -end

    //#NT#2009/11/05#Cliff Lin -begin
    //#NT#Add for resampling
    if ((AudioResampleInfo.bResample == TRUE) &&
        (AudioResampleInfo.AudioTargetSR != AudioSR))
    {
        if ((AudioResampleInfo.AudioTargetSR % AudioSR) != 0)
        {
            debug_err(("Unsupported Resampling Ratio, Target %d, Set %d\r\n", AudioResampleInfo.AudioTargetSR, AudioSR));
        }

        switch (uiAudioResampleInOutRatio)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
            break;

        default:
            debug_err(("Unsupported Resampling Ratio, Target %d, Set %d\r\n", AudioResampleInfo.AudioTargetSR, AudioSR));
            break;
        }

        //#NT#2010/02/02#Chris Hsu -begin
        //#NT#Fix the bug that HDMI can't lock audio when playback if the
        //#NT#sampling rate is not equal to target sampling rate
        // aud_setResampleInfo() already set target sampling rate.
        // These code can be removed
        /*
        for (i=0; i<(uiAudioExtCodecNum + 1); i++)
        {
            if (AudioCodecFunc[i].setSamplingRate != NULL)
            {
                AudioCodecFunc[i].setSamplingRate(AudioResampleInfo.AudioTargetSR);
            }
        }
        */
        //#NT#2010/02/02#Chris Hsu -end
    }
    //#NT#2009/11/05#Cliff Lin -end

    // Enable Audio DMA clock
    //pll_enableClock(ADO_DMACLK);

    // Enable specific interrupt
    uiIntCtrlReg = DAI_TXDMADONE_INT_EN | DAI_TXFIFO_ERROR_INT_EN;

    if (bTCLatchEvent == TRUE)
    {
        uiIntCtrlReg |= DAI_RXTCLATCH_INT_EN;
    }

    if (bTCHitEvent == TRUE)
    {
        uiIntCtrlReg |= DAI_TXTCHIT_INT_EN;
    }

    // Disable AVSync bit
    dai_enableAVSync(FALSE);

    //#NT#2009/12/22#Cliff Lin -begin
    // Set Audio operation mode to "Transmit" (Playback) mode
    AudioMode = AUDIO_TX;
    //#NT#2009/12/22#Cliff Lin -end

    // Clear FIFO
    dai_clrFIFO(DAI_TX);

    // Clear interrupt status, and interrupt flag
    dai_clrIntStatus(DAI_CLEAR_ALL_INT_STS);
    dai_clrFlg();

    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Fix audio queue operation for audio resampling
    // Set DMA starting address and size
    dai_setDMAPara(DAI_TX,pAudioRealBufferRear->uiAddress, pAudioRealBufferRear->uiSize, FALSE );

    // Enable DMA
    dai_enableDMA(DAI_TX,TRUE);

    // Wait for DMA transfer start
    while (dai_getDMACurrAdr(DAI_TX) == pAudioRealBufferRear->uiAddress)
    {
        ;
    }

    // Enable Interrupt
    dai_setIntCtrl(uiIntCtrlReg);

    // Enable Audio
    dai_enableDAI(DAI_TX, TRUE);

    // Set next DMA starting address and size
    if (pAudioRealBufferRear->pNext != pAudioRealBufferFront)
    {
        dai_setDMAPara(DAI_TX, pAudioRealBufferRear->pNext->uiAddress, pAudioRealBufferRear->pNext->uiSize, FALSE );
    }
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Pause audio module

    This function pause audio module

    @return void
*/
void aud_pause(void)
{

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    // Disable Audio
    if (AudioMode == AUDIO_TX)
    {
        dai_enableDAI(DAI_TX, FALSE);
    }
    else
    {
        dai_enableDAI(DAI_RX, FALSE);
    }

    // Wait for Audio really stopped
    while (dai_isBusy() == TRUE )
    {
        ;
    }

    dai_setTimecodeOffset(DAI_TX, dai_getTimecodeValue(DAI_TX));
}

/**
    Resume audio module

    This function resume audio module

    @return void
*/
void aud_resume(void)
{

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    // Enable Audio
    if (AudioMode == AUDIO_TX)
    {
        dai_enableDAI(DAI_TX, TRUE);
    }
    else
    {
        dai_enableDAI(DAI_RX, TRUE);
    }
}

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio timecode related API                                           */
/*                                                                      */
/************************************************************************/

/**
    Set timecode trigger value

    This function set timecode trigger value.
    Timecode is the sample count while record/playback.
    Audio will issue the timecode hit interrupt when sample count matches
    the timecode trigger value if the timecode hit interrupt is enabled.

    @note   Please call aud_setSamplingRate() before this API to make sure
            playback timecode work correctly.

    @param[in] uiTrigger    Timecode trigger value
    @return void
*/
void aud_setTimecodeTrigger(UINT32 uiTrigger)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }
    //#NT#2009/11/05#Cliff Lin -begin
    //#NT#Add for resampling
    if ((AudioResampleInfo.bResample == TRUE) &&
        (AudioResampleInfo.AudioTargetSR != AudioSR))
    {
        dai_setTimecodeTrigger(DAI_TX, uiTrigger * (AudioResampleInfo.AudioTargetSR / AudioSR));
    }
    else
    {
        dai_setTimecodeTrigger(DAI_TX, uiTrigger);
    }
    dai_setTimecodeTrigger(DAI_RX, uiTrigger);
    //#NT#2009/11/05#Cliff Lin -end
}

/**
    Set timecode offset value

    This function set timecode offset value.
    Timecode is the sample count while record/playback.
    Audio will latch the offset value into its counter as the timecode
    initial value when start to record/playback.

    @note   Please call aud_setSamplingRate() before this API to make sure
            playback timecode offset work correctly.

    @param[in] uiTrigger    Timecode offset value
    @return void
*/
void aud_setTimecodeOffset(UINT32 uiOffset)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Add for audio resampling
    if ((AudioResampleInfo.bResample == TRUE) &&
        (AudioResampleInfo.AudioTargetSR != AudioSR))
    {
        dai_setTimecodeOffset(DAI_TX, uiOffset * (AudioResampleInfo.AudioTargetSR / AudioSR));
    }
    else
    {
        dai_setTimecodeOffset(DAI_TX, uiOffset);
    }
    //#NT#2009/12/22#Cliff Lin -end
    dai_setTimecodeOffset(DAI_RX, uiOffset);
}

/**
    Get timecode value

    This function get timecode value.
    Timecode is the sample count while record/playback.
    Timecode value only updated when received VD signal from SIE

    @note   Please call aud_setSamplingRate() before this API to make sure
            playback timecode work correctly.

    @return Timecode value
*/
UINT32 aud_getTimecodeValue(void)
{
    if (aud_isOpened() == FALSE)
    {
        return 0;
    }
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Add for audio resampling
    if (AudioMode == AUDIO_TX)
    {
        if ((AudioResampleInfo.bResample == TRUE) &&
            (AudioResampleInfo.AudioTargetSR != AudioSR))
        {
            return (dai_getTimecodeValue(DAI_TX) / (AudioResampleInfo.AudioTargetSR / AudioSR));

        }
        else
        {
            return dai_getTimecodeValue(DAI_TX);
        }
    }
    //#NT#2009/12/22#Cliff Lin -end
    else // mode == AUDIO_RX
    {
        return dai_getTimecodeValue(DAI_RX);
    }
}

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio mode related API                                               */
/*                                                                      */
/************************************************************************/

/**
    Check operation mode

    This function check audio operation mode.

    @return Playback or record mode
        - @b TRUE:  Playback mode
        - @b FALSE: Record mode
*/
BOOL aud_isPlaybackMode(void)
{
    //#NT#2009/12/22#Cliff Lin -begin
    //#NT#Use new define for audio operation mode
    if(AudioMode == AUDIO_TX)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    //#NT#2009/12/22#Cliff Lin -end
}

/**
    Check whether the Audio module is busy recording/playbacking or not

    This function check whether the Audio module is busy recording/playbacking or not.

    @return Audio module is busy or not
        - @b TRUE:  Busy recording or playbacking
        - @b FALSE: Idle or wait for VD from SIE to start recording
*/
BOOL aud_isBusy(void)
{
    return dai_isBusy();
}

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio AC97 related API                                               */
/*                                                                      */
/************************************************************************/

/**
    Send command to AC97 Codec

    This function send command to AC97 Codec

    @param[in] bWriteCmd    Write or read command
        - @b TRUE:  Write command
        - @b FALSE: Read command
    @param[in] uiRegIdx     The AC97 register index value (0x00 ~ 0x7F)
    @param[in] uiData       AC97 register write data (0 if it's a read command)
    @return void
*/
void aud_sendAC97Command(BOOL bWriteCmd, UINT32 uiRegIdx, UINT32 uiData)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }

    dai_sendAC97Command(bWriteCmd, uiRegIdx, uiData);
}

/**
    Read AC97 Codec respond data

    This function read AC97 respond data

    @param[out] uiRegIdx    The AC97 register index value
    @param[out] uiData      AC97 register data
    @return void
*/
void aud_readAC97Data(UINT32 *puiRegIdx, UINT32 *puiData)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }

    dai_readAC97Data(puiRegIdx, puiData);
}

/**
    Do AC97 cold reset

    This function do AC97 cold reset

    @return void
*/
void aud_coldresetAC97(void)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }

    // Pull Cold Reset pin low
    gpio_clearPin(AudioDeviceObj.uiGPIOColdReset);

    // Do at least 1us delay, here we do 1ms delay
    TimerDelayMs(1);

    // Pull Cold Reset pin high
    gpio_setPin(AudioDeviceObj.uiGPIOColdReset);
}

/**
    Do AC97 warm reset

    This function do AC97 warm reset

    @return void
*/
void aud_warmresetAC97(void)
{
    if (aud_isOpened() == FALSE)
    {
        return;
    }

    dai_warmresetAC97();
}

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio I2S codec command API                                          */
/*                                                                      */
/************************************************************************/

/**
    Send command to I2S codec

    Send command to I2S codec.

    @param[in] uiRegIdx     I2S codec register index
    @param[in] uiData       Data you want to send
    @return void
*/
BOOL aud_sendI2SCommand(UINT32 uiRegIdx, UINT32 uiData)
{
    if (aud_isOpened() == FALSE)
    {
        return FALSE;
    }

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    if (AudioCodecFunc[AudioSelectedCodec].sendCommand != NULL && aud_isOpened() == TRUE)
    {
        return AudioCodecFunc[AudioSelectedCodec].sendCommand(uiRegIdx, uiData);
    //#NT#2010/01/29#Chris Hsu -end
    }
    else
    {
        return FALSE;
    }
}

/**
    Read register data from I2S codec

    Read register data from I2S codec.

    @param[in] uiRegIdx     I2S codec register index you want to read
    @param[out] puiData     Register data
    @return Read data status
        - @b TRUE:  Read data OK
        - @b FALSE: Read data fail
*/
BOOL aud_readI2SData(UINT32 uiRegIdx, UINT32 *puiData)
{
    if (aud_isOpened() == FALSE)
    {
        *puiData = 0;
        return FALSE;
    }

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    if (AudioCodecFunc[AudioSelectedCodec].readData != NULL && aud_isOpened() == TRUE)
    {
        return AudioCodecFunc[AudioSelectedCodec].readData(uiRegIdx, puiData);
    //#NT#2010/01/29#Chris Hsu -end
    }
    else
    {
        *puiData = 0;
        return FALSE;
    }
}

#if 0
#pragma mark -
#endif

/************************************************************************/
/*                                                                      */
/* Audio Misc API                                                       */
/*                                                                      */
/************************************************************************/

/**
    Enable audio interrupts

    This function enable audio event. This function must be called after aud_record()
    or aud_playback().

    @param[in] uiEvent  EventID combination that you want to enable. Only support the following now:
        - @b AUDIO_EVENT_TCLATCH:   Timecode latch
        - @b AUDIO_EVENT_TCHIT:     Timecode hit
    @return void
*/
void aud_enableEvent(UINT32 uiEvent)
{
    UINT32 uiIntCtrlReg = 0x00000000;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    if (uiEvent & AUDIO_EVENT_TCLATCH)
    {
        uiIntCtrlReg |= DAI_RXTCLATCH_INT_EN;
    }

    if (uiEvent & AUDIO_EVENT_TCHIT)
    {
        uiIntCtrlReg |= DAI_TXTCHIT_INT_EN;
    }

    dai_setIntCtrl(uiIntCtrlReg);
}

/**
    Disable audio interrupts

    This function disable audio event. This function must be called after aud_record()
    or aud_playback().

    @param UINT32 uiEvent: EventID combination that you want to disable. Only support the following now:
        - @b AUDIO_EVENT_TCLATCH:   Timecode latch
        - @b AUDIO_EVENT_TCHIT:     Timecode hit
    @return void
*/
void aud_disableEvent(UINT32 uiEvent)
{

    UINT32 uiIntCtrlReg = 0x00000000;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    if (uiEvent & AUDIO_EVENT_TCLATCH)
    {
        uiIntCtrlReg &= ~DAI_RXTCLATCH_INT_EN;
    }

    if (uiEvent & AUDIO_EVENT_TCHIT)
    {
        uiIntCtrlReg &= ~DAI_TXTCHIT_INT_EN;
    }

    dai_setIntCtrl(uiIntCtrlReg);
}

//#NT#2010/01/29#Chris Hsu -begin
//#NT#Support multiple audio codec
/*
    Set extended audio codec

    Set extended audio codec. This function should be called once before aud_init().

    @param[in] AudioCodec           Codec ID, only support AUDIO_CODECSEL_EXT_0 now
        - @b AUDIO_CODECSEL_EXT_0   Extended audio codec 0
    @param[in] pAudioCodecFunc      Audio codec function pointer
    @return void
*/
void aud_setExtCodec(AUDIO_CODECSEL AudioCodec, PAUDIO_CODEC_FUNC pAudioCodecFunc)
{
    //#NT#2010/04/20#Chris Hsu[0010194] -begin
    //#NT#Fix recording muted audio when HDMI is pluged in
    UINT32 i;
    UINT32 *pSrc, *pDst, *pDefault;

    // Check if we support the extended audio codec
    if (AudioCodec != AUDIO_CODECSEL_EXT_0)
    {
        debug_err(("Unsupported extended audio codec\r\n"));
        return;
    }

    // Get function pointer for default codec
    // Only support AUDIO_CODECSEL_EXT_0, mark to reduce code size
//    if (uiAudioExtCodecNum == AUDIO_CODECSEL_DEFAULT)
    {
        audcodec_getFP(&AudioCodecFunc[AUDIO_CODECSEL_DEFAULT]);
    }

    // Assign extended codec number
    // Only support AUDIO_CODECSEL_EXT_0, mark to reduce code size
//    if (uiAudioExtCodecNum < AudioCodec)
    {
        uiAudioExtCodecNum = AudioCodec;
    }

    // Copy function pointer
//    memcpy((void *)&AudioCodecFunc[AudioCodec], (const void *)pAudioCodecFunc, sizeof(AUDIO_CODEC_FUNC));
    pDefault    = (UINT32 *)&(AudioCodecFunc[AUDIO_CODECSEL_DEFAULT]);
    // Only support AUDIO_CODECSEL_EXT_0, mark to reduce code size
//    pDst        = (UINT32 *)&(AudioCodecFunc[AudioCodec]);
    pDst        = (UINT32 *)&(AudioCodecFunc[AUDIO_CODECSEL_EXT_0]);
    pSrc        = (UINT32 *)pAudioCodecFunc;
    for (i=0; i<(sizeof(AUDIO_CODEC_FUNC)>>2); i++)
    {
        // Copy default function pointer to destination
        if (*(pSrc + i) == AUDIO_CODEC_FUNC_USE_DEFAULT)
        {
            *(pDst + i) = *(pDefault + i);
        }
        // Copy extended function pointer to destination
        else
        {
            *(pDst + i) = *(pSrc + i);
        }
    }
    //#NT#2010/04/20#Chris Hsu[0010194] -end
}

/*
    Switch audio codec

    Switch audio codec.

    @param[in] AudioCodec           Codec ID
        - @b AUDIO_CODECSEL_DEFAULT Default audio codec
        - @b AUDIO_CODECSEL_EXT_0   Extended audio codec 0
    @return void
*/
void aud_switchCodec(AUDIO_CODECSEL AudioCodec)
{
    AUDIO_CODECSEL  AudioPreSelectedCodec;

    if (aud_isOpened() == FALSE)
    {
        return;
    }

    // Save previous codec
    AudioPreSelectedCodec   = AudioSelectedCodec;
    // Assign new codec
    AudioSelectedCodec      = AudioCodec;

    // Playing
    if (dai_isDAIEnable(DAI_TX) == TRUE)
    {
        //#NT#2010/04/20#Chris Hsu -begin
        //#NT#Try to fix default codec and extended codec share same function problem
        //#NT#Might still have some problem...
        // If new codec's playback function != previous codec's playback function
        // then inform new codec to playback
        if ((AudioCodecFunc[AudioSelectedCodec].playback != NULL) &&
            (AudioCodecFunc[AudioSelectedCodec].playback != AudioCodecFunc[AudioPreSelectedCodec].playback))
        {
            AudioCodecFunc[AudioSelectedCodec].playback();
        }

        // If new codec's stop function != previous codec's stop function
        // then stop previous codec
        if ((AudioCodecFunc[AudioPreSelectedCodec].stop != NULL) &&
            (AudioCodecFunc[AudioPreSelectedCodec].stop != AudioCodecFunc[AudioSelectedCodec].stop))
        {
            AudioCodecFunc[AudioPreSelectedCodec].stop();
        }
    }

    // Recording
    if (dai_isDAIEnable(DAI_RX) == TRUE)
    {
        // If new codec's record function != previous codec's record function
        // then inform new codec to record
        if ((AudioCodecFunc[AudioSelectedCodec].record != NULL) &&
            (AudioCodecFunc[AudioSelectedCodec].record != AudioCodecFunc[AudioPreSelectedCodec].record))
        //#NT#2010/04/20#Chris Hsu -end
        {
            AudioCodecFunc[AudioSelectedCodec].record();

            // Only stop previous codec if new codec support record function
            if (AudioCodecFunc[AudioPreSelectedCodec].stop != NULL)
            {
                AudioCodecFunc[AudioPreSelectedCodec].stop();
            }
        }
    }
}
//#NT#2010/01/29#Chris Hsu -end

//#NT#2009/11/05#Cliff Lin -begin
//#NT#Add for resampling
/**
    Set playback re-sampling information

    Enable or disable audio playback re-sampling function.

    @note   This function should be called at stop status

    @param[in] pResampleInfo  Information of audio re-sampling
    @return Set re-sampling status
        - @b TRUE:      Operation is successful
        - @b FALSE:     Invalid parameters
*/
BOOL aud_setResampleInfo(PAUDIO_RESAMPLE_INFO pResampleInfo)
{
    UINT32 i;

    if (aud_isOpened() == FALSE)
    {
        return FALSE;
    }

    if (pResampleInfo->bResample == TRUE)
    {
        if (dai_isDAIEnable(DAI_TX) == TRUE)
        {
            debug_err(("Set Information should be at stop status!"));
            return FALSE;
        }
        //#NT#2010/02/02#Chris Hsu -begin
        //#NT#Fix the bug that HDMI can't lock audio when playback if the
        //#NT#sampling rate is not equal to target sampling rate
        // Set target sampling rate to audio codec
        for (i=0; i<(uiAudioExtCodecNum + 1); i++)
        {
            if (AudioCodecFunc[i].setSamplingRate != NULL)
            {
                AudioCodecFunc[i].setSamplingRate(pResampleInfo->AudioTargetSR);
            }
        }
        //#NT#2010/02/02#Chris Hsu -end

        // Buffer size should be considered max bit rate and stereo sound type buffer
        if (pResampleInfo->uiDstBufSize < ((AUDIO_BUF_BLKNUM - 1) * (AUDIO_SR_48000 << 2)))
        {
            debug_err(("Resampling Buffer is too small!"));
            return FALSE;
        }

        memcpy((void *)&AudioResampleInfo, (const void *)pResampleInfo, sizeof(AUDIO_RESAMPLE_INFO));

        for(i=0; i<(AUDIO_BUF_BLKNUM-1); i++)
        {
            uiAudioResampleAddr[i] = pResampleInfo->uiDstBufAddr + (i * (pResampleInfo->AudioTargetSR << 2));
        }

        uiAudioResampleAddrIndex = 0;
    }
    else
    {
        AudioResampleInfo.bResample = FALSE;
    }

    return TRUE;
}
//#NT#2009/11/05#Cliff Lin -end

//@}
