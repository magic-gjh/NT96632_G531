#ifndef _MSDCVENDORSAMPLE_H
#define _MSDCVENDORSAMPLE_H
#include "UMSDC.h"

extern void MSDCVendorSample_Open(void);
extern void MSDCVendorSample_Close(void);
//#NT#2012/03/02#Niven Cho -begin
extern void MsdcNvt_Attach(USB_MSDC_INFO* pInfo);
//#NT#2012/03/02#Niven Cho -end

//#NT#2012/03/02#Philex Lin -begin
extern UINT32 MsdcNvt_GetSIModule(void);
//#NT#2012/03/02#Philex Lin -end

#endif