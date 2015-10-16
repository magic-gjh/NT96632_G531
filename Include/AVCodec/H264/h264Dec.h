#ifndef _H264_CODEC_H_
#define _H264_CODEC_H_

#include "h264.h"

void   h264Dec_resetDecoder(void);
INT32  h264Dec_getMemSizeInternal(UINT16 usWidth, UINT16 usHeight);
INT32  h264Dec_initDecoder(PH264DEC_INIT pH264DecInit);
INT32  h264Dec_prepareOnePicture(UINT32 uiBsAddr, UINT32 uiBsBufLength);
INT32  h264Dec_decodeOnePicture(void);
UINT32 h264Dec_setNextPic(void);
INT32  h264Dec_resetGop(void);
INT32  h264Dec_prepare2ReverseIPicture(UINT32 uiBsAddr, UINT32 uiBsBufLength);
INT32  h264Dec_getRec2Display(UINT32 *puiYAddr, UINT32 *puiUVAddr);

UINT8  h264Dec_getSliceType(void);
UINT32 h264Dec_getPictureNo(void);
void   h264Dec_addPictureNo(void);
INT32  h264Dec_setRecDisplayed(UINT32 uiYAddr);

//---------------------------------------------------------------------------------
#endif  // _H264_CODEC_H_
