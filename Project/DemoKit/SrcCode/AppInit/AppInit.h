/**
    Header file of AppInit.c

    Header file of AppInit.c

    @file       AppInit.h
    @ingroup    mIPRJAPCfg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _APPINIT_H
#define _APPINIT_H

#include "Type.h"
#include "AppInitPhoto.h"
#include "AppInitSetup.h"
#include "AppInitMovie.h"
#include "AppInitMp3.h"
#include "AppInitPlayback.h"
#include "AppInitUSB.h"
#include "AppInitCustom.h"

/**
    @addtogroup mIPRJAPCfg
*/
//@{

extern void     AppInit_Open(UINT32 uiMode);
extern void     AppInit_Close(void);

//@}

#endif
