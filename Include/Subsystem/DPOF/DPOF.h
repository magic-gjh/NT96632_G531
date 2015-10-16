/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       DPOF.h
    @ingroup    mISYSDPOF

    @brief      DPOF functions.
                this file realize the DPOF standard.
    @note       Using Sample :

                set this pic:
                    DPOFOpen();
                    DPOF_NewJobObj(gNegoFSTParam.FilenameID, 0, 1, gNegoFSTParam.DirectoryID);
                    DPOFClose();
                set pics in ALL folder
                    DPOFOpen();
                    DPOF_SetAll(TRUE, 1);
                    DPOFClose();
*/

#ifndef _DPOF_H
#define _DPOF_H

#include "fs_file_op.h"
//#NT#2009/09/24#SA4 -begin
//#NT#2008/10/29#Marx Chiu -begin
#define DPOF_MRK_FULL_PATH      "A:\\MISC\\AUTPRINT.MRK"
//#NT#2008/10/29#Marx Chiu -end
//#NT#2009/09/24#SA4 -end

#define DPOF_QTY_MAX            255 //for Set all only
#define DPOF_MAX_LINE_SIZE      (80)

#define DPOF_PRTTYP_STD         1
#define DPOF_PRTTYP_IDX         2

#define DPOF_CR                 (0x0d)
#define DPOF_LF                 (0x0a)

#define DPOF_STATUS_UNINITIALED 0
#define DPOF_STATUS_INITIALED   1
#define DPOF_AUTO_PRINT         1

#define DPOF_MAX_PLAY_ID        9
#define DPOF_MIN_PLAY_ID        0

#define DPOF_READ_ONLY          2
#define DPOF_READ_AND_CHECK     1

#define DPOF_NO_UPDATE          2
#define DPOF_UPDATE             1

#define DPOF_FILE_PLAY0         (0)
#define DPOF_FILE_PLAY1         (1)
#define DPOF_FILE_PLAY2         (2)
#define DPOF_FILE_PLAY3         (3)
#define DPOF_FILE_PLAY4         (4)
#define DPOF_FILE_PLAY5         (5)
#define DPOF_FILE_PLAY6         (6)
#define DPOF_FILE_PLAY7         (7)
#define DPOF_FILE_PLAY8         (8)
#define DPOF_FILE_PLAY9         (9)
#define DPOF_FILE_PRINT         (10)
#define DPOF_FILE_TRANSFER      (11)

//*1.Define for the result of changing Qty or DSC of single image
#define DPOF_OK           0x0000
#define DPOF_FAIL         0x0001
#define DPOF_NO_FILE     (0x00010|DPOF_FAIL)  //3                    //3
#define DPOF_NOT_JPEG    (0x00100|DPOF_FAIL)  //5

//*2. Define for date-print
#define DPOF_DATEPRT_NO    0
#define DPOF_DATEPRT_YES     1

//#NT#2007/03/28#Lily Kao -begin
#define DPOF_HDR_CRT_MAX_LEN        30
#define DPOF_HDR_REV_MAX_LEN        6

#define DPOF_DSCSTR_MAX_LEN         20
#define DPOF_DSC_UNKNOW             0xFF
#define DPOF_DSC_UPDATENOW          1
#define DPOF_DSC_UPDATELATER        0

//#NT#2009/11/27#Lily Kao -begin
//Merge MintonZ930 DPOF all-index,paper-size 
#define DPOF_PAPER_DEF  0
#define DPOF_PAPER_3x5  0x3
#define DPOF_PAPER_4x6  0x4
#define DPOF_PAPER_5x7  0x5
#define DPOF_PAPER_8x10 0x8
//#NT#2009/11/27#Lily Kao -end

typedef enum
{
    DPOF_HDR_REV  ,
    DPOF_HDR_CRT
}DPOF_HDR_ITEM;

typedef enum
{
    DPOF_GET,
    DPOF_SET
}DPOF_ACCESS_TYPE;

typedef enum
{
    DPOF_DSC_DATE_YMD,        //Year:Month:Date
    DPOF_DSC_DATE_DMY,        //Date:Month:Year
    DPOF_DSC_DATE_MDY         //Month:Date:Year
}DPOF_DSC_DATE_FORMAT;

typedef enum
{
    DPOF_DSC_DATE_TIME,      //Both Date and Hour
    DPOF_DSC_DATE,           //Only date
    DPOF_DSC_TIME            //Only Hour
}DPOF_DSC_FORMAT;
//#NT#2007/03/28#Lily Kao -end

typedef struct{
    UINT16          uiFileID;
    UINT16          uiDirID;
}DPOF_IDX_JOB_OBJ, *PDPOF_IDX_JOB_OBJ;

typedef struct{
    UINT8           uiQty;
    UINT8           uiDSC;
    UINT16          rev;
}DPOF_INFO_OBJ, *PDPOF_INFO_OBJ;

typedef struct{
    UINT8           uiConfirm;
    UINT8           uiQty;
    UINT8           uiDSC;
    UINT8           rev;
    UINT16          uiDirID;
    UINT16          rev2;
} DPOF_SEARCH_INFO, *PDPOF_SEARCH_INFO;

//---- base object ----
typedef struct{
    HNVT_FILE       *pFile;
    char            *pData;
    UINT16          CurDataOffset;
    UINT16          DataLength;     // how many lines in this file
    UINT16          Jobs;           // how many job section in this file
    UINT16          rev;
}DPOF_BASE_OBJ, *PDPOF_BASE_OBJ;


/************************
*  functions declare    *
************************/

extern void         DPOFOpen(UINT8 mode, UINT32 buffAddr);
extern void         DPOFClose(UINT8 mode);
extern UINT16       DPOF_DeleteJob(UINT16 DeleteID, UINT16 DirID);
//#NT#2008/04/03#Lily Kao -begin
//this is an internal used function
//extern UINT16       DPOF_NewJobObj(UINT16 SearchID, UINT8 DSC, UINT8 Qty, UINT16 DirID);
//#NT#2008/04/03#Lily Kao -end

extern UINT16       DPOF_SetQty(UINT16 SearchID, UINT8 Qty, UINT16 DirID);
extern UINT16       DPOF_SetDate(UINT8 YesOrNo);
extern void         DPOF_DeleteAll_and_DeleteMISC(void);

extern UINT16       DPOF_SetAll(UINT8 Confirm, UINT8 Qty, UINT8 DSC);
extern UINT8        DPOF_GetAll(void);

extern UINT8        DPOF_GetQty(UINT16 SearchID, UINT16 DirID);
extern UINT16       DPOF_FindPreviousJPGID(void);
extern UINT16       DPOF_FindNextJPGID(void);
//#NT#2007/03/28#Lily Kao -begin
extern UINT16       DPOF_SetOneDateStr(UINT16 fileID, UINT8 Confirm, UINT16 DirID, char *pDscStr);
//#NT#2007/03/28#Lily Kao -end
extern UINT8        DPOF_GetDate(void);
extern UINT8        DPOF_SetThisDirImageToIdx(UINT16 Dirid);
extern UINT16       DPOF_GetTotalJobs(void);

//#NT#2007/03/16#Lily Kao -begin
extern UINT8                DPOF_AccessHeaderItem(DPOF_HDR_ITEM item, DPOF_ACCESS_TYPE accType, char *pRetVal, INT8 length);
extern void                 DPOF_SetDSCFormat(DPOF_DSC_FORMAT dscFormat);
extern DPOF_DSC_FORMAT      DPOF_GetDSCFormat(void);
extern void                 DPOF_SetDSCDateFormat(DPOF_DSC_DATE_FORMAT dateFormat);
extern DPOF_DSC_DATE_FORMAT DPOF_GetDSCDateFormat(void);
extern UINT16               DPOF_IsCurrExist(UINT16 fileID, UINT16 DirID);
//#NT#2007/03/16#Lily Kao -end

//#NT#2007/04/19#Stephen Wang -begin
//#NT#Add new added API prototype
extern UINT16 DPOF_GetTotalDatePrt(void);
extern UINT16 DPOF_GetTotalPrints(void);
//#NT#2007/04/19#Stephen Wang -end

//#NT#2007/04/18#Lily Kao -begin
extern void DPOF_DumpJob(void);
extern void DPOF_DumpMem(int dumpId);
//#NT#2007/04/18#Lily Kao -end

//#NT#2008/04/03#Lily Kao -begin
//set dpof cropping
extern UINT16 DPOF_SetCrop(UINT16 fileId, UINT16 dirId, UINT16 cropX, UINT16 cropY, UINT16 cropW, UINT16 cropH);
//#NT#2008/04/03#Lily Kao -end
//#NT#2009/09/24#SA4 -begin
//#NT#2008/10/29#Marx Chiu -begin
extern BOOL DPOF_GetHandleNum(UINT32*     pHandleNum);
//#NT#2008/10/29#Marx Chiu -end
//#NT#2009/09/24#SA4 -end

//#NT#2009/11/27#Lily Kao -begin
//Merge MintonZ930 DPOF all-index,paper-size 
extern UINT16 DPOF_SetAllIdx(UINT8 Confirm, UINT8 Qty);
extern UINT8  DPOF_GetAllIdx(void);
extern UINT8  DPOF_GetOneIdx(UINT16 fileID, UINT16 DirID);
extern void   DPOF_SetPapersize(UINT32 papersize);
extern UINT32 DPOF_GetPapersize(void);
//#NT#2009/11/27#Lily Kao -end

#endif //_DPOF_H

