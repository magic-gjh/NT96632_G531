/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_DUMMY.c
    @ingroup    mISYSLens

    @brief      DUMMY lens driver

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/02/24
*/

#include <stdio.h>
#include <stdlib.h>
#include "Lens_int.h"
#include "LensDrv_DUMMY.h"
#include "Type.h"

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is to deal with zoom ISR.
//--------------------------------------------------------------------------------------------------------------------------

void lensDUMMY_zoomIsr_enable(BOOL enable)
{
    return;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for zoom control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 lensDUMMY_zoom_initPart1(void)
{
    return ERR_OK;
}

INT32 lensDUMMY_zoom_initPart2(void)
{
    return ERR_OK;
}

void lensDUMMY_zoom_gotoSection(UINT8 section)
{
    return;
}

UINT8 lensDUMMY_zoom_getCurrentSection(void)
{
    return 0;
}

UINT8 lensDUMMY_zoom_getMaxSection(void)
{
    return 0;
}

UINT8 lensDUMMY_zoom_getNextSection(void)
{
    return 0;
}


INT32 lensDUMMY_zoom_ctrlRelease(void) // return current zoom section
{
    return ERR_OK;
}

INT32 lensDUMMY_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN)
{
    return ERR_OK;
}

void lensDUMMY_zoom_setAutoBoundaryCheck(BOOL en)
{
    return;
}

INT32 lensDUMMY_zoom_retract( void )
{
    return ERR_OPERATION_FAIL;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for focus control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 lensDUMMY_focus_gotoHome(void)
{
    return ERR_OK;
}

UINT32 lensDUMMY_focus_getFL(UINT32 zoomSection)
{
    return 0;
}

INT16 lensDUMMY_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index)
{
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for aperture control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 lensDUMMY_aperture_gotoState(LENS_APERTURE_ACT newState)
{
    return ERR_OK;
}

UINT32 lensDUMMY_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection)
{
    return 0;
}

LENS_APERTURE_ACT lensDUMMY_aperture_getState(void)
{
    return LENS_APERTURE_BIG;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for shutter control.
//--------------------------------------------------------------------------------------------------------------------------

INT32 lensDUMMY_shutter_gotoState(LENS_SHUTTER_ACT newState)
{
    return ERR_OK;
}

UINT32 lensDUMMY_shutter_getPulsePeriod(void)
{
    return LENS_MAX_WAIT_TIME;
}

