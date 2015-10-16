
#ifndef _PHOTODISPLAYTSK_H
#define _PHOTODISPLAYTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "appcommon.h"
#include "Lens.h"
#include "Ide.h"

//
//FLG_ID_PHOTODISPLAY
//
#define FLGPHOTODISPLAY_IDLE                FLGPTN_BIT(0)

#define FLGPHOTODISPLAY_VIDEO1ON            FLGPTN_BIT(1)
#define FLGPHOTODISPLAY_VIDEO1OFF           FLGPTN_BIT(2)
#define FLGPHOTODISPLAY_VIDEO2ON            FLGPTN_BIT(3)
#define FLGPHOTODISPLAY_VIDEO2OFF           FLGPTN_BIT(4)
#define FLGPHOTODISPLAY_CHGSIZE             FLGPTN_BIT(5)
#define FLGPHOTODISPLAY_PREVIEW             FLGPTN_BIT(6)
#define FLGPHOTODISPLAY_VIDEO               FLGPTN_BIT(7)
#define FLGPHOTODISPLAY_VIDEOREC            FLGPTN_BIT(8)
//#NT#2010/10/27#Connie Yu -begin
//#NT#movie DIS update
#define FLGPHOTODISPLAY_DISVIEW             FLGPTN_BIT(12)
//#define FLGPHOTODISPLAY_DISREADY            FLGPTN_BIT(13)
//#define FLGPHOTODISPLAY_DISSTOP             FLGPTN_BIT(14)
//#NT#2010/10/27#Connie Yu -end

typedef struct _PHOTODISPLAY_APPOBJ{
    UINT32  uiMemoryAddr;
    UINT32  uiMemorySize;
    void    (*CallBackFunc)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
    UINT32  uiDisplayFBWidth;
    UINT32  uiDisplayFBHeight;
    UINT32  uiDisplayOutWidth;
    UINT32  uiDisplayOutHeight;
} PHOTODISPLAY_APPOBJ,*PPHOTODISPLAY_APPOBJ;

typedef enum
{
    DISPLAY_MODE_PREVIEW = 0,
    DISPLAY_MODE_VIDEO = 1,
    DISPLAY_MODE_VIDEOREC = 2,

    //#NT#2010/01/22#Connie Yu -begin
    //#NT#DIS Update
    DISPLAY_MODE_DISVIEW = 3,
    DISPLAY_MODE_MAX
    //#NT#2010/01/22#Connie Yu -end
}   DISPLAY_MODE;


typedef enum
{
    _DisplayInHsize,
    _DisplayInVsize,
    _DisplayInLineoffset,
    _DisplayOutHsize,
    _DisplayOutVsize

}   _IdeSize_enum;

typedef struct  _DisplaySizeInfor
{
    UINT    DisplayInHsize;
    UINT    DisplayInVsize;
    UINT    DisplayInLineoffset;
    UINT    DisplayOutHsize;
    UINT    DisplayOutVsize;

}   DisplaySizeInfor;

typedef struct _DisplayIdeParam
{
    IDE_VIDEOID VideoID;
    VIDEO_BUF_ADDR FbAddr;
    VIDEO_BUF_ATTR FbAttr;
    VOSD_WINDOW_ATTR FbWin;

}   DisplayIdeParam;

//-----------------------------------------------------------------------
// function prototype description
//-----------------------------------------------------------------------
extern ER   PhotoDisplay_Open(PPHOTODISPLAY_APPOBJ pPhotoDisplayObj);
extern ER   PhotoDisplay_Close(void);
extern BOOL PhotoDisplay_WaitIdle(BOOL bWaitInfinite);
extern void PhotoDisplay_SetMode(DISPLAY_MODE Mode, BOOL bWaitInfinite);
extern void PhotoDisplay_ChgSize(DisplaySizeInfor* SizeInfor, BOOL bWaitInfinite);
extern void PhotoDisplay_EnableVideo(IDE_VIDEOID VideoID, BOOL bWaitInfinite);
extern void PhotoDisplay_DisableVideo(IDE_VIDEOID VideoID, BOOL bWaitInfinite);
extern UINT PhotoDisplay_SetDisplaySizeInfor(UINT item, UINT value);
extern UINT PhotoDisplay_GetDisplaySizeInfor(UINT item);
extern void PhotoDisplay_IdeConfig(DisplayIdeParam* IdeParam);
//#NT#2010/10/27#Connie Yu -begin
//#NT#movie DIS update
extern void PhotoDisplay_chgDisViewSz(BOOL bWaitInfinite);
//#NT#2010/10/27#Connie Yu -end

#endif // _PHOTODISPLAYTSK_H