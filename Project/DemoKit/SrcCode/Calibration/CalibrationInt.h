/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationInt.h
    @ingroup    mIPRJAPTest

    @brief      Internal header file of calibration task
                Internal header file of calibration task

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALIBRATIONINT_H
#define _CALIBRATIONINT_H

#include <stdio.h>
#include "Type.h"
#include "CalibrationTsk.h"

#define CAL_INVALID_DEFAULT         0xFFFFFFFF

#define KEY_EVENT_CAL               (FLGKEY_UP | FLGKEY_DOWN | FLGKEY_SHUTTER2 | FLGKEY_MENU)

typedef struct
{
    INT8    *pItemName;
    FP      ItemFunction;
} CAL_ITEM;

extern volatile BOOL                g_bCalbirationOpened;
extern          CAL_ITEM            g_CalItem[];
extern          RECT                g_CalStringRect;
extern          UINT32              g_CalMaxItemNum;
extern volatile CAL_APPOBJ          g_CalAppObj;

extern UINT32       Cal_GetDefaultInRoot(INT8 * pExt);

extern void         Cal_SetDefault(void);
extern void         Cal_NoFunction(void);

//#NT#2009/08/31#KS Hung -begin
extern UINT32 Cal_GetMaxItemNum(void);
//#NT#2009/08/31#KS Hung -end

#endif

//@}
