/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Sensor Common Module                                               *
* Description:                                                             *
* Author: Adrian                                                           *
****************************************************************************/

/** \addtogroup SEN_COMMON_Lib */
//@{
#include "sensor_model.h"
#include "OSCommon.h"
#include "interrupt.h"
#include "IDE.h"
#include "Timer.h"
#include "pll.h"
#include "JpgEnc.h"
#include "JPEG.h"
#include "Grph.h"
#include "Filesystsk.h"
#include "Top.h"
#include "i2c.h"
#include "utility.h"

#define    IME_PATH_DRAW_VA_WINDOW    0

unsigned long     AWB_NormRatio = (1<<AWBScaleBit);
unsigned long     AF_NormRatio = (1<<AFScaleBit);

UB PCC2CAP=0;
//#NT#20100919#ethanlua
//for fast zoom key switch issue
Direction DZoom_move = Stop;
//#NT#20100919#ethanlau
FPMPEG4PutSrcImgIdxCallBack g_pMP4Callback = NULL;

UINT32 DVISkipFrmEn = ENABLE;
UINT32  IPE_Vacc_TAb1[8][8];
UINT32  IPE_Vacc_TAb2[8][8];
UINT32  IPE_Vacc_TAb3[16][16];
UINT32  IPE_Vacc_TAb4[16][16];
UINT32  PRE_Vacc_TAb[5];

unsigned int ImeVideoFmt=1;//422

char message[160];
UINT    gpuiFbAdd[3];


RGB_IMAGE_TIFF_HEADER RGB_TIFF_Header =
{
//TIFF header for RGB planar image
{ {0x4D,0x4D}, {0x00,0x2A}, {0x00,0x00,0x00,0x08} },//ImageFileHeader
{  0x00,0x0E                                      },//IFD_Entry_Number
{ {0x00,0xFE}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_NewSubFileType
{ {0x01,0x00}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_ImageWidth       (to be set)
{ {0x01,0x01}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_ImageLength      (to be set)
{ {0x01,0x02}, {0x00,0x03}, {0x00,0x00,0x00,0x03}, {0x00,0x00,0x00,0xB6} },//IFD_Entry_BitsPerSample    (offset value 1)
{ {0x01,0x03}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x01,0x00,0x00} },//IFD_Entry_Compression
{ {0x01,0x06}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x02,0x00,0x00} },//IFD_Entry_PhotometricInterpretation
{ {0x01,0x11}, {0x00,0x04}, {0x00,0x00,0x00,0x03}, {0x00,0x00,0x00,0xBC} },//IFD_Entry_StripOffsets     (offset value 2)
{ {0x01,0x15}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x03,0x00,0x00} },//IFD_Entry_SamplesPerPixel
{ {0x01,0x16}, {0x00,0x04}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0x00} },//IFD_Entry_RowsPerStrip     (to be set)
{ {0x01,0x17}, {0x00,0x04}, {0x00,0x00,0x00,0x03}, {0x00,0x00,0x00,0xC8} },//IFD_Entry_StripByteCounts  (offset value 3)
{ {0x01,0x1A}, {0x00,0x05}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0xD4} },//IFD_Entry_XResolution      (offset value 4)
{ {0x01,0x1B}, {0x00,0x05}, {0x00,0x00,0x00,0x01}, {0x00,0x00,0x00,0xDC} },//IFD_Entry_YResolution      (offset value 5)
{ {0x01,0x1C}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x02,0x00,0x00} },//IFD_Entry_PlanarConfiguration
{ {0x01,0x28}, {0x00,0x03}, {0x00,0x00,0x00,0x01}, {0x00,0x02,0x00,0x00} },//IFD_Entry_ResolutionUnit
{  0x00,0x00,0x00,0x00 },                                                  //NextIDFOffset
{ {0x00,0x08}, {0x00,0x08}, {0x00,0x08} },                                 //BitsPerSampleValue
{ {0x00,0x00,0x00,0xE4}, {0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00} },   //StripOffsetsValue          (to be set)
{ {0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00}, {0x00,0x00,0x00,0x00} },   //StripByteCounts            (to be set)
{  0x00,0x00,0x01,0x2C,0x00,0x00,0x00,0x01 },                              //XResolutionValue
{  0x00,0x00,0x01,0x2C,0x00,0x00,0x00,0x01 }                               //YResolutionValue
};

const UINT32 VA_9PixTable[31][2]=
{
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 0},
{35, 7},
{34, 0},
{33, 0},
{32, 0},
{29, 5},
{28, 3},
{27, 5},
{26, 3},
{24, 7},
{23, 6},
{22, 5},
{22, 0},
{22, 0},
{20, 7},
{19, 7},
{19, 7},
{19, 0},
{19, 0},
{17, 7},
{17, 7}
};
const UINT32 VA_Mod3Table[31][2]=
{
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 7},
{38, 0},
{34, 5},
{34, 0},
{33, 0},
{31, 0},
{29, 5},
{28, 3},
{27, 5},
{26, 3},
{24, 7},
{23, 6},
{22, 5},
{22, 5},
{22, 0},
{20, 7},
{20, 7},
{19, 7},
{19, 0},
{19, 0},
{17, 7},
{17, 7}
};

PSENSOR_APPOBJ  pSensorInfor;
SENSOR_APPOBJ   SensorInfor;
PCAPTURE_APPOBJ pCapInfor;
CAPTURE_APPOBJ  CapInfor;


UINT    gDP_Add_Prv=NULL;
UINT    gDP_Add_Cap=NULL;

IPP_FUNC_INFOR gIPPFuncInfor = {
    DISABLE,//DfsEn;
    DFS_FIRSTPIC,//DfsCnt;
    DISABLE,//PRED2DEn;
    DISABLE,//NLightCapEn;
    NLCap_QVDataIn,//NLightCapSel;
    ENABLE,//FQVEn;
    0,//CapISO;
    DISABLE,//ChrFiltEn;
    0,//ChrFiltNum;
    DISABLE,//FQVInProcess;
    DISABLE,//PreD2DRotEn;
    DISABLE,//ChrCurveEn;
//#NT#2011/09/05#Spark Chou -begin
    //#NT#
    0, //*ChrCBCurve;
    0, //*ChrCRCurve;
    0, //*CapGamma;
    0, //StillAntiShakingIdx;
    DISABLE, //DVIModeEn;
//#NT#2011/09/05#Spark Chou -end
};


INT32 Open_Sensor(PSENSOR_APPOBJ    pSensorObj)
{
    SensorInfor = *pSensorObj;
    return ERR_OK;
}
void ime_mp4_isr(void)
{
    if(g_pMP4Callback != NULL)
    {
        g_pMP4Callback();
    }
}

unsigned int GetImeFrameCnt(void)
{
    //return ime_fc;
    return 0;
}
void ResetImeFrameCnt(void)
{
    //ime_fc=0;
}

//#NT#2009/12/03#Yuppie Lin -begin
//#NT#remove useless code
/*
void GetImgFBAddr(FBADDR_PRAM *pFBAddr_para)
{
    pFBAddr_para->y0_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_Y);
    pFBAddr_para->cb0_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CB);
    pFBAddr_para->cr0_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CR);

    pFBAddr_para->y1_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_Y);
    pFBAddr_para->cb1_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CB);
    pFBAddr_para->cr1_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CR);

    pFBAddr_para->y2_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_Y);
    pFBAddr_para->cb2_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CB);
    pFBAddr_para->cr2_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CR);

    pFBAddr_para->ime_format=ime_getOutputFormat();
    pFBAddr_para->Ipe_fc=0;
    pFBAddr_para->Ime_fc=0;

}
*/
//#NT#2009/12/03#Yuppie Lin -end

void GetActImgFBAddr(ACTFBADDR_PRAM *pActFBAddr_para)
{
    unsigned int active_frame;

    if(GetImeFrameCnt()<2)
        active_frame=0;
    else
        active_frame=(GetImeFrameCnt()-1)%3;

    switch(active_frame)
    {
        case 0:
        pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_Y);
        pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CB);
        pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CR);
        break;
        case 1:
        pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_Y);
        pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CB);
        pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CR);
        break;
        case 2:
        pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_Y);
        pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CB);
        pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CR);
        break;
    }
}
void GetActImgFBAddr_Path1(ACTFBADDR_PRAM *pActFBAddr_para)
{
    switch (ime_getPath1CurrentBufferIndex())
    {
        case 0:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_0,IME_CHANNEL_CR);
        break;
        case 1:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_1,IME_CHANNEL_CR);
        break;
        case 2:
            pActFBAddr_para->y_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr=ime_getOutputPath1Add(IME_BUFNUM_2,IME_CHANNEL_CR);
        break;
    }
}
void GetActImgFBAddr_Path2(ACTFBADDR_PRAM *pActFBAddr_para)
{
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
    if(gImageAlgInfo.DISMode == _DISMode_On)
    {
        //#NT#2010/01/22#Connie Yu -begin
        //#NT#DIS Update
        dis_accumRecProcess();
        dis_updateRecAddr(pActFBAddr_para);
        //#NT#2010/01/22#Connie Yu -end
    }
    else
    {
        switch (ime_getPath2CurrentBufferIndex())
        {
        case 0:
            pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CR);
            break;

        case 1:
            pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CR);
            break;

        case 2:
            pActFBAddr_para->y_addr     = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_Y);
            pActFBAddr_para->cb_addr    = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CB);
            pActFBAddr_para->cr_addr    = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CR);
            break;
        }

        //#NT#2010/01/25#Connie Yu -begin
        //#NT#Compatible with new media Rec flow
        pActFBAddr_para->DisY_Addr      = pActFBAddr_para->y_addr;
        pActFBAddr_para->DisU_Addr      = pActFBAddr_para->cb_addr;
        pActFBAddr_para->DisV_Addr      = pActFBAddr_para->cr_addr;
        pActFBAddr_para->SrcYLineOffset = GetIPPSizeInfor(_ImeOut2LineOffset);
        //#NT#2010/01/25#Connie Yu -end
    }
    //debug_err(("fb2 0x%x 0x%x 0x%x\r\n",pActFBAddr_para->y_addr,pActFBAddr_para->cb_addr,pActFBAddr_para->cr_addr));
//#NT#2009/11/27#Connie Yu -end
}

void SetImeVideoFormat(UINT ImeFmt)
{
    ImeVideoFmt=ImeFmt;
}
UINT GetImeVideoFormat(void)
{
    return ImeVideoFmt;

}
void SetImeVideoResolution(UINT Hsize,UINT Vsize,UINT BufNum)
{
    UINT                Video_Addr;
    pIME_FBADDR_PRAM      pIME_Para2;
    //#NT#2009/11/27#Connie Yu -begin
    //#NT#Add DIS process
    DIS_SIZE newSize;

    if(gImageAlgInfo.DISMode == _DISMode_On)
    {
        newSize.Hsz = Hsize;
        newSize.Vsz = Vsize;
        dis_updateRecordSize(&newSize);
        Hsize = newSize.Hsz;
        Vsize = newSize.Vsz;
        //debug_err(("new rec size %dx%d\r\n",Hsize, Vsize));
    }
    //#NT#2009/11/27#Connie Yu -end
    Video_Addr=IME_D2_Para.y0_addr;
    IME_D2_Para.buf_num = BufNum;
    if(GetImeVideoFormat()==0)//444
    {
        IME_D2_Para.y0_addr=Video_Addr;
        IME_D2_Para.cb0_addr=(Video_Addr+Hsize*Vsize);
        IME_D2_Para.cr0_addr=(Video_Addr+Hsize*Vsize*2);
        IME_D2_Para.y1_addr=Video_Addr+3*Hsize*Vsize;//IME_D1_Addr.y0_addr;
        IME_D2_Para.cb1_addr=(IME_D2_Para.y1_addr+Hsize*Vsize);
        IME_D2_Para.cr1_addr=(IME_D2_Para.y1_addr+Hsize*Vsize*2);
        IME_D2_Para.y2_addr=Video_Addr+6*Hsize*Vsize;
        IME_D2_Para.cb2_addr=(IME_D2_Para.y2_addr+Hsize*Vsize);
        IME_D2_Para.cr2_addr=(IME_D2_Para.y2_addr+Hsize*Vsize*2);
        if (BufNum > 3)
        {
            IME_D2_Para.y3_addr=Video_Addr+9*Hsize*Vsize;
            IME_D2_Para.cb3_addr=(IME_D2_Para.y3_addr+Hsize*Vsize);
            IME_D2_Para.cr3_addr=(IME_D2_Para.y3_addr+Hsize*Vsize*2);
            IME_D2_Para.y4_addr=Video_Addr+12*Hsize*Vsize;
            IME_D2_Para.cb4_addr=(IME_D2_Para.y4_addr+Hsize*Vsize);
            IME_D2_Para.cr4_addr=(IME_D2_Para.y4_addr+Hsize*Vsize*2);
            IME_D2_Para.y5_addr=Video_Addr+18*Hsize*Vsize;
            IME_D2_Para.cb5_addr=(IME_D2_Para.y5_addr+Hsize*Vsize);
            IME_D2_Para.cr5_addr=(IME_D2_Para.y5_addr+Hsize*Vsize*2);
            IME_D2_Para.y6_addr=Video_Addr+21*Hsize*Vsize;
            IME_D2_Para.cb6_addr=(IME_D2_Para.y6_addr+Hsize*Vsize);
            IME_D2_Para.cr6_addr=(IME_D2_Para.y6_addr+Hsize*Vsize*2);
            IME_D2_Para.y7_addr=Video_Addr+24*Hsize*Vsize;
            IME_D2_Para.cb7_addr=(IME_D2_Para.y7_addr+Hsize*Vsize);
            IME_D2_Para.cr7_addr=(IME_D2_Para.y7_addr+Hsize*Vsize*2);
        }
    }
    else if((GetImeVideoFormat()==1)||(GetImeVideoFormat()==2))//422
    {
        IME_D2_Para.y0_addr=Video_Addr;
        IME_D2_Para.cb0_addr=(Video_Addr+Hsize*Vsize);
        IME_D2_Para.cr0_addr=(Video_Addr+Hsize*Vsize*3/2);
        IME_D2_Para.y1_addr=Video_Addr+2*Hsize*Vsize;//IME_D1_Addr.y0_addr;
        IME_D2_Para.cb1_addr=(IME_D2_Para.y1_addr+Hsize*Vsize);
        IME_D2_Para.cr1_addr=(IME_D2_Para.y1_addr+Hsize*Vsize*3/2);
        IME_D2_Para.y2_addr=Video_Addr+4*Hsize*Vsize;
        IME_D2_Para.cb2_addr=(IME_D2_Para.y2_addr+Hsize*Vsize);
        IME_D2_Para.cr2_addr=(IME_D2_Para.y2_addr+Hsize*Vsize*3/2);
        if (BufNum > 3)
        {
            IME_D2_Para.y3_addr=Video_Addr+6*Hsize*Vsize;
            IME_D2_Para.cb3_addr=(IME_D2_Para.y3_addr+Hsize*Vsize);
            IME_D2_Para.cr3_addr=(IME_D2_Para.y3_addr+Hsize*Vsize*3/2);
            IME_D2_Para.y4_addr=Video_Addr+8*Hsize*Vsize;
            IME_D2_Para.cb4_addr=(IME_D2_Para.y4_addr+Hsize*Vsize);
            IME_D2_Para.cr4_addr=(IME_D2_Para.y4_addr+Hsize*Vsize*3/2);
            IME_D2_Para.y5_addr=Video_Addr+10*Hsize*Vsize;
            IME_D2_Para.cb5_addr=(IME_D2_Para.y5_addr+Hsize*Vsize);
            IME_D2_Para.cr5_addr=(IME_D2_Para.y5_addr+Hsize*Vsize*3/2);
            IME_D2_Para.y6_addr=Video_Addr+12*Hsize*Vsize;
            IME_D2_Para.cb6_addr=(IME_D2_Para.y6_addr+Hsize*Vsize);
            IME_D2_Para.cr6_addr=(IME_D2_Para.y6_addr+Hsize*Vsize*3/2);
            IME_D2_Para.y7_addr=Video_Addr+14*Hsize*Vsize;
            IME_D2_Para.cb7_addr=(IME_D2_Para.y7_addr+Hsize*Vsize);
            IME_D2_Para.cr7_addr=(IME_D2_Para.y7_addr+Hsize*Vsize*3/2);
        }
    }
    else if((GetImeVideoFormat()==3)||(GetImeVideoFormat()==4))//420
    {
        IME_D2_Para.y0_addr=Video_Addr;
        IME_D2_Para.cb0_addr=(Video_Addr+Hsize*Vsize);
        IME_D2_Para.cr0_addr=(Video_Addr+Hsize*Vsize*5/4);
        IME_D2_Para.y1_addr=Video_Addr+Hsize*Vsize*3/2;//IME_D1_Addr.y0_addr;
        IME_D2_Para.cb1_addr=(IME_D2_Para.y1_addr+Hsize*Vsize);
        IME_D2_Para.cr1_addr=(IME_D2_Para.y1_addr+Hsize*Vsize*5/4);
        IME_D2_Para.y2_addr=Video_Addr+3*Hsize*Vsize;
        IME_D2_Para.cb2_addr=(IME_D2_Para.y2_addr+Hsize*Vsize);
        IME_D2_Para.cr2_addr=(IME_D2_Para.y2_addr+Hsize*Vsize*5/4);
        if (BufNum > 3)
        {
            IME_D2_Para.y3_addr=Video_Addr+Hsize*Vsize*9/2;
            IME_D2_Para.cb3_addr=(IME_D2_Para.y3_addr+Hsize*Vsize);
            IME_D2_Para.cr3_addr=(IME_D2_Para.y3_addr+Hsize*Vsize*5/4);
            IME_D2_Para.y4_addr=Video_Addr+Hsize*Vsize*6;
            IME_D2_Para.cb4_addr=(IME_D2_Para.y4_addr+Hsize*Vsize);
            IME_D2_Para.cr4_addr=(IME_D2_Para.y4_addr+Hsize*Vsize*5/4);
            IME_D2_Para.y5_addr=Video_Addr+Hsize*Vsize*15/2;
            IME_D2_Para.cb5_addr=(IME_D2_Para.y5_addr+Hsize*Vsize);
            IME_D2_Para.cr5_addr=(IME_D2_Para.y5_addr+Hsize*Vsize*5/4);
            IME_D2_Para.y6_addr=Video_Addr+Hsize*Vsize*9;
            IME_D2_Para.cb6_addr=(IME_D2_Para.y6_addr+Hsize*Vsize);
            IME_D2_Para.cr6_addr=(IME_D2_Para.y6_addr+Hsize*Vsize*5/4);
            IME_D2_Para.y7_addr=Video_Addr+Hsize*Vsize*21/2;
            IME_D2_Para.cb7_addr=(IME_D2_Para.y7_addr+Hsize*Vsize);
            IME_D2_Para.cr7_addr=(IME_D2_Para.y7_addr+Hsize*Vsize*5/4);
        }
    }

    IME_D2_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
//#NT#2011/08/29#Spark Chou -begin
//#NT# DVI Input
    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        IME_D2_Para.in_h=(GetIPPSizeInfor(_IpeOutHsize)+16)>>1;
    }

    IME_D2_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
//#NT#2011/08/29#Spark Chou -end
    IME_D2_Para.out_h=Hsize;
    IME_D2_Para.out_v=Vsize;
    IME_D2_Para.in_format=IME_IN_FMT_422;
    IME_D2_Para.out_format=GetImeVideoFormat();//422
    IME_D2_Para.out_lineoffset=Hsize;
    pIME_Para2=&IME_D2_Para;
    SetIPPSizeInfor(_ImeOut2Hsize, Hsize);
    SetIPPSizeInfor(_ImeOut2Vsize, Vsize);
    SetIPPSizeInfor(_ImeOut2LineOffset, Hsize);
    ime_clrIntFlag();
    ime_waitFlagFrameEnd();
    ime_SetDzoom2Video_Path2(&IME_D2_Para);
}
void SetImePccResolution(UINT Hsize,UINT Vsize)
{
    UINT                Video_Addr;
    pIME_FBADDR_PRAM      pIME_Para1;

//Add for PCC Mode
    switch (Hsize)
    {
    case 2592:
        Photo_SetImageResolution(_Resolution_2592x1944, IPL_MODE_VIDEO);
        break;
    case 2048:
        Photo_SetImageResolution(_Resolution_2048x1536, IPL_MODE_VIDEO);
        break;
    case 1920:
        Photo_SetImageResolution(_Resolution_1920x1080, IPL_MODE_VIDEO);
        break;
    case 1600:
        Photo_SetImageResolution(_Resolution_1600x1200, IPL_MODE_VIDEO);
        break;
    case 1280:
        Photo_SetImageResolution(_Resolution_1280x720, IPL_MODE_VIDEO);
        break;
    case 800:
        Photo_SetImageResolution(_Resolution_800x600, IPL_MODE_VIDEO);
        break;
    case 320:
        Photo_SetImageResolution(_Resolution_320x240, IPL_MODE_VIDEO);
        break;
    default:
        Photo_SetImageResolution(_Resolution_640x480, IPL_MODE_VIDEO);
        break;
    }

    Video_Addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);//IME_D1_Para.y0_addr;
    if(GetImeVideoFormat()==0)//444
    {
        IME_D1_Para.y0_addr=Video_Addr;
        IME_D1_Para.cb0_addr=(Video_Addr+Hsize*Vsize);
        IME_D1_Para.cr0_addr=(Video_Addr+Hsize*Vsize*2);
        IME_D1_Para.y1_addr=Video_Addr+3*Hsize*Vsize;//IME_D1_Addr.y0_addr;
        IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+Hsize*Vsize);
        IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+Hsize*Vsize*2);
        IME_D1_Para.y2_addr=Video_Addr+6*Hsize*Vsize;
        IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+Hsize*Vsize);
        IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+Hsize*Vsize*2);
    }
    else if((GetImeVideoFormat()==1)||(GetImeVideoFormat()==2))//422
    {
        IME_D1_Para.y0_addr=Video_Addr;
        IME_D1_Para.cb0_addr=(Video_Addr+Hsize*Vsize);
        IME_D1_Para.cr0_addr=(Video_Addr+Hsize*Vsize*3/2);
        IME_D1_Para.y1_addr=Video_Addr+2*Hsize*Vsize;//IME_D1_Addr.y0_addr;
        IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+Hsize*Vsize);
        IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+Hsize*Vsize*3/2);
        IME_D1_Para.y2_addr=Video_Addr+4*Hsize*Vsize;
        IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+Hsize*Vsize);
        IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+Hsize*Vsize*3/2);
    }
    else if((GetImeVideoFormat()==3)||(GetImeVideoFormat()==4))//420
    {
        IME_D1_Para.y0_addr=Video_Addr;
        IME_D1_Para.cb0_addr=(Video_Addr+Hsize*Vsize);
        IME_D1_Para.cr0_addr=(Video_Addr+Hsize*Vsize*5/4);
        IME_D1_Para.y1_addr=Video_Addr+Hsize*Vsize*3/2;//IME_D1_Addr.y0_addr;
        IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+Hsize*Vsize);
        IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+Hsize*Vsize*5/4);
        IME_D1_Para.y2_addr=Video_Addr+3*Hsize*Vsize;
        IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+Hsize*Vsize);
        IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+Hsize*Vsize*5/4);
    }

    SetIPPSizeInfor(_ImeOut1Hsize, Hsize);
    SetIPPSizeInfor(_ImeOut1Vsize, Vsize);
    SetIPPSizeInfor(_ImeOut1LineOffset, Hsize);
    //#NT#2011/11/25#Spark Chou -begin
    //#NT#
    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    if ( GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
        IME_D1_Para.in_h = (IME_D1_Para.in_h+16) >> 1;
    //#NT#2011/11/25#Spark Chou -end
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IME_D1_Para.in_format=IME_IN_FMT_422;
    IME_D1_Para.out_format=GetImeVideoFormat();//422
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);
    pIME_Para1=&IME_D1_Para;

    ime_SetDzoom2Video_Path1_TV(&IME_D1_Para);
}

void SetImeIdeResolution(UINT ImeOutHsize,UINT ImeOutVsize, UINT IdeOutHsize,UINT IdeOutVsize)
{ //(ImeOutHsize : ImeOutVsize) must be at panel ratio
    DispSizeInfo.uiDisplayFBWidth = ImeOutHsize;
    DispSizeInfo.uiDisplayFBHeight = ImeOutVsize;
//#NT#2010/03/23#Connie Yu -begin
//#NT#Fix IPL flow
    //if (GetIPPSizeInfor(_SensorRatioIdx) != gImageAlgInfo.ImageRatio)
    if (gImageAlgInfo.DispRatio != gImageAlgInfo.ImageRatio)
    {
        SetIPPSizeInfor(_ImeOut1Hsize, Adj_HSize(DispSizeInfo.uiDisplayFBWidth, Get_HRatio(gImageAlgInfo.ImageRatio,  gImageAlgInfo.DispRatio), 16));
        SetIPPSizeInfor(_ImeOut1Vsize, Adj_VSize(DispSizeInfo.uiDisplayFBHeight, Get_VRatio(gImageAlgInfo.ImageRatio,  gImageAlgInfo.DispRatio), 4));
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }
//#NT#2010/03/23#Connie Yu -end
    else
    {
        SetIPPSizeInfor(_ImeOut1Hsize, DispSizeInfo.uiDisplayFBWidth);
        SetIPPSizeInfor(_ImeOut1Vsize, DispSizeInfo.uiDisplayFBHeight);
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }
    IME_D1_Para.y0_addr=ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    IME_D1_Para.cb0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr0_addr=(IME_D1_Para.y0_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y1_addr=IME_D1_Para.y0_addr+2*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr1_addr=(IME_D1_Para.y1_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);
    IME_D1_Para.y2_addr=IME_D1_Para.y0_addr+4*DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight;
    IME_D1_Para.cb2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight);
    IME_D1_Para.cr2_addr=(IME_D1_Para.y2_addr+DispSizeInfo.uiDisplayFBWidth*DispSizeInfo.uiDisplayFBHeight*3/2);

    IME_D1_Para.in_h=GetIPPSizeInfor(_IpeOutHsize);
    IME_D1_Para.in_v=GetIPPSizeInfor(_IpeOutVsize);
    IME_D1_Para.out_h=GetIPPSizeInfor(_ImeOut1Hsize);
    IME_D1_Para.out_v=GetIPPSizeInfor(_ImeOut1Vsize);
    IME_D1_Para.in_format=IME_IN_FMT_422;
    IME_D1_Para.out_format=IME_OUT_FMT_422_COSITED;
    IME_D1_Para.out_lineoffset=GetIPPSizeInfor(_ImeOut1LineOffset);
    ime_SetDzoom2Video_Path1_TV(&IME_D1_Para);
}

UINT GetJpgBufSize(void)
{
    return JpgSize;
}

UINT32 SetJpgBufSize(UINT32 uiSetValue)
{
    JpgSize = uiSetValue;
    return JpgSize;
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
void SIEReset(void)
{
    if(sie_CheckBusy()||pre_CheckBusy())
    {
        debug_err(("SIEReset() : SIE/PRE status busy, can NOT reset SIE!\r\n"));
    }
    else
    {
        pll_enableSystemReset(SIE_RSTN);
        pll_disableSystemReset(SIE_RSTN);
        sie_Reset(ENABLE);
        sie_Reset(DISABLE);
    }
}
void IPEReset(void)
{
    if(ipe_checkBusy())
    {
        debug_err(("IPEReset() : IPE status busy, can NOT reset IPE!\r\n"));
    }
    else
    {
        ipe_reset(ENABLE);
        pll_enableSystemReset(IPE_RSTN);
        pll_disableSystemReset(IPE_RSTN);
    }
}

//#NT#2009/12/03#Yuppie Lin -begin
//#NT#modify IMEReset
void IMEReset(void)
{
    ime_HWreset();
    #if 0
    int slice_en,start;

    slice_en=ime_getSliceStatus();
    start=ime_getStartStatus();
    if((slice_en==1)||(start==1))
    {
        debug_err(("IMEReset() : IME status busy, can NOT reset IME!\r\n"));
    }
    else
    {
        ime_reset(1);
        ime_start(DISABLE);
        pll_enableSystemReset(IME_RSTN);
        pll_disableSystemReset(IME_RSTN);
    }
    #endif
}
//#NT#2009/12/03#Yuppie Lin -end
void DzoomStop(void)
{
    SEN_INDMSG("Stop DZoom...\r\n");
    DZoom_move = FALSE;
}

//#NT#2010/05/28#ethanlau -begin
//fix dzoom in/out fast switch issue
Direction Get_DzoomFlag(void)
{
    return DZoom_move;
}
void Set_DzoomFlag(Direction CDZoom)
{
     DZoom_move = CDZoom;
}
//#NT#2010/05/28#ethanlau -end

void SetPCC2Cap(UINT *Addr, UINT *JPGsize)
{
#if 0
    UINT uiFlag;

    PCC2CAP=1;
    sta_tsk(SIECAPTSK_ID);
    sta_tsk(IMGSLICETSK_ID);

    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_ImgSize, _IMGSize_5M);
    ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
    ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
    ImgCapture_SetParameter(_SiePath,_PostGamma);
    ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
    ImgCapture_StartCapture();
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_JPGCOMPLETE,TWF_ORW);
    clr_flg(FLG_ID_SLICE, FLGSLICETSK1_JPGCOMPLETE);
    *Addr=ippbuf_GetBuf(IPPBUF_CAP_JPG_1);//JpgOutAddr[0];
    *JPGsize=JPG_Size[0];
#endif
}
void SetPCC2CapRawJpg(UINT *rawAddr, UINT *rawWidth, UINT *rawHeight, UINT *rawBitDepth, UINT *jpgAddr, UINT *jpgSize)
{
#if 0
    UINT uiFlag;
    extern unsigned long JPG_TempAdd[3];

    PCC2CAP=1;
    clr_flg(FLG_ID_SLICE, 0xffffffff);
    sta_tsk(SIECAPTSK_ID);
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_SIECAPIDLE,TWF_ORW);
    sta_tsk(IMGSLICETSK_ID);
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_IDLE,TWF_ORW);
    ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
    ImgCapture_SetParameter(_ImgSize,  _IMGSize_10M);
    ImgCapture_SetParameter(_FileFmt,  _FileFmt_RawJpg);
    ImgCapture_SetParameter(_PreCapBitDepth, _PreBitDepth_8);
    ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
    ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_Single);
    ImgCapture_StartCapture();
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_JPGCOMPLETE,TWF_ORW);
    wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_IDLE,TWF_ORW);

    *rawAddr=ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    *rawWidth=GetIPPSizeInfor(_PreCropCapHsize);
    *rawHeight=GetIPPSizeInfor(_PreCropCapVsize);
    *rawBitDepth=_PreBitDepth_8;
    *jpgAddr=JPG_TempAdd[0]  ;// ippbuf_GetBuf(IPPBUF_CAP_JPG_1);
    *jpgSize=JPG_Size[0];
    ter_tsk(SIECAPTSK_ID);
    ter_tsk(IMGSLICETSK_ID);
#endif

}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
int TimerCountUp(unsigned char action)
{
#if 0
    static unsigned int CountUpTimerID = 0;
    unsigned int    Count=0;
    unsigned char    Status=0;
    int                ms;

    if(action==1)//Start timer count up
    {
        if(timer_open(&CountUpTimerID, NULL) != 0)
            return -5;

        dis_int(_INT_TIMER0 + CountUpTimerID);
        timer_pausePlay(CountUpTimerID,_TIMER_PAUSE);
        ena_int(_INT_TIMER0 + CountUpTimerID);
        timer_set(CountUpTimerID, 100000, _TIMER_CTRL_FREE_RUN|_TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
            return 0;
    }
    else//Stop timer and read out
    {
        Count=timer_getCurrentCount(CountUpTimerID);
        Status=timer_getINTStatus(CountUpTimerID);
        timer_close(CountUpTimerID);
        if(Status)//Over 100s
        {
            return -7;
        }
        else
        {
            ms = Count / 1000;
            return ms;
        }
    }
#endif
return 0;
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void ide_Set2Preview(unsigned int    FB0_Addr,
                     unsigned int   FB1_Addr,
                     unsigned int   FB2_Addr,
                        unsigned int    Hsize,
                     unsigned int    Vsize,
                     unsigned int     LineOffset,
                     unsigned int   Format)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;
    UINT    Ysize,Csize;
    Ysize=LineOffset*Vsize;
    Csize=Ysize/2;

    FB_Addr.B0_Y = FB0_Addr;
    FB_Addr.B0_CB = FB_Addr.B0_Y +Ysize;// FB_W * FB_H;
    FB_Addr.B0_CR = FB_Addr.B0_CB + Csize;//FB_W * FB_H;
    FB_Addr.B1_Y = FB1_Addr;
    FB_Addr.B1_CB = FB_Addr.B1_Y + Ysize;//FB_W * FB_H;
    FB_Addr.B1_CR = FB_Addr.B1_CB + Csize;//FB_W * FB_H;
    FB_Addr.B2_Y = FB2_Addr;
    FB_Addr.B2_CB = FB_Addr.B2_Y + Ysize;//FB_W * FB_H;
    FB_Addr.B2_CR = FB_Addr.B2_CB + Csize;//FB_W * FB_H;
    FB_Addr.uiBufSel = (IDE_VIDEO_ADDR_SEL_BUFFER0 | IDE_VIDEO_ADDR_SEL_BUFFER1 |IDE_VIDEO_ADDR_SEL_BUFFER2);

    FB_attr.Buf_W = Hsize;
    FB_attr.Buf_H = Vsize;
    FB_attr.Buf_Lineoffset = LineOffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 0;
    FB_attr.Buf_Opt = IDE_VIDEO_BUFFER_OPT_2;
    //FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 0;             //three buffer
    FB_win.Source_W = Hsize;
    FB_win.Source_H = Vsize;
    FB_win.Des_W = GetIPPSizeInfor(_ImeOut1Hsize);
    FB_win.Des_H = GetIPPSizeInfor(_ImeOut1Vsize);
    FB_win.Win_Format = Format;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;

    ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    ide_set_source(0);
//    ide_set_video1_bufferop(0,//UINT8 uiBJMODE,
//                            2,//UINT8 uiOPTBUF,
//                            0);//UINT8 uiBUFNUM)

    ide_set_load();
}
void ide_Set2Video(pIME_FBADDR_PRAM pFB_Para)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;

    FB_Addr.B0_Y =  pFB_Para->y0_addr;
    FB_Addr.B0_CB = pFB_Para->cb0_addr;// FB_W * FB_H;
    FB_Addr.B0_CR = pFB_Para->cr0_addr;//FB_W * FB_H;
    FB_Addr.B1_Y =  pFB_Para->y1_addr;
    FB_Addr.B1_CB = pFB_Para->cb1_addr;//FB_W * FB_H;
    FB_Addr.B1_CR = pFB_Para->cr1_addr;//FB_W * FB_H;
    FB_Addr.B2_Y =  pFB_Para->y2_addr;
    FB_Addr.B2_CB = pFB_Para->cb2_addr;//FB_W * FB_H;
    FB_Addr.B2_CR = pFB_Para->cr2_addr;//FB_W * FB_H;
    FB_Addr.uiBufSel = (IDE_VIDEO_ADDR_SEL_BUFFER0 | IDE_VIDEO_ADDR_SEL_BUFFER1 |IDE_VIDEO_ADDR_SEL_BUFFER2);

    FB_attr.Buf_W = pFB_Para->out_h;
    FB_attr.Buf_H = pFB_Para->out_v;
    FB_attr.Buf_Lineoffset = pFB_Para->out_lineoffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 0;
    FB_attr.Buf_Opt = IDE_VIDEO_BUFFER_OPT_2;
    //FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 0;             //three buffer

    FB_win.Source_W = pFB_Para->out_h;
    FB_win.Source_H = pFB_Para->out_v;
    FB_win.Des_W = GetIPPSizeInfor(_ImeOut1Hsize);
    FB_win.Des_H = GetIPPSizeInfor(_ImeOut1Vsize);
    FB_win.Win_Format =  pFB_Para->out_format;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;

    ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    ide_set_source(0);
    //ide_set_video1_bufferop(0,//UINT8 uiBJMODE,
    //                        2,//UINT8 uiOPTBUF,
    //                        0);//UINT8 uiBUFNUM)
    ide_set_load();
}

void ide_Set2Preview2(unsigned int    FB0_Addr,
                     unsigned int   FB1_Addr,
                     unsigned int   FB2_Addr,
                     unsigned int    Hsize,
                     unsigned int    Vsize,
                     unsigned int    OutHsize,
                     unsigned int    OutVsize,
                     unsigned int    LineOffset,
                     unsigned int   Format)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;
    UINT    Ysize,Csize;
    Ysize=LineOffset*Vsize;
    Csize=Ysize/2;

    FB_Addr.B0_Y = FB0_Addr;
    FB_Addr.B0_CB = FB_Addr.B0_Y +Ysize;// FB_W * FB_H;
    FB_Addr.B0_CR = FB_Addr.B0_CB + Csize;//FB_W * FB_H;
    FB_Addr.B1_Y = FB1_Addr;
    FB_Addr.B1_CB = FB_Addr.B1_Y + Ysize;//FB_W * FB_H;
    FB_Addr.B1_CR = FB_Addr.B1_CB + Csize;//FB_W * FB_H;
    FB_Addr.B2_Y = FB2_Addr;
    FB_Addr.B2_CB = FB_Addr.B2_Y + Ysize;//FB_W * FB_H;
    FB_Addr.B2_CR = FB_Addr.B2_CB + Csize;//FB_W * FB_H;
    FB_Addr.uiBufSel = (IDE_VIDEO_ADDR_SEL_BUFFER0 | IDE_VIDEO_ADDR_SEL_BUFFER1 |IDE_VIDEO_ADDR_SEL_BUFFER2);

    FB_attr.Buf_W = Hsize;
    FB_attr.Buf_H = Vsize;
    FB_attr.Buf_Lineoffset = LineOffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 0;
    FB_attr.Buf_Opt = IDE_VIDEO_BUFFER_OPT_2;
    //FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 0;             //three buffer

    FB_win.Source_W = Hsize;
    FB_win.Source_H = Vsize;
    FB_win.Des_W = OutHsize;//PhotoInfor.uiDisplayOutWidth; //PhotoInfor.uiDisplayFBWidth;//gsizeDisplay.uiWinWidth;
    FB_win.Des_H = OutVsize;//PhotoInfor.uiDisplayOutHeight;//PhotoInfor.uiDisplayFBHeight;//gsizeDisplay.uiWinHeight;
    FB_win.Win_Format = Format;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;

    ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    ide_set_source(0);
    //ide_set_video1_bufferop(0,//UINT8 uiBJMODE,
    //                        2,//UINT8 uiOPTBUF,
    //                        0);//UINT8 uiBUFNUM)
    ide_set_load();
}
void ide_Set2Video2(pIME_FBADDR_PRAM pFB_Para)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;

    FB_Addr.B0_Y =  pFB_Para->y0_addr;
    FB_Addr.B0_CB = pFB_Para->cb0_addr;// FB_W * FB_H;
    FB_Addr.B0_CR = pFB_Para->cr0_addr;//FB_W * FB_H;
    FB_Addr.B1_Y =  pFB_Para->y1_addr;
    FB_Addr.B1_CB = pFB_Para->cb1_addr;//FB_W * FB_H;
    FB_Addr.B1_CR = pFB_Para->cr1_addr;//FB_W * FB_H;
    FB_Addr.B2_Y =  pFB_Para->y2_addr;
    FB_Addr.B2_CB = pFB_Para->cb2_addr;//FB_W * FB_H;
    FB_Addr.B2_CR = pFB_Para->cr2_addr;//FB_W * FB_H;
    FB_Addr.uiBufSel = (IDE_VIDEO_ADDR_SEL_BUFFER0 | IDE_VIDEO_ADDR_SEL_BUFFER1 |IDE_VIDEO_ADDR_SEL_BUFFER2);

    FB_attr.Buf_W = pFB_Para->in_h;
    FB_attr.Buf_H = pFB_Para->in_v;
    FB_attr.Buf_Lineoffset = pFB_Para->out_lineoffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 0;
    FB_attr.Buf_Opt = IDE_VIDEO_BUFFER_OPT_2;
    //FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 0;             //three buffer

    FB_win.Source_W = pFB_Para->in_h;
    FB_win.Source_H = pFB_Para->in_v;
    FB_win.Des_W = pFB_Para->out_h;//PhotoInfor.uiDisplayFBWidth;//gsizeDisplay.uiWinWidth;
    FB_win.Des_H = pFB_Para->out_v;//PhotoInfor.uiDisplayFBHeight;//gsizeDisplay.uiWinHeight;
    FB_win.Win_Format =  pFB_Para->out_format;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;

    ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    ide_set_source(0);
    //ide_set_video1_bufferop(0,//UINT8 uiBJMODE,
    //                        2,//UINT8 uiOPTBUF,
    //                        0);//UINT8 uiBUFNUM)
    ide_set_load();
}
void ide_SetSize(pIME_FBADDR_PRAM pFB_Para)
{
    VIDEO_BUF_ADDR FB_Addr;
    VIDEO_BUF_ATTR FB_attr;
    VOSD_WINDOW_ATTR FB_win;

    FB_Addr.B0_Y =  pFB_Para->y0_addr;
    FB_Addr.B0_CB = pFB_Para->cb0_addr;// FB_W * FB_H;
    FB_Addr.B0_CR = pFB_Para->cr0_addr;//FB_W * FB_H;
    FB_Addr.B1_Y =  pFB_Para->y1_addr;
    FB_Addr.B1_CB = pFB_Para->cb1_addr;//FB_W * FB_H;
    FB_Addr.B1_CR = pFB_Para->cr1_addr;//FB_W * FB_H;
    FB_Addr.B2_Y =  pFB_Para->y2_addr;
    FB_Addr.B2_CB = pFB_Para->cb2_addr;//FB_W * FB_H;
    FB_Addr.B2_CR = pFB_Para->cr2_addr;//FB_W * FB_H;
    FB_Addr.uiBufSel = (IDE_VIDEO_ADDR_SEL_BUFFER0 | IDE_VIDEO_ADDR_SEL_BUFFER1 |IDE_VIDEO_ADDR_SEL_BUFFER2);

    FB_attr.Buf_W = pFB_Para->in_h;
    FB_attr.Buf_H = pFB_Para->in_v;
    FB_attr.Buf_Lineoffset = pFB_Para->out_lineoffset/4;
    FB_attr.Buf_L2R = 0;
    FB_attr.Buf_T2B = 0;
    FB_attr.Buf_BJmode = 0;
    FB_attr.Buf_Opt = IDE_VIDEO_BUFFER_OPT_2;
    //FB_attr.Buf_Opt = 0;
    FB_attr.Buf_Num = 0;             //three buffer

    FB_win.Source_W = pFB_Para->in_h;
    FB_win.Source_H = pFB_Para->in_v;
    FB_win.Des_W = pFB_Para->out_h;
    FB_win.Des_H = pFB_Para->out_v;
    FB_win.Win_Format =  pFB_Para->out_format;
    FB_win.Win_X = 0;
    FB_win.Win_Y = 0;
    FB_win.High_Addr = 0;

    ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    ide_set_source(0);
    ide_set_load();
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
/**
  * @brief Set IME to Preview
  */
void ime_Set2Preview(pIME_FBADDR_PRAM  pIME_Para1)
{
    IPPMgrReq.ImeParam1 = pIME_Para1;

    if ( IME_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IME_BeginIPP() error \n"));

    if ( IME_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IME_AllocatResourceIPP() error \n"));

    SenFlowFunc(IME_Chrom_Prv_SETTING, &gSenCBInfo);
    if ( IME_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
}

void ime_SetDzoom2Preview(unsigned int    D_Addr,
                          unsigned int    S_Hsize,
                          unsigned int    S_Vsize,
                          unsigned int    D_Hsize,
                          unsigned int    D_Vsize,
                          unsigned int     LineOffset)
{
#if 0
    long dw;

    ime_setPath(1);
    ime_Path2HScaleUpdown(0);   //dwon
    ime_Path2VScaleUpdown(0);   //down
    ime_InputStripeHDim(0,(S_Hsize)/16,0);
    ime_InputStripeVDim(0,S_Vsize-1,0);
    dw = LineOffset  * D_Vsize;
    set_ImePath1Scale_to_DMAReg(D_Addr,D_Addr+dw,D_Addr+dw*3/2,
                                D_Addr+dw*2,D_Addr+dw*3,D_Addr+dw*7/2,
                                D_Addr+dw*4,D_Addr+dw*5,D_Addr+dw*11/2);
    ime_OutputPath1YcbCrOffset(LineOffset);
    ime_OutputPath1SizeScale(S_Hsize, S_Vsize, D_Hsize, D_Vsize);
    ime_Path1HOutputSize(D_Hsize-1);
    ime_Path1VOutputSize(D_Vsize-1);
    ime_freeRun(ON);
    ime_start(ON);
#endif
}
void ime_Set2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2)
{
    IPPMgrReq.ImeParam1 = pIME_Para1;
    IPPMgrReq.ImeParam2 = pIME_Para2;

    if ( IME_BeginIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IME_BeginIPP() error \n"));

    if ( IME_AllocatResourceIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IME_AllocatResourceIPP() error \n"));

    if ( IME_ExecIPP(&IPPMgrReq, VIDEO_JOB) != IPP_OK)
        debug_err(("IME_ExecIPP() error \n"));
}

void ime_Set2VideoForModeS  (pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2)
{
#if 0
    ime_reset(ON);
    ime_reset(OFF);
    ime_freeRun(ON);    set_ImeModeReg(0,//BOOL src,
                   0,//unsigned int path,
                   1,//BOOL bil,
                   pIME_Para1->in_format,//unsigned int imat,
                   0,//BOOL sten,
                   1,//BOOL ippbe,
                   2,//unsigned int oppbe1,
                   2,// unsigned int oppbe2,
                   0,// BOOL icst_en,
                   0);// BOOL icst_opt)

    ime_InputStripeHDim(0,(pIME_Para1->in_h)/16,0);
    ime_InputStripeVDim(0,pIME_Para1->in_v-1,0);

    //Path 1
    set_ImePath1Scale_to_DMAReg(pIME_Para1->y0_addr,    pIME_Para1->cb0_addr,   pIME_Para1->cr0_addr,
                                pIME_Para1->y1_addr,    pIME_Para1->cb1_addr,    pIME_Para1->cr1_addr,
                                pIME_Para1->y2_addr,    pIME_Para1->cb2_addr,    pIME_Para1->cr2_addr);
    ime_OutputPath1YcbCrOffset(pIME_Para1->out_lineoffset);
    ime_OutputPath1SizeScale(pIME_Para1->in_h, pIME_Para1->in_v, pIME_Para1->out_h, pIME_Para1->out_v);
    //ime_Path1HAverage(0);
    ime_Path1OutputFormat(pIME_Para1->out_format);
    ime_Path1HOutputSize(pIME_Para1->out_h-1);
    ime_Path1VOutputSize(pIME_Para1->out_v-1);

    //Path 2
    set_ImePath2Scale_to_DMAReg(pIME_Para2->y0_addr,    pIME_Para2->cb0_addr,   pIME_Para2->cr0_addr,
                                pIME_Para2->y1_addr,    pIME_Para2->cb1_addr,    pIME_Para2->cr1_addr,
                                pIME_Para2->y2_addr,    pIME_Para2->cb2_addr,    pIME_Para2->cr2_addr);
    ime_OutputPath2YcbCrOffset(pIME_Para2->out_lineoffset);
    ime_OutputPath2SizeScale(pIME_Para2->in_h, pIME_Para2->in_v, pIME_Para2->out_h, pIME_Para2->out_v);
    //ime_Path2HAverage(0);
    ime_Path2OutputFormat(pIME_Para2->out_format);
    ime_Path2HOutputSize(pIME_Para2->out_h-1);
    ime_Path2VOutputSize(pIME_Para2->out_v-1);

    ime_SetCST(103,88,183,198);
    ime_load();

    //$$
    ime_StatustClear(Ime_INT_ALL);
    ime_InitInt();
    ime_start(ON);
#endif
}

void ime_SetDzoom2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2)
{
    IMEParam.op_mode = _CHANGESIZE_2PATH;
    IMEParam.in_size.h_size = pIME_Para1->in_h;
    IMEParam.in_size.v_size = pIME_Para1->in_v;
    IMEParam.format.in_format = pIME_Para1->in_format;

    IMEParam.format.out1_format = pIME_Para1->out_format;
    IMEParam.line_ofs.out1_lineofs = pIME_Para1->out_lineoffset;
    IMEParam.p1_out_size.h_size = pIME_Para1->out_h;
    IMEParam.p1_out_size.v_size = pIME_Para1->out_v;
    IMEParam.p1_out0_addr.y_addr = pIME_Para1->y0_addr;
    IMEParam.p1_out0_addr.cb_addr = pIME_Para1->cb0_addr;
    IMEParam.p1_out0_addr.cr_addr = pIME_Para1->cr0_addr;
    IMEParam.p1_out1_addr.y_addr = pIME_Para1->y1_addr;
    IMEParam.p1_out1_addr.cb_addr = pIME_Para1->cb1_addr;
    IMEParam.p1_out1_addr.cr_addr = pIME_Para1->cr1_addr;
    IMEParam.p1_out2_addr.y_addr = pIME_Para1->y2_addr;
    IMEParam.p1_out2_addr.cb_addr = pIME_Para1->cb2_addr;
    IMEParam.p1_out2_addr.cr_addr = pIME_Para1->cr2_addr;

    IMEParam.format.out2_format = pIME_Para2->out_format;
    IMEParam.line_ofs.out2_lineofs = pIME_Para2->out_lineoffset;
    IMEParam.p2_out_size.h_size = pIME_Para2->out_h;
    IMEParam.p2_out_size.v_size = pIME_Para2->out_v;
    IMEParam.p2_out0_addr.y_addr = pIME_Para2->y0_addr;
    IMEParam.p2_out0_addr.cb_addr = pIME_Para2->cb0_addr;
    IMEParam.p2_out0_addr.cr_addr = pIME_Para2->cr0_addr;
    IMEParam.p2_out1_addr.y_addr = pIME_Para2->y1_addr;
    IMEParam.p2_out1_addr.cb_addr = pIME_Para2->cb1_addr;
    IMEParam.p2_out1_addr.cr_addr = pIME_Para2->cr1_addr;
    IMEParam.p2_out2_addr.y_addr = pIME_Para2->y2_addr;
    IMEParam.p2_out2_addr.cb_addr = pIME_Para2->cb2_addr;
    IMEParam.p2_out2_addr.cr_addr = pIME_Para2->cr2_addr;

    ime_setMode(&IMEParam);
    ime_setStart();
}

void ime_SetDzoom2Video_Path1(pIME_FBADDR_PRAM  pIME_Para1)
{
    IMEParam.op_mode = _CHANGESIZE_1PATH;
    IMEParam.in_size.h_size = pIME_Para1->in_h;
    IMEParam.in_size.v_size = pIME_Para1->in_v;
    IMEParam.format.in_format = pIME_Para1->in_format;

    IMEParam.format.out1_format = pIME_Para1->out_format;
    IMEParam.line_ofs.out1_lineofs = pIME_Para1->out_lineoffset;
    IMEParam.p1_out_size.h_size = pIME_Para1->out_h;
    IMEParam.p1_out_size.v_size = pIME_Para1->out_v;
    IMEParam.p1_out0_addr.y_addr = pIME_Para1->y0_addr;
    IMEParam.p1_out0_addr.cb_addr = pIME_Para1->cb0_addr;
    IMEParam.p1_out0_addr.cr_addr = pIME_Para1->cr0_addr;
    IMEParam.p1_out1_addr.y_addr = pIME_Para1->y1_addr;
    IMEParam.p1_out1_addr.cb_addr = pIME_Para1->cb1_addr;
    IMEParam.p1_out1_addr.cr_addr = pIME_Para1->cr1_addr;
    IMEParam.p1_out2_addr.y_addr = pIME_Para1->y2_addr;
    IMEParam.p1_out2_addr.cb_addr = pIME_Para1->cb2_addr;
    IMEParam.p1_out2_addr.cr_addr = pIME_Para1->cr2_addr;
    ime_setMode(&IMEParam);
    ime_setStart();
}

void ime_SetDzoom2Video_Path1_TV(pIME_FBADDR_PRAM  pIME_Para1)
{
//#NT#2009/12/21#Bowen Li -begin
//#NT#fix bug for TV change ime path1 size
    IMEParam.op_mode = _CHANGESIZE_1PATH;
    if ((ime_getOPState() == _AUTO_2PATH) || (ime_getOPState() == _CHANGESIZE_2PATH))
    {
        IMEParam.op_mode = _CHANGESIZE_2PATH;
    }
//#NT#2009/12/21#Bowen Li -end
    IMEParam.in_size.h_size = pIME_Para1->in_h;
    IMEParam.in_size.v_size = pIME_Para1->in_v;
    IMEParam.format.in_format = pIME_Para1->in_format;

    IMEParam.format.out1_format = pIME_Para1->out_format;
    IMEParam.line_ofs.out1_lineofs = pIME_Para1->out_lineoffset;
    IMEParam.p1_out_size.h_size = pIME_Para1->out_h;
    IMEParam.p1_out_size.v_size = pIME_Para1->out_v;
    IMEParam.p1_out0_addr.y_addr = pIME_Para1->y0_addr;
    IMEParam.p1_out0_addr.cb_addr = pIME_Para1->cb0_addr;
    IMEParam.p1_out0_addr.cr_addr = pIME_Para1->cr0_addr;
    IMEParam.p1_out1_addr.y_addr = pIME_Para1->y1_addr;
    IMEParam.p1_out1_addr.cb_addr = pIME_Para1->cb1_addr;
    IMEParam.p1_out1_addr.cr_addr = pIME_Para1->cr1_addr;
    IMEParam.p1_out2_addr.y_addr = pIME_Para1->y2_addr;
    IMEParam.p1_out2_addr.cb_addr = pIME_Para1->cb2_addr;
    IMEParam.p1_out2_addr.cr_addr = pIME_Para1->cr2_addr;
    ime_setMode(&IMEParam);
    ime_setStart();
}

void ime_SetDzoom2Video_Path2(pIME_FBADDR_PRAM  pIME_Para2)
{
    IMEParam.op_mode = _CHANGESIZE_2PATH;
    IMEParam.in_size.h_size = pIME_Para2->in_h;
    IMEParam.in_size.v_size = pIME_Para2->in_v;
    IMEParam.format.in_format = pIME_Para2->in_format;

    IMEParam.format.out2_format = pIME_Para2->out_format;
    IMEParam.line_ofs.out2_lineofs = pIME_Para2->out_lineoffset;
    IMEParam.p2_out_size.h_size = pIME_Para2->out_h;
    IMEParam.p2_out_size.v_size = pIME_Para2->out_v;
    IMEParam.p2_out0_addr.y_addr = pIME_Para2->y0_addr;
    IMEParam.p2_out0_addr.cb_addr = pIME_Para2->cb0_addr;
    IMEParam.p2_out0_addr.cr_addr = pIME_Para2->cr0_addr;
    IMEParam.p2_out1_addr.y_addr = pIME_Para2->y1_addr;
    IMEParam.p2_out1_addr.cb_addr = pIME_Para2->cb1_addr;
    IMEParam.p2_out1_addr.cr_addr = pIME_Para2->cr1_addr;
    IMEParam.p2_out2_addr.y_addr = pIME_Para2->y2_addr;
    IMEParam.p2_out2_addr.cb_addr = pIME_Para2->cb2_addr;
    IMEParam.p2_out2_addr.cr_addr = pIME_Para2->cr2_addr;
    ime_setMode(&IMEParam);
    ime_setStart();
}

void ime_Set2Capture(STR_YCBCR_ADDR in_addr,
                     STR_YCBCR_ADDR out_addr,
                     unsigned int in_format,
                     unsigned int out_format,
                     STR_SCALE_SIZE_INFO io_size,
                     BOOL src)
{
#if 0
     ime_reset(ON);
     ime_reset(OFF);
     ime_start(OFF);
     ime_load();
     ime_freeRun(OFF);

     set_ImeModeReg(src,//src  0:IPE to IME, 1:DMA to IME
                    1,//path 0: both scale paths, 1: only one scale path, 2: only Bicubic scale path
                    0,//bil 0: bicubic, 1: bilinear
                    in_format,//format 0: 444, 1:422, 2:420
                    1,//sten 0: single stripe, 1: multi stripe
                    0,//ippbe 0: ping-pong buffer disable, 1:ping-pong buffer enable
                    0,//oppbe1 0:one output buffer, 1:two output buffer, 2:three output buffer
                    0,//oppbe2 0:one output buffer, 1:two output buffer, 2:three output buffer
                    0,//icst_en 0:disable, 1:enable
                    0 //icst_opt 0:RGB transfer to YCBCR, 1:YCBCR transfer to RGB
                    );
     //Input

     ime_InputYcbCrAddr0(in_addr.y_addr, in_addr.cb_addr, in_addr.cr_addr);
     ime_InputYcbCrAddr1(in_addr.y_addr, in_addr.cb_addr, in_addr.cr_addr);
     ime_InputYcbCrOffset(io_size.in_h);

     set_ImeInputStripe();

     //Output path1
     ime_OutputPath1SizeScale(io_size.in_h, io_size.in_v, io_size.out_h, io_size.out_v);
     //ime_Path1HAverage(0);
     ime_Path1OutputFormat(out_format);
     ime_Path1HOutputSize(io_size.out_h-1);
     ime_Path1VOutputSize(io_size.out_v-1);
     ime_OutputPath1YcbCrAddr0(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
     ime_OutputPath1YcbCrAddr1(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
     ime_OutputPath1YcbCrAddr2(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
     ime_OutputPath1YcbCrOffset(io_size.out_h);

     /*
     //Output path2
     ime_OutputPath2SizeScale(io_size.in_h, io_size.in_v, io_size.out_h, io_size.out_v);
     //ime_Path2HAverage(0);
     ime_Path2OutputFormat(out_format);
     ime_Path2HOutputSize(io_size.out_h-1);
     ime_Path2VOutputSize(io_size.out_v-1);
     ime_OutputPath2YcbCrAddr0(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
     ime_OutputPath2YcbCrOffset(io_size.out_h);
     */
     ime_SetCST(103,88,183,198);

     ime_load();
     ime_ClearFrameEnd();
     ime_StatustClear(Ime_INT_ALL);
#endif
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void ipe_Set2Preview(IPE_PARAM *IpeParam)
{
    IPPMgrReq.IpeParam = IpeParam;

    if ( IPE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \n"));

    SenFlowFunc(IPE_PRV_SETTING, &gSenCBInfo);

    if ( IPE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \n"));
}

ER ipe_Dram2Dram(IPE_PARAM *IpeParam)
{
    IPE_OPENOBJ IPEOpenObj;

    IPEOpenObj.FP_IPEISR_CB = NULL;
    ipe_open(&IPEOpenObj);

    IPPMgrReq.IpeParam = IpeParam;

    if ( IPE_BeginIPP(&IPPMgrReq, IPE_DRAM_RAWIMGIN_JOB) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, IPE_DRAM_RAWIMGIN_JOB) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \n"));

    IPEModeInfo.IQINFO.VA_PARAM = IpeParam->VaccInfo;
    IPEModeInfo.IQINFO.VA_PARAM.vacc_num = WINDOWS_8X8;
    IPEModeInfo.IQINFO.VA_PARAM.vacc_hv = COMBINED_HV;
    aaa_VAOutputSel(WIN8x8_HV_VALUE, IpeParam->VAAddr);

    IPEModeInfo.OUTINFO.OutSrcSel = VADATA_TYPE;
    IPEModeInfo.OUTINFO.Adr0 =  IpeParam->VAAddr;
    ipe_changeOutput(&IPEModeInfo.OUTINFO);

    //IPE VA parameters
    SetIPEVarAccInfo(&IPEModeInfo.SIZEINFO.INSIZE, &IPEModeInfo.IQINFO.VA_PARAM, NULL);
    //IPE IQ parameters
    SenFlowFunc(IPE_D2D_SETTING, &gSenCBInfo);

    if ( IPE_ExecIPP(&IPPMgrReq, IPE_DRAM_RAWIMGIN_JOB) != IPP_OK)
        debug_err(("IPE_ExecIPP() error \n"));

    ipe_waitFrameEnd(IPE_NOCLRFLG);
    ipe_pause();
    ipe_close();

    //debug_err(("IPE D2D VA %d\r\n",(aaa_GetVariation(30)+aaa_GetVariation(31)+aaa_GetVariation(32)+aaa_GetVariation(33))/4));

    return E_OK;
}

#if 0
#pragma mark -
#endif
void sie_Set2Preview(SIE_PRE_PARAM *SiePreParam)
{
    IPPMgrReq.SIE_PRE_PRV = SiePreParam;

    if (Sensor_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    if ( SIE_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_AllocatResourceIPP() error \n"));

    if ( PRE_BeginIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("PRE_BeginIPP() error \n"));

    if ( PRE_AllocatResourceIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("PRE_AllocatResourceIPP() error \n"));

    SenFlowFunc(PRE_PRV_SETTING, &gSenCBInfo);

    if ( PRE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("PRE_ExecIPP() error \n"));


    if ( SIE_ExecIPP(&IPPMgrReq, PREVIEW_JOB) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
}

void sie_Set2Dzoom(SIE_PRE_PARAM *SiePreParam)
{
    UINT32 AddrOfs;
//#NT#2010/01/22#Jarkko Chang -begin
//#NT#add for ca parameters update when dzoom
    UINT32 HRatio, VRatio, Ratio;

    IPPMgrReq.SIE_PRE_PRV = SiePreParam;
    SenFlowFunc(PRE_PRV_SETTING, &gSenCBInfo);
    pre_SetBreakPoint(IPPMgrReq.SIE_PRE_PRV->PreBp1);

    pre_SetHcropSize((SiePreParam->RawCenterX-SiePreParam->PreCropSizeH/2- SiePreParam->HDActX), SiePreParam->PreCropSizeH);
    pre_SetVcropSize((SiePreParam->RawCenterY-SiePreParam->PreCropSizeV/2- SiePreParam->HDActY[0]), SiePreParam->PreCropSizeV);
    pre_SetOutputSize(SiePreParam->PreOutSizeH);

    if (SiePreParam->VGAType == SENSOR_OUT_THREE_LINE)
    {
        pre_HScale(((SiePreParam->PreCropSizeH-THREE_LINE_DUMMY_PIX)/THREE_LINE),SiePreParam->PreOutSizeH);
    }
    else if (SiePreParam->VGAType == SENSOR_OUT_TWO_LINE)
    {
        pre_HScale((SiePreParam->PreCropSizeH/TWO_LINE),SiePreParam->PreOutSizeH);
    }
    else
    {
        pre_HScale(SiePreParam->PreCropSizeH,SiePreParam->PreOutSizeH);
    }
    AddrOfs = Sen_GetPREAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_PreOutVsize), SiePreParam->PreOutLineOfs, gImageAlgInfo.PrePrvBitDepth);
    pre_setOutputStartAdd((SiePreParam->PreOutAdd0 + AddrOfs));
    pre_setOutputStartAdd1((SiePreParam->PreOutAdd1 + AddrOfs));
    pre_SetOutputLineOffset(SiePreParam->PreOutLineOfs);

    Sen_GetCASubRatio(SiePreParam->VGAType, GetIPPSizeInfor(_CAHSub), GetIPPSizeInfor(_CAVSub), &HRatio, &VRatio);
    Ratio = Sen_GetIntlvRatio(SiePreParam->VGAType);
    pre_setCAOutputAdd(ippbuf_GetBuf(IPPBUF_CA)+ Sen_GetPREAddrOfs(gImageAlgInfo.ImageFlip, GetIPPSizeInfor(_CAVsize), GetIPPSizeInfor(_CALineOffset), 8));
    pre_setCAOutputParam(PATH_PRE_CLOLRGAIN,GetIPPSizeInfor(_CAHSub),GetIPPSizeInfor(_CAVSub),(GetIPPSizeInfor(_PreOutHsize) * Ratio)/HRatio);
//#NT#2010/01/22#Jarkko Chang -end

    pre_Load();
}

void sie_Set2Capture(SIE_PRE_PARAM *SiePreParam)
{
    //#NT#2010/09/17#ethanlau -begin
    //#NT#move to lens control flow
    //pinmux_enable_mes(PINMUX_MES0|PINMUX_MES1);
    //#NT#2010/09/17#ethanlau -end
    pre_pause();
    sie_pause();
    sie_SetSensorSync(SIE_PRE_Para_prv.HDSync,SIE_PRE_Para_prv.HDTotal, SIE_PRE_Para_prv.VDSync*2, SIE_PRE_Para_prv.VDTotal*2);
    sie_ClrIntrStatus(SIE_INT_ALL);
    sie_IntrptEnable(DISABLE,SIE_INTE_ALL);
    sie_ClrIntrStatus(SIE_INT_ALL);

    if(SensorInfor.FP_GPIOMap_CloseShutter!=NULL)
    {
        SensorInfor.FP_GPIOMap_CloseShutter();
    }

    IPPMgrReq.SIE_PRE_CAP = SiePreParam;
    sie_SetBreakPoint(SiePreParam->SieBp1, SiePreParam->SieBp2);
    sie_load();

    if ( Sensor_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("Sensor_BeginIPP() error \n"));

    if ( SIE_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_BeginIPP() error \n"));

    if ( SIE_AllocatResourceIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("SIE_AllocatResourceIPP() error \n"));

    if ( PRE_BeginIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("PRE_BeginIPP() error \n"));

    if ( PRE_AllocatResourceIPP(&IPPMgrReq, CAPTURE_JOB) != IPP_OK)
        debug_err(("PRE_AllocatResourceIPP() error \n"));

    SenFlowFunc(PRE_CAP_SETTING, &gSenCBInfo);
    SenFlowFunc(SIE_PRE_GET_CAP_RAW, &gSenCBInfo);
    if(SensorInfor.FP_GPIOMap_OpenShutter!=NULL)
    {
        SensorInfor.FP_GPIOMap_OpenShutter();
    }

}

/**
    Stop the IPP DRAM access when "the IPP are running"(Important).
    If the IPP are not running,it should not be called or it will hang at sie_waitVD().

    @param void
    @return void
*/
void StopPreviewDramData(void)
{
    UINT32 uiPixel;

    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        DVISkipFrmEn = DISABLE;
        TimerDelayMs(30);
        sie_SensorDataEnable(ON);
        sie_Load();
    }

    uiPixel = sie_ReadPixelCounterStatus();
    TimerDelayUs(1);
    if ((uiPixel == sie_ReadPixelCounterStatus()) ||
        (sie_checkFunctionEnable(SIE_ACT_EN) == 0))
    {
        debug_err(("StopPreviewDramData fail\r\n"));
        DVISkipFrmEn = ENABLE;
        return ;
    }
    //For synchronization
    ime_clrIntFlag();
    if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
        ime_waitFlagFrameEnd();

    //#NT#2010/01/22#Connie Yu -begin
    //#NT#DIS Update
    /*
    if(gImageAlgInfo.DISMode == _DISMode_On)
    {
        aaa_DisLock(LOCK);
        if(dis_isEnabled())
        {
            dis_eventPause();
        }
    }
    */
    //#NT#2010/01/22#Connie Yu -end

    //disable ipe
    ipe_pause();
//#NT#2010/01/28#Jarkko Chang -begin
//#NT#add for ime bug
    ime_freeRun(DISABLE);
//#NT#2010/01/28#Jarkko Chang -end
    ime_clrIntFlag();
    if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
    ime_waitFlagFrameEnd();

    //disable sensor data & pre
    pre_pause();
    sie_pause();
    sie_waitVD(1);

    //reset pre ipe ime
    ipe_reset(ON);
    ime_setStop();
    ime_setReset();

    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
        DVISkipFrmEn = ENABLE;
}

/**
    Restart the IPP DRAM access when the IPP had been stop by calling StopPreviewDramData().
    The IPP register setting should be set properly before calling StartPreviewDramData().

    @param void
    @return void
*/

void StartPreviewDramData(void)
{
    UINT32 uiPixel;

    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        return ;
    }

    uiPixel = sie_ReadPixelCounterStatus();
    TimerDelayUs(1);
    if ((uiPixel != sie_ReadPixelCounterStatus()) && (sie_checkFunctionEnable(SIE_ACT_EN) == 0))
    {
        //For synchronization
        sie_waitVD(1);
        //enable IME
//#NT#2010/01/28#Jarkko Chang -begin
//#NT#add for ime bug
        ime_freeRun(ENABLE);
//#NT#2010/01/28#Jarkko Chang -end
        ime_setStart();
        //enable IPE

        ipe_reset(OFF);
        ipe_start();

        //enable SIE
        sie_start();
        pre_start();

        //sie pre registers are latched after VD
        sie_waitVD(1);

        //#NT#2010/01/22#Connie Yu -begin
        //#NT#DIS Update
        /*
        if(gImageAlgInfo.DISMode == _DISMode_On)
        {
            //dis_processOpen();
            //dis_processLock(UNLOCK);
            aaa_DisLock(UNLOCK);
            if(!dis_isEnabled())
            {
                dis_eventStart();
            }
        }
        */
        //#NT#2010/01/22#Connie Yu -end
    }
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void SetupMPEG4CallBackFun(FPMPEG4PutSrcImgIdxCallBack pmp4callback)
{
    g_pMP4Callback = pmp4callback;
}
//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void GetIMEJPGAddr(UINT StartAddr,IME_FBADDR_PRAM* TmpAddr,UINT IME_OutHeight,UINT IME_OutWidth,UB OutFormat,UB OPPBnum)
{
    UINT Slc_YSize,Slc_CSize;

    Slc_YSize=IME_OutWidth*IME_OutHeight;
    switch(OutFormat)
    {
        case 0://444
        Slc_CSize=Slc_YSize;
        break;
        case 1://422
        Slc_CSize=Slc_YSize/2;
        break;
        case 2://422
        Slc_CSize=Slc_YSize/2;
        break;
        case 3://420
        Slc_CSize=Slc_YSize/4;
        break;
        case 4://420
        Slc_CSize=Slc_YSize/4;
        break;
        default://420
        Slc_CSize=Slc_YSize/4;
        break;
    }

    if(OPPBnum==3)
    {
        TmpAddr->y0_addr= StartAddr;
        TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
        TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;

        TmpAddr->y1_addr= TmpAddr->cr0_addr+Slc_CSize;
        TmpAddr->cb1_addr=TmpAddr->y1_addr+Slc_YSize;
        TmpAddr->cr1_addr=TmpAddr->cb1_addr+Slc_CSize;

        TmpAddr->y2_addr= TmpAddr->cr1_addr+Slc_CSize;
        TmpAddr->cb2_addr=TmpAddr->y2_addr+Slc_YSize;
        TmpAddr->cr2_addr=TmpAddr->cb2_addr+Slc_CSize;
    }
    else if(OPPBnum==1)
    {
        TmpAddr->y0_addr= StartAddr;
        TmpAddr->cb0_addr=TmpAddr->y0_addr+Slc_YSize;
        TmpAddr->cr0_addr=TmpAddr->cb0_addr+Slc_CSize;
    }
    TmpAddr->out_format=OutFormat;

}

UINT GetSlcBufAddr(void)
{
    return ippbuf_GetBuf(IPPBUF_CAP_SLC_BUF);//SlcBufAddr;
}

//#NT#2009/11/04#Yuppie Lin -begin
//#NT#Modified, due to ime_Slice_En(ENABLE) is recycled by ime driver
/*
void TrgImeSlice(void)
{
    //ime_Slice_En(ON);
}
*/
//#NT#2009/11/04#Yuppie Lin -end
void jpg_SliceCfg(UINT JpgYAddr,UINT JpgCbAddr,UINT JpgCrAddr,UINT ImeHsize,UINT ImeVsize,UINT JpgOutputAddr,UINT JpgBufSize, UINT8 JpgQuality)
{
    UINT        JPG_BSize;
    JPGCmpParam    JPGParam;
    char        ER_Code;

    //Address to be setup in 1st time trigger
    JPGParam.RawImageAddr[0] = JpgYAddr;            //Raw Image address Y
    JPGParam.RawImageAddr[1] = JpgCbAddr;           //Raw Image address CB
    JPGParam.RawImageAddr[2] = JpgCrAddr;           //Raw Image address CR
    JPGParam.JPGBSAddr = JpgOutputAddr;             //Compress Bitstream location,
    JPG_BSize = JpgBufSize;                         //assume 3 M buffer size..
    JPGParam.JPGBSBufSize = &JPG_BSize;
    JPGParam.Width = ImeHsize;
    JPGParam.Height =ImeVsize;
    JPGParam.Mode  = JPEG_FORMAT_422 | JPG_Enc_SrcSliceMode;
    JPGParam.bOuputJpgDC = FALSE;
    //#NT#2010/02/24#Meg Lin -begin
    //#NT#add lineoffset
    JPGParam.LineOffsetY = JPGParam.Width;
    JPGParam.LineOffsetCbCr = JPGParam.Width/2;
    //#NT#2010/02/24#Meg Lin -end
    ER_Code = JpegEncOnePic(&JPGParam, JpgQuality);
}

void ipe_SliceCfg(STR_STRIPE_INFOR *Hstripe,STR_STRIPE_INFOR *Vstripe,UW InAddr0,UINT cfapat,UINT ipe_omat, JobTypeEnum Job)
{
    IPE_Para.HStripe = *Hstripe;
    IPE_Para.VStripe = *Vstripe;
    IPE_Para.Add0 = InAddr0;
    IPE_Para.OutFormat = ipe_omat;
    IPE_Para.CfaPat = cfapat;
    switch(gImageAlgInfo.PreCapBitDepth)
    {
        case _PreBitDepth_10 :
            IPE_Para.BitDepth = IPE_PACK_10bit;
            IPE_Para.InLineOfs = GetIPPSizeInfor(_PreCropCapHsize) * 10 / 8;
            break;

        case _PreBitDepth_12 :
            IPE_Para.BitDepth = IPE_PACK_12bit;
            IPE_Para.InLineOfs = GetIPPSizeInfor(_PreCropCapHsize) * 12 / 8;
            break;

        case _PreBitDepth_16 :
            IPE_Para.BitDepth = IPE_PACK_16bit;
            IPE_Para.InLineOfs = GetIPPSizeInfor(_PreCropCapHsize) * 16 / 8;
            break;

        default :
        case _PreBitDepth_8 :
            IPE_Para.BitDepth = IPE_PACK_08bit;
            IPE_Para.InLineOfs = GetIPPSizeInfor(_PreCropCapHsize);
            break;
    }

    IPPMgrReq.IpeParam = &IPE_Para;

    if ( IPE_BeginIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \n"));

    if ( IPE_AllocatResourceIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \n"));

    SenFlowFunc(IPE_CAP_SETTING, &gSenCBInfo);
    //#NT#2011/01/18#JJ Huang -begin
    //#NT# For Cap IME Chroma Filter/Curve
    SenFlowFunc(IME_Chrom_Cap_SETTING, &gSenCBInfo);
    //#NT#2011/01/18#JJ Huang -end

    if ( IPE_ExecIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
}


//#NT#2012/01/03#Jarkko Chang -begin
//#NT# modify for DVI using MST
void ime_to_ipe_Stripe_trans(STR_STRIPE_INFOR *Hstripe,STR_STRIPE_INFOR *Vstripe)
{
    //Get IME stripe information
    GetStripe=ime_getHstripe();
    Hstripe->n=GetStripe.n;
    Hstripe->l=GetStripe.l;
    Hstripe->m=GetStripe.m;
    Hstripe->olap=IPE_OLAP_ON;  //spark
    GetStripe=ime_getVstripe();
    Vstripe->n=GetStripe.n;
    Vstripe->l=GetStripe.l;
    Vstripe->m=GetStripe.m;
    Vstripe->olap = IPE_OLAP_ON;  //spark

    //IME to IPE stripe convert
    if (GetIPPFuncInfor(DVI_MODE_EN) == DISABLE)
    {
        Hstripe->n+=1;
        Hstripe->l+=1;
        Vstripe->n+=8;
    }
}
//#NT#2012/01/03#Jarkko Chang -end

void ime_SliceCfg(IME_FBADDR_PRAM *pIME_Para1,IME_FBADDR_PRAM *pIME_Para2, JobTypeEnum Job)
{
    IPPMgrReq.ImeParam1 = pIME_Para1;
    IPPMgrReq.ImeParam2 = pIME_Para2;

    if (IME_BeginIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_BeginIPP() error \n"));

    if (IME_AllocatResourceIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("IPE_AllocatResourceIPP() error \n"));

    if (IME_ExecIPP(&IPPMgrReq, Job) != IPP_OK)
        debug_err(("SIE_ExecIPP() error \n"));
}

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//--------------------------------------------------------------------------------------------------------------------------
void Time2_Line_Pixel2(UINT32 TimeUs, UINT32 PixClkKHz, UINT32 HDTotal, UINT16 *Line, UINT16 *Pixel2)
{
    UINT32 Temp, LineTmp, Pixel2Tmp;

    Temp = (TimeUs * PixClkKHz) / 1000;
    LineTmp = Temp / HDTotal;
    Pixel2Tmp = (Temp - (LineTmp * HDTotal))/2;
    *(UINT16 *)Line = LineTmp;
    *(UINT16 *)Pixel2 = Pixel2Tmp;
}
//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
UINT GetDPAdd_Prv(void)
{
    return gDP_Add_Prv;
}

UINT GetDPAdd_Cap(void)
{
    return gDP_Add_Cap;
}

void SetDPAdd(UINT DP_Prv, UINT DP_Cap)
{
    gDP_Add_Prv = DP_Prv;
    gDP_Add_Cap = DP_Cap;
}
UINT32 GetCFAPattern(void)
{
    return regCFA_Pattern;
}
UINT32 GetSieCapXCenter(void)
{
    return SIE_CAP_XCENTER;
}
UINT32 GetSieCapYCenter(void)
{
    return SIE_CAP_YCENTER;
}
UINT32 GetVDSyncPrv(void)
{
    return VD_Sync_Prv;
}
UINT32 GetHDSyncPrv(void)
{
    return HD_Sync_Prv;
}
UINT32 GetVDTotalPrv(void)
{
    return VD_Total_Prv;
}
UINT32 GetHDTotalPrv(void)
{
    return HD_Total_Prv;
}
UINT32 GetACTStartXPrv(void)
{
    return ACT_STRX_Prv;
}
UINT32 GetACTStartYPrv(void)
{
    return ACT_STRY_Prv;
}
//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
void pre_Dram2Dram(SIE_PRE_PARAM *SiePreParam)
{
    IPPMgrReq.SIE_PRE_D2D = SiePreParam;
    SenFlowFunc(PRE_D2D_SETTING, &gSenCBInfo);
    dis_int(_INT_PRE);
    pll_setClockRate(PLL_CLKSEL_PRE_CLKSRC, PLL_CLKSEL_PRE_CLKSRC_PRE);
    pll_setClockRate(PLL_CLKSEL_PRE, PLL_CLKSEL_PRE_96);
    PRE_BeginIPP(&IPPMgrReq, PRE_DRAM_RAWIMGIN_JOB);
    PRE_ExecIPP(&IPPMgrReq, PRE_DRAM_RAWIMGIN_JOB);
    PRE_EndIPP(&IPPMgrReq, PRE_DRAM_RAWIMGIN_JOB);
    ena_int(_INT_PRE);
}

//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
//IPE TIFF configuration
void ipe_TIFFCfg(unsigned long InAddr0,
                unsigned long OutYAddr0,
                unsigned long InHsize,
                unsigned long InVsize,
                unsigned long cfapat,
                unsigned long ipe_omat,
                unsigned long ipe_dest)
{
#if 0
    unsigned long features=0,int_en=0;
    STR_IMG_SIZE InImgSz,OutImgSz;
    unsigned long OutYsize,OutCsize;

    ipe_Stop();
    ipe_ResetOn();
    ipe_ResetOff();

    //Input size setting
    InImgSz.hsize=InHsize;//2880;
    InImgSz.vsize=InVsize;//2148;

    //Config capture IPE registers
    //ipe_SetIOTarget(IPE_PARA_DMA,ipe_dest);//DMA->IPE->IME

    //JJ_UI_FileFmt
    if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_8)
        ipe_SetInPacking(IPE_PACK_08bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_10)
        ipe_SetInPacking(IPE_PACK_10bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_12)
        ipe_SetInPacking(IPE_PACK_12bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else if(gImageAlgInfo.PreCapBitDepth==_PreBitDepth_16)
        ipe_SetInPacking(IPE_PACK_16bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit
    else
        ipe_SetInPacking(IPE_PACK_08bit);            //0=12-bit pack mode, 1=10-bit, 2=8-bit, 3=16-bit

    ipe_SetOutFormat(ipe_omat,0);//ipe_omat: 0=444, 1=422, 2=420
    ipe_SetIOPPB(IPE_DISABLE,0,0);//parameters: (ppbi_en,ppbi_ini,ppbo_num);

    OutImgSz=ipe_SizeCfg(InImgSz,AUTO_FRAME_MODE,cfapat,IPE_LINEX9,IPE_OLAP_ON,IPE_OLAP_ON,IPE_ENABLE);    //Parameters:(InIMG,ipe_mode,cfapat,lbufnum,holap,volap)

    OutYsize=(OutImgSz.hsize)*(OutImgSz.vsize);
    switch(ipe_omat)
    {
        case IPE_YCC444:
            OutCsize=OutYsize;
            break;
        case IPE_YCC422:
            OutCsize=OutYsize/2;
            break;
        case IPE_YCC420:
            OutCsize=OutYsize/4;
            break;
        default:
            OutCsize=OutYsize;
            break;
    }

    //IO Dram configuration
    //JJ_UI_FileFmt
    ipe_SetDmaInAddr(InAddr0,0,InImgSz.hsize*gImageAlgInfo.PreCapBitDepth/8);
    //ipe_SetDmaOutAddr0(OutYAddr0,(OutYAddr0+OutYsize),(OutYAddr0+OutYsize+OutCsize),OutImgSz.hsize);

    features|=IPE_NR_EN;
    features|=IPE_RLPF_EN;
    features|=IPE_GLPF_EN;
    features|=IPE_BLPF_EN;
    features|=IPE_CCR_EN;
    features|=IPE_CTRL_EN;
    features|=IPE_HADJ_EN;
    features|=IPE_CADJ_EN;
    features|=IPE_CADJ_YENH_EN;
    features|=IPE_CADJ_YCON_EN;
    features|=IPE_CADJ_CCON_EN;
    features|=IPE_VACC_EN;
    features|=IPE_CGAIN_EN;//Adrian0623
    ipe_SetFeature(features);//Enable capture IPE blocks

    ipe_EnableInt(int_en);//Enable capture IPE interrupt

    //ipe_SetNRWeight(10,45,8,40);
    ipe_SetNRMap(0,0,0);
    ipe_SetColorInterpolation(4, 4, 0);
    ipe_SetColorCorrect(0,Normal_CC);
    ipe_SetYcolorAdj(120,128);
    //ipe_SetEdgeExtractHV(8,8,0,0,-1,-1);
    //ipe_SetEdgeExtractDiag(0,-1,-1,4);
    //ipe_SetEdgeMapping(8,1023,1023,1023,0);
    ipe_SetColorSup(32);

    //ipe_CapParameter(IPEISO100);

    //Clear int status
    ipe_ClearInt(IPE_INT_ALL);
#endif
}

//IME TIFF configuration
void ime_TIFFCfg(STR_YCBCR_ADDR in_addr,
                STR_YCBCR_ADDR out_addr,
                unsigned int in_format,
                unsigned int out_format,
                STR_SCALE_SIZE_INFO io_size,
                BOOL src)
{
#if 0
    ime_reset(ON);
    ime_reset(OFF);
    ime_start(OFF);
    ime_load();
    //ime_freeRun(0);

    ime_setSource(src);//src  0:IPE to IME, 1:DMA to IME
    ime_setPath(1);//path 0: both scale paths, 1: only one scale path, 2: only Bicubic scale path
    ime_Bilinear(0);//bil 0: bicubic, 1: bilinear
    ime_InputFormat(in_format);//format 0: 444, 1:422, 2:420
    ime_StripeMode(1);//sten 0: single stripe, 1: multi stripe
    ime_InputPingpong(0);//ippbe 0: ping-pong buffer disable, 1:ping-pong buffer enable
    ime_Path1OutputPingpong(0);//oppbe1 0:one output buffer, 1:two output buffer, 2:three output buffer
    ime_Path2OutputPingpong(0);//oppbe2 0:one output buffer, 1:two output buffer, 2:three output buffer
    ime_SetCST(103,88,183,198);//CST_PROM0,CST_PROM1,CST_PROM2,CST_PROM3
    ime_ICSTSet(1,1);//icst_en 0:disable, 1:enable  //icst_opt 0:RGB transfer to YCBCR, 1:YCBCR transfer to RGB

    //Input
    ime_InputYcbCrAddr0(in_addr.y_addr, in_addr.cb_addr, in_addr.cr_addr);
    ime_InputYcbCrAddr1(in_addr.y_addr, in_addr.cb_addr, in_addr.cr_addr);
    ime_InputYcbCrOffset(io_size.in_h);

    set_ImeInputStripe();
    //Output path1
    ime_OutputPath1SizeScale(io_size.in_h, io_size.in_v, io_size.out_h, io_size.out_v);
    //ime_Path1HAverage(0);
    ime_Path1OutputFormat(out_format);
    ime_Path1HOutputSize(io_size.out_h-1);
    ime_Path1VOutputSize(io_size.out_v-1);
    ime_OutputPath1YcbCrAddr0(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath1YcbCrAddr1(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath1YcbCrAddr2(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath1YcbCrOffset(io_size.out_h);

    //Output path2
    ime_OutputPath2SizeScale(io_size.in_h, io_size.in_v, io_size.out_h, io_size.out_v);
    //ime_Path2HAverage(0);
    ime_Path2OutputFormat(out_format);
    ime_Path2HOutputSize(io_size.out_h-1);
    ime_Path2VOutputSize(io_size.out_v-1);
    ime_OutputPath2YcbCrAddr0(out_addr.y_addr, out_addr.cb_addr, out_addr.cr_addr);
    ime_OutputPath2YcbCrOffset(io_size.out_h);

    ime_load();
    ime_ClearFrameEnd();
    ime_StatustClear(Ime_INT_ALL);
#endif
}

//For RGB TIFF FileFmt Header
void Set_RGB_TIFF_Header(PTIFF_Param pTIFFparam, PRGB_IMAGE_TIFF_HEADER pRGB_TIFFhearder)
{
    UINT32 ImgHsize, ImgVsize, StripByteCounts,StripOffsets1,StripOffsets2,StripOffsets3,temp;
    ImgHsize = pTIFFparam->ImageHsize;
    ImgVsize = pTIFFparam->ImageVsize;
    StripByteCounts = pTIFFparam->PlaneByteCounts;
    StripOffsets1 = 228;//RGB_TIFF_Header length
    StripOffsets2 = StripOffsets1+StripByteCounts;
    StripOffsets3 = StripOffsets2+StripByteCounts;

    //set ImageWidth value
    temp = ( (0xff000000)&(ImgHsize) );
    temp = (temp>>24);
    pRGB_TIFFhearder->ImageWidth.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(ImgHsize) );
    temp = (temp>>16);
    pRGB_TIFFhearder->ImageWidth.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgHsize) );
    temp = (temp>>8);
    pRGB_TIFFhearder->ImageWidth.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgHsize) );
    pRGB_TIFFhearder->ImageWidth.Value_Offset[3] = (UCHAR)(temp);

    //set ImageLength value
    //set RowsPerStrip value
    temp = ( (0xff000000)&(ImgVsize) );
    temp = (temp>>24);
    pRGB_TIFFhearder->ImageLength.Value_Offset[0] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(ImgVsize) );
    temp = (temp>>16);
    pRGB_TIFFhearder->ImageLength.Value_Offset[1] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgVsize) );
    temp = (temp>>8);
    pRGB_TIFFhearder->ImageLength.Value_Offset[2] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgVsize) );
    pRGB_TIFFhearder->ImageLength.Value_Offset[3] = (UCHAR)(temp);
    pRGB_TIFFhearder->RowsPerStrip.Value_Offset[3] = (UCHAR)(temp);

    //set StripOffset2 value
    temp = ( (0xff000000)&(StripOffsets2) );
    temp = (temp>>24);
    pRGB_TIFFhearder->StripOffsetsValue[1][0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripOffsets2) );
    temp = (temp>>16);
    pRGB_TIFFhearder->StripOffsetsValue[1][1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripOffsets2) );
    temp = (temp>>8);
    pRGB_TIFFhearder->StripOffsetsValue[1][2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripOffsets2) );
    pRGB_TIFFhearder->StripOffsetsValue[1][3] = (UCHAR)(temp);

    //set StripOffset3 value
    temp = ( (0xff000000)&(StripOffsets3) );
    temp = (temp>>24);
    pRGB_TIFFhearder->StripOffsetsValue[2][0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripOffsets3) );
    temp = (temp>>16);
    pRGB_TIFFhearder->StripOffsetsValue[2][1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripOffsets3) );
    temp = (temp>>8);
    pRGB_TIFFhearder->StripOffsetsValue[2][2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripOffsets3) );
    pRGB_TIFFhearder->StripOffsetsValue[2][3] = (UCHAR)(temp);

    //set StripByteCounts value
    temp = ( (0xff000000)&(StripByteCounts) );
    temp = (temp>>24);
    pRGB_TIFFhearder->StripByteCountsValue[0][0] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][0] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripByteCounts) );
    temp = (temp>>16);
    pRGB_TIFFhearder->StripByteCountsValue[0][1] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][1] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripByteCounts) );
    temp = (temp>>8);
    pRGB_TIFFhearder->StripByteCountsValue[0][2] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][2] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripByteCounts) );
    pRGB_TIFFhearder->StripByteCountsValue[0][3] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[1][3] = (UCHAR)(temp);
    pRGB_TIFFhearder->StripByteCountsValue[2][3] = (UCHAR)(temp);
}

//For JPEG TIFF FileFmt Header
void Set_JPEG_TIFF_Header(PTIFF_Param pTIFFparam, PJPEG_IMAGE_TIFF_HEADER pJPEG_TIFFhearder)
{
    UINT32 ImgHsize, ImgVsize, StripByteCounts,temp;
    ImgHsize = pTIFFparam->ImageHsize;
    ImgVsize = pTIFFparam->ImageVsize;
    StripByteCounts = pTIFFparam->ImageByteCounts;

    //set ImageWidth value
    //set TileWidth value
    temp = ( (0xff000000)&(ImgHsize) );
    temp = (temp>>24);
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[0] = (UCHAR)(temp);;

    temp = ( (0x00ff0000)&(ImgHsize) );
    temp = (temp>>16);
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgHsize) );
    temp = (temp>>8);
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgHsize) );
    pJPEG_TIFFhearder->ImageWidth.Value_Offset[3] = (UCHAR)(temp);

    //set ImageLength value
    //set RowsPerStrip value
    temp = ( (0xff000000)&(ImgVsize) );
    temp = (temp>>24);
    pJPEG_TIFFhearder->ImageLength.Value_Offset[0] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(ImgVsize) );
    temp = (temp>>16);
    pJPEG_TIFFhearder->ImageLength.Value_Offset[1] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(ImgVsize) );
    temp = (temp>>8);
    pJPEG_TIFFhearder->ImageLength.Value_Offset[2] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(ImgVsize) );
    pJPEG_TIFFhearder->ImageLength.Value_Offset[3] = (UCHAR)(temp);
    pJPEG_TIFFhearder->RowsPerStrip.Value_Offset[3] = (UCHAR)(temp);

    //set StripByteCounts value
    temp = ( (0xff000000)&(StripByteCounts) );
    temp = (temp>>24);
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[0] = (UCHAR)(temp);

    temp = ( (0x00ff0000)&(StripByteCounts) );
    temp = (temp>>16);
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[1] = (UCHAR)(temp);

    temp = ( (0x0000ff00)&(StripByteCounts) );
    temp = (temp>>8);
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[2] = (UCHAR)(temp);

    temp = ( (0x000000ff)&(StripByteCounts) );
    pJPEG_TIFFhearder->StripByteCounts.Value_Offset[3] = (UCHAR)(temp);
}

//TIFF header copy
UINT TIFF_RGB_CopyHeader(UINT TIFFHeaderAddr,
                        UINT TIFFHsize,
                        UINT TIFFVsize)
{
    TIFF_Param TIFFParam;
    UINT TIFFHearderSize;

    TIFFParam.ImageHsize=TIFFHsize;
    TIFFParam.ImageVsize=TIFFVsize;
    TIFFParam.ImageByteCounts=TIFFHsize*TIFFVsize*3;
    TIFFParam.PlaneByteCounts=TIFFHsize*TIFFVsize;
    TIFFHearderSize=sizeof(RGB_TIFF_Header);
    Set_RGB_TIFF_Header(&TIFFParam, &RGB_TIFF_Header);

    memcpy((void*)TIFFHeaderAddr, (void*)&RGB_TIFF_Header, TIFFHearderSize);

    return TIFFHearderSize;
}

#define    S_Y(addr,vsize,i,j)             *(volatile unsigned char *)(addr+vsize*j+i)
void    Y_LPF1( UINT Addr,
                UINT Hsize,
                UINT Vsize,
                UINT LineOffset)
{
    int     i=0,j=0;

    for(j=1;j<Vsize-1;j++)
    {
        for(i=1;i<Hsize-1;i++)
        {
        S_Y(Addr,LineOffset,i,j)=(  (UINT)(S_Y(Addr,LineOffset,(i),(j))*4)
                                    +(UINT)(S_Y(Addr,LineOffset,(i-1),(j-1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i-1),(j+1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i+1),(j-1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i+1),(j+1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i),(j-1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i),(j+1)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i-1),(j)))
                                    +(UINT)(S_Y(Addr,LineOffset,(i+1),(j)))
                                    )/12;


        }
    }
}
#define    getW(addr)             *(volatile unsigned long *)(addr)
void    Y_LPF2( UINT Addr,
                UINT Hsize,
                UINT Vsize,
                UINT LineOffset)
{
    UINT8   ucBuff[12], ucBuffOut[4];
    UINT32*  puiBuff[3];
    UINT32* puiBuffOut;
    UINT32 iAddr, jAddr;
    UINT32  jLoopMax;
    int     i=0,j=0, k=0;

    puiBuff[0] = (UINT32 *)&ucBuff[0];
    puiBuff[1] = (UINT32 *)&ucBuff[4];
    puiBuff[2] = (UINT32 *)&ucBuff[8];
    puiBuffOut = (UINT32 *)&ucBuffOut[0];

    jLoopMax = Vsize-1;

    iAddr = Addr;

    for(i=0;i<Hsize;i+=4)
    {
        jAddr = iAddr;

        *(UINT32 *)puiBuff[0]=getW(jAddr);
        jAddr += LineOffset;
        *(UINT32 *)puiBuff[1]=getW(jAddr);
        jAddr += LineOffset;
        k=2;

        for(j=1;j<jLoopMax;j++)
        {
            *(UINT32 *)puiBuff[k]=getW(jAddr+LineOffset);
            k=(k+1)%3;

            ucBuffOut[0] = ((UINT32)ucBuff[0]+ucBuff[4]+ucBuff[8])/3;
            ucBuffOut[1] = ((UINT32)ucBuff[1]+ucBuff[5]+ucBuff[9])/3;
            ucBuffOut[2] = ((UINT32)ucBuff[2]+ucBuff[6]+ucBuff[10])/3;
            ucBuffOut[3] = ((UINT32)ucBuff[3]+ucBuff[7]+ucBuff[11])/3;

            getW(jAddr)=*puiBuffOut;

            jAddr += LineOffset;
        }
        iAddr += 4;
    }
}
void    Y_LPF3( UINT Addr,
                UINT Hsize,
                UINT Vsize,
                UINT LineOffset,
                UINT BuffAddr)
{
    UINT32  BuffA;

    {//mem. assign
        BuffA = BuffAddr;
    }

    {//step 1:  copy to buffer
        grph_open();
        grph_enableDMA();
        grph_setImg1(   Addr,
                        LineOffset,
                        Vsize,
                        Hsize);
        grph_setImg2(   BuffA,
                        LineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        GRPH_DST_2,         //GRPH_DST_SEL dstSel
                        0x00,       //GRPH_AOP_MODE aopMode
                        0x00);  //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        grph_close();//mark this line or the second picture will make the cam. abnormal
    }

    {//step 2:  [n-1] + [n+1]
        grph_open();
        grph_enableDMA();
        grph_setImg1(   BuffA+LineOffset*2,
                        LineOffset,
                        Vsize-2,
                        Hsize);
        grph_setImg2(   Addr,
                        LineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        0,          //GRPH_DST_SEL dstSel
                        0x0D,       //GRPH_AOP_MODE aopMode
                        0x09);  //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        grph_close();//mark this line or the second picture will make the cam. abnormal
    }
    {//step 3:  +[n]
        grph_open();
        grph_enableDMA();
        grph_setImg1(   Addr+LineOffset,
                        LineOffset,
                        Vsize-2,
                        Hsize);
        grph_setImg2(   BuffA+LineOffset*2,
                        LineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        0,          //GRPH_DST_SEL dstSel
                        0x0D,       //GRPH_AOP_MODE aopMode
                        0x2e);//0x35);  //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        grph_close();//mark this line or the second picture will make the cam. abnormal
    }
}

/*
This RawBinning()   function is for 8 bit Raw only
*/

UINT RawBinning(UINT SrcAddr,
                UINT SrcLineOffset,
                UINT Hsize,
                UINT Vsize,
                UB Mode)//  0:V_bin         1:V_bin+H_bin(only for NT96432)
{
//step 1
    grph_open();
    grph_enableDMA();
    grph_setImg1(   SrcAddr,
                    SrcLineOffset,
                    Vsize,
                    SrcLineOffset);
/*  grph_setImg2(   SrcAddr+2,
                    SrcLineOffset);
*/
    grph_setImg2(   SrcAddr+2*SrcLineOffset,
                    SrcLineOffset);
    grph_setAOP(    FALSE,          //BOOL b16BitPrc
                    0,          //GRPH_DST_SEL dstSel
                    GRPH_AOP_PLUS_SHF,//0x03,       //GRPH_AOP_MODE aopMode
                    0);//ColorKey); //UINT32 uiProperty == Color Key
#if 1
//#NT#20091012#ethanalu -begin
//last two line is garbage
    grph_setImg1(SrcAddr+(Vsize-4)*SrcLineOffset ,
                    SrcLineOffset,
                    2,
                    SrcLineOffset);
    grph_setImg2(   SrcAddr+(Vsize-2)*SrcLineOffset,
                    SrcLineOffset);
    grph_setAOP(    FALSE,          //BOOL b16BitPrc
                    1,          //GRPH_DST_SEL dstSel
                    GRPH_AOP_A_COPY,//0x03,       //GRPH_AOP_MODE aopMode
                    0);//ColorKey); //UINT32 uiProperty == Color Key
//#NT#20091012#ethanlau -end
#endif
    grph_disableDMA();//DMA seems to keep moving unless being disabled,
    grph_swReset();
    grph_close();//mark this line or the second picture will make the cam. abnormal


/*    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)SrcAddr,//    UB *pBuf,
                    Hsize*Vsize,//UW BufSize,
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
*/
    if(Mode==1)
    {
//step 2
    grph_open();
    grph_enableDMA();
    grph_setImg1(   SrcAddr,
                    SrcLineOffset,
                    Vsize,
                    SrcLineOffset);
#if 0
    grph_setImg2(   SrcAddr+2,
                    SrcLineOffset);
#else //adjust for 16bit input
    grph_setImg2(   SrcAddr+2*2,
                    SrcLineOffset);
#endif
    grph_setAOP(    FALSE,          //BOOL b16BitPrc
                    0,          //GRPH_DST_SEL dstSel
                    GRPH_AOP_PLUS_SHF,//0x03,       //GRPH_AOP_MODE aopMode
                    0);//ColorKey); //UINT32 uiProperty == Color Key
#if 1
//#NT#20091012#ethanalu -begin
//last two column is garbage , for 16bit input
    grph_setImg1(SrcAddr+SrcLineOffset-4*2,
                    SrcLineOffset,
                    Vsize,
                    2*2);
    grph_setImg2(   SrcAddr+SrcLineOffset-2*2,
                    SrcLineOffset);

    grph_setAOP(    FALSE,          //BOOL b16BitPrc
                    1,          //GRPH_DST_SEL dstSel
                    GRPH_AOP_A_COPY,//0x03,       //GRPH_AOP_MODE aopMode
                    0);//ColorKey); //UINT32 uiProperty == Color Key
//#NT#20091012#ethanlau -end
#endif

    grph_disableDMA();//DMA seems to keep moving unless being disabled,
    grph_swReset();
    grph_close();//mark this line or the second picture will make the cam. abnormal
/*
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)SrcAddr,//    UB *pBuf,
                    Hsize*Vsize,//UW BufSize,
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);//UW TimeOut)FST_TIME_INFINITE
*/
    }


    return E_OK;
}

UINT YGaussianBlur(UINT YAddr, //YAddr
                UINT LineOffset,//CbCr lineoffset uint:byte
                UINT Hsize, //CbCr Hsize
                UINT Vsize, //CbCr Vsize
                UB  YRadius)// pixel
{
    UB  i;

    if(YRadius == 0)
        return E_OK;
    grph_open();

    for(i=0;i<YRadius;i++)
    {
//smooth horizontal
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(   YAddr,
                        Hsize,
                        Vsize,
                        Hsize-1);
        grph_setImg2(   YAddr+1,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
//smooth vertical
        if(i<5)
        {
        grph_enableDMA();
        grph_setImg1(   YAddr,
                        Hsize,
                        Vsize-1,
                        Hsize);
        grph_setImg2(   YAddr+(1)*LineOffset,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        }

    }

    grph_close();

    return  E_OK;
}

UINT CbCrGaussianBlur(UINT CbAddr,//CrAddr
                UINT CrAddr, //CrAddr
                UINT LineOffset,//CbCr lineoffset uint:byte
                UINT Hsize, //CbCr Hsize
                UINT Vsize, //CbCr Vsize
                UB  CbRadius,// pixel
                UB  CrRadius)// pixel
{
    UB  i;

    if((CbRadius == 0) && (CrRadius == 0))
        return E_OK;
    grph_open();

    for(i=0;i<CbRadius;i++)
    {
//smooth horizontal
        grph_swReset();
        grph_enableDMA();
        grph_setImg1(   CbAddr,
                        Hsize,
                        Vsize,
                        Hsize-4);
        grph_setImg2(   CbAddr+4,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        (((i+1)%2)<<5),         //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
//smooth vertical
        if(i<8)
        {
        grph_enableDMA();
        grph_setImg1(   CbAddr,
                        Hsize,
                        Vsize-1,
                        Hsize);
        grph_setImg2(   CbAddr+(1)*LineOffset,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        }

    }

    for(i=0;i<CrRadius;i++)
    {
//smooth horizontal
        grph_enableDMA();
        grph_setImg1(   CrAddr,
                        Hsize,
                        Vsize,
                        Hsize-4);
        grph_setImg2(   CrAddr+4,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        (((i+1)%2)<<5),         //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
//smooth vertical
        if(i<8)
        {
        grph_enableDMA();
        grph_setImg1(   CrAddr,
                        Hsize,
                        Vsize-1,
                        Hsize);
        grph_setImg2(   CrAddr+(1)*LineOffset,
                        LineOffset);
        grph_setAOP(    FALSE,          //BOOL b16BitPrc
                        ((i+1)%2)<<5,           //GRPH_DST_SEL dstSel
                        GRPH_AOP_BLENDING,//0x03,       //GRPH_AOP_MODE aopMode
                        GRPH_BLD_WA_PER_2|GRPH_BLD_WB_PER_2);//ColorKey);   //UINT32 uiProperty == Color Key
        grph_disableDMA();//DMA seems to keep moving unless being disabled,
        grph_swReset();
        }

    }
    grph_close();

    return  E_OK;
}

void FrameMode_Process(UINT BayerAddr,
                    UINT BayerHsize,
                    UINT BayerVsize,
                    UINT ImeHsize,
                    UINT ImeVsize,
                    UINT ImeOutAddr )
{
/*
    unsigned int    CapRawAddr,CapIpeAddr;
    STR_YCBCR_ADDR  ime_inAddr,ime_outAddr;
    STR_SCALE_SIZE_INFO io_size;

    CapRawAddr=BayerAddr;
    CapIpeAddr=BayerAddr;

#if RaiseClock
    clk_open();
    *(volatile UINT32 *)0x98102034 = 0x07;
    clk_changeDMA(DMA_CLKSEL_CLK120,0x15,0x10);
    pll_setClockRate(IPE_CLKSEL, IPE_CLKSEL_CLK48);
    pll_setClockRate(IME_CLKSEL, IME_CLKSEL_CLK60);
#endif

    ipe_TIFFCfg(BayerAddr,//unsigned long InAddr0,
                CapIpeAddr,//unsigned long OutYAddr0,
                BayerHsize,//unsigned long InHsize,
                BayerVsize,//unsigned long InVsize,
                IPE_PATR,//unsigned long cfapat,
                IPE_YCC422,//Ipe_YCC444,//unsigned long ipe_omat,
                IPE_PARA_ENGINE);//destination

//#NT#2008/09/09#YH Lee -begin
//#NT#Clear warning
    //IME input not used, give dummy default
    ime_inAddr.y_addr=ImeOutAddr;
    ime_inAddr.cb_addr=ImeOutAddr;
    ime_inAddr.cr_addr=ImeOutAddr;
//#NT#2008/09/09#YH Lee -end

    //IME output RGB 444
    ime_outAddr.y_addr=ImeOutAddr;  //TIFF R plane
    ime_outAddr.cb_addr=ImeOutAddr+ImeHsize*ImeVsize; //TIFF G plane
    ime_outAddr.cr_addr=ImeOutAddr+ImeHsize*ImeVsize*2; //TIFF B plane

    io_size.in_h=BayerHsize-16;
    io_size.in_v=BayerVsize;
    io_size.out_h=ImeHsize;
    io_size.out_v=ImeVsize;

    ime_TIFFCfg(ime_inAddr,
                ime_outAddr,
                1,//input 422
                0,//output format, 0:444, 1:422
                io_size,
                0//0:IPE->IME,1:DMA->IME
                );

    ime_StatustClear(Ime_INT_ALL);
    ime_InitInt();
    ime_clrIntFlag();
    ime_start(1);
    ipe_ClearInt(IPE_INT_ALL);
    ipe_Start();
    ime_StatustClear(Ime_INT_ALL);

    ime_waitFlagFrameEnd();
    ipe_ClearInt(IPE_INT_ALL);

#if RaiseClock
    clk_changeDMA(DMA_CLKSEL_CLK48,0x31,0x0c);
    clk_close();
    pll_setClockRate(IPE_CLKSEL, IPE_CLKSEL_CLK40);
    pll_setClockRate(IME_CLKSEL, IME_CLKSEL_CLK40);
#endif
*/
}

// For sensor open I2C, Add by Iyen
void Sensor_OpenI2C(void)
{
    UINT erReturn;

    erReturn=i2c_open();

    if(erReturn != E_OK)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }

    i2c_enable();

    pinmux_select_i2c(PINMUX_I2C);

    erReturn = i2c_initMaster(96, 10, 4);

    if(erReturn != I2C_SUCCESS)
    {
        debug_err(("Error open I2C driver!!\r\n"));
    }
}

// For sensor close I2C, Add by Iyen
void Sensor_CloseI2C(void)
{
    i2c_disable();

    i2c_close();
}

BOOL  CPU_WriteGammaTab(UW *pre_Gamma_R,UW *pre_Gamma_G,UW *pre_Gamma_B)
{
//#NT#2009/09/02#Jarkko Chang -begin
    if (pre_CheckFunctionEnable(PRE_GAMMA_EN) && ipe_checkFunctionEnable(IPE_PREGAMMA_EN))
    {
        debug_err(("PRE & IPE Gamma CANNOT be on at the same time!\r\n"));
        return FALSE;
    }

    if (pre_CheckFunctionEnable(PRE_GAMMA_EN))
    {
        if(pre_CheckBusy()){
            debug_err(("CANNOT set Gamma when PRE is busy, please move to frame end!\r\n"));
            return FALSE;
        }
        else{
            pre_SetGamma(pre_Gamma_R, pre_Gamma_G, pre_Gamma_B);
            return TRUE;
        }
    }
    else if (ipe_checkFunctionEnable(IPE_PREGAMMA_EN))
    {
        if(ipe_checkBusy()){
            debug_err(("CANNOT set Gamma when IPE is busy, please move to frame end!\r\n"));
            return FALSE;
        }
        else{
            pre_SetGamma(pre_Gamma_R, pre_Gamma_G, pre_Gamma_B);
            return TRUE;
        }
    }
    else
    {
        pre_SetGamma(pre_Gamma_R, pre_Gamma_G, pre_Gamma_B);
        return TRUE;
    }
    /*
    //UINT uiFlag;
    while(1){

        if (pre_CheckFunctionEnable(PRE_GAMMA_EN) && pre_CheckBusy()){
            //return FALSE;
            TimerDelayMs(1);
        }else if (ipe_checkFunctionEnable(IPE_PREGAMMA_EN) && ipe_checkBusy()){
            //return FALSE;
            TimerDelayMs(1);
        }else if (pre_CheckFunctionEnable(PRE_GAMMA_EN)){
            //wai_flg(&uiFlag,FLG_ID_PHOTO,FLGPHOTO_FRAMEEND,TWF_ORW);
            pre_SetGamma(pre_Gamma_R, pre_Gamma_G, pre_Gamma_B);
            //return TRUE;//++ change to while loop
            break;
        }else if (ipe_checkFunctionEnable(IPE_PREGAMMA_EN)){
            //wai_flg(&uiFlag,FLG_ID_SLICE,FLGSLICETSK1_IMEFRAMEEND,TWF_ORW);
            pre_SetGamma(pre_Gamma_R, pre_Gamma_G, pre_Gamma_B);
            //return TRUE;
            break;
        }else {
            pre_SetGamma(pre_Gamma_R, pre_Gamma_G, pre_Gamma_B);
            //return TRUE;
            break;
        }
    }
        return TRUE;
    */
}

ER  SetIPEVarAccInfo(STR_IMG_SIZE *InputSize, STR_VA_INFOR *VAInfo, const UINT32 *VA_Table)
{
    UINT8 PowerNum;
    UINT32 hsize, vsize, stx, sty, hnf;

    //1. VA window hsz size should be even
    //2. To avoid va window V size to small, vsz can be odd number.
    //3. stx  should be even
    //4. sty can be odd number.
    if(VAInfo->vacc_num == WINDOWS_16X16)
    {
        hsize = ((InputSize->hsize - 16)/(VAC_WIN16));
        hsize = ((hsize >> 1) << 1);
        vsize = ((InputSize->vsize - 1)/(VAC_WIN16));

        stx = ((InputSize->hsize - 16) - hsize * VAC_WIN16) >> 1;
        stx = (stx >> 1) << 1;
        sty = ((InputSize->vsize - 1) - vsize * VAC_WIN16) >> 1;
    }
    else
    {
        hsize = ((InputSize->hsize - 16)/(VAC_WIN8));
        hsize = ((hsize >> 1) << 1);
        vsize = ((InputSize->vsize - 1)/(VAC_WIN8));

        stx = ((InputSize->hsize - 16) - hsize * VAC_WIN8) >> 1;
        stx = (stx >> 1) << 1;
        sty = ((InputSize->vsize - 1) - vsize * VAC_WIN8) >> 1;
    }

    //1. It should give specific hsz & stx table for using vacc hv seperate mode.
    //2. The hsz & stx was decided by IPE input size which show on dzoom table.
    if (VAInfo->vacc_hv == SEPARATE_HV){
        hsize = VA_Table[0];
        stx = VA_Table[1];
    }

    PowerNum = 0;
    while (1){
        PowerNum++;
        if ( (hsize >> PowerNum) == 0)
            break;
        if (PowerNum > 15)
            return E_SYS;
    }

    //Enhance VA value
    if (PowerNum >= 2)
        PowerNum -= 2;

    if (PowerNum > IPE_VAC_HNF_MINIMUN )
        hnf = PowerNum - IPE_VAC_HNF_MINIMUN;
    else
        hnf = 0;

    //Return results
    VAInfo->vacc_mode = VACC_AUTO_UPDATED;
    VAInfo->hsz = hsize;
    VAInfo->vsz = vsize;
    VAInfo->stx = stx;
    VAInfo->sty = sty;
    VAInfo->hnf = hnf;

    return E_OK;
}

//#NT#2008/06/11#Lawrence Ho -Begin
UINT32  CPU_UpdatedIPEVarAccTab(IPE_PARAM *IpeParam)
{
    UINT32 cnt_y, cnt_x, ReadData, ReadNumData, ReadVaccData;
    UINT32 BufStart;

    IPEModeInfo.OUTINFO.OutSrcSel = VADATA_TYPE;
    IPEModeInfo.OUTINFO.Adr0 = IpeParam->VAAddr;
    ipe_changeOutput(&IPEModeInfo.OUTINFO);

    ipe_waitDMAEnd(IPE_CLRFLG);

    BufStart = IpeParam->VAAddr;

    if ((IpeParam->VaccInfo.vacc_num == WINDOWS_8X8) && (IpeParam->VaccInfo.vacc_hv == COMBINED_HV))
    {

        for (cnt_y = 0 ; cnt_y < VAC_WIN8 ;  cnt_y++){
            for (cnt_x = 0 ; cnt_x < VAC_WIN8 ;  cnt_x++){
                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;
                // Horizontal & Vertical Combined

                    IPE_Vacc_TAb1[cnt_y][cnt_x] = (UINT32) (ReadNumData);
                    IPE_Vacc_TAb2[cnt_y][cnt_x] = (UINT32) (ReadVaccData);

                BufStart += 4;
            }
        }
                return TAb1_TAb2_UPDATED;
    }
    /*
    }else if ((IpeParam->VaccInfo.vacc_num == WINDOWS_8X8) && (IpeParam->VaccInfo.vacc_hv == SEPARATE_HV))
    {

        for (cnt_y = 0 ; cnt_y < VAC_WIN8 ;  cnt_y++){
            for (cnt_x = 0 ; cnt_x < VAC_WIN8 ;  cnt_x++){
                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                // Horizontal
                if (ReadNumData != 0)
                    IPE_Vacc_TAb1[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                else
                    IPE_Vacc_TAb1[cnt_y][cnt_x] = 0;

                BufStart += 4;

                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                // Vertical
                if (ReadNumData != 0)
                    IPE_Vacc_TAb2[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                else
                    IPE_Vacc_TAb2[cnt_y][cnt_x] = 0;

                }
                BufStart += 4;

            }
                    return TAb1_TAb2_UPDATED;

        */
        if ((IpeParam->VaccInfo.vacc_num == WINDOWS_16X16) && (IpeParam->VaccInfo.vacc_hv == COMBINED_HV))
        {

            for (cnt_y = 0 ; cnt_y < VAC_WIN16 ;  cnt_y++){
                for (cnt_x = 0 ; cnt_x < VAC_WIN16 ;  cnt_x++){
                ReadData = *(volatile UINT32 *)BufStart;
                ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;
                // Horizontal & Vertical Combined
                    IPE_Vacc_TAb3[cnt_y][cnt_x] = (UINT32) (ReadNumData);
                    IPE_Vacc_TAb3[cnt_y][cnt_x] = (UINT32) (ReadVaccData);
                    BufStart += 4;

                }
            }

                    return TAb3_TAb4_UPDATED;
        }
        /*
        }else if ((IpeParam->VaccInfo.vacc_num == WINDOWS_16X16) && (IpeParam->VaccInfo.vacc_hv == SEPARATE_HV))
        {

            for (cnt_y = 0 ; cnt_y < VAC_WIN16 ;  cnt_y++){
                for (cnt_x = 0 ; cnt_x < VAC_WIN16 ;  cnt_x++){
                    ReadData = *(volatile UINT32 *)BufStart;
                    ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                    ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                    // Horizontal
                    if (ReadNumData != 0)
                        PE_Vacc_TAb3[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                    else
                        PE_Vacc_TAb3[cnt_y][cnt_x] = 0;

                    BufStart += 4;
                    // Vertical
                    ReadData = *(volatile UINT32 *)BufStart;
                    ReadVaccData    = ReadData & IPE_VACC_VALUE_MSK;
                    ReadNumData = (ReadData & IPE_VACC_NUM_MSK)>>18;

                    if (ReadNumData != 0)
                        IPE_Vacc_TAb4[cnt_y][cnt_x] = (UINT32) (ReadVaccData/ReadNumData);
                    else
                        IPE_Vacc_TAb4[cnt_y][cnt_x] = 0;

                    }
                    BufStart += 4;

                }
                return TAb3_TAb4_UPDATED;
        }
        */
            return 0xFFFFFFFF; // Fail
}
//#NT#2008/06/11#Lawrence Ho -End

//#NT#2008/09/09#Lawrence Ho -Begin
//#NT#PERF Provide PRE VA Driver for user
BOOL  SetPREVarAccInfo(struct SIE_PRE_PARAM_STRUCT *SiePreParam)
{

    UINT8 PowerNum;

    SiePreParam->VaccIn.stx = (SiePreParam->PreCropSizeH)*3/8;
    SiePreParam->VaccIn.sty = (SiePreParam->PreCropSizeV)*3/8;
    SiePreParam->VaccIn.va_th = 2;
    SiePreParam->VaccIn.vacc_mode = VACC_AUTO_UPDATED;
    SiePreParam->VaccIn.hsz = (SiePreParam->PreCropSizeH - SiePreParam->VaccIn.stx/4)/5;
    SiePreParam->VaccIn.vsz = (SiePreParam->PreCropSizeV - SiePreParam->VaccIn.sty)/4  ;

    // hnf
    PowerNum = 0;
    while (1){
        PowerNum++;
        if ( ((SiePreParam->VaccIn.hsz)>>PowerNum) == 0)
            break;
        if (PowerNum > 11)
            return FALSE;
    }

    if (PowerNum > PRE_VAC_HNF_MINIMUN )//8
        PowerNum = PowerNum - PRE_VAC_HNF_MINIMUN;
    else
        PowerNum = 0;

    //debug_msg("hnf = %d\r\n",PowerNum);
    SiePreParam->VaccIn.hnf = PowerNum;

    // vnf
    PowerNum = 0;
    while (1){
        PowerNum++;
        if ( ((SiePreParam->VaccIn.vsz)>>PowerNum) == 0)
            break;
        if (PowerNum > 15)
            return FALSE;
    }
    if (PowerNum > 4)
        PowerNum = PowerNum - 4;
    else
        PowerNum = 0;

    //debug_msg("vnf = %d\r\n",PowerNum);

    SiePreParam->VaccIn.vnf = PowerNum ;


    // nnf
    PowerNum = 0;
    while (1){
        PowerNum++;
        if ( ((SiePreParam->VaccIn.hsz * SiePreParam->VaccIn.vsz)>>PowerNum) == 0)
            break;
        if (PowerNum > 19)
            return FALSE;
    }

    if (PowerNum > PRE_VAC_NNF_MINIMUN )//8
        PowerNum = PowerNum - PRE_VAC_NNF_MINIMUN;
    else
        PowerNum = 0;

    //debug_msg("nnf = %d\r\n",PowerNum);

    SiePreParam->VaccIn.nnf = PowerNum;

    pre_SetVaccParam(&SiePreParam->VaccIn);

        return TRUE;
}
//#NT#2008/09/09#Lawrence Ho -End

UINT32  CPU_UpdatedPREVarAccTab(struct SIE_PRE_PARAM_STRUCT *SiePreParam)
{
    UINT32 PreIntStatus;
    UINT32 VaccValueArray[5];
    UINT32 VaccNumArray[5];
    UINT32 cnt;

    PreIntStatus = pre_ReadIntrStatus();
    pre_ClrIntrStatus(PRE_INT_VA);

    while(1){
        PreIntStatus = pre_ReadIntrStatus();
        if ( (PreIntStatus & PRE_INT_VA) == PRE_INT_VA )
            break;
    }

    pre_GetVaccValue(VaccValueArray);
    pre_GetVaccNum(VaccNumArray);

    for ( cnt = 0; cnt <5 ; cnt++)
    {
        debug_err(("VaccNumArray[cnt]=%d\t",VaccNumArray[cnt]));
        debug_err(("VaccValueArray[cnt]=%d\n",VaccValueArray[cnt]));

        if (VaccNumArray[cnt] == 0)
            PRE_Vacc_TAb[cnt] = 0;
        else
            PRE_Vacc_TAb[cnt] = (VaccValueArray[cnt]<<4)/VaccNumArray[cnt];
    }
    return 1;
}
//#NT#2008/06/11#Lawrence Ho -End

ER User_VABegin(AF_IPEINFO *AFInfo)
{
    UINT32 ZoomIndex;
    const UINT32 *VA_Table;
//1. Should not disable all ipe interrupt here
//2. Open IPE DMAEND & FRAM interrupt for af used
    sie_waitVD(1);

    IPEModeInfo.IQINFO.VA_PARAM.vacc_src = AFInfo->vacc_src;
    IPEModeInfo.IQINFO.VA_PARAM.vacc_sqr = AFInfo->vacc_sqr;
    IPEModeInfo.IQINFO.VA_PARAM.vacc_gain = AFInfo->vacc_gain;
    IPEModeInfo.IQINFO.VA_PARAM.vacc_num = AFInfo->vacc_num;
    IPEModeInfo.IQINFO.VA_PARAM.va_th = AFInfo->va_th;

    if ( (AFInfo->vacc_outsel == WIN8x8_WEIGHT_VALUE) || (AFInfo->vacc_outsel == WIN8x8_WEIGHT_NUM)
         || (AFInfo->vacc_outsel == WIN16x16_WEIGHT_VALUE) || (AFInfo->vacc_outsel == WIN16x16_WEIGHT_NUM) )
    {
        IPEModeInfo.IQINFO.VA_PARAM.vacc_hv = SEPARATE_HV;
        aaa_SetVAWeight(AFInfo->TotalWeight, AFInfo->HdirWeight);
        ZoomIndex = Get_DzoomIndex();
        if (SIE_PRE_Para_prv.ModeSel == MONITOR_MODE_9PIX){
            VA_Table = VA_9PixTable[ZoomIndex-10];
        }
        else if (SIE_PRE_Para_prv.ModeSel == MONITOR_MODE_3){
            VA_Table = VA_Mod3Table[ZoomIndex-10];
        }
        else{
            debug_err(("User_VABegin() VA table not available for current sensor mode!\r\n"));
            return E_SYS;
        }
    }else {
        IPEModeInfo.IQINFO.VA_PARAM.vacc_hv = COMBINED_HV;
        VA_Table = NULL;
    }

    //IPE VA parameters
    SetIPEVarAccInfo(&IPEModeInfo.SIZEINFO.INSIZE, &IPEModeInfo.IQINFO.VA_PARAM, VA_Table);
    //IPE IQ parameters & IPE setting
    SenFlowFunc(IPE_PRV_AF_SETTING, &gSenCBInfo);
    //IPE DMA parameters
    IPEModeInfo.OUTINFO.OutSrcSel = VADATA_TYPE;
    IPEModeInfo.OUTINFO.Adr0 = ippbuf_GetBuf(IPPBUF_PRV_VA);
    ipe_changeOutput(&IPEModeInfo.OUTINFO);
    ipe_enableDMAOut(ENABLE);

    aaa_VAOutputSel(AFInfo->vacc_outsel, IPEModeInfo.OUTINFO.Adr0);

    //1. In the Beginnig, set PRE VA Parameter
    //2. Provide Edge extraction filter coefficiences when user select va_src = EEXT_ABS
    SetPREVarAccInfo(&SIE_PRE_Para_prv);
    pre_FunctionEnable(ENABLE,PRE_VACC_EN);

    //Make sure ipe register load to IPE Module.
    ime_clrIntFlag();
    ime_waitFlagFrameEnd();

    return E_OK;

}

int  User_VAEnd(AF_IPEINFO *AFInfo)
{
    ipe_enableDMAOut(DISABLE);
//#NT#2008/09/02#Lawrence Ho -begin
//#NT#PERF4 1. Make sure ipe register load to IPE Module.
//          2. The test show it is OK.
    ime_clrIntFlag();
    ime_waitFlagFrameEnd();
//#NT#2008/09/02#Lawrence Ho -end
  //  dis_int(_INT_IPE);
    //#NT#2008/07/21#Lawrence Ho -begin
    //#NT#PERF For speeding capture time, not to close vacc right now
    //sie_waitVD(3);
    //ipe_DisableFeature(IPE_VACC_EN);
    //ipe_EnableInt(!IPE_INT_FRM);        //clear interrupt
    ////ipe_ClearInt(IPE_INT_ALL);
    //#NT#2008/07/21#Lawrence Ho -end
    return 1;
}

UINT DP_Addr_Prv,DP_Addr_Cap;
extern INT32 abs(INT32 i);
//#NT#2008/07/25#YH Lee -begin
//#NT#ARRANGE Integrate IPP functions
void SetIPPFuncInfor(UINT32 FuncsID, UINT32 SetValue)
{
    switch(FuncsID)
    {
        case DFS_EN:
            gIPPFuncInfor.DfsEn = SetValue;
            break;

        case DFS_CNT:
            gIPPFuncInfor.DfsCnt = SetValue;
            break;

        case PRED2D_EN:
            gIPPFuncInfor.PRED2DEn = SetValue;
            break;

        case NLIGHT_CAP_EN:
            gIPPFuncInfor.NLightCapEn = SetValue;
            break;

        case NLIGHT_CAP_SEL:
            gIPPFuncInfor.NLightCapSel = SetValue;
            break;

        case FQV_EN:
            gIPPFuncInfor.FQVEn = SetValue;
            break;

        case CAP_ISO:
            gIPPFuncInfor.CapISO = SetValue;
            break;

        case CHRFILT_EN:
            gIPPFuncInfor.ChrFiltEn = SetValue;
            break;

        case CHRFILT_NUM:
            gIPPFuncInfor.ChrFiltNum = SetValue;
            break;

        case FQV_INPROCESS:
            gIPPFuncInfor.FQVInProcess = SetValue;
            break;
        case PRE_D2D_ROT_EN:
            gIPPFuncInfor.PreD2DRotEn = SetValue;
            break;
        case CHRCURVE_EN:
            gIPPFuncInfor.ChrCurveEn = SetValue;
            break;

        case CHRCBCURVE:
            gIPPFuncInfor.ChrCBCurve = (UINT32 *)SetValue;
            break;

        case CHRCRCURVE:
            gIPPFuncInfor.ChrCRCurve = (UINT32 *)SetValue;
            break;

        case CAPGAMMACURVE:
            gIPPFuncInfor.CapGamma = (UINT32 *)SetValue;
            break;
//#NT#20091008#ethanlau -begin
        case _StillAntiShakingIdx:
            gIPPFuncInfor.StillAntiShakingIdx = SetValue;
            break;
//#NT#20091009#ethanlau -end
//#NT#2011/09/05#Spark Chou -begin
//#NT#
        case DVI_MODE_EN:
            gIPPFuncInfor.DVIModeEn=SetValue;
            break;
//#NT#2011/09/05#Spark Chou -end
        default:
            debug_err(("-E- FuncsID(%d) > MaxID(%d)\r\n", FuncsID, IPPFUNC_MAXID));
            break;
    }
}

UINT32 GetIPPFuncInfor(UINT32 FuncsID)
{
    UINT32 ReturnValue = 0;
    switch(FuncsID)
    {
        case DFS_EN:
            ReturnValue = gIPPFuncInfor.DfsEn;
            break;

        case DFS_CNT:
            ReturnValue = gIPPFuncInfor.DfsCnt;
            break;

        case PRED2D_EN:
            ReturnValue = gIPPFuncInfor.PRED2DEn;
            break;

        case NLIGHT_CAP_EN:
            ReturnValue = gIPPFuncInfor.NLightCapEn;
            break;

        case NLIGHT_CAP_SEL:
            ReturnValue = gIPPFuncInfor.NLightCapSel;
            break;

        case FQV_EN:
            ReturnValue = gIPPFuncInfor.FQVEn;
            break;

        case CAP_ISO:
            ReturnValue = gIPPFuncInfor.CapISO;
            break;

        case CHRFILT_EN:
            ReturnValue = gIPPFuncInfor.ChrFiltEn;
            break;

        case CHRFILT_NUM:
            ReturnValue = gIPPFuncInfor.ChrFiltNum;
            break;

        case FQV_INPROCESS:
            ReturnValue = gIPPFuncInfor.FQVInProcess;
            break;
        case PRE_D2D_ROT_EN:
            ReturnValue = gIPPFuncInfor.PreD2DRotEn;
            break;
        case CHRCURVE_EN:
            ReturnValue = gIPPFuncInfor.ChrCurveEn;
            break;

        case CHRCBCURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.ChrCBCurve;
            break;

        case CHRCRCURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.ChrCRCurve;
            break;

        case CAPGAMMACURVE:
            ReturnValue = (UINT32)gIPPFuncInfor.CapGamma;
            break;
//#NT#20091008#ethanlau -begin
        case _StillAntiShakingIdx:
            ReturnValue = gIPPFuncInfor.StillAntiShakingIdx;
            break;
//#NT#20091009#ethanlau -end
//#NT#2011/09/05#Spark Chou -begin
//#NT#
        case DVI_MODE_EN:
            ReturnValue = gIPPFuncInfor.DVIModeEn;
            break;
//#NT#2011/09/05#Spark Chou -end
        default:
            debug_err(("-E- FuncsID(%d) > MaxID(%d)\r\n", FuncsID, IPPFUNC_MAXID));
            break;
    }

    return ReturnValue;
}
/*
void SetDFSEn(UCHAR Enable)
{
    gDFS_EN = Enable;
    gDFS_CNT = DFS_FIRSTPIC;
}

UCHAR GetDFSEn(void)
{
    return gDFS_EN;
}

void SetDFSCnt(UCHAR Cnt)
{
    gDFS_CNT = Cnt;
}

UCHAR GetDFSCnt(void)
{
    return gDFS_CNT;
}
*/
//#NT#2008/07/25#YH Lee -end

//#NT#2010/05/28#ethanlau -begin
//#NT#chg chromafilter from 433 to 632
void ChromaFilter(UINT CbAddr,//CbAddr
                UINT CrAddr, //CrAddr
                UINT bufferAddr,
                UINT LineOffset,//CbCr lineoffset uint:byte
                UINT Hsize, //CbCr Hsize
                UINT Vsize, //CbCr Vsize
                UINT *Chroma_Param,
                IME_FILT_PARA* ime_filt_para)
{

    grph_open();
    grph_swReset();
    grph_enableDMA();

    //Cb Ver
    grph_setImg1(CbAddr, LineOffset, Vsize, Hsize);
    grph_setImg2(CbAddr, Vsize);
    grph_setGOP2(FALSE,GRPH_DST_1,GRPH_GOP_VTC_CFLT,GRPH_CFLT_PARAM(Chroma_Param[0],Chroma_Param[1],Chroma_Param[2],Chroma_Param[3]));
    //Cr Ver
    grph_setImg1(CrAddr, LineOffset, Vsize, Hsize);
    grph_setImg2(CrAddr, Vsize);
    grph_setGOP2(FALSE,GRPH_DST_1,GRPH_GOP_VTC_CFLT,GRPH_CFLT_PARAM(Chroma_Param[0],Chroma_Param[1],Chroma_Param[2],Chroma_Param[3]));

    grph_disableDMA();
    grph_close();

    ime_filt_para->crv_en = IMEALG_DISABLE;
    //Cb,Cr_Hor
    ime_setChromaFilter(ime_filt_para);

}
//#NT#2010/05/28#ethanlau -end

//#NT#2008/07/11#Bowen Li -begin
void SubSampleRaw(UINT32 SourceAdd, UINT32 TargetAdd, UINT32 InH, UINT32 InV,UINT32 OutH, UINT32 OutV, UINT32 OutOfs, UINT32 FieldNum)
{
    unsigned long SourceData,TargetData,i,j,k;
    unsigned long TempAdd,TempData;
    unsigned long SampleH,SampleV;

    SampleH = InH/(OutH-16);
    if(SampleH%2==0)
        k=1;
    else
        k=0;
    for(j=0;j<OutV;j++)
    {
        SampleV = (InV/FieldNum)*j/OutV;
        if((j%2) != (SampleV%2))
            SampleV+=1;
        TempAdd = SourceAdd+SampleV*FieldNum*InH;

        for(i=0;i<OutH-16;i+=4)
        {
            SourceData = *(UB *)(TempAdd + SampleH*i);
            TempData = SourceData&0xff;
            SourceData = *(UB *)(TempAdd + SampleH*(i+1)-k);
            TempData |= ((SourceData<<8&0xff00));
            SourceData = *(UB *)(TempAdd + SampleH*(i+2));
            TempData |= ((SourceData<<16&0xff0000));
            SourceData = *(UB *)(TempAdd + SampleH*(i+3)-k);
            TempData |= ((SourceData<<24&0xff000000));

            TargetData = TempData;
            *((UW *)(TargetAdd+i+8)) = TargetData;
        }

        TargetData = *((UH *)(TargetAdd+8));
        for(i=0;i<8;i+=2)
        {
            *((UH *)(TargetAdd+i)) = TargetData;
        }

        TargetData = *((UH *)(TargetAdd+OutH-10));
        for(i=OutH-8;i<OutH;i+=2)
        {
            *((UH *)(TargetAdd+i)) = TargetData;
        }
        TargetAdd += OutOfs;
    }
}
//#NT#2008/07/11#Bowen Li -end

//#NT#2008/07/18#Bowen Li -begin
//#NT#add drawvawindow function
void DrawVAWindow(void)
{
    UINT32 i, j, iMax, jMax;
    UINT32 OsdW, OsdH, OsdLOfs;
    UINT32 StartX, StartY, SizeX, SizeY;
    UINT16 Color;

    ide_get_osd1_buf_dim(&OsdW, &OsdH, &OsdLOfs);
    iMax = 8;
    jMax = 8;
    if (IPE_Para.VaccInfo.vacc_num == WINDOWS_16X16)
    {
        iMax = 16;
        jMax = 16;
    }
    SensorInfor.FP_ClearOSD();
    for (j = 0; j < jMax; j ++)
    {
        for (i = 0; i < iMax; i ++)
        {
            if (j & 0x1)
            {
                Color = 8;
                if (i & 0x1)
                {
                    Color = 2;
                }
            }
            else
            {
                Color = 2;
                if (i & 0x1)
                {
                    Color = 8;
                }
            }
//#NT#2008/09/02#Lawrence Ho -begin
//#NT#PERF 1. stx and sty must map to osd coordinate
            StartX = ((IPE_Para.VaccInfo.stx  + IPE_Para.VaccInfo.hsz * i) * OsdW) / GetIPPSizeInfor(_IpeOutHsize);
            StartY = ((IPE_Para.VaccInfo.sty  + IPE_Para.VaccInfo.vsz * j) * OsdH) / ( GetIPPSizeInfor(_IpeOutVsize) - 1 );
            SizeX = (IPE_Para.VaccInfo.hsz * OsdW) / GetIPPSizeInfor(_IpeOutHsize) ;
            SizeY = (IPE_Para.VaccInfo.vsz * OsdH) / (GetIPPSizeInfor(_IpeOutVsize) - 1) ;
            SensorInfor.FP_DrawOSDRect(StartX, StartY, SizeX, SizeY, Color);
//#NT#2008/09/02#Lawrence Ho -end
        }
    }



}
//#NT#2008/07/18#Bowen Li -end


#if  IME_PATH_DRAW_VA_WINDOW
//#NT#2008/07/25#Lawrence Ho -begin
//#NT#add drawvawindow function on IME IMAGE for more accurate position
/**
  DRAW VA WINDOW

  a. Put this function on aaa_AEprocess() flow, because it needs update by frame
  b. Get current ime output frame address by GetActImgFBAddr_Path1(&FB_para1) function.
  c. Assign current y, cb, cr address to DrawVAwindow parameter for drawing window.
     example : DrawVAwindow(FB_para1.y_addr, FB_para1.cb_addr, FB_para1.cr_addr);
*/
void DrawVAWindowOnIme( UINT32 y_addr, UINT32 cb_addr, UINT32 cr_addr)
{
    UINT32 lineOffset, InputH, InputV, OutputH, OutputV;
    UINT32 startx, starty, cntx, cnty;
    UINT32 width, height;
    UINT32 tempx, tempy;
    UINT32 Limitx, Limity;

    //SetIPEVarAccInfo(&IPE_Para);

    lineOffset=GetIPPSizeInfor(_ImeOut1LineOffset);
    InputH = GetIPPSizeInfor(_IpeOutHsize);
    InputV = GetIPPSizeInfor(_IpeOutVsize) ;
    OutputH = GetIPPSizeInfor(_ImeOut1Hsize);
    OutputV = GetIPPSizeInfor(_ImeOut1Vsize);


   if (IME_D1_Para.in_format == IME_IN_FMT_422)
   {

        // Whole VA Winodw
        startx = (IPE_Para.VaccInfo.stx * OutputH) / InputH ;
        starty = (IPE_Para.VaccInfo.sty * OutputV) / InputV ;


        width =  IPE_Para.VaccInfo.vacc_num ? IPE_Para.VaccInfo.hsz*VAC_WIN16 :IPE_Para.VaccInfo.hsz*VAC_WIN8;
        height = IPE_Para.VaccInfo.vacc_num ? IPE_Para.VaccInfo.vsz*VAC_WIN16 :IPE_Para.VaccInfo.vsz*VAC_WIN8;

        width = (width * OutputH) / InputH ;
        height = (height * OutputV) / InputV ;


            for (cntx = startx; cntx < (startx + width); cntx++ )
            {
              *(volatile UINT8*)(y_addr + starty*lineOffset + cntx) = 0xff;
              *(volatile UINT8*)(y_addr + (starty + height)*lineOffset + cntx) = 0xff;

            }

            // Vertical line
            for (cnty = starty; cnty < (starty + height); cnty++)
            {
              *(volatile UINT8*)(y_addr + cnty*lineOffset + startx) = 0xff;
              *(volatile UINT8*)(y_addr + cnty*lineOffset + startx + width) = 0xff;

            }



            //++ Cb
            // Horizontal line
            for (cntx = startx; cntx < (startx + width); cntx++ )
            {
              *(volatile UINT8*)(cb_addr + starty*lineOffset/2 + cntx/2) = 0xff;
              *(volatile UINT8*)(cb_addr + (starty + height)*lineOffset/2 + cntx/2) = 0xff;

            }

            // Vertical line
            for (cnty = starty; cnty < (starty + height); cnty++)
            {
              *(volatile UINT8*)(cb_addr + cnty*lineOffset/2 + startx/2) = 0xff;
              *(volatile UINT8*)(cb_addr + cnty*lineOffset/2 + startx/2 + width/2) = 0xff;

            }


            //++ Cr
            // Horizontal line
            for (cntx = startx; cntx < (startx + width); cntx++ )
            {
              *(volatile UINT8*)(cr_addr + starty*lineOffset/2 + cntx/2) = 0x00;
              *(volatile UINT8*)(cr_addr + (starty + height)*lineOffset/2 + cntx/2) = 0x00;

            }

            // Vertical line
            for (cnty = starty; cnty < (starty + height); cnty++)
            {
              *(volatile UINT8*)(cr_addr + cnty*lineOffset/2 + startx/2) = 0x00;
              *(volatile UINT8*)(cr_addr + cnty*lineOffset/2 + startx/2 + width/2) = 0x00;

            }

     //+++ Center
        // select two winodw
        tempx = IPE_Para.VaccInfo.vacc_num ? IPE_Para.VaccInfo.hsz*7 :IPE_Para.VaccInfo.hsz*3;
        tempy = IPE_Para.VaccInfo.vacc_num ? IPE_Para.VaccInfo.vsz*7 :IPE_Para.VaccInfo.vsz*3;

        startx = ((IPE_Para.VaccInfo.stx + tempx) * OutputH) / InputH ;
        starty = ((IPE_Para.VaccInfo.sty + tempy)* OutputV) / InputV ;

        width =  IPE_Para.VaccInfo.vacc_num ? IPE_Para.VaccInfo.hsz*2 :IPE_Para.VaccInfo.hsz*2;
        height = IPE_Para.VaccInfo.vacc_num ? IPE_Para.VaccInfo.vsz*2 :IPE_Para.VaccInfo.vsz*2;

        width = (width * OutputH) / InputH ;
        height = (height * OutputV) / InputV ;

            // Horizontal line
            for (cntx = startx; cntx < (startx + width); cntx++ )
            {
              *(volatile UINT8*)(y_addr + starty*lineOffset + cntx) = 0xff;
              *(volatile UINT8*)(y_addr + (starty + height)*lineOffset + cntx) = 0xff;

            }

            // Vertical line
            for (cnty = starty; cnty < (starty + height); cnty++)
            {
              *(volatile UINT8*)(y_addr + cnty*lineOffset + startx) = 0xff;
              *(volatile UINT8*)(y_addr + cnty*lineOffset + startx + width) = 0xff;

            }

            //++ Cb
            // Horizontal line
            for (cntx = startx; cntx < (startx + width); cntx++ )
            {
              *(volatile UINT8*)(cb_addr + starty*lineOffset/2 + cntx/2) = 0xff;
              *(volatile UINT8*)(cb_addr + (starty + height)*lineOffset/2 + cntx/2) = 0xff;

            }
          // Vertical line
            for (cnty = starty; cnty < (starty + height); cnty++)
            {
              *(volatile UINT8*)(cb_addr + cnty*lineOffset/2 + startx/2) = 0xff;
              *(volatile UINT8*)(cb_addr + cnty*lineOffset/2 + startx/2 + width/2) = 0xff;

            }
            //++ Cr
            // Horizontal line
            for (cntx = startx; cntx < (startx + width); cntx++ )
            {
              *(volatile UINT8*)(cr_addr + starty*lineOffset/2 + cntx/2) = 0x00;
              *(volatile UINT8*)(cr_addr + (starty + height)*lineOffset/2 + cntx/2) = 0x0;

            }
            // Vertical line
            for (cnty = starty; cnty < (starty + height); cnty++)
            {
              *(volatile UINT8*)(cr_addr + cnty*lineOffset/2 + startx/2) = 0x00;
              *(volatile UINT8*)(cr_addr + cnty*lineOffset/2 + startx/2 + width/2) = 0x00;

            }

   }
}
//#NT#2008/07/25#Lawrence Ho -end
#endif


//#NT#2008/07/23#YH Lee -begin
//#NT#Save preview raw data
void SavePrvPRERaw(void)
{
    UINT32 DramAddr;

    StopPreviewDramData();
    if (pre_ReadCurrentOutBufferID() == 1)
    {
        DramAddr = ippbuf_GetBuf(IPPBUF_PRV_RAW_1);
    }
    else
    {
        DramAddr = ippbuf_GetBuf(IPPBUF_PRV_RAW_2);
    }
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                    (UB*)DramAddr,
                    GetIPPSizeInfor(_PreOutVsize) * SIE_PRE_Para_prv.PreOutLineOfs,
                    0,
                    1,
                    FST_FMT_RAW,
                    FST_TIME_INFINITE);
    debug_err(("GetIPPSizeInfor(_PreOutVsize) = %d SIE_PRE_Para_prv.PreOutLineOfs = %d\r\n", GetIPPSizeInfor(_PreOutVsize), SIE_PRE_Para_prv.PreOutLineOfs));
    StartPreviewDramData();
}
//#NT#2008/07/23#YH Lee -end

//#NT#2009/10/13#Yuppie Lin -begin
//#NT#Modify Lock Display frame
#if 0
//#NT#2008/08/07#YH Lee -begin
//#NT#Lock Display frame
//#NT#2008/09/02#YH Lee -begin
//#NT#Change api naming
void DisplayFBLock(UINT32 LockEn)
//#NT#2008/09/02#YH Lee -end
{
    if(LockEn)
    {
        ime_Path1OutputPingpong(0);
        ide_set_video1_bufferop(0,0,0);
        ime_OutputPath1YcbCrAddr0(IME_D1_Para.y2_addr, IME_D1_Para.cb2_addr, IME_D1_Para.cr2_addr);
    }
    else
    {
        sie_waitVD(2);
        ime_Path1OutputPingpong(2);
        ime_OutputPath1YcbCrAddr0(IME_D1_Para.y0_addr, IME_D1_Para.cb0_addr, IME_D1_Para.cr0_addr);
        ide_set_video1_bufferop(0,2,0);
    }
}
//#NT#2008/08/07#YH Lee -end
#else
void DisplayFBLock(UINT32 LockEn)
{
    IME_MODE_PRAM IMEParam;

    if(LockEn)
    {
        IMEParam.op_mode = _CHANGEBUFFER;
        IMEParam.oppb1_num = IMEALG_1BUFFER;
        IMEParam.p1_out0_addr.y_addr = IME_D1_Para.y2_addr;
        IMEParam.p1_out0_addr.cb_addr = IME_D1_Para.cb2_addr;
        IMEParam.p1_out0_addr.cr_addr = IME_D1_Para.cr2_addr;
        ime_setMode(&IMEParam);
        //#NT#2009/11/27#Connie Yu -begin
        //#Add DIS process
        if(gImageAlgInfo.DISMode == _DISMode_On)
        {
            dis_setFlagFreezeFB(TRUE);
        }
        else
        {
            ide_set_video1_bufferop(IDE_VIDEO_BJMODE_CANT_CROSS_WRITE,IDE_VIDEO_BUFFER_OPT_0,IDE_VIDEO_BUFFER_NUM_3);
        }
        //#NT#2009/11/27#Connie Yu -end
    }
    else
    {
        //#NT#2009/11/27#Connie Yu -begin
        //#Add DIS process
        if(gImageAlgInfo.DISMode == _DISMode_On)
        {
            dis_setFlagFreezeFB(FALSE);
        }
        else
        {
            sie_waitVD(2);
            IMEParam.op_mode = _CHANGEBUFFER;
            IMEParam.oppb1_num = IMEALG_3BUFFER;
            IMEParam.p1_out0_addr.y_addr = IME_D1_Para.y0_addr;
            IMEParam.p1_out0_addr.cb_addr = IME_D1_Para.cb0_addr;
            IMEParam.p1_out0_addr.cr_addr = IME_D1_Para.cr0_addr;
            IMEParam.p1_out1_addr.y_addr = IME_D1_Para.y1_addr;
            IMEParam.p1_out1_addr.cb_addr = IME_D1_Para.cb1_addr;
            IMEParam.p1_out1_addr.cr_addr = IME_D1_Para.cr1_addr;
            IMEParam.p1_out2_addr.y_addr = IME_D1_Para.y2_addr;
            IMEParam.p1_out2_addr.cb_addr = IME_D1_Para.cb2_addr;
            IMEParam.p1_out2_addr.cr_addr = IME_D1_Para.cr2_addr;
            ime_setMode(&IMEParam);
            ide_set_video1_bufferop(IDE_VIDEO_BJMODE_CANT_CROSS_WRITE,IDE_VIDEO_BUFFER_OPT_2,IDE_VIDEO_BUFFER_NUM_3);
        }
        //#NT#2009/11/27#Connie Yu -end
    }
}
#endif
//#NT#2009/10/13#Yuppie Lin -end


void ChromaCurve(UINT32 hSize, UINT32 vSize, UINT32 sourceAddr, UINT32 distAddr, UINT ipe_GaEn, UINT32 *chrSmLUT, UINT32 *rGaLUT, UINT32 *gGaLUT, UINT32 *bGaLUT)
{
    STR_FLDNUM pre_FieldNum;
    unsigned long PreIntStatus;
    IPE_IQINFO IQInfo;

    pre_FieldNum.Fld1=0;
    pre_FieldNum.Fld2=0;
    pre_FieldNum.Fld3=0;
    pre_FieldNum.Fld4=0;
    pre_FieldNum.Fld5=0;
    pre_FieldNum.Fld6=0;
    pre_FieldNum.Fld7=0;
    pre_FieldNum.Fld8=0;
    pre_FieldNum.TotalFld=0;

    IQInfo.FuncUpdateSel = IPE_FUNC_DISABLE;
    IQInfo.FuncSel = IPE_PREGAMMA_EN;
    IQInfo.ParamUpdateSel = 0;
    ipe_changeParam(&IQInfo);
    ipe_pause();
    ipe_start();

    dis_int(_INT_PRE);
    pll_setClockRate(PLL_CLKSEL_PRE_CLKSRC, PLL_CLKSEL_PRE_CLKSRC_PRE);
    pll_setClockRate(PLL_CLKSEL_PRE, PLL_CLKSEL_PRE_96);
    pre_Reset(1);
    pre_en(0);
    pre_ClearAllMode();

    pre_SetFldNum(pre_FieldNum);
    pre_SetPre2DramPath(PATH_POST_GAMMA);
    pre_SieDirectPre(PATH_DRAM_INPUT);
    pre_SetInputSize(hSize/2, vSize);
    pre_SetInputPackMode(2);
    pre_SetInputStartAdd(sourceAddr);
    pre_SetInputLineOffset(hSize/2);

    pre_SetHcropSize(0, hSize/2);
    pre_SetVcropSize(0, vSize);

    pre_FunctionEnable(0, PRE_HSCL_EN);
    pre_SetOutputSize(hSize/2);
    pre_SetOutputPackMode(2);
    pre_SetOutputStartAdd(distAddr);
    pre_SetOutputLineOffset(hSize/2);
    pre_OutputPpbEn(0);

    pre_RstSac();
    pre_ClrIntrStatus(PRE_INT_ALL);

    pre_SetGamma(chrSmLUT, chrSmLUT, chrSmLUT);

    //Start operation
    pre_FunctionEnable(0, PRE_VIG_EN);
    pre_FunctionEnable(0, PRE_GEO_EN);
    pre_FunctionEnable(0, PRE_BLM_EN);
    pre_FunctionEnable(0, PRE_DF_EMBED_EN);
    pre_FunctionEnable(0, PRE_CG_EN);
    pre_FunctionEnable(1, PRE_GAMMA_EN);
    pre_FunctionEnable(0, PRE_VACC_EN);
    pre_FunctionEnable(0, PRE_DF_SUB_EN);

    pre_Reset(0);
    pre_Load();
    pre_en(1);

    PreIntStatus=pre_ReadIntrStatus();
    while(!(PreIntStatus & PRE_INT_FLDEND))
    PreIntStatus=pre_ReadIntrStatus();

    pre_SetGamma(rGaLUT, gGaLUT, bGaLUT);
    pre_FunctionEnable(0, PRE_GAMMA_EN);
    pre_Reset(1);
    pre_Reset(0);
    if(ipe_GaEn==1)
        IQInfo.FuncUpdateSel = IPE_FUNC_ENABLE;
    else
        IQInfo.FuncUpdateSel = IPE_FUNC_DISABLE;
    IQInfo.FuncSel = IPE_PREGAMMA_EN;
    IQInfo.ParamUpdateSel = 0;
    ipe_changeParam(&IQInfo);
    ipe_pause();
    ipe_start();
    ena_int(_INT_PRE);
}

UINT8 Sen_GetIPECfaPat(UINT8 OrgPat, UINT8 FlipType)
{
    UINT8 PatTable[4][4] =
    {
        {IPE_PATR, IPE_PATGR, IPE_PATGB, IPE_PATB},
        {IPE_PATGR, IPE_PATR, IPE_PATB, IPE_PATGB},
        {IPE_PATGB, IPE_PATB, IPE_PATR, IPE_PATGR},
        {IPE_PATB, IPE_PATGB, IPE_PATGR, IPE_PATR}
    };

    return PatTable[OrgPat][FlipType];
}

UINT32 Sen_GetPREAddrOfs(UINT8 FlipType, UINT32 PreOutVSize, UINT32 PreOutLineOfs, UINT32 PreOutBitDepth)
{
    UINT32 OfsValue;

    OfsValue = 0;
    if (FlipType == _Flip_H)
    {
        OfsValue = PreOutLineOfs;
    }
    else if (FlipType == _Flip_V)
    {
        OfsValue = PreOutLineOfs * (PreOutVSize - 1);
    }
    else if (FlipType == _Flip_H_V)
    {
        OfsValue = PreOutLineOfs * PreOutVSize;
    }
    return OfsValue;
}

UINT32 Sen_GetIPEAddrOfs(UINT8 FlipType, UINT32 PreOutHSize, UINT32 PreOutLineOfs, UINT32 PreOutBitDepth)
{
    UINT32 OfsValue;

    OfsValue = 0;
    if (FlipType == _Flip_H)
    {
        OfsValue = PreOutLineOfs - ((PreOutHSize * PreOutBitDepth) >> 3);
    }
    else if (FlipType == _Flip_V)
    {

    }
    else if (FlipType == _Flip_H_V)
    {
        OfsValue = PreOutLineOfs - ((PreOutHSize * PreOutBitDepth) >> 3);
    }
    return OfsValue;
}

UINT32 Sen_GetPREFlipParam(UINT8 FlipType)
{
    UINT32 FlipTable[_Flip_Max_type] =
    {
        0, PRE_HFLIP_EN, PRE_VFLIP_EN, (PRE_HFLIP_EN|PRE_VFLIP_EN)
    };

    return FlipTable[FlipType];
}

void Sen_GetAEHistResult(UINT32 *Hist)
{
#if 0
    GetActImgFBAddr_Path1(&FB_para1);
    aaa_EstimateY2(FB_para1.y_addr, GetPreImeOutHsize(), GetPreImeOutVsize(), GetPreImeOutHsize());
    aaa_Read_HIST_RSLT(Hist);
#endif
}

//#NT#2009/11/02#Bowen Li -begin
//#NT#modify CA function for 2 & 3 line
ER Sen_GetCASubRatio(PRE_VGAFormatEnum VGAType, PreCA_SubsampleRate HSub, PreCA_SubsampleRate VSub, UINT32 *HRatio, UINT32 *VRatio)
{
    UINT32 SubRatio3Line[3][2] =
    {
        {3, 1},
        {6, 2},
        {12, 4}
    };

    if ((VGAType == SENSOR_OUT_PROGRESSIVE) || (VGAType == SENSOR_OUT_TWO_LINE))
    {
//#NT#2010/01/22#Jarkko Chang -begin
//        *HRatio = HSub + 1;
//        *VRatio = VSub + 1;
        *HRatio = SubRatio3Line[HSub][1]*2;
        *VRatio = SubRatio3Line[VSub][1]*2;
//#NT#2010/01/22#Jarkko Chang -end
    }
    else if (VGAType == SENSOR_OUT_THREE_LINE)
    {
        *HRatio = SubRatio3Line[HSub][0];
        *VRatio = SubRatio3Line[VSub][1];
    }
    else
    {
        debug_err(("unsupport format.......\r\n"));
        return E_PAR;
    }
    return E_OK;
}

UINT32 Sen_GetIntlvRatio(PRE_VGAFormatEnum VGAType)
{
    UINT32 Ratio[3] =
    {
        1, 2, 3
    };
    return Ratio[VGAType];
}
//#NT#2009/11/02#Bowen Li -end


//#NT#2009/12/04#YC Peng -begin
//#NT#Add drive mode AdjFocusShot for adjust FF lens
#define NewRawH 640
#define NewRawV 480
#define NewRawCropCenterH (216)
#define NewRawCropCenterV (160)
#define NewRawCropCornerH (NewRawH/2)
#define NewRawCropCornerV (NewRawV/2)
#define CropPosition 30 // 0.3
UINT32  AdjFocusCnt=0;

void SenAdjFocusMode(void)
{
    ImageAlgInfo *AlgInfo;
    IPE_IME_FM_PARAM fmInfo;
    UINT32  OriRawAddr, NewRawAddr, DisplayAddr0, DisplayAddr1;
    UINT32  YAddr0, CbAddr0, CrAddr0;
    UINT32  YAddr1, CbAddr1, CrAddr1;
    UINT32  OriRawH, OriRawV, DisplayH, DisplayV, CropOfsH, CropOfsV;
    UINT32  DisplayBufInUse=0;

    //Param init
    AlgInfo = UI_GetParameter();
    DisplayBufInUse = AdjFocusCnt%2;
    OriRawH = GetIPPSizeInfor(_PreCropCapHsize);
    OriRawV = GetIPPSizeInfor(_PreCropCapVsize);
    DisplayH = GetIPPSizeInfor(_ImeOut1Hsize);
    DisplayV = GetIPPSizeInfor(_ImeOut1Vsize);

    CropOfsH = (OriRawH/2) * CropPosition / 100;
    if(CropOfsH < NewRawCropCornerH/2)
        CropOfsH = 0;
    else
        CropOfsH -= NewRawCropCornerH/2;
    CropOfsH = (CropOfsH/2)*2;

    CropOfsV = (OriRawV/2) * CropPosition / 100;
    if(CropOfsV < NewRawCropCornerV/2)
        CropOfsV = 0;
    else
        CropOfsV -= NewRawCropCornerV/2;
    CropOfsV = (CropOfsV/2)*2;

    //Addr init
    OriRawAddr = ippbuf_GetBuf(IPPBUF_CAP_RAW_1);
    NewRawAddr = ippbuf_GetBuf(IPPBUF_CAP_JPG_1);
    DisplayAddr0 = ippbuf_GetBuf(IPPBUF_CAP_JPG_1) + NewRawH * NewRawV;
    DisplayAddr1 = DisplayAddr0 + DisplayH * DisplayV * 2;

    YAddr0 = DisplayAddr0;
    CbAddr0 = DisplayAddr0 + DisplayH * DisplayV;
    CrAddr0 = DisplayAddr0 + DisplayH * DisplayV * 3/2;

    YAddr1 = DisplayAddr1;
    CbAddr1 = DisplayAddr1 + DisplayH * DisplayV;
    CrAddr1 = DisplayAddr1 + DisplayH * DisplayV * 3/2;

    //RAW Crop process
    grph_open();

    if(AlgInfo->AFWindow == _IMGAFWindow_Center)
    {
        //Center 640x480
        grph_setImg1((OriRawAddr + (OriRawH * (OriRawV/2 - NewRawV/2)) + (OriRawH/2 - NewRawH/2)), OriRawH, NewRawV, NewRawH);
        grph_setImg2((NewRawAddr), NewRawH);
        grph_setImg3((NewRawAddr), NewRawH);
        grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);
    }
    else if(AlgInfo->AFWindow == _IMGAFWindow_ManualPosition)
    {
        //LeftTop
        grph_setImg1((OriRawAddr + (OriRawH * CropOfsV) + (CropOfsH) ), OriRawH, NewRawCropCornerV, NewRawCropCornerH);
        grph_setImg2((NewRawAddr), NewRawH);
        grph_setImg3((NewRawAddr), NewRawH);
        grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

        //RightTop
        grph_setImg1((OriRawAddr + (OriRawH * CropOfsV) + (OriRawH - CropOfsH - NewRawCropCornerH) ), OriRawH, NewRawCropCornerV, NewRawCropCornerH);
        grph_setImg2((NewRawAddr + NewRawH/2), NewRawH);
        grph_setImg3((NewRawAddr + NewRawH/2), NewRawH);
        grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

        //LeftBottom
        grph_setImg1((OriRawAddr + (OriRawH * (OriRawV - CropOfsV - NewRawCropCornerV)) + (CropOfsH) ), OriRawH, NewRawCropCornerV, NewRawCropCornerH);
        grph_setImg2((NewRawAddr + NewRawH * NewRawV/2), NewRawH);
        grph_setImg3((NewRawAddr + NewRawH * NewRawV/2), NewRawH);
        grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

        //RightBottom
        grph_setImg1((OriRawAddr + (OriRawH * (OriRawV - CropOfsV - NewRawCropCornerV)) + (OriRawH - CropOfsH - NewRawCropCornerH) ), OriRawH, NewRawCropCornerV, NewRawCropCornerH);
        grph_setImg2((NewRawAddr + NewRawH * NewRawV/2 + NewRawH/2), NewRawH);
        grph_setImg3((NewRawAddr + NewRawH * NewRawV/2 + NewRawH/2), NewRawH);
        grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);

        //Center
        grph_setImg1((OriRawAddr + (OriRawH * (OriRawV/2 - NewRawCropCenterV/2)) + (OriRawH/2 - NewRawCropCenterH/2) ), OriRawH, NewRawCropCenterV, NewRawCropCenterH);
        grph_setImg2((NewRawAddr + NewRawH * (NewRawV/2 - NewRawCropCenterV/2) + (NewRawH/2 - NewRawCropCenterH/2)), NewRawH);
        grph_setImg3((NewRawAddr + NewRawH * (NewRawV/2 - NewRawCropCenterV/2) + (NewRawH/2 - NewRawCropCenterH/2)), NewRawH);
        grph_setAOP2(8,GRPH_DST_1, GRPH_AOP_A_COPY ,0,0,0);
    }

    grph_close();

    //IPE and IME frame mode
    fmInfo.BayerAddr = NewRawAddr;
    fmInfo.BayerHsize = NewRawH;
    fmInfo.BayerVsize = NewRawV;
    fmInfo.ImeOutPath = IMEALG_1PATH;
    fmInfo.ImeHsize1 = DisplayH;
    fmInfo.ImeVsize1 = DisplayV;
    fmInfo.ImeHofst1 = fmInfo.ImeHsize1;
    fmInfo.ImeFmt1 = IME_OUT_FMT_422_COSITED;

    if(DisplayBufInUse == 0)
    {
        fmInfo.OutputAddr1Y = YAddr0;
        fmInfo.OutputAddr1Cb = CbAddr0;
        fmInfo.OutputAddr1Cr = CrAddr0;
    }
    else
    {
        fmInfo.OutputAddr1Y = YAddr1;
        fmInfo.OutputAddr1Cb = CbAddr1;
        fmInfo.OutputAddr1Cr = CrAddr1;
    }
    SetIPPSizeInfor(_PreCropCapHsize, NewRawH);
    SetIPPSizeInfor(_PreCropCapVsize, NewRawV);
    ipe_ime_Framemode(fmInfo);
    SetIPPSizeInfor(_PreCropCapHsize, OriRawH);
    SetIPPSizeInfor(_PreCropCapVsize, OriRawV);

    //IDE setting
    ide_setV1Buf0Addr(YAddr0, CbAddr0, CrAddr0);
    ide_setV1Buf1Addr(YAddr1, CbAddr1, CrAddr1);

    ide_setV1BufOp(IDE_VIDEO_BJMODE_BUFFER_REPEAT,
                   IDE_VIDEO_BUFFER_OPT_0,
                   IDE_VIDEO_BUFFER_NUM_2);

    if(DisplayBufInUse == 0)
        ide_set_video1_changebuf(IDE_VIDEO_BUFFER_OPT_0);
    else
        ide_set_video1_changebuf(IDE_VIDEO_BUFFER_OPT_1);

    ide_waitFrameEnd();

    AdjFocusCnt++;

    //#NT#2009/12/08#YC Peng -begin
    //#NT#delay 500ms to release CPU from capture task
    TimerDelayMs(500);
    //#NT#2009/12/08#YC Peng -end
}
//#NT#2009/12/04#YC Peng -end

//#NT#2013/02/05#Jarkko Chang -begin
//#NT# modify motion detection api from ime y for av in
BOOL MotionDetection(void)
{
    BOOL motion_dect;

    if (GetIPPFuncInfor(DVI_MODE_EN) == ENABLE)
    {
        motion_dect = MotionDetection_IME_Y();
    }
    else
    {
        motion_dect = MotionDetection_RAW();
    }
    return motion_dect;
}

UINT32 BlockDiffThrs = 10;
UINT32 TotalDiffThres = 64*3;
UINT32 BlockCntThres = 4;
BOOL MotionDetection_RAW(void)
{
    static UINT32 PrevG_acc[64], DetectionCnt=0;
    UINT32 i, Diff, Total_diff=0, DiffCnt=0;

    for(i=0;i<64;i++)
    {
        if(PrevG_acc[i]>G_acc[i])
            Diff = (PrevG_acc[i]-G_acc[i]);
        else
            Diff = (G_acc[i]-PrevG_acc[i]);

        Total_diff += Diff;
        if(Diff > BlockDiffThrs)
            DiffCnt ++;

        PrevG_acc[i] = G_acc[i];
    }

    DetectionCnt ++;
    if(DetectionCnt < 2) // for initialization
        return FALSE;

    if((DiffCnt > BlockCntThres) || (Total_diff > TotalDiffThres))
        return TRUE;
    else
        return FALSE;
}

UINT32 BlockDiffThrs_IME_Y = 20;
UINT32 TotalDiffThres_IME_Y = 64*10;
UINT32 BlockCntThres_IME_Y = 4;
BOOL MotionDetection_IME_Y(void)
{
    UINT16 i,j;
    UINT32 h_size, v_size, offset;
    static UINT32 PrevY_Block_Avg[CA_WINDOW_X][CA_WINDOW_Y], Y_Block_Avg[CA_WINDOW_X][CA_WINDOW_Y], DetectionCnt=0;
    UINT32 Diff, Total_diff=0, DiffCnt=0;
    ACTFBADDR_PRAM  FB_para;

    //get IME active buffer address
    GetActImgFBAddr_Path1(&FB_para);

    h_size = GetIPPSizeInfor(_ImeOut1Hsize);
    v_size = GetIPPSizeInfor(_ImeOut1Vsize);
    offset = GetIPPSizeInfor(_ImeOut1LineOffset);

    for(i=0;i<CA_WINDOW_Y;i++)
    {
        for(j=0;j<CA_WINDOW_X;j++)
        {
            Y_Block_Avg[i][j]=aaa_BlockAverageY(FB_para.y_addr,((j*h_size)/CA_WINDOW_X),((i*v_size)/CA_WINDOW_Y),(h_size/CA_WINDOW_X),(v_size/CA_WINDOW_Y),offset);


            if(PrevY_Block_Avg[i][j]>Y_Block_Avg[i][j])
                Diff = (PrevY_Block_Avg[i][j]-Y_Block_Avg[i][j]);
            else
                Diff = (Y_Block_Avg[i][j]-PrevY_Block_Avg[i][j]);

            Total_diff += Diff;
            if(Diff > BlockDiffThrs_IME_Y)
                DiffCnt ++;

            PrevY_Block_Avg[i][j] = Y_Block_Avg[i][j];
        }
    }

    DetectionCnt ++;
    if(DetectionCnt < 2) // for initialization
        return FALSE;

    if((DiffCnt > BlockCntThres_IME_Y) || (Total_diff > TotalDiffThres_IME_Y))
        return TRUE;
    else
        return FALSE;

}
//#NT#2013/02/05#Jarkko Chang -end

//#NT#2011/08/25#Spark Chou -begin
//#NT# DVI Input
void CheckDVIMode()
{
    if (GetIPPFuncInfor(DVI_MODE_EN) == FALSE)
        return ;
    SensorMode(IPL_GetMode());
#if 0
    static UINT32 PrvTVmode,CurTVmode;
    if (GetIPPFuncInfor(DVI_MODE_EN) == FALSE)
        return ;
    PrvTVmode = gImageAlgInfo.TvInFormat;

    CurTVmode=GetCurretInputMode();
    if(CurTVmode!=PrvTVmode)
    {
        gImageAlgInfo.TvInFormat = CurTVmode;
        PrvTVmode = CurTVmode;
        SensorMode(IPL_GetMode());
    }
#endif
}
//#NT#2011/09/07#Spark Chou -end


//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
char IQDebugInfo[5120];
UINT32 IQDebugInfoLen;
#include <stdarg.h>

void AddVideoIQInfo( const char *format, ...)
{
    int ret;
    UINT32 len;
    va_list args;

    va_start (args, format);

    ret = vsprintf( message, format, args );
    va_end (args);

    len = strlen(message);
    if ( IQDebugInfoLen+len >= 5120 )
        len = 5120 - IQDebugInfoLen - 1;
    memcpy(IQDebugInfo+IQDebugInfoLen, message, len);
    IQDebugInfoLen += len;

    debug_err((message));
}

char* GetVideoIQInfo()
{
    return IQDebugInfo;
}

void ResetVideoIQInfo()
{
    IQDebugInfoLen = 0;
    memset(IQDebugInfo, 0, sizeof(IQDebugInfo));
}
//#NT#2011/10/14#Spark Chou -end
