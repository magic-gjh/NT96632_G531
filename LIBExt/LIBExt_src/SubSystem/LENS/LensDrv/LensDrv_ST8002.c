/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_ST8002.c
    @ingroup    mISYSLens

    @brief      ST8002 lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

/*
    Todo: Chris
    1. Move API related functions to Lens_API.c
    2. Remove power on/off and mode enable/disable function-call in lens action functions.
    3. Refine state machine flags and add change state functions.

        void lensST8002_zoom_changeState(void)
        {
          switch(origState)

          case Zoom_State_Null:

            Zoom_Init_Start();
            state= Zoom_State_InitPart1;

            break;

          case Zoom_State_InitPart1:

            Zoom_init_Finish();
            state= Zoom_State_InitPart2;

            break;
        }

    4. Use the functions in motor function table instead of original hard coding functions.
    5. Re-write function name.
    6. Define lens funciton table in Lens.h
*/
/*
    Todo:
    1. Modify global variable name.
    2. Modify filename as LensDrv_xxx
*/

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include "Interrupt.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "LensDrv_ST8002.h"
#include "GPIO.h"
#include "debug.h"
#include "TOP.h"
#include "Timer.h"
#include "Type.h"
#include "Utility.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------

//lens
static void     lensST8002_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensST8002_isFFLens(void);

//zoom
static INT32    lensST8002_zoom_initPart1(void);
static INT32    lensST8002_zoom_initPart2(void);
static INT32    lensST8002_zoom_ctrlRelease(void); // return current zoom section
static INT32    lensST8002_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
static UINT8    lensST8002_zoom_getCurrentSection(void);
static UINT8    lensST8002_zoom_getNextSection(void);
static UINT8    lensST8002_zoom_getMaxSection(void);
static INT32    lensST8002_zoom_retract( void );
static void     lensST8002_zoom_gotoSection(UINT8 section);
static void     lensST8002_zoom_setAutoBoundaryCheck(BOOL en);
static INT32    lensST8002_zoom_gotoState(LENS_ZOOM_STATE newState);
static void     lensST8002_zoom_changeState(void);
static void     lensST8002_zoom_enablePWM(BOOL enable);
static void     lensST8002_zoom_hdlSpeed(void);

//zoom ISR
static void     lensST8002_zoomIsr(void);
static void     lensST8002_zoomIsr_doNormal(void);
static void     lensST8002_zoomIsr_enable(BOOL enable);

//focus
static void     lensST8002_focus_initPhase(void);
static void     lensST8002_focus_gotoPosition(INT16 position);
static INT32    lensST8002_focus_gotoHome(void);
static INT32    lensST8002_focus_gotoGarage(void);
static void     lensST8002_focus_runStep(UINT8 dir, UINT16 step);
static INT16    lensST8002_focus_getCurrentPosition(void);
static INT16    lensST8002_focus_getMaxPosition(void);
static UINT32   lensST8002_focus_getFL(UINT32 zoomSection);

//aperture
static void     lensST8002_aperture_setMotorDriver(LENS_IRIS_PHASE phase);
static INT32    lensST8002_aperture_gotoState(LENS_APERTURE_ACT position);
static UINT32   lensST8002_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);
static LENS_APERTURE_ACT lensST8002_aperture_getState(void);

//shutter
static INT32    lensST8002_shutter_gotoState(LENS_SHUTTER_ACT newState);

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------

static ZOOM_Struct g_pZoomPara={0};
static FOCUS_Struct g_pFocusPara={0};
static IRIS_Struct g_pIrisPara={0};
static SHUTTER_Struct g_pShutterPara={0};

static UINT16 g_pZoomSectionPulse[(ZOOM_SECTION_TELE+1)]=
{
    ZOOM_POS_GARAGE,        //=3
    ZOOM_POS_WIDE,          //=104
    ZOOM_POS_WIDE + 38/2,   //=123
    ZOOM_POS_WIDE + 61/2,   //=134
    ZOOM_POS_WIDE + 90/2,   //=149
    ZOOM_POS_WIDE + 128/2,  //=168
    ZOOM_POS_WIDE + 181/2,  //=194
    ZOOM_POS_TELE           //=233
};

static UINT16 g_pFocusZoomTblDefault[ZOOM_SECTION_TELE][FOCUS_ZOOM_TABLE_COLUMN]={
//  Inf , 5m  , 2m  , 1.4m, 1m  , 0.8m, 0.6m, 0.5m, 0.4m, 0.3m, 0.05m
{   97  , 101 , 106 , 109 , 114 , 117 , 124 , 130 , 138 , 150 , 337   },
{   90  , 95  , 102 , 106 , 112 , 117 , 125 , 133 , 143 , 159 , 337   },
{   81  , 87  , 96  , 102 , 110 , 116 , 127 , 138 , 151 , 172 , 337   },
{   70  , 79  , 91  , 99  , 111 , 118 , 133 , 147 , 165 , 192 , 337   },
{   56  , 68  , 85  , 96  , 113 , 123 , 144 , 163 , 187 , 224 , 337   },
{   36  , 54  , 80  , 97  , 121 , 136 , 166 , 193 , 226 , 278 , 337   },
{    6  , 37  , 80  , 107 , 144 , 168 , 212 , 253 , 300 , 371 , 337   }
};

static UINT16 g_pFocusZoomTblDist[FOCUS_ZOOM_TABLE_COLUMN]=
{ 0xffff, 500 , 200 , 140 , 100 ,  80 ,  60 ,  50 ,  40 ,  30 ,   5   };

static UINT16 g_pApertureZoomTbl[(ZOOM_SECTION_TELE+1)][2] = {
    { 0,  0},
    {28, 47},
    {31, 52},
    {35, 57},
    {39, 63},
    {42, 69},
    {45, 74},
    {48, 80},
};

static UINT16 g_pFocusLengthTbl[(ZOOM_SECTION_TELE+1)] =
{0, 61, 80, 100, 119, 140, 160, 183};

static UINT g_uiZoomTimerID = LENS_NULL_TIMER;
static UINT8 g_uiIrisCurPos = IRIS_POS_BIG;     //Record current IRIS position.

#if (SHUTTER_CLOSE_INT_VERSION == ENABLE)
static UINT g_uiApertureTimerID = LENS_NULL_TIMER;
volatile static UINT g_uiHandlerMaxStep = 0;
volatile static UINT g_uiHandlerStep =  0;

//Refer to ST-8002 Ver:0 2006.12.01
static UINT32 g_pApertureTimeCnt[IRIS_TOTAL_NONCLOSE_POS_NUM][IRIS_TOTAL_POWERSEQ] =
{
    //(BIG->CLOSE)
    {3000, 3500, 1200, 15300, 0},
    //(SMALL->CLOSE)
    {3000, 3000, 1000, 16000, 0}
};
#endif

static UINT32 g_pApertureCtrl[IRIS_TOTAL_NONCLOSE_POS_NUM][IRIS_TOTAL_POWERSEQ] =
{
    //LL,HH,HL,HH,OFF   (BIG->CLOSE)
    {IRIS_PHASE_LL, IRIS_PHASE_HH, IRIS_PHASE_HL, IRIS_PHASE_HH, IRIS_PHASE_OFF},
    //LL,HH,LH,HH,OFF   (SMALL->CLOSE)
    {IRIS_PHASE_LL, IRIS_PHASE_HH, IRIS_PHASE_LH, IRIS_PHASE_HH, IRIS_PHASE_OFF}
};

static UINT8 g_pFocusStepperPhase[FOCUS_TOTAL_PHASE+1] =
{
    //Input=(IN1, IN2, IN3, IN4)    //Output=(O2,O3,O6,O7)=(/B,B,/A,A)
    STEPPER_PHASE_M4(1,0,0,1),      //0x1001 = 9
    STEPPER_PHASE_M4(0,0,0,1),      //0x1000 = 8
    STEPPER_PHASE_M4(0,1,0,1),      //0x1010 = a
    STEPPER_PHASE_M4(0,1,0,0),      //0x0010 = 2
    STEPPER_PHASE_M4(0,1,1,0),      //0x0110 = 6
    STEPPER_PHASE_M4(0,0,1,0),      //0x0100 = 4
    STEPPER_PHASE_M4(1,0,1,0),      //0x0101 = 5
    STEPPER_PHASE_M4(1,0,0,0),      //0x0001 = 1
    STEPPER_PHASE_M4(0,0,0,0)       //off

};

static UINT8 g_pIrisStepperPhase[IRIS_PHASE_TOTAL_NUM] =
{
    //Input=(IN1, IN2, IN3, IN4)    //Output=(O1,O5)=TerminalName(V01,V03)
    STEPPER_PHASE_M4(1,0,1,0),      //HH
    STEPPER_PHASE_M4(0,1,1,0),      //LH
    STEPPER_PHASE_M4(1,0,0,1),      //HL
    STEPPER_PHASE_M4(0,1,0,1),      //LL
    STEPPER_PHASE_M4(0,0,0,0)       //off
};

static LENS_TAB g_pST8002Obj =
{
    //Lens
    lensST8002_init,
    lensST8002_isFFLens,
    //Zoom
    lensST8002_zoom_initPart1,
    lensST8002_zoom_initPart2,
    lensST8002_zoom_ctrlRelease,
    lensST8002_zoom_ctrlPress,
    lensST8002_zoom_getCurrentSection,
    lensST8002_zoom_getMaxSection,
    lensST8002_zoom_getNextSection,
    lensST8002_zoom_retract,
    lensST8002_zoom_gotoSection,
    lensST8002_zoom_setAutoBoundaryCheck,
    lensST8002_zoomIsr_enable,
    //Focus
    lensST8002_focus_gotoPosition,
    lensST8002_focus_gotoHome,
    lensST8002_focus_gotoGarage,
    lensST8002_focus_getCurrentPosition,
    lensST8002_focus_getMaxPosition,
    lensST8002_focus_getFL,
    //Aperture
    lensST8002_aperture_gotoState,
    lensST8002_aperture_getState,
    lensST8002_aperture_getFNO,
    //Shutter
    lensST8002_shutter_gotoState,
    //Others
    NULL
};

static MOTOR_PWM_TAB g_pST8002PWMTab =
{
    FALSE,
    _ZOOM_PSEUDO_PWM_PERIOD_,
    _ZOOM_PSEUDO_PWM_ONTIME_
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8002_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    g_pST8002Obj.pMotor = pMotorTab;

    //Zoom parameter
    g_pZoomPara.bZoomBoundaryAutoCheck=TRUE;
    g_pZoomPara.ZoomOverRun=ZOOM_OVERRUN_COUNT;// 3

    //Zoom ISR parameter
    gpio_setIntTypePol(pLensDev->uiMotorCtrlGPIOIntrID, GPIO_INTTYPE_EDGE, GPIO_INTPOL_POSHIGH);
    gpio_setIntIsr(pLensDev->uiMotorCtrlGPIOIntrID, lensST8002_zoomIsr);
    gpio_enableInt(pLensDev->uiMotorCtrlGPIOIntrID);

    //Focus parameter
    g_pFocusPara.uiFocusPhase = 0;

    //Aperture parameter
    g_uiIrisCurPos = IRIS_POS_CLOSE;
}

/**
    Get ST8002 lens object

    @return ST8002 lens object pointer

*/
PLENS_TAB lensST8002_getObject(void)
{
    return &g_pST8002Obj;
}

BOOL lensST8002_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with LensTask.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8002_zoom_doOff(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMOFF);
}

static void lensST8002_zoom_doBrake(void)
{
    LENS_MSG(("LENS_M: %s\r\n", __func__));

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
    if(g_uiZoomTimerID != LENS_NULL_TIMER)
    {
        timer_close(g_uiZoomTimerID);
        g_uiZoomTimerID = LENS_NULL_TIMER;
    }
#endif

    set_flg(FLG_ID_LENS, FLGLENS_ZOOMBRK);
}

static void lensST8002_zoom_doForward(void)
{
    g_pZoomPara.Direction = DIRECTION_FWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMFWD);

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
    if(g_pST8002PWMTab.bPwmCtrl && g_uiZoomTimerID == LENS_NULL_TIMER)
    {
        if (timer_open(&g_uiZoomTimerID, lensST8002_zoom_hdlSpeed) == 0)
        {
            g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;
            timer_set_us(g_uiZoomTimerID,
                         (_ZOOM_PSEUDO_PWM_SPEED_ * _ZOOM_PSEUDO_PWM_CHECKCOUNT_),
                         _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                         _TIMER_PLAY);
        }
        else
        {
            LENS_ERR(("LENS_E: timer_open fail (speed check)\r\n"));
        }
    }
#endif
}

static void lensST8002_zoom_doReverse(void)
{
    g_pZoomPara.Direction = DIRECTION_BWD;
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMREV);

#if (_ZOOM_PSEUDO_PWM_ == ENABLE)
    if(g_pST8002PWMTab.bPwmCtrl && g_uiZoomTimerID == LENS_NULL_TIMER)
    {
        if (timer_open(&g_uiZoomTimerID, lensST8002_zoom_hdlSpeed) == 0)
        {
            g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;
            timer_set_us(g_uiZoomTimerID,
                         (_ZOOM_PSEUDO_PWM_SPEED_ * _ZOOM_PSEUDO_PWM_CHECKCOUNT_),
                         _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE,
                         _TIMER_PLAY);
        }
        else
        {
            LENS_ERR(("LENS_E: timer_open fail (speed check)\r\n"));
        }
    }
#endif
}

static void lensST8002_zoom_doCallBack(void)
{
    set_flg(FLG_ID_LENS, FLGLENS_ZOOMCHANGE);
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom ISR.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8002_zoomIsr_enable(BOOL enable)
{
    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    g_pZoomPara.bZoomIsr = enable;
    g_pST8002Obj.pMotor->zoom_setPIInt(enable);
}

static void lensST8002_zoomIsr(void)
{
    if(!g_pZoomPara.bZoomIsr)
        return;

    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    if(g_pST8002Obj.pMotor->zoom_checkPIInt())
    {
        g_pST8002Obj.pMotor->zoom_clearPIInt();

        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            g_pZoomPara.ZoomStep++;
        }
        else
        {
            g_pZoomPara.ZoomStep--;
        }

        switch(g_pZoomPara.uiCurrentState)
        {
        case ZOOM_STATE_NULL:
        case ZOOM_STATE_INSIDEINIT:
        case ZOOM_STATE_OUTSIDEINIT:
        case ZOOM_STATE_INITPART1:
            lensST8002_zoom_changeState();
            break;
        case ZOOM_STATE_INITPART2:
            //do nothing
            break;
        case ZOOM_STATE_IDLE:
        case ZOOM_STATE_RUNNING:
            lensST8002_zoomIsr_doNormal();
            break;
        case ZOOM_STATE_CLOSING:
            if(g_pZoomPara.ZoomStep<=(g_pZoomSectionPulse[0] - (g_pZoomPara.ZoomOverRun-2)/*Closing-Zoom-overrun is smaller*/))
            {
                lensST8002_zoom_doBrake();

                /* Enter ZOOM_STATE_NULL */
                lensST8002_zoom_changeState();
            }
            break;
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

static void lensST8002_zoomIsr_doNormal(void)
{
    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(g_pZoomPara.Direction == DIRECTION_FWD)
        {
            if(g_pZoomPara.ZoomStep > ( ZOOM_POS_TELE - g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensST8002_zoom_doBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensST8002_zoom_changeState();
            }
        }
        else
        {
            if(g_pZoomPara.ZoomStep < ( ZOOM_POS_WIDE + g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
            {
                lensST8002_zoom_doBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                    lensST8002_zoom_changeState();
            }
        }
    }


    if(lensST8002_zoom_getNextSection() != lensST8002_zoom_getCurrentSection() ||
       lensST8002_zoom_getCurrentSection() == ZOOM_SECTION_TELE)
    {
        lensST8002_zoom_doCallBack();
    }

    if(((++g_pZoomPara.StepCnt + g_pZoomPara.ZoomOverRun) >= g_pZoomPara.StepMove))
    {
        lensST8002_zoom_doBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
           lensST8002_zoom_changeState();
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensST8002_zoom_initPart1(void)
{
    LENS_ERR(("LENS_M: lensST8002: zoom initPart1\r\n"));

    lensST8002_zoom_enablePWM(FALSE); //set full speed

    lensST8002_zoomIsr_enable(TRUE);

    g_pZoomPara.uiCurrentState = ZOOM_STATE_NULL;

    /* Change State from ZOOM_STATE_NULL to ZOOM_STATE_INITPART2 */
    lensST8002_zoom_changeState();

    return ERR_OK;
}

static INT32 lensST8002_zoom_initPart2(void)
{
    if(lensST8002_zoom_gotoState(ZOOM_STATE_INITPART2) != ERR_OK)
        return ERR_OPERATION_FAIL;

    TimerDelayMs(100);  //wait 100ms for brake stop

    LENS_ERR(("LENS_M: lensST8002: zoom initPart2 finish, cur ZoomStep=%d\r\n", g_pZoomPara.ZoomStep));

    /* Enter ZOOM_STATE_IDLE */
    lensST8002_zoom_changeState();

    lensST8002_zoom_enablePWM(TRUE); //set PWM speed

    return ERR_OK;
}

static INT32 lensST8002_zoom_gotoState(LENS_ZOOM_STATE newState)
{
    UINT16 cnt=0;
    UINT16 ZoomTimeOut=(_ZOOM_PSEUDO_PWM_SPEED_/1000) * (g_pZoomSectionPulse[ZOOM_SECTION_TELE] + ZOOM_BL_STEP*2 + 20);   //20 is buffer.

    switch(newState)
    {
    case ZOOM_STATE_CLOSING:
        LENS_MSG(("LENS_M: /* Current State=%d --> ZOOM_STATE_CLOSING */\r\n", g_pZoomPara.uiCurrentState));

        g_pZoomPara.uiCurrentState = ZOOM_STATE_CLOSING;
        break;

    default:

        while(g_pZoomPara.uiCurrentState != newState)
        {
            TimerDelayMs(1);
            if(cnt++>ZoomTimeOut)
            {
                LENS_ERR(("LENS_E: lensST8002: Zoom Init Timeout, stop state=%d, cur step=%d\r\n", g_pZoomPara.uiCurrentState, g_pZoomPara.ZoomStep));

                g_pZoomPara.uiCurrentState = ZOOM_STATE_FAIL;

                lensST8002_zoom_doBrake();

                return ERR_OPERATION_FAIL;
            }
        }
    }

    return ERR_OK;
}

static void lensST8002_zoom_changeState(void)
{
    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    switch(g_pZoomPara.uiCurrentState)
    {
    case ZOOM_STATE_NULL:

        if(g_pST8002Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_OUTSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_OUTSIDEINIT;

            LENS_ERR(("LENS_M: Focus retract\r\n"));
            g_pST8002Obj.pMotor->changeMode(MOTOR_MODE_FOCUS);
            lensST8002_focus_gotoGarage();
            LENS_ERR(("LENS_M: Zoom Out\r\n"));
            g_pST8002Obj.pMotor->changeMode(MOTOR_MODE_ZOOM);
            lensST8002_zoom_doReverse();
        }
        else
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_NULL --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            LENS_ERR(("LENS_M: Zoom In, state=%d\r\n", g_pZoomPara.uiCurrentState));

            lensST8002_zoom_doForward();
        }

        break;

    case ZOOM_STATE_FAIL:
        break;

    case ZOOM_STATE_INSIDEINIT:

        if(g_pST8002Obj.pMotor->zoom_checkPR() == ZOOMPR_OUTSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_INSIDEINIT --> ZOOM_STATE_INITPART1 */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART1;

            /* Set ZOOM PR Step*/
            g_pZoomPara.ZoomStep = ZOOM_POS_HOME_PR;

            lensST8002_zoom_doForward();
        }
        break;

    case ZOOM_STATE_OUTSIDEINIT:

        if(g_pST8002Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
        {
            LENS_MSG(("LENS_M: /* ZOOM_STATE_OUTSIDEINIT --> ZOOM_STATE_INSIDEINIT */\r\n"));

            g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;

            lensST8002_zoom_doForward();
        }

        break;

    case ZOOM_STATE_INITPART1:

        /*
            Check after 5/10 counts again, make sure we passed PR
            Somehow the PR we detected before is incorrect
        */
        if(g_pZoomPara.ZoomStep == (ZOOM_POS_HOME_PR+5) ||
           g_pZoomPara.ZoomStep == (ZOOM_POS_HOME_PR+10))
        {
            if(g_pST8002Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
            {
                LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART1 --> ZOOM_STATE_OUTSIDEINIT */\r\n"));
                g_pZoomPara.uiCurrentState = ZOOM_STATE_INSIDEINIT;
            }
        }

        /*  Zoom to Wide */
        if(g_pZoomPara.ZoomStep>=(g_pZoomSectionPulse[1]-g_pZoomPara.ZoomOverRun/*Zoom overrun*/))
        {
            lensST8002_zoom_doBrake();

            LENS_MSG(("LENS_M: /* ZOOM_STATE_INITPART1 --> ZOOM_STATE_INITPART2 */\r\n"));
            g_pZoomPara.uiCurrentState = ZOOM_STATE_INITPART2;
        }

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
        LENS_ERR(("LENS_E: lensST8002: zoom change unknown state\r\n"));
    }
}

static void lensST8002_zoom_runStep(unsigned char dir,unsigned short step)
{
    unsigned long StepCnt=0,count=0,Sts=0;

    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    TimerDelayMs(1);
    if(g_pZoomPara.Direction!= dir)
    {
        //[Chris Chung] What does it do?
        //Ans: There is 40~50% probability to stop on PI signal, it will cause step miss when running opposite direction.
        if(!g_pST8002Obj.pMotor->zoom_checkPI())
        {
            //JJ: I guess the position(ZoomStep) presents the "position of the nearest PI", not the "position of current PI-gap"
            //JJ: Even though, it still doesn't make sense.
            //JJ: This mechanism must be done in other ways, cause Zoom-step-miss is invited here!
            #if 1//JJ: temporally mark it
            if(dir==DIRECTION_FWD)
            {
                //lensalg_err(("<-->"));
                g_pZoomPara.ZoomStep--;
            }
            else
            {
                //lensalg_err(("<++>"));
                g_pZoomPara.ZoomStep++;
            }
            #endif
        }
        else
        {
            //lensalg_err(("<==>"));
        }
    }

    g_pZoomPara.StepMove = step;
    g_pZoomPara.StepCnt = 0;

    /* Enter ZOOM_STATE_RUNNING */
    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
        lensST8002_zoom_changeState();

    if(dir == DIRECTION_FWD)
        lensST8002_zoom_doForward();
    else
        lensST8002_zoom_doReverse();

    if(!g_pZoomPara.bZoomIsr)
    {
        while(1)
        {
            if(g_pST8002Obj.pMotor->zoom_checkPIInt())
            {
                g_pST8002Obj.pMotor->zoom_clearPIInt();

                StepCnt++;
                if(dir == DIRECTION_FWD)
                    g_pZoomPara.ZoomStep++;
                else
                    g_pZoomPara.ZoomStep--;
            }
            if((StepCnt+3) >= step && (Sts==0))
            {
                lensST8002_zoom_doBrake();
                Sts = 1;
            }
            if(Sts == 1)
            {
                count++;
            }
            if(count>50000)
            {
                Sts = 0;
                break;
            }
        }
    }

}

static void lensST8002_zoom_doBL(unsigned short BLSteps)
{
    lensST8002_zoom_runStep(DIRECTION_BWD,BLSteps);
    TimerDelayMs(100);
    lensST8002_zoom_runStep(DIRECTION_FWD,BLSteps);
}

static void lensST8002_zoom_gotoSection(UINT8 section)
{
    if(section>ZOOM_SECTION_TELE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pZoomPara.ZoomStep > g_pZoomSectionPulse[section])
    {
        lensST8002_zoom_runStep(DIRECTION_BWD, g_pZoomPara.ZoomStep - g_pZoomSectionPulse[section]);
    }
    else if(g_pZoomPara.ZoomStep < g_pZoomSectionPulse[section])
    {
        lensST8002_zoom_runStep(DIRECTION_FWD, g_pZoomSectionPulse[section] - g_pZoomPara.ZoomStep);
    }
}

static UINT8 lensST8002_zoom_getTargetSection(void)
{
    unsigned char i=ZOOM_SECTION_WIDE;

    if(g_pZoomPara.Direction == DIRECTION_FWD)
    {
        for(i=ZOOM_SECTION_WIDE; i<=ZOOM_SECTION_TELE; i++)
        {
            if(g_pZoomPara.ZoomStep <= (g_pZoomSectionPulse[i]-g_pZoomPara.ZoomOverRun-2/*2 steps before*/))
                break;
        }
    }
    else if(g_pZoomPara.Direction == DIRECTION_BWD)
    {
        for(i=ZOOM_SECTION_TELE; i>=ZOOM_SECTION_WIDE; i--)
        {
            if(g_pZoomPara.ZoomStep >= (g_pZoomSectionPulse[i]+g_pZoomPara.ZoomOverRun+2/*2 steps before*/))
                break;
        }
    }
    if(i>ZOOM_SECTION_TELE)
        i = ZOOM_SECTION_TELE;
    else if(i<ZOOM_SECTION_WIDE)
        i = ZOOM_SECTION_WIDE;

    return i;
}

static UINT8 lensST8002_zoom_getCurrentSection(void)
{
    unsigned char i;
    for(i=ZOOM_SECTION_WIDE; i<ZOOM_SECTION_TELE; i++)
    {
        if(g_pZoomPara.ZoomStep <= ((g_pZoomSectionPulse[i]+g_pZoomSectionPulse[i+1])/2))
            break;
    }
    return i;
}

static UINT8 lensST8002_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}

static UINT8 lensST8002_zoom_getNextSection(void)
{
    unsigned char i;

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
    {
        i = lensST8002_zoom_getTargetSection();
    }
    else
    {
        i = lensST8002_zoom_getCurrentSection();
    }
    if(i>ZOOM_SECTION_TELE)
        i=ZOOM_SECTION_TELE;
    else if (i<ZOOM_SECTION_WIDE)
        i=ZOOM_SECTION_WIDE;
    return i;
}

static INT32 lensST8002_zoom_ctrlRelease(void) // return current zoom section
{
    unsigned char TargetSection;

    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    g_pZoomPara.bDoOptZoomDraw=FALSE;//AO835@JJ@20080115 OpticalZoom Callback to Draw

    if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
    {
        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            lensST8002_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensST8002_zoom_doBL(ZOOM_BL_STEP);

            lensST8002_zoom_enablePWM(TRUE); //set PWM speed
        }

        LENS_MSG(("LENS_M: lensST8002_zoom_ctrlRelease: already idle state\r\n"));
        return lensST8002_zoom_getCurrentSection();
    }
    else if(g_pZoomPara.uiCurrentState != ZOOM_STATE_RUNNING)
    {
        LENS_ERR(("LENS_E: lensST8002_zoom_ctrlRelease: not in running state\r\n"));
        return ERR_OPERATION_FAIL;
    }

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        TargetSection=lensST8002_zoom_getTargetSection();

        lensST8002_zoom_gotoSection(TargetSection);

        if(lensST8002_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensST8002_zoom_ctrlRelease: go to IDLE state fail\r\n"));
            return ERR_OPERATION_FAIL;
        }

        if(g_pZoomPara.Direction == DIRECTION_BWD)
        {
            lensST8002_zoom_enablePWM(FALSE); //set full speed

            TimerDelayMs(100);//harry
            lensST8002_zoom_doBL(ZOOM_BL_STEP);

            lensST8002_zoom_enablePWM(TRUE); //set PWM speed
        }

        if(lensST8002_zoom_gotoState(ZOOM_STATE_IDLE) != ERR_OK)
        {
            LENS_ERR(("LENS_E: lensST8002_zoom_ctrlRelease: go to IDLE state fail[2]\r\n"));
            return ERR_OPERATION_FAIL;
        }
    }
    else
    {
        lensST8002_zoom_doBrake();

        /* Enter ZOOM_STATE_IDLE */
        if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
            lensST8002_zoom_changeState();
    }

    TimerDelayMs(200);

    //???? Zoom_OnOff(OFF);
    //???? Lens_OnOff(OFF); //motor power control. 0601

    //AO835@JJ@20080115 OpticalZoom Callback to Draw
    {
        lensST8002_zoom_doCallBack();
    }
    //~AO835@JJ@20080115 OpticalZoom Callback to Draw

    return lensST8002_zoom_getCurrentSection();

}

static INT32 lensST8002_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    UINT32 Step_Ori,Step_New;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
        return ERR_OPERATION_FAIL;

    g_pZoomPara.bDoOptZoomDraw=TRUE;//AO835@JJ@20080115 OpticalZoom Callback to Draw

    if(g_pZoomPara.bZoomBoundaryAutoCheck)
    {
        if(Zoom_IN == LENS_ZOOM_IN &&
           lensST8002_zoom_getCurrentSection()<ZOOM_SECTION_TELE)
        {
            lensST8002_zoom_gotoSection(ZOOM_SECTION_TELE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8002_zoom_changeState();
        }
        else if(Zoom_IN == LENS_ZOOM_OUT &&
                lensST8002_zoom_getCurrentSection() >ZOOM_SECTION_WIDE)
        {
            lensST8002_zoom_gotoSection(ZOOM_SECTION_WIDE);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8002_zoom_changeState();
        }
        else
            return ERR_OPERATION_FAIL;
    }
    else
    {
        if(Zoom_IN == LENS_ZOOM_IN)
        {
            lensST8002_zoom_runStep(DIRECTION_FWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8002_zoom_changeState();
        }
        else
        {
            lensST8002_zoom_runStep(DIRECTION_BWD, 0xffff);

            /* Enter ZOOM_STATE_RUNNING */
            if(g_pZoomPara.uiCurrentState == ZOOM_STATE_IDLE)
                lensST8002_zoom_changeState();
        }
    }
    //[Chris Chung] Check if the lens is moving. If it isn't, then return fail.
    Step_Ori = g_pZoomPara.ZoomStep;
    lensST8002_zoom_doCallBack();
    TimerDelayMs(40);
    lensST8002_zoom_doCallBack();
    Step_New = g_pZoomPara.ZoomStep;
    //lensalg_err(("Step_Ori = %d, Step_New=%d\r\n",Step_Ori,Step_New));
    if((abs(Step_New - Step_Ori)<2) && (g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING))
        return ERR_OPERATION_FAIL;
    else
    {
        //Lens_ZoomDCSpeed(255);//0
        return ERR_OK;
    }
}

static void lensST8002_zoom_setAutoBoundaryCheck(BOOL en)
{
    g_pZoomPara.bZoomBoundaryAutoCheck=en;
}

static INT32 lensST8002_zoom_retract( void )
{
    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_IDLE)
    {
        LENS_ERR(("LENS_E: %s: fail state=%d\r\n", __func__, g_pZoomPara.uiCurrentState));
        return ERR_OPERATION_FAIL;
    }

    LENS_ERR(("LENS_M: lensST8002: Zoom Retract\r\n"));

    lensST8002_zoom_enablePWM(FALSE); //set full speed

    if(_ZOOM_PSEUDO_PWM_ == ENABLE)
    {
        if(g_pST8002Obj.pMotor != NULL)
        {
            g_pST8002PWMTab.bPwmCtrl = FALSE;   //set full speed
            g_pST8002Obj.pMotor->setPWMInfo(&g_pST8002PWMTab);
        }
    }

    /* Enter ZOOM_STATE_CLOSING */
    lensST8002_zoom_gotoState(ZOOM_STATE_CLOSING);

    if(g_pZoomPara.ZoomStep<=g_pZoomSectionPulse[0])
    {
        LENS_ERR(("LENS_M: Zoom Already Home!!\r\n"));

        /* Enter ZOOM_STATE_CLOSED */
        lensST8002_zoom_changeState();

        return ERR_OK;
    }

    lensST8002_zoom_doReverse();
    lensST8002_zoom_gotoState(ZOOM_STATE_CLOSED);

    return ERR_OK;
}

/* Disable ISR and zoom reverse as possible */
static INT32 lensST8002_zoom_retract_abnormal(void)
{
    unsigned short i,cnt=0,status=0;

    /* Check if ZOOM_STATE_IDLE */
    if(g_pZoomPara.uiCurrentState != ZOOM_STATE_FAIL)
        return ERR_OPERATION_FAIL;

    LENS_ERR(("LENS_M: Zoom Retract (abnormal mode)\r\n"));

    lensST8002_zoom_enablePWM(FALSE); //set full speed

    /* Enter ZOOM_STATE_CLOSING */
    lensST8002_zoom_gotoState(ZOOM_STATE_CLOSING);

    TimerDelayMs(1);
    lensST8002_zoomIsr_enable(FALSE);

    //[Chris Chung] 250? How about 0xffff?
    //Ans: Yes, we can use 0xffff. It should follow max zoom range in Spec.
    lensST8002_zoom_runStep(DIRECTION_BWD, 250+g_pZoomSectionPulse[ZOOM_SECTION_TELE]);

    while(1)
    {
        if(g_pZoomPara.ZoomStep == i)
        {
            cnt++;
            if(cnt > 5)
            {
                status=1;
                break;
            }
        }
        else
            cnt = 0;
        i = g_pZoomPara.ZoomStep;
        TimerDelayMs(5);
    }

    lensST8002_zoom_doBrake();
    TimerDelayMs(30);

    /* Enter ZOOM_STATE_CLOSED */
    lensST8002_zoom_changeState();

    if(status)
        return ERR_OPERATION_FAIL;
    else
        return ERR_OK;
}

void lensST8002_zoom_enablePWM(BOOL en)
{
#if (_ZOOM_PSEUDO_PWM_ == ENABLE)

    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: pMotor is null\r\n"));
        return;
    }

    g_pST8002PWMTab.bPwmCtrl = en;
    g_pST8002Obj.pMotor->setPWMInfo(&g_pST8002PWMTab);

    if(en)
        g_pZoomPara.ZoomOverRun = _ZOOM_PSEUDO_PWM_OVERRUNCOUNT_;
    else
        g_pZoomPara.ZoomOverRun = ZOOM_OVERRUN_COUNT;
#else

    g_pZoomPara.ZoomOverRun = ZOOM_OVERRUN_COUNT;

#endif
}

void lensST8002_zoom_hdlSpeed(void)
{
#if (_ZOOM_PSEUDO_PWM_ == ENABLE)

    UINT32 diff1, max_diff1_zero_count;
    static UINT32 diff1_zero_count = 0;

    /* No allow diff zero count more than (_ZOOM_PSEUDO_PWM_FORCETOBRAKETIME_) ms */
    max_diff1_zero_count = _ZOOM_PSEUDO_PWM_FORCETOBRAKETIME_/(_ZOOM_PSEUDO_PWM_SPEED_*_ZOOM_PSEUDO_PWM_CHECKCOUNT_/1000);

    diff1 = abs((INT32)(g_pZoomPara.ZoomLastStep - g_pZoomPara.ZoomStep));

    if(diff1 > 0)
        diff1_zero_count = 0;

    LENS_MSG(("LENS_M: diff1=%d\r\n", diff1));

    if(diff1 > (_ZOOM_PSEUDO_PWM_CHECKCOUNT_+_ZOOM_PSEUDO_PWM_CHECKDIFFCOUNT_))
    {
        //slow down
        g_pST8002PWMTab.uiPwmPeriodTime += _ZOOM_PSEUDO_PWM_PERIOD_ADD_;

        lensST8002_zoom_enablePWM(TRUE); //set PWM speed
    }
    else if(diff1 < (_ZOOM_PSEUDO_PWM_CHECKCOUNT_-_ZOOM_PSEUDO_PWM_CHECKDIFFCOUNT_))
    {
        //speed up
        if(g_pST8002PWMTab.uiPwmPeriodTime > (_ZOOM_PSEUDO_PWM_ONTIME_+_ZOOM_PSEUDO_PWM_PERIOD_ADD_))
        {
            g_pST8002PWMTab.uiPwmPeriodTime -= _ZOOM_PSEUDO_PWM_PERIOD_ADD_;

            lensST8002_zoom_enablePWM(TRUE); //set PWM speed
        }

        if(diff1 == 0)
        {
            diff1_zero_count++;

            if(diff1_zero_count == max_diff1_zero_count)
            {
                LENS_ERR(("LENS_M: %s: force to do brake, cur step=%d\r\n", __func__, g_pZoomPara.ZoomStep));
                lensST8002_zoom_doBrake();

                /* Enter ZOOM_STATE_IDLE */
                if(g_pZoomPara.uiCurrentState == ZOOM_STATE_RUNNING)
                   lensST8002_zoom_changeState();
            }
        }
    }
    else
    {
        //do nothing
    }

    g_pZoomPara.ZoomLastStep = g_pZoomPara.ZoomStep;
#endif
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensST8002_focus_setMotorDriver(unsigned char phase)
{
    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    g_pST8002Obj.pMotor->focus_setState(g_pFocusStepperPhase[phase]);
}

static void lensST8002_focus_doBL( unsigned char Steps )
{
    lensST8002_focus_initPhase();

    lensST8002_focus_runStep(DIRECTION_BWD,Steps);

    TimerDelayMs(FOCUS_INIT_DELAY);

    lensST8002_focus_runStep(DIRECTION_FWD,Steps);
}

static void lensST8002_focus_initPhase(void)
{
    lensST8002_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);   //restore original phase

    TimerDelayMs(FOCUS_INIT_DELAY);         //init pulse 10ms
}

static void lensST8002_focus_runStep(UINT8 dir, UINT16 step)
{
    unsigned long Count = 0;

    while(Count++ < step)
    {
        //if(dir==DIRECTION_FWD)
        if(dir==DIRECTION_BWD)//JJ1225
        {
            if(g_pFocusPara.uiFocusPhase >= (FOCUS_TOTAL_PHASE-1))
                g_pFocusPara.uiFocusPhase = 0;
            else
                g_pFocusPara.uiFocusPhase++;
            lensST8002_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);
            //g_pZoomPara.iFocusPosition+=1;
            g_pFocusPara.iFocusPosition-=1;//JJ1225
        }
        else
        {
            if(g_pFocusPara.uiFocusPhase == 0)
                g_pFocusPara.uiFocusPhase = (FOCUS_TOTAL_PHASE-1);
            else
                g_pFocusPara.uiFocusPhase--;

            lensST8002_focus_setMotorDriver(g_pFocusPara.uiFocusPhase);
            //g_pZoomPara.iFocusPosition-=1;
            g_pFocusPara.iFocusPosition+=1;//JJ1225
        }
        TimerDelayUs(FOCUS_SPEED);
    }
}

static void lensST8002_focus_gotoPosition(INT16 position)
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

    lensST8002_focus_initPhase();

    lensST8002_focus_runStep(Dir,Step);

    if(Dir == DIRECTION_BWD)
        lensST8002_focus_doBL( FOCUS_BL_STEP );
}

static INT32 lensST8002_focus_gotoHome(void)
{
    unsigned short sBoundary;

    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    lensST8002_focus_initPhase();

    if(g_pST8002Obj.pMotor->zoom_checkPR() == ZOOMPR_INSDIE_LEVEL)
    {
        LENS_ERR(("LENS_E: lensST8002: ZOOM is inside, focus can't goto home.\r\n"));
        return ERR_OPERATION_FAIL;
    }

    //find PI
    sBoundary = 0;
    while(g_pST8002Obj.pMotor->focus_checkPI() == FOCUSPI_INSDIE_LEVEL)
    {
        lensST8002_focus_runStep(DIRECTION_FWD,10);    //run 10 steps every time for speed
        sBoundary += 10;
        if(sBoundary > FOCUS_FWD_LIMIT_INSIDE_PI)
        {
            LENS_ERR(("LENS_E: B1: %d>%d",sBoundary, FOCUS_FWD_LIMIT_INSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    if(sBoundary > 0)   //check if do FWD.
        TimerDelayMs(FOCUS_INIT_DELAY);

    sBoundary = 0;
    while(g_pST8002Obj.pMotor->focus_checkPI() == FOCUSPI_OUTSDIE_LEVEL)
    {
        lensST8002_focus_runStep(DIRECTION_BWD,1);
        sBoundary += 1;
        if(sBoundary > FOCUS_BWD_LIMIT_OUTSIDE_PI)
        {
            LENS_ERR(("LENS_E: B2: %d>%d",sBoundary, FOCUS_BWD_LIMIT_OUTSIDE_PI));
            return ERR_OPERATION_FAIL;
        }
    }

    lensST8002_focus_doBL( 10 );   //10 count is defined in ST-8002 Ver:0 2006.12.01 for PI signal.

    g_pFocusPara.iFocusPosition=FOCUS_POS_HOME_PR;

    return ERR_OK;
}

static INT32 lensST8002_focus_gotoGarage(void)
{
    if(lensST8002_focus_gotoHome() == ERR_OK)
    {
        lensST8002_focus_gotoPosition(FOCUS_POS_GARAGE);
        return ERR_OK;
    }
    else
    {
        return ERR_OPERATION_FAIL;
    }
}

static INT16 lensST8002_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensST8002_focus_getMaxPosition(void)
{
    return FOCUS_POS_OUTER_STOP;
}

static UINT32 lensST8002_focus_getFL(UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pFocusLengthTbl[zoomSection];
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

#if (SHUTTER_CLOSE_INT_VERSION == ENABLE)
static void lensST8002_aperture_hdlShutterClose(void)
{
    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    switch(g_uiIrisCurPos)
    {
        case IRIS_POS_BIG:
            g_uiHandlerStep ++;
            lensST8002_aperture_setMotorDriver(g_pApertureCtrl[g_uiIrisCurPos][g_uiHandlerStep]);
            break;

        case IRIS_POS_SMALL:
            g_uiHandlerStep ++;
            lensST8002_aperture_setMotorDriver(g_pApertureCtrl[g_uiIrisCurPos][g_uiHandlerStep]);
            break;
    }
    if (g_uiHandlerStep >= (g_uiHandlerMaxStep))
    {
        timer_close(g_uiApertureTimerID);
        lensST8002_aperture_setMotorDriver(g_pApertureCtrl[g_uiIrisCurPos][g_uiHandlerStep]);
        g_uiApertureTimerID = LENS_NULL_TIMER;
        g_uiIrisCurPos = IRIS_POS_CLOSE;

        g_pST8002Obj.pMotor->power(MOTOR_POWER_OFF);
    }
    else
    {
        timer_set_us(g_uiApertureTimerID, g_pApertureTimeCnt[g_uiIrisCurPos][g_uiHandlerStep]-30, _TIMER_CTRL_ONE_SHOT|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
    }
}
#endif

static void lensST8002_aperture_setMotorDriver(LENS_IRIS_PHASE phase)
{
    if(g_pST8002Obj.pMotor == NULL)
    {
        LENS_ERR(("LENS_E: lensST8002: pMotor is null\r\n"));
        return;
    }

    g_pST8002Obj.pMotor->aperture_setState(g_pIrisStepperPhase[phase]);
}

static void lensST8002_aperture_gotoPosition(LENS_APERTURE_ACT position)
{
    if (g_uiIrisCurPos == position)
    {
        return;
    }

    switch(g_uiIrisCurPos)
    {
        case IRIS_POS_BIG:
            if (position  == IRIS_POS_SMALL)
            {
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HL);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(20);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                g_uiIrisCurPos = IRIS_POS_SMALL;
            }
            else if (position  == IRIS_POS_CLOSE)
            {
                //[Chris Chung] Do we need it?
                //Ans: If caller doesn't want to wait the following procedure, we need timer to take care.
                #if (SHUTTER_CLOSE_INT_VERSION == ENABLE)
                g_uiHandlerStep = 0;
                g_uiHandlerMaxStep = 4;
                lensST8002_aperture_setMotorDriver(g_pApertureCtrl[g_uiIrisCurPos][g_uiHandlerStep]);
                if (timer_open(&g_uiApertureTimerID, lensST8002_aperture_hdlShutterClose) == 0)
                {
                    timer_set_us(g_uiApertureTimerID, g_pApertureTimeCnt[g_uiIrisCurPos][0]-30, _TIMER_CTRL_ONE_SHOT|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
                }
                else
                {
                    LENS_ERR(("LENS_E: timer_open fail\r\n"));
                }
                #else
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(3);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayUs(3500);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HL);
                TimerDelayUs(1200);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayUs(15300);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                g_uiIrisCurPos = IRIS_POS_CLOSE;
                #endif
            }
            break;

        case IRIS_POS_SMALL:
            if (position  == IRIS_POS_BIG)
            {
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HL);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(20);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                g_uiIrisCurPos = IRIS_POS_BIG;
            }
            else if (position  == IRIS_POS_CLOSE)
            {
                #if (SHUTTER_CLOSE_INT_VERSION == ENABLE)
                g_uiHandlerStep = 0;
                g_uiHandlerMaxStep = 4;
                lensST8002_aperture_setMotorDriver(g_pApertureCtrl[g_uiIrisCurPos][g_uiHandlerStep]);
                if (timer_open(&g_uiApertureTimerID, lensST8002_aperture_hdlShutterClose) == 0)
                {
                    timer_set_us(g_uiApertureTimerID, g_pApertureTimeCnt[g_uiIrisCurPos][0]-30, _TIMER_CTRL_ONE_SHOT|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
                }
                else
                {
                    debug_err(("timer_open fail\r\n"));
                }
                #else
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(3);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayUs(3000);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LH);
                TimerDelayUs(1000);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayUs(16000);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                g_uiIrisCurPos = IRIS_POS_CLOSE;
                #endif
            }
            break;
        case IRIS_POS_CLOSE:
            if (position  == IRIS_POS_BIG)
            {
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayMs(20);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HL);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(20);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                g_uiIrisCurPos = IRIS_POS_BIG;
            }
            else if (position  == IRIS_POS_SMALL)
            {
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayMs(20);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_HH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LH);
                TimerDelayMs(5);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_LL);
                TimerDelayMs(20);
                lensST8002_aperture_setMotorDriver(IRIS_PHASE_OFF);
                g_uiIrisCurPos = IRIS_POS_SMALL;
            }
            break;
    }
}

static INT32 lensST8002_aperture_gotoState(LENS_APERTURE_ACT newState)
{
    switch(newState)
    {
    case LENS_APERTURE_SMALL:
        lensST8002_aperture_gotoPosition(IRIS_POS_SMALL);
        g_pIrisPara.uiCurrentState = LENS_APERTURE_SMALL;
        break;
    case LENS_APERTURE_BIG:
        lensST8002_aperture_gotoPosition(IRIS_POS_BIG);
        g_pIrisPara.uiCurrentState = LENS_APERTURE_BIG;
        break;
    default:
        debug_err(("lensST8002: aperture goto wrong state=%d\r\n", newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

static UINT32 lensST8002_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    if(zoomSection > (ZOOM_SECTION_TELE+1))
        return 0;

    return g_pApertureZoomTbl[zoomSection][irisPos];
}

static LENS_APERTURE_ACT lensST8002_aperture_getState(void)
{
    return g_pIrisPara.uiCurrentState;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------


static INT32 lensST8002_shutter_gotoState(LENS_SHUTTER_ACT newState)
{
    switch(newState)
    {
    case LENS_SHUTTER_CLOSE:
        lensST8002_aperture_gotoPosition(IRIS_POS_CLOSE);
        g_pShutterPara.uiCurrentState = LENS_SHUTTER_CLOSE;
#if (SHUTTER_CLOSE_INT_VERSION == ENABLE)
        return ERR_OK_TIMER_POWEROFF;
#else
        return ERR_OK;
#endif
    case LENS_SHUTTER_OPEN:
        lensST8002_aperture_gotoPosition(IRIS_POS_BIG);
        g_pShutterPara.uiCurrentState = LENS_SHUTTER_OPEN;
        break;
    default:
        debug_err(("lensST8002: shutter goto wrong state=%d\r\n", newState));
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}

