/*
    @file       SPIFLASH.c
    @ingroup    mISPIFLASH

    @brief      SPI Flash driver

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include <String.h>
#include "spiflash_int.h"
#include "Debug.h"
#include "Strgdef.h"
#include "ErrorNo.h"
#include "spi.h"
#include "nor.h"
#include "cache.h"

static STRG_TAB gSpiFlashObj = {NULL, NULL, nor_open, NULL, NULL, NULL, nor_close, NULL, NULL, NULL, NULL, NULL, NULL,NULL};

static SPI_FLASH_INFO vSpiFlashTable[] = {
    // ESMT F25L016A (2MB)
    {   FLASH_ESMT_ID,      0x20,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    TRUE,   TRUE,
        30000,              2000,   300,    1
    },
    // ESMT F25L032PA (4MB)
    {   FLASH_ESMT_ID,      0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    TRUE,   FALSE,
        100000,             2000,   300,    5
    },
    // Winbond W25X16 (2MB)
    {   FLASH_WINBOND_ID,   0x30,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        40000,              2000,   300,    3
    },
    // Winbond W25X32 (4MB)
    {   FLASH_WINBOND_ID,   0x30,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        80000,              2000,   300,    3
    },
    // Winbond W25X64 (8MB)
    {   FLASH_WINBOND_ID,   0x30,   0x17,
        0x800000,           0x1000, 0x1000, 0x800000/0x1000,    FALSE,   FALSE,
        100000,             1000,   200,    3
    },
    // Winbond W25Q16 (2MB)
    {   FLASH_WINBOND_ID,   0x40,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        40000,              1500,   200,    3
    },
    // Winbond W25Q32 (4MB)
    {   FLASH_WINBOND_ID,   0x40,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        80000,              1500,   200,    3
    },
    // MXIC MX25L1605D (2MB)
    {   FLASH_MXIC_ID,      0x20,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        30000,              2000,   300,    5
    },
    // MXIC MX25L3205D (4MB)
    {   FLASH_MXIC_ID,      0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        50000,              2000,   300,    5
    },
    // MXIC MX25L6405D (8MB)
    {   FLASH_MXIC_ID,      0x20,   0x17,
        0x800000,           0x1000, 0x1000, 0x800000/0x1000,    FALSE,   FALSE,
        80000,              2000,   300,    5
    },
	//#NT#2010/03/18#SA4 -begin
	// EON EN25F16 (2MB)
    {   FLASH_EON_ID,       0x31,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        50000,              2000,   300,    5
    },
    //#NT#2012/08/28#HH Chuang -begin
    //#NT#Support new spi flash
    // EON EN25QH16 (2MB)
    {   FLASH_EON_ID,       0x70,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        30000,              2000,   300,    5
    },
    //#NT#2012/08/28#HH Chuang -end
    // EON EN25F32 (4MB)
    {   FLASH_EON_ID,       0x20,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        50000,              2000,   300,    5
    },
    //#NT#2012/08/28#HH Chuang -begin
    //#NT#Support new spi flash
    // EON EN25QH32 (4MB)
    {   FLASH_EON_ID,       0x70,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        50000,              2000,   300,    5
    },
    //#NT#2012/08/28#HH Chuang -end
    // GigaDevice GD25Q16 (2MB)
    {   FLASH_GD_ID,        0x40,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        30000,              2000,   300,    5
    },
//#NT#2010/03/18#SA4 -end
    //#NT#2012/08/28#HH Chuang -begin
    //#NT#Support new spi flash
    // NS N25S16 (2MB)
    {
        FLASH_NS_ID,        0x30,   0x15,
        0x200000,           0x1000, 0x1000, 0x200000/0x1000,    FALSE,   FALSE,
        25000,              1000,   200,    5
    },
    // NS N25S32 (4MB)
    {   FLASH_EON_ID,       0x30,   0x16,
        0x400000,           0x1000, 0x1000, 0x400000/0x1000,    FALSE,   FALSE,
        60000,              2000,   200,    5
    },
    //#NT#2012/08/28#HH Chuang -end
};

static BOOL bIsOpened = FALSE;
static UINT32 uiSpiFlashIdx = 0;

static ER_SPI nor_waitReady(UINT32 uiWaitMs);
static void nor_readData(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8 *pBuf);
static BOOL nor_pageProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf);
static BOOL nor_aaiProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf);
static void nor_enableWriteLatch(void);
static void nor_disableWriteLatch(void);
static ER   nor_unlock(void);
static ER   nor_lock(void);
static void nor_readID(UINT32 *puiMfgID, UINT32 *puiDevID);
static void nor_readESMTSignature(UINT32 *puiSignID);

/**
    @addtogroup mISPIFLASH
*/
//@{

/**
    @name   SPI Flash Driver API
*/
//@{


/**
    Open NOR flash driver

    @return
        - @b E_OK: open success
        - @b Else: open fail
*/
ER nor_open(void)
{
//    UINT32 uiMfgID, uiDevID;
    NORSTRG norStrg;
    ER erReturn;

    if ((erReturn = nor_lock()) != E_OK)
    {
        return erReturn;
    }

    if (spi_open() != E_OK)
    {
        return E_SYS;
    }

    bIsOpened = TRUE;

    if (nor_getDeviceInfo(&norStrg) == FALSE)
    {
        bIsOpened = FALSE;
        return E_SYS;
    }

    return E_OK;
}

/**
    Close NOR flash driver

    @return
        - @b E_OK: open success
        - @b Else: open fail
*/
ER nor_close(void)
{
    ER erReturn;

    if(nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime) != FLASH_STATUS_READY)
    {
        debug_err(("SPI flash: close: wait ready timeout\r\n"));
    }

    bIsOpened = FALSE;

    if ((erReturn = spi_close()) != E_OK)
    {
        return erReturn;
    }
    nor_unlock();

    return E_OK;
}

/**
    Lock SPI module

    @return
        - @b E_OK: lock success
        - @b Else: lock fail
*/
ER nor_lock(void)
{
//    return = wai_sem(SEMID_SPI);
    return E_OK;
}

/**
    Unlock SPI module

    @return
        - @b E_OK: lock success
        - @b Else: lock fail
*/
ER nor_unlock(void)
{
 //   return = sig_sem(SEMID_SPI);
    return E_OK;
}

/**
    Get Storage object

    Get storage object of SPI flash driver.

    @return pointer to storage object
*/
PSTRG_TAB nor_getStorageObject(void)
{
    return &gSpiFlashObj;
}

/**
    Get device information

    @param[out] pNorInfo    pointer point to PNORSTRG

    @return
        - @b TRUE: get information success
        - @b FALSE: get information fail, maybe flash ID not match
*/
BOOL nor_getDeviceInfo(PNORSTRG pNorInfo)
{
    UINT32  i;
    UINT32  uiMfgID, uiDevID;
    UINT32  uiSignID;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: get devInfo: driver not opened\r\n"));
        return FALSE;
    }

    nor_readID(&uiMfgID, &uiDevID);

    for (i=0; i<sizeof(vSpiFlashTable)/sizeof(vSpiFlashTable[0]); i++)
    {
        if ((vSpiFlashTable[i].uiMfgID == uiMfgID) &&
            (vSpiFlashTable[i].uiMemCapacity == uiDevID))
        {
            pNorInfo->ulSectorSize = vSpiFlashTable[i].uiSectorSize;
            pNorInfo->ulNumberOfSector = vSpiFlashTable[i].uiSectorCnt;
            pNorInfo->ulTotalSize  = vSpiFlashTable[i].uiTotalSize;
            pNorInfo->ulBlockSize = vSpiFlashTable[i].uiBlockSize;

            pNorInfo->cBootDirection = NOR_BOOT_BOTTOM;
            pNorInfo->ucDeviceID = uiDevID;
            pNorInfo->ucSiliconID = uiMfgID;

            //#NT#2012/10/24#Philex Lin-begin
            // read EMST Electronic signature ID
            if (uiMfgID==FLASH_ESMT_ID&&uiDevID==0x15)
            {
                nor_readESMTSignature(&uiSignID);
                if (uiSignID!=0x14)
                {
                  // EMST flash support page programming and don't need AAI program
                  vSpiFlashTable[i].bSupportAAI = FALSE;
                  vSpiFlashTable[i].bSupportEWSR = FALSE;
                }
            }
            //#NT#2012/10/24#Philex Lin-end

            uiSpiFlashIdx = i;
    debug_err(("SPI flash: get devInfo MFG ID:0x%lx, Device ID:0x%lx, uiSpiFlashIdx : %d\r\n", uiMfgID, uiDevID,uiSpiFlashIdx));
            return TRUE;
        }
    }

    debug_err(("SPI flash: get devInfo: Manufacture ID dosen't support. MFG ID:0x%lx, Device ID:0x%lx\r\n", uiMfgID, uiDevID));
    return FALSE;
}

/**
    Erase whole chip

    Erase whole content of nor flash to 0xFF.

    @return
        - @b TRUE: erase success
        - @b FALSE: erase fail
*/
BOOL nor_eraseChip(void)
{
    ER_SPI erReturn;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: eraseChip: driver not opened\r\n"));
        return FALSE;
    }

    if(nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime) != FLASH_STATUS_READY)
    {
        debug_err(("SPI flash: eraseChip: wait flash ready timeout\r\n"));
        return FALSE;
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(FLASH_WINBOND_CMD_CHIP_ERASE);
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime);
    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        debug_err(("SPI flash: eraseChip: timeout after erase\r\n"));
        return FALSE;
    }
}

/**
    Erase a sector

    Erase a sector of nor flash.
    In Winbond W25X16/32/64 series, size of one sector is 0x1000 bytes.

    @param[in] uiByteAddr   Byte address of erased sector. Should be 0x1000 alignment.

    @return
        - @b TRUE: erase success
        - @b FALSE: erase fail
*/
BOOL nor_eraseSector(UINT32 uiByteAddr)
{
    ER_SPI erReturn;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: eraseSector: driver not opened\r\n"));
        return FALSE;
    }

    //Check paprameter
    if((uiByteAddr % vSpiFlashTable[uiSpiFlashIdx].uiSectorSize) != 0)
    {
        debug_err(("SPI flash: eraseSector: byte address boundary error 0x%lx\r\n", uiByteAddr));
        return FALSE;
    }
    if(nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiChipEraseTime) != FLASH_STATUS_READY)
    {
        debug_err(("SPI flash: eraseSector: wait ready timeout\r\n"));
        return FALSE;
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeSingle((FLASH_WINBOND_CMD_SECTOR_ERASE<<24) | (uiByteAddr&0xFFFFFF));
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiSectorEraseTime);
    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        debug_err(("SPI flash: eraseSector: erase timeout\r\n"));
        return FALSE;
    }
}

/**
    Program a sector

    Program a sector of nor flash.
    In Winbond W25X16/32/64 series, size of one sector is 0x1000 bytes.

    @param[in] uiByteAddr   Byte address of programed sector. Should be 0x1000 alignment.
    @param[in] uiSectorSize Byte size of one secotr. Should be 0x1000.
    @param[in] pBuf         pointer point to written data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail
*/
BOOL nor_programSector(UINT32 uiByteAddr, UINT32 uiSectorSize, UINT8* pBuf)
{
    ER_SPI erReturn;
    UINT32 uiProgramCycle, uiCycleIndex;
    UINT32 uiAddrIndex;

    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: program: driver not opened\r\n"));
        return FALSE;
    }

    if (vSpiFlashTable[uiSpiFlashIdx].bSupportAAI == TRUE)
    {
        return nor_aaiProgram(uiByteAddr, uiSectorSize, pBuf);
    }
    else
    {
        uiProgramCycle = uiSectorSize / SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE;

        uiAddrIndex = uiByteAddr;
        for(uiCycleIndex = 0; uiCycleIndex < uiProgramCycle; uiCycleIndex++, uiAddrIndex += SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)
        {
            erReturn = nor_pageProgram(uiAddrIndex, SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE, (pBuf + (uiCycleIndex * SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)));
            if (!erReturn)
            {
                debug_err(("SPI flash: program: fail at flash address 0x%lx\r\n", uiAddrIndex));
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**
    Read NOR flash data

    Read data from NOR flash.

    @param[in] uiByteAddr   Byte address of flash.
    @param[in] uiByteSize   Byte size of read data.
    @param[out] pBuf        Pointer point to store read data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail
*/
BOOL nor_readByBytes(UINT32 uiByteAddr, UINT32 uiByteSize, UINT8 *pBuf)
{
    if (bIsOpened == FALSE)
    {
        debug_err(("SPI flash: read: driver not opened\r\n"));
        return FALSE;
    }

    nor_readData(uiByteAddr, uiByteSize, pBuf);

    return TRUE;
}

//@}

/*
    @name   SPI Flash Driver Internal API
*/
//@{

/*
    Read SPI flash ID

    Read manufacture ID and device ID

    @param[out] puiMfgID    Returned manufacture ID
    @param[out] puiDevID    Returned device ID

    @return void
*/
static void nor_readID(UINT32 *puiMfgID, UINT32 *puiDevID)
{
    UINT32 uiId;

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeReadSingle(FLASH_WINBOND_CMD_RD_JEDEC_ID<<24, &uiId);

    spi_setCSActive(FALSE);

    *puiMfgID = (uiId >> 16) & 0xFF; // Manufacture ID
    *puiDevID = uiId & 0xFF;         // Device ID
}

/*
    Read ESMT SPI Electronic Signature ID

    Read ESMT SPI Electronic Signature ID

    @param[out] puiSignID    Returned manufacture ID

    @return void
*/
static void nor_readESMTSignature(UINT32 *puiSignID)
{
    UINT32 uiId;

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeReadSingle(FLASH_WINBOND_CMD_POWER_UP<<24, &uiId);

    spi_setCSActive(FALSE);

    *puiSignID = uiId & 0xFF;         // EMST electronic Signature ID
}

/*
    Wait SPI flash ready

    Wait SPI flash returned to ready state

    @param[in] uiWaitMs     Timeout setting. (Unit: ms)

    @return
        - @b FLASH_STATUS_READY: success
        - @b FLASH_STATUS_BUSY: fail. Maybe timeout.
*/
static ER_SPI nor_waitReady(UINT32 uiWaitMs)
{
    ER erReturn;

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_enBitMatch(FLASH_WINBOND_CMD_RDSR, 0, FALSE);
    erReturn = spi_waitBitMatch(uiWaitMs);
    spi_setCSActive(FALSE);

    if (erReturn == E_OK)
    {
        return FLASH_STATUS_READY;
    }
    else
    {
        return FLASH_STATUS_BUSY;
    }
}

static void nor_flushReadCache(UINT32 uiStartAddr, UINT32 uiLength)
{
    UINT32 uiPart1Start;
    UINT32 uiPart2Start;
    UINT32 uiPart3Start, uiPart3End;

    //
    //  ex:
    //          Part1_Start word[2]
    //          Part1_End   word[3]
    //          Part2_Start word[4]     <cache line alignment>
    //                      word[5]
    //                      ...
    //          Part2_End   word[4*N - 1]
    //          Part3_Start word[4*N]   <cache line alignment>
    //                      word[4*N + 1]
    //          Part3_End   word[4*N + 2]
    //
    //  part1 and part2 are not cache line alignment.
    //

    // check if start DRAM address is not cache line alignment
    uiPart1Start = (UINT32)uiStartAddr;
    uiPart2Start = (UINT32)uiStartAddr;
    if ((uiPart1Start % 16) != 0)           // one cache line has 4 word
    {
        uiPart2Start = (uiPart1Start + 15) & (~15);
        CPUCleanInvalidateDCache((UINT32)uiStartAddr);
    }
    // check if part 2 exist
    uiPart3End = ((UINT32)uiStartAddr) + uiLength - 1;
    uiPart3Start = (uiPart3End/16) * 16;
    if (uiPart2Start != uiPart3Start)
    {
        CPUInvalidateDCacheBlock(uiPart2Start, uiPart3Start-1);
    }
    // check if end DRAM address is not cache line alignment
    if (uiPart3Start != uiPart3End)
    {
        CPUCleanInvalidateDCache(uiPart3Start);
    }

    CPUDrainWriteBuffer();
}

static void nor_flushWriteCache(UINT32 uiStartAddr, UINT32 uiLength)
{
    CPUCleanInvalidateDCacheBlock(uiStartAddr, uiStartAddr+uiLength);
    CPUDrainWriteBuffer();
}

/*
    Read SPI flash

    @param[in] uiSPIADDR        Byte address of SPI flash
    @param[in] uiByteSize       Byte count of read data. Should be word alignment (i.e. multiple of 4).
    @param[out] pBuf            Pointer point to store read data

    @return void
*/
static void nor_readData(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8 *pBuf)
{
    UINT32 i, uiWord;
    UINT32 uiCount;
    UINT32 uiBufOffset = 0;

    if (CPUChkDCacheEnabled(uiSPIADDR) == TRUE)
    {
        nor_flushReadCache((UINT32)pBuf, uiByteSize);
    }

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeSingle((FLASH_WINBOND_CMD_READ<<24) | (uiSPIADDR&0xFFFFFF));
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    while (uiByteSize != 0)
    {
        if ((uiByteSize/4) > ((SPI_MAX_TRANSFER_CNT>>2)<<2))
        {
            uiCount = (SPI_MAX_TRANSFER_CNT>>2) << 2;

            spi_writeReadData(uiCount, NULL, (UINT32*)(&pBuf[uiBufOffset]), TRUE);
            spi_waitDataDone();

            uiByteSize -= uiCount * 4;
            uiBufOffset += uiCount * 4;
        }
        else
        {
            uiCount = (uiByteSize / 16) * 4;

            if (uiCount != 0)
            {
                spi_writeReadData(uiCount, NULL, (UINT32*)(&pBuf[uiBufOffset]), TRUE);
                spi_waitDataDone();
            }
            if (uiByteSize % 16)
            {
                for (i=0; i<(uiByteSize % 16); i++)
                {
                    spi_readSingle(&uiWord);
                    pBuf[uiBufOffset+uiCount*4+i] = uiWord & 0xFF;
                }
            }

            uiByteSize = 0;
        }
#if 0
        spi_writeReadData(uiCount, NULL, (UINT32*)(&pBuf[uiBufOffset]), TRUE);
        spi_waitDataDone();

        uiByteSize -= uiCount * 4;
        uiBufOffset += uiCount * 4;
#endif
    }

    spi_setCSActive(FALSE);
}

/*
    Page program

    Program a page of SPI flash

    @param[in] uiSPIADDR        Byte address of SPI flash
    @param[in] uiByteSize       Byte count of written data. Should be <= 256 and word alignment (i.e. multiple of 4).
    @param[out] pBuf            Pointer point to store read data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail. Maybe timeout.
*/
static BOOL nor_pageProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf)
{
    ER_SPI erReturn;

    if (CPUChkDCacheEnabled(uiSPIADDR) == TRUE)
    {
        nor_flushWriteCache((UINT32)pBuf, uiByteSize);
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeSingle((FLASH_WINBOND_CMD_PAGEPROG<<24) | (uiSPIADDR&0xFFFFFF));

    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeReadData(uiByteSize/4, (UINT32*)pBuf, NULL, TRUE);
    spi_waitDataDone();
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiPageProgramTime);
    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
    AAI single word program

    Program single word of SPI flash

    @param[in] uiSPIADDR        Byte address of SPI flash
    @param[in] uiByteSize       Byte count of written data. Should be <= 256 and word alignment (i.e. multiple of 4).
    @param[out] pBuf            Pointer point to store read data

    @return
        - @b TRUE: program success
        - @b FALSE: program fail. Maybe timeout.
*/
static BOOL nor_aaiProgram(UINT32 uiSPIADDR, UINT32 uiByteSize, UINT8* pBuf)
{
    UINT32 i, uiWord;
    ER_SPI erReturn;

    if (uiByteSize == 0)
    {
        return FALSE;
    }

    nor_enableWriteLatch();

    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_4BYTES);
    spi_writeSingle((SPI_FLASH_CMD_AAI<<24) | (uiSPIADDR&0xFFFFFF));

    spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
    uiWord = (pBuf[0] << 8) | pBuf[1];
    spi_writeSingle(uiWord);
    spi_setCSActive(FALSE);

    erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiPageProgramTime);

    for (i=2; i<uiByteSize; i+=2)
    {
        spi_setCSActive(TRUE);

        spi_setTransferLen(SPI_TRANSFER_LEN_3BYTES);
        uiWord = (SPI_FLASH_CMD_AAI << 16) | (pBuf[i] << 8) | pBuf[i+1];
        spi_writeSingle(uiWord);

        spi_setCSActive(FALSE);

        erReturn = nor_waitReady(vSpiFlashTable[uiSpiFlashIdx].uiPageProgramTime);
    }

    nor_disableWriteLatch();

    if ((erReturn == FLASH_STATUS_DONE) || (erReturn == FLASH_STATUS_READY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void nor_enableWriteLatch(void)
{
    UINT32 uiWord;

    if (vSpiFlashTable[uiSpiFlashIdx].bSupportEWSR == TRUE)
    {
        spi_setCSActive(TRUE);
        spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
        spi_writeReadSingle((FLASH_WINBOND_CMD_RDSR<<8) | (0xFF), &uiWord);
        spi_setCSActive(FALSE);

        uiWord &= ~(FLASH_STATUS_WP_BITS | FLASH_STATUS_WEL_BITS);

        spi_setCSActive(TRUE);

        spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
        spi_writeSingle(SPI_FLASH_CMD_EWSR);

        spi_setCSActive(FALSE);

        spi_setCSActive(TRUE);
        spi_setTransferLen(SPI_TRANSFER_LEN_2BYTES);
        spi_writeSingle((FLASH_WINBOND_CMD_WRSR<<8) | (uiWord&0xFF));
        spi_setCSActive(FALSE);
    }

    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(FLASH_WINBOND_CMD_WREN);

    spi_setCSActive(FALSE);
}

static void nor_disableWriteLatch(void)
{
    spi_setCSActive(TRUE);

    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(FLASH_WINBOND_CMD_WRDI);

    spi_setCSActive(FALSE);
}

//@}

//@}

