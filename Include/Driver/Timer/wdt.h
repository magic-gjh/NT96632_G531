/**
    Public header file for WDT module.

    This file is the header file that define the API and data type for WDT
    module.

    @file       wdt.h
    @ingroup    mIHALWDT
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/


#ifndef _WDT_H
#define _WDT_H

#include "Type.h"

/**
    @addtogroup mIHALWDT
*/
//@{

// WDT Driver APIs
extern ER       wdt_open(void);
extern ER       wdt_close(void);
extern ER       wdt_waitTimeout(void);
extern ER       wdt_setTrig(void);
extern ER       wdt_setEnable(void);
extern ER       wdt_setMS4B(UINT32 val);
extern ER       wdt_setIntEn(BOOL bEn);
extern ER       wdt_setRstEn(BOOL bEn);
extern ER       wdt_setNahbRstEn(BOOL bEn);
extern ER       wdt_getCnt(UINT32 *uiCnt);

//@}

#endif
