/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       AF_Lib.c
    @ingroup    mISYSLens

    @brief      Auto focus library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/12
*/
#include "OSCommon.h"
#include "ImageAlgInfor.h"
#include "Lens.h"
#include "AF_Lib.h"
#include "Utility.h"
#include "ipe_lib.h"

static FPCALLBACK_GETVA g_fpGetVariation = NULL;

void AF_RegGetVariationCB(FPCALLBACK_GETVA getVariation)
{
    g_fpGetVariation = getVariation;
}

INT32 AF_SearchFullRange(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT16 *pMaxVA)
{
    INT16 idx, maxIdx=0;
    UINT16 tempVA, maxVA=0;
    UINT32 contrastCnt;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    for(idx=startIdx; idx <= (endIdx+jumpStep-1); idx += jumpStep)
    {
        Lens_Focus_Goto(idx);
        sie_waitVD(2);

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = g_fpGetVariation();

        //debug_err(("%s: maxIdx=%d, maxVA=%d\r\n", __func__, idx, tempVA));

        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
    }

    Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    debug_err(("%s: maxIdx=%d, maxVA=%d\r\n", __func__, maxIdx, maxVA));

    return maxIdx;
}

INT32 AF_SearchFullRange2(INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT16 *pMaxVA)
{
    INT16 idx, maxIdx=0;
    UINT16 tempVA, maxVA=0;
    UINT32 contrastCnt;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return 0;
    }

    Lens_Focus_Goto(startIdx);
    sie_waitVD(1);
    Lens_Focus_Goto(startIdx+jumpStep);
    contrastCnt = ipe_getVACnt();

    for(idx = startIdx; idx <= (endIdx+jumpStep-1); idx += jumpStep)
    {
        sie_waitVD(1);
        Lens_Focus_Goto(idx + 2*jumpStep);

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = g_fpGetVariation();

        //debug_err(("%s: maxIdx=%d, maxVA=%d\r\n", __func__, idx, tempVA));

        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
    }

    Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    debug_err(("%s: maxIdx=%d, maxVA=%d\r\n", __func__, maxIdx, maxVA));

    return maxIdx;
}

INT32 AF_Process(void)
{
    UINT16 maxVA;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return AF_STA_FAIL;
    }

    Perf_Open();
    Perf_Mark();

    gAlgMsg.Alg_Mode=MODE_Captrue_Focusing;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUS, &gAlgMsg);
    }

    AF_SearchFullRange2(0, Lens_Focus_GetMaxPosition(), 10, &maxVA);

    gAlgMsg.Alg_Mode=MODE_Captrue_FocusEnd;

    if(maxVA < 1000)  //1000 is from CD500O8/O9 AF.
    {
        gAlgMsg.AF_Status=AFStatus_FAIL;
    }
    else
        gAlgMsg.AF_Status=AFStatus_OK;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
    }

    debug_err(("%s Time: %d ms\r\n", __func__, Perf_GetDuration()/1000));
    Perf_Close();

    if(gAlgMsg.AF_Status==AFStatus_OK)
        return AF_STA_OK;
    else
        return AF_STA_FAIL;
}

INT32 AF_Process2(void)
{
    INT16 startIdx, endIdx;
    UINT16 maxVA;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return AF_STA_FAIL;
    }

    Perf_Open();
    Perf_Mark();

    gAlgMsg.Alg_Mode=MODE_Captrue_Focusing;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUS, &gAlgMsg);
    }

    startIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 2);
    endIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);

    debug_err(("%s: cur section = %d, startIdx = %d, endIdx = %d\r\n",
               __func__, Lens_Zoom_GetSection(), startIdx, endIdx));

    AF_SearchFullRange2(startIdx, endIdx, 4, &maxVA);

    gAlgMsg.Alg_Mode=MODE_Captrue_FocusEnd;

    if(maxVA < 1000)  //1000 is from CD500O8/O9 AF.
    {
        gAlgMsg.AF_Status=AFStatus_FAIL;
    }
    else
        gAlgMsg.AF_Status=AFStatus_OK;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
    }

    debug_err(("%s Time: %d ms\r\n", __func__, Perf_GetDuration()/1000));
    Perf_Close();

    if(gAlgMsg.AF_Status==AFStatus_OK)
        return AF_STA_OK;
    else
        return AF_STA_FAIL;
}

INT32 AF_Process3(INT16 startIdx, INT16 endIdx, INT16 *pMaxIdx, UINT16 *pMaxVA)
{
    INT16 maxIdx;
    UINT16 maxVA;

    if(g_fpGetVariation == NULL)
    {
        debug_err(("Get Variation FP is null!!\r\n"));
        return AF_STA_FAIL;
    }

    if(startIdx >= endIdx)
        return AF_STA_FAIL;

    Perf_Open();
    Perf_Mark();

    gAlgMsg.Alg_Mode=MODE_Captrue_Focusing;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUS, &gAlgMsg);
    }

    maxIdx = AF_SearchFullRange2(startIdx, endIdx, 4, &maxVA);

    if(pMaxIdx != NULL)
        *pMaxIdx = maxIdx;

    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    gAlgMsg.Alg_Mode=MODE_Captrue_FocusEnd;

    if(maxVA < 1000)  //1000 is from CD500O8/O9 AF.
    {
        gAlgMsg.AF_Status=AFStatus_FAIL;
    }
    else
        gAlgMsg.AF_Status=AFStatus_OK;

    if(g_fpAlgExport)
    {
        g_fpAlgExport(ALGMSG_FOCUSEND, &gAlgMsg);
    }

    debug_err(("%s Time: %d ms\r\n", __func__, Perf_GetDuration()/1000));
    Perf_Close();

    if(gAlgMsg.AF_Status==AFStatus_OK)
        return AF_STA_OK;
    else
        return AF_STA_FAIL;
}

