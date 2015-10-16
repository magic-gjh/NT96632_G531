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


// ADC level for key detection
#define KEY_ADC_LEVEL_1         490
#define KEY_ADC_LEVEL_2         610
#define KEY_ADC_LEVEL_3         730
#define KEY_ADC_LEVEL_4         850
#define KEY_ADC_LEVEL_5         960 

// ADC error range
#define ADC_ERR                 80



/**
  Judge key code by reading value from ADC

  Judge key code by reading value from ADC

  @param void
  @return void
*/
UINT32 KeyADC_GetKey(void)
{
    static UINT32 uiPreKey1 = 0, uiKeyCode = 0;
    UINT32 ad1 = 0, ad2 = 0, key1 = 0;

    uiKeyCode = 0;

    ad1 = adc_readData(KEY_ADC_CH_1);
//    debug_msg("ad1 = %d\r\n",ad1);
    if (ad1 > KEY_ADC_LEVEL_5)
    {
        uiKeyCode |= FLGKEY_UP;
        debug_msg("FLGKEY_UP\r\n");
    }
    else if (ad1 > KEY_ADC_LEVEL_4)
    {
        uiKeyCode = FLGKEY_DOWN;
        debug_msg("FLGKEY_DOWN\r\n");
    }
    else if (ad1 > KEY_ADC_LEVEL_3)
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
        debug_msg("FLGKEY_SHUTTER2\r\n");
    }
    else if (ad1 > KEY_ADC_LEVEL_2)
    {
        uiKeyCode = FLGKEY_MODE;
        debug_msg("FLGKEY_MODE\r\n");
    }
    else if (ad1 > KEY_ADC_LEVEL_1)
    {
        uiKeyCode |= FLGKEY_MENU;
        debug_msg("FLGKEY_MENU\r\n");
    }
    else
    {
//        uiKeyCode |= 0;
    }

    ad2 = adc_readData(KEY_ADC_CH_2);
//    debug_msg("ad2 = %d\r\n",ad2);
#if 0	
    if (ad2 > KEY_ADC_LEVEL_3)
    {
        uiKeyCode |= 0;
    }
    else if (ad2 > KEY_ADC_LEVEL_2)
    {
        //uiKeyCode |= FLGKEY_ENTER;
        uiKeyCode |= FLGKEY_MODE;
        debug_msg("FLGKEY_ENTER\r\n");
    }
    else if (ad2 > KEY_ADC_LEVEL_1)
    {
        uiKeyCode |= FLGKEY_RIGHT;
        debug_msg("FLGKEY_RIGHT\r\n");
    }
    else
    {
        uiKeyCode = FLGKEY_PLAYBACK;
        debug_msg("FLGKEY_PLAYBACK\r\n");
    }
#endif
    return uiKeyCode;
}

/**
  Initialize key ADC detection

  Initialize key ADC detection

  @param void
  @return void
*/
void KeyADC_Init(void)
{
#if 1
    //debug_err(("KeyADC_Init\r\n"));
    //if (adc_open(KEY_ADC_CH_1) != E_OK)
    if ((adc_open(KEY_ADC_CH_1) != E_OK) || (adc_open(KEY_ADC_CH_2) != E_OK))
    {
        debug_err(("Can't open ADC for key detect\r\n"));
        return;
    }
    // Set sampling rate to 40KHz and sequential sampling mode
    // Each channel will sample once about 100 us
    adc_setControl(36, TRUE);
    // Set mode for key group 1 detection
    adc_setMode(KEY_ADC_CH_1, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    adc_setMode(KEY_ADC_CH_2, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);
    // Enable adc control logic
    adc_enable();
    adc_triggerOneShot(KEY_ADC_CH_1);
    adc_triggerOneShot(KEY_ADC_CH_2);
#endif
}

//@}
