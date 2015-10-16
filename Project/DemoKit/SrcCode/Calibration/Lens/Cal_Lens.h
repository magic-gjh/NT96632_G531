/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       Cal_Lens.h
    @ingroup    mISYSLens

    @brief      Lens calibration library header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/21
*/

#ifndef _CAL_LENS_H_
#define _CAL_LENS_H_

typedef struct {
    #if (_LENSLIB_ == _LENSLIB_DZ5311_)
    INT32       Far_Focus[10];
    INT32       Near_Focus[10];
    #else
    INT32       Far_Focus[1];
    INT32       Near_Focus[1];
    #endif
} CALLENS_INFO, *PCALLENS_INFO;

typedef enum {
    CALLENS_STA_OK,
    CALLENS_STA_FAIL
} _CALLENS_STATUS_;

#if (_LENSLIB_ == _LENSLIB_DZ5311_)
#define CALLENS_START_IDX   -124
#else
#define CALLENS_START_IDX   0
#endif

UINT8   CalLens_FarFocusAdjust(void);
UINT8   CalLens_NearFocusAdjust(void);

BOOL    CalLens_IsAdjust(void);
INT32   CalLens_GetFarFocusStep(UINT32 zoomSection);
INT32   CalLens_GetNearFocusStep(UINT32 zoomSection);

INT32   CalLens_ReadCalData(void);
INT32   CalLens_WriteCalData(void);

#endif

