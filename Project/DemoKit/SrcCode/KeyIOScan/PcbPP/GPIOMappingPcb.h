/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPIOMapphing.h
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

//--------------------------------------------------------------------
// AGPIO - Output
//--------------------------------------------------------------------
#define GPIO_LCD_SLEEP              (1 | GPIO_IS_DGPIO)

// LCD backlight
#define GPIO_PWM_BLG_PCTL           (5 | GPIO_IS_DGPIO)
#define PAD_PWM_BLG_PCTL               PAD_PIN_DGPIO5
// CCD power
#define GPIO_CCD_PCTL               (C_GPIO_37)
#define PAD_CCD_PCTL             PAD_PIN_MC37

// CCD RESET
#define GPIO_CCD_RESET               (3 | GPIO_IS_DGPIO)
#define PAD_CCD_RESET             PAD_PIN_DGPIO3

// CCD RESET
#define GPIO_LCD_ROTATE               (1 | GPIO_IS_DGPIO)
#define PAD_LCD_ROTATE             PAD_PIN_DGPIO1

// HDMI POWER
#define GPIO_HDMI_POWER               (C_GPIO_53)
#define PAD_HDMI_POWER             PAD_PIN_MC53

// LED
#define GPIO_LED_R                  (2 | GPIO_IS_DGPIO)

#define GPIO_LED_G                  53
#define GPIO_LED_FCS                (2 | GPIO_IS_DGPIO)

// LCD
#define GPIO_LCD_SIF_SEN            P_GPIO_7
#define GPIO_LCD_SIF_SCK            P_GPIO_8
#define GPIO_LCD_SIF_SDA            P_GPIO_9

#define GPIO_MACRO_DETECT           (C_GPIO_35)
#define PAD_MACRO_DETECT                 PAD_PIN_MC35

#define GPIO_USB_DETECT           (C_GPIO_32)
#define PAD_USB_DETECT                 PAD_PIN_MC32
// Storage card detect
#define GPIO_CARD_DETECT            (6 | GPIO_IS_DGPIO)
#define PAD_CARD_DETECT             PAD_PIN_DGPIO6

// SD card write protect
#define GPIO_CARD_WP                (4 | GPIO_IS_DGPIO)
#define PAD_CARD_WP                 PAD_PIN_DGPIO4

// NAND write protect
#define GPIO_NAND_WP                (C_GPIO_17)
#define PAD_NAND_WP                 PAD_PIN_MC17



// Flash trigger
#define GPIO_FLASH_TRIGGER          (S_GPIO_8)
#define PAD_FLASH_TRIGGER                 PAD_PIN_SN_FLCTR

// Flash ready
//#define GPIO_FLASH_READY          (8 | GPIO_IS_DGPIO)//for tdc8a2vm
//#define PAD_FLASH_READY           PAD_PIN_DGPIO8

// Mechanical shutter
#define GPIO_MSHUT_1                20
#define GPIO_MSHUT_2                21


//--------------------------------------------------------------------
// DGPIO
//--------------------------------------------------------------------

#define GPIO_KEY_SHUTTER            (C_GPIO_33)
#define PAD_KEY_SHUTTER             PAD_PIN_MC33
#define GPIO_KEY_MODE               (C_GPIO_34)
#define PAD_KEY_MODE                PAD_PIN_MC34


// Storage card power control pin//high on
#define GPIO_CARD_POWER        (C_GPIO_54)
#define PAD_CARD_POWER 	PAD_PIN_MC54

#define GPIO_TV_DETECT              (C_GPIO_17)
#define PAD_TV_DETECT               PAD_PIN_MC17


#define GPIO_POWER_CTRL             (0 | GPIO_IS_DGPIO)
#define PAD_POWER_CTRL              PAD_PIN_DGPIO0

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


#endif

//@}
