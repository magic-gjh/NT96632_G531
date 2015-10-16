#include "sensor_model.h"
#include "ime.h"
#include "fd_lib.h"
#include "sd_lib.h"
#include "AlbumShot.h"


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
// This area is for ippbuf stuff

// Message APIs
#define ippbuf_err(msg) debug_err(msg)
#define ippbuf_wrn(msg) debug_wrn(msg)
#define ippbuf_ind(msg) debug_ind(msg)

// Static global variable/struct
//#NT#2012/11/28#KS Hung -begin
//#NT#Special application for 5M (2592x1944) UVC camera
static UINT32 g_uiPreivewSize = IME_PRV_MAX_SIZE;
//#NT#2012/11/28#KS Hung -end
static BOOL ippbuf_ResetFlag = ENABLE;
static UINT32 ippbuf_BufObject[IPPBUF_MAX_INDEX]={0};
static FPIPPBUFASSIGNER ippbuf_AssignerObject[IPPBUF_MODE_MAX_INDEX] =
{
ippbuf_SampleAssigner_Prv,
ippbuf_SampleAssigner_Prv,
ippbuf_SampleAssigner_Prv,
ippbuf_SampleAssigner_Cap_Normal,
ippbuf_SampleAssigner_Cap_AlbumShot,
ippbuf_SampleAssigner_Cap_CoupleShot,
ippbuf_SampleAssigner_Cap_PreShot
};

// Small tool
#define ippbuf_WordAlign(x) (((x)+3)/4*4)
#define IPPBUF_NULL 0xffffffff
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This area is for ippbuf external arrangement
void ippbuf_SetResetFlag(BOOL Flag)
{
    ippbuf_ResetFlag = Flag;
}

BOOL ippbuf_GetResetFlag(void)
{
    return ippbuf_ResetFlag;
}

UINT32 ippbuf_Arrange(void)
{
    UINT32 i;
    UINT32 AllocSize, MinAddr;
    UINT32 BufAddr, BufSize;

    AllocSize = 0;
    MinAddr = 0xffffffff;
    for (i = 0; i < IPPBUF_RES_MAX_INDEX; i ++)
    {
        ippbuf_GetResource(i, &BufAddr, &BufSize);
        if (BufAddr != IPPBUF_NULL)
        {
            MinAddr = (MinAddr < BufAddr) ? MinAddr : BufAddr;
            ippbuf_SetBuf(IPPBUF_TOTAL_START, MinAddr);
            AllocSize += BufSize;
        }
    }
    ippbuf_SetBuf(IPPBUF_TOTAL_END, (MinAddr + AllocSize));

    //check memory is continuous or not
    if ((ippbuf_GetBuf(IPPBUF_PRV_START) != IPPBUF_NULL) && (ippbuf_GetBuf(IPPBUF_CAP_START) != IPPBUF_NULL))
    {
        if ((ippbuf_GetBuf(IPPBUF_PRV_START) == ippbuf_GetBuf(IPPBUF_CAP_END)) ||
            (ippbuf_GetBuf(IPPBUF_CAP_START) == ippbuf_GetBuf(IPPBUF_PRV_END)))
        {
            return IPPBUF_E_OK;
        }
        ippbuf_err(("ippbuf_Arrange: continuous mode fail\r\n"));
        return IPPBUF_E_OUTOFBUFF;

    }
    return IPPBUF_E_OK;
}

/**
    ippbuf_SampleAssigner_Prv
        To allocate IPP-Buffers for Preview mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size

    Brief Explanation of PRV memory allocation
    IME_PRV_MAX_SIZE                            //Preview IME Path-1 Img output
        0x21c000
    2*Get_DzoomPrvMaxBuffer(&SIE_PRE_Para_prv)  //Preview SIE Img output
        0x1ab800
    (IPE_MAXVAWORD << 2)                        //Preview IPE VA output
        0x800
    IME_PRV_MAX_SIZE                            //IME Path-2 Img output
        0x21c000
    2*FDE_IN_MAX_HSIZE * FDE_IN_MAX_VSIZE       //Preview FDE Img output
    +(FDE_RSLT_SIZE
        + FDE_IN_MAX_HSIZE * FDE_IN_MAX_VSIZE)
        0xe2000
    TOTAL
        0x6c6000
**/
UINT32 ippbuf_SampleAssigner_Prv(UINT32 BufAddr, UINT32 BufSize)
{
    UINT32 BufSizeInNeed;
    UINT32 BuffCurrPtr, BuffCurrPtrA, BuffCurrPtrB;

    ////Initial Check
    BufSizeInNeed = 0;
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_PRV_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_PRV_END, BuffCurrPtr+BufSize);

    ////Main Assignment
    //Preview IME Path-1 Img output
    ippbuf_SetBuf(IPPBUF_PRV_IME_P1, BuffCurrPtr);
    //#NT#2012/11/28#KS Hung -begin
    //#NT#Special application for 5M (2592x1944) UVC camera
    #if 0
    BuffCurrPtr += IME_PRV_MAX_SIZE;
    #else
    BuffCurrPtr += g_uiPreivewSize;
    #endif
    //#NT#2012/11/28#KS Hung -end
    //Preview SIE Img output
    ippbuf_SetBuf(IPPBUF_PRV_RAW_1, BuffCurrPtr);
    BuffCurrPtr += Get_DzoomPrvMaxBuffer(&SIE_PRE_Para_prv);
    ippbuf_SetBuf(IPPBUF_PRV_RAW_2, BuffCurrPtr);
    BuffCurrPtr += Get_DzoomPrvMaxBuffer(&SIE_PRE_Para_prv);
    if(SIE_PRE_Para_prv.SieOutDest == OUT_PINGPONGBUF_DEST)
    {
        ippbuf_SetBuf(IPPBUF_PRV_SIERAW_1, BuffCurrPtr);
        BuffCurrPtr += Get_DzoomPrvMaxBuffer_SIE(&SIE_PRE_Para_prv);
        ippbuf_SetBuf(IPPBUF_PRV_SIERAW_2, BuffCurrPtr);
        BuffCurrPtr += Get_DzoomPrvMaxBuffer_SIE(&SIE_PRE_Para_prv);
    }

    if (Sen_GetMemMode() == SEN_MEM_COMBINE_MODE)
    {
        ippbuf_SetBuf(IPPBUF_TOTAL_OVLAP_START, BuffCurrPtr);
    }
    //Preview IPE VA output
    ippbuf_SetBuf(IPPBUF_PRV_VA, BuffCurrPtr);
    BuffCurrPtr += (IPE_MAXVAWORD << 2);

//#NT#2012/01/10#Spark Chou -begin
//#NT# For HV Flip Bug
    BuffCurrPtr+= 1024*32;
//#NT#2012/01/10#Spark Chou -end

//#NT#2009/10/16#Jarkko Chang -begin
//#NT#modify for pre ca process
    //Preview CA output
    ippbuf_SetBuf(IPPBUF_CA, BuffCurrPtr);
    BuffCurrPtr += Get_DzoomPrvMaxBuffer_SIE(&SIE_PRE_Para_prv);
//#NT#2009/10/16#Jarkko Chang -end

    //IME Path-2 Img output
    BuffCurrPtrA = BuffCurrPtr;
    ippbuf_SetBuf(IPPBUF_PRV_IME_P2, BuffCurrPtrA);
    BuffCurrPtrA = BuffCurrPtr + IME_PRV_P2_MAX_SIZE;

//BuffCurrPtr = BuffCurrPtrA;

    //#NT#2010/01/25#Connie Yu -begin
    //#NT#DIS update
    //if(gImageAlgInfo.DISMode == _DISMode_On)
//    {
//        ippbuf_SetBuf(IPPBUF_PRV_DIS_BUF, BuffCurrPtr);
//        BuffCurrPtr += Get_DISPrvMaxBuffer();
//        BuffCurrPtr += MDIS_PRV_MAX_SIZE;
//    }

    BuffCurrPtrB = BuffCurrPtr;
    //ippbuf_SetBuf(IPPBUF_PRV_FD_BUF, BuffCurrPtrB);
    //BuffCurrPtrB = BuffCurrPtr + fd_calcBuffSize();
    ippbuf_SetBuf(IPPBUF_PRV_SD_BUF, BuffCurrPtrB);
    BuffCurrPtrB += sd_calcBuffSize();
    ippbuf_SetBuf(IPPBUF_PRV_FD_BUF, BuffCurrPtrB);
    BuffCurrPtrB = BuffCurrPtr + fd_calcBuffSize();
    if ( BuffCurrPtrA > BuffCurrPtrB )
        BuffCurrPtr = BuffCurrPtrA;
    else
        BuffCurrPtr = BuffCurrPtrB;

    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);

    ippbuf_SetBuf(IPPBUF_PRV_CURREND, BuffCurrPtr);

    ////Final Check
    BufSizeInNeed += ippbuf_GetBuf(IPPBUF_PRV_CURREND) -
                     ippbuf_GetBuf(IPPBUF_PRV_START);
    if(BufSizeInNeed>BufSize)
        ippbuf_err(("^Rippbuf_SampleAssigner_Prv: Buf. not enough 0x%x<0x%x\r\n", BufSize, BufSizeInNeed));

    return BufSizeInNeed;
}

/**
    ippbuf_SampleAssigner_Cap_Normal
        To allocate IPP-Buffers for Capture mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
UINT32 ippbuf_SampleAssigner_Cap_Normal(UINT32 BufAddr, UINT32 BufSize)
{
    UINT32 i;
    UINT32 BufSizeInNeed;
    UINT32 BuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize, FQVSize, SIEOutSize;
    UINT8  ShotNum, RawNum, JpgNum;

    ////Extra Prepare
    RawNum = Sen_GetRawNum();
    JpgNum = Sen_GetJpgNum();
    ShotNum=1;

    ////Initial Check
    BufSizeInNeed = 0;
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2);
    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * ShotNum);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;

    //sie out & FQV overlap
    if ((SIE_PRE_Para_cap.SieOutDest == OUT_PINGPONGBUF_DEST) && (GetIPPFuncInfor(FQV_EN) == ENABLE))
    {
        FQVSize = ((GetIPPSizeInfor(_FQVSampleRate) & 0x1) == 0) ?
                ippbuf_WordAlign(((getCapRawSize() * 2) / GetIPPSizeInfor(_FQVSampleRate))) :
                ippbuf_WordAlign((getCapRawSize() / GetIPPSizeInfor(_FQVSampleRate)));
        SIEOutSize = Get_DzoomCapMaxFieldBuffer_SIE(&SIE_PRE_Para_cap);

        ippbuf_SetBuf(IPPBUF_CAP_SIERAW_1, BuffCurrPtr);
        ippbuf_SetBuf(IPPBUF_CAP_SIERAW_2, (BuffCurrPtr + SIEOutSize));
        ippbuf_SetBuf(IPPBUF_CAP_QV_BUF, BuffCurrPtr);

        BuffCurrPtr += (FQVSize > (SIEOutSize * 2)) ? FQVSize : (SIEOutSize * 2);
    }
    else
    {
        if(SIE_PRE_Para_cap.SieOutDest == OUT_PINGPONGBUF_DEST)
        {
            SIEOutSize = Get_DzoomCapMaxFieldBuffer_SIE(&SIE_PRE_Para_cap);
            ippbuf_SetBuf(IPPBUF_CAP_SIERAW_1, BuffCurrPtr);
            BuffCurrPtr += SIEOutSize;
            ippbuf_SetBuf(IPPBUF_CAP_SIERAW_2, BuffCurrPtr);
            BuffCurrPtr += SIEOutSize;
        }

        if (GetIPPFuncInfor(FQV_EN) == ENABLE)
        {
            ippbuf_SetBuf(IPPBUF_CAP_QV_BUF, BuffCurrPtr);

            FQVSize = ((GetIPPSizeInfor(_FQVSampleRate) & 0x1) == 0) ?
                    ippbuf_WordAlign(((getCapRawSize() * 2) / GetIPPSizeInfor(_FQVSampleRate))) :
                    ippbuf_WordAlign((getCapRawSize() / GetIPPSizeInfor(_FQVSampleRate)));
            BuffCurrPtr += FQVSize;
        }
    }

    //Capture Raw & dark frame buffer
    for (i = 0; i < RawNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_RAW_1 + i, BuffCurrPtr);
        BuffCurrPtr += getCapRawSize();
    }

    ippbuf_SetBuf(IPPBUF_CAP_FRAME_Y, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * (GetIPPSizeInfor(_ImeCapOutVsize)));
    ippbuf_SetBuf(IPPBUF_CAP_FRAME_CB, BuffCurrPtr);
    BuffCurrPtr += ((GetIPPSizeInfor(_ImeCapOutHsize) * (GetIPPSizeInfor(_ImeCapOutVsize))) >> 1);
    ippbuf_SetBuf(IPPBUF_CAP_FRAME_CR, BuffCurrPtr);
    BuffCurrPtr += ((GetIPPSizeInfor(_ImeCapOutHsize) * (GetIPPSizeInfor(_ImeCapOutVsize))) >> 1);

    ippbuf_SetBuf(IPPBUF_CAP_RAW_GEOMETRIC, BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_RAW_DARKFRAME, BuffCurrPtr);
    //Capture Jpg buffer
    if(ippbuf_GetBuf(IPPBUF_CAP_END)>BuffCurrPtr)
    {
        BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
        {
            UINT32 tmp;
            tmp = (BuffRemainSize / JpgNum);
            if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
            {
                tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
            }
            else
                tmp = 0;
            tmp -= (tmp % 4);
            JpgBitstreamSize = tmp;
            SetJpgBufSize(JpgBitstreamSize);
        }
    }
    else
    {
        JpgBitstreamSize = 0;// zero is not working, but let's make the normal error-handle deal with this shortage
        SetJpgBufSize(0);
    }
    if(JpgBitstreamSize<CAP_JPG_BSTM_MIN_SIZE)
    {
        ippbuf_err(("ippbuf_SampleAssigner_Cap_Normal: JpgBitStremSize 0x%x<0x%x\r\n", JpgBitstreamSize, CAP_JPG_BSTM_MIN_SIZE));
    }
    for (i = 0; i < JpgNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr+JpgHeaderSize);
        BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
    }

    ////Regular Assign, CURREND
    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    ////Final Check
    BufSizeInNeed += ippbuf_GetBuf(IPPBUF_CAP_CURREND) -
                     ippbuf_GetBuf(IPPBUF_CAP_START);
    if(BufSizeInNeed>BufSize)
        ippbuf_err(("ippbuf_SampleAssigner_Cap_Normal: Buf. not enough 0x%x<0x%x\r\n", BufSize, BufSizeInNeed));

    return BufSizeInNeed;
}

/**
    ippbuf_SampleAssigner_Cap_AlbumShot
        To allocate IPP-Buffers for Capture mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
UINT32 ippbuf_SampleAssigner_Cap_AlbumShot(UINT32 BufAddr, UINT32 BufSize)
{
    UINT32 BufSizeInNeed;
    UINT32 BuffCurrPtr, BuffCurrPtrTmp;
    UINT32 Numerator, Denominator;

    ////Initial Check
    BufSizeInNeed = 0;
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    //Capture Raw buffer
    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);

    ASGetBufRatio(&Numerator, &Denominator);
    if (Get_DzoomCapMaxBuffer(&SIE_PRE_Para_prv) > (GetIPPSizeInfor(_ImeCapOutHsize) * GetIPPSizeInfor(_ImeCapOutVsize) * Numerator / Denominator))
    {
        BuffCurrPtr += Get_DzoomCapMaxBuffer(&SIE_PRE_Para_prv);
    }
    else
    {
        BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * GetIPPSizeInfor(_ImeCapOutVsize) * Numerator / Denominator);
    }

    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * 1);

    BuffCurrPtrTmp = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtrTmp);
    BuffCurrPtrTmp += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * 1);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtrTmp);
    BuffCurrPtrTmp += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * 1);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtrTmp);
    BuffCurrPtrTmp += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;

    ippbuf_SetBuf(IPPBUF_CAP_JPG_1, BuffCurrPtr);
    if(ippbuf_GetBuf(IPPBUF_CAP_END) > BuffCurrPtr)
    SetJpgBufSize(ippbuf_GetBuf(IPPBUF_CAP_END) - BuffCurrPtr);
    else
        SetJpgBufSize(0);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, ippbuf_GetBuf(IPPBUF_CAP_END));

    ////Final Check
    BufSizeInNeed += ippbuf_GetBuf(IPPBUF_CAP_CURREND) -
                     ippbuf_GetBuf(IPPBUF_CAP_START);
    if(ippbuf_GetBuf(IPPBUF_CAP_JPG_1)>ippbuf_GetBuf(IPPBUF_CAP_END))
    {
        ippbuf_err(("ippbuf_SampleAssigner_Cap_AlbumShot: Buf. not enough 0x%x<0x%x\r\n", ippbuf_GetBuf(IPPBUF_CAP_JPG_1), ippbuf_GetBuf(IPPBUF_CAP_END)));
    }

    return BufSizeInNeed;
}

/**
    ippbuf_SampleAssigner_Cap_CoupleShot
        To allocate IPP-Buffers for Capture couple shot mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
UINT32 ippbuf_SampleAssigner_Cap_CoupleShot(UINT32 BufAddr, UINT32 BufSize)
{
    UINT32 i;
    UINT32 BufSizeInNeed;
    UINT32 BuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize;
    UINT8  ShotNum, RawNum, JpgNum;

    ////Extra Prepare
    RawNum = Sen_GetRawNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    JpgNum = Sen_GetJpgNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    ShotNum=1;


    ////Initial Check
    BufSizeInNeed = 0;
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2);
    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * ShotNum);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;

    //Couple shot preview buffer
    ippbuf_SetBuf(IPPBUF_CAP_COUPLESHOTPRV,BuffCurrPtr);
    BuffCurrPtr += 2*GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize);

    //Capture Raw
    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);
    BuffCurrPtr += getCapRawSize();
    ippbuf_SetBuf(IPPBUF_CAP_RAW_2, BuffCurrPtr);

    ippbuf_SetBuf(IPPBUF_CAP_RAW_GEOMETRIC, BuffCurrPtr);

    //Capture Jpg buffer
    if(ippbuf_GetBuf(IPPBUF_CAP_END)>BuffCurrPtr)
    {
        BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
        {
            UINT32 tmp;
            tmp = (BuffRemainSize / JpgNum);
            if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
            {
                tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
            }
            else
                tmp = 0;
            tmp -= (tmp % 4);
            JpgBitstreamSize = tmp;
            SetJpgBufSize(JpgBitstreamSize);
        }
    }
    else
    {
        JpgBitstreamSize = 0;// zero is not working, but let's make the normal error-handle deal with this shortage
        SetJpgBufSize(0);
    }
    if(JpgBitstreamSize<CAP_JPG_BSTM_MIN_SIZE)
    {
        ippbuf_err(("ippbuf_SampleAssigner_Cap_Normal: JpgBitStremSize 0x%x<0x%x\r\n", JpgBitstreamSize, CAP_JPG_BSTM_MIN_SIZE));
    }

    for (i = 0; i < JpgNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr+JpgHeaderSize);
        BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
    }

    ////Regular Assign, CURREND
    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    ////Final Check
    BufSizeInNeed += ippbuf_GetBuf(IPPBUF_CAP_CURREND) -
                     ippbuf_GetBuf(IPPBUF_CAP_START);
    if(BufSizeInNeed>BufSize)
        ippbuf_err(("ippbuf_SampleAssigner_Cap_CoupleShot: Buf. not enough 0x%x<0x%x\r\n", BufSize, BufSizeInNeed));

    return BufSizeInNeed;
}

//#NT#for preshot function
/**
    ippbuf_SampleAssigner_Cap_PreShot
        To allocate IPP-Buffers for Capture preshot mode
    BufAddr:    Buffer Start addr
    BufSize:    Buffer Size
**/
UINT32 ippbuf_SampleAssigner_Cap_PreShot(UINT32 BufAddr, UINT32 BufSize)
{
    UINT32 i;
    UINT32 BufSizeInNeed;
    UINT32 BuffCurrPtr;
    UINT32 BuffRemainSize;
    UINT32 JpgBitstreamSize;
    UINT8  ShotNum, RawNum, JpgNum;

    ////Extra Prepare
    RawNum = Sen_GetRawNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    JpgNum = Sen_GetJpgNum();//seperated files(dependent ones and common ones), but mutual-reference variables(temporal solution)
    ShotNum=1;


    ////Initial Check
    BufSizeInNeed = 0;
    BuffCurrPtr = BufAddr;

    ////Regular Assign, START and END
    //Total Start pointer
    ippbuf_SetBuf(IPPBUF_CAP_START, BuffCurrPtr);
    //Total End pointer
    ippbuf_SetBuf(IPPBUF_CAP_END, BuffCurrPtr+BufSize);

    ////Main Assignment
    //Capture Chroma-Filter Buffer
    ippbuf_SetBuf(IPPBUF_CAP_CHROMA_FILTER, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * CHROMA_BUF_NUM / 2);
    //Capture IME path-1 Output (Slice-Buffer)
    ippbuf_SetBuf(IPPBUF_CAP_SLC_BUF, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2);
    //Capture IME path-2 Output (Small Image, which would be scaled again as QV img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_PRE, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVPreHsize) * GetIPPSizeInfor(_QVPreVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Display img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_DISPLAY, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_QVHsize) * GetIPPSizeInfor(_QVVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (Screenail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_SCREEN, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ScreenHsize) * GetIPPSizeInfor(_ScreenVsize) * 2 * ShotNum);
    //Capture IME QV Image Buffer (thumbnail img)
    ippbuf_SetBuf(IPPBUF_CAP_QV_THUMBNAIL, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ThumbHsize) * GetIPPSizeInfor(_ThumbVsize) * 2 * ShotNum);
    //Capture Data Stamp Buffer
    ippbuf_SetBuf(IPPBUF_CAP_DATA_STAMP, BuffCurrPtr);
    BuffCurrPtr += (GetIPPSizeInfor(_ImeCapOutHsize) * ImeSliceModeToVSize(GetIPPSizeInfor(_CapIMESliceMode)) * SLICE_BUF_NUM * 2)/6;

    //Couple shot preview buffer
    ippbuf_SetBuf(IPPBUF_CAP_PRESHOTPRV,BuffCurrPtr);
    BuffCurrPtr += 2*GetIPPSizeInfor(_ImeOut1Hsize)*GetIPPSizeInfor(_ImeOut1Vsize);

    //Capture Raw
    ippbuf_SetBuf(IPPBUF_CAP_RAW_1, BuffCurrPtr);
    BuffCurrPtr += getCapRawSize();

    ippbuf_SetBuf(IPPBUF_CAP_RAW_GEOMETRIC, BuffCurrPtr);

    //Capture Jpg buffer
    if(ippbuf_GetBuf(IPPBUF_CAP_END)>BuffCurrPtr)
    {
        BuffRemainSize = ippbuf_GetBuf(IPPBUF_CAP_END)-BuffCurrPtr;
        {
            UINT32 tmp;
            tmp = (BuffRemainSize / JpgNum);
            if(tmp > (JpgHeaderSize + CAP_DEBUGINFO_SIZE))
            {
                tmp = tmp - (JpgHeaderSize + CAP_DEBUGINFO_SIZE);
            }
            else
                tmp = 0;
            tmp -= (tmp % 4);
            JpgBitstreamSize = tmp;
            SetJpgBufSize(JpgBitstreamSize);
        }
    }
    else
    {
        JpgBitstreamSize = 0;// zero is not working, but let's make the normal error-handle deal with this shortage
        SetJpgBufSize(0);
    }
    if(JpgBitstreamSize<CAP_JPG_BSTM_MIN_SIZE)
    {
        ippbuf_err(("ippbuf_SampleAssigner_Cap_Normal: JpgBitStremSize 0x%x<0x%x\r\n", JpgBitstreamSize, CAP_JPG_BSTM_MIN_SIZE));
    }

    for (i = 0; i < JpgNum; i ++)
    {
        ippbuf_SetBuf(IPPBUF_CAP_JPG_1+ i, BuffCurrPtr+JpgHeaderSize);
        BuffCurrPtr += (JpgBitstreamSize+(JpgHeaderSize + CAP_DEBUGINFO_SIZE));
    }

    ////Regular Assign, CURREND
    //Current End pointer
    BuffCurrPtr = ippbuf_WordAlign(BuffCurrPtr);
    ippbuf_SetBuf(IPPBUF_CAP_CURREND, BuffCurrPtr);

    ////Final Check
    BufSizeInNeed += ippbuf_GetBuf(IPPBUF_CAP_CURREND) -
                     ippbuf_GetBuf(IPPBUF_CAP_START);
    if(BufSizeInNeed>BufSize)
        ippbuf_err(("ippbuf_SampleAssigner_Cap_CoupleShot: Buf. not enough 0x%x<0x%x\r\n", BufSize, BufSizeInNeed));

    return BufSizeInNeed;
}

/**
    Execute IPP Buffer Assigner

    Assign IPP buffers for certain mode

    @param[in] modeIdx      mode index

    @return
        - @b BufferSize In Need, if return
        - @b Otherwise: error
*/
UINT32 ippbuf_ExecuteAssigner(IPPBUF_MODE modeIdx)
{
    UINT32 BufSizeInNeed, BufAddr, BufSize;
    UINT32 uiReturn=IPPBUF_E_OK;

    if (modeIdx > IPPBUF_MODE_MAX_INDEX)
    {
        ippbuf_err(("ippbuf_ExecuteAssigner: out of index\r\n"));
        uiReturn = IPPBUF_E_NOINDEX;
    }
    else
    {
        if (Sen_GetMemMode() == SEN_MEM_COMBINE_MODE)
        {
            if (modeIdx >= IPPBUF_MODE_CAP_NORMAL)
            {
                BufAddr = ippbuf_GetBuf(IPPBUF_TOTAL_OVLAP_START);
            }
            else
            {
                BufAddr = ippbuf_GetBuf(IPPBUF_TOTAL_START);
            }
            BufSize = ippbuf_GetBuf(IPPBUF_TOTAL_END) - BufAddr;
        }
        else
        {
            if (modeIdx >= IPPBUF_MODE_CAP_NORMAL)
            {
                ippbuf_GetResource(IPPBUF_RES_CAP, &BufAddr, &BufSize);
            }
            else
            {
                ippbuf_GetResource(IPPBUF_RES_PRV, &BufAddr, &BufSize);
            }
        }

        BufSizeInNeed = (ippbuf_AssignerObject[modeIdx])(BufAddr, BufSize);

        if(BufSizeInNeed > BufSize)
            uiReturn = IPPBUF_E_OUTOFBUFF;
    }
    return uiReturn;
}

/**
    Set Memory Resource

    Set memory resources for certain mode

    @param[in] ResIdx       resource index
    @param[in] BufAddr      memory resource start-ptr
    @param[in] BufSize      memory resource start-ptr

    @return
        - @b IPPBUF_E_OK:   operation sucessful
        - @b Otherwise:     error
*/
UINT32 ippbuf_SetResource(IPPBUF_RESOURCE ResIdx, UINT32 BufAddr, UINT32 BufSize)
{
    //after executing the Photo_Close or ImgCapture_Close function,
    //we must be reset ipp buffer for memory continuous mode.
    if (ippbuf_GetResetFlag() == ENABLE)
    {
        ippbuf_SetResetFlag(DISABLE);
        ippbuf_SetBuf(IPPBUF_CAP_START, IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_CAP_END,   IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_PRV_START, IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_PRV_END,   IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_TOTAL_START, IPPBUF_NULL);
        ippbuf_SetBuf(IPPBUF_TOTAL_END,   IPPBUF_NULL);
    }

    //assigned resource
    ippbuf_ind(("ippbuf_SetResource Mod%02d 0x%x 0x%x\r\n", ResIdx, BufAddr, BufSize));
    switch(ResIdx)
    {
        case IPPBUF_RES_CAP:
            ippbuf_SetBuf(IPPBUF_CAP_START, BufAddr);
            ippbuf_SetBuf(IPPBUF_CAP_END,   BufAddr+BufSize);
            break;

        case IPPBUF_RES_PRV:
            ippbuf_SetBuf(IPPBUF_PRV_START, BufAddr);
            ippbuf_SetBuf(IPPBUF_PRV_END,   BufAddr+BufSize);
            break;

        default:
            debug_err(("ippbuf_SetResource: out of index\r\n"));
            return IPPBUF_E_NOINDEX;
    }

    if (Sen_GetMemMode() == SEN_MEM_COMBINE_MODE)
    {
        ippbuf_Arrange();
    }
    return IPPBUF_E_OK;
}

/**
    Get Memory Resource

    Get memory resources

    @param[in] ResIdx       resource index
    @param[out] BufAddr      memory resource start-ptr
    @param[out] BufSize      memory resource start-ptr

    @return
        - @b IPPBUF_E_OK:   operation sucessful
        - @b Otherwise:     error
*/
UINT32 ippbuf_GetResource(IPPBUF_RESOURCE ResIdx, UINT32 *BufAddr, UINT32 *BufSize)
{

    switch(ResIdx)
    {
        case IPPBUF_RES_CAP:
            *BufAddr = ippbuf_GetBuf(IPPBUF_CAP_START);
            *BufSize = (ippbuf_GetBuf(IPPBUF_CAP_END) - *BufAddr);
            break;

        case IPPBUF_RES_PRV:
            *BufAddr = ippbuf_GetBuf(IPPBUF_PRV_START);
            *BufSize = (ippbuf_GetBuf(IPPBUF_PRV_END) - *BufAddr);
            break;

        default:
            *BufAddr = IPPBUF_NULL;
            *BufSize = 0;
            debug_err(("ippbuf_SetResource: out of index\r\n"));
            return IPPBUF_E_NOINDEX;
    }
    return IPPBUF_E_OK;
}

//#NT#IPPBuffer API Intergration080717
/**
    Register Customer Assigner

    Register Assigner for certain mode

    @param[in] modeIdx          mode index
    @param[in] pIPPBufAssigner  customer Assigner

    @return
        - @b IPPBUF_E_OK: Operation successful
        - @b Otherwise: Operation failed
*/
UINT32 ippbuf_RegisterAssigner(IPPBUF_MODE modeIdx, FPIPPBUFASSIGNER pIPPBufAssigner)
{
    ippbuf_ind(("ippbuf_RegisterAssigner FPIPPBUFASSIGNER %d 0x%08x\r\n", modeIdx, pIPPBufAssigner));
    if(modeIdx < IPPBUF_MODE_MAX_INDEX)
    {
        (ippbuf_AssignerObject[modeIdx]) = pIPPBufAssigner;
    }
    else
    {
        ippbuf_err(("ippbuf_RegisterAssigner: out of index\r\n"));
        return IPPBUF_E_NOINDEX;
    }

    return IPPBUF_E_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This area is for ippbuf - APIs

//#NT#2012/11/28#KS Hung -begin
//#NT#Special application for 5M (2592x1944) UVC camera
/**
    Set IPP Preview Size Buffer

    Set certain IPP Preview Size buffer

    @param[in] uiPreviewSize   buffer size

    @return
*/
void ippbuf_SetPreviewSizeBuf(UINT uiPreviewSize)
{
    g_uiPreivewSize = uiPreviewSize;
}


/**
    Get IPP Preview Size Buffer

    Get certain IPP Preview Size buffer

    @param[in]

    @return
*/
UINT32 ippbuf_GetPreviewSizeBuf(void)
{
    return g_uiPreivewSize;
}
//#NT#2012/11/28#KS Hung -end

/**
    Get IPP Buffer

    Set certain IPP buffer

    @param[in] BufferIdx    buffer index
    @param[in] setValue     value to set

    @return
        - @b IPPBUF_E_OK:   operation sucessful
        - @b Otherwise:     error
*/
UINT32 ippbuf_SetBuf(IPPBUF_BUFFER BufferIdx, UINT32 setValue)
{
    ippbuf_ind(("ippbuf_SetBuf Idx:%02d  0x%08x \r\n", BufferIdx, setValue));
    if(BufferIdx < IPPBUF_MAX_INDEX)
    {
        ippbuf_BufObject[BufferIdx] = setValue;
        return IPPBUF_E_OK;
    }
    else
    {
        ippbuf_err(("ippbuf_SetBuf: out of index\r\n"));
        return IPPBUF_E_NOINDEX;
    }
}

/**
    Get IPP Buffer

    Set certain IPP buffer

    @param[in] BufferIdx    buffer index
    @param[in] setValue     value to set

    @return
        - @b Buffer Start-ptr
        - @b IPPBUF_E_NOINDEX(=1): index out of range
*/
UINT32 ippbuf_GetBuf(IPPBUF_BUFFER BufferIdx)
{
    if(BufferIdx < IPPBUF_MAX_INDEX)
    {
        return ippbuf_BufObject[BufferIdx];
    }
    else
    {
        ippbuf_err(("ippbuf_GetBuf: out of index\r\n"));
        return IPPBUF_E_NOINDEX;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
