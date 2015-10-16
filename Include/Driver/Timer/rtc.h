/**
    @file       RTC.h
    @ingroup    mIHALRTC

    @brief      Header file for RTC module driver.

                Header file for RTC module driver.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _RTC_H
#define _RTC_H

#include "Type.h"

/**
    @addtogroup mIHALRTC
*/
//@{

/**
    RTC Date
*/
typedef union
{
    // Format conformed to RTC register
    __packed struct
    {
        UINT    day:5;      ///< Day
        UINT    month:4;    ///< Month
        UINT    year:12;    ///< Year
    } s;
    UINT32      value;      ///< Date value all together Y:M:D
} RTC_DATE;

/**
    RTC Time
*/
typedef union
{
    // Format conformed to RTC register
    __packed struct
    {
        UINT    second:6;   ///< Second
        UINT    minute:6;   ///< Minute
        UINT    hour:5;     ///< Hour
    } s;
    UINT32      value;      ///< Time value all together H:M:S
} RTC_TIME;

#define RTCKEY              0x4C48

//these should match register status bits
#define RTC_ALARM           0x00000001
#define RTC_CSET_DONE       0x00000100
#define RTC_SEC_INT         0x00000200
#define RTC_MIN_INT         0x00000400
#define RTC_HOUR_INT        0x00000800
//pseudo bits to combine two registers into one variable
#define RTC_PWR_SW_INT      0x01000000

// external function declaration
extern ER       rtc_open(void);
extern ER       rtc_close(void);

extern ER       rtc_enable(void);
extern void     rtc_disable(void);
extern void     rtc_reset(void);

extern UINT32   rtc_getKey(void);
extern void     rtc_setKey(UINT32 key);

extern void     rtc_setBaseDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern RTC_DATE rtc_getBaseDate(void);

extern UINT32   rtc_setDate(UINT32 Year, UINT32 Month, UINT32 Day);
extern RTC_DATE rtc_getDate(void);

extern UINT32   rtc_setTime(UINT32 Hour, UINT32 Minute, UINT32 Second);
extern RTC_TIME rtc_getTime(void);

extern UINT32   rtc_setAlarm(UINT32 Hour, UINT32 Minute, UINT32 Second);
extern RTC_TIME rtc_getAlarm(void);
extern BOOL     rtc_isAlarmTime(void);

extern void     rtc_enableInt(UINT32 ens);
extern void     rtc_disableInt(UINT32 dis);
extern UINT32   rtc_getIntEnabled(void);
extern UINT32   rtc_getIntStatus(void);

extern UINT32   rtc_setPWRAlarm(UINT32 uiHour, UINT32 uiMinute);
extern RTC_TIME rtc_getPWRAlarm(void);
extern void     rtc_disablePWRAlarm(void);
extern void     rtc_enablePWRAlarm(void);
extern BOOL     rtc_isPWRAlarmEnabled(void);
extern BOOL     rtc_isPWRAlarm(void);
extern void     rtc_clearPWRAlarmStatus(void);

extern BOOL     rtc_isLeapYear(UINT32 year);
extern UINT32   rtc_convertDate2Days(RTC_DATE date);
extern RTC_DATE rtc_convertDays2Date(UINT32 days);

extern BOOL     rtc_isPowerLost(void);

extern BOOL     rtc_isRegReady(void);
extern void     rtc_forceUpdate(void);

extern void     rtc_resetShutdownTimer(void);
extern void     rtc_poweroffPWR(void);

extern BOOL     rtc_getPWRStatus(void);

//@}

#endif
