/*
    Novatek Touch panel controller interface

    Novatek Touch panel controller interface

    @file       TP_NVT.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include <string.h>
#include "Kernel.h"
#include "Type.h"
#include "pad.h"
#include "adc.h"
#include "GPIO.h"
#include "Debug.h"
#include "Utility.h"
#include "TouchPanel.h"

#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif

//-------------------------------------------------------------------------------------------------
#define TPNVT_PRESSURE_TH           245

/*
#define TP_HW_MAX_X        3562
#define TP_HW_MAX_Y        3180
#define TP_HW_MIN_X        670
#define TP_HW_MIN_Y        1062

#define TP_PANEL_X1          (-40)   //left
#define TP_PANEL_Y1          0       //top
#define TP_PANEL_X2          319     //right
#define TP_PANEL_Y2          239     //bottom

#define TP_PANEL_W          (TP_PANEL_X2-TP_PANEL_X1 + 1)
#define TP_PANEL_H          (TP_PANEL_Y2-TP_PANEL_Y1 + 1)
*/

#define TPNVT_SAMPLE_NUM            4

//-------------------------------------------------------------------------------------------------
//static TP_INIT_STRUCT TPNVT;
//-------------------------------------------------------------------------------------------------
__inline void TPNVT_delay(void)
{
    TimerDelayUs(1);
    //do nothing
    
}
//-------------------------------------------------------------------------------------------------
static void TPNVT_getPressure(UINT16* pZ1, UINT16* pZ2)
{
    adc_stopTP();
    adc_setTPMode(TP_MODE1, TP_PRE, FALSE, NULL);
    adc_startTP();
    while(!adc_isTPDataReady());
    adc_readTPPressure(pZ1, pZ1);
    adc_clearTPDataReady();
    DEBUG_MSG(("[%s]: Z1 %d, Z2 %d\r\n", __func__, *pZ1, *pZ2));
}
//-------------------------------------------------------------------------------------------------
static BOOL TPNVT_isPenDown(void)
{
    adc_setTPMode(TP_MODE1, TP_PD, FALSE, NULL);
    adc_startTP();
    TimerDelayMs(1);
    if(adc_isTPPD())
    {
        DEBUG_MSG(("Pen Down\r\n"));
        adc_clearTPPDSts();
        return TRUE;
    }
    else
    {
        DEBUG_MSG(("Pen Up\r\n"));
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------------
static void TPNVT_getPos(UINT32* pX, UINT32* pY)
{
    INT32   i, j, k;
    UINT32  xsum, ysum, temp;
    UINT16  x[TPNVT_SAMPLE_NUM] = {0}, y[TPNVT_SAMPLE_NUM] = {0};

    temp = 0;
    xsum = ysum = 0;

    for (i = 0; i < TPNVT_SAMPLE_NUM; i++)
    {
        adc_stopTP();
        adc_setTPMode(TP_MODE1, TP_POS, FALSE, NULL);
        adc_startTP();
        while(!adc_isTPDataReady());

        adc_readTPXY(&x[i], &y[i]);
    }

    //ignore the max and min to calculate average
    if (TPNVT_SAMPLE_NUM > 3)
    {
        //sorting x
        for (j = 0; j < TPNVT_SAMPLE_NUM-1; ++j)
        {
            for (k = j+1; k < TPNVT_SAMPLE_NUM; ++k)
            {
                if(x[j] > x[k])
                {
                    temp = x[j];
                    x[j] = x[k];
                    x[k] = temp;
                }
            }
        }

        //sorting y
        for (j = 0; j < TPNVT_SAMPLE_NUM-1; ++j)
        {
            for (k = j+1; k < TPNVT_SAMPLE_NUM; ++k)
            {
                if(y[j] > y[k])
                {
                    temp = y[j];
                    y[j] = y[k];
                    y[k] = temp;
                }
            }
        }

        for (i = 1; i < TPNVT_SAMPLE_NUM-1; i++)
        {
            xsum += x[i];
            ysum += y[i];
        }

        *pX = xsum/(TPNVT_SAMPLE_NUM-2);
        *pY = ysum/(TPNVT_SAMPLE_NUM-2);
    }
    else if (TPNVT_SAMPLE_NUM > 0)//just take average
    {
        for (i = 0; i < TPNVT_SAMPLE_NUM; i++)
        {
            xsum += x[i];
            ysum += y[i];
        }
        *pX = xsum/(TPNVT_SAMPLE_NUM);
        *pY = ysum/(TPNVT_SAMPLE_NUM);
    }

    for (i = 0; i < TPNVT_SAMPLE_NUM; i++)
    {
        DEBUG_MSG(("[TPNVT_getPos] %d: (%d, %d)\r\n", i, x[i], y[i]));
    }
}
//-------------------------------------------------------------------------------------------------
void tp_init(TP_INIT_STRUCT* pPin)
{
    if (adc_open(ADC_CHANNEL_6) != E_OK)
    {
        debug_err(("^RCan't open ADC ch6 for Touch\r\n"));
        return;
    }
    if (adc_open(ADC_CHANNEL_7) != E_OK)
    {
        debug_err(("^RCan't open ADC ch7 for Touch\r\n"));
        return;
    }

    adc_setTPControl(0xA0/*uiGsr*/, 0xA/*uiRes*/, 0x5/*uiSampCyc*/);
}
//-------------------------------------------------------------------------------------------------
BOOL tp_getPos(UINT32* pPosX, UINT32* pPosY)
{
    BOOL    bIsPress;
    UINT16  Z1, Z2; // pressure value
    UINT32  uiTmp;

    #if 0
    TPNVT_getPressure(&Z1, &Z2);

    if(Z2 < (Z1 + TPNVT_PRESSURE_TH))
    {
        bIsPress = TRUE;
    }
    else
    {
        bIsPress = FALSE;
    }
    #else
    bIsPress = TPNVT_isPenDown();
    #endif

    TPNVT_getPos(pPosX, pPosY);

    #if 1
    // X, Y change in this panel
    uiTmp = *pPosX;
    *pPosX = *pPosY;
    *pPosY = uiTmp;
    #endif

    return bIsPress;
}
//-------------------------------------------------------------------------------------------------

