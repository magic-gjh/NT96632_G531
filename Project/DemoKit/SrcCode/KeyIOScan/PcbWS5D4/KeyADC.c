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

// ADC level for key detection

#define KEY_ADC1_LEVEL_1        70
#define KEY_ADC1_LEVEL_2        150
#define KEY_ADC1_LEVEL_3        250
#define KEY_ADC1_LEVEL_4        410

#define KEY_ADC2_LEVEL_1        70
#define KEY_ADC2_LEVEL_2        150



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
    static UINT32 uiPreKey1 = 0,uiPreKey2 = 0,  uiKeyCode1 = 0,uiKeyCode2 = 0;

    UINT32 ad1 = 0, key1 = 0,ad2 = 0, key2 = 0;

    ad1 = adc_readData(KEY_ADC_CH_1);
	//if( (ad1 > 2) &&  (ad1 < 1000) )
	{
        //debug_err(("key adc value 1 = %d\n\r",ad1));
    }
   
    if (ad1 >= KEY_ADC1_LEVEL_1 - ADC_ERR && ad1 <= KEY_ADC1_LEVEL_1 + ADC_ERR)
        key1 = FLGKEY_LEFT;
    else if (ad1 > KEY_ADC1_LEVEL_2 - ADC_ERR && ad1 < KEY_ADC1_LEVEL_2 + ADC_ERR)
        key1 = FLGKEY_MENU;
    else if (ad1 > KEY_ADC1_LEVEL_3 - ADC_ERR && ad1 < KEY_ADC1_LEVEL_3 + ADC_ERR)
        key1 = FLGKEY_DOWN;
    else if (ad1 > KEY_ADC1_LEVEL_4 - ADC_ERR && ad1 < KEY_ADC1_LEVEL_4 + ADC_ERR)
        key1 = FLGKEY_ENTER;

    if (key1 == uiPreKey1)
        uiKeyCode1 = key1;

    uiPreKey1 = key1;



    ad2 = adc_readData(KEY_ADC_CH_2);
	//if( (ad2 > 0) &&  (ad2 < 1000) )
    {
     //   debug_err(("key adc value 2 = %d\n\r",ad2));
    }

    if (ad2 > KEY_ADC2_LEVEL_1 - ADC_ERR && ad2 < KEY_ADC2_LEVEL_1 + ADC_ERR)
        key2 = FLGKEY_SHUTTER1;
    else if (ad2 > KEY_ADC2_LEVEL_2 - ADC_ERR && ad2 < KEY_ADC2_LEVEL_2 + ADC_ERR)
        key2 = FLGKEY_SHUTTER2;


    if (key2 == uiPreKey2)
        uiKeyCode2 = key2;

    uiPreKey2 = key2;

    return (uiKeyCode1 | uiPreKey2);
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

    // Set sampling rate to 40KHz and sequential sampling mode
    // Each channel will sample once about 100 us
    adc_setControl(36, TRUE);

    // Set mode for key detection
    adc_setMode(KEY_ADC_CH_1, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    adc_setMode(KEY_ADC_CH_2, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);

    // Enable adc control logic
    adc_enable();
    adc_triggerOneShot(KEY_ADC_CH_1);
    adc_triggerOneShot(KEY_ADC_CH_2);
}

//@}
