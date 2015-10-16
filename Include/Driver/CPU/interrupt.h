/**
    Header file for Interrupt module

    This file is the header file that define the API for Interrupt module.

    @file       Interrupt.h
    @ingroup    mIHALInterrupt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "Type.h"

/**
    @addtogroup mIHALInterrupt
*/
//@{

/**
      @name     Interrupt Type

      @note for int_setType(), int_setTypeMulti()

      Definition for interrupt type
*/
//@{
#define _INT_TYPE_FIQ       1           ///< FIQ type interrupt
#define _INT_TYPE_IRQ       0           ///< IRQ type interrupt
//@}

/**
      @name     Interrupt ID

      @note for int_enable(), int_disable(), int_setType()

      Interrupt ID definition for usage as argument to API of this module.
*/
//@{
#define _INT_TIMER0         0       ///< Timer 0 interrupt
#define _INT_TIMER1         1       ///< Timer 1 interrupt
#define _INT_TIMER2         2       ///< Timer 2 interrupt
#define _INT_TIMER3         3       ///< Timer 3 interrupt
#define _INT_WDT            4       ///< Watchdog timer interrupt
#define _INT_RTC            5       ///< Real Time Clock interrupt
#define _INT_CG             6       ///< ClockGen interrupt
#define _INT_SPI            7       ///< SPI controller interrupt
#define _INT_REMOTE         8       ///< remote controller interrupt
#define _INT_DIS            9       ///< DIS controller interrupt
#define _INT_FD             10      ///< Face detection controller interrupt
#define _INT_PRE            11      ///< PRE controller interrupt
#define _INT_USBOTG         12      ///< USB controller interrupt
#define _INT_UART           13      ///< UART controller interrupt
#define _INT_I2C            14      ///< I2C controller interrupt
#define _INT_UART2          15      ///< UART2 controller interrupt
#define _INT_SIF            16      ///< SIF controller interrupt
#define _INT_PWM            17      ///< PWM controller interrupt
#define _INT_ADC            18      ///< ADC controller interrupt
#define _INT_GPIO           19      ///< GPIO controller interrupt
#define _INT_AUDIO          20      ///< Audio(DAI) controller interrupt
#define _INT_SIE            21      ///< SIE controller interrupt
#define _INT_IPE            22      ///< IPE controller interrupt
#define _INT_IME            23      ///< IME controller interrupt
#define _INT_IDE            24      ///< IDE controller interrupt
#define _INT_JPEG           25      ///< JPEG controller interrupt
#define _INT_H264           26      ///< H.264 controller interrupt
#define _INT_GRAPHY         27      ///< Graphic engine controller interrupt
#define _INT_AMBADMA_A      28      ///< DMA channel A interrupt
#define _INT_AMBADMA_B      29      ///< DMA channel B interrupt
#define _INT_AMBADMA_C      30      ///< DMA channel C interrupt
#define _INT_AMBADMA_D      31      ///< DMA channel D interrupt

#define _INT_TIMER4         32      ///< Timer 4 interrupt
#define _INT_TIMER5         33      ///< Timer 5 interrupt
#define _INT_TIMER6         34      ///< Timer 6 interrupt
#define _INT_TIMER7         35      ///< Timer 7 interrupt
#define _INT_NAND           36      ///< NAND/xD interrupt
#define _INT_CF             37      ///< CF interrupt
#define _INT_SDIO           38      ///< SDIO interrupt
#define _INT_SDIO2          39      ///< SDIO2 interrupt
#define _INT_MS             40      ///< MS card interrupt
#define _INT_HDMI           41      ///< HDMI interrupt
#define _INT_BMC            42      ///< BMC interrupt
#define _INT_PSBM           43      ///< PSBM interrupt
//@}

/**
    @name Interrupt module all interrupt mask

    @note for int_enableMulti(), int_disableMulti()
*/
//@{
#define _INT_ALL            0xFFFFFFFFFFFFFFFFULL   ///< all interrupt mask
//@}

/**
    @name Interrupt compatible definition

    Define backward compatible MACROs for old interrupt API naming.
*/
//@{
#define int_enable_multi    int_enableMulti         ///< enable multi interrupt
#define int_disable_multi   int_disableMulti        ///< disable multi interrupt
#define int_getflag         int_getFlag             ///< get interrupt flag
#define int_getfiqid        int_getFiqId            ///< get FIQ ID
#define int_getirqid        int_getIrqId            ///< get IRQ ID
#define int_getenable       int_getEnable           ///< get interrupt enable
#define int_gettype         int_getType             ///< get interrupt type
#define int_settype         int_setType             ///< set interrupt type
#define int_settype_multi   int_setTypeMulti        ///< set multi interrupt type
//@}

/*
    Public OS level functions
*/
extern void     int_init(void);

/*
    Public driver specific operating functions
*/
extern void     int_enable(UINT32 intno);
extern void     int_disable(UINT32 intno);

extern void     int_enableMulti(UINT64 intval);
extern void     int_disableMulti(UINT64 intval);

extern UINT64   int_getFlag(void);
extern UINT32   int_getFiqId(void);
extern UINT32   int_getIrqId(void);
extern UINT64   int_getEnable(void);
extern UINT64   int_getType(void);
extern void     int_setType(UINT32 uiIntNo, UINT32 uiType);
extern void     int_setTypeMulti(UINT64 uiIntVal, UINT32 uiType);

//@}

#endif

