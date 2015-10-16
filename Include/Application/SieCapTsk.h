#ifndef _SIECAPTSK_H
#define _SIECAPTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"

#define FLG_CAPTURE FLG_ID_SLICE   // For slice mode


//
//FLG_ID_SLICE
//
#define FLGSLICETSK1_SIECAPIDLE         FLGPTN_BIT(0)
#define FLGSLICETSK1_IDLE               FLGPTN_BIT(1)

#define FLGSLICETSK1_SIERAWEND          FLGPTN_BIT(2)  //SIE raw image capture finished
#define FLGSLICETSK1_SHOT1PIC           FLGPTN_BIT(3)  //Trigger 1 shot
#define FLGSLICETSK1_JPGCOMPLETE        FLGPTN_BIT(4)  //JPG is done
#define FLGSLICETSK1_FSTOK              FLGPTN_BIT(5)  //This event is for General FST Call event
#define FLGSLICETSK1_INDEPSLICE_START   FLGPTN_BIT(6)  //trigger independent slicemode
#define FLGSLICETSK1_INDEPSLICE_END     FLGPTN_BIT(7)  //independent slicemode end
#define FLGSLICETSK1_EXITCAPTURE        FLGPTN_BIT(8)  //This event is for exit capture flow


#define FLGSLICETSK1_ALL                (FLGSLICETSK1_SIERAWEND|FLGSLICETSK1_SHOT1PIC| \
                                        FLGSLICETSK1_JPGCOMPLETE|FLGSLICETSK1_FSTOK| \
                                        FLGSLICETSK1_INDEPSLICE_START|FLGSLICETSK1_INDEPSLICE_END| \
                                        FLGSLICETSK1_EXITCAPTURE)


typedef void (*FPCAPTURECB)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);

extern FPCAPTURECB      g_fpCaptureExport;
extern UINT32           gCapStatus;

extern INT32    ImgSliceOpen(void);
extern INT32    ImgCapture_Open(PCAPTURE_APPOBJ    pCAPInfor);
extern INT32    ImgCapture_Close(void);
extern INT32    ImgCapture_SetParameter(UINT8 item, UINT8 set_value);
extern ImageAlgInfo* ImgCapture_GetParameter(void);
extern INT32    ImgCapture_IsParameterError(void);
extern INT32    ImgCapture_WaitOperationFinish(UINT8 mode);
extern INT32    ImgCapture_StartCapture(void);
extern INT32    ImgCapture_StopBurstCapture(void);
extern INT32    ImgCapture_CSGetCapCounter(void);
extern void     ImgCapture_CSCapCounterReset(void);
extern INT32    ImgCapture_CSModeEnable(UINT32 EnableFlag);
extern INT32    ImgCapture_CSModeReset(void);
extern INT32    ImgCapture_PSGetCapCounter(void);
extern void     ImgCapture_PSCapCounterReset(void);
extern INT32    ImgCapture_PSModeEnable(UINT32 EnableFlag);
extern INT32    ImgCapture_PSModeReset(void);
extern INT32    ImgCapture_ASModeEnable(UINT32 EnableFlag);
extern INT32    ImgCapture_ASModeReset(void);
extern INT32    ImgCapture_ASModeChgDispType(UINT32 DispType);
extern INT32    ImgGetCurJpgBuf(UINT32 *JpgAdr, UINT32 *JpgSize);
//#NT#2009/11/22#SA4 KS Hung -begin
extern INT32    ImgGetPrevJpgBuf(UINT32 *JpgAdr, UINT32 *JpgSize);
//#NT#2009/11/22#SA4 KS Hung -end

#define STATUS_PREVIEWIDLE              0
#define STATUS_PREVIEWBUSY              1
#define STATUS_SINGLE                   2
#define STATUS_BURST3                   3
#define STATUS_INFBURST                 4
#define STATUS_SMALLBURST               5

#endif
