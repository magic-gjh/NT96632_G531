#ifndef _MSDCNVTCB_DISP_H
#define _MSDCNVTCB_DISP_H

//APIs for MSDCVendorNVT_AddCallback_Bi
void MsdcNvtCb_DispCfgGet(void* pData);             //!< Get OSD1/OSD2/VDO1/VDO2 Settings
void MsdcNvtCb_DispVdoBufGetCurrIdx(void* pData);   //!< Get Video1,2 Current Buffer Selection
void MsdcNvtCb_DispEngineEnableSet(void* pData);    //!< Set Engine Enable / Disable
void MsdcNvtCb_DispPaletteGet(void* pData);         //!< Get OSD Palette
void MsdcNvtCb_DispPaletteSet(void* pData);         //!< Set OSD Palette
void MsdcNvtCb_DispGammaEnableSet(void* pData);     //!< Set Gamma Enable / Disable
void MsdcNvtCb_DispGammaTblGetSize(void* pData);    //!< Get Gamma Table Size
void MsdcNvtCb_DispGammaTblSetTbl(void* pData);     //!< Set Gamma Table
void MsdcNvtCb_DispGammaTblGetTbl(void* pData);     //!< Get Gamma Table
void MsdcNvtCb_DispIcstGetInfo(void* pData);        //!< Get Default ICST Coefficients
void MsdcNvtCb_DispIcstSet(void* pData);            //!< Set ICST Coefficients
void MsdcNvtCb_DispIcstGet(void* pData);            //!< Get ICST Coefficients
void MsdcNvtCb_DispSaturationGetInfo(void* pData);  //!< Get Saturation Default
void MsdcNvtCb_DispSaturationSet(void* pData);      //!< Set Saturation
void MsdcNvtCb_DispContrastGetInfo(void* pData);    //!< Get Contrast Default
void MsdcNvtCb_DispContrastSet(void* pData);        //!< Set Contrast
void MsdcNvtCb_DispBrightnessGetInfo(void* pData);  //!< Get Brightness Default
void MsdcNvtCb_DispBrightnessSet(void* pData);      //!< Set Brightness


//Macro for Regisetr Disp Callbacks
#define MSDCNVT_BICALL_REG_DISP() \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispCfgGet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispVdoBufGetCurrIdx); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispEngineEnableSet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispPaletteGet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispPaletteSet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispGammaEnableSet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispGammaTblGetSize); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispGammaTblSetTbl); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispGammaTblGetTbl); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispIcstGetInfo); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispIcstSet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispIcstGet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispSaturationGetInfo); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispSaturationSet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispContrastGetInfo); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispContrastSet); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispBrightnessGetInfo); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_DispBrightnessSet);

#endif
