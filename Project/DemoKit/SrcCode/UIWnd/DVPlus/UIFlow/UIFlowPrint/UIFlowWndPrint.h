//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDPRINT_H
#define UIFLOWWNDPRINT_H

#include "UIFramework.h"
//-------------------------------------------------------------------------------------------------
#include "PTP_API.h"
#include "DPS_Header_Public.h"
#include "UIMenuWndPrint_Confirm.h"
//-------------------------------------------------------------------------------------------------
extern VControl UIFlowWndPrintCtrl;

extern VControl UIFlowWndPrint_PanelCtrl;

extern VControl UIFlowWndPrint_PicBridge_Icon_StaticCtrl;

typedef enum
{
UIFlowWndPrint_PicBridge_Icon_Static_ICON_PICTBRIDGE_S,
UIFlowWndPrint_PicBridge_Icon_Static_MAX
}UIFlowWndPrint_PicBridge_Icon_Static_Setting;
extern VControl UIFlowWndPrint_PicBridge_Text_StaticCtrl;

extern VControl UIFlowWndPrint_PicBridge_Text_StatusCtrl;

typedef enum
{
UIFlowWndPrint_PicBridge_Text_Status_STRID_PLEASE_WAIT,
UIFlowWndPrint_PicBridge_Text_Status_STRID_REMOVEUSBCABLE,
UIFlowWndPrint_PicBridge_Text_Status_STRID_NO_IMAGE,
UIFlowWndPrint_PicBridge_Text_Status_MAX
}UIFlowWndPrint_PicBridge_Text_Status_Setting;
//-------------------------------------------------------------------------------------------------
extern DPS_CAPABILITY_INFO UIPrintDpsCapa;
extern void UIPrint_ResetCappability(void);
extern void UIFlowWndPrint_OnPrintDpsEvent_ResetCapa(void);
extern void UIPrint_ShowNotification(
    UIMenuWndPrint_Confirm_STATE_ENUM   State,
    DPS_EVENT_ID                        EventId);
extern void UIPrint_AddDpsEvent(DPS_EVENT_ID EventId);
extern DPS_EVENT_ID UIPrint_GetDpsEvent(void);
extern void UIPrint_PostNextDpsEvent(void);
//-------------------------------------------------------------------------------------------------
#endif
