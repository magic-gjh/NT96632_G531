/**
    uITRON flag related header file.

    @file       Flag.h
    @ingroup    mIKernelFlag
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _FLG_H
#define _FLG_H

struct  flg
{
    struct tcb*             f_forw;     /*  0 Ptr to first waiting TCB         */
    struct tcb*             f_back;     /*  4 Ptr to last waiting TCB          */
    volatile UB             f_stat;     /*  8 Event flags status               */
    UB                      f_type;     /*  9 Structure type                   */
    UH                      f_rsvd;     /* 10 Reserved (for alignment)         */
    FLGPTN                  f_flgs;     /* 12 Event flags                      */
    struct tcb*             f_ownr;     /* 16 Lock owner TCB                   */
    FLGPTN                  f_setptn;   /* 20 Held set-pattern                 */
    FLGPTN                  f_clrptn;   /* 24 Held clear-pattern               */
};

/**
    @addtogroup mIKernelFlag
*/
//@{

extern void     kini_flg(void);
extern ER       icr_flg(UB flgid);
extern ER       set_flg(ID flgid, FLGPTN setptn);
extern ER       clr_flg(ID flgid, FLGPTN clrptn);
extern ER       wai_flg(PFLGPTN p_flgptn, ID flgid, FLGPTN waiptn, UINT wfmode);
extern FLGPTN   chk_flg(ID flgid, FLGPTN chkptn);

//@}

#endif
