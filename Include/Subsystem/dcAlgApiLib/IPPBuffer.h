/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2008.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: IPPBuffer Header                                                   *
* Description: This file should be duplicated in ImageAlgInfor.h           *
* Author: JJ                                                               *
****************************************************************************/
#ifndef _IPPBUFFER_H
#define _IPPBUFFER_H

#include "Type.h"

/**
    IPP_BUFFER
    Manage all the buffers for Image-Pipeline
    Add ones if necessary
*/
typedef enum {
    //Total Buffers, for all IPP Usage (PRV and Cap are assigned in the same time)
    IPPBUF_TOTAL_START=0,       //start-ptr of memory resources
    IPPBUF_TOTAL_END,           //end-ptr of memory resources
    IPPBUF_TOTAL_OVLAP_START,   //start-ptr of the capture mode
    IPPBUF_TOTAL_CURREND,       //the end of current assignment, "END" - "CURREND" = remain-memory-size

    //Preview Buffers, for all live-view mode (ex: PRV, VDO, PCC .etc)
    IPPBUF_PRV_START,           //start-ptr of memory resources
    IPPBUF_PRV_END,             //end-ptr of memory resources
    IPPBUF_PRV_CURREND,         //the end of current assignment, "END" - "CURREND" = remain-memory-size

    //Preview Buffers, for all live-view mode (ex: PRV, VDO, PCC .etc)
    IPPBUF_CAP_START,           //start-ptr of memory resources
    IPPBUF_CAP_END,             //end-ptr of memory resources
    IPPBUF_CAP_CURREND,         //the end of current assignment, "END" - "CURREND" = remain-memory-size

    IPPBUF_PRV_SIERAW_1,    //for Sensor(SIE) output raw image
    IPPBUF_PRV_SIERAW_2,
    IPPBUF_PRV_RAW_1,       //for Sensor(PRE) output raw image
    IPPBUF_PRV_RAW_2,
    IPPBUF_PRV_VA,          //for Variation-data output
    IPPBUF_PRV_IME_P1,      //for IME YCC image output
    IPPBUF_PRV_IME_P2,
    IPPBUF_PRV_FD_BUF,      //for Face-Detection buffers
    IPPBUF_PRV_SD_BUF,      //for Smile-Detection buffers
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    IPPBUF_PRV_DIS_BUF,     //for Digital Image Stabilizer
    //#NT#2009/11/27#Connie Yu -end
    IPPBUF_CAP_SIERAW_1,    //for Sensor(SIE) output raw image
    IPPBUF_CAP_SIERAW_2,
    IPPBUF_CAP_RAW_1,       //for Sensor Input (PRE output)
    IPPBUF_CAP_RAW_2,
    IPPBUF_CAP_RAW_3,
    IPPBUF_CAP_RAW_4,
    IPPBUF_CAP_RAW_5,
    IPPBUF_CAP_RAW_6,
    IPPBUF_CAP_RAW_GEOMETRIC,   //buffer for Geometric-distortion-recovery
    IPPBUF_CAP_RAW_DARKFRAME,   //buffer for Dark-frame-substraction
    IPPBUF_CAP_SLC_BUF,         //for Slice-mode process buffers (IME-path1 output)
//#NT#2011/09/02#Spark Chou -begin
    //#NT#
    IPPBUF_CAP_FRAME_Y,
    IPPBUF_CAP_FRAME_CB,
    IPPBUF_CAP_FRAME_CR,
//#NT#2011/09/02#Spark Chou -end
    IPPBUF_CAP_QV_PRE,          //for 1st-stage QV buffer (IME-path2 output)
    IPPBUF_CAP_QV_SCREEN,
    IPPBUF_CAP_QV_DISPLAY,      //for 2nd-stage QV buffer (uaually, the one for quick-view)
    IPPBUF_CAP_QV_THUMBNAIL,    //for 3rd-stage QV buffer (usually, the one for thumbnail)
    IPPBUF_CAP_CHROMA_FILTER,   //for chroma-filter buffer
    IPPBUF_CAP_DATA_STAMP,      //for data-stamp(date-imprint) buffer
    IPPBUF_CAP_JPG_1,           //for jpeg header/bitstream
    IPPBUF_CAP_JPG_2,
    IPPBUF_CAP_JPG_3,
    IPPBUF_CAP_JPG_4,
    IPPBUF_CAP_JPG_5,
    IPPBUF_CAP_JPG_6,
    IPPBUF_CAP_COUPLESHOTPRV,
    IPPBUF_CAP_PRESHOTPRV,
    IPPBUF_CAP_QV_BUF,
//#NT#2009/09/24#SA4 -begin
//#NT#2009/08/31#Jarkko Chang -begin
//#NT#add for pre ca process using
    IPPBUF_CA,
//#NT#2009/08/31#Jarkko Chang -end
//#NT#2009/09/24#SA4 -end
    IPPBUF_MAX_INDEX
} IPPBUF_BUFFER;

typedef enum {
    IPPBUF_RES_PRV = 0,
    IPPBUF_RES_CAP,
    IPPBUF_RES_MAX_INDEX
} IPPBUF_RESOURCE;


/**
    IPPBUF_MODE
    Mode selection for IPP-Buffer-Allocation
    Add ones if necessary
*/
typedef enum {
    //Preview modes, for all live-view/running-frames modes
    IPPBUF_MODE_PRV = 0,        // for Preview/Live-view mode
    IPPBUF_MODE_VDO,            // for Movie/Video mode
    IPPBUF_MODE_PCC,            // for PC-Cam mode
    //Capture modes, for all still-image modes
    IPPBUF_MODE_CAP_NORMAL,     // for normal/single-shot capture
    IPPBUF_MODE_CAP_ALBUMSHOT,  // for album-shot mode
    IPPBUF_MODE_CAP_COUPLESHOT,
    IPPBUF_MODE_CAP_PRESHOT,
    IPPBUF_MODE_MAX_INDEX
} IPPBUF_MODE;


/**
    IPPBuffer Error Message
*/
#define IPPBUF_E_OK                0           /* Normal termination */
#define IPPBUF_E_NOINDEX           (1)         /* Index Out of range */
#define IPPBUF_E_OUTOFBUFF         (2)         /* Out of buffer */

/**
    Assigner Function Type

    Set IPP buffers for certain mode

    @param[in-1] UINT32     BufAddr, memory resource start-ptr
    @param[in-2] UINT32     BufSize, memory resource size

    @return      UINT32     BufferSize in Need

*/
typedef UINT32 (*FPIPPBUFASSIGNER)(UINT32, UINT32);

//#NT#2012/11/28#KS Hung -begin
//#NT#Special application for 5M (2592x1944) UVC camera
extern void   ippbuf_SetPreviewSizeBuf(UINT uiPreviewSize);
extern UINT32 ippbuf_GetPreviewSizeBuf(void);
//#NT#2012/11/28#KS Hung -end
extern UINT32 ippbuf_SetBuf(IPPBUF_BUFFER BufferIdx, UINT32 setValue);
extern UINT32 ippbuf_GetBuf(IPPBUF_BUFFER BufferIdx);
extern UINT32 ippbuf_ExecuteAssigner(IPPBUF_MODE modeIdx);
extern UINT32 ippbuf_SetResource(IPPBUF_RESOURCE ResIdx, UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_GetResource(IPPBUF_RESOURCE ResIdx, UINT32 *BufAddr, UINT32 *BufSize);
extern UINT32 ippbuf_RegisterAssigner(IPPBUF_MODE modeIdx, FPIPPBUFASSIGNER pIPPBufAssigner);
extern void ippbuf_SetResetFlag(BOOL Flag);

//Sample Assigner API
extern UINT32 ippbuf_SampleAssigner_Prv(UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_SampleAssigner_Cap_Normal(UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_SampleAssigner_Cap_AlbumShot(UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_SampleAssigner_Cap_CoupleShot(UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_SampleAssigner_Cap_PreShot(UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_SampleAssigner_Cap_Normal_Frame(UINT32 BufAddr, UINT32 BufSize);
#endif// _IPPBUFFER_H


