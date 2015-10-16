/**
    General Purpose I/O controller header file

    General Purpose I/O controller header file

    @file       gpio.h
    @ingroup    mIIOGPIO
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _GPIO_H
#define _GPIO_H

#include "Type.h"

/**
    @addtogroup mIIOGPIO
*/
//@{

/**
    GPIO direction

    GPIO direction definition for gpio_setDir() and gpio_getDir()

    @name   GPIO direction
*/
//@{
#define GPIO_DIR_INPUT              0       ///< GPIO is input direction
#define GPIO_DIR_OUTPUT             1       ///< GPIO is output direction
//@}

/**
    GPIO interrupt type

    GPIO interrupt type definition for type argument of gpio_setIntTypePol()

    @name   GPIO interrupt type
*/
//@{
#define GPIO_INTTYPE_EDGE           0       ///< GPIO interrupt is edge trigger
#define GPIO_INTTYPE_LEVEL          1       ///< GPIO interrupt is level trigger
//@}

/**
    GPIO interrupt polarity

    GPIO interrupt polarity definition for pol argument of gpio_setIntTypePol()

    @name   GPIO interrupt polarity
*/
//@{
#define GPIO_INTPOL_POSHIGH         0       ///< GPIO interrupt polarity is
                                            ///< - positvie edge for edge trigger
                                            ///< - high level for level trigger
#define GPIO_INTPOL_NEGLOW          1       ///< GPIO interrupt polarity is
                                            ///< - negative edge for edge trigger
                                            ///< - low level for level trigger
//@}



/**
    GPIO pins ID definition

    For detail GPIO pin out, please refer to NT96630 data sheet.

    @name   GPIO pins ID definition
*/
//@{
/**
    @name   Storage GPIO / CGPIO
*/
//@{
#define C_GPIO_0    0
#define C_GPIO_1    1
#define C_GPIO_2    2
#define C_GPIO_3    3
#define C_GPIO_4    4
#define C_GPIO_5    5
#define C_GPIO_6    6
#define C_GPIO_7    7
#define C_GPIO_8    8
#define C_GPIO_9    9
#define C_GPIO_10   10
#define C_GPIO_11   11
#define C_GPIO_12   12
#define C_GPIO_13   13
#define C_GPIO_14   14
#define C_GPIO_15   15
#define C_GPIO_16   16
#define C_GPIO_17   17
#define C_GPIO_18   18
#define C_GPIO_19   19
#define C_GPIO_20   20
#define C_GPIO_21   21
#define C_GPIO_22   22
#define C_GPIO_23   23
#define C_GPIO_24   24
#define C_GPIO_25   25
#define C_GPIO_26   26
#define C_GPIO_27   27
#define C_GPIO_28   28
#define C_GPIO_29   29
#define C_GPIO_30   30
#define C_GPIO_31   31
#define C_GPIO_32   32
#define C_GPIO_33   33
#define C_GPIO_34   34
#define C_GPIO_35   35
#define C_GPIO_36   36
#define C_GPIO_37   37
#define C_GPIO_38   38
#define C_GPIO_39   39
#define C_GPIO_40   40
#define C_GPIO_41   41
#define C_GPIO_42   42
#define C_GPIO_43   43
#define C_GPIO_44   44
#define C_GPIO_45   45
#define C_GPIO_46   46
#define C_GPIO_47   47
#define C_GPIO_48   48
#define C_GPIO_49   49
#define C_GPIO_50   50
#define C_GPIO_51   51
#define C_GPIO_52   52
#define C_GPIO_53   53
#define C_GPIO_54   54
#define C_GPIO_55   55
#define C_GPIO_56   56
//@}

/**
    @name   Peripheral GPIO / PGPIO
*/
//@{
#define P_GPIO_SHIFT_BASE 64
#define P_GPIO_0    (0 +P_GPIO_SHIFT_BASE)
#define P_GPIO_1    (1 +P_GPIO_SHIFT_BASE)
#define P_GPIO_2    (2 +P_GPIO_SHIFT_BASE)
#define P_GPIO_3    (3 +P_GPIO_SHIFT_BASE)
#define P_GPIO_4    (4 +P_GPIO_SHIFT_BASE)
#define P_GPIO_5    (5 +P_GPIO_SHIFT_BASE)
#define P_GPIO_6    (6 +P_GPIO_SHIFT_BASE)
#define P_GPIO_7    (7 +P_GPIO_SHIFT_BASE)
#define P_GPIO_8    (8 +P_GPIO_SHIFT_BASE)
#define P_GPIO_9    (9 +P_GPIO_SHIFT_BASE)
#define P_GPIO_10   (10+P_GPIO_SHIFT_BASE)
#define P_GPIO_11   (11+P_GPIO_SHIFT_BASE)
#define P_GPIO_12   (12+P_GPIO_SHIFT_BASE)
#define P_GPIO_13   (13+P_GPIO_SHIFT_BASE)
#define P_GPIO_14   (14+P_GPIO_SHIFT_BASE)
#define P_GPIO_15   (15+P_GPIO_SHIFT_BASE)
#define P_GPIO_16   (16+P_GPIO_SHIFT_BASE)
#define P_GPIO_17   (17+P_GPIO_SHIFT_BASE)
#define P_GPIO_18   (18+P_GPIO_SHIFT_BASE)
#define P_GPIO_19   (19+P_GPIO_SHIFT_BASE)
#define P_GPIO_20   (20+P_GPIO_SHIFT_BASE)
#define P_GPIO_21   (21+P_GPIO_SHIFT_BASE)
#define P_GPIO_22   (22+P_GPIO_SHIFT_BASE)
#define P_GPIO_23   (23+P_GPIO_SHIFT_BASE)
#define P_GPIO_24   (24+P_GPIO_SHIFT_BASE)
#define P_GPIO_25   (25+P_GPIO_SHIFT_BASE)
#define P_GPIO_26   (26+P_GPIO_SHIFT_BASE)
#define P_GPIO_27   (27+P_GPIO_SHIFT_BASE)
#define P_GPIO_28   (28+P_GPIO_SHIFT_BASE)
#define P_GPIO_29   (29+P_GPIO_SHIFT_BASE)
#define P_GPIO_30   (30+P_GPIO_SHIFT_BASE)
#define P_GPIO_31   (31+P_GPIO_SHIFT_BASE)

#define P_GPIO_32   (32+P_GPIO_SHIFT_BASE)
#define P_GPIO_33   (33+P_GPIO_SHIFT_BASE)
#define P_GPIO_34   (34+P_GPIO_SHIFT_BASE)
#define P_GPIO_35   (35+P_GPIO_SHIFT_BASE)
#define P_GPIO_36   (36+P_GPIO_SHIFT_BASE)
#define P_GPIO_37   (37+P_GPIO_SHIFT_BASE)
//@}

/**
    @name   Sensor GPIO / SGPIO
*/
//@{
#define S_GPIO_SHIFT_BASE 128
#define S_GPIO_0    (0 +S_GPIO_SHIFT_BASE)
#define S_GPIO_1    (1 +S_GPIO_SHIFT_BASE)
#define S_GPIO_2    (2 +S_GPIO_SHIFT_BASE)
#define S_GPIO_3    (3 +S_GPIO_SHIFT_BASE)
#define S_GPIO_4    (4 +S_GPIO_SHIFT_BASE)
#define S_GPIO_5    (5 +S_GPIO_SHIFT_BASE)
#define S_GPIO_6    (6 +S_GPIO_SHIFT_BASE)
#define S_GPIO_7    (7 +S_GPIO_SHIFT_BASE)
#define S_GPIO_8    (8 +S_GPIO_SHIFT_BASE)
#define S_GPIO_9    (9 +S_GPIO_SHIFT_BASE)
#define S_GPIO_10   (10+S_GPIO_SHIFT_BASE)
#define S_GPIO_11   (11+S_GPIO_SHIFT_BASE)
#define S_GPIO_12   (12+S_GPIO_SHIFT_BASE)
#define S_GPIO_13   (13+S_GPIO_SHIFT_BASE)
#define S_GPIO_14   (14+S_GPIO_SHIFT_BASE)
#define S_GPIO_15   (15+S_GPIO_SHIFT_BASE)
#define S_GPIO_16   (16+S_GPIO_SHIFT_BASE)
//@}

/**
    @name   LCD GPIO / LGPIO
*/
//@{
#define L_GPIO_SHIFT_BASE 160
#define L_GPIO_0    (0 +L_GPIO_SHIFT_BASE)
#define L_GPIO_1    (1 +L_GPIO_SHIFT_BASE)
#define L_GPIO_2    (2 +L_GPIO_SHIFT_BASE)
#define L_GPIO_3    (3 +L_GPIO_SHIFT_BASE)
#define L_GPIO_4    (4 +L_GPIO_SHIFT_BASE)
#define L_GPIO_5    (5 +L_GPIO_SHIFT_BASE)
#define L_GPIO_6    (6 +L_GPIO_SHIFT_BASE)
#define L_GPIO_7    (7 +L_GPIO_SHIFT_BASE)
#define L_GPIO_8    (8 +L_GPIO_SHIFT_BASE)
#define L_GPIO_9    (9 +L_GPIO_SHIFT_BASE)
#define L_GPIO_10   (10+L_GPIO_SHIFT_BASE)
#define L_GPIO_11   (11+L_GPIO_SHIFT_BASE)
#define L_GPIO_12   (12+L_GPIO_SHIFT_BASE)
#define L_GPIO_13   (13+L_GPIO_SHIFT_BASE)
#define L_GPIO_14   (14+L_GPIO_SHIFT_BASE)
#define L_GPIO_15   (15+L_GPIO_SHIFT_BASE)
#define L_GPIO_16   (16+L_GPIO_SHIFT_BASE)
#define L_GPIO_17   (17+L_GPIO_SHIFT_BASE)
#define L_GPIO_18   (18+L_GPIO_SHIFT_BASE)
#define L_GPIO_19   (19+L_GPIO_SHIFT_BASE)
#define L_GPIO_20   (20+L_GPIO_SHIFT_BASE)
#define L_GPIO_21   (21+L_GPIO_SHIFT_BASE)
#define L_GPIO_22   (22+L_GPIO_SHIFT_BASE)
#define L_GPIO_23   (23+L_GPIO_SHIFT_BASE)
#define L_GPIO_24   (24+L_GPIO_SHIFT_BASE)
#define L_GPIO_25   (25+L_GPIO_SHIFT_BASE)
#define L_GPIO_26   (26+L_GPIO_SHIFT_BASE)
#define L_GPIO_27   (27+L_GPIO_SHIFT_BASE)
#define L_GPIO_28   (28+L_GPIO_SHIFT_BASE)
//@}

/**
    @name   General GPIO / GGPIO
*/
//@{
#define G_GPIO_SHIFT_BASE 192
#define G_GPIO_0    (0 +G_GPIO_SHIFT_BASE)
#define G_GPIO_1    (1 +G_GPIO_SHIFT_BASE)
#define G_GPIO_2    (2 +G_GPIO_SHIFT_BASE)
#define G_GPIO_3    (3 +G_GPIO_SHIFT_BASE)

#define G_GPIO_8    (8 +G_GPIO_SHIFT_BASE)
#define G_GPIO_9    (9 +G_GPIO_SHIFT_BASE)
#define G_GPIO_10   (10+G_GPIO_SHIFT_BASE)
#define G_GPIO_11   (11+G_GPIO_SHIFT_BASE)
//@}

/**
    @name   Dedicated GPIO / DGPIO
*/
//@{
// In order to backward comaptible, DGPIO is used as " GPIO_IS_DGPIO | D_GPIO_* "
#define D_GPIO_SHIFT_BASE 224
#define D_GPIO_0    (0 +D_GPIO_SHIFT_BASE)
#define D_GPIO_1    (1 +D_GPIO_SHIFT_BASE)
#define D_GPIO_2    (2 +D_GPIO_SHIFT_BASE)
#define D_GPIO_3    (3 +D_GPIO_SHIFT_BASE)
#define D_GPIO_4    (4 +D_GPIO_SHIFT_BASE)
#define D_GPIO_5    (5 +D_GPIO_SHIFT_BASE)
#define D_GPIO_6    (6 +D_GPIO_SHIFT_BASE)
//@}

//@}




/**
    @name   GPIO interrupt ID definition
*/
//@{
#define GPIO_INT_00                 0
#define GPIO_INT_01                 1
#define GPIO_INT_02                 2
#define GPIO_INT_03                 3
#define GPIO_INT_04                 4
#define GPIO_INT_05                 5
#define GPIO_INT_06                 6
#define GPIO_INT_07                 7
#define GPIO_INT_08                 8
#define GPIO_INT_09                 9
#define GPIO_INT_10                 10
#define GPIO_INT_11                 11
#define GPIO_INT_12                 12
#define GPIO_INT_13                 13
#define GPIO_INT_14                 14
#define GPIO_INT_15                 15
#define GPIO_INT_16                 16
#define GPIO_INT_17                 17
#define GPIO_INT_18                 18
#define GPIO_INT_19                 19
#define GPIO_INT_20                 20
#define GPIO_INT_21                 21
#define GPIO_INT_22                 22
#define GPIO_INT_23                 23
#define GPIO_INT_24                 24
#define GPIO_INT_25                 25
#define GPIO_INT_26                 26
#define GPIO_INT_27                 27
#define GPIO_INT_28                 28
#define GPIO_INT_29                 29
#define GPIO_INT_30                 30

#define GPIO_INT_32                 32
#define GPIO_INT_33                 33
#define GPIO_INT_34                 34
#define GPIO_INT_35                 35
#define GPIO_INT_36                 36
#define GPIO_INT_37                 37
#define GPIO_INT_38                 38

// for backward compatible

//@}

//Total number of GPIO interrupts
#define GPIO_INT_NUM                39


/**
    @name   GPIO wait interrupt ID definition, Used for gpio_waitIntFlag(GPIO_INT_FLAG*)
*/
//@{
#define GPIO_INT_FLAG00                 (1ll << GPIO_INT_00)
#define GPIO_INT_FLAG01                 (1ll << GPIO_INT_01)
#define GPIO_INT_FLAG02                 (1ll << GPIO_INT_02)
#define GPIO_INT_FLAG03                 (1ll << GPIO_INT_03)
#define GPIO_INT_FLAG04                 (1ll << GPIO_INT_04)
#define GPIO_INT_FLAG05                 (1ll << GPIO_INT_05)
#define GPIO_INT_FLAG06                 (1ll << GPIO_INT_06)
#define GPIO_INT_FLAG07                 (1ll << GPIO_INT_07)
#define GPIO_INT_FLAG08                 (1ll << GPIO_INT_08)
#define GPIO_INT_FLAG09                 (1ll << GPIO_INT_09)
#define GPIO_INT_FLAG10                 (1ll << GPIO_INT_10)
#define GPIO_INT_FLAG11                 (1ll << GPIO_INT_11)
#define GPIO_INT_FLAG12                 (1ll << GPIO_INT_12)
#define GPIO_INT_FLAG13                 (1ll << GPIO_INT_13)
#define GPIO_INT_FLAG14                 (1ll << GPIO_INT_14)
#define GPIO_INT_FLAG15                 (1ll << GPIO_INT_15)
#define GPIO_INT_FLAG16                 (1ll << GPIO_INT_16)
#define GPIO_INT_FLAG17                 (1ll << GPIO_INT_17)
#define GPIO_INT_FLAG18                 (1ll << GPIO_INT_18)
#define GPIO_INT_FLAG19                 (1ll << GPIO_INT_19)
#define GPIO_INT_FLAG20                 (1ll << GPIO_INT_20)
#define GPIO_INT_FLAG21                 (1ll << GPIO_INT_21)
#define GPIO_INT_FLAG22                 (1ll << GPIO_INT_22)
#define GPIO_INT_FLAG23                 (1ll << GPIO_INT_23)
#define GPIO_INT_FLAG24                 (1ll << GPIO_INT_24)
#define GPIO_INT_FLAG25                 (1ll << GPIO_INT_25)
#define GPIO_INT_FLAG26                 (1ll << GPIO_INT_26)
#define GPIO_INT_FLAG27                 (1ll << GPIO_INT_27)
#define GPIO_INT_FLAG28                 (1ll << GPIO_INT_28)
#define GPIO_INT_FLAG29                 (1ll << GPIO_INT_29)
#define GPIO_INT_FLAG30                 (1ll << GPIO_INT_30)

#define GPIO_INT_FLAG32                 (1ll << GPIO_INT_32)
#define GPIO_INT_FLAG33                 (1ll << GPIO_INT_33)
#define GPIO_INT_FLAG34                 (1ll << GPIO_INT_34)
#define GPIO_INT_FLAG35                 (1ll << GPIO_INT_35)
#define GPIO_INT_FLAG36                 (1ll << GPIO_INT_36)
#define GPIO_INT_FLAG37                 (1ll << GPIO_INT_37)
#define GPIO_INT_FLAG38                 (1ll << GPIO_INT_38)
//@}



// For backward comaptible usage
#define GPIO_IS_DGPIO                   (224)
#define gpio_pullSet                    gpio_setPin
#define gpio_pullClear                  gpio_clearPin
#define GPIO_INT_USBWAKEUP              GPIO_INT_27
#define GPIO_INT_USBPLUGIN              GPIO_INT_26


//
// The general api for the GPIO device driver
//
extern ER       gpio_open(void);
extern ER       gpio_close(void);

extern ER       gpio_waitIntFlag(UINT64 WaitFlags);
extern ER       gpio_clearIntFlag(UINT64 ClrFlags);

extern void     gpio_setDir(UINT32 pin, UINT32 dir);
extern UINT32   gpio_getDir(UINT32 pin);
extern void     gpio_setPin(UINT32 pin);
extern void     gpio_clearPin(UINT32 pin);
extern UINT32   gpio_getPin(UINT32 pin);
extern void     gpio_pullSet(UINT32 pin);
extern void     gpio_pullClear(UINT32 pin);
extern UINT32   gpio_readData(UINT32 dataidx);
extern void     gpio_writeData(UINT32 dataidx, UINT32 value);

extern UINT32   dgpio_readData(void);
extern void     dgpio_writeData(UINT32 value);

extern void     gpio_enableInt(UINT32 ipin);
extern void     gpio_disableInt(UINT32 ipin);
extern UINT32   gpio_getIntEnable(UINT32 ipin);
extern void     gpio_clearIntStatus(UINT32 ipin);
extern UINT32   gpio_getIntStatus(UINT32 ipin);
extern void     gpio_setIntTypePol(UINT32 ipin, UINT32 type, UINT32 pol);
extern void     gpio_setIntIsr(UINT32 ipin, FP fpIsr);
extern UINT32   gpio_getIntIsrStatus(UINT32 ipin);



//@}
#endif

