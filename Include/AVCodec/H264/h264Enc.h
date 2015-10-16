#ifndef _H264_CODEC_H_
#define _H264_CODEC_H_

#include "h264.h"

INT32  h264Enc_setupMemory(PH264ENC_INIT pH264EncInit);
void   h264Enc_resetEncoder(void);
INT32  h264Enc_getMemSizeInternal(UINT16 usWidth, UINT16 usHeight);
UINT32 h264Enc_getHeaderAddr(void);
INT32  h264Enc_initEncoder(PH264ENC_INIT pH264EncInit);
INT32  h264Enc_prepareOnePicture(UINT8 ucTurboMode);
INT32  h264Enc_encodeOnePicture(UINT32 uiBsAddr, UINT32 uiBsBufLength);
UINT32 h264Enc_setNextPic(void);

//#NT# 20100113 aphon wu  -begin
//#NT# add src dis offset
//INT32  h264Enc_setSrcBuf(UINT32 uiYAddr);
INT32  h264Enc_setSrcBuf(UINT32 uiYAddr,UINT32 uiUVAddr,UINT32 uiYAddr_dis,UINT32 uiUVAddr_dis);
//#NT# 20100113 aphon wu  -end

UINT8  h264Enc_getRcEnable(void);
INT32  h264Enc_getRcSumQSteps(void);
INT32  h264Enc_getPicCost(void);
UINT8  h264Enc_getSliceType(void);
void   h264Enc_updatePicture(INT32 iPicBits);
void   h264Enc_addPictureNo(void);

//---------------------------------------------------------------------------------
#endif  // _H264_CODEC_H_
