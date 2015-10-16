/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_MD152.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "SiTI MD152"

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
    1. motorMD152_pinin_reset -> motorMD152_resetInputPin
    2. motorMD152_motorobject_get -> motorMD152_getObject
    3. motorMD152_zoomPR_check -> motorMD152_zoom_checkPR
    4. motorMD152_mode_change -> motorMD152_changeMode
    5. motorMD152_zoom_action -> motorMD152_zoom_setState
    6. motorMD152_pwm_process -> motorMD152_pwm_hdlPWM (hdlSpeed)
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
#include "LensMotor_MD152.h"

//static var declare
static MOTOR_TAB        gMd152Obj;//extern object for others
static MOTOR_PVT_OBJ    gMd152PvtObj;//private object for internal usage

//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_select_mes((sel))

//new architecture
#define MOTOR_PINDEF_MOTO_POWER          (gMd152PvtObj.uiPinPower)
#define MOTOR_PINDEF_MOTO_EN1            (gMd152PvtObj.uiPinEnable1)
#define MOTOR_PINDEF_MOTO_EN2            (gMd152PvtObj.uiPinEnable2)
#define MOTOR_PINDEF_MOTO_EN3            (gMd152PvtObj.uiPinEnable3)
#define MOTOR_PINDEF_MOTO_IN1            (gMd152PvtObj.uiPinInput1)//SN_MES0 = AGPIO20
#define MOTOR_PINDEF_MOTO_IN2            (gMd152PvtObj.uiPinInput2)//SN_MES1 = AGPIO21
#define MOTOR_PINDEF_MOTO_IN3            (gMd152PvtObj.uiPinInput3)
#define MOTOR_PINDEF_MOTO_IN4            (gMd152PvtObj.uiPinInput4)
#define MOTOR_PINDEF_IRIS_PIN0           MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_IRIS_PIN1           MOTOR_PINDEF_MOTO_IN2
#define MOTOR_PINDEF_IRIS_PIN2           MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_IRIS_PIN3           MOTOR_PINDEF_MOTO_IN4
#define MOTOR_PINDEF_FOCUS_PIN0          MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_FOCUS_PIN1          MOTOR_PINDEF_MOTO_IN2
#define MOTOR_PINDEF_FOCUS_PIN2          MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_FOCUS_PIN3          MOTOR_PINDEF_MOTO_IN4
#define MOTOR_PINDEF_ZOOM_PIN0           MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_ZOOM_PIN1           MOTOR_PINDEF_MOTO_IN4
#define MOTOR_PINDEF_ZOOM_PR             (gMd152PvtObj.uiPinZoomPR)
#define MOTOR_PINDEF_ZOOM_PI             (gMd152PvtObj.uiPinZoomPI)
#define MOTOR_PINDEF_FOCUS_PI            (gMd152PvtObj.uiPinFocusPI)
#define MOTOR_ITR_ZOOM_PI                (gMd152PvtObj.uiZoomPIINT)//dgpio[0] is for ZOOM PI
#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN2

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorMD152_init(PLENS_DEVICE_OBJ pLensDev);
static void motorMD152_power(MOTOR_POWER state);
static void motorMD152_changeMode(MOTOR_MODE state);
static void motorMD152_setPWMInfo(PMOTOR_PWM_TAB pTab);
static void motorMD152_resetInputPin(void);
static void motorMD152_pwm_hdlPWM(void);
static void motorMD152_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection);
static void motorMD152_zoom_setState(MOTOR_ZOOM_ACT state);
static void motorMD152_zoom_setPIInt(BOOL state);
static void motorMD152_zoom_clearPIInt(void);
static BOOL motorMD152_zoom_checkPIInt(void);
static BOOL motorMD152_zoom_checkPR(void);
static BOOL motorMD152_zoom_checkPI(void);
static BOOL motorMD152_focus_checkPI(void);
static void motorMD152_focus_setState(UINT32 state);
static void motorMD152_aperture_setState(UCHAR state);
static void motorMD152_shutter_setState(UCHAR state);


static MOTOR_TAB gMd152Obj =
{
    motorMD152_init,
    motorMD152_power,
    motorMD152_changeMode,
    motorMD152_setPWMInfo,
    motorMD152_zoom_setState,
    motorMD152_zoom_setPIInt,
    motorMD152_zoom_clearPIInt,
    motorMD152_zoom_checkPIInt,
    motorMD152_zoom_checkPR,
    motorMD152_zoom_checkPI,
    motorMD152_focus_checkPI,
    motorMD152_focus_setState,
    motorMD152_aperture_setState,
    motorMD152_shutter_setState
};

static MOTOR_PVT_OBJ gMd152PvtObj =
{
    FALSE,
    0,0,0,0,0,0,0,0,0,0,0,0,//pin assign

    FALSE,//pwm control
    LENS_NULL_TIMER,
    40000,//pwm period
    30000,//pwm on
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

    Init MD152 pin assignment

    @param None.

    @return None.
*/
static void motorMD152_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init already\n\r"));
        return;
    }
    LENS_ERR(("LENS_M: motorMD152_init\n\r"));

    if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_SIF)
    {
        //Rsv
    }
    else
    {
        #if 1
        gMd152PvtObj.uiPinEnable1 = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_1];
        gMd152PvtObj.uiPinEnable2 = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_2];
        gMd152PvtObj.uiPinEnable3 = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_3];
        gMd152PvtObj.uiPinInput1  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gMd152PvtObj.uiPinInput2  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        gMd152PvtObj.uiPinInput3  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_2];
        gMd152PvtObj.uiPinInput4  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_3];
        gMd152PvtObj.uiPinPower   = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_PWR];
        gMd152PvtObj.uiPinZoomPI  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI];
        gMd152PvtObj.uiPinZoomPR  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR];
        gMd152PvtObj.uiPinFocusPI = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI];
        gMd152PvtObj.uiZoomPIINT  = pLensDev->uiMotorCtrlGPIOIntrID;
        #else
        gMd152PvtObj.uiPinPower = (14|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinEnable1 = (3|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinEnable2 = (4|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinEnable3 = (5|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinInput1 = 20; //SN_MES0 = AGPIO20
        gMd152PvtObj.uiPinInput2 = 21; //SN_MES1 = AGPIO21
        gMd152PvtObj.uiPinInput3 = (6|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinInput4 = (7|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinZoomPI = (0|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinZoomPR = (2|GPIO_IS_DGPIO);
        gMd152PvtObj.uiPinFocusPI = (1|GPIO_IS_DGPIO);
        gMd152PvtObj.uiZoomPIINT = 8;  //dgpio[0] is for ZOOM PI
        #endif
    }

    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);

    gpio_setDir(MOTOR_PINDEF_MOTO_POWER, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_EN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_EN2, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_EN3, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN3, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN4, GPIO_DIR_OUTPUT);

    gpio_clearPin(MOTOR_PINDEF_MOTO_EN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_EN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_EN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);

    gpio_setDir(MOTOR_PINDEF_ZOOM_PR, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_ZOOM_PI, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_FOCUS_PI, GPIO_DIR_INPUT);

    //[Chris Chung] What does it do?
    //Ans: Pull NONE to use foreign input voltage.
    //     The pad number should follow NT96211.
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI], PAD_NONE);

    gMd152PvtObj.bInitYet = TRUE;
    return;
}

/**
    Get MD152 motor object

    @return MD152 motor object pointer

*/
PMOTOR_TAB motorMD152_getObject()
{
    return &gMd152Obj;
}

/**
    motor power setting

    Turn on/off motor power

    @param state - The power state of motor.

    @return None.
*/
static void motorMD152_power(MOTOR_POWER state)
{
    switch(state)
    {
        case MOTOR_POWER_OFF:
            //[ADPK] No power off motor driver all the time except USB mode.
            //gpio_clearPin(MOTOR_PINDEF_MOTO_POWER);
            break;
        case MOTOR_POWER_ON:
            gpio_setPin(MOTOR_PINDEF_MOTO_POWER);
            break;
        default:
            LENS_ERR(("LENS_E: motorMD152_power,UNKNOW\n\r"));
            break;
    }
    return;
}

/**
    motor mode setting

    Set MOTOR_MODE_STANDBY/MOTOR_MODE_ZOOM/MOTOR_MODE_FOCUS/MOTOR_MODE_APERTURE/MOTOR_MODE_SHUTTER to motor

    @param state - The mode state of motor.

    @return None.
*/
static void motorMD152_changeMode(MOTOR_MODE state)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }
    //debug_err(("motorMD152_mode_change, state=%d\n\r", state));
    //check if pwm control enable,we will close it anyway
    if(gMd152PvtObj.bPwmCtrl == TRUE)
    {
        motorMD152_pwm_setState(FALSE,MOTOR_ZOOM_OFF);
    }
    switch(state)
    {
        case MOTOR_MODE_STANDBY:
            motorMD152_resetInputPin();
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN1);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN2);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN3);
            break;
        case MOTOR_MODE_ZOOM:
            motorMD152_resetInputPin();
            gpio_setPin(MOTOR_PINDEF_MOTO_EN1);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN2);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN3);
            //MOTOR_PinMux_Select_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_FOCUS:
            motorMD152_resetInputPin();
            gpio_setPin(MOTOR_PINDEF_MOTO_EN1);
            gpio_setPin(MOTOR_PINDEF_MOTO_EN2);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN3);
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_APERTURE:
            motorMD152_resetInputPin();
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN1);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN2);
            gpio_setPin(MOTOR_PINDEF_MOTO_EN3);
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        case MOTOR_MODE_SHUTTER:
            motorMD152_resetInputPin();
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN1);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN2);
            gpio_setPin(MOTOR_PINDEF_MOTO_EN3);
            MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
            break;
        default:
            LENS_ERR(("LENS_E: motorMD152_changeMode,UNKNOW\n\r"));
            break;
    }
    return;
}

/**
    motor pwm setting

    Set MOTOR_PWM_TAB info to motor

    @param pTab - The MOTOR_PWM_TAB pointer.

    @return None.
*/
static void motorMD152_setPWMInfo(PMOTOR_PWM_TAB pTab)
{
    gMd152PvtObj.bPwmCtrl           = pTab->bPwmCtrl;
    gMd152PvtObj.uiPwmPeriodTime    = pTab->uiPwmPeriodTime;
    gMd152PvtObj.uiPwmOnTime        = pTab->uiPwmOnTime;
}

/**
    Reset input pin of motor

    Reset input pin of motor.

    @param None

    @return None.
*/
static void motorMD152_resetInputPin(void)
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
static void motorMD152_pwm_hdlPWM(void)
{
    if(gMd152PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) > 0 &&
       gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) == 0)
    {
        //current is forward
        timer_set_us(gMd152PvtObj.uiPwmTimerID,
                     (gMd152PvtObj.uiPwmPeriodTime - gMd152PvtObj.uiPwmOnTime),
                     _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                     _TIMER_PLAY);

        motorMD152_zoom_setState(MOTOR_ZOOM_OFF);
    }
    else if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) == 0 &&
            gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) > 0)
    {
        //current is backward
        timer_set_us(gMd152PvtObj.uiPwmTimerID,
                     (gMd152PvtObj.uiPwmPeriodTime - gMd152PvtObj.uiPwmOnTime),
                     _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                     _TIMER_PLAY);

        motorMD152_zoom_setState(MOTOR_ZOOM_OFF);
    }
    else if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) == 0 &&
            gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) == 0)
    {
        //current is off
        timer_set_us(gMd152PvtObj.uiPwmTimerID,
                     gMd152PvtObj.uiPwmOnTime,
                     _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                     _TIMER_PLAY);

        if(gMd152PvtObj.pwmprevdirection == MOTOR_ZOOM_FWD)
        {
            motorMD152_zoom_setState(MOTOR_ZOOM_FWD);
        }
        else if(gMd152PvtObj.pwmprevdirection == MOTOR_ZOOM_BWD)
        {
            motorMD152_zoom_setState(MOTOR_ZOOM_BWD);
        }
        else
        {
            LENS_ERR(("LENS_E: motorMD152_pwm_hdlPWM error case !!\n\r"));
        }
    }
}

/**
    PWM action

    Create / Delete a timer to process PWM control.

    @param state - PWM control handler create or delete
    @param pwmprevdirection - The direction of the PWM.

    @return None.
*/
static void motorMD152_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection)
{
    if(gMd152PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    switch(state)
    {
        case FALSE:
            if(gMd152PvtObj.uiPwmTimerID != LENS_NULL_TIMER)
            {
                LENS_MSG(("LENS_M: motorMD152_pwm_action: close TimerId=%d\r\n", gMd152PvtObj.uiPwmTimerID));
                timer_close(gMd152PvtObj.uiPwmTimerID);
                gMd152PvtObj.uiPwmTimerID = LENS_NULL_TIMER;
                gMd152PvtObj.pwmprevdirection = pwmprevdirection;
            }
            break;
        case TRUE:
            if(gMd152PvtObj.uiPwmTimerID != LENS_NULL_TIMER)
            {
                gMd152PvtObj.pwmprevdirection = pwmprevdirection;
                LENS_MSG(("LENS_M: motorMD152_pwm_action already open\r\n"));
                return;
            }

            if (timer_open(&gMd152PvtObj.uiPwmTimerID, motorMD152_pwm_hdlPWM) == 0)
            {
                gMd152PvtObj.pwmprevdirection = pwmprevdirection;
                timer_set_us(gMd152PvtObj.uiPwmTimerID, gMd152PvtObj.uiPwmOnTime, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
            }
            else
            {
                LENS_ERR(("LENS_E: motorMD152_pwm_action fail\r\n"));
                gMd152PvtObj.uiPwmTimerID = LENS_NULL_TIMER;
            }
            break;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    Zoom PI check

    Check zoom PI status

    @param None.

    @return None.
*/
static BOOL motorMD152_zoom_checkPI(void)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return FALSE;
    }

    return gpio_getPin(MOTOR_PINDEF_ZOOM_PI);
}

/**
    Zoom PI interrupt check

    Check zoom PI interrupt status

    @param None.

    @return BOOL - The zoom PI interrupt status.
*/
static BOOL motorMD152_zoom_checkPIInt(void)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return FALSE;
    }

    return gpio_getIntStatus(MOTOR_ITR_ZOOM_PI);
}


/**
    Zoom PR check

    Check zoom PR status

    @param None.

    @return BOOL - The zoom PR status.
*/
static BOOL motorMD152_zoom_checkPR(void)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return FALSE;
    }

    return gpio_getPin(MOTOR_PINDEF_ZOOM_PR);
}

/**
    Zoom PI interrupt status clear

    clear zoom PI interrupt status

    @param None.

    @return None.
*/
static void motorMD152_zoom_clearPIInt(void)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }

    gpio_clearIntStatus(MOTOR_ITR_ZOOM_PI);
}

/**
    Zoom PI interrupt setting

    Set zoom PI interrupt to DSP

    @param state - The zoom PI interrupt state.

    @return None.
*/
static void motorMD152_zoom_setPIInt(BOOL state)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }

    if(state)
        gpio_enableInt(MOTOR_ITR_ZOOM_PI);
    else
        gpio_disableInt(MOTOR_ITR_ZOOM_PI);
    return;
}

/**
    motor zoom mode action

    Set MOTOR_ZOOM_BRAKE/MOTOR_ZOOM_FWD/MOTOR_ZOOM_BWD/MOTOR_ZOOM_OFF to motor in Zoom mode

    @param state - The action state of zoom mode.

    @return None.
*/
static void motorMD152_zoom_setState(MOTOR_ZOOM_ACT state)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }

    //debug_err(("motorMD152_zoom_action, state=%d\n\r", state));
    switch(state)
    {
        case MOTOR_ZOOM_BRAKE:
            //check if pwm control enable
            if(gMd152PvtObj.bPwmCtrl == TRUE)
            {
                if(gMd152PvtObj.uiPwmTimerID != LENS_NULL_TIMER)
                {
                    timer_close(gMd152PvtObj.uiPwmTimerID);
                    gMd152PvtObj.uiPwmTimerID = LENS_NULL_TIMER;
                }
            }
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN1);
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN0);
            break;
        case MOTOR_ZOOM_FWD:
            //check if pwm control enable
            if(gMd152PvtObj.bPwmCtrl == TRUE)
            {
                motorMD152_pwm_setState(TRUE,state);
            }
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN0);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            break;
        case MOTOR_ZOOM_BWD:
            //check if pwm control enable
            if(gMd152PvtObj.bPwmCtrl == TRUE)
            {
                motorMD152_pwm_setState(TRUE,state);
            }
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN1);
            break;
        case MOTOR_ZOOM_OFF:
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            break;
        default:
            LENS_ERR(("LENS_E: motorMD152_zoom_setState,UNKNOW\n\r"));
            break;
    }
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with focus related API.
//--------------------------------------------------------------------------------------------------------------------------

/**
    Focus PI check

    Check focus PI status

    @param None.

    @return BOOL - The focus PI status.
*/
static BOOL motorMD152_focus_checkPI(void)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return FALSE;
    }

    return gpio_getPin(MOTOR_PINDEF_FOCUS_PI);
}

/**
    motor focus mode action

    Set phase to motor in focus mode

    @param state - The phase state of focus mode.

    @return None.
*/
static void motorMD152_focus_setState(UINT32 state)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_FOCUS_PIN0);
    else
        gpio_clearPin(MOTOR_PINDEF_FOCUS_PIN0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_FOCUS_PIN1);
    else
        gpio_clearPin(MOTOR_PINDEF_FOCUS_PIN1);

    if(state & 0x4)
        gpio_setPin(MOTOR_PINDEF_FOCUS_PIN2);
    else
        gpio_clearPin(MOTOR_PINDEF_FOCUS_PIN2);

    if(state & 0x8)
        gpio_setPin(MOTOR_PINDEF_FOCUS_PIN3);
    else
        gpio_clearPin(MOTOR_PINDEF_FOCUS_PIN3);
    return;
}

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
static void motorMD152_aperture_setState(UCHAR state)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN0);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN1);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN1);

    if(state & 0x4)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN2);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN2);

    if(state & 0x8)
        gpio_setPin(MOTOR_PINDEF_IRIS_PIN3);
    else
        gpio_clearPin(MOTOR_PINDEF_IRIS_PIN3);
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
static void motorMD152_shutter_setState(UCHAR state)
{
    if(!gMd152PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorMD152_init not ready\n\r"));
        return;
    }

    if(state & 0x1)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES0);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES0);

    if(state & 0x2)
        gpio_setPin(MOTOR_PINDEF_SHUTTER_MES1);
    else
        gpio_clearPin(MOTOR_PINDEF_SHUTTER_MES1);

    return;
}


