
#ifndef _PHOTODISTSK_H
#define _PHOTODISTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "appcommon.h"
#include "Alg_IPLMode.h"
#include "PhotoDisplayTsk.h"

//
//FLG_ID_PHOTODIS
//
#define FLGPHOTODIS_DISIDLE        FLGPTN_BIT(0)     // DIS task is idle
#define FLGPHOTODIS_UPDATEIDE      FLGPTN_BIT(1)     // update IDE para
#define FLGPHOTODIS_RESTART        FLGPTN_BIT(2)     // lock
#define FLGPHOTODIS_PAUSE          FLGPTN_BIT(3)     // unlock
//#NT#2010/04/12#Connie Yu  -begin
//#NT#Update DIS process
#define FLGPHOTODIS_MODESTART          FLGPTN_BIT(4)     // start: change resolution, start DIS process
#define FLGPHOTODIS_MODEIDLE           FLGPTN_BIT(5)     // stop: resume resolution
//#NT#2010/04/12#Connie Yu  -end

//
//#define PHOTO_DIS   FLGPHOTODIS_DISIDLE

//#define PHOTODISTSK_ENABLE   1

//#NT#2010/10/22#Connie Yu -begin
//#NT#movie DIS: start/idle without transition
typedef enum
{
    _InitDispInHSize,
    _InitDispInVSize,
    _InitDispOutHSize,
    _InitDispOutVSize,
    ENUM_DUMMY4WORD(DISSIZE_ENUM)
} DISSIZE_ENUM;

typedef enum
{
    PHOTODIS_MODE_START = 0,
    PHOTODIS_MODE_IDLE = 1,
    ENUM_DUMMY4WORD(PHOTODIS_MODE)
} PHOTODIS_MODE;

typedef enum
{
    LOCK_PRV = 0,
    UNLOCK_PRV = 1,
    LOCK_DISVIEW = 3,
    UNLOCK_DISVIEW = 4,
    ENUM_DUMMY4WORD(PHOTODIS_LOCKDISPLAY)
} PHOTODIS_LOCKDISPLAY;


typedef struct  _DisSizeInfor
{
    UINT32    InitDispInHSize;
    UINT32    InitDispInVSize;
    UINT32    InitDispOutHSize;
    UINT32    InitDispOutVSize;
} DisSizeInfor;

typedef struct  _DisLockInfor
{
    UINT32 hsz;
    UINT32 vsz;
    UINT32 lofs;
    IDE_BJMODE ideBJM;
    UINT32 y_addr;
    UINT32 cb_addr;
    UINT32 cr_addr;
} DisLockInfor;
//#NT#2010/10/22#Connie Yu -end

//#NT#2010/07/29#Connie Yu -begin
//#NT#modify IDE input/output size settings
typedef struct _PHOTODIS_APPOBJ{
    //UINT32  uiMemoryAddr;
    //UINT32  uiMemorySize;
    //void    (*CallBackFunc)(UINT64 MessageID, AlgMsgInfo * pAlgMsg_infor);
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
    UINT32  uiDisplayOutWidth;
    UINT32  uiDisplayOutHeight;
} PHOTODIS_APPOBJ,*PPHOTODIS_APPOBJ;
//#NT#2010/07/29#Connie Yu -end

typedef void (*FPPHOTODIS)(void);

//-----------------------------------------------------------------------
// function prototype description
//-----------------------------------------------------------------------
//#NT#2010/07/29#Connie Yu -begin
//#NT#modify IDE input/output size settings
extern ER      PhotoDis_Open(PPHOTODIS_APPOBJ pPhotoInfor);
//#NT#2010/07/29#Connie Yu -end
extern ER      PhotoDis_Close(void);
extern INT32   PhotoDis_registerDIS(FPPHOTODIS pPhotoDIS);
//extern void    PhotoDis_lockDIS(UINT8 lock);
extern BOOL    PhotoDis_isDisLock(void);
//#NT#2010/04/12#Connie Yu  -begin
//#NT#Update DIS process
extern BOOL    PhotoDis_waitIdle(BOOL bWaitInfinite);
//#NT#2010/04/12#Connie Yu -end
extern void    PhotoDis_disEvent(UINT32 uiEventStatus);
extern void    PhotoDis_ipeEvent(UINT32 uiEventStatus);
extern void    PhotoDis_disViewUpdate(DisplayIdeParam* IdeParam);
extern void    PhotoDis_displayComp_HalfDemo(DisplayIdeParam* IdeParam);
extern BOOL    PhotoDis_getFlagVideoBlend(void);
extern void    PhotoDis_setFlagVideoBlend(BOOL enable);
extern void    PhotoDis_setIdePara(DisplayIdeParam* IdePara);
//#NT#2010/10/22#Connie Yu -begin
//#NT#movie DIS: start/idle without transition
void PhotoDis_chgFBDisplay(void);
void PhotoDis_lockDisplay(PHOTODIS_LOCKDISPLAY mode);
void PhotoDis_lockOp(DisLockInfor* lockInfo);
//#NT#2010/10/22#Connie Yu -end
void PhotoDis_resumeFBDisplay(void);
UINT PhotoDis_SetSizeInfor(UINT item, UINT value);
UINT PhotoDis_GetSizeInfor(UINT item);
//extern void PhotoDis_setCurDisplayMode(DISPLAY_MODE mode);
extern void PhotoDis_ChgSize(void);
extern void PhotoDis_InitSize(DisSizeInfor* SizeInfor);
extern void PhotoDis_setMode(PHOTODIS_MODE Mode, BOOL bWaitInfinite);
#endif // _PHOTODISTSK_H
