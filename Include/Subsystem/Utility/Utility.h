/**
    @file       Utility.h
    @ingroup    mISYSUtil

    @brief      Utility function collection.

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.
*/

#ifndef _Utility_H
#define _Utility_H

#include "Type.h"

/*
    @addtogroup mISYSUtil
@{
*/

//
// ScalarUpDown.c
//

#define ScaleDownLevel0         0
#define ScaleDownLevel1         1
#define ScaleDownLevel2         2
#define ScaleDownLevel3         3

typedef struct {
  unsigned char   *src;
  unsigned char   *dst;
  unsigned short InputWidth;
  unsigned short InputHeight;
  unsigned short InputOffset;
  unsigned short OutputWidth;
  unsigned short OutputHeight;
  unsigned short OutputOffset;
  unsigned short ScaleLevel;
  unsigned short reserved;
} JpgScaleUpDown_t;

extern void     JpgScaleUpDown(JpgScaleUpDown_t *);

typedef struct input_file_tag_mix {
    int format;      //0: 444, 1: 422, 2: 420
    int width;
    int height;
    int address;
    unsigned char *name;
} Input_File_format_mix;

typedef struct output_file_tag_mix {
    int format;      //0: 444, 1: 422, 2: 420
    int width;
    int height;
    int address;
    unsigned char *name;
    //unsigned char name[12];
} Output_File_format_mix;


typedef struct input_tag {
    int format;      //0: 444, 1: 422, 2: 420
    int width;
    int height;
    unsigned char *y_address;
    unsigned char *cb_address;
    unsigned char *cr_address;
} Input_format;

typedef struct output_tag {
    int format;      //0: 444, 1: 422, 2: 420
    int width;
    int height;
    unsigned char *y_address;
    unsigned char *cb_address;
    unsigned char *cr_address;
} Output_format;

//#NT#2007/02/14#Scottie -begin
//#IME library have provided like IME Scale function, we don't need the following function anymore.
//extern void     IME_Scale_Set(Input_format Input,Output_format Output);
//#NT#2007/02/14#Scottie -end

//
// Rotate.c
//

typedef struct {
     UINT32     uiImgSrcAddr_Y;
     UINT32     uiImgSrcAddr_Cb;
     UINT32     uiImgSrcAddr_Cr;
     UINT32     uiImgDstAddr_Y;
     UINT32     uiImgDstAddr_Cb;
     UINT32     uiImgDstAddr_Cr;
     UINT16     usImgWidth;
     UINT16     usImgHeight;
     UINT16     usDstWidth;
     UINT16     usDstHeight;

     UINT8      ucSrcFormat;
     //UINT8      ucDstFormat;
     UINT8      ucRotateDir;    // 0->90 ; 1->180 ; 2->Horizontal ; 3->Vertical
} JPGROTATE, *PJPGROTATE;

//JPGROTATE.ucRotateDir
#define ROTATE_DIR_90           0
#define ROTATE_DIR_180          1
#define ROTATE_DIR_270          2
#define ROTATE_DIR_HOR          3
#define ROTATE_DIR_VER          4

//JPGROTATE.ucSrcFormat
#define ROTATE_FORMAT_YUV420    0
#define ROTATE_FORMAT_YUV422    1
#define ROTATE_FORMAT_YUV444    2

extern void     JpgSetRotate(PJPGROTATE pRotateInfo);

//
// Delay.c
//

extern ER       TimerDelayMs(UINT32 ms);
extern ER       TimerDelayUs(UINT32 us);
//#NT#2007/07/12#Chris Hsu -begin
//#Add PWM hardware delay
extern ER       PWMDelayMs(UINT32 uiPwmID, UINT32 uiMs);
extern ER       PWMDelayUs(UINT32 uiPwmID, UINT32 uiMs);
//#NT#2007/07/12#Chris Hsu -end

//
// Perf.c
//

extern ER       Perf_Open(void);
extern ER       Perf_Close(void);
extern UINT32   Perf_Mark(void);
extern UINT32   Perf_GetDuration(void);
extern UINT32   Perf_GetCurrent(void);

//
// Mem.c
//

extern ER       hwmem_open(void);
extern ER       hwmem_close(void);
extern void     hwmem_memcpy(UINT32 uiDst, UINT32 uiSrc, UINT32 uiSize);
extern void     hwmem_memset(UINT32 uiDst, UINT8 ui8Value, UINT32 uiSize);
extern void     hwmem_memset16(UINT32 uiDst, UINT16 ui16Value, UINT32 uiSize);
extern void     hwmem_memset32(UINT32 uiDst, UINT32 ui32Value, UINT32 uiSize);

//
// NvtMbx.c
//
extern void nvt_mbx_init(void);
extern ER nvt_snd_msg(ID mbxid, UINT eventid, UCHAR *pbuf, UINT size);
extern void nvt_rcv_msg(ID mbxid, UINT *eventid, UCHAR **pbuf, UINT *size);
extern BOOL nvt_chk_msg(ID mbxid);


//#NT#2009/08/03#Niven Cho -begin
//#NT#Added., Use Utility Image Scale
//
// UtImageScale.c (Due to 630/450 has different engine and different structure with ime
//               , this function implementation put at Utility\450\ImageScale.c
//                 and Utility\630\ImageScale.c
//

typedef enum _UT_IMAGE_FMT_
{
    UT_IMAGE_FMT_YUV422 = 0, //!< 422 is Better Default for DSC
    UT_IMAGE_FMT_YUV420 = 1,
    UT_IMAGE_FMT_YUV444 = 2
}UT_IMAGE_FMT;

typedef struct _UT_SCALE_SET{
    //Input Settings
    UINT32 InAddrY;
    UINT32 InAddrCb;
    UINT32 InAddrCr;
    UINT32 InWidth;
    UINT32 InHeight;
    UINT32 InLineOffsetY;
    UINT32 InLineOffsetUV;
    UT_IMAGE_FMT InFormat;
    //OutPut Settings
    UINT32 OutAddrY;
    UINT32 OutAddrCb;
    UINT32 OutAddrCr;
    UINT32 OutWidth;
    UINT32 OutHeight;
    UINT32 OutLineOffsetY;
    UINT32 OutLineOffsetUV;
    UT_IMAGE_FMT OutFormat;
}tUT_SCALE_SET;

extern BOOL UtImageScaleByIme(tUT_SCALE_SET* pScale);
extern BOOL UtImageScaleByWarp(tUT_SCALE_SET* pScale);
extern BOOL UtImageScale(tUT_SCALE_SET* pScale); //Default 630:IME, 450:WARP

//#NT#2009/08/03#Niven Cho -end
//#NT#2009/09/24#SA4 -begin
//
// lz.c
//
extern unsigned int LZ_Uncompress( unsigned char *in, unsigned char *out, unsigned int insize );

//#NT#2009/05/06#York Dai -begin
//#NT#add for photo frame function
//#Add FW scale up in special level (2^n)
#define SCALE_UP_SPE_LEVEL_2        1
#define SCALE_UP_SPE_LEVEL_4        2
#define SCALE_UP_SPE_LEVEL_8        3

typedef struct
{
    UINT32      uiLevel;        // Scale up level, SCALE_UP_SPE_LEVEL_X, X = 2, 4, 8
    UINT32      uiSrcWidth;     // Source width
    UINT32      uiSrcHeight;    // Source height
    UINT32      uiFormat;       // Source format, JPEG_FORMAT_420/JPEG_FORMAT_422/JPEG_FORMAT_444
    UINT32      uiSrcY;         // Source Y address
    UINT32      uiSrcCb;        // Source Cb address
    UINT32      uiSrcCr;        // Source Cr address
    UINT32      uiDstY;         // Destination Y address
    UINT32      uiDstCb;        // Destination Cb address
    UINT32      uiDstCr;        // Destination Cr address
} SCALE_UP_SPE, *PSCALE_UP_SPE;

extern void scale_scaleYCbCrUpSpecial(PSCALE_UP_SPE pScale);

//#Add border frame
//
// BorderFrame.c
//

typedef struct
{
    UINT32      uiSrcWidth;     // Source width
    UINT32      uiSrcHeight;    // Source height
    UINT32      uiFormat;       // Source format, JPEG_FORMAT_420/JPEG_FORMAT_422/JPEG_FORMAT_444
    UINT32      uiSrcY;         // Source Y address
    UINT32      uiSrcCb;        // Source Cb address
    UINT32      uiSrcCr;        // Source Cr address
    UINT8       uiKeyY;         // Color key Y, use < operation now
    UINT8       uiKeyCb;        // Color key Cb, use = operation now
    UINT8       uiKeyCr;        // Color key Cr, use = operation now
} BORDER_INFO, *PBORDER_INFO;

extern void     border_fixJPEGCompression(PBORDER_INFO pInfo);
//#NT#2009/05/06#York Dai -end
//#NT#2009/09/24#SA4 -end
/**
@}
*/

#endif //  _Utility_H
