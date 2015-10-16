
#ifndef _PHOTOTSK_H
#define _PHOTOTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "appcommon.h"
#include "Lens.h"
#include "Alg_IPLMode.h"
#include "fd_lib.h"
//
//FLG_ID_PHOTO
//
#define FLGPHOTO_1STIDLE        FLGPTN_BIT(0)
#define FLGPHOTO_2NDIDLE        FLGPTN_BIT(1)
#define FLGPHOTO_3RDIDLE        FLGPTN_BIT(2)

#define FLGPHOTO_ZOOMIN         FLGPTN_BIT(3)      // zoom-in on preview for photo mode
#define FLGPHOTO_ZOOMOUT        FLGPTN_BIT(4)      // zoom-out on preview for photo mode
#define FLGPHOTO_AE             FLGPTN_BIT(5)      // AE on preview for photo mode
#define FLGPHOTO_AWB            FLGPTN_BIT(6)      // AWB on preview for photo mode
#define FLGPHOTO_FRAME_DONE     FLGPTN_BIT(7)      // sie frame done
#define FLGPHOTO_FD             FLGPTN_BIT(8)      //Face Detection
#define FLGPHOTO_AF_VA          FLGPTN_BIT(9)      //AF VA
#define FLGPHOTO_SD             FLGPTN_BIT(10)     //Smile Detection
#define FLGPHOTO_CA             FLGPTN_BIT(11)     //CA process

//
#define PHOTO_1ST   FLGPHOTO_1STIDLE
#define PHOTO_2ND   FLGPHOTO_2NDIDLE
#define PHOTO_3RD   FLGPHOTO_3RDIDLE

#define COLOR_Y         0
#define COLOR_CB        1
#define COLOR_CR        2
#define LOCK            1
#define UNLOCK          0


typedef void (*FPPHOTOCB)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
typedef void (*FPPHOTOAE)(void);
typedef void (*FPPHOTOAWB)(void);
typedef void (*FPPHOTOAF)(void);
typedef void (*FPPHOTOFD)(void);
typedef void (*FPPHOTOSD)(void);
//-----------------------------------------------------------------------
// Global data release
//-----------------------------------------------------------------------
extern AlgMsgInfo    gAlgMsg;
//-----------------------------------------------------------------------
// function prototype description
//-----------------------------------------------------------------------
extern ER      Photo_Open(PPHOTO_APPOBJ pPhotoInfor);
extern ER      Photo_Close(void);
extern UINT    Photo_SetMemory(UINT S_Addr,UINT Size);
extern INT32   Photo_RegisterAE(FPPHOTOAE pPhotoAE);
extern INT32   Photo_RegisterAWB(FPPHOTOAWB pPhotoAWB);
extern INT32   Photo_RegisterFD(FPPHOTOFD pPhotoFD);
extern INT32   Photo_RegisterSD(FPPHOTOSD pPhotoSD);
extern void    Photo_LockAE(UB lock);
extern BOOL    Photo_IsAELock(void);
extern void    Photo_LockAWB(UB lock);
extern BOOL    Photo_IsAWBLock(void);
extern void    Photo_LockFD(UB lock);
extern BOOL    Photo_IsFDLock(void);
extern void    Photo_LockSD(UB lock);
extern BOOL    Photo_IsSDLock(void);
extern void    Photo_SetAeRate(UB rate);
extern UB      Photo_GetAERate(void);
extern void    Photo_SetAwbRate(UB rate);
extern UB      Photo_GetAWBRate(void);
extern void    Photo_SetFdRate(UB rate);
extern UB      Photo_GetFdRate(void);
extern void    Photo_SetSdRate(UB rate);
extern UB      Photo_GetSdRate(void);
extern void    Photo_GetHist(UW *Histogram);
extern BOOL    Photo_WaitPhotoIdle(BOOL bWaitInfinite, UINT8 TaskSel);


extern INT32   Photo_OpticalZoom_InitStart(void);
extern INT32   Photo_OpticalZoom_InitEnd(void);
extern INT32   Photo_OpticalZoom_In(void);
extern INT32   Photo_OpticalZoom_Out(void);
extern INT32   Photo_OpticalZoom_Stop(void);
extern INT32   Photo_OpticalZoom_Retract(void);
extern INT32   Photo_OpticalZoom_GetSection(void);
extern INT32   Photo_OpticalZoom_GetMaxSection(void);

extern INT32   Photo_DZoom_In(void);
extern INT32   Photo_DZoom_Out(void);
extern INT32   Photo_DZoom_StepIn(void);
extern INT32   Photo_DZoom_StepOut(void);
extern INT32   Photo_DZoom_Stop(void);
extern INT32   Photo_DZoom_GotoStep(unsigned int dzoom_step);
extern UINT    Photo_GetDZoomIdx(void);
extern UINT    Photo_GetMaxDzoomIdx(void);

extern INT32   Photo_Focus_Init(void);
extern INT32   Photo_Focus_Goto(unsigned short position);
extern INT32   Photo_Shutter_Move(unsigned char open);
extern INT32   Photo_Aperture_Move(unsigned short NewPosition);
//#NT#2010/09/10#ethanlau -begin
//#NT#remove api
//extern INT32 Photo_SetImageRatio(_IMGRatio_sel RatioIdx);
//#NT#2010/09/10#ethanlau -end
//#NT#2009/09/24#SA4 -begin
//#NT#20090922#ethanlau -begin
extern INT32 Photo_SetImageResolution(_IMGResolution_sel ResoluIdx,IPL_MODE mode);
//#NT#20090922#ethanlau -end
//#NT#2009/09/24#SA4 -end

//#NT#2010/09/09#ethanlau -begin
//#NT#set Dzoom time interval,time unit:VD
extern void Photo_SetDzoomTimeInterval(UINT _time);
//#NT#2010/09/09#ethanlau -end
#endif // _PHOTOTSK_H