#ifndef _MSDCNVTCB_LCD_H
#define _MSDCNVTCB_LCD_H

//APIs for MSDCVendorNVT_AddCallback_Bi
void MsdcNvtCb_LcdGetFreeMem(void* pData); //!< Get Free Memory Pool
void MsdcNvtCb_LcdOpen(void* pData);       //!< Set Lcd Register & Ide Parameters
void MsdcNvtCb_LcdClose(void* pData);      //!< Close LCD
void MsdcNvtCb_LcdShowImage(void* pData);  //!< Show Test Pattern

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_BICALL_REG_LCD() \
    REGISTER_BI_CALLBACK(MsdcNvtCb_LcdGetFreeMem); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_LcdOpen); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_LcdClose); \
    REGISTER_BI_CALLBACK(MsdcNvtCb_LcdShowImage);
    
#endif