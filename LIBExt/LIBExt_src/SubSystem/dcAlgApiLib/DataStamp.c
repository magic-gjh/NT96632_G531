#include "sensor_model.h"
#include "Grph.h"



typedef struct    _DATASTAMP_CTRL{
    BOOL            bEnable;
    UINT32          uiBufferAddr;
    DS_STAMP_INFOR  stmpInfor;
    UINT32          uiXPosition;
    UINT32          uiYPosition;

} DATASTAMP_CTRL, *pDATASTAMP_CTRL;

DATASTAMP_CTRL DataStampCtrlObject={0};//JJ added, 20051004, for data stamping


//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
//this zone is for external Functions

void datastamp_Imprint(UINT SrcAddr, UINT SrcLineOffset, UINT StampAddr, UINT StampLineOffset, UINT StampH, UINT StampV, UB ColorKey, UINT32 BufferAddr)
{
//#NT#2009/08/06#Hideo Lin -begin
//#NT#According to YC's modification on 2007/04/20
#if 1
    grph_open();
    grph_enableDMA();
    grph_setImg1(SrcAddr,
                 SrcLineOffset,
                 StampV,
                 StampH);
    grph_setImg2(StampAddr,
                 StampLineOffset);
    grph_setAOP(0,          //BOOL b16BitPrc
                0,          //GRPH_DST_SEL dstSel
                0x03,       //GRPH_AOP_MODE aopMode
                ColorKey);  //UINT32 uiProperty == Color Key
    grph_disableDMA();//DMA seems to keep moving unless being disabled,
    grph_swReset();
    grph_close();//mark this line or the second picture will make the cam. abnormal
#else
    int Y;
    UINT BufAddr;
//prepare buffer
    BufAddr = BufferAddr;
    for(Y=0; Y<StampV; Y++)
        memcpy((void*)(BufAddr+Y*StampH), (void*)(SrcAddr+Y*SrcLineOffset), StampH);
//step 1
    grph_open();
    grph_enableDMA();
    grph_setImg1(   BufAddr,
                    StampH,
                    StampV,
                    StampH);
    grph_setImg2(   StampAddr,
                    StampLineOffset);
    grph_setAOP(    0,          //BOOL b16BitPrc
                                0,          //GRPH_DST_SEL dstSel
                                0x03,       //GRPH_AOP_MODE aopMode
                                ColorKey);  //UINT32 uiProperty == Color Key
    grph_disableDMA();//DMA seems to keep moving unless being disabled,
    grph_swReset();
    grph_close();//mark this line or the second picture will make the cam. abnormal
//step 2
    grph_open();
    grph_enableDMA();
    grph_setImg1(   SrcAddr,
                    SrcLineOffset,
                    StampV,
                    StampH);
    grph_setImg2(   BufAddr,
                    StampH);
    grph_setAOP(    0,          //BOOL b16BitPrc
                    0,          //GRPH_DST_SEL dstSel
                    0x0D,       //GRPH_AOP_MODE aopMode
                    0x1A);  //UINT32 uiProperty == Color Key
    grph_disableDMA();//DMA seems to keep moving unless being disabled,
    grph_swReset();
    grph_close();//mark this line or the second picture will make the cam. abnormal
#endif
//#NT#2009/08/06#Hideo Lin -end
}


//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------

/**
    Enable/Disable Data-Stamp

    Enable/Disable Data-Stamp

    @param[in] bEn          Enable/Disable

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_Enable(BOOL bEn)
{
    DataStampCtrlObject.bEnable=bEn;

    return DATASTAMP_E_OK;
}

/**
    Check Data-Stamp Enable

    Check Data-Stamp Enable

    @return Enable or Disable
*/
BOOL datastamp_IsEnable(void)
{
    return DataStampCtrlObject.bEnable;
}

/**
    Set Date-Image/Stamp information

    Set Date-Image/Stamp information

    @param[in] pstmpinfor   A pointer of structure with Stamp-information

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_SetStampInfor(DS_STAMP_INFOR *pstmpinfor)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    #if 1
    memcpy(&DataStampCtrlObject,pstmpinfor, sizeof(DS_STAMP_INFOR));
    #else
    DataStampCtrlObject.stmpInfor.AddrY       = pstmpinfor->AddrY;
    DataStampCtrlObject.stmpInfor.AddrCb      = pstmpinfor->AddrCb;
    DataStampCtrlObject.stmpInfor.AddrCr      = pstmpinfor->AddrCr;
    DataStampCtrlObject.stmpInfor.LineOffset  = pstmpinfor->LineOffset;
    DataStampCtrlObject.stmpInfor.Width       = pstmpinfor->Width;
    DataStampCtrlObject.stmpInfor.Height      = pstmpinfor->Height;
    DataStampCtrlObject.stmpInfor.ColorKeyY   = pstmpinfor->ColorKeyY;
    DataStampCtrlObject.stmpInfor.ColorKeyCb  = pstmpinfor->ColorKeyCb;
    DataStampCtrlObject.stmpInfor.ColorKeyCr  = pstmpinfor->ColorKeyCr;
    DataStampCtrlObject.stmpInfor.Format      = pstmpinfor->Format;
    #endif

    if(pstmpinfor->Format == DS_YCCFORMAT_444)
    {
        if(DataStampCtrlObject.stmpInfor.LineOffset%8 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }
    else
    {
        if(DataStampCtrlObject.stmpInfor.LineOffset%4 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }

    return uiReturn;
}

/**
    Get Date-Image/Stamp information

    Get Date-Image/Stamp information

    @param[in] pstmpinfor   A pointer of structure to get Stamp-information

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_GetStampInfor(DS_STAMP_INFOR *pstmpinfor)
{
    #if 1
    memcpy(pstmpinfor, &DataStampCtrlObject, sizeof(DS_STAMP_INFOR));
    #else
    pstmpinfor->AddrY        = DataStampCtrlObject.stmpInfor.AddrY;
    pstmpinfor->AddrCb       = DataStampCtrlObject.stmpInfor.AddrCb;
    pstmpinfor->AddrCr       = DataStampCtrlObject.stmpInfor.AddrCr;
    pstmpinfor->LineOffset   = DataStampCtrlObject.stmpInfor.LineOffset;
    pstmpinfor->Width        = DataStampCtrlObject.stmpInfor.Width;
    pstmpinfor->Height       = DataStampCtrlObject.stmpInfor.Height;
    pstmpinfor->ColorKeyY    = DataStampCtrlObject.stmpInfor.ColorKeyY;
    pstmpinfor->ColorKeyCb   = DataStampCtrlObject.stmpInfor.ColorKeyCb;
    pstmpinfor->ColorKeyCr   = DataStampCtrlObject.stmpInfor.ColorKeyCr;
    pstmpinfor->Format       = DataStampCtrlObject.stmpInfor.Format;
    #endif

    return DATASTAMP_E_OK;
}

/**
    Set Date-Image/Stamp Position

    Set up where to put on stamp

    @param[in] uiXPos       X direction positioning
    @param[in] uiYPos       Y direction positioning

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_SetPosition(UINT32 uiXPos, UINT32 uiYPos)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    DataStampCtrlObject.uiXPosition = uiXPos;
    DataStampCtrlObject.uiYPosition = uiYPos;

    if(uiXPos>1024 || uiYPos>1024)
        uiReturn = DATASTAMP_E_PARAERROR;

    return uiReturn;
}

/**
    Get Date-Image/Stamp Position

    Get the information of where to put on stamp

    @param[in] puiXPos      A pointer to get X direction positioning
    @param[in] puiYPos      A pointer to get Y direction positioning

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_GetPosition(UINT32* puiXPos, UINT32* puiYPos)
{
    *puiXPos = DataStampCtrlObject.uiXPosition;
    *puiYPos = DataStampCtrlObject.uiYPosition;

    return DATASTAMP_E_OK;
}

/**
    DataStamp Bacis Imprinting API

    Imprint stamp-image into background-image

    @param[in] BackgroundInfo   A pointer of structure with background-image information
    @param[in] StampInfo        A pointer of structure with stamp-image information

    @return void
*/
void datastamp_ImprintYCC(DS_BACKGROUND_INFOR *BackgroundInfo,DS_STAMP_INFOR *StampInfo, UINT32 BufferAddr)
{
//    //YCC 422 format
//    UINT YSrcStart,CbSrcStart,CrSrcStart;
//    UINT DataStampHStartPxl,DataStampVStartPxl;
//    UINT Shift=10;

    if(BufferAddr == 0)
    {
        //Assign New Buffer
        BufferAddr = DataStampCtrlObject.uiBufferAddr;
    }

//    //Fixed data stamp coordinate same as large image
//    DataStampHStartPxl = 0;//((StampSrcInfo->SrcHsize)*Hposition)>>Shift;//StampStartRatioShift;
//    DataStampHStartPxl &= 0xfffffff8;//needed to be word-aligned for Cb,Cr
//    DataStampVStartPxl = 0;//((StampSrcInfo->SrcVsize)*Vposition)>>Shift;//StampStartRatioShift;

//    YSrcStart=(StampSrcInfo->SrcAddrY)+DataStampVStartPxl*(StampSrcInfo->SrcYLineOffset)+DataStampHStartPxl;
//    CbSrcStart=(StampSrcInfo->SrcAddrCb)+DataStampVStartPxl*(StampSrcInfo->SrcYLineOffset)/2+DataStampHStartPxl/2;
//    CrSrcStart=(StampSrcInfo->SrcAddrCr)+DataStampVStartPxl*(StampSrcInfo->SrcYLineOffset)/2+DataStampHStartPxl/2;

                //Y
                datastamp_Imprint(BackgroundInfo->AddrY,
                            BackgroundInfo->LineOffset,
                            StampInfo->AddrY,
                            StampInfo->Width,
                            StampInfo->Width,
                            StampInfo->Height,
                            StampInfo->ColorKeyY,
                            BufferAddr);
                //Cb
                datastamp_Imprint(BackgroundInfo->AddrCb,
                            (BackgroundInfo->LineOffset)/2,
                            StampInfo->AddrCb,
                            (StampInfo->Width)/2,
                            (StampInfo->Width)/2,
                            StampInfo->Height,
                            StampInfo->ColorKeyCb,
                            BufferAddr);
                //Cr
                datastamp_Imprint(BackgroundInfo->AddrCr,
                            (BackgroundInfo->LineOffset)/2,
                            StampInfo->AddrCr,
                            (StampInfo->Width)/2,
                            (StampInfo->Width)/2,
                            StampInfo->Height,
                            StampInfo->ColorKeyCr,
                            BufferAddr);
}

//#NT#2009/08/10#Hideo Lin -begin
//#NT#Date stamp for screen-nail test
DATASTAMP_CTRL DataStampCtrlObjectThumb = {0};

UINT32 datastamp_SetStampInforThumb(DS_STAMP_INFOR *pstmpinfor)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    memcpy(&DataStampCtrlObjectThumb, pstmpinfor, sizeof(DS_STAMP_INFOR));

    if(pstmpinfor->Format == DS_YCCFORMAT_444)
    {
        if(DataStampCtrlObjectThumb.stmpInfor.LineOffset%8 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }
    else
    {
        if(DataStampCtrlObjectThumb.stmpInfor.LineOffset%4 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }

    return uiReturn;
}

UINT32 datastamp_GetStampInforThumb(DS_STAMP_INFOR *pstmpinfor)
{
    memcpy(pstmpinfor, &DataStampCtrlObjectThumb, sizeof(DS_STAMP_INFOR));

    return DATASTAMP_E_OK;
}

UINT32 datastamp_SetPositionThumb(UINT32 uiXPos, UINT32 uiYPos)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    DataStampCtrlObjectThumb.uiXPosition = uiXPos;
    DataStampCtrlObjectThumb.uiYPosition = uiYPos;

    if(uiXPos>1024 || uiYPos>1024)
        uiReturn = DATASTAMP_E_PARAERROR;

    return uiReturn;
}

UINT32 datastamp_GetPositionThumb(UINT32* puiXPos, UINT32* puiYPos)
{
    *puiXPos = DataStampCtrlObjectThumb.uiXPosition;
    *puiYPos = DataStampCtrlObjectThumb.uiYPosition;

    return DATASTAMP_E_OK;
}
//#NT#2009/08/10#Hideo Lin -end
