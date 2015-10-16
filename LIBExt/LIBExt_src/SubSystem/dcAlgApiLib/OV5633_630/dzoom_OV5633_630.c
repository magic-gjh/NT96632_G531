//*****************************************************************************
//     Program        :    Digitaql Zoom for MI1030
//     Author        :
//    Goal        :    Setup various digital zoom
//     Functions    :
//*****************************************************************************
#include "sensor_model.h"

#if OV5633_630
UINT16 MAX_Dzoom_Step = 33;
static UINT16 MaxDzTblCnt;

//VGA
UINT16 VDOZOOM_INFOR_TABLE[31][8] =
{
    //sie_in      pre_out     ipe_out    crop size
    { 624 , 468 , 624 , 468 , 608 , 468 , 2464 ,  1860 },
    { 576 , 432 , 576 , 432 , 560 , 432 , 2336,   1712 },
    { 528 , 396 , 528 , 396 , 512 , 396 , 2224,   1668 },
    { 496 , 372 , 496 , 372 , 480 , 372 , 2080,   1560 },
    { 464 , 348 , 464 , 348 , 448 , 348 , 1936,   1452 },
    { 432 , 324 , 432 , 324 , 416 , 324 , 1808,   1356 },
    { 400 , 300 , 400 , 300 , 384 , 300 , 1696,   1272 },
    { 368 , 276 , 368 , 276 , 352 , 276 , 1472,   1104 },
    { 336 , 252 , 336 , 252 , 320 , 252 , 1280,   960 },
    { 304 , 228 , 304 , 228 , 288 , 228 , 1200,   900 },
    { 272 , 204 , 272 , 204 , 256 , 204 , 1040,   780 },
    { 256 , 180 , 256 , 180 , 240 , 180 , 976 ,   732 },
    { 240 , 168 , 240 , 168 , 224 , 168 , 912 ,   684 },
    { 224 , 156 , 224 , 156 , 208 , 156 , 848 ,   636 },
    { 208 , 144 , 208 , 144 , 192 , 144 , 784 ,   588 },
    { 192 , 132 , 192 , 132 , 176 , 132 , 736 ,   552 },
    { 176 , 120 , 176 , 120 , 160 , 120 , 640 ,   480 },// 26
    { 160 , 108 , 160 , 108 , 144 , 108 , 592 ,   440 },//27
    { 144 ,  96 , 144 ,  96 , 128 ,  96 , 512 ,   384 },//28
    { 112 ,  84 , 112 ,  84 ,  96 ,  84 , 624 ,   468 },
    { 96 , 72 , 96 , 72 , 80 , 72 , 544 , 408 },  // 30
    { 80 , 60 , 80 , 60 , 64 , 60 , 496 , 372 },
    { 64 , 48 , 64 , 48 , 48 , 48 , 464 , 348 },
    { 48 , 36 , 48 , 36 , 32 , 36 , 432 , 324 },
    { 32 , 24 , 32 , 24 , 16 , 24 , 400 , 300 },
    { 16 , 12 , 16 , 12 ,  0 , 12 , 368 , 276 },
};

//#NT#20100506#ethanlau -begin
//add 1280x960
UINT16 VDOZOOM_INFOR_MODE_2_TABLE[31][8] =
{
    //sie_in         pre_out         ipe_out         crop size
    {1248,   936,    640,    936,    624,    936,    2464,   1860},
    {1168,   876,    640,    876,    624,    840,    2384,   1788},
    {1088,   816,    640,    816,    624,    816,    2224,   1668},
    {1008,   756,    640,    756,    624,    756,    2080,   1560},
    {944 ,   708,    640,    708,    624,    708,    1936,   1452},
    {880 ,   660,    640,    660,    624,    660,    1808,   1356},
    {816 ,   612,    640,    612,    624,    612,    1696,   1272},
    {768 ,   576,    640,    576,    624,    576,    1568,   1176},
    {720 ,   540,    640,    540,    624,    540,    1472,   1104},
    {672 ,   504,    640,    504,    624,    504,    1376,   1032},
    {624 ,   468,    624,    468,    608,    468,    1280,   960 },//20
    {576 ,   432,    576,    432,    560,    432,    1200,   900 },
    {544 ,   408,    544,    408,    528,    408,    1120,   840 },
    {512 ,   384,    512,    384,    496,    384,    1040,   780 },
    {480 ,   360,    480,    360,    464,    360,    976 ,   732 },
    {448 ,   336,    448,    336,    432,    336,    912 ,   684 },
    {416 ,   312,    416,    312,    400,    312,    848 ,   636 },
    {384 ,   288,    384,    288,    368,    288,    784 ,   588 },
    {352 ,   264,    352,    264,    336,    264,    736 ,   552 },//28
    {336 ,   252,    336,    252,    320,    252,    688 ,   516 },
    {320 ,   240,    320,    240,    304,    240,    640 ,   480 },
    {288 ,   216,    288,    216,    272,    216,    592 ,   444 },
    {272 ,   204,    272,    204,    256,    204,    560 ,   420 },
    {256 ,   192,    256,    192,    240,    192,    512 ,   384 },//33
    {240 ,   180,    240,    180,    224,    180,    480 ,   360 },
};
//#NT#20100506#ethanlau -end

//#NT#2009/12/11#YC Peng -begin
//#NT#Fix monitor mode 3 5x size
//#NT#2009/11/20#YC Peng -begin
//#NT#Dzoom 4x->5x
UINT16 VDOZOOM_INFOR_MODE_3_TABLE[21][8] =
{
//sie_in      pre_out     ipe_out    crop size
{3708,  240, 1232,  720, 1216,  720, 3664, 2736},
{3456,  224, 1136,  672, 1120,  672, 3424, 2552},
{3228,  208, 1072,  624, 1056,  624, 3184, 2380},
{3012,  196,  992,  588,  976,  588, 2976, 2224},
{2808,  180,  928,  540,  912,  540, 2784, 2072},
{2448,  160,  800,  480,  784,  480, 2416, 1804},
{2280,  148,  752,  444,  736,  444, 2256, 1684},
{2124,  136,  704,  408,  688,  408, 2112, 1572},
{1992,  128,  656,  384,  640,  384, 1968, 1468},
{1860,  120,  608,  360,  592,  360, 1840, 1368},
{1728,  112,  560,  336,  544,  336, 1712, 1276},
{1620,  104,  528,  312,  512,  312, 1600, 1192},
{1500,   96,  496,  288,  480,  288, 1488, 1112},
{1404,   92,  464,  276,  448,  276, 1392, 1036},
{1224,   80,  400,  240,  384,  240, 1216,  904},
{1140,   72,  368,  216,  352,  216, 1120,  844},
{1068,   68,  352,  204,  336,  204, 1056,  784},
{ 996,   64,  320,  192,  304,  192,  976,  732},
{ 924,   60,  304,  180,  288,  180,  912,  684},
{ 864,   56,  272,  168,  256,  168,  816,  612},
{ 744,   48,  240,  144,  224,  144,  720,  544},
};
//#NT#2009/11/20#YC Peng -end
//#NT#2009/12/11#YC Peng -end

DZOOM_INFOR VDOZOOM_INFOR = {_Ratio_4_3, VDOZOOM_INFOR_TABLE};
DZOOM_INFOR VDOZOOM_INFOR_MODE_2 = {_Ratio_4_3, VDOZOOM_INFOR_MODE_2_TABLE};
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
    MAX_Dzoom_Step = index;
}

UINT16 Get_DzoomMaxIndex(void)
{
    return MAX_Dzoom_Step;
}

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
    if(DzoomIdx < 10)
    {
        DzoomIdx = 10;
    }
    else if(DzoomIdx > Get_DzoomMaxIndex())
    {
        DzoomIdx = Get_DzoomMaxIndex();
    }

    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;

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
    //debug_msg("#~~~~~~~~~~~~~~~~~#\r\n");
    //for(i=0;i<8;i++)
    //    debug_msg(" %d ",DzTable[idx][i]);
    //debug_msg("\r\n");

    //debug_msg("pre out size: %d %d %d\r\n",GetIPPSizeInfor(_PreOutHsize),GetIPPSizeInfor(_PreOutVsize),GetIPPSizeInfor(_PreOutLineOffset));
    //debug_msg("ime path1 out size: %d %d %d\r\n",GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
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
//#NT#20090922#ethanlau -begin
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
        Hsize = ((HD_Total_Prv_2)-(ACT_STRX_Prv_2));
        Vsize = ((VD_Total_Prv_2)-(ACT_STRY_Prv_2));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_3)
    {
        Hsize = ((HD_Total_Prv_3)-(ACT_STRX_Prv_3));
        Vsize = ((VD_Total_Prv_3)-(ACT_STRY_Prv_3));
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
