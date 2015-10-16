

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SysCfg.h"
#include "Type.h"
#include "FileSysTsk.h"

#include "GPSFileDB.h"

static  char   szPathname[32];
//static  char   szFilename[32];

int GetDcfFileName(char *szFilename)
{
 UINT16    uhFolderId, uhFileId;

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysGetDCFNextID(&uhFolderId,&uhFileId);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if (FilesysGetDCFFileName(szFilename,uhFolderId,(uhFileId-1),FST_FMT_MOV) ==FST_STA_NO_FILE)
    {
        debug_msg("%s, no such file\r\n",__func__);
    } else {
        // change ext name from .mov to .txt
        strcpy((szFilename+strlen(szFilename)-4),"\0");
    }
}

int GetFolderPathName(char *szPath,UINT16 uhFolderID)
{
 char   szFolderName[6];
 int    iRet = FST_STA_OK;


      FilesysWaitCmdFinish(FST_TIME_INFINITE);
      iRet = FilesysGetDCFFolderName(szFolderName,uhFolderID);
      FilesysWaitCmdFinish(FST_TIME_INFINITE);
      if (iRet== FST_STA_OK)
      {
          sprintf(szPath,"A:\\DCIM\\%03d%s\\",uhFolderID,szFolderName);
      }

      return iRet;
}

static node head={0};

static link FindNode(char *Filename)
{
 link ptr;

    ptr = &head;
    while (ptr->rlink != &head)
    {
        ptr = ptr->rlink;
        //debug_msg("%s,ptr->szFileName:%s.Filename:%s\r\n",__func__,ptr->szFileName,Filename);
        if (!strncmp(ptr->szFileName,Filename,25))
            return ptr;
    }

    return ptr;
}

void addNode(char *Filename,char *Extname)
{
 link newnode;
 link SearchNode;

    newnode = malloc(sizeof(node));
    if (newnode==NULL)
    {
        debug_msg("malloc failed\r\n");
        return;
    }
    // extFilename:"AVI" or "TXT"
    if (strncmp(Extname,"AVI",3)&&strncmp(Extname,"TXT",3))
    {
        debug_msg("%s,Reject filename:%s,Extname:%s\r\n",__func__,Filename,Extname);
        return;
    }

    memset(newnode,0,sizeof(node));
    if (head.rlink==0) // double link is empty
    {
        strcpy(newnode->szFileName,Filename);
        newnode->llink = newnode->rlink =&head;
        head.rlink = newnode;
        if (!strncmp(Extname,"AVI",3)) {
            newnode->Status |=Exist_Video;
        }

        if (!strncmp(Extname,"TXT",3)) {
            newnode->Status |=Exist_Log;
        }

    } else {
        SearchNode = FindNode(Filename);
        if (SearchNode->rlink != &head) // searching node already existed
        {
            if (!strncmp(Extname,"AVI",3)) {
                SearchNode->Status |=Exist_Video;
            }

            if (!strncmp(Extname,"TXT",3)) {
                SearchNode->Status |=Exist_Log;
            }

            // free newnode node
            free(newnode);
        } else {
            // case 1: searching node is in trail
            if (!strcmp(SearchNode->szFileName,Filename))
            {
                if (!strncmp(Extname,"AVI",3)) {
                    SearchNode->Status |=Exist_Video;
                }

                if (!strncmp(Extname,"TXT",3)) {
                    SearchNode->Status |=Exist_Log;
                }

                // free newnode node
                free(newnode);
            } else {
            // case 2: only insert new node normally
            // node not found and need to allocate one new node
            // new node is inserted behind head node
            link TmpNode;

                TmpNode = head.rlink;
                newnode->rlink = TmpNode;
                newnode->llink = &head;
                TmpNode->llink = newnode;
                head.rlink = newnode;
                strcpy(newnode->szFileName,Filename);
                if (!strncmp(Extname,"AVI",3)) {
                    newnode->Status |=Exist_Video;
                }

                if (!strncmp(Extname,"TXT",3)) {
                    newnode->Status |=Exist_Log;
                }
            }
        }
    }

}

void DelNodeAll(void)
{
 link ptr,tmpptr;

    ptr = &head;
    ptr = ptr->rlink;
    while (ptr!=&head)
    {
        tmpptr = ptr;
        debug_msg("%s,Del filename:%s\r\n",__func__,tmpptr->szFileName);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        FilesysDeleteByName(strcat(tmpptr->szFileName,".TXT"),FALSE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        ptr = ptr->rlink;
        free(tmpptr);
    }

    // link is empty
    head.rlink = 0;

}

void DelNode(char *Filename,char *Extname)
{
  link delnode;
  link SearchNode;

       // extFilename:"AVI" or "TXT"
       if (strncmp(Extname,"AVI",3)&&strncmp(Extname,"TXT",3))
       {
         debug_msg("%s,Reject filename:%s\r\n",__func__,Filename);
         return;
       }

       SearchNode = FindNode(Filename);
       if (SearchNode!=&head)
       {
           // deleted node need log/video file existed simultaneously
           if (SearchNode->Status == (Exist_Video|Exist_Log))
           {
               //debug_msg("%s,Del filename:%s\r\n",__func__,Filename);
               FilesysWaitCmdFinish(FST_TIME_INFINITE);
               FilesysDeleteByName(strcat(SearchNode->szFileName,".TXT"),FALSE);
               FilesysWaitCmdFinish(FST_TIME_INFINITE);
               SearchNode->rlink->llink = SearchNode->llink;
               SearchNode->llink->rlink = SearchNode->rlink;
               free(SearchNode);
           }
       }
}

static void Filesys_dirCB(FIND_DATA *findDir, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
{
    char    cFileName[9] = {0}; // 8.3 format
    char    cExtName[4] = {0};
    char    cFullFilename[32];


    memcpy(cFileName, findDir->FATMainName, 8);
    memcpy(cExtName, findDir->FATExtName, 3);
//    debug_err(("[Filesys_dirCB]: findDir->FATMainName = %s\r\n", cFileName));
//    debug_err(("[Filesys_dirCB]: findDir->FATExtName = %s\r\n", cExtName));
    strcpy(cFullFilename,szPathname);
    strcat(cFullFilename,cFileName);
    addNode(cFullFilename,cExtName);
}

void GPSFileDBInit(void)
{
  UINT16 huCurFolderId,uhMinFolderID,uhFileID;
  char   szFolderName[6];

      /*
        1. get next DCF DirID/fileID
        2. get Dir pathname
        3. get all filenames in this specific folder
      */
      if (FilesysGetDCFIDBySeq(1,&uhMinFolderID,&uhFileID)!=FST_STA_OK)
      {
        debug_msg("No file is storage\r\n");
        return;
      }
      FilesysWaitCmdFinish(FST_TIME_INFINITE);
      FilesysGetDCFNextID(&huCurFolderId,&uhFileID);
      do {
          if (GetFolderPathName(szPathname,huCurFolderId) == FST_STA_OK)
          {
              FilesysWaitCmdFinish(FST_TIME_INFINITE);
              FilesysDirDirectory(szPathname, Filesys_dirCB, FALSE);
          }
          huCurFolderId--;
      } while (huCurFolderId >= uhMinFolderID);
}


void Print_allNode(CHAR *szArgument)
{
 link ptr;

    if (head.rlink==0)
    {
        debug_msg("link empty\r\n");
        return;
    }
    ptr = &head;
    while (ptr->rlink!=&head)
    {
        ptr = ptr->rlink;
        debug_msg("%s,filename:%s,status:0x%x\r\n",__func__,ptr->szFileName,ptr->Status);
    }
}



