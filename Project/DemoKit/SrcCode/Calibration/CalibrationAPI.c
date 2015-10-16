/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTsk.c
    @ingroup    mIPRJAPTest

    @brief      Calibration task API
                Calibration task API

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <ctype.h>
#include "Kernel.h"
#include "SysCfg.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "FileSysTsk.h"

//#NT#2009/09/01#KS Hung -begin
static DC**     pDCList;
//#NT#2009/09/01#KS Hung -end

#if (_CALIBRATION_MODE_ == ENABLE)

/**
  Find file that filename with extension in root folder

  Find file that filename with extension in root folder and return the main
  filename (convert to integer of UINT32)
  [Calibration internal API]

  @param INT8 *pExt: extension
  @return UINT32: Filename of integer
*/
UINT32 Cal_GetDefaultInRoot(INT8 * pExt)
{
    UINT32          uiResult, i;
    INT8            orgFilename[9];
    FIND_DATA       findData;
    PFILE_SEARCH    pHSearch;

    uiResult = CAL_INVALID_DEFAULT;

    if (Primary_GetStorage() != PRIMARY_STGTYPE_CARD)
    {
        return CAL_INVALID_DEFAULT;
    }

    if (FilesysWaitInitFinish(FST_TIME_NO_WAIT) != FST_STA_OK)
    {
        Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
    }

    FilesysWaitInitFinish(FST_TIME_INFINITE);

    pHSearch = fs_FindFirstFile("A:\\", &findData);
    while (pHSearch)
    {
        if (!M_IsDirectory(findData.attrib))
        {
            if (!strncmp(findData.FATExtName , pExt, 3))
            {
                memcpy(orgFilename, findData.FATMainName, 8);
                orgFilename[8] = '\0';
                sscanf(orgFilename, "%08X", &uiResult);
                for (i=0; i<8; i++)
                {
                    if (isxdigit(*(orgFilename + i)) == 0)
                    {
                        uiResult = CAL_INVALID_DEFAULT;
                        break;
                    }
                }
                break;
            }
        }

        if(fs_FindNextFile(pHSearch, &findData) != FSS_OK)
        {
            break;
        }
    }

    if (pHSearch)
    {
        fs_FindClose(pHSearch);
    }

    return uiResult;
}

/**
  Default function

  Default function

  @param void
  @return void
*/
void Cal_NoFunction(void)
{
    Cal_ShowString("No Function", &g_CalStringRect, TRUE);
}

/**
  Set default settings and generate serial number

  Set default settings and generate serial number

  @param void
  @return void
*/
void Cal_SetDefault(void)
{
    UINT32  uiSerialNumber;
    INT8    StringBuffer[32];

    // Set default settings

    // Generate serial number
    uiSerialNumber = Cal_GetSerialNum(1);
    sprintf(StringBuffer, "Ser #: 0x%.8lX", uiSerialNumber);
    Cal_ShowString(StringBuffer, &g_CalStringRect, TRUE);
}

/**
  Show calibration string on OSD

  Show calibration string on OSD

  @param INT8 *pStr: String pointer
  @param PRECT pRect: Rectangle to display string, if NULL will use default rectangle
  @param BOOL bClear: Clear the rectangle before display string or not.
    TRUE -> Clear
    FALSE -> Don't clear
  @return void
*/
void Cal_ShowString(INT8 *pStr, PRECT pRect, BOOL bClear)
{
//#NT#2009/08/31#KS Hung -begin
#if 0
    SHOWOSD_STRING  DrawString;

    if (pRect != NULL)
    {
        DrawString.pRect            = pRect;
    }
    else
    {
        DrawString.pRect            = &g_CalStringRect;
    }

    DrawString.pString              = (CHAR *)pStr;
    DrawString.uiColorIndex         = SHOWOSD_COLOR_RED;
    DrawString.uiBorderColorIndex   = SHOWOSD_COLOR_GRAY_50;
    DrawString.uiAlignment          = KV_CENTER | KH_LEFT;
    DrawString.bClear               = bClear;

    ShowOSD_DrawString(IDE_OSDID_1, &DrawString);
#else
    pDCList = (DC**)UI_BeginScreen();
    if(bClear)
    {
        GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
    }
    GxGfx_SetTextColor(_OSD_INDEX_RED, _OSD_INDEX_GRAY, 0);
    GxGfx_Text(pDCList[GxGfx_OSD], pRect->uiLeft, pRect->uiTop, pStr);
    UI_EndScreen((UINT32)pDCList);
#endif
//#NT#2009/08/31#KS Hung -end
}

/**
  Get serial number

  This function will scan file that filename with extension "SEN" in root folder
  and use the filename as serial number. It will return the serial number
  and rename the filename to next value.

  @param UINT32 uiIncrement: filename increment of next value
  @return UINT32: Serial number
*/
UINT32 Cal_GetSerialNum(UINT32 uiIncrement)
{
    INT8    orgFilename[16], newFilename[16];
    UINT32  uiSerialNum;


    uiSerialNum = Cal_GetDefaultInRoot("SEN");

    if (uiSerialNum != CAL_INVALID_DEFAULT && uiIncrement != 0)
    {
        sprintf(orgFilename, "A:\\%.8lX.SEN", uiSerialNum);
        sprintf(newFilename, "%.8lX.SEN", (UINT32)(uiSerialNum + uiIncrement));
        fs_rename(orgFilename, newFilename);
    }
    return uiSerialNum;
}

/**
  Get available memory address in calibration task

  Get available memory address in calibration task.

  @param void
  @return UINT32: Available memory address
*/
UINT32 Cal_GetMemoryAddr(void)
{
    return g_CalAppObj.uiMemoryAddr;
}

/**
  Get available memory size in calibration task

  Get available memory size in calibration task.

  @param void
  @return UINT32: Available memory size
*/
UINT32 Cal_GetMemorySize(void)
{
    return g_CalAppObj.uiMemorySize;
}

/**
  Open Calibration task

  Open Calibration task

  @param PCAL_APPOBJ pCalObj: Calibration task object, specify memory available in calibration task.
  @return ER
    E_SYS: Task is already opened
    E_PAR: Parameter error (memory address or size is invalid)
    E_OK : Task opened correctly
*/
ER Cal_Open()
{
    UINT32 uiPoolAddr;

    if (g_bCalbirationOpened == TRUE)
    {
        return E_SYS;
    }

    g_bCalbirationOpened = TRUE;

    //Get memory for calibartion task
    get_blk((VP *)&uiPoolAddr, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    g_CalAppObj.uiMemoryAddr = uiPoolAddr;
    g_CalAppObj.uiMemorySize = POOL_SIZE_CAPTURE;

    GPIOMap_TurnOnLCDBacklight();
    KeyScan_EnableAutoPoweroff(FALSE);

    return E_OK;
}

void Cal_FillRect(PRECT pRect, UINT32 uiBgColor)
{
    pDCList = (DC**)UI_BeginScreen();
    GxGfx_SetShapeColor(uiBgColor, uiBgColor, 0);
    GxGfx_FillRect(pDCList[GxGfx_OSD],pRect->uiLeft,pRect->uiTop,pRect->uiLeft+pRect->uiWidth,pRect->uiTop+pRect->uiHeight);
    UI_EndScreen((UINT32)pDCList);
}


#endif

//@}
