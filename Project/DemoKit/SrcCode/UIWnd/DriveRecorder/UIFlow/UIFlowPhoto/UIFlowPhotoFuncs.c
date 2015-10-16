#include "UIFlowWndPhoto.h"
#include "UIMenuPhotoSettings.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "ImageAlgInfor.h"
#include "FileSysTsk.h"
#include "Debug.h"
#include "UIPhotoObj.h"
#include "UIMenuSetupSettings.h"
#include "UIFlowPhotoFuncs.h"
#include "UISystemStatusSettings.h"


static CHAR g_cFreePicNumStr[16]     = {0};



UINT32 Get_FreePicNum(UINT32 uiSizeIndex, UINT32 uiQualityIndex)
{
    UINT64              uiFreeSpace;
    UINT32              uiCompressRate;
    UINT32              freeImgNum;
    UINT32              reserveSize = 0x100000;
    UINT32              MinCaptureSize;
    _IMGSize_sel        size;
    UINT8               CompressRateTable[QUALITY_SETTING_MAX]
                        ={
                           12,//23,  // QUALITY_FINE     = 0,   25%
                           10,//14,  // QUALITY_STANDARD = 1,   15%
                           8,// 9   // QULAITY_ECONOMY  = 2,   10%
                         };

    if(FilesysGetFreeSpace() < reserveSize)
        uiFreeSpace = 0;
    else
        uiFreeSpace = FilesysGetFreeSpace() - reserveSize;

    debug_ind(("Free Space = %ld\r\n", uiFreeSpace));

    uiCompressRate = CompressRateTable[uiQualityIndex];

    size = Get_SizeValue(uiSizeIndex);
    MinCaptureSize = (JPGIMG_SIZE[size][0] * JPGIMG_SIZE[size][1] * 2) * uiCompressRate / 100;
    debug_ind(("MinCaptureSize = %ld K \r\n", MinCaptureSize/1024));

    // Reserve double size for last file.
    if(uiFreeSpace < MinCaptureSize * 2)
    {
        freeImgNum = 0;
    }
    else    // uiFreeSpace = (N-1)*MinCaptureSize + MinCaptureSize*2
    {
        freeImgNum = (uiFreeSpace - MinCaptureSize * 2) / MinCaptureSize + 1;
    }

    debug_err((">> Free ImgNum = %ld\r\n", freeImgNum));

    return freeImgNum;
}

CHAR *Get_FreePicNumString(UINT32 uiValue)
{
    UINT32 Val;
    if (GPIOMap_DetStrgCard() == FALSE)
    {
		Val =0;
    		sprintf(g_cFreePicNumStr, "%05ld", uiValue);		
    }
    else
    	{
    sprintf(g_cFreePicNumStr, "%05ld", uiValue);
    	}
    return g_cFreePicNumStr;
}

void DCF_SetFileId(void)
{
    UINT16 uhFolderId, uhFileId;

    FilesysGetDCFNextID(&uhFolderId, &uhFileId);

    if (uhFolderId > (Get_FilesysDirId()))
        Set_FilesysDirId(uhFolderId);

    if (uhFileId > (Get_FilesysFileId()))
        Set_FilesysFileId(uhFileId);

    debug_ind(("DCF_SetFileId: Set new DirNumber = %d, FileNumber = %d\r\n", Get_FilesysDirId(), Get_FilesysFileId()));

    FilesysSetWriteNextFileId(((UINT16)Get_FilesysDirId()), ((UINT16)Get_FilesysFileId()), FALSE);
}


