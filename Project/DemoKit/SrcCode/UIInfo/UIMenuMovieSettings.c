#include "Type.h"
#include "SysCfg.h"
#include "UIMenuMovieSettings.h"
#include "Debug.h"
#include "UICfgDefault.h"


static UINT32 guiMovieSizeIndex         = UIDFT_MOVIE_SIZE;
static UINT32 guiMovieQualityIndex      = UIDFT_MOVIE_QUALITY;
static UINT32 guiMovieColorIndex        = UIDFT_MOVIE_COLOR ;
static UINT32 guiMovieSelftimer         = UIDFT_MOVIE_SELFTIMER;
static UINT32 guiMovieDzoomIndex        = UIDFT_MOVIE_DZOOM;
static UINT32 guiMovieDISIndex          = UIDFT_MOVIE_DIS;
static UINT32 guiMovieDateImprintIndex  = UIDFT_MOVIE_DATEIMPRINT;
static UINT32 guiMovieGPSIndex          = MOVIE_GPS_OFF;
static UINT32 guiMovieCyclicRecIndex    = UIDFT_MOVIE_CYCLIC_REC;
static UINT32 guiMovieGolfShotIndex     = UIDFT_MOVIE_GOLF_SHOT;
static UINT32 guiMovieFlashRecIndex     = UIDFT_MOVIE_FLASH_REC;
static UINT32 guiMovieAudioIndex        = UIDFT_MOVIE_AUDIO;
static UINT32 guiMovieMotionDetIndex    = UIDFT_MOVIE_MOTIONDET;
static UINT32 guiMovieGodBlessIndex     = UIDFT_MOVIE_GODBLESS;
static UINT32 guiMovieGsensorIndex     = UIDFT_MOVIE_GSENSOR;


//-----------------------------------------------------------------------------
// Movie Size
//-----------------------------------------------------------------------------
void Set_MovieSizeIndex(UINT32 uiIndex)
{
    guiMovieSizeIndex = uiIndex;
}
UINT32 Get_MovieSizeIndex(void)
{
    return guiMovieSizeIndex;
}

//-----------------------------------------------------------------------------
// Movie Quality
//-----------------------------------------------------------------------------
void Set_MovieQualityIndex(UINT32 uiIndex)
{
    guiMovieQualityIndex = uiIndex;
}
UINT32 Get_MovieQualityIndex(void)
{
    return guiMovieQualityIndex;
}

//-----------------------------------------------------------------------------
// Movie Color
//-----------------------------------------------------------------------------
void Set_MovieColorIndex(UINT32 uiIndex)
{
    guiMovieColorIndex = uiIndex;
}
UINT32 Get_MovieColorIndex(void)
{
    return guiMovieColorIndex;
}

//-----------------------------------------------------------------------------
// Movie Selftimer
//-----------------------------------------------------------------------------
void Set_MovieSelftimerIndex(UINT32 uiIndex)
{
    guiMovieSelftimer = uiIndex;
}
UINT32 Get_MovieSelftimerIndex(void)
{
    return guiMovieSelftimer;
}

//-----------------------------------------------------------------------------
// Movie DZoom
//-----------------------------------------------------------------------------
void Set_MovieUIDzoomIndex(UINT32 uiIndex)
{
    guiMovieDzoomIndex = uiIndex;
}
UINT32 Get_MovieUIDzoomIndex(void)
{
    return guiMovieDzoomIndex;
}

//-----------------------------------------------------------------------------
// Movie DIS
//-----------------------------------------------------------------------------
void Set_MovieDISIndex(UINT32 uiIndex)
{
    guiMovieDISIndex = uiIndex;
}
UINT32 Get_MovieDISIndex(void)
{
    return guiMovieDISIndex;
}

//-----------------------------------------------------------------------------
// Movie Date Imprint State
//-----------------------------------------------------------------------------
void Set_MovieDateImprintIndex(UINT32 uiIndex)
{
    guiMovieDateImprintIndex = uiIndex;
}
UINT32 Get_MovieDateImprintIndex(void)
{
    return guiMovieDateImprintIndex;
}

//-----------------------------------------------------------------------------
// Movie GPS State
//-----------------------------------------------------------------------------
void Set_MovieGPSIndex(UINT32 uiIndex)
{
    guiMovieGPSIndex = uiIndex;
}
UINT32 Get_MovieGPSIndex(void)
{
    return guiMovieGPSIndex;
}

//-----------------------------------------------------------------------------
// Movie Cyclic Record
//-----------------------------------------------------------------------------
void Set_MovieCyclicRecIndex(UINT32 uiIndex)
{
    guiMovieCyclicRecIndex = uiIndex;
}
UINT32 Get_MovieCyclicRecIndex(void)
{
    return guiMovieCyclicRecIndex;
}
UINT32 Get_MovieCyclicRecTime(void)
{
    UINT32 uiCyclicRecTime = 0;

    // cyclic recording handling
    switch (guiMovieCyclicRecIndex)
    {
    case MOVIE_CYCLIC_REC_3MIN:
        uiCyclicRecTime = 180;
        break;
    case MOVIE_CYCLIC_REC_5MIN:
        uiCyclicRecTime = 300;
        break;
    case MOVIE_CYCLIC_REC_10MIN:
        uiCyclicRecTime = 600;
        break;
    case MOVIE_CYCLIC_REC_OFF:
    default:
        break;
    }

    return uiCyclicRecTime;
}

//-----------------------------------------------------------------------------
// Movie Golf Shot
//-----------------------------------------------------------------------------
void Set_MovieGolfShotIndex(UINT32 uiIndex)
{
    guiMovieGolfShotIndex = uiIndex;
}
UINT32 Get_MovieGolfShotIndex(void)
{
    return guiMovieGolfShotIndex;
}

//-----------------------------------------------------------------------------
// Movie Flash Recording
//-----------------------------------------------------------------------------
void Set_MovieFlashRecIndex(UINT32 uiIndex)
{
    guiMovieFlashRecIndex = uiIndex;
}
UINT32 Get_MovieFlashRecIndex(void)
{
    return guiMovieFlashRecIndex;
}

//-----------------------------------------------------------------------------
// Movie Audio state
//-----------------------------------------------------------------------------
void Set_MovieAudioIndex(UINT32 uiIndex)
{
    guiMovieAudioIndex = uiIndex;
}
UINT32 Get_MovieAudioIndex(void)
{
    return guiMovieAudioIndex;
}

//-----------------------------------------------------------------------------
// Movie Motion Detect state
//-----------------------------------------------------------------------------
void Set_MovieMotionDetIndex(UINT32 uiIndex)
{
    guiMovieMotionDetIndex = uiIndex;
}
UINT32 Get_MovieMotionDetIndex(void)
{
    return guiMovieMotionDetIndex;
}

//-----------------------------------------------------------------------------
// God Bless state
//-----------------------------------------------------------------------------
void Set_MovieGodBlessIndex(UINT32 uiIndex)
{
    guiMovieGodBlessIndex = uiIndex;
}
UINT32 Get_MovieGodBlessIndex(void)
{
    return guiMovieGodBlessIndex;
}

//-----------------------------------------------------------------------------
// gsensor setting
//-----------------------------------------------------------------------------
void Set_MovieGsensorIndex(UINT32 uiIndex)
{
    guiMovieGsensorIndex = uiIndex;
}
UINT32 Get_MovieGsensorIndex(void)
{
    return guiMovieGsensorIndex;
}


