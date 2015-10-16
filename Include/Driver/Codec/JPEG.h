/**
    Header file for JPEG module.
    This file is the header file that define the API for JPEG module.

    @file       JPEG.h
    @ingroup    mICodecJPEG
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _JPEG_H
#define _JPEG_H

#include "Type.h"
#include "OSCommon.h"

/**
    @addtogroup mICodecJPEG
*/
//@{

///////////////////////////////////////////////////////////////////////
//JPG File format.
///////////////////////////////////////////////////////////////////////
#define JPEG_FORMAT_420         0x00100000
#define JPEG_FORMAT_444         0x00400000
#define JPEG_FORMAT_422         0x00800000
//#NT#2009/11/23#Meg Lin -begin
//#NT#Add for UV pack format encode
#define JPEG_FORMAT_420_UV      0x01000000
//#NT#2009/11/23#Meg Lin -end

//#NT#Add for UserJPEG level compatible
#define JPEG_PARSE_YUV211       0x00
#define JPEG_PARSE_YUV420       0x01
#define JPEG_PARSE_YUV444       0x02
#define JPEG_PARSE_YUV422       0x03
#define JPEG_PARSE_YUV211V      0x04

///////////////////////////////////////////////////////////////////////////////////////
#define JPG_Enc_Thumbnail       0x00000010
//#NT#In order to add a hidden thumbnail image inside MakerNote, we add one option JPG_Enc_ThumbnailB
#define JPG_Enc_ThumbnailB      0x00000020  //JPG_Enc_ThumbnailB including JPG_Enc_Thumbnail

#define JPG_Enc_AsyncMode       0x00001000
#define JPG_Enc_Enable_Slice    0x00002000
#define JPG_Enc_SrcSliceMode    0x0000A000

//#NT#Move it here from JpgParseHeader.h
#define   JPGFormatYUV211       0  //0, JPEG format macroblock color format is YCbCr422
#define   JPGFormatYUV420       1  //1, JPEG format macroblock color format is YCbCr420
#define   JPGFormatYUV444       2  //2, JPEG format macroblock color format is YCbCr444
#define   JPGFormatYUV422       3  //3, JPEG format macroblock color format is YCbCr422
#define   JPGFormatYUV211V      4  //3, JPEG format macroblock color format is YCbCr422 and vertical

#define   JPG_HW_FMT_420        JPEG_FORMAT_420
#define   JPG_HW_FMT_444        JPEG_FORMAT_444
#define   JPG_HW_FMT_211        JPEG_FORMAT_422
#define   JPG_HW_FMT_422        0x00200000
#define   JPG_HW_FMT_211V       0x01000000
//#NT#Add for NT96433 to support bitstream & scale output enable
#define   JPG_Enc_BSOUTEN       0x01000000
#define   JPG_Enc_BSOUTDIS      0x02000000
#define   JPG_Dec_SCALONLY      0x04000000
#define   JPG_Dec_SCALBOTH      0x08000000

////////////////////////////////////////////////////////////////////////////////////////
//Interrupt Mode
#define _JPG_Int_FrameEnd       0x00000001
#define _JPG_Int_SliceDone      0x00000002
#define _JPG_Int_DecodeErr      0x00000004
#define _JPG_Int_BufEnd         0x00000008

////////////////////////////////////////////////////////////////////////////////////////
// interrupt status for jpeg module
#define _JPEG_Frame_End         0x00000001
#define _JPEG_Slice_Done        0x00000002
#define _JPEG_Decode_Err        0x00000004
#define _JPEG_Buf_End           0x00000008

// -------------------------------------------------
// The general api for the JPEG device driver
// -------------------------------------------------
extern ER   jpeg_open(void);
extern ER   jpeg_close(void);
extern BOOL jpeg_isOpened(void);
extern ER   jpeg_lock(void);
extern ER   jpeg_unlock(void);
extern ID   jpeg_getLockStatus(void);

extern UINT jpeg_getStatus(void);
extern UINT jpeg_getComSize(void);

extern void jpeg_setStartEncode(void);
extern void jpeg_setStartDecode(void);

extern void jpeg_setEndEncode(void);
extern void jpeg_setEndDecode(void);

extern void jpeg_setEnableInt(UINT);
extern UINT jpeg_getAllInt(void);
extern void jpeg_clrAllInt(void);

extern void jpeg_setImgStartAddr(UINT ImgYAddr, UINT ImgCBAddr, UINT ImgCRAddr);
extern UINT jpeg_getImgStartAddr(UINT *ImgYAddr, UINT *ImgCBAddr, UINT *ImgCRAddr);

extern void jpeg_setImgLineOffset(UINT ImgYOfs, UINT ImgCBCROfs);
extern UINT jpeg_getImgLineOffsetY(void);
extern UINT jpeg_getImgLineOffsetC(void);

extern void jpeg_setCmpStartAddr(UINT BSAddr, UINT BufSize);
extern UINT jpeg_getCmpStartAddr(void);

extern void jpeg_setCompressMode(UINT);
extern void jpeg_setFileFormatHW(UINT, UINT, UINT);
extern void jpeg_setEncodeQTabHW(UCHAR *, UCHAR *);
extern void jpeg_setDecodeQTabHW(UCHAR *, UCHAR *);

extern void jpeg_setEncodeHuffTabHW(USHORT *puiHuffTabLAC, USHORT *puiHuffTabLDC, USHORT *puiHuffTabCAC, USHORT *puiHuffTabCDC);

extern void jpeg_setBaseIdxTabHW(UCHAR *puiHuffTabLAC, UCHAR *puiHuffTabLDC, UCHAR *puiHuffTabCAC, UCHAR *puiHuffTabCDC);
extern void jpeg_setMinCodeTabHW(UCHAR *puiHuffTabLAC, UCHAR *puiHuffTabLDC, UCHAR *puiHuffTabCAC, UCHAR *puiHuffTabCDC);
extern void jpeg_setSymbolTabHW(UCHAR *puiHuffTabLAC, UCHAR *puiHuffTabLDC, UCHAR *puiHuffTabCAC, UCHAR *puiHuffTabCDC);

extern void jpeg_setRestartInterval(UINT);
extern void jpeg_setRestartEnable(UINT);

extern UINT jpeg_setSliceCnt(UINT LineNum);
extern UINT jpeg_setSliceEncAddr(UINT LineNum, UINT Y_Addr, UINT Cb_Addr, UINT Cr_Addr);          //This function is for slice mode used.

extern void jpeg_setScaleAddr(UINT Y_Addr, UINT Cb_Addr, UINT Cr_Addr);
extern void jpeg_setScaleRatio(UINT ScaleRatio);
extern void jpeg_getScaleYUVLineOffset(UINT *pY_LineOft, UINT *pUV_LineOft);
extern void jpeg_setBSOutput(BOOL enable);
extern void jpeg_setScaleYUVLineOffset(USHORT uhFormat, UINT *Y_LineOft, UINT *UV_LineOft);
extern void jpeg_setScaleEnable(void);
extern void jpeg_setScaleDisable(void);
extern void jpeg_setTPOSEnable(UINT IfTPOSModeCW);
//#NT#Add for checking if there is only one JPEG output path simultaneously.
extern BOOL jpeg_IsOneOutputPath(void);
//#NT#Add for checking the image format if supported.
extern BOOL jpeg_IsSupportedFormat(USHORT usNumComp, USHORT usScanFreq[3][2], USHORT *pusFileFormat);
//#NT#Add for checking if there is limitation on JPEG scale function.
extern BOOL jpeg_IsScaleLimit(USHORT usWidth);
//#NT#Add for removing the necessity of coding wai_flg() & chk_flg() outside JPEG driver.
extern UINT jpeg_waitDone(void);
extern BOOL jpeg_waitDonePolling(void);
//#NT#Add for support slice go mode.
extern void jpeg_setSliceGoEnable(void);
extern void jpeg_setSliceGoDisable(void);
//#NT#Add for support crop mode.
extern void jpeg_setCrop(UINT32 StartX, UINT32 StartY, UINT32 Width, UINT32 Height);
extern void jpeg_setCropDisable(void);
extern void jpeg_setSrstReg(void);//2010/11/19 Meg Lin move here from jpeg_reg.h

// -------------------------------------------------

//@}

#endif // _JPEG_H
