/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTbl.c
    @ingroup    mIPRJAPTest

    @brief      Calibration item table
                Calibration item table

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include "CalibrationInt.h"
#include "CalibrationTsk.h"
#include "CalBurnIn.h"
#include "CalLCD.h"
#include "CalCIPA.h"
#include "CalSensor.h"

#if (_CALIBRATION_MODE_ == ENABLE)

// -------------------------------------------------------------------
// Internal Global variables
// -------------------------------------------------------------------
// Declare all global variables used in Calibration task here.
// Don't reference these variables in your code.

// Calibration item table, please keep at least one item to make calibration task
// works correctly
CAL_ITEM g_CalItem[]    =
{
    // Display Name             Function pointer
    {"Defect Pixel",            Cal_SenDP           },
    {"ISO",                     Cal_SenISO          },
//    {"Flash",                   Cal_SenFlash        },
//    {"AF Far",                  Cal_SenAFFar        },
//    {"AF Near",                 Cal_SenAFNear       },
//    {"M Shutter",               Cal_SenMShutter     },
    {"AWB",                     Cal_SenAWB          },
//    {"LENS",                    Cal_SenLENS         },
//    {"Write 2 File",            Cal_SenWrite2File   },
    {"Read Cal. Status",        Cal_ReadCalStatus   },
    //#NT#2010/12/15#JJ Huang -begin
    //#NT#For SNR Measure
    {"SNR",                     Cal_SenSNR          },
    //#NT#2010/12/15#JJ Huang -end
    {"Exit",                    Cal_SenExit   },
//    {"Zoom/AF Lasting",         Cal_BurnInZoomAF    },
//    {"Flash Lasting",           Cal_BurnInFlash     },
//    {"168H",                    Cal_BurnIn168H      },
//    {"LCD",                     Cal_LCDVerify       },
//    {"Write Default and SN",    Cal_SetDefault      },
//    {"Read Status",             Cal_NoFunction      },
//    {"Burn In",                 Cal_BurnInBurnIn    },
//    {"CIPA",                    Cal_CIPA            },
};

UINT32 g_CalMaxItemNum      = sizeof(g_CalItem) / sizeof(CAL_ITEM);

//#NT#2009/08/31#KS Hung -begin
UINT32 Cal_GetMaxItemNum(void)
{
    return g_CalMaxItemNum;
}
//#NT#2009/08/31#KS Hung -end

#endif

//@}
