/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemClock.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of SystemClock.c
                Header file of SystemClock.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _SYSTEMCLOCK_H
#define _SYSTEMCLOCK_H

#include "Type.h"

#define SPEED_ID_PHOTO          0
#define SPEED_ID_CAPTURE        1
#define SPEED_ID_VIDEO          2
#define SPEED_ID_PLAYBACK       3
#define SPEED_ID_MP3            4
#define SPEED_ID_USBMSDC        5
#define SPEED_ID_USBSICD        6
#define SPEED_ID_USBPCC         7

//#NT#2009/09/30#HH Chuang -begin
extern void SysClk_Init(void);
//#NT#2009/09/30#HH Chuang -end
extern BOOL SysClk_SetClk(UINT32 uiSpeedID);

#endif

//@}
