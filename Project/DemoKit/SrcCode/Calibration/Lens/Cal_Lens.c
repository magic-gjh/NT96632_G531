/**
    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.

    @file       Cal_Lens.c
    @ingroup    mISYSLens

    @brief      Lens calibration library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA
    @date       2010/02/01
*/

#include    <string.h>

#include    "SysCfg.h"
#include    "Lens.h"
#include    "AF_Lib.h"
#include    "Cal_Alg.h"
#include    "Cal_Lens.h"
#include    "PhotoTsk.h"
#include    "KeyScanTsk.h"
#include    "IDE.h"
#include    "ipe_lib.h"

#if (_CALIBRATION_MODE_ == ENABLE)

static CALLENS_INFO gCalLens_Info = { 0 };

UINT8 CalLens_FarFocusAdjust(void)
{
    INT16 StartIdx,EndIdx;
    UINT8 i;
    UINT  AF_Step;
    FLGPTN uiFlag;
    AF_IPEINFO gAFIPEInfo;
    char str1[32];
    UINT16 maxVA;

    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    Cal_AlgClearScreen();
    Cal_AlgShowString("1. Face ISO chart(far)",0,0);
    TimerDelayMs(10);
    Cal_AlgShowString("2. Press shutter key",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    Cal_AlgClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    Photo_LockCAF(1);
    KeyScan_Suspend();
    Lens_Zoom_Init(LENS_ZOOM_INIT_PART1);
    Lens_Zoom_Init(LENS_ZOOM_INIT_PART2);
    Lens_Zoom_EnableIsr(FALSE);

    gAFIPEInfo.vacc_src = VDET_EVAR;
    gAFIPEInfo.vacc_sqr = 0;
    gAFIPEInfo.vacc_gain = 0;
    gAFIPEInfo.vacc_num = WINDOWS_8X8;
    gAFIPEInfo.va_th = 16;
    gAFIPEInfo.vacc_outsel = WIN8x8_HV_VALUE;
    User_VABegin(&gAFIPEInfo);

    AF_Step=4;
    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        Lens_Focus_GotoHome();
        Lens_Zoom_Goto(i+1);
        sie_waitVD(5);
        StartIdx = CALLENS_START_IDX;
        Lens_Focus_Goto(StartIdx);
        EndIdx = Lens_Focus_GetMaxPosition();
        sie_waitVD(3);
        gCalLens_Info.Far_Focus[i]=AF_SearchFullRange(StartIdx,EndIdx,AF_Step,&maxVA);

        Cal_AlgClearScreen();
        snprintf(str1, 32, "S=%d, Far=%d", i+1, gCalLens_Info.Far_Focus[i]);
        if(i % 2 == 0)
            Cal_AlgShowString(str1,0,30*(i/2));
        else
            Cal_AlgShowString(str1,120,30*(i/2));
    }

    User_VAEnd(&gAFIPEInfo);

    CalLens_WriteCalData();
    KeyScan_Resume();
    Lens_Zoom_Retract();

    /************************************************************/
    Cal_AlgClearScreen();
    Cal_AlgShowString("1. Adjust done (far)",0,0);
    TimerDelayMs(10);
    Cal_AlgShowString("2. Press enter key to leave",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_ENTER);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_ENTER, TWF_ORW|TWF_CLR);
    /*************************************************************/
    return CALLENS_STA_OK;
}

UINT8 CalLens_NearFocusAdjust(void)
{
    INT16 StartIdx,EndIdx;
    UINT8 i;
    UINT  AF_Step;
    FLGPTN uiFlag;
    AF_IPEINFO gAFIPEInfo;
    char str1[32];
    UINT16 maxVA;

    /************************************************************/
    debug_err(("\n\nPlease let camera facing ISO chart. Then press shutter key\r\n\n"));
    Cal_AlgClearScreen();
    Cal_AlgShowString("1. Face ISO chart (near)",0,0);
    TimerDelayMs(10);
    Cal_AlgShowString("2. Press shutter key",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_SHUTTER2, TWF_ORW|TWF_CLR);
    Cal_AlgClearScreen();
    /*************************************************************/

    Photo_LockAE(1);
    Photo_LockAWB(1);
    Photo_LockFD(1);
    Photo_LockSD(1);
    Photo_LockCAF(1);
    KeyScan_Suspend();
    Lens_Zoom_Init(LENS_ZOOM_INIT_PART1);
    Lens_Zoom_Init(LENS_ZOOM_INIT_PART2);
    Lens_Zoom_EnableIsr(FALSE);

    gAFIPEInfo.vacc_src = VDET_EVAR;
    gAFIPEInfo.vacc_sqr = 0;
    gAFIPEInfo.vacc_gain = 0;
    gAFIPEInfo.vacc_num = WINDOWS_8X8;
    gAFIPEInfo.va_th = 16;
    gAFIPEInfo.vacc_outsel = WIN8x8_HV_VALUE;
    User_VABegin(&gAFIPEInfo);

    AF_Step=4;
    for(i=0;i<Lens_Zoom_GetMaxSection();i++)
    {
        Lens_Focus_GotoHome();
        Lens_Zoom_Goto(i+1);
        sie_waitVD(5);
        StartIdx = CALLENS_START_IDX;
        Lens_Focus_Goto(StartIdx);
        EndIdx = Lens_Focus_GetMaxPosition();
        sie_waitVD(3);
        gCalLens_Info.Near_Focus[i]=AF_SearchFullRange(StartIdx,EndIdx,AF_Step,&maxVA);

        Cal_AlgClearScreen();
        snprintf(str1, 32, "S=%d, Near=%d", i+1, gCalLens_Info.Near_Focus[i]);
        if(i % 2 == 0)
            Cal_AlgShowString(str1,0,30*(i/2));
        else
            Cal_AlgShowString(str1,120,30*(i/2));
    }

    User_VAEnd(&gAFIPEInfo);

    CalLens_WriteCalData();
    KeyScan_Resume();
    Lens_Zoom_Retract();

    /************************************************************/
    Cal_AlgClearScreen();
    Cal_AlgShowString("1. Adjust done (near)",0,0);
    TimerDelayMs(10);
    Cal_AlgShowString("2. Press enter key to leave",0,30);
    clr_flg(FLG_ID_KEY, FLGKEY_ENTER);
    wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_ENTER, TWF_ORW|TWF_CLR);
    /*************************************************************/

    return CALLENS_STA_OK;
}

BOOL CalLens_IsAdjust(void)
{
    if(gCalLens_Info.Far_Focus[0] != 0)
        return TRUE;
    else
        return FALSE;
}

INT32 CalLens_GetFarFocusStep(UINT32 zoomSection)
{
    if(zoomSection > 16)
        return 0;

    return gCalLens_Info.Far_Focus[zoomSection-1];
}

INT32 CalLens_GetNearFocusStep(UINT32 zoomSection)
{
    if(zoomSection > 16)
        return 0;

    return gCalLens_Info.Near_Focus[zoomSection-1];
}

INT32 CalLens_ReadCalData(void)
{
    PPSTORE_SECTION_HANDLE  pSection;
    INT32 retValue;

    // Enable PStore
    PStore_EnablePS();

    if ((pSection = PStore_OpenSection(PS_CAL_LENS, PS_RDONLY)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)&gCalLens_Info, 0, sizeof(gCalLens_Info), pSection);
        PStore_CloseSection(pSection);
        retValue = CALLENS_STA_OK;
    }
    else
    {
        debug_err(("%s: Pstore open to write FAIL\r\n", __func__));
        retValue = CALLENS_STA_FAIL;
    }

    // Disable PStore
    PStore_DisablePS();

    debug_err(("%s: status = %d\r\n", __func__, retValue));

    return retValue;
}

INT32 CalLens_WriteCalData(void)
{
    PPSTORE_SECTION_HANDLE  pSection;
    INT32 retValue;

    // Enable PStore
    PStore_EnablePS();

    if ((pSection = PStore_OpenSection(PS_CAL_LENS, PS_RDWR|PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)&gCalLens_Info, 0, sizeof(gCalLens_Info), pSection);
        PStore_CloseSection(pSection);
        retValue = CALLENS_STA_OK;
    }
    else
    {
        debug_err(("%s: Pstore open to write FAIL\r\n", __func__));
        retValue = CALLENS_STA_FAIL;
    }

    // Disable PStore
    PStore_DisablePS();

    debug_err(("%s: status = %d\r\n", __func__, retValue));

    return retValue;
}

#endif
