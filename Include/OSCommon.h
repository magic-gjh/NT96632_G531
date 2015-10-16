/**
    Header file of OSCommon.c

    Common Define, to sync the defination bewteen Application/Middle layer/Driver,
    we create this header file to put our common define that should be the same
    value in all files.

    @file       OSCommon.h
    @ingroup
    @note       Don't Modify the file.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _OSCOMMON_H
#define _OSCOMMON_H

#include "Type.h"
#include "Kernel.h"
#include "Debug.h"

/**
    @addtogroup
*/
//@{

/**
    @name   Task Configuration Error code

    Definition for API: void OSCommon_CfgModuleTask(PTASKINFO pTaskInfo)
*/
//@{
#define TASKCFG_OK          0X00000000      ///< T_CTSK Task: configure OK.
#define TASKCFG_ERRSTACK    0XFFFFFFFF      ///< T_CTSK Task: stack error.
#define TASKCFG_ERROCCUPIED 0XFFFFFFFE      ///< T_CTSK Task: task id error(duplicate).
#define TASKCFG_ERRFUNEMPTY 0XFFFFFFFD      ///< T_CTSK Task: task body is empty.
#define TASKCFG_ERRID       0XFFFFFFFC      ///< T_CTSK Task: task id overflow.
#define TASKCFG_ERRPRI      0XFFFFFFFB      ///< T_CTSK Task: task priority error
#define TASKCFG_ERRPARA     0XFFFFFFFA      ///< T_CTSK Task: parameter is NULL.
//@}

/**
    Task Configuration Infomation
*/
typedef struct _TaskInfo_
{
    T_CTSK Task;            ///< Task Configuration settings.
    CHAR*  TaskName;        ///< Task name.(for Debuging)

}TASKINFO,*PTASKINFO;

extern void OSCommon_CfgModuleTask(PTASKINFO pTaskInfo);
extern void OSCommon_CfgUserTask(PTASKINFO pTaskInfo);


// For Debuging(Internal using, don't reference it)
// Statck checking tag. If you modify this value, please re-build Debug module
#define STACK_CHECK_TAG         0xDEADBEEF
extern CHAR* tasknametable[];
//===========================================================================
//
//===========================================================================

#define INT_VEC_NUM             64              // Total number of hardware interrupt sources

//===========================================================================
// Standard Task IDs
//===========================================================================
typedef enum
{
// 00
    TASK_ID_00,             ///< Don't use this ID
// Mandatory Tasks
    INITSK_ID,              ///< Allocate to system init task.
    FILESYSTSK_ID,          ///< Allocate to file system task.
    //#NT#2010/05/14#Janice Huang -begin
    //#NT#Add fs background task
    FSBACKGROUNDTSK_ID,     ///< Allocate to the background task of file system
    //#NT#2010/05/14#Janice Huang -end
    PRIMARYTSK_ID,          ///< Allocate to primary task(for Modules flow control)
// Playback
    PLAYBAKTSK_ID,          ///< Allocate to playback application task.
//IQ
    PHOTO1STTSK_ID,         ///< Allocate to 1st photo application task.
    IMGSLICETSK_ID,         ///< Allocate to image slice application task.
    SIECAPTSK_ID,           ///< Allocate to sie capture application task.
// USB
    USBCTRLTSK_ID,          ///< Allocate to USB subsystem control library.(USB driver)
    USBSETUPTSK_ID,         ///< Allocate to USB subsystem setup library.(USB driver)
// 10
    USBSTRGTSK_ID,          ///< Allocate to MSDC USB subsystem library.(mass storage device class)
    USBPCCVIDEOTSK_ID,      ///< Allocate to PCC USB subsystem library.(PC camera device class)
    USBSICDTSK_ID,          ///< Allocate to SICD USB subsystem library.(still image device class)
    USBSICDEP3TSK_ID,       ///< Allocate to SICD USB subsystem library.(still image device class)
// AVI
    AVIRECMANAGERTSK_ID,    ///< Allocate to AVI Rec & Dec Application.
    AVIPLAYMANAGERTSK_ID,   ///< Allocate to AVI Rec & Dec Application.
    PLAYAUDIOTSK_ID,        ///< Allocate to AVI Rec & Dec Application.
    PLAYVIDEOTSK_ID,        ///< Allocate to AVI Rec & Dec Application.
    RECVIDEOTSK_ID,         ///< Allocate to AVI Rec & Dec Application.
// MP3
    MP3PLAYTSK_ID,          ///< Allocate to MP3 decoing application.
// 20
    MP3PLAYFILETSK_ID,      ///< Allocate to MP3 decoing application.
// WAV
    WAVSTUDIOTSK_ID,        ///< Allocate to wav studio application.
// Media Framework
    MEDIARECTSK_ID,         ///< Allocate to Media Framework application.
    MEDIAPLAYTSK_ID,        ///< Allocate to Media Framework application.
    MEDIAVIDEORECTSK_ID,    ///< Allocate to Media Framework application.
    MEDIAVIDEOPLAYTSK_ID,   ///< Allocate to Media Framework application.
    MEDIAAUDIOPLAYTSK_ID,   ///< Allocate to Media Framework application.
// GPS
    GPSRECEIVE_ID,          ///< Allocate to GPS Reciver application.
//IQ2
    PHOTO2NDTSK_ID,         ///< Allocate to 2nd photo application task.
    PHOTO3RDTSK_ID,         ///< Allocate to 3rd photo application task.
// 30
    PHOTODISPLAYTSK_ID,     ///< Allocate to photo display application task.
// Lens Control
    LENSCTRLTSK_ID,         ///< Allocate to photo display application task.
//#NT#2010/03/18#SA4(Randy) -begin
    LENSCTRLTSK2_ID,
//#NT#2010/03/18#SA4(Randy) -end
    RAWENC_ID,              ///< Allocate to raw encoding task.
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    PHOTODISTSK_ID,         ///< Allocate to photo DIS application task
    //#NT#2009/11/27#Connie Yu -end
    //#NT#2010/04/26#Klins Chen -begin
    HDMICECTSK_ID,
    //#NT#2010/04/26#Klins Chen -end
    MEDIAAUDIOENCTSK_ID,    ///< Allocate to Media Framework application.
    MEDIARECFSTSK_ID,       //2010/12/15 Meg Lin
    MEDIARECCMDTSK_ID,      //2010/12/15 Meg Lin
    // The following two elements must be the last elements
    // Don't change the order
    MEDIARECAACTESTTSK_ID, //2012/02/23 Meg
    STD_TSK_NUM,        ///< Total number of Standard Task
    ENUM_DUMMY4WORD(STD_TASK)
} STD_TASK;

//===========================================================================
// Standard Task Priority
// User defined tasks Priority are declared in SysCfg.h
// Lower value higher priority
//===========================================================================
// Mandatory Tasks
#define PRI_INIT                 2
#define PRI_FILESYS             10
#define PRI_PRIMARY              4

// Playback
#define PRI_PLAYBACK            12

// IQ
#define PRI_1STPHOTO            16
#define PRI_2NDPHOTO            17
#define PRI_3RDPHOTO            18
#define PRI_IMGSLICE             6
#define PRI_SIECAP               4
#define PRI_DISPLAYPHOTO        16
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
#define PRI_DISPHOTO             8
//#NT#2009/11/27#Connie Yu -end

// USB
#define PRI_USBCTRL             12
#define PRI_USBSETUP            10
#define PRI_USBSTRG              8
#define PRI_USBPCCVIDEO         PRI_USBSETUP
//#NT#2009/09/24#SA4 -begin
#define PRI_USBSICD             10
#define PRI_USBSICDEP3           8
//#NT#2009/09/24#SA4 -end

// AVI
#define PRI_AVIRECMANAGER       12
#define PRI_AVIPLAYMANAGER      18
#define PRI_PLAYAUDIO            6
#define PRI_PLAYVIDEO            8
#define PRI_RECVIDEO             8

// MP3
#define PRI_MP3PLAY             11   //10   //Note: Must be lower than UI task to ok parsing total time!
#define PRI_MP3PLAYFILE         12

// WAV
#define PRI_WAVSTUDIO            8

//Media Framework
#define PRI_MEDIAREC            8 //12 //2010/12/15 Meg Lin
#define PRI_MEDIAVIDEOREC       6 //8  //2010/12/15 Meg Lin
#define PRI_MEDIAAUDIOENC       7 //9  //2010/12/15 Meg Lin
#define PRI_MEDIARECFS          9 //2010/12/15 Meg Lin
#define PRI_MEDIARECCMD         7 //2010/12/15 Meg Lin
#define PRI_MEDIAPLAY           18
#define PRI_MEDIAVIDEOPLAY      8
#define PRI_MEDIAAUDIOPLAY      6

//GPS
#define PRI_GPSRECEIVE          6

//#NT#2010/04/26#Klins Chen -begin
//CEC
#define PRI_CEC                 18
//#NT#2010/04/26#Klins Chen -end

//#NT#2009/09/24#SA4 -begin
// Lens Control
#define PRI_LENSCTRL            1
#define PRI_LENSCTRL2           1
//#NT#2009/09/24#SA4 -end
#define PRI_RAWENC              16
//#NT#2010/05/14#Janice Huang -begin
//#NT#Add fs background task
#define PRI_FSBACKGROUND         3
//#NT#2010/05/14#Janice Huang -end

//===========================================================================
// Standard Task Stack size
//===========================================================================
#define STKSIZE_DUMMY               4

// Mandatory Task
#define STKSIZE_INIT                1024
#define STKSIZE_FILESYS             4096
#define STKSIZE_PRIMARY             2048

// Playback
#define STKSIZE_PLAYBACK            4096

// IQ
#define STKSIZE_1STPHOTO            1024
//#NT#2009/10/30#SA4 -begin
#define STKSIZE_2NDPHOTO            2048
//#NT#2009/10/30#Chiyi Lin -begin
#define STKSIZE_3RDPHOTO            4096
//#NT#2009/10/30#Chiyi Lin -end
//#NT#2009/10/30#SA4 -end
#define STKSIZE_IMGSLICETSK         2048
#define STKSIZE_SIECAPTSK           2048
#define STKSIZE_DISPLAYPHOTO        2048
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
#define STKSIZE_DISPHOTO            1024
//#NT#2009/11/27#Connie Yu -end

// USB
#define STKSIZE_USBCTRL             1024
#define STKSIZE_USBSETUP            2048
#define STKSIZE_USBSTRGVND          1024
#define STKSIZE_USBPCCAMVIDEO       2048
//#NT#2009/09/24#SA4 -begin
#define STKSIZE_USBSICDVND          1024
#define STKSIZE_USBSICDEP3          512
//#NT#2009/09/24#SA4 -end

// AVI
#define STKSIZE_AVIRECMANAGER       1024
#define STKSIZE_AVIPLAYMANAGER      1024
#define STKSIZE_PLAYAUDIO           1024
#define STKSIZE_PLAYVIDEO           1024
#define STKSIZE_RECAUDIO            1024
#define STKSIZE_RECVIDEO            1024

// MP3
#define STKSIZE_MP3PLAY             8192
#define STKSIZE_MP3PLAYFILESYS      2048

// WAV
#define STKSIZE_WAVSTUDIO           1024

// Media Framework
#define STKSIZE_MEDIAREC            2048
#define STKSIZE_MEDIAPLAY           2048
//#NT#2010/02/02#Meg Lin -begin
#define STKSIZE_MEDIAVIDEOREC       2048//1024
#define STKSIZE_MEDIAAUDIOENC       8196
#define STKSIZE_MEDIAVIDEOPLAY      2048//1024
//#NT#2009/02/02#Meg Lin -end
#define STKSIZE_MEDIAAUDIOPLAY      1024
#define STKSIZE_MEDIARECFS          4096 //2010/12/15 Meg Lin
#define STKSIZE_MEDIARECCMD         2048 //2010/12/15 Meg Lin

// GPS
#define STKSIZE_GPSRECEIVE          1024

//#NT#2010/04/26#Klins Chen -begin
// CEC
#define STKSIZE_CEC                 1024
//#NT#2010/04/26#Klins Chen -end

//#NT#2009/09/21#Chris Chung -begin
#define STKSIZE_LENSCTRL         1024
//#NT#2010/03/18#SA4 -begin
#define STKSIZE_LENSCTRL2        1024
//#NT#2010/03/18#SA4 -end
//#NT#2009/09/21#Chris Chung -end
// Raw encode
#define STKSIZE_RAWENC              1024
//#NT#2010/05/14#Janice Huang -begin
//#NT#Add fs background task
#define STKSIZE_FSBACKGROUND    1024
//#NT#2010/05/14#Janice Huang -end

//===========================================================================
// Standard Flag ID value symbols
// User defined flag IDs are declared in SysCfg.h
//===========================================================================
typedef enum
{
// 00
    FLG_ID_INT,             ///< Interrupt for driver
    FLG_ID_GPIO,            ///< GPIO Interupt
    FLG_ID_FILESYS,         ///< File system task
    FLG_ID_PSTORE,          ///< PStore
    FLG_ID_STIMER,          ///< Soft timer
    FLG_ID_PHOTO,           ///< For photo task
    FLG_ID_SLICE,           ///< For slice mode
    FLG_ID_PLAYBACK,        ///< Playback task
    FLG_ID_AVI,             ///< AVI
    FLG_ID_MEDIA,           ///< Media Framework
// 10
    FLG_ID_RECMEDIA,        ///< Media Framework
    FLG_ID_USBTSK,          ///< USB INT
    FLG_ID_DPS,             ///< For DPS task
    FLG_ID_PRIMARY,         ///< Primary task
    FLG_ID_WAV,             ///< Wave studio task
    FLG_ID_MP3,             ///< MP3
    FLG_ID_GPS,             ///< GPS
    FLG_ID_INT2,            ///< Interrupt 2 for driver
    FLG_ID_IPL,             ///< image pipeline driver
    FLG_ID_USBCLASS,        ///< USB device class (SICD/MSDC/PCC)
// 20
    FLG_ID_PHOTODISPLAY,    ///< For photo display task
    FLG_ID_EP3TSK,          ///< USB EP3 task
    FLG_ID_LENS,            ///< Lens Control
    FLG_ID_RAWENC,          ///< Raw Encoding task
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    FLG_ID_PHOTODIS,        ///< Photo DIS task
    //#NT#2009/11/27#Connie Yu -end
    //#NT#2010/04/26#Klins Chen -begin
    FLG_ID_HDMICEC,         ///< HDMI-CEC task
    //#NT#2010/04/26#Klins Chen -end
    //#NT#2010/05/14#Janice Huang -begin
    //#NT#Add fs background task
    FLG_ID_FSBACKGROUND,
    //#NT#2010/05/14#Janice Huang -end

    // The following two elements must be the last elements
    // Don't change the order
    STD_EVF_NUM,        // Total number of Standard Flag
    ENUM_DUMMY4WORD(STD_FLAG)
} STD_FLAG;

//===========================================================================
//  Standard Semaphore IDs
//===========================================================================
typedef enum
{
// 00
    SEMID_PSBM,
    SEMID_JPEG,
    SEMID_H264,
    SEMID_CRYPT,
    SEMID_BMC,
    SEMID_AMBA_DMA,
    SEMID_SIF,
    SEMID_I2C,
    SEMID_SPI,
    SEMID_IDE,
// 10
    SEMID_HDMI,
    SEMID_DAI,
    SEMID_CF,
    SEMID_SDIO,
    SEMID_SDIO2,
    SEMID_XDNAND,
    SEMID_MS,
    SEMID_USB,
    SEMID_UART,
    SEMID_UART2,
// 20
    SEMID_RTC,
    SEMID_TIMER,
    SEMID_WDT,
    SEMID_PWM_0,
    SEMID_PWM_1,
    SEMID_PWM_2,
    SEMID_PWM_3,
    SEMID_PWM_4,
    SEMID_PWM_5,
    SEMID_PWM_6,
// 30
    SEMID_PWM_7,
    SEMID_PWM_CCNT,
    SEMID_ADC_0,
    SEMID_ADC_1,
    SEMID_ADC_2,
    SEMID_ADC_3,
    SEMID_ADC_4,
    SEMID_ADC_5,
    SEMID_ADC_6,
    SEMID_ADC_7,
// 40
    SEMID_GRAPHIC,
    SEMID_REMOTE,
    SEMID_SIE,
    SEMID_PRE,
    SEMID_IPE,
    SEMID_IME,
    SEMID_DIS,
    SEMID_FDE,
    SEMID_KNLTIMER,
    SEMID_SICD,
// 50
    //#NT#2010/05/14#Janice Huang -begin
    //#NT#Add fs background task
    SEMID_FSBACKGROUND,
    //#NT#2010/05/14#Janice Huang -end
    //#NT#2011/10/11#HH Chuang -begin
    //#NT#Add virtual SDIO driver
    SEMID_VIRTUAL_SDIO,
    SEMID_VIRTUAL_SDIO2,
    //#NT#2012/03/04#HH Chuang -begin
    //#NT#Add virtual SDIO driver
    SEMID_VIRTUAL_SDIO3,
    //#NT#2012/03/04#HH Chuang -end
    //#NT#2011/10/11#HH Chuang -end

    // The following two elements must be the last elements
    // Don't change the order
    STD_SEM_NUM,        // Total number of Standard Semaphore
    ENUM_DUMMY4WORD(STD_SEM)
} STD_SEM;

//===========================================================================
//  Standard Mailbox IDs
//===========================================================================
typedef enum
{
// 00
    MBXID_UART_RX,
    MBXID_UART_TX,

    // The following two elements must be the last elements
    // Don't change the order
    STD_MBX_NUM,        // Total number of Standard Mailbox
    ENUM_DUMMY4WORD(STD_MBX)
} STD_MBX;

#define BUF_NUM                 255

//===========================================================================
//  Fixed size memory pool IDs, Sizes and Counts
//===========================================================================
// ID
typedef enum
{
// 00
    POOL_ID_FS_SECTOR,
    POOL_ID_FS_FAT,
    POOL_ID_STORAGE_SMC,
    POOL_ID_SICD,

    // The following two elements must be the last elements
    // Don't change the order
    STD_FPL_NUM,        // Total number of Fixed size memory pool
    ENUM_DUMMY4WORD(STD_MPOOL)
} STD_MPOOL;

// Size
// Note: Pool size must be 32 bytes alignment
#define POOL_SIZE_FS_SECTOR     SIZE_ALIGN32_U(0x8020)      // 32,800 bytes (512*62+4+28, 28 for 32 bytes align)
#define POOL_SIZE_FS_FAT        SIZE_ALIGN32_U(0x80020)     // 131,104 bytes (131072+8+24, 24 for 32 bytes align)//2010/12/15 Meg Lin
#define POOL_SIZE_STORAGE_SMC   SIZE_ALIGN32_U(0x4000)      // 16384 bytes
//#NT#2009/09/24#SA4 -begin
#define POOL_SIZE_SICD          SIZE_ALIGN32_U(0x100000)    // 1 MB
#define POOL_SIZE_COMMON        ((POOL_SIZE_FS_SECTOR * POOL_CNT_FS_SECTOR) +\
                                 (POOL_SIZE_FS_FAT * POOL_CNT_FS_FAT) +\
                                 (POOL_SIZE_STORAGE_SMC * POOL_CNT_STORAGE_SMC) +\
                                 (POOL_SIZE_SICD * POOL_CNT_SICD))
//#NT#2009/09/24#SA4 -end

// Count
#define POOL_CNT_FS_SECTOR      2
#define POOL_CNT_FS_FAT         1
#define POOL_CNT_STORAGE_SMC    1
//#NT#2009/09/24#SA4 -begin
#define POOL_CNT_SICD           1
//#NT#2009/09/24#SA4 -end

//===========================================================================
// Modified uITRON RTOS data structure
//===========================================================================

typedef struct _SYSTEM_EVF_INFO_
{
    UB          evfid;          // Event Flag ID
    //UINT        value;        // Event Flag Value
} SYSTEM_EVF_TABLE;

typedef struct _SYSTEM_MBX_INFO_
{
    UB          mbxid;          // Mail Box ID
} SYSTEM_MBX_TABLE;

extern UCHAR pucStkDummy[];
extern void DummyTsk(void);

//@}

#endif
