#ifndef UI_CFG_DEFAULT_H
#define UI_CFG_DEFAULT_H

#include "Type.h"
#include "SysCfg.h"
#include "UIMenuPhotoSettings.h"
#include "UIMenuMovieSettings.h"
#include "UIMenuPlaySettings.h"
#include "UIMenuPrintSettings.h"
#include "UIMenuSetupSettings.h"
#include "UISystemStatusSettings.h"


//-----------------------------------------------------------------------------
// System
//-----------------------------------------------------------------------------
//#define UIDFT_DSC_MODE                  DSC_MODE_PHOTO_AUTO
//#define UIDFT_DSC_MODE_TRANS_PARAM      DSCMODE_SWITCH_FORCE
//#define UIDFT_DSC_MODE_TRANS_DIR        DSCMODE_SWITCHDIR_DONT_CARE
#define UIDFT_FS_STATUS                 FS_NOT_INIT
#define UIDFT_CARD_STATUS               CARD_REMOVED
#define UIDFT_BATTERY_LVL               0
#define UIDFT_MP3FTB_RENEW              MP3FTB_RENEW_ON

#if (UI_STYLE == UI_STYLE_DVPLUS)
//-----------------------------------------------------------------------------
// Setup
//-----------------------------------------------------------------------------
#define UIDFT_VOLUME                    VOLUME_LVL_2
//#define UIDFT_SHUTTER_TONE              TONE_TYPE_1
//#define UIDFT_KEY_TONE                  TONE_TYPE_1
//#define UIDFT_SELFTIME_TONE             TONE_TYPE_1
//#define UIDFT_POWER_TONE                TONE_TYPE_1
#define UIDFT_BEEP                      BEEP_ON
#define UIDFT_LANGUAGE                  ENGLISH
#define UIDFT_TV_MODE                   TV_MODE_NTSC
#define UIDFT_LCDOFF                    LCDOFF_DISABLED
#define UIDFT_POWEROFF                  POWEROFF_5MIN
#define UIDFT_USBMODE                   USB_MODE_MSDC
#define UIDFT_DATE_FORMAT               DATE_FORMAT_YMD
#define UIDFT_TIME_FORMAT               TIME_FORMAT_24HOURS
#define UIDFT_FREQUENCY                 FREQ_50HZ
#define UIDFT_LCDBRIGHTNESS             LCDBRT_LVL_01
#define UIDFT_DISPLAY                   DISPLAY_NORMAL
#define UIDFT_OPENING                   OPENING_OFF

//-----------------------------------------------------------------------------
// Photo
//-----------------------------------------------------------------------------
#define UIDFT_PHOTO_SIZE                SIZE_1M
#define UIDFT_QUALITY                   QUALITY_BEST
#define UIDFT_AFWINDOW                  AFWINDOW_SINGLE
#define UIDFT_AFBEAM                    AFBEAM_ON
#define UIDFT_METERING                  METERING_CENTER_WEIGHTED
#define UIDFT_QREVIEW                   QREVIEW_2SEC
#define UIDFT_HISTOGRAM                 HISTOGRAM_OFF
#define UIDFT_DATEIMPRINT               DATEIMPRINT_OFF
#define UIDFT_COLOR                     COLOR_STANDARD
#define UIDFT_ISO                       ISO_AUTO
#define UIDFT_EV                        EV_00
#define UIDFT_WB                        WB_AUTO
#define UIDFT_SHARPNESS                 SHARPNESS_NORMAL
#define UIDFT_SATURATION                SATURATION_NORNAL
#define UIDFT_CONTRAST                  CONTRAST_NORMAL
#define UIDFT_SCENE                     SCENE_AUTO
#define UIDFT_SELFTIMER                 SELFTIMER_OFF
#define UIDFT_FLASH                     FLASH_OFF
#define UIDFT_MACRO                     MACRO_OFF
#define UIDFT_CAPTUREMODE               CAPTURE_MODE_SINGLE
#define UIDFT_FD                        FD_OFF
#define UIDFT_ANTISHAKING               ANTISHAKING_OFF
#define UIDFT_CONTSHOT                  CONT_SHOT_OFF

//-----------------------------------------------------------------------------
// Movie
//-----------------------------------------------------------------------------
#define UIDFT_MOVIE_SIZE                MOVIE_SIZE_720P
#define UIDFT_MOVIE_QUALITY             MOVIE_QUALITY_FINE
#define UIDFT_MOVIE_COLOR               MOVIE_COLOR_STANDARD
#define UIDFT_MOVIE_DZOOM               MOVIE_DZOOM_10X
#define UIDFT_MOVIE_SELFTIMER           MOVIE_SELFTIMER_OFF
#define UIDFT_MOVIE_FPS                 MOVIE_FPS_30
#define UIDFT_MOVIE_DIS                 MOVIE_DIS_OFF
#define UIDFT_MOVIE_DATEIMPRINT         MOVIE_DATEIMPRINT_OFF
#define UIDFT_MOVIE_CYCLIC_REC          MOVIE_CYCLIC_REC_OFF
#define UIDFT_MOVIE_GOLF_SHOT           MOVIE_GOLFSHOT_OFF
#define UIDFT_MOVIE_FLASH_REC           MOVIE_FLASHREC_OFF
#elif (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
//-----------------------------------------------------------------------------
// Setup
//-----------------------------------------------------------------------------
#define UIDFT_VOLUME                    VOLUME_LVL_2
#define UIDFT_BEEP                      BEEP_ON
#define UIDFT_LANGUAGE                  SCHINESE//ENGLISH
#define UIDFT_TV_MODE                   TV_MODE_NTSC
#define UIDFT_LCDOFF                    LCDOFF_DISABLED
#define UIDFT_POWEROFF                  POWEROFF_DISABLED
#define UIDFT_USBMODE                   USB_MODE_MSDC
#define UIDFT_DATE_FORMAT               DATE_FORMAT_DMY
#define UIDFT_TIME_FORMAT               TIME_FORMAT_24HOURS
#define UIDFT_FREQUENCY                 FREQ_50HZ
#define UIDFT_LCDBRIGHTNESS             LCDBRT_LVL_01
#define UIDFT_DISPLAY                   DISPLAY_NORMAL
#define UIDFT_OPENING                   OPENING_OFF
#define UIDFT_LCD_OFF              LCDOFF_1MIN
#define UIDFT_DELAYPOWEROFF              DELAY_POWER_OFF_15S
#define UIDFT_IMAGEROTATE              IMAGE_ROTATE_OFF

//-----------------------------------------------------------------------------
// Photo
//-----------------------------------------------------------------------------
#define UIDFT_PHOTO_SIZE                SIZE_1M
#define UIDFT_QUALITY                   QUALITY_BEST
#define UIDFT_AFWINDOW                  AFWINDOW_SINGLE
#define UIDFT_AFBEAM                    AFBEAM_ON
#define UIDFT_METERING                  METERING_CENTER_WEIGHTED
#define UIDFT_QREVIEW                   QREVIEW_0SEC
#define UIDFT_HISTOGRAM                 HISTOGRAM_OFF
#define UIDFT_DATEIMPRINT               DATEIMPRINT_DATE_TIME
#define UIDFT_COLOR                     COLOR_STANDARD
#define UIDFT_ISO                       ISO_AUTO
#define UIDFT_EV                        EV_00
#define UIDFT_WB                        WB_AUTO
#define UIDFT_SHARPNESS                 SHARPNESS_NORMAL
#define UIDFT_SATURATION                SATURATION_NORNAL
#define UIDFT_CONTRAST                  CONTRAST_NORMAL
#define UIDFT_SCENE                     SCENE_AUTO
#define UIDFT_SELFTIMER                 SELFTIMER_OFF
#define UIDFT_FLASH                     FLASH_OFF
#define UIDFT_MACRO                     MACRO_OFF
#define UIDFT_CAPTUREMODE               CAPTURE_MODE_SINGLE
#define UIDFT_FD                        FD_OFF
#define UIDFT_ANTISHAKING               ANTISHAKING_ON
#define UIDFT_CONTSHOT                  CONT_SHOT_OFF

//-----------------------------------------------------------------------------
// Movie
//-----------------------------------------------------------------------------
#define UIDFT_MOVIE_SIZE                MOVIE_SIZE_720P
#define UIDFT_MOVIE_QUALITY             MOVIE_QUALITY_FINE
#define UIDFT_MOVIE_COLOR               MOVIE_COLOR_STANDARD
#define UIDFT_MOVIE_DZOOM               MOVIE_DZOOM_10X
#define UIDFT_MOVIE_SELFTIMER           MOVIE_SELFTIMER_OFF
#define UIDFT_MOVIE_FPS                 MOVIE_FPS_30
#define UIDFT_MOVIE_DIS                 MOVIE_DIS_OFF
#define UIDFT_MOVIE_DATEIMPRINT         MOVIE_DATEIMPRINT_ON
#define UIDFT_MOVIE_CYCLIC_REC          MOVIE_CYCLIC_REC_5MIN
#define UIDFT_MOVIE_GOLF_SHOT           MOVIE_GOLFSHOT_OFF
#define UIDFT_MOVIE_FLASH_REC           MOVIE_FLASHREC_OFF
#define UIDFT_MOVIE_AUDIO               MOVIE_AUDIO_ON
#define UIDFT_MOVIE_DATESTAMP           MOVIE_DATEIMPRINT_ON
#define UIDFT_MOVIE_MOTIONDET           MOVIE_MOTIONDET_OFF
#define UIDFT_MOVIE_GODBLESS            MOVIE_GODBLESS_OFF
#define UIDFT_MOVIE_GSENSOR             MOVIE_GSENSOR_MED
#else
#error UI_STYLE not defined.
#endif

//-----------------------------------------------------------------------------
// Playback
//-----------------------------------------------------------------------------
#define UIDFT_SLIDE_INTERVAL            SLIDE_3SEC
#define UIDFT_DPOF_PRT_NUM              0
#define UIDFT_PLAYMOV_SPEED             PLAYMOV_SPEED_FWD_1X

//-----------------------------------------------------------------------------
// Printer
//-----------------------------------------------------------------------------
#define UIDFT_PTR_SIZE                  PSIZE_DEFAULT
#define UIDFT_PTR_TYPE                  PLAYOUT_DEFAULT
#define UIDFT_PTR_DATE                  PDATE_ON
#define UIDFT_PTR_MODE                  PMODE_SINGLE
#define UIDFT_PTR_ALL_NUM               0


#endif
