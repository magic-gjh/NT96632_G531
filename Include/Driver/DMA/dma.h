/**
    Header file for DMA module.

    This file is the header file that define the API for DMA module.

    @file       DMA.h
    @ingroup    mIHALDMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _DMA_H
#define _DMA_H

#include "Type.h"

/**
    @addtogroup mIHALDMA
*/
//@{

/**
    @name DMA priority setting

    NT96630 DMA priority setting
    (High > Middle > Normal)

    @note Used in dma_setChannelPriority(), dma_getChannelPriority()
*/
//@{
#define DMA_PRIORITY_NORMAL     0   ///< Normal(default/low) priority
#define DMA_PRIORITY_MIDDLE     1   ///< Middle priority
#define DMA_PRIORITY_HIGH       2   ///< High priority
//@}

/**
    @name DMA channel encoding

    NT96630 DMA channel setting

    @note Used in dma_setChannelPriority(), dma_getChannelPriority()
*/
//@{
#define DMA_CH_AHBC             1   ///< AHBC
#define DMA_CH_USB              2   ///< USB
#define DMA_CH_H264_SRC         3   ///< H.264 SRC
#define DMA_CH_H264_URP         4   ///< H.264 URP
#define DMA_CH_H264_REC         5   ///< H.264 REC
#define DMA_CH_H264_NT          6   ///< H.264 NT
#define DMA_CH_H264_SW          7   ///< H.264 SW
#define DMA_CH_H264_BS          8   ///< H.264 BS
#define DMA_CH_PSBM             9   ///< PSBM
#define DMA_CH_JPG_IMG          10  ///< JPEG IMG
#define DMA_CH_JPG_BS           11  ///< JPEG BS
#define DMA_CH_SIE_DVO          12  ///< SIE DVO
#define DMA_CH_SM               13  ///< SM/xD/NAND
#define DMA_CH_PRE_DVI          14  ///< PRE DVI
#define DMA_CH_PRE_DPC          15  ///< PRE DPC
#define DMA_CH_PRE_ECS          16  ///< PRE ECS
#define DMA_CH_PRE_PREO         17  ///< PRE PREO
#define DMA_CH_IPE_IN           18  ///< IPE IN
#define DMA_CH_IPE_OUT1         19  ///< IPE OUT1
#define DMA_CH_IPE_OUT2         20  ///< IPE OUT2
#define DMA_CH_DIS1             22  ///< DIS1
#define DMA_CH_DIS2             23  ///< DIS2
#define DMA_CH_IME1_Y           24  ///< IME1 Y
#define DMA_CH_IME1_CB          25  ///< IME1 CB
#define DMA_CH_IME1_CR          26  ///< IME1 CR
#define DMA_CH_IME2_Y           27  ///< IME2 Y
#define DMA_CH_IME2_CB          28  ///< IME2 CB
#define DMA_CH_IME2_CR          29  ///< IME2 CR
#define DMA_CH_IME3_Y           30  ///< IME3 Y
#define DMA_CH_IME3_CB          31  ///< IME3 CB

#define DMA_CH_IME3_CR          (32+0)  ///< IME3 CR
#define DMA_CH_IDEV1_Y          (32+1)  ///< IDEV1 Y
#define DMA_CH_IDEV1_CB         (32+2)  ///< IDEV1 CB
#define DMA_CH_IDEV1_CR         (32+3)  ///< IDEV1 CR
#define DMA_CH_IDEV2_Y          (32+4)  ///< IDEV2 Y
#define DMA_CH_IDEV2_CB         (32+5)  ///< IDEV2 CB
#define DMA_CH_IDEV2_CR         (32+6)  ///< IDEV2 CR
#define DMA_CH_IDE_OSD1         (32+7)  ///< IDE OSD1
#define DMA_CH_IDE_OSD2         (32+8)  ///< IDE OSD2
#define DMA_CH_FD_CH0           (32+9)  ///< FD CH0
#define DMA_CH_FD_CH1           (32+10) ///< FD CH1
#define DMA_CH_FD_CH2           (32+11) ///< FD CH2
#define DMA_CH_DAI_PLY          (32+12) ///< DAI PLY
#define DMA_CH_DAI_REC          (32+13) ///< DAI REC
#define DMA_CH_GRAPH0           (32+14) ///< GRAPH0
#define DMA_CH_GRAPH1           (32+15) ///< GRAPH1
#define DMA_CH_STGFIFO          (32+16) ///< STG FIFO
#define DMA_CH_SDIO             (32+17) ///< SDIO
#define DMA_CH_CF               (32+18) ///< CF
//@}
#define DMA_CH_COUNT            (32+19)

/**
    @name Old DMA priority setting

    NT96630 DMA priority setting (old version).

    @note Use them in dma_setPriority() and dma_getPriority() \n

    Example: Set AHBC, PSBM, STGFIFO to higher priority\n
        dma_setPriority(P0_AHBC|P0_PSBM, P1_STGFIFO);
*/
//@{
// reserve: 1<<0
#define P0_AHBC                 (1<<1)          ///< priority of AHB controller
#define P0_USB                  (1<<2)          ///< priority of USB controller
#define P0_H264_SRC             (1<<3)          ///< priority of H.264 SRC
#define P0_H264_URP             (1<<4)          ///< priority of H.264 URP
#define P0_H264_REC             (1<<5)          ///< priority of H.264 REC
#define P0_H264_NT              (1<<6)          ///< priority of H.264 NT
#define P0_H264_SW              (1<<7)          ///< priority of H.264 SW
#define P0_H264_BS              (1<<8)          ///< priority of H.264 BS
#define P0_PSBM                 (1<<9)          ///< priority of PSBM
#define P0_JPG_IMG              (1<<10)         ///< priority of JPEG image
#define P0_JPG_BS               (1<<11)         ///< priority of JPEG bit stream
#define P0_SIE_DVO              (1<<12)         ///< priority of SIE DVO
#define P0_SM                   (1<<13)         ///< priority of SM/xD/NAND controller
#define P0_PRE_DVI              (1<<14)         ///< priority of PRE DVI
#define P0_PRE_DPC              (1<<15)         ///< priority of PRE DPC
#define P0_PRE_ECS              (1<<16)         ///< priority of PRE ECS
#define P0_PRE_PREO             (1<<17)         ///< priority of PRE PREO
#define P0_IPE_IN               (1<<18)         ///< priority of IPE IN
#define P0_IPE_OUT1             (1<<19)         ///< priority of IPE OUT1
#define P0_IPE_OUT2             (1<<20)         ///< priority of IPE OUT2
// reserve: 1<<21
#define P0_DIS1                 (1<<22)         ///< priority of DIS1
#define P0_DIS2                 (1<<23)         ///< priority of DIS2
#define P0_IME1_Y               (1<<24)         ///< priority of IME1 Y
#define P0_IME1_CB              (1<<25)         ///< priority of IME1 CB
#define P0_IME1_CR              (1<<26)         ///< priority of IME1 CR
#define P0_IME2_Y               (1<<27)         ///< priority of IME2 Y
#define P0_IME2_CB              (1<<28)         ///< priority of IME2 CB
#define P0_IME2_CR              (1<<29)         ///< priority of IME2 CR
#define P0_IME3_Y               (1<<30)         ///< priority of IME3 Y
#define P0_IME3_CB              (1<<31)         ///< priority of IME3 CB

#define P1_IME3_CR              (1<<0)          ///< priority of IME3 CR
#define P1_IDEV1_Y              (1<<1)          ///< priority of IDEV1 Y
#define P1_IDEV1_CB             (1<<2)          ///< priority of IDEV1 CB
#define P1_IDEV1_CR             (1<<3)          ///< priority of IDEV1 CR
#define P1_IDEV2_Y              (1<<4)          ///< priority of IDEV2 Y
#define P1_IDEV2_CB             (1<<5)          ///< priority of IDEV2 CB
#define P1_IDEV2_CR             (1<<6)          ///< priority of IDEV2 CR
#define P1_IDE_OSD1             (1<<7)          ///< priority of IDE OSD1
#define P1_IDE_OSD2             (1<<8)          ///< priority of IDE OSD2
#define P1_FD_CH0               (1<<9)          ///< priority of FD CH0
#define P1_FD_CH1               (1<<10)         ///< priority of FD CH1
#define P1_FD_CH2               (1<<11)         ///< priority of FD CH2
#define P1_DAI_PLY              (1<<12)         ///< priority of DAI play
#define P1_DAI_REC              (1<<13)         ///< priority of DAI record
#define P1_GRAPH0               (1<<14)         ///< priority of GRAPHIC0
#define P1_GRAPH1               (1<<15)         ///< priority of GRAPHIC1
#define P1_STGFIFO              (1<<16)         ///< priority of Storage FIFO
#define P1_SDIO                 (1<<17)         ///< priority of SDIO
#define P1_CF                   (1<<18)         ///< priority of CF
//@}

/**
    @name DRAM type encoding

    DRAM type encoding returned by dma_getDRAMType();
*/
//@{
#define  DDR_TYPE_DDR1          0   ///< DDR1 DRAM
#define  DDR_TYPE_DDR2          1   ///< DDR2 DRAM
//@}

extern void     dma_setPriority(UINT32 uiP0, UINT32 uiP1);
extern void     dma_getPriority(UINT32 *puiP0, UINT32 *puiP1);
extern ER       dma_setChannelPriority(UINT32 uiChannel, UINT32 uiPriority);
extern ER       dma_getChannelPriority(UINT32 uiChannel, UINT32* pPriority);

extern UINT8    dma_getDRAMType(void);
extern void     dma_setPrechargePowerDown(BOOL flag);

//#NT#2009/11/09#HH Chuang -begin
//#NT#support cache flush in DMA driver
extern void     dma_flushReadCache(UINT32 uiStartAddr, UINT32 uiLength);
extern void     dma_flushWriteCache(UINT32 uiStartAddr, UINT32 uiLength);
//#NT#2009/11/09#HH Chuang -end

//@}

#endif
