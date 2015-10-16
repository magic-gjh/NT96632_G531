#ifndef UI_MENUAVISETTING_H
#define UI_MENUAVISETTING_H

#include "Type.h"



//-----------------------------------------------------------------------------
// Movie Size
//-----------------------------------------------------------------------------
typedef enum
{
    //MOVIE_SIZE_1080P_HD,//1920*1080 P10
    //MOVIE_SIZE_1080P,//1440*1080 P15
    MOVIE_SIZE_720P,//1280*720 P30
    MOVIE_SIZE_WVGA,//848*480  P60
    MOVIE_SIZE_VGA,
    MOVIE_SIZE_QVGA,//320*240  P30
    MOVIE_SIZE_SETTING_MAX,     
} MOVIE_SIZE_SETTING;

//-----------------------------------------------------------------------------
// Movie Quality
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_QUALITY_FINE = 0,
    MOVIE_QUALITY_STANDARD,
    MOVIE_QUALITY_ECONOMY,
    MOVIE_QUALITY_SETTING_MAX
} MOVIE_QUALITY_SETTING;

//-----------------------------------------------------------------------------
// Movie Color
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_COLOR_STANDARD = 0,
    MOVIE_COLOR_VIVID,
    MOVIE_COLOR_SEPIA,
    MOVIE_COLOR_MONO,
    MOVIE_COLOR_SETTING_MAX
} MOVIE_COLOR_SETTING;

//-----------------------------------------------------------------------------
// Movie DZoom
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_DZOOM_10X = 0,
    MOVIE_DZOOM_20X,
    MOVIE_DZOOM_30X,
    MOVIE_DZOOM_40X,
    MOVIE_DZOOM_SETTING_MAX
} MOVIE_DZOOM_SETTING;

//-----------------------------------------------------------------------------
// Movie Selftimer
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_SELFTIMER_OFF = 0x00,
    MOVIE_SELFTIMER_ON_2S,
    MOVIE_SELFTIMER_ON_10S,
    MOVIE_SELFTIMER_ON_10_2S,
    MOVIE_SELFTIMER_SETTING_MAX
} MOVIE_SELFTIMER_SETTIMG;

//-----------------------------------------------------------------------------
// Movie Frame Rate
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_FPS_15 = 0,
    MOVIE_FPS_30,
    MOVIE_FPS_SETTING_MAX
} MOVIE_FPS_SETTING;

//-----------------------------------------------------------------------------
// Movie DIS
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_DIS_OFF,
    MOVIE_DIS_ON,
    MOVIE_DIS_SETTING_MAX
} MOVIE_DIS_SETTING;

//-----------------------------------------------------------------------------
// Movie Date Imprint State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_DATEIMPRINT_OFF = 0,
    MOVIE_DATEIMPRINT_ON,
    MOVIE_DATEIMPRINT_SETTING_MAX
} MOVIE_DATEIMPRINT_SETTING;


//-----------------------------------------------------------------------------
// Movie GPS State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_GPS_OFF = 0,
    MOVIE_GPS_ON,
    MOVIE_GPS_SETTING_MAX
} MOVIE_GPS_SETTING;

//-----------------------------------------------------------------------------
// Movie Cyclic Recording
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_CYCLIC_REC_OFF = 0,
    MOVIE_CYCLIC_REC_3MIN,
    MOVIE_CYCLIC_REC_5MIN,
    MOVIE_CYCLIC_REC_10MIN,
    MOVIE_CYCLIC_REC_SETTING_MAX
} MOVIE_CYCLIC_REC_SETTING;

//-----------------------------------------------------------------------------
// Movie Golf Shot State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_GOLFSHOT_OFF = 0,
    MOVIE_GOLFSHOT_ON,
    MOVIE_GOLFSHOT_SETTING_MAX
} MOVIE_GOLFSHOT_SETTING;

//-----------------------------------------------------------------------------
// Movie Flash Record State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_FLASHREC_OFF = 0,
    MOVIE_FLASHREC_ON,
    MOVIE_FLASHREC_SETTING_MAX
} MOVIE_FLASHREC_SETTING;


//-----------------------------------------------------------------------------
// Movie Motion Detect State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_MOTIONDET_OFF = 0,
    MOVIE_MOTIONDET_ON,
    MOVIE_MOTIONDET_SETTING_MAX
} MOVIE_MOTIONDET_SETTING;

//-----------------------------------------------------------------------------
// Movie Audio State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_AUDIO_OFF = 0,
    MOVIE_AUDIO_ON,
    MOVIE_AUDIO_SETTING_MAX
} MOVIE_AUDIO_SETTING;

//-----------------------------------------------------------------------------
// God Bless State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_GODBLESS_OFF = 0,
    MOVIE_GODBLESS_ON,
    MOVIE_GODBLESS_SETTING_MAX
} MOVIE_GODBLESS_SETTING;

//-----------------------------------------------------------------------------
// Gsensor State
//-----------------------------------------------------------------------------
typedef enum
{
    MOVIE_GSENSOR_OFF = 0,
    MOVIE_GSENSOR_LOW,
    MOVIE_GSENSOR_MED,
    MOVIE_GSENSOR_HIGH,    
    MOVIE_GSENSOR_SETTING_MAX
} MOVIE_GSENSOR_SETTING;

//-----------------------------------------------------------------------------
//DELAY POWEROFF
//-----------------------------------------------------------------------------
typedef enum
{
    DELAY_POWER_OFF_OFF = 0,
    DELAY_POWER_OFF_15S,
    DELAY_POWER_OFF_1MIN,
    DELAY_POWER_OFF_5MIN,
    DELAY_POWER_OFF_30MIN,
    DELAY_POWER_OFF_60MIN,    
    DELAY_POWER_OFF_MAX
} DELAYPOWER_OFF_SETTING;
//-----------------------------------------------------------------------------
// LCD OFF
//-----------------------------------------------------------------------------
typedef enum
{
    LCDOFF_OFF = 0,
    LCDOFF_1MIN,
    LCDOFF_3MIN,
    LCDOFF_5MIN,
    LCDOFF_MAX
} LCDOFF_TIMER_SETTING;

//-----------------------------------------------------------------------------
//IMAGE ROTATE
//-----------------------------------------------------------------------------
typedef enum
{
    IMAGE_ROTATE_OFF = 0,
    IMAGE_ROTATE_ON,   
    IMAGE_ROTATE_MAX
} IMAGE_ROTATE_SETTING;

//-----------------------------------------------------------------------------
//ACC OFF POWEROFF FLOW
//-----------------------------------------------------------------------------
typedef enum
{
    ACC_OFF_POWEROFF_FALSE = 0,
    ACC_OFF_POWEROFF_TRUE,   
    ACC_OFF_POWEROFF_MAX
} ACC_OFF_POWEROFF_SETTING;
//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern void     Set_MovieSizeIndex(UINT32 uiIndex);
extern UINT32   Get_MovieSizeIndex(void);

extern void     Set_MovieQualityIndex(UINT32 uiIndex);
extern UINT32   Get_MovieQualityIndex(void);

extern void     Set_MovieColorIndex(UINT32 uiIndex);
extern UINT32   Get_MovieColorIndex(void);

extern void     Set_MovieSelftimerIndex(UINT32 uiIndex);
extern UINT32   Get_MovieSelftimerIndex(void);

extern void     Set_MovieUIDzoomIndex(UINT32 uiIndex);
extern UINT32   Get_MovieUIDzoomIndex(void);

extern void     Set_MovieDISIndex(UINT32 uiIndex);
extern UINT32   Get_MovieDISIndex(void);

extern void     Set_MovieDateImprintIndex(UINT32 uiIndex);
extern UINT32   Get_MovieDateImprintIndex(void);

#endif
