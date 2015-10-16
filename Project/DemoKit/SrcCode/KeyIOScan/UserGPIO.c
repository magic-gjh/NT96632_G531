/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UserGPIO.c
    @ingroup    mIPRJAPKeyIO

    @brief      GPIO mapping for low level library.
                This file does GPIO initialization and GPIO detection for low level library

    @note       These functions prototype are defined by respective low level library

    @version    V1.00.000
    @author     Stephen Wang
    @date       2006/11/06
*/

#include "SysCfg.h"
#include "GPIOMapping.h"
#include "UserGPIO.h"

/**
    @addtogroup mIPRJAPKeyIO
*/
//@{

#if (_STORAGECARD_ == _STORAGECARD_MSPRO_)

#include "MSPro.h"

/**
    Enable card power.

    @param bOn TRUE=turn on card power, FALSE=turn off card power
*/
void MSCardUser_EnableCardPower(BOOL bOn)
{
    if (bOn)
        gpio_setPin(DGPIO_CARD_POWER);
    else
        gpio_clearPin(DGPIO_CARD_POWER);
}

/**
    Check card insertion state.

    @return TRUE=card inserted, FALSE=card removed
*/
BOOL MSCardUser_CheckCardInserted(void)
{
    return GPIOMap_DetStrgCard();
}

/**
    Initialize card I/O pad.
*/
void MSCardUser_InitCardIO(void)
{
    /* Initialize DGPIO9 to be output direction */
    gpio_setDir(DGPIO_CARD_POWER, GPIO_DIR_OUTPUT);
    pad_setDrivingSink(PAD_DS_CARD, PAD_DRIVINGSINK_6MA);
}
#endif

#if (_STORAGECARD_ == _STORAGECARD_XD_)
/**
    Enable card power.    
    
    @param bPowerOn TRUE for turn on the card power, FALSE for turn off the  card power
*/

void XDUser_EnableCardPower(BOOL bPowerOn)
{
    gpio_setDir(DGPIO_CARD_POWER, GPIO_DIR_OUTPUT);
    
    if(bPowerOn)
        gpio_setPin(DGPIO_CARD_POWER);
    else
        gpio_clearPin(DGPIO_CARD_POWER);
       
}

#endif

#if (_STORAGECARD_ == _STORAGECARD_SDIO_ || _STORAGECARD_ == _STORAGECARD_SDIO2_)
BOOL SDIOCardUser_CheckCardInserted(void)
{
    return GPIOMap_DetStrgCard();
}

BOOL SDIOCardUser_CheckCardWP(void)
{
    return GPIOMap_DetStrgCardWP();
}
#endif

#if(_STORAGECARD_ == _STORAGECARD_SDIO2_)
BOOL SDIO2CardUser_CheckCardInserted(void)
{
    return GPIOMap_DetSDIO2Card();
}

BOOL SDIO2CardUser_CheckCardWP(void)
{
    return GPIOMap_DetSDIO2CardWP();
}

void SDIO2CardUser_InitCardIO(void)
{
    pad_setPullUpDown(PAD_SDIO2, PAD_PULLUP);    
    // need 6mA to drive some SD/MMC card
    pad_setDrivingSink(PAD_DS_SDIO2, PAD_DRIVINGSINK_6MA);
}
#endif

//@}
