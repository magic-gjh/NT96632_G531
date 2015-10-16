/**
    Crypt module driver.
 
    This file is the driver of Crypt module.
 
    @file       Crypt.h
    @ingroup    mICrypt
    @note       Nothing.
 
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

*/

#ifndef _CRYPTO_H
#define _CRYPTO_H

#include "Type.h"

/**
    @addtogroup mICrypt
*/
//@{

/**
    @name   Status of crypto

    Definition for crypto status
*/
//@{

#define CRYPT_OK              0         ///< Status success
#define CRYPT_WRONG_KEY_LEN   -1        ///< Key length error
#define CRYPT_WRONG_INPUT_LEN -2        ///< Input length error

//@}


/**
    crypto struct
*/
typedef struct _Crypt_Set
{
    UINT8  ucMode;              ///< Crypt engine mode
    UINT8  ucType;              ///< Crypt engine type
    UINT8  ucByteEndian;        ///< Word endian
    UINT8  ucBitOrder;          ///< Bit order
    UINT8  ucInput[16];         ///< Input test
    UINT8  ucKey[24];           ///< Key
    UINT8  ucOutput[16];        ///< Output test
}Crypt_Set, *pCrypt_Set;

/**
   @name Exported functions for crypto access.
*/
//@{

extern void     crypt_attach(void);
extern void     crypt_detach(void);
extern ER       crypt_open(void);
extern ER       crypt_close(void);
extern void     crypt_setCryptoMode(pCrypt_Set ptrCrypt);
extern void     crypt_setCryptoKey(pCrypt_Set ptrCrypt, INT Len);
extern void     crypt_setCryptoInput(pCrypt_Set ptrCrypt, INT Len);
extern void     crypt_getCryptoOutput(pCrypt_Set ptrCrypt, INT Len);

//@}

#endif

//@}
