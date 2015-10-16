/**
    Header file of GPIO mapping

    This file is the header file of GPIO mapping

    @file       GPIOMapphing.h
    @ingroup    mIPRJAPKeyIO
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _GPIOMAPPINGPCB_H
#define _GPIOMAPPINGPCB_H

#include "pad.h"
#include "gpio.h"

/**
    @addtogroup mIPRJAPKeyIO
*/
//@{

//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
// LCD reset pin
#define GPIO_PWR_ON              (0| GPIO_IS_DGPIO)

// LCD reset pin
#define GPIO_LCD_RESET              (2| GPIO_IS_DGPIO)
// LCD backlight
#define GPIO_PWM_BLG_PCTL           S_GPIO_15

// CCD power
#define GPIO_CCD_PCTL               C_GPIO_36

//#NT#20100410#ethanlau -begin
#define GPIO_CCD_RESET              D_GPIO_3
//#NT#20100410#ethanlau -end
// LCD CS/CLK/DATA
#define GPIO_LCD_SIF_SEN            (P_GPIO_7)//SBCS0/P_GPIO[7]
#define GPIO_LCD_SIF_SCK            (P_GPIO_8)//SBCK0/P_GPIO[8]
#define GPIO_LCD_SIF_SDA            (P_GPIO_9)//SBDAT0/P_GPIO[9]


// LED
#define GPIO_LED_G                  S_GPIO_13
#define GPIO_LED_W                  S_GPIO_14

#define GPIO_MUTE_CTRL              C_GPIO_46

#define GPIO_POPSW_CTRL             C_GPIO_47

#define GPIO_IRIS_A                 S_GPIO_6
#define GPIO_IRIS_B                 S_GPIO_7

#define GPIO_MSH_A                  S_GPIO_9
#define GPIO_MSH_B                  S_GPIO_10

#define GPIO_FLASH_TRIGGER          S_GPIO_8
#define GPIO_FLASH_CHARGE           S_GPIO_14

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
#define GPIO_POWER_CTRL             C_GPIO_49
#define PAD_POWER_CTRL              PAD_PIN_MC49

#define GPIO_TFT_ROATAE             C_GPIO_53
#define PAD_TFT_ROTATE              PAD_PIN_MC53

#define GPIO_FLASH_READY            C_GPIO_51
#define PAD_FLASH_READY             PAD_PIN_MC51
// Macro key detect
#define GPIO_KEY_MACRO              C_GPIO_48
#define PAD_KEY_MACRO               PAD_PIN_MC48

// AV in detect
#define GPIO_TV_DETECT              C_GPIO_50
#define PAD_TV_DETECT               PAD_PIN_MC50

// HDMI in detect
#define GPIO_HDMI_DETECT            G_GPIO_11
#define PAD_HDMI_DETECT             PAD_PIN_HOTPLUG

// Storage card detect
#define GPIO_CARD_DETECT            D_GPIO_6
#define PAD_CARD_DETECT             PAD_PIN_DGPIO6

// SD card write protect
#define GPIO_CARD_WP                D_GPIO_4
#define PAD_CARD_WP                 PAD_PIN_DGPIO4

#endif

//@}
