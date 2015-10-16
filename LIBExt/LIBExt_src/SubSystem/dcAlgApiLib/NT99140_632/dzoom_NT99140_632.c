//*****************************************************************************
//     Program        :    Digitaql Zoom for NT99140
//     Author        :
//    Goal        :    Setup various digital zoom
//     Functions    :
//*****************************************************************************
#include "sensor_model.h"

#if NT99140_632
static UINT16 MAX_Dzoom_Step = 30;
static UINT16 MaxDzTblCnt;
//const UINT16 usImgRatioTable[_Ratio_Num][_Ratio_Num] =
//{
   // {IMG_RATIO_UINT,    228,                192},
   // {IMG_RATIO_UINT,    IMG_RATIO_UINT,     216},
  //  {IMG_RATIO_UINT,    IMG_RATIO_UINT,     IMG_RATIO_UINT}
//};

UINT16 VDOZOOM_INFOR_TABLE[21][8] =
{
    //sie_in        pre_out         ipe_out          crop size
{1280,  720, 1280,  720, 1264,  720, 1280, 720},
{1200,  672, 1200,  672, 1184,  672, 1200, 672},
{1120,  628, 1120,  628, 1104,  628, 1120, 628},
{1040,  584, 1040,  584, 1024,  584, 1040, 584},
{ 976,  544,  976,  544,  960,  544,  976, 544},
{ 912,  508,  912,  508,  896,  508,  912, 508},
{ 848,  476,  848,  476,  832,  476,  848, 476},
{ 784,  444,  784,  444,  768,  444,  784, 444},
{ 736,  412,  736,  412,  720,  412,  736, 412},
{ 688,  384,  688,  384,  672,  384,  688, 384},
{ 640,  360,  640,  360,  624,  360,  640,  360},
{ 592,  336,  592,  336,  576,  336,  592,  336},
{ 560,  312,  560,  312,  544,  312,  560,  312},
{ 512,  292,  512,  292,  496,  292,  512,  292},
{ 480,  272,  480,  272,  464,  272,  480,  272},
{ 448,  256,  448,  256,  432,  256,  448,  256},
{ 416,  236,  416,  236,  400,  236,  416,  236},
{ 400,  220,  400,  220,  384,  220,  400,  220},
{ 368,  208,  368,  208,  352,  208,  368,  208},
{ 336,  192,  336,  192,  320,  192,  336,  192},
{ 320,  180,  320,  180,  304,  180,  320,  180},
};

UINT16 VDOZOOM_INFOR_MODE_2_TABLE[21][8] =
{
    //sie_in        pre_out         ipe_out          crop size
{1280,    720,    1280,    720,   1264,    720,    1280,     720},
{1316,    720,    1296,    720,   1280,    720,    3408,    2563},
{1228,    672,    1216,    672,   1200,    672,    3184,    2401},
{1148,    628,    1136,    628,   1120,    628,    2976,    2249},
{1068,    584,    1056,    584,   1040,    584,    2784,    2107},
{1000,    544,    992 ,    544,   976 ,    544,    2608,    1974},
{936 ,    508,    928 ,    508,   912 ,    508,    2432,    1849},
{876 ,    472,    864 ,    472,   848 ,    472,    2272,    1732},
{816 ,    440,    816 ,    440,   800 ,    440,    2128,    1622},
{760 ,    408,    752 ,    408,   736 ,    408,    1984,    1519},
{708 ,    380,    704 ,    380,   688 ,    380,    1856,    1423},
{660 ,    352,    656 ,    352,   640 ,    352,    1728,    1333},
{616 ,    328,    608 ,    328,   592 ,    328,    1616,    1249},
{576 ,    304,    576 ,    304,   560 ,    304,    1504,    1170},
{536 ,    284,    528 ,    284,   512 ,    284,    1408,    1096},
{500 ,    264,    496 ,    264,   480 ,    264,    1312,    1026},
{468 ,    244,    464 ,    244,   448 ,    244,    1216,    961 },
{436 ,    228,    432 ,    228,   416 ,    228,    1136,    900 },
{408 ,    212,    400 ,    212,   384 ,    212,    1056,    843 },
{380 ,    196,    368 ,    196,   352 ,    196,    976 ,    789 },
{352 ,    180,    352 ,    180,   336 ,    180,    912 ,    739 },
};

UINT16 VDOZOOM_INFOR_MODE_3_TABLE[21][8] =
{
    //sie_in        pre_out         ipe_out          crop size
{1280,    720,    1280,    720,   1264,    720,    1280,    720},
{1684,   1288,    1456,   1288,   1440,   1288,    3408,    2563},
{1576,   1204,    1456,   1204,   1440,   1204,    3184,    2401},
{1472,   1124,    1456,   1124,   1440,   1124,    2976,    2249},
{1376,   1052,    1376,   1052,   1360,   1052,    2784,    2107},
{1284,   984 ,    1280,   984 ,   1264,   984 ,    2608,    1974},
{1200,   920 ,    1200,   920 ,   1184,   920 ,    2432,    1849},
{1120,   860 ,    1120,   860 ,   1104,   860 ,    2272,    1732},
{1048,   804 ,    1040,   804 ,   1024,   804 ,    2128,    1622},
{980 ,   752 ,    976 ,   752 ,   960 ,   752 ,    1984,    1519},
{916 ,   700 ,    912 ,   700 ,   896 ,   700 ,    1856,    1423},
{856 ,   652 ,    848 ,   652 ,   832 ,   652 ,    1728,    1333},
{800 ,   608 ,    800 ,   608 ,   784 ,   608 ,    1616,    1249},
{748 ,   568 ,    736 ,   568 ,   720 ,   568 ,    1504,    1170},
{700 ,   528 ,    688 ,   528 ,   672 ,   528 ,    1408,    1096},
{652 ,   492 ,    640 ,   492 ,   624 ,   492 ,    1312,    1026},
{608 ,   460 ,    608 ,   460 ,   592 ,   460 ,    1216,    961 },
{568 ,   428 ,    560 ,   428 ,   544 ,   428 ,    1136,    900 },
{528 ,   400 ,    528 ,   400 ,   512 ,   400 ,    1056,    843 },
{492 ,   372 ,    480 ,   372 ,   464 ,   372 ,    976 ,    789 },
{460 ,   348 ,    448 ,   348 ,   432 ,   348 ,    912 ,    739 },
};

DZOOM_INFOR VDOZOOM_INFOR = {_Ratio_16_9, VDOZOOM_INFOR_TABLE};
DZOOM_INFOR VDOZOOM_INFOR_MODE_2 = {_Ratio_16_9, VDOZOOM_INFOR_MODE_2_TABLE};
DZOOM_INFOR VDOZOOM_INFOR_MODE_3 = {_Ratio_16_9, VDOZOOM_INFOR_MODE_3_TABLE};

INT zoom_index = 10;
SIZE_INFOR    LCD_Size, JPEG_Pre_Size,JPEG_Cap_Size;
SIZE_INFOR    IPE_Out_Prv_Size, IPE_Out_Cap_Size;
SIZE_INFOR    PRE_Hsub_Prv_Size, PRE_Hsub_Cap_Size;
SIZE_INFOR    IME_Out_Prv_Size, IME_Out_Cap_Size;
SIZE_INFOR    PRE_Crop_Prv_Size, PRE_Crop_Cap_Size;

void Set_DzoomIndex(INT idx)
{
    zoom_index = idx;
}
INT Get_DzoomIndex(void)
{
    return zoom_index;
}
void Set_DzoomMaxIndex(UINT16 index)
{
    if(index >  MaxDzTblCnt)
    {
        MAX_Dzoom_Step = MaxDzTblCnt;
        debug_err(("Out of Dzoom table range, force max to:%d\r\n",MaxDzTblCnt));
    }
    else
        MAX_Dzoom_Step = index;
}

UINT16 Get_DzoomMaxIndex(void)
{
    if(MAX_Dzoom_Step!=0)
        return MAX_Dzoom_Step;
    else
        return MaxDzTblCnt;
}


//#NT#20090912#ethanlau -begin
//add mode
DZOOM_INFOR* Get_DZoomInfor(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    DZOOM_INFOR *pTableInfor;

    pTableInfor = &VDOZOOM_INFOR;
    switch(SIE_PRE_PARAM->VGAType)
    {
        case SENSOR_OUT_TWO_LINE:
        case SENSOR_OUT_THREE_LINE:
        default:
            if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_2)
            {
                pTableInfor = &VDOZOOM_INFOR_MODE_2;
            }
            else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_3)
            {
                pTableInfor = &VDOZOOM_INFOR_MODE_3;
            }
            else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE)
            {
                pTableInfor = &VDOZOOM_INFOR;
            }
            return pTableInfor;
    }
}

UINT16 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx)
{
    return usImgRatioTable[ImgRatioIdx][DefaultRatioIdx];
}

UINT16 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx)
{
    return usImgRatioTable[DefaultRatioIdx][ImgRatioIdx];
}

UINT32 Adj_VSize(UINT32 InSize, UINT16 VRatio, UINT16 Align)
{
    UINT32 rtValue;

    rtValue = InSize * VRatio / IMG_RATIO_UINT;
    rtValue = (rtValue + (Align - 1)) / Align * Align;
    return rtValue;
}

UINT32 Adj_HSize(UINT32 InSize, UINT16 HRatio, UINT16 Align)
{
    UINT32 rtValue;

    rtValue = InSize;
    rtValue = InSize * HRatio / IMG_RATIO_UINT;
    rtValue = (rtValue + (Align - 1)) / Align * Align;
    return rtValue;
}

UINT32 Adj_PreLineOffset(UINT32 LineOffset, PRE_VGAFormatEnum VGAType, UINT8 PreBitDepth)
{
    UINT32 lofs;

    lofs = LineOffset * PreBitDepth / 8;
    if (VGAType == SENSOR_OUT_THREE_LINE)
    {
        lofs = (lofs + (PRE_BLOCK_BUF_SIZE/3 - 1)) / (PRE_BLOCK_BUF_SIZE/3) * (PRE_BLOCK_BUF_SIZE/3);
    }
    else if (VGAType == SENSOR_OUT_TWO_LINE)
    {
        lofs = (lofs + (PRE_BLOCK_BUF_SIZE/2 - 1)) / (PRE_BLOCK_BUF_SIZE/2) * (PRE_BLOCK_BUF_SIZE/2);
    }
    return lofs;
}
//#NT#20090921#ethanlau -begin
void Set_digitalzoom(UINT DzoomIdx, struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 lofs, i, idx, AdjSize;
    DZOOM_INFOR *pTableInfor;
    UINT16 (*DzTable)[8], HRatio, VRatio, AdjTable[1][8];

    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;
    //#NT#2010/09/09#ethanlau -begin
    //avoid dzoom max index error causeed by dzoom talbe max size different between different tbl,index is start from 10 not 0
    MaxDzTblCnt = pTableInfor->TblSize + 10-1;
    if(DzoomIdx < 10)
    {
        DzoomIdx = 10;
    }
    else if(DzoomIdx > Get_DzoomMaxIndex())
    {
        DzoomIdx = Get_DzoomMaxIndex();
    }

    debug_msg(" %d, %d, %d, %d \r\n",MaxDzTblCnt,pTableInfor->TblSize,DzoomIdx,Get_DzoomMaxIndex());
    
    //#NT#2010/09/09#ethanlau -end
    SetIPPSizeInfor(_SensorRatioIdx, pTableInfor->SensorRatio);
    //for image pipeline
    if (GetIPPSizeInfor(_SensorRatioIdx) != gImageAlgInfo.ImageRatio)
    {
        //get adjust ratio
        HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
        VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));

        for (i = 0; i < 8; i ++) AdjTable[0][i] = DzTable[DzoomIdx - 10][i];

        if ((HRatio != IMG_RATIO_UINT) && (VRatio == IMG_RATIO_UINT))
        {
            AdjTable[0][6] = Adj_HSize(AdjTable[0][6], HRatio, 16);
            if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_TWO_LINE)
            {
                AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 16);
                AdjSize = AdjTable[0][0] / 2;
            }
            else if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_THREE_LINE)
            {
                AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 12);
                AdjSize = ((AdjTable[0][0] - THREE_LINE_DUMMY_PIX) / 3);
            }
            else
            {
                AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 16);
                AdjSize = AdjTable[0][0];
            }

            if (AdjSize  > DzTable[DzoomIdx - 10][2])
            {
                AdjTable[0][2] = DzTable[DzoomIdx - 10][2];
            }
            else
            {
                AdjTable[0][2] = AdjSize / 16 * 16;
            }
            AdjTable[0][4] = AdjTable[0][2] - IPE_HSIZE_IODIFF;
        }
        else if ((HRatio == IMG_RATIO_UINT) && (VRatio != IMG_RATIO_UINT))
        {
            AdjTable[0][1] = Adj_VSize(AdjTable[0][1], VRatio, 4);
            AdjTable[0][7] = Adj_VSize(AdjTable[0][7], VRatio, 4);
            if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_TWO_LINE)
            {
                AdjTable[0][5] = AdjTable[0][3] = AdjTable[0][1] * 2;
            }
            else if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_THREE_LINE)
            {
                AdjTable[0][5] = AdjTable[0][3] = AdjTable[0][1] * 3;
            }
            else
            {
                AdjTable[0][5] = AdjTable[0][3] = AdjTable[0][1];
            }
        }
        else
        {
            debug_err(("dzoom..not support\r\n"));
        }

        idx = 0;
        DzTable = (UINT16(*)[8])AdjTable;
    }
    else
    {
        idx = DzoomIdx - 10;
    }

    if (gImageAlgInfo.ImageRatio != gImageAlgInfo.DispRatio)
    {
        //get adjust ratio
        HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);
        VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);

        SetIPPSizeInfor(_ImeOut1Hsize, Adj_HSize(DispSizeInfo.uiDisplayFBWidth, HRatio, 16));
        SetIPPSizeInfor(_ImeOut1Vsize, Adj_VSize(DispSizeInfo.uiDisplayFBHeight, VRatio, 4));
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }
    else
    {
        SetIPPSizeInfor(_ImeOut1Hsize, DispSizeInfo.uiDisplayFBWidth);
        SetIPPSizeInfor(_ImeOut1Vsize, DispSizeInfo.uiDisplayFBHeight);
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }


    lofs = Adj_PreLineOffset(DzTable[idx][2], SIE_PRE_PARAM->VGAType, gImageAlgInfo.PrePrvBitDepth);
    SetIPPSizeInfor(_PreOutLineOffset, lofs);
    SetIPPSizeInfor(_IpeOutHsize,DzTable[idx][4]);
    SetIPPSizeInfor(_IpeOutVsize,DzTable[idx][5]);
    SetIPPSizeInfor(_PreOutHsize,DzTable[idx][2]);
    SetIPPSizeInfor(_PreOutVsize,DzTable[idx][3]);
    SetIPPSizeInfor(_PreCropHsize,DzTable[idx][0]);
    SetIPPSizeInfor(_PreCropVsize,DzTable[idx][1]);
    SetIPPSizeInfor(_IpeCapOutHsize,DzTable[idx][6] - IPE_HSIZE_IODIFF);
    SetIPPSizeInfor(_IpeCapOutVsize,DzTable[idx][7]);
    SetIPPSizeInfor(_ImeCapOutHsize,DzTable[idx][6] - IPE_HSIZE_IODIFF);
    SetIPPSizeInfor(_ImeCapOutVsize,DzTable[idx][7]);
    SetIPPSizeInfor(_PreCropCapHsize,DzTable[idx][6]);
    SetIPPSizeInfor(_PreCropCapVsize,DzTable[idx][7]);
//    debug_msg("#~~~~~~~~~~~~~~~~~#\r\n");
//    debug_msg(" %d, %d \r\n",DzoomIdx,idx);
//    for(i=0;i<8;i++)
//        debug_msg(" %d ",DzTable[idx][i]);
//    debug_msg("\r\n");

//    debug_msg("pre out size: %d %d %d\r\n",GetIPPSizeInfor(_PreOutHsize),GetIPPSizeInfor(_PreOutVsize),GetIPPSizeInfor(_PreOutLineOffset));
//    debug_msg("ime path1 out size: %d %d %d\r\n",GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
}
//#NT#20090921#ethanlau -end
void SetImageOutSize(UINT16 Hsize, UINT16 Vsize)
{
    SetIPPSizeInfor(_JpegCapHsize,Hsize);
    SetIPPSizeInfor(_JpegCapVsize,Vsize);
}

UINT32 Get_DzoomPrvMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    DZOOM_INFOR *pTableInfor;
    UINT16 (*DzTable)[8];
    UINT32 lofs;

    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;
    lofs = Adj_PreLineOffset(DzTable[0][2], SIE_PRE_PARAM->VGAType, gImageAlgInfo.PrePrvBitDepth);
    return (lofs * (DzTable[0][5]));
}

//#NT#20090912#ethanlau -begin
UINT32 Get_DzoomPrvMaxBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Hsize, Vsize, BufferSize;

    Hsize = Vsize = 0;
    if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_2)
    {
        Hsize = ((HD_Total_Prv_1)-(ACT_STRX_Prv_1));
        Vsize = ((VD_Total_Prv_1)-(ACT_STRY_Prv_1));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_3)
    {
        Hsize = ((HD_Total_Prv_2)-(ACT_STRX_Prv_2));
        Vsize = ((VD_Total_Prv_2)-(ACT_STRY_Prv_2));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE)
    {
        Hsize = ((HD_Total_Prv)-(ACT_STRX_Prv));
        Vsize = ((VD_Total_Prv)-(ACT_STRY_Prv));
    }
    Hsize = Hsize&0xfffffffc;

    if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_8BIT)
    {
        BufferSize = Hsize*Vsize;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_12BIT)
    {
        BufferSize = Hsize*Vsize*3/2;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_16BIT)
    {
        BufferSize = Hsize*Vsize*2;
    }
    else
    {
        BufferSize = Hsize*Vsize*2;//give me more if you're not sure
        debug_err(("ERROR\r\n"));
    }

    BufferSize = (BufferSize+3)&0xfffffffc;

    return BufferSize;
}
//#NT#20090912#ethanlau -end
UINT32 Get_DzoomCapMaxFieldBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Hsize, Vsize, BufferSize;

    Hsize = ((HD_Total_Cap)-(ACT_STRX_Cap));
    Vsize = ((VD_Total_Cap)-(ACT_STRY_Cap));

    Hsize = Hsize&0xfffffffc;

    if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_8BIT)
    {
        BufferSize = Hsize*Vsize;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_12BIT)
    {
        BufferSize = Hsize*Vsize*3/2;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_16BIT)
    {
        BufferSize = Hsize*Vsize*2;
    }
    else
    {
        BufferSize = Hsize*Vsize*2;//give me more if you're not sure
        debug_err(("ERROR\r\n"));
    }

    BufferSize = (BufferSize+3)&0xfffffffc;

    return BufferSize;
}

UINT32 Get_DzoomCapMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Width, Height;
    UINT16 (*DzTable)[8], HRatio, VRatio;
    DZOOM_INFOR *pTableInfor;

    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;

    HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
    VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
    //#NT#20091012#ethanlau -begin
    //Width = (Adj_HSize(DzTable[0][6], HRatio, 16) * gImageAlgInfo.PrePrvBitDepth / 8);
    Width = (Adj_HSize(DzTable[0][6], HRatio, 16) * gImageAlgInfo.PreCapBitDepth / 8);
    //#NT#20091012#ethanlau -end
    Height = Adj_VSize(DzTable[0][7], VRatio, 4);
    return (Width * Height);
}
//#NT#2010/09/11#ethanlau -begin
//#NT#return Dzoom ratio for UI
UINT32 GetRealDZoomRatio(UINT32 idx,struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 ratio;
    DZOOM_INFOR *pTableInfor;
    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    ratio = (((pTableInfor->DzTable[0][1]+4) * 100)+pTableInfor->DzTable[idx - 10][1]-1) / pTableInfor->DzTable[idx - 10][1];

    if ((ratio % 10) > 4)
        ratio = ratio / 10 + 1;
    else
        ratio = ratio / 10;

    return (ratio);
}
//#NT#2010/09/11#ethanlau -end
//#NT#2010/09/11#ethanlau -begin
//#NT#Use Dzoom ratio to estimate new dzoom table index.(to maintain ratio between different sensor mode)
UINT GetDZoomIdxFromRatio(UINT Ratio,struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    DZOOM_INFOR *pTableInfor;
    UINT i,_ratio;
    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    for(i=0;i<pTableInfor->TblSize;i++)
    {
        _ratio = (((pTableInfor->DzTable[0][1]+4) * 100)+pTableInfor->DzTable[i][1]-1) / pTableInfor->DzTable[i][1];
        if ((_ratio % 10) > 4)
            _ratio = _ratio / 10 + 1;
        else
            _ratio = _ratio / 10;
        if(_ratio >= Ratio)
        {
            Set_DzoomIndex((i+10));
            return (i+10);
        }
    }
    Set_DzoomIndex((i+10));
    return (i+10);
}
//#NT#2010/09/11#ethanlau -end
#endif//----------------------------------------------------------------------------------------------------
