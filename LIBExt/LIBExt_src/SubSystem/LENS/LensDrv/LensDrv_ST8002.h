/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_ST8002.h
    @ingroup    mISYSLens

    @brief      ST8002 lens driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSDRV_ST8002_H
#define _LENSDRV_ST8002_H
//#include "constant.h"
#include "Type.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Zoom related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct ZOOMPARA
{
    /* Used in all states */
    volatile UINT8          uiCurrentState; //Record current state in zoom state machine.
    volatile BOOL           bZoomIsr;       //Open Zoom ISR.
    volatile UINT8          uiZoomWideCnt;  //Count how many times the lens has zoom to wide.
    volatile UINT8          Direction;      //Record current zoom direction.
    volatile UINT8          ZoomOverRun;    //Limit zoom over-run step.
    volatile INT16          ZoomStep;       //Record current zoom step position.
    volatile INT16          ZoomLastStep;   //Record last zoom step position.
    volatile BOOL           bZoomBoundaryAutoCheck;   //Auto check zoom bundary.
    volatile BOOL           bDoOptZoomDraw;           //OpticalZoom Callback to Draw.
    volatile unsigned short StepMove;       //Limit lens zoom steps in running state.
    volatile unsigned short StepCnt;        //Record how many steps the lens has zoom in running state.
} ZOOM_Struct;

typedef enum _LENS_ZOOM_STATE {
    ZOOM_STATE_NULL = 0,        //unknow state
    ZOOM_STATE_FAIL,            //fail state
    ZOOM_STATE_INSIDEINIT,      //init from inside state
    ZOOM_STATE_OUTSIDEINIT,     //init from outside state
    ZOOM_STATE_INITPART1,       //InsideInit ok and enter part1 state
    ZOOM_STATE_INITPART2,       //InitPart1 ok and enter part2 state
    ZOOM_STATE_IDLE,            //idle state
    ZOOM_STATE_RUNNING,         //running state
    ZOOM_STATE_CLOSING,         //closing state
    ZOOM_STATE_CLOSED           //closed state
} LENS_ZOOM_STATE;

typedef enum _LENS_ZOOM_PR_LEVEL {
    ZOOMPR_INSDIE_LEVEL = 0,
    ZOOMPR_OUTSDIE_LEVEL
} LENS_ZOOM_PR_LEVEL;

#define ZOOM_STANDARD_A         54
#define ZOOM_POS_INNER_STOP     0
#define ZOOM_POS_GARAGE         ((208-202)/2)
#define ZOOM_POS_HOME_PR        ((208 - ZOOM_STANDARD_A) / 2)
#define ZOOM_POS_WIDE           (ZOOM_POS_HOME_PR + (ZOOM_STANDARD_A / 2))
#define ZOOM_POS_TELE           (ZOOM_POS_WIDE + (259 / 2))
#define ZOOM_POS_OUTER_STOP     ((266+208)/2)

#define ZOOM_SECTION_TELE       7
#define ZOOM_SECTION_WIDE       1
#define ZOOM_BL_STEP            7

#define _ZOOM_PSEUDO_PWM_          ENABLE
/* The following valus is experimental. */
#define ZOOM_OVERRUN_COUNT                 4
#define _ZOOM_PSEUDO_PWM_PERIOD_           60000   //unit: us
#define _ZOOM_PSEUDO_PWM_PERIOD_ADD_       5000    //unit: us
#define _ZOOM_PSEUDO_PWM_ONTIME_           10000   //unit: us
#define _ZOOM_PSEUDO_PWM_ONTIME_ADD_       3000    //unit: us
#define _ZOOM_PSEUDO_PWM_SPEED_            10000   //unit: us/step
#define _ZOOM_PSEUDO_PWM_CHECKCOUNT_       5       //unit: step
#define _ZOOM_PSEUDO_PWM_CHECKDIFFCOUNT_   1       //unit: step (_LENS_ZOOM_PSEUDO_PWM_CHECKCOUNT_*20%)
#define _ZOOM_PSEUDO_PWM_OVERRUNCOUNT_     1       //unit: step
#define _ZOOM_PSEUDO_PWM_FORCETOBRAKETIME_ 200     //unit: ms

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Focus related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct FOCUSPARA
{
    volatile UINT8          uiFocusPhase;
    volatile INT16          iFocusPosition;
} FOCUS_Struct;

typedef enum _LENS_FOCUS_PI_LEVEL {
    FOCUSPI_OUTSDIE_LEVEL = 0,
    FOCUSPI_INSDIE_LEVEL
} LENS_FOCUS_PI_LEVEL;

#define FOCUS_POS_HOME_DEF       0
#define FOCUS_POS_INNER_STOP     (FOCUS_POS_HOME_DEF-104)
#define FOCUS_POS_GARAGE         (FOCUS_POS_HOME_DEF-60-10)//-10 to avoid BackLash hit the wall
#define FOCUS_POS_FAR            (FOCUS_POS_HOME_DEF-60)
#define FOCUS_POS_HOME_PR        (FOCUS_POS_HOME_DEF-0)
#define FOCUS_POS_NORMAL_WIDE    (FOCUS_POS_HOME_DEF+97)
#define FOCUS_POS_NEAR           (FOCUS_POS_HOME_DEF+440)
#define FOCUS_POS_OUTER_STOP     (FOCUS_POS_HOME_DEF+465)

#define FOCUS_BWD_LIMIT_OUTSIDE_PI  ((FOCUS_POS_OUTER_STOP-FOCUS_POS_HOME_PR)+20)   //buffer = 20
#define FOCUS_FWD_LIMIT_INSIDE_PI   ((FOCUS_POS_HOME_PR-FOCUS_POS_INNER_STOP)+200)  //buffer = 200 (experimental)
#define FOCUS_BL_STEP               6
#define FOCUS_SPEED                 1000   //unit: us
#define FOCUS_INIT_DELAY            10     //unit: ms

#define FOCUS_TOTAL_PHASE           8

#define FOCUS_ZOOM_TABLE_COLUMN     11

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for IRIS related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct IRISPARA
{
    volatile UINT8          uiCurrentState; //Record current LENS_IRIS_STATE in IRIS state machine.
} IRIS_Struct;

typedef enum _LENS_IRIS_PHASE {
    IRIS_PHASE_HH = 0,
    IRIS_PHASE_LH,
    IRIS_PHASE_HL,
    IRIS_PHASE_LL,
    IRIS_PHASE_OFF,
    IRIS_PHASE_TOTAL_NUM    //Total Phase Number
} LENS_IRIS_PHASE;

typedef enum _LENS_IRIS_POSITION {
    IRIS_POS_BIG = 0,
    IRIS_POS_SMALL,
    IRIS_POS_CLOSE,
    IRIS_POS_TOTAL_NUM      //Total Positon Number
} LENS_IRIS_POSITION;

#define IRIS_TOTAL_NONCLOSE_POS_NUM     (IRIS_POS_TOTAL_NUM-1)

#define IRIS_TOTAL_POWERSEQ  5

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Shutter related definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef struct SHUTTERPARA //revise by 2006/05/08
{
    volatile UINT8          uiCurrentState; //Record current LENS_SHUTTER_STATE in SHUTTER state machine.
} SHUTTER_Struct;

#define SHUTTER_CLOSE_INT_VERSION   ENABLE

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for other definition.
//--------------------------------------------------------------------------------------------------------------------------

typedef enum _LENS_DIRECTION {
    DIRECTION_FWD = 0,
    DIRECTION_BWD
} LENS_DIRECTION;

#define STEPPER_PHASE_M4(a,b,c,d)   ((a)|((b)<<1)|((c)<<2) | ((d)<<3))
#define STEPPER_PHASE_M2(ap,am)     ((ap)|((am)<<1))

#endif
