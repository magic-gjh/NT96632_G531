/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_AT5510.h
    @ingroup    mISYSLens

    @brief      SiTI AT5510 driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSMOTOR_AT5510_H
#define _LENSMOTOR_AT5510_H
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
    UINT32        uiPinMotorEn;
    //AT5550 usage
    UINT32        uiPinInput1;
    UINT32        uiPinInput2;
    UINT32        uiPinInput3;
    UINT32        uiPinInput4;

    //I2C,AT5510 usage
    UINT32        uiDeviceAddress;
    BOOL          bPowerDown;

    //pwm control related var
    BOOL          bPwmCtrl;
    UINT          uiPwmTimerID;
    UINT32        uiPwmonTime;
    UINT32        uiPwmoffTime;
    MOTOR_ZOOM_ACT        pwmprevdirection;
} MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

#define _LENS_ZOOM_PSEUDO_PWM_          ENABLE
#define _LENS_ZOOM_PSEUDO_PWM_PERIOD_   40000   //unit: us
#define _LENS_ZOOM_PSEUDO_PWM_ONTIME_   10000   //This value is experimental.

#endif

