#ifndef _MSDCNVTCB_PHOTO_H
#define _MSDCNVTCB_PHOTO_H

#include "ImageAlgInfor.h"

//APIs for Device UI Flow
FPALGCALLBACK MsdcNvt_HookImageCb(FPALGCALLBACK HookCb);

//APIs for MSDCVendorNVT_AddCallback_Bi
void MsdcNvtCb_PhotoBegin(void* pData);
void MsdcNvtCb_GetDisplaySettings(void* pData);
void MsdcNvtCb_GetDisplayCurrentBufSel(void* pData);
void MsdcNvtCb_CaptureJpgWidthGetInfo(void* pData);
void MsdcNvtCb_CaptureRawWidthGetInfo(void* pData);

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_BICALL_REG_PHOTO() \
    REGISTER_BI_CALLBACK(MsdcNvtCb_PhotoBegin); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_GetDisplaySettings); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_GetDisplayCurrentBufSel); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_CaptureJpgWidthGetInfo); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_CaptureRawWidthGetInfo);
    
#endif