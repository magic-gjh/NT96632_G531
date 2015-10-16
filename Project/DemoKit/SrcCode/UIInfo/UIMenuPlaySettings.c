#include "Type.h"
#include "SysCfg.h"
#include "UIMenuPlaySettings.h"
#include "UICfgDefault.h"


static UINT32 guiSlideIndex         = UIDFT_SLIDE_INTERVAL;
static UINT32 guiDPOFPrtIndex       = UIDFT_DPOF_PRT_NUM;
static UINT32 guiPlayMovSpeedIndex  = UIDFT_PLAYMOV_SPEED;


//-----------------------------------------------------------------------------
// SlideShow Interval
//-----------------------------------------------------------------------------
void Set_SlideIndex(UINT32 uiIndex)
{
    guiSlideIndex = uiIndex;
}
UINT32 Get_SlideIndex(void)
{
    return guiSlideIndex;
}

//-----------------------------------------------------------------------------
// DPOF Print Number
//-----------------------------------------------------------------------------
void Set_DPOFPrtIndex(UINT32 uiIndex)
{
    guiDPOFPrtIndex = uiIndex;
}
UINT32 Get_DPOFPrtIndex(void)
{
    return guiDPOFPrtIndex;
}

//-----------------------------------------------------------------------------
// PlayMov Speed
//-----------------------------------------------------------------------------
void Set_PlayMovSpeedIndex(UINT32 uiIndex)
{
    guiPlayMovSpeedIndex = uiIndex;
}
UINT32 Get_PlayMovSpeedIndex(void)
{
    return guiPlayMovSpeedIndex;
}


