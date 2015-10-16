/**
    Header file for USB device

    This file is the header file for USB device module driver

    @file       usb.h
    @ingroup    mIUSBDevice
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _USB_H
#define _USB_H

#include <Type.h>

#include "usb_desc.h"
#include "usb_define.h"

/**
    @addtogroup mIUSBDevice
*/
//@{


/**
    Endpoint Block number used
*/
typedef enum
{
    BLKNUM_NOT_USE,                             ///< block not used
    BLKNUM_SINGLE,                              ///< use single block 
    BLKNUM_DOUBLE,                              ///< use double block 
    BLKNUM_TRIPLE,                              ///< use triple block 
    ENUM_DUMMY4WORD(USB_EP_BLKNUM)
} USB_EP_BLKNUM;

/**
    Endpoint direction
*/
typedef enum
{
    EP_DIR_NOT_USE,                             ///< Endpoint direction not set
    EP_DIR_IN,                                  ///< Endpoint direction is IN
    EP_DIR_OUT,                                 ///< Endpoint direction is OUT
    ENUM_DUMMY4WORD(USB_EP_DIR)
} USB_EP_DIR;

/**
    Endpoint transfer type
*/
typedef enum
{
    EP_TYPE_NOT_USE,                            ///< Endpoint transfer type not set
    EP_TYPE_ISOCHRONOUS,                        ///< Endpoint transfer type is isochronous
    EP_TYPE_BULK,                               ///< Endpoint transfer type is bulk
    EP_TYPE_INTERRUPT,                          ///< Endpoint transfer type is interrupt
    ENUM_DUMMY4WORD(USB_EP_TYPE)
} USB_EP_TYPE;

/**
    transaction number for high bandwidth endpoint
*/
typedef enum
{
    HBW_NOT,                                    ///< not high bandwidth
    HBW_1,                                      ///< one transaction
    HBW_2,                                      ///< two transaction
    HBW_3,                                      ///< three transaction
    ENUM_DUMMY4WORD(USB_EP_HBW)
} USB_EP_HBW;

/**
   USB controller state
*/
typedef enum
{
    USB_CONTROLLER_STATE_NONE,                  ///< USB controller state is none
    USB_CONTROLLER_STATE_RESET,                 ///< USB controller state is reset
    USB_CONTROLLER_STATE_SUSPEND,               ///< USB controller state is suspend
    ENUM_DUMMY4WORD(USB_CONTROLLER_STATE)
} USB_CONTROLLER_STATE;

/**
   USB End Point number
*/
typedef enum
{
    USB_EP0,                                    ///< USB End Point number 0
    USB_EP1,                                    ///< USB End Point number 1
    USB_EP2,                                    ///< USB End Point number 2
    USB_EP3,                                    ///< USB End Point number 3
    USB_EP4,                                    ///< USB End Point number 4
    USB_EP5,                                    ///< USB End Point number 5
    USB_EP6,                                    ///< USB End Point number 6
    USB_EP7,                                    ///< USB End Point number 7
    USB_EP8,                                    ///< USB End Point number 8
    ENUM_DUMMY4WORD(USB_EP)
} USB_EP;


/**
    Endpoint configuration
*/
typedef  struct Endpoint_Config {
    BOOL           Enable;                      ///< enable this endpoint or not
    UINT32         BlkSize;                     ///< block size : 64/512/1024
    USB_EP_BLKNUM  BlkNum;                      ///< block number : single/double/triple
    USB_EP_DIR     Direction;                   ///< IN or OUT drection
    USB_EP_TYPE    TrnsferType;                 ///< bulk/interrupt/isochronous
    UINT32         MaxPktSize;                  ///< max packet size
    USB_EP_HBW     HighBandWidth;               ///< transzction number for high bandwidth endpoint
} USB_ENP_CONFIG;


/**
    USB Management Infomation Table
*/
typedef struct {
    USB_DEVICE_DESC  *pDevDesc;                 ///< USB Device Descriptor ptr
    USB_CONFIG_DESC  *pConfigDesc;              ///< USB Config Descriptor ptr
    USB_CONFIG_DESC  *pConfigOthersDesc;        ///< USB Config Descriptor ptr
    USB_CONFIG_DESC  *pConfigDescHS;            ///< USB HighSpeed Descriptor
    USB_CONFIG_DESC  *pConfigDescFS;            ///< USB FullSpeed Descriptor
    USB_CONFIG_DESC  *pConfigDescFSOther;       ///< USB HighSpeed Descriptor
    USB_CONFIG_DESC  *pConfigDescHSOther;       ///< USB FullSpeed Descriptor
    USB_ENP_CONFIG   EPxConfig_HS[8];           ///< set Highspeed endpoint configuration
    USB_ENP_CONFIG   EPxConfig_FS[8];           ///< set Fullspeed endpoint configuration
    USB_STRING_DESC  *pStringDesc[4];           ///< USB String Descriptor ptr
    USB_DEVICE_DESC  *pDevQualiDesc;            ///< USB Qualifier ptr; High speed vs low speed.
    unsigned short   bNumConfigurations;        ///< configuration number
    unsigned short   bNumStrings;               ///< string number
    void             (*fpOpen_Needed_FIFO)(void);  ///< callback function to enable/disable FIFO
    void             (*fpEvent_CB)(UINT32 uEvent); ///< ISR callback function
    UINT8            USBType;                   ///< USB_MSD, USB_SIDC, USB_PCC

    //For the USB device mode require to know device enter suspend mode.
    //If the USB device don't care suspend mode, set this callback function as NULL.
    FP               fUSB_SuspendFun;           ///< callback function pointer for suspend
} USB_MNG;


/**
    Interface and Endpoint Descriptor data
*/
typedef struct {
    USB_INTERFACE_DESC  InterfaceDesc;          ///< Interface Descriptor
    USB_ENDPOINT_DESC   *pEndpointDesc;         ///< pointer of EndpointDescriptor
} USB_IF_EP_DESC_DATA;


/** 
    Interface of Configuration and Interface Descriptor 
*/
typedef struct {
    UINT8               bInterfaceNumber;       ///< Interface Number
    UINT8               bNumAltSet;             ///< Number of AlternateSetting
    USB_IF_EP_DESC_DATA *pIf_Ep_Data;           ///< pointer of Interface and
                                                ///< EndpointDescriptor data
} USB_CFG_IF_DESC_INTF;

/** 
    Configuration Descriptor data 
*/
typedef struct {
    USB_CONFIG_DESC       ConfigDesc;           ///< Config Descriptor
    USB_CFG_IF_DESC_INTF  *pCfg_If_Intf;        ///< interface between configuration and interface descriptor
} USB_CONFIG_DESC_DATA;

/**
    current configuration
*/
typedef struct {
    UCHAR   config;                             ///< current config number
    UCHAR   alt[1];                             ///< current alternate settings
} USB_CONFIG;

/**
    device request
*/
typedef struct {
    UINT8       bmRequestType;                  ///< request type               
    UINT8       bRequest;                       ///< request                    
    UINT16      wValue;                         ///< value of bRequest defined  
    UINT16      wIndex;                         ///< value of bRequest defined  
    UINT16      wLength;                        ///< data length of data stage  
} USB_DEVICE_REQUEST;

/**
    control transfer
*/
typedef struct {
    UINT8          *pData;                      ///< data pointer
    USB_DEVICE_REQUEST DeviceRequest;           ///< device request
    unsigned short wLength;                     ///< length
    unsigned short reserved;                    ///< reserved
} CONTROL_XFER;

/**
    setup data
*/
typedef struct {
    USB_DEVICE_REQUEST  DevReq;                                  ///< device request             
    UCHAR               SetupData[512];                          ///< received setup data        
    UCHAR               DevDescData[USB_DEV_LENGTH];             ///< device descriptor          
    UCHAR               CfgDescData[USB_CONFIG_DESC_TOTAL_SIZE]; ///< config descriptor          
    UCHAR               *pCfgDescData[USB_NUM_CONFIG];           ///< pointer of config descriptor */
} USB_SETUP_DATA;


extern USB_MNG  gUSBMng;          // USB management information
extern CONTROL_XFER  ControlData;

extern FP *gfpVendorRequest;
extern FP gfpClassRequest;


// USB Driver API, in <usb.c>
extern ER usb_open(void);
extern void usb_PowerOnInit(BOOL bHighSpeed);
extern BOOL usb_StateChange(void); 
extern ER usb_close(void);
extern void usb_SetStandByWakeup(FP fpIsr);
extern void usb_setControllerState(USB_CONTROLLER_STATE eUSBCtrlState);
extern USB_CONTROLLER_STATE usb_getControllerState(void);
//#NT#2010/08/05#HH Chuang -begin
//#NT#support charger/host detection
extern void usb_setChargerDetection(BOOL bEnable);
extern void usb_setChargerDetectTimeout(UINT32 uiTimeout);
//#NT#2010/08/05#HH Chuang -end

// USB read /write functions, in <usb_EPConfig.c>
extern void usb_RetSetupData(void);
extern ER usb_readEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_writeEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_abortEndpoint(USB_EP EPn);
extern ER usb_setEPRead(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_setEPWrite(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);
extern ER usb_waitEPDone(USB_EP EPn, UINT32 *pDMALen);
// USB endpoint and FIFO setting functions
extern void usb_maskEPINT(USB_EP EPn);
extern void usb_unmaskEPINT(USB_EP EPn);
extern void usb_setEP0Done(void);
extern void usb_setEP0Stall(void);
extern void usb_setEPinStall(USB_EP EPn);
extern void usb_setEPoutStall(USB_EP EPn);
extern void usb_clrEPFIFO(USB_EP EPn);
extern void usb_clrFIFO(void);
extern void usb_setTX0Byte(USB_EP EPn);
extern ULONG usb_getEPByteCount(USB_EP EPn);
//#NT#2010/04/21#KH Chang -begin
extern UINT32 usb_getSOF(void);
//#NT#2010/04/21#KH Chang -end

// USB misc functions, in <usb_misc.c>
extern void usb_EnableHighSpeed(BOOL bHighSpeed);
extern BOOL usb_isHighSpeedEnabled(void);
extern BOOL usb_CxLoopbackTest(void);

//@}

#endif // _USB_H
