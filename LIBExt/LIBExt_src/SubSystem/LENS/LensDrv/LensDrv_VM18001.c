/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_VM18001.c
    @ingroup    mISYSLens

    @brief      VM18001 lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/02/20
*/

#include <stdio.h>
#include <stdlib.h>
#include "Lens.h"
#include "LensDrv_VM18001.h"
#include "LensDrv_DUMMY.h"
#include "debug.h"
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
static void     lensVM18001_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static BOOL     lensVM18001_isFFLens(void);

//zoom
static INT32    lensVM18001_zoom_initPart1(void);
static INT32    lensVM18001_zoom_ctrlRelease(void); // return current zoom section
static INT32    lensVM18001_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
static UINT8    lensVM18001_zoom_getCurrentSection(void);
static UINT8    lensVM18001_zoom_getNextSection(void);
static UINT8    lensVM18001_zoom_getMaxSection(void);
static INT32    lensVM18001_zoom_retract( void );
static void     lensVM18001_zoom_gotoSection(UINT8 section);

//focus
static void     lensVM18001_focus_gotoPosition(INT16 position);
static INT32    lensVM18001_focus_gotoGarage(void);
static INT16    lensVM18001_focus_getCurrentPosition(void);
static INT16    lensVM18001_focus_getMaxPosition(void);

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Gloabal variables used through out whole file
//--------------------------------------------------------------------------------------------------------------------------
static ZOOM_Struct g_pZoomPara={0};
static FOCUS_Struct g_pFocusPara={0};

static LENS_TAB g_pVM18001Obj =
{
    //Lens
    lensVM18001_init,
    lensVM18001_isFFLens,
    //Zoom
    lensVM18001_zoom_initPart1,
    lensDUMMY_zoom_initPart2,
    lensVM18001_zoom_ctrlRelease,
    lensVM18001_zoom_ctrlPress,
    lensVM18001_zoom_getCurrentSection,
    lensVM18001_zoom_getMaxSection,
    lensVM18001_zoom_getNextSection,
    lensVM18001_zoom_retract,
    lensVM18001_zoom_gotoSection,
    lensDUMMY_zoom_setAutoBoundaryCheck,
    lensDUMMY_zoomIsr_enable,
    //Focus
    lensVM18001_focus_gotoPosition,
    lensDUMMY_focus_gotoHome,
    lensVM18001_focus_gotoGarage,
    lensVM18001_focus_getCurrentPosition,
    lensVM18001_focus_getMaxPosition,
    lensDUMMY_focus_getFL,
    //Aperture
    lensDUMMY_aperture_gotoState,
    lensDUMMY_aperture_getState,
    lensDUMMY_aperture_getFNO,
    //Shutter
    lensDUMMY_shutter_gotoState,
    //Others
    NULL
};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensVM18001_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    g_pVM18001Obj.pMotor = pMotorTab;

    //Focus parameter
    g_pFocusPara.iFocusPosition = 0;
}

/**
    Get FF lens object

    @return FF lens object pointer

*/
PLENS_TAB lensVM18001_getObject(void)
{
    return &g_pVM18001Obj;
}

BOOL lensVM18001_isFFLens(void)
{
    return FALSE;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

static INT32 lensVM18001_zoom_initPart1(void)
{
    g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD);
    TimerDelayMs(ZOOM_SECTION_TIME_FWD_DELAY);
    g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF);
    g_pZoomPara.uiCurrentSection = ZOOM_SECTION_WIDE;
    g_pZoomPara.uiCurrentFakeSection = ZOOM_SECTION_WIDE;

    return ERR_OK;
}

static void lensVM18001_zoom_runStep(unsigned char dir)
{
    if(dir == DIRECTION_FWD)
    {
        g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD);

        TimerDelayMs(ZOOM_SECTION_TIME_BWD_DELAY);

        g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF);

        g_pZoomPara.uiCurrentSection++;
    }
    else
    {
        g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD);

        TimerDelayMs(ZOOM_SECTION_TIME_FWD_DELAY);

        g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF);

        g_pZoomPara.uiCurrentSection--;
    }
}

static void lensVM18001_zoom_gotoSection(UINT8 section)
{
    if(section>ZOOM_SECTION_TELE_FAKE || section<ZOOM_SECTION_WIDE)
        return;

    if(g_pVM18001Obj.pMotor == NULL)
    {
        debug_err(("lensVM18001: pMotor is null\r\n"));
        return;
    }

    if(section == g_pZoomPara.uiCurrentFakeSection)
    {
        g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF);
        return;
    }

    switch(g_pZoomPara.uiCurrentSection)
    {
    case ZOOM_SECTION_WIDE:
        lensVM18001_zoom_runStep(DIRECTION_FWD);
        break;

    case ZOOM_SECTION_TELE:
        lensVM18001_zoom_runStep(DIRECTION_BWD);
        break;

    default:
        break;
    }

    g_pZoomPara.uiCurrentFakeSection = section;
}

static UINT8 lensVM18001_zoom_getCurrentSection(void)
{
    return g_pZoomPara.uiCurrentFakeSection;
}

static UINT8 lensVM18001_zoom_getMaxSection(void)
{
    return ZOOM_SECTION_TELE_FAKE;
}

static UINT8 lensVM18001_zoom_getNextSection(void)
{
    if(lensVM18001_zoom_getCurrentSection() != lensVM18001_zoom_getMaxSection())
        return (lensVM18001_zoom_getCurrentSection()+1);
    else
        return lensVM18001_zoom_getMaxSection();
}

static INT32 lensVM18001_zoom_ctrlRelease(void) // return current zoom section
{
    return lensVM18001_zoom_getCurrentSection();
}

static INT32 lensVM18001_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    switch(Zoom_IN)
    {
    case LENS_ZOOM_IN:
        g_pZoomPara.Direction = DIRECTION_FWD;
        lensVM18001_zoom_gotoSection(g_pZoomPara.uiCurrentFakeSection+1);
        break;
    case LENS_ZOOM_OUT:
        g_pZoomPara.Direction = DIRECTION_BWD;
        lensVM18001_zoom_gotoSection(g_pZoomPara.uiCurrentFakeSection-1);
        break;
    default:
        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}


static INT32 lensVM18001_zoom_retract( void )
{
    if(g_pVM18001Obj.pMotor == NULL)
    {
        debug_err(("lensVM18001: pMotor is null\r\n"));
        return ERR_OPERATION_FAIL;
    }

    g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_FWD);
    TimerDelayMs(ZOOM_SECTION_TIME_FWD_DELAY);
    g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF);
    TimerDelayMs(ZOOM_SECTION_TIME_REVERSE_DELAY);
    g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_BWD);
    TimerDelayMs(ZOOM_SECTION_TIME_BWD_DELAY);
    g_pVM18001Obj.pMotor->zoom_setState(MOTOR_ZOOM_OFF);

    g_pZoomPara.uiCurrentSection = ZOOM_SECTION_TELE;
    g_pZoomPara.uiCurrentFakeSection = 0;

    return ERR_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

static void lensVM18001_focus_setMotorDriver(UINT32 position)
{
    if(g_pVM18001Obj.pMotor == NULL)
    {
        debug_err(("lensVM18001: pMotor is null\r\n"));
        return;
    }

    g_pVM18001Obj.pMotor->focus_setState(position);
}

static void lensVM18001_focus_gotoPosition(INT16 position)
{
    if(position >= 0)
    {
        lensVM18001_focus_setMotorDriver(position);
        g_pFocusPara.iFocusPosition=position;
    }
}

static INT32 lensVM18001_focus_gotoGarage(void)
{
    lensVM18001_focus_gotoPosition(FOCUS_POS_GARAGE);

    return ERR_OK;
}

static INT16 lensVM18001_focus_getCurrentPosition(void)
{
    return g_pFocusPara.iFocusPosition;
}

static INT16 lensVM18001_focus_getMaxPosition(void)
{
    return FOCUS_POS_OUTER_STOP;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------


