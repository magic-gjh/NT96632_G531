/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyADC.c
    @ingroup    mIPRJAPKeyIO

    @brief      Key detection by using ADC
                Key detection by using ADC

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "GPIOMapping.h"
#include "KeyScanTsk.h"
#include "KeyADC.h"
#include "ADC.h"

// ADC channel for key group 1 detection
#define KEYADC_CH_0             ADC_CHANNEL_0
#define KEYADC_CH_1             ADC_CHANNEL_1

// ADC level for key detection
#define KEYADC_LEVEL_0          440
#define KEYADC_LEVEL_1          555
#define KEYADC_LEVEL_2          675
#define KEYADC_LEVEL_3          790
#define KEYADC_LEVEL_4          900

/**
    Judge key code by reading value from ADC

    Judge key code by reading value from ADC

    @return Key code (FLGKEY_XXXX, please refer to KeyScanTsk.h)
*/
UINT32 KeyADC_GetKey(void)
{
    UINT32 uiData, uiKeyCode;

    // Read channel 0
    uiData = adc_readData(KEYADC_CH_0);

    if (uiData > KEYADC_LEVEL_4)
    {
        uiKeyCode = FLGKEY_UP;
    }
    else if (uiData > KEYADC_LEVEL_3)
    {
        uiKeyCode = FLGKEY_DOWN;
    }
    else if (uiData > KEYADC_LEVEL_2)
    {
        uiKeyCode = FLGKEY_LEFT;
    }
    else if (uiData > KEYADC_LEVEL_1)
    {
        uiKeyCode = FLGKEY_RIGHT;
    }
    else if (uiData > KEYADC_LEVEL_0)
    {
        uiKeyCode = FLGKEY_ENTER;
    }
    else
    {
        uiKeyCode = 0;
    }

    // Read channel 1
    uiData = adc_readData(KEYADC_CH_1);

    if (uiData > KEYADC_LEVEL_4)
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
    }
    else if (uiData > KEYADC_LEVEL_3)
    {
        uiKeyCode |= FLGKEY_SHUTTER1;
    }
    else if (uiData > KEYADC_LEVEL_2)
    {
        uiKeyCode |= FLGKEY_DEL;
    }

    return uiKeyCode;
}

/**
    Initialize key ADC detection

    Initialize key ADC detection

    @return void
*/
void KeyADC_Init(void)
{
    if ((adc_open(KEYADC_CH_0) != E_OK) || (adc_open(KEYADC_CH_1) != E_OK))
    {
        debug_err(("Can't open ADC for key detect\r\n"));
        return;
    }
    
    // VolDet_Init() already enable ADC and set extra sampling count, sampling rate
    // and average.

    // Set channel divider for key channel 0 / 1 to 18, key will detect per 9.728 ms
    // Real sampling rate will be 1953.125 / (channel divider + 1) = 102.796 Hz
    adc_setChDivider(KEYADC_CH_0, 18);
    adc_setChDivider(KEYADC_CH_1, 18);

    // Set mode for key detection, continuous mode, disable interrupt and no ISR
    adc_setMode(KEYADC_CH_0, TRUE, FALSE, NULL);
    adc_setMode(KEYADC_CH_1, TRUE, FALSE, NULL);
}

//@}
