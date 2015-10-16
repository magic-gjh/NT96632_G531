/**
    Header file of PWM module driver

    This file is the header file of PWM module

    @file       PWM.h
    @ingroup    mIIOPWM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _PWM_H
#define _PWM_H

#include "Type.h"

/**
    @addtogroup mIIOPWM
*/
//@{

/**
    Invert PWM output signal or not for pwm_set() API
*/
typedef enum
{
    PWM_SIGNAL_NORMAL,      ///< Don't invert PWM output signal
    PWM_SIGNAL_INVERT       ///< Invert PWM output signal
} PWM_SIGNAL_MODE;

/**
    Invert CCNT input signal or not for pwm_setCCNT() API
*/
typedef enum
{
    PWM_CCNT_SIGNAL_NORMAL, ///< Don't invert CCNT input signal
    PWM_CCNT_SIGNAL_INVERT  ///< Invert CCNT input signal
} PWM_CCNT_SIGNAL_MODE;


/**
    PWM CCNT mode for pwm_setCCNT() API
*/
typedef enum
{
    PWM_CCNT_MODE_PULSE,    ///< Counting pulse
    PWM_CCNT_MODE_EDGE      ///< Counting edge, both rising and falling edges
} PWM_CCNT_MODE;


/**
    @name PWM ID

    PWM ID for PWM driver API
*/
//@{
#define PWMID_0                 0x00000001  ///< PWM ID 0
#define PWMID_1                 0x00000002  ///< PWM ID 1
#define PWMID_2                 0x00000004  ///< PWM ID 2
#define PWMID_3                 0x00000008  ///< PWM ID 3
#define PWMID_4                 0x00000010  ///< PWM ID 4
#define PWMID_5                 0x00000020  ///< PWM ID 5
#define PWMID_6                 0x00000040  ///< PWM ID 6
#define PWMID_7                 0x00000080  ///< PWM ID 7
#define PWMID_CCNT              0x00008000  ///< PWM ID CCNT
//@}

/**
    PWM configuration structure
*/
typedef struct
{
    UINT32  uiDiv;                  ///< Clock divider, PWM clock = 3MHZ / (uiDiv + 1), 0 ~ 0xFF
    UINT32  uiPrd;                  ///< How many PWM clock per period, 0 ~ 0xFF
                                    ///< @note uiRise <= uiFall <= uiPrd
    UINT32  uiRise;                 ///< Rising at which clock
                                    ///< @note uiRise <= uiFall <= uiPrd
    UINT32  uiFall;                 ///< Falling at which clock
                                    ///< @note uiRise <= uiFall <= uiPrd
    UINT32  uiOnCycle;              ///< Output cycle, 0 ~ 0x3FF
                                    ///< - @b PWM_FREE_RUN: Free Run
                                    ///< - @b Others: How many cycles (PWM will stop after output the cycle counts)
    PWM_SIGNAL_MODE  uiInv;         ///< Invert PWM output signal or not
                                    ///< - @b PWM_SIGNAL_NORMAL: Don't invert PWM output signal
                                    ///< - @b PWM_SIGNAL_INVERT: Invert PWM output signal
} PWM_CFG, *PPWM_CFG;

/**
    PWM CCNT configuration structure
*/
typedef struct
{
    UINT32  uiNum;                  ///< Count down number, 0 ~ 0xFFFF
    UINT32  uiFilter;               ///< Filter, 0 ~ 7 (1 ~ 8 clock), signal less than this filter will be ignored
    PWM_CCNT_SIGNAL_MODE  uiInv;    ///< Invert CCNT input signal or not
                                    ///< - @b PWM_CCNT_SIGNAL_NORMAL: Don't invert CCNT input signal
                                    ///< - @b PWM_CCNT_SIGNAL_INVERT: Invert CCNT input signal
    PWM_CCNT_MODE  uiMode;          ///< CCNT mode
                                    ///< - @b PWM_CCNT_MODE_PULSE: Counting pulse
                                    ///< - @b PWM_CCNT_MODE_EDGE: Counting edge, both rising and falling edges
} PWM_CCNT_CFG, *PPWM_CCNT_CFG;

/**
    MSTP configuration structure
*/
typedef struct
{
    UINT32  uiDiv;                  ///< Clock divider, PWM clock = 3MHZ / (uiDiv + 1), 0 ~ 0xFF
    UINT32  uiCh;                   ///< Mirco step channel type ,0: A , 1: B ,2: ~A ,3: ~B
    UINT32  uiPh;                   ///< Micro step starting phase ,phase 0 ~ 7
    UINT32  uiDir;                  ///< Micro step moving direction ,0: Counterclockwise , 1: Clockwise
    UINT32  uiOnCycle;              ///< Number of phase for Micro step mode
} MSTP_CFG, *PMSTP_CFG;

/**
    @name Backward compatible definitions

    For old version PWM driver
*/
//@{
#define PWM_struct              PWM_CFG     ///< For old version PWM driver, don't use anymore
#define PPWM_struct             PPWM_CFG    ///< For old version PWM driver, don't use anymore
//@}

#define PWM_FREE_RUN            0           ///< For pwm_set() API, output cycle is free run


// PWM Driver APIs
extern ER pwm_open(UINT32 uiPWMId);
extern ER pwm_close(UINT32 uiPWMId);
extern ER pwm_set(UINT32 uiPWMId, PPWM_CFG pPWMCfg);
extern ER pwm_setCCNT(UINT32 uiPWMId, PPWM_CCNT_CFG pCCNTCfg);
extern ER pwm_stop(UINT32 uiPWMId);
extern ER pwm_en(UINT32 uiPWMId);
//#NT#2009/12/21#HH Chuang -begin
//#NT#add PWM reload API
extern ER pwm_reload(UINT32 uiPWMId);
//#NT#2009/12/21#HH Chuang -end
extern ER pwm_ms_stop(UINT32 uiPWMId);
extern ER pwm_ms_en(UINT32 uiPWMId);
extern ER pwm_wait(UINT32 uiPWMId);

//@}

#endif
