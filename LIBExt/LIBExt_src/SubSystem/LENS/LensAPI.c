/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensAPI.c
    @ingroup    mISYSLens

    @brief      Lens API functions

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

/*
    Todo: Justin
    1. Define device object.
    2. Use the functions in lens function table instead of original hard coding functions.
    3. Implement power on/off and mode enable/disable here.
*/

#include <string.h>
#include "OSCommon.h"
#include "GPIO.h"
#include "Lens_int.h"
#include "LensAPI.h"
#include "Timer.h"


static LENS_MODULE_OBJ gLensModuleObj =
{
    LENS_NONE,                      ///< The current state of lens module
    NULL,                           ///< The function table of motor driver
    NULL                            ///< The function table of lens driver
};


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static Lens_Module_ChangeState(LENS_MODULE_STATE state)
{
    LENS_MSG(("LENS_M: %s state=%d\r\n", __func__, state));
    loc_cpu();  // enter critical section
    gLensModuleObj.uiLensModuleState = state;
    unl_cpu();  // exit critical section
}

void Lens_Module_Init(PLENS_DEVICE_OBJ pLensDev, PLENS_TAB pLensTab, PMOTOR_TAB pMotorTab)
{
    LENSCTRL_APPOBJ LensCtrlObj;
    LENS_ERR(("LENS_M: Lens_Module_Init\n\r"));

    if(pLensDev == NULL||
       pLensTab == NULL||
       pMotorTab == NULL)
    {
        LENS_ERR(("LENS_E: %s param error!!\r\n", __func__));
        return;
    }


    //init module object
    Lens_Module_ChangeState(LENS_INITOK);
    gLensModuleObj.pLensObj = pLensTab;
    gLensModuleObj.pMotorObj = pMotorTab;

    //init motor driver
    gLensModuleObj.pMotorObj->init(pLensDev);

    //init lens driver
    gLensModuleObj.pLensObj->init(pLensDev, gLensModuleObj.pMotorObj);

    if(gLensModuleObj.pLensObj->isFFLens())
    {
        Lens_Module_ChangeState(LENS_IDLE);
    }

    //init lens control task
    LensCtrl_Close();
    memset(&LensCtrlObj, 0, sizeof(LENSCTRL_APPOBJ));
    LensCtrlObj.pLens = pLensTab;
    LensCtrlObj.pMotor = pMotorTab;
    LensCtrl_Open(&LensCtrlObj);

    return;
}

UINT32 Lens_Module_GetState(void)
{
    return gLensModuleObj.uiLensModuleState;
}

void Lens_OnOff(unsigned char ON_OFF)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    if(ON_OFF < MOTOR_POWER_MAX)
    {
        gLensModuleObj.pMotorObj->power(ON_OFF);
    }
    else
    {
        gLensModuleObj.pMotorObj->power(MOTOR_POWER_ON);
    }
}

void Lens_HandleAutoPowerOff(void)
{
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_STANDBY);

    Lens_OnOff(MOTOR_POWER_OFF);
}

void Lens_AutoPowerOff(UINT32 waitMS)
{
    UINT TimerID;

    if( timer_openAutoClose(&TimerID, Lens_HandleAutoPowerOff) == E_OK)
    {
        //clr_flg(FLG_ID_INT, (FLGINT2_TIMER0<<TimerID));
        timer_set(TimerID, waitMS, _TIMER_CTRL_ONE_SHOT|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
    }
    else
    {
        set_flg(FLG_ID_LENS, FLGLENS_POWEROFF);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 Lens_Zoom_Init(LENS_ZOOM_INIT_STATE part)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    switch(part)
    {
    case LENS_ZOOM_INIT_PART1:

        if(gLensModuleObj.uiLensModuleState == LENS_ZOOMMOVING)
            return ERR_OK;

        //1.1 Power ON
        Lens_OnOff(ON);

        //1.2 Change Lens State
        Lens_Module_ChangeState(LENS_ZOOMMOVING);

        //1.3 Set Zoom Mode
        gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

        //1.4 Init Zoom Part1
        gLensModuleObj.pLensObj->zoom_initPart1();

        return ERR_OK;

    case LENS_ZOOM_INIT_PART2:

        //2.1 Init Zoom Part2
        if(gLensModuleObj.pLensObj->zoom_initPart2() == ERR_OK)
        {
            //2.1.1 Set Aperture Mode
            gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_APERTURE);

            //2.1.2 Set Big Aperture
            gLensModuleObj.pLensObj->aperture_gotoState(LENS_APERTURE_BIG);

            //2.1.3 Set Shutter Mode
            gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_SHUTTER);

            //2.1.4 Open Shutter
            gLensModuleObj.pLensObj->shutter_gotoState(LENS_SHUTTER_OPEN);

            //2.1.5 Set Focus Mode
            gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

            //2.1.6 Focus to Home
            gLensModuleObj.pLensObj ->focus_gotoHome();

            //2.1.7 Change Lens State
            Lens_Module_ChangeState(LENS_IDLE);
        }
        else
        {
            LENS_ERR(("LENS_E: %s init part2 error!!\r\n", __func__));

            //2.2.1 Change Lens State
            Lens_Module_ChangeState(LENS_ERROR);

            //2.2.2 Power OFF
            Lens_OnOff(OFF);

            return ERR_OPERATION_FAIL;
        }
        break;

    default:
        LENS_ERR(("LENS_E: %s: init wrong state = %d\r\n", __func__, part));
        return ERR_OPERATION_FAIL;
    }
}

INT32 Lens_Zoom_GetSection(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->zoom_getCurrentSection();
}

INT32 Lens_Zoom_GetMaxSection(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->zoom_getMaxSection();
}

INT32 Lens_Zoom_Goto(UINT8 section)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_MSG(("LENS_M: Lens_Zoom_Goto section=%d\r\n", section));

    Lens_Module_ChangeState(LENS_ZOOMMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //3. Set Zoom In
    gLensModuleObj.pLensObj->zoom_gotoSection(section);

    //4. Power OFF
    Lens_OnOff(OFF);

    Lens_Module_ChangeState(LENS_IDLE);

    return ERR_OK;
}

INT32 Lens_Zoom_In(void)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
    case LENS_ZOOMMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_ERR(("LENS_M: Lens_Zoom_In\n\r"));

    Lens_Module_ChangeState(LENS_ZOOMMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //3. Set Zoom In
    gLensModuleObj.pLensObj->zoom_ctrlPress(LENS_ZOOM_IN);

    return ERR_OK;
}

INT32 Lens_Zoom_Out(void)
{
    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_IDLE:
    case LENS_ZOOMMOVING:
        break;
    default:
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_ERR(("LENS_M: Lens_Zoom_Out\n\r"));

    Lens_Module_ChangeState(LENS_ZOOMMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Zoom Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);

    //3. Set Zoom Out
    gLensModuleObj.pLensObj->zoom_ctrlPress(LENS_ZOOM_OUT);

    return ERR_OK;
}

INT32 Lens_Zoom_Stop(void)
{
    INT32 Status;

    if(gLensModuleObj.uiLensModuleState == LENS_IDLE)
        return ERR_OK;

    if(gLensModuleObj.uiLensModuleState != LENS_ZOOMMOVING)
    {
        LENS_ERR(("LENS_E: Lens_Zoom_Stop fail state=%d\r\n", gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    LENS_ERR(("LENS_M: Lens_Zoom_Stop\n\r"));

    //4. Set Zoom Out
    Status = gLensModuleObj.pLensObj->zoom_ctrlRelease();

    //5. Power OFF
    Lens_OnOff(OFF);

    Lens_Module_ChangeState(LENS_IDLE);

    if((Status > gLensModuleObj.pLensObj->zoom_getMaxSection()) || Status <= 0)
    {
        return ERR_OPERATION_FAIL;
    }
    else
    {
        return ERR_OK;
    }
}

INT32 Lens_Zoom_Retract(void)
{
    INT32 status;

    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    if(gLensModuleObj.pLensObj->isFFLens())
        return ERR_OK;

    switch(gLensModuleObj.uiLensModuleState)
    {
    case LENS_ZOOMMOVING:
    case LENS_FOCUSMOVING:
    case LENS_IRISMOVING:
    case LENS_SHUTTERMOVING:
        LENS_ERR(("LENS_E: Lens_Zoom_Retract fail state=%d\r\n", gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    default:
        break;
    }

    LENS_ERR(("LENS_M: Lens Retracting...\r\n"));

    Lens_Module_ChangeState(LENS_ZOOMMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Focus Garage
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);
    gLensModuleObj.pLensObj ->focus_gotoGarage();

    //3. Retract Zoom
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_ZOOM);
    gLensModuleObj.pLensObj->zoom_retract();

    //4. Close Shutter
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_SHUTTER);
    status = gLensModuleObj.pLensObj->shutter_gotoState(LENS_SHUTTER_CLOSE);

    //5. Power OFF
    if(status != ERR_OK_TIMER_POWEROFF)
        Lens_OnOff(OFF);

    Lens_Module_ChangeState(LENS_INITOK);

    return ERR_OK;
}

void Lens_Zoom_EnableIsr(BOOL enable)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return;

    gLensModuleObj.pLensObj->zoomIsr_enable(enable);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 Lens_Focus_Goto(INT16 position)
{
    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    if(position > gLensModuleObj.pLensObj->focus_getMaxPosition())
        return ERR_OPERATION_FAIL;

    Lens_Module_ChangeState(LENS_FOCUSMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

    //3. Set Focus Go2
    gLensModuleObj.pLensObj ->focus_gotoPosition(position);

    //4. Power OFF
    Lens_OnOff(OFF);

    Lens_Module_ChangeState(LENS_IDLE);

    return ERR_OK;
}

INT32 Lens_Focus_GotoHome(void)
{
    INT32 status;

    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
    {
        LENS_ERR(("LENS_E: %s fail state=%d\r\n", __func__, gLensModuleObj.uiLensModuleState));
        return ERR_OPERATION_FAIL;
    }

    Lens_Module_ChangeState(LENS_FOCUSMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Focus Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_FOCUS);

    //3. Set Focus Home
    status = gLensModuleObj.pLensObj ->focus_gotoHome();

    //4. Power OFF
    Lens_OnOff(OFF);

    Lens_Module_ChangeState(LENS_IDLE);

    return status;
}

INT32 Lens_Focus_GetCurrentPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getCurrentPosition();
}

INT32 Lens_Focus_GetMaxPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getMaxPosition();
}

UINT32 Lens_Focus_GetFL(UINT32 zoomSection)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getFL(zoomSection);
}

INT16 Lens_Focus_GetZoomTableValue(UINT32 zoomSection, UINT32 index)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->focus_getZoomTblValue(zoomSection, index);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 Lens_Aperture_Move(unsigned short NewPosition)
{
    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
        return ERR_OPERATION_FAIL;

    Lens_Module_ChangeState(LENS_IRISMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Aperture Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_APERTURE);

    //3. Move Aperture
    gLensModuleObj.pLensObj->aperture_gotoState((LENS_APERTURE_ACT)NewPosition);

    //4. Power OFF
    Lens_AutoPowerOff(LENS_MAX_WAIT_TIME);

    Lens_Module_ChangeState(LENS_IDLE);

    return ERR_OK;
}

INT32 Lens_Aperture_GetPosition(void)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->aperture_getState();
}

UINT32 Lens_Aperture_GetFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(gLensModuleObj.uiLensModuleState == LENS_NONE)
        return ERR_OPERATION_FAIL;

    return gLensModuleObj.pLensObj->aperture_getFNO(irisPos, zoomSection);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 Lens_Shutter_Move(unsigned char open)
{
    INT32 status;

    if(gLensModuleObj.uiLensModuleState != LENS_IDLE)
        return ERR_OPERATION_FAIL;

    Lens_Module_ChangeState(LENS_SHUTTERMOVING);

    //1. Power ON
    Lens_OnOff(ON);

    //2. Set Shutter Mode
    gLensModuleObj.pMotorObj->changeMode(MOTOR_MODE_SHUTTER);

    //3. Open/Close Shutter
    status = gLensModuleObj.pLensObj->shutter_gotoState((LENS_SHUTTER_ACT)open);

    //4. Power OFF
    if(status != ERR_OK_TIMER_POWEROFF)
    {
        Lens_AutoPowerOff(gLensModuleObj.pLensObj->shutter_getPulsePeriod());
    }

    Lens_Module_ChangeState(LENS_IDLE);

    return ERR_OK;
}


