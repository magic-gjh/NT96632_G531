#ifndef UI_SYSTEM_STATUS_SETTING_H
#define UI_SYSTEM_STATUS_SETTING_H

#include "Type.h"
#include "Gesture_Public.h"

// Firmware version string max length
#define MAX_VER_STR_LEN             32

// Storage paramter
#define STORAGE_RESERVED_CLUS_NUM   4

// Storage validation
#define STORAGE_CHECK_FULL          0
#define STORAGE_CHECK_LOCKED        1
#define STORAGE_CHECK_ERROR         2
#define STORAGE_CHECK_SPACE_FULL    3
#define STORAGE_CHECK_CLUSTER       4

// Definition of min number
#define PHOTO_PIC_NUM_MIN           0   // Minimum acceptable free picture number
#define PANO_PIC_NUM_MIN            3   // Minimum acceptable free picture number in panorama mode
#define MOVIE_REC_TIME_MIN          3   // Minimum acceptable free movie recording time


//-----------------------------------------------------------------------------
// FS Status
//-----------------------------------------------------------------------------
typedef enum
{
    FS_NOT_INIT = 0,
    FS_INIT_OK,
    FS_UNFORMATTED,
    FS_UNKNOWN_FORMAT,
    FS_DISK_ERROR,
    FS_STATUS_TOTAL_NUM
} FS_STATUS;

//-----------------------------------------------------------------------------
// Card Status
//-----------------------------------------------------------------------------
typedef enum
{
    CARD_REMOVED = 0,
    CARD_INSERTED,
    CARD_LOCKED,
    CARD_STATUS_TOTAL_NUM
} CARD_STATUS;

//-----------------------------------------------------------------------------
// Battery Status
//-----------------------------------------------------------------------------
typedef enum
{
    BATTERY_FULL = 0,
    BATTERY_MED,
    BATTERY_LOW,
    BATTERY_EMPTY,
    BATTERY_EXHAUSTED,
    BATTERY_STATUS_TOTAL_NUM
} BATTERY_STATUS;

//-----------------------------------------------------------------------------
// MP3 FileTable Renew
//-----------------------------------------------------------------------------
typedef enum
{
    MP3FTB_RENEW_OFF = 0,
    MP3FTB_RENEW_ON,
    MP3FTB_RENEW_SETTING_MAX
} MP3FTB_RENEW_SETTING;

//-----------------------------------------------------------------------------
// Bootloader Info
//-----------------------------------------------------------------------------
#define BLINFO_DRAMEND_OFFSET       0x00010000      // 64K bytes (32K bytes are fw c runtime heap size)
#define BLINFO_START_ADDR           (DRAM_END - BLINFO_DRAMEND_OFFSET)

#define MAX_BLINFO_BLNAME_LEN       16              // Bootloader bin name, ex: 12345678.BIN
#define MAX_BLINFO_FWNAME_LEN       16              // Firmware bin name, ex: 12345678.BIN
#define MAX_BLINFO_VER_LEN          16              // Bootloader version, ex: v1.0
#define MAX_BLINFO_DATE_LEN         16              // Bootloader build date, ex: 10/25/2010
#define MAX_BLINFO_TIME_LEN         16              // Bootloader build time, ex: 18:14:52
#define MAX_BLINFO_DRAMTYPE_LEN     16              // Dram type, ex: RAMTYPE_DDR2
#define MAX_BLINFO_DRAMSIZE_LEN     16              // Dram size, ex: SIZE_32MB
#define MAX_BLINFO_STORAGEINT_LEN   16              // Internal storage, ex: NandEccHamming

// Bootloader info object
typedef struct _BLINFO_OBJ
{
    char    strBLName[MAX_BLINFO_BLNAME_LEN];           ///< Bootloader bin name
    char    strFWName[MAX_BLINFO_FWNAME_LEN];           ///< Firmware bin name
    char    strBLVer[MAX_BLINFO_VER_LEN];               ///< Bootloader version
    char    strBLDate[MAX_BLINFO_DATE_LEN];             ///< Bootloader build date
    char    strBLTime[MAX_BLINFO_TIME_LEN];             ///< Bootloader build time
    char    strDramType[MAX_BLINFO_DRAMTYPE_LEN];       ///< Dram type
    char    strDramSize[MAX_BLINFO_DRAMSIZE_LEN];       ///< Dram size
    char    strStorageInt[MAX_BLINFO_STORAGEINT_LEN];   ///< Internal storage
} BLINFO_OBJ, *PBLINFO_OBJ;

//-----------------------------------------------------------------------------
// Menu Store Info
//-----------------------------------------------------------------------------
typedef struct _UIMenuUIMenuStoreInfo
{
    /*--- General ---*/
    CHAR        strFwVersion[MAX_VER_STR_LEN];
    UINT32      uiInfoSize;

    /*--- Photo ---*/
    UINT32      uiSizeIndex;
    UINT32      uiQualityIndex;
    UINT32      uiSceneIndex;
    UINT32      uiISOIndex;
    UINT32      uiEVIndex;
    UINT32      uiWBIndex;
    UINT32      uiMeteringIndex;
    UINT32      uiSharpnessIndex;
    UINT32      uiSaturationIndex;
    UINT32      uiFDIndex;
    UINT32      uiAntiShakingIndex;
    UINT32      uiQuickReviewIndex;
    UINT32      uiDateImprintIndex;
    UINT32      uiDirNumber;
    UINT32      uiFileNumber;

    /*--- Movie ---*/
    UINT32      uiAVISizeIndex;
    UINT32      uiAVIQualityIndex;
    UINT32      uiAVIColorIndex;
    UINT32      uiMovieDISIndex;
    UINT32      uiMovieDateImprintIndex;
    UINT32      uiMovieAudioOnIndex;
    UINT32      uiMovieMotionDetIndex;
    UINT32      uiMovieGodBlessIndex;
    UINT32      uiMovieCyclicRecIndex;
    UINT32      uiMovieGolfShotIndex;
    UINT32      uiMovieFlashRecIndex;
    UINT32      uiMovieGsensorIndex;

    /*--- Setup ---*/
    UINT32      uiBeepIndex;
    UINT32      uiVolumeIndex;
    UINT32      uiLanguageIndex;
    UINT32      uiTVModeIndex;
    UINT32      uiPowerOffIndex;
    UINT32      uiFreqIndex;
    UINT32      uiBrightIndex;
    UINT32      uiDateFormat;
    UINT32      uiOpening;
    UINT32      uiLcdOffIndex;	
    UINT32      uiDelayPowerOffIndex;	
    UINT32      uiRotateIndex;	
    UINT32      uiACCOffIndex;

    /*--- Print ---*/
    //Rsvd

    /*--- Gesture ---*/
    #if (_TouchPanel_ != DISABLE)
    GESTURE_CALI_DATA_STRUCT    GestureCaliData;
    BOOL        bIsTouchCal;    // Is touch panel calibrated?
    #endif

    /*--- USB serial string desc3 ---*/
    UINT16 USBSerialStrDesc3[16];

} UIMenuStoreInfo;

//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern CHAR     *GetVersionString(void);

extern void     SetCardStatus(UINT32 uiStatus);
extern UINT32   GetCardStatus(void);

extern void     SetBatteryLevel(UINT32 uiStatus);
extern UINT32   GetBatteryLevel(void);

extern void     SetFSStatus(UINT32 uiStatus);
extern UINT32   GetFSStatus(void);
extern const CHAR *GetFSStatusString(UINT32 uiStatus);

extern void     Set_MP3FtbRenewIndex(UINT32 uiIndex);
extern UINT32   Get_MP3FtbRenewIndex(void);

extern void     Init_BLInfo(void);
extern void     Get_BLInfo(PBLINFO_OBJ pBLInfo);

extern BOOL     IsDCIMWritable(void);
extern BOOL     UI_Validate_Storage(UINT32 uiCheckItem, UINT32 *pFreeCount);

extern void     Init_MenuInfo(void);
extern void     Save_MenuInfo(void);
extern void     SysReset_MenuInfo(void);

#endif
