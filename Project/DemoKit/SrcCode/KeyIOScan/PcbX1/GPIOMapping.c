/*
    GPIO mapping

    This file do GPIO initialization and GPIO detection

    @file       GPIOMapphing.c
    @ingroup    mIPRJAPKeyIO
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "GPIOMapping.h"
#include "KeyScanTsk.h"
#include "top.h"
#include "pll.h"
#include "SensorFP.h"
#include "PWM.h"
#include "LCDTV.h"

/**
    @addtogroup mIPRJAPKeyIO
*/
//@{

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

    // Configure output pins
    // LCD reset (low reset)
    gpio_setDir(GPIO_LCD_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_LCD_RESET);
    // LCD backlight
    gpio_clearPin(GPIO_PWM_BLG_PCTL);
    gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);

    // CCD power
    gpio_clearPin(GPIO_CCD_PCTL);
    gpio_setDir(GPIO_CCD_PCTL, GPIO_DIR_OUTPUT);

    //sensor hw reset
    //#NT#20100410#ethanalu -begin
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_RESET);
    //#NT#20100410#ethanlau -end

    // Power
    gpio_setDir(GPIO_PWR_ON, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_PWR_ON);

    // LED
    gpio_setDir(GPIO_LED_G, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_G);
    gpio_setDir(GPIO_LED_W, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_W);

    gpio_setDir(GPIO_MUTE_CTRL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_MUTE_CTRL);

    gpio_setDir(GPIO_POPSW_CTRL, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_POPSW_CTRL);

    // IRIS & MSH GPIO CONFIG
    gpio_setDir(GPIO_IRIS_A, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_IRIS_A);
    gpio_setDir(GPIO_IRIS_B, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_IRIS_B);

    gpio_setDir(GPIO_MSH_A, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_MSH_A);
    gpio_setDir(GPIO_MSH_B, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_MSH_B);

    // strobe charge
    gpio_setDir(GPIO_FLASH_CHARGE, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_FLASH_CHARGE);

    pad_setPullUpDown(PAD_FLASH_READY, PAD_PULLUP);
    gpio_setDir(GPIO_FLASH_READY, GPIO_DIR_INPUT);

    // TFT ONOFF DETECT POWER CONTROL
    pad_setPullUpDown(PAD_POWER_CTRL, PAD_PULLUP);
    gpio_setDir(GPIO_POWER_CTRL, GPIO_DIR_INPUT);

    // TFT Roatae detect
    pad_setPullUpDown(PAD_TFT_ROTATE, PAD_PULLUP);
    gpio_setDir(GPIO_TFT_ROATAE, GPIO_DIR_INPUT);

    // Configure input pins
    // Macro key detect
    pad_setPullUpDown(PAD_KEY_MACRO, PAD_PULLDOWN);
    gpio_setDir(GPIO_KEY_MACRO, GPIO_DIR_INPUT);

    // AV in detect
    pad_setPullUpDown(PAD_TV_DETECT, PAD_PULLUP);
    gpio_setDir(GPIO_TV_DETECT, GPIO_DIR_INPUT);

     // HMDI in detect
    pad_setPullUpDown(PAD_HDMI_DETECT, PAD_PULLDOWN);
    gpio_setDir(GPIO_HDMI_DETECT, GPIO_DIR_INPUT);

    // Storage card detect
    pad_setPullUpDown(PAD_CARD_DETECT, PAD_PULLUP);
    gpio_setDir(GPIO_CARD_DETECT, GPIO_DIR_INPUT);

    // SD card write protect
    pad_setPullUpDown(PAD_CARD_WP, PAD_PULLUP);
    gpio_setDir(GPIO_CARD_WP, GPIO_DIR_INPUT);
}

/**
    Detect Storage card is inserted or not

    Detect Storage card is inserted or not.
    Return TRUE if storage card is inserted, FALSE if storage card is removed.

    @return TRUE -> Storage card inserted, FALSE -> Storage card removed
*/
BOOL GPIOMap_DetStrgCard(void)
{
    return (gpio_getPin(GPIO_CARD_DETECT) == 1 ? TRUE : FALSE);
}

/**
    Detect Storage card is write protected or not

    Detect Storage card is write protected or not.
    Return TRUE if storage card is write protected, FALSE if storage card is not write protected.

    @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL GPIOMap_DetStrgCardWP(void)
{
    return FALSE;
    return (gpio_getPin(GPIO_CARD_WP) == 0 ? FALSE : TRUE);
}

/**
  Detect key is pressed.

  Detect key is pressed.
  Return key pressed status (refer to KeyScanTsk.h)

  @param void
  @return UINT32
*/
UINT32 GPIOMap_DetKey(void)
{
    return 0;
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
    return (gpio_getPin(GPIO_TV_DETECT) == 0 ? TRUE : FALSE);
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
    return !(BOOL)gpio_getPin(GPIO_POWER_CTRL);
}

/**
  Detect macro

  Detect macro

  @param void
  @return BOOL: TRUE -> Is macro, FALSE -> Isn't macro
*/
BOOL GPIOMap_DetMacro(void)
{
    return (BOOL)gpio_getPin(GPIO_KEY_MACRO);
}
BOOL GPIOMap_DetDel(void)
{
    UINT32  uiTestKey = KeyADC_GetKey();
    //debug_err(("ENTER TEST MODE:0x%.8x\n\r", uiTestKey));

    return uiTestKey & FLGKEY_DEL;
}

/**
  Detect engineering mode special key

  Detect engineering mode special key

  @param void
  @return BOOL: TRUE -> Is engineering mode requested, FALSE -> Isn't engineering mode requested
*/
BOOL GPIOMap_DetEngMode(void)
{
    #if 0
    //return (gpio_getPin(GPIO_KEY_SHUTTER) == 0 ? TRUE : FALSE);
    return ((KeyADC_GetKey() & FLGKEY_SHUTTER2) == FLGKEY_SHUTTER2 ? TRUE : FALSE);
    #else
    return FALSE;
    #endif
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
    //LCD_RegWrite(0x05, 0x7E);
    gpio_setPin(GPIO_PWM_BLG_PCTL);
}

/**
  Turn off LCD backlight

  Turn off LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDBacklight(void)
{
    gpio_clearPin(GPIO_PWM_BLG_PCTL);
    //LCD_RegWrite(0x05, 0x7C);
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
    return gpio_getPin(GPIO_TFT_ROATAE);
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
    debug_msg("%s,GPIO_CCD_PCTL:%d\r\n",__func__,GPIO_CCD_PCTL);
    gpio_clearPin(GPIO_CCD_PCTL);
}

/**
  Turn off CCD power

  Turn off CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffCCDPower(void)
{
    gpio_setPin(GPIO_CCD_PCTL);
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
    return (BOOL)gpio_getPin(GPIO_CCD_PCTL);
}

#if 0
#pragma mark -
#endif

/**
  Turn on LED

  Turn on LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_WHITE
    GPIOMAP_LED_FCS
  @return void
*/
void GPIOMap_TurnOnLED(UINT32 uiLED)
{

    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        gpio_setPin(GPIO_LED_G);
        break;
    case GPIOMAP_LED_WHITE:
        gpio_setPin(GPIO_LED_W);
        break;

    case GPIOMAP_LED_FCS:
    default:
        //gpio_setPin(GPIO_LED_FCS);
        break;
    }

    //gpio_setPin(GPIO_LED_G);
}

/**
  Turn off LED

  Turn off LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_WHITE
    GPIOMAP_LED_FCS
  @return void
*/
void GPIOMap_TurnOffLED(UINT32 uiLED)
{

    switch (uiLED)
    {
    case GPIOMAP_LED_GREEN:
        gpio_clearPin(GPIO_LED_G);
        break;

    case GPIOMAP_LED_WHITE:
        gpio_clearPin(GPIO_LED_W);
        break;

    case GPIOMAP_LED_FCS:
    default:
        //gpio_clearPin(GPIO_LED_FCS);
        break;
    }

    //gpio_clearPin(GPIO_LED_G);
}

/**
  Check whether LED is on or not

  Check whether LED is on or not.
  Return TRUE if LED is on, return FALSE if LED is off.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_WHITE
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

    case GPIOMAP_LED_WHITE:
        return (BOOL)gpio_getPin(GPIO_LED_W);

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
    #if 1
    if (bCharge == TRUE)
    {
        gpio_setPin(GPIO_FLASH_CHARGE);
        /*
        PWM_struct PWMInfo;

        pwm_open(PWMID_5);
        PWMInfo.uiDiv       = 59;    // 75KHz
        PWMInfo.uiPrd       = 16;   // basic period
        if (bHighCurrent)
        {
            PWMInfo.uiRise      = 0;    //
            PWMInfo.uiFall      = 14;   //
        }
        else
        {
            PWMInfo.uiRise      = 0;    //
            PWMInfo.uiFall      = 7;    //
        }
        PWMInfo.uiOnCycle   = 0;    // free run
        PWMInfo.uiInv       = 0;    // not invert
        pwm_set(PWMID_5, &PWMInfo);
        pwm_en(PWMID_5);
        */
        debug_err(("...start charging flash\r\n"));
    }
    else
    {
        gpio_clearPin(GPIO_FLASH_CHARGE);
        //pwm_close(PWMID_5);
        debug_err(("...Stop charging flash\r\n"));
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
    return (BOOL) FALSE;
    return (BOOL)gpio_getPin(GPIO_FLASH_CHARGE);
}

BOOL GPIOMap_IsFlashReady(void)
{
    return (BOOL) TRUE;
    return (BOOL)gpio_getPin(GPIO_FLASH_READY);
}

void GPIOMap_TriggerFlash(BOOL bOn)
{
    return;
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
    /*
    gpio_setDir(GPIO_NAND_WP, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_NAND_WP);    // write protect
    */
}

void GPIOMap_MShutOpen(void)
{

}

void GPIOMap_MShutClose(void)
{

}

void GPIOMap_MShutOff(void)
{

}

void GPIOMap_TurnOnSpeaker(void)
{
    gpio_setPin(GPIO_MUTE_CTRL);
}

void GPIOMap_TurnOffSpeaker(void)
{
    gpio_clearPin(GPIO_MUTE_CTRL);
}

BOOL GPIOMap_IsMute(void)
{
    return !(BOOL)gpio_getPin(GPIO_MUTE_CTRL);
}

void GPIOMap_POPSWActive(BOOL bON)
{
    if (bON)
        gpio_setPin(GPIO_POPSW_CTRL);
    else
        gpio_clearPin(GPIO_POPSW_CTRL);
}
//@}
