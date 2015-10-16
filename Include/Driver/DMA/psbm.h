/**
    PSBM module driver.

    This file is the driver of PSBM module.

    @file       psbm.h
    @ingroup    mIPSBM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/

/**
    @addtogroup mIPSBM
*/
//@{

#ifndef _PSBM_H
#define _PSBM_H

/**
    @name   Status of psbm

    Definition for psbm status
*/
//@{

#define      STATUS_OK                      0       ///< Status success
#define      STATUS_ERR                    -1       ///< Status fail
#define      TOTAL_CMD                      4096    ///< Total command
#define      PSBM_SRH                       0       ///< Pattern search
#define      PSBM_SRH_REP                   1       ///< Pattern search and replacement
#define      PSBM_BM                        2       ///< Block move

#define      PSBM_SRH_FOUND                 0       ///< Pattern search found
#define      PSBM_SRH_ENDHIT                1       ///< Pattern search End Hit
#define      PSBM_SRH_FOUND_ENDHIT          2       ///< Pattern search found and End Hit
#define      PSBM_SRH_DONTCARE              3       ///< Do not care
//@}

/**
   @name Exported functions for psbm access.
*/
//@{
extern void   psbm_attach(void);
extern void   psbm_detach(void);
extern ER     psbm_open(UINT32* BMCmdAddr, UINT8 IntEn);
extern ER     psbm_close(void);
extern INT    psbm_addBMCmd(UINT32 *BMCmdAddr, UINT32 DstAddr, UINT32 SrcAddr, UINT32 Len);
extern void   psbm_BMstart(UINT32  *BMCmdAddr);
extern void   psbm_resetBM(UINT32  *BMCmdAddr);
extern void   psbm_setPSBMMode(UINT8 Mode);
extern void   psbm_waitBMfinsih(UINT32 *BMCmdAddr);
extern ER     psbm_waitINT(UINT32 *BMCmdAddr);
extern void   psbm_SWresetPSBM(void);

extern INT  psbm_startPS(UINT32 Mode,
             UINT32 BufAddr,
             UINT32 BufLen,
             UINT32 fResi,
             UINT32 Def[4],
             UINT32 Msk[4],
             UINT32 DefCfg,
             UINT32 Rep,
             UINT32 RepLen);

extern void psbm_getPatternSearchResult(UINT32 *pResult, UINT32 *pFoundAddr);
//@}

#endif

//@}