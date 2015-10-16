#ifndef _FLASHLIGHT_ZT3120_630_H_
#define _FLASHLIGHT_ZT3120_630_H_

//#NT#2009/10/16#Jarkko Chang -begin
extern UINT Flash_Rg,Flash_Gg,Flash_Bg;
//#NT#2009/10/16#Jarkko Chang -end
extern UINT flash_Charge(void);
extern void flash_PreFlashFlow(void);
void flash_Evaluate(void);
void flash_PreFlashExpSet(void);
void flash_RedEyeReduct(void);
void flash_ConfigPreFlash(void);
void flash_ConfigMainFlash(void);
void flash_TriggerFlash(void);
void flash_CtrlPinSelect(void);
void flash_AEConverge(UINT Yflash);

extern unsigned int flash_getCurAdcVal(void);

extern FLASH_STRUCT Flash_Param;
#endif
