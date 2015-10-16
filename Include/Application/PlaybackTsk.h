/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       PlaybackTsk.h
    @ingroup    mIAPPPlay

    @brief      All function for PlaybackTsk mode

    @note       Nothing.

    @version    V0.00.001
    @author     Penny Chen
    @date       2005/10/25
*/

/** \addtogroup mIAPPPlay */
//@{

#ifndef _PLAYBACKAPI_H
#define _PLAYBACKAPI_H

#include "Type.h"
#include "JpgDec.h"
#include "OSCommon.h"
#include "AppCommon.h"
#include "SlideEffect.h"
#include "FileDB.h"
#include "AVILib.h"
#include "MediaReadLib.h"

//-----------------------------------------------------
//#NT#2007/05/02#Scottie -begin
//#add for navigator function
typedef struct _PB_PAN_INFO
{
    UINT16  PanMaxX;            ///<  Pan mode, max X addr
    UINT16  PanMaxY;            ///<  Pan mode, max Y addr
    UINT16  PanSrcStartX;       ///<  Pan mode, current X addr
    UINT16  PanSrcStartY;       ///<  Pan mode, current Y addr
} PB_PAN_INFO, *PPB_PAN_INFO;
//#NT#2007/05/02#Scottie -end

typedef struct _PLAY_OBJ
{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(void);
    UINT32  (*Decode1stASFFunc)(UINT32 uiTempBufAddr, UINT32 uiTempBufSize, USHORT *pImageWidth, USHORT *pImageHeight,
                             UINT32 Decoded_Y_FrameAddr, UINT32 Decoded_U_FrameAddr, UINT32 Decoded_V_FrameAddr);
//#NT#2007/08/10#Corey Ke -begin
//#Let 1st mpeg1 decode func assigned from project
    UINT32  (*Decode1stMPEG1Func)(UINT32 uiTempBufAddr, UINT32 uiTempBufSize, USHORT *pImageWidth, USHORT *pImageHeight,
                             UINT32 Decoded_Y_FrameAddr, UINT32 Decoded_U_FrameAddr, UINT32 Decoded_V_FrameAddr);
//#NT#2007/08/10#Corey Ke -end
    UINT32  uiDeviceWidth;
    UINT32  uiDeviceHeight;
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
    UINT32  ui1stPlayMode;

    UINT16  usPlayFileFormat;
//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#if 0
    UINT8   *uiMoveImgSpeedTable;           //speed table
#endif
    UINT32  uiMoveImgDelayTime;
//#NT#2009/06/03#Corey Ke -end
    UINT8   Orientation;                    ///   PLAY_HORIZONTAL / PLAY_VERTICAL
    UINT32  Flags;                          ///   bit 0 : PB_ENABLE_TRANSFROM or not
                                            ///   bit 1 : PB_ENABLE_SHOW_BACKGROUND_IN_THUMB or not
                                            ///   bit 2 : PB_ENABLE_SEARCH_FILE_WITHOUT_DCF or not
    FILEDB_HANDLE       FileHandle;         ///   File handle to communicate with file DB

    UINT8   uiThumbShowImageMode;           ///   PB_SHOW_THUMB_IMAGE_ONE_BY_ONE / PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME
    UINT32  uiPanelWidth;                   ///   Real panel width after panel scaling
    UINT32  uiPanelHeight;                  ///   Real panel height after panel scaling
    BOOL    bEnableAutoRotate;              ///   TRUE: Auto-rotate image according to EXIF-Orientation
    RECT    ThumbOnlyRect;                  ///   Show thumb only jpeg in specific area
    BOOL    bDisplayLastImg;                ///   TRUE: Auto-display the last image after starting Playback task
//#NT#2007/05/02#Scottie -begin
//#add for navigator function
    PPB_PAN_INFO    pstPanInfo;             ///< pointer for Playback Pan info object
//#NT#2007/05/02#Scottie -end

//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
    UINT32  uiMovieBGAddr;
    UINT32  uiMovieBGSize;
    UINT32  uiBadFileBGAddr;
    UINT32  uiBadFileBGSize;
    UINT32  uiWaveBGAddr;
    UINT32  uiWaveBGSize;
#if 0
    UINT32  uiMovieBGAddrY;                 ///< image Y buffer address for movie background
    UINT32  uiMovieBGAddrCb;                ///< image Cb buffer address for movie background
    UINT32  uiMovieBGAddrCr;                ///< image Cr buffer address for movie background
//#NT#2007/05/16#Scottie -begin
//#Support showing bad file warning image
    UINT32  uiBadFileBGAddrY;               ///< image Y buffer address for bad file background
    UINT32  uiBadFileBGAddrCb;              ///< image Cb buffer address for bad file background
    UINT32  uiBadFileBGAddrCr;              ///< image Cr buffer address for bad file background
//#NT#2007/05/16#Scottie -end
//#NT#2009/01/14#Scottie -begin
//#NT#Add for drawing the wav sample image
    UINT32  uiWavFileBGAddrY;               ///< image Y buffer address for wav file background
    UINT32  uiWavFileBGAddrCb;              ///< image Cb buffer address for wav file background
    UINT32  uiWavFileBGAddrCr;              ///< image Cr buffer address for wav file background
//#NT#2009/01/14#Scottie -end
//#NT#2007/09/20#Scottie -begin
    UINT32  uiMemFB2Addr;                   ///< memory pool FB2 buffer address
//#NT#2007/09/20#Scottie -end
#endif
//#NT#2009/10/20#Corey Ke -end
//#NT#2009/06/10#Corey Ke -begin
//#NT#Support for H.264
    void    (*Decode1stMediaFunc)(MEDIA_PLAY_1ST_INFO *pMediaInfo);
//#NT#2009/06/10#Corey Ke -end
} PLAY_OBJ, *PPLAY_OBJ;

// Structure for play single image command object
typedef struct
{
    UINT8   PlayCommand;            // PB_SINGLE_CURR / PB_SINGLE_NEXT / PB_SINGLE_PREV / PB_SINGLE_PRIMARY
    UINT8   MoveImgMode;            // Move window effect, refer to PLAY_MOVWIN_XXXX
    UINT8   JumpOffset;             // The JumpOffset of read next / previous file
} PLAY_SINGLE_OBJ, *PPLAY_SINGLE_OBJ;

// Structure for browser (thumbnail) mode command object
typedef struct
{
    UINT8   BrowserCommand;         // PB_BROWSER_CURR / PB_BROWSER_NEXT / PB_BROWSER_PREV
    UINT8   HorNums;                // How many images in horizontal direction, HorNums * VerNums must <=25
    UINT8   VerNums;                // How many images in vertical direction, HorNums * VerNums must <=25
    UINT8   MoveImgMode;            // Move window effect, refer to SLIDE_EFFECT_XXXX (SlideEffect.h)
    UINT8   JumpOffset;             // The JumpOffset of read next / previous file
    PRECT   pRectBrowser;           // Pointer of the rect-array of browser files
    BOOL    bReDecodeImages;        // Used to re-decode all thumb images or not, TRUE / FALSE
                                    // if JumpOffset is still in the same page, set it as FALSE,
                                    // otherwise it means playback has to decode all images in another page, set it as TRUE
} PLAY_BROWSER_OBJ, *PPLAY_BROWSER_OBJ;


//#Structure for show specific file in video1/video2 and FB buf/tmpFB and spec size
typedef struct
{
    UINT8   PlayFileBuf;            // PLAY_SPEC_FILE_IN_FRAME_BUFFER / PLAY_SPEC_FILE_IN_TMP_FRAME_BUFFER
    UINT8   PlayFileVideo;          // PLAY_SPEC_FILE_IN_VIDEO_1 / PLAY_SPEC_FILE_IN_VIDEO_2
    UINT8   PlayFileClearBuf;       // PLAY_SPEC_FILE_WITH_CLEAR_BUF / PLAY_SPEC_FILE_WITH_NOT_CLEAR_BUF
    RECT    PlayRect;               // Pointer of the rect-array of file
} PLAY_SPECFILE_OBJ, *PPLAY_SPECFILE_OBJ;

//#NT#2007/05/10#Corey Ke -begin
//#Support to save crop image
typedef struct
{
    UINT32   DisplayStart_X;        // crop display start x
    UINT32   DisplayStart_Y;        // crop display start y
    UINT32   DisplayWidth;          // crop display width
    UINT32   DisplayHeight;         // crop display height
    BOOL     IfOverWrite;           // TRUE / FALSE, use to overwrite or not
} PLAY_CROPSAVE_OBJ, *PPLAY_CROPSAVE_OBJ;
//#NT#2007/05/10#Corey Ke -end

//#NT#2007/06/05#Corey Ke -begin
//#Support copy to card/namd with all/specific files
//#NT#2008/12/30#Meg Lin -begin
//#modify fileFormat to UINT16
typedef struct
{
    UINT16   FileId;                // Copy file Id, only apply when PLAY_COPY_SPECIFIC_FILE
    UINT16   DirId;                 // Copy dir Id, only apply when PLAY_COPY_SPECIFIC_FILE
    UINT16   FileFormat;            // Copy file format, only apply when PLAY_COPY_SPECIFIC_FILE, PLAYFMT_JPG / PLAYFMT_AVI / PLAYFMT_ASF / PLAYFMT_WAV
    UINT8    CopyToDist;            // PLAY_COPY_TO_CARD / PLAY_COPY_TO_NAMD
    UINT8    CopyMode;              // PB_COPY2CARD_NEW_DIR / PB_COPY2CARD_APPEND / PB_COPY2CARD_NO_RENAME / PB_COPY2CARD_NEW_DIR_ORG_FILE
    UINT8    CopyFiles;             // PLAY_COPY_ALL_FILES / PLAY_COPY_SPECIFIC_FILE
} PLAY_COPYTO_OBJ, *PPLAY_COPYTO_OBJ;
//#NT#2008/12/30#Meg Lin -end
//#NT#2007/06/05#Corey Ke -end

//#NT#2009/12/07#Corey Ke -begin
//#NT#Unify resize API
typedef enum {
    PLAY_RESIZE_NEW_FILE            = 0,
    PLAY_RESIZE_OVERWRITE           = 1,
    PLAY_RESIZE_SAVE_NONE           = 2,
    ENUM_DUMMY4WORD(PB_RESIZE_TYPE)
} PB_RESIZE_TYPE;


typedef struct
{
    UINT32           NewWidth;
    UINT32           NewHeight;
    PB_RESIZE_TYPE   SaveType;
    UINT32           FileSize;
} PLAY_RESIZE_OBJ, *PPLAY_RESIZE_OBJ;
//#NT#2009/12/07#Corey Ke -end

//#NT#2011/06/30#Hideo Lin -begin
//#NT#Add for set opening logo function
typedef enum {
    PLAY_LOGO_QVGA,
    PLAY_LOGO_VGA
} PLAY_LOGO_SIZE;

typedef struct
{
    PLAY_LOGO_SIZE  LogoSize;       // Logo size
    UINT32  uiJpegAddress;          // Resize JPEG bitstream address
    UINT32  uiJpegSize;             // Resize JPEG bitstream size
} PLAY_RESIZE_LOGO, *PPLAY_RESIZE_LOGO;
//#NT#2011/06/30#Hideo Lin -end

typedef void (*SliderCb)(void);

//-------------------------------------------------------------------------

// File format
#define PLAYFMT_UNKNOWN                 0x0000
#define PLAYFMT_JPG                     0x0001
#define PLAYFMT_WAV                     0x0002
#define PLAYFMT_AVI                     0x0004
#define PLAYFMT_ASF                     0x0008
#define PLAYFMT_RAW                     0x0010
#define PLAYFMT_TIF                     0x0020
#define PLAYFMT_JPGMEMO                 0x0010
#define PLAYFMT_DPOF                    0x0020
#define PLAYFMT_READONLY                0x0040
#define PLAYFMT_MPG                     0x0080
//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
#define PLAYFMT_MOVMJPG                 0x0100
//#NT#2007/10/17#Meg Lin -end
// Wait time
#define PB_WAIT_NOWAIT                  0x00
#define PB_WAIT_INFINITE                0xFF

// Rotate image
#define PLAY_ROTATE_DIR_UNKNOWN         0xFF
#define PLAY_ROTATE_DIR_0               PLAY_ROTATE_DIR_UNKNOWN
#define PLAY_ROTATE_DIR_90              0x00
#define PLAY_ROTATE_DIR_180             0x01
#define PLAY_ROTATE_DIR_270             0x02
#define PLAY_ROTATE_DIR_HOR             0x03
#define PLAY_ROTATE_DIR_VER             0x04
//#NT#2009/11/03#Corey Ke -begin
//#NT#support rotate transpose & transverse
#define PLAY_ROTATE_DIR_TSP             0x05
#define PLAY_ROTATE_DIR_TSV             0x06
//#NT#2009/11/03#Corey Ke -end

//#NT#2009/01/15#Scottie -begin
//#NT#Support image enhancing functions
// Image enhance
#define PB_IMGENH_BLACKWHITE            0x01
#define PB_IMGENH_SEPIA                 0x02
#define PB_IMGENH_NEGATIVE              0x03
//#NT#2009/01/15#Scottie -end


/**
    JPEG Exif-Orientation Tag
*/
typedef enum {
    PLAY_EXIF_ORI_DEFAULT         = 1,  ///<1, Default orientation
    PLAY_EXIF_ORI_FLIP_HORIZONTAL = 2,  ///<2, flip horizontal
    PLAY_EXIF_ORI_ROTATE_180      = 3,  ///<3, rotate 180 deg
    PLAY_EXIF_ORI_FLIP_VERTICAL   = 4,  ///<4, flip vertical
    PLAY_EXIF_ORI_TRANSPOSE       = 5,  ///<5, transpose
    PLAY_EXIF_ORI_ROTATE_90       = 6,  ///<6, rotate 90 deg
    PLAY_EXIF_ORI_TRANSVERSE      = 7,  ///<7, transverse
    PLAY_EXIF_ORI_ROTATE_270      = 8   ///<8, rotate 270 deg
} PB_JPEG_EXIF_ORI;

// DPOF
#define PLAYDPOF_OPEN                   0x00000001
#define PLAYDPOF_SETONE                 0x00000002
#define PLAYDPOF_SETALL                 0x00000004
#define PLAYDPOF_RESETALL               0x00000008
#define PLAYDPOF_CLOSE                  0x00000010
#define PLAYDPOF_GETONEQTY              0x00000020
#define PLAYDPOF_GETONEDATE             0x00000040
#define PLAYDPOF_GETALL                 0x00000080
#define PLAYDPOF_SETONE_DATEONLY        0x00000100
#define PLAYDPOF_SETONE_QTYONLY         0x00000200
#define PLAYDPOF_SETREV                 0x00000400
#define PLAYDPOF_SETCRT                 0x00000800
#define PLAYDPOF_UPDATE                 0x00001000
#define PLAYDPOF_RELOAD                 0x00002000

//#NT#2009/11/27#Lily Kao -begin
//Merge MintonZ930 DPOF all-index,paper-size
#define PLAYDPOF_SETIDXALL              0x00004000
#define PLAYDPOF_RESETIDXALL            0x00008000
//#NT#2009/07/16#Chris Hsu -begin
//#NT#Add set / get print size and get index API.
#define PLAYDPOF_SETPRINTSIZEALL        0x00010000
#define PLAYDPOF_GETPRINTSIZEALL        0x00020000
#define PLAYDPOF_GETIDX                 0x00040000
//#NT#2009/07/16#Chris Hsu -end
//#NT#2009/11/27#Lily Kao -end

//#NT#2009/11/17#SA4(Hideo Lin) -begin
//#NT#Added for user data (ex. social network tag, favorite tag)
#define PLAY_USERDATA_SOCIALNETWORK     0x01
#define PLAY_USERDATA_FAVORITE          0x02

#define PLAY_SOCIALNETWORK_SETONE       0x01 // set current image/movie
#define PLAY_SOCIALNETWORK_RESETONE     0x02 // reset current image/movie
#define PLAY_SOCIALNETWORK_SETMULTI     0x04 // set selected images/movies
#define PLAY_SOCIALNETWORK_RESETMULTI   0x08 // reset selected images/movies
#define PLAY_SOCIALNETWORK_SETALL       0x10 // set all images/movies
#define PLAY_SOCIALNETWORK_RESETALL     0x20 // reset all images/movies

#define PLAY_FAVORITE_ONE               0x01
#define PLAY_FAVORITE_MULTI             0x02
#define PLAY_UNFAVORITE_ONE             0x04
#define PLAY_UNFAVORITE_MULTI           0x08
//#NT#2009/11/17#SA4(Hideo Lin) -end

// Protect file
#define PLAY_PROTECT_ONE                0x01
#define PLAY_PROTECT_ALL                0x02
#define PLAY_UNPROTECT_ONE              0x04
#define PLAY_UNPROTECT_ALL              0x08

// Format storage
#define PLAY_FORMAT_CARD                0x01
#define PLAY_FORMAT_NAND                0x02

// Photo display orientation
#define PLAY_HORIZONTAL                 0x01
#define PLAY_VERTICAL                   0x02

// Display mode
#define PLAY_SINGLE                     0x01
#define PLAY_SLIDE_SHOW                 0x02

#define PB_ENABLE_TRANSFROM                     0x00000001
#define PB_DISABLE_TRANSFROM                    0x00000000
#define PB_ENABLE_SHOW_BACKGROUND_IN_THUMB      0x00000002
#define PB_DISABLE_SHOW_BACKGROUND_IN_THUMB     0x00000000
#define PB_ENABLE_SEARCH_FILE_WITHOUT_DCF       0x00000004
#define PB_DISABLE_SEARCH_FILE_WITHOUT_DCF      0x00000000
#define PB_ENABLE_SHOW_BG_IN_MOVIE              0x00000008
#define PB_DISABLE_SHOW_BG_IN_MOVIE             0x00000000
//#NT#2007/05/17#Scottie -begin
//#Support showing bad file warning image
#define PB_ENABLE_SHOW_BG_IN_BADFILE            0x00000010
//#NT#2007/05/17#Scottie -end
//#NT#2007/06/15#Corey Ke -begin
//#Support play files sort by date
#define PB_ENABLE_PLAY_FILE_BY_DATE             0x00000020
//#NT#2007/06/15#Corey Ke -end
//#NT#2007/06/26#Corey Ke -begin
//#Support to play thumb in different page when different folder
#define PB_ENABLE_THUMB_WITH_DIFF_FOLDER        0x00000040  /// This flag should not co-exist with PB_ENABLE_PLAY_FILE_BY_DATE
//#NT#2007/06/26#Corey Ke -end
//#NT#2009/01/14#Scottie -begin
//#NT#Add for drawing the wav sample image
#define PB_ENABLE_SHOW_BG_IN_WAVFILE            0x00000080
//#NT#2009/01/14#Scottie -end
//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#define PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF       0x00000100
//#NT#2009/06/03#Corey Ke -end
//#NT#2009/12/10#Corey Ke -begin
//#NT#Add decode primary without hidden mechanism
#define PB_ENABLE_FORCE_TO_DECODE_PRIMARY       0x00000200
//#NT#2009/12/10#Corey Ke -end
//#NT#2009/12/21#Corey Ke -begin
//#NT#Add thumb fill black flag
#define PB_ENABLE_THUMB_FILL_BLACK              0x00000400
//#NT#2009/12/10#Corey Ke -end


#define PB_SHOW_THUMB_IMAGE_ONE_BY_ONE          0x00000001
#define PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME    0x00000002

// Copy to card mode
#define PB_COPY2CARD_NEW_DIR            0x00
#define PB_COPY2CARD_APPEND             0x01
#define PB_COPY2CARD_NO_RENAME          0x02
#define PB_COPY2CARD_NEW_DIR_ORG_FILE   0x03

#define PB_PANEL_NO_SCALING             0   // For panel real width / heigh is equal to panel win_width / win_height

#define PB_THUMB_ONLY_DEFAULT_SETTING   0
//-----------------------------------------------------
// New playback task status
//-----------------------------------------------------
#define PB_STA_UNKNOWN                  0x00000000
#define PB_STA_NOIMAGE                  0x00000001
#define PB_STA_ERR_FILE                 0x00000002
#define PB_STA_ERR_DECODE               0x00000004
#define PB_STA_ERR_WRITE                0x00000008
#define PB_STA_DONE                     0x00000010
#define PB_STA_BUSY                     0x00000020
#define PB_STA_STANDBY                  0x00000040
#define PB_STA_INITFAIL                 0x00000080

//-----------------------------------------------------
// New playback command param.
//-----------------------------------------------------
#define PB_SINGLE_CURR                  0x01
#define PB_SINGLE_NEXT                  0x02
#define PB_SINGLE_PREV                  0x04
#define PB_SINGLE_PRIMARY               0x08
#define PB_SINGLE_SLIDE_START           0x10
#define PB_SINGLE_SLIDE_STOP            0x20
#define PB_SINGLE_THUMB                 0x40


#define PB_BROWSER_CURR                 0x01
#define PB_BROWSER_NEXT                 0x02
#define PB_BROWSER_PREV                 0x04
//#NT#2007/05/18#Scottie -begin
//#To decode thumbnail only, or return decode error.
#define PB_BROWSER_THUMB                0x40
//#NT#2007/05/18#Scottie -end

#define PB_DELETE_ONE                   0x01
#define PB_DELETE_ALL                   0x02

#define PLAYZOOM_IN                     0x01
#define PLAYZOOM_OUT                    0x02
#define PLAYZOOM_UP                     0x04
#define PLAYZOOM_DOWN                   0x08
#define PLAYZOOM_LEFT                   0x10
#define PLAYZOOM_RIGHT                  0x20
#define PLAYZOOM_USER                   0x40

#define PB_PICBRIDGE_ON                 0x55
#define PB_PICBRIDGE_OFF                0

#define PLAYMODE_USERJPGOUT             77
#define PLAYMODE_QVIEW                  66      // show first image from SDRAM Q-view buffer

#define PLAYMODE_PICBRIDGE              55
#define PLAYMODE_ONLYTHUMB              33      // first decode always decode only thumbnail
#define PLAYMODE_UNKNOWN                99      // first decode the max-file-id file
#define PLAYMODE_PRIMARY                10
#define PLAYMODE_AVI                    11
#define PLAYMODE_ROTATE                 12
#define PLAYMODE_WAV                    13
#define PLAYMODE_ASF                    14
#define PLAYMODE_MPG                    16

#define PLAYMODE_THUMB                  9
#define PLAYMODE_SLIDESHOW              15
//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
#define PLAYMODE_MOVMJPG                17
//#NT#2007/10/17#Meg Lin -end

#define PLAYFILEINFO_FILEID             1
#define PLAYFILEINFO_FILESIZE           2
#define PLAYFILEINFO_FILEFORMAT         3
#define PLAYFILEINFO_FILESEQ            4
#define PLAYFILEINFO_FILENUMS           5
#define PLAYFILEINFO_DIRNUMS            6
#define PLAYFILEINFO_MAXDIRID           7
#define PLAYFILEINFO_DIRID              8
#define PLAYFILEINFO_FILENUMS_INDIR     9
#define PLAYFILEINFO_FILESEQ_INDIR      10
#define PLAYFILEINFO_MAXFILEID_INDIR    11
#define PLAYFILEINFO_ISFILEREADONLY     12
#define PLAYFILEINFO_ISWITHMENO         13

#define PLAY_USER_JPEG_IN_FRAME_BUFFER      1
#define PLAY_USER_JPEG_IN_TMP_FRAME_BUFFER  2

// Show specific file in video1/video2 and FB buf/tmpFB and spec size
#define PLAY_SPEC_FILE_IN_FRAME_BUFFER      0
#define PLAY_SPEC_FILE_IN_TMP_FRAME_BUFFER  1

#define PLAY_SPEC_FILE_IN_VIDEO_1           0
#define PLAY_SPEC_FILE_IN_VIDEO_2           1

#define PLAY_SPEC_FILE_WITH_CLEAR_BUF       0
#define PLAY_SPEC_FILE_WITH_NOT_CLEAR_BUF   1

//#NT#2007/06/05#Corey Ke -begin
//#Support copy to card/namd with all/specific files
#define PLAY_COPY_TO_CARD                   0
#define PLAY_COPY_TO_NAMD                   1

#define PLAY_COPY_ALL_FILES                 0
#define PLAY_COPY_SPECIFIC_FILE             1
//#NT#2007/06/05#Corey Ke -end

//#NT#2007/06/15#Corey Ke -begin
//#Support play files sort by date
#define PLAY_FILE_ALL                       0
#define PLAY_FILE_BY_DAY                    1
#define PLAY_FILE_BY_MONTH                  2
#define PLAY_FILE_BY_YEAR                   3
//#NT#2007/06/15#Corey Ke -end

//#NT#2007/06/15#Corey Ke -begin
//#Support play files sort by date
#define PB_GET_DAY_FROM_DATE(x)             (x & 0x1F)
#define PB_GET_MONTH_FROM_DATE(x)           ((x >> 5) & 0x0F)
#define PB_GET_YEAR_FROM_DATE(x)            ((x >> 9) & 0x7F)+1980
//#NT#2007/06/15#Corey Ke -end

//-----------------------------------------------------------------------------------
extern INT32    PB_Open(PPLAY_OBJ pPlayObj);
extern INT32    PB_Close(UINT32 TimeOut);
extern UINT32   PB_GetStatus(void);
//----- play image op command -------------------------------------------------------------------
extern void     PB_PlaySingleMode(PPLAY_SINGLE_OBJ pPlaySingleObj);
extern UINT32   PB_PlayBrowserMode(PPLAY_BROWSER_OBJ pPlayBrowserObj);
extern void     PB_PlayZoomMode(UINT32 PlayCommand);
extern UINT32   PB_PlayZoomPanMode(UINT32 PlayCommand, UINT32 MoveOffset);
extern void     PB_PlayDeleteMode(UINT32 PlayCommand);
extern void     PB_PlaySliderModeInit(UINT32 PlayCommand, UINT32 SliderTime, SliderCb);
extern void     PB_PlaySliderMode(UINT32 PlayCommand, UINT32 MoveImgMode);
//#NT#2007/06/25#Corey Ke -begin
//#Support play files sort by date
extern UINT32   PB_PlayByDateInit(void);
extern void     PB_PlayByDateUnInit(void);
extern UINT16   PB_GetTotalPlayDay(void);
extern UINT16   PB_GetFirstPlayDay(void);
extern UINT16   PB_GetLastPlayDay(void);
extern UINT16   PB_GetNextPlayDay(void);
extern UINT16   PB_GetPrevPlayDay(void);
extern UINT16   PB_GetCurrPlayDay(void);
extern UINT16   PB_GetCurrPlayDaySeq(void);
extern UINT16   PB_GetCurrPlayYearSeq(void);
extern UINT16   PB_GetCurrPlayDaySeqInThisYear(void);
extern UINT16   PB_GetPlayDaysNuminYear(UINT16 SpecYear);
extern UINT16   PB_GetTotalPlayMonth(void);
extern UINT16   PB_GetFirstPlayMonth(void);
extern UINT16   PB_GetNextPlayMonth(void);
extern UINT16   PB_GetPrevPlayMonth(void);
extern UINT16   PB_GetTotalPlayYear(void);
extern UINT16   PB_GetFirstPlayYear(void);
extern UINT16   PB_GetNextPlayYear(void);
extern UINT16   PB_GetPrevPlayYear(void);
extern UINT16   PB_GetTotalFilesNumOfSpecDate(void);
extern void     PB_SetPlayDate(UINT32 DateType, UINT16 Date);
//#NT#2007/06/25#Corey Ke -end

//#NT#2009/12/16#Corey Ke -begin
//#NT#Add playback support file format mechanism
extern void     PB_SetPlayFormat(UINT32 supportFormat);
//#NT#2009/12/16#Corey Ke -end

extern void     PB_SetDPOFOpenClose(UINT32 DpofOPMode);
extern void     PB_SetCurrDPOFQty(UINT32 DpofOPMode, UINT32 DpofPrtNum, UINT32 DpofDateOn);
extern void     PB_SetProtectFile(UINT32 ProtectMode);

//#NT#2009/12/07#Corey Ke -begin
//#NT#Unify resize API

#if 0
extern void     PB_SetReSizeFile(UINT32 NewWidth, UINT32 NewHeight, UINT32 IfOverWrite);
#else
#define PB_SetReSizeFile(newWidth, newHeight, ifOverWrite) \
{                                                        \
    PLAY_RESIZE_OBJ PlayResizeObj;                       \
    PlayResizeObj.NewWidth = newWidth;                   \
    PlayResizeObj.NewHeight = newHeight;                 \
    PlayResizeObj.SaveType = ifOverWrite;                \
    PB_SetReSize(&PlayResizeObj);                        \
}
#endif
//#NT#2009/12/07#Corey Ke -end

//#NT#2009/12/07#Corey Ke -begin
//#NT#Unify resize API
extern void     PB_SetReSize(PPLAY_RESIZE_OBJ pPlayResizeObj);
//#NT#2009/12/07#Corey Ke -begin
extern void     PB_SetReQtyFile(UINT32 NewQty, UINT32 IfOverWrite);
extern void     PB_SetRotateCurrFile(UINT32 RotateDir, UINT32 IfOverWrite);
//#NT#2007/12/26#Scottie -begin
//#Add bIfOverWrite parameter
//extern void     PB_SetEXIF_Orientation(PB_JPEG_EXIF_ORI EXIFOri, BOOL bDecodeIMG);
extern void     PB_SetEXIF_Orientation(PB_JPEG_EXIF_ORI EXIFOri, BOOL bDecodeIMG, BOOL bIfOverWrite);
//#NT#2007/12/26#Scottie -end
extern UINT32   PB_SetCopy2Card(UINT8 ucCopyMode);
//#NT#2007/06/05#Corey Ke -begin
//#Support copy to card/namd with all/specific files
extern UINT32   PB_SetCopyTo(PPLAY_COPYTO_OBJ pPlayCopyToObj);
//#NT#2007/06/05#Corey Ke -end
extern void     PB_SetIMEIDESize(PIMEIDE_ATTR  pIMEIDEAttr);
extern INT32    PB_WaitCommandFinish(UINT32 TimeOut);
extern INT32    PB_SetFormatStorage(UINT32 StrgType);
extern INT32    PB_SetRotateDisplay(UINT32 RotateDir);
extern INT32    PB_SetReArrangeFileID(void);
extern void     PB_PlayUserJPEG(UINT32 SrcAddr, UINT32 SrcSize, RECT OutRect, UINT8 DstBuf);
extern void     PB_PlaySpecFile(PPLAY_SPECFILE_OBJ pPlayFileObj);
extern void     PB_1stVideoFrameInit(void);
extern void     PB_Set1stVideoFrame(UINT16 uiDstLU_X, UINT16 uiDstLU_Y, UINT16 uiDstSZ_W, UINT16 uiDstSZ_H);
//#NT#2009/01/15#Scottie -begin
//#NT#Support image enhancing functions
extern INT32    PB_SetImgEnhDisplay(UINT8 ucEnhOption);
extern INT32    PB_SetImgEnhCurrFile(UINT8 ucEnhOption, UINT32 IfOverWrite);
//#NT#2009/01/15#Scottie -end

//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
extern void     PB_SetMovieBGInfo(UINT32 movieBSAddr, UINT32 movieBSSize);
extern void     PB_SetBadFileBGInfo(UINT32 badFileBSAddr, UINT32 badFileBSSize);
extern void     PB_SetWaveBGInfo(UINT32 waveBSAddr, UINT32 waveBSSize);
//#NT#2009/10/20#Corey Ke -end

//#NT#2009/03/19#Photon Lin -begin
//#Get frame buffer
extern void     PB_GetCurr3IMETempFBAddr(UINT32 *pFBAddrY, UINT32 *pFBAddrCb, UINT32 *pFBAddrCr);
//#NT#2009/03/19#Photon Lin -end

//#NT#2009/11/25#Corey Ke -begin
//#NT#Support user resize for UI usage
extern void     PB_GetUserResizeAddrAndSize(UINT32 *startAddress, UINT32 *size);
//#NT#2009/11/25#Corey Ke -end
//----- get info. -------------------------------------------------------------------
extern UINT32   PB_GetFILELock(void);
extern UINT32   PB_GetFILEWithMemo(void);
extern UINT32   PB_GetCurrMode(void);
extern UINT32   PB_GetCurFileIndex(void);
extern UINT32   PB_GetCurrZoomIndex(void);
extern UINT32   PB_GetCurFileNameId(void);
extern UINT32   PB_GetCurFileSeqId(void);
extern UINT32   PB_GetCurFileFormat(void);
extern UINT32   PB_GetCurFileDirId(void);
extern UINT32   PB_GetCurThumbNums(void);
extern UINT32   PB_GetSlideInterval(void);
extern PJPGHEAD_DEC_CFG     PB_GetCurrImageInfo(void);
extern PJPGHEAD_DEC_CFG     PB_GetCurrImageInfoBeforeDec(BOOL bParseEXIF);
extern UINT16   *PB_GetAllThumbDirIdArray(void);
extern UINT16   *PB_GetAllThumbFileIdArray(void);
extern UINT16   *PB_GetAllThumbFileFormatArray(void);
//#NT#2007/05/18#Scottie -begin
//#Add a API to get AllThumb decode result for showing error message
extern BOOL     *PB_GetAllThumbDecErrArray(void);
//#NT#2007/05/18#Scottie -end
extern UINT16   *PB_GetAllThumbSeqIdArray(void);
extern UINT32   PB_GetCurrDPOFQty(void);
extern UINT32   PB_GetCurrDPOFDateOn(void);
extern UINT32   PB_GetCurrDPOFAll(void);
extern UINT32   PB_GetCurrFileSysInfo(UINT32 WhichInfo);
extern PAVIHEAD_INFO    PB_GetCurrAVIInfo(void);
extern PRECT    PB_GetThumbRect(void);
extern UINT32   PB_GetRotateDir(void);
extern PPLAY_OBJ    PB_GetPBObject(void);
//----- for AVI get info ---------------------------------------------
extern UINT32   PB_GetCurrDataBufAddr(void);
extern UINT32   PB_GetCurrDataBufSize(void);
extern void     PB_GetCurr3FBAddr(UINT32 *pFBAddrY, UINT32 *pFBAddrCb, UINT32 *pFBAddrCr);
extern void     PB_GetCurr3TempFBAddr(UINT32 *pFBAddrY, UINT32 *pFBAddrCb, UINT32 *pFBAddrCr);
//----- setting -------------------------------------
//#NT#2007/05/22#Scottie -begin
//#Increase the precision of Zoom scale
//extern void     PB_SetZoomLevelTbl(UINT8 *pZoomLevelTbl);
extern void     PB_SetZoomLevelTbl(UINT32 *pZoomLevelTbl);
//#NT#2007/05/22#Scottie -end
extern void     PB_SetThumbRect(PRECT pRectDrawThumb);
extern void     PB_SetSlideInterval(UINT32 SlideShowInt);
//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#if 0
extern void     PB_SetSlideSpeedTable(UINT8 *SpeedTable);
#endif
//#NT#2009/06/03#Corey Ke -end
extern void     PB_SetPictureBridgeON(UINT32 IfEnable);
extern INT32    PB_SetPBObject(PPLAY_OBJ pPlayObj);

//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
extern void     PB_EnableFeature(UINT32 PlayFeature);
extern void     PB_DisableFeature(UINT32 PlayFeature);
//#NT#2009/06/03#Corey Ke -end

//----- others ----------------------------------------
extern void     PB_ReadImage(UINT32, INT8 *, UINT32, UINT32, UINT32);
extern void     PB_OpenSpecFile(UINT32 PlayDirId, UINT32 PlayFileId);
//#NT#2007/07/18#Scottie -begin
//#Add bOnlyOpen parameter to decide if open file then read some data or only query file
//extern void     PB_OpenSpecFileBySeq(UINT32 uiSeqID);
extern void     PB_OpenSpecFileBySeq(UINT32 uiSeqID, BOOL bOnlyQuery);
//#NT#2007/07/18#Scottie -end
//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
#if 0
extern void     PB_SetReSizeBuffer(UINT32 uiAddr, UINT32 uiSize);
#endif
//#NT#2009/10/20#Corey Ke -end

extern void     PB_SetCurrDPOFQtyOnly(UINT32 DpofPrtNum);
extern void     PB_SetCurrDPOFDateOnly(UINT32 DpofDateOn, char *pDscStr);
extern UINT8    PB_DPOFGetCreatorName(char * pRetVal, INT8 length);
extern UINT8    PB_DPOFGetRevision(char * pRetVal, INT8 length);
extern void     PB_DPOFSetDSCFormat(UINT8 dscFormat, UINT8 updateNow);
extern UINT8    PB_DPOFGetDSCFormat(void);
extern void     PB_DPOFSetDSCDateFormat(UINT8 dateFormat, UINT8 updateNow);
extern UINT8    PB_DPOFGetDSCDateFormat(void);
extern UINT8    PB_DPOFSetRevision(char * pRetVal, INT8 length, UINT8 updateNow);
extern UINT8    PB_DPOFSetCreatorName(char * pRetVal, INT8 length, UINT8 updateNow);
extern UINT8    PB_DPOFSave(void);
extern UINT8    PB_DPOFCancel(void);
extern UINT16   PB_SetCurrDPOFQtyDate(UINT32 DpofPrtNum, UINT32 DpofDateOn, char *pDscStr);
extern UINT8    PB_SetCurrDPOFQtyOnly2(UINT32 DpofPrtNum);
extern UINT8    PB_SetCurrDPOFDateOnly2(UINT32 DpofDateOn, char *pDscStr);
extern UINT16   PB_SetCurrDPOFQtyDate3(UINT32 DpofPrtNum, UINT32 DpofDateOn);//not in VSS; check only
//set dpof cropping
extern UINT8    PB_SetCurrDPOFCrop(UINT16 cropX, UINT16 cropY, UINT16 cropW, UINT16 cropH);
//#NT#2009/11/27#Lily Kao -begin
//Merge MintonZ930 DPOF all-index,paper-size
extern void     PB_SetIdxAllDPOFQty(UINT32 DpofOPMode);
//#NT#2009/07/16#Chris Hsu -begin
//#NT#Add set / get print size and get index API.
extern void     PB_SetDPOFAllPrintSize(UINT32 uiPrintSize);
extern UINT32   PB_GetDPOFAllPrintSize(void);
extern UINT32   PB_GetCurrDPOFIdx(void);
//#NT#2009/07/16#Chris Hsu -end
//#NT#2009/11/27#Lily Kao -end

//#NT#2009/11/17#SA4(Hideo Lin) -begin
//#NT#Added for user data (ex. social network tag, favorite tag)
extern UINT32   PB_GetSocialNetworkValue(void);
extern UINT32   PB_GetJpgSocialNetworkValue(void);
extern UINT32   PB_GetMovSocialNetworkValue(void);
extern void     PB_SetSocialNetworkValue(UINT32 uiSetMode, UINT32 uiValue);
extern INT32    PB_UpdateEXIFSocialNetworkValue(UINT32 uiDirectoryID, UINT32 uiFileNameID, USHORT uwValue);
extern INT32    PB_SetMovSocialNetworkValue(UINT32 uiDirectoryID, UINT32 uiFileNameID, UINT32 uiValue);
extern UINT16   *PB_GetAllThumbSocialNetworkArray(void);
extern BOOL     PB_CompareMovUserDataIdentify(UINT8 *pucIdentify);
//#NT#2009/11/17#SA4(Hideo Lin) -end

//#NT#2009/11/05#SA4 -begin
//#NT#For HD500C3 WAV file, When HDMI out, it has a progress bar of playing time on video layer
//#NT#and the other is on OSD layer. The video layer is 4:3 and the OSD is 16:9, so
//#NT#the width of progress bar on OSD layer will be large than video layer.
extern void PB_Scale2DisplayDisableRatioTrans(BOOL bRatioTrans);
extern BOOL PB_Scale2DisplayGetRatioTransBoolean(void);
//#NT#2009/11/05#SA4 -end


#endif

//@}
