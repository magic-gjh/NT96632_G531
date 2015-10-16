/**
    @file       cfio.h
    @ingroup    mICardCFIO

    @brief      Header file for the CFIO module APIs.

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef _CFIO_H
#define _CFIO_H

#include "Type.h"

/**
    @addtogroup mICardCFIO
@{
*/

typedef void (*CFIO_INTCB)(void);
typedef void (*CFIO_BUSSWITCH_CB)(UINT32);

/**
    Information needed for the CFIO module.
*/
typedef struct
{
    UINT32 DMABurstSize;    ///< Set the size of one DMA burst in words.
    BOOL   DMAEnable;       ///< Decide whether the DMA function is enabled.
    BOOL   CFIOIntEnable;   ///< Decide whether the interrupt function of the CF_RDY pin is used.
    CFIO_INTCB CFIOIntCB;   ///< If the CF_RDY pin triggers a rising edge, then this CB function will be executed.
    CFIO_BUSSWITCH_CB CFIOBusSwitchCB;  ///< Bus switch CB function to switch bus interface between "CFIO_SWITCH_TO_CFIO" & "CFIO_SWITCH_TO_STORAGE"
} CFIOStruct,*pCFIOStruct;

/*
    Defined values below are used for the parameter "TargetPortSize" in the functions
    cfio_readData() & cfio_writeData() :
*/
#define CFIO_TARGET_PORT_SIZE8BITS     0x1  // The target port is a 16-bits register.
#define CFIO_TARGET_PORT_SIZE16BITS    0x2  // The target port is a 8-bits register.

/*
    Defined values below are used for the parameter "Addr_Inc" in the functions
    cfio_readData() & cfio_writeData() :
*/
#define CFIO_ADDRESS_INCREMENT         0x1  // The target port address will be kept the same during the whole transfer.
#define CFIO_ADDRESS_NO_INCREMENT      0x2  // The target port address will be increased by 1 or 2 after each port access.

/*
    Defined values below are used for the parameter "DMABurstSzie" in the functions cfio_setDMABurstSize()
    and the member "DMABurstSize" of the structure "CFIOStruct",
*/
#define CFIO_DMA_BURST_SIZE_1WORD      0x1  // 8 words = 32 bytes
#define CFIO_DMA_BURST_SIZE_4WORD      0x2  // 4 words = 16 bytes
#define CFIO_DMA_BURST_SIZE_8WORD      0x3  // 1 word  = 4 bytes

/*
    Usage :
        void XXX_Bus_SwitchCB(UINT32 SwitchTo)
        {
            switch(SwitchTo)
            {
                case CFIO_SWITCH_TO_CFIO:
                    // Function to switch bus to CFIO interface
                    break;

                case CFIO_SWITCH_TO_STORAGE:
                    // Function to switch bus to normal storage
                    break;

                default:
                    break;
            }
        }
*/
#define CFIO_SWITCH_TO_CFIO            0x1
#define CFIO_SWITCH_TO_STORAGE         0x2

extern ER cfio_open(pCFIOStruct pCFIOinfo);
extern ER cfio_close(void);
extern ER cfio_HWreset(UINT ResetNs);
extern UINT8 cfio_inb(UINT32 Addr);
extern void cfio_outb( UINT32 Addr, UINT8 Value);
extern UINT16 cfio_inw(UINT32 Addr);
extern void cfio_outw( UINT32 Addr, UINT16 Value);
extern ER cfio_readData(UINT8 *pBuf, UINT32 Addr, UINT32 Length, UINT32 TargetPortSize, UINT32 Addr_Inc);
extern ER cfio_writeData(UINT8 *pBuf, UINT32 Addr, UINT32 Length, UINT32 TargetPortSize, UINT32 Addr_Inc);
extern void cfio_eanbleDMATransfer(BOOL ifEnable);
extern void cfio_setDMABurstSize(UINT32 DMABurstSzie);
extern UINT8 cfio_getIntPinStatus(void);

//@}

#endif //_CFIO_H
