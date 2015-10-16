#ifndef _MSDCNVTCB_CUSTOMSI_H
#define _MSDCNVTCB_CUSTOMSI_H

//#NT#2010/01/07#KS Hung -begin
//#NT#Color Shading Calibration
typedef struct _REG_SETTING {
    USHORT        wRegNum;
    USHORT        wValue;
} REG_SETTING;

typedef struct _SENSOR_REG_LIST {
    USHORT        wSensorID;              // 用於識別
    USHORT        wRegCount;              // 有多少個設定值
    REG_SETTING   RegSettings[103];
} SENSOR_REG_LIST;
//#NT#2010/01/07#KS Hung -end

typedef struct _RAW_DATA_INFO{
    UINT32 parentOK;
    UINT32 address;
    UINT32 width;
    UINT32 height;
}RAW_DATA_INFO;

typedef struct _DEVICE_PARAM {

    USHORT  wDataSize;          // = sizeof(DEVICE_PARAM)
    USHORT  wFactoryVer;        // = FACTORY_VER
    UINT8   byLangSel;          // 多語初設值
    UINT8   byUsbMode;          // 0=PC CAM, 1=DISK
    UINT8   byAvMode;           // 0=EAR, 1=AV Out, 2=AV In
    UINT8   byFlicker;          // 0=60Hz, 1=50Hz
    UINT8   byTvType;           // 0=NTSC, 1=PAL

    UINT8   byEngineerMode;        // 0 = Normal, 1 = Engineer Mode
    UINT8   byTVPlayBackOnly;      // 1 = Read Only
    UINT8   byMSReadOnly;          // 1 = Memory Storage Read Only
    UINT8   bySupportMp3;          // 1 = MP3 On

    UINT8   byEnableMask;
    ULONG   ulCalibItems;

} DEVICE_PARAM;

//APIs for MSDCVendorNVT_AddCallback_Si
BOOL CustomSi_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets));

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_SICALL_REG_CUSTOMSI() \
    CustomSi_Register_SiCall(MSDCVendorNVT_AddCallback_Si);

//#NT#2009/12/07#KS Hung -begin
extern void SetMsdcVendorVideoRecFlag(BOOL bFlag);
extern BOOL GetMsdcVendorVideoRecFlag(void);
//#NT#2009/12/07#KS Hung -end

#endif
