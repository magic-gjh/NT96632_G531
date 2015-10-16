#include "OSCommon.h"
#include "SysCfg.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_Photo.h"
#include "PrimaryTsk.h"
#include "FilesysTsk.h"
#include "SieCapTsk.h"
#include "Utility.h"
#include "ide.h"
#include <string.h>

typedef struct _MSDC_CB_CTRL_{
    BOOL bCapFstOK;
    FPALGCALLBACK HookCb; //!< Hooked Image Event CB
}tMSDC_CB_CTRL;


//Declare Internal Functions
//IQ's Event Callback
static void  MsdcNvtCb_ImageCB(unsigned int uiEventID, AlgMsgInfo *pInfo);

//Declare Internal Variables
static tMSDC_CB_CTRL gCtrl = {0};

//Hook IQs Callback
FPALGCALLBACK MsdcNvt_HookImageCb(FPALGCALLBACK HookCb)
{
    gCtrl.HookCb = HookCb;
    return MsdcNvtCb_ImageCB;
}

//Private Image Callback
static void MsdcNvtCb_ImageCB(unsigned int uiEventID, AlgMsgInfo *pInfo)
{
    switch(uiEventID)
    {
     case ALGMSG_CAPFSTOK:
        gCtrl.bCapFstOK = TRUE;
        break;
    }

    gCtrl.HookCb(uiEventID,pInfo);
}

void MsdcNvtCb_PhotoBegin(void* pData)
{
    tMSDCEXT_PARENT* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_PARENT,pData);

    if(pDesc==NULL)
        return;

    pDesc->bOK = FALSE;

    if(Primary_GetCurrentMode()!=PRIMARY_MODE_PHOTO)
    {
        Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        Primary_Wait4ChangeModeDone();
    }


    gCtrl.bCapFstOK = FALSE;

    pDesc->bOK = TRUE;
}

void MsdcNvtCb_GetDisplaySettings(void* pData)
{
    IDE_COLOR_FORMAT Fmt;
    tMSDCEXT_DISPLAY_SETTING* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISPLAY_SETTING,pData);
    if(pDesc==NULL)
        return;

    pDesc->Parent.bOK = FALSE;

     ide_getV1Buf0Addr(&pDesc->AddrY[0], &pDesc->AddrU[0], &pDesc->AddrV[0]);
     ide_getV1Buf1Addr(&pDesc->AddrY[1], &pDesc->AddrU[1], &pDesc->AddrV[1]);
     ide_getV1Buf2Addr(&pDesc->AddrY[2], &pDesc->AddrU[2], &pDesc->AddrV[2]);
     ide_getV1BufDim(&pDesc->Width, &pDesc->HeightY, &pDesc->PitchY);

     pDesc->PitchY <<= 2;

     ide_getV1Fmt(&Fmt);
     switch(Fmt)
     {
        case COLOR_YCBCR444:
            pDesc->PitchUV = pDesc->PitchY;
            pDesc->HeightUV = pDesc->HeightY;
            break;
        case COLOR_YCBCR422:
            pDesc->PitchUV = pDesc->PitchY >> 1;
            pDesc->HeightUV = pDesc->HeightY;
            break;
        case COLOR_YCBCR420:
            pDesc->PitchUV = pDesc->PitchY >> 1;
            pDesc->HeightUV = pDesc->HeightY >> 1;
            break;
     }

     pDesc->Parent.bOK = TRUE;
}

void MsdcNvtCb_GetDisplayCurrentBufSel(void* pData)
{
    IDE_BJMODE uiBJMODE;
    IDE_OP_BUF uiOPTBUF;
    IDE_BUF_NUM uiBUFNUM;
    tMSDCEXT_DISPLYA_CURRENT* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_DISPLYA_CURRENT,pData);
    if(pDesc==NULL)
        return;

    pDesc->Parent.bOK = FALSE;

    ide_getV1BufOp(&uiBJMODE,&uiOPTBUF,&uiBUFNUM);
    pDesc->OPTBUF = (UINT32)uiOPTBUF;
    pDesc->BUFNUM = (UINT32)uiBUFNUM;

    pDesc->Parent.bOK = TRUE;
}

void MsdcNvtCb_CaptureJpgWidthGetInfo(void* pData)
{
    UINT32 uiPoolAddrEnd;
    UW BufSize=0;
    PFILE_TSK_DCF_PARAM    pParam;

    tMSDCEXT_INFO_CAPTURE_JPG* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_INFO_CAPTURE_JPG,pData);
    if(pDesc==NULL)
        return;

    pDesc->Parent.bOK = FALSE;

    gCtrl.bCapFstOK = FALSE;

    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_ImgSize,  _IMGSize_10M);
    ImgCapture_SetParameter(_FileFmt,  _FileFmt_Jpg);
    ImgCapture_SetParameter(_PreCapBitDepth, _PreBitDepth_8);
    ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
    ImgCapture_SetParameter(_DriveMode,  _IMGDriveMode_Single);
    ImgCapture_StartCapture();

    while(!gCtrl.bCapFstOK)
        TimerDelayMs(100);

    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    pParam = FilesysGetFSTParam();

    if (get_blk((void*)&uiPoolAddrEnd, POOL_ID_CAPTURE) != E_OK)
    {
        pDesc->Parent.bOK = FALSE;
        return;
    }
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddrEnd);
    uiPoolAddrEnd = uiPoolAddrEnd + POOL_SIZE_CAPTURE;

    //For Open File Only
    if(FilesysReadWriteSpeciFile(FST_CMD_READ_SPECIFIC_FILE,
            pParam->DirectoryID,
            pParam->FilenameID,
            FST_FMT_JPG,
            NULL,
            &BufSize,
            FST_TIME_INFINITE)!=FST_STA_OK)
    {
        pDesc->Parent.bOK = FALSE;
        return;
    }

    pDesc->JpgFileSize = pParam->pFile->fileSize;
    //32Bytes Alignment for File System
    pDesc->JpgAddr = (uiPoolAddrEnd - pDesc->JpgFileSize)&(~0x1F);

    //Read To Read File
    BufSize = (UW)pDesc->JpgFileSize;
    if(FilesysReadWriteSpeciFile(FST_CMD_READ_SPECIFIC_FILE,
            pParam->DirectoryID,
            pParam->FilenameID,
            FST_FMT_JPG,
            (UB *)pDesc->JpgAddr,
            &BufSize,
            FST_TIME_INFINITE)!=FST_STA_OK)
    {
        pDesc->Parent.bOK = FALSE;
        return;
    }

    //Delete Current File After Read To Memory
    FilesysDeleteFile(pParam->DirectoryID,pParam->FilenameID,FST_FMT_JPG);

    pDesc->Parent.bOK = TRUE;
    // Enable IDE video1 output
    ide_enable_video(IDE_VIDEOID_1);

}

void MsdcNvtCb_CaptureRawWidthGetInfo(void* pData)
{
    UW BufSize;
    UINT32 uiPoolAddrEnd;
    UINT uiValidFileFormat;
    PFILE_TSK_DCF_PARAM pParam;

    tMSDCEXT_INFO_CAPTURE_RAW* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_INFO_CAPTURE_RAW,pData);
    if(pDesc==NULL)
        return;

    FilesysGetValidFileFormat(&uiValidFileFormat);
    uiValidFileFormat |= FST_FMT_RAW;
    FilesysSetValidFileFormat(uiValidFileFormat);

    pDesc->Parent.bOK = FALSE;

    gCtrl.bCapFstOK = FALSE;

    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_ImgSize,  _IMGSize_10M);
    ImgCapture_SetParameter(_FileFmt,  _FileFmt_RawJpg);
    ImgCapture_SetParameter(_PreCapBitDepth, _PreBitDepth_8);
    ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
    ImgCapture_SetParameter(_DriveMode,  _IMGDriveMode_Single);
    ImgCapture_StartCapture();

    while(!gCtrl.bCapFstOK)
        TimerDelayMs(100);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    pParam = FilesysGetFSTParam();

    //Delete Jpg First
    FilesysDeleteFile(pParam->DirectoryID,pParam->FilenameID,FST_FMT_JPG);

    if (get_blk((void*)&uiPoolAddrEnd, POOL_ID_CAPTURE) != E_OK)
    {
        pDesc->Parent.bOK = FALSE;
        return;
    }
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddrEnd);
    uiPoolAddrEnd = uiPoolAddrEnd + POOL_SIZE_CAPTURE;

    pDesc->RawWidth=GetIPPSizeInfor(_PreCropCapHsize);
    pDesc->RawHeight=GetIPPSizeInfor(_PreCropCapVsize);
    pDesc->RawNbit = 8;
    pDesc->RawFileSize = pDesc->RawWidth*pDesc->RawHeight*((pDesc->RawNbit+7)>>3);
    //32Bytes Alignment for File System
    pDesc->RawAddr = (uiPoolAddrEnd - pDesc->RawFileSize)&(~0x1F);

    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    //Read To Read File
    BufSize = (UW)pDesc->RawFileSize;
    if(FilesysReadWriteSpeciFile(FST_CMD_READ_SPECIFIC_FILE,
            pParam->DirectoryID,
            pParam->FilenameID,
            FST_FMT_RAW,
            (UB *)pDesc->RawAddr,
            &BufSize,
            FST_TIME_INFINITE)!=FST_STA_OK)
    {
        pDesc->Parent.bOK = FALSE;
        return;
    }

    //Delete Current File After Read To Memory
    FilesysDeleteFile(pParam->DirectoryID,pParam->FilenameID,FST_FMT_RAW);

    pDesc->Parent.bOK = TRUE;
}
