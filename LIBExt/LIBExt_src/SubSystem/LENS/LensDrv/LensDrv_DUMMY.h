/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensDrv_DUMMY.h
    @ingroup    mISYSLens

    @brief      DUMMY lens driver header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSDRV_DUMMY_H
#define _LENSDRV_DUMMY_H
//#include "constant.h"
#include "Type.h"
#include "Lens.h"

//Zoom ISR
void    lensDUMMY_zoomIsr_enable(BOOL enable);
//Zoom
INT32   lensDUMMY_zoom_initPart1(void);
INT32   lensDUMMY_zoom_initPart2(void);
INT32   lensDUMMY_zoom_ctrlRelease(void); // return current zoom section
INT32   lensDUMMY_zoom_ctrlPress(LENS_ZOOM_ACT Zoom_IN); // 1: zoom in, 0:zoom out
UINT8   lensDUMMY_zoom_getCurrentSection(void);
UINT8   lensDUMMY_zoom_getNextSection(void);
UINT8   lensDUMMY_zoom_getCurrentSection(void);
UINT8   lensDUMMY_zoom_getMaxSection(void);
INT32   lensDUMMY_zoom_retract( void );
void    lensDUMMY_zoom_gotoSection(UINT8 section);
void    lensDUMMY_zoom_setAutoBoundaryCheck(BOOL en);
//Focus
INT32   lensDUMMY_focus_gotoHome(void);
UINT32  lensDUMMY_focus_getFL(UINT32 zoomSection);
INT16   lensDUMMY_focus_getZoomTblValue(UINT32 zoomSection, UINT32 index);
//Aperture
INT32   lensDUMMY_aperture_gotoState(LENS_APERTURE_ACT newState);
UINT32  lensDUMMY_aperture_getFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);
LENS_APERTURE_ACT lensDUMMY_aperture_getState(void);
//Shutter
INT32   lensDUMMY_shutter_gotoState(LENS_SHUTTER_ACT newState);
UINT32  lensDUMMY_shutter_getPulsePeriod(void);

#endif
