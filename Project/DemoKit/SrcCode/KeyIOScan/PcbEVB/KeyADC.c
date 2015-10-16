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
#define KEY_ADC_CH_1            ADC_CHANNEL_1
//#define KEY_ADC_CH_2            ADC_CHANNEL_3

// ADC level for key detection
#define KEY_ADC_LEVEL_1         70
#define KEY_ADC_LEVEL_2         170
#define KEY_ADC_LEVEL_3         270
#define KEY_ADC_LEVEL_4         390
#define KEY_ADC_LEVEL_5         490
#define KEY_ADC_LEVEL_6         590
#define KEY_ADC_LEVEL_7         680
#define KEY_ADC_LEVEL_8         770
#define KEY_ADC_LEVEL_9         860

// ADC error range
#define ADC_ERR                 30


/**
  Judge key code by reading value from ADC

  Judge key code by reading value from ADC

  @param void
  @return void
*/
UINT32 KeyADC_GetKey(void)
{
#if 0
    static UINT32 uiPreKey1 = 0, uiKeyCode = 0;

    UINT32 ad1 = 0, key1 = 0;

    ad1 = adc_readData(KEY_ADC_CH_1);

    if (ad1 > KEY_ADC_LEVEL_1 - ADC_ERR && ad1 < KEY_ADC_LEVEL_1 + ADC_ERR)
        key1 = FLGKEY_MODE;
    else if (ad1 > KEY_ADC_LEVEL_2 - ADC_ERR && ad1 < KEY_ADC_LEVEL_2 + ADC_ERR)
        key1 = FLGKEY_UP;
    else if (ad1 > KEY_ADC_LEVEL_3 - ADC_ERR && ad1 < KEY_ADC_LEVEL_3 + ADC_ERR)
        key1 = FLGKEY_DOWN;
    else if (ad1 > KEY_ADC_LEVEL_4 - ADC_ERR && ad1 < KEY_ADC_LEVEL_4 + ADC_ERR)
        key1 = FLGKEY_ENTER;
    else if (ad1 > KEY_ADC_LEVEL_5 - ADC_ERR && ad1 < KEY_ADC_LEVEL_5 + ADC_ERR)
        key1 = FLGKEY_LEFT;
    else if (ad1 > KEY_ADC_LEVEL_6 - ADC_ERR && ad1 < KEY_ADC_LEVEL_6 + ADC_ERR)
        key1 = FLGKEY_RIGHT;
    else if (ad1 > KEY_ADC_LEVEL_7 - ADC_ERR && ad1 < KEY_ADC_LEVEL_7 + ADC_ERR)
        key1 = FLGKEY_MENU;
    else if (ad1 > KEY_ADC_LEVEL_8 - ADC_ERR && ad1 < KEY_ADC_LEVEL_8 + ADC_ERR)
        key1 = FLGKEY_PLAYBACK;
    else if (ad1 > KEY_ADC_LEVEL_9 - ADC_ERR && ad1 < KEY_ADC_LEVEL_9 + ADC_ERR)
        key1 = FLGKEY_DEL;

    if (key1 == uiPreKey1)
        uiKeyCode = key1;

    uiPreKey1 = key1;

    return uiKeyCode;
#endif
}

/**
  Initialize key ADC detection

  Initialize key ADC detection

  @param void
  @return void
*/
void KeyADC_Init(void)
{
#if 0
    debug_err(("KeyADC_Init\r\n"));
    if (adc_open(KEY_ADC_CH_1) != E_OK)
    {
        debug_err(("Can't open ADC for key group 1\r\n"));
        return;
    }
    // Set sampling rate to 40KHz and sequential sampling mode
    // Each channel will sample once about 100 us
    adc_setControl(36, TRUE);
    // Set mode for key group 1 detection
    adc_setMode(KEY_ADC_CH_1, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    // Enable adc control logic
    adc_enable();
    adc_triggerOneShot(KEY_ADC_CH_1);
#endif
}

//@}
