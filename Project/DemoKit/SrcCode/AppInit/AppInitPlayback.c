/*
    @file       AppInitPlayback.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for Playback mode.

                Get memory and set parameters for Playback mode.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

/**
    @addtogroup mIPRJAPCfg
*/
//@{

#include <string.h>
#include "SysCfg.h"
#if (_WAVSTUDIO_MODE_ == ENABLE)
#include "WavStudioTsk.h"
#endif
#include "UIFlow.h"
#include "GlobalVar.h"
#include "AppInit.h"
#include "BG_Images.h"
#include "ime.h"
#include "ime_lib.h"
#include "pll.h"
#include "KeyScanTsk.h"
#include "MediaPlayAPI.h"
#include "videoDecode.h"
#include "MediaReadLib.h"

UINT32  g_uiMovieBGAddrY;       //< image Y buffer address for movie background
UINT32  g_uiMovieBGAddrCb;      //< image Cb buffer address for movie background
UINT32  g_uiMovieBGAddrCr;      //< image Cr buffer address for movie background
UINT32  g_uiBadFileBGAddrY;     //< image Y buffer address for Bad File background
UINT32  g_uiBadFileBGAddrCb;    //< image Cb buffer address for Bad File background
UINT32  g_uiBadFileBGAddrCr;    //< image Cr buffer address for Bad File background
UINT32  g_uiWavFileBGAddrY;     //< image Y buffer address for Wav File background
UINT32  g_uiWavFileBGAddrCb;    //< image Cb buffer address for Wav File background
UINT32  g_uiWavFileBGAddrCr;    //< image Cr buffer address for Wav File background

#if (_WAV_SINGLE_USERDATA_ == ENABLE)
UINT8   g_uiWAVHeaderUserData[180] =
    {
    'L', 'I', 'S', 'T',
    0x30, 0x00, 0x00, 0x00,
    'I', 'N', 'F', 'O',
    'I', 'N', 'A', 'M',
    0x10, 0x00, 0x00, 0x00,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
    'I', 'C', 'R', 'D',
    0x0C, 0x00, 0x00, 0x00,
    '2', '0', '0', '8', '-', '0', '4', '-', '2', '5', 0x00, 0x00,
    'L', 'I', 'S', 'T',
    0x74, 0x00, 0x00, 0x00,
    'e', 'x', 'i', 'f',
    'e', 'v', 'e', 'r',
    0x04, 0x00, 0x00, 0x00,
    '0', '2', '2', '1',
    'e', 'r', 'e', 'l',
    0x0E, 0x00, 0x00, 0x00,
    'I', 'M', 'A', 'G', '0', '0', '0', '1', '.', 'J', 'P', 'G', 0x00, 0x00,
    'e', 't', 'i', 'm',
    0x0E, 0x00, 0x00, 0x00,
    '1', '2', ':', '3', '4', ':', '5', '6', '.', '7', '8', '9', 0x00, 0x00,
    'e', 'c', 'o', 'r',
    0x18, 0x00, 0x00, 0x00,
    'N', 'o', 'v', 'a', 't', 'e', 'k', ' ', 'M', 'i',
    'c', 'o', 'e', 'l', 'e', 'c', ' ', 'C', 'O', 'R',
    'P', ' ', 0x00, 0x00,
    'e', 'm', 'd', 'l',
    0x10, 0x00, 0x00, 0x00,
    'N', 'T', '9', '6', '4', '3', '2', '-', '2', '0',
    '0', '8', '-', '0', '4', 0x00
};
#endif
#if (_WAVSTUDIO_MODE_ == ENABLE)
extern void FlowCommon_WAVStudioCB(UINT32 uiEventID);
#endif
//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#if 0
// Slide effect speed table
static UINT8 SlideSpeedTable[SLIDE_EFFECT_IMAGE_STEP] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
#endif
//#NT#2009/06/03#Corey Ke -end

void AppinitMedia_Decode1stVideo(MEDIA_PLAY_1ST_INFO *pMediaInfo)
{
    MEDIA_GET1STV   get1stV;
    MEDIA_FIRST_INFO media1stV;
    MEDIA_DSP_1STV  dsp1stV;
    UINT32          h264Addr, h264Len;
    VIDEO_DECODER   *pDecobj;
    CONTAINERPARSER *ptrContainer;
    ER customER;//2011/11/18 Meg Lin

    get1stV.addr = pMediaInfo->fileStartAddr;
    get1stV.size = pMediaInfo->freeSize;
//#NT#2010/05/05#Meg Lin -begin
//new feature: using MediaPlayer to play AVI files
#if (_MOVIE_FMT_ == _MOVIE_FMT_MOVH264_)
    get1stV.validType = MEDIA_FILEFORMAT_MOV;
#else //_MOVIE_FMT_AVIH264_
    get1stV.validType = MEDIA_FILEFORMAT_AVI;
#endif
    get1stV.validType = MEDIA_FILEFORMAT_AVI;//2010/12/15 Meg Lin
    MediaPlay_Show1stVideo(&get1stV, 0, &media1stV);
    if (media1stV.type == 0)
    {
        debug_err(("not valid filetype\r\n"));
        pMediaInfo->imageWidth = 0;
        pMediaInfo->imageHeight= 0;
        return;
    }
    //#NT#2010/08/09#Meg Lin -begin
    else if (media1stV.dur == 0)
    {
        debug_err(("moov atom error\r\n"));
        pMediaInfo->imageWidth = 0;
        pMediaInfo->imageHeight= 0;
        return;
    }
    //#NT#2010/08/09#Meg Lin -end
    dsp1stV.addr = (get1stV.addr+ media1stV.pos);
    dsp1stV.size = media1stV.size;
    dsp1stV.decodeBuf = pMediaInfo->yAddr;
    dsp1stV.decodeBufSize = 0x1000000;

    ptrContainer = gFormatParser[media1stV.type].parser;
    (ptrContainer->GetInfo)(MEDIAREADLIB_GETINFO_H264DESC, &h264Addr, &h264Len, 0);
    dsp1stV.width = media1stV.wid;
    dsp1stV.height= media1stV.hei;
    dsp1stV.h264DescAddr= h264Addr;
    dsp1stV.h264DescLen= h264Len;
    pMediaInfo->videoCodec = MEDIAPLAY_VIDEO_MJPG;
    pDecobj = MP_MjpgDec_getVideoDecodeObject();
    if (dsp1stV.h264DescLen)
    {
        pMediaInfo->videoCodec = MEDIAPLAY_VIDEO_H264;
        pDecobj = MP_H264Dec_getVideoDecodeObject();
        if (media1stV.type == MEDIA_PARSEINDEX_AVI)
        {
            dsp1stV.addr += dsp1stV.h264DescLen;//1st video including h264 desc string
            dsp1stV.size -= dsp1stV.h264DescLen;
        }
    }
//#NT#2010/05/05#Meg Lin -end
    if (pDecobj->CustomizeFunc)
    {
        //#NT#2011/11/18#Meg Lin -begin
        customER = (pDecobj->CustomizeFunc)(MEDIAVID_CUSTOM_DECODE1ST, &dsp1stV);
        if (customER != E_OK)
        {
            debug_msg("^G Decoding 1st pic fails....");
        }
        //#NT#2011/11/18#Meg Lin -end
    }
    pMediaInfo->yAddr = dsp1stV.y_Addr;
    pMediaInfo->cbAddr = dsp1stV.cb_Addr;
    pMediaInfo->crAddr = dsp1stV.cr_Addr;
    pMediaInfo->imageWidth= media1stV.wid;
    pMediaInfo->imageHeight= media1stV.hei;
    pMediaInfo->filetype = media1stV.type;
    pMediaInfo->jpgType = dsp1stV.jpgType;
    MediaPlay_SetDecode1stParam(pMediaInfo);
}

void AppInit_ModePlayback_BGOnV2(void)
{
#if BG_MOVIE
    UINT32  uiPoolAddr, uiFlag;
    DECJPG_INFO DecOneJPGInfo;
    JPGHEAD_DEC_CFG     JPGCfg;
    UINT32 imageSize;
    UINT32 bufSize;
    STR_SCALE_PRAM_EXT  stIMEScale;
    ER  retV = E_OK;

    // Put the BG image to POOL_ID_FB2 for Movie background
    retV = get_blk((void*)&uiPoolAddr, POOL_ID_FB2);
    if (retV != E_OK)
    {
        debug_err(("PB> get memory FAIL!\r\n"));
    }
    rel_blk(POOL_ID_FB2, (VP)uiPoolAddr);

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGMovie_320x240;

    DecOneJPGInfo.pDstAddr          = (UINT8 *)uiPoolAddr;
    DecOneJPGInfo.JPGFileSize       = BG_MOVIE_BS_SIZE;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;
//#NT#2009/05/20#Corey Ke -end

    // Decode startup JPG, DecodeOneJPG() will open JPEG driver
    retV = DecodeOneJPG(&DecOneJPGInfo);
    if (retV != E_OK)
    {
        debug_err(("PB> decode MovieBG FAIL!\r\n"));
    }

//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
#if 0
//#NT#2008/11/11#Scottie -begin
//#NT#Use jpeg_waitDone() instead of wai_flg() & jpeg_getStatus()
//    wai_flg((UINT *)&uiFlag, FLG_ID_INT, FLGINT_JPEG, TWF_ORW | TWF_CLR);
    uiFlag = jpeg_waitDone();
    jpeg_setEndDecode();
    jpeg_close();

    // Check JPG decoder status
//    if ((jpeg_getStatus() & _JPG_Int_FrameEnd) == 0)
    if (uiFlag & _JPG_Int_FrameEnd) == 0)
//#NT#2008/11/11#Scottie -end
    {
        debug_err(("PB> decode MovieBG FAIL!\r\n"));
    }
#endif
//#NT#2009/05/20#Corey Ke -end

    imageSize = JPGCfg.imagewidth * JPGCfg.imageheight;
    bufSize = g_LCDSize.uiWidth * g_LCDSize.uiHeight;

    g_uiMovieBGAddrY = DecOneJPGInfo.uiOutAddrCr + imageSize;
    g_uiMovieBGAddrCb = g_uiMovieBGAddrY + bufSize;
    g_uiMovieBGAddrCr = g_uiMovieBGAddrCb + bufSize;

    stIMEScale.src = 1;// IME source 0:IPEtoIME 1:DMAtoIME

    if(JPGCfg.fileformat == JPGFormatYUV211)
    {
        stIMEScale.in_format  = IME_IN_FMT_422;
    }
    else
    {
        stIMEScale.in_format  = IME_IN_FMT_420;
    }

    stIMEScale.out_format = IME_OUT_FMT_422_COSITED;

    stIMEScale.in_h           = JPGCfg.imagewidth;
    stIMEScale.in_v           = JPGCfg.imageheight;
    stIMEScale.in_lineoffset  = JPGCfg.lineoffsetY;
    stIMEScale.in_p1_addr     = DecOneJPGInfo.uiOutAddrY;
    stIMEScale.in_p2_addr     = DecOneJPGInfo.uiOutAddrCb;
    stIMEScale.in_p3_addr     = DecOneJPGInfo.uiOutAddrCr;

    stIMEScale.out_h          = g_LCDSize.uiWidth;
    stIMEScale.out_v          = g_LCDSize.uiHeight;
    stIMEScale.out_lineoffset = g_LCDSize.uiWidth;
    stIMEScale.out_p1_addr    = g_uiMovieBGAddrY;
    stIMEScale.out_p2_addr    = g_uiMovieBGAddrCb;
    stIMEScale.out_p3_addr    = g_uiMovieBGAddrCr;

    ime_DMAScale_ext(stIMEScale);
#endif// BG_MOVIE
}
#if 0
#include "ime_lib.h"
#include "pll.h"
void AppInit_ModePlayback_BGOnFB2(void)
{
#if (BG_BADFILE)
    ER                  wResult = E_OK;
    UINT32              uiPoolAddr, uiFlag;
    UINT32              uiImageSize, uiBufSize;
    DECJPG_INFO         DecOneJPGInfo;
    JPGHEAD_DEC_CFG     JPGCfg;
    STR_SCALE_PRAM_EXT  stIMEScale;

    uiBufSize = g_LCDSize.uiWidth * g_LCDSize.uiHeight;
#if BG_MOVIE
    // after movie background
    uiPoolAddr = g_uiMovieBGAddrCr + (uiBufSize >> 1);
#else
    // Put the BG image to POOL_ID_FB2 for bad file background
    wResult = get_blk((void*)&uiPoolAddr, POOL_ID_FB2);
    if (wResult != E_OK)
    {
        debug_err(("PB> get memory FAIL!\r\n"));
    }
    rel_blk(POOL_ID_FB2, (VP)uiPoolAddr);
#endif

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGBadFile_320x240;
    DecOneJPGInfo.pDstAddr          = (UINT8 *)uiPoolAddr;
    DecOneJPGInfo.JPGFileSize       = BG_BADFILE_BS_SIZE;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;
//#NT#2009/05/20#Corey Ke -end

    // Decode startup JPG, DecodeOneJPG() will open JPEG driver
    wResult = DecodeOneJPG(&DecOneJPGInfo);
    if (wResult != E_OK)
    {
        debug_err(("PB> decode BadFileBG FAIL!\r\n"));
    }

//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
#if 0
//#NT#2008/11/11#Scottie -begin
//#NT#Use jpeg_waitDone() instead of wai_flg()
//    wai_flg((UINT *)&uiFlag, FLG_ID_INT, FLGINT_JPEG, TWF_ORW | TWF_CLR);
    uiFlag = jpeg_waitDone();
    jpeg_setEndDecode();
    jpeg_close();

    // Check JPG decoder status
//    if ((jpeg_getStatus() & _JPG_Int_FrameEnd) == 0)
    if ((uiFlag & _JPG_Int_FrameEnd) == 0)
//#NT#2008/11/11#Scottie -end
    {
        debug_err(("PB> decode BadFileBG FAIL!\r\n"));
    }
#endif
//#NT#2009/05/20#Corey Ke -end

    uiImageSize = JPGCfg.imagewidth * JPGCfg.imageheight;

    g_uiBadFileBGAddrY = DecOneJPGInfo.uiOutAddrCr + (uiImageSize >> 1);
    g_uiBadFileBGAddrCb = g_uiBadFileBGAddrY + uiBufSize;
    g_uiBadFileBGAddrCr = g_uiBadFileBGAddrCb + (uiBufSize >> 1);

    stIMEScale.src = 1;// IME source 0:IPEtoIME 1:DMAtoIME

    if(JPGCfg.fileformat == JPGFormatYUV211)
    {
        stIMEScale.in_format  = IME_IN_FMT_422;
    }
    else
    {
        stIMEScale.in_format  = IME_IN_FMT_420;
    }

    stIMEScale.out_format = IME_OUT_FMT_422_COSITED;

    stIMEScale.in_h           = JPGCfg.imagewidth;
    stIMEScale.in_v           = JPGCfg.imageheight;
    stIMEScale.in_lineoffset  = JPGCfg.lineoffsetY;
    stIMEScale.in_p1_addr     = DecOneJPGInfo.uiOutAddrY;
    stIMEScale.in_p2_addr     = DecOneJPGInfo.uiOutAddrCb;
    stIMEScale.in_p3_addr     = DecOneJPGInfo.uiOutAddrCr;

    stIMEScale.out_h          = g_LCDSize.uiWidth;
    stIMEScale.out_v          = g_LCDSize.uiHeight;
    stIMEScale.out_lineoffset = g_LCDSize.uiWidth;
    stIMEScale.out_p1_addr    = g_uiBadFileBGAddrY;
    stIMEScale.out_p2_addr    = g_uiBadFileBGAddrCb;
    stIMEScale.out_p3_addr    = g_uiBadFileBGAddrCr;

    ime_DMAScale_ext(stIMEScale);
//#NT#2009/01/14#Scottie -begin
//#NT#Add for drawing the wav sample image
//#endif// BG_BADFILE
#elif (BG_WAVFILE)
    #if 1//new
    ER                  wResult = E_OK;
    UINT32              uiPoolAddr, uiFlag;
    UINT32              uiImageSize, uiBufSize;
    DECJPG_INFO         DecOneJPGInfo;
    JPGHEAD_DEC_CFG     JPGCfg;

    //STR_SCALE_PRAM_EXT  stIMEScale;
    IME_OPENOBJ IMEOpenObj;
    IME_MODE_PRAM OneIme;

    uiBufSize = g_LCDSize.uiWidth * g_LCDSize.uiHeight;

    // Put the BG image to POOL_ID_FB2 for movie file background
    wResult = get_blk((void*)&uiPoolAddr, POOL_ID_IQ);
    if (wResult != E_OK)
    {
        debug_err(("PB> get memory FAIL!\r\n"));
    }
    rel_blk(POOL_ID_IQ, (VP)uiPoolAddr);

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGWavFile_320x240;
    DecOneJPGInfo.pDstAddr          = (UINT8 *)uiPoolAddr;

    DecOneJPGInfo.JPGFileSize       = BG_WAVFILE_BS_SIZE;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;
//#NT#2009/05/20#Corey Ke -end

    // Decode startup JPG, DecodeOneJPG() will open JPEG driver
    wResult = DecodeOneJPG(&DecOneJPGInfo);
    if (wResult != E_OK)
    {
        debug_err(("PB> decode WavFileBG FAIL!\r\n"));
    }

    uiImageSize = JPGCfg.imagewidth * JPGCfg.imageheight;

    g_uiWavFileBGAddrY = DecOneJPGInfo.uiOutAddrCr + (uiImageSize >> 1);
    g_uiWavFileBGAddrCb = g_uiWavFileBGAddrY + uiBufSize;
    g_uiWavFileBGAddrCr = g_uiWavFileBGAddrCb + (uiBufSize >> 1);

    OneIme.op_mode = _D2D;
    OneIme.in_src = IMEALG_DMA2IME;

    if(JPGCfg.fileformat == JPGFormatYUV211)
    {
        //stIMEScale.in_format  = IME_IN_FMT_422;
        OneIme.format.in_format      = IME_IN_FMT_422;
    }
    else
    {
        //stIMEScale.in_format  = IME_IN_FMT_420;
        OneIme.format.in_format      = IME_IN_FMT_420;
    }
    OneIme.in_size.h_size        = JPGCfg.imagewidth;
    OneIme.in_size.v_size        = JPGCfg.imageheight;
    OneIme.line_ofs.in_lineofs   = JPGCfg.lineoffsetY;
    OneIme.in_addr.y_addr        = DecOneJPGInfo.uiOutAddrY;
    OneIme.in_addr.cb_addr       = DecOneJPGInfo.uiOutAddrCb;
    OneIme.in_addr.cr_addr       = DecOneJPGInfo.uiOutAddrCr;

    OneIme.ycc_format.in_format  = 0;//pScale->in_pack;

    OneIme.path_num = IMEALG_1PATH;

    OneIme.format.out1_format    = IME_OUT_FMT_422_COSITED;
    OneIme.p1_out_size.h_size    = g_LCDSize.uiWidth;
    OneIme.p1_out_size.v_size    = g_LCDSize.uiHeight;

    OneIme.line_ofs.out1_lineofs = g_LCDSize.uiWidth;
    OneIme.p1_out0_addr.y_addr   = g_uiWavFileBGAddrY;
    OneIme.p1_out0_addr.cb_addr  = g_uiWavFileBGAddrCb;
    OneIme.p1_out0_addr.cr_addr  = g_uiWavFileBGAddrCr;

    OneIme.ycc_format.out1_format= IMEALG_YCBCR_PLANAR;
    OneIme.ycc_format.out2_format= IMEALG_YCBCR_PLANAR;

    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    IMEOpenObj.FP_IMEISR_CB = NULL;
    ime_open(&IMEOpenObj);
    ime_setMode(&OneIme);
    ime_clrIntFlag();
    ime_setStart();
    ime_waitFlagFrameEnd();
    ime_setStop();
    ime_close();

    #else
    ER                  wResult = E_OK;
    UINT32              uiPoolAddr, uiFlag;
    UINT32              uiImageSize, uiBufSize;
    DECJPG_INFO         DecOneJPGInfo;
    JPGHEAD_DEC_CFG     JPGCfg;
    STR_SCALE_PRAM_EXT  stIMEScale;

    uiBufSize = g_LCDSize.uiWidth * g_LCDSize.uiHeight;

    // Put the BG image to POOL_ID_FB2 for movie file background
    wResult = get_blk((void*)&uiPoolAddr, POOL_ID_FB2);
    if (wResult != E_OK)
    {
        debug_err(("PB> get memory FAIL!\r\n"));
    }
    rel_blk(POOL_ID_FB2, (VP)uiPoolAddr);

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGWavFile_320x240;
    DecOneJPGInfo.pDstAddr          = (UINT8 *)uiPoolAddr;
    DecOneJPGInfo.JPGFileSize       = BG_WAVFILE_BS_SIZE;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;
//#NT#2009/05/20#Corey Ke -end

    // Decode startup JPG, DecodeOneJPG() will open JPEG driver
    wResult = DecodeOneJPG(&DecOneJPGInfo);
    if (wResult != E_OK)
    {
        debug_err(("PB> decode WavFileBG FAIL!\r\n"));
    }

//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
#if 0
    uiFlag = jpeg_waitDone();
    jpeg_setEndDecode();
    jpeg_close();

    // Check JPG decoder status
    if ((uiFlag & _JPG_Int_FrameEnd) == 0)
    {
        debug_err(("PB> decode WavFileBG FAIL!\r\n"));
    }
#endif
//#NT#2009/05/20#Corey Ke -end

    uiImageSize = JPGCfg.imagewidth * JPGCfg.imageheight;

    g_uiWavFileBGAddrY = DecOneJPGInfo.uiOutAddrCr + (uiImageSize >> 1);
    g_uiWavFileBGAddrCb = g_uiWavFileBGAddrY + uiBufSize;
    g_uiWavFileBGAddrCr = g_uiWavFileBGAddrCb + (uiBufSize >> 1);

    stIMEScale.src = IME_SRC_DMA;
    if(JPGCfg.fileformat == JPGFormatYUV211)
    {
        stIMEScale.in_format  = IME_IN_FMT_422;
    }
    else
    {
        stIMEScale.in_format  = IME_IN_FMT_420;
    }
    stIMEScale.out_format = IME_OUT_FMT_422_COSITED;

    stIMEScale.in_h           = JPGCfg.imagewidth;
    stIMEScale.in_v           = JPGCfg.imageheight;
    stIMEScale.in_lineoffset  = JPGCfg.lineoffsetY;
    stIMEScale.in_p1_addr     = DecOneJPGInfo.uiOutAddrY;
    stIMEScale.in_p2_addr     = DecOneJPGInfo.uiOutAddrCb;
    stIMEScale.in_p3_addr     = DecOneJPGInfo.uiOutAddrCr;

    stIMEScale.out_h          = g_LCDSize.uiWidth;
    stIMEScale.out_v          = g_LCDSize.uiHeight;
    stIMEScale.out_lineoffset = g_LCDSize.uiWidth;
    stIMEScale.out_p1_addr    = g_uiWavFileBGAddrY;
    stIMEScale.out_p2_addr    = g_uiWavFileBGAddrCb;
    stIMEScale.out_p3_addr    = g_uiWavFileBGAddrCr;

    //ime_DMAScale_ext(stIMEScale);
    #endif
#endif// BG_WAVFILE
//#NT#2009/01/14#Scottie -end
}
#endif

static UINT16 AppInit_PB_Fmt = PLAYFMT_JPG | PLAYFMT_WAV | PLAYFMT_AVI | PLAYFMT_MOVMJPG;
static AppInit_Playback_Mode AppInit_PB_Mode = APP_PLAYBACK_FLOW;//APP_PLAYBACK_NORMAL;

static void AppInit_ModePlayback_Set_Fmt(UINT16 fmt)
{
    UINT32 fsfmt = 0;
    AppInit_PB_Fmt = fmt;

    if((fmt&PLAYFMT_JPG) == PLAYFMT_JPG)
        fsfmt = fsfmt | FST_FMT_JPG;
    if((fmt&PLAYFMT_WAV) == PLAYFMT_WAV)
        fsfmt = fsfmt | FST_FMT_WAV;
    if((fmt&PLAYFMT_AVI) == PLAYFMT_AVI)
        fsfmt = fsfmt | FST_FMT_AVI;
    if((fmt&PLAYFMT_MOVMJPG) == PLAYFMT_MOVMJPG)
        fsfmt = fsfmt | FST_FMT_MOV;

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysCloseActFile();
    FilesysSetValidFileFormat(fsfmt);
    FilesysScanDCFObj();
    return;
}
static UINT16 AppInit_ModePlayback_Get_Fmt(void)
{
    return AppInit_PB_Fmt;
}
void AppInit_ModePlayback_Set_Mode(AppInit_Playback_Mode mode)
{
    AppInit_PB_Mode = mode;
    switch(mode)
    {
    case APP_PLAYBACK_NORMAL:
    case APP_PLAYBACK_FLOW:
#if (_MOVIE_FMT_ == _MOVIE_FMT_MOVH264_)
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_JPG |PLAYFMT_AVI);//2010/12/15 Meg Lin
#else //_MOVIE_FMT_AVIH264_
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_JPG | PLAYFMT_AVI);
#endif
        break;
    case APP_PLAYBACK_WAV:
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_WAV);
        break;
    case APP_PLAYBACK_BR_JPG:
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_JPG);
        break;
    case APP_PLAYBACK_BR_VIDEO:
#if (_MOVIE_FMT_ == _MOVIE_FMT_MOVH264_)
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_MOVMJPG);
#else //_MOVIE_FMT_AVIH264_
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_AVI);
#endif
        break;
    case APP_PLAYBACK_BR_WAV:
        AppInit_ModePlayback_Set_Fmt(PLAYFMT_WAV);
        break;
    }
}
AppInit_Playback_Mode AppInit_ModePlayback_Get_Mode(void)
{
    return AppInit_PB_Mode;
}
/**
  Initialize application for Playback mode

  Initialize application for Playback mode.

  @param void
  @return void
*/
void AppInit_ModePlayback(void)
{
    UINT32              uiPoolAddr;
    PLAY_OBJ            PlayObj;
#if(_WAVSTUDIO_MODE_ == ENABLE)
    WAVSTUDIO_APPOBJ    WavObj;
    WAVSTUDIO_FMT       WavFormat;
#endif
    UINT32              uiBufSize, uiNeedBufSize;

    memset(&PlayObj, 0, sizeof(PLAY_OBJ));

    get_blk((VP*)&(uiPoolAddr), POOL_ID_IQ);
    rel_blk(POOL_ID_IQ, (VP)uiPoolAddr);

    PlayObj.uiMemoryAddr        = uiPoolAddr;
#if (_RAM_DISK_ == ENABLE)
    PlayObj.uiMemorySize        = POOL_SIZE_IQ + POOL_SIZE_CAPTURE - POOL_SIZE_WAV - POOL_SIZE_RAM_DISK - POOL_SIZE_RAM2_DISK;
#else
    PlayObj.uiMemorySize        = POOL_SIZE_IQ + POOL_SIZE_CAPTURE - POOL_SIZE_WAV;
#endif
    PlayObj.CallBackFunc        = NULL;
    PlayObj.Decode1stASFFunc    = NULL;
    PlayObj.Decode1stMPEG1Func  = NULL;
    PlayObj.Decode1stMediaFunc  = AppinitMedia_Decode1stVideo;

    PlayObj.usPlayFileFormat    = AppInit_ModePlayback_Get_Fmt();//PLAYFMT_JPG | PLAYFMT_WAV | PLAYFMT_AVI | PLAYFMT_MOVMJPG;

    PlayObj.uiDeviceWidth       = g_LCDSize.uiWinWidth;
    PlayObj.uiDeviceHeight      = g_LCDSize.uiWinHeight;
    PlayObj.uiDisplayFBWidth    = g_LCDSize.uiWidth;
    PlayObj.uiDisplayFBHeight   = g_LCDSize.uiHeight;
    PlayObj.ui1stPlayMode       = PLAYMODE_UNKNOWN;

//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#if 0
    PlayObj.uiMoveImgSpeedTable = SlideSpeedTable;
#endif
    PlayObj.uiMoveImgDelayTime = 0;
//#NT#2009/06/03#Corey Ke -end
    PlayObj.Orientation         = PLAY_HORIZONTAL;
#if (1)
    PlayObj.Flags               = PB_ENABLE_TRANSFROM;//PB_DISABLE_TRANSFROM;
#if (BG_MOVIE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_MOVIE;
#endif
#if (BG_THUMBNAIL)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BACKGROUND_IN_THUMB;
#endif
#if (BG_BADFILE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_BADFILE;
#endif
//#NT#2009/01/14#Scottie -begin
//#NT#Add for drawing the wav sample image
#if (BG_WAVFILE)
    PlayObj.Flags              |= PB_ENABLE_SHOW_BG_IN_WAVFILE;
#endif
//#NT#2009/01/14#Scottie -end
#else
    PlayObj.Flags               = PB_DISABLE_TRANSFROM;
#endif
    if(KeyScan_IsHDMIPlugIn())
        PlayObj.Flags |= PB_ENABLE_FORCE_TO_DECODE_PRIMARY;//HDMI mode,we decode primay picture

    if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_MOVIE)
    {// Prepare image for movie background on FB2
//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
#if 0
        AppInit_ModePlayback_BGOnV2();
        PlayObj.uiMovieBGAddrY  = g_uiMovieBGAddrY;
        PlayObj.uiMovieBGAddrCb = g_uiMovieBGAddrCb;
        PlayObj.uiMovieBGAddrCr = g_uiMovieBGAddrCr;
#endif
#if (BG_MOVIE)
        PlayObj.uiMovieBGAddr = (UINT32)g_ucBGMovie_320x240;
        PlayObj.uiMovieBGSize = BG_MOVIE_BS_SIZE;
#endif
//#NT#2009/10/20#Corey Ke -end
    }

    if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_BADFILE)
    {// Prepare image for bad file background on FB2
//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
#if 0
        AppInit_ModePlayback_BGOnFB2();
        PlayObj.uiBadFileBGAddrY  = g_uiBadFileBGAddrY;
        PlayObj.uiBadFileBGAddrCb = g_uiBadFileBGAddrCb;
        PlayObj.uiBadFileBGAddrCr = g_uiBadFileBGAddrCr;
#endif
#if (BG_BADFILE)
        PlayObj.uiBadFileBGAddr = (UINT32)g_ucBGBadFile_320x240;
        PlayObj.uiBadFileBGSize = BG_BADFILE_BS_SIZE;
#endif
//#NT#2009/10/20#Corey Ke -end
    }

//#NT#2009/01/14#Scottie -begin
//#NT#Add for drawing the wav sample image
    if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_WAVFILE)
    {// Prepare image for wav file background on FB2
//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
#if 0
        AppInit_ModePlayback_BGOnFB2();
        PlayObj.uiWavFileBGAddrY  = g_uiWavFileBGAddrY;
        PlayObj.uiWavFileBGAddrCb = g_uiWavFileBGAddrCb;
        PlayObj.uiWavFileBGAddrCr = g_uiWavFileBGAddrCr;
#endif
#if (BG_WAVFILE)
        PlayObj.uiWaveBGAddr = (UINT32)g_ucBGWavFile_320x240;
        PlayObj.uiWaveBGSize = BG_WAVFILE_BS_SIZE;
#endif
//#NT#2009/10/20#Corey Ke -end
    }
//#NT#2009/01/14#Scottie -end

//#NT#2009/10/20#Corey Ke -begin
//#NT#optimize playback buffer usage
#if 0
//#NT#2007/09/20#Scottie -begin
//#Reserve memory pool FB2 for being the tmp buf for TV plug & unplug mode..
    uiBufSize = g_LCDSize.uiWidth * g_LCDSize.uiHeight;
    if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_BADFILE)
    {
        uiNeedBufSize = (320*240*2) + (uiBufSize*2) + (uiBufSize*2) + (uiBufSize*2);
        if(uiNeedBufSize > POOL_SIZE_FB2)
        {
            PlayObj.uiMemFB2Addr = 0;
            debug_err(("PB> assign memory FAIL(1)!\r\n"));
        }
        else
        {
            PlayObj.uiMemFB2Addr = g_uiBadFileBGAddrCr + (uiBufSize >> 1);
        }
    }
//#NT#2009/01/14#Scottie -begin
//#NT#Add for drawing the wav sample image
    else if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_WAVFILE)
    {
        uiNeedBufSize = (320*240*2) + (uiBufSize*2) + (uiBufSize*2);
        if(uiNeedBufSize > POOL_SIZE_FB2)
        {
            PlayObj.uiMemFB2Addr = 0;
            debug_err(("PB> assign memory FAIL(1)!\r\n"));
        }
        else
        {
            PlayObj.uiMemFB2Addr = g_uiWavFileBGAddrCr + (uiBufSize >> 1);
        }
    }
//#NT#2009/01/14#Scottie -end
    else if (PlayObj.Flags & PB_ENABLE_SHOW_BG_IN_MOVIE)
    {
        uiNeedBufSize = (320*240*2) + (uiBufSize*2) + (uiBufSize*2);
        if(uiNeedBufSize > POOL_SIZE_FB2)
        {
            PlayObj.uiMemFB2Addr = 0;
            debug_err(("PB> assign memory FAIL(2)!\r\n"));
        }
        else
        {
            PlayObj.uiMemFB2Addr = g_uiMovieBGAddrCr + (uiBufSize >> 1);
        }
    }
    else
    {
        get_blk((void*)&uiPoolAddr, POOL_ID_FB2);
        rel_blk(POOL_ID_FB2, (VP)uiPoolAddr);

        uiNeedBufSize = (320*240*2) + (uiBufSize*2);
        if(uiNeedBufSize > POOL_SIZE_FB2)
        {
            PlayObj.uiMemFB2Addr = 0;
            debug_err(("PB> assign memory FAIL(3)!\r\n"));
        }
        else
        {
            PlayObj.uiMemFB2Addr = uiPoolAddr;
        }
    }
//#NT#2007/09/20#Scottie -end
#endif
//#NT#2009/10/20#Corey Ke -end

    PlayObj.uiThumbShowImageMode   = PB_SHOW_THUMB_IMAGE_ONE_BY_ONE;//PB_SHOW_THUMB_IMAGE_IN_THE_SAME_TIME;
    if(KeyScan_IsHDMIPlugIn() == TRUE)
    {
        PlayObj.uiPanelWidth           = g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
        PlayObj.uiPanelHeight          = g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
    }
    else if(KeyScan_IsTVPlugIn() == TRUE)
    {
        //#NT#2009/11/11#Randy -begin
        //TV is 4:3 output
        PlayObj.uiPanelWidth           = 320;//g_LCDSize.uiWinWidth;//PB_PANEL_NO_SCALING;
        PlayObj.uiPanelHeight          = 240;//g_LCDSize.uiWinHeight;//PB_PANEL_NO_SCALING;
        //#NT#2009/11/11#Randy -end
    }
    else
    {//LCD panel
        #if (_LCDTYPE_ == _LCDTYPE_TXDT300C_)
        PlayObj.uiPanelWidth           = PB_PANEL_NO_SCALING;
        PlayObj.uiPanelHeight          = PB_PANEL_NO_SCALING;
        #else
        PlayObj.uiPanelWidth           = 320;//PB_PANEL_NO_SCALING;
        PlayObj.uiPanelHeight          = 240;//PB_PANEL_NO_SCALING;
        #endif
    }
    PlayObj.bEnableAutoRotate      = TRUE;
    PlayObj.ThumbOnlyRect.uiLeft   = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.ThumbOnlyRect.uiTop    = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.ThumbOnlyRect.uiWidth  = PB_THUMB_ONLY_DEFAULT_SETTING;
    PlayObj.ThumbOnlyRect.uiHeight = PB_THUMB_ONLY_DEFAULT_SETTING;
    //#NT#2009/05/26#KS Hung -begin
    PlayObj.bDisplayLastImg        = TRUE;
    //#NT#2009/05/26#KS Hung -end
    // Open playback task
    if (PB_Open(&PlayObj) != E_OK)
    {
        debug_err(("Error open playback task\r\n"));
    }
    //#NT#2009/10/23#KS Hung -begin
    //#NT#Solve the speed is too slow for slide show.
    if (KeyScan_IsHDMIPlugIn() == TRUE)
    {
        SE_SetSpeed(4);
        PB_EnableFeature(PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF);
    }
    else if (KeyScan_IsTVPlugIn() == TRUE)
    {
        SE_SetSpeed(2);
        PB_EnableFeature(PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF);
    }
    else // Hideo@100322: should set SE speed back!
    {
        SE_SetSpeed(2);
        PB_DisableFeature(PB_ENABLE_SLIDE_SHOW_IN_SMALL_BUF);
    }
    //#NT#2009/10/23#KS Hung -end
    // Do first video frame object initial
    PB_1stVideoFrameInit();
#if (BG_MOVIE)
    // Set the position and size of first video frame
    if (KeyScan_IsTVPlugIn() == TRUE)
    {
        if (g_uiTVMode == TV_MODE_NTSC)
        {
            PB_Set1stVideoFrame(VIDFRAME_POS_X_NTSC, VIDFRAME_POS_Y_NTSC,
                                VIDFRAME_WIDTH_NTSC, VIDFRAME_HEIGHT_NTSC);
        }
        else//if (g_uiTVMode == TV_MODE_PAL)
        {
            PB_Set1stVideoFrame(VIDFRAME_POS_X_PAL, VIDFRAME_POS_Y_PAL,
                                VIDFRAME_WIDTH_PAL, VIDFRAME_HEIGHT_PAL);
        }
    }
    else
    {
        PB_Set1stVideoFrame(VIDFRAME_POS_X, VIDFRAME_POS_Y,
                            VIDFRAME_WIDTH, VIDFRAME_HEIGHT);
    }
#endif

//#NT#2008/11/20#Hideo Lin -begin
//#NT#Use _WAVSTUDIO_MODE_ to control WAV settings
#if (_WAVSTUDIO_MODE_ == ENABLE)
    WavFormat.AudioChannel      = AUDIO_CH_RIGHT;
    WavFormat.AudioSamplingRate = AUDIO_SR_32000;//AUDIO_SR_11025;
  #if (_VM_FORMAT_ == _VM_FORMAT_PCM8_)
    // PCM 8 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_8;
  #elif (_VM_FORMAT_ == _VM_FORMAT_PCM16_)
    // PCM 16 bits
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_PCM_16;
  #elif (_VM_FORMAT_ == _VM_FORMAT_ADPCM_)
    // ADPCM
    WavFormat.AudioCodec        = AUDIOCODEC_IMA_ADPCM;
    WavFormat.uiBitsPerSample   = WAV_BITS_PER_SAM_ADPCM;
  #elif (_VM_FORMAT_ == _VM_FORMAT_ASFPCM_)
    // ASF PCM
    #if (WAVFILE_ASF_ENABLE == ENABLE)
    WavFormat.AudioCodec        = AUDIOCODEC_PCM;
    WavFormat.FileFormat        = WAVFILE_ASF;
    #else
        #error WAV file does not support ASF format!
    #endif
  #endif
    WavStudio_SetFormat(&WavFormat);

    // Set maximum recording time
    WavStudio_SetMaxRecTime(0xFFFFFFFF);

    // Set user data
  #if (_WAV_SINGLE_USERDATA_ == ENABLE)
    WavStudio_SetUserData((UINT32)&g_uiWAVHeaderUserData, (UINT32)sizeof(g_uiWAVHeaderUserData));
  #else
    WavStudio_SetUserData(0, 0);
  #endif

    // Set WAV application object
    WavObj.uiMemoryAddr         = uiPoolAddr + POOL_SIZE_CAPTURE - POOL_SIZE_WAV;
    WavObj.uiMemorySize         = POOL_SIZE_WAV;
    //#NT#2009/03/24#Hideo Lin -begin
    //#NT#For wav playback in playback mode
    //WavObj.WavStudioCB        = (WAVSTUDIO_CB)FlowCommon_WAVStudioCB;
    WavObj.WavStudioCB          = (WAVSTUDIO_CB)FlowCommon_WAVStudioCB;
    //#NT#2009/03/24#Hideo Lin -end

    // Open Wav studio task
    if (WavStudio_Open(&WavObj) == E_NOMEM)
    {
        debug_err(("Not enough memory for wav studio task\r\n"));
    }
#endif
//#NT#2008/11/20#Hideo Lin -end
}

//@}
