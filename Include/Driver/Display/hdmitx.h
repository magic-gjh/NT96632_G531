/**
    Header file for HDMI module

    Header file for HDMI module.

    @file       hdmitx.h
    @ingroup    mIDISPHDMITx
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef  _HDMITX_H
#define  _HDMITX_H

#include "Type.h"

/**
    @addtogroup mIDISPHDMITx
*/
//@{


/**
    HDMI-Tx output image data format

    Select the hdmi-tx output image data format, RGB444 is recommended for usage because
    it is mandatory supported format in HDMI spec. YCbCr format is just optional format in HDMI spec.
*/
//@{
typedef enum
{
    HDMI_MODE_RGB444=0,  ///<    Output format is RGB-444
    HDMI_MODE_YCBCR422,  ///<    Output format is YCbCr-422
    HDMI_MODE_YCBCR444,  ///<    Output format is YCbCr-444

    ENUM_DUMMY4WORD(HDMI_MODE)
}HDMI_MODE;
//@}





/**
    HDMI-Tx output Video Format ID
*/
//@{
typedef enum
{
    HDMI_640X480P60         =1,     ///< HDMI Video format is 640x480 & Progressive 60fps
    HDMI_720X480P60         =2,     ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
    HDMI_720X480P60_16X9    =3,     ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
    HDMI_1280X720P60        =4,     ///< HDMI Video format is 1280x720 & Progressive 60fps
    HDMI_1920X1080I60       =5,     ///< HDMI Video format is 1920x1080 & Interlaced 60fps
    HDMI_720X480I60         =6,     ///< HDMI Video format is 720x480 & Interlaced 60fps
    HDMI_720X480I60_16X9    =7,     ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
    HDMI_720X240P60         =8,     ///< HDMI Video format is 720x240 & Progressive 60fps
    HDMI_720X240P60_16X9    =9,     ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
    HDMI_1440X480I60        =10,    ///< HDMI Video format is 1440x480 & Interlaced 60fps
    HDMI_1440X480I60_16X9   =11,    ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
    HDMI_1440X240P60        =12,    ///< HDMI Video format is 1440x240 & Progressive 60fps
    HDMI_1440X240P60_16X9   =13,    ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
    HDMI_1440X480P60        =14,    ///< HDMI Video format is 1440x480 & Progressive 60fps
    HDMI_1440X480P60_16X9   =15,    ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
    HDMI_1920X1080P60       =16,    ///< HDMI Video format is 1920x1080 & Progressive 60fps
    HDMI_720X576P50         =17,    ///< HDMI Video format is 720x576 & Progressive 50fps
    HDMI_720X576P50_16X9    =18,    ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
    HDMI_1280X720P50        =19,    ///< HDMI Video format is 1280x720 & Progressive 50fps
    HDMI_1920X1080I50       =20,    ///< HDMI Video format is 1920x1080 & Interlaced 50fps
    HDMI_720X576I50         =21,    ///< HDMI Video format is 720x576 & Interlaced 50fps
    HDMI_720X576I50_16X9    =22,    ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
    HDMI_720X288P50         =23,    ///< HDMI Video format is 720x288 & Progressive 50fps
    HDMI_720X288P50_16X9    =24,    ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
    HDMI_1440X576I50        =25,    ///< HDMI Video format is 1440x576 & Interlaced 50fps
    HDMI_1440X576I50_16X9   =26,    ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
    HDMI_1440X288P50        =27,    ///< HDMI Video format is 1440x288 & Progressive 50fps
    HDMI_1440X288P50_16X9   =28,    ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
    HDMI_1440X576P50        =29,    ///< HDMI Video format is 1440x576 & Progressive 50fps
    HDMI_1440X576P50_16X9   =30,    ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
    HDMI_1920X1080P50       =31,    ///< HDMI Video format is 1920x1080 & Progressive 50fps
    HDMI_1920X1080P24       =32,    ///< HDMI Video format is 1920x1080 & Progressive 24fps
    HDMI_1920X1080P25       =33,    ///< HDMI Video format is 1920x1080 & Progressive 25fps
    HDMI_1920X1080P30       =34,    ///< HDMI Video format is 1920x1080 & Progressive 30fps
    HDMI_1920X1080I50_VT1250=39,    ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
    HDMI_1920X1080I100      =40,    ///< HDMI Video format is 1920x1080 & Interlaced  100fps
    HDMI_1280X720P100       =41,    ///< HDMI Video format is 1280X720  & Progressive 100fps
    HDMI_720X576P100        =42,    ///< HDMI Video format is 720X576   & Progressive 100fps
    HDMI_720X576P100_16X9   =43,    ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
    HDMI_720X576I100        =44,    ///< HDMI Video format is 720X576  & Interlaced 100fps
    HDMI_720X576I100_16X9   =45,    ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
    HDMI_1920X1080I120      =46,    ///< HDMI Video format is 1920X1080 & Interlaced 120fps
    HDMI_1280X720P120       =47,    ///< HDMI Video format is 1280X720  & Progressive 120fps
    HDMI_720X480P120        =48,    ///< HDMI Video format is 720X480   & Progressive 120fps
    HDMI_720X480P120_16X9   =49,    ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
    HDMI_720X480I120        =50,    ///< HDMI Video format is 720X480  & Interlaced 120fps
    HDMI_720X480I120_16X9   =51,    ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
    HDMI_720X576P200        =52,    ///< HDMI Video format is 720X576  & Progressive 200fps
    HDMI_720X576P200_16X9   =53,    ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
    HDMI_720X576I200        =54,    ///< HDMI Video format is 720X576  & Interlaced 200fps
    HDMI_720X576I200_16X9   =55,    ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
    HDMI_720X480P240        =56,    ///< HDMI Video format is 720X480  & Progressive 240fps
    HDMI_720X480P240_16X9   =57,    ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
    HDMI_720X480I240        =58,    ///< HDMI Video format is 720X480  & Interlaced 240fps
    HDMI_720X480I240_16X9   =59,    ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

    HDMI_VID_NO_CHANGE  =0xFF,  ///< HDMI Video format use current setting
    ENUM_DUMMY4WORD(HDMI_VIDEOID)
}HDMI_VIDEOID;
//@}

/**
    HDMI-Tx input audio format
*/
//@{
typedef enum
{
    HDMI_AUDIO32KHZ,                ///< HDMI input Audio is 32KHz
    HDMI_AUDIO44_1KHZ,              ///< HDMI input Audio is 44.1KHz
    HDMI_AUDIO48KHZ,                ///< HDMI input Audio is 48KHz

    HDMI_AUDIO_NO_CHANGE  = 0xFF,   ///< HDMI Audio format use current setting
    ENUM_DUMMY4WORD(HDMI_AUDIOFMT)
}HDMI_AUDIOFMT;
//@}

/**
    HDMI EDID flags

    Each bit represents TV device support this feature or not.
    These flags are used for hdmitx_getEdidInfo() to determine the TV supported audio/video formats.

    @name   HDMI EDID parsing results flags
*/
//@{
#define HDMITX_DTD1080P60       0x0001  // VID = 16
#define HDMITX_DTD720P60        0x0002  // VID = 4
#define HDMITX_DTD640X480P60    0x0004  // VID = 1
#define HDMITX_DTD720X480P60    0x0008  // VID = 2/3
#define HDMITX_DTD1080I60       0x0010  // VID = 5
#define HDMITX_DTD720X480I60    0x0020  // VID = 6/7
#define HDMITX_DTD1080P50       0x0100  // VID = 31
#define HDMITX_DTD720P50        0x0200  // VID = 19
#define HDMITX_DTD576P50        0x0400  // VID = 17/18/29/30/37/38
#define HDMITX_DTD1080I50       0x1000  // VID = 20/39
#define HDMITX_DTD576I50        0x2000  // VID = 21/22/25/26

#define HDMITX_TV_YCBCR444     0x10000
#define HDMITX_TV_YCBCR422     0x20000
//#NT#2009/12/14#Klins Chen#[0008145] -begin
#define HDMITX_TV_UNDERSCAN    0x40000
//#NT#2009/12/14#Klins Chen#[0008145] -end
#define HDMITX_TV_HDMI         0x80000
#define HDMITX_ADO_32KHZ      0x100000
#define HDMITX_ADO_44KHZ      0x200000
#define HDMITX_ADO_48KHZ      0x400000
#define HDMITX_ADO_MORE       0x800000
//@}






/**
    CEC ISR callback function prototype
*/
typedef void (*CecISRCallbackFunc)(UINT32 uiCallID, UINT32 uiParam);

/**
    CEC ISR callback event define

    CEC ISR callback event define

    @name CEC ISR callback event define
*/
//@{
#define CEC_INTRX_EN_INT                (0x1<<0)    ///< Received one frame.
#define CEC_INTTX_EN_INT                (0x1<<1)    ///< Transmit one frame.
#define CEC_INTRF_EN_INT                (0x1<<2)    ///< Retry count over.
#define CEC_LOST_EN_INT                 (0x1<<3)    ///< Lost Arbitration.
#define CEC_RXOV_EN_INT                 (0x1<<4)    ///< Rx no get EOM bit.
#define CEC_ERR_EN_INT                  (0x1<<5)    ///< Error occured in the signal line.
//@}


extern ER       hdmitx_open(void);
extern ER       hdmitx_close(void);
//#NT#2009/12/23#Klins Chen -begin
extern void     hdmitx_setAV(HDMI_VIDEOID *VID_Code,HDMI_AUDIOFMT ADOFMT,HDMI_MODE ModeFMT);
//#NT#2009/12/23#Klins Chen -end
extern UINT32   hdmitx_getEdidInfo(void);
//#NT#2009/12/29#Klins Chen -begin
extern void     hdmitx_setAudMute(BOOL bAMute);
extern void     hdmitx_setAVMute(BOOL En);
//#NT#2009/12/29#Klins Chen -end
//#NT#2010/01/04#Klins Chen -begin
extern void     hdmitx_forceRGB(BOOL bRGB);
//#NT#2010/01/04#Klins Chen -end
//#NT#2010/01/25#Klins Chen -begin
extern BOOL     hdmitx_isOpened(void);
extern BOOL     hdmitx_checkHotplug(void);
//#NT#2010/01/25#Klins Chen -end
//#NT#2010/03/02#Klins Chen -begin
extern void hdmitx_dumpInfo(void);
//#NT#2010/03/02#Klins Chen -end
extern void     hdmitx_verifyVideoID(HDMI_VIDEOID *VID_Code);



//
//  CEC Related APIs
//
extern ER       hdmicec_open(void);
extern ER       hdmicec_close(void);
extern void     hdmicec_hookCallback(CecISRCallbackFunc IsrCB);

extern void     hdmicec_setLocalAddr(UINT8 uiAddr);
extern void     hdmicec_setFreeTime1(UINT32 uiMs);
extern void     hdmicec_setFreeTime2(UINT32 uiMs);
extern void     hdmicec_setFreeTime3(UINT32 uiMs);
extern void     hdmicec_setRetryCnt(UINT32 uiCnt);
extern void     hdmicec_setTxEdgeOfs(UINT32 uiRise, UINT32 uiFall);
extern void     hdmicec_setRxEdgeOfs(UINT32 uiStart, UINT32 uiData, UINT32 uiSample);
extern void     hdmicec_setRxLowErrPeriod(UINT32 uiErrPeriod);

extern UINT32   hdmicec_getRxFrameSize(void);
extern void     hdmicec_setTxFrameSize(UINT32 uiSize);
extern void     hdmicec_setTxSrcAddr(UINT32 uiSrcAddr);
extern void     hdmicec_setTxDstAddr(UINT32 uiDstAddr);
extern void     hdmicec_setTxData(UINT8 *puiData, UINT32 uiSize);
extern void     hdmicec_getRxData(UINT8 *puiData, UINT32 *puiSize);

//@}

#endif
