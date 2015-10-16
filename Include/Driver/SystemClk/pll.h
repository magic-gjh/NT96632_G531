/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIHALSysCG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include "Type.h"



/** \addtogroup mIHALSysCG */
//@{

/**
    PLL oscillator source

    This is for pll_setPLLOscSrc().
*/
//@{
typedef enum
{
    PLL_OSC_SRC_PHY,       ///< PLL 1/2 Osc source is from USB PHY.
    PLL_OSC_SRC_AUD        ///< PLL 1/2 Osc source is from external audio clock.
} PLL_OSC_SRC;
//@}


#define PLL_CLKSEL_R1_OFFSET        32
#define PLL_CLKSEL_R2_OFFSET        64
#define PLL_CLKSEL_R3_OFFSET        96
#define PLL_CLKSEL_R4_OFFSET        128
#define PLL_CLKSEL_R5_OFFSET        160
#define PLL_CLKSEL_R6_OFFSET        192

/**
    Clock select ID

    This is used for pll_setClockRate() & pll_getClockRate().
*/
//@{
typedef enum
{
    PLL_CLKSEL_CPU          =  0,                               ///< Clock Select Module ID: CPU
    PLL_CLKSEL_AHB          =  4,                               ///< Clock Select Module ID: AHB bus
    PLL_CLKSEL_DMA          =  12,                              ///< Clock Select Module ID: DMA
    PLL_CLKSEL_PRE          = (PLL_CLKSEL_R1_OFFSET + 0),       ///< Clock Select Module ID: PRE
    PLL_CLKSEL_IPE          = (PLL_CLKSEL_R1_OFFSET + 4),       ///< Clock Select Module ID: IPE
    PLL_CLKSEL_DIS          = (PLL_CLKSEL_R1_OFFSET + 8),       ///< Clock Select Module ID: DIS
    PLL_CLKSEL_IME          = (PLL_CLKSEL_R1_OFFSET + 12),      ///< Clock Select Module ID: IME
    PLL_CLKSEL_FDE          = (PLL_CLKSEL_R1_OFFSET + 16),      ///< Clock Select Module ID: FDE
    PLL_CLKSEL_CRM          = (PLL_CLKSEL_R1_OFFSET + 20),      ///< Clock Select Module ID: Chroma filter
    PLL_CLKSEL_JPEG         = (PLL_CLKSEL_R2_OFFSET + 0),       ///< Clock Select Module ID: JPEG
    PLL_CLKSEL_H264         = (PLL_CLKSEL_R2_OFFSET + 4),       ///< Clock Select Module ID: H.264
    PLL_CLKSEL_GRAPHIC      = (PLL_CLKSEL_R2_OFFSET + 8),       ///< Clock Select Module ID: Graphic engine
    PLL_CLKSEL_CE           = (PLL_CLKSEL_R2_OFFSET + 16),      ///< Clock Select Module ID: Crypt engine
    PLL_CLKSEL_PSBM         = (PLL_CLKSEL_R2_OFFSET + 20),      ///< Clock Select Module ID: Pattern search and block moving engine
    PLL_CLKSEL_XD           = (PLL_CLKSEL_R3_OFFSET + 0),       ///< Clock Select Module ID: xD
    PLL_CLKSEL_SDIOMS       = (PLL_CLKSEL_R3_OFFSET + 4),       ///< Clock Select Module ID: SDIO
    PLL_CLKSEL_CF           = (PLL_CLKSEL_R3_OFFSET + 8),       ///< Clock Select Module ID: CF/ATA
    PLL_CLKSEL_SDIO2        = (PLL_CLKSEL_R3_OFFSET + 12),      ///< Clock Select Module ID: SDIO2
    PLL_CLKSEL_SIE_MCLKSRC  = (PLL_CLKSEL_R4_OFFSET + 0),       ///< Clock Select Module ID: SIE MCLK source
    PLL_CLKSEL_PRE_CLKSRC   = (PLL_CLKSEL_R4_OFFSET + 2),       ///< Clock Select Module ID: PRE clock source
    PLL_CLKSEL_IDE_CLKSRC   = (PLL_CLKSEL_R4_OFFSET + 3),       ///< Clock Select Module ID: IDE clokc source
    PLL_CLKSEL_ADO_CLKSRC   = (PLL_CLKSEL_R4_OFFSET + 4),       ///< Clock Select Module ID: Audio system clock source
    PLL_CLKSEL_ADO_INT      = (PLL_CLKSEL_R4_OFFSET + 5),       ///< Clock Select Module ID: Audio internal clock select
    PLL_CLKSEL_ADO_INTMETHOD= (PLL_CLKSEL_R4_OFFSET + 6),       ///< Clock Select Module ID: Audio internal clock generation method
    PLL_CLKSEL_SIE_MCLKDIV  = (PLL_CLKSEL_R4_OFFSET + 8),       ///< Clock Select Module ID: SIE MCLK divider
    PLL_CLKSEL_IDE_CLKDIV   = (PLL_CLKSEL_R4_OFFSET + 16),      ///< Clock Select Module ID: IDE clock divider
    PLL_CLKSEL_ADO_CLKMUX   = (PLL_CLKSEL_R4_OFFSET + 24),      ///< Clock Select Module ID: Audio clock MUX
    PLL_CLKSEL_SP_CLKDIV    = (PLL_CLKSEL_R5_OFFSET + 0),       ///< Clock Select Module ID: Special clock divider
    PLL_CLKSEL_ADO_CLKDIV   = (PLL_CLKSEL_R5_OFFSET + 16),      ///< Clock Select Module ID: Audio clock divider
    PLL_CLKSEL_CEC_CLKDIV   = (PLL_CLKSEL_R6_OFFSET + 0),       ///< Clock Select Module ID: CEC clock divider

    ENUM_DUMMY4WORD(PLL_CLKSEL)
}PLL_CLKSEL;
//@}


/**
    CPU clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_CPU).

    @name   CPU clock rate value
*/
//@{
#define PLL_CLKSEL_CPU_240          (0x00 << PLL_CLKSEL_CPU)    ///< Select CPU clock 240MHz
#define PLL_CLKSEL_CPU_160          (0x01 << PLL_CLKSEL_CPU)    ///< Select CPU clock 160MHz
#define PLL_CLKSEL_CPU_120          (0x02 << PLL_CLKSEL_CPU)    ///< Select CPU clock 120MHz
#define PLL_CLKSEL_CPU_80           (0x03 << PLL_CLKSEL_CPU)    ///< Select CPU clock 80MHz
#define PLL_CLKSEL_CPU_PLL1         (0x04 << PLL_CLKSEL_CPU)    ///< Select CPU clock as PLL1
#define PLL_CLKSEL_CPU_PLL1_D2      (0x05 << PLL_CLKSEL_CPU)    ///< Select CPU clock as PLL1/2
#define PLL_CLKSEL_CPU_PLL2         (0x06 << PLL_CLKSEL_CPU)    ///< Select CPU clock as PLL2
#define PLL_CLKSEL_CPU_PLL2_D2      (0x07 << PLL_CLKSEL_CPU)    ///< Select CPU clock as PLL2/2
//@}

/**
    AHB clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_AHB).

    @name   AHB clock rate value
*/
//@{
#define PLL_CLKSEL_AHB_CPUCLK       (0x00 << PLL_CLKSEL_AHB)    ///< Select AHB clock as CPU clock
#define PLL_CLKSEL_AHB_CPUCLK_D2    (0x01 << PLL_CLKSEL_AHB)    ///< Select AHB clock as (CPU clock/2)
#define PLL_CLKSEL_AHB_CPUCLK_D3    (0x02 << PLL_CLKSEL_AHB)    ///< Select AHB clock as (CPU clock/3)
#define PLL_CLKSEL_AHB_CPUCLK_D4    (0x03 << PLL_CLKSEL_AHB)    ///< Select AHB clock as (CPU clock/4)
//@}



/**
    DMA clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_DMA).

    @name   DMA clock rate value
*/
//@{
#define PLL_CLKSEL_DMA_240          (0x00 << PLL_CLKSEL_DMA)    ///< Select DMA clock 240MHz
#define PLL_CLKSEL_DMA_160          (0x01 << PLL_CLKSEL_DMA)    ///< Select DMA clock 160MHz
#define PLL_CLKSEL_DMA_120          (0x02 << PLL_CLKSEL_DMA)    ///< Select DMA clock 120MHz
#define PLL_CLKSEL_DMA_80           (0x03 << PLL_CLKSEL_DMA)    ///< Select DMA clock 80MHz
#define PLL_CLKSEL_DMA_PLL1         (0x04 << PLL_CLKSEL_DMA)    ///< Select DMA clock as PLL1
#define PLL_CLKSEL_DMA_PLL1_D2      (0x05 << PLL_CLKSEL_DMA)    ///< Select DMA clock as PLL1/2
#define PLL_CLKSEL_DMA_PLL1_D3      (0x06 << PLL_CLKSEL_DMA)    ///< Select DMA clock as PLL1/3
#define PLL_CLKSEL_DMA_PLL1_D4      (0x07 << PLL_CLKSEL_DMA)    ///< Select DMA clock as PLL1/4
//@}

/**
    PRE clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_PRE).

    @name   PRE clock rate value
*/
//@{
#define PLL_CLKSEL_PRE_96           (0x00 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R1_OFFSET))   ///< Select PRE clock as 96MHz
#define PLL_CLKSEL_PRE_80           (0x01 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R1_OFFSET))   ///< Select PRE clock as 80MHz
#define PLL_CLKSEL_PRE_60           (0x02 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R1_OFFSET))   ///< Select PRE clock as 60MHz
#define PLL_CLKSEL_PRE_40           (0x03 << (PLL_CLKSEL_PRE - PLL_CLKSEL_R1_OFFSET))   ///< Select PRE clock as 40MHz
//@}

/**
    IPE clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_IPE).

    @name   IPE clock rate value
*/
//@{
#define PLL_CLKSEL_IPE_80           (0x00 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET))   ///< Select IPE clock as 80MHz
#define PLL_CLKSEL_IPE_60           (0x01 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET))   ///< Select IPE clock as 60MHz
#define PLL_CLKSEL_IPE_48           (0x02 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET))   ///< Select IPE clock as 48MHz
#define PLL_CLKSEL_IPE_40           (0x03 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET))   ///< Select IPE clock as 40MHz
//@}

/**
    DIS clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_DIS).

    @name   DIS clock rate value
*/
//@{
#define PLL_CLKSEL_DIS_80           (0x00 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R1_OFFSET))   ///< Select DIS clock as 80MHz
#define PLL_CLKSEL_DIS_60           (0x01 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R1_OFFSET))   ///< Select DIS clock as 60MHz
#define PLL_CLKSEL_DIS_48           (0x02 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R1_OFFSET))   ///< Select DIS clock as 48MHz
#define PLL_CLKSEL_DIS_40           (0x03 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R1_OFFSET))   ///< Select DIS clock as 40MHz
//@}

/**
    IME clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_IME).

    @name   IME clock rate value
*/
//@{
#define PLL_CLKSEL_IME_120          (0x00 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET))   ///< Select IME clock as 120MHz
#define PLL_CLKSEL_IME_96           (0x01 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET))   ///< Select IME clock as 96MHz
#define PLL_CLKSEL_IME_80           (0x02 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET))   ///< Select IME clock as 80MHz
#define PLL_CLKSEL_IME_60           (0x03 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET))   ///< Select IME clock as 60MHz
//@}

/**
    FDE clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_FDE).

    @name   FDE clock rate value
*/
//@{
#define PLL_CLKSEL_FDE_80           (0x00 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R1_OFFSET))   ///< Select FDE clock as 80MHz
#define PLL_CLKSEL_FDE_60           (0x01 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R1_OFFSET))   ///< Select FDE clock as 60MHz
#define PLL_CLKSEL_FDE_40           (0x02 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R1_OFFSET))   ///< Select FDE clock as 40MHz
#define PLL_CLKSEL_FDE_30           (0x03 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R1_OFFSET))   ///< Select FDE clock as 30MHz
//@}

/**
    CRM clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_CRM).

    @name   CRM clock rate value
*/
//@{
#define PLL_CLKSEL_CRM_120          (0x00 << (PLL_CLKSEL_CRM - PLL_CLKSEL_R1_OFFSET))   ///< Select CRM clock as 120MHz
#define PLL_CLKSEL_CRM_96           (0x01 << (PLL_CLKSEL_CRM - PLL_CLKSEL_R1_OFFSET))   ///< Select CRM clock as 96MHz
#define PLL_CLKSEL_CRM_80           (0x02 << (PLL_CLKSEL_CRM - PLL_CLKSEL_R1_OFFSET))   ///< Select CRM clock as 80MHz
#define PLL_CLKSEL_CRM_60           (0x03 << (PLL_CLKSEL_CRM - PLL_CLKSEL_R1_OFFSET))   ///< Select CRM clock as 60MHz
//@}

/**
    JPEG clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_JPEG).

    @name   JPEG clock rate value
*/
//@{
#define PLL_CLKSEL_JPEG_120         (0x01 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as 120MHz
//#NT#2010/01/12#Klins Chen -begin
#define PLL_CLKSEL_JPEG_96          (0x02 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as 90MHz
//#NT#2010/01/12#Klins Chen -end
#define PLL_CLKSEL_JPEG_80          (0x03 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as 800MHz
#define PLL_CLKSEL_JPEG_PLL1_D2     (0x04 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as PLL1/2
#define PLL_CLKSEL_JPEG_PLL1_D3     (0x05 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as PLL1/3
#define PLL_CLKSEL_JPEG_PLL2_D2     (0x06 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as PLL2/2
#define PLL_CLKSEL_JPEG_PLL2_D3     (0x07 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R2_OFFSET))   ///< Select JPEG clock as PLL2/3
//@}

/**
    H264 clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_H264).

    @name   H264 clock rate value
*/
//@{
#define PLL_CLKSEL_H264_120         (0x00 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as 120MHz
#define PLL_CLKSEL_H264_96          (0x01 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as 96MHz
#define PLL_CLKSEL_H264_80          (0x02 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as 80MHz
#define PLL_CLKSEL_H264_60          (0x03 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as 60MHz
#define PLL_CLKSEL_H264_PLL1_D2     (0x04 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as PLL1/2
#define PLL_CLKSEL_H264_PLL1_D3     (0x05 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as PLL1/3
#define PLL_CLKSEL_H264_PLL2_D3     (0x06 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as PLL2/2
#define PLL_CLKSEL_H264_PLL2_D4     (0x07 << (PLL_CLKSEL_H264 - PLL_CLKSEL_R2_OFFSET))   ///< Select H.264 clock as PLL2/3
//@}

/**
    Graphic clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_GRAPHIC).

    @name   Graphic clock rate value
*/
//@{
#define PLL_CLKSEL_GRAPHIC_120      (0x01 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as 120MHz
#define PLL_CLKSEL_GRAPHIC_96       (0x02 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as 96MHz
#define PLL_CLKSEL_GRAPHIC_80       (0x03 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as 80MHz
#define PLL_CLKSEL_GRAPHIC_PLL1     (0x04 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as PLL1
#define PLL_CLKSEL_GRAPHIC_PLL1_D2  (0x05 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as PLL1/2
#define PLL_CLKSEL_GRAPHIC_PLL2     (0x06 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as PLL2
#define PLL_CLKSEL_GRAPHIC_PLL2_D2  (0x07 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R2_OFFSET))   ///< Select Graphic clock as PLL2/2
//@}

/**
    Data Encryption clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_CE).

    @name   Data Encryption clock rate value
*/
//@{
#define PLL_CLKSEL_CE_160           (0x00 << (PLL_CLKSEL_CE - PLL_CLKSEL_R2_OFFSET))   ///< Select Crypt Engine clock as 160MHz
#define PLL_CLKSEL_CE_120           (0x01 << (PLL_CLKSEL_CE - PLL_CLKSEL_R2_OFFSET))   ///< Select Crypt Engine clock as 120MHz
#define PLL_CLKSEL_CE_96            (0x02 << (PLL_CLKSEL_CE - PLL_CLKSEL_R2_OFFSET))   ///< Select Crypt Engine clock as 96MHz
#define PLL_CLKSEL_CE_80            (0x03 << (PLL_CLKSEL_CE - PLL_CLKSEL_R2_OFFSET))   ///< Select Crypt Engine clock as 80MHz
//@}

/**
    PSBM clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_PSBM).

    @name   PSBM clock rate value
*/
//@{
#define PLL_CLKSEL_PSBM_80          (0x00 << (PLL_CLKSEL_PSBM - PLL_CLKSEL_R2_OFFSET))  ///< Select PSBM clock as 80MHz
#define PLL_CLKSEL_PSBM_60          (0x01 << (PLL_CLKSEL_PSBM - PLL_CLKSEL_R2_OFFSET))  ///< Select PSBM clock as 60MHz
#define PLL_CLKSEL_PSBM_48          (0x02 << (PLL_CLKSEL_PSBM - PLL_CLKSEL_R2_OFFSET))  ///< Select PSBM clock as 48MHz
#define PLL_CLKSEL_PSBM_30          (0x03 << (PLL_CLKSEL_PSBM - PLL_CLKSEL_R2_OFFSET))  ///< Select PSBM clock as 30MHz
//@}

/**
    SM clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_SM).

    @name   SM clock rate value
*/
//@{
#define PLL_CLKSEL_XD_60            (0x00 << (PLL_CLKSEL_XD - PLL_CLKSEL_R3_OFFSET))  ///< Select xD clock as 60MHz
#define PLL_CLKSEL_XD_48            (0x01 << (PLL_CLKSEL_XD - PLL_CLKSEL_R3_OFFSET))  ///< Select xD clock as 48MHz
#define PLL_CLKSEL_XD_40            (0x02 << (PLL_CLKSEL_XD - PLL_CLKSEL_R3_OFFSET))  ///< Select xD clock as 40MHz
#define PLL_CLKSEL_XD_20            (0x03 << (PLL_CLKSEL_XD - PLL_CLKSEL_R3_OFFSET))  ///< Select xD clock as 20MHz
//@}

/**
    SDIO / MS clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_SDIOMS).

    @name   SDIO / MS clock rate value
*/
//@{
#define PLL_CLKSEL_SDIOMS_60        (0x00 << (PLL_CLKSEL_SDIOMS - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO/MS clock as 60MHz
#define PLL_CLKSEL_SDIOMS_48        (0x01 << (PLL_CLKSEL_SDIOMS - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO/MS clock as 48MHz
#define PLL_CLKSEL_SDIOMS_40        (0x02 << (PLL_CLKSEL_SDIOMS - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO/MS clock as 40MHz
#define PLL_CLKSEL_SDIOMS_24        (0x03 << (PLL_CLKSEL_SDIOMS - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO/MS clock as 24MHz
#define PLL_CLKSEL_SDIOMS_20        (0x04 << (PLL_CLKSEL_SDIOMS - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO/MS clock as 20MHz
//@}

/**
    CF clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_CF).

    @name   CF clock rate value
*/
//@{
#define PLL_CLKSEL_CF_120           (0x00 << (PLL_CLKSEL_CF - PLL_CLKSEL_R3_OFFSET))  ///< Select CF/ATA clock as 120MHz
#define PLL_CLKSEL_CF_96            (0x01 << (PLL_CLKSEL_CF - PLL_CLKSEL_R3_OFFSET))  ///< Select CF/ATA clock as 96MHz
#define PLL_CLKSEL_CF_80            (0x02 << (PLL_CLKSEL_CF - PLL_CLKSEL_R3_OFFSET))  ///< Select CF/ATA clock as 80MHz
#define PLL_CLKSEL_CF_60            (0x03 << (PLL_CLKSEL_CF - PLL_CLKSEL_R3_OFFSET))  ///< Select CF/ATA clock as 60MHz
//@}

/**
    SDIO2 clock rate value

    This is used for pll_setClockRate(PLL_CLKSEL_SDIO2).

    @name   SDIO2 clock rate value
*/
//@{
#define PLL_CLKSEL_SDIO2_48         (0x00 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO2 clock as 48MHz
#define PLL_CLKSEL_SDIO2_24         (0x01 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO2 clock as 24MHz
#define PLL_CLKSEL_SDIO2_20         (0x02 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R3_OFFSET))  ///< Select SDIO2 clock as 20MHz
//@}

/**
    SIE MCLK clock source value

    This is used for pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC).

    @name   SIE MCLK clock source value
*/
//@{
#define PLL_CLKSEL_SIE_MCLKSRC_480  (0x01 << (PLL_CLKSEL_SIE_MCLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select SIE MCLK clock source as 480MHz
#define PLL_CLKSEL_SIE_MCLKSRC_PLL1 (0x02 << (PLL_CLKSEL_SIE_MCLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select SIE MCLK clock source as PLL1
#define PLL_CLKSEL_SIE_MCLKSRC_PLL2 (0x03 << (PLL_CLKSEL_SIE_MCLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select SIE MCLK clock source as PLL2
//@}

/**
    PRE clock source value

    This is used for pll_setClockRate(PLL_CLKSEL_PRE_CLKSRC).

    @name   PRE clock source value
*/
//@{
#define PLL_CLKSEL_PRE_CLKSRC_PRE   (0x00 << (PLL_CLKSEL_PRE_CLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select PRE clock source from PRE_CLKSEL
#define PLL_CLKSEL_PRE_CLKSRC_PXCLK (0x01 << (PLL_CLKSEL_PRE_CLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select PRE clock source from PXCLK pin.
//@}

/**
    IDE clock source value

    This is used for pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC).

    @name   IDE clock source value
*/
//@{
#define PLL_CLKSEL_IDE_CLKSRC_240   (0x00 << (PLL_CLKSEL_IDE_CLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select IDE clock source as 240MHz
#define PLL_CLKSEL_IDE_CLKSRC_PLL2  (0x01 << (PLL_CLKSEL_IDE_CLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select IDE clock source as PLL2
//@}

/**
    Audio clock source value

    This is used for pll_setClockRate(PLL_CLKSEL_ADO_CLKSRC).

    @name   Audio clock source value
*/
//@{
#define PLL_CLKSEL_ADO_CLKSRC_EXT   (0x00 << (PLL_CLKSEL_ADO_CLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select Audio clock source external clock
#define PLL_CLKSEL_ADO_CLKSRC_INT   (0x01 << (PLL_CLKSEL_ADO_CLKSRC - PLL_CLKSEL_R4_OFFSET))  ///< Select Audio clock source internal clock
//@}

/**
    Audio internal clock source value

    This is used for pll_setClockRate(PLL_CLKSEL_ADO_INT).

    @name   Audio internal clock source value
*/
//@{
#define PLL_CLKSEL_ADO_INT_CLK      (0x00 << (PLL_CLKSEL_ADO_INT - PLL_CLKSEL_R4_OFFSET))  ///< Select Audio internal clock source from internal audio clock
#define PLL_CLKSEL_ADO_INT_CRY      (0x01 << (PLL_CLKSEL_ADO_INT - PLL_CLKSEL_R4_OFFSET))  ///< Select Audio internal clock source from internal crystal pad
//@}

/**
    Audio internal clock method value

    This is used for pll_setClockRate(PLL_CLKSEL_ADO_INT).

    @name   Audio internal clock method value
*/
//@{
#define PLL_CLKSEL_ADO_INTMETHOD_MUX    (0x00 << (PLL_CLKSEL_ADO_INTMETHOD - PLL_CLKSEL_R4_OFFSET))  ///< Select Audio internal generation method by ADO_CLKMUX
#define PLL_CLKSEL_ADO_INTMETHOD_DIV    (0x01 << (PLL_CLKSEL_ADO_INTMETHOD - PLL_CLKSEL_R4_OFFSET))  ///< Select Audio internal generation method by ADO_CLKDIV
//@}

/**
    Audio internal clock mux value

    This is used for pll_setClockRate(PLL_CLKSEL_ADO_CLKMUX).

    @name   Audio internal clock mux value
*/
//@{
#define PLL_CLKSEL_ADO_CLKMUX_24P576    (0x00 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 24.5760 MHz
#define PLL_CLKSEL_ADO_CLKMUX_12P288    (0x01 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 12.2880 MHz
#define PLL_CLKSEL_ADO_CLKMUX_8P192     (0x02 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 8.1920 MHz
#define PLL_CLKSEL_ADO_CLKMUX_6P144     (0x03 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 6.1440 MHz
#define PLL_CLKSEL_ADO_CLKMUX_4P096     (0x04 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 4.0960 MHz
#define PLL_CLKSEL_ADO_CLKMUX_3P072     (0x05 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 3.0720 MHz
#define PLL_CLKSEL_ADO_CLKMUX_2P048     (0x06 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 2.0480 MHz
#define PLL_CLKSEL_ADO_CLKMUX_11P2896   (0x07 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 11.2896 MHz
#define PLL_CLKSEL_ADO_CLKMUX_5P6448    (0x08 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 5.6448 MHz
#define PLL_CLKSEL_ADO_CLKMUX_2P8224    (0x09 << (PLL_CLKSEL_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))  ///< 2.8224 MHz
//@}


/**
    SIE clock divider

    This definition is used for setting SIE Mclk divider

    @name   SIE clock divider
*/
//@{
#define PLL_SIE_MCLKDIV(x)               (x << (PLL_CLKSEL_SIE_MCLKDIV - PLL_CLKSEL_R4_OFFSET)) ///< Used for pll_setClockRate(PLL_CLKSEL_SIE_MCLKDIV)
//@}

/**
    IDE clock divider

    This definition is used for setting IDE clock divider

    @name   IDE clock divider
*/
//@{
#define PLL_IDE_CLKDIV(x)               (x << (PLL_CLKSEL_IDE_CLKDIV - PLL_CLKSEL_R4_OFFSET))   ///< Used for pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV)
//@}

/**
    SP clock divider

    This definition is used for setting special clock divider

    @name   Special clock divider
*/
//@{
#define PLL_SP_CLKDIV(x)               (x << (PLL_CLKSEL_SP_CLKDIV - PLL_CLKSEL_R5_OFFSET))     ///< Used for pll_setClockRate(PLL_CLKSEL_SP_CLKDIV)
//@}

/**
    Audio clock divider

    This definition is used for setting audio clock divider

    @name   Audio clock divider
*/
//@{
#define PLL_ADO_CLKDIV(x)               (x << (PLL_CLKSEL_ADO_CLKDIV - PLL_CLKSEL_R5_OFFSET))   ///< Used for pll_setClockRate(PLL_CLKSEL_ADO_CLKDIV)
//@}

/**
    CEC clock divider

    This definition is used for setting cec clock divider

    @name   CEC clock divider
*/
//@{
#define PLL_CEC_CLKDIV(x)               (x << (PLL_CLKSEL_CEC_CLKDIV - PLL_CLKSEL_R6_OFFSET))   ///< Used for pll_setClockRate(PLL_CLKSEL_CEC_CLKDIV)
//@}



/**
    Clock Enable ID

    This definition is used for pll_enableClock() and pll_disableClock().
*/
//@{
typedef enum
{
    BOOT_HCLK,              ///< Boot controller AHB clock
    DMA_CLK,                ///< DMA(SDRAM) clock
    SIE_MCLK = 4,           ///< SIE MCLK
    SIE_PXCLK,              ///< SIE pixel clock
    PRE_CLK,                ///< PRE clock
    IPE_CLK,                ///< IPE clock
    DIS_CLK,                ///< DIS clock
    IME_CLK,                ///< IME clock
    FDE_CLK,                ///< FDE clock
    CRM_CLK,                ///< Chroma filter clock
    SP_CLK,                 ///< special clock
    IDE1_CLK = 16,          ///< IDE clock - 1
    IDE2_CLK,               ///< IDE clock - 2
    JPG_CLK,                ///< JPEG clock
    H264_CLK,               ///< H.264 clock
    GRAPH_CLK,              ///< Graphic engine clock
    HDMI_CLK,               ///< HDMI clock
    CEC_CLK,                ///< CEC clock
    USB_HCLK = 24,          ///< USB AHB clock
    DAI_CLK,                ///< DAI clock
    ADO_SCLK,               ///< Audio clock
    XD_CLK = 32,            ///< xD clock
    SDIO_CLK,               ///< SDIO clock
    MS_CLK,                 ///< MS clock
    CF_CLK,                 ///< CF clock
    SDIO2_CLK,              ///< SDIO2 clock
    I2C_CLK = 32 + 8,       ///< I2C clock
    SIF_CLK,                ///< SIF clock
    SPI_CLK,                ///< SPI clock
    UART_CLK,               ///< UART clock
    UART2_CLK,              ///< UART2 clock
    REMOTE_CLK,             ///< Remote clock
    RTC_CLK = 32 + 16,      ///< RTC clock
    ADC_CLK,                ///< ADC clock
    WDT_CLK,                ///< Watch dog timer clock
    CE_CLK = 32 + 24,       ///< Crypt engine clock
    PSBM_CLK,               ///< PSBM clock
    TMR_PCLK = 64,          ///< Timer APB clock
    TMR0_CLK,               ///< Timer-0 clock
    TMR1_CLK,               ///< Timer-1 clock
    TMR2_CLK,               ///< Timer-2 clock
    TMR3_CLK,               ///< Timer-3 clock
    TMR4_CLK,               ///< Timer-4 clock
    TMR5_CLK,               ///< Timer-5 clock
    TMR6_CLK,               ///< Timer-6 clock
    TMR7_CLK,               ///< Timer-7 clock
    PWMCCNT_CLK = 64 + 16,  ///< PWM CCNT clock(3 MHz)
    PWM0_CLK,               ///< PWM-0 clock
    PWM1_CLK,               ///< PWM-1 clock
    PWM2_CLK,               ///< PWM-2 clock
    PWM3_CLK,               ///< PWM-3 clock
    PWM4_CLK,               ///< PWM-4 clock
    PWM5_CLK,               ///< PWM-5 clock
    PWM6_CLK,               ///< PWM-6 clock
    PWM7_CLK,               ///< PWM-7 clock

    ENUM_DUMMY4WORD(CG_EN)
} CG_EN;
//@}

/**
    System Reset ID

    This definition is used for pll_enableSystemReset() and pll_disableSystemReset()
*/
//@{
typedef enum
{
    PLL_RSTN_DRAMC,         ///< Reset SDRAM controller
    BOOT_RSTN,              ///< Reset Boot ROM controller
    BRG_RSTN,               ///< Reset APB bridge controller
    DATAPATH_RSTN,          ///< Reset AMBA decoder
    INTC_RSTN,              ///< Reset INTERRUPT controller
    GPIO_RSTN,              ///< Reset GPIO controller
    PAD_RSTN,               ///< Reset PAD controller
    TOP_RSTN,               ///< Reset TOP controller
    SIE_RSTN = 12,          ///< Reset SIE controller
    PRE_RSTN,               ///< Reset PRE controller
    IPE_RSTN,               ///< Reset IPE controller
    DIS_RSTN,               ///< Reset DIS controller
    IME_RSTN,               ///< Reset IME controller
    FDE_RSTN,               ///< Reset FDE controller
    IDE_RSTN = 20,          ///< Reset IDE controller
    JPG_RSTN,               ///< Reset JPEG controller
    H264_RSTN,              ///< Reset H.264 controller
    GRAPH_RSTN,             ///< Reset Graphic controller
    HDMI_RSTN,              ///< Reset HDMI controller
    USB_RSTN = 28,          ///< Reset USB controller
    ADO_RSTN,               ///< Reset AUDIO controller
    DAI_RSTN,               ///< Reset DAI controller
    XD_RSTN = 32,           ///< Reset xD controller
    SDIO_RSTN,              ///< Reset SDIO controller
    MS_RSTN,                ///< Reset MS controller
    CF_RSTN,                ///< Reset CF/ATA controller
    SDIO2_RSTN,             ///< Reset SDIO2 controller
    I2C_RSTN = 32 + 8,      ///< Reset I2C controller
    SIF_RSTN,               ///< Reset SIF controller
    SPI_RSTN,               ///< Reset SPI controller
    UART_RSTN,              ///< Reset UART controller
    UART2_RSTN,             ///< Reset UART2 controller
    REMOTE_RSTN,            ///< Reset Remote controller
    RTC_RSTN = 32 + 16,     ///< Reset RTC controller
    ADC_DRSTN,              ///< Reset ADC Digital circuit
    ADC_ARSTN,              ///< Reset ADC Analog circuit
    WDT_RSTN,               ///< Reset Watch dog timer controller
    TMR_RSTN,               ///< Reset TIMER module
    PWM_RSTN,               ///< Reset PWM controller
    CE_RSTN = 32 + 24,      ///< Reset Crypt controller
    PSBM_RSTN,               ///< Reset PSBM controller

    ENUM_DUMMY4WORD(CG_RSTN)
} CG_RSTN;
//@}

//#NT#2010/01/12#Klins Chen -begin
typedef enum
{
    SIEMCLK_FREQ,           ///< SIE MCLK Select ID
    IDECLK_FREQ,            ///< IDE CLK Select ID
    SPCLK_FREQ,             ///< SP CLK Select ID
    ADOCLK_FREQ,            ///< Audio CLK Select ID
    CECCLK_FREQ,            ///< CEC CLK Select ID

    PLL_CLKFREQ_MAXNUM,
    ENUM_DUMMY4WORD(PLL_CLKFREQ)
} PLL_CLKFREQ;
//#NT#2010/01/12#Klins Chen -end


extern void    pll_setPLLOscSrc(PLL_OSC_SRC OscSrc);

extern void    pll_setPLL1(UINT32 uiLoopDiv, UINT32 uiPreDiv);
extern void    pll_getPLL1(UINT32 *puiLoopDiv, UINT32 *puiPreDiv);
extern void    pll_setPLL1En(BOOL bEnable);
extern UINT32  pll_getPLL1Freq(void);
extern void    pll_setPLL2(UINT32 uiLoopDiv, UINT32 uiPreDiv);
extern void    pll_getPLL2(UINT32 *puiLoopDiv, UINT32 *puiPreDiv);
extern void    pll_setPLL2En(BOOL bEnable);
extern UINT32  pll_getPLL2Freq(void);

extern void    pll_setPLLOSC(UINT32 uiOSC);
extern UINT32  pll_getPLLOSC(void);

extern BOOL    pll_getPhyEnable(void);
extern BOOL    pll_getPhyReady(void);

extern void    pll_setCPUCCKR(UINT32 uiCCKR);
extern void    pll_setClockRate(PLL_CLKSEL uiNum, UINT32 uiValue);
extern UINT32  pll_getClockRate(PLL_CLKSEL uiNum);
extern void    pll_enableClock(CG_EN Num);
extern void    pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);

extern void    pll_disableClock(CG_EN Num);
extern BOOL    pll_isClockEnabled(CG_EN Num);
//#NT#2010/01/12#Klins Chen -begin
extern ER      pll_setClockFreq(PLL_CLKFREQ ClkID, UINT32 uiFreq);
//#NT#2010/01/12#Klins Chen -end

extern void    pll_enableSystemReset(CG_RSTN Num);
extern void    pll_disableSystemReset(CG_RSTN Num);
//@}

#endif

