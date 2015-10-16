/**
    TOP controller header

    Sets the pinmux of each module.

    @file       top.h
    @ingroup    mIHALSysTOP
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _TOP_H
#define _TOP_H

#include "Type.h"

/**
    @addtogroup mIHALSysTOP
*/
//@{

/**
    Definition for pinmux gpio

    The pinout of each module can be selected as GPIO or by module control.
    Use this definition to select pins as GPIO usage.
\n  This definition can be used such as pinmux_select_mclk()/ pinmux_select_flctr()/ pinmux_select_i2c() ... etc.

    @name   Pinmux gpio
*/
//@{
#define PINMUX_GPIO             0x00000000  ///< Pins as GPIO
//@}

//
// TOP Control Register 0
//

/**
    Boot source select ID

    Boot source value for pinmux_get_bootsource().

    @name   Boot source select ID
*/
//@{
#define PINMUX_BOOT_NAND_HAM_ECC    0x0000  ///< Nand with Hamming ECC.
#define PINMUX_BOOT_XD_ECC_SOCKET   0x0001  ///< xD with Hamming ECC and XD_SOCKET enable.
#define PINMUX_BOOT_NAND            0x0002  ///< Nand without ECC.
#define PINMUX_BOOT_USB_AUTO        0x0003  ///< USB auto speed selection.
#define PINMUX_BOOT_SDIO            0x0004  ///< SDIO and SD_PINMODE disable.
#define PINMUX_BOOT_CF              0x0005  ///< CF/ATA True IDE mode.
#define PINMUX_BOOT_MS              0x0006  ///< MS.(Support MS-Pro/MS-HG)
#define PINMUX_BOOT_NOR             0x0007  ///< NOR flash.
#define PINMUX_BOOT_NAND_RS_ECC     0x0008  ///< Nand with RS ECC.
#define PINMUX_BOOT_SDIO2           0x0009  ///< SDIO2
#define PINMUX_BOOT_XD_SOCKET       0x000A  ///< xD with Hamming ECC and XD_SOCKET enable.
#define PINMUX_BOOT_SPI             0x000B  ///< Serial flash.(SPI)
#define PINMUX_BOOT_USB_FS          0x000C  ///< USB force full speed.
#define PINMUX_BOOT_SDIO_PINMODE    0x000D  ///< SDIO and SD_PINMODE enable.
#define PINMUX_BOOT_XD_ECC          0x000E  ///< xD with Hamming ECC and XD_SOCKET disable.
#define PINMUX_BOOT_XD              0x000F  ///< xD without Hamming ECC and XD_SOCKET disable.
//@}
#define PINMUX_BOOT_BITMASK         0x000F
#define PINMUX_BOOT_SHIFT           0
#define PINMUX_BOOT_SHIFT2          16

//
// TOP Control Register 1
//

/**
    Storage interface Active select ID

    The storage pinmux control is seperating into two stage: EXIST & ACTIVE states.
    When the storage "EXIST" state is set, only some of dedicated pins is controlled by storage engine.
    The storage engine can only control the complete bus pins when EXIST & ACTIVE are set simultaneously.
    This design allows several storages could exist at a system and work simultaneously.
\n  Note that if only the ACTIVE is set without EXIST asserted, storage engine could not control any of the bus pins.
\n  Set ACTIVE/EXIST state from pinmux_active_storage()/ pinmux_deactive_storage()/ pinmux_setexist_storage()/ pinmux_unsetexist_storage().
*/
//@{
typedef enum
{
    PINMUX_ACT_NAND = (0x01 << 0),  ///< Activate Nand.
    PINMUX_ACT_PROM = (0x01 << 1),  ///< Activate PROM.
    PINMUX_ACT_SROM = (0x01 << 2),  ///< Activate Serial ROM.
    PINMUX_ACT_ATA  = (0x01 << 3),  ///< Activate ATA.
    PINMUX_ACT_SDIO2= (0x01 << 4),  ///< Activate SDIO2.
    PINMUX_ACT_SDIO = (0x01 << 5),  ///< Activate SDIO.
    PINMUX_ACT_XD   = (0x01 << 6),  ///< Activate xD.
    PINMUX_ACT_MS   = (0x01 << 7),  ///< Activate MS.
    PINMUX_ACT_CF   = (0x01 << 8),  ///< Activate CF.
    PINMUX_ACT_EROM = (0x01 << 9),  ///< Activate embedded ROM.

    ENUM_DUMMY4WORD(PINMUX_ACTID)
}PINMUX_ACTID;
//@}

/**
    Storage interface Exist select ID

    The storage pinmux control is seperating into two stage: EXIST & ACTIVE states.
    When the storage "EXIST" state is set, only some of dedicated pins is controlled by storage engine.
    The storage engine can only control the complete bus pins when EXIST & ACTIVE are set simultaneously.
    This design allows several storages could exist at a system and work simultaneously.
\n  Note that if only the ACTIVE is set without EXIST asserted, storage engine could not control any of the bus pins.
\n  Set ACTIVE/EXIST state from pinmux_active_storage()/ pinmux_deactive_storage()/ pinmux_setexist_storage()/ pinmux_unsetexist_storage().
*/
//@{
typedef enum
{
    PINMUX_EXIST_NAND  =       (0x01 << 12), ///< Nand Exist.      Dedicated pins: CS0/CS1/NAND_WP
    PINMUX_EXIST_PROM  =       (0x01 << 13), ///< PROM Exist.      Dedicated pins: CS0/CS1
    PINMUX_EXIST_SROM  =       (0x01 << 14), ///< Serial ROM Exist.Dedicated pins: S_CS
    PINMUX_EXIST_ATA   =       (0x01 << 15), ///< ATA Exist.       Dedicated pins: RESET/IOSC16/DMARQ/CS0/CS1/INTRQ/IORDY
    PINMUX_EXIST_SDIO2 =       (0x01 << 16), ///< SDIO2 Exist.     Dedicated pins: SDIO2_CLK/SDIO2_CMD
    PINMUX_EXIST_SDIO  =       (0x01 << 17), ///< SDIO Exist.      Dedicated pins: SDIO_CLK/SDIO_CMD
    PINMUX_EXIST_XD    =       (0x01 << 18), ///< xD Exist.        Dedicated pins: XD_CS/XD_WP
    PINMUX_EXIST_MS    =       (0x01 << 19), ///< MS Exist.        Dedicated pins: MS_D0/MS_BS/MS_CLK
    PINMUX_EXIST_CF    =       (0x01 << 20), ///< CF Exist.        Dedicated pins: RESET/IOIS16/INPACK/CE1/CE2/RDY/WAIT

    ENUM_DUMMY4WORD(PINMUX_EXISTID)
}PINMUX_EXISTID;
//@}
#define PINMUX_ACT_SHIFT        0
#define PINMUX_ACT_MASK         0x3FF
#define PINMUX_EXIST_SHIFT      12
#define PINMUX_EXIST_MASK       0x1FF


/**
    Card type ID

    Get current storage ACTIVE/EXIST state status by using pinmux_getactive_storage() & pinmux_getexist_storage().
*/
//@{
typedef enum
{
    GPIO_CARDTYPE_NONE   =       0x000,   ///< None of storage.
    GPIO_CARDTYPE_NAND   =       0x001,   ///< Nand.
    GPIO_CARDTYPE_PROM   =       0x002,   ///< PROM.
    GPIO_CARDTYPE_SROM   =       0x004,   ///< Serial ROM.
    GPIO_CARDTYPE_ATA    =       0x008,   ///< ATA.
    GPIO_CARDTYPE_SDIO2  =       0x010,   ///< SDIO2.
    GPIO_CARDTYPE_SDIO   =       0x020,   ///< SDIO.
    GPIO_CARDTYPE_XD     =       0x040,   ///< XD.
    GPIO_CARDTYPE_MS     =       0x080,   ///< MS.
    GPIO_CARDTYPR_CF     =       0x100,   ///< CF.

    ENUM_DUMMY4WORD(PINMUX_CARDTYPE)
}PINMUX_CARDTYPE;
//@}

//
// TOP Control Register 2
//

#define PINMUX_PROMSIZE_SHIFT       0
#define PINMUX_PROMCS_SHIFT         4
#define PINMUX_PROMDATA_SHIFT       6
#define PINMUX_PROM_PINMODE_SHIFT   7
#define PINMUX_PROM_IOWAIT_SHIFT    8
#define PINMUX_MMCDATA_SHIFT        12
#define PINMUX_NANDCS_SHIFT         13
#define PINMUX_MSDATA_SHIFT         14
#define PINMUX_LCD_SHIFT            20
#define PINMUX_PLCD_SHIFT           24
#define PINMUX_HDMIIF_SHIFT         26
#define PINMUX_LCD_DE_SHIFT         27
#define PINMUX_LCD_CCIR_SHIFT       28


/**
    Parallel ROM address bits selection

    Parallel ROM address bits can be configured as different size when different size of PROM is used.
    Address bits can be selected as 18/19/20/21/22/4/8/1 bits.
    Use these definitions in pinmux_select_promsize()/ pinmux_get_promsize() to select/get the configuration.
*/
//@{
typedef enum
{
    PINMUX_PROMSIZE_256KB  = (0x00 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A17.
    PINMUX_PROMSIZE_512KB  = (0x01 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A18.
    PINMUX_PROMSIZE_1024KB = (0x02 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A19.
    PINMUX_PROMSIZE_2048KB = (0x03 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A20.
    PINMUX_PROMSIZE_4096KB = (0x04 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A21.
    PINMUX_PROMSIZE_16B    = (0x05 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A3.
    PINMUX_PROMSIZE_256B   = (0x06 << PINMUX_PROMSIZE_SHIFT), ///< A0 ~ A7.
    PINMUX_PROMSIZE_1B     = (0x07 << PINMUX_PROMSIZE_SHIFT), ///< One of A0/ A1/ A2/ A3/ A11/ A12.

    ENUM_DUMMY4WORD(PINMUX_PROMSIZE)
}PINMUX_PROMSIZE;
//@}
#define PINMUX_PROMSIZE_BITMASK (0x07 << PINMUX_PROMSIZE_SHIFT)

/**
    Parallel ROM CS select

    PROM chip select pin can be selected as one of CS0/CS1 or both.
    Use for pinmux_select_promcs() & pinmux_get_promcs().
*/
//@{
typedef enum
{
    PINMUX_PROMCS_CS0   =    (0x00 << PINMUX_PROMCS_SHIFT), ///< Only CS0 as PROM CS.
    PINMUX_PROMCS_CS1   =    (0x01 << PINMUX_PROMCS_SHIFT), ///< Only CS1 as PROM CS.
    PINMUX_PROMCS_CS0_1 =    (0x02 << PINMUX_PROMCS_SHIFT), ///< CS0+CS1 as PROM CS.

    ENUM_DUMMY4WORD(PINMUX_PROMCS)
}PINMUX_PROMCS;
//@}
#define PINMUX_PROMCS_BITMASK   (0x03 << PINMUX_PROMCS_SHIFT)

/**
    Parallel ROM bus width selection

    Parallel ROM bus width can be selected as 8 or 16 bits.
    Use for pinmux_select_promwidth()/ pinmux_get_promwidth().
*/
//@{
typedef enum
{
    PINMUX_PROMDATA_8BIT  =  (0x00 << PINMUX_PROMDATA_SHIFT), ///< PROM 8 bits bus.
    PINMUX_PROMDATA_16BIT =  (0x01 << PINMUX_PROMDATA_SHIFT), ///< PROM 16 bits bus.

    ENUM_DUMMY4WORD(PINMUX_PROMDATA)
}PINMUX_PROMDATA;
//@}
#define PINMUX_PROMDATA_BITMASK (0x01 << PINMUX_PROMDATA_SHIFT)

/**
    Parallel ROM pinout interface selection

    Select parallel ROM pinout to storage or LCD interface.
    Use for pinmux_select_prom_pinmode() / pinmux_get_prom_pinmode().
*/
//@{
typedef enum
{
    PINMUX_PROM_STORAGE   =       (0x00 << PINMUX_PROM_PINMODE_SHIFT), ///< PROM storage interface.
    PINMUX_PROM_LCD       =       (0x01 << PINMUX_PROM_PINMODE_SHIFT), ///< PROM LCD interface.

    ENUM_DUMMY4WORD(PINMUX_PROM)
}PINMUX_PROM;
//@}
#define PINMUX_PROM_PINMODE_BITMASK  (0x01 << PINMUX_PROM_PINMODE_SHIFT)

/**
    Parallel ROM iowait selection

    Enable Parallel ROM iowait pin.
    Use for pinmux_enanble_iowait()/ pinmux_disable_iowait()/ pinmux_get_iowait().

    @name   Parallel ROM iowait selection
*/
//@{
#define PINMUX_PROM_IOWAIT           (0x01 << PINMUX_PROM_IOWAIT_SHIFT) ///< PROM iowait
//@}

/**
    MMC data bus width

    Select MMC data bus 4/8 bits.
    Use for pinmux_select_mmcwidth()/ pinmux_get_mmcwidth().
*/
//@{
typedef enum
{
    PINMUX_MMCDATA_4BIT   =  (0x00 << PINMUX_MMCDATA_SHIFT),  ///< MMC 4 bits mode.
    PINMUX_MMCDATA_8BIT   =  (0x01 << PINMUX_MMCDATA_SHIFT),  ///< MMC 8 bits mode.

    ENUM_DUMMY4WORD(PINMUX_MMCDATA)
}PINMUX_MMCDATA;
//@}
#define PINMUX_MMCDATA_BITMASK  (0x01 << PINMUX_MMCDATA_SHIFT)

/**
    Nand CS selection

    Select One/Two CS pins for Nand.
    Use for pinmux_select_nandcs()/ pinmux_get_nandcs().
*/
//@{
typedef enum
{
    PINMUX_NANDCS_1    =     (0x00 << PINMUX_NANDCS_SHIFT), ///< CS0 as Nand CS pin.
    PINMUX_NANDCS_2    =     (0x01 << PINMUX_NANDCS_SHIFT), ///< CS0+CS1 as Nand CS pins.

    ENUM_DUMMY4WORD(PINMUX_NANDCS)
}PINMUX_NANDCS;
//@}
#define PINMUX_NANDCS_BITMASK   (0x01 << PINMUX_NANDCS_SHIFT)

/**
    MS data bus width selection

    Select MS data bus 1/4/8 bits mode.
    Use for pinmux_select_mswidth()/ pinmux_get_mswidth().
*/
//@{
typedef enum
{
    PINMUX_MSDATA_SERIAL =   (0x00 << PINMUX_MSDATA_SHIFT), ///< MS one bit mode.
    PINMUX_MSDATA_4BIT   =   (0x01 << PINMUX_MSDATA_SHIFT), ///< MS 4 bits mode.
    PINMUX_MSDATA_8BIT   =   (0x02 << PINMUX_MSDATA_SHIFT), ///< MS 8 bits mode.

    ENUM_DUMMY4WORD(PINMUX_MSDATA)
}PINMUX_MSDATA;
//@}
#define PINMUX_MSDATA_BITMASK   (0x03 << PINMUX_MSDATA_SHIFT)

/**
    LCD interface type selection.

    Select LCD interface type.
    Use for pinmux_select_lcd()/ pinmux_get_lcd().
*/
//@{
typedef enum
{
    PINMUX_LCD_GPIO            = PINMUX_GPIO,                 ///< LCD as GPIO.
    PINMUX_LCD_CCIR656         = (0x01 << PINMUX_LCD_SHIFT),  ///< LCD as CCIR656.
    PINMUX_LCD_CCIR601         = (0x02 << PINMUX_LCD_SHIFT),  ///< LCD as CCIR601.
    PINMUX_LCD_PARALLEL        = (0x03 << PINMUX_LCD_SHIFT),  ///< LCD as parallel interface.
    PINMUX_LCD_SERIAL_RGB_8BIT = (0x04 << PINMUX_LCD_SHIFT),  ///< LCD as SRGB-8bit.
    PINMUX_LCD_SERIAL_RGB_6BIT = (0x05 << PINMUX_LCD_SHIFT),  ///< LCD as SRGB-6bit.
    PINMUX_LCD_SERL_YCBCR_8BIT = (0x06 << PINMUX_LCD_SHIFT),  ///< LCD as SYCbCr-8bit.
    PINMUX_LCD_HDMI_IF         = (0x07 << PINMUX_LCD_SHIFT),  ///< LCD as HDMI interface.

    ENUM_DUMMY4WORD(PINMUX_LCD)
}PINMUX_LCD;
//@}
#define PINMUX_LCD_BITMASK          (0x07 << PINMUX_LCD_SHIFT)

/**
    Parallel LCD data width selection.

    When LCD parallel interface is selected, the parallel interface can be configurable
    as RGB888/RGB666 by using pinmux_select_plcdwidth()/ pinmux_get_plcdwidth().
*/
//@{
typedef enum
{
    PINMUX_PLCD_RGB888   =   (0x00 << PINMUX_PLCD_SHIFT), ///< RGB-888.
    PINMUX_PLCD_RGB666   =   (0x01 << PINMUX_PLCD_SHIFT), ///< RGB-666.

    ENUM_DUMMY4WORD(PINMUX_PLCD)
}PINMUX_PLCD;
//@}
#define PINMUX_PLCD_BITMASK     (0x03 << PINMUX_PLCD_SHIFT)

/**
    Select HDMI interface data width

    When LCD HDMI interface is selected, the HDMI interface can be configurable as 24/16 bits bus
    by using pinmux_select_hdmiifwidth()/ pinmux_get_hdmiifwidth().
*/
//@{
typedef enum
{
    PINMUX_HDMIIF_24BIT  =   (0x00 << PINMUX_HDMIIF_SHIFT), ///< HDMI 24 bits bus.
    PINMUX_HDMIIF_16BIT  =   (0x01 << PINMUX_HDMIIF_SHIFT), ///< HDMI 16 bits bus.

    ENUM_DUMMY4WORD(PINMUX_HDMIIF)
}PINMUX_HDMIIF;
//@}
#define PINMUX_HDMIIF_BITMASK   (0x01 << PINMUX_HDMIIF_SHIFT)

/**
    LCD DE enable.

    Using by pinmux_get_de().

    @name   LCD DE enable
*/
//@{
#define PINMUX_LCD_DE           (0x01 << PINMUX_LCD_DE_SHIFT) ///< LCD DE pins.
//@}
#define PINMUX_LCD_DE_BITMASK   (0x01 << PINMUX_LCD_DE_SHIFT)

/**
    LCD CCIR data width selection.

    When LCD CCIR601/656 mode is selected, the data bus can be configurable as 8/16 bits
    by using pinmux_select_lcdccirwidth()/ pinmux_get_lcdccirwidth().
*/
//@{
typedef enum
{
    PINMUX_LCD_CCIR_DATA_8BIT  =  (0x00 << PINMUX_LCD_CCIR_SHIFT), ///< LCD CCIR bus 8 bits.
    PINMUX_LCD_CCIR_DATA_16BIT =  (0x01 << PINMUX_LCD_CCIR_SHIFT), ///< LCD CCIR bus 16 bits.

    ENUM_DUMMY4WORD(PINMUX_LCD_CCIR_DATA)
}PINMUX_LCD_CCIR_DATA;
//@}
#define PINMUX_LCD_CCIR_DATA_BITMASK (0x01 << PINMUX_LCD_CCIR_SHIFT)

//
// TOP Control Register 3
//

#define PINMUX_SENSOR_SHIFT             0
#define PINMUX_SENSOR_CCIR_SHIFT        8
#define PINMUX_SEN_CCIR_WIDTH_SHIFT     9
#define PINMUX_PWM_SHIFT                16

/**
    Sensor bus width selection.

    Sensor bus width can be selected as 12/10/8 bits.
    Use for pinmux_select_sensor()/ pinmux_get_sensor().
*/
//@{
typedef enum
{
    PINMUX_SENSOR_12BIT  =   (0x00 << PINMUX_SENSOR_SHIFT), ///< Sensor 12 bits mode.
    PINMUX_SENSOR_10BIT  =   (0x01 << PINMUX_SENSOR_SHIFT), ///< Sensor 10 bits mode.
    PINMUX_SENSOR_8BIT   =   (0x02 << PINMUX_SENSOR_SHIFT), ///< Sensor 8 bits mode.

    ENUM_DUMMY4WORD(PINMUX_SENSOR)
}PINMUX_SENSOR;
//@}
#define PINMUX_SENSOR_BITMASK   (0x03 << PINMUX_SENSOR_SHIFT)

/**
    Pinmux of sensor MCLK

    Use for pinmux_select_mclk()/ pinmux_get_mclk().

    @name   Pinmux of sensor MCLK
*/
//@{
#define PINMUX_MCLK             (0x01 << 3) ///< Pinmux of sensor mclk.
//@}

/**
    Pinmux of mechanical shutter 0/1.

    Use for pinmux_enable_mes()/ pinmux_disable_mes()/ pinmux_get_mes().
*/
//@{
typedef enum
{
    PINMUX_MES0     =        (0x01 << 4), ///< MES0.
    PINMUX_MES1     =        (0x01 << 5), ///< MES1.

    ENUM_DUMMY4WORD(PINMUX_MES)
}PINMUX_MES;
//@}

/**
    Pinmux of flash control

    Use for pinmux_select_flctr()/ pinmux_get_flctr().

    @name   Pinmux of flash control
*/
//@{
#define PINMUX_FLCTR            (0x01 << 6) ///< FLCTR.
//@}

/**
    Pinmux of specail clock

    Use for pinmux_select_spclk()/ pinmux_get_spclk().

    @name   Pinmux of specail clock
*/
//@{
#define PINMUX_SP_CLK           (0x01 << 7) ///< Special clock.
//@}

/**
    Pinmux of sensor mode.

    Select sensor interface as SENSOR or CCIR mode.
    Use for pinmux_select_senccir()/ pinmux_get_senccir().

    @name   Pinmux of sensor mode
*/
//@{
#define PINMUX_SENSOR_SEN          (0x00 << PINMUX_SENSOR_CCIR_SHIFT) ///< Sensor mode.
#define PINMUX_SENSOR_CCIR         (0x01 << PINMUX_SENSOR_CCIR_SHIFT) ///< CCIR mode.
//@}
#define PINMUX_SENSOR_CCIR_BITMASK (0x01 << PINMUX_SENSOR_CCIR_SHIFT)

/**
    Select sensor CCIR data width

    Select sensor ccir data bus width as 8/16 bits.
    Use for pinmux_select_senccirwidth()/ pinmux_get_senccirwidth().
*/
//@{
typedef enum
{
    PINMUX_SEN_CCIR_8BIT     =       (0x00 << PINMUX_SEN_CCIR_WIDTH_SHIFT), ///< 8 bits mode.
    PINMUX_SEN_CCIR_16BIT    =       (0x01 << PINMUX_SEN_CCIR_WIDTH_SHIFT), ///< 16 bits mode.

    ENUM_DUMMY4WORD(PINMUX_SEN_CCIR)
}PINMUX_SEN_CCIR;
//@}
#define PINMUX_SEN_CCIR_WIDTH_BITMASK   (0x01 << PINMUX_SEN_CCIR_WIDTH_SHIFT)

/**
    Pinmux of PWM interface

    Use for pinmux_enable_pwm()/ pinmux_disable_pwm()/ pinmux_get_pwm().
*/
//@{
typedef enum
{
    PINMUX_PWM0     =        (0x01 << (PINMUX_PWM_SHIFT + 0)),  ///< PWM0.
    PINMUX_PWM1     =        (0x01 << (PINMUX_PWM_SHIFT + 1)),  ///< PWM1.
    PINMUX_PWM2     =        (0x01 << (PINMUX_PWM_SHIFT + 2)),  ///< PWM2.
    PINMUX_PWM3     =        (0x01 << (PINMUX_PWM_SHIFT + 3)),  ///< PWM3.
    PINMUX_PWM4     =        (0x01 << (PINMUX_PWM_SHIFT + 4)),  ///< PWM4.
    PINMUX_PWM5     =        (0x01 << (PINMUX_PWM_SHIFT + 5)),  ///< PWM5.
    PINMUX_PWM6     =        (0x01 << (PINMUX_PWM_SHIFT + 6)),  ///< PWM6.
    PINMUX_PWM7     =        (0x01 << (PINMUX_PWM_SHIFT + 7)),  ///< PWM7.

    ENUM_DUMMY4WORD(PINMUX_PWM)
}PINMUX_PWM;
//@}
#define PINMUX_BITMASK          (0xFF << PINMUX_PWM_SHIFT)

//
// TOP Control Register 4
//

#define PINMUX_AUDIO_SHIFT      2
#define PINMUX_SIF_SHFIT        4
#define PINMUX_UART2_SHIFT      17

/**
    Pinmux of I2C interface.

    Use for pinmux_select_i2c()/ pinmux_get_i2c().

    @name   Pinmux of I2C interface
*/
//@{
#define PINMUX_I2C              (0x01 << 0) ///< Pinmux of I2C.
//@}

/**
    Pinmux of audio mclk.

    Use for pinmux_select_audmclk()/ pinmux_get_audmclk().

    @name   Pinmux of audio mclk
*/
//@{
#define PINMUX_AUDIO_MCLK       (0x01 << 1) ///< Pinmux of audio mclk.
//@}

/**
    Pinmux of audio interface.

    Use for pinmux_select_audio()/ pinmux_get_audio().
*/
//@{
typedef enum
{
    PINMUX_ADUIO_GPIO   =    PINMUX_GPIO,                     ///< Audio as GPIO.
    PINMUX_AUDIO_AC97   =    (0x01 << PINMUX_AUDIO_SHIFT),    ///< Audio as AC97.
    PINMUX_AUDIO_I2S    =    (0x02 << PINMUX_AUDIO_SHIFT),    ///< Audio as I2S.

    ENUM_DUMMY4WORD(PINMUX_ADUIO)
}PINMUX_ADUIO;
//@}
#define PINMUX_AUDIO_BITMASK    (0x03 << PINMUX_AUDIO_SHIFT)

/**
    Pinmux of SIF interface

    Use for pinmux_enable_sif()/ pinmux_disable_sif()/ pinmux_get_sif().
*/
//@{
typedef enum
{
    PINMUX_SIF_CH0     =     (0x01 << (PINMUX_SIF_SHFIT + 0)),    ///< SIF channel 0.
    PINMUX_SIF_CH1     =     (0x01 << (PINMUX_SIF_SHFIT + 1)),    ///< SIF channel 1.
    PINMUX_SIF_CH2     =     (0x01 << (PINMUX_SIF_SHFIT + 2)),    ///< SIF channel 2.
    PINMUX_SIF_CH3     =     (0x01 << (PINMUX_SIF_SHFIT + 3)),    ///< SIF channel 3.
    PINMUX_SIF_CH4     =     (0x01 << (PINMUX_SIF_SHFIT + 4)),    ///< SIF channel 4.
    PINMUX_SIF_CH5     =     (0x01 << (PINMUX_SIF_SHFIT + 5)),    ///< SIF channel 5.

    ENUM_DUMMY4WORD(PINMUX_SIF)
}PINMUX_SIF;
//@}
#define PINMUX_SIF_BITMASK      (0x3F << PINMUX_SIF_SHFIT)

/**
    Pinmux of SPI interface.

    Use for pinmux_select_spi()/ pinmux_get_spi().

    @name   Pinmux of SPI interface
*/
//@{
#define PINMUX_SPI_SERIAL       (0x01 << 12) ///< SPI.
//@}

/**
    SPI mode selection

    Use for pinmux_select_spimode()/ pinmux_get_spimode().

    @name   SPI mode selection
*/
//@{
#define PINMUX_SPI_MODE_STORAGE (0x00 << 13) ///< SPI pins to storage
#define PINMUX_SPI_MODE_SERIAL  (0x01 << 13) ///< SPI pins to parallel interface.
//@}
#define PINMUX_SPI_MODE_BITMASK (0x01 << 13)

/**
    Pinmux of uart interface.

    Use for pinmux_select_uart()/ pinmux_get_uart().

    @name   Pinmux of uart interface
*/
//@{
#define PINMUX_UART            (0x01 << 16) ///< Uart.
//@}

/**
    Pinmux of uart2 interface.

    Use for pinmux_select_uart2()/ pinmux_get_uart2().
*/
//@{
typedef enum
{
    PINMUX_UART2_GPIO   =    (0x00 << PINMUX_UART2_SHIFT),    ///< Uart2 as GPIO.
    PINMUX_UART2_2PIN   =    (0x01 << PINMUX_UART2_SHIFT),    ///< Uart2 2 pins mode.(TX/RX)
    PINMUX_UART2_4PIN   =    (0x02 << PINMUX_UART2_SHIFT),    ///< Uart2 4 pins mode.(TX/RX/CTS/RTS)
    PINMUX_UART2_8PIN   =    (0x03 << PINMUX_UART2_SHIFT),    ///< Uart2 8 pins mode.

    ENUM_DUMMY4WORD(PINMUX_UART2)
}PINMUX_UART2;
//@}
#define PINMUX_UART2_BITMASK    (0x03 << PINMUX_UART2_SHIFT)

/**
    Pinmux of remote interface.

    Use for pinmux_select_remote()/ pinmux_get_remote().

    @name   Pinmux of remote interface
*/
//@{
#define PINMUX_REMOTE           (0x01 << 19)    ///< Remote Rx.
//@}

/**
    Pinmux of USB OTG interface.

    Use for pinmux_select_usbotg()/ pinmux_get_usbotg().

    @name   Pinmux of USB OTG interface.
*/
//@{
#define PINMUX_USBOTG           (0x01 << 20)    ///< Pinmux of USB_DRVBUS.
//@}

/**
    Pinmux of ICE interface

    Use for pinmux_select_goice()/ pinmux_get_goice()/ pinmux_select_ickimsidi()/ pinmux_get_ickimsidi().

    @name   Pinmux of ICE interface
*/
//@{
#define PINMUX_GOICE            (0x01 << 24)    ///< GOICE.
#define PINMUX_ICKIMSIDI        (0x01 << 25)    ///< ICK/IMS/IDI.
//@}

/**
    Pinmux of HDMI DDC/CEC/Hotplug interface.

    Use for pinmux_select_ddc()/ pinmux_select_cec()/ pinmux_select_hotplug()/ pinmux_get_ddc()/ pinmux_get_cec() / pinmux_get_hotplug().

    @name   Pinmux of HDMI DDC/CEC/Hotplug interface
*/
//@{
#define PINMUX_DDC              (0x01 << 28)    ///< DDCSDA / DDCSCL.
#define PINMUX_CEC              (0x01 << 29)    ///< CEC.
#define PINMUX_HOTPLUG          (0x01 << 30)    ///< Hotplug.
//@}

//
// TOP Control Register 5
//

#define PINMUX_PROM_ONEBITSEL_SHIFT     0
#define PINMUX_USB_MODE_SHIFT           16

/**
    PROM one bit address selection.

    Use for pinmux_select_prom_onebitaddr()/ pinmux_get_prom_onebitaddr().
*/
//@{
typedef enum
{
    PINMUX_PROM_ONEBITSEL_A0    =    (0x00 << PINMUX_PROM_ONEBITSEL_SHIFT), ///< Select A0.
    PINMUX_PROM_ONEBITSEL_A1    =    (0x01 << PINMUX_PROM_ONEBITSEL_SHIFT), ///< Select A1.
    PINMUX_PROM_ONEBITSEL_A2    =    (0x02 << PINMUX_PROM_ONEBITSEL_SHIFT), ///< Select A2.
    PINMUX_PROM_ONEBITSEL_A3    =    (0x03 << PINMUX_PROM_ONEBITSEL_SHIFT), ///< Select A3.
    PINMUX_PROM_ONEBITSEL_A11   =    (0x04 << PINMUX_PROM_ONEBITSEL_SHIFT), ///< Select A11.
    PINMUX_PROM_ONEBITSEL_A12   =    (0x05 << PINMUX_PROM_ONEBITSEL_SHIFT), ///< Select A12.

    ENUM_DUMMY4WORD(PINMUX_PROM_ONEBITSEL)
}PINMUX_PROM_ONEBITSEL;
//@}
#define PINMUX_PROM_ONEBITSEL_BITMASK   (0x07 << PINMUX_PROM_ONEBITSEL_SHIFT)

/**
    Select XD pin mode.

    Use for pinmux_select_xd_socket()/ pinmux_get_xd_socket().

    @name   Select XD pin mode
*/
//@{
#define PINMUX_XD_SOCKET                (0x01 << 4) ///< XD_SOCKET.
//@}

/**
    Select SDIO pin mode

    Use for pinmux_select_sdio_pinmode()/ pinmux_get_sdio_pinmode().

    @name   Select SDIO pin mode
*/
//@{
#define PINMUX_SDIO_PINMODE             (0x01 << 5) ///< SDIO_PINMODE.
//@}

/**
    Select USB mode

    Use for pinmux_select_usb_mode()/ pinmux_get_usb_mode().
*/
//@{
typedef enum
{
    PINMUX_USB_MODE_OTG       =      (0x00 << PINMUX_USB_MODE_SHIFT),   ///< Usb at OTG mode
    PINMUX_USB_MODE_DEVICE    =      (0x01 << PINMUX_USB_MODE_SHIFT),   ///< Usb at Device mode
    PINMUX_USB_MODE_HOST      =      (0x02 << PINMUX_USB_MODE_SHIFT),   ///< Usb at Host mode

    ENUM_DUMMY4WORD(PINMUX_USB_MODE)
}PINMUX_USB_MODE;
//@}
#define PINMUX_USB_MODE_MASK            (0x03 << PINMUX_USB_MODE_SHIFT)


//
// PinMux Control Debug Port Register
//

/**
    Debug port select ID

    Debug port select value for pinmux_select_debug().

    @name   Debug port select ID
*/
//@{
#define PINMUX_DEBUGPORT_BITMASK    0x003F
#define PINMUX_DEBUGPORT_AMBARD     0x0000
#define PINMUX_DEBUGPORT_AMBAWR     0x0001
#define PINMUX_DEBUGPORT_AHB        0x0002
#define PINMUX_DEBUGPORT_AHBC0      0x0003
#define PINMUX_DEBUGPORT_AHBC1      0x0004
#define PINMUX_DEBUGPORT_APBBRG     0x0005
#define PINMUX_DEBUGPORT_DDRIO      0x0006
#define PINMUX_DEBUGPORT_DDR        0x0007
#define PINMUX_DEBUGPORT_USB        0x0008
#define PINMUX_DEBUGPORT_CKG        0x0009
#define PINMUX_DEBUGPORT_SIE        0x000A
#define PINMUX_DEBUGPORT_IPE        0x000B
#define PINMUX_DEBUGPORT_IME        0x000C
#define PINMUX_DEBUGPORT_IDE        0x000D
#define PINMUX_DEBUGPORT_DIS        0x000E
#define PINMUX_DEBUGPORT_FD         0x000F
#define PINMUX_DEBUGPORT_JPEG       0x0010
#define PINMUX_DEBUGPORT_H264       0x0011
#define PINMUX_DEBUGPORT_ADM        0x0012
#define PINMUX_DEBUGPORT_SDIO       0x0013
#define PINMUX_DEBUGPORT_SDIO2      0x0014
#define PINMUX_DEBUGPORT_AUDIO      0x0015
#define PINMUX_DEBUGPORT_CFATA      0x0016
#define PINMUX_DEBUGPORT_SPI        0x0017
#define PINMUX_DEBUGPORT_GRPH       0x0018
#define PINMUX_DEBUGPORT_XD         0x0019
#define PINMUX_DEBUGPORT_SSP        0x001A
#define PINMUX_DEBUGPORT_SIF        0x001B
#define PINMUX_DEBUGPORT_MS         0x001C
#define PINMUX_DEBUGPORT_RMT        0x001D
#define PINMUX_DEBUGPORT_I2C        0x001E
#define PINMUX_DEBUGPORT_PSBM       0x001F
#define PINMUX_DEBUGPORT_HDMI       0x0020
#define PINMUX_DEBUGPORT_UART       0x0021
#define PINMUX_DEBUGPORT_UART2      0x0022

#define PINMUX_DEBUGPORT_ENABLE     0x0100  ///< OR'd bit with debug select ID
//@}




//
// API for Top controller
//
extern UINT32           pinmux_get_bootsource(void);

extern void             pinmux_active_storage(PINMUX_ACTID uiStg);
extern void             pinmux_deactive_storage(PINMUX_ACTID uiStg);
extern PINMUX_CARDTYPE  pinmux_getactive_storage(void);

extern void             pinmux_setexist_storage(PINMUX_EXISTID uiStg);
extern void             pinmux_unsetexist_storage(PINMUX_EXISTID uiStg);
extern PINMUX_CARDTYPE  pinmux_getexist_storage(void);

extern void             pinmux_select_promsize(PINMUX_PROMSIZE uiSize);
extern PINMUX_PROMSIZE  pinmux_get_promsize(void);

extern void             pinmux_select_promcs(PINMUX_PROMCS uiCS);
extern PINMUX_PROMCS    pinmux_get_promcs(void);

extern void             pinmux_select_promwidth(PINMUX_PROMDATA uiDataWidth);
extern PINMUX_PROMDATA  pinmux_get_promwidth(void);

extern void             pinmux_select_prom_pinmode(PINMUX_PROM uiPinMode);
extern PINMUX_PROM      pinmux_get_prom_pinmode(void);

extern void             pinmux_enanble_iowait(UINT32 uiMes);
extern void             pinmux_disable_iowait(UINT32 uiMes);
extern UINT32           pinmux_get_iowait(void);

extern void             pinmux_select_mmcwidth(PINMUX_MMCDATA uiDataWidth);
extern PINMUX_MMCDATA   pinmux_get_mmcwidth(void);

extern void             pinmux_select_nandcs(PINMUX_NANDCS uiCS);
extern PINMUX_NANDCS    pinmux_get_nandcs(void);

extern void             pinmux_select_mswidth(PINMUX_MSDATA uiDataWidth);
extern PINMUX_MSDATA    pinmux_get_mswidth(void);

extern void             pinmux_select_lcd(PINMUX_LCD uiLCD);
extern PINMUX_LCD       pinmux_get_lcd(void);

extern void             pinmux_select_plcdwidth(PINMUX_PLCD uiDataWidth);
extern PINMUX_PLCD      pinmux_get_plcdwidth(void);

extern void             pinmux_select_hdmiifwidth(PINMUX_HDMIIF uiDataWidth);
extern PINMUX_HDMIIF    pinmux_get_hdmiifwidth(void);

extern void             pinmux_enable_de(void);
extern void             pinmux_disable_de(void);
extern UINT32           pinmux_get_de(void);

extern void                 pinmux_select_lcdccirwidth(PINMUX_LCD_CCIR_DATA uiDataWidth);
extern PINMUX_LCD_CCIR_DATA pinmux_get_lcdccirwidth(void);

extern void             pinmux_select_sensor(PINMUX_SENSOR uiSensor);
extern PINMUX_SENSOR    pinmux_get_sensor(void);

extern void             pinmux_select_mclk(UINT32 uiMclk);
extern UINT32           pinmux_get_mclk(void);

extern void             pinmux_enable_mes(PINMUX_MES uiMes);
extern void             pinmux_disable_mes(PINMUX_MES uiMes);
extern PINMUX_MES       pinmux_get_mes(void);

extern void             pinmux_select_flctr(UINT32 uiFlctr);
extern UINT32           pinmux_get_flctr(void);

extern void             pinmux_select_spclk(UINT32 uiSPClk);
extern UINT32           pinmux_get_spclk(void);

extern void             pinmux_select_senccir(UINT32 uiSenMode);
extern UINT32           pinmux_get_senccir(void);

extern void             pinmux_select_senccirwidth(PINMUX_SEN_CCIR uiDataDidth);
extern PINMUX_SEN_CCIR  pinmux_get_senccirwidth(void);

extern void             pinmux_enable_pwm(PINMUX_PWM uiPWMs);
extern void             pinmux_disable_pwm(PINMUX_PWM uiPWMs);
extern PINMUX_PWM       pinmux_get_pwm(void);

extern void             pinmux_select_i2c(UINT32 uiI2C);
extern UINT32           pinmux_get_i2c(void);

extern void             pinmux_select_audmclk(UINT32 uiAudMclk);
extern UINT32           pinmux_get_audmclk(void);

extern void             pinmux_select_audio(PINMUX_ADUIO uiAduio);
extern PINMUX_ADUIO     pinmux_get_audio(void);

extern void             pinmux_enable_sif(PINMUX_SIF uiSIF);
extern void             pinmux_disable_sif(PINMUX_SIF uiSIF);
extern PINMUX_SIF       pinmux_get_sif(void);

extern void             pinmux_select_spi(UINT32 uiSPI);
extern UINT32           pinmux_get_spi(void);

extern void             pinmux_select_spimode(UINT32 uiSPIMode);
extern UINT32           pinmux_get_spimode(void);

extern void             pinmux_select_uart(UINT32 uiUART);
extern UINT32           pinmux_get_uart(void);

extern void             pinmux_select_uart2(PINMUX_UART2 uiUART2);
extern PINMUX_UART2     pinmux_get_uart2(void);

extern void             pinmux_select_remote(UINT32 uiREMOTE);
extern UINT32           pinmux_get_remote(void);

extern void             pinmux_select_usbotg(UINT32 uiUSBOTG);
extern UINT32           pinmux_get_usbotg(void);

extern void             pinmux_select_goice(UINT32 uiGoice);
extern UINT32           pinmux_get_goice(void);

extern void             pinmux_select_ickimsidi(UINT32 uiIckImsIdi);
extern UINT32           pinmux_get_ickimsidi(void);

extern void             pinmux_select_ddc(UINT32 uiDDC);
extern UINT32           pinmux_get_ddc(void);

extern void             pinmux_select_cec(UINT32 uiCEC);
extern UINT32           pinmux_get_cec(void);

extern void             pinmux_select_hotplug(UINT32 uiHotplug);
extern UINT32           pinmux_get_hotplug(void);

extern void                     pinmux_select_prom_onebitaddr(PINMUX_PROM_ONEBITSEL uiAddr);
extern PINMUX_PROM_ONEBITSEL    pinmux_get_prom_onebitaddr(void);

extern void             pinmux_select_xd_socket(UINT32 uiXdSocket);
extern UINT32           pinmux_get_xd_socket(void);

extern void             pinmux_select_sdio_pinmode(UINT32 uiSdioMode);
extern UINT32           pinmux_get_sdio_pinmode(void);

extern void             pinmux_select_usb_mode(PINMUX_USB_MODE uiUsbMode);
extern PINMUX_USB_MODE  pinmux_get_usb_mode(void);

extern void             pinmux_select_debugport(UINT uiDebug);

//@}    //addtogroup mIHALSysCfg

#endif

