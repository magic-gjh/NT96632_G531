/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       AF_Lib.h
    @ingroup    mISYSLens

    @brief      auto focus function header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/12
*/
#ifndef _AF_LIB_H
#define _AF_LIB_H

#include "Type.h"

typedef enum {
    AF_STA_OK,
    AF_STA_FAIL
} _AF_STATUS_;

typedef UINT32 (*FPCALLBACK_GETVA)(void);

extern void     AF_RegGetVariationCB(FPCALLBACK_GETVA getVariation);
extern INT32    AF_SearchFullRange(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT16 *pMaxVA);

extern INT32    AF_Process(void);
extern INT32    AF_Process2(void);
extern INT32    AF_Process3(INT16 startIdx, INT16 endIdx, INT16 *pMaxIdx, UINT16 *pMaxVA);

#endif
