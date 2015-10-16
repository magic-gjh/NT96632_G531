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
#define GPIO_LCD_SLEEP              87
#define GPIO_LCD_RESET              88
// LCD backlight
#define GPIO_PWM_BLG_PCTL           50
// CCD power
#define GPIO_CCD_PCTL               51

// LED
#define GPIO_LED_R                  91
#define GPIO_LED_G                  53
#define GPIO_LED_FCS                48

// LCD
#define GPIO_LCD_SIF_SEN            P_GPIO_7
#define GPIO_LCD_SIF_SCK            P_GPIO_8
#define GPIO_LCD_SIF_SDA            P_GPIO_9

#define GPIO_MACRO_DETECT           28//add for tdc8a2vm

// Storage card detect
#define GPIO_CARD_DETECT            16
#define PAD_CARD_DETECT             PAD_PIN_MC16

// SD card write protect
#define GPIO_CARD_WP                32
#define PAD_CARD_WP                 PAD_PIN_MC32

// NAND write protect
#define GPIO_NAND_WP                (8 | GPIO_IS_DGPIO) //for tdc8a2vm
#define PAD_NAND_WP                 PAD_PIN_DGPIO8

// Flash charge
//#define GPIO_FLASH_CHARGE         30//for tdc8a2vm??

// Flash trigger
#define GPIO_FLASH_TRIGGER          22

// Flash ready
//#define GPIO_FLASH_READY          (8 | GPIO_IS_DGPIO)//for tdc8a2vm
//#define PAD_FLASH_READY           PAD_PIN_DGPIO8

// Mechanical shutter
#define GPIO_MSHUT_1                20
#define GPIO_MSHUT_2                21

//#define GPIO_POWER_CTRL             (7 | GPIO_IS_DGPIO)//for tdc8a2vm
//#define PAD_POWER_CTRL              PAD_PIN_DGPIO7


//--------------------------------------------------------------------
// DGPIO
//--------------------------------------------------------------------
// KEYS
//#define GPIO_KEY_ZOOMIN           (11 | GPIO_IS_DGPIO)//for tdc8a2vm
//#define PAD_KEY_ZOOMIN            PAD_PIN_DGPIO11
//#define GPIO_KEY_ZOOMOUT          (10 | GPIO_IS_DGPIO)
//#define PAD_KEY_ZOOMOUT           PAD_PIN_DGPIO10
#define GPIO_KEY_SHUTTER            (9 | GPIO_IS_DGPIO)
#define PAD_KEY_SHUTTER             PAD_PIN_DGPIO9
#define GPIO_KEY_UP                 (13 | GPIO_IS_DGPIO)
#define PAD_KEY_UP                  PAD_PIN_DGPIO13
#define GPIO_KEY_DOWN               (14 | GPIO_IS_DGPIO)
#define PAD_KEY_DOWN                PAD_PIN_DGPIO14
#define GPIO_KEY_LEFT               (15 | GPIO_IS_DGPIO)
#define PAD_KEY_LEFT                PAD_PIN_DGPIO15
#define GPIO_KEY_RIGHT              (16 | GPIO_IS_DGPIO)
#define PAD_KEY_RIGHT               PAD_PIN_DGPIO16
//#define GPIO_KEY_ENTER            (17 | GPIO_IS_DGPIO)
//#define PAD_KEY_ENTER             PAD_PIN_DGPIO17
#define GPIO_KEY_PB                 (17 | GPIO_IS_DGPIO)//for tdc8a2vm
#define PAD_KEY_PB                  PAD_PIN_DGPIO17
//#define GPIO_KEY_DISPLAY          (19 | GPIO_IS_DGPIO)
//#define PAD_KEY_DISPLAY           PAD_PIN_DGPIO19
#define GPIO_KEY_MENU               (18 | GPIO_IS_DGPIO)
#define PAD_KEY_MENU                PAD_PIN_DGPIO18
#define GPIO_KEY_MODE               (12 | GPIO_IS_DGPIO)
#define PAD_KEY_MODE                PAD_PIN_DGPIO12
//#define GPIO_KEY_POWER            (7 | GPIO_IS_DGPIO)
//#define PAD_KEY_POWER             PAD_PIN_DGPIO7



// Storage card power control pin
#define GPIO_CARD_POWER             (9 | GPIO_IS_DGPIO)

#define GPIO_TV_DETECT              (6 | GPIO_IS_DGPIO)
#define PAD_TV_DETECT               1 //Change to incorrect pad //PAD_PIN_TV_PLUGIN

#define GPIO_KEY_SHUTTER1           (7 | GPIO_IS_DGPIO)
#define PAD_KEY_SHUTTER1             2 //Change to incorrect pad //PAD_PIN_DGPIO7
#define GPIO_KEY_SHUTTER2           (8 | GPIO_IS_DGPIO)
#define PAD_KEY_SHUTTER2             3 //Change to incorrect pad //PAD_PIN_DGPIO8

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


//#NT#20090902#ethanlau -begin
#if (_SENSORLIB_ == _SENSORLIB_OV5653_630_)
#define GPIO_CCD_PWND   D_GPIO_0
#endif
//#NT#20090902#ethanlau -end
#endif
//@}
