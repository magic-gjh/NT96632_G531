/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensCtrlTsk.c
    @ingroup    mISYSLens

    @brief      Lens control task functions
                This file is the control task functions of Lens module

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#include <string.h>
#include "OSCommon.h"
#include "Lens_int.h"
#include "ImageAlgInfor.h"

static BOOL                 bLensCtrlOpened = FALSE;
static LENSCTRL_APPOBJ      gLensCtrlObj;
static LENSCTRL_PARAMOBJ    gLensCtrlParamObj;

static MOTOR_INFO_TAB g_pLensCtrlMotorInfoTab =
{
    //PWM
    FALSE,
    0,
    0,

    //Zoom
    FALSE
};

/**
  Open Lens Control task

  Open Lens Control task.
  The parameter is not using now, just for compliance

  Return value is listed below:
  E_SYS: Task is already opened
  E_OK: No error

  @param LENSCTRL_APPOBJ pLensCtrlObj: Lens Control application object
  @return ER
*/
ER LensCtrl_Open(PLENSCTRL_APPOBJ pLensCtrlObj)
{
    if (bLensCtrlOpened == TRUE)
    {
        return E_SYS;
    }

    memcpy(&gLensCtrlObj, pLensCtrlObj, sizeof(LENSCTRL_APPOBJ));

    clr_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD |
                         FLGLENS_ZOOMREV |
                         FLGLENS_ZOOMBRK |
                         FLGLENS_ZOOMOFF |
                         FLGLENS_ZOOMCHANGE |
                         FLGLENS_FOCUSFWD   |
                         FLGLENS_FOCUSREV   |
                         FLGLENS_APERTUREMOVE |
                         FLGLENS_SHUTTERMOVE  |
                         FLGLENS_POWEROFF     |
                         FLGLENS_IDLE);

    bLensCtrlOpened = TRUE;

    sta_tsk(LENSCTRLTSK_ID);

    return E_OK;
}

/**
  Close Play Sound task

  Close Play Sound task.

  Return value is listed below:
  E_SYS: Task is already closed
  E_OK: No error

  @param void
  @return ER
*/
ER LensCtrl_Close(void)
{
    if (bLensCtrlOpened == FALSE)
    {
        return E_SYS;
    }

    bLensCtrlOpened = FALSE;

    ter_tsk(LENSCTRLTSK_ID);

    return E_OK;
}

PLENSCTRL_APPOBJ LensCtrl_Get(void)
{
    if (bLensCtrlOpened == FALSE)
    {
        return NULL;
    }
    return &gLensCtrlObj;
}

ER LensCtrl_RegCB(FPLENSCALLBACK cb)
{
    gLensCtrlObj.fpLensCB = cb;
    return E_OK;
}

BOOL LensCtrl_Set(PLENSCTRL_APPOBJ pLensCtrlObj)
{
    if (bLensCtrlOpened == FALSE)
    {
        return FALSE;
    }
    memcpy(&gLensCtrlObj, pLensCtrlObj, sizeof(LENSCTRL_APPOBJ));

    return TRUE;
}

ER LensCtrl_SetParam(PLENSCTRL_PARAMOBJ pLensCtrlParamObj)
{
    memcpy(&gLensCtrlParamObj, pLensCtrlParamObj, sizeof(LENSCTRL_PARAMOBJ));

    if(gLensCtrlObj.pMotor != NULL)
    {
        g_pLensCtrlMotorInfoTab.bWaitZoomFinish = gLensCtrlParamObj.bWaitZoomFinish;

        gLensCtrlObj.pMotor->setMotorInfo(&g_pLensCtrlMotorInfoTab);
    }

    return E_OK;
}

ER LensCtrl_WaitCmdFinish(UINT32 TimeOut)
{
    FLGPTN  nFlag;

    if (bLensCtrlOpened == FALSE)
    {
        return E_SYS;
    }

    if(TimeOut == LENSCTRL_WAIT_INFINITE)
    {
        wai_flg(&nFlag, FLG_ID_LENS, FLGLENS_IDLE, TWF_ORW);
    }

    return E_OK;
}

/**
  Lens control task

  Lens control task to handle interrupt event.

  @param void
  @return void
*/
void LensCtrlTsk(void)
{
    FLGPTN  uiFlag;

    while (1)
    {
        set_flg(FLG_ID_LENS, FLGLENS_IDLE);
        wai_flg(&uiFlag, FLG_ID_LENS,
                         FLGLENS_ZOOMFWD |
                         FLGLENS_ZOOMREV |
                         FLGLENS_ZOOMBRK |
                         FLGLENS_ZOOMOFF |
                         FLGLENS_ZOOMCHANGE |
                         FLGLENS_FOCUSFWD   |
                         FLGLENS_FOCUSREV   |
                         FLGLENS_APERTUREMOVE |
                         FLGLENS_SHUTTERMOVE  |
                         FLGLENS_POWEROFF
                       , TWF_ORW | TWF_CLR);
        clr_flg(FLG_ID_LENS, FLGLENS_IDLE);

        if(gLensCtrlObj.pMotor == NULL)
        {
            LENS_ERR(("LENS_E: %s pMotor is null\r\n", __func__));
            continue;
        }

        if(uiFlag & FLGLENS_ZOOMBRK)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_BRAKE, 0);
        }

        if(uiFlag & FLGLENS_ZOOMFWD)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_FWD, gLensCtrlParamObj.uiZoomStep);
        }

        if(uiFlag & FLGLENS_ZOOMREV)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_BWD, gLensCtrlParamObj.uiZoomStep);
        }

        if(uiFlag & FLGLENS_ZOOMOFF)
        {
            gLensCtrlObj.pMotor->zoom_setState(MOTOR_ZOOM_OFF, 0);
        }

        if(uiFlag & FLGLENS_ZOOMCHANGE)
        {
            if(gLensCtrlObj.fpLensCB != NULL)
            {
                AlgMsgInfo    AlgMsg;

                AlgMsg.OzoomIndex = gLensCtrlObj.pLens->zoom_getNextSection();
                gLensCtrlObj.fpLensCB(ALGMSG_CHGOZOOMSTEP, &AlgMsg);
            }
        }

        if(uiFlag & FLGLENS_FOCUSFWD)
        {
            gLensCtrlObj.pMotor->focus_setState(MOTOR_FOCUS_FWD, gLensCtrlParamObj.uiFocusStep);
        }

        if(uiFlag & FLGLENS_FOCUSREV)
        {
            gLensCtrlObj.pMotor->focus_setState(MOTOR_FOCUS_BWD, gLensCtrlParamObj.uiFocusStep);
        }

        if(uiFlag & FLGLENS_APERTUREMOVE)
        {
            gLensCtrlObj.pMotor->aperture_setState(gLensCtrlParamObj.uiApertureState);
        }

        if(uiFlag & FLGLENS_SHUTTERMOVE)
        {
            gLensCtrlObj.pMotor->shutter_setState(gLensCtrlParamObj.uiShutterState);
        }

        if(uiFlag & FLGLENS_POWEROFF)
        {
            TimerDelayMs(LENS_MAX_WAIT_TIME);
            gLensCtrlObj.pMotor->changeMode(MOTOR_MODE_STANDBY);

            gLensCtrlObj.pMotor->power(MOTOR_POWER_OFF);
        }

    }
}

