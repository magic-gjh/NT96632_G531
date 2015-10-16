/**
    Public header file for system initialization

    Public header file for system initialization.

    @file       SystemInit.h
    @ingroup    mIPRJAPCfg
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _SYSTEMINIT_H
#define _SYSTEMINIT_H

#include "OSCommon.h"

/**
    @addtogroup mIPRJAPCfg
*/
//@{

// PLL OSC clock, 12 MHz
#define SYSINIT_PLL_OSC_CLOCK_MHZ   12

// Enable/Disable MCLK ouput
#define SYSINIT_OUTPUT_MCLK         ENABLE

//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
// Enable / Disable DRAM clock source using PLL1
#define SYSINIT_DRAM_CLKSRC_PLL1    ENABLE

// ENABLE -> Logo will display on Video1, DISABLE -> Logo will display on Video2
#define SYSINIT_LOGO_V1             ENABLE

#if ((SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE) || \
     (SYSINIT_OUTPUT_MCLK == ENABLE))
// PLL1 configuration (For DRAM and Sensor)
// OSC = 12 MHz, PLL output = 12 * 40 / 2 = 240 MHz
#define SYSINIT_PLL1_LOOPDIV        40
#define SYSINIT_PLL1_PREDIV         2
#endif

// PLL2 configuration (For Display)
// OSC = 12 MHz, PLL2 output = 12 * 18 / 1 / 2 = 108 MHz
#define SYSINIT_PLL2_LOOPDIV        18
#define SYSINIT_PLL2_PREDIV         1
//#NT#2010/02/10#Chris Hsu -end

extern void         SystemInit(void);
extern void         SysInit_InitLCD(void);
extern void         SysInit_InitPStore(void);
//#NT#2009/02/16#JustinLee -begin
//#Added lens module init entry point in system
extern void         SysInit_InitLens(void);
//#NT#2009/02/16#JustinLee -end

extern void         SysInit_DisplaySplashScreen(UINT32 uiShowLogo,UINT8 *pBgBuf, UINT32 uiSize);

//@}

#endif
