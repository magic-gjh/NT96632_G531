/**
    @file       FDMA.h
    @ingroup    mIHALDMA

    @brief      Header file for FDMA module.

                This file is the header file that define the API for FDMA module.

    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

/**
    @addtogroup mIHALDMA
*/
//@{

#ifndef _FDMA_H
#define _FDMA_H

#include "Type.h"

/**
    FDMA channel ID
*/
typedef enum
{
    FDMA_CH_A,          ///< FDMA channel A
    FDMA_CH_B,          ///< FDMA channel B
//#NT#2008/05/20#Stephen Wang -begin
    FDMA_CH_C,          ///< FDMA channel C
//#NT#2008/05/20#Stephen Wang -end
    NUM_FDMA            ///< FDMA total channel
} FDMACH;

/**
    @name   FDMA status

    Definition for FDMA status

    These are return values for fdma_wait()
*/
//@{
#define FDMA_INT_OK                 0x55    ///< FDMA finished successfully
#define FDMA_INT_ERR                0xEE    ///< FDMA ended with error
//@}

/**
    @name   FDMA Configuration Status Register bits

    Bit mapping of main configuration status register(CSR)

    @note These setting only available in NT96611/610 and is for driver internal use
*/
//@{
#define DMA_CSR_M1_BIGENDIAN        0x00000004
#define DMA_CSR_M1_LITTLEENDIAN     0x00000000
#define DMA_CSR_M0_BIGENDIAN        0x00000002
#define DMA_CSR_M0_LITTLEENDIAN     0x00000000
#define DMA_CSR_DMACEN              0x00000001
//@}

/**
    @name   FDMA Synchronization Register bits

    Bit mapping of synchronization register

    @note These setting only available in NT96611/610 and is for driver internal use
*/
//@{
#define DMA_SYNC_ENABLE             0x00000001
#define DMA_SYNC_DISABLE            0x00000000
//@}

/**
    @name   FDMA Channel Control Register bits

    Bit mapping of channel control register

    The combination of these settings are used as uiCommand parameter
    of fdma_channelrequest().
*/
//@{
#define DMA_CSR_TC_MSK              0x80000000
#define DMA_CSR_CHPRJ_HIGHEST       0x00C00000
#define DMA_CSR_CHPRJ_2ND           0x00800000
#define DMA_CSR_CHPRJ_3RD           0x00400000
#define DMA_CSR_PRTO3               0x00200000
#define DMA_CSR_PRTO2               0x00100000
#define DMA_CSR_PRTO1               0x00080000
#define DMA_CSR_PRTLow              0x00000000
#define DMA_CSR_SRC_BURST_SIZE_1    0x00000000
#define DMA_CSR_SRC_BURST_SIZE_4    0x00010000
#define DMA_CSR_SRC_BURST_SIZE_8    0x00020000
#define DMA_CSR_SRC_BURST_SIZE_16   0x00030000
#define DMA_CSR_SRC_BURST_SIZE_32   0x00040000
#define DMA_CSR_SRC_BURST_SIZE_64   0x00050000
#define DMA_CSR_SRC_BURST_SIZE_128  0x00060000
#define DMA_CSR_SRC_BURST_SIZE_256  0x00070000

#define DMA_CSR_ABT                 0x00008000
#define DMA_CSR_SRC_WIDTH_8         0x00000000
#define DMA_CSR_SRC_WIDTH_16        0x00000800
#define DMA_CSR_SRC_WIDTH_32        0x00001000

#define DMA_CSR_DST_WIDTH_8         0x00000000
#define DMA_CSR_DST_WIDTH_16        0x00000100
#define DMA_CSR_DST_WIDTH_32        0x00000200

#define DMA_CSR_MODE_NORMAL         0x00000000
#define DMA_CSR_MODE_HANDSHAKE      0x00000080

#define DMA_CSR_SRC_INCREMENT       0x00000000
#define DMA_CSR_SRC_DECREMENT       0x00000020
#define DMA_CSR_SRC_FIX             0x00000040

#define DMA_CSR_DST_INCREMENT       0x00000000
#define DMA_CSR_DST_DECREMENT       0x00000008
#define DMA_CSR_DST_FIX             0x00000010

#define DMA_CSR_SRCAD_CTL           0x00000060
#define DMA_CSR_DSTAD_CTL           0x00000018

#define DMA_CSR_SRC_SEL             0x00000004
#define DMA_CSR_DST_SEL             0x00000002

#define DMA_CHANNEL_ENABLE          0x00000001
#define DMA_CHANNEL_DISABLE         0x00000000

#define DMA_CSR_CHPR1               0x00C00000
#define DMA_CSR_SRC_SIZE            0x00070000
#define DMA_CSR_SRC_WIDTH           0x00003800
#define DMA_CSR_DST_WIDTH           0x00000700
//@}

/**
    @name   FDMA Channel Configuration Register bits

    Bit mapping of channel configuration register

    @note These setting only available in NT96611/610 and is for driver internal use
*/
//@{
#define DMA_CFG_INT_TC_MSK_Enable   0x00000001
#define DMA_CFG_INT_TC_MSK_Disable  0x00000000
#define DMA_CFG_INT_ERR_MSK_Enable  0x00000002
#define DMA_CFG_INT_ERR_MSK_Disable 0x00000000
//@}

/**
    @name   FDMA Linked List Control Descriptor registers

    Bit mapping of Linked List Control Descriptor

    @note These setting only available in NT96611/610 and is for driver internal use
*/
//@{
#define DMA_LLP_TC_MSK              0x10000000
#define DMA_LLP_SRC_WIDTH_8         0x00000000
#define DMA_LLP_SRC_WIDTH_16        0x02000000
#define DMA_LLP_SRC_WIDTH_32        0x04000000
#define DMA_LLP_DST_WIDTH_8         0x00000000
#define DMA_LLP_DST_WIDTH_16        0x00400000
#define DMA_LLP_DST_WIDTH_32        0x00800000
#define DMA_LLP_SRC_INCREMENT       0x00000000
#define DMA_LLP_SRC_DECREMENT       0x00100000
#define DMA_LLP_SRC_FIX             0x00200000
#define DMA_LLP_DST_INCREMENT       0x00000000
#define DMA_LLP_DST_DECREMENT       0x00040000
#define DMA_LLP_DST_FIX             0x00080000
#define DMA_LLP_SRC_SEL             0x00020000
#define DMA_LLP_DST_SEL             0x00010000
//@}

//---------------------------------------------------------------------------
extern UINT32 fdma_open(FDMACH uiDMAId);
extern UINT32 fdma_close(FDMACH uiDMAId);
extern UINT32 fdma_channelrequest(FDMACH uiDMAId, UINT32 SrcAddr, UINT32 DstAddr, UINT32 BlockSize, UINT32 uiCommand);
extern UINT32 fdma_wait(FDMACH uiDMAId);
extern void   fdma_stop(FDMACH uiDMAId);
//#NT#2007/12/17#HH Chuang -begin
//#NT#fix SD card remove problem
extern UINT32 fdma_checkenable(FDMACH uiDMAId);
//#NT#2008/01/14#HH Chuang -begin
// modify DMA error handler to support synchronous call
extern void fmda_errorhandler(FDMACH uiDMAId, BOOL bSynchronous);
//extern void fmda_errorhandler(FDMACH uiDMAId, UINT32 SrcAddr, UINT32 DstAddr, UINT32 TransferBytes);
//#NT#2008/01/14#HH Chuang -end
//#NT#2007/12/17#HH Chuang -end

#endif

//@}
