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


// LCD backlight
#define GPIO_PWM_BLG_PCTL           D_GPIO_5

// LED
#define GPIO_LED_R                  C_GPIO_54//no use
#define GPIO_LED_G                  C_GPIO_53//no use
#define GPIO_LED_FCS                62//no use

//#define GPIO_WHITE_LED              C_GPIO_38

// LCD
#define GPIO_LCD_SIF_SEN            P_GPIO_7
#define GPIO_LCD_SIF_SCK            P_GPIO_8
#define GPIO_LCD_SIF_SDA            P_GPIO_9


//#define GPIO_LCD_ROTATE             (0 | GPIO_IS_DGPIO)
//#define PAD_LCD_ROTATE              PAD_PIN_DGPIO0
// TV
#define GPIO_TV_DETECT              C_GPIO_17
#define PAD_TV_DETECT               PAD_PIN_MC17


// Storage card power control pin
//#define DGPIO_CARD_POWER            S_GPIO_15

// Storage card detect
#define GPIO_CARD_DETECT            D_GPIO_6
#define PAD_CARD_DETECT             PAD_PIN_DGPIO6

// SD card write protect
//#define GPIO_CARD_WP                D_GPIO_4
//#define PAD_CARD_WP                 PAD_PIN_DGPIO4

// NAND write protect
//#define GPIO_NAND_WP                C_GPIO_15
//#define PAD_NAND_WP                 PAD_PIN_MC15

// CCD reset
#define GPIO_CCD_RESET              C_GPIO_56          
#define PAD_CCD_RESET              	PAD_PIN_MC56

//--------------------------------------------------------------------
// Other x_GPIO
//--------------------------------------------------------------------
//HDMI detection
#define GPIO_HDMI_DETECT            G_GPIO_11
#define PAD_HDMI_DETECT             PAD_PIN_HOTPLUG

#define GPIO_GSENSOR_INT           C_GPIO_47
#define PAD_GSENSOR_INT             PAD_PIN_MC47

#define GPIO_ACC_12V           C_GPIO_48
#define PAD_ACC_12V            PAD_PIN_MC48
#endif

//@}
