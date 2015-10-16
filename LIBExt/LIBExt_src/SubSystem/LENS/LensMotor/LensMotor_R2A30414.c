/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_R2A30414.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "SiTI R2A30414"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/06/16
*/
#include "OSCommon.h"
#include "PWM.h"
#include "SIF.h"
#include "GPIO.h"
#include "pad.h"
#include "top.h"
#include "Timer.h"
#include "Lens.h"
#include "LensMotor_R2A30414.h"
#include "LensMotor_Dummy.h"

//static var declare
static MOTOR_TAB        gR2A30414Obj;//extern object for others
static MOTOR_PVT_OBJ    gR2A30414PvtObj;//private object for internal usage

//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_select_mes((sel))

//new architecture
#define MOTOR_PINDEF_MOTO_POWER          (gR2A30414PvtObj.uiPinPower)
#define MOTOR_PINDEF_MOTO_IN1            (gR2A30414PvtObj.uiPinInput1)
#define MOTOR_PINDEF_MOTO_IN2            (gR2A30414PvtObj.uiPinInput2)
#define MOTOR_PINDEF_MOTO_IN3            (gR2A30414PvtObj.uiPinInput3)
#define MOTOR_PINDEF_MOTO_IN4            (gR2A30414PvtObj.uiPinInput4)
#define MOTOR_PINDEF_MOTO_IN5            (gR2A30414PvtObj.uiPinInput5)
#define MOTOR_PINDEF_MOTO_IN6            (gR2A30414PvtObj.uiPinInput6)

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
#define MOTOR_PINDEF_ZOOM_PR             (gR2A30414PvtObj.uiPinZoomPR)
#define MOTOR_PINDEF_ZOOM_PI             (gR2A30414PvtObj.uiPinZoomPI)
#define MOTOR_PINDEF_FOCUS_PI            (gR2A30414PvtObj.uiPinFocusPI)
#define MOTOR_ITR_ZOOM_PI                (gR2A30414PvtObj.uiZoomPIINT)//dgpio[0] is for ZOOM PI
#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN2

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorR2A30414_init(PLENS_DEVICE_OBJ pLensDev);
static void motorR2A30414_power(MOTOR_POWER state);
static void motorR2A30414_changeMode(MOTOR_MODE state);
static void motorR2A30414_resetInputPin(void);
static void motorR2A30414_pwm_hdlPWM(void);
static void motorR2A30414_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection);
static void motorR2A30414_zoom_setState(MOTOR_ZOOM_ACT state);
static void motorR2A30414_zoom_setPIInt(BOOL state);
static void motorR2A30414_zoom_clearPIInt(void);
static BOOL motorR2A30414_zoom_checkPIInt(void);
static BOOL motorR2A30414_zoom_checkPR(void);
static BOOL motorR2A30414_zoom_checkPI(void);
static BOOL motorR2A30414_focus_checkPI(void);
static void motorR2A30414_focus_setState(UINT32 state);
static void motorR2A30414_aperture_setState(UCHAR state);
static void motorR2A30414_shutter_setState(UCHAR state);
static void motorR2A30414_sif_init();
static void motorR2A30414_sif_WrReg(UINT32 addr,UINT32 reg,UINT32 val);
static void motorR2A30414_channel_init();

static MOTOR_TAB gR2A30414Obj =
{
    motorR2A30414_init,
    motorR2A30414_power,
    motorR2A30414_changeMode,
    motorDUMMY_setPWMInfo,
    motorR2A30414_zoom_setState,
    motorR2A30414_zoom_setPIInt,
    motorR2A30414_zoom_clearPIInt,
    motorR2A30414_zoom_checkPIInt,
    motorR2A30414_zoom_checkPR,
    motorR2A30414_zoom_checkPI,
    motorR2A30414_focus_checkPI,
    motorR2A30414_focus_setState,
    motorR2A30414_aperture_setState,
    motorR2A30414_shutter_setState
};

static MOTOR_PVT_OBJ gR2A30414PvtObj =
{
    //GPIO related var
    FALSE,//BOOL          bInitYet;
    0,//UINT32        uiPinPower;
    0,//UINT32        uiPinInput1;
    0,//UINT32        uiPinInput2;
    0,//UINT32        uiPinInput3;
    0,//UINT32        uiPinInput4;
    0,//UINT32        uiPinInput5;
    0,//UINT32        uiPinInput6;

    0,//UINT32        uiPinZoomPI;
    0,//UINT32        uiPinZoomPR;
    0,//UINT32        uiPinFocusPI;
    0,//UINT32        uiZoomPIINT;

    //SIF,R2A30414 usage
    PINMUX_SIF_CH0,//SIF_CH        uiSIFChannel;

    //pwm control related var
    FALSE,//BOOL          bPwmCtrl;
    LENS_NULL_TIMER,//UINT          uiPwmTimerID;
    10000,//UINT32        uiPwmonTime;
    30000,//UINT32        uiPwmoffTime;
    MOTOR_ZOOM_OFF,//MOTOR_ZOOM_ACT        pwmprevdirection;
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with common related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor init

    Init R2A30414 pin assignment

    @param None.

    @return None.
*/
static void motorR2A30414_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init already\n\r"));
        return;
    }
    debug_err(("motorR2A30414_init\n\r"));

    if((pLensDev->uiMotorCtrlMode & MOTOR_CTRL_SIF) == MOTOR_CTRL_SIF)
    {
        //Resv,init sif channel mode!!
        gR2A30414PvtObj.uiSIFChannel = pLensDev->LensSIFPinObj.uiSIFChannel;
        motorR2A30414_sif_init();
        motorR2A30414_channel_init();
    }

    if((pLensDev->uiMotorCtrlMode & MOTOR_CTRL_GPIO) == MOTOR_CTRL_GPIO)
    {
        gR2A30414PvtObj.uiPinInput1  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gR2A30414PvtObj.uiPinInput2  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        gR2A30414PvtObj.uiPinInput3  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_2];
        gR2A30414PvtObj.uiPinInput4  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_3];
        gR2A30414PvtObj.uiPinInput5  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_4];
        gR2A30414PvtObj.uiPinInput6  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_5];
        gR2A30414PvtObj.uiPinPower   = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_PWR];
        gR2A30414PvtObj.uiPinZoomPI  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI];
        gR2A30414PvtObj.uiPinZoomPR  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR];
        gR2A30414PvtObj.uiPinFocusPI = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI];
        gR2A30414PvtObj.uiZoomPIINT  = pLensDev->uiMotorCtrlGPIOIntrID;
    }

    //MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);

    gpio_setDir(MOTOR_PINDEF_MOTO_POWER, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN1, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN3, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN4, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN5, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN6, GPIO_DIR_OUTPUT);

    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN5);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN6);

    gpio_setDir(MOTOR_PINDEF_ZOOM_PR, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_ZOOM_PI, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_FOCUS_PI, GPIO_DIR_INPUT);

    //[Chris Chung] What does it do?
    //Ans: Pull NONE to use foreign input voltage.
    //     The pad number should follow NT96211.
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI], PAD_NONE);

    gR2A30414PvtObj.bInitYet = TRUE;
    return;
}

/**
    Get R2A30414 motor object

    @return R2A30414 motor object pointer

*/
PMOTOR_TAB motorR2A30414_getObject()
{
    debug_err(("call motorR2A30414_getObject\r\n"));
    return &gR2A30414Obj;
}

/**
    motor power setting

    Turn on/off motor power

    @param state - The power state of motor.

    @return None.
*/
static void motorR2A30414_power(MOTOR_POWER state)
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;

    switch(state)
    {
        case MOTOR_POWER_OFF:
            addr = Address_common_Sel;
            reg = Rcomm_PSCtrl;
            val = ((0x0 << 9) | //Power Off
                   (0x0 << 8) | //PS12
                   (0x0 << 7) | //PS3
                   (0x0 << 6)  //PS4
                  );
            motorR2A30414_sif_WrReg(addr,reg,val);
            //gpio_clearPin(MOTOR_PINDEF_MOTO_POWER);
            break;
        case MOTOR_POWER_ON:
            addr = Address_common_Sel;
            reg = Rcomm_PSCtrl;
            val = ((0x1 << 9) | //Power On
                   (0x1 << 8) | //PS12
                   (0x1 << 7) | //PS3
                   (0x1 << 6)  //PS4
                  );
            motorR2A30414_sif_WrReg(addr,reg,val);
            //gpio_setPin(MOTOR_PINDEF_MOTO_POWER);
            break;
        default:
            debug_err(("[R2A30414]motorR2A30414_power,UNKNOW\n\r"));
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
static void motorR2A30414_changeMode(MOTOR_MODE state)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return;
    }
    //debug_err(("motorR2A30414_mode_change, state=%d\n\r", state));
    //check if pwm control enable,we will close it anyway
    if(gR2A30414PvtObj.bPwmCtrl == TRUE)
    {
        motorR2A30414_pwm_setState(FALSE,MOTOR_ZOOM_OFF);
    }
    switch(state)
    {
        case MOTOR_MODE_STANDBY:
            motorR2A30414_resetInputPin();
            break;
        case MOTOR_MODE_ZOOM:
            motorR2A30414_resetInputPin();
            break;
        case MOTOR_MODE_FOCUS:
            motorR2A30414_resetInputPin();
            break;
        case MOTOR_MODE_APERTURE:
            motorR2A30414_resetInputPin();
            break;
        case MOTOR_MODE_SHUTTER:
            motorR2A30414_resetInputPin();
            break;
        default:
            debug_err(("[R2A30414]motorR2A30414_changeMode,UNKNOW\n\r"));
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
static void motorR2A30414_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN5);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN6);
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
static void motorR2A30414_pwm_hdlPWM(void)
{
    if(gR2A30414PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) > 0 &&
       gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) == 0)
    {
        //current is forward
        timer_set_us(gR2A30414PvtObj.uiPwmTimerID, gR2A30414PvtObj.uiPwmoffTime, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

        motorR2A30414_zoom_setState(MOTOR_ZOOM_OFF);
    }
    else if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) == 0 &&
            gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) > 0)
    {
        //current is backward
        timer_set_us(gR2A30414PvtObj.uiPwmTimerID, gR2A30414PvtObj.uiPwmoffTime, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

        motorR2A30414_zoom_setState(MOTOR_ZOOM_OFF);
    }
    else if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) > 0 &&
            gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) > 0)
    {
        //current is brake
        timer_set_us(gR2A30414PvtObj.uiPwmTimerID, gR2A30414PvtObj.uiPwmoffTime, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

        motorR2A30414_zoom_setState(MOTOR_ZOOM_OFF);
    }
    else if(gpio_getPin(MOTOR_PINDEF_ZOOM_PIN0) == 0 &&
            gpio_getPin(MOTOR_PINDEF_ZOOM_PIN1) == 0)
    {
        //current is off
        timer_set_us(gR2A30414PvtObj.uiPwmTimerID, gR2A30414PvtObj.uiPwmonTime, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);

        if(gR2A30414PvtObj.pwmprevdirection == MOTOR_ZOOM_FWD)
        {
            motorR2A30414_zoom_setState(MOTOR_ZOOM_FWD);
        }
        else if(gR2A30414PvtObj.pwmprevdirection == MOTOR_ZOOM_BWD)
        {
            motorR2A30414_zoom_setState(MOTOR_ZOOM_BWD);
        }
        else if(gR2A30414PvtObj.pwmprevdirection == MOTOR_ZOOM_BRAKE)
        {
            motorR2A30414_zoom_setState(MOTOR_ZOOM_BRAKE);
        }
        else
        {
            debug_err(("<Lens_ZoomHandler> error case !!\n\r"));
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
static void motorR2A30414_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection)
{
    if(gR2A30414PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    debug_err(("motorR2A30414_pwm_action\r\n"));

    switch(state)
    {
        case FALSE:
            if(gR2A30414PvtObj.uiPwmTimerID != LENS_NULL_TIMER)
            {
                debug_err(("motorR2A30414_pwm_action: close TimerId=%d\r\n", gR2A30414PvtObj.uiPwmTimerID));
                timer_close(gR2A30414PvtObj.uiPwmTimerID);
                gR2A30414PvtObj.uiPwmTimerID = LENS_NULL_TIMER;
                gR2A30414PvtObj.pwmprevdirection = pwmprevdirection;
            }
            break;
        case TRUE:
            if(gR2A30414PvtObj.uiPwmTimerID != LENS_NULL_TIMER)
            {
                gR2A30414PvtObj.pwmprevdirection = pwmprevdirection;
                debug_err(("motorR2A30414_pwm_action already open\r\n"));
                return;
            }

            if (timer_open(&gR2A30414PvtObj.uiPwmTimerID, motorR2A30414_pwm_hdlPWM) == 0)
            {
                gR2A30414PvtObj.pwmprevdirection = pwmprevdirection;
                timer_set_us(gR2A30414PvtObj.uiPwmTimerID, gR2A30414PvtObj.uiPwmonTime, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
            }
            else
            {
                debug_err(("motorR2A30414_pwm_action fail\r\n"));
                gR2A30414PvtObj.uiPwmTimerID = LENS_NULL_TIMER;
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
static BOOL motorR2A30414_zoom_checkPI(void)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return FALSE;
    }
    debug_err(("motorR2A30414_zoomPI_check\n\r"));
    return gpio_getPin(MOTOR_PINDEF_ZOOM_PI);
}

/**
    Zoom PI interrupt check

    Check zoom PI interrupt status

    @param None.

    @return BOOL - The zoom PI interrupt status.
*/
static BOOL motorR2A30414_zoom_checkPIInt(void)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
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
static BOOL motorR2A30414_zoom_checkPR(void)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return FALSE;
    }
    //debug_err(("motorR2A30414_zoomPR_check\n\r"));
    return gpio_getPin(MOTOR_PINDEF_ZOOM_PR);
}

/**
    Zoom PI interrupt status clear

    clear zoom PI interrupt status

    @param None.

    @return None.
*/
static void motorR2A30414_zoom_clearPIInt(void)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
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
static void motorR2A30414_zoom_setPIInt(BOOL state)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return;
    }
    debug_err(("motorR2A30414_zoomPIInt_set\n\r"));
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
static void motorR2A30414_zoom_setState(MOTOR_ZOOM_ACT state)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return;
    }

    //debug_err(("motorR2A30414_zoom_action, state=%d\n\r", state));
    switch(state)
    {
        case MOTOR_ZOOM_BRAKE:
            //check if pwm control enable
            if(gR2A30414PvtObj.bPwmCtrl == TRUE)
            {
                motorR2A30414_pwm_setState(TRUE,state);
            }
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN1);
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN0);
            break;
        case MOTOR_ZOOM_FWD:
            //check if pwm control enable
            if(gR2A30414PvtObj.bPwmCtrl == TRUE)
            {
                motorR2A30414_pwm_setState(TRUE,state);
            }
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN0);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            break;
        case MOTOR_ZOOM_BWD:
            //check if pwm control enable
            if(gR2A30414PvtObj.bPwmCtrl == TRUE)
            {
                motorR2A30414_pwm_setState(TRUE,state);
            }
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN1);
            break;
        case MOTOR_ZOOM_OFF:
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            break;
        default:
            debug_err(("[R2A30414]motorR2A30414_zoom_setState,UNKNOW\n\r"));
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
static BOOL motorR2A30414_focus_checkPI(void)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return FALSE;
    }
    //debug_err(("motorR2A30414_focusPI_check\n\r"));
    return gpio_getPin(MOTOR_PINDEF_FOCUS_PI);
}

/**
    motor focus mode action

    Set phase to motor in focus mode

    @param state - The phase state of focus mode.

    @return None.
*/
static void motorR2A30414_focus_setState(UINT32 state)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return;
    }

    //debug_err(("motorR2A30414_focus_action\n\r"));
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
static void motorR2A30414_aperture_setState(UCHAR state)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return;
    }

    //debug_err(("motorR2A30414_aperture_action\n\r"));
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
static void motorR2A30414_shutter_setState(UCHAR state)
{
    if(!gR2A30414PvtObj.bInitYet)
    {
        debug_err(("motorR2A30414_init not ready\n\r"));
        return;
    }

    debug_err(("motorR2A30414_shutter_action\n\r"));
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with SIF related API.
//--------------------------------------------------------------------------------------------------------------------------
static void motorR2A30414_sif_init()
{
    SIF_CTRL    SIFCtrl;
    SIF_CH      sif_ch;

    sif_ch = gR2A30414PvtObj.uiSIFChannel;

    switch(sif_ch)
    {
    case SIF_CH0:
        pinmux_enable_sif(PINMUX_SIF_CH0);
        break;
    case SIF_CH1:
        pinmux_enable_sif(PINMUX_SIF_CH1);
        break;
    case SIF_CH2:
        pinmux_enable_sif(PINMUX_SIF_CH2);
        break;
    case SIF_CH3:
        pinmux_enable_sif(PINMUX_SIF_CH3);
        break;
    default:
        pinmux_enable_sif(PINMUX_SIF_CH0);
        break;
    }
    sif_open();
    // Setup SIF channel parameter
    SIFCtrl.uiMode          = SIF_MODE_0010;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 16;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = TRUE;
    sif_setControl(sif_ch, &SIFCtrl);
}
static void motorR2A30414_sif_WrReg(UINT32 addr,UINT32 reg,UINT32 val)
{
    UINT32 uiSIFData = 0;
    //MSB
    uiSIFData = ((val & 0x3ff) << VALUE_SHIFT_OFFSET);//10 bit value
    uiSIFData |= ((reg & 0xf) << REGISTER_SHIFT_OFFSET);//4 bit register
    uiSIFData |= ((addr & 0x3) << ADDRESS_SHIFT_OFFSET);//2 bit address
    uiSIFData = (uiSIFData << 16);
    sif_sendMSB(gR2A30414PvtObj.uiSIFChannel, uiSIFData);
}

static void motorR2A30414_channel_init()
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;

//channel 1 and channel 2(IRIS/SHUTTER)
    addr = Address_12ch_Sel;
    reg = R12ch_Mode;
    val = ((0x0 << 9) | //Forward
           (0x1 << 6) | //1-2 Phase
           (0x0 << 4) | //no acc/dec
           (0x0 << 1) | //phase position
           (0x1 << 0)   //enable
          );
    motorR2A30414_sif_WrReg(addr,reg,val);

//channel 3 and channel 4(AF)
    addr = Address_34ch_Sel;
    reg = R34ch_Mode;
    val = ((0x0 << 9) | //Forward
           (0x1 << 6) | //1-2 Phase
           (0x0 << 4) | //no acc/dec
           (0x0 << 1) | //phase position
           (0x1 << 0)   //enable
          );
    motorR2A30414_sif_WrReg(addr,reg,val);
//channel 6(ZOOM)
    addr = Address_56ch_Sel;
    reg = R56ch_Mode;
    val = 0;
    motorR2A30414_sif_WrReg(addr,reg,val);
//channel 7(ZOOM)

}

