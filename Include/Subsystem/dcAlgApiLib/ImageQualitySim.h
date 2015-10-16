#ifndef _IMAGE_QUALITY_SIM_H_
#define _IMAGE_QUALITY_SIM_H_

#include "type.h"

//#NT#2009/11/23#Linkin Huang -begin
//#NT# add for Simulation tool
//extern BOOL SimToolEn;
//#NT#2009/11/23#Linkin Huang -end

extern BOOL isExecutePhotoMode;

extern UINT IndepSliceGetJPGAddr(void);
extern UINT IndepSliceGetJPGSize(void);

extern UINT32 GetIndepAddr(void);
extern UINT32 Trigger_IndepSlice630(void);
extern void SimToolTest(void);

#endif
