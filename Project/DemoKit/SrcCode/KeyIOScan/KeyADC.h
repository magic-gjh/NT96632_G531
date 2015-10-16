/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyADC.h
    @ingroup    mIPRJAPKeyIO

    @brief      Header file of key detection by using ADC
                Header file of key detection by using ADC

    @note       Nothing.

    @date       2005/08/24
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#ifndef _KEYADC_H
#define _KEYADC_H

#include "SysCfg.h"

// ADC channel for key group 1 * group 2 detection
#if (_PCB_ == _PCB_HD500C7_REAL_ || _PCB_ == _PCB_HD500C7_TOUCH_ || _PCB_ == _PCB_HD500C6_REAL_)
#define KEY_ADC_CH_1            ADC_CHANNEL_1
#define KEY_ADC_CH_2            ADC_CHANNEL_3
#else    
#define KEY_ADC_CH_1            ADC_CHANNEL_1
#define KEY_ADC_CH_2            ADC_CHANNEL_2
#endif


#define VOLDET_KEY_LVL_KEYA      0
#define VOLDET_KEY_LVL_KEYB      1
#define VOLDET_KEY_LVL_SHUTTER1  2
#define VOLDET_KEY_LVL_UP        3
#define VOLDET_KEY_LVL_DOWN      4
#define VOLDET_KEY_LVL_LEFT      5
#define VOLDET_KEY_LVL_RIGHT     6
#define VOLDET_KEY_LVL_MENU      7
#define VOLDET_KEY_LVL_MODE      8
#define VOLDET_KEY_LVL_REC       9
#define VOLDET_KEY_LVL_ENTER     10
#define VOLDET_KEY_LVL_ZOOMIN    11
#define VOLDET_KEY_LVL_ZOOMOUT   12
#define VOLDET_KEY_LVL_DELETE    13
#define VOLDET_KEY_LVL_SHUTTER2  14
#define VOLDET_KEY_LVL_PLAYBACK  15
#define VOLDET_KEY_LVL_NULL      16

extern void     KeyADC_Init(void);
extern UINT32   KeyADC_GetKey(void);
#endif
//@}
