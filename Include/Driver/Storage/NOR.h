#ifndef _NORDEF_H
#define _NORDEF_H

#include "Strgdef.h"

#define NOR_BOOT_TOP            1
#define NOR_BOOT_BOTTOM         -1
#define NOR_BASE_ADDRESS        0x20000000
#define NOR_HIGH_ADDR_PROBLEM   1


typedef struct
{
    UINT32  ulTotalSize;                   //How many bytes the memory device has.
    UINT32  ulNumberOfSector;              //How many sectors the memory device have (minimum earsed unit).
    UINT32  ulSectorSize;                  //How many bytes the sector has. (Minimum erase unit)
    UINT32  ulBlockSize;                   //How many bytes a block has. (Minimum PStore access unit)
    signed char   cBootDirection;          //Boot sector is located in the top sector or bottom sector (1 for boot from top, -1 for boot from bottom).
    UINT8   ucSiliconID ;
    UINT8   ucDeviceID;
}NORSTRG, *PNORSTRG;

extern PSTRG_TAB nor_getStorageObject(void);

extern BOOL nor_getDeviceInfo(PNORSTRG pNorInfo);
extern BOOL nor_eraseSector(UINT32 SectorAddr);
extern BOOL nor_programSector(UINT32 sector_addr, UINT32 sector_size, UINT8 *data);
extern BOOL nor_readByBytes(UINT32 addr, UINT32 size, UINT8 *data);
extern ER   nor_open(void);
extern ER   nor_close(void);
extern BOOL nor_eraseChip(void);
extern void nor_setGPIOForCS(UINT32 ulGPIO);
//For storage object
ER nor_writeSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
ER nor_readSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
ER nor_format(void);

extern void nor_printerRetry(UINT32* ulE, UINT32* ulP, UINT32* ulR);
extern void nor_ClearRetry(void);


#endif
