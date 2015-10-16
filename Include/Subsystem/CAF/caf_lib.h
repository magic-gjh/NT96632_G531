#ifndef _CAF_LIB_H
#define _CAF_LIB_H

//#NT#2010/02/22#Jarkko Chang -begin
typedef BOOL (*FPCALLBACK_GETSearchRange)(void);
//#NT#2010/02/22#Jarkko Chang -end
void caf_AutoFocusProcess(void);
//#NT#2009/02/14#Bowen Li -begin
void caf_setInitFlag(BOOL flag);
void caf_Lock(BOOL flag);
//#NT#2009/02/23#Bowen Li -begin
void caf_VAPprepare(BOOL flag);
//#NT#2009/02/23#Bowen Li -end
//#NT#2009/02/14#Bowen Li -end
//#NT#2009/02/28#Bowen Li -begin
UINT16 caf_GetFocusDist(void);
//#NT#2009/02/28#Bowen Li -end
UINT32 caf_ChkHist(UINT32 Percent, UINT32 Step);
BOOL caf_GetVAFlag(void);
#endif
