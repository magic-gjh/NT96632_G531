/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       LensMotor_R2A30423.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "Renesas R2A30423"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/09/25
*/
#include "OSCommon.h"
#include "pwm.h"
#include "sif.h"
#include "gpio.h"
#include "pad.h"
#include "top.h"
#include "pll.h"
#include "timer.h"
#include "Lens_int.h"
#include "LensMotor_R2A30423.h"
#include "LensMotor_Dummy.h"

//static var declare
static MOTOR_TAB        gR2A30423Obj;//extern object for others
static MOTOR_PVT_OBJ    gR2A30423PvtObj;//private object for internal usage

//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_enable_mes((sel))
#define MOTOR_PINMUX_SELECT_SPCLK(sel) pinmux_select_spclk((sel))

//new architecture
#define MOTOR_PINDEF_MOTO_POWER          (gR2A30423PvtObj.uiPinPower)
#define MOTOR_PINDEF_MOTO_CLOCK          (gR2A30423PvtObj.uiPinClock)
#define MOTOR_PINDEF_MOTO_RESET          (gR2A30423PvtObj.uiPinReset)
#define MOTOR_PINDEF_MOTO_IN1            (gR2A30423PvtObj.uiPinInput1)
#define MOTOR_PINDEF_MOTO_IN2            (gR2A30423PvtObj.uiPinInput2)
#define MOTOR_PINDEF_MOTO_IN3            (gR2A30423PvtObj.uiPinInput3)
#define MOTOR_PINDEF_MOTO_IN4            (gR2A30423PvtObj.uiPinInput4)
#define MOTOR_PINDEF_MOTO_IN5            (gR2A30423PvtObj.uiPinInput5)
#define MOTOR_PINDEF_MOTO_IN6            (gR2A30423PvtObj.uiPinInput6)

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
#define MOTOR_PINDEF_ZOOM_PR             (gR2A30423PvtObj.uiPinZoomPR)
#define MOTOR_PINDEF_ZOOM_PI             (gR2A30423PvtObj.uiPinZoomPI)
#define MOTOR_PINDEF_FOCUS_PI            (gR2A30423PvtObj.uiPinFocusPI)
#define MOTOR_ITR_ZOOM_PI                (gR2A30423PvtObj.uiZoomPIINT)//dgpio[0] is for ZOOM PI
#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN2
#define MOTOR_PINDEF_MOTO_EXT1           (gR2A30423PvtObj.uiMotExt1)
#define MOTOR_PINDEF_MOTO_EXT2           (gR2A30423PvtObj.uiMotExt2)
#define MOTOR_PINDEF_MOTO_MOB1           (gR2A30423PvtObj.uiMotMOB1)
#define MOTOR_PINDEF_MOTO_MOB2           (gR2A30423PvtObj.uiMotMOB2)

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorR2A30423_init(PLENS_DEVICE_OBJ pLensDev);
static void motorR2A30423_power(MOTOR_POWER state);
static void motorR2A30423_changeMode(MOTOR_MODE state);
static void motorR2A30423_setMotorInfo(PMOTOR_INFO_TAB pTab);
static void motorR2A30423_resetInputPin(void);
static void motorR2A30423_resetMotor(void);
static void motorR2A30423_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1);
static void motorR2A30423_zoom_setPIInt(BOOL state);
static void motorR2A30423_zoom_clearPIInt(void);
static BOOL motorR2A30423_zoom_checkPIInt(void);
static BOOL motorR2A30423_zoom_checkPR(void);
static BOOL motorR2A30423_zoom_checkPI(void);
static BOOL motorR2A30423_focus_checkPI(void);
static void motorR2A30423_focus_setState(UINT32 state, UINT32 param1);
static void motorR2A30423_aperture_setState(UCHAR state);
static void motorR2A30423_shutter_setState(UCHAR state);
static void motorR2A30423_sif_init(void);
static void motorR2A30423_sif_WrReg(UINT32 addr,UINT32 reg,UINT32 val);
static void motorR2A30423_sif_focus(BOOL bForward, UINT32 step);
static void motorR2A30423_sif_zoom(BOOL bForward, UINT32 step);
static void motorR2A30423_sif_zoom_stop(void);

static MOTOR_TAB gR2A30423Obj =
{
    motorR2A30423_init,
    motorR2A30423_power,
    motorR2A30423_changeMode,
    motorR2A30423_setMotorInfo,
    motorR2A30423_zoom_setState,
    motorR2A30423_zoom_setPIInt,
    motorR2A30423_zoom_clearPIInt,
    motorR2A30423_zoom_checkPIInt,
    motorR2A30423_zoom_checkPR,
    motorR2A30423_zoom_checkPI,
    motorR2A30423_focus_checkPI,
    motorR2A30423_focus_setState,
    motorR2A30423_aperture_setState,
    motorR2A30423_shutter_setState
};

static MOTOR_PVT_OBJ gR2A30423PvtObj =
{
    //GPIO related var
    FALSE,//BOOL          bInitYet;
    0,//UINT32        uiPinPower;
    0,//UINT32        uiPinClock;
    0,//UINT32        uiPinReset;
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

    0,//UINT32        uiMotExt1;
    0,//UINT32        uiMotExt2;
    0,//UINT32        uiMotMOB1;
    0,//UINT32        uiMotMOB2;

    //SIF,R2A30423 usage
    LENS_SIF_CONTROLLER,//UINT32        uiSIFType;
    SIF_CH0,//SIF_CH        uiSIFChannel;
    0,//UINT32        uiGPIOSen;
    0,//UINT32        uiGPIOClk;
    0,//UINT32        uiGPIOData;

    //Other info
    TRUE//BOOL          bWaitZoomFinish;
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with common related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor init

    Init R2A30423 pin assignment

    @param None.

    @return None.
*/
static void motorR2A30423_init(PLENS_DEVICE_OBJ pLensDev)
{
    PWM_CFG     PWMInfo;
    UINT32      err, id = PWMID_0;

    if(gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init already\n\r"));
        return;
    }
    LENS_ERR(("LENS_M: motorR2A30423_init\n\r"));

    if((pLensDev->uiMotorCtrlMode & MOTOR_CTRL_SIF) == MOTOR_CTRL_SIF)
    {
        gR2A30423PvtObj.uiSIFType = pLensDev->LensSIFPinObj.uiSIFType;
        //Resv,init sif channel mode!!
        if(pLensDev->LensSIFPinObj.uiSIFType != LENS_SIF_GPIOSIM)
        {
            gR2A30423PvtObj.uiSIFChannel = pLensDev->LensSIFPinObj.uiSIFChannel;
            motorR2A30423_sif_init();
        }
        else
        {
            gR2A30423PvtObj.uiGPIOSen = pLensDev->LensSIFPinObj.uiGPIOSen;
            gR2A30423PvtObj.uiGPIOClk = pLensDev->LensSIFPinObj.uiGPIOClk;
            gR2A30423PvtObj.uiGPIOData = pLensDev->LensSIFPinObj.uiGPIOData;

            LENS_ERR(("%d %d %d\n\r",pLensDev->LensSIFPinObj.uiGPIOSen,
                                     pLensDev->LensSIFPinObj.uiGPIOClk,
                                     pLensDev->LensSIFPinObj.uiGPIOData));

            gpio_setDir(gR2A30423PvtObj.uiGPIOSen , GPIO_DIR_OUTPUT);
            gpio_setDir(gR2A30423PvtObj.uiGPIOClk, GPIO_DIR_OUTPUT);
            gpio_setDir(gR2A30423PvtObj.uiGPIOData, GPIO_DIR_OUTPUT);

            gpio_clearPin(gR2A30423PvtObj.uiGPIOSen);
            gpio_clearPin(gR2A30423PvtObj.uiGPIOData);
            gpio_setPin(gR2A30423PvtObj.uiGPIOClk);
        }
    }

    if((pLensDev->uiMotorCtrlMode & MOTOR_CTRL_GPIO) == MOTOR_CTRL_GPIO)
    {
        gR2A30423PvtObj.uiPinPower      = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_PWR];
        gR2A30423PvtObj.uiPinClock      = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_1];
        gR2A30423PvtObj.uiPinReset      = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_2];
        gR2A30423PvtObj.uiPinZoomPR     = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR];
        gR2A30423PvtObj.uiPinFocusPI    = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI];
        gR2A30423PvtObj.uiMotExt1       = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MOT_EXT1];
        gR2A30423PvtObj.uiMotExt2       = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MOT_EXT2];
        gR2A30423PvtObj.uiMotMOB1       = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MOT_MOB1];
        gR2A30423PvtObj.uiMotMOB2       = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MOT_MOB2];

        gR2A30423PvtObj.uiZoomPIINT     = pLensDev->uiMotorCtrlGPIOIntrID;
    }

    //MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);
    gpio_setDir(MOTOR_PINDEF_MOTO_POWER, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_CLOCK, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_RESET, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_ZOOM_PR, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_FOCUS_PI, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_EXT1, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_EXT2, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_MOB1, GPIO_DIR_INPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_MOB2, GPIO_DIR_INPUT);
    //[Chris Chung] What does it do?
    //Ans: Pull NONE to use foreign input voltage.
    //     The pad number should follow NT96xxx spec.
    //pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR], PAD_NONE);
    pad_setPullUpDown(pLensDev->puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI], PAD_NONE);

    gR2A30423PvtObj.bInitYet = TRUE;
    return;
}

/**
    Get R2A30423 motor object

    @return R2A30423 motor object pointer

*/
PMOTOR_TAB motorR2A30423_getObject(void)
{
    return &gR2A30423Obj;
}

/**
    motor power setting

    Turn on/off motor power

    @param state - The power state of motor.

    @return None.
*/
static void motorR2A30423_power(MOTOR_POWER state)
{
    switch(state)
    {
        case MOTOR_POWER_OFF:
            break;
        case MOTOR_POWER_ON:
            if(gpio_getPin(MOTOR_PINDEF_MOTO_POWER) == 0)
            {
                gpio_setPin(MOTOR_PINDEF_MOTO_POWER);
#if 0
                MOTOR_PINMUX_SELECT_SPCLK(PINMUX_SP_CLK);

                pll_setClockRate(PLL_CLKSEL_SP_CLKDIV, PLL_SP_CLKDIV(47)); //5Mhz

                pll_enableClock(SP_CLK);
#else
                gpio_setPin(MOTOR_PINDEF_MOTO_CLOCK);
#endif
            }
            break;
        default:
            LENS_ERR(("LENS_E: motorR2A30423_power,UNKNOW\n\r"));
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
static void motorR2A30423_changeMode(MOTOR_MODE state)
{
    static MOTOR_MODE lastState = MOTOR_MODE_STANDBY;

    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return;
    }

    LENS_MSG(("LENS_M: %s: state=%d\n\r", __func__, state));

    if(lastState == state)
        return;
    else
        lastState = state;

    switch(state)
    {
        case MOTOR_MODE_STANDBY:
            break;
        case MOTOR_MODE_ZOOM:
            //motorR2A30423_resetMotor();
            break;
        case MOTOR_MODE_FOCUS:
            motorR2A30423_resetMotor();
            break;
        case MOTOR_MODE_APERTURE:
            break;
        case MOTOR_MODE_SHUTTER:
            break;
        default:
            LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
            break;
    }
    return;
}

/**
    motor info setting

    Set MOTOR_INFO_TAB info to motor

    @param pTab - The MOTOR_INFO_TAB pointer.

    @return None.
*/
static void motorR2A30423_setMotorInfo(PMOTOR_INFO_TAB pTab)
{
    gR2A30423PvtObj.bWaitZoomFinish = pTab->bWaitZoomFinish;
}

/**
    Reset input pin of motor

    Reset input pin of motor.

    @param None

    @return None.
*/
static void motorR2A30423_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN3);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN4);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN5);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN6);
    return;
}

/**
    H/W reset motor.

    H/W reset motor.

    @param None

    @return None.
*/
static void motorR2A30423_resetMotor(void)
{
    TimerDelayMs(1);

    gpio_clearPin(MOTOR_PINDEF_MOTO_RESET);

    TimerDelayMs(1);

    gpio_setPin(MOTOR_PINDEF_MOTO_RESET);

    TimerDelayMs(10);
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
static BOOL motorR2A30423_zoom_checkPI(void)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return FALSE;
    }
    LENS_MSG(("LENS_M: motorR2A30423_zoomPI_check\n\r"));
    return gpio_getPin(MOTOR_PINDEF_ZOOM_PI);
}

/**
    Zoom PI interrupt check

    Check zoom PI interrupt status

    @param None.

    @return BOOL - The zoom PI interrupt status.
*/
static BOOL motorR2A30423_zoom_checkPIInt(void)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
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
static BOOL motorR2A30423_zoom_checkPR(void)
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;
    static BOOL bFirstTime = TRUE;

    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return FALSE;
    }

    if(bFirstTime)
    {
        //channel common
        addr = Address_common_Sel;

        reg = Rcomm_PSOut;
        val = 0x3C0;            // On: PS, PS12, PS3, PS4
        motorR2A30423_sif_WrReg(addr,reg,val);

        //channel common
        addr = Address_common_Sel;

        reg = Rcomm_PIOut;
        val = 0x300;            // On: PI1, PI2
        motorR2A30423_sif_WrReg(addr,reg,val);

        TimerDelayMs(10);

        bFirstTime = FALSE;
    }

    return gpio_getPin(MOTOR_PINDEF_ZOOM_PR);
}

/**
    Zoom PI interrupt status clear

    clear zoom PI interrupt status

    @param None.

    @return None.
*/
static void motorR2A30423_zoom_clearPIInt(void)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
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
static void motorR2A30423_zoom_setPIInt(BOOL state)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return;
    }
    LENS_MSG(("LENS_M: motorR2A30423_zoomPIInt_set\n\r"));
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
static void motorR2A30423_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return;
    }

    LENS_MSG(("LENS_M: %s, state=%d, param1=%d\n\r", __func__, state, param1));
    switch(state)
    {
        case MOTOR_ZOOM_BRAKE:
            motorR2A30423_sif_zoom_stop();
            break;
        case MOTOR_ZOOM_FWD:
            motorR2A30423_resetMotor();
            motorR2A30423_sif_zoom(TRUE, param1);
            break;
        case MOTOR_ZOOM_BWD:
            motorR2A30423_resetMotor();
            motorR2A30423_sif_zoom(FALSE, param1);
            break;
        case MOTOR_ZOOM_OFF:
            break;
        default:
            LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
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
static BOOL motorR2A30423_focus_checkPI(void)
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;
    static BOOL bFirstTime = TRUE;

    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return FALSE;
    }
    //LENS_MSG(("LENS_M: motorR2A30423_focusPI_check\n\r"));

    if(bFirstTime)
    {
        //channel common
        addr = Address_common_Sel;

        reg = Rcomm_PSOut;
        val = 0x3C0;            // On: PS, PS12, PS3, PS4
        motorR2A30423_sif_WrReg(addr,reg,val);

        //channel common
        addr = Address_common_Sel;

        reg = Rcomm_PIOut;
        val = 0x300;            // On: PI1, PI2
        motorR2A30423_sif_WrReg(addr,reg,val);

        TimerDelayMs(10);

        bFirstTime = FALSE;
    }

    return gpio_getPin(MOTOR_PINDEF_FOCUS_PI);
}

/**
    motor focus mode action

    Set phase to motor in focus mode

    @param state - The phase state of focus mode.

    @return None.
*/
static void motorR2A30423_focus_setState(UINT32 state, UINT32 param1)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return;
    }

    //LENS_MSG(("LENS_M: %s, state=%d param1=%d\n\r", __func__, state, param1));

    switch(state)
    {
        case MOTOR_FOCUS_FWD:
            motorR2A30423_sif_focus(TRUE, param1);
            break;
        case MOTOR_FOCUS_BWD:
            motorR2A30423_sif_focus(FALSE, param1);
            break;
        default:
            LENS_ERR(("LENS_E: %s,UNKNOW\n\r", __func__));
            break;
    }

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
static void motorR2A30423_aperture_setState(UCHAR state)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return;
    }

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
static void motorR2A30423_shutter_setState(UCHAR state)
{
    if(!gR2A30423PvtObj.bInitYet)
    {
        LENS_ERR(("LENS_E: motorR2A30423_init not ready\n\r"));
        return;
    }

    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with SIF related API.
//--------------------------------------------------------------------------------------------------------------------------
static void motorR2A30423_sif_init(void)
{
    SIF_CTRL    SIFCtrl;

    switch(gR2A30423PvtObj.uiSIFChannel)
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
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 16;
    SIFCtrl.uiClkDiv        = 5;    // 48/(2*(5+1)) = 4Mhz
    SIFCtrl.bMSB            = TRUE;
    sif_setControl(gR2A30423PvtObj.uiSIFChannel, &SIFCtrl);
}

#if 1
static void delay(UINT32 times)
{
//    TimerDelayUs(times);
//    return;
    for(;times > 0; times--)
        ;
}
static void motorR2A30423_gpio_WrReg(UINT32 val)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(gR2A30423PvtObj.uiGPIOSen , GPIO_DIR_OUTPUT);
    gpio_setDir(gR2A30423PvtObj.uiGPIOClk, GPIO_DIR_OUTPUT);
    gpio_setDir(gR2A30423PvtObj.uiGPIOData, GPIO_DIR_OUTPUT);

    gpio_clearPin(gR2A30423PvtObj.uiGPIOSen);
    gpio_clearPin(gR2A30423PvtObj.uiGPIOData);
    gpio_setPin(gR2A30423PvtObj.uiGPIOClk);

    data = (val>>16);//(UINT32)addr << 8 | (UINT32)value;

    gpio_setPin(gR2A30423PvtObj.uiGPIOSen);
    delay(500);
    for(j = 16; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(gR2A30423PvtObj.uiGPIOData);
        else
            gpio_clearPin(gR2A30423PvtObj.uiGPIOData);

        delay(100);
        gpio_clearPin(gR2A30423PvtObj.uiGPIOClk);
        delay(200);
        gpio_setPin(gR2A30423PvtObj.uiGPIOClk);
        delay(100);
    }

    delay(500);
    gpio_clearPin(gR2A30423PvtObj.uiGPIOSen);
}

static void motorR2A30423_gpio_WrRegRd(UINT32 val,UINT32 *read_back)
{
    UINT32 data;
    UINT32 j;
    UINT32  temp_data = 0;

    gpio_setDir(gR2A30423PvtObj.uiGPIOSen , GPIO_DIR_OUTPUT);
    gpio_setDir(gR2A30423PvtObj.uiGPIOClk, GPIO_DIR_OUTPUT);
    gpio_setDir(gR2A30423PvtObj.uiGPIOData, GPIO_DIR_OUTPUT);

    gpio_setDir(C_GPIO_17 , GPIO_DIR_INPUT);

    gpio_clearPin(gR2A30423PvtObj.uiGPIOSen);
    gpio_clearPin(gR2A30423PvtObj.uiGPIOData);
    gpio_setPin(gR2A30423PvtObj.uiGPIOClk);

    data = (val>>16);//(UINT32)addr << 8 | (UINT32)value;

    gpio_setPin(gR2A30423PvtObj.uiGPIOSen);
    delay(500);
    for(j = 16; j > 0; j--)
    {
        //temp_data = temp_data | (gpio_getPin(C_GPIO_17)<<(j-1));
        temp_data = temp_data | (gpio_getPin(MOTOR_PINDEF_MOTO_MOB1)<<(j-1));

        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(gR2A30423PvtObj.uiGPIOData);
        else
            gpio_clearPin(gR2A30423PvtObj.uiGPIOData);

        delay(100);
        gpio_clearPin(gR2A30423PvtObj.uiGPIOClk);
        delay(200);
        gpio_setPin(gR2A30423PvtObj.uiGPIOClk);
        delay(100);
    }

    delay(500);
    gpio_clearPin(gR2A30423PvtObj.uiGPIOSen);

    //debug_err(("motorR2A30423_gpio_WrRegRd, temp_data= %d\n\r",temp_data));
    *read_back = temp_data;
}
void Test_Read_Motor_Reg(UINT32 addr,UINT32 *read_back)
{
    UINT32 uiSIFData = 0;
    UINT32 val=0;
    UINT32 reg = R12ch_ReadPulsePos;
    UINT32 addr1=0;
    //motorR2A30414_resetMotor();
    {

      //  gpio_setDir( C_GPIO_34, GPIO_DIR_INPUT);
        //channel common
        addr1 = Address_common_Sel;

        reg = Rcomm_ExtMobOut;
        val = 0x080;            // High during exec, MOB12, EXT12, EXT34
        //debug_err(("[1]Test_Read_Motor_Reg ,addr1=0x%x,reg=0x%x,val=0x%x\n\r",addr1,reg,val));
        motorR2A30423_sif_WrReg(addr1,reg,val);
    }
    //MSB
    val = 0;
    reg = R12ch_ReadPulsePos;

    uiSIFData = ((val & 0x3ff) << VALUE_SHIFT_OFFSET);//10 bit value
    uiSIFData |= ((reg & 0xf) << REGISTER_SHIFT_OFFSET);//4 bit register
    uiSIFData |= ((addr & 0x3) << ADDRESS_SHIFT_OFFSET);//2 bit address
    uiSIFData = (uiSIFData << 16);
    //debug_err(("Test_Read_Motor_Reg ,uiSIFData=0x%x\n\r",uiSIFData));
    motorR2A30423_gpio_WrRegRd(uiSIFData,read_back);
    debug_err(("Test_Read_Motor_Reg ,addr=[%d] read_back = 0x%x\n\r",addr,*read_back));
return;
}
#endif

static void motorR2A30423_sif_WrReg(UINT32 addr,UINT32 reg,UINT32 val)
{
    UINT32 uiSIFData = 0;

    //MSB
    uiSIFData = ((val & 0x3ff) << VALUE_SHIFT_OFFSET);//10 bit value
    uiSIFData |= ((reg & 0xf) << REGISTER_SHIFT_OFFSET);//4 bit register
    uiSIFData |= ((addr & 0x3) << ADDRESS_SHIFT_OFFSET);//2 bit address
    uiSIFData = (uiSIFData << 16);


    if(gR2A30423PvtObj.uiSIFType == LENS_SIF_GPIOSIM)
    {
        motorR2A30423_gpio_WrReg(uiSIFData);
    }
    else
    {
        sif_sendMSB(gR2A30423PvtObj.uiSIFChannel, uiSIFData);
    }

}

static void motorR2A30423_sif_focus(BOOL bForward, UINT32 step)
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;
    UINT32 count = 0;
    BOOL   bMotorBwd;

    if(bForward)
        bMotorBwd = TRUE;
    else
        bMotorBwd = FALSE;

    //channel 1 and channel 2
    addr = Address_12ch_Sel;

    reg = R12ch_Mode;
    val = ((bMotorBwd << 9) |   // 0: Forward , 1: Backward
           (0         << 6) |   // Micro-stepping
           (0         << 4) |   // No acc/dec
           (0         << 1) |   // A 100% B 100%
           (1         << 0)     // Disabled
          );
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_Pulse1;
    val = (step%16)*64;     // 64 micro step = 1 step
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_Pulse2;
    val = (step/16);
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_PulseRate0;
    //val = 0x04F;            // 1000pps
    //val = 0x057;            // 900pps
    //val = 0x062;            // 800pps
    //val = 0x070;            // 700pps
    val = 0x09D;            // 500pps
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_PulseRateRange;
    val = 0x280;            // 0.2 us + 256 steps
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_PrePulseGen;
    val = 0x0C0;            // 9.83 ms
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_PostPulseGen;
    val = 0x0C0;            // 9.83 ms
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R12ch_Volt;
    val = 0x280;            // 3.0 v
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_PSOut;
    val = 0x3C0;            // On: PS, PS12, PS3, PS4
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_PIOut;
    val = 0x300;            // On: PI1, PI2
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_ExtMobOut;
    val = 0x000;            // High during exec, MOB12, EXT12, EXT34
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel 1 and channel 2
    addr = Address_12ch_Sel;

    reg = R12ch_PerCHOpCtrl;
    val = 0x200;            // Enable operation
    motorR2A30423_sif_WrReg(addr,reg,val);

    //wait until execute start
    while(gpio_getPin(MOTOR_PINDEF_MOTO_EXT1) == 0)
    {
        TimerDelayUs(100);
        count++;

        if(count > 10000)
        {
            LENS_ERR(("LENS_E: %s start timeout!!\n\r", __func__));
            break;
        }
    }

    //wait until execute finish
    count = 0;
    while(gpio_getPin(MOTOR_PINDEF_MOTO_EXT1) == 1)
    {
        TimerDelayUs(100);
        count++;

        if(count > 50000)
        {
            LENS_ERR(("LENS_E: %s finish timeout!!\n\r", __func__));
            break;
        }
    }

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_PSOut;
    val = 0x000;            // Off: PS, PS12, PS3, PS4
    motorR2A30423_sif_WrReg(addr,reg,val);
}

static void motorR2A30423_sif_zoom(BOOL bForward, UINT32 step)
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;
    UINT32 count = 0;
    BOOL   bMotorBwd;
    UINT32 i=0;

    if(bForward)
    {
        bMotorBwd = TRUE;
    }
    else
    {
        bMotorBwd = FALSE;
    }

    //channel 3 and channel 4
    addr = Address_34ch_Sel;

    reg = R34ch_Mode;
    val = ((bMotorBwd << 9) |   // 0: Forward , 1: Backward
           (0         << 6) |   // Micro-stepping
           (0         << 4) |   // No acc/dec
           (0         << 1) |   // A 100% B 100%
           (1         << 0)     // Disabled
          );
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_Pulse1;
    val = (step%16)*64;     // 64 micro step = 1 step
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_Pulse2;
    val = (step/16);
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_PulseRate0;
    //val = 0x070;            // 700pps
    val = 0x09D;            // 500pps
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_PulseRateRange;
    val = 0x280;            // 0.2 us + 256 steps
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_PrePulseGen;
    val = 0x0C0;            // 9.83 ms
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_PostPulseGen;
    val = 0x0C0;            // 9.83 ms
    motorR2A30423_sif_WrReg(addr,reg,val);

    reg = R34ch_Volt;
    val = 0x280;            // 3.0 v
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_PSOut;
    val = 0x3C0;            // On: PS, PS12, PS3, PS4
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_PIOut;
    val = 0x300;            // On: PI1, PI2
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;

    reg = Rcomm_ExtMobOut;
    val = 0x000;            // High during exec, MOB12, EXT12, EXT34
    motorR2A30423_sif_WrReg(addr,reg,val);


    //channel 3 and channel 4
    addr = Address_34ch_Sel;

    reg = R34ch_PerCHOpCtrl;
    val = 0x200;            // Enable operation
    motorR2A30423_sif_WrReg(addr,reg,val);

    //wait until execute start
    while(gpio_getPin(MOTOR_PINDEF_MOTO_EXT2) == 0)
    {
        TimerDelayUs(100);
        count++;

        if(count > 10000)
        {
            LENS_ERR(("LENS_E: %s start timeout!!\n\r", __func__));
            break;
        }
    }

    //wait until execute finish
    if(gR2A30423PvtObj.bWaitZoomFinish)
    {
        count = 0;
        while(gpio_getPin(MOTOR_PINDEF_MOTO_EXT2) == 1)
        {
            TimerDelayUs(100);
            count++;

            if(count > 50000)
            {
                LENS_ERR(("LENS_E: %s finish timeout!!\n\r", __func__));
                break;
            }
        }

        //channel common
        addr = Address_common_Sel;

        reg = Rcomm_PSOut;
        val = 0x000;            // Off: PS, PS12, PS3, PS4
        motorR2A30423_sif_WrReg(addr,reg,val);
    }
}

static void motorR2A30423_sif_zoom_stop()
{
    UINT32 addr;
    UINT32 reg;
    UINT32 val;

    //channel 3 and channel 4
    addr = Address_34ch_Sel;
    reg = R34ch_PerCHOpCtrl;
    val = 0x300;            // Disable operation
    motorR2A30423_sif_WrReg(addr,reg,val);

    //channel common
    addr = Address_common_Sel;
    reg = Rcomm_PSOut;
    val = 0x000;            // Off: PS, PS12, PS3, PS4
    motorR2A30423_sif_WrReg(addr,reg,val);
}
