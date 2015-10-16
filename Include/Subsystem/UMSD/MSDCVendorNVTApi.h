#ifndef _MSDCVENDORNVTAPI_H
#define _MSDCVENDORNVTAPI_H

#include "OSCommon.h"

typedef struct _MSCDVENDOR_CFG{
    UINT8* pMemCache;                               //!< Cache-able Memory (0x24000 Bytes Requirement) Front-End:0x10000,Back-End:0x10000,Msg:0x4000
    UINT32 nSizeCache;                              //!< n Bytes of Cache Memory
    ID     SemID;                                   //!< Semaphore ID for DBGSYS
    ID     TaskID;                                  //!< Working Task ID for Command Send
    void   (*fpDoCommand)(CHAR *pcCmdStr);          //!< Callback for calling command
    void   (*fpRegHookDbgMsg)(ER (*PutStr)(CHAR*)); //!< Register debug_msg output function
}tMSDCVENDOR_CFG;

//APIs for MSDCVENDOR_NVT (MSDC Vendor)
extern BOOL     MSDCVendorNVT_Configure(tMSDCVENDOR_CFG* pCfg);                                     //!< Configure Msdc Vendor NVT System for Initial
extern UINT8*   MSDCVendorNVT_GetDataBufferAddress(void);                                           //!< Get Data Address for Single Direct Callback Function
extern UINT32   MSDCVendorNVT_GetDataBufferSize(void);                                              //!< Get Data Size for Single Direct Callback Function
extern BOOL     MSDCVendorNVT_AddCallback_Bi(char* szName,void (*fp)(void* pData));                 //!< PlugIn Vendor Function(BiDirection, form PC Set then Get automatic)
extern BOOL     MSDCVendorNVT_AddCallback_Si(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets);    //!< Plugin Vendor Function(Single Direction,from PC-Get and PC-Set, UPccCustom Compatiable)
extern UINT32   MSDCVendorNVT_GetTransSize(void);                                                   //!< for MSDCVendorNVT_AddCallback_Si functions implementation use
extern void     MsdcVendorNvtTsk(void);                                                             //!< This Task is for Do Background Command
#endif
