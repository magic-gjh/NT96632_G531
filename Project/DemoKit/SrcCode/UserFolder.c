
/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       UserFolder.h
    @ingroup    mIPRJAPCfg

    @brief      User defined DCF Folder/File name functions
                User defined DCF Folder/File name functions

    @note       Nothing.

    @date       2011/01/24
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "UserFolder.h"


static BOOL USERFolder_GetFreeFolderID(UINT16 *uhFolderID,UINT16 huFileID)
{
 UINT16    lastFolderID,uhCurFolderID;
 char      pcurPathName[6],pDateName[6];
 RTC_DATE  curDate;

    curDate = rtc_getDate();
    sprintf(pDateName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);
    uhCurFolderID = lastFolderID = *uhFolderID;
    // check free DCF folder ID, from 100/101/....to 999 maybe
    do {
        if (FilesysGetDCFFolderName(pcurPathName,uhCurFolderID) == FST_STA_OK)
        {
            UserFolder_Dgb(("%s, used uhFolderID=%d,%s\r\n",__func__,uhCurFolderID,pcurPathName));
            if (!strncmp(pDateName,pcurPathName,5))
            { // use current Folder ID as next write DCF folder ID
                if (huFileID < MAX_DCF_FILE_NUM)
                {
                    *uhFolderID = uhCurFolderID;
                    return TRUE;
                }
            }

            uhCurFolderID++;
            if (lastFolderID==uhCurFolderID)
            {
                debug_msg("No Free DCF ID\r\n");
                return FALSE;
            }
            // searching avaiable DCF ID from MIN_DCF_DIR_NUM
            if (uhCurFolderID >= MAX_DCF_DIR_NUM)
                uhCurFolderID = MIN_DCF_DIR_NUM;
        } else {
            // Get free DCF Folder ID and return uhFolderID
            *uhFolderID = uhCurFolderID;
            return TRUE;
        }
    } while (1);

}


static void UserFolder_SetFreeFolderName(UINT16 uhFreeFolderId,UINT16 uhFileId)
{
 UINT16    uhCurFolderId,uhCurFileId;
 RTC_DATE  curDate;
 char      pPathName[6];


    FilesysGetDCFNextID(&uhCurFolderId,&uhCurFileId);

    if (uhFreeFolderId != uhCurFolderId)
    {
        curDate = rtc_getDate();
        sprintf(pPathName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);

        FilesysWaitInitFinish(FST_TIME_INFINITE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysSetDCFPath((UB *)pPathName, TRUE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysSetWriteNextFileId(uhFreeFolderId,uhFileId,FALSE);
    }

}

static BOOL USERFolder_FolderCreate(void)
{
 UINT16    uhFolderId, uhFileId;
 BOOL      bzFolderCreate =TRUE;


    // Get Free DCF Folder ID
    FilesysGetDCFNextID(&uhFolderId,&uhFileId);
    bzFolderCreate = USERFolder_GetFreeFolderID(&uhFolderId,uhFileId);
    if (bzFolderCreate == FALSE)
    {
        return bzFolderCreate; // no free DCF Folder ID
    }

    UserFolder_Dgb(("%s,free FolderID:%d\r\n",__func__,uhFolderId));
    // Set Free DCF folder name
    UserFolder_SetFreeFolderName(uhFolderId,uhFileId);

    return bzFolderCreate;
}

static void USERFolder_FileCreate(void)
{
 RTC_TIME  curTime;
 char      pFileName[6];

    curTime = rtc_getTime();
    sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
    FilesysSetDCFFilename((INT8 *)pFileName, FST_FMT_ANYFORMAT);
}

void USERFolder_set(void)
{

    // for Folder create issue
    // 1. rename 999xxxxx to 100xxxxx
    if (USERFolder_FolderCreate()==FALSE)
    { // No free DCF Folder ID
        return ;
    }

    // 2. from 23:59 doday to 00:00 tomorrow
    USERFolder_FileCreate();
}


//@}
