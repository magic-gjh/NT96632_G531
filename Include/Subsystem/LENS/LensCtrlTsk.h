/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensCtrlTsk.h
    @ingroup    mISYSLens

    @brief      Lens control task functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENSCTRLTSK_H
#define _LENSCTRLTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "Lens.h"
#include "OSCommon.h"

//
//FLG_ID_LENS
//
#define FLGLENS_ZOOMFWD                 0x00000001
#define FLGLENS_ZOOMREV                 0x00000002
#define FLGLENS_ZOOMBRK                 0x00000004
#define FLGLENS_ZOOMOFF                 0x00000008
#define FLGLENS_ZOOMCHANGE              0x00000010
#define FLGLENS_FOCUSFWD                0x00000020
#define FLGLENS_FOCUSREV                0x00000040
#define FLGLENS_APERTUREMOVE            0x00000080
#define FLGLENS_SHUTTERMOVE             0x00000100
#define FLGLENS_IDLE                    0x00000200
#define FLGLENS_POWEROFF                0x00000400

// Wait time
#define LENSCTRL_WAIT_NOWAIT            0x00
#define LENSCTRL_WAIT_INFINITE          0xFF

typedef void (*FPLENSCALLBACK)(UINT32 MessageID, AlgMsgInfo * pAlgMsg_infor);

typedef struct
{
    UINT32          uiMemoryAddr;
    UINT32          uiMemorySize;
    PLENS_TAB       pLens;
    PMOTOR_TAB      pMotor;
    FPLENSCALLBACK  fpLensCB;
} LENSCTRL_APPOBJ, *PLENSCTRL_APPOBJ;

typedef struct
{
    UINT32          uiZoomStep;
    BOOL            bWaitZoomFinish;
    UINT32          uiFocusStep;
    UINT32          uiApertureState;
    UINT32          uiShutterState;
} LENSCTRL_PARAMOBJ, *PLENSCTRL_PARAMOBJ;

extern ER   LensCtrl_Open(PLENSCTRL_APPOBJ pLensCtrlObj);
extern ER   LensCtrl_Close(void);
extern PLENSCTRL_APPOBJ LensCtrl_Get(void);
extern ER   LensCtrl_RegCB(FPLENSCALLBACK cb);
extern BOOL LensCtrl_Set(PLENSCTRL_APPOBJ pLensCtrlObj);
extern ER   LensCtrl_SetParam(PLENSCTRL_PARAMOBJ pLensCtrlParamObj);
extern ER   LensCtrl_WaitCmdFinish(UINT32 TimeOut);

#endif
