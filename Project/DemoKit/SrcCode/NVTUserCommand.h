#ifndef NVT_USER_COMMAND_H
#define NVT_USER_COMMAND_H
#include "UIFramework.h"

typedef UINT32 NVTEVT;
typedef UINT32 (* GETINDEX_PTR)(void);

enum
{
    NVTEVT_KEY_EVT_START    = NVTEVT_SYSCMD_MAX,
    NVTEVT_KEY_UP           = NVTEVT_KEY_EVT_START,
    NVTEVT_KEY_DOWN,        //11
    NVTEVT_KEY_LEFT,
    NVTEVT_KEY_RIGHT,
    NVTEVT_KEY_ENTER,
    NVTEVT_KEY_TELE,
    NVTEVT_KEY_WIDE,
    NVTEVT_KEY_POWER,
    NVTEVT_KEY_DEL,
    NVTEVT_KEY_ZOOMIN,
    NVTEVT_KEY_ZOOMOUT,
    NVTEVT_KEY_SHUTTER1,
    NVTEVT_KEY_SHUTTER2,
    NVTEVT_KEY_MODE,
    NVTEVT_KEY_PLAYBACK,
    NVTEVT_KEY_MENU,
    NVTEVT_KEY_CUSTOM1,
    NVTEVT_KEY_CUSTOM2,
    NVTEVT_KEY_DISPLAY,
    NVTEVT_KEY_MAX  //29
};

typedef enum
{
    NVTEVT_USERCMD_START    =     NVTEVT_KEY_MAX + 1,       //30
    NVTEVT_TIMER            = NVTEVT_USERCMD_START,
    NVTEVT_BATTERY,
    NVTEVT_CALLBACK,
    NVTEVT_STORAGE_CHANGE,
    NVTEVT_STORAGE_INIT,
    NVTEVT_MACRO_CHANGE,
    NVTEVT_PLAYRETURN,
    NVTEVT_CAPTURE,
    NVTEVT_CAPTURE_STOP,
    NVTEVT_SMARTSCENECHECK,
    NVTEVT_PLAYINIT,
    NVTEVT_PLAYREINIT,
    NVTEVT_PLAYNEXT,
    NVTEVT_PLAYPREV,
    NVTEVT_PLAYCURR,
    NVTEVT_MOVPLAY,
    NVTEVT_MOVSTOP,
    NVTEVT_MOVPAUSE,
    NVTEVT_MOVRESUME,
    NVTEVT_MOVFWD,
    NVTEVT_MOVBWD,
    NVTEVT_MOVFWDSTOP,
    NVTEVT_MOVBWDSTOP,
    NVTEVT_WAVPLAY,
    NVTEVT_WAVSTOP,
    NVTEVT_WAVPAUSE,
    NVTEVT_WAVRESUME,
    NVTEVT_WAVRECORD,
    NVTEVT_WAVRECORDSTOP,
    NVTEVT_WAVRECORDPASUE,
    NVTEVT_WAVRECORDRESUME,
    NVTEVT_AUD_VOLUME,
    NVTEVT_ZOOMINIT,
    NVTEVT_ZOOMIN,
    NVTEVT_ZOOMOUT,
    NVTEVT_ZOOMSTOP,
    NVTEVT_ZOOMNAVI,
    NVTEVT_SLIDEINIT,
    NVTEVT_SLIDEKEEP,
    NVTEVT_SLIDESTOP,
    //#NT#2009/04/13#Lily Kao -begin
    NVTEVT_START_RECAVI,
    NVTEVT_STOP_RECAVI,
    NVTEVT_INIT_PLAYAVI,
    NVTEVT_CLOSE_PLAYAVI,
    NVTEVT_FWDPLAY_PLAYAVI,
    NVTEVT_BWDPLAY_PLAYAVI,
    NVTEVT_FWDSTEPPLAY_PLAYAVI,
    NVTEVT_BWDSTEPPLAY_PLAYAVI,
    NVTEVT_PAUSE_PLAYAVI,
    //#NT#2009/04/13#Lily Kao -end
    NVTEVT_PRINT_INIT,
    NVTEVT_PRINT_CLOSE,
    NVTEVT_PRINT_START_PRINT,
    NVTEVT_PRINT_START_ALL,
    NVTEVT_PRINT_START_INDEX,
    NVTEVT_PRINT_START_DPOF,
    NVTEVT_PRINT_STOP_PRINT,
    NVTEVT_PRINT_CONTINUE_PRINT,
    NVTEVT_PRINT_CONTINUE_DPOF,
    NVTEVT_PRINT_GET_CAPABILITY_ALL,
    NVTEVT_PRINT_GET_CAPABILITY_PAPERTYPE,
    NVTEVT_PRINT_GET_CAPABILITY_LAYOUT,
    NVTEVT_PRINT_SET_JOB_CONFIG,
    NVTEVT_CHANGE_DSCMODE,
    NVTEVT_FORCETO_LIVEVIEW_MODE,
    NVTEVT_FORCETO_PLAYBACK_MODE,
    NVTEVT_FLASHCHARGING,
    NVTEVT_FLASHCHARGING_PROC,
    NVTEVT_FLASHCHARGING_END,
    NVTEVT_SAVE_TP_CAL, // save touch panel calibration information
    NVTEVT_OSD_REFLASH,
    NVTEVT_MOVIEAUTORECORD,
    NVTEVT_MOVIEAUTORECORDSTOP,    
    NVTEVT_USERCMD_END    //53

}NVT_USER_CMD;

typedef enum
{
    NVTEVT_EXE_SELFTIMER= NVTEVT_USERCMD_END + 1,
    NVTEVT_EXE_FLASH,
    NVTEVT_EXE_CAPTUREMODE,
    NVTEVT_EXE_QUALITY,
    NVTEVT_EXE_SIZE,
    NVTEVT_EXE_SCENEMODE,
    NVTEVT_EXE_EV,
    NVTEVT_EXE_WB,
    NVTEVT_EXE_METERING,
    NVTEVT_EXE_ISO,
    NVTEVT_EXE_COLOR,
    NVTEVT_EXE_SATURATION,
    NVTEVT_EXE_SHARPNESS,
    NVTEVT_EXE_FD,
    NVTEVT_EXE_ANTISHAKING,
    NVTEVT_EXE_QUICKREVIEW,
    NVTEVT_EXE_DATEIMPRINT,
    NVTEVT_EXE_GPS,
    NVTEVT_EXE_CYCLIC_REC,
    NVTEVT_EXE_GOD_BLESS,
    NVTEVT_EXE_GOLF_SHOT,
    NVTEVT_EXE_FLASH_REC,
    NVTEVT_EXE_AUDIO_SET,
    NVTEVT_EXE_MOTION_DET,
    NVTEVT_EXE_START_FLASH_REC,
    NVTEVT_EXE_CONTSHOT,
    NVTEVT_EXE_DEL,
    NVTEVT_EXE_COPY2CARD,
    NVTEVT_EXE_SLIDE,
    NVTEVT_EXE_DPOF,
    NVTEVT_EXE_PROTECT,
    NVTEVT_EXE_THUMBINIT,
    NVTEVT_EXE_THUMB,
    NVTEVT_EXE_FORMAT,
    NVTEVT_EXE_BEEP,
    NVTEVT_EXE_VOLUME,
    NVTEVT_EXE_LANGUAGE,
    NVTEVT_EXE_TVMODE,
    NVTEVT_EXE_POWEROFF,
    NVTEVT_EXE_SYSRESET,
    NVTEVT_EXE_NUMRESET,
    NVTEVT_EXE_FREQ,
    NVTEVT_EXE_LCDOFF,    
    NVTEVT_EXE_DELAYPOWEROFF,    
    NVTEVT_EXE_IMAGEROTATE,        
    NVTEVT_EXE_GSENSOR,            
    NVTEVT_EXE_BRIGHT,
    NVTEVT_EXE_DISPLAY,
    NVTEVT_EXE_MOVIESIZE,
    NVTEVT_EXE_MOVIEDIS,
    NVTEVT_EXE_DATEFORMAT,
    NVTEVT_EXE_USB,
    NVTEVT_EXE_OPENING,
    NVTEVT_EXE_END
}NVT_EXE_EVT;

typedef enum
{
    NVTEVT_CB_JPGOK = NVTEVT_EXE_END + 1,
    NVTEVT_CB_OZOOMSTEPCHG,
    NVTEVT_CB_DZOOMSTEPCHG,
    NVTEVT_CB_FOCUSEND,
    NVTEVT_CB_FDEND,
    NVTEVT_CB_SDEND,
    NVTEVT_CB_CAPTUREEND,
    NVTEVT_CB_CAPFSTOK,
    NVTEVT_CB_SMARTSCENE,
    NVTEVT_CB_SLIDE_KEEP,
    NVTEVT_CB_MOVIE_START,
    NVTEVT_CB_MOVIE_ONE_SEC,
    NVTEVT_CB_MOVIE_FINISH,
    NVTEVT_CB_MOVIE_FULL,
    NVTEVT_CB_MOVIE_WR_ERROR,
    NVTEVT_CB_MOVIE_CULT_NORMAL,
    NVTEVT_CB_MOVIE_ONE_VIDEOFRAME,
    NVTEVT_CB_MOVIE_OVERTIME,
    NVTEVT_CB_MP3PLAY_ONESECOND,
    NVTEVT_CB_MP3PLAY_FINISH,
    NVTEVT_CB_PRINT_DPS_EVENT,
    NVTEVT_CB_END
}NVT_CB_EVT;

typedef enum
{
    NVTEVT_BKW_START = NVTEVT_CB_END + 1,
    NVTEVT_BKW_DELALL = NVTEVT_BKW_START,
    NVTEVT_BKW_DELONE,
    NVTEVT_BKW_COPY2CARD,
    NVTEVT_BKW_FORMAT,
    NVTEVT_BKW_FORMATCARD,
    NVTEVT_BKW_FORMATNAND,
    NVTEVT_BKW_PROTECTALL,
    NVTEVT_BKW_UNPROTECTALL,
    NVTEVT_BKW_WAIT_PTR_INIT,
    NVTEVT_BKW_PLAYMOV,
    NVTEVT_BKW_STOPMOV,
    NVTEVT_BKW_PAUSEMOV,
    NVTEVT_BKW_END

}NVT_BKW_CMD;

typedef enum
{
    NVTEVT_TIMER_ANIMATE_ICON  = NVTEVT_BKW_END + 20,
    NVTEVT_PLAYINFO_TIMER,
    NVTEVT_TIMER_MODE,
    NVTEVT_SELFTIMER,
    NVTEVT_SELFTIMER_OFFSHOWN,
    NVTEVT_FLASHTIMER,
    NVTEVT_PWROFFTIMER,
    NVTEVT_UPDATETIMETIMER,
    NVTEVT_DISPTIMER,
    NVTEVT_SELFTIMER_LED,
    NVTEVT_SELFTIMER_RAPID_LED,
    NVTEVT_QUICKVIEWTIMER,
    NVTEVT_HISTOGRAMTIMER,
    //NVTEVT_SMARTSCENETIMER,
    NVTEVT_05SEC_TIMER,
    NVTEVT_1SEC_TIMER,
    NVTEVT_2SEC_TIMER,
    NVTEVT_3SEC_TIMER,
    NVTEVT_FUNNEL_TIMER,
    NVTEVT_TEST_BUTTON_OK,
    NVTEVT_WRNMSG_WND_EXP_TIMER,
    NVTEVT_STRG_CHK_TIMER,
    NVTEVT_MARQUEE_TIMER,
    NVTEVT_USER_EVT_END

}NVT_USER_EVT;

typedef enum
{
    NVTRET_CANCEL = NVTEVT_USER_EVT_END+1,
    NVTRET_CONTINUE,
    NVTRET_CONFIRM,
    NVTRET_SET,
    NVTRET_UNSET,
    NVTRET_SET_NUM,
    NVTRET_SINGLEOBJ,
    NVTRET_PLAYBACKKEY,
    NVTRET_DELETE,
    NVTRET_COPY2CARD,
    NVTRET_COPY2CARDFULL,
    NVTRET_SLIDE_SHOW,
    NVTRET_PROTECT,
    NVTRET_THUMBNAIL,
    NVTRET_MAGNIFY,
    NVTRET_DELETEALL,
    NVTRET_DELETEONE,
    NVTRET_FORMAT,
    NVTRET_WAITMOMENT,
    NVTRET_WAIT_SCREEN,
    NVTRET_CLOSE_SESSION,
    NVTRET_WRNMSG,
    NVTRET_ENTER_MENU,
    NVTRET_NULL_RET
}NVT_WND_RETURN_VALUE;


#endif
