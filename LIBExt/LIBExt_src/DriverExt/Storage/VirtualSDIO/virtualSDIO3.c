/*
    Virtual SDIO3 Card driver

    @file       virtualSDIO3.c
    @ingroup    mICardSDIO

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include "sdio.h"
#include "StrgDef.h"
#include "ErrorNO.h"
#include "OSCommon.h"
#include "virtualSDIO.h"
#include <string.h>

// SD file system format definition
#define VSDIO_FMT_SYSID_01_SECTOR_CNT       (32680)
#define VSDIO_FMT_SYSID_04_SECTOR_CNT       (65536)
#define VSDIO_FMT_SYSID_0B_SECTOR_CNT       (8422686720LLU/512)

#define VSDIO_MBR_MAX_HEAD                  (0xFE)
#define VSDIO_MBR_MAX_SECTOR                (63)
#define VSDIO_MBR_MAX_CYLINDER              (1023)

#define VSDIO_FMT_2MB_SECTOR_CNT            (2UL*1024*1024/512)
#define VSDIO_FMT_8MB_SECTOR_CNT            (8UL*1024*1024/512)
#define VSDIO_FMT_16MB_SECTOR_CNT           (16UL*1024*1024/512)
#define VSDIO_FMT_32MB_SECTOR_CNT           (32UL*1024*1024/512)
#define VSDIO_FMT_64MB_SECTOR_CNT           (64UL*1024*1024/512)
#define VSDIO_FMT_128MB_SECTOR_CNT          (128UL*1024*1024/512)
#define VSDIO_FMT_256MB_SECTOR_CNT          (256UL*1024*1024/512)
#define VSDIO_FMT_504MB_SECTOR_CNT          (504UL*1024*1024/512)
#define VSDIO_FMT_1008MB_SECTOR_CNT         (1008UL*1024*1024/512)
#define VSDIO_FMT_1GB_SECTOR_CNT            (1UL*1024*1024*1024/512)
#define VSDIO_FMT_2016MB_SECTOR_CNT         (2016UL*1024*1024/512)
#define VSDIO_FMT_2GB_SECTOR_CNT            (2UL*1024*1024*1024/512)
#define VSDIO_FMT_4032MB_SECTOR_CNT         (4032UL*1024*1024/512)
#define VSDIO_FMT_8032_5MB_SECTOR_CNT       (8422686720ULL/512)

#define MBR_FAT12                           0x01
#define MBR_FAT16                           0x06
#define MBR_FAT32                           0x0C
#define FSI_LEADSIG_VAL                     0x41615252
#define FSI_STRUCSIG_VAL                    0x61417272
#define FSI_LEADSIG                         0
#define FSI_STRUCSIG                        484
#define FSI_FREE_COUNT                      488
#define FSI_NXT_FREE                        492

//first partition's structure in MBR sector
#define _0x1BE                              446     // 0x1BE



//
//  Internal function prototypes
//
static ER virtualSdio3_openMemBus(void);
static ER virtualSdio3_closeMemBus(void);
static ER virtualSdio3_writeSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt);
static ER virtualSdio3_readSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt);
static ER virtualSdio3_format(void);

//
//  Private driver specific variables
//
//static BOOL bIsAttached = FALSE;
static BOOL bIsLocked = FALSE;
static BOOL bIsOpened = FALSE;
static STRG_CAP gSDIO_SdmmcCap;
static STRG_TAB gSDIOObj = {STORAGE_DETACH|STORAGE_SDIO, &gSDIO_SdmmcCap, virtualSdio3_open, virtualSdio3_writeSectors, virtualSdio3_readSectors, virtualSdio3_format, virtualSdio3_close, virtualSdio3_openMemBus, virtualSdio3_closeMemBus, NULL, NULL, NULL, NULL, NULL};
//static SDMMC_INFO_STRUCT sdInfo;

static SDIO_ID gSdioID;
static PSTRG_TAB pSdioStrg = NULL;
static UINT32 guiSdioBaseLBA = 0;
static UINT32 guiSdioSecCnt = 0;

static INT8 strFormat_OemID[9] = "SYSTEMID";
static INT8 strFormat_VolumeLabel[12] = "VOLUME1    ";
static BOOL bFormat_RandVolID = TRUE;
static UINT32 uiFormat_VolID = 0;


/**
    @addtogroup mICardSDIO
*/
//@{

/**
    @name   Virtual SDIO Driver Public OS level API
*/
//@{

/**
    Open Virtual SDIO driver Init Mem card

    This function will initialize software/hardware states and make SD card enters
    transfer state.

    @return
        - @b E_OK: open sucess
        - @b E_PAR: driver internal parameter error, maybe linking error or DRAM error
        - @b E_SYS: driver is already opened OR other fail
*/
ER virtualSdio3_open(void)
{
    BOOL bOpen;
    ER erReturn;

    if (pSdioStrg)
    {
        if (gSdioID == SDIO_ID_1)
        {
            bOpen = sdio_isOpened();
        }
        else
        {
            bOpen = sdio2_isOpened();
        }
        if (bOpen == TRUE)
        {
            return E_OK;
        }

        erReturn = pSdioStrg->Open();
        gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;
        gSDIO_SdmmcCap.uiBytesPerSector = 512;
        if ((guiSdioBaseLBA+guiSdioSecCnt) > pSdioStrg->pStrgCap->uiTotalSectors)
        {
            // resize
            gSDIO_SdmmcCap.uiTotalSectors = guiSdioSecCnt
                        = pSdioStrg->pStrgCap->uiTotalSectors - guiSdioBaseLBA;
            debug_wrn(("^R%s: resize virtual storage to %d sectors\r\n", __func__, guiSdioSecCnt));
        }
        else
        {
            gSDIO_SdmmcCap.uiTotalSectors = guiSdioSecCnt;
        }
        if (guiSdioSecCnt <= VSDIO_FMT_2MB_SECTOR_CNT)          // 2MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 2;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 16;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_16MB_SECTOR_CNT)    // 16MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 2;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 32;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_32MB_SECTOR_CNT)    // 32MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 4;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 32;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_128MB_SECTOR_CNT)   // 128MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 8;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 32;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_256MB_SECTOR_CNT)   // 256MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 16;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 32;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_504MB_SECTOR_CNT)   // 504MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 16;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 63;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_1008MB_SECTOR_CNT)  // 1008MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 32;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 63;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_2016MB_SECTOR_CNT)  // 2016MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 64;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 63;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_4032MB_SECTOR_CNT) // 4032MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 128;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 63;
        }
        else //if (guiSdioSecCnt <= (32768*1024*1024/512))// 32768MB
        {
            gSDIO_SdmmcCap.uiTotalHeads = 255;
            gSDIO_SdmmcCap.uiSectorsPerTrack = 63;
        }
        gSDIO_SdmmcCap.uiTotalCylinders = ((guiSdioSecCnt-1) / (gSDIO_SdmmcCap.uiTotalHeads * gSDIO_SdmmcCap.uiSectorsPerTrack));
        if (erReturn != E_OK)
        {
            return erReturn;
        }

        return E_OK;
    }

    return E_SYS;
}

/**
    Close Virtual SDIO driver access.

    This function will make SD card enters idle state.
    And disable SD clock, CKGEN clock.

    @return
        - @b E_OK: close success
        - @b E_SYS: driver is not opened
*/
ER virtualSdio3_close(void)
{
    ER erReturn;

    if (pSdioStrg)
    {
        //HH: need to check if sdio is opened by another virtual driver?
        erReturn = pSdioStrg->Close();
        gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;
        if (erReturn != E_OK)
        {
            return erReturn;
        }
        return E_OK;
    }

    return E_SYS;
}

/**
    Query Virtual SDIO driver opened status.

    This function is used to return whether SDIO driver hardware is
    currently opened for operation or not.

    @return
        - @b TRUE: SDIO driver is opened
        - @b FALSE: SDIO driver is not opened
*/
BOOL virtualSdio3_isOpened(void)
{
    return bIsOpened;
}

/**
    Lock Virtual SDIO driver resource.

    @return
        - @b E_OK: lock success
        - @b Else: lock fail
*/
ER virtualSdio3_lock(void)
{
    ER erReturn;

    erReturn = wai_sem(SEMID_VIRTUAL_SDIO3);

    bIsLocked = TRUE;

    return erReturn;
}

/**
    Unlock Virtual SDIO driver resource.

    @return
        - @b E_OK: unlock success
        - @b Else: unlock fail
*/
ER virtualSdio3_unlock(void)
{

    if(bIsLocked)
    {
        bIsLocked = FALSE;
        return sig_sem(SEMID_VIRTUAL_SDIO3);
    }

    return E_OK;
}

//@}


/**
    @name   Virtual SDIO Driver Public driver specific operating API
*/
//@{

/*
    Open Virtual SDIO driver access.

    This function will re-active pinmux for SDIO.
    And it will make SD card re-enters transfer state.

    @note
    This function is designed to co-operate with sdio_closeMemBus(). \n
    In a multi-storage environment, upper layer (maybe file system) may \n
    need to work on 2 (or more) storage devices that some signals are pinmux \n
    together. If upper layer need to operate on STORAGE_A, STORAGE_B, and then \n
    re operate on STORAGE_A. The flow should be: \n
        - STORAGE_A->closeMemBus();
        - STORAGE_B->openMemBus();
        - STORAGE_B->someOp();
        - STORAGE_B->closeMemBus();
        - STORAGE_A->openMemBus();
        - STORAGE_A->someOtherOp();

    @return
        - @b E_OK: open mem bus sucess
        - @b E_SYS: open mem bus fail
*/
static ER virtualSdio3_openMemBus(void)
{
    ER erReturn;

    if (pSdioStrg == NULL)
    {
        return E_SYS;
    }

    virtualSdio_lock();

    erReturn = pSdioStrg->OpenMemBus();
    gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;

    virtualSdio_unlock();

    return erReturn;
}

/*
    Close Virtual SDIO driver access.

    This function will make SD card enters standby state.
    And it will disable interrupt and CKGEN clock.

    @note
    This function is designed to co-operate with sdio_openMemBus(). \n
    In a multi-storage environment, upper layer (maybe file system) may \n
    need to work on 2 (or more) storage devices that some signals are pinmux \n
    together. If upper layer need to operate on STORAGE_A, STORAGE_B, and then \n
    re operate on STORAGE_A. The flow should be: \n
        - STORAGE_A->closeMemBus();
        - STORAGE_B->openMemBus();
        - STORAGE_B->someOp();
        - STORAGE_B->closeMemBus();
        - STORAGE_A->openMemBus();
        - STORAGE_A->someOtherOp();

    @return
        - @b E_OK: close mem bus success
        - @b E_SYS: close mem bus fail
*/
static ER virtualSdio3_closeMemBus(void)
{
    ER erReturn;

    if (pSdioStrg == NULL)
    {
        return E_SYS;
    }

    virtualSdio3_lock();

    erReturn = pSdioStrg->CloseMemBus();
    gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;

    virtualSdio3_unlock();

    return erReturn;
}

/*
    Write SD card sectors from SDIO interface

    Processing of writing N sector onto SD/MMC

    @param[in] pcBuf    src data addr (DRAM address)
    @param[in] ulLBAddr which Sector in the current LUN is specified
    @param[in] ulSctCnt the nums of sector data will be write

    @return
        - @b E_OK: write success
        - @b E_SYS: driver is not opened OR driver status error OR write encounting error
*/
static ER virtualSdio3_writeSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt)
{
    ER erReturn;

    if (pSdioStrg == NULL)
    {
        return E_SYS;
    }

    // Check boundary
    if ((ulLBAddr + ulSctCnt) > pSdioStrg->pStrgCap->uiTotalSectors)
    {
        return E_SYS;
    }

    virtualSdio3_lock();

    erReturn = pSdioStrg->WrSectors(pcBuf, ulLBAddr+guiSdioBaseLBA, ulSctCnt);
    gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;

    virtualSdio3_unlock();

    return erReturn;
}

/*
    Read SD card sectors from SDIO interface

    Processing of reading N sector from SD/MMC

    @param[out] pcBuf   dst data addr (DRAM address)
    @param[in] ulLBAddr which Sector in the current LUN is specified
    @param[in] ulSctCnt the nums of sector data will be write

    @return
        - @b E_OK: read success
        - @b E_SYS: driver is not opened OR driver status error OR read encounting error
*/
static ER virtualSdio3_readSectors(INT8 *pcBuf, UINT32 ulLBAddr, UINT32 ulSctCnt)
{
    ER erReturn;

    if (pSdioStrg == NULL)
    {
        return E_SYS;
    }

    // Check boundary
    if ((ulLBAddr + ulSctCnt) > pSdioStrg->pStrgCap->uiTotalSectors)
    {
        return E_SYS;
    }

    virtualSdio3_lock();

    erReturn = pSdioStrg->RdSectors(pcBuf, ulLBAddr+guiSdioBaseLBA, ulSctCnt);
    gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;

    virtualSdio3_unlock();

    return erReturn;
}

/*
    Format SDIO card disk.

    @return
        - @b E_OK: format success
        - @b E_SYS: driver is not opened OR write sector encounting error
*/
static ER virtualSdio3_format(void)
{
    ER      erReturn = E_SYS;
    UINT32  i, uiSectorPerFat = 0,uiRootDirSec,uiWriteSec,uiBufSize;
    UINT32  uiTotalWrSec =0,uiWrSec =0; // for format callback
    UINT32  uiFatBits;                  // bits per FAT entry
    UINT32  uiSysAreaSize;              // sector count of system area
    UINT32  uiBuSize;                   // size of boundry unit
    UINT32  uiMaxCluster;               // max cluster number
    UINT32  uiSectorPerFatPrime;        // sector per FAT SF' used in parameter computation
    UINT32  uiFatType;
    UINT32  usStartHead, usStartSector, usStartCylinder, usEndHead, usEndSector, usEndCylinder;
    UINT32  uiTotalCluster;
    UINT32  uiSectorPerCluster, uiSectorPerTrack, uiReserveSector, uiRelativeSector;
    UINT32  uiHeadNum;
    UINT32  uiSystemId = 0;
    INT8    *pucSecBuf, *pdata;
    const INT8  tmpFAT16[]  ={"FAT16   "};
    const INT8  tmpFAT12[]  ={"FAT12   "};
    const INT8  tmpFAT32[]  ={"FAT32   "};

//    virtualSdio_lock();

    ///////////////////////
//    sdio_ind(("SDIO%d : Formatting...\r\n", id));
//    sdio_ind(("SDIO%d fmt: total sector = %d\r\n", id, vgSDIO_SdmmcInfo[id].uiTotalSector));

//    if (vpSDIOObj[id] == NULL)
//    {
//        debug_err(("SDIO%d fmt: storage object NULL, not initialized\r\n", id));
//        return E_SYS;
//    }
    if ((gSDIOObj.uiStrgStatus & ~STORAGE_CHANGED) != (STORAGE_SDIO | STORAGE_READY))
    {
        debug_err(("%s: card not exist or not ready: 0x%lx\r\n", __func__, gSDIOObj.uiStrgStatus));
        return E_SYS;
    }

    if (guiSdioSecCnt <= VSDIO_FMT_2GB_SECTOR_CNT)  // FAT12/FAT16
    {
        //
        // decide FAT type, system ID
        //
        if (guiSdioSecCnt <= VSDIO_FMT_8MB_SECTOR_CNT)
        {
            uiSectorPerCluster = 16;
            uiBuSize = 16;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_64MB_SECTOR_CNT)
        {
            uiSectorPerCluster = 32;
            uiBuSize = 32;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_256MB_SECTOR_CNT)
        {
            uiSectorPerCluster = 32;
            uiBuSize = 64;
        }
        else if (guiSdioSecCnt <= VSDIO_FMT_1GB_SECTOR_CNT)
        {
            uiSectorPerCluster = 32;
            uiBuSize = 128;
        }
        else
        {
            uiSectorPerCluster = 64;
            uiBuSize = 128;
        }

        if ((guiSdioSecCnt / uiSectorPerCluster) < 4085)
        {
            uiFatType = MBR_FAT12;
            uiFatBits = 12;
        }
        else
        {
            uiFatType = MBR_FAT16;
            uiFatBits = 16;
        }

        if (guiSdioSecCnt < VSDIO_FMT_SYSID_01_SECTOR_CNT)
        {
            uiSystemId = 0x01;
        }
        else if (guiSdioSecCnt < VSDIO_FMT_SYSID_04_SECTOR_CNT)
        {
            uiSystemId = 0x04;
        }
        else
        {
            uiSystemId = 0x06;
        }

        //
        // compute format parameters
        //

        // 1. sector per cluster (SC): uiSectorPerCluster
        // 2. number of root-directory entries (RDE): 512
        // 3. sector size (SS): 512
        // 4. reserved sector count (RSC): 1
        // 5. total sectors (TS): vgSDIO_SdmmcInfo.uiTotalSector
        // 6. FAT bits: uiFatBits
        uiReserveSector = 1;

        // 7. Sector per FAT: SF = ceil( TS/SC*FAT bits / (SS * 8) )
        if (((guiSdioSecCnt / uiSectorPerCluster * uiFatBits) % (8 * 512)) == 0)
        {
            uiSectorPerFat = (guiSdioSecCnt / uiSectorPerCluster * uiFatBits) / (8 * 512);
        }
        else
        {
            uiSectorPerFat = (guiSdioSecCnt / uiSectorPerCluster * uiFatBits) / (8 * 512) + 1;
        }
        debug_ind(("%s: SF = %d\r\n", __func__, uiSectorPerFat));

        do {
            // 8. number of sectors in the system area:
            //      SSA = RSC + 2*SF + ceil( 32*RDE / SS )
            uiSysAreaSize = 1 + 2*uiSectorPerFat + (32*512/512);
            debug_ind(("%s: SSA = %d\r\n", __func__, uiSysAreaSize));

            // 9. number of sectors in MBR:
            //      NOM + SSA = BU * n
            for (i=1; uiSysAreaSize > (i*uiBuSize); i++);
            uiRelativeSector = i*uiBuSize - uiSysAreaSize;
            debug_ind(("%s: NOM = %d\r\n", __func__, uiRelativeSector));

            // 10. If NOM isn't equal to BU, NOM is added BU
            if (uiRelativeSector != uiBuSize)
            {
                uiRelativeSector += uiBuSize;
            }
            debug_ind(("%s: NOM(2) = %d\r\n", __func__, uiRelativeSector));

            do
            {
                // 11. Max cluster number (MAX):
                //      MAX = ip ( (TS-NOM-SSA) / SC ) + 1
                uiMaxCluster = (guiSdioSecCnt - uiRelativeSector - uiSysAreaSize) / uiSectorPerCluster + 1;
                debug_ind(("%s: MAX = %d\r\n", __func__, uiMaxCluster));

                // 12. recalculate SF
                //      SF' = ceil { [2 + (MAX - 1)] * FAT bits / (SS * 8) }
                if ((((2 + (uiMaxCluster - 1)) * uiFatBits) % (512 * 8)) == 0)
                {
                    uiSectorPerFatPrime = (2 + (uiMaxCluster - 1)) * uiFatBits / (512 * 8);
                }
                else
                {
                    uiSectorPerFatPrime = (2 + (uiMaxCluster - 1)) * uiFatBits / (512 * 8) + 1;
                }
                debug_ind(("%s: SF' = %d\r\n", __func__, uiSectorPerFatPrime));

                // 13. If SF' is greater than SF, NOM is added BU.
                if (uiSectorPerFatPrime > uiSectorPerFat)
                {
                    uiRelativeSector += uiBuSize;
                    debug_ind(("%s: new NOM = %d\r\n", __func__, uiRelativeSector));
                }
            } while (uiSectorPerFatPrime > uiSectorPerFat);

            // 14. If SF' != SF, SF' is used as SF.
            if (uiSectorPerFatPrime != uiSectorPerFat)
            {
                uiSectorPerFat = uiSectorPerFatPrime;
                debug_ind(("%s: new SF = %d\r\n", __func__, uiSectorPerFat));
            }
            else
            {
                break;
            }
        } while (1);

        // total written sector: MBR(1)+BPB(1)+FAT*2+Root+FAT1(1)+FAT2(1)
        uiTotalWrSec = 1+1+2*uiSectorPerFat+32+1+1;
    }
    else                                            // FAT32
    {
        //
        // decide FAT type, system ID
        //
        uiSectorPerCluster = 64;
        uiBuSize = 8192;

        uiFatType = MBR_FAT32;
        uiFatBits = 32;                 // each FAT entry is 32 bits

        // If ending location < 8032.5MB (8,422,686,720) => 0Bh
        // Else => 0Ch
        if (guiSdioSecCnt <= VSDIO_FMT_SYSID_0B_SECTOR_CNT)
        {
            uiSystemId = 0x0B;
        }
        else
        {
            uiSystemId = 0x0C;
        }

        //
        // comute format parameters
        //

        // 1. sector per cluster (SC): 64
        // 2. sector size (SS): 512
        // 3. total sectors (TS): vgSDIO_SdmmcInfo.uiTotalSector
        // 4. FAT bits: uiFatBits
        // 5. Sector per FAT: SF = ceil( TS/SC*FAT bits / (SS * 8) )
        if (((guiSdioSecCnt / 64 * uiFatBits) % (512 * 8)) == 0)
        {
            uiSectorPerFat = (guiSdioSecCnt / 64 * uiFatBits) / (512 * 8);
        }
        else
        {
            uiSectorPerFat = (guiSdioSecCnt / 64 * uiFatBits) / (512 * 8) + 1;
        }
        debug_ind(("%s: SF = %d\r\n", __func__, uiSectorPerFat));

        // 6. number of sectors in MBR(NOM): NOM = BU
        uiRelativeSector = uiBuSize;
        debug_ind(("%s: NOM = %d\r\n", __func__, uiRelativeSector));

        do {
            // 7. Reserved Sector Count (RSC): RSC = BU * n - 2 * SF
            for (i=1; uiBuSize*i <= 2*uiSectorPerFat; i++);
            uiReserveSector = uiBuSize*i - 2*uiSectorPerFat;
            debug_ind(("%s: RSC = %d\r\n", __func__, uiReserveSector));

            // 8. If RSC is smaller than 9, RSC is added BU.
            if (uiReserveSector < 9)
            {
                uiReserveSector += uiBuSize;
                debug_ind(("%s: RSC(2) = %d\r\n", __func__, uiReserveSector));
            }

            // 9. number of sectors in the system area (SSA):
            //      SSA = RSC + 2*SF
            uiSysAreaSize = uiReserveSector + 2*uiSectorPerFat;
            debug_ind(("%s: SSA = %d\r\n", __func__, uiSysAreaSize));

            do
            {
                // 10. Max cluster number (MAX):
                //      MAX = ip ( (TS-NOM-SSA) / SC ) + 1
                uiMaxCluster = (guiSdioSecCnt - uiRelativeSector - uiSysAreaSize) / 64 + 1;
                debug_ind(("%s: MAX = %d\r\n", __func__, uiMaxCluster));

                // 11. recalculate SF
                //      SF' = ceil { [2 + (MAX - 1)] * FAT bits / (SS * 8) }
                if (((2 + (uiMaxCluster - 1)) * uiFatBits % (512 * 8)) == 0)
                {
                    uiSectorPerFatPrime = (2 + (uiMaxCluster - 1)) * uiFatBits / (512 * 8);
                }
                else
                {
                    uiSectorPerFatPrime = (2 + (uiMaxCluster - 1)) * uiFatBits / (512 * 8) + 1;
                }
                debug_ind(("%s: SF' = %d\r\n", __func__, uiSectorPerFatPrime));

                // 12. If SF' is greater than SF, SSA and RSC are added BU.
                if (uiSectorPerFatPrime > uiSectorPerFat)
                {
                    uiReserveSector += uiBuSize;
                    uiSysAreaSize += uiBuSize;
                    debug_ind(("%s: new SSA = %d\r\n", __func__, uiSysAreaSize));
                    debug_ind(("%s: new RSC = %d\r\n", __func__, uiReserveSector));
                }
            } while (uiSectorPerFatPrime > uiSectorPerFat);

            // 13. If SF' != SF, 'SF-1' is used as new SF
            if (uiSectorPerFatPrime != uiSectorPerFat)
            {
                uiSectorPerFat--;
                debug_ind(("%s: new SF = %d\r\n", __func__, uiSectorPerFat));
            }
            else
            {
                break;
            }
        } while (1);

        // total written sectors: MBR(1)+BPB(2)+ExtraBPB(2)+ResrvBPB(2)+2*FAT+Root+FAT1(1)+FAT2(1)
        uiTotalWrSec = 1+2+2+2+2*uiSectorPerFat+uiSectorPerCluster+1+1;
    }

    // Calculate CHS parameters
    if (guiSdioSecCnt <= VSDIO_FMT_2MB_SECTOR_CNT)
    {
        uiHeadNum    = 2;
        uiSectorPerTrack  = 16;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_16MB_SECTOR_CNT)
    {
        uiHeadNum    = 2;
        uiSectorPerTrack  = 32;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_32MB_SECTOR_CNT)
    {
        uiHeadNum    = 4;
        uiSectorPerTrack  = 32;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_128MB_SECTOR_CNT)
    {
        uiHeadNum    = 8;
        uiSectorPerTrack  = 32;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_256MB_SECTOR_CNT)
    {
        uiHeadNum    = 16;
        uiSectorPerTrack  = 32;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_504MB_SECTOR_CNT)
    {
        uiHeadNum    = 16;
        uiSectorPerTrack  = 63;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_1008MB_SECTOR_CNT)
    {
        uiHeadNum    = 32;
        uiSectorPerTrack  = 63;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_2016MB_SECTOR_CNT)
    {
        uiHeadNum    = 64;
        uiSectorPerTrack  = 63;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_2GB_SECTOR_CNT)
    {
        uiHeadNum    = 128;
        uiSectorPerTrack  = 63;
    }
    else if (guiSdioSecCnt <= VSDIO_FMT_4032MB_SECTOR_CNT)
    {
        uiHeadNum    = 128;
        uiSectorPerTrack  = 63;
    }
    else
    {
        uiHeadNum    = 255;
        uiSectorPerTrack  = 63;
    }

    get_blk((VP*)&pucSecBuf, POOL_ID_FS_SECTOR);

    memset(pucSecBuf, 0, 512);

    // Master Boot Record
    usStartHead = (uiRelativeSector % (uiHeadNum*uiSectorPerTrack)) / uiSectorPerTrack;
    usStartSector = (uiRelativeSector % uiSectorPerTrack) + 1;
    usStartCylinder = uiRelativeSector / (uiHeadNum * uiSectorPerTrack);
    if (guiSdioSecCnt < VSDIO_FMT_8032_5MB_SECTOR_CNT)
    {
        usEndHead = ((guiSdioSecCnt-1) % (uiHeadNum*uiSectorPerTrack)) / uiSectorPerTrack;
        usEndSector = ((guiSdioSecCnt-1) % uiSectorPerTrack) + 1;
        usEndCylinder = (guiSdioSecCnt-1) / (uiHeadNum * uiSectorPerTrack);
    }
    else
    {
        usEndHead = VSDIO_MBR_MAX_HEAD;
        usEndSector = VSDIO_MBR_MAX_SECTOR;
        usEndCylinder = VSDIO_MBR_MAX_CYLINDER;
    }

    pucSecBuf[_0x1BE] = 0x80; // 0x0 Boot Indicator
    pucSecBuf[_0x1BE+1] = usStartHead; // 0x1 Starting Head
    // 0x2 Starting Sector.Only bit 0.5 are used. bit 6.7 are the upper two bits for the Starting Cylinder field.
    pucSecBuf[_0x1BE+2] = (usStartSector & 0x3F) + (((usStartCylinder & 0x300)>>8)<<6);
    // 0x3 Starting Cylinder. Contains only the lower 8 bits of the cylinder value.
    pucSecBuf[_0x1BE+3] = usStartCylinder & 0xFF;
    pucSecBuf[_0x1BE+4] = uiSystemId; // 0x4 System ID
    pucSecBuf[_0x1BE+5] = usEndHead; // 0x5 Ending Head
    // 0x6 Ending Sector. only bit 0.5 are used. bit 6.7 are the upper two bits for the Ending Cylinder field
    pucSecBuf[_0x1BE+6] = (usEndSector & 0x3F) + (((usEndCylinder & 0x300)>>8)<<6);
    // 0x7 Ending Cylinder. Contains only the lower 8 bits of the cylinder value.
    pucSecBuf[_0x1BE+7] = usEndCylinder & 0xFF;
    pucSecBuf[_0x1BE+8] = uiRelativeSector & 0xFF; // 0x8-0xB Relative Sector
    pucSecBuf[_0x1BE+9] = (uiRelativeSector >> 8) & 0xFF;
    pucSecBuf[_0x1BE+10] = (uiRelativeSector >> 16) & 0xFF;
    pucSecBuf[_0x1BE+11] = (uiRelativeSector >> 24) & 0xFF;
    // 0xC-0xF Total Sector
    pucSecBuf[_0x1BE+12] = (guiSdioSecCnt-uiRelativeSector) & 0xFF;
    pucSecBuf[_0x1BE+13] = ((guiSdioSecCnt-uiRelativeSector) >> 8) & 0xFF;
    pucSecBuf[_0x1BE+14] = ((guiSdioSecCnt-uiRelativeSector) >> 16) & 0xFF;
    pucSecBuf[_0x1BE+15] = ((guiSdioSecCnt-uiRelativeSector) >> 24) & 0xFF;
    pucSecBuf[0x1FE] = 0x55; // 0x1FE-1FF Sector End Marker
    pucSecBuf[0x1FF] = 0xAA;

    debug_ind(("%s: Write MBR\r\n", __func__));
    erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, 0, 1);
    if (erReturn != E_OK)
    {
        rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
        debug_err(("%s: write MBR fail, sector addr: 0x%lx\r\n", __func__, 0));
        return E_SYS;
    }
//    if (vpSDIOObj[id] == NULL)
//    {
//        debug_err(("SDIO%d fmt: storage object NULL, not initialized2\r\n", id));
//        return E_SYS;
//    }
    if (gSDIOObj.Strg_CB)
    {
        (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
    }

    memset(pucSecBuf, 0, 512);

    /* Bios Parameter Block */
    pucSecBuf[0x0] = 0xEB; // 0x0-0x2 Jump Instruction
    pucSecBuf[0x1] = 0x00;
    pucSecBuf[0x2] = 0x90;

    pdata = (INT8 *)strFormat_OemID;
    for(i=3; i<11; i++)
    {
        pucSecBuf[i] = *pdata++;
    }
    pucSecBuf[0xB] = 0x00; // 0xB-0xC Bytes Per Sector
    pucSecBuf[0xC] = 0x02;
    pucSecBuf[0xD] = uiSectorPerCluster;    // 0xD Sectors Per Cluster
    pucSecBuf[0xE] = uiReserveSector & 0xFF;
    pucSecBuf[0xF] = (uiReserveSector>>8) & 0xFF;
    pucSecBuf[0x10] = 0x02; // 0x10 Number of FAT
    pucSecBuf[0x11] = 0x00; // 0x11-0x12 Root Entries
    if(uiFatType == MBR_FAT32)
    {
        pucSecBuf[0x12] = 0x00; // FAT32 should be 0
    }
    else
    {
        pucSecBuf[0x12] = 0x02;
    }
    if (uiFatType == MBR_FAT12)
    { // FAT12
        if ((guiSdioSecCnt > 64896) && (guiSdioSecCnt <= 129792)) // 64M
        {
            pucSecBuf[0x20] = (guiSdioSecCnt-uiRelativeSector) & 0xFF;
            pucSecBuf[0x21] = ((guiSdioSecCnt-uiRelativeSector) >> 8) & 0xFF;
            pucSecBuf[0x22] = ((guiSdioSecCnt-uiRelativeSector) >> 16) & 0xFF;
            pucSecBuf[0x23] = ((guiSdioSecCnt-uiRelativeSector) >> 24) & 0xFF;
        }
        else
        {
            pucSecBuf[0x13] = (guiSdioSecCnt-uiRelativeSector) & 0xFF; // 0x13-0x14 small sectors (FAT12)
            pucSecBuf[0x14] = ((guiSdioSecCnt-uiRelativeSector) >> 8) & 0xFF;
        }

        pdata = (INT8 *)&tmpFAT12[0];
        for(i=0x36; i<0x3E; i++)
        {
            pucSecBuf[i] = *pdata++;
        }
    }
    else if(uiFatType == MBR_FAT16)
    { // FAT16
        pucSecBuf[0x20] = (guiSdioSecCnt-uiRelativeSector) & 0xFF; // 0x20-0x23 Large Sectors (FAT 16)
        pucSecBuf[0x21] = ((guiSdioSecCnt-uiRelativeSector) >> 8) & 0xFF;
        pucSecBuf[0x22] = ((guiSdioSecCnt-uiRelativeSector) >> 16) & 0xFF;
        pucSecBuf[0x23] = ((guiSdioSecCnt-uiRelativeSector) >> 24) & 0xFF;

        pdata = (INT8 *)&tmpFAT16[0];
        for(i=0x36; i<0x3E; i++)
        {
            pucSecBuf[i] = *pdata++;
        }
    }
    else
    {       //FAT32
        pucSecBuf[0x20] = (guiSdioSecCnt-uiRelativeSector) & 0xFF;
        pucSecBuf[0x21] = ((guiSdioSecCnt-uiRelativeSector) >> 8)& 0xFF;
        pucSecBuf[0x22] = ((guiSdioSecCnt-uiRelativeSector) >> 16)& 0xFF;
        pucSecBuf[0x23] = ((guiSdioSecCnt-uiRelativeSector) >> 24)& 0xFF;

    }

    pucSecBuf[0x15] = 0xF8; // 0x15 Medis Descriptor

    if(uiFatType == MBR_FAT32)
    {
        pucSecBuf[0x16]= 0x00;
        pucSecBuf[0x17]= 0x00;
    }
    else
    {
        pucSecBuf[0x16] = uiSectorPerFat & 0xFF; // 0x16-0x17 Sectors Per FAT
        pucSecBuf[0x17] = (uiSectorPerFat >> 8) & 0xFF;
    }
    pucSecBuf[0x18] = uiSectorPerTrack; // 0x18-0x19 Sectors Per Track
    pucSecBuf[0x19] = 0x00;
    pucSecBuf[0x1A] = uiHeadNum; //0x1A-0x1B Number of Heads
    pucSecBuf[0x1B] = 0x00;
    pucSecBuf[0x1C] = uiRelativeSector & 0xFF;  // 0x1C-0x1F Hidden Sectors
    pucSecBuf[0x1D] = (uiRelativeSector >> 8) & 0xFF;
    pucSecBuf[0x1E] = (uiRelativeSector >> 16) & 0xFF;
    pucSecBuf[0x1F] = (uiRelativeSector >> 24) & 0xFF;
    if(uiFatType == MBR_FAT32)
    {
        RTC_TIME time;
        RTC_DATE day;

        pucSecBuf[0x24] = uiSectorPerFat & 0xFF;
        pucSecBuf[0x25] = (uiSectorPerFat >> 8) & 0xFF;
        pucSecBuf[0x2C] = 0x02; //Root cluster
        pucSecBuf[0x30] = 0x01; //FSInfo
        pucSecBuf[0x32] = 0x06; //boot sector
        pdata = (INT8 *)&tmpFAT32[0];
        for(i=0x52; i<0x5A; i++)
        {
            pucSecBuf[i] = *pdata++;
        }

        pucSecBuf[0x40] = 0x80; // 0x40 Physical Drive Number
        // 0x41 Reserved. 0x0
        pucSecBuf[0x42] = 0x29; // 0x42 Extended Boot Signature
        // 0x43-46 Volume ID number
        if (bFormat_RandVolID == TRUE)
        {
            time = rtc_getTime();
            pucSecBuf[0x43] = time.value & 0xFF;
            pucSecBuf[0x44] = (time.value >> 16) & 0xFF;
            day = rtc_getDate();
            pucSecBuf[0x45] = day.value & 0xFF;
            pucSecBuf[0x46] = (day.value >> 16) & 0xFF;
        }
        else
        {
            pucSecBuf[0x43] = uiFormat_VolID & 0xFF;
            pucSecBuf[0x44] = (uiFormat_VolID >> 8) & 0xFF;
            pucSecBuf[0x45] = (uiFormat_VolID >> 16) & 0xFF;
            pucSecBuf[0x46] = (uiFormat_VolID >> 24) & 0xFF;
        }
        pdata = (INT8 *)strFormat_VolumeLabel;
        for(i=0x47; i<0x52; i++)
        {
            pucSecBuf[i] = *pdata++;
        }
    }
    else
    {
        RTC_TIME time;
        RTC_DATE day;

        pucSecBuf[0x24] = 0x80; // 0x24 Physical Drive Number
        // 0x25 Reserved. 0x0
        pucSecBuf[0x26] = 0x29; // 0x26 Extended Boot Signature
        // 0x27-2A Volume ID number
        if (bFormat_RandVolID == TRUE)
        {
            time = rtc_getTime();
            pucSecBuf[0x27] = time.value & 0xFF;
            pucSecBuf[0x28] = (time.value >> 16) & 0xFF;
            day = rtc_getDate();
            pucSecBuf[0x29] = day.value & 0xFF;
            pucSecBuf[0x2A] = (day.value >> 16) & 0xFF;
        }
        else
        {
            pucSecBuf[0x27] = uiFormat_VolID & 0xFF;
            pucSecBuf[0x28] = (uiFormat_VolID >> 8) & 0xFF;
            pucSecBuf[0x29] = (uiFormat_VolID >> 16) & 0xFF;
            pucSecBuf[0x2A] = (uiFormat_VolID >> 24) & 0xFF;
        }
        pdata = (INT8 *)strFormat_VolumeLabel;
        for(i=0x2B; i<0x36; i++)
        {
            pucSecBuf[i] = *pdata++;
        }
    }

    pucSecBuf[0x1FE] = 0x55; // 0x1FE-0x1FF End of Sector Maker
    pucSecBuf[0x1FF] = 0xAA;

    debug_ind(("%s: Write BPB\r\n", __func__));
    erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector, 1);
    if (erReturn != E_OK)
    {
        rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
        debug_err(("%s: write BPB fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector));
        return E_SYS;
    }
//    if (vpSDIOObj[id] == NULL)
//    {
//        debug_err(("SDIO%d fmt: storage object NULL, not initialized3\r\n", id));
//        return E_SYS;
//    }
    if (gSDIOObj.Strg_CB)
    {
        (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
    }

    if(uiFatType == MBR_FAT32)
    {
        debug_ind(("%s: Write backup BPB\r\n", __func__));
        erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector+6, 1);
        if (erReturn != E_OK)
        {
            rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
            debug_err(("%s: write backup BPB fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector + 6));
            return E_SYS;
        }

//        if (vpSDIOObj[id] == NULL)
//        {
//            debug_err(("SDIO%d fmt: storage object NULL, not initialized4\r\n", id));
//            return E_SYS;
//        }
        if (gSDIOObj.Strg_CB)
        {
            (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
        }
    }

    if(uiFatType == MBR_FAT32)
    {
        //  write FSINFO
        memset(pucSecBuf, 0, 512);

        pucSecBuf[FSI_LEADSIG] = FSI_LEADSIG_VAL & 0xFF;
        pucSecBuf[FSI_LEADSIG+1] = (FSI_LEADSIG_VAL >> 8) & 0xFF;
        pucSecBuf[FSI_LEADSIG+2] = (FSI_LEADSIG_VAL >> 16) & 0xFF;
        pucSecBuf[FSI_LEADSIG+3] = (FSI_LEADSIG_VAL >> 24) & 0xFF;

        pucSecBuf[FSI_STRUCSIG] = FSI_STRUCSIG_VAL & 0xFF;
        pucSecBuf[FSI_STRUCSIG+1] = (FSI_STRUCSIG_VAL >> 8) & 0xFF;
        pucSecBuf[FSI_STRUCSIG+2] = (FSI_STRUCSIG_VAL >> 16) & 0xFF;
        pucSecBuf[FSI_STRUCSIG+3] = (FSI_STRUCSIG_VAL >> 24) & 0xFF;

        uiTotalCluster = (guiSdioSecCnt - 2*uiSectorPerFat)/uiSectorPerCluster;
        uiTotalCluster = 0xFFFFFFFF;
        pucSecBuf[FSI_FREE_COUNT] = uiTotalCluster & 0xFF;
        pucSecBuf[FSI_FREE_COUNT+1] = (uiTotalCluster >> 8) & 0xFF;
        pucSecBuf[FSI_FREE_COUNT+2] = (uiTotalCluster >> 16) & 0xFF;
        pucSecBuf[FSI_FREE_COUNT+3] = (uiTotalCluster >> 24) & 0xFF;

        pucSecBuf[FSI_NXT_FREE] = 0x02; // first driver from cluster 2
        pucSecBuf[0x1FE] = 0x55; // 0x1FE-0x1FF End of Sector Maker
        pucSecBuf[0x1FF] = 0xAA;

        debug_ind(("%s: Write FS_INFO\r\n", __func__));
        erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + 1, 1); //FS_INFO
        if (erReturn != E_OK)
        {
            rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
            debug_err(("%s: write FS_INFO fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector + 1));
            return E_SYS;
        }
//        if (vpSDIOObj[id] == NULL)
//        {
//            debug_err(("SDIO%d fmt: storage object NULL, not initialized5\r\n", id));
//            return E_SYS;
//        }
        if (gSDIOObj.Strg_CB)
        {
            (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
        }

        debug_ind(("%s: Write backup FS_INFO\r\n", __func__));
        erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + 1 + 6, 1); //FS_INFO
        if (erReturn != E_OK)
        {
            rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
            debug_err(("%s: write backup FS_INFO fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector + 1 + 6));
            return E_SYS;
        }
//        if (vpSDIOObj[id] == NULL)
//        {
//            debug_err(("SDIO%d fmt: storage object NULL, not initialized6\r\n", id));
//            return E_SYS;
//        }
        if (gSDIOObj.Strg_CB)
        {
            (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
        }

        memset(pucSecBuf, 0, 510);
        pucSecBuf[0x1FE] = 0x55;
        pucSecBuf[0x1FF] = 0xAA;
        debug_ind(("%s: Write reserved boot sector\r\n", __func__));
        erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + 2, 1); // reserved boot secotr
        if (erReturn != E_OK)
        {
            rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
            debug_err(("%s: write reserved boot sector fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector + 2));
            return E_SYS;
        }
//        if (vpSDIOObj[id] == NULL)
//        {
//            debug_err(("SDIO%d fmt: storage object NULL, not initialized7\r\n", id));
//            return E_SYS;
//        }
        if (gSDIOObj.Strg_CB)
        {
            (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
        }

        debug_ind(("%s: Write reserved boot sector 2\r\n", __func__));
        erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + 2 + 6, 1); // reserved boot secotr
        if (erReturn != E_OK)
        {
            rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
            debug_err(("%s: write reserved boot sector2 fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector + 2 + 6));
            return E_SYS;
        }
//        if (vpSDIOObj[id] == NULL)
//        {
//            debug_err(("SDIO%d fmt: storage object NULL, not initialized8\r\n", id));
//            return E_SYS;
//        }
        if (gSDIOObj.Strg_CB)
        {
            (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
        }
    }

    uiBufSize = 32 *1024 ; /*POOL_SIZE_FS_SECTOR */
    memset(pucSecBuf, 0, uiBufSize);
    // Clear FAT and Root directory
    if(uiFatType == MBR_FAT32)
    {
        uiRootDirSec = uiSectorPerCluster;
    }
    else
    {
        uiRootDirSec = 32;
    }

    uiWriteSec = ( uiBufSize ) / 512;

    debug_ind(("%s: Clear FAT Table 1 and 2\r\n", __func__));
    for(i = 0; i < (2 * uiSectorPerFat + uiRootDirSec); i+=uiWriteSec)
    {
        if(i+uiWriteSec <  (2 * uiSectorPerFat + uiRootDirSec))
        {
            erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + uiReserveSector + i, uiWriteSec);
        }
        else
        {
            erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + uiReserveSector + i, (2 * uiSectorPerFat + uiRootDirSec) - i);
        }

        if (erReturn != E_OK)
        {
            rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
            debug_err(("%s: clear FAT fail, sector addr: 0x%lx\r\n", __func__, uiRelativeSector + uiReserveSector + i));
            return E_SYS;
        }

        if(i+uiWriteSec <  (2 * uiSectorPerFat + uiRootDirSec))
        {
            uiWrSec += uiWriteSec;
        }
        else
        {
            uiWrSec += (2 * uiSectorPerFat + uiRootDirSec) - i;
        }

//        if (vpSDIOObj[id] == NULL)
//        {
//            debug_err(("SDIO%d fmt: storage object NULL, not initialized9\r\n", id));
//            return E_SYS;
//        }
        if (gSDIOObj.Strg_CB)
        {
            (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,uiWrSec,0);
        }
    }

    if (uiFatType == MBR_FAT12)
    {
        pucSecBuf[0x00] = 0xF8;
        pucSecBuf[0x01] = 0xFF;
        pucSecBuf[0x02] = 0xFF;
    }
    else if(uiFatType == MBR_FAT16)
    {
        pucSecBuf[0x00] = 0xF8;
        pucSecBuf[0x01] = 0xFF;
        pucSecBuf[0x02] = 0xFF;
        pucSecBuf[0x03] = 0xFF;
    }
    else
    {
        pucSecBuf[0x00] = 0xF8;
        pucSecBuf[0x01] = 0xFF;
        pucSecBuf[0x02] = 0xFF;
        pucSecBuf[0x03] = 0x0F;

        pucSecBuf[0x04] = 0xFF;
        pucSecBuf[0x05] = 0xFF;
        pucSecBuf[0x06] = 0xFF;
        pucSecBuf[0x07] = 0x0F;

        pucSecBuf[0x08] = 0xFF;
        pucSecBuf[0x09] = 0xFF;
        pucSecBuf[0x0A] = 0xFF;
        pucSecBuf[0x0B] = 0x0F;
    }

    // FAT 1
    debug_ind(("%s: Write FAT Table 1\r\n", __func__));
    erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + uiReserveSector, 1);   //FAT32 FAT table should reserved 32 sectors
    if (erReturn != E_OK)
    {
        rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
        debug_err(("%s: write FAT1 fail, sector addr: 0x%0x\r\n", __func__, uiRelativeSector + uiReserveSector));
        return E_SYS;
    }
//    if (vpSDIOObj[id] == NULL)
//    {
//        debug_err(("SDIO%d fmt: storage object NULL, not initialized10\r\n", id));
//        return E_SYS;
//    }
    if (gSDIOObj.Strg_CB)
    {
        (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
    }

    // FAT 2
    debug_ind(("%s: Write FAT Table 2\r\n", __func__));
    erReturn = gSDIOObj.WrSectors((INT8 *)pucSecBuf, uiRelativeSector + uiSectorPerFat + uiReserveSector, 1); //FAT32 FAT table should reserved 32 sectors
    if (erReturn != E_OK)
    {
        rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
        debug_err(("%s: write FAT2 fail, sector addr: 0x%0x\r\n", __func__, uiRelativeSector + uiSectorPerFat + uiReserveSector));
        return E_SYS;
    }
//    if (vpSDIOObj[id] == NULL)
//    {
//        debug_err(("SDIO%d fmt: storage object NULL, not initialized11\r\n", id));
//        return E_SYS;
//    }
    if (gSDIOObj.Strg_CB)
    {
        (gSDIOObj.Strg_CB)(STRG_CB_ID_FORMAT,uiTotalWrSec,++uiWrSec,0);
    }

    rel_blk(POOL_ID_FS_SECTOR, (VP) pucSecBuf);
    debug_ind(("%s: Format OK\r\n", __func__));

    return erReturn;
    ///////////////////////

//    erReturn = sdioPrtcl_format(SDIO_HOST_ID_1);
//    gSDIOObj.uiStrgStatus = pSdioStrg->uiStrgStatus;

//    virtualSdio_unlock();

    return erReturn;
}

/**
    Get SDIO storage object

    @return SDIO storage object pointer
*/
PSTRG_TAB virtualSdio3_getStorageObject(void)
{
    return &gSDIOObj;
}

/**
    Set SD/MMC/SDIO card format parameters

    Since each project may has differenct format parameter.
    This function is provided to project layer to set format parameter.
    (SUGGESTED to be invoked in system init phase)

    @param[in] pOemID       Oem ID string in PBS
    @param[in] pVolumeLab   volume label in PBS
    @param[in] bRandomVolID Random volume ID
                            - @b TRUE: VolID in PBS is generated randomly
                            - @b FALSE: VolID is the same as uiVolID
    @param[in] uiVolID      VolID when bRandomVolID is TRUE

    @return none
*/
void virtualSdio3_setFormatParam(INT8* pOemID, INT8* pVolumeLab, BOOL bRandomVolID, UINT32 uiVolID)
{
    UINT32 i, len;

    len = strlen(pOemID);
    for (i=0; i<sizeof(strFormat_OemID)-1; i++)
    {
        if (i < len)
        {
            strFormat_OemID[i] = pOemID[i];
        }
        else
        {
            strFormat_OemID[i] = ' ';
        }
    }

    len = strlen(pVolumeLab);
    for (i=0; i<sizeof(strFormat_VolumeLabel)-1; i++)
    {
        if (i < len)
        {
            strFormat_VolumeLabel[i] = pVolumeLab[i];
        }
        else
        {
            strFormat_VolumeLabel[i] = ' ';
        }
    }

    bFormat_RandVolID = bRandomVolID;
    uiFormat_VolID = uiVolID;
}

/**
    Set virtual SDIO physical mapping

    @param[in] pStrg        storage object of mounted SDIO
    @param[in] uiBaseLBA    starting LBA on SDIO
    @param[in] uiSecCnt     sector count of virtual SDIO driver

    @return
        - @b E_OK: operation OK
*/
ER virtualSdio3_setMapping(SDIO_ID sdioID, UINT32 uiBaseLBA, UINT32 uiSecCnt)
{
    if (sdioID == SDIO_ID_1)
    {
        pSdioStrg = sdio_getStorageObject();
    }
    else
    {
        pSdioStrg = sdio2_getStorageObject();
    }
    guiSdioBaseLBA = uiBaseLBA;
    guiSdioSecCnt = uiSecCnt;

    return E_OK;
}

//@}

//@}




