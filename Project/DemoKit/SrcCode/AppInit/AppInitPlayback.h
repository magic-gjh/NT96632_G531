/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitPlayback.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of AppInitPlayback.c
                Header file of AppInitPlayback.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _APPINITPLAYBACK_H
#define _APPINITPLAYBACK_H

#include "Type.h"

typedef enum
{
    APP_PLAYBACK_NORMAL,
    APP_PLAYBACK_WAV,
    APP_PLAYBACK_BR_JPG,
    APP_PLAYBACK_BR_VIDEO,
    APP_PLAYBACK_BR_WAV,
    APP_PLAYBACK_FLOW,
    APP_PLAYBACK_MAX
}AppInit_Playback_Mode;
extern void     AppInit_ModePlayback(void);
extern void AppInit_ModePlayback_Set_Mode(AppInit_Playback_Mode mode);
extern AppInit_Playback_Mode AppInit_ModePlayback_Get_Mode(void);
#endif

//@}

