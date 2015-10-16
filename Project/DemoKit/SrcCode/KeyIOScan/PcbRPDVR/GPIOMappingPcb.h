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


//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
// LCD reset pin
//#define GPIO_PWR_ON              (0| GPIO_IS_DGPIO)
#define GPIO_PWR_ON              (1| GPIO_IS_DGPIO)


// LCD reset pin
#define GPIO_LCD_RESET              (2| GPIO_IS_DGPIO)

#define GPIO_PWM_BLG_PCTL           S_GPIO_15

// CCD power
#define GPIO_CCD_PCTL               C_GPIO_36

// CCD power
#define GPIO_CCD_SN_TRIGGER               C_GPIO_48

// Sensor RESET (V Arlen ???)
//#define GPIO_SN_OE                  D_GPIO_3


#define GPIO_CCD_RESET              D_GPIO_3


// LED
#define GPIO_LED_G                  S_GPIO_13
#define GPIO_LED_W                  S_GPIO_14
#define GPIO_LED_R                  S_GPIO_16

#define GPIO_MUTE_CTRL              C_GPIO_46

#define GPIO_POPSW_CTRL             C_GPIO_47

#define GPIO_IRIS_A                 S_GPIO_6
#define GPIO_IRIS_B                 S_GPIO_7

#define GPIO_MSH_A                  S_GPIO_9
#define GPIO_MSH_B                  S_GPIO_10







// LCD backlight (V Arlen ???)
//#define GPIO_PWM_LCD_PCTL           S_GPIO_15

// Storage card power control pin
#define GPIO_CARD_POWER             C_GPIO_39




// Charge battery enable
//#define GPIO_BAT_CHARGE             G_GPIO_3

//S_GPIO_10 --> BC
//#define GPIO_BC_PCTL                S_GPIO_10

// LCD interface (V Arlen)
#define GPIO_LCD_SIF_SEN            P_GPIO_7
#define GPIO_LCD_SIF_SCK            P_GPIO_8
#define GPIO_LCD_SIF_SDA            P_GPIO_9

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
#if 0
#define GPIO_POWER_CTRL             C_GPIO_49
#define PAD_POWER_CTRL              PAD_PIN_MC49
#endif

// TV detect
#define GPIO_TV_DETECT              C_GPIO_50
#define PAD_TV_DETECT               PAD_PIN_MC50


#define GPIO_POWER_CHARGE             C_GPIO_48
#define PAD_POWER_CHARGE            PAD_PIN_MC48


#define GPIO_ADP_DET             C_GPIO_53
#define PAD_ADP_DET              PAD_PIN_MC53


// HDMI in detect (V Arlen)
#define GPIO_HDMI_DETECT            G_GPIO_11
#define PAD_HDMI_DETECT             PAD_PIN_HOTPLUG


#define GPIO_TFT_DETECT               (0| GPIO_IS_DGPIO)
#define PAD_TFT_DETECT             PAD_PIN_DGPIO0

// Storage card detect (V Arlen)
#define GPIO_CARD_DETECT            (6 | GPIO_IS_DGPIO)
#define PAD_CARD_DETECT             PAD_PIN_DGPIO6

#if 0
// SD card write protect (V Arlen)
#define GPIO_CARD_WP                (4 | GPIO_IS_DGPIO)
#define PAD_CARD_WP                 PAD_PIN_DGPIO4
#endif

//--------------------------------------------------------------------
// Other
//--------------------------------------------------------------------
// Mode dial
#define GPIO_MODE_1                 50
#define PAD_MODE_1                  PAD_PIN_PWM2
#define GPIO_MODE_2                 51
#define PAD_MODE_2                  PAD_PIN_PWM3
#define GPIO_MODE_3                 52
#define PAD_MODE_3                  PAD_PIN_PWM4
#define GPIO_MODE_4                 53
#define PAD_MODE_4                  PAD_PIN_PWM5

#endif

//@}
