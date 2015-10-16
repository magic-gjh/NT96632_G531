#include     "sensor_model.h"
#include     "SIF.h"
#include    "FileSysTsk.h"
#include    "utility.h"

#define        SEN_COMMAND    1
extern INT32 Photo_SetImageRatio(_IMGRatio_sel RatioIdx);
#if        SEN_COMMAND
extern void Get_Sensor_CalInfor(void);//JJ added, 20050722


UINT32 uiAE_TEST = FALSE;
UINT32 uiAeIsoGain = 100;
UINT32 uiAeExpoTime = 33333;
UINT32 ShowFPS = FALSE;

#define    CAPMODE_SINGLE            1//copied from SieCapTsk.h
#define    CAPMODE_NORMALBURST        2//copied from SieCapTsk.h
#define    CAPMODE_SMALLBURST        3//copied from SieCapTsk.h
//#NT#20090902#ethanlau -begin
#if OV5653_630
void RegisterWrite(char *pcStr);
void RegisterRead(char *pcStr);
extern const UINT32 Sensor_Param[92][2];
extern const UINT32 Sensor_Mode[37][7];
extern BOOL AE_UartMsg;
extern BOOL AE_OsdMsg;
extern BOOL RAWBinning_ENABLE;
BOOL PASS;

extern BOOL AWB_DebugMsg;
extern BOOL AEDUMMYLINE;
extern UINT32 A1,A2,A3;
extern UINT16 AE_Flag;
#endif
//#NT#20090902#ethanlau end
#if 0
void    write_reg(char *pcStr);
void    read_reg(char *pcStr);
#endif
void    sen_cmd(char *pcCmdStr);
void    sen_cmd(char *pcCmdStr)
{
    int i;

    switch (*pcCmdStr)
    {
#if 0
        case 'a':
            if (!strncmp(pcCmdStr,"aver",4))
                ;//print_Y();
            else if (!strncmp(pcCmdStr,"al",2))
                ;//AE_Lock(1);
            else if (!strncmp(pcCmdStr,"aul",3))
                ;//AE_Lock(0);
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
        break;
#endif
        case 'a':
            if(!strncmp(pcCmdStr,"as ",3))
            {
                UINT32 Addr,Size;
                sscanf((pcCmdStr+3), "%x %x", &Addr, &Size);
                FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                                (UB*)Addr,
                                Size,
                                0,
                                1,
                                FST_FMT_RAW,
                                FST_TIME_INFINITE);
            }
#if OV5653_630
            else if(!strncmp(pcCmdStr,"apclk ",6))
            {
                UINT32 a1,a2,a3;
                sscanf(pcCmdStr+6,"%d %d %d",&a1,&a2,&a3);
                A1 = a1;
                A2 = a2;
                A3 = a3;
                debug_msg("%d %d %d\r\n",A1,A2,A3);

            }
            else if(!strncmp(pcCmdStr,"aedummyline ",12))
            {
                UINT32 _a;
                sscanf(pcCmdStr+12,"%d",&_a);

                AEDUMMYLINE = _a;
                debug_msg("AEDUMMYLINE=%d\r\n",AEDUMMYLINE);
            }
            else if(!strncmp(pcCmdStr,"ap ",3))
            {
                UINT32 _a;
                sscanf(pcCmdStr+3,"%d",&_a);
                PASS = _a;
                debug_msg("PASS = %d\r\n",_a);
            }
            else if(!strncmp(pcCmdStr,"aw ",3))
            {
                RegisterWrite(pcCmdStr+3);
            }
            else if(!strncmp(pcCmdStr,"ar ",3))
            {
                RegisterRead(pcCmdStr+3);
            }
            else if(!strncmp(pcCmdStr,"aemsg ",6))
            {
                UINT32 _a,_b;
                sscanf(pcCmdStr+6,"%d %d",&_a,&_b);
                AE_UartMsg = _a;
                AE_OsdMsg = _b;
            }
            else if(!strncmp(pcCmdStr,"antishaking ",12))
            {
                UINT32 _a;
                sscanf(pcCmdStr+12,"%d",&_a);
                UI_SetParameter(_StillAntiShaking,_a);
                debug_msg("set antishaking level = %d\r\n",_a);
            }
            else if(!strncmp(pcCmdStr,"anst ",5))
            {
                UINT32 _a;
                sscanf(pcCmdStr+5,"%d",&_a);
                SetAntishaking_RawbiningEnalbe(_a);
                debug_msg("set RAWBinning_ENABLE=%d\r\n",GetAntishaking_RawbiningEnalbe());
            }
#endif
            else if(!strncmp(pcCmdStr,"aelock",6))
            {
                aaa_AeLock(1);
            }
            else if(!strncmp(pcCmdStr,"aeunlock",6))
            {
                aaa_AeLock(0);
            }
            else if(!strncmp(pcCmdStr,"ar ",3))
            {
                UINT8 arg_num;
                UINT32 uiAddr;
                arg_num = sscanf((pcCmdStr+3), "%x", &uiAddr);
                debug_err(("Read Reg[0x%lX, 0x%X]\r\n", uiAddr, Sensor_ReadReg(uiAddr)));
            }
            else if(!strncmp(pcCmdStr,"aw ",3))
            {
                UINT32 uiAddr, uiData;
                sscanf((pcCmdStr+3), "%x %x", &uiAddr, &uiData);
                Sensor_WriteReg(uiAddr, uiData);
            }
            else if(!strncmp(pcCmdStr,"awblock",7))
            {
                aaa_AwbLock(LOCK);
                debug_msg("Lock awb\r\n");
            }
            else if(!strncmp(pcCmdStr,"awbunlock",9))
            {
                aaa_AwbLock(UNLOCK);
                debug_msg("Unlock awb\r\n");
            }
            else if(!strncmp(pcCmdStr,"atime ",6))
            {

                UINT32 uiValue;
                uiAE_TEST = TRUE;
                sscanf((pcCmdStr+6), "%d", &uiValue);
                uiAeExpoTime = uiValue;
            }
            else if(!strncmp(pcCmdStr,"again ",6))
            {
                UINT32 uiValue;
                uiAE_TEST = TRUE;
                sscanf((pcCmdStr+6), "%d", &uiValue);
                uiAeIsoGain = uiValue;
            }
#if 0
            else if(!strncmp(pcCmdStr,"awbmsg ",7))
            {
                UINT32 _a;
                sscanf(pcCmdStr+7,"%d",&_a);
                AWB_DebugMsg = _a;
                debug_msg("AWB_DebugMsg = %d\r\n",AWB_DebugMsg);
            }
#endif
#if 0
        case 'c':
            if (!strncmp(pcCmdStr,"cal dp old",11))
            {
/*
                if(t1==0)
                    Get_Sensor_CalInfor();
                DP_Search();
*/
            }
            else if(!strncmp(pcCmdStr,"colorgain ",10))
            {
                UINT32 r=0,g=0,b=0;
                sscanf(pcCmdStr+10,"%d %d %d",r,g,b);
                debug_msg("Setcolorgain:R=%d,G=%d,B=%d\r\n",r,g,b);
                aaa_SetIPEColorgain(r, g, g, b);
            }
            else if (!strncmp(pcCmdStr,"cal infor",11))
            {
/*
                Get_Sensor_CalInfor();
*/            }
            else if (!strncmp(pcCmdStr,"cal dp th",9))
            {
/*
                if(t1==0)
                    Get_Sensor_CalInfor();
                sscanf(pcCmdStr+9,"%d %d %d %d",&t1,&t2,&t3,&t4);
                    CalInfor.BrightLimitPrv=t1;
                    CalInfor.BrightLimitCap=t2;
                    CalInfor.DarkLimitPrv=    t3;
                    CalInfor.DarkLimitCap=    t4;
*/
            }
            else if (!strncmp(pcCmdStr,"chroma",6))
            {
                debug_err(("chroma filter test...\r\n"));
                //Chroma_filter_test();
            }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
        break;
        case 'd':
            if (!strncmp(pcCmdStr,"drawca",6))
            {
                //DrawCAWindow();
            }
            else if (!strncmp(pcCmdStr,"drawva",6))
            {
                //DrawVAWindow();
            }
            else
            {
                SEN_WARMSG("bad command or invalid parameters\r\n");
            }
            break;
#endif
        case 'c':
            if(!strncmp(pcCmdStr,"chroma ",7))
            {
                //UINT32 _a1,_a2,_a3;
                //sscanf(pcCmdStr+7,"%d %d %d",&_a1,&_a2,&_a3);
                //debug_msg("chroma = %d %d %d\r\n",_a1,_a2,_a3);
                //IMECHROMAPARAM[ISONUM0].crv_delta = _a1;
                //IMECHROMAPARAM[ISONUM0].havg_size = _a2;
                //IMECHROMAPARAM[ISONUM0].havg_coeff = _a3;
            }
        case 'f':
            if (!strncmp(pcCmdStr,"fd ",3))
            {
                UINT8 _a;
                sscanf(pcCmdStr+3,"%d",&_a);
                if(_a==1)
                {
                    aaa_FdLock(UNLOCK);
                    debug_msg("fd unlock...\r\n");
                }
                else if(_a==0)
                {
                    aaa_FdLock(LOCK);
                    debug_msg("fd lock...\r\n");
                }
                else
                    debug_err(("only 1 or 0...\r\n"));
                //fd_test();
            }
#if 0
            else if(!strncmp(pcCmdStr,"flash verify",12))
            {
                Verify_FlashCurve();
                debug_msg("verify flash curve\r\n");
            }
#endif
            break;

        case 'g':
            if (!strncmp(pcCmdStr,"getpreraw",9))
            {
                debug_err(("Get Pre RAW(%dx%d)...\r\n",GetIPPSizeInfor(_IpeOutVsize),(GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF)));
                FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)ippbuf_GetBuf(IPPBUF_PRV_RAW_1),
                       GetIPPSizeInfor(_IpeOutVsize) * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF),
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);

                FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)ippbuf_GetBuf(IPPBUF_PRV_RAW_2),
                       GetIPPSizeInfor(_IpeOutVsize) * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF),
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
                debug_msg("PRV RAW ADDR %8x %8x \r\n",ippbuf_GetBuf(IPPBUF_PRV_RAW_1),ippbuf_GetBuf(IPPBUF_PRV_RAW_2));
            }
            else if (!strncmp(pcCmdStr,"getrawy",7))
            {
                unsigned short r,g,b;
                debug_msg("Cap Raw Y  = %d\r\n",CalculateRawY(ippbuf_GetBuf(IPPBUF_PRV_RAW_1),0,0,GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF,GetIPPSizeInfor(_IpeOutVsize),8,0,&r,&g,&b));
            }
#if OV5653_630
            else if(!strncmp(pcCmdStr,"getsenreg",9))
            {
                UINT32 a[200],i,count=0;
                UINT32 table_size;
                table_size = (sizeof(Sensor_Param)/(4*2));
                for(i=0;i<table_size;i++)
                {
                    //a[count] = Sensor_ReadReg(Sensor_Param[i][0]);
                    //count++;
                    debug_msg("0x%x = 0x%x\r\n",Sensor_Param[i][0],Sensor_ReadReg(Sensor_Param[i][0]));
                }
                table_size = (sizeof(Sensor_Mode)/(4*SEN_MODE_MAX));
                for(i=0;i<table_size;i++)
                {
                    //a[count] = Sensor_ReadReg(Sensor_Mode[i][0]);
                    //count++;
                    debug_msg("0x%x = 0x%x\r\n",Sensor_Mode[i][0],Sensor_ReadReg(Sensor_Mode[i][0]));
                }
            }
#endif
            else if (!strncmp(pcCmdStr,"getimeraw",9))
            {
                debug_err(("Get Ime RAW(%dx%d)...\r\n",GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize) ));
                FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)ippbuf_GetBuf(IPPBUF_PRV_IME_P1),
                       GetIPPSizeInfor(_ImeOut1Hsize) * (GetIPPSizeInfor(_ImeOut1Vsize)*6 ),
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
            }
        break;
        case 'p':
            if(!strncmp(pcCmdStr,"prebin ",7))
            {
                UINT32 _a;
                sscanf(pcCmdStr+7,"%d",&_a);
                if(_a==1)
                {
                    pre_setHBining(1,0);
                    pre_HScale(IPPMgrReq.SIE_PRE_PRV->PreCropSizeH,IPPMgrReq.SIE_PRE_PRV->PreOutSizeH);
                    pre_load();
                    AE_Param.ExpoLine = AE_Param.ExpoLine>>1;
                    SetPreExpLine(AE_Param.ExpoLine);
                }
                else
                {
                    pre_setHBining(0,0);
                    pre_HScale(IPPMgrReq.SIE_PRE_PRV->PreCropSizeH,IPPMgrReq.SIE_PRE_PRV->PreOutSizeH);
                    pre_load();
                    AE_Param.ExpoLine = AE_Param.ExpoLine<<1;
                    SetPreExpLine(AE_Param.ExpoLine);
                }
            }
        case 'i':
            if (!strncmp(pcCmdStr,"iso ",4))
            {
                UINT32 _a;
                //AE_Flag = Flag_SetPgaGain|Flag_SetExpoTime;
                sscanf(pcCmdStr+4,"%d",&_a);
                AE_Param.ExpoLine = AE_Param.ExpoLine*AE_Param.ISO_Gain/_a;
                AE_Param.ISO_Gain = _a;
                AeISOGain2TGValue(AE_Param.ISO_Gain);
                AeSet_isr();
                debug_msg("iso = %d, expoline = %d\r\n",AE_Param.ISO_Gain,AE_Param.ExpoLine);
            }

#if 0
            if (!strncmp(pcCmdStr,"i0",2))
                {
                    SEN_INDMSG("IRIS 0 ......\r\n");
                    Lens_Aperture_Move(0);
                }
            else if (!strncmp(pcCmdStr,"i1",2))
                {
                    SEN_INDMSG("IRIS 1 ......\r\n");
                    Lens_Aperture_Move(1);
                }
            else if (!strncmp(pcCmdStr,"i2",2))
                {
                    SEN_INDMSG("IRIS 2 ......\r\n");
                    Lens_Aperture_Move(2);
                }

            else if(!strncmp(pcCmdStr,"iso50",5))
            {
                AE_FIXISO(_IMGISO_50);
                debug_msg("fix iso50\r\n");
            }
            else if(!strncmp(pcCmdStr,"iso100",6))
            {
                AE_FIXISO(_IMGISO_100);
                debug_msg("fix iso100\r\n");
            }
            else if(!strncmp(pcCmdStr,"iso200",6))
            {
                AE_FIXISO(_IMGISO_200);
                debug_msg("fix iso200\r\n");
            }
            else if(!strncmp(pcCmdStr,"iso400",6))
            {
                AE_FIXISO(_IMGISO_400);
                debug_msg("fix iso400\r\n");
            }
            else if(!strncmp(pcCmdStr,"isoauto",7))
            {
                AE_FIXISO(_IMGISO_Auto);
                debug_msg("fix isoauto\r\n");
            }
            else if (!strncmp(pcCmdStr,"iso50adj ",9))
            {
                UINT32 ave_y;
                sscanf(pcCmdStr+9,"%d",&gAGC_ISO50);
                debug_msg("ISO50Adj %d\r\n",gAGC_ISO50);
                aaa_AeLock(LOCK);
                debug_msg("Aelock\r\n");
                AeISOGain2TGValue(100);
                AeExpoTime2TGValue(33333,GetIPPSizeInfor(_VDTotal_Preview), GetIPPSizeInfor(_HDTotal_Preview));
                SetPreExpLine(AE_Param.ExpoLine);
                debug_msg("AE_Param.ExpoLine=%d\r\n",AE_Param.ExpoLine);
                sie_waitVD(8);
                while(1)
                {
                    ave_y=aaa_EstimateY2(FB_para1.y_addr,GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
                    debug_msg("ave_y=%d\r\n",ave_y);
                    if(ave_y<40)
                        break;
                    sie_waitVD(5);
                }
            }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
#endif
        break;
        case 'l':
#if 0
            if (!strncmp(pcCmdStr,"li",2))
            {
                lens_pins_init();
                LensInit();
                SEN_INDMSG("Lens Init...\r\n");
                //Lens_Zoom_InitStart();
                //Lens_Zoom_InitEnd();
            }
            else if (!strncmp(pcCmdStr,"lr",2))
            {
                LensRetract();
                SEN_INDMSG("Lens retract...\r\n");
            }
            else if (!strncmp(pcCmdStr,"lao",3))
                Lens_Aperture_Move(0);
            else if (!strncmp(pcCmdStr,"lac",3))
                Lens_Aperture_Move(1);
            else if (!strncmp(pcCmdStr,"lafw",4))
            {
                gImageAlgInfo.AFPosition++;
                if(gImageAlgInfo.AFPosition>4)
                    gImageAlgInfo.AFPosition=0;
                //DrawVAWindow();
                SEN_INDMSG("AF Window Change...\r\n");
            }
            else if (!strncmp(pcCmdStr,"laf",3))
            {
                AF_Process();
                SEN_INDMSG("AF...\r\n");
            }
            else if (!strncmp(pcCmdStr,"lsc",3))
                Lens_Shutter_Move(0);
            else if (!strncmp(pcCmdStr,"lso",3))
                Lens_Shutter_Move(1);
            else if (!strncmp(pcCmdStr,"lzi",3))
                ZOOM_RunStep(0,60);
            else if (!strncmp(pcCmdStr,"lzo",3))
                ZOOM_RunStep(1,60);
            else if (!strncmp(pcCmdStr,"lfi",3))
            {
                FocusPowerOnOff(1);
                Focus_RunStep(0,20);
                FocusPowerOnOff(0);
            }
            else if (!strncmp(pcCmdStr,"lfo",3))
            {
                FocusPowerOnOff(1);
                Focus_RunStep(1,20);
                FocusPowerOnOff(0);
            }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
#endif
        break;
        case 't':
            if(!strncmp(pcCmdStr,"test ",5))
            {
                UINT32 _a;
                IPL_OBJ obj;
                sscanf(pcCmdStr+5,"%d",&_a);
                obj.uiCmd = IPL_CMD_CHG_MODE;
                obj.Mode = IPL_MODE_OFF;
                IPL_SetCmd(&obj);
                UI_SetParameter(_ImageFlip, _a);
                obj.uiCmd = IPL_CMD_CHG_MODE;
                obj.Mode = IPL_MODE_PREVIEW;
                IPL_SetCmd(&obj);
            }
        break;

        case 's':
            if (!strncmp(pcCmdStr,"sreg",4))
                ;//write_reg((pcCmdStr+4));
            else if (!strncmp(pcCmdStr,"sfps on",7))
            {
                ShowFPS = TRUE;
            }
            else if (!strncmp(pcCmdStr,"sfps off",8))
            {
                ShowFPS = FALSE;
            }
            else if (!strncmp(pcCmdStr,"szoom",5))
                ;//set_dzoom((pcCmdStr+5));
            else if (!strncmp(pcCmdStr,"sev",3))
                ;//set_ev((pcCmdStr+3));
            else if (!strncmp(pcCmdStr,"sexp",4))
                ;//set_exp((pcCmdStr+3));
            else if (!strncmp(pcCmdStr,"sagc",4))
                ;//set_agc((pcCmdStr+3));
            else if (!strncmp(pcCmdStr,"s2",2))
            {
             }
            else if (!strncmp(pcCmdStr,"s3",2))
            {
            }
            else if (!strncmp(pcCmdStr,"sdlock",6))
            {

            }
            else if (!strncmp(pcCmdStr,"sdunlock",8))
            {
            }
            else if(!strncmp(pcCmdStr,"setdzt ",7))
            {
                UINT _a;
                sscanf(pcCmdStr+7,"%d",&_a);
                Photo_SetDzoomTimeInterval(_a);
                debug_msg("dzoom time interval = %d VD\r\n",_a);
            }
            else if(!strncmp(pcCmdStr,"setseninitreg",13))
            {
                Sensor_WriteReg(0x3012, 0x80);
                TimerDelayMs(100);
                Sensor_Init();

            }
            else if(!strncmp(pcCmdStr,"setsenmodereg ",14))
            {
                UINT32 _a;
                sscanf(pcCmdStr+14,"%d",&_a);
                Sensor_SetMode(_a);
            }
            //#NT#2010/10/28#Connie Yu -begin
            else if(!strncmp(pcCmdStr,"save eth",8))
            {
                FilesysReadWrite(FST_CMD_WRITE_NEXT_FILE,
                        (UB*)ippbuf_GetBuf(IPPBUF_PRV_DIS_BUF),
                        GetIPPSizeInfor(_IpeOutHsize) * GetIPPSizeInfor(_IpeOutVsize)*3/8,
                        0,
                        1,
                        FST_FMT_RAW,
                        FST_TIME_INFINITE);
                debug_err(("save eth %d x %d\r\n",GetIPPSizeInfor(_IpeOutHsize),GetIPPSizeInfor(_IpeOutVsize)));
            }
            //#NT#2010/10/28#Connie Yu -end
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
        break;

        case 'u':
            if (!strncmp(pcCmdStr,"uiset ",6))
            {
                UINT32 a,b;
                sscanf(pcCmdStr+6,"%d %d",&a,&b);
                UI_SetParameter(a,b);
                debug_msg("Set UI : item=%d,value=%d\r\n",a,b);
            }
            else if (!strncmp(pcCmdStr,"uiget ",6))
            {
                UINT32 a,b;
                PImageAlgInfo Addr;
                sscanf(pcCmdStr+6,"%d %d",&a,&b);
                Addr = UI_GetParameter();

                //debug_msg("Get UI : item=%d,value=%d\r\n",a,b);
            }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
            break;
        case 'z':
            if (!strncmp(pcCmdStr,"zit",3))
                {
                   SEN_INDMSG("DZoom In test ......\r\n");
                    //aaa_AwbLock(LOCK);
                    for(i=10;i<=30;i++)
                    {
                        Photo_DZoom_In();
                        TimerDelayMs(100);
                    }
                }
            else if (!strncmp(pcCmdStr,"zot",3))
                {
                   SEN_INDMSG("DZoom Out test ......\r\n");
                   //aaa_AwbLock(LOCK);
                    for(i=30;i>=10;i--)
                    {
                        Photo_DZoom_Out();
                        TimerDelayMs(100);
//                        set_flg(FLG_ID_PHOTO, FLGPHOTO_ZOOMIN);
                    }
                    //aaa_AwbLock(UNLOCK);
                }
            else if (!strncmp(pcCmdStr,"zis",3))
            {
                    INT index;
                    index = Get_DzoomIndex() + 1;
                    Set_DzoomIndex(index);
                    if(Get_DzoomIndex()<10)
                       Set_DzoomIndex(10);
                    if(Get_DzoomIndex()>30)
                       Set_DzoomIndex(30);



                    //Set_digitalzoom(zoom_index);
                    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                       SetDzoom2Video(Get_DzoomIndex());
                    else
                        SetDzoom2Preview(Get_DzoomIndex());
                    //aaa_AwbLock(UNLOCK);
            }
            else if (!strncmp(pcCmdStr,"zos",3))
            {
                    INT index;
                    index = Get_DzoomIndex() - 1;
                    Set_DzoomIndex(index);
                    if(Get_DzoomIndex()<10)
                       Set_DzoomIndex(10);
                    if(Get_DzoomIndex()>30)
                       Set_DzoomIndex(30);

                    //Set_digitalzoom(zoom_index);
                    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                       SetDzoom2Video(Get_DzoomIndex());
                    else
                        SetDzoom2Preview(Get_DzoomIndex());

                    //aaa_AwbLock(UNLOCK);
            }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
        break;

        case 'v':

        break;
        /*
        case 't':
            if (!strncmp(pcCmdStr,"t0",2))
                {
                    SEN_INDMSG("OB Test ......\r\n");
                    TestOB();
                }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
        */
        break;


//#endif

#if 0//Sharp_8M

            case 'n':    // switch mode
                    if(!strncmp(pcCmdStr,"noise 50", 8))
                        TestNoise(0);
                    else if(!strncmp(pcCmdStr,"noise 100", 9))
                        TestNoise(1);
                    else if(!strncmp(pcCmdStr,"noise 200", 9))
                        TestNoise(2);
                    else if(!strncmp(pcCmdStr,"noise 400", 9))
                        TestNoise(3);
                    break;
            case 'l':
                if (!strncmp(pcCmdStr,"li",2))
                {
                    LensInit();
                    SEN_INDMSG("Lens Init...\r\n");
                }
                if (!strncmp(pcCmdStr,"lr",2))
                {
                    //LensRetract();
                    SEN_INDMSG("Lens retract...\r\n");
                }
                else if (!strncmp(pcCmdStr,"laf",3))
                {
                    AF_Process();
                    SEN_INDMSG("AF...\r\n");
                }
                else if(!strncmp(pcCmdStr,"lm ",3))
                {
                    sscanf(pcCmdStr+4,"%d %d \n",&item,& data);
                    UI_SetParameter(item,data);
                }
                else if(!strncmp(pcCmdStr,"la ",3))
                {
                    sscanf(pcCmdStr+4,"%d\n",& data);
                    Lens_Aperture_Move(data);
                }
                else if(!strncmp(pcCmdStr,"lav ",4))
                {
                    sscanf(pcCmdStr+5,"%d\n",& data);
                    SetAVidx(data);
                }
                else if(!strncmp(pcCmdStr,"ltv ",4))
                {
                    sscanf(pcCmdStr+5,"%d\n",& data);
                    SetTVidx(data);
                }
                else if(!strncmp(pcCmdStr,"ls ",3))
                {
                    sscanf(pcCmdStr+4,"%d\n",& data);
                    pinmux_select_mes(0);
                    *(unsigned long volatile *)(0x98100004)&=~(0x3<<10);//MES0,1 set to GPIO controlled
                    sie_waitVD(3);
                    lens_set_shutter(data);
                }
                break;

        case 'r':
            if (!strncmp(pcCmdStr,"rreg",4))
                read_reg((pcCmdStr+4));
            else if (!strncmp(pcCmdStr,"rpi",3))
            {
                #if 0
                debug_err(("zpi=%d\r\n",Check_Zoom_PI()));
                debug_err(("zpr=%d\r\n",Check_Zoom_PR()));
                debug_err(("fpi=%d\r\n",Check_Focus_PI()));
                #endif
            }
            else
                SEN_WARMSG("bad command or invalid parameters\r\n");
        break;
#endif
    }
}
//#NT#20090902#ethanlau -begin
#if OV5653_630
extern void Sensor_WriteReg(UINT32 addr, UINT32 value);
extern UINT32 Sensor_ReadReg(UINT32 addr);

void RegisterWrite(char *pcStr)
{
    UINT32 uiAddr, uiData;
    debug_msg("RegisterWrite\r\n");
    if(!strncmp(pcStr,"sensor ",7))
    {
        sscanf((pcStr+7), "%x %x", &uiAddr, &uiData);
        Sensor_WriteReg(uiAddr, uiData);
    }
    else
        debug_err(("missing syntax:sensor \r\n "));
}

void RegisterRead(char *pcStr)
{
    UINT32 uiAddr, uiData,arg_num;
    debug_msg("RegisterRead\r\n");
    if(!strncmp(pcStr,"sensor ",7))
    {
        arg_num = sscanf((pcStr+7), "%x", &uiAddr);
        debug_msg("Read Reg[%x, %x]\r\n", uiAddr, Sensor_ReadReg(uiAddr));
    }
    else
        debug_err(("missing syntax:sensor \r\n "));
}
#endif
//#NT#20090902#ethanlau -end
#if 0
void    write_reg(char *pcStr)
{
#if 0
     unsigned int     uiSaddr,uiWdata;
    unsigned char    arg_num;
//    char        message[128];
    arg_num=sscanf(pcStr," %x %x \n",&uiSaddr,&uiWdata);
#if    Panasonic_5M
    Sensor_TG_CFG(uiSaddr, uiWdata);
//    PanaTG_CFG(uiSaddr, uiWdata);
#endif
//    ov_iic_write(0x60,uiSaddr,uiWdata);//for onivison
    sprintf(message,"Write  reg_address data:  0x%2x  0x%2x    \r\n",uiSaddr,uiWdata);
    SEN_INDMSG(message);
#endif
}

void read_reg(char *pcStr)
{
/*
    unsigned int    uiSaddr,uiRdata;
    unsigned char    arg_num;

    arg_num=sscanf(pcStr," %x \n",&uiSaddr);
    uiRdata=iic_read(0xba, uiSaddr)>>16;//for micron
    debug_err(("Read reg_address data:  0x%2x  0x%2x   \r\n",uiSaddr,uiRdata));
*/
}
#endif

#endif
