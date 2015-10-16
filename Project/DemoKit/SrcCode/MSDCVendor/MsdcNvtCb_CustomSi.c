#include "OSCommon.h"
#include "MSDCVendorNVTApi.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_CustomSi.h"
#include "SysCfg.h"
#include "UIMenuPhotoSettings.h"
#include "UIMenuSetupSettings.h"
#include "UISystemObj.h"
#include "UIPhotoObj.h"
#include "NVTUserCommand.h"
#include "PrimaryTsk.h"
#include "cal_common_630.h"
#include "ImageAlgInfor.h"
#include "CalibrationInt.h"
#include "Cal_Alg.h"
#include "KeyScanTsk.h"
#include "PlaySoundTsk.h"
#include "GPIOMapping.h"
#include "UIFlowWndMovie.h"

//#NT#declare the TekSrping AP function
//Step 1: Declare custom functions
//Declare Gets Functions
static void GetSayHi(void);      //test
static void GetSayHello(void);   //test
static void GetFlashTestresult(void);
static void GetSdTestResult(void);
static void GetSdProtectResult(void);
static void GetLcdCloseFlag(void);
static void GetDefectPixelResult(void);
static void GetTestResult(void);
static void GetRawDataInfo(void);
//Declare Sets Functions
static void SetSayHi(void);      //test
static void SetSayHello(void);   //test
static void SetFlashTest(void);
static void SetSdCardTest(void);
static void SetLcdCloseFlag(void);
static void SetVideoRecAndPlay(void);
static void SetDefectPixel(void);
static void SetColorShadingRegister(void);
static void SetSetOut(void);
static void SetPWROff(void);
static void SetLedTest(void);
static void SetAudioTest(void);


//Delcare the global variables for various test
static BOOL   bSdTestResult = TRUE;
static BOOL   bSdProtectSts = FALSE;
static UINT32 bDPCErrorCode = 0xFFFFFFFF;
static BOOL   bMsdcVendorVideoRecord = FALSE;
static BOOL   bIsFlashTesting = FALSE;
static BOOL   bLSCTestResult = FALSE;

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void)=
{
        //GetSayHi,                   // 0, #define ID_GET_SAY_HI    0 //in AP Side
        //GetSayHello,                // 1, #define ID_GET_SAY_HELLO 1 //in AP Side
        GetFlashTestresult,
        GetSdTestResult,
        GetSdProtectResult,
        GetLcdCloseFlag,

        GetDefectPixelResult,
        GetTestResult,
        GetRawDataInfo,             // 6, #define ID_GET_RAW_DATA_INFO 6 //in AP Side
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void)=
{
        //SetFlashTest,                   // 0, #define ID_SET_SAY_HI    0 //in AP Side
        //SetSayHello,                // 1, #define ID_SET_SAY_HELLO 1 //in AP Side
        SetFlashTest,
        SetSdCardTest,
        SetLcdCloseFlag,
        SetVideoRecAndPlay,
        SetDefectPixel,
        SetColorShadingRegister,
        SetSetOut,
        SetPWROff,
        SetLedTest,
        SetAudioTest,
};

//Step 4: Provide API for Register Single Direction Functions
BOOL CustomSi_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets))
{
    return fpMSDCVendorNVT_AddCallback_Si(MSDCCustomGet,sizeof(MSDCCustomGet)/sizeof(MSDCCustomGet[0]),MSDCCustomSet,sizeof(MSDCCustomSet)/sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
static void GetSayHi(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("@@@@@@ Get Say Hi @@@@@@\r\n");
    *pData = 12345;
}

void GetSayHello(void)
{
    UINT32  *pData    = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    debug_msg("uiLength=%d\r\n",uiLength);

    if(uiLength!=sizeof(UINT32)) //invalid data
        return;

    debug_msg("@@@@@@ Get Say Hello @@@@@@\r\n");
    *pData = 67890;
}

//#NT#2009/12/03#KS Hung -begin
//#NT#TekSrping AP function
static void GetFlashTestresult(void)
{
    return;
}

static void GetSdTestResult(void)
{

}

static void GetSdProtectResult(void)
{


}

static void GetLcdCloseFlag(void)
{

}

static void GetDefectPixelResult(void)
{

}

BOOL GetMsdcVendorVideoRecFlag(void)
{
    return bMsdcVendorVideoRecord;
}
//#NT#2009/12/08#KS Hung -end

static void GetTestResult(void)
{

}

static void GetRawDataInfo(void)
{


}

void SetSayHi(void)
{

}

void SetSayHello(void)
{

}

static void SetFlashTest(void)
{

}

static void SetSdCardTest(void)
{

}

static void SetLcdCloseFlag(void)
{

}

static void SetVideoRecAndPlay(void)
{

}

void SetMsdcVendorVideoRecFlag(BOOL bFlag)
{
    bMsdcVendorVideoRecord = bFlag;
}

static void SetDefectPixel(void)
{

}

static void SetColorShadingRegister(void)
{

}

static void SetSetOut(void)
{

}

static void SetPWROff(void)
{

}

static void SetLedTest(void)
{

}

static void SetAudioTest(void)
{

}

