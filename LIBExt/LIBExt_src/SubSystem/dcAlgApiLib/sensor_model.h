
/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2004.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Sensor Library Header File                                                 *
* Description: Use this file to select which sensor library want to be used.                                                          *
* Author: Adrian                                                               *
****************************************************************************/

#ifndef _SENSOR_MODEL_H
#define _SENSOR_MODEL_H
#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "sie_lib.h"
#include "pre_lib.h"
#include "ipe_lib.h"
#include "ime_lib.h"
#include "aaa_lib.h"
#include "fde_lib.h"
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
#include "dis_lib.h"
#include "dis_common.h"
//#NT#2009/11/27#Connie Yu -end
#include "IPPManager.h"
#include "sen_common.h"
#include "ImageAlgInfor.h"
#include "Alg_IPLMode.h"
#include "IPPBuffer.h"
//#NT#2010/05/28#ethanlau -begin
//#NT#add for ime chroma filter
#include "grph.h"
//#NT#2010/05/28#ethanlau -end
#if 0//Panasonic_10M_630
    #include    "ae_Panasonic10M_630.h"
    #include    "awb_Panasonic10M_630.h"
    #include    "sie_Panasonic10M_630.h"
    #include    "SensorSys_Panasonic10M_630.h"
    #include    "flashlight_Panasonic10M_630.h"
    #include    "IQSetting_Panasonic10M_630.h"
    #include    "dzoom_Panasonic10M_630.h"
    #include    "isr_Panasonic10M_630.h"
    #include    "tg_MN12081A_630.h"
    #include    "SensorFlowFunc_Panasonic10M_630.h"
#elif 0//MI1030_630
    #include    "ae_MI1030_630.h"
    #include    "awb_MI1030_630.h"
    #include    "sie_MI1030_630.h"
    #include    "SensorSys_MI1030_630.h"
    #include    "flashlight_MI1030_630.h"
    #include    "IQSetting_MI1030_630.h"
    #include    "dzoom_MI1030_630.h"
    #include    "isr_MI1030_630.h"
    #include    "MI1030.h"
    #include    "SensorFlowFunc_MI1030_630.h"
#elif 0//MI5100_630
    #include    "ae_MI5100_630.h"
    #include    "awb_MI5100_630.h"
    #include    "sie_MI5100_630.h"
    #include    "SensorSys_MI5100_630.h"
    #include    "flashlight_MI5100_630.h"
    #include    "IQSetting_MI5100_630.h"
    #include    "dzoom_MI5100_630.h"
    #include    "isr_MI5100_630.h"
    #include    "MI5100.h"
    #include    "SensorFlowFunc_MI5100_630.h"
#elif 0//MI5100_TVP5150_630
    #include    "ae_MI5100_TVP5150_630.h"
    #include    "awb_MI5100_TVP5150_630.h"
    #include    "sie_MI5100_TVP5150_630.h"
    #include    "SensorSys_MI5100_TVP5150_630.h"
    #include    "flashlight_MI5100_TVP5150_630.h"
    #include    "IQSetting_MI5100_TVP5150_630.h"
    #include    "dzoom_MI5100_TVP5150_630.h"
    #include    "isr_MI5100_TVP5150_630.h"
    #include    "MI5100.h"
    #include    "SensorFlowFunc_MI5100_TVP5150_630.h"
#elif 0//Panasonic10M_34550_630
    #include    "ae_Panasonic10M_34550_630.h"
    #include    "awb_Panasonic10M_34550_630.h"
    #include    "sie_Panasonic10M_34550_630.h"
    #include    "SensorSys_Panasonic10M_34550_630.h"
    #include    "flashlight_Panasonic10M_34550_630.h"
    #include    "IQSetting_Panasonic10M_34550_630.h"
    #include    "dzoom_Panasonic10M_34550_630.h"
    #include    "isr_Panasonic10M_34550_630.h"
    #include    "tg_ADDI9004_34550_630.h"
    #include    "SensorFlowFunc_Panasonic10M_34550_630.h"
#elif 0//OV9710_630
    #include    "ae_OV9710_630.h"
    #include    "awb_OV9710_630.h"
    #include    "sie_OV9710_630.h"
    #include    "SensorSys_OV9710_630.h"
    #include    "flashlight_OV9710_630.h"
    #include    "IQSetting_OV9710_630.h"
    #include    "dzoom_OV9710_630.h"
    #include    "isr_OV9710_630.h"
    #include    "OV9710_630.h"
    #include    "SensorFlowFunc_OV9710_630.h"
#elif 0//OV9710_DUAL_630
    #include    "ae_OV9710_Dual_630.h"
    #include    "awb_OV9710_Dual_630.h"
    #include    "sie_OV9710_Dual_630.h"
    #include    "SensorSys_OV9710_Dual_630.h"
    #include    "flashlight_OV9710_Dual_630.h"
    #include    "IQSetting_OV9710_Dual_630.h"
    #include    "dzoom_OV9710_Dual_630.h"
    #include    "isr_OV9710_Dual_630.h"
    #include    "OV9710_Dual_630.h"
    #include    "SensorFlowFunc_OV9710_Dual_630.h"
//#NT#20090901 ethanlau -begin
#elif 0//OV5653_630
    #include    "ae_OV5653_630.h"
    #include    "awb_OV5653_630.h"
    #include    "sie_OV5653_630.h"
    #include    "SensorSys_OV5653_630.h"
    #include    "flashlight_OV5653_630.h"
    #include    "IQSetting_OV5653_630.h"
    #include    "dzoom_OV5653_630.h"
    #include    "isr_OV5653_630.h"
    #include    "OV5653.h"
    #include    "SensorFlowFunc_OV5653_630.h"
//#nt#20090901 ethanlau -end
//#NT#20100410 ethanlau -begin
#elif 0//OV5633_630
    #include    "ae_OV5633_630.h"
    #include    "awb_OV5633_630.h"
    #include    "sie_OV5633_630.h"
    #include    "SensorSys_OV5633_630.h"
    #include    "flashlight_OV5633_630.h"
    #include    "IQSetting_OV5633_630.h"
    #include    "dzoom_OV5633_630.h"
    #include    "isr_OV5633_630.h"
    #include    "OV5633_630.h"
    #include    "SensorFlowFunc_OV5633_630.h"
//#nt#20100410 ethanlau -end
//#NT#2010/05/26#ethanlau -begin
#elif 0//MI5130_630
    #include    "ae_MI5130_630.h"
    #include    "awb_MI5130_630.h"
    #include    "sie_MI5130_630.h"
    #include    "SensorSys_MI5130_630.h"
    #include    "flashlight_MI5130_630.h"
    #include    "IQSetting_MI5130_630.h"
    #include    "dzoom_MI5130_630.h"
    #include    "isr_MI5130_630.h"
    #include    "MI5130.h"
    #include    "SensorFlowFunc_MI5130_630.h"
//#NT#2010/05/26#ethanlau -end
#elif 0//A1040_630
    #include    "ae_A1040_630.h"
    #include    "awb_A1040_630.h"
    #include    "af_A1040_630.h"
    #include    "sie_A1040_630.h"
    #include    "SensorSys_A1040_630.h"
    #include    "flashlight_A1040_630.h"
    #include    "IQSetting_A1040_630.h"
    #include    "dzoom_A1040_630.h"
    #include    "isr_A1040_630.h"
    #include    "A1040_630.h"
    #include    "SensorFlowFunc_A1040_630.h"
#elif 0//NT99140_632
    #include    "ae_NT99140_632.h"
    #include    "awb_NT99140_632.h"
    #include    "sie_NT99140_632.h"
    #include    "SensorSys_NT99140_632.h"
    #include    "flashlight_NT99140_632.h"
    #include    "IQSetting_NT99140_632.h"
    #include    "dzoom_NT99140_632.h"
    #include    "isr_NT99140_632.h"
    #include    "NT99140.h"
    #include    "SensorFlowFunc_NT99140_632.h"
#elif 1//NT99141_632
    #include    "ae_NT99141_632.h"
    #include    "awb_NT99141_632.h"
    #include    "sie_NT99141_632.h"
    #include    "SensorSys_NT99141_632.h"
    #include    "flashlight_NT99141_632.h"
    #include    "IQSetting_NT99141_632.h"
    #include    "dzoom_NT99141_632.h"
    #include    "isr_NT99141_632.h"
    #include    "NT99141.h"
    #include    "SensorFlowFunc_NT99141_632.h"
#elif 0//ZT3150_630
    #include    "ae_ZT3150_630.h"
    #include    "awb_ZT3150_630.h"
    #include    "sie_ZT3150_630.h"
    #include    "SensorSys_ZT3150_630.h"
    #include    "flashlight_ZT3150_630.h"
    #include    "IQSetting_ZT3150_630.h"
    #include    "dzoom_ZT3150_630.h"
    #include    "isr_ZT3150_630.h"
    #include    "ZT3150.h"
    #include    "SensorFlowFunc_ZT3150_630.h"
#elif 0//ZT3120_630
    #include    "ae_ZT3120_630.h"
    #include    "awb_ZT3120_630.h"
    #include    "sie_ZT3120_630.h"
    #include    "SensorSys_ZT3120_630.h"
    #include    "flashlight_ZT3120_630.h"
    #include    "IQSetting_ZT3120_630.h"
    #include    "dzoom_ZT3120_630.h"
    #include    "isr_ZT3120_630.h"
    #include    "ZT3120.h"
    #include    "SensorFlowFunc_ZT3120_630.h"    
#endif
    #include    "Lens.h"
#endif


