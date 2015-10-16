#include "SysCfg.h"
#include "UMSDC.h"
#include "MSDCVendor.h"
#include "MSDCVendorNVTApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Int.h"
#include "USBMakerParamInit.h"
#include "CommandTsk.h"

#if(_MSDCVENDOR_ == _MSDCVENDOR_NVT_)

//------------------------------------------------------------------------------
// Multi Module Selection
//------------------------------------------------------------------------------
#define CFG_MSDC_VENDOR_PHOTO   ENABLE  //!< Preview / Capture
#define CFG_MSDC_VENDOR_LCD     ENABLE  //!< LCD Driver Adjustment
#define CFG_MSDC_VENDOR_DISP    ENABLE  //!< Display Engine Adjustment (IDE Gamma)
#define CFG_MSDC_VENDOR_FILE    ENABLE  //!< Remote File System
#define CFG_MSDC_VENDOR_ADJ     ENABLE  //!< Universal Adjustment
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Single Module Selection
//------------------------------------------------------------------------------
#define SI_MODULE_NONE          0  //!< No Single Module
#define SI_MODULE_CUSTOM_SI     1  //!< Customer Develop Module
#define SI_MODULE_IQSIM         2  //!< Image Quality Simulation Tool
//#NT#2012/03/02#Niven Cho -begin
#define SI_MODULE_TIME_SI       3  //!< PC Time Sync

//Configure your Single Module. Module can be SI_MODULE_NONE / SI_MODULE_CUSTOM_SI / SI_MODULE_IQSIM..etc
#define CFG_SI_MODULE SI_MODULE_CUSTOM_SI
//#NT#2012/03/02#Niven Cho -end


//------------------------------------------------------------------------------
// Config Multi Module Include Headers
//------------------------------------------------------------------------------
#if(CFG_MSDC_VENDOR_PHOTO==ENABLE)
#include "MsdcNvtCb_Photo.h"
#endif
#if(CFG_MSDC_VENDOR_LCD==ENABLE)
#include "MsdcNvtCb_Lcd.h"
#endif
#if(CFG_MSDC_VENDOR_DISP==ENABLE)
#include "MsdcNvtCb_Disp.h"
#endif
#if(CFG_MSDC_VENDOR_FILE==ENABLE)
#include "MsdcNvtCb_File.h"
#endif
#if(CFG_MSDC_VENDOR_ADJ==ENABLE)
#include "MsdcNvtCb_Adj.h"
#endif
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Config Single Module Include Headers
//------------------------------------------------------------------------------
#if(CFG_SI_MODULE==SI_MODULE_CUSTOM_SI)
#include "MsdcNvtCb_CustomSi.h"
#elif (CFG_SI_MODULE==SI_MODULE_IQSIM)
#include "MsdcNvtCb_IQSim.h"
//#NT#2012/03/02#Niven Cho -begin
#elif (CFG_SI_MODULE==SI_MODULE_TIME_SI)
#include "MsdcNvtCb_TimeSi.h"
//#NT#2012/03/02#Niven Cho -end
#endif

//#NT#2012/03/02#Niven Cho -begin
//Msdc System will use front memory to be CBW data
#define SIZE_CBW_DATA (sizeof(UINT32)*16)
//#NT#2012/03/02#Niven Cho -end

static UINT8 MsdcVendorCache[0x24000];
static BOOL null_lun_DetStrgCard(void) {return FALSE;}
static BOOL null_lun_DetStrgCardWP(void) {return TRUE;}
static STRG_CAP null_lun_Cap = {0};
static ER null_lun_open() {return E_OK;}
static ER null_lun_writeSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt){return E_OK;}
static ER null_lun_readSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt){return E_OK;}
static ER null_lun_format(void){return E_OK;}
static ER null_lun_close(void){return E_OK;}
static ER null_lun_openMemBus(void){return E_OK;}
static ER null_lun_closeMemBus(void){return E_OK;}
static STRG_TAB NullLunObj = {STORAGE_DETACH, &null_lun_Cap, null_lun_open, null_lun_writeSectors, null_lun_readSectors, null_lun_format, null_lun_close, null_lun_openMemBus, null_lun_closeMemBus, NULL, NULL, NULL, NULL, NULL};

void MSDCVendorSample_Open(void)
{
    tMSDCVENDOR_CFG     tMsdcVendorCfg = {0};
    USB_MSDC_INFO       MSDCInfo = {0};

    //#NT#2009/09/01#Niven Cho -begin
    //#NT#Modified., Due to PC Limit Max Packet Size is 64k Bytes, so MsdcVendor work need to cache buffer with 0x24000 bytes
    tMsdcVendorCfg.pMemCache        = MsdcVendorCache;
    tMsdcVendorCfg.nSizeCache       = sizeof(MsdcVendorCache);
    tMsdcVendorCfg.SemID            = SEMID_MSDC_DBGYSYS;
    tMsdcVendorCfg.TaskID           = MSDC_DBGSYS_TSK_ID;
    tMsdcVendorCfg.fpDoCommand      = CmdTsk_DoCommand;
    //#NT#2009/12/02#KS Hung -begin
    //#NT#Only add commect
    tMsdcVendorCfg.fpRegHookDbgMsg  = debug_register_putstr;

    if(!MSDCVendorNVT_Configure(&tMsdcVendorCfg))
    {
        debug_err(("MSDCVendorNVT_Configure() Failed!\r\n"));
        return;
    }
    //#NT#2009/12/02#KS Hung -end

    //Register Bi - Direction Functions
    #if(CFG_MSDC_VENDOR_PHOTO==ENABLE)
    MSDCNVT_BICALL_REG_PHOTO();
    #endif
    #if(CFG_MSDC_VENDOR_LCD==ENABLE)
    MSDCNVT_BICALL_REG_LCD();
    #endif
    #if(CFG_MSDC_VENDOR_DISP==ENABLE)
    MSDCNVT_BICALL_REG_DISP();
    #endif
    #if(CFG_MSDC_VENDOR_FILE==ENABLE)
    MSDCNVT_BICALL_REG_FILE();
    #endif
    #if(CFG_MSDC_VENDOR_ADJ==ENABLE)
    MSDCNVT_BICALL_REG_ADJ();
    #endif

    //Register Single Direction Functions
    #if(CFG_SI_MODULE==SI_MODULE_CUSTOM_SI)
    MSDCNVT_SICALL_REG_CUSTOMSI();
    #elif (CFG_SI_MODULE==SI_MODULE_IQSIM)
    MSDCNVT_SICALL_REG_IQSIM();
    //#NT#2012/03/02#Niven Cho -begin
    #elif (CFG_SI_MODULE==SI_MODULE_TIME_SI)
    MSDCNVT_SICALL_REG_TIMESI();
    //#NT#2012/03/02#Niven Cho -end
    #endif

    MSDCInfo.uiMsdcBufAddr = (UINT32)MsdcNvt_GetDataBufferAddress();
    MSDCInfo.uiMsdcBufSize = (UINT32)MsdcNvt_GetDataBufferAddress();
    USBMakerInit_UMSD(&MSDCInfo);

    //The callback functions for the MSDC Vendor command.
    //If project doesn't need the MSDC Vendor command, set this callback function as NULL.
    MSDCInfo.msdc_check_cb = MSDCVendor_Verify_CB;
    MSDCInfo.msdc_vendor_cb = MSDCVendor_Vendor_CB;

    //Make a Null Lun
    MSDCInfo.pStrgDevObj[0] = &NullLunObj;
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)null_lun_DetStrgCard;
    MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)null_lun_DetStrgCardWP;
    MSDCInfo.LUNs = 1;

    // Open MSDC task
    if (USBMSDC_Open(&MSDCInfo) != E_OK)
    {
        debug_err(("Error open USB MSDC task\r\n"));
    }
    else
    {
        debug_msg("Enter to MSDC Ext Mode\r\n");
    }
}

void MSDCVendorSample_Close(void)
{
    USBMSDC_Close();
    debug_msg("Leave to MSDC Ext Mode\r\n");
}

//#NT#2012/03/02#Niven Cho -begin
void MsdcNvt_Attach(USB_MSDC_INFO* pInfo)
{
    tMSDCVENDOR_CFG     tMsdcVendorCfg = {0};

    tMsdcVendorCfg.pMemCache        = (UINT8*)(pInfo->uiMsdcBufAddr+SIZE_CBW_DATA);
    tMsdcVendorCfg.nSizeCache       = pInfo->uiMsdcBufSize-SIZE_CBW_DATA;
    tMsdcVendorCfg.SemID            = SEMID_MSDC_DBGYSYS;
    tMsdcVendorCfg.TaskID           = MSDC_DBGSYS_TSK_ID;
    if(!MSDCVendorNVT_Configure(&tMsdcVendorCfg))
    {
        debug_err(("MSDCVendorNVT_Configure() Failed!\r\n"));
        return;
    }

    #if(CFG_SI_MODULE==SI_MODULE_TIME_SI)
    MSDCNVT_SICALL_REG_TIMESI();
    #endif

    pInfo->msdc_check_cb = MSDCVendor_Verify_CB;
    pInfo->msdc_vendor_cb = MSDCVendor_Vendor_CB;
}
//#NT#2012/03/02#Niven Cho -end

//#NT#2012/03/05#Philex Lin -begin
UINT32 MsdcNvt_GetSIModule(void)
{
    return CFG_SI_MODULE;
}
//#NT#2012/03/05#Philex Lin -end

#endif
