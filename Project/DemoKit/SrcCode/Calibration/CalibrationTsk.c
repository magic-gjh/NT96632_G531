/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.c
    @ingroup    mIPRJAPTest

    @brief      Calibration task
                Calibration task

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include "Kernel.h"
#include "SysCfg.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"
//#include "ShowOSDFuncs.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "AppInit.h"

#if (_CALIBRATION_MODE_ == ENABLE)

// -------------------------------------------------------------------
// Internal Global variables
// -------------------------------------------------------------------
// Declare all global variables used in Calibration task here.
// Don't reference these variables in your code.
volatile BOOL               g_bCalbirationOpened = FALSE;
         RECT               g_CalStringRect;
volatile CAL_APPOBJ         g_CalAppObj = { NULL, NULL };



#endif

//@}
