/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_MD152.h
    @ingroup    mISYSLens

    @brief      SiTI MD152 driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSMOTOR_MD152_H
#define _LENSMOTOR_MD152_H
#include "Type.h"

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    BOOL          bInitYet;
    UINT32        uiPinPower;
    UINT32        uiPinEnable1;
    UINT32        uiPinEnable2;
    UINT32        uiPinEnable3;
    UINT32        uiPinInput1;
    UINT32        uiPinInput2;
    UINT32        uiPinInput3;
    UINT32        uiPinInput4;
    UINT32        uiPinZoomPI;
    UINT32        uiPinZoomPR;
    UINT32        uiPinFocusPI;
    UINT32        uiZoomPIINT;

    //pwm control related var
    BOOL          bPwmCtrl;
    UINT          uiPwmTimerID;
    UINT32        uiPwmPeriodTime;
    UINT32        uiPwmOnTime;
    MOTOR_ZOOM_ACT        pwmprevdirection;
} MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

#endif
