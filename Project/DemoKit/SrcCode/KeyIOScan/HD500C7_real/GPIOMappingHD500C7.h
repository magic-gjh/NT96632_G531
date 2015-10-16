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


// LCD power (ON: High; SLEEP: low)
#define GPIO_LCD_SLEEP              (D_GPIO_5)//DGPIO5
#define PAD_LCD_SLEEP                 PAD_PIN_DGPIO5
// LCD reset (low reset)
//#define GPIO_LCD_RESET              (P_GPIO_33)//SPI_DO/P_GPIO[33]
//#define PAD_LCD_RESET                 PAD_PIN_SPI_DO
// LCD SHDN (low reset)
//#define GPIO_LCD_SHDN              (P_GPIO_32)//SPI_CLK/P_GPIO[32]
//#define PAD_LCD_SHDN                 PAD_PIN_SPI_CLK
// LCD backlight
#define GPIO_PWM_BLG_PCTL           (S_GPIO_14)//PWM5/S_GPIO[14]
#define PAD_PWM_BLG_PCTL                 PAD_PIN_PWM5
// LCD CS/CLK/DATA
#define GPIO_LCD_SIF_SEN            (P_GPIO_7)//SBCS0/P_GPIO[7]
#define GPIO_LCD_SIF_SCK            (P_GPIO_8)//SBCK0/P_GPIO[8]
#define GPIO_LCD_SIF_SDA            (P_GPIO_9)//SBDAT0/P_GPIO[9]


// CCD power
//#define GPIO_CCD_PCTL             62      // no use
// CCD reset
#define GPIO_CCD_RESET              (D_GPIO_2)//DGPIO2
#define PAD_CCD_RESET                 PAD_PIN_DGPIO2
// CCD standby
#define GPIO_CCD_STANDBY         (D_GPIO_3)//DGPIO3
#define PAD_CCD_STANDBY                 PAD_PIN_DGPIO3
//CCD power
//#define GPIO_CCD_PCTL               (11 | GPIO_IS_DGPIO)
//#define PAD_CCD_PCTL                PAD_PIN_DGPIO11

//Video Light
#define VIDEO_FLOOD_LIGHT         S_GPIO_8


// Flash trigger
#define GPIO_FLASH_TRIGGER          S_GPIO_8//FLCTR/S_GPIO[8]
// Flash charge
//#define GPIO_FLASH_CHARGE           S_GPIO_9//S_GPIO_13//PWM4/S_GPIO[13]
// Flash ready
#define GPIO_FLASH_READY             P_GPIO_16//SBDAT23/P_GPIO[16]
#define PAD_FLASH_READY               PAD_PIN_SBDAT23



//SD Power Control(low:power on   high:power off)
#define GPIO_SD_PCTL                (S_GPIO_15)//PWM6/S_GPIO[15]
#define PAD_SD_PCTL                 PAD_PIN_PWM6
//SD card detect(low:SD inserted   high:SD removed)
//#define GPIO_CARD_DETECT           (D_GPIO_6)//DGPIO6
//#define PAD_CARD_DETECT             PAD_PIN_DGPIO6
// SD card write protect
#define GPIO_CARD_WP                 (D_GPIO_4)//DGPIO4
#define PAD_CARD_WP                 PAD_PIN_DGPIO4
//SD card detect(low:SD inserted   high:SD removed)
#define GPIO_CARD_DETECT           (D_GPIO_6)//DGPIO6
#define PAD_CARD_DETECT             PAD_PIN_DGPIO6



//HDMI detection
#define GPIO_KEY_HDMI_DET           G_GPIO_11//HOTPLUG/G_GPIO[11]
#define PAD_KEY_HDMI_DET            PAD_PIN_HOTPLUG
#define GPIO_HDMI_PCTL                (S_GPIO_12)//PWM3/S_GPIO[12]
#define PAD_HDMI_PCTL                 PAD_PIN_PWM3

//TV ON
#define GPIO_TV_ON                  (P_GPIO_14) //SBCS3/P_GPIO[14]
#define PAD_TV_ON                  PAD_PIN_SBCS3
#define GPIO_TV_DETECT              (P_GPIO_31)//SPI_CS/P_GPIO[31]
#define PAD_TV_DETECT                 PAD_PIN_SPI_CS

#define GPIOMAP_LED_MOVIE           3
#define GPIOMAP_LED_PHOTO           4
#define GPIOMAP_LED_PB              5
// LED
#define GPIO_LED_R                  S_GPIO_9//PWM0/S_GPIO[9]
#define GPIO_LED_G                  S_GPIO_11//PWM1/S_GPIO[10]
#define GPIO_LED_FCS                S_GPIO_10//PWM2/S_GPIO[11]


// NAND write protect
#define GPIO_NAND_WP                C_GPIO_15//MC15/C_GPIO[15]
#define PAD_NAND_WP                 PAD_PIN_MC15


//Key
//Shutter
#define GPIO_KEY_ENTER                 C_GPIO_51//MC51/C_GPIO[51]
#define PAD_KEY_ENTER                  PAD_PIN_MC51
#define GPIO_KEY_SHUT1                 C_GPIO_54//MC54/C_GPIO[54]
#define PAD_KEY_SHUT1                  PAD_PIN_MC54
#define GPIO_KEY_SHUT2                 C_GPIO_55//MC55/C_GPIO[55]
#define PAD_KEY_SHUT2                  PAD_PIN_MC55
#define GPIO_KEY_MOVIE                 C_GPIO_56//MC56/C_GPIO[56]
#define PAD_KEY_MOVIE                  PAD_PIN_MC56

#define GPIO_KEY_TELE                  P_GPIO_26//P_GPIO_25//UART2_CTS/P_GPIO[26]
#define PAD_KEY_TELE                   PAD_PIN_UART2_CTS//PAD_PIN_UART2_RTS
#define GPIO_KEY_WIDE                  P_GPIO_25//P_GPIO_26//UART2_RTS/P_GPIO[25]
#define PAD_KEY_WIDE                   PAD_PIN_UART2_RTS//PAD_PIN_UART2_CTS
#define GPIO_KEY_MENU                  C_GPIO_52//UART2_RX/P_GPIO[22]
#define PAD_KEY_MENU                   PAD_PIN_MC52
#define GPIO_KEY_NETWORK               C_GPIO_53//UART2_RX/P_GPIO[22]
#define PAD_KEY_NETWORK                PAD_PIN_MC53

#define GPIO_KEY_MODE               C_GPIO_39
#define PAD_KEY_MODE                PAD_PIN_MC39

// TFT_Off_Det pin
#define GPIO_TFT_OFF_DET                C_GPIO_38
#define PAD_TFT_OFF_DET                 PAD_PIN_MC38

//Zoom Lens
#define GPIO_MOTOR_POWER               D_GPIO_1//D_GPIO1
#define PAD_MOTOR_POWER                 PAD_PIN_DGPIO1
#define GPIO_MOTOR_CLKEN               L_GPIO_11
#define PAD_MOTOR_CLKEN                 PAD_PIN_LCD11


//Low Battery Ctrl
#define GPIO_BC_CTL               D_GPIO_0//D_GPIO1
#define PAD_BC_CTL                PAD_PIN_DGPIO0

//LCD rotation detection
//#NT#2010/03/08#Chris Chung -begin
#define GPIO_KEY_LCD_ROT_DET             C_GPIO_32
#define PAD_KEY_LCD_ROT_DET              PAD_PIN_MC32
//#NT#2010/03/08#Chris Chung -end

#define GPIO_POWER_CTRL             (7 | GPIO_IS_DGPIO)
#define PAD_POWER_CTRL              PAD_PIN_DGPIO7

// CMOS Power on Sequence Contorl
#define GPIO_CCD_VCC28V            (S_GPIO_16)//  (P_GPIO_16)//SBDAT23/P_GPIO[16]
#define GPIO_CCD_VCC18V            (S_GPIO_13)//PWM4/S_GPIO[13]
#if 0
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
#endif



// Storage card power control pin
//#define DGPIO_CARD_POWER            (9 | GPIO_IS_DGPIO)

//#define DGPIO_KEY_TV_DET            (6 | GPIO_IS_DGPIO)
//#define PAD_KEY_TV_DET              PAD_PIN_TV_PLUGIN

//#define DGPIO_KEY_SHUTTER1        (7 | GPIO_IS_DGPIO)
//#define PAD_KEY_SHUTTER1          PAD_PIN_DGPIO7
//#define DGPIO_KEY_SHUTTER2        (8 | GPIO_IS_DGPIO)
//#define PAD_KEY_SHUTTER2          PAD_PIN_DGPIO8

// SYSRST
#define GPIO_SYSRST                 GPIO_KEY_NULL//(19 | GPIO_IS_DGPIO)

// LENS
#define GPIO_LENS_MOT_EXT2          C_GPIO_34

#endif

//@}
