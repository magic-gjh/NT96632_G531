
#ifndef  __GPSFileDB_H_
#define __GPSFileDB_H_

#include "Type.h"

#define    FileLen           (32)

#define    Exist_Video       (0x00000001U)
#define    Exist_Log         (0x00000002U)
#define    Vide_Log_Sync     (0x00000004U)


typedef struct list {
    char szFileName[FileLen];
    int  Status;
    struct list *llink;
    struct list *rlink;
} node,*link;


extern void GPSFileDBInit(void);
extern void DelNode(char *Filename,char *Extname);
extern void addNode(char *Filename,char *Extname);
extern int  GetFolderPathName(char *szPath,UINT16 uhFolderID);
extern int  GetDcfFileName(char *szFilename);
extern void Print_allNode(CHAR *szArgument);

#endif // __GPSFileDB_H_