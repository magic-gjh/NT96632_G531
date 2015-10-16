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
#define VOLDET_BATTERY_ADC_CH       ADC_CHANNEL_0
#define VOLDET_FLASH_ADC_CH         ADC_CHANNEL_2
//#NT#2009/10/21#Philex Lin - begin
#define VOLDET_TV_EARPHONE_ADC_CH   ADC_CHANNEL_5
//#NT#2009/10/21#Philex Lin - end

#define VOLDET_TVIN_LVL             0
#define VOLDET_EARPHONE_LVL         1

// ENABLE for continuous, DISABLE for one-shot
#define VOLDET_ADC_CONT_MODE        ENABLE

#if (VOLDET_ADC_CONT_MODE == ENABLE)
#define VOLDET_ADC_MODE             TRUE
#else
#define VOLDET_ADC_MODE             FALSE
#endif

// Battery voltage
#if 1
#define VOLDET_BATTERY_ADC_LVL0     725     // ~ 3.55V
#define VOLDET_BATTERY_ADC_LVL1     745     // ~ 3.65V
#define VOLDET_BATTERY_ADC_LVL2     765     // ~ 3.75V
#define VOLDET_BATTERY_ADC_LVL3     785     // ~ 3.85V
#define VOLDET_BATTERY_ADC_EMPTY    705     // ~ 3.3V
#define VOLDET_BATTERY_ADC_TH       2       // 7
#else
#define VOLDET_BATTERY_ADC_LVL0     380     // ~3,6V
#define VOLDET_BATTERY_ADC_LVL1     400    // ~ 3,8V
#define VOLDET_BATTERY_ADC_LVL2     420     // ~ 4.0V
#define VOLDET_BATTERY_ADC_LVL3     450     // ~ 4.20V
#define VOLDET_BATTERY_ADC_TH       1       // 7
#endif
// for Alkaline
/*#define VOLDET_BATTERY_ADC_LVL0     574     // ~ 4.80V
#define VOLDET_BATTERY_ADC_LVL1     621     // ~ 5.20V
#define VOLDET_BATTERY_ADC_LVL2     655     // ~ 5.50V
#define VOLDET_BATTERY_ADC_LVL3     701     // ~ 5.85V
*/
// for NiMH
/*#define VOLDET_BATTERY_ADC_LVL0_NI  550     // ~ 4.60V
#define VOLDET_BATTERY_ADC_LVL1_NI  574     // ~ 4.80V
#define VOLDET_BATTERY_ADC_LVL2_NI  599     // ~ 5.00V
#define VOLDET_BATTERY_ADC_LVL3_NI  621     // ~ 5.20V
#define VOLDET_BATTERY_ADC_TH       5       // 7
*/
// ADC value for flash empty/full/threshold
#define VOLDET_FLASH_ADC_EMPTY      113     // 260 V
#define VOLDET_FLASH_ADC_FULL       850// 250 V //1000 290V
#define VOLDET_FLASH_ADC_TH         10

//#NT#2009/10/21#Philex Lin - begin
#define VOLDET_TV_IN_ADC            146
#define VOLDET_EARPHONE_IN_ADC      48
#define VOLDET_TV_IN_ADC_TH         5
//#NT#2009/10/21#Philex Lin - end

#define VOLDET_FLASH_LVL_UNKNOWN    0xFFFFFFFF
#define VOLDET_BATTERY_LVL_UNKNOWN  0xFFFFFFFF
#define VOLDET_TVIN_LVL_UNKNOWN     0xFFFFFFFF

//#NT#2010/3/29#Philex -begin
static BOOL bIsADCEmptyLevel = FALSE;
//#NT#2010/3/29#Philex -end

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
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(VOLDET_FLASH_ADC_CH);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(VOLDET_FLASH_ADC_CH);

    return uiADCValue;
#else
    return adc_readData(VOLDET_FLASH_ADC_CH);
#endif
}

//#NT#2009/10/21#Philex Lin - begin
/**
  Get TV_EarPhone voltage ADC value

  Get TV_EarPhone voltage ADC value

  @param void
  @return UINT32 ADC value
*/
UINT32 VolDet_GetTV_EarPhoneADC(void)
{
    UINT32 uiADCValue;
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(VOLDET_TV_EARPHONE_ADC_CH);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(VOLDET_TV_EARPHONE_ADC_CH);

    return uiADCValue;
#else
    uiADCValue = adc_readData(VOLDET_TV_EARPHONE_ADC_CH);
//    debug_err(("uiADCValue=%d\r\n",uiADCValue));
    return adc_readData(VOLDET_TV_EARPHONE_ADC_CH);
#endif
}
//#NT#2009/10/21#Philex Lin - end
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
    if (adc_open(VOLDET_FLASH_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for flash detection\r\n"));
        return;
    }
    //#NT#2009/10/21#Philex Lin - begin
    if (adc_open(VOLDET_TV_EARPHONE_ADC_CH) != E_OK)
    {
        debug_err(("VOLDET ERR: Can't open ADC channel for TV_EarPhone detection\r\n"));
        return;
    }
    //#NT#2009/10/21#Philex Lin - end

    // Set sampling rate to 40KHz and sequential sampling mode
    // Each channel will sample once about 100 us
    adc_setControl(36, TRUE);

    // Set mode for battery voltage detection
    adc_setMode(VOLDET_BATTERY_ADC_CH, VOLDET_ADC_MODE, FALSE, NULL);

    // Set mode for flash voltage detection
    adc_setMode(VOLDET_FLASH_ADC_CH, VOLDET_ADC_MODE, FALSE, NULL);
    //#NT#2009/10/21#Philex Lin - begin
    // Set mode for TV/EarPhone voltage detection
    adc_setMode(VOLDET_TV_EARPHONE_ADC_CH, VOLDET_ADC_MODE, FALSE, NULL);
    //#NT#2009/10/21#Philex Lin - end

    // Enable adc control logic
    adc_enable();

#if (VOLDET_ADC_CONT_MODE == DISABLE)
    adc_triggerOneShot(VOLDET_BATTERY_ADC_CH);
    adc_triggerOneShot(VOLDET_FLASH_ADC_CH);
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


#if 1   //#NewUx porting: Temporarily marked
    //for TDC
    batt_adc_lvl3 = VOLDET_BATTERY_ADC_LVL3;
    batt_adc_lvl2 = VOLDET_BATTERY_ADC_LVL2;
    batt_adc_lvl1 = VOLDET_BATTERY_ADC_LVL1;
    batt_adc_lvl0 = VOLDET_BATTERY_ADC_LVL0;
    uiCurBatteryADC = VolDet_GetBatteryADC();
//    debug_err(("battery ADC = %d\r\n", uiCurBatteryADC));

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
#else
    return VOLDET_BATTERY_LVL_3;
#endif
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

    static UINT32   uiPreFlashLvl = VOLDET_FLASH_LVL_UNKNOWN;
    static UINT32   uiRetFlashLvl;
    UINT32          uiFlashADC, uiCurFlashLvl;

    uiFlashADC = VolDet_GetFlashADC();
   //debug_err(("Flash ADC = %d\r\n", uiFlashADC));

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
 //  debug_err(("uiRetFlashLvl = %d\r\n", uiRetFlashLvl));
    return uiRetFlashLvl;


}

/**
  Get TV in voltage level

  Get TV_IN voltage level.

  @param void
  @return UINT32 TV_IN Level, refer to VoltageDet.h -> VOLDET_FLASH_LVL_XXXX
*/
BOOL VolDet_GetTVInLevel(void)
{
    static UINT32   uiPreTvInLvl = VOLDET_TVIN_LVL_UNKNOWN;
    static UINT32   uiRetTvInLvl;
    UINT32          uiTvInADC, uiCurTvInLvl;

    uiTvInADC = VolDet_GetTV_EarPhoneADC();
//    debug_err(("Flash ADC = %d\r\n", uiTvInADC));

    if ((uiTvInADC >=(VOLDET_EARPHONE_IN_ADC - VOLDET_FLASH_ADC_TH))&&
        (uiTvInADC <=(VOLDET_EARPHONE_IN_ADC + VOLDET_FLASH_ADC_TH)))
    {
        uiCurTvInLvl = VOLDET_EARPHONE_LVL;
    }

    // Debounce
    if ((uiCurTvInLvl == uiPreTvInLvl) ||
        (uiPreTvInLvl == VOLDET_TVIN_LVL_UNKNOWN))
    {
        uiRetTvInLvl = uiCurTvInLvl;
    }
    uiPreTvInLvl = uiCurTvInLvl;

    if (uiRetTvInLvl==VOLDET_EARPHONE_LVL) {
        return FALSE; // EarPhone plug in
    } else {
        return TRUE;  // TV plug in
    }

}

//#NT#2010/3/29#Philex - begin
// add the following APIs for turning off panel in low power condition(< 3.3V)
BOOL VolDet_IsADCEmptyLevel(void)
{
  #define  LoopNum 10
  BOOL bIsEmpty = FALSE;
  int  i;
  UINT32 uiADC;

   for (i=0;i<LoopNum;i++)
   {
        uiADC = VolDet_GetBatteryADC();
        debug_msg("uiADC=%d\r\n",uiADC);
        if (uiADC >(VOLDET_BATTERY_ADC_LVL0-VOLDET_BATTERY_ADC_TH))
            break;
   }

   if (i==LoopNum)
     bIsEmpty = TRUE;

   return bIsEmpty;
}

void VolDet_SetEmptyLevel(BOOL bSetEmpty)
{
    bIsADCEmptyLevel = bSetEmpty;
}

BOOL VolDet_GetEmptyLevel(void)
{
    return bIsADCEmptyLevel;
}
//#NT#2010/3/29#Philex - end
//@}
