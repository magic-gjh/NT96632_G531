/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SysCfg.h
    @ingroup

    @brief      Header file of SysCfg.c
                Config the task, event, semaphore, memory pool, memory layout, mailbox

    @note       Nothing.

    @date       2005/11/22
*/

#ifndef _SYSCFG_H
#define _SYSCFG_H

#include "Type.h"
#include "OSCommon.h"
#include "KerCfg.h"
#include "PStore.h"

//===========================================================================
// Module functions selection
// The following symbols may depends on these module functions selection.
// You must take care the library during linking time too.
//===========================================================================
//---------------------------------------------------------------------------
// For IQ Modules
//---------------------------------------------------------------------------
#define _IQ_MODE_                   ENABLE      // Enable/Disable IQ App functions

//---------------------------------------------------------------------------
// For Playback Module
//---------------------------------------------------------------------------
#define _PLAYBACK_MODE_             ENABLE      // Enable/Disable Playback App functions

#define _PLAYBACK_FILESHOW_COMB_    0           // Playback file show in combined group
#define _PLAYBACK_FILESHOW_SEPA_    1           // Playback file show in separate group
#define _PLAYBACK_FILESHOW_         _PLAYBACK_FILESHOW_COMB_

//---------------------------------------------------------------------------
// For Movie Mode Modules
//---------------------------------------------------------------------------
#define _MOVIE_FMT_AVIMJPEG_    0
#define _MOVIE_FMT_MOVH264_     1
#define _MOVIE_FMT_AVIH264_     2

#define _MOVIE_FMT_             _MOVIE_FMT_MOVH264_
//#define _MOVIE_FMT_             _MOVIE_FMT_AVIH264_

#if   (_MOVIE_FMT_ == _MOVIE_FMT_AVIMJPEG_)
#define _MOVIE_AVI_USERDATA_    DISABLE     // Enable/Disable AVI MJPEG with embeded user data
#endif

#define _MOVIE_PIM_MODE_        ENABLE      // Enable/Disable Picture In Movie function
#define _MOVIE_DIS_MODE_        ENABLE      // Enable/Disable DIS function
#define _MOVIE_DATE_STAMP_      ENABLE      // Enable/Disable movie date stamp function
#define _MOVIE_REC_SEAMLESS_    ENABLE     // Enable/Disable movie seamless recording function

//---------------------------------------------------------------------------
// For USB Modules
//---------------------------------------------------------------------------
#define _USBMSDC_MODE_          ENABLE      // Enable/Disable USB MSDC function
#define _USBSICD_MODE_          ENABLE      // Enable/Disable USB SICD function
#define _USBPCC_MODE_           ENABLE      // Enable/Disable USB PCC function

// MSDC modes
#if (_USBMSDC_MODE_ == ENABLE)
#define _MSDC_SINGLE_           0           // Only the current active storage device can be accessed through USB MSDC, i.e. card "OR" nand.
#define _MSDC_ALL_              1           // All the storage devices can be accessed concurrently through USB MSDC, i.e. card "AND" nand.
#define _MSDC_SINGLE_NAND2_     2           // Only the active storage device plus the NAND2 can be accessed concurrently through USB MSDC.
#define _MSDC_ALL_NAND2_        3           // All the storage devices plus the NAND2 can be accessed concurrently through USB MSDC.
#define _MSDC_SINGLE_NAND3_     4           // Only the active storage device plus the NAND3 can be accessed concurrently through USB MSDC.
#define _MSDC_ALL_NAND3_        5           // All the storage devices plus the NAND3 can be accessed concurrently through USB MSDC.
#define _MSDC_MODE_             _MSDC_SINGLE_

  #if ((_MSDC_MODE_ == _MSDC_ALL_NAND3_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_) || (_MSDC_MODE_ == _MSDC_ALL_NAND2_))
    #define NAND2_SIZE          0x800000   // In bytes,and this value must be divisible by the block size of the NAND
  #endif
  #if ((_MSDC_MODE_ == _MSDC_SINGLE_NAND3_) || (_MSDC_MODE_ == _MSDC_ALL_NAND3_))
    #define NAND3_SIZE          0xA00000   // In bytes,and this value must be divisible by the block size of the NAND
  #endif
#endif

//---------------------------------------------------------------------------
// For MP3 Module
//---------------------------------------------------------------------------
#define _MP3_MODE_              ENABLE     // Enable/Disable MP3 player function, 0 = Disable

//---------------------------------------------------------------------------
// For GPS Receiver Module
//---------------------------------------------------------------------------
#define _GPS_RECEIVER_          ENABLE     // Enable/Disable GPS Reciver function

//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
//---------------------------------------------------------------------------
// For Video Debug Info
//---------------------------------------------------------------------------
#define _MOVIE_IQLOG_          DISABLE     // Enable/Disable Video IQLOG
//#NT#2011/10/14#Spark Chou -end

//---------------------------------------------------------------------------
// For GPS Receiver Module
//---------------------------------------------------------------------------
#define _GPS_RECEIVER_LOG_     DISABLE     // Enable/Disable GPS Reciver log function

//#NT#2012/09/05#Philex Lin -begin
//#NT# flag for pressing power button longer to startup system
#define _LONG_STARTUP_          DISABLE     // Enable/Disable pressing power button to startup system
//#NT#2012/09/05#Philex Lin -end


//---------------------------------------------------------------------------
// For GPS Receiver Module
//---------------------------------------------------------------------------
#define _WATERMARK_EMBEDDED_    DISABLE     // Enable/Disable Watermrk logo function

//---------------------------------------------------------------------------
// For Watch dog timer function
//---------------------------------------------------------------------------
#define _WDT_CONTROL_           ENABLE     // Enable/Disable watch dog timer function

//#NT#2010/04/26#Klins Chen -begin
//---------------------------------------------------------------------------
// For HDMI CEC Module
//---------------------------------------------------------------------------
#define _CEC_CONTROL_           DISABLE     // Enable/Disable HDMI-CEC function
//#NT#2010/04/26#Klins Chen -end

//---------------------------------------------------------------------------
// For WAV Module
//---------------------------------------------------------------------------
#define _WAVSTUDIO_MODE_        ENABLE//DISABLE     // Enable/Disable WAV file record/playback function

#if (_WAVSTUDIO_MODE_ == ENABLE)
// Voice memo type
#define _VM_TYPE_SINGLE_        0           // Single WAV file
#define _VM_TYPE_EMBEDDED_      1           // Embedded WAV into JPG file
#define _VM_TYPE_               _VM_TYPE_SINGLE_

// Voice memo format
#define _VM_FORMAT_PCM8_        0
#define _VM_FORMAT_PCM16_       1
#define _VM_FORMAT_ADPCM_       2
#define _VM_FORMAT_ASFPCM_      3
#define _VM_FORMAT_             _VM_FORMAT_PCM16_

#define _WAV_SINGLE_USERDATA_   DISABLE     // Enable/Disable WAV (Single file) user data in header
#else
#define _WAV_SINGLE_USERDATA_   DISABLE
#endif

//---------------------------------------------------------------------------
// For Project Module
//---------------------------------------------------------------------------
#define _COMMANDTSK_MODE_       ENABLE      // Enable/Disable Command task
#define _SYSINFO_MODE_          ENABLE      // Enable/Disable Get System Information function
#define _CALIBRATION_MODE_      ENABLE      // Enable/Disable Calibration
#define _USBWRITE_MODE_         DISABLE     // Enable/Disable Write data to Nand Flash via USB PCC mode function
#define _LOADFW_INMAIN_         DISABLE     // Enable/Disable load FW to run in main code
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
#define _RAM_DISK_              ENABLE     // Enable/Disable RAM disk
#else
#define _RAM_DISK_              DISABLE    // Enable/Disable RAM disk
#endif

//---------------------------------------------------------------------------
// PStore section name ID
//---------------------------------------------------------------------------
#define PS_SYS_PARAM            MAKE_PS_SECNAME_ID('S', 'Y', 'S', 'P')
#define PS_CAL_DATA             MAKE_PS_SECNAME_ID('C', 'A', 'L', 'D')
#define PS_CAL_LENS             MAKE_PS_SECNAME_ID('L', 'E', 'N', 'S')
#define PS_BG_LOGO              MAKE_PS_SECNAME_ID('B', 'G', 'L', 'G')
#define PS_MP3FONT_DATA         MAKE_PS_SECNAME_ID('M', 'F', 'N', 'T')

//---------------------------------------------------------------------------
// OSD color
//---------------------------------------------------------------------------
#define _OSD_COLOR_4_BIT_       0           // 16 colors
#define _OSD_COLOR_8_BIT_       1           // 256 colors
#define _OSD_COLOR_             _OSD_COLOR_8_BIT_

//===========================================================================
// Memory layout
//===========================================================================
// FW size in DRAM (Code + RO + RW + ZI)
// Now defined in MakeConfig.txt
//#define FRE_MEM                 0x00200000

#if ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00001000)
    #define END_MEM             (DRAM_END - 0x00001000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00002000)
    #define END_MEM             (DRAM_END - 0x00002000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00004000)
    #define END_MEM             (DRAM_END - 0x00004000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00008000)
    #define END_MEM             (DRAM_END - 0x00008000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00010000)
    #define END_MEM             (DRAM_END - 0x00010000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00020000)
    #define END_MEM             (DRAM_END - 0x00020000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00040000)
    #define END_MEM             (DRAM_END - 0x00040000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00080000)
    #define END_MEM             (DRAM_END - 0x00080000)
#elif ((DRAM_END - HeapEnd_CRuntime + HeapSize_CRuntime) <= 0x00100000)
    #define END_MEM             (DRAM_END - 0x00100000)
#else
    #define END_MEM             (DRAM_END - 0x00200000)
#endif

//===========================================================================
// User defined Task IDs
// Standard tasks IDs are declared in OSCommon.h
//===========================================================================
typedef enum
{
    // First element must be STD_TSK_NUM
    KEYSCANTSK_ID = STD_TSK_NUM,        // KeyScan task
    COMMANDTSK_ID,                      // UART command task
    PLAYSOUNDTSK_ID,                    // Play sound task
    UIFRAMEWORKTSK_ID,                  // UIFramework task
    UIBACKGROUNDTSK_ID,                 // Backworker task
    MSDC_DBGSYS_TSK_ID,                 // USB debug task

    // The following two elements must be the last elements
    // Don't change the order
    USER_TSK_MAX_NUM,
    ENUM_DUMMY4WORD(USER_TSK)
} USER_TSK;

#define USER_TSK_NUM            (USER_TSK_MAX_NUM - STD_TSK_NUM)

//===========================================================================
// User defined Task Priority
// Standard tasks priority are declared in OSCommon.h
// Lower value higher priority
//===========================================================================
#define PRI_KEYSCAN              6
#define PRI_COMMAND              4
#define PRI_PLAYSOUND            2
#define PRI_UIFRAMEWORK         10
#define PRI_BACKWORKER          11
//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
#define PRI_MSDC_DBGSYS         10
//#NT#2009/09/09#Niven Cho -end

//===========================================================================
// User defined task Stack size
//===========================================================================
#define STKSIZE_KEYSCAN         2048
#define STKSIZE_COMMAND         8192//2012/02/23 Meg
#define STKSIZE_PLAYSOUND       2048
#define STKSIZE_UIFRAMEWORK     4096
#define STKSIZE_UIBACKGROUND    2048
//#NT#2009/09/09#Niven Cho -begin
//#NT#Added., for USB Debug System
#define STKSIZE_MSDC_DBGSYS     4096
//#NT#2009/09/09#Niven Cho -end

//===========================================================================
// User defined flag IDs
//===========================================================================
typedef enum
{
    // First element must be STD_EVF_NUM
    FLG_ID_KEY = STD_EVF_NUM,
    FLG_ID_KEY_REL,
    FLG_ID_KEY_CONT,
    FLG_ID_KEY_LONG,
    FLG_ID_SOUND,
    FLG_ID_FRAMEWORK,

    // The following two elements must be the last elements
    // Don't change the order
    USER_EVF_MAX_NUM,
    ENUM_DUMMY4WORD(USER_FLAG)
} USER_FLAG;

#define USER_EVF_NUM            (USER_EVF_MAX_NUM - STD_EVF_NUM)

//===========================================================================
//  User defined Semaphore IDs
//===========================================================================
typedef enum
{
    // First element must be STD_SEM_NUM
    SEMID_FRAMEWORK_WINDOW = STD_SEM_NUM,
    SEMID_FRAMEWORK_DRAW,
    SEMID_UIGRAPHIC,
    SEMID_MSDC_DBGYSYS,

    // The following two elements must be the last elements
    // Don't change the order
    USER_SEM_MAX_NUM,
    ENUM_DUMMY4WORD(USER_SEM)
} USER_SEM;

#define USER_SEM_NUM            (USER_SEM_MAX_NUM - STD_SEM_NUM)

//===========================================================================
//  User defined Mailbox IDs
//===========================================================================
typedef enum
{
    // First element must be STD_MBX_NUM
    MBXID_SYS_EVENT = STD_MBX_NUM,
    MBXID_BKG_EVENT,

    // The following two elements must be the last elements
    // Don't change the order
    USER_MBX_MAX_NUM,
    ENUM_DUMMY4WORD(USER_MBX)
} USER_MBX;

#define USER_MBX_NUM            (USER_MBX_MAX_NUM - STD_MBX_NUM)

//===========================================================================
// OSD width and height
//===========================================================================
#define OSD_W                   320
#define OSD_H                   240

//===========================================================================
// Fixed size memory pool IDs, Sizes and Counts
//===========================================================================
/****** Fixed Size Memory Pool IDs *******/
typedef enum
{
    // First element must be STD_FPL_NUM
    POOL_ID_DEFECT_PIXEL = STD_FPL_NUM,
    POOL_ID_OSD1,
    POOL_ID_OSDTEMP,
    POOL_ID_OSD2,
    POOL_ID_GX,
    POOL_ID_BG,
    POOL_ID_BGTEMP,
    POOL_ID_FTABLE,
    POOL_ID_OSD_DUMP,
    POOL_ID_IQ,
    POOL_ID_CAPTURE,

    // The following two elements must be the last elements
    // Don't change the order
    USER_FPL_MAX_NUM,
    ENUM_DUMMY4WORD(USER_MPOOL)
} USER_MPOOL;

#define USER_FPL_NUM            (USER_FPL_MAX_NUM - STD_FPL_NUM)

/****** Fixed Size Memory Pool Counts *******/
#define POOL_CNT_DEFECT_PIXEL   1
#define POOL_CNT_OSD1           1
#define POOL_CNT_OSD2           1
#define POOL_CNT_OSDTEMP        0
#define POOL_CNT_GX             1
#define POOL_CNT_BG             1
#define POOL_CNT_BGTEMP         1
#define POOL_CNT_FTABLE         1
#define POOL_CNT_IQ             1
#define POOL_CNT_CAPTURE        1
#if (OSD_DUMP==OSD_DUMP_ENABLE)
#define POOL_CNT_OSD_DUMP       1
#else
#define POOL_CNT_OSD_DUMP       0
#endif

/****** Fixed Size Memory Pool Sizes *******/
// Note: Pool size must be 32 bytes alignment
// Defect pixel
#define POOL_SIZE_DEFECT_PIXEL  SIZE_ALIGN32(0x14000)       // 80KB

// OSD
#if (_OSD_COLOR_ == _OSD_COLOR_8_BIT_)
#define POOL_SIZE_OSD           SIZE_ALIGN32(OSD_W*OSD_H)   // OSD -> 320*240 pixel, 8 bits for one pixel
#else
#define POOL_SIZE_OSD           SIZE_ALIGN32(OSD_W*OSD_H/2) // OSD -> 320*240 pixel, 4 bits for one pixel
#endif

// Gxlib
#define POOL_SIZE_GX            SIZE_ALIGN32(OSD_W*OSD_H*4) // at leat 4k for Gxlib

// BG
#define POOL_SIZE_BG            SIZE_ALIGN32(OSD_W*OSD_H*2)
#define POOL_SIZE_BGTEMP        SIZE_ALIGN32(POOL_SIZE_BG)

// MP3 File Table
#define POOL_SIZE_FTABLE        SIZE_ALIGN32(0x100000)      // 1MB is recommended for MP3 file table

// OSD dump buffer
#define POOL_SIZE_OSD_DUMP      SIZE_ALIGN32(0x100000)      // 1MB is recommended for OSD Dump buffer

// IQ
//#NT#2010/10/28#Connie Yu -begin
//#NT#update movie DIS
//#define POOL_SIZE_IQ            SIZE_ALIGN32(0x1800000)
#define POOL_SIZE_IQ            SIZE_ALIGN32(0x1820000)
//#NT#2010/10/28#Connie Yu -end

// Capture
#define POOL_SIZE_CAPTURE       ((END_MEM - FRE_MEM - POOL_SIZE_COMMON) -\
                                 (POOL_SIZE_DEFECT_PIXEL * POOL_CNT_DEFECT_PIXEL) -\
                                 (POOL_SIZE_OSD * (POOL_CNT_OSD1 + POOL_CNT_OSD2 + POOL_CNT_OSDTEMP)) -\
                                 (POOL_SIZE_GX * POOL_CNT_GX) -\
                                 (POOL_SIZE_BG * POOL_CNT_BG) -\
                                 (POOL_SIZE_BGTEMP * POOL_CNT_BGTEMP)-\
                                 (POOL_SIZE_FTABLE * POOL_CNT_FTABLE) -\
                                 (POOL_SIZE_OSD_DUMP * POOL_CNT_OSD_DUMP) -\
                                 (POOL_SIZE_IQ * POOL_CNT_IQ))

/****** For Project Only (Not Fixed Size) Memory Pool Sizes *******/
#if (_WAVSTUDIO_MODE_ == ENABLE)
  #if (_VM_TYPE_ == _VM_TYPE_SINGLE_)
    #define POOL_SIZE_WAV           0x00080000  // 512 KB
  #else
    #define POOL_SIZE_WAV           0x00400000  // 4 MB
  #endif
#else
    #define POOL_SIZE_WAV           0
#endif

#if (_RAM_DISK_ == ENABLE)
#define POOL_SIZE_RAM_DISK      0x200000    // RAM disk buffer size (1MB)
//#NT#2010/10/25#Philex - begin
// define to 0 because RAM2_DISK don't be necessary
#define POOL_SIZE_RAM2_DISK     0x000000    // RAM2 disk buffer size (1.125MB)
//#NT#2010/10/25#Philex - end
#else
#define POOL_SIZE_RAM_DISK      0x000000    // RAM disk buffer size (0MB)
#define POOL_SIZE_RAM2_DISK     0x000000    // RAM2 disk buffer size (0MB)
#endif

#if (_MOVIE_PIM_MODE_ == ENABLE)
// For 420 UV packed format, MaxWidth = 1440, MaxHeight = 1080
// MaxWidth * MaxHeight * 2 + 240KB (screennail, thumbnail) +
// JPEG bitstream (1280x720 = 300KB, 1440x1080 = 500KB)
#define POOL_SIZE_PIM           SIZE_ALIGN4_U(0x003B0600)
#endif

#if (_MP3_MODE_ == ENABLE)
#define POOL_SIZE_MP3           0x00480000  // MP3 decode buffer size
#define POOL_SIZE_MP3FONT       0x00500000  // MP3 unicode font data buffer size
#else
#define POOL_SIZE_MP3           0x00000000  // MP3 decode buffer size
#define POOL_SIZE_MP3FONT       0x00000000  // MP3 unicode font data buffer size
#endif

#endif
