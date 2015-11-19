/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CopyRest.c
    @ingroup    mIPRJAPCfg

    @brief      Copy rest FW code
                Copy rest FW code and do some hardware initialization.

    @note       Nothing.

    @date       2006/02/08
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include <string.h>
#include "CopyRest.h"
#include "Kernel.h"
#include "SysCfg.h"
#include "Nand.h"
#include "Lens.h"
#include "SIF.h"
#include "Top.h"
#include "Pll.h"
#include "JpgEnc.h"
#include "GPIOMapping.h"
#include "Timer.h"
#include "SystemInit.h"
#include "Utility.h"
#include "UICustomer.h"

// Reserved size for splash screen (Maximum: 640x480, 422)
#define SPLASH_SCREEN_RESERVED_SIZE     0x100000

// This value should be the FW binary file size
extern UINT32   Image$$GENERAL_ZI$$ZI$$Base;

// This value should be the FW total size (RO + RW + ZI)
extern UINT32   Image$$GENERAL_ZI$$ZI$$Limit;

// This value is the part one + part two size
extern UINT32   Load$$PART_THREE_RO$$Base;
// To determinate that should we copy part two RW
extern UINT32   Load$$PART_TWO_RW$$Base;
extern UINT32   Image$$PART_TWO_RW$$Base;
extern UINT32   Image$$PART_TWO_RW$$Length;
// To determinate that should we copy part three RW
//extern UINT32   Load$$PART_THREE_RW$$Base;
//extern UINT32   Image$$PART_THREE_RW$$Base;
//extern UINT32   Image$$PART_THREE_RW$$Length;

static UINT32   uiStartBlockNum, uiDestAddress;
static UINT32   uiNandBlockSize;
static BOOL     bPartialLoading;
static BOOL     bFWCompressFlag;
static UINT32   uiCompressFlag;
//#NT#2008/12/09#Chris Chung -begin
//#Support Partial Load and FW Decompression
/**
  @function CopyRest_CheckCompressed

  Check if it is a compressed FW. The is related to compressed tool(bfc.exe).
  The current writed header in bfc.exe is as follow:

      fwrite( "BCL1", 4, 1, f );
      WriteWord32( algo, f );       //algo: The compression algorithm
      WriteWord32( insize, f );     //insize: The input data size
      WriteWord32( outsize, f );    //outsize: The output/compressed data size

  @param unsigned char *buf: the buffer that we want to check.
  @return TRUE or FALSE
*/
static BOOL CopyRest_CheckCompressed(UINT8 *buf)
{
    // compress key word "BCL1" (it's better to check algorithm number 9 also)
    if (*buf == 'B' && *(buf + 1) == 'C' && *(buf + 2) == 'L' && *(buf + 3) == '1')
        return TRUE;
    return FALSE;
}

/**
  Run part one code

  Run part one code loaded by loader

  @param void
  @return void
*/
void CopyRest_RunPartOne(void)
{
    UINT32 uiFWLen = 0;

    //#NT#2010/02/10#Chris Hsu -begin
    //#NT#Add PLL init code and fix LED doesn't work when FW code size is too large
    // Init GPIO
    GPIOMap_Init();

    // Check memory size for FW
    if ((UINT32)&Image$$GENERAL_ZI$$ZI$$Limit > FRE_MEM)
    {
        UINT32 uiTimerID;

        debug_err(("\r\nFW code (%ld) is too large for current setting\r\nPlease modify FRE_MEM in SysCfg.h\r\n", &Image$$GENERAL_ZI$$ZI$$Limit));

        if (timer_open((UINT *)&uiTimerID, NULL) == E_OK)
        {
            timer_set(uiTimerID, 500, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
        }

        while (1)
        {
            timer_waitTimeup(uiTimerID);
            GPIOMap_TurnOffLED(GPIOMAP_LED_RED);
            GPIOMap_TurnOnLED(GPIOMAP_LED_GREEN);

            timer_waitTimeup(uiTimerID);
            GPIOMap_TurnOffLED(GPIOMAP_LED_GREEN);
            GPIOMap_TurnOnLED(GPIOMAP_LED_RED);
        }
    }

    // Set PLL OSC clock
    // Default is 12 MHz
	//pll_setPLLOSC(SYSINIT_PLL_OSC_CLOCK_MHZ * 1000000);

    // PLL2 default output frequency is 297 MHz
    // Enable PLL2 and configure to 108 MHz (For Display)
    // Disable PLL2 first
    pll_setPLL2En(FALSE);
    pll_setPLL2(SYSINIT_PLL2_LOOPDIV, SYSINIT_PLL2_PREDIV);
    pll_setPLL2En(TRUE);

#if ((SYSINIT_DRAM_CLKSRC_PLL1 == ENABLE) || \
     (SYSINIT_OUTPUT_MCLK == ENABLE))
    // If loader didn't configure PLL1, we must configure here
    // PLL1 default output frequency is 264 MHz
    /*
    // Enable PLL1 and configure to 240MHz (For DRAM and Sensor)
    // Disable PLL1 first
    pll_setPLL1En(FALSE);
    pll_setPLL1(SYSINIT_PLL1_LOOPDIV, SYSINIT_PLL1_PREDIV);
    pll_setPLL1En(TRUE);
    */
#endif
    //#NT#2010/02/10#Chris Hsu -end

    // Init JPEG
    JpegInit();

//#NT#2012/09/05#Philex Lin -begin
//#NT# pressing power button longer to startup system
#if (_LONG_STARTUP_ == DISABLE)
    // Init LCD and display splash screen
    SysInit_InitLCD();

    //#NT#2008/01/04#Chris Hsu -begin
    //#PStore size must set before 1st nand_open()
    // SysInit_InitPStore() will set FW reserved size, NAND2 size and
    // 1. System parameter block size (If not using PStore)
    // 2. PStore size (If using PStore)
    // Above information must set before first nand_open() to make sure
    // these information are correct.
    SysInit_InitPStore();
    //#NT#2008/01/04#Chris Hsu -end

    // Display poweron logo here
    if (1)//(FALSE == UI_ShowLogoFile(PS_BG_LOGO))
    {
        SysInit_DisplaySplashScreen(LOGO_STARTUP,0,0);
    }
    TimerDelayMs(500);
#else
    //#NT#2008/01/04#Chris Hsu -begin
    //#PStore size must set before 1st nand_open()
    // SysInit_InitPStore() will set FW reserved size, NAND2 size and
    // 1. System parameter block size (If not using PStore)
    // 2. PStore size (If using PStore)
    // Above information must set before first nand_open() to make sure
    // these information are correct.
    SysInit_InitPStore();
    //#NT#2008/01/04#Chris Hsu -end
#endif
//#NT#2012/09/05#Philex Lin -end


    //#NT#2009/02/16#JustinLee -begin
    //#Added lens module init entry point in system
    // Init lens module, including lens control task
    SysInit_InitLens();
    //#NT#2009/02/16#JustinLee -end
}

/**
  Copy part two FW from NAND flash

  Copy part two FW code from NAND flash

  @param void
  @return void
*/
void CopyRest_CopyPartTwo(void)
{
    UINT32      uiBlockCnt, uiRemainingSize;
    UINT32      uicompressCodeAddress;
    UINT32      uiDataSize;
    UINT32      uiTmpDataSize;

    // Check the size that loader loaded
    // If the size is 0, loader load all FW already
    uiDestAddress = *(volatile UINT32 *)FW_PARTIAL_LOADING_OFFSET;
    if (uiDestAddress == 0)
    {
        bPartialLoading = FALSE;
        return;
    }
    else
    {
        bPartialLoading = TRUE;
    }

    uiCompressFlag = *(volatile UINT32 *)FW_COMPRESS_FLAG_OFFSET;

    // Remain Code was compressed or not
    // uiCompressFlag == 0 --> Not compress
    // uiCompressFlag == 1 --> compressed
    if(uiCompressFlag == 0)
        bFWCompressFlag = FALSE;
    else
        bFWCompressFlag = TRUE;

    uicompressCodeAddress = *(volatile UINT32 *)FW_RESERVED1_OFFSET;

    // Run from NAND
    if(uicompressCodeAddress == 0)
    {
        // Remain data in NAND (SPI) was not compressed
        if(bFWCompressFlag == FALSE)
        {
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
            nor_open();

            //#NT#2008/12/16#KS Hung -begin
            //#NT#If Part three smaller than part one size, the block count is zero.
            //if ((UINT32)&Load$$PART_THREE_RO$$Base > uiDestAddress)
            if ((UINT32)&Image$$GENERAL_ZI$$ZI$$Base > uiDestAddress)
            {
            uiBlockCnt      = ((UINT32)&Image$$GENERAL_ZI$$ZI$$Base - uiDestAddress) / SPI_SECTOR_SIZE;
            uiRemainingSize = (UINT32)&Image$$GENERAL_ZI$$ZI$$Base % SPI_SECTOR_SIZE;
            if (uiRemainingSize != 0)
                uiBlockCnt++;
            }
            else
            {
                uiBlockCnt = 0;
            }

            debug_err(("CopyPartTwo: uiDestAddress = 0x%08x, read size = 0x%08x\r\n", uiDestAddress, uiBlockCnt * SPI_SECTOR_SIZE));
            nor_readByBytes(LOADER_SIZE + uiDestAddress, uiBlockCnt * SPI_SECTOR_SIZE, (UINT8 *)uiDestAddress);
            //#NT#2008/12/16#KS Hung -end

            // Copy part two RW section
            // Image is execution section, and Load is binary file(load) section
            if ((UINT32)&Image$$PART_TWO_RW$$Base != (UINT32)&Load$$PART_TWO_RW$$Base)
            {
                memcpy((void *)(&Image$$PART_TWO_RW$$Base), (const void *)(&Load$$PART_TWO_RW$$Base), (UINT32)&Image$$PART_TWO_RW$$Length);
            }

//            uiDestAddress += (uiBlockCnt * SPI_SECTOR_SIZE);  // mark for dummy, philex, 2009/11/18
#else
            // Temporary disable NAND interrupt to improve read performance.
            // We will re-enable NAND interrupt after all FW are loaded.
            nand_setIntEn(FALSE);

            // Copy part two FW code
            // Open NAND
            nand_open();

            // Read NAND information
            uiNandBlockSize = nand_getBlockSize();

            uiStartBlockNum = (uiDestAddress / uiNandBlockSize) + FW_START_NAND_BLOCK;
            //uiBlockCnt      = ((UINT32)&Load$$PART_THREE_RO$$Base - uiDestAddress) / uiNandBlockSize;
            //uiRemainingSize = (UINT32)&Load$$PART_THREE_RO$$Base % uiNandBlockSize;
            uiBlockCnt      = ((UINT32)&Image$$GENERAL_ZI$$ZI$$Base - uiDestAddress) / uiNandBlockSize;
            uiRemainingSize = (UINT32)&Image$$GENERAL_ZI$$ZI$$Base % uiNandBlockSize;

            if (uiRemainingSize != 0)
            {
                uiBlockCnt++;
            }

            // Read NAND data to DRAM
            nand_readReservedBlock((INT8 *)uiDestAddress, uiStartBlockNum, uiBlockCnt);

            // Copy part two RW section
            // Image is execution section, and Load is binary file(load) section
            if ((UINT32)&Image$$PART_TWO_RW$$Base != (UINT32)&Load$$PART_TWO_RW$$Base)
            {
                memcpy((void *)(&Image$$PART_TWO_RW$$Base), (const void *)(&Load$$PART_TWO_RW$$Base), (UINT32)&Image$$PART_TWO_RW$$Length);
            }
#if 0   // mark for dummy, philex, 2009/11/18
            // For next part
            uiStartBlockNum += uiBlockCnt;
            uiDestAddress   += (uiBlockCnt * uiNandBlockSize);
#endif
#endif
        }
        else
        {
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
            UINT32  uiTmpBufAddr;

            debug_err(("CopyPartTwo: remain FW is compress part 1 size = 0x%x\r\n", uiDestAddress));
            //#NT#2008/12/15#Philex -end

            // Get memory for temporary buffer
            get_blk((VP *)&uiTmpBufAddr, POOL_ID_FS_FAT);
            rel_blk(POOL_ID_FS_FAT, (VP)uiTmpBufAddr);

            nor_open();
            nor_readByBytes(LOADER_SIZE + uiDestAddress, FW_COMPRESS_HEADER_SIZE, uiTmpBufAddr);


            // check if it's compressed data
            if (CopyRest_CheckCompressed((UINT8 *)uiTmpBufAddr))
            {
                memcpy((void *)&uiTmpDataSize, (void *)(uiTmpBufAddr+12), 4);

                uiDataSize = ((uiTmpDataSize & 0x000000FF) << 24) |  ((uiTmpDataSize & 0x0000FF00) << 8) | ((uiTmpDataSize & 0x00FF0000) >> 8) | ((uiTmpDataSize & 0xFF000000) >> 24);

                debug_err(("compressed data size=%d\r\n", uiDataSize));

                nor_readByBytes(LOADER_SIZE + uiDestAddress + FW_COMPRESS_HEADER_SIZE, uiDataSize, uiTmpBufAddr);

                // decompression
                debug_err(("decompressing..."));
                LZ_Uncompress((UINT8 *)uiTmpBufAddr, (unsigned char *)uiDestAddress, uiDataSize);
                debug_err(("OK!\n\r"));
            }
            nor_close();
#else
            debug_err(("Storage NAND should not partial compress\r\n"));
#endif
        }
    }
    // Copy part two RW section
    // Image is execution section, and Load is binary file(load) section
    if ((UINT32)&Image$$PART_TWO_RW$$Base != (UINT32)&Load$$PART_TWO_RW$$Base)
    {
        debug_err(("OK1\n\r"));
        memcpy((void *)(&Image$$PART_TWO_RW$$Base), (const void *)(&Load$$PART_TWO_RW$$Base), (UINT32)&Image$$PART_TWO_RW$$Length);
    }
}

/**
  Run part two code

  Run part two code loaded by CopyRest_CopyPartTwo().
  Here we display splash screen, job to do: Play start up sound

  @param void
  @return void
*/
void CopyRest_RunPartTwo(void)
{
    // Init JPEG
    //JpegInit();

    // Init LCD and display splash screen
    //SysInit_InitLCD();

    // Display poweron logo
    //SysInit_DisplaySplashScreen();
}

//@}
