#include "Kernel.h"
#include "SysCfg.h"

// Ready queue
struct dcq          grdyque;

// Running tcb pointer
struct tcb*         gruntcb;

// Time queue
struct dlq          gtsvque;

// Free buf queue
struct buf*         gbuffre;

// Scheduling flag
volatile union sch  gschflg;

// Task
    struct tcb  gtcbarr[STD_TSK_NUM+USER_TSK_NUM];  // Array of tcb
    struct tcb* gtcbadr[STD_TSK_NUM+USER_TSK_NUM];  // Array of pointers to tcb

// Semaphore
    struct sem  gsemarr[STD_SEM_NUM+USER_SEM_NUM];  // Array of sem
    struct sem* gsemadr[STD_SEM_NUM+USER_SEM_NUM];  // Array of pointers to sem

// Flag
    struct flg  gflgarr[STD_EVF_NUM+USER_EVF_NUM];  // Array of flg
    struct flg* gflgadr[STD_EVF_NUM+USER_EVF_NUM];  // Array of pointers to flg

// Mailbox
    struct mbx  gmbxarr[STD_MBX_NUM+USER_MBX_NUM];  // Array of mbx
    struct mbx* gmbxadr[STD_MBX_NUM+USER_MBX_NUM];  // Array of pointers to mbx

// Buffer
    struct buf  gbufarr[BUF_NUM];                   // Array of buf

// Fixed memory pool
struct fpl  gfplarr[STD_FPL_NUM+USER_FPL_NUM];  // Array of fpl
struct fpl* gfpladr[STD_FPL_NUM+USER_FPL_NUM];  // Array of pointers to fpl

// Fixed memory pool start
VP          gfremem = (void*)FRE_MEM;

// Fixed memory pool end
VP          gendmem = (void*)END_MEM;

// Total interrupt source number
const int   gAllINTNums = INT_VEC_NUM;

// Total tcb number
const int   gtcbnum = STD_TSK_NUM + USER_TSK_NUM;

// Total sem number
const int   gsemnum = STD_SEM_NUM + USER_SEM_NUM;

// Total flg number
const int   gflgnum = STD_EVF_NUM + USER_EVF_NUM;

// Total mbx number
const int   gmbxnum = STD_MBX_NUM + USER_MBX_NUM;

// Total fpl number
const int   gfplnum = STD_FPL_NUM + USER_FPL_NUM;

// Total buf number
const int   gbufnum = BUF_NUM;

// ISR table
FP          ALL_INT_tbl[INT_VEC_NUM + 1];
