/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMappingPcb.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of GPIO mapping
                This file is the header file of GPIO mapping

    @note       Nothing.
    @date       2005/12/05
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _GPIOMAPPINGPCB_H
#define _GPIOMAPPINGPCB_H

#include "pad.h"
#include "GPIO.h"
#include "SysCfg.h"


//5V Power Supply
#define GPIO_5V_ENABLE              S_GPIO_12//PWM3

//LCD pwr ctrl
#define GPIO_LCD_PWR                D_GPIO_5//DGPIO5

#define GPIO_AUDIO_PWR              P_GPIO_16

#define GPIO_LCD_SLEEP              62//no use

//#define GPIO_LCD_RESET            (2| GPIO_IS_DGPIO)

// LCD backlight
#define GPIO_PWM_BLG_PCTL           S_GPIO_14//PWM5

// CCD power
//#define GPIO_CCD_PCTL             62      // no use

// CCD reset
#define GPIO_CCD_RESET              D_GPIO_3
#define GPIO_CCD_PWN                 C_GPIO_48

#define GPIO_CCD_PCTL               (11 | GPIO_IS_DGPIO)
#define PAD_CCD_PCTL                PAD_PIN_DGPIO11


// LED
#define GPIO_LED_R                  62//no use
#define GPIO_LED_G                  62//no use
#define GPIO_LED_FCS                62//no use

#define GPIO_WHITE_LED				D_GPIO_1

#define GPIO_POWER_LED              S_GPIO_9//PWM0
// LCD
#define GPIO_LCD_SIF_SEN            P_GPIO_7
#define GPIO_LCD_SIF_SCK            P_GPIO_8
#define GPIO_LCD_SIF_SDA            P_GPIO_9

#define GPIO_MACRO_DETECT           47

#define GPIO_LCD_ROTATE             C_GPIO_46
#define PAD_LCD_ROTATE              PAD_PIN_MC46

#define GPIO_LCD_CLOSE             C_GPIO_52
#define PAD_LCD_CLOSE              PAD_PIN_MC52

// TV
#define GPIO_TV_DETECT              C_GPIO_51//MC51
#define PAD_TV_DETECT               PAD_PIN_MC51 
#define GPIO_TV_ON                  P_GPIO_14 //SBCS3
#define PAD_TV_ON                   PAD_PIN_SBCS3

//sensor 
//#define GPIO_SENSOR_ROTATE			D_GPIO_3
//#define PAD_SENSOR_ROTATE			PAD_PIN_DGPIO3

// Storage card power control pin
#define DGPIO_CARD_POWER            S_GPIO_15

// Storage card detect
#define GPIO_CARD_DETECT            D_GPIO_6
#define PAD_CARD_DETECT             PAD_PIN_DGPIO6

// SD card write protect
#define GPIO_CARD_WP                D_GPIO_4
#define PAD_CARD_WP                 PAD_PIN_DGPIO4

// NAND write protect
#define GPIO_NAND_WP                C_GPIO_15
#define PAD_NAND_WP                 PAD_PIN_MC15

// Flash charge
//#define GPIO_FLASH_CHARGE         30//for tdc8a2vm??

// Flash trigger
#define GPIO_FLASH_TRIGGER          22

//Video Light
#define VIDEO_FLOOD_LIGHT         S_GPIO_8

// Flash ready
//#define GPIO_FLASH_READY          (8 | GPIO_IS_DGPIO)//for tdc8a2vm
//#define PAD_FLASH_READY           PAD_PIN_DGPIO8

// Mechanical shutter
#define GPIO_MSHUT_1                20
#define GPIO_MSHUT_2                21

#define GPIO_POWER_CTRL             (7 | GPIO_IS_DGPIO)
#define PAD_POWER_CTRL              PAD_PIN_DGPIO7

//#define GPIO_KEY_ENTER              C_GPIO_51
//#define PAD_KEY_ENTER               PAD_PIN_MC51
#define GPIO_KEY_MENU               C_GPIO_52
#define PAD_KEY_MENU                PAD_PIN_MC52
#define GPIO_KEY_SHUTTER2           C_GPIO_17//C_GPIO_55
#define PAD_KEY_SHUTTER2            PAD_PIN_MC17//PAD_PIN_MC55
//#define GPIO_KEY_MODE              C_GPIO_56
//#define PAD_KEY_MODE               PAD_PIN_MC56
#define GPIO_KEY_TELE                  P_GPIO_26//UART2_CTS/P_GPIO[26]
#define PAD_KEY_TELE                   PAD_PIN_UART2_CTS//PAD_PIN_UART2_RTS
#define GPIO_KEY_WIDE                  P_GPIO_25//UART2_RTS/P_GPIO[25]
#define PAD_KEY_WIDE                   PAD_PIN_UART2_RTS//PAD_PIN_UART2_CTS


// SYSRST
#define GPIO_SYSRST                 (19 | GPIO_IS_DGPIO)

// LENS_RST
#define GPIO_LENS_RST               (10 | GPIO_IS_DGPIO)

//--------------------------------------------------------------------
// Other x_GPIO
//--------------------------------------------------------------------
//HDMI detection
#define GPIO_HDMI_DETECT            G_GPIO_11
#define PAD_HDMI_DETECT             PAD_PIN_HOTPLUG

//HDMI detection
#define GPIO_DC5V_DETECT            C_GPIO_39//MC39
#define PAD_DC5V_DETECT             PAD_PIN_MC39

#endif

//@}
