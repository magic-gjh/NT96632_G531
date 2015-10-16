/**
    Header file of Primary Task

    Exported header file of primary task.

    @file       PrimaryTsk.h
    @ingroup    mIAPPMain
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _PRIMARYTSK_H
#define _PRIMARYTSK_H

#include "OSCommon.h"
#include "StrgDef.h"

/**
    @addtogroup mIAPPMain
*/
//@{

/**
    Mode for Primary_ChangeMode() API
*/
typedef enum
{
    PRIMARY_MODE_UNKNOWN,       ///< No primary mode is selected
    PRIMARY_MODE_PHOTO,         ///< Photo capture mode
    PRIMARY_MODE_PLAYBACK,      ///< Playback mode
    PRIMARY_MODE_SETUP,         ///< Setup mode
    PRIMARY_MODE_USBMSDC,       ///< USB MSDC mode
//#NT#2009/09/24#SA4 -begin
    PRIMARY_MODE_USBSICD,       ///< USB SICD/Pictbridge mode
//#NT#2009/09/24#SA4 -end
    PRIMARY_MODE_USBPCC,        ///< USB PC camera mode
    PRIMARY_MODE_MOVIE,         ///< Movie recording mode
    PRIMARY_MODE_MUSIC,         ///< Music playback mode
    PRIMARY_MODE_DPFMENU,       ///< DPF menu mode
    PRIMARY_MODE_VIDEO,         ///< Video playback mode
//#NT#2009/09/24#SA4 -begin
    PRIMARY_MODE_ENGINEER,      ///< Engineering mode
    PRIMARY_MODE_DUMMY,         ///< Dummy mode
//#NT#2009/09/24#SA4 -end
    PRIMARY_MODE_CUSTOM1=16,    ///< Custom mode 1
    PRIMARY_MODE_CUSTOM2,       ///< Custom mode 2
    PRIMARY_MODE_VOICE,//PRIMARY_MODE_CUSTOM3,       ///< Custom mode 3
    PRIMARY_MODE_CUSTOM4,       ///< Custom mode 4
    PRIMARY_MODE_CUSTOM5,       ///< Custom mode 5
    PRIMARY_MODE_CUSTOM6,       ///< Custom mode 6
    PRIMARY_MODE_CUSTOM7,       ///< Custom mode 7
    PRIMARY_MODE_CUSTOM8,       ///< Custom mode 8
    PRIMARY_MODE_CUSTOM9,       ///< Custom mode 9
    PRIMARY_MODE_CUSTOM10,      ///< Custom mode 10
    PRIMARY_MODE_CUSTOM11,      ///< Custom mode 11
    PRIMARY_MODE_CUSTOM12,      ///< Custom mode 12

    ENUM_DUMMY4WORD(PrimaryModeID)
} PrimaryModeID;

/**
    Storage type for Primary_ChangeStorage() API
*/
typedef enum
{
    PRIMARY_STGTYPE_UNKNOWN,    ///< No/Not valid storage type is selected
    PRIMARY_STGTYPE_NAND=28,    ///< Storage type is on board NAND flash
    PRIMARY_STGTYPE_CARD,       ///< Storage type is card
    PRIMARY_STGTYPE_CARD2,      ///< Reserved for twin card slot, but not for current chip revision

    ENUM_DUMMY4WORD(PrimaryStorageID)
} PrimaryStorageID;

/**
    DCF name structure
*/
typedef struct
{
    BOOL    bCreateDir;         ///< Create DCF folder when init File System or not
    CHAR    pDirName[6];        ///< Folder name, 5 characters
    CHAR    pFileName[5];       ///< File name, 4 characters
    CHAR    pVideoName[5];  ///< Video file name, 4 characters
    CHAR    pVideoExt[4];   ///< Video file extension, 3 characters
    CHAR    pAudioName[5];  ///< Audio file name, 4 characters
    CHAR    pAudioExt[4];   ///< Audio file extension, 3 characters
    CHAR    pRAWName[5];    ///< RAW file name, 4 characters
    CHAR    pTIFFName[5];   ///< TIFF file name, 4 characters
    //#NT#2011/04/08#Chris Chung -begin
    //#add custom type
    CHAR    pCustomExt[4];  ///< Custom file extension, 3 characters
    //#NT#2011/04/08#Chris Chung -end
} PRIMARY_DCFNAME, *PPRIMARY_DCFNAME;

/**
    Primary task file system call back prototype
*/
typedef void (*PRIMARY_FSINITCB)(UINT32);

//#NT#2009/09/24#SA4 -begin
#define PRI_TIME_NO_WAIT            (0)
#define PRI_TIME_INFINITE           (0xFFFFFFFF)

#define PRI_STA_BUSY                (-1)
#define PRI_STA_IDLE                (0)
//#NT#2009/09/24#SA4 -end

// Initialize operations
extern void             Primary_InitDCFName(PPRIMARY_DCFNAME pDCFName);
extern void             Primary_InitStorageCard(PSTRG_TAB pStrgObj);

// Storage operations
extern BOOL             Primary_ChangeStorage(PrimaryStorageID StorageID);
//#NT#2008/12/16#Chris Hsu -begin
//#NT#Add the function to wait for change storage done
extern void             Primary_Wait4ChangeStorageDone(void);
//#NT#2008/12/16#Chris Hsu -end
extern PrimaryStorageID Primary_GetChangeStorage(void);
extern PrimaryStorageID Primary_GetStorage(void);
extern PSTRG_TAB        Primary_GetStorageObj(PrimaryStorageID StorageID);
extern void             Primary_SetFSInitCB(PRIMARY_FSINITCB FSInitCB);

// Operating mode operations
extern BOOL             Primary_ChangeMode(PrimaryModeID ModeID);
//#NT#2008/12/16#Chris Hsu -begin
//#NT#Add the function to wait for change mode done
extern void             Primary_Wait4ChangeModeDone(void);
//#NT#2008/12/16#Chris Hsu -end
extern PrimaryModeID    Primary_GetChangeMode(void);
extern PrimaryModeID    Primary_GetCurrentMode(void);
extern PrimaryModeID    Primary_GetPreviousMode(void);
//#NT#2009/09/24#SA4 -begin
extern INT32            Primary_CheckIdle(UINT32 uiTimeOut);
//#NT#2009/09/24#SA4 -end

#endif

//@}
