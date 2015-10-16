#ifndef _MEDIAVIDEODECODE_H
#define _MEDIAVIDEODECODE_H


#include "Audio.h"
#include "FileDB.h"
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "KERNEL.h"
#include "OSCommon.h"


#define MEDIAVID_DECODENEXT     1
#define MEDIAVID_DECODEPREV     2
/*

#define MEDIAVID_GETINFO_NOWYCBCR 1
#define MEDIAVID_GETINFO_PREVYCBCR 2

#define MEDIAVID_SETINFO_264Addr  0x81
#define MEDIAVID_SETINFO_264YAddr 0x82//p1 = y addr, p2 = UV addr
typedef struct _VIDEO_YCBCRADDR
{
    UINT32 yAddr;
    UINT32 cbAddr;
    UINT32 crAddr;
} VIDEO_YCBCRADDR;

typedef struct _VIDEO_DISPLAYINFO
{
    UINT32 firstFramePos;
    UINT32 firstFrameSize;
    UINT32 rawAddr;//in: after decoded, Y addr (cb, cr counted)
    UINT32 rawSize;//in: size for YCbCr
    UINT32 dspWidth;//in:display width
    UINT32 dspHeight;//in:display height
    UINT32 dspType;//in:display type
    UINT32 rawWidth;//out: raw width
    UINT32 rawHeight;//out:raw height
    UINT32 rawType;//out:  raw type
    UINT32 rawCbAddr;//out: after decoded, cb addr
    UINT32 rawCrAddr;//out: after decoded, cr addr 
    UINT32 h264DescAddr;
    UINT32 h264DescLen;

} VIDEO_DISPLAYINFO;

typedef struct _VIDEO_DECODER
{
    ER (*Initialize)(UINT32 fileAddr, VIDEO_DISPLAYINFO *pobj);
    ER (*GetInfo)(UINT32 type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
    ER (*SetInfo)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);
    ER (*DecodeOne)(UINT32 type, UINT32 fileAddr, UINT32 p2, UINT32 *p3);
    ER (*WaitDecodeDone)(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);
    ER (*CustomizeFunc)(UINT32 type, void *pobj);
    UINT32 checkID;
} VIDEO_DECODER;
*/
#endif //_MEDIAVIDEODECODE_H
