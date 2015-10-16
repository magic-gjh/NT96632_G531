#include "UIFramework.h"
#include "UIGraphics.h"
#include "PlaybackTsk.h"
#include "UIBackgroundObj.h"
#include "NVTUserCommand.h"
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "PrimaryTsk.h"
#include "UIPlayObj.h"
#include "UIMenuSetupSettings.h"
#include "SysCfg.h"
#include "FileSysTsk.h"

#define NVT_PTR_RESPOSE_TIME            15  //15 seconds
#define NVT_PC_RESPOSE_TIME             10  //10 seconds

static UINT32 BackgroundDelAll(void);
static UINT32 BackgroundDelOne(void);
static UINT32 BackgroundCopy2Card(void);
static UINT32 BackgroundFormat(void);
static UINT32 BackgroundFormatCard(void);
static UINT32 BackgroundFormatNand(void);
static UINT32 BackgroundProtectAll(void);
static UINT32 BackgroundUnProtectAll(void);
static UINT32 BackgroundWaitPrintInit(void);
static UINT32 BackgroundPlayMedia(void);
static UINT32 BackgroundStopMedia(void);
static UINT32 BackgroundPauseMedia(void);

BACKGROUND_JOB_ENTRY gBackgroundExtFuncTable[] =
{
    {NVTEVT_BKW_DELALL,             BackgroundDelAll},
    {NVTEVT_BKW_DELONE,             BackgroundDelOne},
    {NVTEVT_BKW_COPY2CARD,          BackgroundCopy2Card},
    {NVTEVT_BKW_FORMAT,             BackgroundFormat},
    {NVTEVT_BKW_FORMATCARD,         BackgroundFormatCard},
    {NVTEVT_BKW_FORMATNAND,         BackgroundFormatNand},
    {NVTEVT_BKW_PROTECTALL,         BackgroundProtectAll},
    {NVTEVT_BKW_UNPROTECTALL,       BackgroundUnProtectAll},
    {NVTEVT_BKW_WAIT_PTR_INIT,      BackgroundWaitPrintInit},
    {NVTEVT_BKW_PLAYMOV,            BackgroundPlayMedia},
    {NVTEVT_BKW_STOPMOV,            BackgroundStopMedia},
    {NVTEVT_BKW_PAUSEMOV,           BackgroundPauseMedia},
    {0,                             0},
};

static UINT32 BackgroundDelAll(void)
{
    INT32 ret;

    PB_PlayDeleteMode(PB_DELETE_ALL);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundDelOne(void)
{
    INT32 ret;

    PB_PlayDeleteMode(PB_DELETE_ONE);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundCopy2Card(void)
{
#if 0
    INT32 ret;

    PB_SetCopy2Card(PB_COPY2CARD_NEW_DIR);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
#else
    INT32           ret;
    UINT32          uiDirId, uiFileId, uiFileType, uiTotalFileNum, uiCopyFileNum;
    COPYTO_INFO     CopyTo;
    char            *pBuf;

    //Save gResrInfo of file system
    uiDirId = PB_GetCurrFileSysInfo(PLAYFILEINFO_DIRID);
    uiFileId = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEID);
    uiFileType = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEFORMAT);
    FilesysSaveReservedInfo(uiDirId, uiFileId, uiFileType);

    //Change to NAND and get total file number
    Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);
    Primary_Wait4ChangeStorageDone();
    uiTotalFileNum = PB_ReadCurrFileSysInfo(PLAYFILEINFO_FILENUMS);

    //Do copy
    get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, pBuf);

    ret = FilesysCopyTo(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND),
                        Primary_GetStorageObj(PRIMARY_STGTYPE_CARD),
                        pBuf,
                        POOL_SIZE_CAPTURE,
                        FST_COPYTO_APPEND,
                        &uiCopyFileNum,
                        NULL,
                        FALSE);


    debug_err(("###chris: total file num=%d, copy file num=%d\r\n", uiTotalFileNum, uiCopyFileNum));

    //Change back to Card
    Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
    Primary_Wait4ChangeStorageDone();

    //Restore gResrInfo of file system
    FilesysRestoreReservedInfo();

    if(uiTotalFileNum == uiCopyFileNum)
        return UIBKW_STA_OK;
    else
        return UIBKW_STA_CARDFULL;
#endif
}

static UINT32 BackgroundFormat(void)
{
    int ret;
    PSTRG_TAB   pWhichObj;

    FilesysGetStrgObj(&pWhichObj);
    ret = FilesysFormat(pWhichObj, FALSE);

    return (UINT32)ret;
}

static UINT32 BackgroundFormatCard(void)
{
    int ret;

    if (Primary_GetCurrentMode() == PRIMARY_MODE_PLAYBACK)
    {
        PB_SetFormatStorage(PLAY_FORMAT_CARD);
        ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    else
    {
        ret = FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_CARD), FALSE);
    }

    if (Primary_GetStorage() == PRIMARY_STGTYPE_CARD)
    {
        Set_FilesysDirId(MIN_DCF_DIR_NUM);
        Set_FilesysFileId(MIN_DCF_FILE_NUM);
        FilesysSetWriteNextFileId(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM,FALSE);
    }

    return (UINT32)ret;
}

static UINT32 BackgroundFormatNand(void)
{
    int ret;

    if (Primary_GetCurrentMode() == PRIMARY_MODE_PLAYBACK)
    {
        PB_SetFormatStorage(PLAY_FORMAT_NAND);
        ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);
    }
    else
    {
        ret = FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), FALSE);
    }

    if (Primary_GetStorage() == PRIMARY_STGTYPE_NAND)
    {
        Set_FilesysDirId(MIN_DCF_DIR_NUM);
        Set_FilesysFileId(MIN_DCF_FILE_NUM);
        FilesysSetWriteNextFileId(MIN_DCF_DIR_NUM,MIN_DCF_FILE_NUM,FALSE);
    }

    return (UINT32)ret;
}

static UINT32 BackgroundProtectAll(void)
{
    INT32 ret;

    PB_SetProtectFile(PLAY_PROTECT_ALL);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundUnProtectAll(void)
{
    INT32 ret;

    PB_SetProtectFile(PLAY_UNPROTECT_ALL);
    ret = PB_WaitCommandFinish(PB_WAIT_INFINITE);

    return (UINT32)ret;
}

static UINT32 BackgroundWaitPrintInit(void)
{
#if 0   //#NewUx porting: Temporarily marked for compile error
    UINT    uiUSBFlag;
    UINT8 notFirst = 1;
    UINT32 notResponseTime =0;

    debug_msg("BackgroundWaitPrintInit\r\n ");
    clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL);
    clr_flg(FLG_DPS, FLGDPS_1SECOND|FLGDPS_FIRSTPTPCMD);

    while(notFirst)
    {
        wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_FIRSTPTPCMD|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);

        if ((uiUSBFlag & FLGDPS_FIRSTPTPCMD))
        {
            notFirst = 0;
        }
        else
        {
            notResponseTime++;
            if (notResponseTime > NVT_PTR_RESPOSE_TIME )
            {
                debug_err(("Wait FLGDPS_FIRSTPTPCMD failed!!!, notResponseTime > %d sec\r\n", NVT_PTR_RESPOSE_TIME));
                return FLGDPS_INITFAIL;
            }
        }
    }

    wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);
    notResponseTime = 0;

    while((notResponseTime < NVT_PC_RESPOSE_TIME) && (uiUSBFlag & FLGDPS_1SECOND))
    {
        wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_INITFAIL|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);
        notResponseTime ++;
    }

    if (notResponseTime == NVT_PC_RESPOSE_TIME)
    {
        debug_err(("Wait init failed!!!, notResponseTime > %d sec\r\n", NVT_PC_RESPOSE_TIME));
        return FLGDPS_INITFAIL;
    }

    if (uiUSBFlag & FLGDPS_INITFAIL)
    {
        notResponseTime= 0;
        clr_flg(FLG_DPS, FLGDPS_INITOK|FLGDPS_1SECOND);

        while (notResponseTime < NVT_PTR_RESPOSE_TIME)
        {
            wai_flg(&uiUSBFlag, FLG_DPS, FLGDPS_INITOK|FLGDPS_1SECOND, TWF_ORW|TWF_CLR);
            debug_err(("Init Fail, wait flag again:uiUSBFlag=%x, notResponseTime=%x\r\n", uiUSBFlag, notResponseTime));
            if (uiUSBFlag & FLGDPS_INITOK)
            {
                break;
            }
            else
            {
                notResponseTime++;
            }
        }

        if (notResponseTime == NVT_PTR_RESPOSE_TIME)
        {
            return FLGDPS_INITFAIL;
        }
    }

    if (uiUSBFlag & FLGDPS_INITOK)
    {
        debug_msg("Wait PrintInit ok !!!\r\n ");
        return FLGDPS_INITOK;

    }
    else
    {
        return FLGDPS_INITFAIL;
    }
#else
    return 0;
#endif
}

UINT32 BackgroundPlayMedia(void)
{
    //FlowPB_PlayMedia();
    return 0;
}
UINT32 BackgroundStopMedia(void)
{
    //FlowPB_StopMeida();
    return 0;
}
UINT32 BackgroundPauseMedia(void)
{
    //FlowPB_PauseMeida();
    return 0;
}

