/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensMotor_AT5510.c
    @ingroup    mISYSLens

    @brief      The driver of Motor Driver "SiTI AT5510"

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/
#include "OSCommon.h"
#include "PWM.h"
#include "SIF.h"
#include "i2c.h"
#include "GPIO.h"
#include "pad.h"
#include "top.h"
#include "Timer.h"
#include "Lens.h"
#include "LensMotor_DUMMY.h"
#include "LensMotor_AT5510.h"

//static var declare
static MOTOR_TAB        gAT5510Obj;//extern object for others
static MOTOR_PVT_OBJ    gAT5510PvtObj;//private object for internal usage
//PINMUX Interface
#define MOTOR_PINMUX_SELECT_MES(sel) pinmux_select_mes((sel))
//new architecture
#define MOTOR_PINDEF_MOTO_POWER           (gAT5510PvtObj.uiPinPower)
#define MOTOR_PINDEF_MOTO_EN              (gAT5510PvtObj.uiPinMotorEn)
#define MOTOR_PINDEF_MOTO_IN1A            (gAT5510PvtObj.uiPinInput1)
#define MOTOR_PINDEF_MOTO_IN1B            (gAT5510PvtObj.uiPinInput2)
#define MOTOR_PINDEF_MOTO_IN2A            (gAT5510PvtObj.uiPinInput3)//SN_MES0 = AGPIO20
#define MOTOR_PINDEF_MOTO_IN2B            (gAT5510PvtObj.uiPinInput4)//SN_MES1 = AGPIO21

#define MOTOR_PINDEF_ZOOM_PIN0           MOTOR_PINDEF_MOTO_IN2A
#define MOTOR_PINDEF_ZOOM_PIN1           MOTOR_PINDEF_MOTO_IN2B
#if 0
#define MOTOR_PINDEF_IRIS_PIN0           MOTOR_PINDEF_MOTO_IN1
#define MOTOR_PINDEF_IRIS_PIN1           MOTOR_PINDEF_MOTO_IN2

#define MOTOR_PINDEF_SHUTTER_MES0        MOTOR_PINDEF_MOTO_IN3
#define MOTOR_PINDEF_SHUTTER_MES1        MOTOR_PINDEF_MOTO_IN4
#endif
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is static func declare
//--------------------------------------------------------------------------------------------------------------------------
static void motorAT5510_init(PLENS_DEVICE_OBJ pLensDev);
static void motorAT5510_power(MOTOR_POWER state);
static void motorAT5550_zoom_setState(MOTOR_ZOOM_ACT state);
static void motorAT5510_focus_setState(UINT32 state);
static void motorAT5510_resetInputPin(void);
static void motorAT5550_pwm_hdlPWM(void);
static void motorAT5550_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection);

static void motorAT5510_i2c_WrReg(UINT32 value);
static UINT32 motorAT5510_i2c_RdReg(void);

static MOTOR_TAB gAT5510Obj =
{
    motorAT5510_init,
    motorAT5510_power,
    motorDUMMY_changeMode,
    motorDUMMY_setPWMInfo,
    motorAT5550_zoom_setState,//AT5550
    motorDUMMY_zoom_setPIInt,//AT5550
    motorDUMMY_zoom_clearPIInt,//AT5550
    motorDUMMY_zoom_checkPIInt,//AT5550
    motorDUMMY_zoom_checkPR,//AT5550
    motorDUMMY_zoom_checkPI,//AT5550
    motorDUMMY_focus_checkPI,//AT5510
    motorAT5510_focus_setState,//AT5510
    motorDUMMY_aperture_setState,//DUMMY
    motorDUMMY_shutter_setState//DUMMY
};

static MOTOR_PVT_OBJ gAT5510PvtObj =
{
    FALSE,0,0,
    0,0,0,0,//pin assign
    0x18,FALSE,//0001100
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

    Init AT5510 pin assignment

    @param None.

    @return None.
*/
static void motorAT5510_init(PLENS_DEVICE_OBJ pLensDev)
{
    if(gAT5510PvtObj.bInitYet)
    {
        debug_err(("motorAT5510_init already\n\r"));
        return;
    }

    if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_SIF)
    {
    }
    else if(pLensDev->uiMotorCtrlMode == MOTOR_CTRL_I2C)
    {
        //work around here,AT5510+AT5550
        //assign pin number from user,to be construct.
        //gAT5510PvtObj.uiPinInput1  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        //gAT5510PvtObj.uiPinInput2  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        gAT5510PvtObj.uiPinInput3  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_0];
        gAT5510PvtObj.uiPinInput4  = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_IN_1];
        gAT5510PvtObj.uiPinPower   = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_PWR];
        gAT5510PvtObj.uiPinMotorEn = pLensDev->puiMotorCtrlGPIOArray[LENS_IO_MODE_1];
        debug_err(("[AT5510]motorAT5510_init,%d %d 0x%x 0x%x\n\r",gAT5510PvtObj.uiPinInput3,gAT5510PvtObj.uiPinInput4,gAT5510PvtObj.uiPinPower,gAT5510PvtObj.uiPinMotorEn));
    }

    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);

    gpio_setDir(MOTOR_PINDEF_MOTO_POWER, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_EN,    GPIO_DIR_OUTPUT);
    //gpio_setDir(MOTOR_PINDEF_MOTO_IN1A, GPIO_DIR_OUTPUT);
    //gpio_setDir(MOTOR_PINDEF_MOTO_IN1B, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2A, GPIO_DIR_OUTPUT);
    gpio_setDir(MOTOR_PINDEF_MOTO_IN2B, GPIO_DIR_OUTPUT);

    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN1A);
    //gpio_clearPin(MOTOR_PINDEF_MOTO_IN1B);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2A);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2B);

    gAT5510PvtObj.bInitYet = TRUE;
    return;
}

/**
    Get AT5510 motor object

    @return AT5510 motor object pointer

*/
PMOTOR_TAB motorAT5510_getObject()
{
    return &gAT5510Obj;
}

/**
    motor power setting

    Turn on/off motor power

    @param state - The power state of motor.

    @return None.
*/
static void motorAT5510_power(MOTOR_POWER state)
{
    switch(state)
    {
        case MOTOR_POWER_OFF:
            //gpio_clearPin(MOTOR_PINDEF_MOTO_POWER);
            //gpio_setPin(MOTOR_PINDEF_MOTO_EN);    //Set PD pin as 1 to disable AT5510.
            break;
        case MOTOR_POWER_ON:
            gpio_setPin(MOTOR_PINDEF_MOTO_POWER);
            gpio_clearPin(MOTOR_PINDEF_MOTO_EN);    //Set PD pin as 0 to enable AT5510.
            break;
        default:
            debug_err(("[AT5510]motorAT5510_power,UNKNOW\n\r"));
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
static void motorAT5510_resetInputPin(void)
{
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1A);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN1B);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2A);
    gpio_clearPin(MOTOR_PINDEF_MOTO_IN2B);
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
static void motorAT5550_pwm_hdlPWM(void)
{
    if(gAT5510PvtObj.bPwmCtrl != TRUE)
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
static void motorAT5550_pwm_setState(BOOL state,MOTOR_ZOOM_ACT pwmprevdirection)
{
    if(gAT5510PvtObj.bPwmCtrl != TRUE)
    {
        return;
    }

    debug_err(("motorAT5510_pwm_action\r\n"));

    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom related API.
//--------------------------------------------------------------------------------------------------------------------------
/**
    motor zoom mode action

    Set MOTOR_ZOOM_BRAKE/MOTOR_ZOOM_FWD/MOTOR_ZOOM_BWD/MOTOR_ZOOM_OFF to motor in Zoom mode

    @param state - The action state of zoom mode.

    @return None.
*/
static void motorAT5550_zoom_setState(MOTOR_ZOOM_ACT state)
{
    if(!gAT5510PvtObj.bInitYet)
    {
        debug_err(("motorAT5550_init not ready\n\r"));
        return;
    }

    //debug_err(("motorMD152_zoom_action, state=%d\n\r", state));
    MOTOR_PINMUX_SELECT_MES(PINMUX_GPIO);

    switch(state)
    {
        case MOTOR_ZOOM_BRAKE:
            //check if pwm control enable
            if(gAT5510PvtObj.bPwmCtrl == TRUE)
            {
                motorAT5550_pwm_setState(TRUE,state);
            }
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            break;
        case MOTOR_ZOOM_FWD:
            //check if pwm control enable
            if(gAT5510PvtObj.bPwmCtrl == TRUE)
            {
                motorAT5550_pwm_setState(TRUE,state);
            }
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN0);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            break;
        case MOTOR_ZOOM_BWD:
            //check if pwm control enable
            if(gAT5510PvtObj.bPwmCtrl == TRUE)
            {
                motorAT5550_pwm_setState(TRUE,state);
            }
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            gpio_setPin(MOTOR_PINDEF_ZOOM_PIN1);
            break;
        case MOTOR_ZOOM_OFF:
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN1);
            gpio_clearPin(MOTOR_PINDEF_ZOOM_PIN0);
            break;
        default:
            debug_err(("[AT5510]motorAT5550_zoom_setState,UNKNOW\n\r"));
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
    motor focus mode action

    Set phase to motor in focus mode

    @param state - The phase state of focus mode.

    @return None.
*/
static void motorAT5510_focus_setState(UINT32 state)
{
    UINT32 value;
    if(!gAT5510PvtObj.bInitYet)
    {
        debug_err(("motorAT5510_init not ready\n\r"));
        return;
    }
    value = state;
    motorAT5510_i2c_WrReg(value);
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with aperture related API.
//--------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with shutter related API.
//--------------------------------------------------------------------------------------------------------------------------




//The address of the AT5510 is 0001100
static void motorAT5510_i2c_WrReg(UINT32 value)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulData1 = 0, ulData2 = 0;
    BOOL        bPowerDown = gAT5510PvtObj.bPowerDown;

    i2c_lock();

    ulWriteAddr = (gAT5510PvtObj.uiDeviceAddress | 0x00);
    ulData1     = (bPowerDown << 7) | (value>>4);//PD - D9 D8 D7 D6 D5 D4
    ulData2     = ((value & 0x0000000f)<<4);//D3 D2 D1 D0 - - - -
    //debug_err(("[AT5510]I2C-W device-addr = 0x%x, value = 0x%x\r\n",ulWriteAddr,value));
    erReturn=i2c_open();

    if(erReturn != E_OK)
    {
        debug_err(("Error open I2C driver!!\r\n"));
        goto FUNC_END;
    }

    pinmux_select_i2c(PINMUX_I2C);

    erReturn = i2c_initMaster(96, 10, 4);
    //erReturn = i2c_initMaster(64, 10, 4);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
        goto FUNC_END;
    }

    i2c_enable();
    erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

    if (erReturn != I2C_SUCCESS)
    {
        //[Tricky] Re-transmit again to avoid 1st transmit data error!!
        erReturn=i2c_masterTransmit(ulWriteAddr, 1, 0);

        if (erReturn != I2C_SUCCESS)
        {
            debug_err(("Error transmit data (write addr)!!\r\n"));
            goto FUNC_END;
        }
    }

    erReturn=i2c_masterTransmit(ulData1, 0, 0);

    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error transmit data (data1) !!\r\n"));
        goto FUNC_END;
    }

    erReturn=i2c_masterTransmit(ulData2, 0, 1);

    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error transmit data (data2) !!\r\n"));
        goto FUNC_END;
    }

FUNC_END:

    i2c_reset();
    i2c_unlock();
    //i2c_close();

    //debug_err(("WriteReg [0x%.8X, 0x%.8X, 0x%.8X]\r\n", ulWriteAddr, ulData1, ulData2));
}

static UINT32 motorAT5510_i2c_RdReg(void)
{
    UINT        erReturn;
    UINT32      ulReadAddr, ulData, ulData1, ulData2;

    i2c_lock();

    ulReadAddr  =(gAT5510PvtObj.uiDeviceAddress | 0x01);
    debug_err(("[AT5510]I2C-R device-addr = 0x%x\r\n",ulReadAddr));
    erReturn=i2c_open();

    if(erReturn != E_OK)
    {
        debug_err(("Error open I2C driver!!\r\n"));
        goto FUNC_END;
    }

    pinmux_select_i2c(PINMUX_I2C);

    erReturn = i2c_initMaster(96, 10, 4);
    //erReturn = i2c_initMaster(64, 10, 4);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
        goto FUNC_END;
    }

    i2c_enable();
    erReturn=i2c_masterTransmit(ulReadAddr, 1, 0);

    if (erReturn !=  I2C_SUCCESS)
    {
        debug_err(("Error transmit addr before receive!!\r\n"));
        goto FUNC_END;
    }

    erReturn=i2c_masterReceive(&ulData1, 0, 0);

    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error receive data1!!\r\n"));
        goto FUNC_END;
    }

    erReturn=i2c_masterReceive(&ulData2, 0, 1);

    if (erReturn != I2C_SUCCESS)
    {
        debug_err(("Error receive data2!!\r\n"));
        goto FUNC_END;
    }

FUNC_END:

    i2c_reset();
    i2c_unlock();
    //i2c_close();

    ulData=((ulData1<<8)|(ulData2));
    //debug_err(("ReadReg [0x%.8X, 0x%.8X]\r\n", addr, ulData));
    debug_err(("[AT5510]I2C-R ulData = 0x%x\r\n",ulData));
    return ulData;
}

static void motorAT5510_setPowerDownMode(BOOL bState)
{
    UINT32  val = 0x8000;
    if(bState == TRUE)
        val = 0x8000;//power down
    else
        val = 0x0000;//power up
    motorAT5510_i2c_WrReg(val);
}
