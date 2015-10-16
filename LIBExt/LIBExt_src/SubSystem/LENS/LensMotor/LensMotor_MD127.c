/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_MD127.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "SiTI MD127"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/05/12
*/

#include "OSCommon.h"
#include "PWM.h"
#include "SIF.h"
#include "GPIO.h"
#include "pad.h"
#include "top.h"
#include "Timer.h"
#include "Lens.h"
#include "LensMotor_DUMMY.h"
#include "LensMotor_MD127.h"

//static var declare
static MOTOR_TAB        gMD127Obj;//extern object for others
static MOTOR_PVT_OBJ    gMD127PvtObj;//private object for internal usage
//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_enable_mes((sel))
//new architecture
#define MOTOR_PINDEF_MOTO_IN1            (gMD127PvtObj.uiPinInput1)//SN_MES0 = AGPIO20
#define MOTOR_PINDEF_MOTO_IN2            (gMD127PvtObj.uiPinInput2)//SN_MES1 = AGPIO21
//#define MOTOR_PINDEF_MOTO_IN3            (gMD127PvtObj.uiPinInput3)
//#define MOTOR_PINDEF_MOTO_IN4            (gMD127PvtObj.uiPinInput4)

//#define MOTOR_PINDEF_IRIS_PIN0           MOTOR_PINDEF_MOTO_IN3
//#define MOTOR_PINDEF_IRIS_PIN1           MOTOR_PINDEF_MOTO_IN4

#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN2

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorMD127_init(PLENS_DEVICE_OBJ pLensDev);
static void motorMD127_changeMode(MOTOR_MODE state);
static void motorMD127_resetInputPin(void);
static void motorMD127_aperture_setState(UCHAR state);
static void motorMD127_shutter_setState(UCHAR state);


static MOTOR_TAB gMD127Obj =
{
    motorMD127_init,
    motorDUMMY_power,
    motorMD127_changeMode,
    motorDUMMY_setPWMInfo,
    motorDUMMY_zoom_setState,
    motorDUMMY_zoom_setPIInt,
    motorDUMMY_zoom_clearPIInt,
    motorDUMMY_zoom_checkPIInt,
    motorDUMMY_zoom_checkPR,
    motorDUMMY_zoom_checkPI,
    motorDUMMY_focus_checkPI,
    motorDUMMY_focus_setState,
    motorMD127_aperture_setState,
    motorMD127_shutter_setState
};

static MOTOR_PVT_OBJ gMD127PvtObj =
{
    FALSE,
    0,0,0,0,//pin assign

    FALSE,//pwm control
    LENS_NULL_TIMER,
    10000,//pwm on
    30000,//pwm off
    MOTOR_ZOOM_OFF
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with common related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor init

    Init MD127 pin assignment

    @param None.

    @return None.
*/
static void motorMD127_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gMD127PvtObj.bInitYet)
    {
        debug_err(("motorMD127_init already\n\r"));
        return;
    }

    if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_SIF)
    {
    }
    else
    {
        //assign pin number from user,to be construct.
        gMD127PvtObj.uiPinInput1  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gMD127PvtObj.uiPinInput2  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        //gMD127PvtObj.uiPinInput3  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_2];
        //gMD127PvtObj.uiPinInput4  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_3];
        //debug_err(("[MD127]motorMD127_init,%d %d %d %d\n\r",gMD127PvtObj.uiPinInput1,gMD127PvtObj.uiPinInput2,gMD127PvtObj.uiPinInput3,gMD127PvtObj.uiPinInput4));
    }

    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2, GPIO_DIR_OUTPUT);
    //gpio_setDir(MOTOR_PINDEF_MOTO_IN3, GPIO_DIR_OUTPUT);
    //gpio_setDir(MOTOR_PINDEF_MOTO_IN4, GPIO_DIR_OUTPUT);

    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);

    gMD127PvtObj.bInitYet = TRUE;
    return;
}

/**
    Get MD127 motor object

    @return MD127 motor object pointer

*/
PMOTOR_TAB motorMD127_getObject()
{
    return &gMD127Obj;
}

/**
    motor mode setting

    Set MOTOR_MODE_STANDBY/MOTOR_MODE_ZOOM/MOTOR_MODE_FOCUS/MOTOR_MODE_APERTURE/MOTOR_MODE_SHUTTER to motor

    @param state - The mode state of motor.

    @return None.
*/
static void motorMD127_changeMode(MOTOR_MODE state)
{
    if(!gMD127PvtObj.bInitYet)
    {
        debug_err(("motorMD127_init not ready\n\r"));
        return;
    }
    //debug_err(("[MD127]motorMD127_mode_change, state=%d\n\r", state));
    switch(state)
    {
        case MOTOR_MODE_STANDBY:
            motorMD127_resetInputPin();
            break;
        case MOTOR_MODE_ZOOM:
            motorMD127_resetInputPin();
            //MOTOR_PinMux_Select_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_FOCUS:
            motorMD127_resetInputPin();
//            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_APERTURE:
            motorMD127_resetInputPin();
//            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_SHUTTER:
            motorMD127_resetInputPin();
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        default:
            debug_err(("[MD127]motorMD127_changeMode,UNKNOW\n\r"));
            break;
    }
    return;
}

/**
    Reset input pin of motor

    Reset input pin of motor.

    @param None

    @return None.
*/
static void motorMD127_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom related API.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with focus related API.
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with aperture related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    motor aperture mode action

    Set phase to motor in aperture mode

    @param state - The phase state of aperture mode.

    @return None.
*/
static void motorMD127_aperture_setState(UCHAR state)
{
    if(!gMD127PvtObj.bInitYet)
    {
        debug_err(("motorMD127_init not ready\n\r"));
        return;
    }
/*
    pinmux_disable_pwm(PINMUX_PWM5|PINMUX_PWM6);
    //debug_err(("[MD127]motorMD127_aperture_setState,state=0x%x\n\r",state));

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN0);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN1);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN1);

    TimerDelayMs(27);// 20
    gpio_clearPin(MOTOR_PINDEF_IRIS_PIN0);
    gpio_clearPin(MOTOR_PINDEF_IRIS_PIN1);
    return;
*/
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with shutter related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    motor shutter mode action

    Set phase to motor in shutter mode

    @param state - The phase state of shutter mode.

    @return None.
*/
static void motorMD127_shutter_setState(UCHAR state)
{
    if(!gMD127PvtObj.bInitYet)
    {
        debug_err(("motorMD127_init not ready\n\r"));
        return;
    }
    debug_err(("[MD127]motorMD127_shutter_setState,state=0x%x\n\r",state));

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES1);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES1);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES0);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES0);
    TimerDelayMs(27);// 20
    gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES0);
    gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES1);

    return;
}

