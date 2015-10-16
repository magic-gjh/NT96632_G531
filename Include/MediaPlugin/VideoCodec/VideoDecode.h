/**
    Header file of video decoding codec library

    Exported header file of video decoding codec library.

    @file       VideoDecode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEODECODE_H
#define _VIDEODECODE_H


#include "Audio.h"
#include "FileDB.h"
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
/**
    @addtogroup mIAVCODEC
*/
//@{


#define VIDDEC_YCBCRMAXBLOCK    11
#define VIDDEC_YCBCRMINBLOCK    3 //2010/05/20 Meg Lin
/**
    type of getting information from video decoding codec library.
    format: ER (*GetInfo)(VIDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3); 
*/
typedef enum
{
    MEDIAVID_GETINFO_NOWYCBCR   =0x1,   ///< p1:y addr(out), p2:cb addr(out), p3: cr addr(out)  
    MEDIAVID_GETINFO_PREVYCBCR  =0x2,   ///< p1:prev y addr(out), p2: prev cb addr(out), p3: prev cr addr(out) 
    ENUM_DUMMY4WORD(VIDCODEC_GETINFO_TYPE)
} VIDCODEC_GETINFO_TYPE;

/**
    type of setting information from video decoding codec library.
    format: ER (*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum
{
    MEDIAVID_SETINFO_264Addr  =0x81,    ///< p1:address for H264 description (in)
    MEDIAVID_SETINFO_264YAddr =0x82,    ///< p1:y addr(in), p2:UV addr(in)
    MEDIAVID_SETINFO_CLOSEBUS =0x83,    ///< no parameters
    MEDIAVID_SETINFO_264BBPLAY=0x84,    ///< p1:Backward or not(in)
    MEDIAVID_SETINFO_RESETGOP =0x85,    ///< no parameters
    ENUM_DUMMY4WORD(VIDCODEC_SETINFO_TYPE)
} VIDCODEC_SETINFO_TYPE;

/**
    @name type for CustomizeFunc() in VIDEO_DECODER

    type for CustomizeFunc() in VIDEO_DECODER.
*/
//@{
#define MEDIAVID_CUSTOM_DECODE1ST   0x01    ///< p1:(MEDIA_DSP_1STV *) object(out)
#define MEDIAVID_CUSTOM_DECODE_SPECPIC   0x02    ///< p1:(MEDIA_DEC_SPECIFICPIC_INFO *) object(out)
//@}


/**
    struture of video frame information. Used for CustomizeFunc().
*/
typedef struct 
{
    UINT32 addr;        ///< in:video frame addr
    UINT32 size;        ///< in:video frame size
    UINT32 decodeBuf;   ///< in:output raw addr
    UINT32 decodeBufSize;///< in:max size for outputing raw
    UINT32 y_Addr;      ///< out:output Y addr
    UINT32 cb_Addr;     ///< out:output cb addr
    UINT32 cr_Addr;     ///< out:output cr addr
    UINT32 width;       ///< out:video frame width
    UINT32 height;      ///< out:video frame height
    UINT32 h264DescAddr;///< in:h264 description addr 
    UINT32 h264DescLen; ///< in:h264 description length
    UINT32 jpgType;     ///< out:jpg type, JPGFormatYUV420 or others
} MEDIA_DSP_1STV;

typedef struct
{
    UINT32 yAddrDsp; ///< in: y addr as displayed
} MEDIA_DSP_Y; //2011/05/11 Meg

/**
    struture of video data.
*/
typedef struct
{
    UINT32 yAddr;   ///< y address
    UINT32 cbAddr;  ///< cb address
    UINT32 crAddr;  ///< cr address
} VIDEO_YCBCRADDR;

/**
    struture of video display setting. Used for Initialize()
*/
typedef struct
{
    UINT32 firstFramePos;   ///<in: first video frame position
    UINT32 firstFrameSize;  ///<in: first video frame size
    UINT32 rawAddr;         ///<in: after decoded, Y addr (cb, cr counted)
    UINT32 rawSize;         ///<in: size for YCbCr
    UINT32 dspWidth;        ///<in: display width
    UINT32 dspHeight;       ///<in: display height
    UINT32 dspType;         ///<in: display type
    UINT32 rawWidth;        ///<in: raw width
    UINT32 rawHeight;       ///<in: raw height
    UINT32 rawType;         ///<in: raw type, (jdcfg.fileformat)
    UINT32 rawCbAddr;       ///<out: after decoded, cb addr
    UINT32 rawCrAddr;       ///<out: after decoded, cr addr 
    UINT32 h264DescAddr;    ///<in: h264 description addr
    UINT32 h264DescLen;     ///<in: h264 description length
    UINT32 jpgType;         ///<in: jpg type   

} VIDEO_DISPLAYINFO;


typedef struct
{
    UINT32 wantNum;     //[in] wantNum, want starts from 1
    UINT32 bsAddr;      //[in] bitstream addr
    UINT32 bsSize;      //[in] bitstream totalsize
    UINT32 outYaddr;    //[in/out] y raw data output addr
    UINT32 ylineoft;    //[in] y lineoffset
    UINT32 height;      //[in] height
    UINT32 h264DescAddr;//[in] h264 desc addr
    UINT32 h264DescSize;//[in] h264 desc size
    UINT32 endRawAddr;  //[out] used raw end addr
    UINT32 outUVaddr;   //[out] uv raw data output addr
} MEDIA_DEC_SPECIFICPIC_INFO;


/**
    Struture of video decoder
*/
typedef struct
{
    ER (*Initialize)(UINT32 fileAddr, VIDEO_DISPLAYINFO *pobj);///< initialize decoder
    ER (*GetInfo)(VIDCODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);///< get information from decoder
    ER (*SetInfo)(VIDCODEC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);///< set information to decoder
    ER (*DecodeOne)(UINT32 type, UINT32 fileAddr, UINT32 p2, UINT32 *p3);///< decode one frame, p1: frame addr(in), p2: frame size(in)
    ER (*WaitDecodeDone)(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);///< wait decoding finished, p1: E_OK or not(out)
    ER (*CustomizeFunc)(UINT32 type, void *pobj);///< customize function, if needed
    UINT32 checkID;///< check ID for decoders
} VIDEO_DECODER, *PVIDEO_DECODER;

/*
    Public funtions to get video decoding object
*/
//@{
extern PVIDEO_DECODER MP_H264Dec_getVideoDecodeObject(void);
extern PVIDEO_DECODER MP_MjpgDec_getVideoDecodeObject(void);
//@}


//@}

#endif //_VIDEODECODE_H

