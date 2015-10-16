#include "SysCfg.h"
#include "Kernel.h"
#include "ImageAlgInfor.h"
#include "SysCfg.h"
#include "cal_common_630.h"
#include "Cal_Alg.h"
#include "Cache.h"
//#NT#2008/07/08#Bowen Li -begin
#include "Cal_Api.h"
//#NT#2008/07/08#Bowen Li -end
//#NT#2007/04/02#Chris Hsu -begin
//#Add PStore sample code
#if (_PSTORE_VER_ == _PSTORE_VER_100_)
//#NT#2007/04/02#Chris Hsu -end
#include "FilesysTsk.h"
#else
#include "PStore.h"
#endif

volatile Adjust_Struct  AAA_Adjust;
UINT     gCapRawAddr    = 0;
static BOOL Done_Cal    = FALSE;
static BOOL FirstRead   = TRUE;

//----------------------------------------------------------------------------------------------------
ER WriteCalData(void)
{
    #define WRITECALDATA_TAKE_NAND_BACK 0
    //#NT#2008/11/27#Jarkko Chang -begin
    //#remove warning msg
    //    UINT8   i;
    //#NT#2008/11/27#Jarkko Chang -end
    #if WRITECALDATA_TAKE_NAND_BACK
    //#NT#2008/11/27#Jarkko Chang -begin
    //#remove warning msg
    static NandAdjust_Struct Nand_Data;
    //#NT#2008/11/27#Jarkko Chang -end    
    if(AAA_Adjust.ISO_Status != _ISO_Status)
    {
        AAA_Adjust.Agc_ISOMin = Nand_Data.Agc_ISOMin;
    }

    if(AAA_Adjust.Mshutter_Status[0] != _Mshutter_Status)
    {
        AAA_Adjust.Mshutter = Nand_Data.Mshutter;
    }

    if(AAA_Adjust.FTP_Status != _FTP_Status)
    {
        AAA_Adjust.FlashTriggerPoint=Nand_Data.FlashTriggerPoint;
    }
    if(AAA_Adjust.AWB_Status != _AWB_Status)
    {
        AAA_Adjust.AWB.Rgain = Nand_Data.AWB.Rgain;
        AAA_Adjust.AWB.Bgain = Nand_Data.AWB.Bgain;
    }
    if(AAA_Adjust.Flash_Status != _Flash_Status)
    {
        AAA_Adjust.Flash_Y_Avg  = Nand_Data.Flash_Y_Avg;
    }
    if(AAA_Adjust.OB_Status != _OB_Status)
    {
        AAA_Adjust.OB_Min=Nand_Data.OB_Min;
    }
    if(AAA_Adjust.FFocus_Status != _FF_Status)
    {
        for(i=0;i<15;i++)
            AAA_Adjust.Far_Focus[i]=Nand_Data.Far_Focus[i];
    }
    if(AAA_Adjust.NFocus_Status != _NF_Status)
    {
        for(i=0;i<15;i++)
            AAA_Adjust.Near_Focus[i]=Nand_Data.Near_Focus[i];
    }
    #endif // WRITECALDATA_TAKE_NAND_BACK

    CPUCleanInvalidateDCacheBlock((UINT32)&AAA_Adjust, (UINT32)&AAA_Adjust + sizeof(AAA_Adjust));
    CPUDrainWriteBuffer();

//#NT#2009/10/15#KS Hung -begin
//#Add PStore sample code
#if 0//(_PSTORE_VER_ == _PSTORE_VER_100_)
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysNANDWriteSysParamBlockByBytes((CHAR *)&AAA_Adjust, 0, sizeof(AAA_Adjust));
#else
{
    PPSTORE_SECTION_HANDLE  pSection;

    //#NT#2007/06/01#Chris Hsu -begin
    //#Fix wrong session name
    if ((pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR|PS_CREATE)) != E_PS_SECHDLER)
    //#NT#2007/06/01#Chris Hsu -end
    {
        PStore_WriteSection((UINT8 *)&AAA_Adjust, 0, sizeof(AAA_Adjust), pSection);
        PStore_CloseSection(pSection);
    }
    else
        debug_err(("Pstore open to write FAIL\r\n"));

}
#endif
//#NT#2009/10/15#KS Hung -end

    //Read AAA_Adjust from NAND again to avoid Nand_Data value be set to AAA_Adjust
    FirstRead=TRUE;
    ReadCalData();
    return  E_OK;
}


ER ReadCalData(void)
{
    if (FirstRead)
        FirstRead = FALSE;
    else
        return E_OK;

    CPUCleanInvalidateDCacheBlock((UINT32)&AAA_Adjust, (UINT32)&AAA_Adjust + sizeof(AAA_Adjust));
    CPUDrainWriteBuffer();

//#NT#2009/10/15#KS Hung -begin
//#Add PStore sample code
#if 0//(_PSTORE_VER_ == _PSTORE_VER_100_)
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysNANDReadSysParamBlockByBytes((CHAR *)&AAA_Adjust, 0, sizeof(AAA_Adjust));
#else
{
    PPSTORE_SECTION_HANDLE  pSection;

    //#NT#2007/06/01#Chris Hsu -begin
    //#Fix wrong session name
    if ((pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR)) != E_PS_SECHDLER)
    //#NT#2007/06/01#Chris Hsu -end
    {
        PStore_ReadSection((UINT8 *)&AAA_Adjust, 0, sizeof(AAA_Adjust), pSection);
        PStore_CloseSection(pSection);
    }
    else
        debug_err(("Pstore open to read FAIL\r\n"));

    debug_msg("AAA_Adjust : %d,%d %d\r\n",AAA_Adjust.AWB_Status,AAA_Adjust.AWB.Rgain,AAA_Adjust.AWB.Bgain);

}
#endif
//#NT#2009/10/15#KS Hung -end

    debug_ind(("Read Adjust Data from Nand\r\n"));
    Done_Cal = TRUE;
#if (_CALIBRATION_MODE_ == ENABLE)
//    memcpy((UB*)(g_CS_GainTblBufAdd), (UB*)(AAA_Adjust.CSC_Table), ColorShadingTbl_H*ColorShadingTbl_V*3);

    CAL_PassAdjustData();
#endif
    return E_OK;
}
