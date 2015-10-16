/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       USBPCC.h
    @ingroup    mISYSUPCC

    @brief      USB PCC device class API header.
                USB PCC device class API header.

*/

#ifndef _USBPCC_H
#define _USBPCC_H

/*-----------------------------------
    Includes
-----------------------------------*/
#include "Type.h"
#include "USB.h"

/**
    \addtogroup mISYSUPCC
@{
*/

/**
    USB Infomation Table for PCC.
*/
typedef enum {
    PCC_MAX_SIZE_VGA,
    PCC_MAX_SIZE_HD,
    //#NT#2012/11/28#KS Hung -begin
    PCC_MAX_SIZE_2592x1944
    //#NT#2012/11/28#KS Hung -end
} PCC_MAX_SIZE;

typedef struct {

    USB_STRING_DESC     *pManuStringDesc;    ///< USB Manufacturer String Descriptor ptr
    USB_STRING_DESC     *pProdStringDesc;    ///< USB Product String Descriptor ptr
    USB_STRING_DESC     *pSerialStringDesc;  ///< USB Serial Number String Descriptor ptr
    UINT16              VID;                 ///< Customized USB Vendor ID
    UINT16              PID;                 ///< Customized USB Product ID
    UINT32              UsbPccMemAdr;        ///< Buffer address for USB operation
    UINT32              UsbPccMemSize;       ///< Buffer size for USB operation
    //#NT#2009/11/22#SA4 KS Hung -begin
    //#NT#add to control PCC vendor function
    BOOL                bEnableVendorFunc;   ///< Enable PCC vendor function or not
    //#NT#2009/11/22#SA4 KS Hung -end
    //#NT#2011/01/05#Hideo Lin -begin
    //#NT#For PCC maximum size setting
    PCC_MAX_SIZE        PccMaxSize;          ///< PCC maximum size
    //#NT#2011/01/05#Hideo Lin -end

    //******************************************************************************
    // Flash CallBack, Feb 10, 2006 Added.
    //******************************************************************************
    UINT32    (*BurnIn)(UINT32 BlockID, UINT8 *Buf, UINT32 Length);   //CallBack Function for Nane Burn-In
    UINT32    (*IsFinish)(void);                                      //CallBack Function for Check Ready
    UINT32    (*FlashRead)(UINT32 BlockID, UINT32 Offset, UINT32 *Size, UINT8 *RBuf);      //CallBack Function for flash read

    //#NT#2008/08/14#Irene -begin
    //#NT#CallBack Function for Vendor Request
    // New:
    // 1. Add a callback function for vendor request
    UINT32    (*VendorProcess)(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf);
    //#NT#2008/08/14#Irene -end

} USB_PCC_INFO;

//#NT#2009/11/22#SA4 KS Hung -begin
//#NT#add for CSC PC calibration
typedef void (*FPSETPCC2CAPRAWCB)(UINT *rawAddr, UINT *rawWidth, UINT *rawHeight, UINT *rawBitDepth, UINT *jpgAddr, UINT *jpgSize);

extern UINT     UPccOpen(USB_PCC_INFO *pClassInfo);
extern void     UPccClose(void);
extern void     USBPccVideoTsk(void);
extern void     USBPccRegSetPCC2CapRawCB(FPSETPCC2CAPRAWCB fp);

//#NT#2009/09/25#KH Chang -begin
// Register vendor command.
extern ER       UPccRegisterVendorCmd(UINT8 startReq, UINT8 endReq);
//#NT#2009/09/25#KH Chang -end
//#NT#2009/11/22#SA4 KS Hung -end
//@}

#endif  // _USBPCC_H

