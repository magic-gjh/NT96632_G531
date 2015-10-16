/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_A3901.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "SiTI A3901"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

/*
    Todo: Randy
    1. Re-write function name.
    2. Define motor funciton table in Lens.h, refer to AUDIO_CODEC_FUNC.
    3. Clear unused dummy function.
    4. Use device obj to set pin.
*/
/*
    Todo:
    1. motorA3901_pinin_reset -> motorA3901_resetInputPin
    2. motorA3901_motorobject_get -> motorA3901_getObject
    3. motorA3901_zoomPR_check -> motorA3901_zoom_checkPR
    4. motorA3901_mode_change -> motorA3901_changeMode
    5. motorA3901_zoom_action -> motorA3901_zoom_setState
    6. motorA3901_pwm_process -> motorA3901_pwm_hdlPWM (hdlSpeed)
`   7. MOTOR_PinMux_Select_MES -> upper case
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
#include "LensMotor_A3901.h"

//static var declare
static MOTOR_TAB        gA3901Obj;//extern object for others
static MOTOR_PVT_OBJ    gA3901PvtObj;//private object for internal usage
//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_select_mes((sel))
//new architecture
#define MOTOR_PINDEF_MOTO_IN1            (gA3901PvtObj.uiPinInput1)//SN_MES0 = AGPIO20
#define MOTOR_PINDEF_MOTO_IN2            (gA3901PvtObj.uiPinInput2)//SN_MES1 = AGPIO21
#define MOTOR_PINDEF_MOTO_IN3            (gA3901PvtObj.uiPinInput3)
#define MOTOR_PINDEF_MOTO_IN4            (gA3901PvtObj.uiPinInput4)

#define MOTOR_PINDEF_IRIS_PIN0           MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_IRIS_PIN1           MOTOR_PINDEF_MOTO_IN2

#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN4

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorA3901_init(PLENS_DEVICE_OBJ pLensDev);
static void motorA3901_changeMode(MOTOR_MODE state);
static void motorA3901_resetInputPin(void);
static void motorA3901_pwm_hdlPWM(void);
static void motorA3901_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection);
static void motorA3901_aperture_setState(UCHAR state);
static void motorA3901_shutter_setState(UCHAR state);


static MOTOR_TAB gA3901Obj =
{
    motorA3901_init,
    motorDUMMY_power,
    motorA3901_changeMode,
    motorDUMMY_setPWMInfo,
    motorDUMMY_zoom_setState,
    motorDUMMY_zoom_setPIInt,
    motorDUMMY_zoom_clearPIInt,
    motorDUMMY_zoom_checkPIInt,
    motorDUMMY_zoom_checkPR,
    motorDUMMY_zoom_checkPI,
    motorDUMMY_focus_checkPI,
    motorDUMMY_focus_setState,
    motorA3901_aperture_setState,
    motorA3901_shutter_setState
};

static MOTOR_PVT_OBJ gA3901PvtObj =
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

    Init A3901 pin assignment

    @param None.

    @return None.
*/
static void motorA3901_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gA3901PvtObj.bInitYet)
    {
        debug_err(("motorA3901_init already\n\r"));
        return;
    }

    if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_SIF)
    {
    }
    else
    {
        //assign pin number from user,to be construct.
        gA3901PvtObj.uiPinInput1  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gA3901PvtObj.uiPinInput2  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        gA3901PvtObj.uiPinInput3  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_2];
        gA3901PvtObj.uiPinInput4  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_3];
        //debug_err(("[A3901]motorA3901_init,%d %d %d %d\n\r",gA3901PvtObj.uiPinInput1,gA3901PvtObj.uiPinInput2,gA3901PvtObj.uiPinInput3,gA3901PvtObj.uiPinInput4));
    }

    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN3, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN4, GPIO_DIR_OUTPUT);

    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);

    gA3901PvtObj.bInitYet = TRUE;
    return;
}

/**
    Get A3901 motor object

    @return A3901 motor object pointer

*/
PMOTOR_TAB motorA3901_getObject()
{
    return &gA3901Obj;
}

/**
    motor mode setting

    Set MOTOR_MODE_STANDBY/MOTOR_MODE_ZOOM/MOTOR_MODE_FOCUS/MOTOR_MODE_APERTURE/MOTOR_MODE_SHUTTER to motor

    @param state - The mode state of motor.

    @return None.
*/
static void motorA3901_changeMode(MOTOR_MODE state)
{
    if(!gA3901PvtObj.bInitYet)
    {
        debug_err(("motorA3901_init not ready\n\r"));
        return;
    }
    //debug_err(("[A3901]motorA3901_mode_change, state=%d\n\r", state));
    //check if pwm control enable,we will close it anyway
    if(gA3901PvtObj.bPwmCtrl == TRUE)
    {
        motorA3901_pwm_setState(FALSE,MOTOR_ZOOM_OFF);
    }
    switch(state)
    {
        case MOTOR_MODE_STANDBY:
            motorA3901_resetInputPin();
            break;
        case MOTOR_MODE_ZOOM:
            motorA3901_resetInputPin();
            //MOTOR_PinMux_Select_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_FOCUS:
            motorA3901_resetInputPin();
//            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_APERTURE:
            motorA3901_resetInputPin();
//            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_SHUTTER:
            motorA3901_resetInputPin();
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        default:
            debug_err(("[A3901]motorA3901_changeMode,UNKNOW\n\r"));
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
static void motorA3901_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with pwm related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    PWM control handler.

    DC motor will use this process handler to set the voltage output period.

    @param None

    @return None.
*/
static void motorA3901_pwm_hdlPWM(void)
{
    if(gA3901PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    return;
}

/**
    PWM action

    Create / Delete a timer to process PWM control.

    @param state - PWM control handler create or delete
    @param pwmprevdirection - The direction of the PWM.

    @return None.
*/
static void motorA3901_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection)
{
    if(gA3901PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    debug_err(("motorA3901_pwm_action\r\n"));

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
static void motorA3901_aperture_setState(UCHAR state)
{
    if(!gA3901PvtObj.bInitYet)
    {
        debug_err(("motorA3901_init not ready\n\r"));
        return;
    }
    pinmux_disable_pwm(PINMUX_PWM5|PINMUX_PWM6);
    //debug_err(("[A3901]motorA3901_aperture_setState,state=0x%x\n\r",state));

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
static void motorA3901_shutter_setState(UCHAR state)
{
    if(!gA3901PvtObj.bInitYet)
    {
        debug_err(("motorA3901_init not ready\n\r"));
        return;
    }
    debug_err(("[A3901]motorA3901_shutter_setState,state=0x%x\n\r",state));

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

