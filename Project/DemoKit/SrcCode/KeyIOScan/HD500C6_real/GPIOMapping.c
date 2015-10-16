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
#include "VoltageDet.h"
#include "audio.h"

void GPIOMap_TriggerFlash(BOOL bOn);

void DelayMs(UINT32 ms)
{
    UINT32 Delay;

    if(TimerDelayMs(ms)==0)
        return;
    for(Delay=0;Delay<ms*1000;Delay++)
        ;
}


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
    debug_err(("HD500C6_Real: GPIOMap_Init()\r\n"));
    // Open GPIO driver
    gpio_open();

    // M-shutter pin 1
    //gpio_setDir(GPIO_MSHUT_1, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_MSHUT_1);

    // M-shutter pin 2
    //gpio_setDir(GPIO_MSHUT_2, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_MSHUT_2);

    // NAND write protect (low protect)
    //gpio_setDir(GPIO_NAND_WP, GPIO_DIR_OUTPUT);
    //gpio_setPin(GPIO_NAND_WP);

    // Storage Card detect (low detect)
    gpio_setDir(GPIO_CARD_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_DETECT, PAD_PULLUP);

    // Storage Card write protect (low protect)
    gpio_setDir(GPIO_CARD_WP, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_WP, PAD_PULLDOWN);

    // TFT_OFF_DET pin
    gpio_setDir(GPIO_TFT_OFF_DET, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_TFT_OFF_DET, PAD_PULLUP);

    // Macro detect,low detect
    //gpio_setDir(GPIO_MACRO_DETECT, GPIO_DIR_INPUT);
    //pad_setPullUpDown(PAD_PIN_SBCS1, PAD_PULLUP);
    //gpio_setPin(GPIO_MACRO_DETECT);
    // TV ON,low detect
    gpio_setDir(GPIO_TV_ON, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_TV_ON);
//    gpio_setPin(GPIO_TV_ON);

    // TV detect,low detect
    //pinmux_select_goice(PINMUX_GPIO);
    gpio_setDir(GPIO_TV_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_TV_DETECT, PAD_PULLUP);
//    pad_setPullUpDown(PAD_TV_DETECT, PAD_PULLUP);

    // HDMI detect,low detect
    //pinmux_select_goice(PINMUX_GPIO);
    gpio_setDir(GPIO_KEY_HDMI_DET, GPIO_DIR_INPUT);
    pad_setPullUpDown(GPIO_KEY_HDMI_DET, PAD_PULLUP);
//    pad_setPullUpDown(GPIO_KEY_HDMI_DET, PAD_PULLDOWN);
    // HDMI +5V power(high: power on   low:power off)
    gpio_setDir(GPIO_HDMI_PCTL, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_HDMI_PCTL);
    gpio_setPin(GPIO_HDMI_PCTL);

    // SD power(low: power on   high:power off)
    gpio_setDir(GPIO_SD_PCTL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SD_PCTL);

    // LED
    gpio_setDir(GPIO_LED_R, GPIO_DIR_OUTPUT);
    gpio_setDir(GPIO_LED_G, GPIO_DIR_OUTPUT);
    //#NT#2009/10/21#Philex Lin - begin
    // mark this line to meet ADPK's spec
//    gpio_clearPin(GPIO_LED_R);
    //#NT#2009/10/21#Philex Lin - end
    gpio_setDir(GPIO_LED_FCS, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_LED_FCS);

    // LCD reset (low reset)
    //gpio_setDir(GPIO_LCD_RESET, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_LCD_RESET);

    // LCD SHDN (low reset)
    //gpio_setDir(GPIO_LCD_SHDN, GPIO_DIR_OUTPUT);
    //gpio_setPin(GPIO_LCD_SHDN);

    // LCD power (ON: High; SLEEP: low)
    gpio_setDir(GPIO_LCD_SLEEP, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_LCD_SLEEP);

    //#NT#2009/03/17#Hideo Lin -begin
    //#NT#Modify LCD backlight initial status
    // LCD backlight (ON: LOW)
    gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_PWM_BLG_PCTL);
    //#NT#2009/03/17#Hideo Lin -end

    // Sensor power (low active)
    //gpio_setDir(GPIO_CCD_PCTL, GPIO_DIR_OUTPUT);
    //gpio_clearPin(GPIO_CCD_PCTL);

    // Sensor reset (low reset)
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    //gpio_setPin(GPIO_CCD_RESET);
    gpio_setPin(GPIO_CCD_RESET);

    // Sensor standby (low standby)
    gpio_setDir(GPIO_CCD_STANDBY, GPIO_DIR_OUTPUT);
    //gpio_setPin(GPIO_CCD_RESET);
    gpio_setPin(GPIO_CCD_STANDBY);

    // Flash charge (high active)
    gpio_setDir(GPIO_FLASH_CHARGE, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_FLASH_CHARGE);

    // Flash trigger (high active)
    gpio_setDir(GPIO_FLASH_TRIGGER, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_FLASH_TRIGGER);

    // Flash ready
    gpio_setDir(GPIO_FLASH_READY, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_FLASH_READY, PAD_PULLDOWN);

    // Low battery ctrl (high active)
    gpio_setDir(GPIO_BC_CTL, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_BC_CTL);

    // TV plug-in detected
//    gpio_setDir(GPIO_TV_DETECT, GPIO_DIR_INPUT);
//    pad_setPullUpDown(GPIO_TV_DETECT, PAD_PULLUP);

    //LCD rotation detection
    gpio_setDir(GPIO_KEY_LCD_ROT_DET, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_LCD_ROT_DET, PAD_NONE);


    // Key init
    gpio_setDir(GPIO_KEY_ENTER, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_ENTER, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_SHUT1, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_SHUT1, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_SHUT2, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_SHUT2, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_MOVIE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_MOVIE, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_TELE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_TELE, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_WIDE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_WIDE, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_MENU, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_MENU, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_MODE, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_MODE, PAD_PULLUP);
    gpio_setDir(GPIO_KEY_NETWORK, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_KEY_NETWORK, PAD_PULLUP);

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
//    return FALSE;
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
#if (_STORAGECARD_ == _STORAGECARD_SDMMC_ || _STORAGECARD_ == _STORAGECARD_SDIO_)
    debug_err(("[GPIOMap_DetStrgCardWP]=%d\n\r",(gpio_getPin(GPIO_CARD_WP) == 0 ? FALSE : TRUE)));
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

    if (gpio_getPin(GPIO_KEY_SHUT1) == 0){
        uiKeyCode |= FLGKEY_SHUTTER1;
//        debug_err(("SHUT1\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_SHUT2) == 0){
        uiKeyCode |= FLGKEY_SHUTTER2;
//        debug_err(("SHUT2\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_TELE) == 0){
        uiKeyCode |= FLGKEY_ZOOMIN;
//        debug_err(("TELE\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_WIDE) == 0){
        uiKeyCode |= FLGKEY_ZOOMOUT;
//        debug_err(("WIDE\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_MENU) == 0){
        uiKeyCode |= FLGKEY_MENU;
//        debug_err(("MENU\r\n"));
    }
#if 0
    if (gpio_getPin(GPIO_KEY_NETWORK) == 0){
        uiKeyCode |= FLGKEY_NETWORK;
//        debug_err(("NETWORK\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_MOVIE) == 0){
        uiKeyCode |= FLGKEY_MOVIE;
//        debug_err(("MOVIE\r\n"));
    }
#endif
    if (gpio_getPin(GPIO_KEY_MODE) == 0){
        uiKeyCode |= FLGKEY_MODE;
//        debug_err(("MODE\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_ENTER) == 0){
        uiKeyCode |= FLGKEY_ENTER;
        //debug_err(("ENTER\r\n"));
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
    UINT32 mode=GPIOMAP_MD_PHOTO;//GPIOMAP_MD_PLAYBACK;
#if 0
    mode =  gpio_getPin(GPIO_MODE_1)        |
            gpio_getPin(GPIO_MODE_2) << 1   |
            gpio_getPin(GPIO_MODE_3) << 2   |
            gpio_getPin(GPIO_MODE_4) << 3;

#endif
    return mode;
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
    return (BOOL)gpio_getPin(GPIO_KEY_HDMI_DET);
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
  static UINT32 uiCurStatus,uiPreStatus=0xFF;
  BOOL bzBool = FALSE;

    uiCurStatus = gpio_getPin(GPIO_TV_DETECT);
    #if(_PCB_ == _PCB_HD500C6_REAL_)
    if (gpio_getPin(GPIO_TV_DETECT)==0)
    #else
    if (gpio_getPin(GPIO_TV_DETECT))
    #endif
    {
        gpio_setPin(GPIO_TV_ON);
        //#NT#2009/12/03#Philex - begin
        // mark this line for fixed the following bug,
        // 插TV錄影 所成的錄影檔會沒生聲音
//        aud_setOutput(AUDIO_OUTPUT_HP);
				//#NT#2009/12/03#Philex - end
        if (VolDet_GetTVInLevel()){ // TV plug in
            bzBool =TRUE;
        } else { // earphone plug in
            bzBool = FALSE;
        }
    } else { // TV/earPhone plug out
        if (uiCurStatus!=uiPreStatus){
            gpio_clearPin(GPIO_TV_ON);
//            aud_setOutput(AUDIO_OUTPUT_SPK);
        }
        bzBool = FALSE;
    }
    uiPreStatus = uiCurStatus;
    return (BOOL)bzBool;
    // return (BOOL)gpio_getPin(GPIO_TV_DETECT);
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
    //return FALSE;
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
    //return (BOOL)gpio_getPin(GPIO_MACRO_DETECT);
    return FALSE;
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


static BOOL g_White_LDE_on = FALSE;
static UINT32 g_Fall;
BOOL GPIOMap_IsWhiteLightOn()
{
    return g_White_LDE_on;
}
/**
  Turn on white light (for video recording)

  Turn on white light (for video recording)

  @param void
  @return void
*/
void GPIOMap_TurnOnWhiteLight()
{
    PWM_struct  PWMInfo;
    UINT32      err, id = PWMID_5;
    g_White_LDE_on = TRUE;
    GPIOMap_TriggerFlash(TRUE);
#if 0
    pinmux_enable_pwm(PINMUX_PWM5);
    pwm_close(id);
    err = pwm_open(id);
    PWMInfo.uiDiv       = 1;    // 200KHz (base-clock (48MHz) / (div + 1) / basic-period)
    PWMInfo.uiPrd       = 120;   // basic period
    PWMInfo.uiRise      = 0;    //
    PWMInfo.uiFall      = 5;//temp solution,need to check//25;//30;//20;//10;//48;
    PWMInfo.uiOnCycle   = 0;    // free run
    PWMInfo.uiInv       = 0;    // not invert

    err = pwm_set(id, &PWMInfo);
    err = pwm_en(id);
    g_White_LDE_on = TRUE;
    g_Fall = PWMInfo.uiFall;
    debug_err(("[GPIOMap_TurnOnWhiteLight] = 40\n\r"));
#endif
}

/**
  Turn off white light (for video recording)

  Turn off white light (for video recording)

  @param void
  @return void
*/
void GPIOMap_TurnOffWhiteLight(void)
{
//    pinmux_disable_pwm(PINMUX_PWM5);
    GPIOMap_TriggerFlash(FALSE);
    g_White_LDE_on = FALSE;
}


BOOL GPIOMap_IsTFTOff(void)
{
    //#NT#2010/03/08#Chris Chung -begin
    return (BOOL)gpio_getPin(GPIO_TFT_OFF_DET);
    //#NT#2010/03/08#Chris Chung -end
}


/**
  Turn on LCD backlight

  Turn on LCD backlight.

  @param void
  @return void
*/
static BOOL g_bLCDBacklightOn = TRUE;
/**
void GPIOMap_TurnOnLCDBacklight(void)
{
#if (_LCDTYPE_ == _LCDTYPE_AUCN01_ || _LCDTYPE_ == _LCDTYPE_TOPPOLYTD020_)
    ////TurnOnBacklight();
#endif
    //gpio_clearPin(GPIO_PWM_BLG_PCTL);
    gpio_setPin(GPIO_PWM_BLG_PCTL);
    g_bLCDBacklightOn = TRUE;
}
**/
void GPIOMap_TurnOnLCDBacklight(void)
{
    //#NT#2010/3/29#Philex - begin
    // panel off in low power condition(< 3.3V)
    if (VolDet_GetEmptyLevel()==FALSE)
    //#NT#2010/3/29#Philex - end
    {
        if (KeyScan_IsTVPlugIn() == FALSE &&
            KeyScan_IsHDMIPlugIn() == FALSE)
            gpio_setPin(GPIO_PWM_BLG_PCTL);
    }
}

/**
  Turn off LCD backlight

  Turn off LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDBacklight(void)
{
    //gpio_setPin(GPIO_PWM_BLG_PCTL);
    gpio_clearPin(GPIO_PWM_BLG_PCTL);
#if (_LCDTYPE_ == _LCDTYPE_AUCN01_ || _LCDTYPE_ == _LCDTYPE_TOPPOLYTD020_)
    ////TurnOffBacklight();
#endif
    g_bLCDBacklightOn = FALSE;
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
    debug_err(("<< gpio_getPin(GPIO_PWM_BLG_PCTL) = %d\r\n", gpio_getPin(GPIO_PWM_BLG_PCTL)));
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
    gpio_setPin(GPIO_LCD_SLEEP);

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
//    debug_err(("TurnOffLCDPower\r\n"));
    gpio_clearPin(GPIO_LCD_SLEEP);
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
    return (BOOL)gpio_getPin(GPIO_LCD_SLEEP);//TRUE;
}

/**
  Detect LCD rotate

  Detect LCD rotate

  @param void
  @return BOOL: TRUE -> LCD rotate, FALSE -> LCD not rotate
*/
BOOL GPIOMap_DetLCDRotate(void)
{
    //#NT#2010/01/25#Chris Chung -begin
    return !(BOOL)gpio_getPin(GPIO_KEY_LCD_ROT_DET);
    //#NT#2010/01/25#Chris Chung -end
}

#if 0
#pragma mark -
#endif

void GPIOMap_TurnOnHDMI5V(void)
{
    gpio_setPin(GPIO_HDMI_PCTL);
}
void GPIOMap_TurnOffHDMI5V(void)
{
    gpio_clearPin(GPIO_HDMI_PCTL);
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
    gpio_setPin(GPIO_CCD_RESET);
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
        gpio_setPin(GPIO_LED_G);
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
        gpio_clearPin(GPIO_LED_G);
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
    }
    else
    {
        gpio_clearPin(GPIO_FLASH_CHARGE);
    }
    #else
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

        pwm_stop(PWMID_2);
        //debug_err(("Stop charging flash\r\n"));
    }
    //ntk@winston 2008/04/08 @add for tdc8a2vm end
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
    return (BOOL)gpio_getPin(GPIO_FLASH_CHARGE);
    //return FALSE;
}

BOOL GPIOMap_IsFlashReady(void)
{
    return (BOOL)gpio_getPin(GPIO_FLASH_READY);
    //ntk@winston 2008/04/08 @add for tdc8a2vm begin
    //return SenFP_IsFlashReady();
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
//    pinmux_select_mes(0);
    //gpio_setPin(GPIO_MSHUT_1);
   // gpio_clearPin(GPIO_MSHUT_2);
   //gpio_clearPin(GPIO_MSHUT_1);
   //gpio_setPin(GPIO_MSHUT_2);
}

void GPIOMap_MShutClose(void)
{
//    pinmux_select_mes(0);
    //gpio_clearPin(GPIO_MSHUT_1);
    //gpio_setPin(GPIO_MSHUT_2);
    //gpio_setPin(GPIO_MSHUT_1);
    //gpio_clearPin(GPIO_MSHUT_2);
}

void GPIOMap_MShutOff(void)
{
//    pinmux_select_mes(0);
    //gpio_clearPin(GPIO_MSHUT_1);
    //gpio_clearPin(GPIO_MSHUT_2);
}
void MI5120_SensorReset(void)
{
    debug_err(("Sensor Reset\r\n"));
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_RESET);
    DelayMs(100);
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



//@}
