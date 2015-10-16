//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndSetupDateTimeRes.c"
#include "UIMenuWndSetupDateTime.h"
//#NT#2010/06/01#Chris Chung -begin
#include "UIMenuWndSetup.h"
#include "PrimaryTsk.h"
#include "UIMenuWndPhotoStill.h"
#include "UIMenuWndMovieVideo.h"
#include "UIMenuWndPlayback.h"
#include "UISystemObj.h"
#include "UIMenuSetupSettings.h"
#include "Utility.h"

#define DATETIME_AMPM_ENABLE    DISABLE

#define DATETIME_MAX_YEAR       2050
#define DATETIME_DEFAULT_YEAR   2010
#define DATETIME_DEFAULT_MONTH     1
#define DATETIME_DEFAULT_DAY       1
#define DATETIME_DEFAULT_HOUR      0
#define DATETIME_DEFAULT_MINUTE    0
#define DATETIME_DEFAULT_SECOND    0

/* (57, 53) is the parent's start (x,y). */
#define DATETIME_START_X        57
#define DATETIME_START_Y        53

#define DATETIME_SHIFT_X        20

typedef enum {
    UI_DATETIME_IDX_Y,
    UI_DATETIME_IDX_M,
    UI_DATETIME_IDX_D,
    UI_DATETIME_IDX_HR,
    UI_DATETIME_IDX_MIN,
    UI_DATETIME_IDX_SEC,
    UI_DATETIME_IDX_SWITCH
} _UI_DATETIME_IDX_;

//#NT#2010/06/01#Chris Chung -end

//---------------------UIMenuWndSetupDateTimeCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndSetupDateTime)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_Tab)
CTRL_LIST_END

//----------------------UIMenuWndSetupDateTimeCtrl Event---------------------------
INT32 UIMenuWndSetupDateTime_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_OnKeyMode(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupDateTime)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndSetupDateTime_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndSetupDateTime_OnClose)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndSetupDateTime_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndSetupDateTime_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndSetupDateTime_OnKeyMode)
EVENT_END

//#NT#2010/06/01#Chris Chung -begin
static UINT32 g_year=DATETIME_MAX_YEAR,g_month=DATETIME_DEFAULT_MONTH,g_day=DATETIME_DEFAULT_DAY;
static UINT32 g_hour=DATETIME_DEFAULT_HOUR,g_min=DATETIME_DEFAULT_MINUTE,g_second=DATETIME_DEFAULT_SECOND;
static char   itemY_Buf[32] = "0000";
static char   itemM_Buf[32] = "00";
static char   itemD_Buf[32] = "00";
static char   itemHR_Buf[32] = "00";
static char   itemMIN_Buf[32] = "00";
static char   itemSEC_Buf[32] = "00";
static char   itemAMPM_Buf[32] = "AM";
static char   itemSwitch_Buf[32] = "YY";
static char   itemOther0_Buf[32] = "/";
static char   itemOther2_Buf[32] = ":";
static char   itemOther4_Buf[32] = "/MM/DD";

static UINT32 UIMenuWndSetupDateTime_GetDayMax(void)
{
    UINT32  year, month, day_max;

    year  = g_year;
    month = g_month;

    switch (month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
        day_max = 30;
        break;
    case 2:
        day_max = ((year % 4) || (year == 2000)) ? 28 : 29;
        break;
    default:
        day_max = 31;
        break;
    }

    return (day_max);
}

static void UIMenuWndSetupDateTime_UpdateInfo(void)
{
    /* rect1_x: YYYY MM DD position */
    Ux_RECT rect1_1_1 = { 96-DATETIME_START_X,  61-DATETIME_START_Y, 163-DATETIME_START_X, 117-DATETIME_START_Y};
    Ux_RECT rect1_1_2 = {116-DATETIME_START_X,  61-DATETIME_START_Y, 163-DATETIME_START_X, 117-DATETIME_START_Y};
    Ux_RECT rect1_2   = {173-DATETIME_START_X,  61-DATETIME_START_Y, 212-DATETIME_START_X, 117-DATETIME_START_Y};
    Ux_RECT rect1_3   = {229-DATETIME_START_X,  61-DATETIME_START_Y, 268-DATETIME_START_X, 117-DATETIME_START_Y};
    /* rect2_x: HH:MI:SEC AM position */
    Ux_RECT rect2_1   = {116-DATETIME_START_X-DATETIME_SHIFT_X, 104-DATETIME_START_Y, 155-DATETIME_START_X-DATETIME_SHIFT_X, 160-DATETIME_START_Y};
    Ux_RECT rect2_2   = {152-DATETIME_START_X-DATETIME_SHIFT_X, 117-DATETIME_START_Y, 172-DATETIME_START_X-DATETIME_SHIFT_X, 146-DATETIME_START_Y};
    Ux_RECT rect2_3   = {173-DATETIME_START_X-DATETIME_SHIFT_X, 104-DATETIME_START_Y, 212-DATETIME_START_X-DATETIME_SHIFT_X, 160-DATETIME_START_Y};
    Ux_RECT rect2_4   = {208-DATETIME_START_X-DATETIME_SHIFT_X, 117-DATETIME_START_Y, 228-DATETIME_START_X-DATETIME_SHIFT_X, 146-DATETIME_START_Y};
    Ux_RECT rect2_5   = {229-DATETIME_START_X-DATETIME_SHIFT_X, 104-DATETIME_START_Y, 268-DATETIME_START_X-DATETIME_SHIFT_X, 160-DATETIME_START_Y};
    Ux_RECT rect2_6   = {268-DATETIME_START_X-DATETIME_SHIFT_X, 104-DATETIME_START_Y, 307-DATETIME_START_X-DATETIME_SHIFT_X, 160-DATETIME_START_Y};

    if(g_day > UIMenuWndSetupDateTime_GetDayMax())
        g_day = UIMenuWndSetupDateTime_GetDayMax();

    snprintf(itemY_Buf, 32, "%4d", g_year);
    snprintf(itemM_Buf, 32, "%2d", g_month);
    snprintf(itemD_Buf, 32, "%2d", g_day);
    #if (DATETIME_AMPM_ENABLE == ENABLE)
    {
        if(g_hour <= 12)
            snprintf(itemHR_Buf, 32, "%2d", g_hour);
        else
            snprintf(itemHR_Buf, 32, "%2d", g_hour-12);

        if(g_hour <= 11)
            snprintf(itemAMPM_Buf, 32, "AM");
        else
            snprintf(itemAMPM_Buf, 32, "PM");
    }
    #else
    snprintf(itemHR_Buf, 32, "%2d", g_hour);
    #endif
    snprintf(itemMIN_Buf, 32, "%2d", g_min);
    snprintf(itemSEC_Buf, 32, "%2d", g_second);

    switch(Get_DateFormatIndex())
    {
    case DATE_FORMAT_DMY:
        snprintf(itemSwitch_Buf, 32, "DD");
        snprintf(itemOther4_Buf, 32, "/MM/YY");
        {
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_DCtrl, rect1_1_2);
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_MCtrl, rect1_2);
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_YCtrl, rect1_3);
        }
        break;
    case DATE_FORMAT_MDY:
        snprintf(itemSwitch_Buf, 32, "MM");
        snprintf(itemOther4_Buf, 32, "/DD/YY");
        {
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_MCtrl, rect1_1_2);
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_DCtrl, rect1_2);
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_YCtrl, rect1_3);
        }
        break;
    case DATE_FORMAT_YMD:
        snprintf(itemSwitch_Buf, 32, "YY");
        snprintf(itemOther4_Buf, 32, "/MM/DD");
        {
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_YCtrl, rect1_1_1);
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_MCtrl, rect1_2);
            UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_DCtrl, rect1_3);
        }
        break;
    }

    #if (DATETIME_AMPM_ENABLE == ENABLE)
    UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_HRCtrl,           rect2_1);
    UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_VALUE_Other2Ctrl, rect2_2);
    UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_MINCtrl,          rect2_3);
    UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_VALUE_Other3Ctrl, rect2_4);
    UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_SECCtrl,          rect2_5);
    UxCtrl_SetPos(&UIMenuWndSetupDateTime_YMD_AMPMCtrl,         rect2_6);
    #endif

    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_YCtrl, 0, BTNITM_STRID, Txt_Pointer(itemY_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_MCtrl, 0, BTNITM_STRID, Txt_Pointer(itemM_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_DCtrl, 0, BTNITM_STRID, Txt_Pointer(itemD_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_HRCtrl, 0, BTNITM_STRID, Txt_Pointer(itemHR_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_MINCtrl, 0, BTNITM_STRID, Txt_Pointer(itemMIN_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_SECCtrl, 0, BTNITM_STRID, Txt_Pointer(itemSEC_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_AMPMCtrl, 0, BTNITM_STRID, Txt_Pointer(itemAMPM_Buf));
    UxButton_SetItemData(&UIMenuWndSetupDateTime_YMD_SwitchCtrl, 0, BTNITM_STRID, Txt_Pointer(itemSwitch_Buf));

    UxStatic_SetData(&UIMenuWndSetupDateTime_YMD_VALUE_Other0Ctrl, STATIC_VALUE, Txt_Pointer(itemOther0_Buf));
    UxStatic_SetData(&UIMenuWndSetupDateTime_YMD_VALUE_Other1Ctrl, STATIC_VALUE, Txt_Pointer(itemOther0_Buf));
    UxStatic_SetData(&UIMenuWndSetupDateTime_YMD_VALUE_Other2Ctrl, STATIC_VALUE, Txt_Pointer(itemOther2_Buf));
    UxStatic_SetData(&UIMenuWndSetupDateTime_YMD_VALUE_Other3Ctrl, STATIC_VALUE, Txt_Pointer(itemOther2_Buf));
    UxStatic_SetData(&UIMenuWndSetupDateTime_YMD_VALUE_Other4Ctrl, STATIC_VALUE, Txt_Pointer(itemOther4_Buf));
}
//#NT#2010/06/01#Chris Chung -end

INT32 UIMenuWndSetupDateTime_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2010/06/01#Chris Chung -begin
    RTC_DATE    Date;
    RTC_TIME    Time;

    Date = rtc_getDate();
    Time = rtc_getTime();
    g_year = Date.s.year;
    g_month = Date.s.month;
    g_day = Date.s.day;
    g_hour = Time.s.hour;
    g_min = Time.s.minute;
    g_second = Time.s.second;

    UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, 0);

    #if (DATETIME_AMPM_ENABLE == ENABLE)
    UxCtrl_SetShow(&UIMenuWndSetupDateTime_YMD_AMPMCtrl, TRUE);
    #else
    UxCtrl_SetShow(&UIMenuWndSetupDateTime_YMD_AMPMCtrl, FALSE);
    #endif

    UIMenuWndSetupDateTime_UpdateInfo();
    //#NT#2010/06/01#Chris Chung -end
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    switch(Primary_GetCurrentMode())
    {
    case PRIMARY_MODE_MOVIE:
        Ux_CloseWindow(&UIMenuWndMovieVideoCtrl,0);
        break;
    case PRIMARY_MODE_PHOTO:
        Ux_CloseWindow(&UIMenuWndPhotoStillCtrl,0);
        break;
    case PRIMARY_MODE_PLAYBACK:
        Ux_CloseWindow(&UIMenuWndPlaybackCtrl,0);
        break;
    }
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2009/08/19#Chris Chung -begin
    rtc_setDate(g_year, g_month, g_day);
    rtc_setTime(g_hour, g_min, g_second);
    Ux_CloseWindow(pCtrl,0);
    //#NT#2009/08/19#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndSetupDateTime_TabCtrl Event---------------------------
INT32 UIMenuWndSetupDateTime_Tab_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_Tab_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_Tab_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndSetupDateTime_Tab_OnKeyDown(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndSetupDateTime_Tab)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndSetupDateTime_Tab_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIMenuWndSetupDateTime_Tab_OnKeyRight)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndSetupDateTime_Tab_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndSetupDateTime_Tab_OnKeyDown)
EVENT_END

INT32 UIMenuWndSetupDateTime_Tab_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2010/05/19#Chris Chung -begin
    switch(Get_DateFormatIndex())
    {
    case DATE_FORMAT_DMY:
        switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
        {
        case UI_DATETIME_IDX_Y:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_M);
            break;
        case UI_DATETIME_IDX_M:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_D);
            break;
        case UI_DATETIME_IDX_D:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_SWITCH);
            break;
        case UI_DATETIME_IDX_HR:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_Y);
            break;
        default:
            Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
        }
        break;
    case DATE_FORMAT_MDY:
        switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
        {
        case UI_DATETIME_IDX_Y:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_D);
            break;
        case UI_DATETIME_IDX_M:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_SWITCH);
            break;
        case UI_DATETIME_IDX_D:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_M);
            break;
        case UI_DATETIME_IDX_HR:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_Y);
            break;
        default:
            Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
        }
        break;
    case DATE_FORMAT_YMD:
        switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
        {
        case UI_DATETIME_IDX_Y:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_SWITCH);
            break;
        default:
            Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
        }
    }
    UIMenuWndSetupDateTime_UpdateInfo();
    //#NT#2010/05/19#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_Tab_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2010/05/19#Chris Chung -begin
    switch(Get_DateFormatIndex())
    {
    case DATE_FORMAT_DMY:
        switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
        {
        case UI_DATETIME_IDX_Y:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_HR);
            break;
        case UI_DATETIME_IDX_M:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_Y);
            break;
        case UI_DATETIME_IDX_D:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_M);
            break;
        case UI_DATETIME_IDX_SWITCH:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_D);
            break;
        default:
            Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
        }
        break;
    case DATE_FORMAT_MDY:
        switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
        {
        case UI_DATETIME_IDX_Y:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_HR);
            break;
        case UI_DATETIME_IDX_M:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_D);
            break;
        case UI_DATETIME_IDX_D:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_Y);
            break;
        case UI_DATETIME_IDX_SWITCH:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_M);
            break;
        default:
            Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
        }
        break;
    case DATE_FORMAT_YMD:
        switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
        {
        case UI_DATETIME_IDX_SWITCH:
            UxTab_SetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS, UI_DATETIME_IDX_Y);
            break;
        default:
            Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
        }
    }
    UIMenuWndSetupDateTime_UpdateInfo();
    //#NT#2010/05/19#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_Tab_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2010/05/19#Chris Chung -begin
    switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
    {
    case UI_DATETIME_IDX_Y:
        if(g_year==DATETIME_MAX_YEAR)
            g_year = DATETIME_DEFAULT_YEAR;
        else
            g_year++;
        break;
    case UI_DATETIME_IDX_M:
        if(g_month==12)
            g_month=1;
        else
            g_month++;
        break;
    case UI_DATETIME_IDX_D:
        if(g_day==UIMenuWndSetupDateTime_GetDayMax())
            g_day=1;
        else
            g_day++;
        break;
    case UI_DATETIME_IDX_HR:
        if(g_hour==23)
            g_hour=0;
        else
            g_hour++;
        break;
    case UI_DATETIME_IDX_MIN:
        if(g_min==59)
            g_min=0;
        else
            g_min++;
        break;
    case UI_DATETIME_IDX_SEC:
        if(g_second==59)
            g_second=0;
        else
            g_second++;
        break;
    case UI_DATETIME_IDX_SWITCH:
        switch(Get_DateFormatIndex())
        {
        case DATE_FORMAT_DMY:
            Set_DateFormatIndex(DATE_FORMAT_MDY);
            break;
        case DATE_FORMAT_MDY:
            Set_DateFormatIndex(DATE_FORMAT_YMD);
            break;
        case DATE_FORMAT_YMD:
        default:
            Set_DateFormatIndex(DATE_FORMAT_DMY);
        }
        break;
    }

    UIMenuWndSetupDateTime_UpdateInfo();
    //#NT#2010/05/19#Chris Chung -end
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndSetupDateTime_Tab_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2010/05/19#Chris Chung -begin
    switch(UxTab_GetData(&UIMenuWndSetupDateTime_TabCtrl, TAB_FOCUS))
    {
    case UI_DATETIME_IDX_Y:
        if(g_year==DATETIME_DEFAULT_YEAR)
            g_year = DATETIME_MAX_YEAR;
        else
            g_year--;
        break;
    case UI_DATETIME_IDX_M:
        if(g_month==1)
            g_month=12;
        else
            g_month--;
        break;
    case UI_DATETIME_IDX_D:
        if(g_day==1)
            g_day=UIMenuWndSetupDateTime_GetDayMax();
        else
            g_day--;
        break;
    case UI_DATETIME_IDX_HR:
        if(g_hour==0)
            g_hour=23;
        else
            g_hour--;
        break;
    case UI_DATETIME_IDX_MIN:
        if(g_min==0)
            g_min=59;
        else
            g_min--;
        break;
    case UI_DATETIME_IDX_SEC:
        if(g_second==0)
            g_second=59;
        else
            g_second--;
        break;
    case UI_DATETIME_IDX_SWITCH:
        switch(Get_DateFormatIndex())
        {
        case DATE_FORMAT_DMY:
            Set_DateFormatIndex(DATE_FORMAT_YMD);
            break;
        case DATE_FORMAT_MDY:
            Set_DateFormatIndex(DATE_FORMAT_DMY);
            break;
        case DATE_FORMAT_YMD:
        default:
            Set_DateFormatIndex(DATE_FORMAT_MDY);
        }
        break;
    }

    UIMenuWndSetupDateTime_UpdateInfo();
    //#NT#2010/05/19#Chris Chung -end
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndSetupDateTime_YMD_YCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_Y)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_MCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_M)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_DCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_D)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_HRCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_HR)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_MINCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_MIN)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_SECCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_SEC)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_SwitchCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_Switch)
EVENT_END

//----------------------UIMenuWndSetupDateTime_Tab_YMD_VALUECtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_Tab_YMD_VALUE)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_VALUE_Other0Ctrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other0)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_VALUE_Other1Ctrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other1)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_VALUE_Other2Ctrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other2)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_VALUE_Other3Ctrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other3)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_VALUE_Other4Ctrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other4)
EVENT_END

//----------------------UIMenuWndSetupDateTime_YMD_AMPMCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndSetupDateTime_YMD_AMPM)
EVENT_END

