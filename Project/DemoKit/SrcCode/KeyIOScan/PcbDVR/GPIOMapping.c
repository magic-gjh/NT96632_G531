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

    // M-shutter pin 1
    gpio_setDir(GPIO_MSHUT_1, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_MSHUT_1);
//--------------------------------------------------------------------
// PINMUX
//--------------------------------------------------------------------
    //HDMI
    pinmux_select_hotplug(PINMUX_GPIO);

    pinmux_enable_mes(PINMUX_GPIO);

    //Video Light
    pinmux_select_flctr(PINMUX_GPIO);//VIDEO_FLOOD_LIGHT

    //key HDMI
    gpio_setDir(GPIO_HDMI_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_HDMI_DETECT, PAD_PULLDOWN);

    // M-shutter pin 2
    gpio_setDir(GPIO_MSHUT_2, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_MSHUT_2);

    // NAND write protect (low protect)
    gpio_setDir(GPIO_NAND_WP, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_NAND_WP);

    // Storage Card detect (low detect)
    gpio_setDir(GPIO_CARD_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_DETECT, PAD_PULLUP);

    // Storage Card write protect (high protect)
    gpio_setDir(GPIO_CARD_WP, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_WP, PAD_PULLDOWN/*PAD_PULLUP?*/);

    // SD power(low: power on   high:power off)
    gpio_setDir(DGPIO_CARD_POWER, GPIO_DIR_OUTPUT);
    gpio_clearPin(DGPIO_CARD_POWER);

    // key
    //gpio_setDir(GPIO_KEY_ENTER, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_KEY_ENTER, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_MENU, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_MENU, PAD_PULLUP);
    //gpio_setDir(GPIO_KEY_MODE, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_KEY_MODE, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_SHUTTER2, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_SHUTTER2, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_TELE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_TELE, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_WIDE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_WIDE, PAD_PULLUP);

    // Macro detect,low detect
    //gpio_setDir(GPIO_MACRO_DETECT, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_PIN_SBCS1, PAD_PULLUP);
    //gpio_setPin(GPIO_MACRO_DETECT);

    // TV detect,low detect
    //pinmux_select_goice(PINMUX_GPIO);//#Linfg@0915#for dv121na
    gpio_setDir(GPIO_TV_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_TV_DETECT, PAD_PULLUP);
    gpio_setDir(GPIO_TV_ON, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_TV_ON);

    //gpio_setDir(GPIO_CCD_PCTL, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_CCD_PCTL);

    //shutter key
    //pinmux_select_ick(PINMUX_GPIO);
    //gpio_setDir(GPIO_KEY_SHUTTER, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_KEY_SHUTTER, PAD_PULLUP);

    //Key POWER
    //gpio_setDir(GPIO_KEY_POWER, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_KEY_POWER, PAD_PULLUP);

    // POWER Control
    //gpio_setDir(GPIO_POWER_CTRL, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_POWER_CTRL, PAD_PULLDOWN);

    // LED
    /*gpio_setDir(GPIO_LED_R, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_LED_R);*/
    gpio_setDir(GPIO_LED_FCS, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_FCS);

    gpio_setDir(GPIO_WHITE_LED, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_WHITE_LED);

    gpio_setDir(VIDEO_FLOOD_LIGHT, GPIO_DIR_OUTPUT);
    gpio_clearPin(VIDEO_FLOOD_LIGHT);

    // LCD reset (low reset)
    //gpio_setDir(GPIO_LCD_RESET, GPIO_DIR_OUTPUT);//#Linfg@0915#for dv121na
    //gpio_setPin(GPIO_LCD_RESET);

    // LCD power (ON: HIGH; SLEEP: LOW)
    gpio_setDir(GPIO_LCD_PWR, GPIO_DIR_OUTPUT);//#Linfg@0915#for dv121na
    gpio_setPin(GPIO_LCD_PWR);

    // LCD backlight (ON: HIGH)
    //gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_PWM_BLG_PCTL);

    // Sensor power (low active)
    //gpio_setDir(GPIO_CCD_PCTL, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_CCD_PCTL);

#if (_SENSORLIB_ == _SENSORLIB_OV5653_630_)
    //gpio_setDir(GPIO_CCD_PWN, GPIO_DIR_OUTPUT);
    //gpio_setPin(GPIO_CCD_PWN);
    gpio_setDir(GPIO_CCD_PWN, GPIO_DIR_INPUT);
#elif (_SENSORLIB_ == _SENSORLIB_OV9710_630_)
    gpio_setDir(GPIO_CCD_PWN, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_PWN);
    //gpio_setDir(GPIO_CCD_PWN, GPIO_DIR_INPUT);
#elif (_SENSORLIB_ == _SENSORLIB_NT99140_632_)
    gpio_setDir(GPIO_CCD_PWN, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_PWN);
#endif
    // Sensor reset (low reset)
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_RESET);
    TimerDelayMs(10);
    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(10);
    gpio_setPin(GPIO_CCD_RESET);
    TimerDelayMs(10);

    // Flash charge (high active)
    //gpio_setDir(GPIO_FLASH_CHARGE, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_FLASH_CHARGE);

    // Flash trigger (high active)
    gpio_setDir(GPIO_FLASH_TRIGGER, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_FLASH_TRIGGER);

    gpio_setDir(GPIO_5V_ENABLE, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_5V_ENABLE);

    // Flash ready
    //gpio_setDir(GPIO_FLASH_READY, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_FLASH_READY, PAD_PULLDOWN);

    //Open audio driver
    gpio_setDir(GPIO_AUDIO_PWR, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_AUDIO_PWR);

    //Open power led
    gpio_setDir(GPIO_POWER_LED, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_POWER_LED);

    gpio_setDir(GPIO_DC5V_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_DC5V_DETECT, PAD_NONE);

         // LCDRotate
    gpio_setDir(GPIO_LCD_ROTATE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_LCD_ROTATE,PAD_PULLUP);

    //LCD CLOSE
    gpio_setDir(GPIO_LCD_CLOSE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_LCD_CLOSE,PAD_PULLUP);

    // LCDRotate
    //gpio_setDir(GPIO_SENSOR_ROTATE, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_SENSOR_ROTATE,PAD_PULLUP);
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
    return (gpio_getPin(GPIO_CARD_DETECT) == 0 ? TRUE : FALSE);
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
#if (_STORAGECARD_ == _STORAGECARD_SDMMC_ || _STORAGECARD_ == _STORAGECARD_SDIO_ || _STORAGECARD_ == _STORAGECARD_SDIO2_)
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
   UINT32 uiKeyCode = 0;

    //if (gpio_getPin(GPIO_KEY_ENTER) == 0)
    //{
    //    uiKeyCode |= FLGKEY_ENTER;
        //debug_err(("ENTER\r\n"));
    //}

    //if (gpio_getPin(GPIO_KEY_MENU) == 0)
    //{
    //    uiKeyCode |= FLGKEY_MENU;
        //debug_err(("MENU\r\n"));
    //}

    //if (gpio_getPin(GPIO_KEY_MODE) == 0)
    //{
     //   uiKeyCode |= FLGKEY_MODE;
        //debug_err(("MOVIE\r\n"));
    //}

    if (gpio_getPin(GPIO_KEY_SHUTTER2) == 0)
    {
        uiKeyCode |= FLGKEY_ENTER;//FLGKEY_SHUTTER2;
        //debug_err(("FLGKEY_REC\r\n"));
    }
    if (gpio_getPin(GPIO_KEY_TELE) == 0)
    {
        uiKeyCode |= FLGKEY_UP;//FLGKEY_DOWN;
        //debug_err(("FLGKEY_UP\r\n"));
    }
    if (gpio_getPin(GPIO_KEY_WIDE) == 0)
    {
        uiKeyCode |= FLGKEY_DOWN;//FLGKEY_UP;
        //debug_err(("FLGKEY_DOWN\r\n"));
    }
    uiKeyCode |= KeyADC_GetKey();

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
#if (VOLDET_TV_EARPHONE == ENABLE)
    static UINT32 uiCurStatus,uiPreStatus=0xFF;
    BOOL bzBool = FALSE;

    uiCurStatus = gpio_getPin(GPIO_TV_DETECT);
    if (gpio_getPin(GPIO_TV_DETECT)==0)
    {
        gpio_setPin(GPIO_TV_ON);
        if (VolDet_GetTVInLevel()){ // TV plug in
            bzBool =TRUE;
        } else { // earphone plug in
            bzBool = FALSE;
        }
    } else { // TV/earPhone plug out
        if (uiCurStatus!=uiPreStatus){
            gpio_clearPin(GPIO_TV_ON);
        }
        bzBool = FALSE;
    }
    uiPreStatus = uiCurStatus;
    return (BOOL)bzBool;
#else
    return (BOOL)!gpio_getPin(GPIO_TV_DETECT);
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
    //if (gpio_getPin(GPIO_KEY_SHUTTER2) == 0)
    //{
    //    return TRUE;
    //}
    //return FALSE;

    return (BOOL)gpio_getPin(GPIO_DC5V_DETECT);
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
#if 0
    return FALSE;
#else
    static UINT32 uiCurStatus,uiPreStatus=0xFF;
    BOOL bzBool = FALSE;

    uiCurStatus = gpio_getPin(GPIO_TV_DETECT);
    if (gpio_getPin(GPIO_TV_DETECT)==0)
    {
        gpio_setPin(GPIO_TV_ON);
        //#NT#2009/12/03#Philex - begin
        // mark this line for fixed the following bug,
        // 插TV錄影 所成的錄影檔會沒生聲音
//        aud_setOutput(AUDIO_OUTPUT_HP);
                //#NT#2009/12/03#Philex - end
        if (VolDet_GetTVInLevel() == FALSE){ // earphone plug in
            bzBool =TRUE;
        }
    } else { // TV/earPhone plug out
        if (uiCurStatus!=uiPreStatus){
            gpio_clearPin(GPIO_TV_ON);
        }
        bzBool = FALSE;
    }
    uiPreStatus = uiCurStatus;
    return (BOOL)bzBool;
//    return FALSE;
#endif
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

#if 0
#pragma mark -
#endif

/**
  Detect engineering mode special key

  Detect engineering mode special key

  @param void
  @return BOOL: TRUE -> Is engineering mode requested, FALSE -> Isn't engineering mode requested
*/
BOOL GPIOMap_DetEngMode(void)
{
    #if 0
    return (gpio_getPin(GPIO_KEY_ENTER) == 0 ? TRUE : FALSE);
    #else
    return FALSE;
    #endif
}

/**
  Turn on LCD backlight

  Turn on LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOnLCDBacklight(void)
{
    gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);
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
    gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_PWM_BLG_PCTL);
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
    // backlight control of DV569, ON: HIGH, OFF: LOW
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
    //gpio_setPin(GPIO_LCD_SLEEP);
}

/**
  Turn off LCD power

  Turn off LCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDPower(void)
{
    //gpio_clearPin(GPIO_LCD_SLEEP);
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
    return (BOOL)gpio_getPin(GPIO_LCD_SLEEP);
}

/**
  Detect LCD rotate

  Detect LCD rotate

  @param void
  @return BOOL: TRUE -> LCD rotate, FALSE -> LCD not rotate
*/
BOOL GPIOMap_DetLCDRotate(void)
{
    return !(BOOL)gpio_getPin(GPIO_LCD_ROTATE);
}

/**
  Detect LCD Close

  Detect LCD Close

  @param void
  @return BOOL: TRUE -> LCD rotate, FALSE -> LCD not rotate
*/
BOOL GPIOMap_DetLCDClose(void)
{
    return !(BOOL)gpio_getPin(GPIO_LCD_CLOSE);
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

//#NT#2008/11/28#Winston Li -begin
    //gpio_setPin(GPIO_CCD_PCTL);//enable sensor power
   // gpio_setPin(GPIO_CCD_RESET);
//#NT#2008/11/28#Winston Li -end
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
    pll_disableClock(SIE_MCLK); */
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
    /*
    pll_enableClock(SIE_MCLK);
    if (Sensor_ReadReg(0x030C) & 0x2)
    {
        return FALSE;
    }
    */
    return TRUE;
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

    return FALSE;
}

void GPIOMap_TurnOnWhiteLight(UINT32 uiLED)
{
    gpio_setPin(GPIO_WHITE_LED);
}
void GPIOMap_TurnOffWhiteLight(UINT32 uiLED)
{
    gpio_clearPin(GPIO_WHITE_LED);
}
BOOL GPIOMap_IsWhiteLightOn(UINT32 uiLED)
{
    return (BOOL)gpio_getPin(GPIO_WHITE_LED);
}

void GPIOMap_TurnOnIRLed(UINT32 uiLED)
{
    gpio_setPin(VIDEO_FLOOD_LIGHT);
}
void GPIOMap_TurnOffIRLed(UINT32 uiLED)
{
    gpio_clearPin(VIDEO_FLOOD_LIGHT);
}
BOOL GPIOMap_IsIRLedOn(UINT32 uiLED)
{
    return (BOOL)gpio_getPin(VIDEO_FLOOD_LIGHT);
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
    /*
    if (bCharge == TRUE)
    {
        gpio_setPin(GPIO_FLASH_CHARGE);
    }
    else
    {
        gpio_clearPin(GPIO_FLASH_CHARGE);
    }
    */

    /* DV569 has no flash light!!
    //ntk@winston 2008/04/08 @add for tdc8a2vm begin
    if (bCharge == TRUE)
    {

        PWM_struct PWMInfo;

        pwm_open(PWMID_2);
        PWMInfo.uiDiv       = 2;    // 75KHz
        PWMInfo.uiPrd       = 10;   // basic period
        PWMInfo.uiRise      = 0;    // +30
        PWMInfo.uiFall      = 4;    // -70
        PWMInfo.uiOnCycle   = 0;    // free run
        PWMInfo.uiInv       = 0;    // not invert
        pwm_set(PWMID_2, &PWMInfo);
        pwm_en(PWMID_2);
        //debug_err(("Charge flash\r\n"));
    }
    else
    {
        pwm_close(PWMID_2);
        //debug_err(("Stop charging flash\r\n"));
    }
    //ntk@winston 2008/04/08 @add for tdc8a2vm end
    */
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
    //return (BOOL)gpio_getPin(GPIO_FLASH_CHARGE);
    return FALSE;
}

BOOL GPIOMap_IsFlashReady(void)
{
    //return (BOOL)gpio_getPin(GPIO_FLASH_READY);
    //ntk@winston 2008/04/08 @add for tdc8a2vm begin
    return SenFP_IsFlashReady();
    //ntk@winston 2008/04/08 @add for tdc8a2vm end
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
    //pinmux_select_mes(0);
    //gpio_setPin(GPIO_MSHUT_1);
   // gpio_clearPin(GPIO_MSHUT_2);
   gpio_clearPin(GPIO_MSHUT_1);
   gpio_setPin(GPIO_MSHUT_2);
}

void GPIOMap_MShutClose(void)
{
    //pinmux_select_mes(0);
    //gpio_clearPin(GPIO_MSHUT_1);
    //gpio_setPin(GPIO_MSHUT_2);
    gpio_setPin(GPIO_MSHUT_1);
    gpio_clearPin(GPIO_MSHUT_2);
}

void GPIOMap_MShutOff(void)
{
    //pinmux_select_mes(0);
    gpio_clearPin(GPIO_MSHUT_1);
    gpio_clearPin(GPIO_MSHUT_2);
}
void MI5120_SensorReset(void)
{
    debug_err(("Sensor Reset\r\n"));
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(100);
    gpio_setPin(GPIO_CCD_RESET);
}

void MI5120_SensorStandby(void)
{
    /*
    debug_err(("Sensor Standby\r\n"));
    gpio_setDir(GPIO_CCD_PWN, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_PWN);
    DelayMs(100);
    gpio_setPin(GPIO_CCD_PWN);
    */
}

BOOL GPIOMap_TurnOnAudioPower(UINT32 uiLED)
{
    gpio_setPin(GPIO_AUDIO_PWR);
}
BOOL GPIOMap_TurnOffAudioPower(UINT32 uiLED)
{
    gpio_clearPin(GPIO_AUDIO_PWR);
}

BOOL GPIOMap_TurnOnPowerLed(void)
{
    gpio_setPin(GPIO_POWER_LED);
}
BOOL GPIOMap_TurnOffPowerLed(void)
{
    gpio_clearPin(GPIO_POWER_LED);
}
BOOL GPIOMap_IsPowerLedOn(void)
{
    return (BOOL)gpio_getPin(GPIO_POWER_LED);
}

BOOL GPIOMap_DetSensorRotate(void)
{
    //#PIC#2011/03/01#Creator -begin
    //MENU CONTROL ROTATE
    //return !gpio_getPin(GPIO_SENSOR_ROTATE);
    return FALSE;//Get_SensorRotateIndex();
    //#PIC#2011/03/01#Creator -end

}
//@}
