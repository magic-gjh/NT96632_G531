/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DeviceCtrl.c
    @ingroup    mIPRJAPCfg

    @brief      Control device
                Control LCD/LCD backlight on/off, CCD power on/off.
                Lens...

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "DeviceCtrl.h"
#include "SystemInit.h"
#include "Sie_Lib.h"
#include "PLL.h"
#include "ImageAlgInfor.h"


#if 0   //IQ has new IPL mode APIs
static volatile UINT32      uiIPLMode = DEVCTRL_IPLMODE_OFF;

/**
  Get current image pipeline mode

  Get current image pipeline mode.

  @param void
  @return UINT32 image pipeline mode
*/
UINT32 DevCtrl_GetIPLMode(void)
{
    return uiIPLMode;
}

/**
  Set image pipeline mode

  Set image pipeline mode.

  @param UINT32 uiMode: Image pipeline mode
  @return void
*/
void DevCtrl_SetIPLMode(UINT32 uiMode)
{
    uiIPLMode = uiMode;

    switch (uiMode)
    {
    case DEVCTRL_IPLMODE_PREVIEW:
#if (SYSINIT_OUTPUT_MCLK == ENABLE)
        // Enable MCLK output
        pll_enableClock(SIE_MCLK);
#endif
        Set2Preview();
        break;

    case DEVCTRL_IPLMODE_VIDEO:
#if (SYSINIT_OUTPUT_MCLK == ENABLE)
        // Enable MCLK output
        pll_enableClock(SIE_MCLK);
#endif
        Set2Video();
        break;

    case DEVCTRL_IPLMODE_PCC:
#if (SYSINIT_OUTPUT_MCLK == ENABLE)
        // Enable MCLK output
        pll_enableClock(SIE_MCLK);
#endif
        Set2Preview();
        break;

//#NT#2008/03/18#Adrian -begin
//added "case DEVCTRL_IPLMODE_OFF" back
        //#NT#2008/03/16#Adrian -begin
        //The original code, the ImagePipelines were not stopped completely,
        //only SIE was stopped.
        //StopPreviewDramData() will stop SIE/IPE/IME.
    case DEVCTRL_IPLMODE_OFF:
        // Disable CROP function
        //sie_Function_Enable(FALSE, SIE_CROP_EN);
        // Make sure SIE doesn't output data any more
        //SensorDelayVD(1);
        StopPreviewDramData();
        //#NT#2008/03/16#Adrian -end
#if (SYSINIT_OUTPUT_MCLK == ENABLE)
        // Disable MCLK output
        pll_disableClock(SIE_MCLK);
#endif
        break;

     default:
        break;
//#NT#2008/03/18#Adrian -end

    }
}
#endif

//@}
