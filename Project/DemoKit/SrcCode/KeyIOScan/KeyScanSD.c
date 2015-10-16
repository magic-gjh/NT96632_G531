/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanSD.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect SD card status
                Detect SD card insert/remove, lock/unlock

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "SysCfg.h"
#include "PrimaryTsk.h"
#include "KeyScanInt.h"
#include "FilesysTsk.h"
#include "KeyScanTsk.h"
#include "UIFlow.h"
#include "GlobalVar.h"
#include "GPIOMapping.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UISystemStatusSettings.h"
#if (_STORAGECARD_ == _STORAGECARD_XD_)//#NT#2007/1/22 #CL Wang
#include "XD.h"
#include "NAND.h"
#elif (_STORAGECARD_ == _STORAGECARD_MSPRO_)
#include "MSPRO.h"
#elif (_STORAGECARD_ == _STORAGECARD_CF_)
#include "CF.h"
#elif (_STORAGECARD_ == _STORAGECARD_SDIO_)
#include "sdio.h"
#elif (_STORAGECARD_ == _STORAGECARD_SDIO2_)
#include "sdio.h"
#endif


BOOL g_bFilesysAbortCmd = FALSE;

static volatile UINT32  uiStrgCardStatus    = KEYSCAN_CARD_UNKNOWN;
static volatile UINT32  uiStrgMacroStatus   = KEYSCAN_MACRO_UNKNOWN;
static volatile BOOL    bStrgCardLock       = FALSE;
static volatile BOOL    bInternalStrgFirst       = FALSE;

extern UINT32 MacroShowFlag;

static BOOL g_b1stTimeStrgInit = TRUE;

#if (KEYSCAN_CARDDET_INT == ENABLE)
static volatile UINT32  uiStrgCardIntCnt  = 0;

/**
  ISR of SD card detection

  ISR of SD card detection

  @param void
  @return void
*/
static void KeyScan_DetStrgCardIsr(void)
{
    uiStrgCardIntCnt++;

    // Debounce
    if (uiStrgCardIntCnt > 1)
    {
        if (uiStrgCardStatus == KEYSCAN_CARD_REMOVED)
        {
            // Set interrupt type to level trigger, active high
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);

#if (_STORAGECARD_ == _STORAGECARD_XD_) //Change the signal level of SMC controller first when xD card inserted.
            xd_xDCardInserted();
#endif

            Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);
            uiStrgCardStatus = KEYSCAN_CARD_INSERTED;

#if (KEYSCAN_SDWRPTDET_GPIO == ENABLE)
            bStrgCardLock = GPIOMap_DetStrgCardWP();
#endif
        }
        else
        {
            // Set interrupt type to level trigger, active low
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);

#if (_STORAGECARD_ == _STORAGECARD_XD_) //Change the signal level of SMC controller first when xD card removed.
            xd_xDCardRemoved();
#endif
            Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);
            uiStrgCardStatus = KEYSCAN_CARD_REMOVED;
            bStrgCardLock = FALSE;
        }

        uiStrgCardIntCnt = 0;
    }
}
#endif

/**
  Detect SD card is inserted or not

  Detect SD card is inserted or not.
  If SD card is inserted, then detect SD card write protect status and
  store it in bStrgCardLock
  [KeyScan internal API]

  @param void
  @return void
*/
void KeyScan_DetStrgCard(void)
{
    static UINT32   uiStrgCardPrevSts = KEYSCAN_CARD_UNKNOWN;
    UINT32          uiStrgCardCurSts;

    if (bInternalStrgFirst==FALSE)
    {
        if (GPIOMap_DetStrgCard() == TRUE)
        {
            uiStrgCardCurSts = KEYSCAN_CARD_INSERTED;
        }
        else
        {
            uiStrgCardCurSts = KEYSCAN_CARD_REMOVED;
        }
    } else {
        uiStrgCardCurSts = KEYSCAN_CARD_REMOVED;
    }

    // Debounce
    if ((uiStrgCardCurSts == uiStrgCardPrevSts) &&
        (uiStrgCardCurSts != uiStrgCardStatus))
    {
        if(!KeyScan_IsHDMIPlugIn()&&(!KeyScan_IsTVPlugIn()))
        {
              if((Primary_GetCurrentMode()!= PRIMARY_MODE_USBMSDC)&&(Primary_GetCurrentMode()!= PRIMARY_MODE_USBPCC))
              	{
	        if(!GPIOMap_IsLCDBacklightOn())
	        {
			GPIOMap_TurnOnLCDBacklight();
		  }
              	}
        }    
        if (uiStrgCardCurSts == KEYSCAN_CARD_INSERTED)
        {
#if (KEYSCAN_CARDDET_INT == ENABLE)
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
#endif

#if (_STORAGECARD_ == _STORAGECARD_XD_) //Change the signal level of SMC controller first when xD card inserted.
            xd_xDCardInserted();
#endif
            bStrgCardLock = GPIOMap_DetStrgCardWP();

            SetFSStatus(FS_NOT_INIT);
            Primary_ChangeStorage(PRIMARY_STGTYPE_CARD);

#if (KEYSCAN_SDWRPTDET_GPIO == ENABLE)
            #if (_PCB_ == _PCB_EVB_)
            TimerDelayMs(2000);//wait WP detect ready,need to confirm
            #endif
            bStrgCardLock = GPIOMap_DetStrgCardWP();
#endif
            if (bStrgCardLock)
            {
                SetCardStatus(CARD_LOCKED);
            }
            else
            {
                SetCardStatus(CARD_INSERTED);
            }

            debug_err(("Storage Card inserted!\r\n"));
        }
        else
        {
#if (KEYSCAN_CARDDET_INT == ENABLE)
            gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
#endif

#if (_STORAGECARD_ == _STORAGECARD_XD_) //Change the signal level of SMC controller first when xD card removed.
            xd_xDCardRemoved();
#endif

            SetFSStatus(FS_NOT_INIT);
            Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);

            bStrgCardLock = FALSE;

            SetCardStatus(CARD_REMOVED);

            debug_err(("Storage Card removed!\r\n"));
        }


        if (uiStrgCardStatus != KEYSCAN_CARD_UNKNOWN)
        {
#if (KEYSCAN_CARD_HOTPLUG_FUNC == ENABLE)
            #if (_MP3_MODE_ == ENABLE)
            // Set mp3 filetable renew request to on (coz current storage is changed)
            Set_MP3FtbRenewIndex(MP3FTB_RENEW_ON);
            #endif

            // Open storage change notice window to wait NVTEVT_STORAGE_CHANGE evt
            // Then utilize mode change (to current mode) to re-init current mode and ui window
            FlowWrnStrgHotPlug_Open();
#else
            //power off while insert or remove card
            FilesysAbortCmd(FST_CMD_CLOSE_FILE      );
            FilesysAbortCmd(FST_CMD_DELETE_FILE     );
            FilesysAbortCmd(FST_CMD_FORMAT          );
            FilesysAbortCmd(FST_CMD_STORAGE_COPY_TO );
            g_bFilesysAbortCmd = TRUE;

            //set fst done for PB_WAIT_INFINITE
            set_flg(FLG_ID_FILESYS, FLGFILESYS_DONE);
            KeyScan_PoweroffFlow();
#endif
        }

        uiStrgCardStatus = uiStrgCardCurSts;

#if (KEYSCAN_CARDDET_INT == ENABLE)
        gpio_setIntIsr(KEYSCAN_CD_GPIO_INT, KeyScan_DetStrgCardIsr);
        gpio_enableInt(KEYSCAN_CD_GPIO_INT);
#endif
    }

    uiStrgCardPrevSts = uiStrgCardCurSts;
}

/**
  Storage callback function

  Storage (file system) initialization callback function.
  You can know the status of file system initialization.

  @param UINT32 uiStatus: The status of file system initialization
    FST_STA_OK              (Initialization OK)
    FLGKEY_STG_INITFAIL     (Unknown format, FAT error...)
    FLGKEY_STG_DISKERR      (Storage error)
  @return void
*/
void KeyScan_StorageCB(UINT32 uiStatus)
{
    switch (uiStatus)
    {
        case FST_STA_OK:
            debug_msg("FST_STA_OK!\r\n");
            SetFSStatus(FS_INIT_OK);
            break;
        case FST_STA_DISK_UNFORMAT:
            debug_msg("FST_STA_DISK_UNFORMAT!\r\n");
            SetFSStatus(FS_UNFORMATTED);
            break;
        case FST_STA_DISK_UNKNOWN_FORMAT:
            debug_msg("FST_STA_DISK_UNKNOWN_FORMAT!\r\n");
            SetFSStatus(FS_UNKNOWN_FORMAT);
            break;
        case FST_STA_CARD_ERR:
            debug_msg("FST_STA_CARD_ERR!\r\n");
            SetFSStatus(FS_DISK_ERROR);
            break;
        case FST_STA_OUT_OF_ID:
            debug_msg("FST_STA_OUT_OF_ID!\r\n");
            SetFSStatus(FS_INIT_OK);
            break;
        default:
            break;
    }

    if(g_b1stTimeStrgInit)
    {
        Ux_PostEvent(NVTEVT_STORAGE_INIT, 0);
        g_b1stTimeStrgInit = FALSE;
    }
    else
    {
        //We must make sure UIFlowWndWrnStrgHotPlug is opened!!!
        TimerDelayMs(200);
        Ux_PostEvent(NVTEVT_STORAGE_CHANGE, 0);
    }
}

/**
  Get SD card lock status

  Get SD card lock status

  @param void
  @return BOOL: TRUE -> Card is locked, FALSE -> Card is not locked
*/
BOOL KeyScan_IsStrgCardLocked(void)
{
    return bStrgCardLock;
}

void Keyscan_SetInternalFirst(BOOL bFirst)
{
    bInternalStrgFirst = bFirst;
}

BOOL Keyscan_GetInternalFirst(void)
{
    return bInternalStrgFirst ;
}

//@}
