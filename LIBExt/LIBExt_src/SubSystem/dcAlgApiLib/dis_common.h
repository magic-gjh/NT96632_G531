/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2009.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Connie Yu                                                         *
****************************************************************************/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_COMMON_H_
#define _DIS_COMMON_H_

#include "dis_lib.h"
#include "ImageAlgInfor.h"
#include "JpgEnc.h"

//#NT#2010/10/27#Connie Yu -begin
//#NT#independent DIS spare buffer
#define MDIS_PRV_MAX_SIZE   640 * 576 * 2 * 2
//#NT#2010/10/27#Connie Yu -end

typedef struct
{
    UINT32  Hsz;
    UINT32  Vsz;
} DIS_SIZE;


//#NT#2010/09/03#Connie Yu -begin
//#NT#update for movie DIS
//extern void dis_DISprocess(void);
//extern BOOL dis_setSmoothCentering(void);
void dis_lockDis4Cap(UINT8 lock);
ER dis_configDisHw(void);
extern void dis_updateDisplaySize(void);
extern void dis_updateRecordSize(DIS_SIZE* size);
extern void dis_updateJpegLofs(PJPGCmpParam JpgParam);
extern void dis_processOpen(void);
extern void dis_processClose(void);
extern void dis_processLock(BOOL lock);
//extern void dis_setIPPMode(DIS_MODE mode);
extern UINT32 Get_DISPrvMaxBuffer(void);
extern void dis_accumUpdateProcess(void);
extern void dis_accumRecProcess(void);
extern MOTION_VECTOR dis_getIdeCorrVec(void);
extern UINT16 dis_getDisViewRatio(void);
extern void dis_enableIPEEdgeOut(UINT8 enable);
extern BOOL dis_getFlagVideoBlend(void);
extern void dis_setFlagVideoBlend(BOOL enable);
//extern void dis_getRecFbInfo(ACTFBADDR_PRAM *pDisRecFbInfo);
extern void dis_resetCorrVec(void);
//extern void dis_Need2UpdateDisplaySz(BOOL update);
extern void dis_updateRecAddr(ACTFBADDR_PRAM *pDisRecFbInfo);
extern void dis_eventStart(void);
extern void dis_eventPause(void);
extern DIS_PROC_EVENT dis_getProcEvent(void);
extern UINT8 dis_getImeBufferID(void);
extern void dis_setFlagFreezeFB(BOOL bFreeze);
extern UINT32 dis_chgSizeByDisViewRatio(UINT32 size);
//#NT#2010/04/12#Connie Yu  -end
extern BOOL dis_checkBusy(void);
extern void dis_init(void);
ER dis_chgDisSizeConfig(void);
void dis_setLockMatch(void);
//#NT#2010/09/03#Connie Yu -end

#endif

