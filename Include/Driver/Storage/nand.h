/**
    Nand flash driver export variables and function prototypes.

    Nand flash driver export variables and function prototypes file

    @file       nand.h
    @ingroup    mICardNAND
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/


#ifndef _NAND_H
#define _NAND_H

#include "StrgDef.h"

/**
@addtogroup mICardNAND

*/

//@{


//------------------------------------------------------------
// The general api for the Nand device driver
//------------------------------------------------------------

//NAND driver error code
//bit 31...28    Card type : NAND : 0
//bit 27...12    NAND error
//bit 11...0     NAND / SMC /xD error


// ------------------------------------------------------------------------
// NAND driver error code
// ------------------------------------------------------------------------
//
#define NAND_ERROR_OK                       0x00000000

#define NAND_ERROR_INSUFFICIENT_FREE_BLOCK  0x00010000
#define NAND_ERROR_TOO_MANY_BAD_UNITS       0x00080000 //Too many bad blocks result in write protection rasen.(write and erase is prohibited)

//NAND / SMC /xD common error (Please keep below error code number the same with xD driver (xD.h))
//Data error
#define NAND_ERROR_DATA_STATUS              0x00000001 //Data status is corrupt
#define NAND_ERROR_ECC_FIELD1               0x00000002
#define NAND_ERROR_ECC_FIELD2               0x00000004
#define NAND_ERROR_DOUBLE_MAPPING           0x00000008

//Operation error
#define NAND_ERROR_READ_ERROR               0x00000010
#define NAND_ERROR_WRITE_ERROR              0x00000020
#define NAND_ERROR_DEVICE_ERROR             0x00000040

typedef void (*PrCardProtectFunc) (BOOL);


/* NAND / xD device information*/
typedef struct
{
    UINT32 uiPhysicalBlockCount;    //< blocks in the device
    UINT32 uiPagePerBlock;          //< pages in a block
    UINT32 uiBytesPerPageData;      //< data size in bytes (excluding spare area)
    UINT32 uiSparePerPage     ;     //< spare size in each page
    UINT32 uiBlockSize;             //< block size in bytes
    UINT16 uiCardType;              //< Card type
    UINT8  ucMakerCode;             //< 0xEC:samsung, 0x98:toshiba
    UINT8  ucDeviceCode;
} NandXdInfoStruct, *PNandXdInfoStruct;

extern STRG_TAB gNandObj;
// Driver API

//Open standard storage API for low level access
extern ER        nand_open(void);
extern ER        nand_close(void);
extern ER        nand_readSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt);
extern ER        nand_writeSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt);
extern ER        nand_format(void);
extern UINT32    nand_getLastError(void);
extern void      nand_clearLastError(void);
extern PSTRG_TAB nand2_getStorageObject(void);
extern ER       nand2_writeSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern ER       nand2_readSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern void     nand2_setSize(UINT32 size);
ER              nand2_format(void);
void            nand2_FormatEraseNAND(void);

extern PSTRG_TAB nand3_getStorageObject(void);
extern ER       nand3_writeSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern ER       nand3_readSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
extern void     nand3_setSize(UINT32 size);
ER              nand3_format(void);
void            nand3_FormatEraseNAND(void);

extern UINT32    nand_readID(void);
extern void      nand_setReservedSize(UINT32 size);
extern void      nand_setReservedStartBlockIndex(UINT32 BlockIndex);
extern void      nand_setSysParamBlockNum(UINT32 size);
extern UINT32    nand_getBlockSize(void);
extern ER        nand_readSysParamBlockSeq(INT8 *pcBuf, UINT16 block, UINT16 count);
extern ER        nand_writeSysParamBlockSeq(INT8 *pcBuf, UINT16 block, UINT16 count);
extern ER        nand_readSysParamBlock(INT8 *pcBuf, UINT16 block);
extern ER        nand_writeSysParamBlock(INT8 *pcBuf, UINT16 block);
extern ER        nand_readSysParamBlockByBytes(INT8 *pcBuf, UINT16 block, UINT32 bytes);
extern ER        nand_writeSysParamBlockByBytes(INT8 *pcBuf, UINT16 block, UINT32 bytes);
extern ER        nand_readReservedBlock(INT8 *pcBuf, UINT16 uiLogicalBlockNum, UINT16 uiBlkCnt);
extern ER        nand_writeReservedBlock(INT8 *pcBuf, UINT16 uiLogicalBlockNum, UINT16 uiBlkCnt);
extern ER        nand_readReservedBlockByBytes(INT8 *pcBuf, UINT16 uiLogicalBlockNum, UINT32 bytes);
extern ER        nand_setIntEn(BOOL bEn);
extern void      nand_enableCardProtection(BOOL bProtection);
extern PSTRG_TAB nand_getStorageObject(void);
extern void      nand_regCardWriteProtectFunc(PrCardProtectFunc CardWriteProtectFunc);
extern BOOL      nand_getDeviceInfo(PNandXdInfoStruct pNandInfo);

extern BOOL      nand_eraseCheckBadBlock(UINT32 block_num);
extern UINT32    nand_eraseCheckFAT(void);
extern BOOL      nand_readCheckBadBlock(UINT32 block_num);
extern UINT32    nand_readCheckAll(BOOL bDumpAllBlock);
extern UINT32    nand_readCheckAllBadBlocks(BOOL bMarkErrorBlock);

extern ER        nand_readOperation(INT8 *buffer, UINT32 pageAddress, UINT32 numPage);
extern ER        nand_readPageSpareData1(UINT32 *spare1, UINT32 pageAddress);
extern ER        nand_readPageSpareDataTotal(UINT32 *spare0,UINT32 *spare1, UINT32 *spare2, UINT32 *spare3, UINT32 pageAddress);


extern void      nandps_getpstoreMapTable(UINT16 ** MapTable);

extern void      EraseAllNANDBlockMaping(void);




extern void nand_setValidErase(BOOL bValidErase);
extern void nand_setCPUAccessErr(BOOL bcpu_Acc);
extern void nand_setCPUAccessSpare(BOOL bcpu_Acc);
extern ER     nand_writeOneReservedBlock(UINT32 PhyBlkIdx, UINT32 LogBlkIdx, UINT8 *Buf);
extern UINT32 nand_getReservedSize(void);
extern UINT32 nand_getReservedStartBlockIndex(void);

extern UINT32 nand_readwriteCheckFAT(INT8 *pBuf);
extern UINT32 nand_readwriteCheckAll(INT8 *pBuf, UINT32 uistart_block);

extern void nand_setSpare(UINT32 spare0,UINT32 spare1, UINT32 spare2, UINT32 spare3);
#endif // _NAND_H


