/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalSensor.c
    @ingroup    mIPRJAPTest

    @brief      Sensor calibration
                Sensor calibration

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2006/01/10
*/

/** \addtogroup mIPRJAPTest */
//@{

#include "Kernel.h"
#include "SysCfg.h"
#include "cal_common_630.h"
#include "ImageAlgInfor.h"
#include "CalibrationInt.h" //#NT#2007/12/18#Stan Lin
//#NT#2007/12/18#Stan Lin -Remove #include "CalibrationTsk.h"
#include "KeyScanTsk.h"
#include "Filesystsk.h"
#if (_CALIBRATION_MODE_ == ENABLE)

void Cal_SenExit(void)
{
  MenuCalibration_Close();
}

void Cal_SenDP(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_DEFECTPIXEL, &CalParam);
}

void Cal_SenISO(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_ISO, &CalParam);
}

void Cal_SenFlash(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FLASH, &CalParam);
}
void Cal_SenFlashAWB(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FLASH_AWB, &CalParam);
}
void Cal_SenFlashRedeyeTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FLASH_REDEYE_TEST, &CalParam);
}

void Cal_SenFlashGnoShots(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FLASH_GNO_SHOTS, &CalParam);
}


void Cal_SenAFFar(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_AF_FAR, &CalParam);
}

void Cal_SenAFNear(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_AF_NEAR, &CalParam);
}
void Cal_SenFocusOffset(void)
{//JJ0325
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FOCUSOFFSET, &CalParam);
}

void Cal_SenMShutter(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_MSHUTTER, &CalParam);
}

void Cal_SenAWB(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_AWB, &CalParam);
}

//add Lens Test item
void Cal_SenLENS(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_LENS, &CalParam);
}

void Cal_SenWrite2File(void)
{
    UINT32  result;
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_WR2FILE, &CalParam);
    Cal_SetDefault();
    TimerDelayMs(500);
    result = Alg_Cal(CAL_READSTATUS, &CalParam);

    if(!result)
    {
        while(1) ;;
    }
}

void Cal_ReadCalStatus(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_READSTATUS, &CalParam);
}

//#NT#2007/04/17#YC Peng -begin
//#add OB and VIG adjust
void Cal_SenOB(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_OB, &CalParam);
}

void Cal_SenVIG(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_VIG, &CalParam);
}
//#NT#2007/04/17#YC Peng -end


void Cal_DP_AWB_Mshutter(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();

    Alg_Cal(CAL_DEFECTPIXEL_WHITE, &CalParam);
    Alg_Cal(CAL_AWB, &CalParam);
    Alg_Cal(CAL_MSHUTTER, &CalParam);

    //show status
    TimerDelayMs(500);
    Alg_Cal(CAL_READSTATUS, &CalParam);
}


void Cal_Reset_Adjust()
{
    memset(AAA_Adjust,0,sizeof(AAA_Adjust));
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    //#NT#2011/01/03#Chris Chung -begin
    //#NT#mark un-used file system API
    #if 0
    FilesysNANDWriteSysParamBlockByBytes((CHAR *)&AAA_Adjust, CAL_BLOCK, sizeof(AAA_Adjust));
    #endif
    //#NT#2011/01/03#Chris Chung -end
}


void Cal_MShutterLasting(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_MSHUTTER_LASTING, &CalParam);
}


void Cal_SenMShutterSWTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_MSHUTTER_SW_TEST, &CalParam);
}
void Cal_SenApertureSWTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_APERTURE_SW_TEST, &CalParam);
}
void Cal_SenFAETest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FAE_TEST, &CalParam);
}
void Cal_SenZoomSpeedTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_ZOOM_SPEED_TEST, &CalParam);
}
void Cal_SenZoomStepMissTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_ZOOM_STEPMISS_TEST, &CalParam);
}
void Cal_SenZoomStop2PR(void)//JJ0228
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_ZOOM_STOP2PR, &CalParam);
}
void Cal_SenFocusStepMissTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_FOCUS_STEPMISS_TEST, &CalParam);
}
void Cal_SenAFTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_AF_TEST, &CalParam);
}
void Cal_SenStainTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_STAIN_TEST, &CalParam);
}
void Cal_SenTVResolutionTest(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_TVRESOLUTION_TEST, &CalParam);
}

//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
void Cal_SenSNR(void)
{
    ALG_CAL_PARAM CalParam;
    CalParam.FnShowString = Cal_ShowString;
    CalParam.MemoryAddr = Cal_GetMemoryAddr();
    CalParam.MemorySize = Cal_GetMemorySize();
    Alg_Cal(CAL_SNR, &CalParam);
}
//#NT#2010/12/15#JJ Huang -end


#endif

//@}
