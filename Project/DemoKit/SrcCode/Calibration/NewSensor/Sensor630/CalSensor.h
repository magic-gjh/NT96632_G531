/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalSensor.h
    @ingroup    mIPRJAPTest

    @brief      Header file for sensor calibration
                This file is the header file for sensor calibration

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2006/01/10
*/

/** \addtogroup mIPRJAPTest */
//@{

#ifndef _CALSENSOR_H
#define _CALSENSOR_H

#include "Type.h"

extern void     Cal_SenExit(void);
extern void     Cal_SenDP(void);
extern void     Cal_SenISO(void);
extern void     Cal_SenFlash(void);
extern void     Cal_SenFlashAWB(void);
extern void     Cal_SenFlashRedeyeTest(void);
extern void     Cal_SenFlashGnoShots(void);
extern void     Cal_SenAFFar(void);
extern void     Cal_SenAFNear(void);
extern void     Cal_SenFocusOffset(void);//JJ0325
extern void     Cal_SenMShutter(void);
extern void     Cal_SenAWB(void);
//add Lens Test item
extern void     Cal_SenLENS(void);
extern void     Cal_SenWrite2File(void);
extern void     Cal_ReadCalStatus(void);
//#NT#2007/04/17#YC Peng -begin
//#extern functions
extern void     Cal_SenOB(void);
extern void     Cal_SenVIG(void);

extern void     Cal_DP_AWB_Mshutter(void);

extern void     Cal_Reset_Adjust(void);
extern void     Cal_MShutterLasting(void);
//#NT#2007/04/17#YC Peng -end

extern void     Cal_SenMShutterSWTest(void);
extern void     Cal_SenApertureSWTest(void);
extern void     Cal_SenFAETest(void);
extern void     Cal_SenZoomSpeedTest(void);
extern void     Cal_SenZoomStepMissTest(void);
extern void     Cal_SenZoomStop2PR(void);//JJ0228
extern void     Cal_SenFocusStepMissTest(void);
extern void     Cal_SenAFTest(void);
extern void     Cal_SenStainTest(void);
extern void     Cal_SenTVResolutionTest(void);
//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
extern void     Cal_SenSNR(void);
//#NT#2010/12/15#JJ Huang -end

#endif

//@}
