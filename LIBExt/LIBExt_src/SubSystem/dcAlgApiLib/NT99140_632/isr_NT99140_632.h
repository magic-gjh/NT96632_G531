#ifndef ISR_NT99140_632_H
#define ISR_NT99140_632_H

void sie_isrCB(UINT32 IntStatus);
void pre_isrCB(UINT32 IntStatus);
void ime_isrCB(UINT32 IntStatus);
void ipe_isrCB(UINT32 IntStatus);
//#NT#2009/11/27#Connie Yu -begin
//#NT#Add DIS process
void dis_isrCB(UINT32 IntStatus);
//#NT#2009/11/27#Connie Yu -end
extern volatile unsigned long cap_cnt;

#endif
