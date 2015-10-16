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
#include "KeyADC.h"
#include "ADC.h"

#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

// ADC level for key detection
#define KEY_ADC_LOW             0
#define KEY_ADC_HIGH            1023

//Channel  1
#define KEY_UP_ADC              0
#define KEY_DOWN_ADC            245
#define KEY_LEFT_ADC            515
#define KEY_RIGHT_ADC           960//810

//Channel  2
#define KEY_MENU_ADC           245// 0//245
#define KEY_MODE_ADC            0//515//0
#define KEY_PLAY_ADC            960//245
#define KEY_DELETE_ADC          248//510

#define KEY_VALUE_TH            25

/**
  Judge key code by reading value from ADC

  Judge key code by reading value from ADC

  @param void
  @return void
*/
UINT32 KeyADC_GetKey(void)
{
    UINT32 uiADCValue1, uiADCValue2;

    uiADCValue1 = adc_readData(KEY_ADC_CH_1);
    uiADCValue2 = adc_readData(KEY_ADC_CH_2);

    /*-------- Channel 1--------*/
    if ((uiADCValue1 >= 0) &&
        (uiADCValue1 < (KEY_UP_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc1:%d\r\n",uiADCValue1));
        return VOLDET_KEY_LVL_UP;
    }
    else if ((uiADCValue1 > (KEY_DOWN_ADC - KEY_VALUE_TH))
       && (uiADCValue1 < (KEY_DOWN_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc1:%d\r\n",uiADCValue1));
        return VOLDET_KEY_LVL_DOWN;
    }
    else if ((uiADCValue1 > (KEY_LEFT_ADC - KEY_VALUE_TH))
       && (uiADCValue1 < (KEY_LEFT_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc1:%d\r\n",uiADCValue1));
        return VOLDET_KEY_LVL_LEFT;
    }
    else if ((uiADCValue1 > (KEY_RIGHT_ADC - KEY_VALUE_TH))
       && (uiADCValue1 < (KEY_RIGHT_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc1:%d\r\n",uiADCValue1));
        return VOLDET_KEY_LVL_RIGHT;
    }

    /*-------- Channel 2--------*/
    if ((uiADCValue2 >= 0) &&
        (uiADCValue2 < (KEY_MODE_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc2:%d\r\n",uiADCValue2));
      return VOLDET_KEY_LVL_MODE;
    }
    else if ((uiADCValue2 >= (KEY_MENU_ADC - KEY_VALUE_TH)) &&
        (uiADCValue2 < (KEY_MENU_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc2:%d\r\n",uiADCValue2));
      return VOLDET_KEY_LVL_MENU;
    }
    else if ((uiADCValue2 > (KEY_PLAY_ADC - KEY_VALUE_TH))
       && (uiADCValue2 < (KEY_PLAY_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc2:%d\r\n",uiADCValue2));
        return VOLDET_KEY_LVL_PLAYBACK;
    }
    else if ((uiADCValue2 > (KEY_DELETE_ADC - KEY_VALUE_TH))
       && (uiADCValue2 < (KEY_DELETE_ADC + KEY_VALUE_TH)))
    {
        DEBUG_MSG(("adc2:%d\r\n",uiADCValue2));
        return VOLDET_KEY_LVL_DELETE;
    }

    return VOLDET_KEY_LVL_NULL;
}

UINT32 KeyADC_GetSpecialKey(void) // for DV565 special key combination detection
{
    UINT32 uiADCValue1, uiADCValue2;

    uiADCValue1=  adc_readData(KEY_ADC_CH_1);
    uiADCValue2=  adc_readData(KEY_ADC_CH_2);

    if ((uiADCValue1 > (KEY_RIGHT_ADC - KEY_VALUE_TH))
       && (uiADCValue1 < (KEY_RIGHT_ADC + KEY_VALUE_TH)))   // key RIGHT
    {
        if ((uiADCValue2 >= 0) &&
            (uiADCValue2 < (KEY_MODE_ADC + KEY_VALUE_TH)))  // key MODE
        {
            return TRUE;
        }
    }
    return FALSE;
}

/**
  Initialize key ADC detection

  Initialize key ADC detection

  @param void
  @return void
*/
void KeyADC_Init(void)
{
    if (adc_open(KEY_ADC_CH_1) != E_OK)
    {
        debug_err(("Can't open ADC for key group 1\r\n"));
        return;
    }

    if (adc_open(KEY_ADC_CH_2) != E_OK)
    {
        debug_err(("Can't open ADC for key group 2\r\n"));
        return;
    }

    // VolDet_Init() already enable ADC and set extra sampling count, sampling rate
    // and average.

    // Set channel divider for key channel to 18, key will detect per 9.728 ms
    // Real sampling rate will be 1953.125 / (channel divider + 1) = 102.796 Hz
    adc_setChDivider(KEY_ADC_CH_1, 18);

    // Set mode for key group 1 detection
    adc_setMode(KEY_ADC_CH_1, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);

    // Set mode for key group 2 detection
    adc_setMode(KEY_ADC_CH_2, TRUE/*continuous mode*/, FALSE/*disable interrupt*/, NULL);

    // Enable adc control logic
    adc_enable();
    //adc_triggerOneShot(KEY_ADC_CH_1);
    //adc_triggerOneShot(KEY_ADC_CH_2);
}

UINT32 gAdcRead = 4;

void KeyADC_SetReadChannel(UINT32 ch)
{
    gAdcRead = ch;
}

void KeyADC_ReadChannel(void)
{
    switch (gAdcRead)
    {
    case 0:
        DEBUG_MSG(("ad0 = %d\r\n", adc_readData(ADC_CHANNEL_0)));
        break;
    case 1:
        DEBUG_MSG(("ad1 = %d\r\n", adc_readData(ADC_CHANNEL_1)));
        break;
    case 2:
        DEBUG_MSG(("ad2 = %d\r\n", adc_readData(ADC_CHANNEL_2)));
        break;
    case 3:
        DEBUG_MSG(("ad3 = %d\r\n", adc_readData(ADC_CHANNEL_3)));
        break;
    }
}

//@}
