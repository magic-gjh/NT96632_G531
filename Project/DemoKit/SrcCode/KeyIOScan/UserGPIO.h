/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       UserGPIO.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of UserGPIO.c

    @note       Nothing.

    @date       2007/02/02
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _USERGPIO_H
#define _USERGPIO_H

#include "SysCfg.h"

#if (_STORAGECARD_ == _STORAGECARD_MSPRO_)
extern void     MSCardUser_EnableCardPower(BOOL bOn);
extern BOOL     MSCardUser_CheckCardInserted(void);
extern void     MSCardUser_InitCardIO(void);
#endif

#if (_STORAGECARD_ == _STORAGECARD_XD_)
extern void     XDUser_EnableCardPower(BOOL bPowerOn);
#endif

#if (_STORAGECARD_ == _STORAGECARD_SDIO_ || _STORAGECARD_ == _STORAGECARD_SDIO2_)
extern    BOOL    SDIOCardUser_CheckCardInserted(void);
extern    BOOL    SDIOCardUser_CheckCardWP(void);
#endif

#if (_STORAGECARD_ == _STORAGECARD_SDIO2_)
extern    BOOL      SDIO2CardUser_CheckCardInserted(void);
extern    BOOL      SDIO2CardUser_CheckCardWP(void);
extern    void      SDIO2CardUser_InitCardIO(void);
#endif

#endif

//@}
