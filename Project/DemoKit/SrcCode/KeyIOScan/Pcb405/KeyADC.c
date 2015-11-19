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
#include "KeyScanTsk.h"

#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

// ADC level for key detection
#define KEY_ADC_LOW             0
#define KEY_ADC_HIGH            1023

#define KEY_ADC_CH_1            ADC_CHANNEL_1
#define KEY_ADC_CH_2            ADC_CHANNEL_2


//Channel  1
#define KEY_ADC_LEVEL_1         155//mode
#define KEY_ADC_LEVEL_2         333//up
#define KEY_ADC_LEVEL_3         643//down
#define KEY_ADC_LEVEL_4         480//enter
#define KEY_ADC_LEVEL_5         775//menu


#define ADC_ERR                 20

/**
  Judge key code by reading value from ADC

  Judge key code by reading value from ADC

  @param void
  @return void
*/
UINT32 KeyADC_GetKey(void)
{
	static UINT32 uiPreKey1 = 0, uiKeyCode = 0;

    UINT32 ad1 = 0, key1 = 0;

    ad1 = adc_readData(KEY_ADC_CH_1);
	//debug_err(("ad1 == %d\r\n",ad1));
    
    if (ad1 < (KEY_ADC_LEVEL_1 + ADC_ERR))
	{
        key1 = FLGKEY_ENTER;
        //debug_err(("KEY MODE\r\n"));
    }
    else if ((ad1 > (KEY_ADC_LEVEL_2 - ADC_ERR)) && (ad1 < (KEY_ADC_LEVEL_2 + ADC_ERR)))
	{
        key1 = FLGKEY_MODE;
        //debug_err(("KEY UP\r\n"));
    }
    else if ((ad1 > (KEY_ADC_LEVEL_3 - ADC_ERR)) && (ad1 < (KEY_ADC_LEVEL_3 + ADC_ERR)))
	{
        key1 = FLGKEY_DOWN;
        //debug_err(("KEY DOWN\r\n"));
    }
    else if ((ad1 > (KEY_ADC_LEVEL_4 - ADC_ERR)) && (ad1 < (KEY_ADC_LEVEL_4 + ADC_ERR)))
	{
        key1 = FLGKEY_UP;
        //debug_err(("KEY ENTER\r\n"));
    }
    else if ((ad1 > (KEY_ADC_LEVEL_5 - ADC_ERR)) && (ad1 < (KEY_ADC_LEVEL_5 + ADC_ERR)))
	{
        key1 = FLGKEY_MENU;
        //debug_err(("KEY MENU\r\n"));
    }   

    if (key1 == uiPreKey1)
        uiKeyCode = key1;
    uiPreKey1 = key1;

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
