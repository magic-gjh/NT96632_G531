/**
    Copyright   Novatek Microelectronics Corp. 2010. All rights reserved.

    @file       IrRxParser.h
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

#ifndef _IR_RX_PARSER_H_
#define _IR_RX_PARSER_H_

#include "SysCfg.h"

#if (_PCB_ == _PCB_EVB_)
#include "PcbEVB/IrRxParserPcb.h"
#elif (_PCB_ == _PCB_WS_)
#include "PcbWS/IrRxParserPcb.h"
#elif (_PCB_ == _PCB_PP_)
#include "PcbPP/IrRxParserPcb.h"
#else
#include "PcbEVB/IrRxParserPcb.h"
#endif

//-----------------------------------------------------------------------------
// Definitions for APIs
//-----------------------------------------------------------------------------
// IR Code parsing definitions
#define IR_PARSE_KEY_ERR                0
#define IR_KEY_CODE_INVALID             0xFFFFFFFF

// IR Command/Data Code definitions
#define MAX_CMD_STR_LENGTH              16
#define MAX_CMD_ENTRY_NUM               32


typedef struct _IR_CODE_CMD
{
    UINT32          uiCmdCode;
    UINT32          uiMappedKey;
    CHAR            CmdString[MAX_CMD_STR_LENGTH];
} IR_CODE_CMD, *PIR_CODE_CMD;

typedef struct _IR_CODE_TABLE
{
    UINT32          uiCustomCode;
    IR_CODE_CMD     CmdCodeSet[MAX_CMD_ENTRY_NUM];
} IR_CODE_TABLE, *PIR_CODE_TABLE;




// Reference table pointer for parsing activity
extern PIR_CODE_TABLE g_pIrCodeTb;

// APIs
extern UINT32 IrRx_ParseKey(PIR_CODE_TABLE pIrCodeTable, UINT32 uiDataContent);
extern UINT32 IrRx_ValidatePatterns(UINT32 uiCustom, UINT32 uiDataContent);



#endif

//@}

