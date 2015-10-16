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
#include "LCDTV.h"
#include "GlobalVar.h"

#include "gsensor.h"

void MI5131_SensorReset(void);
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
    debug_msg("DVR gpio init\r\n");
    // Open GPIO driver
    gpio_open();


    gpio_setDir(GPIO_PWR_ON, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_PWR_ON);

    // Configure output pins
    // LCD reset (low reset)
    gpio_setDir(GPIO_LCD_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_LCD_RESET);


    // LCD backlight
    gpio_setDir(GPIO_PWM_BLG_PCTL, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_PWM_BLG_PCTL);

    // CCD power
    gpio_setDir(GPIO_CCD_PCTL, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_PCTL);

    // Sensor OE
//    gpio_setDir(GPIO_SN_OE, GPIO_DIR_OUTPUT);
//    gpio_clearPin(GPIO_SN_OE);

    //Sensor hw reset
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_RESET);

#if 0
    // Power IC EN
    gpio_setDir(GPIO_PWR_ON, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_PWR_ON);
#endif

    // SN_TRIGGER
    gpio_setDir(GPIO_CCD_SN_TRIGGER, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_SN_TRIGGER);



#if 1
    // LED
    gpio_setDir(GPIO_LED_G, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_G);
    gpio_setDir(GPIO_LED_W, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_W);

    gpio_setDir(GPIO_LED_R, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_LED_R);
#endif

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







//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------


    pad_setPullUpDown(PAD_TFT_DETECT, PAD_PULLUP);
    gpio_setDir(GPIO_TFT_DETECT, GPIO_DIR_INPUT);


#if 0
    // TFT ONOFF DETECT POWER CONTROL
    pad_setPullUpDown(PAD_POWER_CTRL, PAD_PULLUP);
    gpio_setDir(GPIO_POWER_CTRL, GPIO_DIR_INPUT);
#endif


    // TV detect
    pad_setPullUpDown(PAD_TV_DETECT, PAD_PULLUP);
    gpio_setDir(GPIO_TV_DETECT, GPIO_DIR_INPUT);

    //GPIO_POWER_CHARGE
    pad_setPullUpDown(PAD_POWER_CHARGE, PAD_PULLUP);
    gpio_setDir(GPIO_POWER_CHARGE, GPIO_DIR_INPUT);

    // ADP detect
    pad_setPullUpDown(PAD_ADP_DET, PAD_PULLUP);
    gpio_setDir(GPIO_ADP_DET, GPIO_DIR_INPUT);

    // BC
//    gpio_setDir(GPIO_BC_PCTL, GPIO_DIR_OUTPUT);
//    gpio_setPin(GPIO_BC_PCTL);




#if 0
    // Battery charge
    pinmux_select_ickimsidi(PINMUX_GPIO);
    gpio_setDir(GPIO_BAT_CHARGE, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_BAT_CHARGE);
#endif

    // HMDI in detect
    gpio_setDir(GPIO_HDMI_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_HDMI_DETECT, PAD_PULLDOWN);

    // Storage card detect
    gpio_setDir(GPIO_CARD_DETECT, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_DETECT, PAD_PULLUP);

#if 0
    // SD card write protect
    gpio_setDir(GPIO_CARD_WP, GPIO_DIR_INPUT);
    pad_setPullUpDown(PAD_CARD_WP, PAD_PULLUP);
#endif
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
    BOOL bR = (gpio_getPin(GPIO_CARD_DETECT) == 0 ? FALSE : TRUE);

//      debug_msg("TV_DETECT = %d\r\n",gpio_getDir(GPIO_TV_DETECT));
//      debug_msg("POWER_CHARGE = %d\r\n",gpio_getDir(GPIO_POWER_CHARGE));
//      debug_msg("TFT_DETECT = %d\r\n",gpio_getDir(GPIO_TFT_DETECT));

//    debug_msg("GPIOMap_DetStrgCard = %d\r\n",bR);
    return bR;
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
    return FALSE;
#if 0//(_STORAGECARD_ == _STORAGECARD_SDMMC_ || _STORAGECARD_ == _STORAGECARD_SDIO_)
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
#if 0
    if (gpio_getPin(GPIO_KEY_SHUT1) == 0){
        uiKeyCode |= FLGKEY_SHUTTER1;
     //   debug_err(("SHUT1\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_SHUT2) == 0){
        uiKeyCode |= FLGKEY_SHUTTER2;
     //   debug_err(("SHUT2\r\n"));
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

    if (gpio_getPin(GPIO_KEY_MODE) == 0){
        uiKeyCode |= FLGKEY_MODE;
//        debug_err(("MODE\r\n"));
    }

    if (gpio_getPin(GPIO_KEY_ENTER) == 0){
        uiKeyCode |= FLGKEY_ENTER;
        //debug_err(("ENTER\r\n"));
    }
#endif
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
#if 0
    UINT32 mode=GPIOMAP_MD_PHOTO;//GPIOMAP_MD_PLAYBACK;
    mode =  gpio_getPin(GPIO_MODE_1)        |
            gpio_getPin(GPIO_MODE_2) << 1   |
            gpio_getPin(GPIO_MODE_3) << 2   |
            gpio_getPin(GPIO_MODE_4) << 3;

#endif
    return 0;//mode;
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







static BOOL g_TV_OutPut_ON = FALSE;
void GPIOMap_TurnOff_TV_OutPut(void)
{
//  debug_msg("GPIOMap_TurnOffIRlight\r\n");

    g_TV_OutPut_ON = FALSE;
}

void GPIOMap_TurnOn_TV_OutPut(void)
{
//  debug_msg("GPIOMap_TurnOnIRlight\r\n");

    g_TV_OutPut_ON = TRUE;
}

BOOL GPIOMap_Is_TV_OutPut(void)
{
//  debug_msg("GPIOMap_IsIRlightOn\r\n");

    return g_TV_OutPut_ON;
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
    static UINT32 uiCurStatus,uiPreStatus=0xFF;
    BOOL bzBool = FALSE;

    uiCurStatus = gpio_getPin(GPIO_TV_DETECT);

    if (gpio_getPin(GPIO_TV_DETECT)==0)
    {
        gpio_setPin(GPIO_TV_ON);
        //#NT#2009/12/03#Philex - begin
        // mark this line for fixed the following bug,
        // 插TV錄影 所成的錄影檔會沒生聲音
                //#NT#2009/12/03#Philex - end
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
    // return (BOOL)gpio_getPin(GPIO_TV_DETECT);
#else
    return GPIOMap_Is_TV_OutPut();//(BOOL)gpio_getPin(GPIO_TV_DETECT);
#endif
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
#if 0
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
#else
    return FALSE;
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
    return FALSE;
    //return (BOOL)gpio_getPin(GPIO_POWER_CTRL);
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
#if 0
    //#NT#2010/03/08#Chris Chung -begin
    return (BOOL)gpio_getPin(GPIO_TFT_OFF_DET);
    //#NT#2010/03/08#Chris Chung -end
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
    debug_msg("GPIOMap_TurnOnLCDBacklight\r\n");
    //LCD_RegWrite(0x05, 0x7E);
    gpio_setPin(GPIO_PWM_BLG_PCTL);

    // Delay, or PWM can't enable again. Need to confirm!!
    //DelayMs(10);

    // Send command to LCD to turn on internal PWM
    //VideoController_SendCommand(uiGPIOMapLCDBacklightOn);
}

/**
  Turn off LCD backlight

  Turn off LCD backlight.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDBacklight(void)
{
        // Send command to LCD to turn off internal PWM
    //VideoController_SendCommand(uiGPIOMapLCDBacklightOff);

#if 0
    // Wait for command sent
    if (g_LCDCtrlObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_waitTransferEnd();
    }
#endif
    // Delay, or PWM can't enable again. Need to confirm!!
    //DelayMs(10);

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
//    debug_err(("<< gpio_getPin(GPIO_PWM_BLG_PCTL) = %d\r\n", gpio_getPin(GPIO_PWM_BLG_PCTL)));
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
#if 0
    //gpio_clearPin(GPIO_LCD_SLEEP);
    gpio_setPin(GPIO_LCD_SLEEP);
#endif
}

/**
  Turn off LCD power

  Turn off LCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffLCDPower(void)
{
#if 0
    //gpio_setPin(GPIO_LCD_SLEEP);
//    debug_err(("TurnOffLCDPower\r\n"));
    gpio_clearPin(GPIO_LCD_SLEEP);
#endif
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
#if 0
    return (BOOL)gpio_getPin(GPIO_LCD_SLEEP);//TRUE;
#else
        return FALSE;
#endif
}

/**
  Detect LCD rotate

  Detect LCD rotate

  @param void
  @return BOOL: TRUE -> LCD rotate, FALSE -> LCD not rotate
*/
BOOL GPIOMap_DetLCDRotate(void)
{
    return !(BOOL)gpio_getPin(GPIO_ADP_DET);
#if 0
    //debug_err(("GPIOMap_DetLCDRotate=%d\n\r",!(BOOL)gpio_getPin(GPIO_KEY_LCD_ROT_DET)));
    //#NT#2010/01/25#Chris Chung -begin
    return !(BOOL)gpio_getPin(GPIO_KEY_LCD_ROT_DET);
    //#NT#2010/01/25#Chris Chung -end
#else
		return FALSE;
#endif
}

#if 0
#pragma mark -
#endif

void GPIOMap_TurnOnHDMI5V(void)
{
#if 0
    gpio_setPin(GPIO_HDMI_PCTL);
#else
        ;
#endif
}
void GPIOMap_TurnOffHDMI5V(void)
{
#if 0
    gpio_clearPin(GPIO_HDMI_PCTL);
#else
        ;
#endif
}

#if 0
#pragma mark -
#endif
static BOOL g_turnonCCDPower = FALSE;
/**
  Turn on CCD power

  Turn on CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOnCCDPower(void)
{
#if 0//((_SENSORLIB_ == _SENSORLIB_MI5130_630_) || (_SENSORLIB_ == _SENSORLIB_MI5131_630_))
//    MI5130_SensorStandby();
    //gpio_setDir(GPIO_CCD_STANDBY, GPIO_DIR_INPUT);
    //gpio_clearPin(GPIO_CCD_STANDBY);
    //TimerDelayMs(100);

    gpio_setDir(GPIO_CCD_VCC28V, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_VCC28V);
    TimerDelayMs(10);
//    TimerDelayMs(300);
    gpio_setDir(GPIO_CCD_VCC18V, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_VCC18V);
    TimerDelayMs(10);
    //MI5130_SensorReset(); //HW reset
    //TimerDelayMs(30);
    MI5131_SensorReset();
    g_turnonCCDPower = TRUE;
#endif
#if 0//(_SENSORLIB_ == _SENSORLIB_OV5653_630_)
    //#NT#2010/09/20#Jarkko Chang -begin
    //#NT#external DVDD source power on seq.
    gpio_setDir(GPIO_CCD_STANDBY, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_STANDBY);

    // CCD reset
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_CCD_RESET);

    gpio_setDir(GPIO_CCD_VCC28V, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_VCC28V);
    TimerDelayMs(10);

    gpio_setDir(GPIO_CCD_VCC18V, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_VCC18V);
    TimerDelayMs(10);
    gpio_setPin(GPIO_CCD_RESET);
    TimerDelayMs(20);
    gpio_clearPin(GPIO_CCD_STANDBY);
    TimerDelayMs(40);
//    debug_msg("---------->GPIOMap_TurnOnCCDPower\r\n");
    //#NT#2010/09/20#Jarkko Chang -end
    g_turnonCCDPower = TRUE;
#endif
}

/**
  Turn off CCD power

  Turn off CCD power.

  @param void
  @return void
*/
void GPIOMap_TurnOffCCDPower(void)
{
/*
    gpio_clearPin(GPIO_CCD_VCC28V);
    TimerDelayMs(5);

    gpio_clearPin(GPIO_CCD_VCC18V);
    TimerDelayMs(5);

    gpio_clearPin(GPIO_CCD_STANDBY);
    TimerDelayMs(5);
    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(5);

    g_turnonCCDPower = FALSE;
    */
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
    //return g_turnonCCDPower;
    return TRUE;
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
#if 0
    switch (uiLED)
    {
    case GPIOMAP_LED_MOVIE:
        gpio_setPin(GPIO_LED_R);
        break;
    case GPIOMAP_LED_PHOTO:
        gpio_setPin(GPIO_LED_G);
        break;
    case GPIOMAP_LED_PB:
        gpio_setPin(GPIO_LED_FCS);
        break;

    case GPIOMAP_LED_GREEN:
        //gpio_setPin(GPIO_LED_G);
        break;

    case GPIOMAP_LED_RED:
        //gpio_setPin(GPIO_LED_R);
        break;

    case GPIOMAP_LED_FCS:
    default:
        //gpio_setPin(GPIO_LED_FCS);
        break;
    }
#endif
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
#if 0
    switch (uiLED)
    {
    case GPIOMAP_LED_MOVIE:
        gpio_clearPin(GPIO_LED_R);
        break;
    case GPIOMAP_LED_PHOTO:
        gpio_clearPin(GPIO_LED_G);
        break;
    case GPIOMAP_LED_PB:
        gpio_clearPin(GPIO_LED_FCS);
        break;

    case GPIOMAP_LED_GREEN:
        //gpio_clearPin(GPIO_LED_G);
        break;

    case GPIOMAP_LED_RED:
        //gpio_clearPin(GPIO_LED_R);
        break;

    case GPIOMAP_LED_FCS:
    default:
        //gpio_clearPin(GPIO_LED_FCS);
        break;
    }
#endif
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
#if 0
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
#endif
    return FALSE;
}

//#PIC#2010/04/20#Creator -begin
void GPIOMap_TurnOnFloodLight(BOOL bEn)
{
#if 0
    pinmux_select_flctr(PINMUX_GPIO);
    gpio_setDir(VIDEO_FLOOD_LIGHT, GPIO_DIR_OUTPUT);

    if (bEn)
      gpio_setPin(VIDEO_FLOOD_LIGHT);
    else
			gpio_clearPin(VIDEO_FLOOD_LIGHT);
#endif
}
BOOL GPIOMap_IsFloodLightOn(void)
{
#if 0
    return (BOOL)gpio_getPin(VIDEO_FLOOD_LIGHT);
#else
        return FALSE;
#endif
}
//#PIC#2010/04/20#Creator -end
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
    return;
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
    //return (BOOL)gpio_getPin(GPIO_FLASH_CHARGE);
    //return FALSE;
}

BOOL GPIOMap_IsFlashReady(void)
{
#if 0
    return (BOOL)gpio_getPin(GPIO_FLASH_READY);
    //ntk@winston 2008/04/08 @add for tdc8a2vm begin
    //return SenFP_IsFlashReady();
    //ntk@winston 2008/04/08 @add for tdc8a2vm end
#else
        return FALSE;
#endif
}

void GPIOMap_TriggerFlash(BOOL bOn)
{
#if 0
    //debug_err(("!!!! GPIOMap_TriggerFlash = %d\n\r",bOn));
    pinmux_select_flctr(PINMUX_GPIO);
    gpio_setDir(GPIO_FLASH_TRIGGER, GPIO_DIR_OUTPUT);

    if (bOn)
        gpio_setPin(GPIO_FLASH_TRIGGER);
    else
        gpio_clearPin(GPIO_FLASH_TRIGGER);
#endif
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
#if 0
    gpio_setDir(GPIO_NAND_WP, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_NAND_WP);    // write protect
#endif
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

//#NT#2009/10/29#Robin Chen -begin
//modify for hd500c5
#if 1
void MI5130_SensorReset(void)
{
    debug_err(("Sensor Reset\r\n"));
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_RESET);
    gpio_clearPin(GPIO_CCD_RESET);
    DelayMs(20);
    gpio_setPin(GPIO_CCD_RESET);
    debug_err(("Sensor Reset Fin\r\n"));
}
#endif

void MI5131_SensorReset(void)
{
    debug_err(("Sensor Reset\r\n"));
    gpio_setDir(GPIO_CCD_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_RESET);
    gpio_clearPin(GPIO_CCD_RESET);
    DelayMs(20);
    gpio_setPin(GPIO_CCD_RESET);
    debug_err(("Sensor Reset Fin\r\n"));
}

void GPIOMap_MI1040_SensorReset(void)
{
    debug_err(("Sensor Reset\r\n"));

    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(100);
    gpio_setPin(GPIO_CCD_RESET);

//    gpio_setPin(GPIO_SN_OE);
//    TimerDelayMs(30);
//    gpio_clearPin(GPIO_SN_OE);

    debug_err(("Sensor Reset Fin\r\n"));
}

//#NT#2009/10/29#Robin Chen -end

//@}
