/**
    PAD controller header

    PAD controller header

    @file       pad.h
    @ingroup    mIHALSysPAD
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _PAD_H
#define _PAD_H

#include "Type.h"

/** \addtogroup mIHALSysPAD */
//@{

/**
    Pad type select ID

    Pad type value for pad_setPullUpDown(), pad_getPullUpDown().
*/
//@{
typedef enum
{
    PAD_NONE         =       0x00,       ///< none of pull up/down
    PAD_PULLDOWN     =       0x01,       ///< 75K pull down
    PAD_PULLUP       =       0x02,       ///< 75K pull up
    PAD_KEEPER       =       0x03,       ///< 75K keeper

    ENUM_DUMMY4WORD(PAD_PULL)
}PAD_PULL;
//@}

/**
    Pad slew rate select ID

    Pad slew rate value for pad_setSlewRate(), pad_getSlewRate().
*/
//@{
typedef enum
{
    PAD_SLEWRATE_FAST   =    0x00,       ///< Pad slew rate fast
    PAD_SLEWRATE_SLOW   =    0x01,       ///< Pad slew rate slow

    ENUM_DUMMY4WORD(PAD_SLEWRATE)
}PAD_SLEWRATE;
//@}

/**
    Pad driving select ID

    Pad driving value for pad_setDrivingSink(), pad_getDrivingSink().
*/
//@{
typedef enum
{
    PAD_DRIVINGSINK_2MA  =   0x01,       ///< Pad driver/sink 2mA
    PAD_DRIVINGSINK_4MA  =   0x02,       ///< Pad driver/sink 4mA
    PAD_DRIVINGSINK_6MA  =   0x04,       ///< Pad driver/sink 6mA
    PAD_DRIVINGSINK_8MA  =   0x08,       ///< Pad driver/sink 8mA
    PAD_DRIVINGSINK_12MA =   0x10,       ///< Pad driver/sink 12mA
    PAD_DRIVINGSINK_16MA =   0x20,       ///< Pad driver/sink 16mA

    ENUM_DUMMY4WORD(PAD_DRIVINGSINK)
}PAD_DRIVINGSINK;
//@}


/**
    Pad pin ID.

    Pad ID of pad_setPullUpDown () and pad_getPullUpDown ()

    @name   Pad pin ID
*/
//@{
#define PAD_PIN_MC_BASE         0
#define PAD_PIN_MC0             (PAD_PIN_MC_BASE + 0)   ///< MC0/C_GPIO0
#define PAD_PIN_MC1             (PAD_PIN_MC_BASE + 2)   ///< MC1/C_GPIO1
#define PAD_PIN_MC2             (PAD_PIN_MC_BASE + 4)   ///< MC2/C_GPIO2
#define PAD_PIN_MC3             (PAD_PIN_MC_BASE + 6)   ///< MC3/C_GPIO3
#define PAD_PIN_MC4             (PAD_PIN_MC_BASE + 8)   ///< MC4/C_GPIO4
#define PAD_PIN_MC5             (PAD_PIN_MC_BASE + 10)  ///< MC5/C_GPIO5
#define PAD_PIN_MC6             (PAD_PIN_MC_BASE + 12)  ///< MC6/C_GPIO6
#define PAD_PIN_MC7             (PAD_PIN_MC_BASE + 14)  ///< MC7/C_GPIO7
#define PAD_PIN_MC8             (PAD_PIN_MC_BASE + 16)  ///< MC8/C_GPIO8
#define PAD_PIN_MC9             (PAD_PIN_MC_BASE + 18)  ///< MC9/C_GPIO9
#define PAD_PIN_MC10            (PAD_PIN_MC_BASE + 20)  ///< MC10/C_GPIO10
#define PAD_PIN_MC11            (PAD_PIN_MC_BASE + 22)  ///< MC11/C_GPIO11
#define PAD_PIN_MC12            (PAD_PIN_MC_BASE + 24)  ///< MC12/C_GPIO12
#define PAD_PIN_MC13            (PAD_PIN_MC_BASE + 26)  ///< MC13/C_GPIO13
#define PAD_PIN_MC14            (PAD_PIN_MC_BASE + 28)  ///< MC14/C_GPIO14
#define PAD_PIN_MC15            (PAD_PIN_MC_BASE + 30)  ///< MC15/C_GPIO15

#define PAD_PIN_MC16            (PAD_PIN_MC_BASE + 32)  ///< MC16/C_GPIO16
#define PAD_PIN_MC17            (PAD_PIN_MC_BASE + 34)  ///< MC17/C_GPIO17
#define PAD_PIN_MC18            (PAD_PIN_MC_BASE + 36)  ///< MC18/C_GPIO18
#define PAD_PIN_MC19            (PAD_PIN_MC_BASE + 38)  ///< MC19/C_GPIO19
#define PAD_PIN_MC20            (PAD_PIN_MC_BASE + 40)  ///< MC20/C_GPIO20
#define PAD_PIN_MC21            (PAD_PIN_MC_BASE + 42)  ///< MC21/C_GPIO21
#define PAD_PIN_MC22            (PAD_PIN_MC_BASE + 44)  ///< MC22/C_GPIO22
#define PAD_PIN_MC23            (PAD_PIN_MC_BASE + 46)  ///< MC23/C_GPIO23
#define PAD_PIN_MC24            (PAD_PIN_MC_BASE + 48)  ///< MC24/C_GPIO24
#define PAD_PIN_MC25            (PAD_PIN_MC_BASE + 50)  ///< MC25/C_GPIO25
#define PAD_PIN_MC26            (PAD_PIN_MC_BASE + 52)  ///< MC26/C_GPIO26
#define PAD_PIN_MC27            (PAD_PIN_MC_BASE + 54)  ///< MC27/C_GPIO27
#define PAD_PIN_MC28            (PAD_PIN_MC_BASE + 56)  ///< MC28/C_GPIO28
#define PAD_PIN_MC29            (PAD_PIN_MC_BASE + 58)  ///< MC29/C_GPIO29
#define PAD_PIN_MC30            (PAD_PIN_MC_BASE + 60)  ///< MC30/C_GPIO30
#define PAD_PIN_MC31            (PAD_PIN_MC_BASE + 62)  ///< MC31/C_GPIO31

#define PAD_PIN_MC32            (PAD_PIN_MC_BASE + 64)  ///< MC32/C_GPIO32
#define PAD_PIN_MC33            (PAD_PIN_MC_BASE + 66)  ///< MC33/C_GPIO33
#define PAD_PIN_MC34            (PAD_PIN_MC_BASE + 68)  ///< MC34/C_GPIO34
#define PAD_PIN_MC35            (PAD_PIN_MC_BASE + 70)  ///< MC35/C_GPIO35
#define PAD_PIN_MC36            (PAD_PIN_MC_BASE + 72)  ///< MC36/C_GPIO36
#define PAD_PIN_MC37            (PAD_PIN_MC_BASE + 74)  ///< MC37/C_GPIO37
#define PAD_PIN_MC38            (PAD_PIN_MC_BASE + 76)  ///< MC38/C_GPIO38
#define PAD_PIN_MC39            (PAD_PIN_MC_BASE + 78)  ///< MC39/C_GPIO39
#define PAD_PIN_MC40            (PAD_PIN_MC_BASE + 80)  ///< MC40/C_GPIO40
#define PAD_PIN_MC41            (PAD_PIN_MC_BASE + 82)  ///< MC41/C_GPIO41
#define PAD_PIN_MC42            (PAD_PIN_MC_BASE + 84)  ///< MC42/C_GPIO42
#define PAD_PIN_MC43            (PAD_PIN_MC_BASE + 86)  ///< MC43/C_GPIO43
#define PAD_PIN_MC44            (PAD_PIN_MC_BASE + 88)  ///< MC44/C_GPIO44
#define PAD_PIN_MC45            (PAD_PIN_MC_BASE + 90)  ///< MC45/C_GPIO45
#define PAD_PIN_MC46            (PAD_PIN_MC_BASE + 92)  ///< MC46/C_GPIO46
#define PAD_PIN_MC47            (PAD_PIN_MC_BASE + 94)  ///< MC47/C_GPIO47

#define PAD_PIN_MC48            (PAD_PIN_MC_BASE + 96)  ///< MC48/C_GPIO48
#define PAD_PIN_MC49            (PAD_PIN_MC_BASE + 98)  ///< MC49/C_GPIO49
#define PAD_PIN_MC50            (PAD_PIN_MC_BASE +100)  ///< MC50/C_GPIO50
#define PAD_PIN_MC51            (PAD_PIN_MC_BASE +102)  ///< MC51/C_GPIO51
#define PAD_PIN_MC52            (PAD_PIN_MC_BASE +104)  ///< MC52/C_GPIO52
#define PAD_PIN_MC53            (PAD_PIN_MC_BASE +106)  ///< MC53/C_GPIO53
#define PAD_PIN_MC54            (PAD_PIN_MC_BASE +108)  ///< MC54/C_GPIO54
#define PAD_PIN_MC55            (PAD_PIN_MC_BASE +110)  ///< MC55/C_GPIO55
#define PAD_PIN_MC56            (PAD_PIN_MC_BASE +112)  ///< MC56/C_GPIO56



#define PAD_PIN_SN_BASE         128
#define PAD_PIN_SN_PCLK         (PAD_PIN_SN_BASE + 0)      ///< SN_PCLK
#define PAD_PIN_SN_VD           (PAD_PIN_SN_BASE + 2)      ///< SN_VD
#define PAD_PIN_SN_HD           (PAD_PIN_SN_BASE + 4)      ///< SN_HD
#define PAD_PIN_SN_D0           (PAD_PIN_SN_BASE + 6)      ///< SN_D0
#define PAD_PIN_SN_D1           (PAD_PIN_SN_BASE + 8)      ///< SN_D1
#define PAD_PIN_SN_D2           (PAD_PIN_SN_BASE + 10)     ///< SN_D2
#define PAD_PIN_SN_D3           (PAD_PIN_SN_BASE + 12)     ///< SN_D3
#define PAD_PIN_SN_D4           (PAD_PIN_SN_BASE + 14)     ///< SN_D4
#define PAD_PIN_SN_D5           (PAD_PIN_SN_BASE + 16)     ///< SN_D5
#define PAD_PIN_SN_D6           (PAD_PIN_SN_BASE + 18)     ///< SN_D6
#define PAD_PIN_SN_D7           (PAD_PIN_SN_BASE + 20)     ///< SN_D7
#define PAD_PIN_SN_D8           (PAD_PIN_SN_BASE + 22)     ///< SN_D8
#define PAD_PIN_SN_D9           (PAD_PIN_SN_BASE + 24)     ///< SN_D9
#define PAD_PIN_SN_D10          (PAD_PIN_SN_BASE + 26)     ///< SN_D10
#define PAD_PIN_SN_D11          (PAD_PIN_SN_BASE + 28)     ///< SN_D11
#define PAD_PIN_SN_MCLK         (PAD_PIN_SN_BASE + 30)     ///< SN_MCLK

#define PAD_PIN_SP_CLK          (PAD_PIN_SN_BASE + 32)     ///< SP_CLK
#define PAD_PIN_SN_MES0         (PAD_PIN_SN_BASE + 34)     ///< SN_MES0
#define PAD_PIN_SN_MES1         (PAD_PIN_SN_BASE + 36)     ///< SN_MES1
#define PAD_PIN_SN_FLCTR        (PAD_PIN_SN_BASE + 38)     ///< SN_FLCTR
#define PAD_PIN_PWM0            (PAD_PIN_SN_BASE + 40)     ///< PWM0
#define PAD_PIN_PWM1            (PAD_PIN_SN_BASE + 42)     ///< PWM1
#define PAD_PIN_PWM2            (PAD_PIN_SN_BASE + 44)     ///< PWM2
#define PAD_PIN_PWM3            (PAD_PIN_SN_BASE + 46)     ///< PWM3
#define PAD_PIN_PWM4            (PAD_PIN_SN_BASE + 48)     ///< PWM4
#define PAD_PIN_PWM5            (PAD_PIN_SN_BASE + 50)     ///< PWM5
#define PAD_PIN_PWM6            (PAD_PIN_SN_BASE + 52)     ///< PWM6
#define PAD_PIN_PWM7            (PAD_PIN_SN_BASE + 54)     ///< PWM6



#define PAD_PIN_PERIPHERAL_BASE  192
#define PAD_PIN_I2CSDA           (PAD_PIN_PERIPHERAL_BASE + 0)  ///< I2CSDA
#define PAD_PIN_I2CSCL           (PAD_PIN_PERIPHERAL_BASE + 2)  ///< I2CSCL
#define PAD_PIN_ASBCLK           (PAD_PIN_PERIPHERAL_BASE + 4)  ///< ASBCLK
#define PAD_PIN_ASYNC            (PAD_PIN_PERIPHERAL_BASE + 6)  ///< ASYNC
#define PAD_PIN_ASDO             (PAD_PIN_PERIPHERAL_BASE + 8)  ///< ASDO
#define PAD_PIN_ASDI             (PAD_PIN_PERIPHERAL_BASE + 10) ///< ASDI
#define PAD_PIN_ASMCLK           (PAD_PIN_PERIPHERAL_BASE + 12) ///< ASMCLK
#define PAD_PIN_SBCS0            (PAD_PIN_PERIPHERAL_BASE + 14) ///< SBCS0
#define PAD_PIN_SBCK0            (PAD_PIN_PERIPHERAL_BASE + 16) ///< SBCK0
#define PAD_PIN_SBDAT0           (PAD_PIN_PERIPHERAL_BASE + 18) ///< SBDAT0
#define PAD_PIN_SBCS1            (PAD_PIN_PERIPHERAL_BASE + 20) ///< SBCS1
#define PAD_PIN_SBCK1            (PAD_PIN_PERIPHERAL_BASE + 22) ///< SBCK1
#define PAD_PIN_SBDAT1           (PAD_PIN_PERIPHERAL_BASE + 24) ///< SBDAT1
#define PAD_PIN_SBCS2            (PAD_PIN_PERIPHERAL_BASE + 26) ///< SBCS2
#define PAD_PIN_SBCS3            (PAD_PIN_PERIPHERAL_BASE + 28) ///< SBCS3
#define PAD_PIN_SBCK23           (PAD_PIN_PERIPHERAL_BASE + 30) ///< SBCK23

#define PAD_PIN_SBDAT23          (PAD_PIN_PERIPHERAL_BASE + 32) ///< SBDAT23
#define PAD_PIN_SBCS4            (PAD_PIN_PERIPHERAL_BASE + 34) ///< SBCS4
#define PAD_PIN_SBCS5            (PAD_PIN_PERIPHERAL_BASE + 36) ///< SBCS5
#define PAD_PIN_SBCK45           (PAD_PIN_PERIPHERAL_BASE + 38) ///< SBCK45
#define PAD_PIN_SBDAT45          (PAD_PIN_PERIPHERAL_BASE + 40) ///< SBDAT45
#define PAD_PIN_UART_TX          (PAD_PIN_PERIPHERAL_BASE + 42) ///< UART-TX
#define PAD_PIN_UART_RX          (PAD_PIN_PERIPHERAL_BASE + 44) ///< UART-RX
#define PAD_PIN_UART2_TX         (PAD_PIN_PERIPHERAL_BASE + 46) ///< UART2-TX
#define PAD_PIN_UART2_RX         (PAD_PIN_PERIPHERAL_BASE + 48) ///< UART2-RX
#define PAD_PIN_UART2_RTS        (PAD_PIN_PERIPHERAL_BASE + 50) ///< UART2-RTS
#define PAD_PIN_UART2_CTS        (PAD_PIN_PERIPHERAL_BASE + 52) ///< UART2-CTS
#define PAD_PIN_UART2_DTR        (PAD_PIN_PERIPHERAL_BASE + 54) ///< UART2-DTR
#define PAD_PIN_UART2_DSR        (PAD_PIN_PERIPHERAL_BASE + 56) ///< UART2-DSR
#define PAD_PIN_UART2_DCD        (PAD_PIN_PERIPHERAL_BASE + 58) ///< UART2-DCD
#define PAD_PIN_UART2_RI         (PAD_PIN_PERIPHERAL_BASE + 60) ///< UART2-RI
#define PAD_PIN_SPI_CS           (PAD_PIN_PERIPHERAL_BASE + 62) ///< SPI_CS

#define PAD_PIN_SPI_CLK          (PAD_PIN_PERIPHERAL_BASE + 64) ///< SPI_CLK
#define PAD_PIN_SPI_DO           (PAD_PIN_PERIPHERAL_BASE + 66) ///< SPI_DO
#define PAD_PIN_SPI_DI           (PAD_PIN_PERIPHERAL_BASE + 68) ///< SPI_DI
#define PAD_PIN_RM_RX            (PAD_PIN_PERIPHERAL_BASE + 70) ///< RM_RX
#define PAD_PIN_USB_ID           (PAD_PIN_PERIPHERAL_BASE + 72) ///< USB_ID
#define PAD_PIN_USB_DRVBUS       (PAD_PIN_PERIPHERAL_BASE + 74) ///< USB_DRVBUS




#define PAD_PIN_LCD_BASE        288
#define PAD_PIN_LCD0            (PAD_PIN_LCD_BASE + 0)      ///< LCD0
#define PAD_PIN_LCD1            (PAD_PIN_LCD_BASE + 2)      ///< LCD1
#define PAD_PIN_LCD2            (PAD_PIN_LCD_BASE + 4)      ///< LCD2
#define PAD_PIN_LCD3            (PAD_PIN_LCD_BASE + 6)      ///< LCD3
#define PAD_PIN_LCD4            (PAD_PIN_LCD_BASE + 8)      ///< LCD4
#define PAD_PIN_LCD5            (PAD_PIN_LCD_BASE + 10)     ///< LCD5
#define PAD_PIN_LCD6            (PAD_PIN_LCD_BASE + 12)     ///< LCD6
#define PAD_PIN_LCD7            (PAD_PIN_LCD_BASE + 14)     ///< LCD7
#define PAD_PIN_LCD8            (PAD_PIN_LCD_BASE + 16)     ///< LCD8
#define PAD_PIN_LCD9            (PAD_PIN_LCD_BASE + 18)     ///< LCD9
#define PAD_PIN_LCD10           (PAD_PIN_LCD_BASE + 20)     ///< LCD10
#define PAD_PIN_LCD11           (PAD_PIN_LCD_BASE + 22)     ///< LCD11
#define PAD_PIN_LCD12           (PAD_PIN_LCD_BASE + 24)     ///< LCD12
#define PAD_PIN_LCD13           (PAD_PIN_LCD_BASE + 26)     ///< LCD13
#define PAD_PIN_LCD14           (PAD_PIN_LCD_BASE + 28)     ///< LCD14
#define PAD_PIN_LCD15           (PAD_PIN_LCD_BASE + 30)     ///< LCD15

#define PAD_PIN_LCD16           (PAD_PIN_LCD_BASE + 32)     ///< LCD16
#define PAD_PIN_LCD17           (PAD_PIN_LCD_BASE + 34)     ///< LCD17
#define PAD_PIN_LCD18           (PAD_PIN_LCD_BASE + 36)     ///< LCD18
#define PAD_PIN_LCD19           (PAD_PIN_LCD_BASE + 38)     ///< LCD19
#define PAD_PIN_LCD20           (PAD_PIN_LCD_BASE + 40)     ///< LCD20
#define PAD_PIN_LCD21           (PAD_PIN_LCD_BASE + 42)     ///< LCD21
#define PAD_PIN_LCD22           (PAD_PIN_LCD_BASE + 44)     ///< LCD22
#define PAD_PIN_LCD23           (PAD_PIN_LCD_BASE + 46)     ///< LCD23
#define PAD_PIN_LCD24           (PAD_PIN_LCD_BASE + 48)     ///< LCD24
#define PAD_PIN_LCD25           (PAD_PIN_LCD_BASE + 50)     ///< LCD25
#define PAD_PIN_LCD26           (PAD_PIN_LCD_BASE + 52)     ///< LCD26
#define PAD_PIN_LCD27           (PAD_PIN_LCD_BASE + 54)     ///< LCD27
#define PAD_PIN_HDMI_INT        (PAD_PIN_LCD_BASE + 56)     ///< HDMI_INT




#define PAD_PIN_SYSTEM_BASE     352
#define PAD_PIN_RESET           (PAD_PIN_SYSTEM_BASE + 0)   ///< RESET#
#define PAD_PIN_TESTEN          (PAD_PIN_SYSTEM_BASE + 2)   ///< TEST
#define PAD_PIN_IDI             (PAD_PIN_SYSTEM_BASE + 4)   ///< TCK
#define PAD_PIN_GOICE           (PAD_PIN_SYSTEM_BASE + 6)   ///< TMS
#define PAD_PIN_IMS             (PAD_PIN_SYSTEM_BASE + 8)   ///< TDI
#define PAD_PIN_ICK             (PAD_PIN_SYSTEM_BASE + 10)  ///< TDO
#define PAD_PIN_CEC             (PAD_PIN_SYSTEM_BASE + 20)  ///< CEC
#define PAD_PIN_DDCSDA          (PAD_PIN_SYSTEM_BASE + 22)  ///< DDC_SDA
#define PAD_PIN_DDCSCL          (PAD_PIN_SYSTEM_BASE + 24)  ///< DDC_SCL
#define PAD_PIN_HOTPLUG         (PAD_PIN_SYSTEM_BASE + 26)  ///< HOTPLUG


#define PAD_PIN_DGPIO_BASE      384
#define PAD_PIN_DGPIO0          (PAD_PIN_DGPIO_BASE + 0)     ///< DGPIO0
#define PAD_PIN_DGPIO1          (PAD_PIN_DGPIO_BASE + 2)     ///< DGPIO1
#define PAD_PIN_DGPIO2          (PAD_PIN_DGPIO_BASE + 4)     ///< DGPIO2
#define PAD_PIN_DGPIO3          (PAD_PIN_DGPIO_BASE + 6)     ///< DGPIO3
#define PAD_PIN_DGPIO4          (PAD_PIN_DGPIO_BASE + 8)     ///< DGPIO4
#define PAD_PIN_DGPIO5          (PAD_PIN_DGPIO_BASE + 10)    ///< DGPIO5
#define PAD_PIN_DGPIO6          (PAD_PIN_DGPIO_BASE + 12)    ///< DGPIO6
#define PAD_PIN_MAX             (PAD_PIN_DGPIO_BASE + 14)


//DGPIO alias group begin, for backward compatible
#define PAD_PIN_SN_FIELD         PAD_PIN_DGPIO0         ///< DGPIO0/SN_FIELD
#define PAD_PIN_SN_C0            PAD_PIN_DGPIO1         ///< DGPIO1/SN_C0
#define PAD_PIN_SN_C1            PAD_PIN_DGPIO2         ///< DGPIO2/SN_C1
#define PAD_PIN_SN_C2            PAD_PIN_DGPIO3         ///< DGPIO3/SN_C2
#define PAD_PIN_SN_C3            PAD_PIN_DGPIO4         ///< DGPIO4/SN_C3
#define PAD_PIN_SN_PICNT         PAD_PIN_DGPIO5         ///< DGPIO5/SN_PICNT
#define PAD_PIN_SI_INT           PAD_PIN_DGPIO5         ///< DGPIO5/SI_INT
#define PAD_PIN_TV_PLUGIN        PAD_PIN_DGPIO5         ///< DGPIO5/TV_PLUGIN
#define PAD_PIN_CD               PAD_PIN_DGPIO6         ///< DGPIO6/Card Detect
//DGPIO alias group end

//@}

/**
    Pad slew rate pin ID.

    Pad ID of pad_setSlewRate() and pad_getSlewRate().
*/
//@{
typedef enum
{
    PAD_SR_CARD      =      0,    ///< Slew Rate ID of Storage interface.
    PAD_SR_SENSOR    =      1,    ///< Slew Rate ID of Sensor interface.
    PAD_SR_PWM       =      2,    ///< Slew Rate ID of PWM interface.
    PAD_SR_DGPIO     =      3,    ///< Slew Rate ID of DGPIO interface.
    PAD_SR_I2S_AC97  =      4,    ///< Slew Rate ID of I2S/AC97 interface.
    PAD_SR_LCD       =      5,    ///< Slew Rate ID of LCD interface.
    PAD_SR_OTHERS    =      6,    ///< Slew Rate ID of Others.

    ENUM_DUMMY4WORD(PAD_SR)
}PAD_SR;
//@}

/**
    Pad driving pin ID.

    Pad ID of pad_setDrivingSink() and pad_getDrivingSink()
*/
//@{
typedef enum
{
    PAD_DS_CARD       =      0,       ///< Driving/Sink ID of Storage interface.
    PAD_DS_MC10       =      16,      ///< Driving/Sink ID of MC10.
    PAD_DS_MC46       =      18,      ///< Driving/Sink ID of MC46.
    PAD_DS_MC47       =      20,      ///< Driving/Sink ID of MC47.

    PAD_DS_SENSOR     =     2,        ///< Driving/Sink ID of Sensor interface.
    PAD_DS_SN_MCLK    =     26,       ///< Driving/Sink ID of Sensor MCLK.
    PAD_DS_SN_SPCLK   =     22,       ///< Driving/Sink ID of Special clock.
    PAD_DS_FLCTR      =     28,       ///< Driving/Sink ID of flash control.

    PAD_DS_PWM        =     4,        ///< Driving/Sink ID of PWM interface.

    PAD_DS_DGPIO      =     6,        ///< Driving/Sink ID of DGPIO.

    PAD_DS_I2S_AC97   =     8,        ///< Driving/Sink ID of I2S/AC97.

    PAD_DS_LCD        =     10,       ///< Driving/Sink ID of LCD interface.
    PAD_DS_LCD8       =     30,       ///< Driving/Sink ID of pin LCD8.

    PAD_DS_OTHERS     =     12,       ///< Driving/Sink ID of Others.
    PAD_DS_SPICLK     =     24,       ///< Driving/Sink ID of SPI clock.
    PAD_DS_CEC        =    (32+0),    ///< Driving/Sink ID of CEC.
    PAD_DS_DDC        =    (32+2),    ///< Driving/Sink ID of DDC interface.

    ENUM_DUMMY4WORD(PAD_DS)
}PAD_DS;
//@}
#define PAD_DS_MAX            (PAD_DS_DDC + 2)


extern ER pad_setPullUpDown(UINT pin, PAD_PULL pulltype);
extern ER pad_getPullUpDown(UINT pin, PAD_PULL *pulltype);
extern ER pad_setSlewRate(PAD_SR name, PAD_SLEWRATE rate);
extern ER pad_getSlewRate(PAD_SR name, PAD_SLEWRATE *rate);
extern ER pad_setDrivingSink(PAD_DS name, PAD_DRIVINGSINK driving);
extern ER pad_getDrivingSink(PAD_DS name, PAD_DRIVINGSINK *driving);

//@}

#endif
