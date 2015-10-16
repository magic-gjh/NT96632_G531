/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitUSB.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for USB mode
                Get memory and set parameters for USB mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "AppInitUSB.h"
#include "USBMakerParamInit.h"
#include "GlobalVar.h"
#include "PrimaryTsk.h"
#include "PlaybackTsk.h"
#include "PhotoTsk.h"
#include "UIFlow.h"
#include "UISystemStatusSettings.h"
#include "KeyScanTsk.h"
//#NT#2012/03/02#Niven Cho -begin
#include "MsdcVendorSample.h"
//#NT#2012/03/02#Niven Cho -end
#include "GPIOMapping.h"
#include "NAND.h"
#include "RamDisk.h"
//#NT#2009/03/24#Marx Chiu -begin
#include "DPS_Header_Public.h"
#include "PTP_API.h"
//#NT#2009/03/24#Marx Chiu -end
//#NT#2011/10/12#HH Chuang -begin
//#NT#support multi partition on single SD card
#include "virtualSDIO.h"
//#NT#2011/10/12#HH Chuang -end

//for USBPCC mode
extern void     FlowPhoto_ImageCB(UINT32 uiEventID, AlgMsgInfo *pInfo);

//#NT#2009/11/25#SA4 KS Hung -begin
//#NT#Color Shading Calibration with PC AP. Only for HD500C3 project.
static BOOL g_bAppInitUSBPCCVendorFunc = FALSE;
//#NT#2009/11/25#SA4 KS Hung -end


#if (_USBMSDC_MODE_ == ENABLE)
/**
  Initialize application for USB MSDC mode

  Initialize application for USB MSDC mode.

  @return void
*/
void MSDC_LED_CB(void)
{
    debug_msg("@");
}

void AppInit_ModeUSBMSDC(void)
{
    USB_MSDC_INFO       MSDCInfo;
    UINT32              uiPoolAddr,i;
    PrimaryStorageID    uiStgType;
    PSTRG_TAB           pStrgObj;

    // Get buffer memory for MSDC task, MSDC share the same buffer memory with SICD,
    // make sure the buffer memory size is enough for both
    get_blk((VP*)&uiPoolAddr, POOL_ID_SICD);
    rel_blk(POOL_ID_SICD, (VP)uiPoolAddr);

    MSDCInfo.uiMsdcBufAddr = uiPoolAddr;
    MSDCInfo.uiMsdcBufSize = POOL_SIZE_SICD;
    USBMakerInit_UMSD(&MSDCInfo);

    //The callback functions for the MSDC Vendor command.
    //If project doesn't need the MSDC Vendor command, set this callback function as NULL.
    MSDCInfo.msdc_check_cb = NULL;
    // Vendor command callback function
    MSDCInfo.msdc_vendor_cb = NULL;
    // Read / Write MSDC callback function
    //MSDCInfo.msdc_RW_Led_CB = MSDC_LED_CB;
    MSDCInfo.msdc_RW_Led_CB = NULL;
    // Host issue StopUnit command callback function
    MSDCInfo.msdc_StopUnit_Led_CB = NULL;
    // Host issue USB suspend
    MSDCInfo.msdc_UsbSuspend_Led_CB = NULL;

    // Set Default Luns as Storage
    for(i=0;i<MAX_LUN;i++)
        MSDCInfo.msdc_type[i] = MSDC_STRG;

//#NT#2011/10/12#HH Chuang -begin
//#NT#support multi partition on single SD card
#if ((_MSDC_MODE_ == _MSDC_SINGLE_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_)||(_MSDC_MODE_ == _MSDC_SINGLE_NAND3_)||(_MSDC_MODE_ == _MSDC_SINGLE_VSDIO2_))
//#if ((_MSDC_MODE_ == _MSDC_SINGLE_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_)||(_MSDC_MODE_ == _MSDC_SINGLE_NAND3_))
//#NT#2011/10/12#HH Chuang -end

    //Get the idChangeToStorage to distinguish bad FAT from bad card
    uiStgType = Primary_GetChangeStorage();
    pStrgObj = Primary_GetStorageObj(uiStgType);

    MSDCInfo.pStrgDevObj[0] = pStrgObj;

    if(uiStgType == PRIMARY_STGTYPE_NAND)
    {
        MSDCInfo.msdc_storage_detCB[0] = NULL;
        MSDCInfo.msdc_strgLock_detCB[0] = NULL;
    }
    else if(uiStgType == PRIMARY_STGTYPE_CARD)
    {
        MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)GPIOMap_DetStrgCard;
        MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)GPIOMap_DetStrgCardWP;
    }

#if (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_)
    MSDCInfo.pStrgDevObj[1] = nand2_getStorageObject();
    MSDCInfo.msdc_storage_detCB[1] = NULL;
    MSDCInfo.msdc_strgLock_detCB[1] = NULL;

    MSDCInfo.LUNs = 2;

    //#NT#2008/03/26#Johnny Wu -begin
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_type[1] = MSDC_CDROM;
    //#NT#2008/03/26#Johnny Wu -end
//#NT#2009/01/06#Cliff Lin -begin
//#Added for the third Nand region
#elif (_MSDC_MODE_ == _MSDC_SINGLE_NAND3_)

    MSDCInfo.pStrgDevObj[1] = nand3_getStorageObject();
    MSDCInfo.msdc_storage_detCB[1] = NULL;
    MSDCInfo.msdc_strgLock_detCB[1] = NULL;
    MSDCInfo.LUNs = 2;
//#NT#2009/01/06#Cliff Lin -end

    //#NT#2008/03/26#Johnny Wu -begin
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_type[1] = MSDC_CDROM;
    //#NT#2008/03/26#Johnny Wu -end
//#NT#2011/10/12#HH Chuang -begin
//#NT#support multi partition on single SD card
#elif (_MSDC_MODE_ == _MSDC_SINGLE_VSDIO2_)
    MSDCInfo.pStrgDevObj[1] = virtualSdio2_getStorageObject();
    MSDCInfo.msdc_storage_detCB[1] = NULL;
    MSDCInfo.msdc_strgLock_detCB[1] = NULL;
    MSDCInfo.LUNs = 1;

    MSDCInfo.msdc_type[0] = MSDC_STRG;
    //MSDCInfo.msdc_type[1] = MSDC_CDROM;
//#NT#2011/10/12#HH Chuang -end
    #else
    MSDCInfo.LUNs = 1;

    //#NT#2008/03/26#Johnny Wu -begin
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    //#NT#2008/03/26#Johnny Wu -end
    #endif

#elif ((_MSDC_MODE_ == _MSDC_ALL_) || (_MSDC_MODE_ == _MSDC_ALL_NAND2_) || (_MSDC_MODE_ == _MSDC_ALL_NAND3_))

    MSDCInfo.pStrgDevObj[0] = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);
    #if (_RAM_DISK_ == ENABLE)
        MSDCInfo.pStrgDevObj[1] = ramdsk_getStorageObject();
    #else
        MSDCInfo.pStrgDevObj[1] = Primary_GetStorageObj(PRIMARY_STGTYPE_NAND);
    #endif

    MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)GPIOMap_DetStrgCard;
    MSDCInfo.msdc_storage_detCB[1] = NULL;

    MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)GPIOMap_DetStrgCardWP;
    MSDCInfo.msdc_strgLock_detCB[1] = NULL;

#if (_MSDC_MODE_ == _MSDC_ALL_NAND2_)
    #if (_RAM_DISK_ == ENABLE)
        MSDCInfo.pStrgDevObj[2] = ramdsk2_getStorageObject();
    #else
        MSDCInfo.pStrgDevObj[2] = nand2_getStorageObject();
    #endif
    MSDCInfo.msdc_storage_detCB[2] = NULL;
    MSDCInfo.msdc_strgLock_detCB[2] = NULL;

    MSDCInfo.LUNs = 3;

    //#NT#2008/03/26#Johnny Wu -begin
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_type[1] = MSDC_STRG;
    MSDCInfo.msdc_type[2] = MSDC_CDROM;
    //#NT#2008/03/26#Johnny Wu -end

#elif (_MSDC_MODE_ == _MSDC_ALL_NAND3_)

    MSDCInfo.pStrgDevObj[2] = nand2_getStorageObject();
    MSDCInfo.msdc_storage_detCB[2] = NULL;
    MSDCInfo.msdc_strgLock_detCB[2] = NULL;
    MSDCInfo.pStrgDevObj[3] = nand3_getStorageObject();
    MSDCInfo.msdc_storage_detCB[3] = NULL;
    MSDCInfo.msdc_strgLock_detCB[3] = NULL;
    MSDCInfo.LUNs = 4;

    //#NT#2009/02/03#Hideo Lin -begin
    //#NT#For NAND3 (4 LUNs)
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_type[1] = MSDC_STRG;
    MSDCInfo.msdc_type[2] = MSDC_STRG;
    MSDCInfo.msdc_type[3] = MSDC_CDROM;
    //#NT#2009/02/03#Hideo Lin -end
#else
    MSDCInfo.LUNs = 2;

    //#NT#2008/03/26#Johnny Wu -begin
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_type[1] = MSDC_CDROM;
    //#NT#2008/03/26#Johnny Wu -end
#endif


#endif

    //#NT#2012/03/02#Niven Cho -begin
    #if (_MSDCVENDOR_== _MSDCVENDOR_NVT_)
    //Attach Slim MsdcNvt for Access Memory & Update F/W Only
    //這裡可以改成按某Key插入USB後,才Call這支Function
    /*
    //------------------------------------------------------------------------------
    // Single Module Selection
    //------------------------------------------------------------------------------
    #define SI_MODULE_NONE          0  //!< No Single Module
    #define SI_MODULE_CUSTOM_SI     1  //!< Customer Develop Module
    #define SI_MODULE_IQSIM         2  //!< Image Quality Simulation Tool
    #define SI_MODULE_TIME_SI       3  //!< PC Time Sync
    */
    if (MsdcNvt_GetSIModule()==3)
        MsdcNvt_Attach(&MSDCInfo);
    #endif
    //#NT#2012/03/02#Niven Cho -end

    // Open MSDC task
    if (USBMSDC_Open(&MSDCInfo) != E_OK)
    {
        debug_err(("Error open USB MSDC task\r\n"));
    }

    #if (_MP3_MODE_ == ENABLE)
    // Set mp3 filetable renew request to on (coz new files may enter current storage)
    Set_MP3FtbRenewIndex(MP3FTB_RENEW_ON);
    #endif
}
#endif

#if (_USBSICD_MODE_ == ENABLE)
/**
  Initialize application for USB SICD mode

  Initialize application for USB SICD mode.

  @param void
  @return void
*/
void AppInit_ModeUSBSICD(void)
{
    //#NT#2009/03/24#Marx Chiu -begin
    //MarxChiu_20090324 USB_SICD_INFO   SICDInfo;
    USB_SICD_INFO   SICDInfo;
    //#NT#2009/03/24#Marx Chiu -end
    //PLAY_OBJ        PlayObj;
//#NT#2007/03/23#Meg Lin -begin
//need a buffer to save coming file !!

    UINT32          uiPoolAddr, uiPoolAddr2;
//#NT#2007/03/22#Meg Lin -begin
    //cardLock should be set to usicd_open()

    PrimaryStorageID    uiStgType;
    PSTRG_TAB           pStrgObj;

    // Get memory for SICD task
    get_blk((VP*)&uiPoolAddr, POOL_ID_SICD);
    rel_blk(POOL_ID_SICD, (VP)uiPoolAddr);

    //get ID_IQ for testing !!! maybe a new block needed ??
    get_blk((VP*)&(uiPoolAddr2), POOL_ID_IQ);
    rel_blk(POOL_ID_IQ, (VP)uiPoolAddr2);

    //#NT#2009/03/24#Marx Chiu -begin
    //MarxChiu_20090324 usbsicd_setSendObjBufAddrSize((UINT32)uiPoolAddr2, 0x100000);
    PTP_setSendObjBufAddrSize((UINT32)uiPoolAddr2, 0x100000);
    //#NT#2009/03/24#Marx Chiu -end

//#NT#2007/03/23#Meg Lin -end

    //#NT#2009/03/24#Marx Chiu -begin
    /*MarxChiu_20090324
    SICDInfo.uiSicdBufAddr = uiPoolAddr;
    SICDInfo.uiSicdBufSize = POOL_SIZE_SICD;
    MarxChiu_20090324*/
    USBMakerInit_USICD(&SICDInfo);
    PTP_Init(uiPoolAddr,POOL_SIZE_SICD);
    //#NT#2009/03/24#Marx Chiu -end

    // Query current active storage
    uiStgType = Primary_GetStorage();
    pStrgObj = Primary_GetStorageObj(uiStgType);

    // Check card lock status, only storage card has this status, where NAND flash does not
    if (uiStgType == PRIMARY_STGTYPE_CARD)
    {
        SICDInfo.uiStrgCardLock = KeyScan_IsStrgCardLocked();
    }
    else if (uiStgType == PRIMARY_STGTYPE_NAND)
    {
        SICDInfo.uiStrgCardLock = FALSE;
    }

    //#NT#2009/03/24#Marx Chiu -begin
    SICDInfo.fpCommandProcess = PTP_CommandProcess;
    SICDInfo.fpStopTransfer   = PTP_StopTransfer;
    SICDInfo.fpDeviceReset    = PTP_ResetDevice;
    //#NT#2009/03/24#Marx Chiu -end

    // Open SICD task
    //#NT#2009/03/24#Marx Chiu -begin
    //MarxChiu_20090324 if (usbsicd_open(pStrgObj, &SICDInfo) != E_OK)
    if (Usicd_Open(pStrgObj, &SICDInfo) != E_OK)
    //#NT#2009/03/24#Marx Chiu -end
    {
        debug_err(("Error open USB SICD task\r\n"));
    }

    //#NT#2009/03/24#Marx Chiu -begin
    DPS_Init();
    //#NT#2009/03/24#Marx Chiu -end
}
#endif

#if (_USBPCC_MODE_ == ENABLE)
/**
  Initialize application for USB PCC mode

  Initialize application for USB PCC mode.

  @param void
  @return void
*/
void AppInit_ModeUSBPCC(void)
{
    USB_PCC_INFO    PCCInfo;
    UINT32          uiPoolAddr;
    PHOTO_APPOBJ    PhotoObj;

    // Get memory for Photo task
    get_blk((VP *)&uiPoolAddr, POOL_ID_IQ);
    rel_blk(POOL_ID_IQ, (VP)uiPoolAddr);

    PhotoObj.uiMemoryAddr       = uiPoolAddr;
    PhotoObj.uiMemorySize       = POOL_SIZE_IQ;
    PhotoObj.CallBackFunc       = (FPALGCALLBACK)FlowPhoto_ImageCB;
    PhotoObj.uiDisplayFBWidth   = g_LCDSize.uiWidth;
    PhotoObj.uiDisplayFBHeight  = g_LCDSize.uiHeight;
    // Open Photo task for AE & AWB
    if (Photo_Open(&PhotoObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for photo task\r\n"));
    }

    // Register AE & AWB function pointer
    Photo_RegisterAE((FPPHOTOAE)aaa_AEprocess);
    Photo_RegisterAWB((FPPHOTOAWB)aaa_AWBprocess);

    // Reserved capture buffer for image capturing and PCC use SICD buffer
    // Get memory for PCC task
#if 0
    get_blk((VP*)&uiPoolAddr, POOL_ID_SICD);
    rel_blk(POOL_ID_SICD, (VP)uiPoolAddr);

    PCCInfo.UsbPccMemAdr    = uiPoolAddr;
    PCCInfo.UsbPccMemSize   = POOL_SIZE_SICD;
#else // for PCC 720p (need more buffer; use last 2MB of POOL_ID_CAPTURE temporary)
    get_blk((VP*)&uiPoolAddr, POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    PCCInfo.UsbPccMemAdr        = uiPoolAddr + POOL_SIZE_CAPTURE - 0x200000; // POOL_SIZE_CAPTURE should be larger than 2MB
    PCCInfo.UsbPccMemSize       = 0x200000;
#endif
    PCCInfo.BurnIn          = NULL;
    PCCInfo.FlashRead       = NULL;
    PCCInfo.IsFinish        = NULL;
    //#NT#2009/11/25#SA4 KS Hung -begin
    //#NT#Color Shading Calibration with PC AP. Only for HD500C3
    PCCInfo.bEnableVendorFunc = g_bAppInitUSBPCCVendorFunc;
    //#NT#2009/11/25#SA4 KS Hung -end
    //#NT#2011/01/05#Hideo Lin -begin
    //#NT#For PCC maximum size setting
    PCCInfo.PccMaxSize     = PCC_MAX_SIZE_HD;
    //PCCInfo.PccMaxSize     = PCC_MAX_SIZE_VGA;
    //#NT#2011/01/05#Hideo Lin -end

    USBMakerInit_UPCC(&PCCInfo);

    if (UPccOpen(&PCCInfo) != E_OK)
    {
        debug_err(("Error open USB PCC task\r\n"));
    }
}
#endif

//@}
