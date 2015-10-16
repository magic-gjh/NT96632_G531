/**
    DIS Process API

    This file contains the control interface of DIS process.

    @file       dis_common.c
    @ingroup    mISYSAlg
    @note       N/A

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


/** \addtogroup mISYSAlg */
//@{

#include "sensor_model.h"
#include "dis_common.h"

//#NT#2010/09/24#Connie Yu -begin
//#NT#movie DIS update
#define VER2 ENABLE //DISABLE
//#NT#2010/09/24#Connie Yu -end

MOTION_VECTOR DIS_MDSAddr[8]={  //For Video DIS
    {16,16},
    {320,16},
    {16,248},
    {320,248},
    {16,16},
    {16,16},
    {16,16},
    {16,16}
};

//#NT#2010/09/03#Connie Yu -begin
//#NT#update for 460 DIS
UINT16          dis_newViewRatio=110; //New size = original*dis_newViewRatio/100
//DIS_MODE        gDis_IppMode=DIS_IPPMODE_Preview;
MOTION_VECTOR   imv;
MOTION_VECTOR   gIdeCorr, gRecCorr;
//ACTFBADDR_PRAM  DisRec;
//BOOL gFlg_need2UpdateDisplaySz=TRUE;
volatile DIS_PROC_EVENT gDisProcEvent=DIS_PROC_NOOP;
//MOTION_INFOR dis_mv[MAXMDSNUM*16];
IPE_IQINFO IQInfo;
volatile BOOL gDisFreezeFB=FALSE;
VECTOR gIdeCenter, gIdeVecBefLock;
volatile BOOL bLockMatch=FALSE;
//MOTION_INFOR disRec_mv[MAXMDSNUM*16];
MOTION_VECTOR   rec_imv;
//UINT32 curETH = 0x20;
//DIS_ADATH_OP gAdapThresOp = DIS_ADATH_NOOP;
UINT32 gCurDisConfig_InH, gCurDisConfig_InV;
//#NT#2010/09/03#Connie Yu -end

//#NT#2010/10/28#Connie Yu -begin
//#NT#movie DIS update
/**
    DIS: Reset correction vectors
*/
void dis_resetCorrVec(void)
{
    imv.x=0; imv.y=0;
    rec_imv.x=0; rec_imv.y=0;
}

/**
    Lock DIS hardware engine
    @param lock UNLOCK: unlock DIS HW, LOCK:lock DIS HW
*/
ER dis_configDisHw(void)
{
    DIS_PARAM modeInfo;
    UINT32 InSz, OutSz, ofso;

    modeInfo.DIS_operation = DIS_ENGINE_SET2PRV;
    ofso = (((GetIPPSizeInfor(_IpeOutHsize)>>3) + 3) >> 2) << 2;
    InSz = ofso * GetIPPSizeInfor(_IpeOutVsize);
    InSz = ((InSz+3)>>2) << 2;
    OutSz = 16 * 16 * 2 * 4; // = mds * blk/mds * MV word/blk * byte/word
    modeInfo.InAdd0 = ippbuf_GetBuf(IPPBUF_PRV_DIS_BUF);
    modeInfo.InAdd1 = modeInfo.InAdd0 + InSz;
    modeInfo.InAdd2 = modeInfo.InAdd1 + InSz;
    modeInfo.OutAdd0 = modeInfo.InAdd2 + InSz;
    modeInfo.OutAdd1 = modeInfo.OutAdd0 + OutSz;
    modeInfo.InSizeH = GetIPPSizeInfor(_IpeOutHsize);
    modeInfo.InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    modeInfo.InLineOfs = (((modeInfo.InSizeH>>3) + 3) >> 2) << 2;
    modeInfo.intEn = 0; //enable when process is unlocked

    //#NT#2010/09/03#Connie Yu -begin
    //#NT#update for 460 DIS
    gCurDisConfig_InH = GetIPPSizeInfor(_IpeOutHsize);
    gCurDisConfig_InV = GetIPPSizeInfor(_IpeOutVsize);
    //#NT#2010/09/03#Connie Yu -end
    //debug_err(("add 0x%x, insz %d, outsz %d\r\n",modeInfo.InAdd0,InSz,OutSz));

    return dis_setMode(&modeInfo);
}

//#NT#2010/09/24#Connie Yu -begin
//#NT#movie DIS update
ER dis_chgDisSizeConfig(void)
{
    DIS_PARAM modeInfo;

    gCurDisConfig_InH = GetIPPSizeInfor(_IpeOutHsize);
    gCurDisConfig_InV = GetIPPSizeInfor(_IpeOutVsize);

    modeInfo.DIS_operation = DIS_ENGINE_SET2CHGSIZE;
    modeInfo.InSizeH = GetIPPSizeInfor(_IpeOutHsize);
    modeInfo.InSizeV = GetIPPSizeInfor(_IpeOutVsize);
    return dis_setMode(&modeInfo);
}
//#NT#2010/09/24#Connie Yu -end

void dis_enableIPEEdgeOut(UINT8 enable)
{
    //IPE_IQINFO IQInfo;
    UINT32 add0, InSz, ofso;
    IPE_ETH_PARAM ethParam;
    STR_IPE_EDGE_KER_PARA  edgeKerH;
    STR_IPE_EDGE_KER_PARA  edgeKerV;
    //ER errCode;

    if(enable)
    {
        //Set I/O
        ofso = (((GetIPPSizeInfor(_IpeOutHsize)>>3) + 3) >> 2) << 2;
        InSz = ofso * GetIPPSizeInfor(_IpeOutVsize);
        InSz = ((InSz+3)>>2) << 2;
        add0 = ippbuf_GetBuf(IPPBUF_PRV_DIS_BUF);
        IPEModeInfo.OUTINFO.AdrEth0 = add0;
        IPEModeInfo.OUTINFO.AdrEth1 = add0+InSz;
        IPEModeInfo.OUTINFO.AdrEth2 = add0+InSz+InSz;
        ipe_changeOutput(&IPEModeInfo.OUTINFO);
        IPEModeInfo.SIZEINFO.LofsEth = ofso;
        ipe_changeSize(&IPEModeInfo.SIZEINFO);

        //Set edge threshold out param
        IPEModeInfo.IQINFO.ParamUpdateSel = IPE_SET_ETH | IPE_SET_EDGE;
        ethParam.Eth = 0x20; //0x20;
        ethParam.HVopt = HVOPT_AVGABS;
        IPEModeInfo.IQINFO.ETH_PARAM= &ethParam;
        edgeKerH.Coeff0 = 12;
        edgeKerH.Coeff1 = 8;
        edgeKerH.Coeff2 = 2;
        edgeKerH.Coeff3 = 0;
        edgeKerH.Coeff4 = -6;
        edgeKerH.Coeff5 = 0;
        edgeKerH.Coeff6 = 0;
        edgeKerH.Coeff7 = -4;
        edgeKerH.Coeff8 = -1;
        edgeKerH.EdgeDiv = 3; //4;
        edgeKerH.EdgeSym = 1;
        IPEModeInfo.IQINFO.EdgeKerH = &edgeKerH;
        edgeKerV.Coeff0 = 12;
        edgeKerV.Coeff1 = 8;
        edgeKerV.Coeff2 = 2;
        edgeKerV.Coeff3 = 0;
        edgeKerV.Coeff4 = -6;
        edgeKerV.Coeff5 = 0;
        edgeKerV.Coeff6 = -4;
        edgeKerV.Coeff7 = 0;
        edgeKerV.Coeff8 = -1;
        edgeKerV.EdgeDiv = 3; //4;
        edgeKerV.EdgeSym = 1;
        IPEModeInfo.IQINFO.EdgeKerV= &edgeKerV;
        IPEModeInfo.IQINFO.FuncUpdateSel = IPE_FUNC_SET; //IPE_FUNC_ENABLE;
        IPEModeInfo.IQINFO.FuncSel |= IPE_EDGETH_EN;
        ipe_changeParam(&IPEModeInfo.IQINFO);
    }
    else
    {
        IQInfo.ParamUpdateSel = IPE_SET_NONE;
        IQInfo.FuncUpdateSel = IPE_FUNC_DISABLE;
        IQInfo.FuncSel = IPE_EDGETH_EN;
        ipe_changeParam(&IQInfo);
        //#NT#2010/10/22#Connie Yu -begin
        //#NT#make sure no one else opens ETH
        IPEModeInfo.IQINFO.FuncSel &= (~IPE_EDGETH_EN);
        //#NT#2010/10/22#Connie Yu -end
    }
}

/*
void dis_Need2UpdateDisplaySz(BOOL update)
{
    gFlg_need2UpdateDisplaySz = update;
}
*/

void dis_updateDisplaySize(void)
{
    //#NT#2010/10/26#Connie Yu -begin
    //#NT#seperate movie DIS from IME on 16:9 calculation
    UINT32 Ratio, inSizeW, inSizeH;
    //Change IME path1 Output size at display/panel ratio
    //Will change to image ratio in SetImeIdeResolution()
    //DispSizeInfo is kept at panel ratio for dzoom flow
    SetIPPSizeInfor(_ImeOut1Hsize, dis_chgSizeByDisViewRatio(DispSizeInfo.uiDisplayFBWidth));
    SetIPPSizeInfor(_ImeOut1Vsize, dis_chgSizeByDisViewRatio(DispSizeInfo.uiDisplayFBHeight));
    SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    //debug_err(("r %d %d %d\r\n",GetIPPSizeInfor(_SensorRatioIdx),gImageAlgInfo.ImageRatio,gImageAlgInfo.DispRatio));
    //if (GetIPPSizeInfor(_SensorRatioIdx) != gImageAlgInfo.ImageRatio)
    if (gImageAlgInfo.ImageRatio != gImageAlgInfo.DispRatio)
    {
        Ratio = Get_HRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);
        inSizeW = Adj_HSize(DispSizeInfo.uiDisplayFBWidth, Ratio, 16);
        Ratio = Get_VRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);
        inSizeH = Adj_VSize(DispSizeInfo.uiDisplayFBHeight, Ratio, 4);
    }
    else
    {
        Ratio = 0;
        inSizeW = DispSizeInfo.uiDisplayFBWidth;
        inSizeH = DispSizeInfo.uiDisplayFBHeight;
    }
    SetIPPSizeInfor(_DisDisplayHSize, inSizeW);
    SetIPPSizeInfor(_DisDisplayVSize, inSizeH);

    //debug_err(("ime %d %d, disd %d %d\r\n",GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_DisDisplayHSize),GetIPPSizeInfor(_DisDisplayVSize)));

    gIdeCorr.x = (GetIPPSizeInfor(_DisDisplayHSize)*(dis_newViewRatio-100)/100)>>1;
    gIdeCorr.y = (GetIPPSizeInfor(_DisDisplayVSize)*(dis_newViewRatio-100)/100)>>1;
    gIdeCenter.x = gIdeCorr.x;
    gIdeCenter.y = gIdeCorr.y;
    //#NT#2010/10/26#Connie Yu -end
}

void dis_updateRecordSize(DIS_SIZE* size)
{
    SetIPPSizeInfor(_DisRecordHSize, size->Hsz);
    SetIPPSizeInfor(_DisRecordVSize, size->Vsz);
    size->Hsz = dis_chgSizeByDisViewRatio(size->Hsz);
    size->Vsz = dis_chgSizeByDisViewRatio(size->Vsz);
    ime_waitFlagFrameEnd();
    //#NT#2010/10/26#Connie Yu -begin
    //#NT#seperate movie DIS from IME on 16:9 calculation
    gRecCorr.x = (GetIPPSizeInfor(_DisRecordHSize)*(dis_newViewRatio-100)/100)>>1;
    gRecCorr.y = (GetIPPSizeInfor(_DisRecordVSize)*(dis_newViewRatio-100)/100)>>1;
    //#NT#2010/10/26#Connie Yu -end
}

void dis_updateJpegLofs(PJPGCmpParam JpgParam)
{
    JpgParam->LineOffsetY = GetIPPSizeInfor(_ImeOut2LineOffset);
    switch(JpgParam->Mode)
    {
        case JPEG_FORMAT_444:
            JpgParam->LineOffsetCbCr= JpgParam->LineOffsetY;
            break;
        case JPEG_FORMAT_422:
        case JPEG_FORMAT_420:
        default:
            JpgParam->LineOffsetCbCr= JpgParam->LineOffsetY/2;
            break;
    }
    //debug_err(("jpg lofs %d\r\n",JpgParam->LineOffsetY));
}

void dis_updateRecAddr(ACTFBADDR_PRAM *pDisRecFbInfo) //for UV packing 420 only
{
    UINT32 offs_Y, offs_C;

    pDisRecFbInfo->bUsingDIS = TRUE;
    pDisRecFbInfo->SrcYLineOffset = GetIPPSizeInfor(_ImeOut2LineOffset);
    offs_Y = gRecCorr.y*pDisRecFbInfo->SrcYLineOffset + gRecCorr.x;
    //for UV packing 420
    pDisRecFbInfo->SrcUVLineOffset = GetIPPSizeInfor(_ImeOut2LineOffset);
    offs_C = (gRecCorr.y/2)*pDisRecFbInfo->SrcUVLineOffset + ((gRecCorr.x>>1)<<1);

    switch(ime_getPath2CurrentBufferIndex())
    {
    case 0:
        pDisRecFbInfo->BorderY_Addr   = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_Y);//for H.264
        pDisRecFbInfo->BorderU_Addr   = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CB);
        pDisRecFbInfo->BorderV_Addr   = ime_getOutputPath2Add(IME_BUFNUM_0,IME_CHANNEL_CR);
        break;
    case 1:
        pDisRecFbInfo->BorderY_Addr   = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_Y);
        pDisRecFbInfo->BorderU_Addr   = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CB);
        pDisRecFbInfo->BorderV_Addr   = ime_getOutputPath2Add(IME_BUFNUM_1,IME_CHANNEL_CR);
        break;
    case 2:
    default:
        pDisRecFbInfo->BorderY_Addr   = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_Y);
        pDisRecFbInfo->BorderU_Addr   = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CB);
        pDisRecFbInfo->BorderV_Addr   = ime_getOutputPath2Add(IME_BUFNUM_2,IME_CHANNEL_CR);
        break;
    }

    pDisRecFbInfo->DisY_Addr = pDisRecFbInfo->BorderY_Addr + offs_Y; //for H.264
    pDisRecFbInfo->DisU_Addr = pDisRecFbInfo->BorderU_Addr + offs_C;
    pDisRecFbInfo->DisV_Addr = pDisRecFbInfo->BorderV_Addr + offs_C;

    pDisRecFbInfo->y_addr = pDisRecFbInfo->BorderY_Addr;
    pDisRecFbInfo->cb_addr = pDisRecFbInfo->BorderU_Addr;
    pDisRecFbInfo->cr_addr = pDisRecFbInfo->BorderV_Addr;

    //debug_err(("************************ 0x%x 0x%x\r\n",pDisRecFbInfo->BorderY_Addr, pDisRecFbInfo->BorderU_Addr));
}
//#NT#2010/01/25#Connie Yu -end

void dis_processOpen(void)
{
    DIS_OPENOBJ DISOpenObj;
    UINT32 intEn;

    aaa_DisLock(UNLOCK);
    dis_updateDisplaySize();
    //ipe set2preview
    intEn = ipe_getIntEnable();
    intEn |= (IPE_INTE_FRM|IPE_INTE_ETHOUTEND);
    ipe_changeInterrupt(intEn);
    gDisProcEvent=DIS_PROC_NOOP;
    dis_attach();
    DISOpenObj.FP_DISISR_CB = dis_isrCB;
    dis_open(&DISOpenObj);
    dis_resetCorrVec(); //Reset DIS flow

    dis_configDisHw();
    dis_enableInt(ENABLE, DIS_INT_FRM);
    dis_load();

    //Enable IPE ETH out
    dis_enableIPEEdgeOut(ENABLE);
    //ime_waitFlagFrameEnd();
    //debug_err(("dis proc open\r\n"));
}

void dis_eventStart(void)
{
    DIS_PARAM modeInfo;

    if(!dis_isEnabled())
    {
        gDisProcEvent=DIS_PROC_UPDATE;

        modeInfo.DIS_operation = DIS_ENGINE_SET2RUN; //Start DIS HW
        dis_setMode(&modeInfo);
        //debug_err(("*** Estart\r\n"));
    }
    else
    {
        gDisProcEvent=DIS_PROC_UPDATE;
        //debug_err(("*** Estart bypass\r\n"));
    }
}

void dis_eventPause(void)
{
    DIS_PARAM modeInfo;

    //aaa_DisLock(LOCK);
    dis_clrIntrStatus(DIS_INT_FRM);
    dis_waitFrameEnd(TRUE);
    modeInfo.DIS_operation = DIS_ENGINE_SET2PAUSE;
    dis_setMode(&modeInfo);

    //To keep frame rate
    gDisProcEvent=DIS_PROC_UPDATE; //DIS_PROC_NOOP;
    //debug_err(("*** Epause\r\n"));
}

void dis_processClose(void)
{
    UINT32 intEn;

    //aaa_DisLock(LOCK);

    if(dis_isEnabled())
    {
        dis_eventPause();
        dis_enableIPEEdgeOut(DISABLE);
    }
    //ipe set2preview
    intEn = ipe_getIntEnable();
    intEn &= (~(IPE_INTE_FRM|IPE_INTE_ETHOUTEND));
    ipe_changeInterrupt(intEn);
    dis_close();
    dis_detach();
    gDisProcEvent=DIS_PROC_NOOP;

    aaa_DisLock(LOCK);
    //debug_err(("dis proc close\r\n"));
}

/*
void dis_processLock(BOOL lock)
{
    if(lock)
    {
        if(!aaa_IsDisLock())
        {
            gDisProcEvent = DIS_PROC_PAUSE;
            //aaa_DisLock(LOCK);
            dis_setLockMatch();
        }
    }
    else
    {
        if(aaa_IsDisLock())
        {
            gDisProcEvent = DIS_PROC_RESTART;
            //aaa_DisLock(UNLOCK);
        }
    }
}
*/


void dis_accumUpdateProcess(void) //accumulation for display
{
    COMPENSATION_INFO compInfo;
    VECTOR ideBound;
    VECTOR ipeOut, ime1out; //ideIn;
#if VER2
    DIS_GM dgmInfo;
#endif

    //ideBound.x = GetIPPSizeInfor(_ImeOut1Hsize) - GetIPPSizeInfor(_DisDisplayHSize);
    //ideBound.y = GetIPPSizeInfor(_ImeOut1Vsize) - GetIPPSizeInfor(_DisDisplayVSize);
    ideBound.x = GetIPPSizeInfor(_DisDisplayHSize)*(dis_newViewRatio-100)/100;
    ideBound.y = GetIPPSizeInfor(_DisDisplayVSize)*(dis_newViewRatio-100)/100;
    ipeOut.y=GetIPPSizeInfor(_IpeOutVsize);
    ipeOut.x=GetIPPSizeInfor(_IpeOutHsize);
    //ideIn.y=GetIPPSizeInfor(_ImeOut1Vsize);
    //ideIn.x=GetIPPSizeInfor(_ImeOut1Hsize);
    ime1out.y=GetIPPSizeInfor(_DisDisplayVSize)*dis_newViewRatio/100;
    ime1out.x=GetIPPSizeInfor(_DisDisplayHSize)*dis_newViewRatio/100;
    compInfo.boundry_hori=ipeOut.x*(dis_newViewRatio-100)/(dis_newViewRatio*2);
    compInfo.boundry_vert=ipeOut.y*(dis_newViewRatio-100)/(dis_newViewRatio*2);
    compInfo.dzoomRate = 1;

    //debug_err(("idebound %d %d\r\n",ideBound.x,ideBound.y));

#if VER2
    dgmInfo.comp = &compInfo;
    dgmInfo.imv = &imv;
    dgmInfo.scoreLv = DIS_MVSCORE_NORMAL;
    dgmInfo.stickyLv = DIS_STICKY_HIGH;
    dis_getIntMVT1(dgmInfo);
#else
    dis_getVideoIMV(&imv, dis_mv, compInfo);
#endif

    //debug_err(("imv %d %d\r\n",imv.x, imv.y));

    if((gCurDisConfig_InH!=GetIPPSizeInfor(_IpeOutHsize)) || (gCurDisConfig_InV!=GetIPPSizeInfor(_IpeOutVsize)))
    {
        //#NT#Avoid transition and offset during Dzoom
        dis_setLockMatch();
        //Reconfig DIS engine & do not update gIdeCorr
        dis_chgDisSizeConfig();
    }
    else
    {
#if 0
        //debug_err(("imv %d %d, cnt %d\r\n",imv.x,imv.y,ipe_getEdgeThCnt()));

        //#for 16:9 image ratio
        //DIS correction vector for LCD display
        gIdeCorr.y = (-imv.y)*ime1out.y;
        gIdeCorr.y = gIdeCorr.y/((INT32)ipeOut.y);
        if(bLockMatch)
        {
            gIdeCenter.y = gIdeVecBefLock.y - gIdeCorr.y;
            gIdeCorr.y = gIdeVecBefLock.y;
        }
        else
            gIdeCorr.y = gIdeCorr.y + gIdeCenter.y;
        gIdeCorr.x = (-imv.x)*ime1out.x;
        gIdeCorr.x = gIdeCorr.x/((INT32)ipeOut.x);
        if(bLockMatch)
        {
            gIdeCenter.x = gIdeVecBefLock.x - gIdeCorr.x;
            gIdeCorr.x = gIdeVecBefLock.x;
            bLockMatch = FALSE;
        }
        else
            gIdeCorr.x = gIdeCorr.x + gIdeCenter.x;
#else
        gIdeCorr.y = (-imv.y)*ime1out.y;
        gIdeCorr.y = gIdeCorr.y/((INT32)ipeOut.y);
        gIdeCorr.y = gIdeCorr.y + (ideBound.y/2);
        gIdeCorr.x = (-imv.x)*ime1out.x;
        gIdeCorr.x = gIdeCorr.x/((INT32)ipeOut.x);
        gIdeCorr.x = gIdeCorr.x + (ideBound.x/2);
#endif

        //Check if imv exceeds compensation range
        if((gIdeCorr.x>(ideBound.x-1)) || (gIdeCorr.x<0)){ // X direction
            //debug_err(("%d hits HB\r\n",gIdeCorr.x));
            if(gIdeCorr.x<0)
            {
                imv.x = (INT32)(compInfo.boundry_hori);
                gIdeCorr.x=0;
            }
            else
            {
                imv.x = (INT32)(-compInfo.boundry_hori);
                gIdeCorr.x=ideBound.x-1;
            }
        }
        if((gIdeCorr.y>(ideBound.y-1)) || (gIdeCorr.y<0)){ // Y direction
            //debug_err(("%d hits YB\r\n",gIdeCorr.y));
            if(gIdeCorr.y<0)
            {
                imv.y = (INT32)(compInfo.boundry_vert);
                gIdeCorr.y=0;
            }
            else
            {
                imv.y = (INT32)(-compInfo.boundry_vert);
                gIdeCorr.y=ideBound.y-1;
            }
        }
    }
    //debug_err(("m%d, %d %d  \r\n",ime_getPath1CurrentBufferIndex(),gIdeCorr.x,gIdeCorr.y));
    //debug_err(("b%d \r\n",ime_getPath1CurrentBufferIndex()));
}

void dis_accumRecProcess(void) //accumulation for imep2 & codec
{
    COMPENSATION_INFO compInfo;
    VECTOR recBound;
    VECTOR ipeOut, ime2Out;
#if VER2
    DIS_GM rgmInfo;
#endif

    //recBound.x = GetIPPSizeInfor(_ImeOut2Hsize) - GetIPPSizeInfor(_DisRecordHSize);
    //recBound.y = GetIPPSizeInfor(_ImeOut2Vsize) - GetIPPSizeInfor(_DisRecordVSize);
    recBound.x = GetIPPSizeInfor(_DisRecordHSize)*(dis_newViewRatio-100)/100;
    recBound.y = GetIPPSizeInfor(_DisRecordVSize)*(dis_newViewRatio-100)/100;
    ipeOut.y=GetIPPSizeInfor(_IpeOutVsize);
    ipeOut.x=GetIPPSizeInfor(_IpeOutHsize);
    //ime2Out.y=GetIPPSizeInfor(_ImeOut2Vsize);
    //ime2Out.x=GetIPPSizeInfor(_ImeOut2Hsize);
    ime2Out.y=GetIPPSizeInfor(_DisRecordVSize)*dis_newViewRatio/100;
    ime2Out.x=GetIPPSizeInfor(_DisRecordHSize)*dis_newViewRatio/100;
    compInfo.boundry_hori=ipeOut.x*(dis_newViewRatio-100)/(dis_newViewRatio*2);
    compInfo.boundry_vert=ipeOut.y*(dis_newViewRatio-100)/(dis_newViewRatio*2);
    compInfo.dzoomRate = 1;

#if VER2
    rgmInfo.comp = &compInfo;
    rgmInfo.imv = &rec_imv;
    rgmInfo.scoreLv = DIS_MVSCORE_NORMAL;
    rgmInfo.stickyLv = DIS_STICKY_HIGH;
    dis_getIntMVT2(rgmInfo);
#else
    dis_getVideoIMV(&rec_imv, disRec_mv, compInfo);
#endif

    gRecCorr.y = (-rec_imv.y)*ime2Out.y;
    gRecCorr.y = gRecCorr.y/((INT32)ipeOut.y);
    gRecCorr.y = gRecCorr.y + (recBound.y/2);
    gRecCorr.x = (-rec_imv.x)*ime2Out.x;
    gRecCorr.x = gRecCorr.x/((INT32)ipeOut.x);
    gRecCorr.x = gRecCorr.x + (recBound.x/2);
    //Check if imv exceeds compensation range
    if((gRecCorr.x>(recBound.x-1)) || (gRecCorr.x<0)){  // X direction
        if(gRecCorr.x<0)
        {
            rec_imv.x = (INT32)(compInfo.boundry_hori);
            gRecCorr.x=0;
        }
        else
        {
            rec_imv.x = (INT32)(-compInfo.boundry_hori);
            gRecCorr.x=recBound.x-1;
        }
        //debug_ind(("   bound Vid H\r\n"));
    }
    if((gRecCorr.y>(recBound.y-1)) || (gRecCorr.y<0)){  // Y direction
        if(gRecCorr.y<0)
        {
            rec_imv.y = (INT32)(compInfo.boundry_vert);
            gRecCorr.y=0;
        }
        else
        {
            rec_imv.y = (INT32)(-compInfo.boundry_vert);
            gRecCorr.y=recBound.y-1;
        }
        //debug_ind(("   bound Vid V\r\n"));
    }
}

/*
void dis_setIPPMode(DIS_MODE mode)
{
    if(mode != gDis_IppMode)
        dis_resetCorrVec();
    gDis_IppMode = mode;
}
*/

UINT32 Get_DISPrvMaxBuffer(void)
{
    UINT32 InBufSz, OutBufSz, ofso;

    ofso = (((GetIPPSizeInfor(_IpeOutHsize)>>3) + 3) >> 2) << 2;
    InBufSz = 3 * ofso * GetIPPSizeInfor(_IpeOutVsize); //binary plane
    OutBufSz = 2 * (16 * 16 * 2 * 4); // = mds * blk/mds * MV word/blk * byte/word
    return (((InBufSz+OutBufSz+3)>>2)<<2);
}

UINT32 dis_chgSizeByDisViewRatio(UINT32 size)
{
    return ((((size*dis_newViewRatio/100)+1)>>1)<<1);
}

UINT16 dis_getDisViewRatio(void)
{
    return dis_newViewRatio;
}

MOTION_VECTOR dis_getIdeCorrVec(void)
{
    return gIdeCorr;
}

DIS_PROC_EVENT dis_getProcEvent(void)
{
    return gDisProcEvent;
}

UINT8 dis_getImeBufferID(void)
{
    if(gDisFreezeFB)
        return 0; //buffer0
    else
        return ime_getPath1CurrentBufferIndex();

}
void dis_setFlagFreezeFB(BOOL bFreeze)
{
    gDisFreezeFB = bFreeze;
}

void dis_setLockMatch(void)
{
    gIdeVecBefLock.x = gIdeCorr.x;
    gIdeVecBefLock.y = gIdeCorr.y;
    bLockMatch = TRUE;
}
//#NT#2010/10/28#Connie Yu -end

//@}

