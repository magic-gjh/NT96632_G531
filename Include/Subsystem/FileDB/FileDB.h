/*
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       FilesDb.h
    @ingroup

    @brief

    @note       Nothing.

    @version    V0.00.001
    @author     Lincy Lin
    @date       2007/02/05
*/


#ifndef _FILESDB_H_
#define _FILESDB_H_

#include "type.h"
#include "Fs_file_op.h"
#include "FilesysTsk.h"


typedef INT16 FILEDB_HANDLE;
//#NT#2007/03/22#lincy-begin
//#Modify for support use Dynamic memory to store the files info
//#NT#2007/07/30#lincy-begin
//#Change the max file num to 10 when use static memory
//#define FILEDB_STATIC_MEM_MAX_FILE_NUM  200
#define FILEDB_STATIC_MEM_MAX_FILE_NUM  10
//#NT#2007/07/30#lincy-end
//#NT#2007/07/27#lincy-begin
//#Enlarge the max file number to 5000
//#define FILEDB_MAX_FILE_NUM             1000
#define FILEDB_MAX_FILE_NUM             5000
//#NT#2007/07/27#lincy-end
//#NT#2007/08/06#Lincy Lin-begin
//#Add some error handling for filter out the same DCF number folder ID                             
#define FILEDB_MAX_DIFF_DCF_FOLDER      (FILEDB_MAX_FILE_NUM/5)
//#NT#2007/08/06#Lincy Lin-end

//#NT#2007/03/22#lincy-end

#define FILEDB_CREATE_ERROR   -1
#define FILEDB_NUM             2
#define FILE_FILTER_MAX_LEN   40

//#NT#2007/05/10#lincy-begin
//#Modify for support get files count by file type
//#NT#2007/06/13#lincy-begin
//#Add WAV file type
//#NT#2007/07/03#lincy-begin
//#Add MOV file type
#define FILEDB_SUPPORT_FILE_TYPE_NUM  6
//#NT#2007/07/03#lincy-end
//#NT#2007/06/13#lincy-end
//#NT#2007/05/10#lincy-end
#define FILEDB_FMT_JPG  0x00000001
#define FILEDB_FMT_MP3  0x00000002
#define FILEDB_FMT_AVI  0x00000004
//#NT#2007/03/23#lincy-begin
//#Add ASF file type
#define FILEDB_FMT_ASF  0x00000008
//#NT#2007/03/23#lincy-end
//#NT#2007/06/13#lincy-begin
//#Add WAV file type
#define FILEDB_FMT_WAV  0x00000010
//#NT#2007/06/13#lincy-end
//#NT#2007/07/03#lincy-begin
//#Add MOV file type
#define FILEDB_FMT_MOV  0x00000020
//#NT#2007/07/03#lincy-end
#define FILEDB_GET_DAY_FROM_DATE(x)     (x & 0x1F)
#define FILEDB_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)
#define FILEDB_GET_YEAR_FROM_DATE(x)    ((x >> 9) & 0x7F)+1980

//#NT#2007/03/28#lincy-begin
//#Add the sorting feature for FileDB
//#NT#2007/06/13#lincy-begin
//#Add the new sort type for FileDB
typedef enum {
    FILEDB_SORT_BY_NONE = 0x00,    
    FILEDB_SORT_BY_CREDATE,
    FILEDB_SORT_BY_MODDATE,
    FILEDB_SORT_BY_NAME,
    FILEDB_SORT_BY_SIZE,  
    FILEDB_SORT_BY_FILETYPE        
} FILEDB_SORT_TYPE;
//#NT#2007/06/13#lincy-end
//#NT#2007/03/28#lincy-end

typedef struct {
    char                 filename[KFS_FILENAME_MAX_LENG+1];    ///< File Name
    char                 filePath[KFS_FILEPATH_MAX_LENG+1];    ///< File full path
    UINT8                attrib;                               ///< File attribute.
    UINT32               fileSize;                             ///< File size in bytes.
    UINT32               fileType;                             ///< File type, the value could be FILEDB_FMT_JPG, FILEDB_FMT_MP3 or FILEDB_FMT_AVI ...
    //#NT#2007/06/13#lincy-begin
    //#Add the new file info for FileDB
    UINT16               creTime;                              ///< File created time. bit0~4: seconds/2, bit5~10: minutes, bit11~15: hours.
    UINT16               creDate;                              ///< File created date. bit0~4: day(1-31), bit5~8: month(1-12), bit9~15: year(0-127) add 1980 to convert
    UINT16               lastWriteTime;                        ///< The latest time of write file.
    UINT16               lastWriteDate;                        ///< The latest date of write file.
    //#NT#2007/06/13#lincy-end    
} FILEDB_FILE_ATTR, *PFILEDB_FILE_ATTR;

typedef struct {
    char*                rootPath;                             ///<  Root path
    UINT32               fileFilter;
    BOOL                 bIsRecursive;                         ///<  If recursive search this path
    BOOL                 bIsCyclic;                            ///<  If cyclic search
    //#NT#2007/03/22#lincy-begin
    //#Modify for support use Dynamic memory to store the files info
    BOOL                 bIsUseDynaMem;                        ///<  If Use Dynamic Memory
    UINT32               u32MemAddr;                           ///<  The Dynamic Memory address
    UINT32               u32MemSize;                           ///<  The Dynamic Memory Size
    //#NT#2007/03/22#lincy-end
    //#NT#2007/06/01#Lincy Lin-begin
    //#Add new parameter for Choose to move file index to last item when Create File DB 
    BOOL                 bIsMoveToLastFile;                    ///<  If want to move file index to last file       
    //#NT#2007/06/01#Lincy Lin-end
    //#NT#2007/06/13#Lincy Lin-begin
    //#Add new parameter for just get DCF file when Create File DB     
    BOOL                 bIsDCFFileOnly;
    //#NT#2007/06/13#Lincy Lin-end    
    //#NT#2007/07/30#Lincy Lin-begin
    //#Add new parameter for filter out the same DCF number folder ID 
    BOOL                 bIsFilterOutSameDCFNumFolder;
    //#NT#2007/07/30#Lincy Lin-end
} FILEDB_INIT_OBJ, *PFILEDB_INIT_OBJ;

typedef struct {
    BOOL                 bIsUsed;                              ///< if this FileDB is used
    UINT16               u16TotalFilesNum;                     ///<  Total items count in this directory
    UINT16               u16CurrentFileIndex;                  ///<  Current file Index
    UINT32               fileFilter;
    BOOL                 bIsRecursive;                         ///<  If recursive search this path
    BOOL                 bIsCyclic;                            ///<  If cyclic search
    //#NT#2007/03/22#lincy-begin
    //#Modify for support use Dynamic memory to store the files info
    BOOL                 bIsUseDynaMem;                        ///<  If Use Dynamic Memory
    UINT32               u32MemAddr;                           ///<  The Dynamic Memory address
    UINT32               u32MaxSotreItemsNum;                  ///<  The Max Number of Files Info can be sotred in the Dynamic Memory
    //#NT#2007/03/28#lincy-begin
    //#Add the sorting feature for FileDB
    FILEDB_SORT_TYPE     sortType;                             ///<  The sort type of this database
    //#NT#2007/06/13#lincy-begin
    //#Add one sorting parameter for choose small first or large first   
    BOOL                 bIsSortLargeFirst;
    //#NT#2007/06/13#lincy-end    
    //#NT#2007/08/06#Lincy Lin-begin
    //#Modify for support max files exactly = FILEDB_MAX_FILE_NUM                             
    UINT16               sortIndexArray[FILEDB_MAX_FILE_NUM+FILEDB_MAX_DIFF_DCF_FOLDER];  ///<  The sorting mapping index
    //#NT#2007/08/06#Lincy Lin-end
    //#NT#2007/03/28#lincy-end    
    FILEDB_FILE_ATTR     file[FILEDB_STATIC_MEM_MAX_FILE_NUM];
    //#NT#2007/03/22#lincy-end
    //#NT#2007/05/10#lincy-begin
    //#Modify for support get files count by file type
    UINT16               filesNum[FILEDB_SUPPORT_FILE_TYPE_NUM];  ///<  The files count of one file type   
    //#NT#2007/05/10#lincy-end    
    //#NT#2007/05/30#Lincy Lin-begin
    //#Add new function for refresh the File DataBase when some files are deleted 
    char                 rootPath[KFS_FILEPATH_MAX_LENG+1];    ///<  Root path       
    //#NT#2007/05/30#Lincy Lin-end    
    //#NT#2007/06/13#Lincy Lin-begin
    //#Add new parameter for just get DCF file when Create File DB     
    BOOL                 bIsDCFFileOnly;
    //#NT#2007/06/13#Lincy Lin-end    
    //#NT#2007/07/30#Lincy Lin-begin
    //#Add new parameter for filter out the same DCF number folder ID 
    BOOL                 bIsFilterOutSameDCFNumFolder;
    //#NT#2007/07/30#Lincy Lin-end   
} FILEDB_INFO, *PFILEDB_INFO;


extern FILEDB_HANDLE FileDB_Create(FILEDB_INIT_OBJ fileDbInitObj);
extern void FileDB_Release(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_SearhFile(FILEDB_HANDLE fileDbHandle, INT16 i16FileIndex);
extern UINT16 FileDB_GetTotalFileNum(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_CurrFile(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_NextFile(FILEDB_HANDLE fileDbHandle);
extern PFILEDB_FILE_ATTR FileDB_PrevFile(FILEDB_HANDLE fileDbHandle);
extern UINT16 FileDB_GetCurrFileIndex(FILEDB_HANDLE fileDbHandle);
//#NT#2007/03/28#lincy-begin
//#Add the sorting feature for FileDB   
//#NT#2007/06/13#lincy-begin
//#Add one sorting parameter for choose small first or large first   
extern void FileDB_SortBy(FILEDB_HANDLE fileDbHandle, FILEDB_SORT_TYPE sortType, BOOL bIsSortLargeFirst);  
//#NT#2007/06/13#lincy-end
//#NT#2007/03/28#lincy-end
//#NT#2007/05/10#lincy-begin
//#Modify for support get files count by file type
extern UINT16 FileDB_GetFilesNumByFileType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);    
//#NT#2007/05/10#lincy-end   
//#NT#2007/05/30#Lincy Lin-begin
//#Add new function for refresh the File DataBase when some files are deleted
extern void FileDB_Refresh(FILEDB_HANDLE fileDbHandle);
//#NT#2007/05/30#Lincy Lin-end
#endif  //_FILESDB_H_
