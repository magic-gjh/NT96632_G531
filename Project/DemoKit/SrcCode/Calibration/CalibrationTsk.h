/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.h
    @ingroup    mIPRJAPTest

    @brief      Header file of calibration task
                Header file of calibration task

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALIBRATIONTSK_H
#define _CALIBRATIONTSK_H

#include "Type.h"

typedef struct
{
    UINT32          uiMemoryAddr;
    UINT32          uiMemorySize;
} CAL_APPOBJ, *PCAL_APPOBJ;

//#NT#2009/08/31#KS Hung -begin
//#NT#Cancel th argument in Cal_Open
extern ER       Cal_Open();
//#NT#2009/08/31#KS Hung -end

extern void     Cal_ShowString(INT8 *pStr, PRECT pRect, BOOL bClear);
extern UINT32   Cal_GetSerialNum(UINT32 uiIncrement);

extern UINT32   Cal_GetMemoryAddr(void);
extern UINT32   Cal_GetMemorySize(void);

#endif

//@}
