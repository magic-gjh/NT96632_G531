//*****************************************************************************
//     Program        :    Digitaql Zoom for MI1030
//     Author        :
//    Goal        :    Setup various digital zoom
//     Functions    :
//*****************************************************************************
#include "sensor_model.h"

#if ZT3150_630
//#NT#2009/10/20#Jarkko Chang -begin
//#NT#modify for hd500c3 8x dzoom using
UINT16 MAX_Dzoom_Step = 40;
const UINT8 gucDzoomTable[31]={
10,
//step0
11,
12,
14,
18,
20,
//step1
22,
24,
26,
28,
30,
//step2
32,
34,
36,
38,
40,
//step3
42,
44,
46,
48,
50,
//step4
52,54,57,60,63,66,70,73,76,80
};
//#NT#2009/10/20#Jarkko Chang -end


UINT16 VDOZOOM_INFOR_TABLE[61][8];

UINT16 VDOZOOM_INFOR_MODE_2_TABLE[61][8];

UINT16 VDOZOOM_INFOR_MODE_3_TABLE[21][8];

//video
UINT16 VDOZOOM_INFOR_TABLE_DVI1[41][8] =
{
{	2560	,	720	,	2560	,	720	,	2544	,	720	,	2560	,	720	},
{	2496	,	700	,	2496	,	700	,	2480	,	700	,	2496	,	700	},
{	2432	,	684	,	2432	,	684	,	2416	,	684	,	2432	,	684	},
{	2368	,	664	,	2368	,	664	,	2352	,	664	,	2368	,	664	},
{	2304	,	648	,	2304	,	648	,	2288	,	648	,	2304	,	648	},
{	2240	,	628	,	2240	,	628	,	2224	,	628	,	2240	,	628	},
{	2176	,	612	,	2176	,	612	,	2160	,	612	,	2176	,	612	},
{	2112	,	592	,	2112	,	592	,	2096	,	592	,	2112	,	592	},
{	2048	,	576	,	2048	,	576	,	2032	,	576	,	2048	,	576	},
{	1984	,	556	,	1984	,	556	,	1968	,	556	,	1984	,	556	},
{	1920	,	540	,	1920	,	540	,	1904	,	540	,	1920	,	540	},
{	1856	,	520	,	1856	,	520	,	1840	,	520	,	1856	,	520	},
{	1792	,	504	,	1792	,	504	,	1776	,	504	,	1792	,	504	},
{	1728	,	484	,	1728	,	484	,	1712	,	484	,	1728	,	484	},
{	1664	,	468	,	1664	,	468	,	1648	,	468	,	1664	,	468	},
{	1600	,	448	,	1600	,	448	,	1584	,	448	,	1600	,	448	},
{	1536	,	432	,	1536	,	432	,	1520	,	432	,	1536	,	432	},
{	1472	,	412	,	1472	,	412	,	1456	,	412	,	1472	,	412	},
{	1408	,	396	,	1408	,	396	,	1392	,	396	,	1408	,	396	},
{	1344	,	376	,	1344	,	376	,	1328	,	376	,	1344	,	376	},
{	1280	,	360	,	1280	,	360	,	1264	,	360	,	1280	,	360	},
{	1376	,	384	,	1376	,	384	,	1360	,	384	,	1376	,	384	},
{	1328	,	372	,	1328	,	372	,	1312	,	372	,	1328	,	372	},
{	1264	,	352	,	1264	,	352	,	1248	,	352	,	1264	,	352	},
{	1216	,	340	,	1216	,	340	,	1200	,	340	,	1216	,	340	},
{	1152	,	324	,	1152	,	324	,	1136	,	324	,	1152	,	324	},
{	1104	,	308	,	1104	,	308	,	1088	,	308	,	1104	,	308	},
{	1040	,	292	,	1040	,	292	,	1024	,	292	,	1040	,	292	},
{	992	,	276	,	992	,	276	,	976	,	276	,	992	,	276	},
{	928	,	260	,	928	,	260	,	912	,	260	,	928	,	260	},
{	880	,	244	,	880	,	244	,	864	,	244	,	880	,	244	},
{	816	,	228	,	816	,	228	,	800	,	228	,	816	,	228	},
{	768	,	216	,	768	,	216	,	752	,	216	,	768	,	216	},
{	704	,	196	,	704	,	196	,	688	,	196	,	704	,	196	},
{	656	,	184	,	656	,	184	,	640	,	184	,	656	,	184	},
{	592	,	164	,	592	,	164	,	576	,	164	,	592	,	164	},
{	544	,	152	,	544	,	152	,	528	,	152	,	544	,	152	},
{	480	,	132	,	480	,	132	,	464	,	132	,	480	,	132	},
{	432	,	120	,	432	,	120	,	416	,	120	,	432	,	120	},
{	368	,	100	,	368	,	100	,	352	,	100	,	368	,	100	},
{	320	,	88	,	320	,	88	,	304	,	88	,	320	,	88	},
};

UINT16 VDOZOOM_INFOR_TABLE_DVI2[41][8] =
{
{	2560	,	720	,	2560	,	720	,	2544	,	720	,	800	,	380	},
{	2496	,	700	,	2496	,	700	,	2480	,	700	,	2496	,	700	},
{	2432	,	684	,	2432	,	684	,	2416	,	684	,	2432	,	684	},
{	2368	,	664	,	2368	,	664	,	2352	,	664	,	2368	,	664	},
{	2304	,	648	,	2304	,	648	,	2288	,	648	,	2304	,	648	},
{	2240	,	628	,	2240	,	628	,	2224	,	628	,	2240	,	628	},
{	2176	,	612	,	2176	,	612	,	2160	,	612	,	2176	,	612	},
{	2112	,	592	,	2112	,	592	,	2096	,	592	,	2112	,	592	},
{	2048	,	576	,	2048	,	576	,	2032	,	576	,	2048	,	576	},
{	1984	,	556	,	1984	,	556	,	1968	,	556	,	1984	,	556	},
{	1920	,	540	,	1920	,	540	,	1904	,	540	,	1920	,	540	},
{	1856	,	520	,	1856	,	520	,	1840	,	520	,	1856	,	520	},
{	1792	,	504	,	1792	,	504	,	1776	,	504	,	1792	,	504	},
{	1728	,	484	,	1728	,	484	,	1712	,	484	,	1728	,	484	},
{	1664	,	468	,	1664	,	468	,	1648	,	468	,	1664	,	468	},
{	1600	,	448	,	1600	,	448	,	1584	,	448	,	1600	,	448	},
{	1536	,	432	,	1536	,	432	,	1520	,	432	,	1536	,	432	},
{	1472	,	412	,	1472	,	412	,	1456	,	412	,	1472	,	412	},
{	1408	,	396	,	1408	,	396	,	1392	,	396	,	1408	,	396	},
{	1344	,	376	,	1344	,	376	,	1328	,	376	,	1344	,	376	},
{	1280	,	360	,	1280	,	360	,	1264	,	360	,	1280	,	360	},
{	1376	,	384	,	1376	,	384	,	1360	,	384	,	1376	,	384	},
{	1328	,	372	,	1328	,	372	,	1312	,	372	,	1328	,	372	},
{	1264	,	352	,	1264	,	352	,	1248	,	352	,	1264	,	352	},
{	1216	,	340	,	1216	,	340	,	1200	,	340	,	1216	,	340	},
{	1152	,	324	,	1152	,	324	,	1136	,	324	,	1152	,	324	},
{	1104	,	308	,	1104	,	308	,	1088	,	308	,	1104	,	308	},
{	1040	,	292	,	1040	,	292	,	1024	,	292	,	1040	,	292	},
{	992	,	276	,	992	,	276	,	976	,	276	,	992	,	276	},
{	928	,	260	,	928	,	260	,	912	,	260	,	928	,	260	},
{	880	,	244	,	880	,	244	,	864	,	244	,	880	,	244	},
{	816	,	228	,	816	,	228	,	800	,	228	,	816	,	228	},
{	768	,	216	,	768	,	216	,	752	,	216	,	768	,	216	},
{	704	,	196	,	704	,	196	,	688	,	196	,	704	,	196	},
{	656	,	184	,	656	,	184	,	640	,	184	,	656	,	184	},
{	592	,	164	,	592	,	164	,	576	,	164	,	592	,	164	},
{	544	,	152	,	544	,	152	,	528	,	152	,	544	,	152	},
{	480	,	132	,	480	,	132	,	464	,	132	,	480	,	132	},
{	432	,	120	,	432	,	120	,	416	,	120	,	432	,	120	},
{	368	,	100	,	368	,	100	,	352	,	100	,	368	,	100	},
{	320	,	88	,	320	,	88	,	304	,	88	,	320	,	88	},
};

UINT16 VDOZOOM_INFOR_TABLE_DVI3[41][8] =
{
//sie_in      pre_out     ipe_out    crop size
//{    3840    ,    720    ,    3840    ,    720    ,    3824    ,    720    ,    800    ,    380    },
{    3840    ,    544    ,    3840    ,    544    ,    3824    ,    544    ,    3824    ,    544    },
{    3744    ,    700    ,    3744    ,    700    ,    3728    ,    700    ,    3744    ,    700    },
{    3648    ,    684    ,    3648    ,    684    ,    3632    ,    684    ,    3648    ,    684    },
{    3552    ,    664    ,    3552    ,    664    ,    3536    ,    664    ,    3552    ,    664    },
{    3456    ,    648    ,    3456    ,    648    ,    3440    ,    648    ,    3456    ,    648    },
{    3360    ,    628    ,    3360    ,    628    ,    3344    ,    628    ,    3360    ,    628    },
{    3264    ,    612    ,    3264    ,    612    ,    3248    ,    612    ,    3264    ,    612    },
{    3168    ,    592    ,    3168    ,    592    ,    3152    ,    592    ,    3168    ,    592    },
{    3072    ,    576    ,    3072    ,    576    ,    3056    ,    576    ,    3072    ,    576    },
{    2976    ,    556    ,    2976    ,    556    ,    2960    ,    556    ,    2976    ,    556    },
{    2880    ,    540    ,    2880    ,    540    ,    2864    ,    540    ,    2880    ,    540    },
{    2784    ,    520    ,    2784    ,    520    ,    2768    ,    520    ,    2784    ,    520    },
{    2688    ,    504    ,    2688    ,    504    ,    2672    ,    504    ,    2688    ,    504    },
{    2592    ,    484    ,    2592    ,    484    ,    2576    ,    484    ,    2592    ,    484    },
{    2496    ,    468    ,    2496    ,    468    ,    2480    ,    468    ,    2496    ,    468    },
{    2400    ,    448    ,    2400    ,    448    ,    2384    ,    448    ,    2400    ,    448    },
{    2304    ,    432    ,    2304    ,    432    ,    2288    ,    432    ,    2304    ,    432    },
{    2208    ,    412    ,    2208    ,    412    ,    2192    ,    412    ,    2208    ,    412    },
{    2112    ,    396    ,    2112    ,    396    ,    2096    ,    396    ,    2112    ,    396    },
{    2016    ,    376    ,    2016    ,    376    ,    2000    ,    376    ,    2016    ,    376    },
{    1920    ,    360    ,    1920    ,    360    ,    1904    ,    360    ,    1920    ,    360    },
{    2064    ,    384    ,    2064    ,    384    ,    2048    ,    384    ,    2064    ,    384    },
{    1984    ,    372    ,    1984    ,    372    ,    1968    ,    372    ,    1984    ,    372    },
{    1904    ,    356    ,    1904    ,    356    ,    1888    ,    356    ,    1904    ,    356    },
{    1824    ,    340    ,    1824    ,    340    ,    1808    ,    340    ,    1824    ,    340    },
{    1728    ,    324    ,    1728    ,    324    ,    1712    ,    324    ,    1728    ,    324    },
{    1648    ,    308    ,    1648    ,    308    ,    1632    ,    308    ,    1648    ,    308    },
{    1568    ,    292    ,    1568    ,    292    ,    1552    ,    292    ,    1568    ,    292    },
{    1488    ,    276    ,    1488    ,    276    ,    1472    ,    276    ,    1488    ,    276    },
{    1392    ,    260    ,    1392    ,    260    ,    1376    ,    260    ,    1392    ,    260    },
{    1312    ,    244    ,    1312    ,    244    ,    1296    ,    244    ,    1312    ,    244    },
{    1232    ,    228    ,    1232    ,    228    ,    1216    ,    228    ,    1232    ,    228    },
{    1152    ,    216    ,    1152    ,    216    ,    1136    ,    216    ,    1152    ,    216    },
{    1056    ,    196    ,    1056    ,    196    ,    1040    ,    196    ,    1056    ,    196    },
{    976    ,    180    ,    976    ,    180    ,    960    ,    180    ,    976    ,    180    },
{    896    ,    168    ,    896    ,    168    ,    880    ,    168    ,    896    ,    168    },
{    816    ,    152    ,    816    ,    152    ,    800    ,    152    ,    816    ,    152    },
{    720    ,    132    ,    720    ,    132    ,    704    ,    132    ,    720    ,    132    },
{    640    ,    120    ,    640    ,    120    ,    624    ,    120    ,    640    ,    120    },
{    560    ,    104    ,    560    ,    104    ,    544    ,    104    ,    560    ,    104    },
{    480    ,    88    ,    480    ,    88    ,    464    ,    88    ,    480    ,    88    },
};

DZOOM_INFOR VDOZOOM_INFOR = {_Ratio_4_3, VDOZOOM_INFOR_TABLE};
DZOOM_INFOR VDOZOOM_INFOR_MODE_2 = {_Ratio_4_3, VDOZOOM_INFOR_MODE_2_TABLE};
DZOOM_INFOR VDOZOOM_INFOR_MODE_3 = {_Ratio_4_3, VDOZOOM_INFOR_MODE_2_TABLE};
DZOOM_INFOR VDOZOOM_INFOR_DVI1 = {_Ratio_4_3, VDOZOOM_INFOR_TABLE_DVI1};
DZOOM_INFOR VDOZOOM_INFOR_DVI2 = {_Ratio_4_3, VDOZOOM_INFOR_TABLE_DVI2};
DZOOM_INFOR VDOZOOM_INFOR_DVI3 = {_Ratio_8_3, VDOZOOM_INFOR_TABLE_DVI3};

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
    debug_err(("Get_DZoomInfor  SIE_PRE_PARAM->ModeSel = %d\n\r",SIE_PRE_PARAM->ModeSel));

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
            if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
            {
                if ( gImageAlgInfo.TvInFormat == _TvInFormat_Mode1 )
                {
                    pTableInfor = &VDOZOOM_INFOR_DVI1;
                }
                else if ( gImageAlgInfo.TvInFormat == _TvInFormat_Mode2 )
                {
                    pTableInfor = &VDOZOOM_INFOR_DVI2;
                }
                else
                {
                    pTableInfor = &VDOZOOM_INFOR_DVI3;
                }
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

#if 1
    debug_err(("DzoomIdx =%d\r\n", DzoomIdx));
    debug_err(("_PreOutLineOffset =%d\r\n", GetIPPSizeInfor(_PreOutLineOffset)));
    debug_err(("_IpeOutHsize      =%d\r\n", GetIPPSizeInfor(_IpeOutHsize)    ));
    debug_err(("_IpeOutVsize      =%d\r\n", GetIPPSizeInfor(_IpeOutVsize)    ));
    debug_err(("_PreOutHsize      =%d\r\n", GetIPPSizeInfor(_PreOutHsize)    ));
    debug_err(("_PreOutVsiz       =%d\r\n", GetIPPSizeInfor(_PreOutVsize)    ));
    debug_err(("_PreCropHsize     =%d\r\n", GetIPPSizeInfor(_PreCropHsize)   ));
    debug_err(("_PreCropVsize     =%d\r\n", GetIPPSizeInfor(_PreCropVsize)   ));
    debug_err(("_ImeOut1Hsize     =%d\r\n", GetIPPSizeInfor(_ImeOut1Hsize)   ));
    debug_err(("_ImeOut1Vsize     =%d\r\n", GetIPPSizeInfor(_ImeOut1Vsize)   ));
    debug_err(("_ImeOut1LineOffset     =%d\r\n", GetIPPSizeInfor(_ImeOut1Hsize)   ));
/*
    debug_err(("_IpeCapOutHsize   =%d\r\n", GetIPPSizeInfor(_IpeCapOutHsize) ));
    debug_err(("_IpeCapOutVsize   =%d\r\n", GetIPPSizeInfor(_IpeCapOutVsize) ));
    debug_err(("_ImeCapOutHsize   =%d\r\n", GetIPPSizeInfor(_ImeCapOutHsize) ));
    debug_err(("_ImeCapOutVsize   =%d\r\n", GetIPPSizeInfor(_ImeCapOutVsize) ));
    debug_err(("_PreCropCapHsize  =%d\r\n", GetIPPSizeInfor(_PreCropCapHsize)));
    debug_err(("_PreCropCapVsize  =%d\r\n", GetIPPSizeInfor(_PreCropCapVsize)));
    */
#endif
}

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

    pTableInfor = &VDOZOOM_INFOR_DVI3;
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;
    lofs = Adj_PreLineOffset(DzTable[0][2], SIE_PRE_PARAM->VGAType, gImageAlgInfo.PrePrvBitDepth);
    return (lofs * DzTable[0][5]);
}

UINT32 Get_DzoomPrvMaxBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Hsize, Vsize, BufferSize;

    Hsize = Vsize = 0;
    Hsize = ((HD_Total_Prv_DVI3)-(ACT_STRX_Prv_DVI3));
    Vsize = ((VD_Total_Prv_DVI3)-(ACT_STRY_Prv_DVI3));

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

    Width = (Adj_HSize(DzTable[0][6], HRatio, 16) * gImageAlgInfo.PrePrvBitDepth / 8);
    Height = Adj_VSize(DzTable[0][7], VRatio, 4);
    return (Width * Height);
}
#endif//----------------------------------------------------------------------------------------------------
