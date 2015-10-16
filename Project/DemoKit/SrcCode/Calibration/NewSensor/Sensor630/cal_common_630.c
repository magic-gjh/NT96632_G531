#include <stdio.h>
#include <string.h>
#include "Kernel.h"
#include ".\Cal_Header.h" // There is too many Cal_Header.h under Calibration/.
#include "ImageAlgInfor.h"
#include "FilesysTsk.h"
#include "cal_common_630.h"
#include "PrimaryTsk.h"
#include "PhotoTsk.h"
#include "SysCfg.h"
#include "Lens.h"
//#NT#2009/09/18#Jarkko Chang -begin
#include "SystemClock.h"
#include "IDE.h"
//#NT#2009/09/18#Jarkko Chang -end
//#NT#2007/12/18#Stan Lin -Remove #include "CalibrationTsk.h"
#include    "pre_lib.h"
#include    "UIResource.h"

#if (_CALIBRATION_MODE_ == ENABLE)
#define CAL_COMMON_TEST_CODE    0
#define Block_Y_Field_Cap       8
#define SensorRawSize           (2576*1940)//0x500000

FN_CalShowString fnCalShowString = NULL;
Sensor_Cal_Struct CalInfor;
UINT32 CapFldOffsetX;
UINT32 CapFldOffsetY;
//UINT DP_Addr_Prv,DP_Addr_Cap;
UINT guiCalFlags=0;
INT HwDpTestFlag = 1;
UINT16 DP_Num_Prv=0,DP_Num_Cap=0,DP_Num_Dzoom=0;
UINT8 DFPC_TEST=0;

//#NT#2009/09/01#KS Hung -begin
extern UINT gCapRawAddr;
//#NT#2009/09/01#KS Hung -end
extern void DFP_DarkAEPrv(void);
extern void DFP_DarkAECap(void);
INT32 abs(INT32 i)
{
    if (i >= 0)
    {
        return i;
    }
    return (-i);
}
ER Alg_Cal(char AlgCal_item, ALG_CAL_PARAM *pParam)
{
    UINT32 uiTmp;
    UINT8 ErrorCode;
    INT8 strTmp[64];

    fnCalShowString = pParam->FnShowString;
    gCapRawAddr = pParam->MemoryAddr;
    uiTmp = pParam->MemorySize;
    debug_err(("gCapRawAddr: 0x%x MemorySize:0x%x \r\n", gCapRawAddr, uiTmp));

    Get_Sensor_CalInfor();
    ReadCalData();

//#NT#2009/09/18#Jarkko Chang -begin
    //ide_disable_osd(IDE_OSDID_1);
//#NT#20091117#ethanlau -begin
    //Photo_SetImageRatio(_Ratio_4_3);
//#NT#20091117#ethanlau -end
//    ide_disable_video(IDE_VIDEOID_1);
    // Init Photo and Capture task and open
    AppInit_ModePhoto();
    // Control CCD/LCD power on/off, lens, shutter...
    DevCtrl_ModePhoto();

    SysClk_SetClk(SPEED_ID_PHOTO);
//#NT#2009/09/18#Jarkko Chang -end

    switch( AlgCal_item)
    {
        case CAL_ISO:
            ErrorCode=ISOAdjust();
            break;
        case CAL_MSHUTTER:
            ErrorCode=MshutterAdjust();
            break;
        case CAL_DEFECTPIXEL:
            ErrorCode=DP_Adjust();
            break;
        case CAL_AWB:
            ErrorCode=AWB_Adjust();
            break;
        case CAL_FLASH:
            ErrorCode=FlashlightAEAdjust();
            break;
        case CAL_FLASH_AWB:
            ErrorCode=FlashlightAWBAdjust();
            break;
        case CAL_FLASH_REDEYE_TEST:
            ErrorCode=FlashlightRedEyeTest();
            break;
        case CAL_FLASH_GNO_SHOTS:
            ErrorCode=FlashlightGnoShots();
            break;
        case CAL_AF_FAR:
            ErrorCode=FarFocusAdjust();
            break;
        case CAL_AF_NEAR:
            ErrorCode=NearFocusAdjust();
            break;
        case CAL_WR2FILE:
            ErrorCode=WriteAdjustData2File(1);
            break;
        case CAL_READSTATUS:
            ErrorCode=ReadCalStatus();
            break;
        case CAL_LENS:
            break;
        case CAL_OB:
            ErrorCode=OBAdjust();
            break;
        case CAL_VIG:
            ErrorCode=LensShading_Adjust();
            break;
        case CAL_DEFECTPIXEL_WHITE:
            ErrorCode=DP_Adjust_White();
            break;
        case CAL_DEFECTPIXEL_DARK:
            ErrorCode=DP_Adjust_Dark();
            break;
        case CAL_MSHUTTER_LASTING:
            ErrorCode=MshutterLasting();
            break;

        case CAL_MSHUTTER_SW_TEST:
            ErrorCode=MshutterSwitchTest();
            break;
        case CAL_APERTURE_SW_TEST:
            ErrorCode=IrisSwitchTest();
            break;
        case CAL_FAE_TEST:
            ErrorCode=AFETest();
            break;
        case CAL_ZOOM_SPEED_TEST:
            ErrorCode=ZoomSpeedTest();
            break;
        case CAL_ZOOM_STEPMISS_TEST:
            ErrorCode=ZoomStepMissTest();
            break;
        case CAL_ZOOM_STOP2PR://JJ0228
            ErrorCode=ZoomPRAdjust();
            break;
        case CAL_FOCUS_STEPMISS_TEST:
            ErrorCode=FocusStepMissTest();
            break;
        case CAL_AF_TEST:
            ErrorCode=AFTest();
            break;
        case CAL_STAIN_TEST:
            ErrorCode=StainTest();
            break;
        case CAL_TVRESOLUTION_TEST:
            ErrorCode=TVResolutionTest();
            break;
        case CAL_FOCUSOFFSET://JJ0325
            ErrorCode=FocusOffsetAdjust();
            break;
        //#NT#2010/12/15#JJ Huang -begin
        //#NT#For SNR Measure
        case CAL_SNR:
            ErrorCode=SNRTest();
            break;
        //#NT#2010/12/15#JJ Huang -end

        default:
            debug_ind(("No this item\r\n"));
            break;
    }

    gCapRawAddr = NULL;

    return ErrorCode;
}

//----------------------------------------------------------------------------------------------------
void DP_PrvCfg(UINT addr,UINT Xsize,UINT Ysize)
{
    //JJ_433Cal: use new api// sie_Set2Preview(GetSieXcenter(), GetSieYcenter(), Xsize, Ysize, Xsize, 1, addr);
    sie_Load();
    sie_Reset(0);

    sie_SensorDataEnable(1);
    //#NT#2009/09/01#KS Hung -begin
    //SensorDelayVD(2);
    sie_SensorDataEnable(0);
    //SensorDelayVD(2);
    pre_SetPre2DramPath(0);//#NT#2007/10/23#JJ Huang     sie_Set_OutSrc(0);            // 0:pre color gain
    pre_FunctionEnable(DISABLE, PRE_HSCL_EN);

    if(DFPC_TEST)
    {
//        sie_Set_DefectPixel(DP_Addr_Prv,DP_Addr_Cap);
        //JJ_433CAL: need review// pre_SetInputStartAdd2(prv dp addr);
        pre_FunctionEnable(ENABLE, PRE_BLM_EN);
    }
    else
    {
        pre_FunctionEnable(DISABLE, PRE_BLM_EN);
        //sie_Set_DefectPixel(0,0);
        //JJ_433CAL: need review// pre_SetInputStartAdd2(0);
    }
    sie_SensorDataEnable(1);
    //SensorDelayVD(6);
    sie_SensorDataEnable(0);
    //SensorDelayVD(2);
    //#NT#2009/09/01#KS Hung -end
}

//----------------------------------------------------------------------------------------------------
void SortDPdata(UINT32* Data,UINT16 Num)
{
    UINT i,j,temp;

    for(i=Num;i>1;i--)
    {
        for(j=0;j<(i-1);j++)
        {
            if(Data[j]>Data[j+1])
            {
                temp=Data[j];
                Data[j]=Data[j+1];
                Data[j+1]=temp;
            }
        }
    }
}

void SortDPdata2Fld(UINT32* Data,UINT16 Num)
{
    UINT32 i,j,temp, NumIdx, X, Y;
    UINT16 DP_Num_Field_Cap[SensorCapTotalFld]={0};//JJ added, 20050706
    UINT32 *Field_Defect_Pixel[SensorCapTotalFld];//JJ added, 20050706
    //UINT8 Field_Num[Block_Y_Field_Cap]={(UINT8)Field_Num1,(UINT8)Field_Num2,(UINT8)Field_Num3,(UINT8)Field_Num4,
    //                                (UINT8)Field_Num5,(UINT8)Field_Num6,(UINT8)Field_Num7,(UINT8)Field_Num8};
    UINT CapRawAddr;
    UINT8 Field_Num[Block_Y_Field_Cap]={Field_Num1, Field_Num2, Field_Num3, Field_Num4, Field_Num5, Field_Num6, Field_Num7, Field_Num8};

    debug_err(("SortDPdata2Fld:  total num = %d\r\n", Num));
    //JJ_433CAL: need a compatible api// Get_fieldNum((UINT8*)Field_Num);//#NT#2007/10/23#JJ Huang     sie_Get_fieldNum((UINT8*)Field_Num);
    //memory setting
    CapRawAddr=ippbuf_GetBuf(IPPBUF_CAP_JPG_1);
    j=0;
    for(i=0; i<SensorCapTotalFld; i++)
    {
        Field_Defect_Pixel[i] = (UINT32*)(CapRawAddr+j);
        j += DP_Cap_MAX*4;
    }
    //de-group
    for(i=0;i<Num;i++)
    {
        temp=Data[i];
        if(temp==0xffffffff)//in case 'Num' is larger than correct value
            break;

        X = (temp&0xffff        );
        Y = ((temp>>16)&0xffff    );
        NumIdx = (Y)%SensorCapTotalFld;
        //debug_err(("Sort-DP%4d Fld%3d/%3d (%4d,%4d->%4d)\r\n",i,NumIdx,Field_Num[NumIdx],X,Y,Y/SensorCapTotalFld));
        Y = (Y/SensorCapTotalFld);
//        X += CapFldOffsetX;
//        Y += CapFldOffsetY;
        temp = (X&0xffff) | ((Y<<16)&0xffff0000);

        *(Field_Defect_Pixel[NumIdx]+DP_Num_Field_Cap[NumIdx]) = temp;
        DP_Num_Field_Cap[NumIdx]++;
    }

    //add ending
    for(i=0;i<SensorCapTotalFld;i++)
    {
        NumIdx = Field_Num[i];
        *(Field_Defect_Pixel[NumIdx]+DP_Num_Field_Cap[NumIdx]) = 0xffffffff;
        DP_Num_Field_Cap[NumIdx] ++;//count endding symbol
        AAA_Adjust.DP_Cap_Num[i] = (DP_Num_Field_Cap[NumIdx]);
        debug_err(("SortDPdata2Fld:  fld-%d num = %d\r\n", i,DP_Num_Field_Cap[NumIdx]));
    }
#if 0
    //check
    for(i=0;i<SensorCapTotalFld;i++)
    {
        NumIdx = Field_Num[i];
        if(DP_Num_Field_Cap[NumIdx]>2032)
            return;//field dp numbers unacceptable
    }
#endif
    //cleaning
    memset(Data, 0xAA, (DP_Cap_MAX*4));
    //connect
#if 0
    j=0;
    for(i=0;i<SensorCapTotalFld;i++)
    {
        NumIdx = Field_Num[i];
        if((j+DP_Num_Field_Cap[NumIdx])<=2048)
        {//one block copy
            //NumIdx = Field_Num[i];
            memcpy( Data+j+(2064*i),
                    Field_Defect_Pixel[NumIdx],
                    4*(DP_Num_Field_Cap[NumIdx]));
            j += DP_Num_Field_Cap[NumIdx];
            j = j%2048;
            if(i<(SensorCapTotalFld-1))
            {//prepare 16 DP for next field since SIE got a chance to grab first
                Y=i+1;
                X=16;
                while(X>0 && Y<(SensorCapTotalFld))
                {
                    NumIdx = Field_Num[Y];

                    temp = (DP_Num_Field_Cap[NumIdx]<X)?DP_Num_Field_Cap[NumIdx]:X;
                    memcpy( Data+j+(2064*i)+(16-X),
                            Field_Defect_Pixel[NumIdx],
                            4*(temp));
                    X -= temp;
                    Y ++;
                }
            }
        }
        else
        {//two block copy
            //NumIdx = Field_Num[i];
            memcpy( Data+j+(2064*(i)),
                    Field_Defect_Pixel[NumIdx],
                    4*(2048-j));
            memcpy( Data+(2064*(i)),
                    (Field_Defect_Pixel[NumIdx]+(2048-j)),
                    4*((j+DP_Num_Field_Cap[NumIdx])-2048));
            j += DP_Num_Field_Cap[NumIdx];
            j = j%2048;
            if(i<(SensorCapTotalFld-1))
            {//prepare 16 DP for next field since SIE got a chance to grab first
                Y=i+1;
                X=16;
                while(X>0 && Y<(SensorCapTotalFld))
                {
                    NumIdx = Field_Num[Y];

                    temp = (DP_Num_Field_Cap[NumIdx]<X)?DP_Num_Field_Cap[NumIdx]:X;
                    memcpy( Data+j+(2064*i)+(16-X),
                            Field_Defect_Pixel[NumIdx],
                            4*(temp));
                    X -= temp;
                    Y ++;
                }
                //JJ test 0309
                memcpy(Data+(2064*i)+2048,Data+(2064*i),16*4);
            }
        }
    }
#else
    j=0;
    for(i=0;i<SensorCapTotalFld;i++)
    {
        NumIdx = Field_Num[i];
        memcpy( Data+j,
                Field_Defect_Pixel[NumIdx],
                4*(DP_Num_Field_Cap[NumIdx]));
        j += DP_Num_Field_Cap[NumIdx];
    }
#endif
}
//----------------------------------------------------------------------------------------------------
BOOL CheckContinueDP(UINT32* Data, UINT16 Num, UINT16 mode)
{
    UINT i,count1,count2,Max_Continue_DP;

    if(mode==Prv_Mode)
        Max_Continue_DP=CalInfor.MaxContDPPrv;
    else if(mode==Cap_Mode)
        Max_Continue_DP=CalInfor.MaxContDPCap;
//    else if(mode==Dzoom_Mode)
//            Max_Continue_DP=Max_Continue_DP_Dzoom;

    if(Num>1)
    {
        for(i=0;i<(Num-1);i++)
        {
        if((Data[i]&0xffff0000)==(Data[i+1]&0xffff0000))
        {
            if((Data[i+1]-Data[i])==1)//pixel coutinue
                count1++;
            else
                count1=0;

            if((Data[i+1]-Data[i])==2)//the same color continue
                count2++;
            else
                count2=0;

            if((count1>=Max_Continue_DP)||(count2>=(Max_Continue_DP/2)))
                return CAL_TRUE;
        }
        else
        {
            count1=0;
            count2=0;
        }
    }
    return CAL_FALSE;
}
    else
    {
        return CAL_FALSE;
    }
}

void WriteAdjustData1(char state)
{
    char msg;
//    char Buff[64];
    UINT16 i;
    char DPfile[16]="A:\\AdjData0.txt";

    HNVT_FILE     *pFile;

    //DPfile[10]= '0'+state;
    pFile = fs_fopen(DPfile,"w");
    if(pFile == NULL)
        debug_ind(("File open fail !!\r\n"));

    msg=sprintf(message,"DP:Prv w=%d b=%d,Cap w=%d b=%d \r\n",
        AAA_Adjust.DP_Status.Prv_White,AAA_Adjust.DP_Status.Prv_Black,
        AAA_Adjust.DP_Status.Cap_White,AAA_Adjust.DP_Status.Cap_Black);//,AAA_Adjust.DP_Status.Dzoom_White,AAA_Adjust.DP_Status.Dzoom_Black);
    fs_fwrite(message, 1, msg, pFile);

    msg=sprintf(message,"OB: Code=0x%x  value=%x\r\n",AAA_Adjust.OB_Status,AAA_Adjust.OB_Min);
    fs_fwrite(message, 1, msg, pFile);

    msg=sprintf(message,"AWB: Code=0x%x  value=%x %x\r\n",AAA_Adjust.AWB_Status,AAA_Adjust.AWB.Rgain,AAA_Adjust.AWB.Bgain);
    fs_fwrite(message, 1, msg, pFile);

    msg=sprintf(message,"ISO: Code=0x%x  value=%x\r\n",AAA_Adjust.ISO_Status,AAA_Adjust.Agc_ISOMin);
    fs_fwrite(message, 1, msg, pFile);

    msg=sprintf(message,"FTP: Code=0x%x  value=%x\r\n",AAA_Adjust.FTP_Status,AAA_Adjust.FlashTriggerPoint);
    fs_fwrite(message, 1, msg, pFile);

//    msg=sprintf(message,"Flash: Code=0x%x  value=%x %x\r\n",AAA_Adjust.FlashAE_Status,AAA_Adjust.Flash_Y_Avg,AAA_Adjust.FlashAgc_Cap);
//    fs_fwrite(message, 1, msg, pFile);

    msg=sprintf(message,"Shade: Code=0x%x  value= ",AAA_Adjust.Shading_Status);
    fs_fwrite(message, 1, msg, pFile);
    for (i=0;i<4;i++)
    {
        msg=sprintf(message,"%x ",AAA_Adjust.Shading_Gain[i]);
        fs_fwrite(message, 1, msg, pFile);
        if((i+1)%4==0)
        {
            msg=sprintf(message,"\r\n");
            fs_fwrite(message, 1, msg, pFile);
        }
    }

    msg=sprintf(message,"ISO_Status: Code=0x%x  value=%x \r\n",AAA_Adjust.ISO_Status,AAA_Adjust.Agc_ISOMin);
    fs_fwrite(message, 1, msg, pFile);

    msg=sprintf(message,"Prv DP\r\n");
    fs_fwrite(message, 1, msg, pFile);
    for(i=0;i<=DP_Prv_MAX;i++)
    {
        msg=sprintf(message,"%8lx ",AAA_Adjust.Defect_Pixel[i]);
        fs_fwrite(message, 1, msg, pFile);
        if((i+1)%8==0)
        {
            msg=sprintf(message,"\r\n");
            fs_fwrite(message, 1, msg, pFile);
        }
        if(AAA_Adjust.Defect_Pixel[i]==0xffffffff)
            break;
    }
    msg=sprintf(message,"\r\n");
    fs_fwrite(message, 1, msg, pFile);
    msg=sprintf(message,"Cap DP\r\n");
    fs_fwrite(message, 1, msg, pFile);
    for(i=0;i<=DP_Cap_MAX;i++)
    {
        msg=sprintf(message,"%8lx ",AAA_Adjust.Defect_Pixel[i+DP_Prv_MAX]);
        fs_fwrite(message, 1, msg, pFile);
        if((i+1)%8==0)
        {
            msg=sprintf(message,"\r\n");
            fs_fwrite(message, 1, msg, pFile);
        }
        if((i>160)||(AAA_Adjust.Defect_Pixel[i+DP_Prv_MAX]==0xffffffff))
        //if((AAA_Adjust.Defect_Pixel[i+DP_Prv_MAX]==0xffffffff))
            break;
    }
    /*
    msg=sprintf(message,"\r\n");
    fs_fwrite(message, 1, msg, pFile);
    msg=sprintf(message,"Dzoom DP\r\n");
    fs_fwrite(message, 1, msg, pFile);
    for(i=0;i<=DP_Dzoom_MAX;i++)
    {
        msg=sprintf(message,"%8x ",AAA_Adjust.DP_Dzoom[i]);
        fs_fwrite(message, 1, msg, pFile);
        if((i+1)%8==0)
        {
            msg=sprintf(message,"\r\n");
            fs_fwrite(message, 1, msg, pFile);
        }
        if(AAA_Adjust.DP_Dzoom[i]==0xffffffff)
            break;
    }
    */
    fs_fclose(pFile);
//    sprintf(message,"Hello");
//    DisplayString(50,50,message);

}

void DP_Correction(UINT32 BayAdd, UINT16 CenterX, UINT16 CenterY, UINT16 Window_w, UINT16 Window_h, UINT32 DPAddr)
{
    UINT16 Pixel_Data[8][2],Pixel_Diff[8],Area_Avg,x,y,Cnt,Image_Width_Cap;
    UINT8 Min_Value,Max_Value,i,Min_Diff,Min_Dir=0;
    UINT16 StartX,StartY,EndX,EndY;

    StartX=CenterX-(Window_w/2);
    EndX=CenterX+(Window_w/2);
    StartY=CenterY-(Window_h/2);
    EndY=CenterY+(Window_h/2);

    debug_ind(("Correct Defect Pixel...\r\n"));
    Image_Width_Cap=IMAGE_SIZE_H;

    Cnt=0;
    //BayAdd=Sensor_Offset_0x00;//+2*Image_Width+2;//start address=(2,2)
    while(Cnt<=DP_Cap_MAX)
    {
        //x=(Defect_Pixel_Cap[Cnt])&0xffff;
        //y=(Defect_Pixel_Cap[Cnt]>>16)&0xffff;
#if 0 //original code
        x=(AAA_Adjust.Defect_Pixel[DP_Prv_MAX+Cnt])&0xffff;
        y=(AAA_Adjust.Defect_Pixel[DP_Prv_MAX+Cnt]>>16)&0xffff;
#else //JJ added, 20050711, AAA_Adjust may be polluted while doing DP calibration
        x=(*(UINT32*)(DPAddr+Cnt*4))&0xffff;
        y=(*(UINT32*)(DPAddr+Cnt*4)>>16)&0xffff;
#endif

        if(x==0xffff && y==0xffff)
            break;

        if((x<StartX) || (x>EndX) || (y<StartY) || (y>EndY))
        {
            Cnt++;
            continue;
        }
        else
        {
            x-=StartX;
            y-=StartY;
        }
        Pixel_Data[0][0]=*(UINT8 *)(BayAdd+(y-2)*Image_Width_Cap+(x-2));
        Pixel_Data[0][1]=*(UINT8 *)(BayAdd+(y-4)*Image_Width_Cap+(x-4));
        Pixel_Data[1][0]=*(UINT8 *)(BayAdd+(y-2)*Image_Width_Cap+(x));
        Pixel_Data[1][1]=*(UINT8 *)(BayAdd+(y-4)*Image_Width_Cap+(x));
        Pixel_Data[2][0]=*(UINT8 *)(BayAdd+(y-2)*Image_Width_Cap+(x+2));
        Pixel_Data[2][1]=*(UINT8 *)(BayAdd+(y-4)*Image_Width_Cap+(x+4));
        Pixel_Data[3][0]=*(UINT8 *)(BayAdd+(y)*Image_Width_Cap+(x-2));
        Pixel_Data[3][1]=*(UINT8 *)(BayAdd+(y)*Image_Width_Cap+(x-4));
        Pixel_Data[4][0]=*(UINT8 *)(BayAdd+(y)*Image_Width_Cap+(x+2));
        Pixel_Data[4][1]=*(UINT8 *)(BayAdd+(y)*Image_Width_Cap+(x+4));
        Pixel_Data[5][0]=*(UINT8 *)(BayAdd+(y+2)*Image_Width_Cap+(x-2));
        Pixel_Data[5][1]=*(UINT8 *)(BayAdd+(y+4)*Image_Width_Cap+(x-4));
        Pixel_Data[6][0]=*(UINT8 *)(BayAdd+(y+2)*Image_Width_Cap+(x));
        Pixel_Data[6][1]=*(UINT8 *)(BayAdd+(y+4)*Image_Width_Cap+(x));
        Pixel_Data[7][0]=*(UINT8 *)(BayAdd+(y+2)*Image_Width_Cap+(x+2));
        Pixel_Data[7][1]=*(UINT8 *)(BayAdd+(y+4)*Image_Width_Cap+(x+4));
        Area_Avg=0;
        Min_Value=255;
        Max_Value=0;
        Min_Diff=255;
        for(i=0;i<8;i++)
        {
            if(Pixel_Data[i][0]<Min_Value)
                Min_Value=Pixel_Data[i][0];
            if(Pixel_Data[i][0]>Max_Value)
                Max_Value=Pixel_Data[i][0];
            Area_Avg+=Pixel_Data[i][0];
            Pixel_Diff[i]=abs(Pixel_Data[i][0]-Pixel_Data[i][1]);
            if(Pixel_Diff[i]<Min_Diff)
            {
                Min_Diff=Pixel_Diff[i];
                Min_Dir=i;
            }
        }
        Area_Avg/=8;
        if((Max_Value-Min_Value)<=20)
            *(UINT8 *)(BayAdd+y*Image_Width_Cap+x)=Area_Avg;//don't have much difference, set to average
        else
        {
            if(Pixel_Data[Min_Dir][1]>Pixel_Data[Min_Dir][0])
            {
                if(Pixel_Data[Min_Dir][0]>Min_Diff)
                    *(UINT8 *)(BayAdd+y*Image_Width_Cap+x)=Pixel_Data[Min_Dir][0]-Min_Diff;
                else
                    *(UINT8 *)(BayAdd+y*Image_Width_Cap+x)=0;
            }
            else
            {
                if((Pixel_Data[Min_Dir][0]+Min_Diff)<=255)
                    *(UINT8 *)(BayAdd+y*Image_Width_Cap+x)=Pixel_Data[Min_Dir][0]+Min_Diff;
                else
                    *(UINT8 *)(BayAdd+y*Image_Width_Cap+x)=255;
            }
        }
        Cnt++;
    }
    debug_ind(("Total count=%d\r\n",Cnt));

}

void SetCalFlag(UINT value)
{
    guiCalFlags=value;
}

UINT GetCalFlag(void)
{
    return guiCalFlags;
}

void SaveRawData(UINT RawAdd,UINT ImWidth,UINT ImHeight,UINT Code)
{
    UINT Size;
    Size=ImWidth*ImHeight;//Raw Data size
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UINT32 Cmd,
                    (UINT8*)RawAdd,//    UINT8 *pBuf,
                    Size,//UINT32 BufSize,
                    0,//UINT32 Pos,
                    0,//BOOL CloseFile,
                    FST_FMT_JPG,//UINT32 FileFormat,
                    FST_TIME_NO_WAIT);
}

void ClearScreen(void)
{
    RECT Param;

    if(fnCalShowString != NULL)
    {
        Param.uiLeft = 0;
        Param.uiTop = 0;
        Param.uiHeight = 240;
        Param.uiWidth = 320;
        fnCalShowString("", &Param, TRUE);
    }
}

void Cal_AlgShowString(char *pStr, UINT16 X, UINT16 Y)
{
#if 1
    RECT g_CalStringRect;
    Cal_FillRect(&g_CalStringRect, _OSD_INDEX_BLACK);
    g_CalStringRect.uiLeft    = X;
    g_CalStringRect.uiTop     = Y;
    g_CalStringRect.uiWidth   = 300;
    g_CalStringRect.uiHeight  = 30;
    // Show item number and name
    Cal_ShowString(pStr, &g_CalStringRect, TRUE);
#else
    RECT rect;

    rect.uiLeft = X;
    rect.uiTop = Y;
    rect.uiWidth = 300;//OSD_W;
    rect.uiHeight = 30;//OSD_H;

    //Cal_ShowString(pStr, &rect, TRUE);
    if(fnCalShowString)
    {
        fnCalShowString(pStr, &rect, TRUE);
    }
#endif
}

void Cal_AlgClearScreen(void)
{
    RECT Param;
    Param.uiLeft = 0;
    Param.uiTop = 0;
    Param.uiHeight = OSD_H;
    Param.uiWidth = OSD_W;

    //Cal_ShowString("", &Param, TRUE);
    if(fnCalShowString != NULL)
    {
        fnCalShowString("", &Param, TRUE);
    }
}

void Cal_AlgDisplayData(UINT16 x,UINT16 y,UINT32 Data)
{
    char str[64];
    sprintf(str,"%ld",Data);
    Cal_AlgShowString(str,x,y);
}

void Cal_AlgDisplayErrorCode(UINT16 x,UINT16 y,UINT8 code)
{
    char str[20];
    sprintf(str,"ERROR CODE %d",code);
    Cal_AlgShowString(str,x,y);
}

#define DFP_HISTO_BIN_NUM 64
void DFP_Bayer_getHisto(UINT16 *puiHisto, UINT8 fieldNum, UINT32 BayAdd, UINT16 width, UINT16 height, UINT16 LineOffset, UINT16 OffsetY, UINT16 Block_X, UINT16 Block_Y)
{
    UINT16 i,j,X,Y, m, n;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY,ExtraX,ExtraY;
    UINT16 blockX, blockY;
    int Diff;
    UINT32 uiBuf;
    UINT8 ucBuf;
    UINT32 Yadd, Xadd, Jadd, Iadd, JaddIncrement;
    UINT8 BayInd;
    UINT32 BayAvg[4], BaySum[4];

    {//compute Blocking parameters
        EndBlockX=width/Block_X;
        EndBlockY=height/Block_Y;
        RemainX=width-(EndBlockX*Block_X);
        RemainY=height-(EndBlockY*Block_Y);
    }

    for(i=0; i<DFP_HISTO_BIN_NUM*fieldNum; i++)
     {//clean Histogram
          puiHisto[i] = 0;
       }


    {//initial before j-loop
        Jadd = BayAdd;
        JaddIncrement = Block_Y*LineOffset;
    }

    for(j=0;(j<EndBlockY);j++)
    {

        {//initial before i-loop
            Iadd = Jadd;
        }

        for(i=0;(i<EndBlockX);i++)
        {
            {//setting
                //block size
                if(i==(EndBlockX-1))
                    ExtraX=RemainX;
                else
                    ExtraX=0;
                if(j==(EndBlockY-1))
                    ExtraY=RemainY;
                else
                    ExtraY=0;
                blockY = (Block_Y+ExtraY);
                blockX = (Block_X+ExtraX);
                //summation buffer cleaning
                for(X=0;X<4;X++)
                    BaySum[X] = 0;
            }

            {//initial before Y-loop
                Yadd = Iadd;
                BayInd=0x0;
            }

            for(Y=0;Y<blockY;Y++)
            {
                {//initial before X-loop
                    Xadd = Yadd;
                }

                {//X=0 Job
                    uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                    uiBuf = uiBuf>>((Xadd&0x3)<<3);//"<<3" = "*8"
                    ucBuf = uiBuf;
                    BaySum[BayInd] += ucBuf;
                    {//loop accumulation
                        Xadd += 1;
                        BayInd = BayInd ^ 0x1;
                    }
                }

                for(X=1;X<blockX;X++)
                {
                    if(Xadd&0x3)
                        uiBuf = uiBuf>>8;
                    else
                        uiBuf = *((UINT32*)Xadd);
                    ucBuf = uiBuf;
                    BaySum[BayInd] += ucBuf;
                    {//loop accumulation
                        Xadd += 1;
                        BayInd = BayInd ^ 0x1;
                    }
                }

                {//loop accumulation
                    Yadd += LineOffset;
                    BayInd = BayInd ^ 0x2;
                }
            }

            {//averaging
                uiBuf = ((blockY)*(blockX))>>2;//"<<2" = "/4"
                for(X=0;X<4;X++)
                    BayAvg[X] = BaySum[X]/uiBuf;
            }

            {//initial before Y-loop
                Yadd = Iadd;
                BayInd=0x0;
            }

            for(Y=0;Y<blockY;Y++)
            {
                {//initial before X-loop
                    Xadd = Yadd;
                }

                for(X=0;X<blockX;X++)
                {
                    {//getting data in byte
                        if((!(Xadd&0x3)) || X==0)
                        {
                            uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                            if(X==0)
                                uiBuf = uiBuf>>(8*(Xadd&0x3));
                        }
                        else
                            uiBuf = uiBuf>>8;
                        ucBuf = uiBuf;
                    }

                    {//compute Diff
///                        if(environment==Status_Black)
///                            Diff=ucBuf-Avg;//don't mind dark pixel in the dark
///                        else if(environment==Status_White)
                            Diff = ucBuf-BayAvg[BayInd];
                            if(Diff<0)
                                Diff = -Diff;
                    }

                    {//check on histogram
                        m = (j*Block_Y+Y+OffsetY)%fieldNum;
                        n = (Diff<DFP_HISTO_BIN_NUM)?Diff:(DFP_HISTO_BIN_NUM-1);
                        if(puiHisto[m*DFP_HISTO_BIN_NUM+n]<0xffff)
                            puiHisto[m*DFP_HISTO_BIN_NUM+n]++;
                    }


                    {//loop accumulation
                        BayInd = BayInd ^ 0x1;
                        Xadd += 1;
                    }
                }//loop X

                {//loop accumulaiton
                    BayInd = BayInd ^ 0x2;
                    Yadd += LineOffset;
                }
            }//loop Y

            {//loop accumulation
                Iadd += Block_X;
            }

        }//loop i

        {//loop accumulation
            Jadd += JaddIncrement;
        }

    }//loop j
}

UINT32 DFP_Bayer_Search(UINT32 BayAdd, UINT16 width, UINT16 height, UINT16 LineOffset, UINT32 DPadd, UINT16 OffsetX, UINT16 OffsetY, UINT16 StartCnt, UINT16 MaxCnt,UINT8 Threshold, UINT16 Block_X, UINT16 Block_Y)
{
    UINT16 i,j,X,Y,m;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY,ExtraX,ExtraY;
    UINT16 blockX, blockY;
    INT Diff;
    UINT32 uiBuf, uiBuf2;
    UINT8 ucBuf;
    UINT32 Yadd, Xadd, Jadd, Iadd, JaddIncrement;
    UINT8 BayInd;
    UINT32 BayAvg[4], BaySum[4];
    UINT32 DPadd_Cur, DPadd_Tmp;
    UINT8 DP_Double_Check, DP_Double;
    UINT16 DP_Cnt;
    UINT32 returnValue;
    BOOL autoThreshold;
    UINT8 puiDynamicThreshold[SensorCapTotalFld];
    UINT16 puiHisto[SensorCapTotalFld*DFP_HISTO_BIN_NUM];
    UINT16 fieldStartCnt[SensorCapTotalFld]={0};

    autoThreshold = (Threshold==0)?TRUE:FALSE;

    debug_err(("DFP_Bayer_Search: OffsetX=%d, OffsetY=%d\r\n",OffsetX,OffsetY));

    debug_err(("Threshold: %d\r\n", Threshold));
    if(autoThreshold)
    {
        DFP_Bayer_getHisto(puiHisto, SensorCapTotalFld, BayAdd, width,
        height, LineOffset, OffsetY, Block_X, Block_Y);

        DPadd_Tmp=DPadd;
        debug_err(("fieldStartCnt: \r\n"));
        for(i=0; i<StartCnt; i++)
        {
            uiBuf = *(UINT32 volatile*)(DPadd_Tmp);
            uiBuf = (uiBuf>>16)&0xffff;
            fieldStartCnt[uiBuf%SensorCapTotalFld]++;
            DPadd_Tmp += 4;
        }
        //debug_err(("%d %d %d %d", fieldStartCnt[0], fieldStartCnt[1], fieldStartCnt[2], fieldStartCnt[3]));
        //debug_err(("\r\n"));

        debug_err(("DFP_Bayer_getHisto: \r\n"));
        for(j=0; j<SensorCapTotalFld; j++)
        {
            debug_err(("field %d :    ", j));
            m=0;
            for(i=0; i<DFP_HISTO_BIN_NUM; i++)
            {
                debug_err(("%d ", puiHisto[j*DFP_HISTO_BIN_NUM+i]));
            }
            debug_err(("\r\n"));
        }
        debug_err(("\r\n"));


        debug_err(("dynamicThreshold: \r\n"));
        for(j=0; j<SensorCapTotalFld; j++)
        {
            debug_err(("field %d : MAX %d   ", j, ((MaxCnt-StartCnt)/SensorCapTotalFld-fieldStartCnt[j])));
            m=0;
            for(i=(DFP_HISTO_BIN_NUM-1); i>0; i--)
            {
                m+=puiHisto[j*DFP_HISTO_BIN_NUM+i];
                //debug_err(("<%d,%d>", m,((MaxCnt-StartCnt)/SensorCapTotalFld-fieldStartCnt[j])));
                if(m>((MaxCnt-StartCnt)/SensorCapTotalFld-fieldStartCnt[j]))
                    break;
            }
            puiDynamicThreshold[j] = i;
            debug_err(("%d :#%d \r\n", i,m-puiHisto[j*DFP_HISTO_BIN_NUM+i]));
        }
        debug_err(("\r\n"));

        Threshold=puiDynamicThreshold[SensorCapTotalFld-1];
        debug_err(("Threshold: %d\r\n", Threshold));
    }

    EndBlockX=width/Block_X;
    EndBlockY=height/Block_Y;
    RemainX=width-(EndBlockX*Block_X);
    RemainY=height-(EndBlockY*Block_Y);
    DP_Cnt = StartCnt;
    DPadd_Cur = DPadd + (StartCnt<<2);
    if(StartCnt)
    {
        DP_Double_Check=1;
    }
    else
    {
        DP_Double_Check=0;
    }

    Jadd = BayAdd;
    JaddIncrement = Block_Y*LineOffset;

    for(j=0;(j<EndBlockY);j++)
    {
        //initial before i-loop
        Iadd = Jadd;
        for(i=0;(i<EndBlockX);i++)
        {
            //setting
            //block size
            if(i==(EndBlockX-1))
            {
                ExtraX=RemainX;
            }
            else
            {
                ExtraX=0;
            }
            if(j==(EndBlockY-1))
            {
                ExtraY=RemainY;
            }
            else
            {
                ExtraY=0;
            }
            blockY = (Block_Y+ExtraY);
            blockX = (Block_X+ExtraX);
            //summation buffer cleaning
            for(X=0;X<4;X++)
            {
                BaySum[X] = 0;
            }

            //initial before Y-loop
            Yadd = Iadd;
            BayInd=0x0;

            for(Y=0;Y<blockY;Y++)
            {
                //initial before X-loop
                Xadd = Yadd;


                //X=0 Job
                uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                uiBuf = uiBuf>>((Xadd&0x3)<<3);//"<<3" = "*8"
                ucBuf = uiBuf;
                BaySum[BayInd] += ucBuf;
                 //loop accumulation
                Xadd += 1;
                BayInd = BayInd ^ 0x1;

                for(X=1;X<blockX;X++)
                {
                    if(Xadd&0x3)
                    {
                        uiBuf = uiBuf>>8;
                    }
                    else
                    {
                        uiBuf = *((UINT32*)Xadd);
                    }
                    ucBuf = uiBuf;
                    BaySum[BayInd] += ucBuf;
                    //loop accumulation
                    Xadd += 1;
                    BayInd = BayInd ^ 0x1;
                }

                //loop accumulation
                Yadd += LineOffset;
                BayInd = BayInd ^ 0x2;
            }

            //averaging
            uiBuf = ((blockY)*(blockX))>>2;//"<<2" = "/4"
            for(X=0;X<4;X++)
            {
                BayAvg[X] = BaySum[X]/uiBuf;
            }

            //initial before Y-loop
            Yadd = Iadd;
            BayInd=0x0;

            for(Y=0;Y<blockY;Y++)
            {
                //initial before X-loop
                Xadd = Yadd;

                if(autoThreshold)
                {
                    m = (j*Block_Y+Y+OffsetY)%SensorCapTotalFld;
                    Threshold = puiDynamicThreshold[m];
                }
                for(X=0;X<blockX;X++)
                {
                    //getting data in byte
                    if((!(Xadd&0x3)) || X==0)
                    {
                        uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                        if(X==0)
                        uiBuf = uiBuf>>(8*(Xadd&0x3));
                    }
                    else
                    {
                        uiBuf = uiBuf>>8;
                    }
                    ucBuf = uiBuf;

                    //compute Diff
                    Diff = ucBuf-BayAvg[BayInd];
                    if(Diff<0)
                    {
                        Diff = -Diff;
                    }


                    if(Diff>Threshold)
                    {
                        //recording DP found
                        uiBuf2 = ((((j*Block_Y+Y+OffsetY)<<16)&0xffff0000)|((i*Block_X+X+OffsetX)&0xffff));
                        /*
                        debug_err(("DP%4d 0x%08x=(%4d,%3d) Val%3d Avg%3d Diff%3d\r\n",
                        DP_Cnt,
                        ((((j*Block_Y+Y+OffsetY)<<16)&0xffff0000)+((i*Block_X+X+OffsetX)&0xffff)),
                        (((j*Block_Y+Y+OffsetY))&0xffff), ((i*Block_X+X+OffsetX)&0xffff),
                        ucBuf, BayAvg[BayInd], Diff));
                        */
                        //DP double/DP repeat check
                        DP_Double=0;
                        if(DP_Double_Check)
                        {
                            DPadd_Tmp=DPadd;
                            for(m=0;m<StartCnt;m++)
                            {
                                if(*(UINT32 volatile*)(DPadd_Tmp)==uiBuf2)
                                {
                                    DP_Double=1;
                                    break;
                                }
                                DPadd_Tmp += 4;
                            }
                        }

                        if(!DP_Double)
                        {
                            if(DP_Cnt>=MaxCnt)
                            {
#if DP_SEARCH_TILL_END
                                goto DP_SEARCH_TILL_END_JUMP_END;
#else
                                goto DP_SEARCH_TOO_MUCH;
#endif
                            }

                            //update DP buffer
                            *(UINT32 volatile*)(DPadd_Cur)=uiBuf2;
#if DP_SEARCH_TILL_END
                            DP_SEARCH_TILL_END_JUMP_END:
#endif
                            //update parameters
                            DP_Cnt++;
                            DPadd_Cur += 4;
                        }
                    }

                    //loop accumulation
                    BayInd = BayInd ^ 0x1;
                    Xadd += 1;
                }//loop X

                //loop accumulaiton
                BayInd = BayInd ^ 0x2;
                Yadd += LineOffset;
            }//loop Y

            //loop accumulation
            Iadd += Block_X;
        }//loop i

        //loop accumulation
        Jadd += JaddIncrement;
    }//loop j
#if !DP_SEARCH_TILL_END
    DP_SEARCH_TOO_MUCH:
#endif
    returnValue = DP_Cnt - StartCnt;
    return returnValue;
}

UINT32 DFP_VGA_Search(UINT32 RGGBAddr, UINT16 width, UINT16 height, UINT16 LineOffset, UINT32 DPadd, UINT16 OffsetX, UINT16 OffsetY, UINT16 StartCnt, UINT16 MaxCnt,UINT8 Threshold, UINT16 Block_X, UINT16 Block_Y)
{
    #define PATTERN_SIZE 4 //RGGB
    UINT32 RGGBAvg[PATTERN_SIZE], RGGBValue[PATTERN_SIZE], *puwDPCurAddr, uwTempValue;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY, ExtraX, ExtraY;
    UINT16 DP_Cnt, i , j, k, x, y;
    INT16 hDiff;
    UINT8 DP_Double_Check_Flag, CheckFlag, *pubBlockAddr, *pubTempAddr;

    //compute blocking parameter
    EndBlockX = width / Block_X;
    EndBlockY = height / Block_Y;
    RemainX = width - (EndBlockX * Block_X);
    RemainY = height - (EndBlockY * Block_Y);

    //set defect pixel address and recheck
    DP_Cnt = StartCnt;
    puwDPCurAddr = (UINT32 *)DPadd + DP_Cnt;
    if (StartCnt)
    {
        DP_Double_Check_Flag = 1;
    }
    else
    {
        DP_Double_Check_Flag = 0;
    }


    for (y = 0; y < EndBlockY; y ++)
    {
        for (x = 0; x < EndBlockX; x ++)
        {
            // reset average value to zero
            RGGBAvg[0] = RGGBAvg[1] = RGGBAvg[2] = RGGBAvg[3] = 0;
            RGGBValue[0] = RGGBValue[1] = RGGBValue[2] = RGGBValue[3] = 0;

            // current block address;
            //#NT#2008/08/19#JJ Huang -begin
            //#NT#replace width by lineoffset
            pubBlockAddr = (UINT8 *)(RGGBAddr + (LineOffset * y * Block_Y) + (Block_X * x));
            //#NT#2008/08/19#JJ Huang -end

            ExtraY = 0;
            if (y == (EndBlockY - 1)) ExtraY = RemainY;

            ExtraX = 0;
            if (x == (EndBlockX - 1)) ExtraX = RemainX;

            // calculate average value;
            for (j = 0; j < (Block_Y + ExtraY); j ++)
            {
                pubTempAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * LineOffset)); // change line
                i = (Block_X + ExtraX);
                while (i >= PATTERN_SIZE)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;

                    RGGBAvg[1] += *pubTempAddr;
                    RGGBValue[1] ++;
                    pubTempAddr ++;

                    RGGBAvg[2] += *pubTempAddr;
                    RGGBValue[2] ++;
                    pubTempAddr ++;

                    RGGBAvg[3] += *pubTempAddr;
                    RGGBValue[3] ++;
                    pubTempAddr ++;

                    i -= PATTERN_SIZE;
                }

                if (i == 3)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;

                    RGGBAvg[1] += *pubTempAddr;
                    RGGBValue[1] ++;
                    pubTempAddr ++;

                    RGGBAvg[2] += *pubTempAddr;
                    RGGBValue[2] ++;
                    pubTempAddr ++;
                }
                else if (i == 2)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;

                    RGGBAvg[1] += *pubTempAddr;
                    RGGBValue[1] ++;
                    pubTempAddr ++;
                }
                else if (i == 1)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;
                }
            }
            RGGBAvg[0] /= RGGBValue[0];
            RGGBAvg[1] /= RGGBValue[1];
            RGGBAvg[2] /= RGGBValue[2];
            RGGBAvg[3] /= RGGBValue[3];

            //get data and calculate diff
            for (j = 0; j < (Block_Y + ExtraY); j ++)
            {
                pubTempAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * LineOffset)); // change line

                for (i = 0; i < (Block_X + ExtraX); i ++)
                {
                    hDiff = abs((*pubTempAddr - RGGBAvg[(i & 0x3)]));

                    if (hDiff > Threshold)
                    {
                        //record defect pixel 0xYYYYXXXX
                        uwTempValue = (((((Block_Y * y) + j + OffsetY) << 16) & 0xffff0000)
                                        | (((Block_X * x) + i + OffsetX) & 0x0000ffff));

                        CheckFlag = 0;
                        if (DP_Double_Check_Flag)
                        {
                            for (k = 0; k < StartCnt; k ++)
                            {
                                if (((UINT32 *)DPadd)[k] == uwTempValue)
                                {
                                    CheckFlag = 1;
                                    break;
                                }
                            }
                        }

                        if (CheckFlag == 0)
                        {
                            *puwDPCurAddr = uwTempValue;
                            //increase index
                            puwDPCurAddr ++;
                            DP_Cnt ++;
                        }

                        if (DP_Cnt >= MaxCnt)
                        {
                            sprintf(message, "-E- defect pixel count overflow");
                            debug_err((message));
                            return (DP_Cnt - StartCnt);
                        }
                    }
                    //next address
                    pubTempAddr ++;
                }
            }
        }
    }

    return (DP_Cnt - StartCnt);
}

//#NT#2008/08/19#JJ Huang -begin
//#NT#Add an API for panasonice CCD Preview DP Search
UINT32 DFP_9PxlMode_Search(UINT32 RawAddr, UINT16 width, UINT16 height, UINT16 LineOffset, UINT32 DPadd, UINT16 OffsetX, UINT16 OffsetY, UINT16 StartCnt, UINT16 MaxCnt,UINT8 Threshold, UINT16 Block_X, UINT16 Block_Y)
{
    #define PATTERN_X_SIZE  6
    #define PATTERN_Y_SIZE  2
    #define PATTERN_GROUP   12
    const UINT8 PatternTemplete[PATTERN_Y_SIZE][PATTERN_X_SIZE]=
    {
        { 0, 1, 2, 3, 4, 5},
        { 6, 7, 8, 9,10,11},
     };
    UINT32 PatternAvg[PATTERN_GROUP], PatternCnt[PATTERN_GROUP];

    UINT32 *puwDPCurAddr, uwTempValue;

    UINT8 *pubBlockAddr, *pubYAddr, *pubXAddr;

    UINT8 PatternIdx;

    INT16 hDiff;




    UINT16 RemainX,RemainY,EndBlockX,EndBlockY, ExtraX, ExtraY;
    UINT16 DP_Cnt, i , j, k, x, y;
    UINT8 DP_Double_Check_Flag, CheckFlag;

    {//Check and modify fatal arguments
        if(Block_X%PATTERN_X_SIZE)
        {
            Block_X=((Block_X/PATTERN_X_SIZE)+1)*PATTERN_X_SIZE;
        }
        if(Block_Y%PATTERN_Y_SIZE)
        {
            Block_Y=((Block_Y/PATTERN_Y_SIZE)+1)*PATTERN_Y_SIZE;
        }
    }
    {//compute blocking parameter
        EndBlockX = width / Block_X;
        EndBlockY = height / Block_Y;
        RemainX = width - (EndBlockX * Block_X);
        RemainY = height - (EndBlockY * Block_Y);
    }
    {//set defect pixel address and recheck
        DP_Cnt = StartCnt;
        puwDPCurAddr = (UINT32 *)DPadd + DP_Cnt;
        if (StartCnt)
        {
            DP_Double_Check_Flag = 1;
        }
        else
        {
            DP_Double_Check_Flag = 0;
        }
    }
    {//debug message
        debug_err(("width %d   height %d LineOffset %d\r\n", width, height,LineOffset));
        debug_err(("OffsetY %d   OffsetX %d\r\n", OffsetY, OffsetX));
    }


    for (y = 0; y < EndBlockY; y ++)
    {
        for (x = 0; x < EndBlockX; x ++)
        {
            {//Initial
                for (i=0; i<(PATTERN_X_SIZE*PATTERN_Y_SIZE); i ++)
                {
                    PatternAvg[i] = 0;
                    PatternCnt[i] = 0;
                }
                pubBlockAddr = (UINT8 *)(RawAddr + (LineOffset * y * Block_Y) + (Block_X * x));
                ExtraY = 0;
                if (y == (EndBlockY - 1))
                    ExtraY = RemainY;
                ExtraX = 0;
                if (x == (EndBlockX - 1))
                    ExtraX = RemainX;
            }

            for (j = 0; j < (Block_Y + ExtraY); j ++)
            {// calculate average value;
                {//Initial
                    pubYAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * LineOffset)); // change line
                    pubXAddr = pubYAddr;
                }
                for (i = 0; i < (Block_X + ExtraX); i ++)
                {
                    PatternIdx = (j%PATTERN_Y_SIZE)*PATTERN_X_SIZE + (i%PATTERN_X_SIZE);
                    PatternAvg[PatternIdx] += *pubXAddr;
                    PatternCnt[PatternIdx] ++;
                    pubXAddr++;
                }
            }

            {//Compute average
                for (i=0; i<(PATTERN_X_SIZE*PATTERN_Y_SIZE); i ++)
                {
                    PatternAvg[i] /= PatternCnt[i] ;
                }
            }

            for (j = 0; j < (Block_Y + ExtraY); j ++)
            {// compute diff. and threshold out DP
                {//Initial
                    pubYAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * LineOffset)); // change line
                    pubXAddr = pubYAddr;
                }
                for (i = 0; i < (Block_X + ExtraX); i ++)
                {
                    {//compute difference
                        PatternIdx = (j%PATTERN_Y_SIZE)*PATTERN_X_SIZE + (i%PATTERN_X_SIZE);
                        hDiff = abs((*pubXAddr - PatternAvg[PatternIdx]));
                    }

                    if (hDiff > Threshold)
                    {//record defect pixel 0xYYYYXXXX
                        uwTempValue = (((((Block_Y * y) + j + OffsetY) << 16) & 0xffff0000)
                                        | (((Block_X * x) + i + OffsetX) & 0x0000ffff));

                        CheckFlag = 0;
                        #if 0
                        debug_err(("DP%4d 0x%08x=(%4d,%3d) Val%3d Avg%3d Diff%3d\r\n",
                            DP_Cnt,
                            ((((j+y*Block_Y+OffsetY)<<16)&0xffff0000)+((i+x*Block_X+OffsetX)&0xffff)),
                            (((j+y*Block_Y+OffsetY))&0xffff), ((i+x*Block_X+OffsetX)&0xffff),
                            *pubXAddr, PatternAvg[PatternIdx], hDiff));
                        #endif
                        if (DP_Double_Check_Flag)
                        {
                            for (k = 0; k < StartCnt; k ++)
                            {
                                if (((UINT32 *)DPadd)[k] == uwTempValue)
                                {
                                    CheckFlag = 1;
                                    break;
                                }
                            }
                        }

                        if (CheckFlag == 0)
                        {
                            *puwDPCurAddr = uwTempValue;
                            //increase index
                            puwDPCurAddr ++;
                            DP_Cnt ++;
                        }

                        if (DP_Cnt >= MaxCnt)
                        {
                            sprintf(message, "-E- defect pixel count overflow");
                            debug_err((message));
                            return (DP_Cnt - StartCnt);
                        }
                    }
                    {//increase
                        pubXAddr++;
                    }
                }
            }


        }
    }

    return (DP_Cnt - StartCnt);
}
//#NT#2008/08/19#JJ Huang -end

void DFP_GetOnePicPrv(unsigned int addr,unsigned int Xsize,unsigned int Ysize)
{
    debug_err(("DFP_GetOnePicPrv (0x%08x, %d, %d)\r\n", addr, Xsize, Ysize));

    //JJ_433Cal: use new api// CalSie_Set2Preview(GetSieXcenter(), GetSieYcenter(), Xsize, Ysize, Xsize, 1, addr);

    sie_Load();
    sie_Reset(0);
    sie_SensorDataEnable(1);
    //#NT#2009/09/01#KS Hung -begin
    //SensorDelayVD(2);
    sie_SensorDataEnable(0);
    //SensorDelayVD(2);
    pre_FunctionEnable(DISABLE, PRE_HSCL_EN);
    if(DFPC_TEST)
    {
//        sie_Set_DefectPixel(DP_Addr_Prv,DP_Addr_Cap);
        //JJ_433CAL: need review// pre_SetInputStartAdd2(cap dp addr);
        pre_FunctionEnable(ENABLE, PRE_BLM_EN);
    }
    else
    {
        pre_FunctionEnable(DISABLE, PRE_BLM_EN);
//        sie_Set_DefectPixel(0,0);
        //JJ_433CAL: need review// pre_SetInputStartAdd2(0);
    }
    sie_SensorDataEnable(1);
    //SensorDelayVD(6);
    TimerDelayMs(200);
    sie_SensorDataEnable(0);
    //SensorDelayVD(2);
    //#NT#2009/09/01#KS Hung -end

#if 0//CAL_SAVE_RAW
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)addr,//   UB *pBuf,
                    Xsize*Ysize,//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif
}

void DFP_GetOnePicCap(unsigned int addr,unsigned int Xsize,unsigned int Ysize, unsigned char environment)
{
    //Photo_LockAE(1);
    SetCalFlag(1);

    if(DFPC_TEST)
    {
//        sie_Set_DefectPixel(DP_Addr_Prv,DP_Addr_Cap);
        //JJ_433CAL: need review// pre_SetInputStartAdd2(cap dp addr);
        pre_FunctionEnable(ENABLE, PRE_BLM_EN);
    }
    else
    {
        pre_FunctionEnable(DISABLE, PRE_BLM_EN);
//        sie_Set_DefectPixel(0,0);
        //JJ_433CAL: need review// pre_SetInputStartAdd2(0);
    }
    #if 0
    sie_Set2Capture(    addr,
                        GetSieCapXCenter(),//SIE_CAP_XCENTER,
                        GetSieCapYCenter(),//SIE_CAP_YCENTER,
                        Xsize,
                        Ysize);
    #else
    CalSie_Set2Capture( addr,
                        GetSieCapXCenter(),//SIE_CAP_XCENTER,
                        GetSieCapYCenter(),//SIE_CAP_YCENTER,
                        Xsize,
                        Ysize,
                        2,//UINT8 SiePath, 0:PreColorGain, 1:PreGamma, 2:PostGamma
                        0);//1); //BOOL autoOB, 0:manual, 1:auto
    #endif

    if(DFPC_TEST)
    {
//        DP_Correction(addr,Xsize/2,Ysize/2,Xsize,Ysize, DP_Addr_Cap);
    }

    SetCalFlag(0);
    //Photo_LockAE(0);

#if 0//CAL_SAVE_RAW
    FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,//UW Cmd,
                    (UB*)addr,//   UB *pBuf,
                    Xsize*Ysize,//JJ_UI_FileFmt
                    0,//UW Pos,
                    1,//BOOL CloseFile,
                    FST_FMT_RAW,//UW FileFormat,
                    FST_TIME_INFINITE);//FST_TIME_NO_WAIT);
#endif
}


#if 0//mark useless



#endif//0

#endif//_CALIBRATION_MODE_

