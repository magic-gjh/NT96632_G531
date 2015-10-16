#ifndef _FLASHLIGHT_A1040_630_H_
#define _FLASHLIGHT_A1040_630_H_

//#NT#2010/09/10#ethanlau -begin
//#NT#flash enum
typedef enum
{
    Flash_off = 0,
    Flash_dummy = 1,
    Flash_Enable = 2,
    Flash_RedEye = 3
} _Flash_Mode;
//#NT#2010/09/10#ethanlau -end

extern UINT32 Flash_Rg;
extern UINT32 Flash_Bg;

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
