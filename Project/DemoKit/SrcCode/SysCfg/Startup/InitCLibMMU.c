/*++

Copyright (c) 2005  Novatek Microelectronics Corporation

Module Name:
    InitCLibMMU.c

Abstract:
    1. Initialize environment for C runtime, floating-point processing,
       functions in ctype.h and atoi(), printf(), scanf() family
    2. Initialize MMU

Notes:
    Copyright (c) 2005 Novatek Microelectronics Corporation.  All Rights Reserved.

Revision History:
    03/17/05: Created by Chris Hsu
--*/

#include <Locale.h>
#include <rt_fp.h>
#include <rt_misc.h>
#include "Type.h"
#include "UART.h"
#include "SysCfg.h"
#include "Cache.h"

//#NT#2009/05/25#Elvis Chuang -begin
//#Added., Catch Case of Div 0
/* this function is called instead of the original __aeabi_idiv0() */
void __rt_raise(INT signal, INT type)
{
    ID callerTskId;
    get_tid(&callerTskId);
    //Case A: Div by integer zero[Signal=2],[type=2]
    // insert code to process a divide by zero
    debug_msg("^R[Signal=%d],[type=%d], [tsk=%d]",signal,type,callerTskId);
    if( (signal == 2) && (type == 2) )
    {
        debug_msg("^R => Divide By Zero\r\n");
    }
}
void __ttywrch(int ch)
{
    uart_putChar((CHAR)ch);
}
//#NT#2009/05/25#Elvis Chuang -end
//#NT#2010/05/10#Elvis Chuang -begin
//floating point exception handle
extern void $Super$$__fpl_exception(void);
void $Sub$$__fpl_exception(void)
{
    ID callerTskId;
    get_tid(&callerTskId);
    debug_msg("^R Floating Point Lib[FPL] exception, [tsk=%d]\r\n",callerTskId);
    $Super$$__fpl_exception();
}
//#NT#2010/05/10#Elvis Chuang -end
#define FA510_ENABLE_CACHE          ON

#define FA510_I_CACHE               ON
#define FA510_D_CACHE               ON
#define FA510_BTB                   ON
#define FA510_LOCK_CRITICAL_CACHE   ON
#define FA510_ALIGNMENT_CHECK       OFF
#define INIT_CLIB                   ON

/**
  Initialize stack and heap

  Initialize stack and heap. To use scatter loading, the function must be reimplemeted

  @param unsigned R0
  @param unsigned SP
  @param unsigned R2
  @param unsigned SL
  @return struct __initial_stackheap
*/
__value_in_regs struct __initial_stackheap __user_initial_stackheap(unsigned R0, unsigned SP, unsigned R2, unsigned SL)
{
    struct __initial_stackheap config;

    //#NT#2008/08/15#Chris Hsu -begin
    //#NT#Remove compiler warning message about "variable may be used before being set"
    config.heap_base    = (unsigned int)(HeapEnd_CRuntime - HeapSize_CRuntime);
    // Ignored in one-region memory model: the memory region is
    // taken to be all the space between heap_base and stack_base
    config.heap_limit   = 0;
    // inherit SP from the execution environment
    config.stack_base   = SP;
    // Ignored in one-region memory model: the memory region is
    // taken to be all the space between heap_base and stack_base
    config.stack_limit  = 0;
    //#NT#2008/08/15#Chris Hsu -end
    return config;
}
//#NT#2009/08/11#Elvis Chuang -begin
//Replace the reset handler after power on.
UINT32 CallerAddress;
extern void NULL_Func_Handler(void);
void NewResetHandler(void)
{
    ID callerTskId;
    get_tid(&callerTskId);
    debug_msg("^R NULL Function Call -Err- [TaskID=%d@0x%08X]\r\n",callerTskId,CallerAddress);
}
//#NT#2009/08/11#Elvis Chuang -end
/**
  Initialize C Lib

  Initialize ANSI C library.

  @param void
  @return void
*/
void InitCLib(void)
{
    //#NT#2009/08/11#Elvis Chuang -begin
    //Replace the reset handler after the power on
    *((UINT32*)0x00000020) = (UINT32)NULL_Func_Handler;
    //#NT#2009/08/11#Elvis Chuang -end
    // To use the following functions, SWI must be implemented/enabled
#if INIT_CLIB
    // Initialize Standard C runtime heap
    __rt_lib_init(END_MEM, HeapEnd_CRuntime);
    // For floating-point processing
    _fp_init();
    // For functions in ctype.h and atoi(), printf(), scanf() family
    setlocale(LC_ALL, "C");
    /*
    // For functions in math.h
    __rt_raise(INT signal, INT type);
    // For malloc()
    _init_alloc(UINT base, UINT top);
    */
#endif
}

/**
  Lockdown critical code and data

  Lockdown critical code and data in cache.
  Here we lockdown exception table.

  @param void
  @return void
*/
void LockCriticalCodeAndData(void)
{
#if FA510_LOCK_CRITICAL_CACHE
    // Lockdown I-Cache
    CPULockICache();
    CPUPrefetchICache(0x10);
    CPUUnlockICache();

    // Lockdown D-Cache
    CPULockDCache();
    CPUPrefetchDCache(0x30);
    CPUUnlockDCache();
#endif
}

/**
  Enable Cache and MPU

  Enable Cache and MPU.

  @param void
  @return void
*/
void EnableCacheMPU(void)
{
//#NT#2009/01/19#Hideo Lin -begin
//#Automatically select suitable configuration according to FRE_MEM
    UINT32  i, region, size[3], addr[3];
//#NT#2009/01/19#Hideo Lin -end

    //#NT#2010/06/01#Chris Hsu -begin
    //#NT#Clean data cache to DRAM before disable MMU
    // Since boot loader might enable data cache, and some data might
    // be in DATA cache. We have to flush data cache to DRAM before
    // disable MMU.
    CPUCleanInvalidateDCacheAll();
    CPUDrainWriteBuffer();
    //#NT#2010/06/01#Chris Hsu -end

    // Disabel MMU first
    CPUSetMMUEnable(DISABLE);
    // Disable all data/instruction regions
    CPUDisableAllInstrRegion();
    CPUDisableAllDataRegion();

#if FA510_ENABLE_CACHE
    /************************************************************************/
    /*                             !! Note !!                               */
    /* The region base will be automatically aligned to size boundary by    */
    /* hardware. Thus, the real region base will be "(Base / Size) * Size". */
    /************************************************************************/

/*-------- region 0 0x00000000 - 0xFFFFFFFF(4G byte) - full access & uncacheable & unbufferable --------*/
/*-------- This region is for buffer, set to uncacheable to avoid inconsistent access of CPU and DMA --------*/

    // Instruction region
    CPUSetInstrRegionBaseSize(0, 0, MEM_REGION_4G, ENABLE);
    CPUSetInstrRegionProtect(0, RW_ALL);
    CPUSetICacheCacheable(0, UNCACHEABLE);

    // Data region
    CPUSetDataRegionBaseSize(0, 0, MEM_REGION_4G, ENABLE);
    CPUSetDataRegionProtect(0, RW_ALL);
    CPUSetDCacheCacheable(0, UNCACHEABLE);
    CPUSetWriteBufable(0, UNBUFFERABLE);

//#NT#2009/01/19#Hideo Lin -begin
//#Automatically select suitable configuration according to FRE_MEM
/*-------- region 1&2 (Code, RO, RW, ZI data) - full access & cacheable & bufferable --------*/
/*-------- This region is for firmware code --------*/

    #if (FRE_MEM <= 0x00100000)     // 1.0 MB
    {
        region = 2;
        size[0] = MEM_REGION_1M;
        addr[0] = 0x00000000;
    }
    #elif (FRE_MEM <= 0x00180000)   // 1.5 MB
    {
        region = 3;
        size[0] = MEM_REGION_1M;
        size[1] = MEM_REGION_512K;
        addr[0] = 0x00000000;
        addr[1] = 0x00100000;
    }
    #elif (FRE_MEM <= 0x00200000)   // 2.0 MB
    {
        region = 2;
        size[0] = MEM_REGION_2M;
        addr[0] = 0x00000000;
    }
    #elif (FRE_MEM <= 0x00240000)   // 2.25 MB
    {
        region = 3;
        size[0] = MEM_REGION_2M;
        size[1] = MEM_REGION_256K;
        addr[0] = 0x00000000;
        addr[1] = 0x00200000;
    }
    #elif (FRE_MEM <= 0x00280000)   // 2.5 MB
    {
        region = 3;
        size[0] = MEM_REGION_2M;
        size[1] = MEM_REGION_512K;
        addr[0] = 0x00000000;
        addr[1] = 0x00200000;
    }
    #elif (FRE_MEM <= 0x00300000)   // 3.0 MB
    {
        region = 3;
        size[0] = MEM_REGION_2M;
        size[1] = MEM_REGION_1M;
        addr[0] = 0x00000000;
        addr[1] = 0x00200000;
    }
    #elif (FRE_MEM <= 0x00380000)   // 3.5 MB
    {
        region = 4;
        size[0] = MEM_REGION_2M;
        size[1] = MEM_REGION_1M;
        size[2] = MEM_REGION_512K;
        addr[0] = 0x00000000;
        addr[1] = 0x00200000;
        addr[2] = 0x00300000;
    }
    #else                           // 4.0 MB
    {
        region = 3;
        size[0] = MEM_REGION_2M;
        size[1] = MEM_REGION_2M;
        addr[0] = 0x00000000;
        addr[1] = 0x00200000;
    }
    #endif

    // Instruction region
    for (i = 1; i < region; i++)
    {
        CPUSetInstrRegionBaseSize(i, addr[i - 1], size[i - 1], ENABLE);
        CPUSetInstrRegionProtect(i, RW_ALL);
        CPUSetICacheCacheable(i, CACHEABLE);
    }

    // Data region
    for (i = 1; i < region; i++)
    {
        CPUSetDataRegionBaseSize(i, addr[i - 1], size[i - 1], ENABLE);
        CPUSetDataRegionProtect(i, RW_ALL);
        CPUSetDCacheCacheable(i, CACHEABLE);
        CPUSetWriteBufable(i, BUFFERABLE);
    }

/*-------- region 3 (Stack and C Runtime Heap) - full access & cacheable & bufferable --------*/

    // Data region
#if ((DRAM_END - END_MEM) <= 0x00001000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_4K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00002000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_8K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00004000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_16K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00008000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_32K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00010000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_64K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00020000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_128K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00040000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_256K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00080000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_512K, ENABLE);
#elif ((DRAM_END - END_MEM) <= 0x00100000)
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_1M, ENABLE);
#else
    CPUSetDataRegionBaseSize(region, END_MEM, MEM_REGION_2M, ENABLE);
#endif
    CPUSetDataRegionProtect(region, RW_ALL);
    CPUSetDCacheCacheable(region, CACHEABLE);
    CPUSetWriteBufable(region, BUFFERABLE);
//#NT#2009/01/19#Hideo Lin -end

// #if FA510_ENABLE_CACHE
#endif

/*-------- Config control register --------*/
#if FA510_I_CACHE
    CPUSetICacheEnable(ENABLE);
#else
    CPUSetICacheEnable(DISABLE);
#endif

    // Before enable the branch target buffer(BTB), the ECR bit 0 must be set
#if FA510_BTB
    CPUSetExtEnable(ENABLE);
    CPUSetBTBEnable(ENABLE);
#else
    CPUSetExtEnable(ENABLE);
    CPUSetBTBEnable(DISABLE);
#endif

#if FA510_D_CACHE
    CPUSetDCacheEnable(ENABLE);
#else
    CPUSetDCacheEnable(DISABLE);
#endif

#if FA510_ALIGNMENT_CHECK
    CPUSetExtEnable(ENABLE);
    CPUSetAlignCheckEnable(ENABLE);
#else
    CPUSetExtEnable(ENABLE);
    CPUSetAlignCheckEnable(DISABLE);
#endif

    // Enable MMU
    CPUSetMMUEnable(ENABLE);

/*-------- Clean and invalidate all cache --------*/
#if FA510_I_CACHE
    CPUInvalidateICacheAll();
#endif

#if FA510_D_CACHE
    CPUCleanInvalidateDCacheAll();
#endif

#if FA510_BTB
    CPUFlushBTBAll();
#endif

/*-------- Lock Critical Code And Data into cache --------*/
#if FA510_LOCK_CRITICAL_CACHE
    LockCriticalCodeAndData();
#endif
}   /* EnableCacheMPU() */

