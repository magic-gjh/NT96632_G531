/**
    Header file for embedded audio module

    This file is the header file that define the API and data type
    for embedded audio module


    @file       EAC.h
    @ingroup    mIAudEAC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/



#ifndef _EAC_H
#define _EAC_H

#include "Type.h"

/** \addtogroup mIAudEAC */
//@{

/**
     EAC output path
*/
typedef enum
{
    EAC_OUTPUT_SPK,           ///< Speaker
    EAC_OUTPUT_LINE,          ///< Line Out
    EAC_OUTPUT_HP,            ///< Head phone
    ENUM_DUMMY4WORD(EAC_OUTPUT)
} EAC_OUTPUT;

/**
     EAC Output Mixer type
*/
typedef enum
{
    EAC_MIXER_NORMAL,           ///< Normal Mixer : R=R, L=L
    EAC_MIXER_RLAVG,            ///< Average Mixer : R=(R+L)/2, L=(R+L)/2
    ENUM_DUMMY4WORD(EAC_MIXER)
} EAC_MIXER;


// API
extern void eac_init(void);
extern BOOL eac_isDACEnable(void);
extern void eac_setAnalogVolume(EAC_OUTPUT EacOutput, UINT32 uiVol);
extern void eac_setDigitalVolume(UINT32 uiVol);
extern void eac_setDigitalGain(UINT32 uiGain);
extern void eac_setAnalogGain(UINT32 uiGain);
extern void eac_setALC_MaxGain(UINT32 uiMaxgain);
extern void eac_setALC_Target(UINT32 uiTarget);
extern void eac_setALC_NoiseThd(UINT32 uiNoiseThd);
extern void eac_setALC(BOOL bAlcEn);
extern void eac_setNG(BOOL NoiseGate_en);
extern void eac_setBoostA(BOOL bBoostA_en);
extern void eac_setBoostD(BOOL bBoostD_en);
extern void eac_setDACPwr(BOOL bEn);
extern void eac_setDACOutput(EAC_OUTPUT EacOutput, BOOL bEn);
extern void eac_setDACReset(BOOL bEn);
extern void eac_setADCPwr(BOOL bEn);
extern void eac_setADCReset(BOOL bEn);
extern BOOL eac_sendCommand(UINT32 uiReg, UINT32 uiData);
extern BOOL eac_readData(UINT32 uiReg, UINT32 *puiData);
extern BOOL eac_isALCEnable(void);
extern void eac_setDACMixer(EAC_MIXER EacMixType);
//#NT#2010/02/08#Cliff Lin -begin
//#NT#Added New API
extern BOOL eac_chkDACOutputEn(EAC_OUTPUT EacOutput);
//#NT#2010/02/08#Cliff Lin -begin



//@}
#endif



