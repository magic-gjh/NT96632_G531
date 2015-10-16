/*
    Detect touch panel gesture

    Detect touch panel gesture

    @file       Gesture.c
    @ingroup    mISYSGesture
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Kernel.h"
#include "Type.h"
#include "Debug.h"
#include "Utility.h"
#include "TouchPanel.h"
#include "Gesture_Public.h"
#include "Gesture_Private.h"
//-------------------------------------------------------------------------------------------------
static GESTURE_INFO_STRUCT Gesture;
//-------------------------------------------------------------------------------------------------
void Gesture_AddEvent(GESTURE_EVENT_ENUM  EventId)
{
    GESTURE_MSG(("GESTURE_M:AddEvent:Count=%d,EventId=%d\r\n",Gesture.EventQueue.Count,EventId));

    if(Gesture.EventQueue.Count < GESTURE_EVENT_QUEUE_AMOUNT)
    {
        Gesture.EventQueue.Event[Gesture.EventQueue.Write]=EventId;

        Gesture.EventQueue.Write++;
        if(Gesture.EventQueue.Write == GESTURE_EVENT_QUEUE_AMOUNT)
        {
           Gesture.EventQueue.Write = 0; 
        }

        Gesture.EventQueue.Count++;
    }
}
//-------------------------------------------------------------------------------------------------
GESTURE_EVENT_ENUM Gesture_GetEvent(void)
{
    GESTURE_EVENT_ENUM TempEvent = GESTURE_EVENT_IDLE;

    if(Gesture.EventQueue.Count > 0)
    {
        TempEvent = Gesture.EventQueue.Event[Gesture.EventQueue.Read];

        Gesture.EventQueue.Read++;
        if(Gesture.EventQueue.Read == GESTURE_EVENT_QUEUE_AMOUNT)
        {
           Gesture.EventQueue.Read = 0; 
        }

        Gesture.EventQueue.Count--;
    }
    GESTURE_MSG(("GESTURE_M:GetEvent:Count=%d,EventId=%d\r\n",Gesture.EventQueue.Count,TempEvent));

    return TempEvent;
}
//-------------------------------------------------------------------------------------------------
void Gesture_MappingLCD(
    UINT32*     pPosX,
    UINT32*     pPosY)
{
    GESTURE_MSG(("GESTURE_M:MappingLCD:PosX=%d,PosY=%d,RX=%d,RY=%d,LCDW=%d,LCDH=%d,MinX=%d,MinY=%d,MaxX=%d,MaxY=%d\r\n",(*pPosX),(*pPosY),Gesture.CalData.ReverseX,Gesture.CalData.ReverseY,Gesture.LcdInfo.Width,Gesture.LcdInfo.Height,Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));

    if((*pPosX) > Gesture.CalData.MaxX)
    {
        (*pPosX) = Gesture.CalData.MaxX;
    }
    else
    if((*pPosX) < Gesture.CalData.MinX)
    {
        (*pPosX) = Gesture.CalData.MinX;
    }

    if((*pPosY) > Gesture.CalData.MaxY)
    {
        (*pPosY) = Gesture.CalData.MaxY;
    }
    else
    if((*pPosY) < Gesture.CalData.MinY)
    {
        (*pPosY) = Gesture.CalData.MinY;
    }

    if(Gesture.CalData.ReverseX)
    {
        (*pPosX) = Gesture.LcdInfo.Width - (Gesture.LcdInfo.Width * ((*pPosX) - Gesture.CalData.MinX)/(Gesture.CalData.MaxX - Gesture.CalData.MinX));
    }
    else
    {
        (*pPosX) = Gesture.LcdInfo.Width * ((*pPosX) - Gesture.CalData.MinX)/(Gesture.CalData.MaxX - Gesture.CalData.MinX);
    }

    if(Gesture.CalData.ReverseY)
    {
        (*pPosY) = Gesture.LcdInfo.Height - (Gesture.LcdInfo.Height * ((*pPosY) - Gesture.CalData.MinY)/(Gesture.CalData.MaxY - Gesture.CalData.MinY));
    }
    else
    {
        (*pPosY) = Gesture.LcdInfo.Height * ((*pPosY) - Gesture.CalData.MinY)/(Gesture.CalData.MaxY - Gesture.CalData.MinY);
    }

    if((*pPosX) > 0)
    {
        (*pPosX) -= 1;
    }

    if((*pPosY) > 0)
    {
        (*pPosY) -= 1;
    }

    GESTURE_MSG(("GESTURE_M:MappingLCD:Out:X=%d, Y=%d\r\n",(*pPosX),(*pPosY)));
}
//-------------------------------------------------------------------------------------------------
GESTURE_EVENT_ENUM Gesture_GetGestureSlide(void)
{
    UINT32  MoveDistanceX, MoveDistanceY;
    GESTURE_EVENT_ENUM   Event = GESTURE_EVENT_IDLE;

    MoveDistanceX = abs(Gesture.Move.EndPosX-Gesture.Move.StartPosX);
    MoveDistanceY = abs(Gesture.Move.EndPosY-Gesture.Move.StartPosY);

    if(MoveDistanceX > Gesture.Threshold.SlideRange)
    {
        if(Gesture.Move.EndPosX >= Gesture.Move.StartPosX)
        {
            Event = GESTURE_EVENT_SLIDE_RIGHT;
        }
        else
        {
            Event = GESTURE_EVENT_SLIDE_LEFT;
        }

        if(MoveDistanceX < MoveDistanceY)
        {
            if(Gesture.Move.EndPosY >= Gesture.Move.StartPosY)
            {
                Event = GESTURE_EVENT_SLIDE_DOWN;
            }
            else
            {
                Event = GESTURE_EVENT_SLIDE_UP;
            }
        }
    }
    else
    if(MoveDistanceY > Gesture.Threshold.SlideRange)
    {
        if(Gesture.Move.EndPosY >= Gesture.Move.StartPosY)
        {
            Event = GESTURE_EVENT_SLIDE_DOWN;
        }
        else
        {
            Event = GESTURE_EVENT_SLIDE_UP;
        }

        if(MoveDistanceY < MoveDistanceX)
        {
            if(Gesture.Move.EndPosX >= Gesture.Move.StartPosX)
            {
                Event = GESTURE_EVENT_SLIDE_RIGHT;
            }
            else
            {
                Event = GESTURE_EVENT_SLIDE_LEFT;
            }
        }
    }

    GESTURE_MSG(("GESTURE_M:GetGestureMove:DisX=%d, DisY=%d, SX=%d, SY=%d, EX=%d, EY=%d, Event=%d\r\n",MoveDistanceX,MoveDistanceY,Gesture.Move.StartPosX,Gesture.Move.StartPosY,Gesture.Move.EndPosX,Gesture.Move.EndPosY,Event));

    if(Event != GESTURE_EVENT_IDLE)
    {
        Gesture_AddEvent(Event);
    }

    return Event;
}
//-------------------------------------------------------------------------------------------------
void Gesture_GetGesture(
    BOOL        IsPress,
    UINT32      PosX,
    UINT32      PosY)
{
    Gesture.Time.Count++;

    if(IsPress)
    {
        if(!Gesture.Status.Press)
        {
            // First Touch panel press
            Gesture.Status.Press = IsPress;
            Gesture.Time.LastPress = Gesture.Time.Count;
            Gesture.Move.StartPosX = PosX;
            Gesture.Move.StartPosY = PosY;
            Gesture.Move.EndPosX = PosX;
            Gesture.Move.EndPosY = PosY;

            Gesture.Gesture.Event = GESTURE_EVENT_PRESS;
        }
        else
        {
            if( !(Gesture.Status.Hold) &&
                abs(Gesture.Move.StartPosX-PosX < Gesture.Threshold.HoldRange) &&
                abs(Gesture.Move.StartPosY-PosY < Gesture.Threshold.HoldRange))
            {
                if((Gesture.Time.Count-Gesture.Time.LastPress) > Gesture.Threshold.HoldTime)
                {
                    Gesture.Status.Hold = TRUE;
                    Gesture.Gesture.Event = GESTURE_EVENT_HOLD;
                }
                else
                {
                    Gesture.Gesture.Event = GESTURE_EVENT_PRESS;
                }
            }
            else
            {
                if( Gesture.Move.EndPosX != PosX ||
                    Gesture.Move.EndPosY != PosY)
                {
                    Gesture.Gesture.Event = GESTURE_EVENT_MOVE;
                }
                else
                {
                    Gesture.Gesture.Event = GESTURE_EVENT_PRESS;
                }
            }

            Gesture.Move.EndPosX = PosX;
            Gesture.Move.EndPosY = PosY;
        }

        Gesture.Gesture.PosX = PosX;
        Gesture.Gesture.PosY = PosY;
    }
    else
    {
        if(Gesture.Status.Press)
        {
            // First touch panel release
            Gesture.Status.Press = IsPress;

            if(Gesture_GetGestureSlide() == GESTURE_EVENT_IDLE)
            {
                if(!(Gesture.Status.Hold))
                {
                    Gesture_AddEvent(GESTURE_EVENT_CLICK);
                }

                if((Gesture.Time.Count-Gesture.Time.LastRelease) < Gesture.Threshold.DoubleClickTime)
                {
                    Gesture_AddEvent(GESTURE_EVENT_DOUBLE_CLICK);
                }
            }

            Gesture.Status.Hold = FALSE;
            Gesture.Gesture.Event = GESTURE_EVENT_RELEASE;
            Gesture.Time.LastRelease = Gesture.Time.Count;
        }
        else
        {
            Gesture.Gesture.Event = Gesture_GetEvent();
        }
    }

    GESTURE_MSG(("GESTURE_M:GetGesture:IsPress=%d, X=%d, Y=%d, Event=%d\r\n",IsPress,PosX,PosY,Gesture.Gesture.Event));
}
//-------------------------------------------------------------------------------------------------
void Gesture_DetGesture(GESTURE_GESTURE_STRUCT* pGesture)
{
    BOOL    IsPress;
    UINT32  PosX, PosY;

    GESTURE_MSG(("GESTURE_M:DetGesture\r\n"));

    IsPress = tp_getPos(&PosX, &PosY);
    Gesture_MappingLCD(&PosX, &PosY);
    Gesture_GetGesture(IsPress, PosX, PosY);

    GESTURE_MSG(("GESTURE_M:GetGesture:Event:%d, X=%d, Y=%d\r\n",Gesture.Gesture.Event,Gesture.Gesture.PosX,Gesture.Gesture.PosY));
    memcpy((void*)pGesture,(void*)&(Gesture.Gesture), sizeof(GESTURE_GESTURE_STRUCT));
}
//-------------------------------------------------------------------------------------------------
void Gesture_Cal(void)
{
    if(Gesture.CalData.TopLeftX == Gesture.CalData.BottomRightX)
    {
        GESTURE_ERR(("GESTURE_E:Cal:X is the same!:%d\r\n",Gesture.CalData.TopLeftX));
    }
    else
    if(Gesture.CalData.TopLeftX < Gesture.CalData.BottomRightX)
    {
        Gesture.CalData.ReverseX = FALSE;
        Gesture.CalData.MinX = Gesture.CalData.TopLeftX;
        Gesture.CalData.MaxX = Gesture.CalData.BottomRightX;
    }
    else
    {
        Gesture.CalData.ReverseX = TRUE;
        Gesture.CalData.MinX = Gesture.CalData.BottomRightX;
        Gesture.CalData.MaxX = Gesture.CalData.TopLeftX;
    }

    if(Gesture.CalData.TopLeftY == Gesture.CalData.BottomRightY)
    {
        GESTURE_ERR(("GESTURE_E:Cal:Y is the same!:%d\r\n",Gesture.CalData.TopLeftY));
    }
    else
    if(Gesture.CalData.TopLeftY < Gesture.CalData.BottomRightY)
    {
        Gesture.CalData.ReverseY = FALSE;
        Gesture.CalData.MinY = Gesture.CalData.TopLeftY;
        Gesture.CalData.MaxY = Gesture.CalData.BottomRightY;
    }
    else
    {
        Gesture.CalData.ReverseY = TRUE;
        Gesture.CalData.MinY = Gesture.CalData.BottomRightY;
        Gesture.CalData.MaxY = Gesture.CalData.TopLeftY;
    }

    GESTURE_MSG(("GESTURE_M:Cal:RX=%d, RY=%d\r\n",Gesture.CalData.ReverseX,Gesture.CalData.ReverseY));
    GESTURE_MSG(("GESTURE_M:Cal:TLX=%d, TLY=%d , BRX=%d, BRY=%d\r\n",Gesture.CalData.TopLeftX,Gesture.CalData.TopLeftY,Gesture.CalData.BottomRightX,Gesture.CalData.BottomRightY));
    GESTURE_MSG(("GESTURE_M:Cal:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));
}
//-------------------------------------------------------------------------------------------------
void Gesture_GetCalData(GESTURE_CALI_DATA_STRUCT* pCalData)
{
    GESTURE_MSG(("GESTURE_M:GetCalData:RX=%d, RY=%d\r\n",Gesture.CalData.ReverseX,Gesture.CalData.ReverseY));
    GESTURE_MSG(("GESTURE_M:GetCalData:TLX=%d, TLY=%d , BRX=%d, BRY=%d\r\n",Gesture.CalData.TopLeftX,Gesture.CalData.TopLeftY,Gesture.CalData.BottomRightX,Gesture.CalData.BottomRightY));
    GESTURE_MSG(("GESTURE_M:GetCalData:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));

    memcpy((void*)pCalData, (void*)&(Gesture.CalData), sizeof(GESTURE_CALI_DATA_STRUCT));
}
//-------------------------------------------------------------------------------------------------
void Gesture_SetCalData(GESTURE_CALI_DATA_STRUCT* pCalData)
{
    GESTURE_MSG(("GESTURE_M:SetCalData:RX=%d, RY=%d\r\n",pCalData->ReverseX,pCalData->ReverseY));
    GESTURE_MSG(("GESTURE_M:SetCalData:TLX=%d, TLY=%d , BRX=%d, BRY=%d\r\n",pCalData->TopLeftX,pCalData->TopLeftY,pCalData->BottomRightX,pCalData->BottomRightY));
    GESTURE_MSG(("GESTURE_M:SetCalData:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",pCalData->MinX,pCalData->MinY,pCalData->MaxX,pCalData->MaxY));

    memcpy((void*)&(Gesture.CalData), (void*)pCalData, sizeof(GESTURE_CALI_DATA_STRUCT));
}
//-------------------------------------------------------------------------------------------------
BOOL Gesture_DetCalData(GESTURE_CAL_TYPE_ENUM Cal)
{
    GESTURE_MSG(("GESTURE_M:DetCalData:%d\r\n",Cal));

    switch(Cal)
    {
        case GESTURE_CAL_TYPE_TOP_LEFT:
        {
            return tp_getPos(&(Gesture.CalData.TopLeftX), &(Gesture.CalData.TopLeftY));
        }

        case GESTURE_CAL_TYPE_BOTTOM_RIGHT:
        {
            return tp_getPos(&(Gesture.CalData.BottomRightX), &(Gesture.CalData.BottomRightY));
        }

        default:
        {
            GESTURE_ERR(("GESTURE_E:GetCalData:Unknow Cal:%d\r\n",Cal));
            return FALSE;
        }
    }
}
//-------------------------------------------------------------------------------------------------
void Gesture_Init(GESTURE_INIT_STRUCT* pInitPara)
{
    GESTURE_MSG(("GESTURE_M:Init:LCD W=%d, H=%d\r\n",pInitPara->LcdInfo.Width,pInitPara->LcdInfo.Height));

    Gesture.Status.Press = FALSE;
    Gesture.Status.Hold = FALSE;
    Gesture.Time.Count = 0;
    Gesture.Time.LastPress = 0;
    Gesture.Time.LastRelease = 0;

    memcpy((void*)&(Gesture.Threshold), (void*)&(pInitPara->Threshold), sizeof(GESTURE_THRESHOLD_STRUCT));
    memcpy((void*)&(Gesture.LcdInfo), (void*)&(pInitPara->LcdInfo), sizeof(GESTURE_LCD_STRUCT));
}
//-------------------------------------------------------------------------------------------------

