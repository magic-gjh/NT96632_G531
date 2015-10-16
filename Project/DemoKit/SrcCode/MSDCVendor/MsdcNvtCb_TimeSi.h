#ifndef _MSDCNVTCB_TIMESI_H
#define _MSDCNVTCB_TIMESI_H

//APIs for MSDCVendorNVT_AddCallback_Si
BOOL TimeSi_Register_SiCall(BOOL (*fpMSDCVendorNVT_AddCallback_Si)(FP *fpTblGet,UINT8 nGets,FP *fpTblSet,UINT8 nSets));

//Macro for Regisetr Disp Callbacks
#define MSDCNVT_SICALL_REG_TIMESI() \
    TimeSi_Register_SiCall(MSDCVendorNVT_AddCallback_Si);

#endif