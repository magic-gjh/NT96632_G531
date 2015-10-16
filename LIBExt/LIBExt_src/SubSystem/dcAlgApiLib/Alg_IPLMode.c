/*
    IPL command API

    This file is the API of the IPL control.

    @file       Alg_IPLMode.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "sensor_model.h"
#include "ErrorNo.h"
#include "pll.h"

/** \addtogroup mISYSAlg */
//@{


typedef ER (*FP_ChgMode) (IPL_OBJ *Obj);
ER IPL_ChgModeOffTo(IPL_OBJ *Obj);
ER IPL_ChgModeIdleTo(IPL_OBJ *Obj);
ER IPL_ChgModePrvTo(IPL_OBJ *Obj);
ER IPL_ChgModeVidTo(IPL_OBJ *Obj);
ER IPL_ChgModeRecTo(IPL_OBJ *Obj);
ER IPL_ChgModePccTo(IPL_OBJ *Obj);
ER IPL_ChgModeCapTo(IPL_OBJ *Obj);


static IPL_MODE gIPLCurMode = IPL_MODE_OFF;
//#NT#20090929#Bowen Li -begin
//#NT#add new function for change clock
static IPL_MODE gIPLNextMode = IPL_MODE_OFF;
//#NT#20090929#Bowen Li -end
static FP_ChgMode ChgModeAct[IPL_MODE_NUM] =
{
    IPL_ChgModeOffTo,
    IPL_ChgModeIdleTo,
    IPL_ChgModePrvTo,
    IPL_ChgModeVidTo,
    IPL_ChgModeRecTo,
    IPL_ChgModePccTo,
    IPL_ChgModeCapTo
};


/**
    IPL get mode.

    IPL get current mode.

    @return return current mode.
*/
IPL_MODE IPL_GetMode(void)
{
    return gIPLCurMode;
}

//#NT#20090929#Bowen Li -begin
//#NT#add new function for change clock
IPL_MODE IPL_GetNextMode(void)
{
    return gIPLNextMode;
}
//#NT#20090929#Bowen Li -end

/**
    IPL change mode

    IPL change mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
//#NT#20090929#Bowen Li -end
ER IPL_ChgMode(IPL_OBJ *Obj)
{
    ER rt;
    if (ChgModeAct[gIPLCurMode] == NULL)
    {
        debug_err(("IPL chg mode error (%d)\r\n", Obj->Mode));
        return E_NOSPT;
    }
    //#NT#20090929#Bowen Li -begin
    //#NT#add new function for change clock
    gIPLNextMode = Obj->Mode;
    //#NT#20090929#Bowen Li -end
    rt = ChgModeAct[gIPLCurMode](Obj);

    if (rt == E_OK)
    {
        debug_err(("IPL chg mode from %d to %d\r\n", gIPLCurMode, Obj->Mode));
        gIPLCurMode = Obj->Mode;
    }
    else
    {
        debug_err(("IPL chg mode fail (%d mode to %d)\r\n",gIPLCurMode, Obj->Mode));
    }
    return rt;
}

/**
    IPL set command

    IPL set command.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_SetCmd(IPL_OBJ *Obj)
{
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
    static IPL_MODE EnterSleepMode = IPL_MODE_OFF;
    if ((Obj->uiCmd & IPL_CMD_CHG_MODE) || (Obj->uiCmd & IPL_CMD_WAKE_UP) || (Obj->uiCmd & IPL_CMD_SLEEP))
    {
        if (Obj->uiCmd & IPL_CMD_SLEEP)
        {
            if (gIPLCurMode != IPL_MODE_IDLE)
            {
                EnterSleepMode = gIPLCurMode;
            }
            Obj->Mode = IPL_MODE_IDLE;
        }
    else if (Obj->uiCmd & IPL_CMD_WAKE_UP)
    {
            if (gIPLCurMode != IPL_MODE_IDLE)
            {
                debug_err(("IPL command fail\r\n"));
                return E_SYS;
            }
            Obj->Mode = EnterSleepMode;
        }
        return IPL_ChgMode(Obj);
    }
//#NT#2009/10/08#Bowen Li -end
    else if (Obj->uiCmd != NULL)
    {
        if ((gIPLCurMode == IPL_MODE_OFF) || (gIPLCurMode == IPL_MODE_IDLE) || (gIPLCurMode == IPL_MODE_CAP))
        {
            debug_err(("IPL command fail\r\n"));
            return E_SYS;
        }
        if (Obj->uiCmd & IPL_CMD_CHG_SIZE_IME1)
        {
            debug_err(("IME width = %d, height = %d\r\n", Obj->uiImeOutWidth1, Obj->uiImeOutHeight1));
//#NT#2009/12/21#Bowen Li -begin
//#NT#fix bug for TV change ime path1 size
            SetImePccResolution(Obj->uiImeOutWidth1, Obj->uiImeOutHeight1);
            //SetImeIdeResolution(Obj->uiImeOutWidth1, Obj->uiImeOutHeight1, 0, 0);
//#NT#2009/12/21#Bowen Li -end
            return E_OK;
        }
        else if (Obj->uiCmd & IPL_CMD_CHG_SIZE_IME2)
        {
            debug_err(("IME width = %d, height = %d\r\n", Obj->uiImeOutWidth2, Obj->uiImeOutHeight2));
            SetImeVideoResolution(Obj->uiImeOutWidth2, Obj->uiImeOutHeight2,Obj->uiImeBufNum2);
            return E_OK;
        }
        else if (Obj->uiCmd & IPL_CMD_SET_DZOOM)
        {
            if ((Obj->uiDzoomStep >= 10) && (Obj->uiDzoomStep <= Get_DzoomMaxIndex()))
            {
                Set_DzoomIndex(Obj->uiDzoomStep);
                if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                    SetDzoom2Video(Get_DzoomIndex());
                else
                    SetDzoom2Preview(Get_DzoomIndex());
                return E_OK;
            }
            else
            {
                debug_err(("zoom index overflow %d\r\n", Obj->uiDzoomStep));
                return E_SYS;
            }
        }
    }
    debug_err(("IPL command error (%d)\r\n", Obj->uiCmd));
    return E_NOSPT;
}


/**
    IPL stop to dram.

    stop IPL output to dram.

    @return ER IPL status.
*/
//#NT#2009/10/29#Bowen Li -begin
//#NT#rearrange judgement flow
//#NT#2009/09/29#Bowen Li -begin
//#NT#add new function for change clock
ER IPL_Stop2Dram(void)
{
    IPL_MODE CurMode, NextMode;

    CurMode = IPL_GetMode();
    NextMode = IPL_GetNextMode();
    debug_ind(("IPL_Stop2Dram %d %d\r\n", CurMode, NextMode));
    if (((CurMode == IPL_MODE_PREVIEW) && (NextMode == IPL_MODE_CAP)) ||
        ((CurMode == IPL_MODE_PREVIEW) && (NextMode == IPL_MODE_PCC)) ||
        ((CurMode == IPL_MODE_VIDEO) && (NextMode == IPL_MODE_PCC)) ||
        ((CurMode == IPL_MODE_VIDEO) && (NextMode == IPL_MODE_VIDEOREC)) ||
        ((CurMode == IPL_MODE_PCC) && (NextMode == IPL_MODE_PREVIEW)) ||
        ((CurMode == IPL_MODE_PCC) && (NextMode == IPL_MODE_VIDEO)) ||
        ((CurMode == IPL_MODE_VIDEOREC) && (NextMode == IPL_MODE_VIDEO)))
    {
        StopPreviewDramData();
    }
    return E_OK;
}
//#NT#2009/09/29#Bowen Li -end
//#NT#2009/10/29#Bowen Li -end

/**
    IPL change off mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeOffTo(IPL_OBJ *Obj)
{
    SIE_OPENOBJ SIEOpenObj;
    PRE_OPENOBJ PREOpenObj;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;

    SIEOpenObj.FP_SIEISR_CB = sie_isrCB;
    PREOpenObj.FP_PREISR_CB = pre_isrCB;
    IPEOpenObj.FP_IPEISR_CB = ipe_isrCB;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = ime_isrCB;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;

    switch(Obj->Mode)
    {
        case IPL_MODE_PREVIEW:
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            if (SensorInfor.bSieMClkEn == ENABLE)
            {
                pll_enableClock(SIE_MCLK);
            }
//#NT#2009/10/08#Bowen Li -end
            sie_open(&SIEOpenObj);
            pre_open(&PREOpenObj);
            ipe_open(&IPEOpenObj);
            ime_open(&IMEOpenObj);
            Set2Preview(Obj);
            return E_OK;

        case IPL_MODE_VIDEO:
            if (SensorInfor.bSieMClkEn == ENABLE)
            {
                pll_enableClock(SIE_MCLK);
            }
            sie_open(&SIEOpenObj);
            pre_open(&PREOpenObj);
            ipe_open(&IPEOpenObj);
            ime_open(&IMEOpenObj);
            Set2Preview(Obj);
            return E_OK;
        case IPL_MODE_PCC:
            sie_open(&SIEOpenObj);
            pre_open(&PREOpenObj);
            ipe_open(&IPEOpenObj);
            ime_open(&IMEOpenObj);
            Set2Pcc(Obj);
            return E_OK;

        case IPL_MODE_OFF:
            return E_OK;

        case IPL_MODE_VIDEOREC:
        case IPL_MODE_IDLE:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change idle mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeIdleTo(IPL_OBJ *Obj)
{
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            pre_close();
            sie_close();
            //#NT#2010/11/01#ethanlau -begin
            //#NT#marked for ov5653 untill sensor hw confirmed
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            //if (SensorInfor.bSieMClkEn == ENABLE)
            //{
            //    pll_disableClock(SIE_MCLK);
            //}
            //#NT#2010/11/01#ethanlau -end
//#NT#2009/10/08#Bowen Li -end
            ipe_close();
            ime_close();
            return E_OK;

        case IPL_MODE_IDLE:
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            gSenCBInfo.IplSleepLevel = Obj->uiSleepLevel;
            SenFlowFunc(IPL_SLEEP, &gSenCBInfo);
            return E_OK;

        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
        case IPL_MODE_PCC:
            SenFlowFunc(IPL_WAKEUP, &gSenCBInfo);
            return E_OK;
//#NT#2009/10/08#Bowen Li -end

        case IPL_MODE_VIDEOREC:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change preview mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModePrvTo(IPL_OBJ *Obj)
{

    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            StopPreviewDramData();
            pre_close();
            sie_close();
            //#NT#2010/11/01#ethanlau -begin
            //#NT#marked for ov5653 untill sensor hw confirmed
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            //if (SensorInfor.bSieMClkEn == ENABLE)
            //{
            //    pll_disableClock(SIE_MCLK);
            //}
            //#NT#2010/11/01#ethanlau -end
//#NT#2009/10/08#Bowen Li -end
            ipe_close();
            ime_close();
            return E_OK;

        case IPL_MODE_CAP:
            //#NT#20090929#Bowen Li -begin
            //#NT#add new function for change clock
            if(g_fpAlgExport)
            {
                g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
            }
            //#NT#20090929#Bowen Li -end
            //#NT#2010/04/09#Jarkko Chang -begin
            //#NT#add for stop ime correctly
            ime_freeRun(IMEALG_DISABLE);
            ime_clrIntFlag();
            ime_waitFlagFrameEnd();
            //#NT#2010/04/09#Jarkko Chang -end
            ipe_pause();
            ipe_close();
            ime_setStop();
            ime_close();
            return E_OK;
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
        case IPL_MODE_IDLE:
            gSenCBInfo.IplSleepLevel = Obj->uiSleepLevel;
            SenFlowFunc(IPL_SLEEP, &gSenCBInfo);
            return E_OK;

        case IPL_MODE_PREVIEW:
        case IPL_MODE_PCC:
            return E_OK;
//#NT#2009/10/08#Bowen Li -end
        case IPL_MODE_VIDEO:
            return E_OK;

        case IPL_MODE_VIDEOREC:
        default:
            return E_PAR;
    }
}

/**
    IPL change video mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeVidTo(IPL_OBJ *Obj)
{
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            StopPreviewDramData();
            pre_close();
            sie_close();
            //#NT#2010/11/01#ethanlau -begin
            //#NT#marked for ov5653 untill sensor hw confirmed
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            //if (SensorInfor.bSieMClkEn == ENABLE)
            //{
            //    pll_disableClock(SIE_MCLK);
            //}
            //#NT#2010/11/01#ethanlau -end
//#NT#2009/10/08#Bowen Li -end
            ipe_close();
            ime_close();
            return E_OK;

        case IPL_MODE_VIDEOREC:
            Set2Video(Obj);
            return E_OK;
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
        case IPL_MODE_IDLE:
            gSenCBInfo.IplSleepLevel = Obj->uiSleepLevel;
            SenFlowFunc(IPL_SLEEP, &gSenCBInfo);
            return E_OK;

        case IPL_MODE_VIDEO:
        case IPL_MODE_PCC:
            return E_OK;
        case IPL_MODE_PREVIEW:
            return E_OK;

        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}
/**
    IPL change video recording mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeRecTo(IPL_OBJ *Obj)
{
    switch(Obj->Mode)
    {
        case IPL_MODE_VIDEO:
            Set2VideoLiveview(Obj);
            return E_OK;

        case IPL_MODE_OFF:
        case IPL_MODE_IDLE:
        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_PCC:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change pcc mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModePccTo(IPL_OBJ *Obj)
{
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            StopPreviewDramData();
            pre_close();
            sie_close();
            //#NT#2010/11/01#ethanlau -begin
            //#NT#marked for ov5653 untill sensor hw confirmed
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
            //if (SensorInfor.bSieMClkEn == ENABLE)
            //{
            //    pll_disableClock(SIE_MCLK);
            //}
            //#NT#2010/11/01#ethanlau -end
//#NT#2009/10/08#Bowen Li -end
            ipe_close();
            ime_close();
            return E_OK;

        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
            Set2VideoLiveview(Obj);
            return E_OK;
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
        case IPL_MODE_IDLE:
            gSenCBInfo.IplSleepLevel = Obj->uiSleepLevel;
            SenFlowFunc(IPL_SLEEP, &gSenCBInfo);
            return E_OK;

        case IPL_MODE_PCC:
            return E_OK;
//#NT#2009/10/08#Bowen Li -end
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change capture mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeCapTo(IPL_OBJ *Obj)
{
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;

    IPEOpenObj.FP_IPEISR_CB = ipe_isrCB;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = ime_isrCB;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;

    switch(Obj->Mode)
    {
        case IPL_MODE_PREVIEW:
            ipe_open(&IPEOpenObj);
            ime_open(&IMEOpenObj);
            Set2Preview(Obj);
            return E_OK;

        case IPL_MODE_CAP:
            return E_OK;

        case IPL_MODE_OFF:
        case IPL_MODE_IDLE:
        case IPL_MODE_VIDEO:
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_PCC:
        default:
            return E_PAR;
    }
}


