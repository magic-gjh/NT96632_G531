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
#define KEY_ADC_CH_2            ADC_CHANNEL_2
#define KEY_ADC_CH_3            ADC_CHANNEL_3

// ADC level for key detection
#define KEY_ADC1_LEVEL_1        30
#define KEY_ADC1_LEVEL_2        247
#define KEY_ADC1_LEVEL_3        515

#define KEY_ADC2_LEVEL_1        30
#define KEY_ADC2_LEVEL_2        247
#define KEY_ADC2_LEVEL_3        515

#define KEY_ADC3_LEVEL_1        30
#define KEY_ADC3_LEVEL_2        247
#define KEY_ADC3_LEVEL_3        515

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
    static UINT32 uiPreKey1 = 0,uiPreKey2 = 0,uiPreKey3 = 0,  uiKeyCode1 = 0,uiKeyCode2 = 0,uiKeyCode3 = 0;

    UINT32 ad1 = 0, key1 = 0,ad2 = 0, key2 = 0,ad3 = 0, key3 = 0;

    ad1 = adc_readData(KEY_ADC_CH_1);
    if( (ad1 > 2) &&  (ad1 < 1000) )
    {
        //debug_err(("key adc value 1 = %d\n\r",ad1));
    }
    if (ad1 >= KEY_ADC1_LEVEL_1 - ADC_ERR && ad1 <= KEY_ADC1_LEVEL_1 + ADC_ERR)
        key1 = FLGKEY_RIGHT;
    else if (ad1 > KEY_ADC1_LEVEL_2 - ADC_ERR && ad1 < KEY_ADC1_LEVEL_2 + ADC_ERR)
        key1 = FLGKEY_PLAYBACK;
    else if (ad1 > KEY_ADC1_LEVEL_3 - ADC_ERR && ad1 < KEY_ADC1_LEVEL_3 + ADC_ERR)
        key1 = FLGKEY_MENU;

    if (key1 == uiPreKey1)
        uiKeyCode1 = key1;

    uiPreKey1 = key1;

    ad2 = adc_readData(KEY_ADC_CH_2);
    if( (ad2 > 2) &&  (ad2 < 1000) )
    {
        //debug_err(("key adc value 2 = %d\n\r",ad2));
    }
    if (ad2 >= KEY_ADC2_LEVEL_1 - ADC_ERR && ad2 <= KEY_ADC2_LEVEL_1 + ADC_ERR)
        key2 = FLGKEY_ZOOMIN;
    else if (ad2 > KEY_ADC2_LEVEL_2 - ADC_ERR && ad2 < KEY_ADC2_LEVEL_2 + ADC_ERR)
        key2 = FLGKEY_SHUTTER2;
    else if (ad2 > KEY_ADC2_LEVEL_3 - ADC_ERR && ad2 < KEY_ADC2_LEVEL_3 + ADC_ERR)
        key2 = FLGKEY_ZOOMOUT;

    if (key2 == uiPreKey2)
        uiKeyCode2 = key2;

    uiPreKey2 = key2;

    ad3 = adc_readData(KEY_ADC_CH_3);
    if( (ad3 > 2) &&  (ad3 < 1000) )
    {
        //debug_err(("key adc value 3 = %d\n\r",ad3));
    }
    if (ad3 >= KEY_ADC3_LEVEL_1 - ADC_ERR && ad3 <= KEY_ADC3_LEVEL_1 + ADC_ERR)
        key3 = FLGKEY_LEFT;
    else if (ad3 > KEY_ADC3_LEVEL_2 - ADC_ERR && ad3 < KEY_ADC3_LEVEL_2 + ADC_ERR)
        key3 = FLGKEY_MODE;
    else if (ad3 > KEY_ADC3_LEVEL_3 - ADC_ERR && ad3 < KEY_ADC3_LEVEL_3 + ADC_ERR)
        key3 = FLGKEY_DEL;

    if (key3 == uiPreKey3)
        uiKeyCode3 = key3;

    uiPreKey3 = key3;

    return (uiKeyCode1 | uiKeyCode2 | uiKeyCode3);
}

/**
  Initialize key ADC detection

  Initialize key ADC detection

  @param void
  @return void
*/
void KeyADC_Init(void)
{
    debug_err(("KeyADC_Init\r\n"));

    if (adc_open(KEY_ADC_CH_1) != E_OK)
    {
        debug_err(("KeyADC_Init: Can't open ADC for key group 1\r\n"));
        return;
    }
    if (adc_open(KEY_ADC_CH_2) != E_OK)
    {
        debug_err(("KeyADC_Init: Can't open ADC for key group 2\r\n"));
        return;
    }
    if (adc_open(KEY_ADC_CH_3) != E_OK)
    {
        debug_err(("KeyADC_Init: Can't open ADC for key group 3\r\n"));
        return;
    }
    // Set sampling rate to 40KHz and sequential sampling mode
    // Each channel will sample once about 100 us
    adc_setControl(36, TRUE);

    // Set mode for key detection
    adc_setMode(KEY_ADC_CH_1, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    adc_setMode(KEY_ADC_CH_2, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    adc_setMode(KEY_ADC_CH_3, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);

    // Enable adc control logic
    adc_enable();
    adc_triggerOneShot(KEY_ADC_CH_1);
    adc_triggerOneShot(KEY_ADC_CH_2);
    adc_triggerOneShot(KEY_ADC_CH_3);
}

//@}
