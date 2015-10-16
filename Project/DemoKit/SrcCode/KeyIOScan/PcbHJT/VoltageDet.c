/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       VoltageDet.c
    @ingroup    mIPRJAPKeyIO

    @brief      Voltage detection module
                This file is the voltage detection module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "VoltageDet.h"
#include "Adc.h"

#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

#define VOLDET_FLASH_LIGHT          DISABLE // DV121 has no flash light

// ADC channel for battery and flash voltage detection
#define VOLDET_BATTERY_ADC_CH       ADC_CHANNEL_0
#define VOLDET_FLASH_ADC_CH         ADC_CHANNEL_3

// ENABLE for continuous, DISABLE for one-shot
#define VOLDET_ADC_CONT_MODE        ENABLE

#if (VOLDET_ADC_CONT_MODE == ENABLE)
#define VOLDET_ADC_MODE             TRUE
#else
#define VOLDET_ADC_MODE             FALSE
#endif

// Battery voltage
/*
#define VOLDET_BATTERY_ADC_LVL0     560     // ~ 3,6V
#define VOLDET_BATTERY_ADC_LVL1     582     // ~ 3,8V
#define VOLDET_BATTERY_ADC_LVL2     615     // ~ 4.0V
#define VOLDET_BATTERY_ADC_LVL3     648     // ~ 4.20V
#define VOLDET_BATTERY_ADC_TH       1       // 7
*/
#define VOLDET_BATTERY_ADC_LVL0     560     // ~ 3.45V
#define VOLDET_BATTERY_ADC_LVL1     582     // ~ 3.6V
#define VOLDET_BATTERY_ADC_LVL2     615     // ~ 3.8V
#define VOLDET_BATTERY_ADC_LVL3     648     // ~ 4.0V
#define VOLDET_BATTERY_ADC_TH       2       // 7

// ADC value for flash empty/full/threshold
#define VOLDET_FLASH_ADC_EMPTY      113     // 260 V
#define VOLDET_FLASH_ADC_FULL       850// 250 V //1000 290V
#define VOLDET_FLASH_ADC_TH         10

#define VOLDET_FLASH_LVL_UNKNOWN    0xFFFFFFFF
#define VOLDET_BATTERY_LVL_UNKNOWN  0xFFFFFFFF

/**
  Get battery voltage ADC value

  Get battery voltage ADC value

  @param void
  @return UINT32 ADC value
*/
UINT32 VolDet_GetBatteryADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(VOLDET_BATTERY_ADC_CH);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(VOLDET_BATTERY_ADC_CH);

    return uiADCValue;
#else
    return adc_readData(VOLDET_BATTERY_ADC_CH);
#endif
}

/**
  Get flash voltage ADC value

  Get flash voltage ADC value

  @param void
  @return UINT32 ADC value
*/
UINT32 VolDet_GetFlashADC(void)
{
#if (VOLDET_FLASH_LIGHT == ENABLE)
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(VOLDET_FLASH_ADC_CH);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(VOLDET_FLASH_ADC_CH);

    return uiADCValue;
#else
    return adc_readData(VOLDET_FLASH_ADC_CH);
#endif
#endif
}

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/
void VolDet_Init(void)
{
    if (adc_open(VOLDET_BATTERY_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for battery detection\r\n"));
        return;
    }

#if (VOLDET_FLASH_LIGHT == ENABLE)
    if (adc_open(VOLDET_FLASH_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for flash detection\r\n"));
        return;
    }
#endif

    // Set extra sample count to 0, ADC will change channel every 16 clocks
    adc_setExtraSampCnt(0);

    // Set sampling average to 2, each channel will sample twice and return average
    adc_setSampleAvg(ADC_SAMPAVG_2);

    // Set sampling rate per channel to 1953.125 Hz
    // Sampling rate per channel = 8,000,000 / (15 + 1) / 8 / (16 + 0) / 2 = 1953.125
    // 15 of (15 + 1) is sampling divider, valid value: 0 ~ 15
    // 8 is total channel number, fixed
    // 0 of (16 + 0) is extra sample count
    // 2 is sampling average
    // Parameter 1 is useless in NT96630 (Only support sequential mode)
    adc_setControl(15, TRUE);

    // Set channel divider for battery to 255, battery will be detected per 131.072 ms
    // Real sampling rate will be 1953.125 / (channel divider + 1) = 7.629 Hz
    adc_setChDivider(VOLDET_BATTERY_ADC_CH, 255);

    // Set mode for battery voltage detection
    adc_setMode(VOLDET_BATTERY_ADC_CH, VOLDET_ADC_MODE, FALSE, NULL);

#if (VOLDET_FLASH_LIGHT == ENABLE)
    // Set channel divider for flash light voltage to 255
    // Real sampling rate will be 1953.125 / (channel divider + 1) = 7.629 Hz
    adc_setChDivider(VOLDET_FLASH_ADC_CH, 255);

    // Set mode for flash voltage detection
    adc_setMode(VOLDET_FLASH_ADC_CH, VOLDET_ADC_MODE, FALSE, NULL);
#endif

    // Enable adc control logic
    adc_enable();

#if (VOLDET_ADC_CONT_MODE == DISABLE)
    adc_triggerOneShot(VOLDET_BATTERY_ADC_CH);
#if (VOLDET_FLASH_LIGHT == ENABLE)
    adc_triggerOneShot(VOLDET_FLASH_ADC_CH);
#endif
#endif
}

/**
  Get battery voltage level

  Get battery voltage level.
  If battery voltage level is VOLDET_BATTERY_LVL_EMPTY, it means
  that you have to power off the system.

  @param void
  @return UINT32 Battery Level, refer to VoltageDet.h -> VOLDET_BATTERY_LVL_XXXX
*/
UINT32 VolDet_GetBatteryLevel(void)
{
    static UINT32   uiPreBatteryLvl     = VOLDET_BATTERY_LVL_UNKNOWN;
    static UINT32   uiPreBatteryADC     = 0;
    static UINT32   uiRetBatteryLvl;
    UINT32          uiCurBatteryADC, uiCurBatteryLvl;
    UINT32          batt_adc_lvl3, batt_adc_lvl2, batt_adc_lvl1, batt_adc_lvl0;

    //for TDC
    batt_adc_lvl3 = VOLDET_BATTERY_ADC_LVL3;
    batt_adc_lvl2 = VOLDET_BATTERY_ADC_LVL2;
    batt_adc_lvl1 = VOLDET_BATTERY_ADC_LVL1;
    batt_adc_lvl0 = VOLDET_BATTERY_ADC_LVL0;
    uiCurBatteryADC = VolDet_GetBatteryADC();
    DEBUG_MSG(("VOLDET: battery ADC = %d\r\n", uiCurBatteryADC));

    // Rising
    if (uiCurBatteryADC > uiPreBatteryADC)
    {
        if (uiCurBatteryADC > (batt_adc_lvl3 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_3;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl2 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_2;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl1 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_1;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl0 + VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_0;
        }
        else
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_EMPTY;
        }
    }
    // Falling
    else
    {
        if (uiCurBatteryADC > (batt_adc_lvl3 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_3;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl2 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_2;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl1 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_1;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl0 - VOLDET_BATTERY_ADC_TH))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_0;
        }
        else
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_EMPTY;
        }
    }

    // Debounce
    if ((uiCurBatteryLvl == uiPreBatteryLvl) ||
        (uiPreBatteryLvl == VOLDET_BATTERY_LVL_UNKNOWN))
    {
        uiRetBatteryLvl = uiCurBatteryLvl;
    }
    uiPreBatteryLvl = uiCurBatteryLvl;
    uiPreBatteryADC = uiCurBatteryADC;

    return uiRetBatteryLvl;
}

/**
  Get flash voltage level

  Get flash voltage level.
  If battery voltage level is VOLDET_FLASH_LVL_EMPTY, it means
  that you have to charge the flash.

  @param void
  @return UINT32 Flash Level, refer to VoltageDet.h -> VOLDET_FLASH_LVL_XXXX
*/
UINT32 VolDet_GetFlashLevel(void)
{
#if (VOLDET_FLASH_LIGHT == ENABLE)
    static UINT32   uiPreFlashLvl = VOLDET_FLASH_LVL_UNKNOWN;
    static UINT32   uiRetFlashLvl;
    UINT32          uiFlashADC, uiCurFlashLvl;

    uiFlashADC = VolDet_GetFlashADC();
    DEBUG_MSG(("VOLDET: flash ADC = %d\r\n", uiFlashADC));

    if (uiFlashADC >(VOLDET_FLASH_ADC_FULL + VOLDET_FLASH_ADC_TH))
    {
        uiCurFlashLvl = VOLDET_FLASH_LVL_FULL;
    }
    /*else{

        uiCurFlashLvl = VOLDET_FLASH_LVL_EMPTY;

    }*/
    else if ((uiFlashADC + VOLDET_FLASH_ADC_TH) < VOLDET_FLASH_ADC_EMPTY)
    {
        uiCurFlashLvl = VOLDET_FLASH_LVL_EMPTY;
    }
    else
    {
        uiCurFlashLvl = VOLDET_FLASH_LVL_HALF;
    }

    // Debounce
    if ((uiCurFlashLvl == uiPreFlashLvl) ||
        (uiPreFlashLvl == VOLDET_FLASH_LVL_UNKNOWN))
    {
        uiRetFlashLvl = uiCurFlashLvl;
    }
    uiPreFlashLvl = uiCurFlashLvl;
    DEBUG_MSG(("VOLDET: uiRetFlashLvl = %d\r\n", uiRetFlashLvl));

    return uiRetFlashLvl;
#else
    return VOLDET_FLASH_LVL_UNKNOWN;
#endif
}

//@}
