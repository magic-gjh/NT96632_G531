/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.c
    @ingroup    mIPRJAPKeyIO

    @brief      GPIO mapping
                This file do GPIO initialization and GPIO detection

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "GPIOMapping.h"
#include "KeyScanTsk.h"
#include "KeyADC.h"
#include "top.h"
#include "pll.h"
#include "SensorFP.h"
#include "PWM.h"

/**
  Delay between toggle GPIO pin of input/output

  Delay between toggle GPIO pin of input/output

  @param void
  @return void
*/
static void GPIOMap_DetKeyDelay(void)
{
    dgpio_readData();
    dgpio_readData();
}

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void GPIOMap_Init(void)
{
    // Open GPIO driver
    gpio_open();

//--------------------------------------------------------------------
// PINMUX
//--------------------------------------------------------------------
    //HDMI
    pinmux_select_hotplug(PINMUX_GPIO);

//--------------------------------------------------------------------
// S_GPIO sensor interface
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// P_GPIO serial interface
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// L_GPIO LCD interface
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// G_GPIO
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// C_GPIO memory card interface
//--------------------------------------------------------------------
    //key HDMI
    gpio_setDir(GPIO_HDMI_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_HDMI_DETECT, PAD_PULLDOWN);

    //--------------------------------------------------------------------
    // DGPIO
    //--------------------------------------------------------------------
#if (_CHIP_ != _CHIP_613_) && (_CHIP_ != _CHIP_200_)
#if 0
    // Key
    // Input matrix, pull up
    gpio_setDir(GPIO_KEY_IN_0, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_IN_0, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_IN_1, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_IN_1, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_IN_2, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_IN_2, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_IN_3, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_IN_3, PAD_PULLUP);

    // Output matrix, pull up
    gpio_setDir(GPIO_KEY_OUT_0, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_OUT_0, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_OUT_1, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_OUT_1, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_OUT_2, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_OUT_2, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_OUT_3, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_OUT_3, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_OUT_4, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_OUT_4, PAD_PULLUP);
#else
    gpio_setDir(GPIO_KEY_SHUTTER1, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_SHUTTER1, PAD_PULLUP);

    gpio_setDir(GPIO_KEY_SHUTTER2, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_SHUTTER2, PAD_PULLUP);
#endif
#endif

    //--------------------------------------------------------------------
    // AGPIO
    //--------------------------------------------------------------------
    // Storage Card detect
    gpio_setDir(GPIO_CARD_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_DETECT, PAD_PULLUP);

    // Storage Card write protect
    gpio_setDir(GPIO_CARD_WP, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_WP, PAD_PULLUP);

#if 0
    // LED
    gpio_setDir(GPIO_LED_R, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_R);
    gpio_setDir(GPIO_LED_G, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_G);

    // LCD RESET and SLEEP
    gpio_setDir(GPIO_LCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LCD_RESET);

    gpio_setDir(GPIO_LCD_SLEEP, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LCD_SLEEP);

    // LCD Backlight
    gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_PWM_BLG_PCTL);

    // CCD power
    gpio_setDir(GPIO_CCD_PCTL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_PCTL);

    // Flash charge
    gpio_setDir(GPIO_FLASH_CHARGE, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_FLASH_CHARGE);

    // FCS LED
    gpio_setDir(GPIO_LED_FCS, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_LED_FCS);
#endif

    // TV plug-in, DGPIO6 is default GPIO input and pull-up
    //gpio_setDir(GPIO_TV_DETECT, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_TV_DETECT, PAD_PULLUP);
//#NT#20090902#ethanlau -begin
#if (_SENSORLIB_ == _SENSORLIB_OV5653_630_)
    gpio_setDir(GPIO_CCD_PWND, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_PWND);
    TimerDelayMs(20);
    gpio_clearPin(GPIO_CCD_PWND);
#endif
//#NT#20090902#ethanlau -end
}

/**
  Detect Storage card is inserted or not

  Detect Storage card is inserted or not.
  Return TRUE if storage card is inserted, FALSE if storage card is removed.

  @param void
  @return BOOL: TRUE -> Storage card inserted, FALSE -> Storage card removed
*/
BOOL GPIOMap_DetStrgCard(void)
{
	#if 0
	return (gpio_getPin(GPIO_CARD_DETECT) == 0 ? TRUE : FALSE);
	#else
	return TRUE;
	#endif
}

/**
  Detect Storage card is write protected or not

  Detect Storage card is write protected or not.
  Return TRUE if storage card is write protected, FALSE if storage card is not write protected.

  @param void
  @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL GPIOMap_DetStrgCardWP(void)
{
#if 0
    return (gpio_getPin(GPIO_CARD_WP) == 0 ? FALSE : TRUE);
#else
    return FALSE;
#endif
}

/**
  Detect key is pressed.

  Detect key is pressed.
  Return key pressed status (refer to GPIOMapping.h)

  @param void
  @return UINT32
*/
UINT32 GPIOMap_DetKey(void)
{
    UINT32 uiGPIOData, uiKeyCode = 0;

//#NT#2009/06/01#Chris Chung -begin
//#NT#mark all key detection for 2-series EVB
#if 0
   // if (gpio_getPin(GPIO_KEY_ZOOMIN) == 0)
    //    uiKeyCode |= FLGKEY_ZOOMIN;

    //if (gpio_getPin(GPIO_KEY_ZOOMOUT) == 0)
     //   uiKeyCode |= FLGKEY_ZOOMOUT;

    if (gpio_getPin(GPIO_KEY_SHUTTER) == 0)
        uiKeyCode |= FLGKEY_SHUTTER2;

    if (gpio_getPin(GPIO_KEY_UP) == 0)
        uiKeyCode |= FLGKEY_UP;

    if (gpio_getPin(GPIO_KEY_DOWN) == 0)
        uiKeyCode |= FLGKEY_DOWN;

    if (gpio_getPin(GPIO_KEY_LEFT) == 0)
        uiKeyCode |= FLGKEY_LEFT;

    if (gpio_getPin(GPIO_KEY_RIGHT) == 0)
        uiKeyCode |= FLGKEY_RIGHT;

    if (gpio_getPin(GPIO_KEY_PB) == 0)
        uiKeyCode |= FLGKEY_PLAYBACK;

    //if (gpio_getPin(GPIO_KEY_DISPLAY) == 0)
    //    uiKeyCode |= FLGKEY_DISPLAY;

    //if (gpio_getPin(GPIO_KEY_MENU) == 0)
    //    uiKeyCode |= FLGKEY_MENU;

    if (gpio_getPin(GPIO_KEY_MODE) == 0)
        uiKeyCode |= FLGKEY_MODE;
#endif
//#NT#2009/06/01#Chris Chung -end

    return uiKeyCode;
}

/**
  Detect mode dial

  Detect mode dial

  @param void
  @return UINT32
*/
UINT32 GPIOMap_DetModeDial(void)
{
    /*
    UINT32 mode;

    mode =  gpio_getPin(GPIO_MODE_1)        |
            gpio_getPin(GPIO_MODE_2) << 1   |
            gpio_getPin(GPIO_MODE_3) << 2   |
            gpio_getPin(GPIO_MODE_4) << 3;

    return mode;
    */
    return 0;
}

/**
  Detect TV is plugging in or unplugged

  Detect TV is plugging in or unplugged.
  Return TRUE if TV is plugging in, FALSE if TV is unplugged.

  @param void
  @return BOOL: TRUE -> TV is plugging in, FALSE -> TV is unplugged
*/
BOOL GPIOMap_DetTV(void)
{
    #if 0
    return !(BOOL)gpio_getPin(GPIO_TV_DETECT);
    #else
    return FALSE;
    #endif
}

/**
  Detect HDMI is plugging in or unplugged

  Detect HDMI is plugging in or unplugged.
  Return TRUE if HDMI is plugging in, FALSE if HDMI is unplugged.

  @param void
  @return BOOL: TRUE -> HDMI is plugging in, FALSE -> HDMI is unplugged
*/
BOOL GPIOMap_DetHDMI(void)
{
    return (BOOL)gpio_getPin(GPIO_HDMI_DETECT);
}

/**
  Detect AC power is plugging in or unplugged

  Detect AC power is plugging in or unplugged.
  Return TRUE if AC power is plugging in, FALSE if AC power is unplugged.

  @param void
  @return BOOL: TRUE -> AC power is plugging in, FALSE -> AC power is unplugged
*/
BOOL GPIOMap_DetACPower(void)
{
    return FALSE;
}

/**
  Detect Audio is plugging in or unplugged

  Detect Audio is plugging in or unplugged.
  Return TRUE if Audio is plugging in, FALSE if Audio is unplugged.

  @param void
  @return BOOL: TRUE -> Audio is plugging in, FALSE -> Audio is unplugged
*/
BOOL GPIOMap_DetAudio(void)
{
    return FALSE;
}

/**
  Detect power off key is pressed or not

  Detect power off key is pressed or not.
  Return TRUE if power off key is pressed, FALSE if power off key is released.

  @param void
  @return BOOL: TRUE -> power off key is pressed, FALSE -> power off key is released
*/
BOOL GPIOMap_DetPoweroff(void)
{
    #if 0
	return !(BOOL)gpio_getPin(GPIO_POWER_CTRL);
	#else
	return FALSE;
	#endif
}

/**
  Detect macro

  Detect macro

  @param void
  @return BOOL: TRUE -> Is macro, FALSE -> Isn't macro
*/
BOOL GPIOMap_DetMacro(void)
{
	#if 0
    return (BOOL)gpio_getPin(GPIO_MACRO_DETECT);
	#else
	return FALSE;
	#endif
}

/**
  Detect engineering mode special key

  Detect engineering mode special key

  @param void
  @return BOOL: TRUE -> Is engineering mode requested, FALSE -> Isn't engineering mode requested
*/
BOOL GPIOMap_DetEngMode(void)
{
    return FALSE;
}

#if 0
#pragma mark -
#endif

/**
  Turn on LCD backlight

  Turn on LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOnLCDBacklight(void)
{
    gpio_clearPin(GPIO_PWM_BLG_PCTL);
   // gpio_setPin(GPIO_PWM_BLG_PCTL);
}

/**
  Turn off LCD backlight

  Turn off LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDBacklight(void)
{
    gpio_setPin(GPIO_PWM_BLG_PCTL);
    //gpio_clearPin(GPIO_PWM_BLG_PCTL);
}

/**
  Check whether LCD backlight is on or not

  Check whether LCD backlight is on or not.
  Return TRUE if LCD backlight is on, return FALSE if LCD backlight is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsLCDBacklightOn(void)
{
    //return !(BOOL)gpio_getPin(GPIO_PWM_BLG_PCTL);
        return (BOOL)gpio_getPin(GPIO_PWM_BLG_PCTL);

}

#if 0
#pragma mark -
#endif

/**
  Turn on LCD power

  Turn on LCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOnLCDPower(void)
{
    //gpio_clearPin(GPIO_LCD_SLEEP);
}

/**
  Turn off LCD power

  Turn off LCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDPower(void)
{
    //gpio_setPin(GPIO_LCD_SLEEP);
}

/**
  Check whether LCD power is on or not

  Check whether LCD power is on or not.
  Return TRUE if LCD power is on, return FALSE if LCD power is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsLCDPowerOn(void)
{
    return TRUE;
}

/**
  Detect LCD rotate

  Detect LCD rotate

  @param void
  @return BOOL: TRUE -> LCD rotate, FALSE -> LCD not rotate
*/
BOOL GPIOMap_DetLCDRotate(void)
{
    return FALSE;
}

#if 0
#pragma mark -
#endif

/**
  Turn on CCD power

  Turn on CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOnCCDPower(void)
{
   /* static BOOL FirstInit = TRUE;
    if (FirstInit == TRUE)
    {
        FirstInit = FALSE;
        return;
    }
    pll_enableClock(SIE_MCLK);
    Sensor_WriteReg8bit(0x0100, 0x01);
    pll_disableClock(SIE_MCLK);
    */

    //gpio_setPin(GPIO_CCD_RESET);
}

/**
  Turn off CCD power

  Turn off CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffCCDPower(void)
{
    /*pll_enableClock(SIE_MCLK);
    Sensor_WriteReg8bit(0x0100, 0x00);
    pll_disableClock(SIE_MCLK);*/
}

/**
  Check whether CCD power is on or not

  Check whether CCD power is on or not.
  Return TRUE if CCD power is on, return FALSE if CCD power is off.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsCCDPowerOn(void)
{
    /*pll_enableClock(SIE_MCLK);
    if (Sensor_ReadReg(0x030C) & 0x2)
    {
        return FALSE;
    }
    return TRUE;*/
}

#if 0
#pragma mark -
#endif

/**
  Turn on LED

  Turn on LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return void
*/
void GPIOMap_TurnOnLED(UINT32 uiLED)
{
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        ////gpio_setPin(GPIO_LED_G);
        break;

    case GPIOMAP_LED_RED:
        gpio_setPin(GPIO_LED_R);
        break;

    case GPIOMAP_LED_FCS:
    default:
        gpio_setPin(GPIO_LED_FCS);
        break;
    }
}

/**
  Turn off LED

  Turn off LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return void
*/
void GPIOMap_TurnOffLED(UINT32 uiLED)
{
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        ////gpio_clearPin(GPIO_LED_G);
        break;

    case GPIOMAP_LED_RED:
        gpio_clearPin(GPIO_LED_R);
        break;

    case GPIOMAP_LED_FCS:
    default:
        gpio_clearPin(GPIO_LED_FCS);
        break;
    }
}

/**
  Check whether LED is on or not

  Check whether LED is on or not.
  Return TRUE if LED is on, return FALSE if LED is off.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return BOOL
*/
BOOL GPIOMap_IsLEDOn(UINT32 uiLED)
{
    /*
    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        return (BOOL)gpio_getPin(GPIO_LED_G);

    case GPIOMAP_LED_RED:
        return (BOOL)gpio_getPin(GPIO_LED_R);

    case GPIOMAP_LED_FCS:
    default:
        return (BOOL)gpio_getPin(GPIO_LED_FCS);
    }
    */
    return FALSE;
}

#if 0
#pragma mark -
#endif

/**
  Charge flash or stop charging flash

  Charge flash or stop charging flash.

  @param BOOL bCharge: TRUE -> Charge flash, FALSE -> Stop charging flash
  @param BOOL bHighCurrent: TRUE -> Charging with high current, FALSE -> Charging with low current
  @return void
*/
void GPIOMap_ChargeFlash(BOOL bCharge, BOOL bHighCurrent)
{
    #if 0
    if (bCharge == TRUE)
    {

        PWM_struct PWMInfo;

        pwm_open(PWMID_6);
        PWMInfo.uiDiv       = 2;    // 75KHz
        PWMInfo.uiPrd       = 10;   // basic period
        PWMInfo.uiRise      = 0;    // +30
        PWMInfo.uiFall      = 4;    // -70
        PWMInfo.uiOnCycle   = 0;    // free run
        PWMInfo.uiInv       = 0;    // not invert
        pwm_set(PWMID_6, &PWMInfo);
        pwm_en(PWMID_6);
        debug_err(("Charging flash\r\n"));
    }
    else
    {

        pwm_close(PWMID_6);
        debug_err(("Stop charging flash\r\n"));
    }
    #endif
}

/**
  Check whether flash is charging or not

  Check whether flash is charging or not.
  Return TRUE if flash is charging, otherwise return FALSE.

  @param void
  @return BOOL
*/
BOOL GPIOMap_IsFlashCharging(void)
{
    return FALSE;
}

BOOL GPIOMap_IsFlashReady(void)
{
    return TRUE;
}

void GPIOMap_TriggerFlash(BOOL bOn)
{
    pinmux_select_flctr(PINMUX_GPIO);
    gpio_setDir(GPIO_FLASH_TRIGGER, GPIO_DIR_OUTPUT);

    if (bOn)
        gpio_setPin(GPIO_FLASH_TRIGGER);
    else
        gpio_clearPin(GPIO_FLASH_TRIGGER);
}

#if 0
#pragma mark -
#endif

/**
  Output poweroff control signal to high

  Output poweroff control signal to high.

  @param void
  @return void
*/
void GPIOMap_SetPoweroffCtrlHigh(void)
{
    //gpio_setPin(GPIO_POWEROFF_CTRL);
}

/**
  Output poweroff control signal to low

  Output poweroff control signal to low.

  @param void
  @return void
*/
void GPIOMap_SetPoweroffCtrlLow(void)
{
    //gpio_clearPin(GPIO_POWEROFF_CTRL);
}

#if 0
#pragma mark -
#endif

/**
  Turn on system power (enable power hold pin)

  Turn on system power (enable power hold pin).

  @param void
  @return void
*/
void GPIOMap_TurnOnSysPower(void)
{
    //gpio_setPin(GPIO_PWR_HOLD);
}

/**
  Turn off system power (release power hold pin)

  Turn off system power (release power hold pin).

  @param void
  @return void
*/
void GPIOMap_TurnOffSysPower(void)
{
    //gpio_clearPin(GPIO_PWR_HOLD);
}

void GPIOMap_SetNANDWp(void)
{
    gpio_setDir(GPIO_NAND_WP, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_NAND_WP);    // write protect
}

void GPIOMap_MShutOpen(void)
{
   pinmux_enable_mes(0);
   //gpio_setPin(GPIO_MSHUT_1);
   //gpio_clearPin(GPIO_MSHUT_2);
   gpio_clearPin(GPIO_MSHUT_1);
   gpio_setPin(GPIO_MSHUT_2);
}

void GPIOMap_MShutClose(void)
{
    pinmux_enable_mes(0);
    //gpio_clearPin(GPIO_MSHUT_1);
    //gpio_setPin(GPIO_MSHUT_2);
    gpio_setPin(GPIO_MSHUT_1);
    gpio_clearPin(GPIO_MSHUT_2);
}

void GPIOMap_MShutOff(void)
{
    pinmux_enable_mes(0);
    gpio_clearPin(GPIO_MSHUT_1);
    gpio_clearPin(GPIO_MSHUT_2);
}



//@}
