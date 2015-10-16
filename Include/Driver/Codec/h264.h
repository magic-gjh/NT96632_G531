//! h264 module driver header
/*! h264 module driver definition

    @file       h264.h
    @ingroup    mICodecH264

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _H264_H_
#define _H264_H_

#include "type.h"
//#include "h264_reg.h"

/*!
    @addtogroup mICodecH264
*/
//@{


#define H264ENC_NUM_SRC         3   /*!< number of encoder register object . */
#define H264ENC_NUM_REC         3   /*!< number of reconstruction buffer in encoder. */ //2011/07/19 Meg for 1920x1080
#define H264ENC_DEBUG           0   /*!< debug for encoder. */

#define H264DEC_NUM_SRC         H264ENC_NUM_SRC   /*!< number of decoder register object. */
#define H264DEC_NUM_REC         5   /*!< number of reconstruction buffer in decoder. */
#define H264DEC_DEBUG_CHECKSUM  0   /*!< debug checksum for decoder. */

#define H264_MODE_ENCODE    0x0 /*!< Encode mode. */
#define H264_MODE_DECODE    0x1 /*!< Decode mode. */

#define H264_PIC_FSH_INT        0x00000001  /*!< Picture Finish Interrupt. */
#define H264_BSDMA_END_INT      0x00000002  /*!< BSDMA Interrupt. */
#define H264_ERROR_INT          0x00010000  /*!< BSDMA Interrupt. */
#define H264_ERROR_BSDMA_INT    0x00200000  /*!< BSDMA Interrupt. */

/* picture type */
#define H264_IPIC   0x2
#define H264_PPIC   0x0
#define H264_BPIC   0x1
#define H264_RBPIC  0x3

#define H264_ALL_P_SLICE    0x5
#define H264_ALL_B_SLICE    0x6
#define H264_ALL_I_SLICE    0x7

#define MEM_NOT_USED    0xFF

//! h264 open mode
/*!
*/
typedef enum _H264Mode
{
    H264_OPEN_ENCODE = 0,   /*!< Encode. */
    H264_OPEN_DECODE = 1    /*!< Decode. */
} H264Mode;

//! h264 module status
/*!
*/
typedef enum _H264Status
{
    H264_STATUS_ERROR_SRC_BUF_IS_NOT_USED = -12,  /*!< Current source buffer is not used. */
    H264_STATUS_ERROR_YADDR_INVALID       = -11,  /*!< Luma address of current picture is invalid. */
    H264_STATUS_ERROR_DISPLAY_FULL        = -10,  /*!< All display-buffer is full. */
    H264_STATUS_ERROR_DISPLAY_EMPTY       = -9,   /*!< There is no extra buffer to be displayed. */
    H264_STATUS_ERROR_REC_BUF_FULL        = -8,   /*!< All recontruction buffer is full. */
    H264_STATUS_ERROR_REGOBJ_EMPTY        = -7,   /*!< There is no register object for h264 to process. */
    H264_STATUS_ERROR_REGOBJ_FULL         = -6,
    H264_STATUS_ERROR_NO_BS_BUF           = -5,   /*!< h264 module is already closed. */
    H264_STATUS_ERROR_NO_SRC_BUF          = -4,   /*!< There is no free source buffer. */
    H264_STATUS_ERROR_CLOSED              = -3,   /*!< h264 module is already closed. */
    H264_STATUS_ERROR_OPENED              = -2,   /*!< h264 module open error. */
    H264_STATUS_ERROR                     = -1,   /*!< Error. */
    H264_STATUS_OK                        = 0     /*!< OK. */
} H264Status;

//! h264 module event
/*!
*/
typedef enum _H264Event
{
    H264_EVENT_HW_FINISH  = 1   /*!< process of hardware is finished. */
} H264Event;

/* ======================================================================= */
/* H264 Driver                                                             */
/* ======================================================================= */
typedef struct _H264_OPEN
{
    UINT8 ucMode;
} H264_OPEN, *PH264_OPEN;

typedef struct _H264ENC_INIT
{
    UINT32 uiH264EncSrcAddr[H264ENC_NUM_SRC][2];
    UINT32 uiH264EncInteralAddr;

    UINT16 usWidth;
    UINT16 usHeight;
    UINT16 usDispWidth;     // @chingho 2010 11 30
    UINT16 usDispHeight;    // @chingho 2010 11 30
    UINT32 uiTotalPicNum;
    UINT32 uiMbsInSlice;
    UINT8  ucIQp;
    UINT8  ucPQp;
    UINT8  ucBQp;
    UINT8  ucConfigFilter;
    UINT8  ucDisableFilter;
    INT8   cAlphaC0offset;
    INT8   cBetaOffset;
    UINT8  ucPFrameNum;
    UINT8  ucBFrameNum;
    UINT8  ucRbFrameNum;
    UINT8  ucLog2MaxFrameNum;
    UINT8  ucLowBandwith;
    UINT8  ucEntropyCoding;
    UINT8  ucCabacIdc;
    UINT8  ucClosedGop;
    UINT32 uiBitrate;
    UINT8  ucRcMode;
    UINT8  ucFramerate;
    UINT8  ucStuffEn;
    UINT8  ucStuffType;
    UINT8  ucSkipEn;
    void   (*pFucCallBack)(UINT32 uiEvent);

    //#NT# 20100113 #aphon wu -begin
    //#NT# new feature:dis
    UINT16  usYDISLineOffset;
    UINT16  usUVDISLineOffset;
    //#NT# 20100113 #aphon wu -end

} H264ENC_INIT, *PH264ENC_INIT;

typedef struct _H264DEC_INIT
{
    UINT32 uiH264DecInteralAddr;
    UINT32 uiH264DecHeaderAddr;

    UINT16 usWidth;
    UINT16 usHeight;

    UINT32 uiBsAddr;
    UINT32 uiBsLength;

    UINT32 uiTotalPicNum;
    void   (*pFucCallBack)(UINT32 uiEvent);

} H264DEC_INIT, *PH264DEC_INIT;

typedef struct _H264ENC_REG_DATA{
    UINT8   ucMode;
    UINT8   ucMaxFrmNumMinus4;
    UINT8   ucMaxPicOrdCntLsbMinus4;
    UINT8   ucEntropyCodingModeFlag;
    UINT8   ucDbFilterCtrlPresent;
    UINT8   ucBwTurboEn;
    UINT8   ucMblRcEn;
    UINT8   ucLambda;
    UINT8   ucOutputColInfoEn;
    UINT8   ucListEn;
    UINT8   ucOutputRcnstDataEn;
    UINT8   ucStuffingDataType;
    UINT8   ucNumMbRowPerSliceMinus1;
    UINT8   ucSliceIdrFlag;
    UINT8   ucSliceType;
    UINT8   ucSliceCabacInitIdc;
    UINT8   ucSliceQpDelta;
    UINT8   ucSliceDisableDbFilterIdc;
    UINT8   ucSliceAlphaC0OffsetDiv2;
    UINT8   ucSliceBetaOffsetDiv2;
    UINT8   ucSliceRefPicListReorderingFlagL0;
    UINT8   ucSliceAbsDiffPicNumMinus1;
    UINT8   ucRcStuffingByteEn;
    UINT8   ucReserved;
    UINT16  usSliceFrmNum;
    UINT16  usSliceIdrPicId;
    UINT32  uiSlicePicOrderCntLsb;
    UINT32  uiRcTargetBits;
    UINT32  uiRcMbBitsLBnd;

} H264ENC_REG_DATA, *PH264ENC_REG_DATA;

typedef struct _H264ENC_HW_ATTR
{
    H264ENC_REG_DATA h264EncCtrl;

    UINT16  usWidth;
    UINT16  usHeight;
    UINT16  usYDISLineOffset;
    UINT16  usUVDISLineOffset;
    UINT16  usYRecLineOffset;
    UINT16  usUVRecLineOffset;

    UINT32  uiINTEnable;

    UINT32  uiBSDMACmdBufAddr;
    UINT32  uiSliceLenAddr;

    UINT32  uiSrcAddr_Y;
    UINT32  uiSrcAddr_UV;

    UINT32  uiRecAddr_Y;
    UINT32  uiRecAddr_UV;
    UINT32  uiRecAddr_Y4;
    UINT32  uiRecAddr_Y16;

    UINT32  uiL0RefAddr_Y;
    UINT32  uiL0RefAddr_UV;
    UINT32  uiL0RefAddr_Y4;
    UINT32  uiL0RefAddr_Y16;

    UINT32  uiL1RefAddr_Y;
    UINT32  uiL1RefAddr_UV;
    UINT32  uiL1RefAddr_Y4;
    UINT32  uiL1RefAddr_Y16;

    UINT32  uiColInfoWtAddr;
    UINT32  uiColInfoRdAddr;

    UINT32  uiURIAddr;
    UINT32  uiRecURAddr;

}H264ENC_HW_ATTR, *PH264ENC_HW_ATTR;

typedef struct _H264DEC_REG_DATA{
    UINT8   ucMode;
    UINT8   ucMaxFrmNumMinus4;
    UINT8   ucMaxPicOrdCntLsbMinus4;
    UINT8   ucPicOrdCntType;
    UINT8   ucDeltaPicOrderAlwaysZeroFlag;
    UINT8   ucFrameMbsOnlyFlag;
    UINT8   ucEntropyCodingModeFlag;
    UINT8   ucDbFilterCtrlPresent;
    UINT8   ucOutputColInfoEn;
    UINT8   ucPicOrderPresentFlag;
    UINT8   ucRedundantPicCntPresentFlag;
    UINT8   ucSliceIdrFlag;
    UINT8   ucSliceType;
    UINT8   ucSliceCabacInitIdc;
    UINT8   ucSliceQpDelta;
    UINT8   ucSliceDisableDbFilterIdc;
    UINT8   ucSliceAlphaC0OffsetDiv2;
    UINT8   ucSliceBetaOffsetDiv2;
    UINT8   ucSliceRefPicListReorderingFlagL0;
    UINT8   ucSliceAbsDiffPicNumMinus1;
    UINT8   ucReserved;
    UINT16  usSliceFrmNum;
    UINT16  usSliceIdrPicId;
    UINT32  uiSlicePicOrderCntLsb;

} H264DEC_REG_DATA, *PH264DEC_REG_DATA;

typedef struct _H264DEC_HW_ATTR
{
    H264DEC_REG_DATA  h264DecCtrl;

    UINT16  usWidth;
    UINT16  usHeight;
    UINT16  usYRecLineOffset;
    UINT16  usUVRecLineOffset;

    UINT32  uiINTEnable;

    UINT32  uiDecMaxLen;

    UINT32  uiBSDMACmdBufAddr;

    UINT32  uiRecAddr_Y;
    UINT32  uiRecAddr_UV;

    UINT32  uiL0RefAddr_Y;
    UINT32  uiL0RefAddr_UV;

    UINT32  uiL1RefAddr_Y;
    UINT32  uiL1RefAddr_UV;

    UINT32  uiColInfoWtAddr;
    UINT32  uiColInfoRdAddr;

    UINT32  uiURIAddr;
    UINT32  uiRecURAddr;

} H264DEC_HW_ATTR, *PH264DEC_HW_ATTR;

/* ////////////////////////////////////////////////////////// */
/*  Enable/disable and do power management to drive hardware  */
/* ---------------------------------------------------------- */
/*  hardware library level that is OS independent             */
/* ////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////// */
/* Open/close driver to operate                               */
/* ---------------------------------------------------------- */
/* OS driver level that may need to be modified               */
/* when porting for different OS                              */
/* ////////////////////////////////////////////////////////// */
INT32 h264_open(PH264_OPEN pH264Open);
INT32 h264_close(void);
BOOL  h264_isOpen(void);

/* ////////////////////////////////////////////////////////// */
/* Protect driver resource while operating                    */
/* ---------------------------------------------------------- */
/* OS driver level that may need to be modified               */
/* when porting for different OS                              */
/* ////////////////////////////////////////////////////////// */

/* H264 Register Object API */
void   h264_resetReg(void);
void   h264_setupRegObj(UINT32 uiAddr);
INT32  h264_getUnusedRegObj(void);
void   h264_releaseUsedRegObj(void);
INT32  h264_getToBeUsedRegObjIdx(void);
INT32  h264_getCurrRegObjSrcIdx(void);
INT32  h264_getCurrRegObjNotRefIdx(void);
INT32  h264_getCurrRegObjOutput(UINT8 *pucOutput1, UINT8 *pucOutput2);
INT32  h264_getRegSize(void);
UINT32 h264_waitINT(void);

extern UINT32 guih264IntStatus;
extern UINT32 h264_checkINT(void);//2010/12/15 Meg Lin



//@}

//---------------------------------------------------------------------------------
#endif  // _H264_H_
