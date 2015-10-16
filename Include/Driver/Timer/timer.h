/**
    Public header file for timer module.

    This file is the header file that define the API and data type for timer
    module.

    @file       timer.h
    @ingroup    mIHALTimer
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _TIMER_H
#define _TIMER_H

#include "Type.h"

/** \addtogroup mIHALTimer */
//@{

/**
    @name Timer ID definition.
    @brief Contant symbols for Timer ID.
*/
//@{
#define TIMER_0         0
#define TIMER_1         1
#define TIMER_2         2
#define TIMER_3         3
#define TIMER_4         4
#define TIMER_5         5
#define TIMER_6         6
#define TIMER_7         7
//@}

/**
    @name Contant symbols for timer mode.
    @brief Contant symbols for uiMode in timer_set() and timer_set_us().
*/
//@{
#define _TIMER_CTRL_FREE_RUN        0x00000002      ///< Timer operation mode is "free run"
#define _TIMER_CTRL_ONE_SHOT        0x00000000      ///< Timer operation mode is "one shot"
#define _TIMER_CTRL_INT_ENABLE      0x00000008      ///< Enable timeout interrupt
//@}

/**
    @name Contant symbols for uiState.
    @brief Contant symbols for uiState in timer_set(), timer_set_us(), timer_pausePlay().
*/
//@{
#define _TIMER_PAUSE                0               ///< Stop timer
#define _TIMER_PLAY                 1               ///< Start timer
//@}

/**
    @name Backward compatible definitions

    For old version timer driver
*/
//@{
#define timer_set(uiTIMERId, uiIntval, uiMode, uiState)         timer_cfg(uiTIMERId, 1000*uiIntval, uiMode, uiState)
#define timer_set_us(uiTIMERId, uiIntval, uiMode, uiState)      timer_cfg(uiTIMERId, uiIntval, uiMode, uiState)
//@}

//Timer Driver API
extern ER timer_open(UINT *p_uiTIMERId, FP EventHandler);
extern ER timer_close(UINT uiTIMERId);
extern ER timer_openAutoClose(UINT *p_uiTIMERId, FP EventHandler);
extern ER timer_cfg(UINT uiTIMERId, UINT uiIntval, UINT uiMode, UINT uiState);
extern ER timer_pausePlay(UINT uiTIMERId, UINT uiState);
extern ER timer_waitTimeup(UINT uiTIMERId);
extern ER timer_checkTimeup(UINT uiTIMERId, BOOL *pbEn);
//Timer Utility API
extern UINT32 timer_getCurrentCount(UINT TimerID);
extern UINT32 timer_getTargetCount(UINT TimerID);
//@}

#endif // _TIMER_H


