/**
    Change clock APIs header file

    Change clock APIs header.

    @file       clock.h
    @ingroup    mIHALSysCG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _CLOCK_H
#define _CLOCK_H

#include "Type.h"


/** \addtogroup mIHALSysCG */
//@{

typedef struct
{
    UINT32      uiDMATiming1;
    UINT32      uiDMATiming2;
} DMA_TIMING, *PDMA_TIMING;

typedef struct
{
    UINT32      uiConfig;
    UINT32      uiTiming0;
    UINT32      uiTiming1;
    UINT32      uiDelayConfig;
    UINT32      uiEngineer;
} DMA_SETTING, *PDMA_SETTING;

typedef struct
{
    UINT32      uiSlewRate_PPPN;
    UINT32      uiSlewRate_DDR;
    UINT32      uiCtlDrvSink_PPPN;
    UINT32      uiConfig_PPPN;
} DMA_DRIVING, *PDMA_DRIVING;


/**
    Power Down mode definition

    This definition is used for clk_powerdown() & clk_changeCPUAHB().

    @name   Power Down mode definition
*/
//@{
#define PLL_PDN_MODE_CLKSCALING     0x00    ///< CPU Clock Scaling
#define PLL_PDN_MODE_SLEEP1         0x01    ///< CPU enter power down mode 1
#define PLL_PDN_MODE_SLEEP2         0x02    ///< CPU enter power down mode 2
#define PLL_PDN_MODE_SLEEP3         0x03    ///< CPU enter power down mode 3
//@}


extern ER      clk_open(void);
extern ER      clk_close(void);
//#NT#2009/09/30#HH Chuang -begin
//#NT#change clk_changeDMA prototype
extern BOOL    clk_changeDMA(UINT32 uiDMAClk, PDMA_TIMING pDMATiming, PDMA_SETTING pDMASetting);
//#NT#2009/09/30#HH Chuang -end
extern BOOL    clk_changeCPUAHB(UINT32 uiCPUClk, UINT32 uiAHBClk);
extern BOOL    clk_powerdown(UINT32 uiMode);

//@}

#endif
