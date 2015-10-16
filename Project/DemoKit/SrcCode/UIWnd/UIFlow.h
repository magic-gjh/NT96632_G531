/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIFlow.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Flow Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIFLOW_H
#define _UIFLOW_H

#include "SysCfg.h"
//UIApp
#include "UIPhotoObj.h"
#include "UIMovieObj.h"
#include "UIPlayObj.h"
#include "UISystemObj.h"
#include "UIPrintObj.h"
//UIFlow
#include "UIFlowWndPhoto.h"
#include "UIFlowWndMovie.h"
#include "UIFlowWndPlay.h"
#include "MediaRecAPI.h"

extern ER FlowPhoto_Open(void);
extern ER FlowPhoto_Close(void);

extern ER FlowPB_Open(void);
extern ER FlowPB_Close(void);

extern ER FlowMovie_Open(void);
extern ER FlowMovie_Close(void);

extern ER FlowMP3_Open(void);
extern ER FlowMP3_Close(void);

extern ER FlowWrnBatteryEmpty_Open(void);
extern ER FlowWrnBatteryEmpty_Close(void);

extern ER FlowWrnStrgHotPlug_Open(void);
extern ER FlowWrnStrgHotPlug_Close(void);

extern void FlowDCF_UpdateName(BOOL IsJPG);
extern ER MenuUSB_Open(void);
extern ER MenuUSB_Close(void);

extern ER MenuPrint_Open(void);
extern ER MenuPrint_Close(void);

extern ER MenuCalibration_Open(void);
extern ER MenuCalibration_Close(void);

extern ER MenuPCB_Open(void);
extern ER MenuPCB_Close(void);

#endif
