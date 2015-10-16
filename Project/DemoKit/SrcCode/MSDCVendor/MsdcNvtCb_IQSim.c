#include "OSCommon.h"
#include "MsdcNvtCb_Int.h"
#include "MSDCVendorNVTApi.h"
#include "MsdcNvtCb_CustomSi.h"  //marked by Linkin
#include "SysCfg.h"
#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "ime_lib.h"
#include "ImageQualitySim.h"   //marked by Linkin

#include "SieCapTsk.h"
#include "Flag.h"
#include "PrimaryTsk.h"
#include "UMSDC.h"

#define IQCMD_SET          0x9a00f000
#define IQCMD_IQLOCK       0x9a00f001
#define IQCMD_SAVEPRVRAW_START 0x9a00f002
#define IQCMD_SAVEPRVRAW_END   0x9a00f003
#define IQCMD_PRVRAWINFO_GET   0x9a00f004
#define IQCMD_I2C           0x9a00f004
#define AECMD_ENABLE       0x9a00f101
#define AECMD_EXPOTIME     0x9a00f102
#define AECMD_ISO          0x9a00f103
#define AECMD_RAWY         0x9a00f104
#define AECMD_LV           0x9a00f105
#define AWBCMD_ENABLE      0x9a00f201
#define AWBCMD_GAIN        0x9a00f202
#define AWBCMD_CASET       0x9a00f203
#define AWBCMD_CADUMP      0x9a00f204
#define AFCMD_ENABLE       0x9a00f301
#define AFCMD_POSITION     0x9a00f302
#define AFCMD_VA           0x9a00f304


UINT32 IQCmd;
UINT32 IQParameter;

extern UINT32 uiAE_TEST;
extern UINT32 uiAeIsoGain;
extern UINT32 uiAeExpoTime;

UINT32 AE__GetLV(void);

//Step 1: Declare custom functions
//Declare Gets Functions
void GetBinAddr(void);
void GetRawAddr(void);
void GetJpgAddr(void);
void GetJpgOk(void);
void GetJpgSize(void);
void GetSysStatus(void);
void GetIQCtrl(void);

extern void     aaa_AeLock(UINT8 lock);
extern BOOL     Is_AELock(void);

//Declare Sets Functions
void SetConvert(void);
void SetIQCtrl(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void)=
{
        GetBinAddr,  // 0
        GetRawAddr,  // 1
        GetJpgAddr,  // 2
        GetJpgOk,    // 3
        GetJpgSize,  // 4
        GetSysStatus, // 5
        GetIQCtrl, //6
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void)=
{
        SetConvert,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        SetIQCtrl,  //6
};

//Step 4: Provide API for Register Single Direction Functions
BOOL IQSim_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets))
{
    return fpMSDCVendorNVT_AddCallback_Si(MSDCCustomGet,sizeof(MSDCCustomGet)/sizeof(MSDCCustomGet[0]),MSDCCustomSet,sizeof(MSDCCustomSet)/sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
UINT8 StartCnt = 0;
void GetBinAddr(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

/*
    if(StartCnt == 0)
    {

        //disable ipe
      ipe_pause();

      //disable sensor data & pre
      pre_pause();
      sie_pause();

      //reset pre ipe ime
      ipe_reset(ON);
      ime_setStop();
      ime_setReset();


      pre_close();
      //debug_err(("pre_close ok\r\n"));
      sie_close();
      //debug_err(("sie_close ok\r\n"));

      //reset pre ipe ime
      ipe_close();
      ime_close();

        //aaa_NtSmrFilterEnable(DISABLE);



    aaa_AeLock(TRUE);
      aaa_AwbLock(TRUE);
      aaa_FdLock(TRUE);
      aaa_SdLock(TRUE);
      //aaa_DisLock(TRUE); //Linkin

      sie_waitVD(10);

      StopPreviewDramData();

      pre_close();
      //debug_err(("pre_close ok\r\n"));
      sie_close();
      //debug_err(("sie_close ok\r\n"));

      //debug_err(("StopPreviewDramData ok\r\n"));
      ime_close();
      //debug_err(("ime_close ok\r\n"));
      ipe_close();


    }
    StartCnt = 1;
*/

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("===== Get Bin Addr =====\r\n");
    //*pData = GetIndepAddr();
    //debug_err(("BinAddr = 0x%x\r\n", GetIndepAddr()));
}

void GetJpgAddr(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    //debug_msg("uiLength=%d\r\n",uiLength);

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("===== Get JPEG Addr =====\r\n");
    //*pData = IndepSliceGetJPGAddr();
    //debug_err(("JpgAddr = 0x%x\r\n", GetIndepAddr()));
}

void GetJpgSize(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    //debug_msg("uiLength=%d\r\n",uiLength);

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("===== Get JPEG Size =====\r\n");
    //*pData = IndepSliceGetJPGSize();  // Linkin
    //debug_err(("JpgSize = 0x%x\r\n", IndepGetJpgSize()));
}

void GetJpgOk(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    //Indep_WaitComplete();

    //debug_msg("uiLength=%d\r\n",uiLength);

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("===== Get JPEG OK =====\r\n");
    *pData = 1;
    //debug_err(("JpgOk = 0x%x\r\n", 1));


}

void GetRawAddr(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    //debug_msg("uiLength=%d\r\n",uiLength);

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("===== Get Raw Addr =====\r\n");
    //*pData = GetIndepAddr()+5000*64*6+0x100000+8192;  // Linkin
}

UINT32 ConnectToSensor = 1;
BOOL isExecutePhotoMode = FALSE;
BOOL haveStopEngines = FALSE;
void GetSysStatus(void)
{
  UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
  UINT32   uiLength = MsdcNvt_GetTransSize();

  UINT32   IPL_Status_Sim = 0;

  if(ConnectToSensor == 1)
    IPL_Status_Sim = IPL_GetMode();

  if((IPL_Status_Sim == 0) && (USBMSDC_GetStatus() == UMSDC_COMMANDOK) && (isExecutePhotoMode == TRUE) && (haveStopEngines == FALSE)) // IPL_MODE_OFF = 2
  {
      debug_msg("IPL_Status_Sim : %d\r\n", IPL_Status_Sim);
      debug_msg("isExecutePhotoMode : %d\r\n", isExecutePhotoMode);
      //disable ipe
      ipe_pause();

      //disable sensor data & pre
      pre_pause();
      sie_pause();

      //reset pre ipe ime
      ipe_reset(ON);
      ime_setStop();
      ime_setReset();

      pre_close();
      sie_close();

      //reset pre ipe ime
      ipe_close();
      ime_close();

      ConnectToSensor = 0;
      isExecutePhotoMode = TRUE;
      haveStopEngines = TRUE;
  }
  else if((IPL_Status_Sim == 2)  && (USBMSDC_GetStatus() == UMSDC_COMMANDOK) && (isExecutePhotoMode == TRUE) && (haveStopEngines == FALSE)) // IPL_MODE_PREVIEW = 2
  {
      debug_msg("IPL_Status_Sim : %d\r\n", IPL_Status_Sim);
      debug_msg("isExecutePhotoMode : %d\r\n", isExecutePhotoMode);

      aaa_AeLock(TRUE);
      aaa_AwbLock(TRUE);
      aaa_FdLock(TRUE);
      aaa_SdLock(TRUE);
      //aaa_DisLock(TRUE); //Linkin

      sie_waitVD(10);

      StopPreviewDramData();

      pre_close();
      sie_close();

      ime_close();
      ipe_close();

      ConnectToSensor = 0;
      isExecutePhotoMode = TRUE;

      haveStopEngines = TRUE;
  }
  else
  {
      // do something ...
  }

  if(isExecutePhotoMode == FALSE)
  {
      debug_err(("Please get into Photo mode ...\r\n"));
  }

  debug_msg("===== Get System Status =====\r\n");
  *pData = ConnectToSensor;
}


void SetConvert(void)
{
    UINT32* pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();
    PFLGPTN    uiFlag;

    //debug_msg("###### Set Convert ###### \r\n");

    //Trigger_IndepSlice630();

/*
    if(uiLength)
    {
        if(pData[0]==TRUE)
        {
            //TriggerIndepFrame();  // Linkin
            //debug_msg("Set Convert\r\n");
            sta_tsk(IMGSLICETSK_ID);
            debug_msg("IMGSLICETSK_ID ... done\r\n");

            clr_flg(FLG_ID_SLICE, FLGSLICETSK1_INDEPSLICE_END);
            debug_msg("clear FLGSLICETSK1_INDEPSLICE_END ... done\r\n");

            // Start to do Independent Slice.
            set_flg(FLG_ID_SLICE, FLGSLICETSK1_INDEPSLICE_START);
            debug_msg("set FLGSLICETSK1_INDEPSLICE_START ... done\r\n");


            // Wait an Image completion.
            wai_flg(uiFlag, FLG_ID_SLICE, FLGSLICETSK1_INDEPSLICE_END, TWF_CLR);
            debug_msg("wait FLGSLICETSK1_INDEPSLICE_END ... done\r\n");

            //debug_err(("FLGSLICETSK1_INDEPSLICE_END  end\r\n"));
            // Stop the IMGSLICE task.
            ter_tsk(IMGSLICETSK_ID);
        }
    }
    */
}

void GetIQCtrl(void)
{
    UINT32 uiRaccAddr, uiGaccAddr, uiBaccAddr;
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();
    debug_msg("Get %02X, %02X\r\n",IQCmd,IQParameter);
    if ( IQCmd == AFCMD_ENABLE )
    {
        pData[0] = Is_AFLock();
    }
    else if ( IQCmd == IQCMD_IQLOCK )
    {
        pData[0] = Is_AELock();
    }
    else if ( IQCmd == AECMD_ENABLE )
    {
        pData[0] = uiAE_TEST;
    }
    else if ( IQCmd == AECMD_EXPOTIME )
    {
        pData[0] = uiAeExpoTime;
    }
    else if ( IQCmd == AECMD_ISO )
    {
        pData[0] = uiAeIsoGain;
    }
    else if ( IQCmd == AECMD_LV )
    {
        pData[0] = AE__GetLV();
    }

    debug_msg("%X\r\n",pData[0]);
}

void SetIQCtrl(void)
{
    UINT32* pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();

    if(!uiLength)
        return ;
    if ( pData[0] == IQCMD_SET )
    {
        IQCmd = pData[1];
        IQParameter = pData[2];
    }
    else if ( pData[0] == IQCMD_IQLOCK )
    {
        aaa_AeLock(pData[1]);
    }
    else if ( pData[0] == IQCMD_I2C )
    {
        debug_err(("Set %04X, %02X\r\n",pData[1],pData[2]));
        //Sensor_WriteReg8bit(pData[1], pData[2]);
    }
    else if ( pData[0] == AECMD_ENABLE )
    {
        //aaa_AeLock(pData[1]);
        uiAE_TEST = pData[1];
    }
    else if ( pData[0] == AECMD_EXPOTIME )
    {
        uiAeExpoTime = pData[1];
    }
    else if ( pData[0] == AECMD_ISO )
    {
        uiAeIsoGain = pData[1];
    }


}

