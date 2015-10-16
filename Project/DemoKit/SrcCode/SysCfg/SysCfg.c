/*
    Config the Task System Configuration

                Config the Task System Configuration.

    @file       SysCfg.c
    @ingroup
    @note       Nothing.

   Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"

//===========================================================================
// User defined task table
//===========================================================================
extern void KeyScanTsk(void);
extern void CommandTsk(void);
extern void PlaySoundTsk(void);
extern void UIFrameworkTsk(void);
extern void UIBackgroundTsk(void);

//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
extern void MsdcVendorNvtTsk(void);
//#NT#2009/09/09#Niven Cho -end

//===========================================================================
// User task stack
//===========================================================================
__align(4) UCHAR pucStkKeyScan[STKSIZE_KEYSCAN];
__align(4) UCHAR pucStkCommand[STKSIZE_COMMAND];
__align(4) UCHAR pucStkPlaySound[STKSIZE_PLAYSOUND];
__align(4) UCHAR pucStkUIFramework[STKSIZE_UIFRAMEWORK];
__align(4) UCHAR pucStkUIBackgound[STKSIZE_UIBACKGROUND];
//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
#if(_MSDCVENDOR_ == _MSDCVENDOR_NVT_)
__align(4) UCHAR pucStkMsdcVendorNvtTsk[STKSIZE_MSDC_DBGSYS];
#endif
//#NT#2009/09/09#Niven Cho -end

//===========================================================================
// User defined task initital table
// ID, Function pointer, Priority, Stack Size, Stack Address
//===========================================================================
__align(4) T_CTSK user_systasktabl[USER_TSK_NUM] =
{
    // Task ID,             Priority,           Stack size,             Func,               Stack
    {KEYSCANTSK_ID,         PRI_KEYSCAN,        STKSIZE_KEYSCAN,        KeyScanTsk,         pucStkKeyScan},
    {COMMANDTSK_ID,         PRI_COMMAND,        STKSIZE_COMMAND,        CommandTsk,         pucStkCommand},
    {PLAYSOUNDTSK_ID,       PRI_PLAYSOUND,      STKSIZE_PLAYSOUND,      PlaySoundTsk,       pucStkPlaySound},
    {UIFRAMEWORKTSK_ID,     PRI_UIFRAMEWORK,    STKSIZE_UIFRAMEWORK,    UIFrameworkTsk,     pucStkUIFramework},
    {UIBACKGROUNDTSK_ID,    PRI_BACKWORKER,     STKSIZE_UIBACKGROUND,   UIBackgroundTsk,    pucStkUIBackgound},
    //#NT#2009/09/09#Niven Cho -begin
    //#NT#Added., for USB Debug System
    #if(_MSDCVENDOR_ == _MSDCVENDOR_NVT_)
    {MSDC_DBGSYS_TSK_ID,    PRI_MSDC_DBGSYS,    STKSIZE_MSDC_DBGSYS,    MsdcVendorNvtTsk,   pucStkMsdcVendorNvtTsk},
    #else
    {MSDC_DBGSYS_TSK_ID,    PRI_MSDC_DBGSYS,    STKSIZE_MSDC_DBGSYS,    DummyTsk,           pucStkDummy},
    #endif
    //#NT#2009/09/09#Niven Cho -end
};

//=====================================================================
//  User defined event flag table
//=====================================================================
__align(4) SYSTEM_EVF_TABLE user_sysevftabl[USER_EVF_NUM] =
{
    {FLG_ID_KEY},
    {FLG_ID_KEY_REL},
    {FLG_ID_KEY_CONT},
    {FLG_ID_KEY_LONG},
    {FLG_ID_SOUND},
    {FLG_ID_FRAMEWORK},
};

//=====================================================================
//  User defined semaphore table
//=====================================================================
__align(4) T_CSEM user_syssemtabl[USER_SEM_NUM] =
{
    {SEMID_FRAMEWORK_WINDOW,  128, 1, 1},
    {SEMID_FRAMEWORK_DRAW,    128, 1, 1},
    {SEMID_UIGRAPHIC,         128, 1, 1},
    //#NT#2009/09/09#Niven Cho -begin
    //#NT#Added., New USB Driver Support
    {SEMID_MSDC_DBGYSYS,        128,        1,          1},
    //#NT#2009/09/09#Niven Cho -end
};

//=====================================================================
//  User defined mailbox
//=====================================================================
__align(4) SYSTEM_MBX_TABLE user_sysmbxtabl[USER_MBX_NUM] =
{
    {MBXID_SYS_EVENT},
    {MBXID_BKG_EVENT},
};

//=====================================================================
//  Fixed memory pool
//=====================================================================
// The address of the pool will be always start at 32 bytes boundary
__align(4) T_CMPF user_sysfpltabl[USER_FPL_NUM] =
{
    // Pool ID,             Pool size,              Pool Counter
    {POOL_ID_DEFECT_PIXEL,  POOL_SIZE_DEFECT_PIXEL, POOL_CNT_DEFECT_PIXEL}, // defect pixel buffer
    {POOL_ID_OSD1,          POOL_SIZE_OSD,          POOL_CNT_OSD1},         // OSD1 buffer
    {POOL_ID_OSDTEMP,       POOL_SIZE_OSD,          POOL_CNT_OSDTEMP},      // OSD temp buffer
    {POOL_ID_OSD2,          POOL_SIZE_OSD,          POOL_CNT_OSD2},         // OSD2 buffer
    {POOL_ID_GX,            POOL_SIZE_GX,           POOL_CNT_GX},           // Frame buffer
    {POOL_ID_BG,            POOL_SIZE_BG,           POOL_CNT_BG},           // jpg background buffer
    {POOL_ID_BGTEMP,        POOL_SIZE_BGTEMP,       POOL_CNT_BGTEMP},       // jpg background buffer
    {POOL_ID_FTABLE,        POOL_SIZE_FTABLE,       POOL_CNT_FTABLE},       // MP3 file table buffer
    {POOL_ID_OSD_DUMP,      POOL_SIZE_OSD_DUMP,     POOL_CNT_OSD_DUMP},     // OSD dump buffer
    {POOL_ID_IQ,            POOL_SIZE_IQ,           POOL_CNT_IQ},           // IQ buffer
    {POOL_ID_CAPTURE,       POOL_SIZE_CAPTURE,      POOL_CNT_CAPTURE}       // Capture buffer
};
