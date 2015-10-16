#ifndef _CAL_API_H
#define _CAL_API_H

#include "type.h"

//#NT#2008/07/08#Bowen Li -begin
//#NT#add cal status
#define _Iris_Test_Status       0x10
#define _Mshutter_Test_Status   0x11
#define _FAE_Test_Status        0x12
#define _Zoom_SpeedTest_Status  0x14
#define _Zoom_StepMissTest_Status  0x15
#define _Zoom_Stop2PR_Status        0x16
#define _Focus_StepMissTest_Status  0x16
#define _FF_Status 0x81
#define _NF_Status 0x86
#define _Focus_Test_Status      0x87
#define _Focus_Offset_Status    0x88

#define _RTC_Test_Status        0x31
#define _EmptyVolt_Status       0x32
#define _Burnin_Status          0x33
#define _Language_Status        0x34

#define _ISO_Status 0x20
#define _Mshutter_Status 0x1B
#define _FTP_Status 0x21
#define _Prv_W_Status 0x41
#define _Prv_B_Status 0x42
#define _Cap_W_Status 0x45
#define _Cap_B_Status 0x46
#define _Dzoom_W_Status 0x4A
#define _Dzoom_B_Status 0x4B
#define _AWB_Status 0x52
#define _Flash_Status 0x61
#define _OB_Status 0x71
#define _Shading_Status 0x81

typedef enum {
    _CalStatus_AWB,
    _CalStatus_PrvDP_W,
    _CalStatus_PrvDP_B,
    _CalStatus_CapDP_W,
    _CalStatus_CapDP_B,
    _CalStatus_ISO,
    _CalStatus_OB,
    _CalStatus_Mshutter,
    _CalStatus_FocusNear,
    _CalStatus_FocusFar,
    _CalStatus_LensShading,
    _CalStatus_ColorShading,
    _CalStatus_GeoDistortion,
    _CalStatus_FlashTriggerPointer,
    _CalStatus_FlashShading,
    _CalStatus_FlashFire,
    _CalStatus_FlashAWB,
    _CalStatus_FlashADC,
    _CalStatus_ZoomStop2PR,

    _Cal_AWB_Statistic,
    _Cal_DP_PreviewAddr,
    _Cal_DP_CaptureAddr,
    _Cal_DP_PreviewNum,
    _Cal_DP_CaptureNum,
    _Cal_OB,
    _Cal_Mshutter,
    _Cal_ISO,
    _Cal_Focus_Far,
    _Cal_Focus_Near,
    _Cal_LensShading,
    _Cal_ColorShading,
    _Cal_GeoDistortion,
    _Cal_Flash_TriggerPoint,
    _Cal_Flash_Fire,
    _Cal_Flash_AWB,
    _Cal_Zoom_Stop2PR

} _CAL_Item;

typedef struct _CalAWB_Struct{
    UINT R;
    UINT G;
    UINT B;
    UINT Rgain;
    UINT Ggain;
    UINT Bgain;
}CalAWB_Struct;
typedef struct _CalFlash_Struct{
    UINT FlashTriggerPoint;
    UINT FlashFire;
    UINT FlashR;
    UINT FlashG;
    UINT FlashB;

}CalFlash_Struct;


typedef struct _Cal_Struct{
    UINT    CalStatus_AWB;
    UINT    CalStatus_PrvDP_W;
    UINT    CalStatus_PrvDP_B;
    UINT    CalStatus_CapDP_W;
    UINT    CalStatus_CapDP_B;
    UINT    CalStatus_ISO;
    UINT    CalStatus_OB;
    UINT    CalStatus_Mshutter;
    UINT    CalStatus_FocusNear;
    UINT    CalStatus_FocusFar;
    UINT    CalStatus_LensShading;
    UINT    CalStatus_ColorShading;
    UINT    CalStatus_GeoDistortion;
    UINT    CalStatus_FlashTriggerPointer;
    UINT    CalStatus_FlashShading;
    UINT    CalStatus_FlashFire;
    UINT    CalStatus_FlashAWB;
    UINT    CalStatus_FlashADC;

    CalAWB_Struct   Cal_AWBStatistic;
    CalFlash_Struct Cal_Flash;
    UINT    Cal_DP_PreviewAddr;
    UINT    Cal_DP_CaptureAddr;
    UINT    Cal_DP_PreviewNum;
    UINT    Cal_DP_CaptureNum;
    UINT    Cal_OB;
    UINT    Cal_Mshutterr_Data[3];
    UINT    Cal_ISO_Data[3];
    UINT    Cal_Focus_FarTable[40];
    UINT    Cal_Focus_NearTable[40];
}Cal_Struct;
//#NT#2008/07/08#Bowen Li -end
/**
  Cal_SetCalibrationData
  Set calibration data
  @param UINT CalItem, Calibration status and items, please check enum "_CAL_Item"
  @param UINT  pCalData[], The pointer of detail calibration data array,
                            the content is different for dirfferent items.
                            It must communication with SensorLibrary.
  @return ER    Error Code,
*/
ER Cal_SetCalibrationData(UINT CalItem, //Calibration Status and Items, check _CAL_Item
                        UINT  pCalData[]);  //The related data of calibration items

/**
  Cal_GetCalibrationData
  Get calibration data
  @param void,
  @return Cal_Struct    retrun a pointer to calibration struct
*/

Cal_Struct* Cal_GetCalibrationData(void);//Get the Calibration Struct address
//ER Cal_WriteCalData(UINT  para);
//ER Cal_ReadCalData(UINT       para);

#endif
