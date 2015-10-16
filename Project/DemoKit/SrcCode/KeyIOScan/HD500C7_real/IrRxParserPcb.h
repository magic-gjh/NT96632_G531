/**
    Copyright   Novatek Microelectronics Corp. 2010. All rights reserved.

    @file       IrRxParserPcb.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of IrRx received data parser module
                This file is the header file of IrRx received data parser module

    @note       Nothing.

    @version    V1.00.000
    @author     JustinLee
    @date       2010/03/31
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _IR_RX_PARSER_PCB_H_
#define _IR_RX_PARSER_PCB_H_

#include "SysCfg.h"


// IR Address/Custom Code definitions
#define IR_CODE_CUSTOM_NEC01            0x86
#define IR_CODE_CUSTOM_NEC02            0x00

// NEC01
#define NEC01_CMD_UP                    0x0B
#define NEC01_CMD_DOWN                  0x0A
#define NEC01_CMD_LEFT                  0x05
#define NEC01_CMD_RIGHT                 0x15
#define NEC01_CMD_ENTER                 0x09
#define NEC01_CMD_ZOOMIN                0x1F
#define NEC01_CMD_ZOOMOUT               0x17
#define NEC01_CMD_MENU                  0x16
#define NEC01_CMD_MODE                  0x14
#define NEC01_CMD_PLAYBACK              0x11

// NEC02
#define NEC02_CMD_UP                    0x1F
#define NEC02_CMD_DOWN                  0x54
#define NEC02_CMD_LEFT                  0x5D
#define NEC02_CMD_RIGHT                 0x59
#define NEC02_CMD_ENTER                 0x5C
#define NEC02_CMD_ZOOMIN                0x02
#define NEC02_CMD_ZOOMOUT               0x46
#define NEC02_CMD_MENU                  0x14
#define NEC02_CMD_MODE                  0x10
#define NEC02_CMD_PLAYBACK              0x1C



#endif

//@}

