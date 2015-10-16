/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2008.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Cal_Api.c                                                           *
* Description:     Calibration application interface between "Project" and "SensorLibrary"                                      *
* Author:                                                                    *
****************************************************************************/

#include    "ErrorNo.h"
#include    "Cal_Api.h"

Cal_Struct    gCalStructData
={0};

/**
  Cal_SetCalibrationData
  Set calibration data
  @param UINT CalItem, Calibration status and items, please check enum "_CAL_Item"
  @param UINT  *pCalData, The pointer of detail calibration data array,
                              the content is different for dirfferent items.
                              It must communicate with SensorLibrary.
  @return ER    Error Code,
*/

ER Cal_SetCalibrationData(UINT CalItem,    //Calibration Status and Items, check _CAL_Item
                        UINT  *pCalData)    //The pointer of detail calibration data  array
{
    int i;

    switch(CalItem)
    {
        case    _CalStatus_AWB:
            gCalStructData.CalStatus_AWB=pCalData[0];
            break;
        case    _CalStatus_PrvDP_W:
            gCalStructData.CalStatus_PrvDP_W=pCalData[0];
            break;
        case    _CalStatus_PrvDP_B:
            gCalStructData.CalStatus_PrvDP_B=pCalData[0];
            break;
        case    _CalStatus_CapDP_W:
            gCalStructData.CalStatus_CapDP_W=pCalData[0];
            break;
        case    _CalStatus_CapDP_B:
            gCalStructData.CalStatus_CapDP_B=pCalData[0];
            break;
        case    _CalStatus_ISO:
            gCalStructData.CalStatus_ISO=pCalData[0];
            break;
        case    _CalStatus_OB:
            gCalStructData.CalStatus_OB=pCalData[0];
            break;
        case    _CalStatus_Mshutter:
            gCalStructData.CalStatus_Mshutter=pCalData[0];
            break;
        case    _CalStatus_FocusNear:
            gCalStructData.CalStatus_FocusNear=pCalData[0];
            break;
        case    _CalStatus_FocusFar:
            gCalStructData.CalStatus_FocusFar=pCalData[0];
            break;
        case    _CalStatus_LensShading:
            gCalStructData.CalStatus_LensShading=pCalData[0];
            break;
        case    _CalStatus_ColorShading:
            gCalStructData.CalStatus_ColorShading=pCalData[0];
            break;
        case    _CalStatus_GeoDistortion:
            gCalStructData.CalStatus_GeoDistortion=pCalData[0];
            break;
        case    _CalStatus_FlashTriggerPointer:
            gCalStructData.CalStatus_FlashTriggerPointer=pCalData[0];
            break;
        case    _CalStatus_FlashShading:
            gCalStructData.CalStatus_FlashShading=pCalData[0];
            break;
        case    _CalStatus_FlashFire:
            gCalStructData.CalStatus_FlashFire=pCalData[0];
            break;
        case    _CalStatus_FlashAWB:
            gCalStructData.CalStatus_FlashAWB=pCalData[0];
            break;
        case    _CalStatus_FlashADC:
            gCalStructData.CalStatus_FlashADC=pCalData[0];
            break;

        //Data
        case    _Cal_AWB_Statistic:
            gCalStructData.Cal_AWBStatistic.Rgain = pCalData[0];
            gCalStructData.Cal_AWBStatistic.Bgain = pCalData[1];
#if 0
            gCalStructData.Cal_AWBStatistic.R=pCalData[0];
            gCalStructData.Cal_AWBStatistic.G=pCalData[1];
            gCalStructData.Cal_AWBStatistic.B=pCalData[2];
#endif
            break;
        case    _Cal_DP_PreviewAddr:
            gCalStructData.Cal_DP_PreviewAddr=pCalData[0];
            break;
        case    _Cal_DP_CaptureAddr:
            gCalStructData.Cal_DP_CaptureAddr=pCalData[0];
            break;
        //#NT#20091120#ethanlau -begin
        case    _Cal_DP_PreviewNum:
            gCalStructData.Cal_DP_PreviewNum;
            debug_msg("cal data : %d %d %d\r\n",pCalData[0],pCalData[1],pCalData[2]);
            break;
        case    _Cal_DP_CaptureNum:
            gCalStructData.Cal_DP_CaptureNum = pCalData[0];
            break;
        //#NT#20091120#ethanlau -end
        case    _Cal_OB:
            gCalStructData.Cal_OB=pCalData[0];
            break;
        case    _Cal_Mshutter:
            gCalStructData.Cal_Mshutterr_Data[0]=pCalData[0];
            gCalStructData.Cal_Mshutterr_Data[1]=pCalData[1];
            gCalStructData.Cal_Mshutterr_Data[2]=pCalData[2];
            break;
        case    _Cal_ISO:
            gCalStructData.Cal_ISO_Data[0]=pCalData[0];
            gCalStructData.Cal_ISO_Data[1]=pCalData[1];
            gCalStructData.Cal_ISO_Data[2]=pCalData[2];
            break;



        case    _Cal_Flash_TriggerPoint:
            gCalStructData.Cal_Flash.FlashTriggerPoint=pCalData[0];
            break;

        case    _Cal_Flash_Fire:
            gCalStructData.Cal_Flash.FlashFire=pCalData[0];
            break;
        case    _Cal_Flash_AWB:
            gCalStructData.Cal_Flash.FlashR=pCalData[0];
            gCalStructData.Cal_Flash.FlashG=pCalData[1];
            gCalStructData.Cal_Flash.FlashB=pCalData[2];
            break;
        case    _Cal_Focus_Far:
            //for(i=0;i++;i<40)
            for(i = 0; i < 40; i++)
                gCalStructData.Cal_Focus_FarTable[i]=pCalData[i];
            break;
        case    _Cal_Focus_Near:
            //for(i=0;i++;i<40)
            for(i = 0; i < 40; i++)
                gCalStructData.Cal_Focus_NearTable[i]=pCalData[i];
            break;
    }

    return E_OK;
}

/**
  Cal_GetCalibrationData
  Get calibration data
  @param void,
  @return Cal_Struct*,    retrun a pointer to calibration struct
*/

Cal_Struct* Cal_GetCalibrationData(void)    //Get the Calibration struct pointer
{

    return    &gCalStructData;

}