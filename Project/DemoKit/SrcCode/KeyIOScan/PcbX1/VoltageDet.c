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

// ADC channel for battery and flash voltage detection
#define VOLDET_BATTERY_ADC_CH       ADC_CHANNEL_3
#define VOLDET_HWPCB_ADC_CH         ADC_CHANNEL_2

// Battery voltage
#define VOLDET_BATTERY_ADC_LVL0     770     // ~3,6V
#define VOLDET_BATTERY_ADC_LVL1     886     // ~ 3,8V
#define VOLDET_BATTERY_ADC_LVL2     910     // ~ 4.0V
#define VOLDET_BATTERY_ADC_TH       10       // 7

// ADC level
#define VOLDET_HWPCB_ADC_LVL0       323
#define VOLDET_HWPCB_ADC_LVL1       440
#define VOLDET_HWPCB_ADC_LVL2       555
#define VOLDET_HWPCB_ADC_LVL3       675
#define VOLDET_HWPCB_ADC_LVL4       790
#define VOLDET_HWPCB_ADC_LVL5       900

#define VOLDET_BATTERY_LVL_UNKNOWN  0xFF000000
/**
  Get battery voltage ADC value

  Get battery voltage ADC value

  @param void
  @return UINT32 ADC value
*/
UINT32 VolDet_GetBatteryADC(void)
{
    return adc_readData(VOLDET_BATTERY_ADC_CH);
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
    /*
    if (adc_open(VOLDET_FLASH_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for flash detection\r\n"));
        return;
    }
    */

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

    // Set channel divider for battery to 255, battery will detect per 131.072 ms
    // Real sampling rate will be 1953.125 / (channel divider + 1) = 7.629 Hz
    adc_setChDivider(VOLDET_BATTERY_ADC_CH, 255);

    // Set mode for battery voltage detection, continuous mode, disable interrupt and no ISR
    adc_setMode(VOLDET_BATTERY_ADC_CH, TRUE, FALSE, NULL);

    
    // Set mode for flash voltage detection, continuous mode, disable interrupt and no ISR
    adc_setMode(VOLDET_HWPCB_ADC_CH, TRUE, FALSE, NULL);
    

    // Enable adc control logic
    adc_enable();
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
#if 1 
    static UINT32   uiPreBatteryLvl     = VOLDET_BATTERY_LVL_UNKNOWN;
    static UINT32   uiPreBatteryADC     = 0;
    static UINT32   uiRetBatteryLvl;
    UINT32          uiCurBatteryADC, uiCurBatteryLvl;
    UINT32          batt_adc_lvl2, batt_adc_lvl1, batt_adc_lvl0;


    //for TDC
    batt_adc_lvl2 = VOLDET_BATTERY_ADC_LVL2;
    batt_adc_lvl1 = VOLDET_BATTERY_ADC_LVL1;
    batt_adc_lvl0 = VOLDET_BATTERY_ADC_LVL0;
    uiCurBatteryADC = VolDet_GetBatteryADC();
    //debug_err(("battery ADC = %d\r\n", uiCurBatteryADC));

    // Rising
    if (uiCurBatteryADC > uiPreBatteryADC)
    {
        if (uiCurBatteryADC > (batt_adc_lvl2 + VOLDET_BATTERY_ADC_TH))
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
    }
    // Falling
    else
    {
        if (uiCurBatteryADC > (batt_adc_lvl2))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_2;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl1))
        {
            uiCurBatteryLvl = VOLDET_BATTERY_LVL_1;
        }
        else if (uiCurBatteryADC > (batt_adc_lvl0))
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
#else
    return VOLDET_BATTERY_LVL_3;
#endif
}

/**
  
**/
UINT32 VolDet_GetHWPCB(void)
{
    UINT32 uiData, uiHwVer;

    uiData = adc_readData(VOLDET_HWPCB_ADC_CH);

    if (uiData > VOLDET_HWPCB_ADC_LVL5)
    {
        uiHwVer = 1;
    }
    else if (uiData > VOLDET_HWPCB_ADC_LVL4)
    {
        uiHwVer = 2;
    }
    else if (uiData > VOLDET_HWPCB_ADC_LVL3)
    {
        uiHwVer = 10;
    }
    else if (uiData > VOLDET_HWPCB_ADC_LVL2)
    {
        uiHwVer = 11;
    }
    else if (uiData > VOLDET_HWPCB_ADC_LVL1)
    {
        uiHwVer = 12;
    }
    else if (uiData > VOLDET_HWPCB_ADC_LVL0)
    {
        uiHwVer = 13;
    }
    else
    {
        uiHwVer = 14;
    }

    return uiHwVer;
}

//@}
