
#ifndef UI_SYSTEMOBJ_H
#define UI_SYSTEMOBJ_H

#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIMenuSetupSettings.h"
#include "UISystemStatusSettings.h"


//-----------------------------------------------------------------------------
// Global Definitions
//-----------------------------------------------------------------------------
// Change DSC mode type
#define DSCMODE_CHGTO_NEXT              0
#define DSCMODE_CHGTO_PREV              1
#define DSCMODE_CHGTO_CURR              2

//-----------------------------------------------------------------------------
// Type Define
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// APIs
//-----------------------------------------------------------------------------
extern VControl UISystemObjCtrl;
extern EVENT_ENTRY UISystemObjCtrlCmdMap[];

extern UINT32   Get_BeepValue(UINT32 uiIndex);
extern UINT32   Get_VolumeValue(UINT32 uiIndex);
extern UINT32   Get_PowerOffValue(UINT32 uiIndex);
extern UINT32   Get_FreqValue(UINT32 uiIndex);
extern UINT32   Get_LanguageValue(UINT32 uiIndex);

//#NT#2010/10/25#Philex Lin -begin
// for Ramdisk
extern int SysGetCaptureAddr(UINT32 *addr);
extern int SysGetRamDiskAddr(UINT32 *addr);
//#NT#2010/10/25#Philex Lin -end

#endif

