/*
    CP2056 Touch panel controller interface

    CP2056 Touch panel controller interface

    @file       CP2056.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include <string.h>
#include "Type.h"
#include "pad.h"
#include "GPIO.h"
#include "Debug.h"
#include "Utility.h"
#include "TouchPanel.h"
//-------------------------------------------------------------------------------------------------
#define CP2056_STARTBIT             0x80
#define CP2056_MEASURE_Y            0x10
#define CP2056_MEASURE_X            0x50
#define CP2056_MEASURE_Z1           0x30
#define CP2056_MEASURE_Z2           0x40
#define CP2056_MODE_8BIT            0x08
#define CP2056_MODE_12BIT           0x00
#define CP2056_REF_SINGLE           0x04
#define CP2056_REF_DIFF             0x00
#define CP2056_PWR_DOWN             0x00
#define CP2056_PWR_REFOFF_ADCON     0x01
#define CP2056_PWR_REFON_ADCOFF     0x02
#define CP2056_PWR_FULL             0x03

#define CP2056_REF_MODE             CP2056_REF_DIFF
#define CP2056_PWR_MODE             CP2056_PWR_FULL

#define CP2056_MODE                 (CP2056_REF_MODE|CP2056_PWR_MODE)

// X, Y use 12 bits to have better precision
#define CP2056_GETX                 (CP2056_STARTBIT|CP2056_MEASURE_X|CP2056_MODE_12BIT|CP2056_MODE)
#define CP2056_GETY                 (CP2056_STARTBIT|CP2056_MEASURE_Y|CP2056_MODE_12BIT|CP2056_MODE)

// 8 bits is enough for pressure
#define CP2056_PRESSURE_8BIT        ENABLE
#if (CP2056_PRESSURE_8BIT == ENABLE)
#define CP2056_GETP1                (CP2056_STARTBIT|CP2056_MEASURE_Z1|CP2056_MODE_8BIT|CP2056_MODE)
#define CP2056_GETP2                (CP2056_STARTBIT|CP2056_MEASURE_Z2|CP2056_MODE_8BIT|CP2056_MODE)
#else //12-bit
#define CP2056_GETP1                (CP2056_STARTBIT|CP2056_MEASURE_Z1|CP2056_MODE_12BIT|CP2056_MODE)
#define CP2056_GETP2                (CP2056_STARTBIT|CP2056_MEASURE_Z2|CP2056_MODE_12BIT|CP2056_MODE)
#endif

#define CP2056_PRESSURE_TH          245
//-------------------------------------------------------------------------------------------------
static TP_INIT_STRUCT CP2056;
//-------------------------------------------------------------------------------------------------
__inline void cp2056_delay(void)
{
    TimerDelayUs(1);
    //do nothing
    
}
//-------------------------------------------------------------------------------------------------
static void cp2056_toggleClock(void)
{
    cp2056_delay();
    // DCLK high
    gpio_setPin(CP2056.Clk);

    cp2056_delay();
    // DCLK low
    gpio_clearPin(CP2056.Clk);
}
//-------------------------------------------------------------------------------------------------
static void cp2056_getPressure(
    UINT32*     pZ1,
    UINT32*     pZ2)
{
    UINT32 i;

    // CS, DCLK low
    gpio_clearPin(CP2056.Cs);
    gpio_clearPin(CP2056.Clk);

    cp2056_delay();

    // Output command
    for (i=8; i>0; i--)
    {
        if (((CP2056_GETP1 >> (i - 1)) & 0x01) == 0)
        {
            gpio_clearPin(CP2056.Out);
        }
        else
        {
            gpio_setPin(CP2056.Out);
        }

        // Toggle clock to produce one clock pulse
        cp2056_toggleClock();
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pZ1 = 0;

    for (i=14; i>0; i--)
    {
        cp2056_delay();

        // Output command
        if (i<9)
        {
            if (((CP2056_GETP2 >> (i - 1)) & 0x01) == 0)
            {
                gpio_clearPin(CP2056.Out);
            }
            else
            {
                gpio_setPin(CP2056.Out);
            }
        }

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        #if (CP2056_PRESSURE_8BIT == ENABLE)
        if (i > 6)
        {
            *pZ1 = *pZ1 | (gpio_getPin(CP2056.In) << (i - 7));
        }
        #else // 12-bit
        if (i > 2)
        {
            *pZ1 = *pZ1 | (gpio_getPin(CP2056.In) << (i - 3));
        }
        #endif

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pZ2 = 0;

    #if (CP2056_PRESSURE_8BIT == ENABLE)
    for (i=8; i>0; i--)
    #else // 12-bit
    for (i=12; i>0; i--)
    #endif
    {
        cp2056_delay();

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        *pZ2 = *pZ2 | (gpio_getPin(CP2056.In) << (i - 1));

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    cp2056_delay();

    // CS high
    gpio_setPin(CP2056.Cs);

    #if (CP2056_PRESSURE_8BIT != ENABLE) // 12-bit
    *pZ1 = (*pZ1)>>4;
    *pZ2 = (*pZ2)>>4;
    #endif
}
//-------------------------------------------------------------------------------------------------
static void cp2056_getPos(
    UINT32*     pX,
    UINT32*     pY)
{
    UINT32 i;

    // CS, DCLK low
    gpio_clearPin(CP2056.Cs);
    gpio_clearPin(CP2056.Clk);

    // Output command
    for (i=8; i>0; i--)
    {
        if (((CP2056_GETX >> (i - 1)) & 0x01) == 0)
        {
            gpio_clearPin(CP2056.Out);
        }
        else
        {
            gpio_setPin(CP2056.Out);
        }

        // Toggle clock to produce one clock pulse
        cp2056_toggleClock();
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pX = 0;

    for (i=14; i>0; i--)
    {
        cp2056_delay();

        // Output command
        if (i<9)
        {
            if (((CP2056_GETY >> (i - 1)) & 0x01) == 0)
            {
                gpio_clearPin(CP2056.Out);
            }
            else
            {
                gpio_setPin(CP2056.Out);
            }
        }

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        if (i > 2)
        {
            *pX = *pX | (gpio_getPin(CP2056.In) << (i - 3));
        }

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    // DOUT low
    gpio_clearPin(CP2056.Out);

    // Toggle clock to produce one clock pulse
    cp2056_toggleClock();

    // Read data
    *pY = 0;

    for (i=12; i>0; i--)
    {
        cp2056_delay();

        // DCLK high
        gpio_setPin(CP2056.Clk);

        // Read data
        *pY = *pY | (gpio_getPin(CP2056.In) << (i - 1));

        cp2056_delay();

        // DCLK low
        gpio_clearPin(CP2056.Clk);
    }

    cp2056_delay();

    // CS high
    gpio_setPin(CP2056.Cs);
}
//-------------------------------------------------------------------------------------------------
void tp_init(TP_INIT_STRUCT* pPin)
{
    memcpy((void*)&CP2056, pPin, sizeof(TP_INIT_STRUCT));

    // IO init
    gpio_setDir(CP2056.Cs, GPIO_DIR_OUTPUT);
    gpio_setPin(CP2056.Cs);
    gpio_setDir(CP2056.Clk, GPIO_DIR_OUTPUT);
    gpio_clearPin(CP2056.Clk);
    gpio_setDir(CP2056.Out, GPIO_DIR_OUTPUT);
    gpio_clearPin(CP2056.Out);
    gpio_setDir(CP2056.In, GPIO_DIR_INPUT);
    pad_setPullUpDown(CP2056.In, PAD_PULLUP);
}
//-------------------------------------------------------------------------------------------------
BOOL tp_getPos(
    UINT32*     pPosX,
    UINT32*     pPosY)
{
    BOOL    bIsPress;
    UINT32  uiTmp;

    cp2056_getPressure(pPosX,pPosY);

    if((*pPosY) < ((*pPosX) + CP2056_PRESSURE_TH))
    {
        bIsPress = TRUE;
    }
    else
    {
        bIsPress = FALSE;
    }

    cp2056_getPos(pPosX,pPosY);

    // X, Y change in this panel
    uiTmp = *pPosX;
    *pPosX = *pPosY;
    *pPosY = uiTmp;

    return bIsPress;
}
//-------------------------------------------------------------------------------------------------

