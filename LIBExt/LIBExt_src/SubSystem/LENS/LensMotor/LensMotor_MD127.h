/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_MD127.h
    @ingroup    mISYSLens

    @brief      SiTI MD127 driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/05/12
*/

#ifndef _LENSMOTOR_MD127_H
#define _LENSMOTOR_MD127_H
#include "Type.h"

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    BOOL          bInitYet;
    UINT32        uiPinInput1;
    UINT32        uiPinInput2;
    UINT32        uiPinInput3;
    UINT32        uiPinInput4;

    //pwm control related var
    BOOL          bPwmCtrl;
    UINT          uiPwmTimerID;
    UINT32        uiPwmonTime;
    UINT32        uiPwmoffTime;
    MOTOR_ZOOM_ACT        pwmprevdirection;
} MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;


#endif

