/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       LensDrv_DZ5311.c
    @ingroup    mISYSLens

    @brief      A-optronics DZ5311 lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/09/30
*/

#include <stdio.h>
#include <stdlib.h>
#include "interrupt.h"
#include "Lens_int.h"
#include "LensDrv_DZ5311.h"
#include "LensDrv_DUMMY.h"
#include "gpio.h"
#include "debug.h"
#include "top.h"
#include "timer.h"
#include "Type.h"
#include "Utility.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------

//lens
static void     lensDZ5311_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensDZ5311_isFFLens(void);

//zoom
static INT32    lensDZ5311_zoom_initPart1(void);
static INT32    lensDZ5311_zoom_initPart2(void);
static INT32    lensDZ5311_zoom_ctrlRelease(void); // return current zoom section
static INT32    lensDZ5311_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
static UINT8    lensDZ5311_zoom_getCurrentSection(void);
static UINT8    lensDZ5311_zoom_getNextSection(void);
static UINT8    lensDZ5311_zoom_getMaxSection(void);
static INT32    lensDZ5311_zoom_retract( void );
static void     lensDZ5311_zoom_gotoSection(UINT8 section);
static INT32    lensDZ5311_zoom_gotoState(LENS_ZOOM_STATE newState);
static void     lensDZ5311_zoom_changeState(void);

//zoom ISR
static void     lensDZ5311_zoomIsr_enable(BOOL enable);
static void     lensDZ5311_zoomIsr(void);

//focus
static void     lensDZ5311_focus_initPhase(void);
static void     lensDZ5311_focus_gotoPosition(INT16 position);
static INT32    lensDZ5311_focus_gotoHome(void);
static INT32    lensDZ5311_focus_gotoGarage(void);
static void     lensDZ5311_focus_runStep(UINT8 dir, UINT16 step);
static INT16    lensDZ5311_focus_getCurrentPosition(void);
static INT16    lensDZ5311_focus_getMaxPosition(void);
static UINT32   lensDZ5311_focus_getFL(UINT32 zoomSection);
static INT16    lensDZ5311_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index);

//aperture
static UINT32   lensDZ5311_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);

//shutter

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static ZOOM_Struct g_pZoomPara={0};
static FOCUS_Struct g_pFocusPara={0};

static UINT16 g_pZoomSectionPulse[(ZOOM_SECTION_TELE+1)]=
{
    ZOOM_POS_GARAGE,
    ZOOM_POS_WIDE,
    ZOOM_POS_WIDE + 90,
    ZOOM_POS_WIDE + 180,
    ZOOM_POS_WIDE + 270,
    ZOOM_POS_WIDE + 360,
    ZOOM_POS_WIDE + 451,
    ZOOM_POS_WIDE + 541,
    ZOOM_POS_WIDE + 631,
    ZOOM_POS_WIDE + 721,
    ZOOM_POS_TELE
};

static INT16 g_pFocusZoomTblDefault[ZOOM_SECTION_TELE][FOCUS_ZOOM_TABLE_COLUMN]={
//  Inf , 0.5m, 0.2m, 0.1m
{    12 , -24 , -90,   0   },
{   190 , 154 ,  96,  14   },
{   297 , 261 , 202, 120   },
{   358 , 322 , 263, 181   },
{   387 , 350 , 292,   0   },
{   393 , 356 , 298,   0   },
{   382 , 346 , 287,   0   },
{   358 , 322 , 263,   0   },
{   324 , 288 , 229,   0   },
{   283 , 247 , 188,   0   }
};

static UINT16 g_pFocusZoomTblDist[FOCUS_ZOOM_TABLE_COLUMN]=
{ 0xffff, 50 , 20 , 10   };

static UINT16 g_pApertureZoomTbl[(ZOOM_SECTION_TELE+1)] = {
    0,
    36,
    39,
    43,
    46,
    49,
    52,
    55,
    58,
    61,
    64
};

static UINT16 g_pFocusLengthTbl[(ZOOM_SECTION_TELE+1)] = {
    0,
    75,
    89,
    103,
    117,
    131,
    145,
    159,
    173,
    187,
    201
};

static LENS_TAB g_pDZ5311Obj =
{
    //Lens
    lensDZ5311_init,
    lensDZ5311_isFFLens,
    //Zoom
    lensDZ5311_zoom_initPart1,
    lensDZ5311_zoom_initPart2,
    lensDZ5311_zoom_ctrlRelease,
    lensDZ5311_zoom_ctrlPress,
    lensDZ5311_zoom_getCurrentSection,
    lensDZ5311_zoom_getMaxSection,
    lensDZ5311_zoom_getNextSection,
    lensDZ5311_zoom_retract,
    lensDZ5311_zoom_gotoSection,
    lensDUMMY_zoom_setAutoBoundaryCheck,
    lensDZ5311_zoomIsr_enable,
    //Focus
    lensDZ5311_focus_gotoPosition,
    lensDZ5311_focus_gotoHome,
    lensDZ5311_focus_gotoGarage,
    lensDZ5311_focus_getCurrentPosition,
    lensDZ5311_focus_getMaxPosition,
    lensDZ5311_focus_getFL,
    lensDZ5311_focus_getZoomTblValue,
    //Aperture
    lensDUMMY_aperture_gotoState,
    lensDUMMY_aperture_getState,
    lensDZ5311_aperture_getFNO,
    //Shutter
    lensDUMMY_shutter_gotoState,
    lensDUMMY_shutter_getPulsePeriod,
    //Others
    NULL
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensDZ5311_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    LENS_MSG(("LENS_M: %s\r\n", __func__));

    g_pDZ5311Obj.pMotor = pMotorTab;

    //add by randy
    //Zoom ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID, GPIO_INTTYPE_EDGE, GPIO_INTPOL_POSHIGH);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID, lensDZ5311_zoomIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID);
}

/**
    Get DZ5311 lens object

    @return DZ5311 lens object pointer

*/
PLENS_TAB lensDZ5311_getObject(void)
{
    return &g_pDZ5311Obj;
}

BOOL lensDZ5311_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with LensTask.
//--------------------------------------------------------------------------------------------------------------------------
static void lensDZ5311_task_zoomForward(UINT32 step, BOOL bWaitZoomFinish)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    lensCtrlParamObj.uiZoomStep = step;
    lensCtrlParamObj.bWaitZoomFinish = bWaitZoomFinish;

    LensCtrl_SetParam(&lensCtrlParamObj);

    g_pZoomPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD);

    if(bWaitZoomFinish)
        LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
}

static void lensDZ5311_task_zoomReverse(UINT32 step, BOOL bWaitZoomFinish)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    lensCtrlParamObj.uiZoomStep = step;
    lensCtrlParamObj.bWaitZoomFinish = bWaitZoomFinish;

    LensCtrl_SetParam(&lensCtrlParamObj);

    g_pZoomPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMREV);

    if(bWaitZoomFinish)
        LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
}

static void lensDZ5311_task_zoomBrake(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMBRK);
}

static void lensDZ5311_task_doCallBack(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMCHANGE);
}

static void lensDZ5311_task_focusForward(UINT32 step)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    lensCtrlParamObj.uiFocusStep = step;

    LensCtrl_SetParam(&lensCtrlParamObj);

    set_flg(FLG_ID_LENS, FLGLENS_FOCUSFWD);
    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
}

static void lensDZ5311_task_focusReverse(UINT32 step)
{
    LENSCTRL_PARAMOBJ lensCtrlParamObj;

    lensCtrlParamObj.uiFocusStep = step;

    LensCtrl_SetParam(&lensCtrlParamObj);

    set_flg(FLG_ID_LENS, FLGLENS_FOCUSREV);
    LensCtrl_WaitCmdFinish(LENSCTRL_WAIT_INFINITE);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom ISR.
//--------------------------------------------------------------------------------------------------------------------------
static void lensDZ5311_zoomIsr_enable(BOOL enable)
{
    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensDZ5311: pMotor is null\r\n"));
        return;
    }

    g_pZoomPara.bZoomIsr = enable;
    g_pDZ5311Obj.pMotor->zoom_setPIInt(enable);
}

static void lensDZ5311_zoomIsr(void)
{
    static UINT32 uiMOB2 = 0;

    if(!g_pZoomPara.bZoomIsr)
        return;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensDZ5311: pMotor is null\r\n"));
        return;
    }

    if(g_pDZ5311Obj.pMotor->zoom_checkPIInt())
    {
        g_pDZ5311Obj.pMotor->zoom_clearPIInt();
        uiMOB2 = (uiMOB2+1)%2;  // 2 MOB count = 1 zoom step

        if(uiMOB2 == 0)
        {
            if(g_pZoomPara.Direction == DIRECTION_FWD)
            {
                g_pZoomPara.ZoomStep++;
            }
            else
            {
                g_pZoomPara.ZoomStep--;
            }

            //debug_err(("### g_pZoomPara.ZoomStep = %d\r\n", g_pZoomPara.ZoomStep));

            switch(g_pZoomPara.uiCurrentState)
            {
            case ZOOM_STATE_NULL:
            case ZOOM_STATE_INSIDEINIT:
            case ZOOM_STATE_OUTSIDEINIT:
            case ZOOM_STATE_INITPART1:
                lensDZ5311_zoom_changeState();
                break;
            case ZOOM_STATE_INITPART2:
            case ZOOM_STATE_IDLE:
                //do nothing
                break;
            case ZOOM_STATE_RUNNING:
                if(g_pZoomPara.uiLastSection != lensDZ5311_zoom_getCurrentSection())
                {
                    g_pZoomPara.uiLastSection = lensDZ5311_zoom_getCurrentSection();
                    lensDZ5311_task_doCallBack();
                }
                break;
            case ZOOM_STATE_CLOSING:
            case ZOOM_STATE_CLOSED:
                //do nothing
                break;
            case ZOOM_STATE_FAIL:
                LENS_ERR(("LENS_E: lensST8002: zoom isr fail state\r\n"));
                break;
            default:
                LENS_ERR(("LENS_E: lensST8002: zoom isr wrong state = %d\r\n", g_pZoomPara.uiCurrentState));
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensDZ5311_zoom_initPart1(void)
{
    LENS_ERR(("LENS_M: lensDZ5311: zoom initPart1\r\n"));

    g_pZoomPara.uiCurrentState = ZOOM_STATE_NULL;

    /* Change State from ZOOM_STATE_NULL to ZOOM_STATE_INITPART2 */
    lensDZ5311_zoom_changeState();

    return ERR_OK;
}

static INT32 lensDZ5311_zoom_initPart2(void)
{
    if(lensDZ5311_zoom_gotoState(ZOOM_STATE_INITPART2) != ERR_OK)
        return ERR_OPERATION_FAIL;

    if(g_pZoomPara.ZoomStep > ZOOM_POS_HOME_PR)
        lensDZ5311_task_zoomReverse(g_pZoomPara.ZoomStep - ZOOM_POS_HOME_PR, TRUE);

    LENS_ERR(("LENS_M: lensDZ5311: zoom initPart2 finish, cur ZoomStep=%d\r\n", g_pZoomPara.ZoomStep));

    /* Enter ZOOM_STATE_IDLE */
    lensDZ5311_zoom_changeState();

    g_pZoomPara.uiLastSection = lensDZ5311_zoom_getCurrentSection();

    return ERR_OK;
}

static INT32 lensDZ5311_zoom_gotoState(LENS_ZOOM_STATE newState)
{
    UINT16 cnt=0;
    UINT16 FocusTimeOut=FOCUS_SPEED_PER_STEP * (FOCUS_BWD_LIMIT_OUTSIDE_PI + FOCUS_BL_STEP*2 + 100);   //100 is buffer.
    UINT16 ZoomTimeOut=ZOOM_SPEED_PER_STEP * (g_pZoomSectionPulse[ZOOM_SECTION_TELE] + ZOOM_BL_STEP*2 + 100);   //100 is buffer.

    switch(newState)
    {
    case ZOOM_STATE_CLOSING:
        LENS_MSG(("LENS_M: /* Current State=%d --> ZOOM_STATE_CLOSING */\r\n", g_pZoomPara.uiCurrentState));

        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSING;
        break;

    case ZOOM_STATE_CLOSED:
        LENS_MSG(("LENS_M: /* Current State=%d --> ZOOM_STATE_CLOSED */\r\n", g_pZoomPara.uiCurrentState));

        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSED;
        break;

    default:

        while(g_pZoomPara.uiCurrentState != newState)
        {
            TimerDelayMs(1);
            if(cnt++>(FocusTimeOut+ZoomTimeOut))
            {
                LENS_ERR(("LENS_E: lensDZ5311: Zoom Init Timeout, stop state=%d, cur step=%d\r\n", g_pZoomPara.uiCurrentState, g_pZoomPara.ZoomStep));

                lensDZ5311_task_zoomBrake();
                g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;
                return ERR_OPERATION_FAIL;
            }
        }
    }

    return ERR_OK;
}

static void lensDZ5311_zoom_changeState(void)
{
    UINT32 count = 0;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: %s: pMotor is null\r\n", __func__));
        return;
    }

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_NULL:

        if(g_pZoomPara.bZoomIsr)
        {
            if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

                LENS_ERR(("LENS_M: Focus retract\r\n"));
                g_pDZ5311Obj.pMotor->changeMode(MOTOR_MODE_FOCUS);
                lensDZ5311_focus_gotoHome();
                LENS_ERR(("LENS_M: Zoom Out\r\n"));
                g_pDZ5311Obj.pMotor->changeMode(MOTOR_MODE_ZOOM);
                lensDZ5311_task_zoomReverse(ZOOM_POS_OUTER_STOP, FALSE);
            }
            else
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

                LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

                lensDZ5311_task_zoomForward(ZOOM_POS_OUTER_STOP, FALSE);
            }
        }
        else
        {

            if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

                LENS_ERR(("LENS_M: Focus retract\r\n"));

                lensDZ5311_focus_gotoHome();

                LENS_ERR(("LENS_M: Zoom Out\r\n"));
                while(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
                {
                    lensDZ5311_task_zoomReverse(100, TRUE);
                    count += 100;

                    if(count > ZOOM_POS_OUTER_STOP)
                    {
                        LENS_ERR(("LENS_M: Zoom Out, Fail..\r\n"));
                        break;
                    }
                }
            }

            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

            count = 0;

            while(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                lensDZ5311_task_zoomForward(4, TRUE);
                count += 4;

                if(count > ZOOM_POS_TELE)
                {
                    LENS_ERR(("LENS_M: Zoom In, Fail..\r\n"));
                    break;
                }
            }

            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_INITPART2 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART2;

            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;
        }

        break;

    case ZOOM_STATE_FAIL:
        break;

    case ZOOM_STATE_INSIDEINIT:

        if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;

            lensDZ5311_task_zoomBrake();

            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_INITPART2 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART2;
        }
        break;

    case ZOOM_STATE_OUTSIDEINIT:

        if(g_pDZ5311Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_OUTSIDEINIT --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            lensDZ5311_task_zoomForward(ZOOM_POS_OUTER_STOP, FALSE);
        }
        break;

    case ZOOM_STATE_INITPART1:
        break;

    case ZOOM_STATE_INITPART2:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART2 --> ZOOM_STATE_IDLE */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;

        break;
    case ZOOM_STATE_IDLE:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_IDLE --> ZOOM_STATE_RUNNING */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_RUNNING;

        break;
    case ZOOM_STATE_RUNNING:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_RUNNING --> ZOOM_STATE_IDLE */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_IDLE;

        break;
    case ZOOM_STATE_CLOSING:

        LENS_MSG(("LENS_M: /* ZOOM_STATE_CLOSING --> ZOOM_STATE_CLOSED */\r\n"));
        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSED;

        break;
    case ZOOM_STATE_CLOSED:
        //do nothing
        break;
    default:
        LENS_ERR(("LENS_E: lensDZ5311: zoom change unknown state\r\n"));
    }
}

static void lensDZ5311_zoom_doBL()
{
    lensDZ5311_task_zoomReverse(FOCUS_BL_STEP, TRUE);
    //TimerDelayMs(100)
    lensDZ5311_task_zoomForward(FOCUS_BL_STEP, TRUE);
}

static void lensDZ5311_zoom_gotoSection(UINT8 section)
{
    UINT32 step;

    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
    {
        step = g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section];
        lensDZ5311_task_zoomReverse(step, TRUE);
        lensDZ5311_zoom_doBL();

        if(g_pZoomPara.bZoomIsr == FALSE)
            g_pZoomPara.ZoomStep -= step;
    }
    else if(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[section])
    {
        step = g_pZoomSectionPulse[section] - g_pZoomPara.ZoomStep;
        lensDZ5311_task_zoomForward(step, TRUE);

        if(g_pZoomPara.bZoomIsr == FALSE)
            g_pZoomPara.ZoomStep += step;
    }

    lensDZ5311_task_doCallBack();
}

static UINT8 lensDZ5311_zoom_getTargetSection(void)
{
    unsigned char i=ZOOM_SECTION_WIDE;

    if(g_pZoomPara.Direction == DIRECTION_FWD)
    {
        for(i=ZOOM_SECTION_WIDE; i<=ZOOM_SECTION_TELE; i++)
        {
            if(g_pZoomPara.ZoomStep <= g_pZoomSectionPulse[i])
                break;
        }
    }
    else if(g_pZoomPara.Direction == DIRECTION_BWD)
    {
        for(i=ZOOM_SECTION_TELE; i>=ZOOM_SECTION_WIDE; i--)
        {
            if(g_pZoomPara.ZoomStep >= g_pZoomSectionPulse[i])
                break;
        }
    }
    if(i>ZOOM_SECTION_TELE)
        i = ZOOM_SECTION_TELE;
    else if(i<ZOOM_SECTION_WIDE)
        i = ZOOM_SECTION_WIDE;

    return i;
}

static UINT8 lensDZ5311_zoom_getCurrentSection(void)
{
    unsigned char i;
    for(i=ZOOM_SECTION_WIDE; i<ZOOM_SECTION_TELE; i++)
    {
        if(g_pZoomPara.ZoomStep <= ((g_pZoomSectionPulse[i]+g_pZoomSectionPulse[i+1])/2))
            break;
    }
   // debug_err(("lensDZ5311_zoom_getCurrentSection = %d[%d]\n\r",i,g_pZoomPara.ZoomStep));
    return i;
}

static UINT8 lensDZ5311_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

static UINT8 lensDZ5311_zoom_getNextSection(void)
{
    unsigned char i;

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
    {
        i = lensDZ5311_zoom_getTargetSection();
    }
    else
    {
        i = lensDZ5311_zoom_getCurrentSection();
    }
    if(i>ZOOM_SECTION_TELE)
        i=ZOOM_SECTION_TELE;
    else if (i<ZOOM_SECTION_WIDE)
        i=ZOOM_SECTION_WIDE;
    return i;
}

static INT32 lensDZ5311_zoom_ctrlRelease(void) // return current zoom section
{
    UINT32 uiTargetSection;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: %s: pMotor is null\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.bZoomIsr == TRUE)
    {
        uiTargetSection = lensDZ5311_zoom_getTargetSection();

        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            while(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[uiTargetSection]-ZOOM_OVERRUN_COUNT)
            {
                TimerDelayMs(1);
            }

            lensDZ5311_task_zoomBrake();
            lensDZ5311_zoom_changeState();
        }
        else
        {
            while(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[uiTargetSection]+ZOOM_OVERRUN_COUNT)
            {
                TimerDelayMs(1);
            }

            lensDZ5311_task_zoomBrake();
            lensDZ5311_zoom_doBL();
            lensDZ5311_zoom_changeState();
        }

        LENS_MSG(("LENS_M: %s: cur section = %d, step = %d\r\n",
                  __func__, lensDZ5311_zoom_getCurrentSection(), g_pZoomPara.ZoomStep));
    }

    return lensDZ5311_zoom_getCurrentSection();
}

static INT32 lensDZ5311_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    UINT32 step;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
        return ERR_OPERATION_FAIL;

    if(Zoom_IN == LENS_ZOOM_IN)
    {
        if(lensDZ5311_zoom_getCurrentSection()<ZOOM_SECTION_TELE)
        {
            if(g_pZoomPara.bZoomIsr == FALSE)
            {
                lensDZ5311_zoom_gotoSection(lensDZ5311_zoom_getCurrentSection()+1);

            }
            else
            {
                step = ZOOM_POS_TELE - g_pZoomPara.ZoomStep;
                lensDZ5311_task_zoomForward(step, FALSE);
                /* ZOOM_STATE_IDLE -> ZOOM_STATE_RUNNING */
                lensDZ5311_zoom_changeState();
            }
        }
    }
    else
    {
        if(lensDZ5311_zoom_getCurrentSection()>ZOOM_SECTION_WIDE)
        {
            if(g_pZoomPara.bZoomIsr == FALSE)
            {
                lensDZ5311_zoom_gotoSection(lensDZ5311_zoom_getCurrentSection()-1);
            }
            else
            {
                step = g_pZoomPara.ZoomStep - ZOOM_POS_WIDE;
                lensDZ5311_task_zoomReverse(step, FALSE);
                /* ZOOM_STATE_IDLE -> ZOOM_STATE_RUNNING */
                lensDZ5311_zoom_changeState();
            }
        }
    }

    return ERR_OK;
}

static INT32 lensDZ5311_zoom_retract( void )
{
    UINT32 count = 0;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
    {
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, g_pZoomPara.uiCurrentState));
        return ERR_OPERATION_FAIL;
    }

    LENS_ERR(("LENS_M: lensDZ5311: Zoom Retract\r\n"));

    /* Enter ZOOM_STATE_CLOSING */
    lensDZ5311_zoom_gotoState(ZOOM_STATE_CLOSING);

    lensDZ5311_focus_gotoHome();

    if(g_pZoomPara.ZoomStep > ZOOM_POS_GARAGE)
        lensDZ5311_task_zoomReverse(g_pZoomPara.ZoomStep - ZOOM_POS_GARAGE, TRUE);

    lensDZ5311_zoom_gotoState(ZOOM_STATE_CLOSED);

    return ERR_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensDZ5311_focus_runStep(UINT8 dir, UINT16 step)
{
    if(dir==DIRECTION_FWD)
    {
        lensDZ5311_task_focusForward(step);
    }
    else
    {
        lensDZ5311_task_focusReverse(step);
    }
}

static void lensDZ5311_focus_gotoPosition(INT16 position)
{
    unsigned short Step;
    unsigned char Dir;

    if(g_pFocusPara.iFocusPosition > position)
    {
        Dir = DIRECTION_BWD;
        Step = g_pFocusPara.iFocusPosition - position;
    }
    else
    {
        Dir = DIRECTION_FWD;
        Step = position - g_pFocusPara.iFocusPosition;
    }

    if(Step==0)
        return;

    if(Step > (FOCUS_POS_OUTER_STOP-FOCUS_POS_INNER_STOP))
    {
        LENS_ERR(("LENS_E: %s: Step (%d) is overflow\r\n", __func__, Step));
        return;
    }

    if(Dir == DIRECTION_BWD)
    {
#if 1
        lensDZ5311_focus_runStep(Dir,Step+FOCUS_BL_STEP);
        TimerDelayMs(10);//?
        lensDZ5311_focus_runStep(DIRECTION_FWD, FOCUS_BL_STEP);
#else
        lensDZ5311_focus_runStep(Dir,Step);
#endif
        g_pFocusPara.iFocusPosition -= Step;
    }
    else
    {
#if 0
        lensDZ5311_focus_runStep(Dir,Step+FOCUS_BL_STEP);
        TimerDelayMs(10);
        lensDZ5311_focus_runStep(DIRECTION_BWD, FOCUS_BL_STEP);
#else
        lensDZ5311_focus_runStep(Dir,Step);
#endif
        g_pFocusPara.iFocusPosition += Step;
    }
}

static INT32 lensDZ5311_focus_gotoHome(void)
{
    unsigned short sBoundary;

    if(g_pDZ5311Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: %s: pMotor is null\r\n", __func__));
        return ERR_OPERATION_FAIL;
    }

    //find PI
    sBoundary = 0;
    while(g_pDZ5311Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensDZ5311_focus_runStep(DIRECTION_BWD,100);    //run 10 steps every time for speed
        sBoundary += 100;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            LENS_ERR(("LENS_E: B1: %d>%d",sBoundary, FOCUS_BWD_LIMIT_OUTSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    sBoundary = 0;
    while(g_pDZ5311Obj.pMotor->focus_checkPI() == FOCUSPI_INSDIE_LEVEL)
    {
        lensDZ5311_focus_runStep(DIRECTION_FWD,4);
        sBoundary += 4;
        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            LENS_ERR(("LENS_E: B2: %d>%d",sBoundary, FOCUS_FWD_LIMIT_INSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;

    return ERR_OK;
}

static INT32 lensDZ5311_focus_gotoGarage(void)
{
    if(lensDZ5311_focus_gotoHome() == ERR_OK)
    {
        lensDZ5311_focus_gotoPosition(FOCUS_POS_GARAGE);
        return ERR_OK;
    }
    else
    {
        return ERR_OPERATION_FAIL;
    }
}

static INT16 lensDZ5311_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensDZ5311_focus_getMaxPosition(void)
{
    return FOCUS_POS_NEAR;
}

static UINT32 lensDZ5311_focus_getFL(UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pFocusLengthTbl[zoomSection];
}

static INT16 lensDZ5311_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    if(index > FOCUS_ZOOM_TABLE_COLUMN-1)
        return 0;

    return g_pFocusZoomTblDefault[zoomSection-1][index];
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

static UINT32 lensDZ5311_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pApertureZoomTbl[zoomSection];
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------


