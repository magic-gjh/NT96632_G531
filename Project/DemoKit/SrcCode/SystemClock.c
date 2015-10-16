/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemClock.c
    @ingroup    mIPRJAPCfg

    @brief      Change system clock (CPU/AHB/DMA)
                Change system clcok based on different operation mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "OSCommon.h"
#include "SystemClock.h"
#include "DMA.h"
#include "Clock.h"
#include "pll.h"
#include "ide.h"
#include "Audio.h"
#include "Alg_IPLMode.h"
#include "FileSysTsk.h"
//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#include "SystemInit.h"
//#NT#2010/02/10#Chris Hsu -end

// Internal compile option for IDE control in DMA clock change
// Because this compile option is not a feature selection and is implementation dependent,
// we do not put this setting in global header.
// method 1, wait VD only.  But if not all interrupts are disabled and there is task scheduling during VD period,
// this method might fail because it may leave VD period after return from ISR or scheduler,
// and this might cause system crash potentially
#define WAIT_IDE_VD     1
// method 2, disable/enable IDE, this method might cause potential very short white blinking
#define WAIT_IDE_IDE    2
// method 3, disable/enable Video/OSD buffer, this method might cause potential very short black blinking
#define WAIT_IDE_WIN    3
#define CHANGE_DMA_CLOCK_IDE_OPT            WAIT_IDE_VD

// DMA clock = 264 MHz is not very stable now, don't use
#define SYSCLK_DMA_SUPPORT_264M             DISABLE
// DMA clock = 160 MHz is not available now, don't use
#define SYSCLK_DMA_SUPPORT_160M             DISABLE

// DRAM parameter
#define DDRII_DLL_DMA120MHz_CONFIG          0x00000162
#define DDRII_DLL_DMA120MHz_TIMING0         0x31102152
#define DDRII_DLL_DMA120MHz_TIMING1         0x0380000C
#define DDRII_DLL_DMA120MHz_DLYCFG          0x80410406
#define DDRII_DLL_DMA120MHz_ENGINEER        0x00002008

#if (SYSCLK_DMA_SUPPORT_160M == ENABLE)
#define DDRII_DLL_DMA160MHz_CONFIG          0x00000162
#define DDRII_DLL_DMA160MHz_TIMING0         0x41202162
#define DDRII_DLL_DMA160MHz_TIMING1         0x04E00010
#define DDRII_DLL_DMA160MHz_DLYMODE         0x00000000
#define DDRII_DLL_DMA160MHz_DLYCFG          0x80410206
#define DDRII_DLL_DMA160MHz_ENGINEER        0x00003008
#endif

#define DDRII_DLL_DMA240MHz_CONFIG          0x00000182
#define DDRII_DLL_DMA240MHz_TIMING0         0x61303293
#define DDRII_DLL_DMA240MHz_TIMING1         0x07530019
#define DDRII_DLL_DMA240MHz_DLYCFG          0x80410206
#define DDRII_DLL_DMA240MHz_ENGINEER        0x00003008

#if (SYSCLK_DMA_SUPPORT_264M == ENABLE)
#define DDRII_DLL_DMA264MHz_CONFIG          0x000001A2
#define DDRII_DLL_DMA264MHz_TIMING0         0x693132A3
#define DDRII_DLL_DMA264MHz_TIMING1         0x08100021
#define DDRII_DLL_DMA264MHz_DLYCFG          0x80410206
#define DDRII_DLL_DMA264MHz_ENGINEER        0x00003008
#endif

// Static variables
static UINT32       uiDMAClock;
static UINT32       uiCPUClock;
static UINT32       uiAHBClock;
static UINT32       uiDRAMType;
#if (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_WIN)
static UINT32       uiIDEWinEN;
#endif
static BOOL         bIDEIsEnabled;
static BOOL         bIsAudioBusy;

/**
    Get DMA clock parameter according to clock

    Get DMA clock parameter according to clock.

    @param[in]  uiClock         DMA clock
    @param[out] pDMASetting     DMA setting parameter
    @return void
*/
static void SysClk_GetDMAParam(UINT32 uiClock, PDMA_SETTING pDMASetting)
{
    // Mark to reduce code size, NT96630 doesn't support DDR1 now
    /*
    if (uiDRAMType == DDR_TYPE_DDR1)
    {
    }
    else
    */
    {
        switch (uiClock)
        {
//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#if (SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE)
        case PLL_CLKSEL_DMA_PLL1:
#else
        case PLL_CLKSEL_DMA_240:
#endif
//#NT#2010/02/10#Chris Hsu -end
            // Setting for 240 MHz
            pDMASetting->uiConfig       = DDRII_DLL_DMA240MHz_CONFIG;
            pDMASetting->uiTiming0      = DDRII_DLL_DMA240MHz_TIMING0;
            pDMASetting->uiTiming1      = DDRII_DLL_DMA240MHz_TIMING1;
            pDMASetting->uiDelayConfig  = DDRII_DLL_DMA240MHz_DLYCFG;
            pDMASetting->uiEngineer     = DDRII_DLL_DMA240MHz_ENGINEER;
            break;

//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#if ((SYSINIT_DRAM_CLKSRC_PLL1 == DISABLE) && \
     (SYSCLK_DMA_SUPPORT_160M == ENABLE))
//#NT#2010/02/10#Chris Hsu -end
        case PLL_CLKSEL_DMA_160:
            // Setting for 160 MHz
            pDMASetting->uiConfig       = DDRII_DLL_DMA160MHz_CONFIG;
            pDMASetting->uiTiming0      = DDRII_DLL_DMA160MHz_TIMING0;
            pDMASetting->uiTiming1      = DDRII_DLL_DMA160MHz_TIMING1;
            pDMASetting->uiDelayConfig  = DDRII_DLL_DMA160MHz_DLYCFG;
            pDMASetting->uiEngineer     = DDRII_DLL_DMA160MHz_ENGINEER;
            break;
#endif

//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#if (SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE)
        case PLL_CLKSEL_DMA_PLL1_D2:
#else
        case PLL_CLKSEL_DMA_120:
#endif
//#NT#2010/02/10#Chris Hsu -end
            // Setting for 120 MHz
            pDMASetting->uiConfig       = DDRII_DLL_DMA120MHz_CONFIG;
            pDMASetting->uiTiming0      = DDRII_DLL_DMA120MHz_TIMING0;
            pDMASetting->uiTiming1      = DDRII_DLL_DMA120MHz_TIMING1;
            pDMASetting->uiDelayConfig  = DDRII_DLL_DMA120MHz_DLYCFG;
            pDMASetting->uiEngineer     = DDRII_DLL_DMA120MHz_ENGINEER;
            break;

        /*
        case PLL_CLKSEL_DMA_PLL1:
        case PLL_CLKSEL_DMA_PLL1_D2:
        case PLL_CLKSEL_DMA_PLL1_D3:
        case PLL_CLKSEL_DMA_PLL1_D4:
            // Please place correct setting for real clock
            // Ex: PLL1_D2 = 264 MHz
#if (SYSCLK_DMA_SUPPORT_264M == ENABLE)
            // Setting for 264 MHz
            pDMASetting->uiConfig       = DDRII_DLL_DMA264MHz_CONFIG;
            pDMASetting->uiTiming0      = DDRII_DLL_DMA264MHz_TIMING0;
            pDMASetting->uiTiming1      = DDRII_DLL_DMA264MHz_TIMING1;
            pDMASetting->uiDelayConfig  = DDRII_DLL_DMA264MHz_DLYCFG;
            pDMASetting->uiEngineer     = DDRII_DLL_DMA264MHz_ENGINEER;
#endif
            break;
        */

        default:
            debug_err(("Unsupported DMA clock!\r\n"));
            while (1)
            {
                ;
            }
            break;
        }
    }
}

/**
    Pause all modules which will affect the stability of system clock change.
*/
static void SysClk_PauseAll(void)
{
    // Ask file system to pause any read/write
    FilesysPause();
    FilesysWaitPaused();

    // Pause audio
    bIsAudioBusy = aud_isBusy();
    if (bIsAudioBusy == TRUE)
    {
        debug_ind(("Pause Audio\r\n"));
        aud_pause();
    }

    // Pause image pipeline
    IPL_Stop2Dram();

    // Pause IDE
    // Although do ide_wait_frame_end() without disable IDE seems work in most cases,
    // Chances are that it may be interrupted between ide_wait_frame_end and the following
    // clk_changeDMA() in some implementation with different tasks configuration.
    // So it is safe to always disable IDE before calling to clk_changeDMA().
    // However, it will have LCD display to be freezing or flashing for two frames time, so
    // care must be taken in switching clock frequency.
    bIDEIsEnabled = ide_get_en();

    if (bIDEIsEnabled)
    {
#if   (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_VD)
        ide_wait_frame_end();
#elif (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_WIN)
        uiIDEWinEN = ide_get_window_en();
        ide_set_allwindow_dis();
        ide_set_load();
        ide_wait_frame_end();
#elif (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_IDE)
        ide_wait_frame_end();
        ide_set_en(FALSE);
#endif
    }
}

/**
    Resume modules after system clock change.
*/
static void SysClk_ResumeAll(void)
{
    // Resume file system read / write
    FilesysResume();

    // Resume audio
    if (bIsAudioBusy == TRUE)
    {
        aud_resume();
    }

    if (bIDEIsEnabled)
    {
#if   (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_VD)
        //ide_wait_frame_end();
#elif (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_WIN)
        ide_set_allwindow_en(uiIDEWinEN);
        ide_set_load();
        ide_wait_frame_end();
#elif (CHANGE_DMA_CLOCK_IDE_OPT == WAIT_IDE_IDE)
        ide_set_en(TRUE);
        ide_wait_frame_end();
#endif
    }
}

//#NT#2009/09/30#HH Chuang -begin
/**
    Init system clock.

    Get default CPU / AHB / DMA clock, DRAM type and open clock driver.

    @note This function should be called in SysInit_InitEssentialHW().

    @return void
*/
void SysClk_Init(void)
{
    uiDMAClock = pll_getClockRate(PLL_CLKSEL_DMA);
    uiCPUClock = pll_getClockRate(PLL_CLKSEL_CPU);
    uiAHBClock = pll_getClockRate(PLL_CLKSEL_AHB);
    uiDRAMType = dma_getDRAMType();

    // Doesn't support DDR I now!
    if (uiDRAMType == DDR_TYPE_DDR1)
    {
        debug_err(("NT96630 doesn't support DDR I now!\r\n"));
        while (1)
        {
            ;
        }
    }

    clk_open();
}
//#NT#2009/09/30#HH Chuang -end

/**
    Change system clock.

    @note This function need to be customized to change specific clock for
        respective modes.

    @param uiSpeedID    Clock speed ID for respective mode
    @return TRUE if successful
*/
BOOL SysClk_SetClk(UINT32 uiSpeedID)
{
    DMA_SETTING DMASetting;
    UINT32      uiNewDMAClock, uiNewCPUClock, uiNewAHBClock;

    switch (uiSpeedID)
    {
    case SPEED_ID_PHOTO:
        // CPU = 80 MHz, AHB = 80 MHz, DMA = 240 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_80;
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK;
//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#if (SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE)
        uiNewDMAClock = PLL_CLKSEL_DMA_PLL1;
#else
        uiNewDMAClock = PLL_CLKSEL_DMA_240;
#endif
//#NT#2010/02/10#Chris Hsu -end
        break;

    case SPEED_ID_USBMSDC:
    case SPEED_ID_USBSICD:
    case SPEED_ID_USBPCC:
        // CPU = 80 MHz, AHB = 80 MHz, DMA = 120 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_80;
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK;
//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#if (SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE)
        uiNewDMAClock = PLL_CLKSEL_DMA_PLL1_D2;
#else
        uiNewDMAClock = PLL_CLKSEL_DMA_120;
#endif
//#NT#2010/02/10#Chris Hsu -end
        break;

    case SPEED_ID_CAPTURE:
    case SPEED_ID_VIDEO:
    case SPEED_ID_PLAYBACK:
        // CPU = 120 MHz, AHB = 120 MHz, DMA = 240 MHz
        uiNewCPUClock = PLL_CLKSEL_CPU_120;
        uiNewAHBClock = PLL_CLKSEL_AHB_CPUCLK;
//#NT#2010/02/10#Chris Hsu -begin
//#NT#Add code to use PLL1 as DRAM clock source
#if (SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE)
        uiNewDMAClock = PLL_CLKSEL_DMA_PLL1;
#else
        uiNewDMAClock = PLL_CLKSEL_DMA_240;
#endif
//#NT#2010/02/10#Chris Hsu -end
        break;

    default:
        return FALSE;
    }

    // Only change clock if new clock doesn't match old clock
    if (uiNewDMAClock != uiDMAClock ||
        uiNewCPUClock != uiCPUClock ||
        uiNewAHBClock != uiAHBClock)
    {
        if (uiNewDMAClock != uiDMAClock)
        {
            // Get DMA parameter according to new DMA clock
            SysClk_GetDMAParam(uiNewDMAClock, &DMASetting);

            SysClk_PauseAll();
            clk_changeDMA(uiNewDMAClock, NULL, &DMASetting);
            SysClk_ResumeAll();
            uiDMAClock = uiNewDMAClock;

            debug_ind(("DMA clock is now %ld\r\n", uiNewDMAClock));
        }

        if (uiNewCPUClock != uiCPUClock ||
            uiNewAHBClock != uiAHBClock)
        {
            clk_changeCPUAHB(uiNewCPUClock, uiNewAHBClock);
            uiCPUClock = uiNewCPUClock;
            uiAHBClock = uiNewAHBClock;

            debug_ind(("CPU/AHB clock is now %ld/%ld\r\n", uiNewCPUClock, uiNewAHBClock));
        }
    }

    return TRUE;
}

//@}
