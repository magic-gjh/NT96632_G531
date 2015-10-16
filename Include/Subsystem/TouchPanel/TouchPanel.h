/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       TouchPanel.h
    @ingroup
    @brief      Header file for touch panel controller

    @note       Nothing.

*/

/**
    @addtogroup
*/
//@{

#ifndef TOUCHPANEL_H
#define TOUCHPANEL_H
//-------------------------------------------------------------------------------------------------
typedef struct
{
    UINT32  Cs;
    UINT32  Clk;
    UINT32  Out;
    UINT32  In;
}TP_INIT_STRUCT;
//-------------------------------------------------------------------------------------------------
extern BOOL tp_getPos(
    UINT32*     pPosX,
    UINT32*     pPosY);
extern void tp_init(TP_INIT_STRUCT* pGpio);
//-------------------------------------------------------------------------------------------------
#endif

//@}