#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "UISystemStatusSettings.h"
#include "UIMenuPhotoSettings.h"
#include "UIMenuSetupSettings.h"
#include "UIFlowPhotoFuncs.h"
#include "UIPhotoObj.h"
#include "UIPlayObj.h"
#include "UIMovieObj.h"
#include "UISystemObj.h"
#include "Debug.h"
#include "SysCfg.h"
#include "FwVersion.h"
#include "UICfgDefault.h"
#include "PrimaryTsk.h"
#include "AviRecAPI.h"
#include "MediaRecAPI.h"


static UINT32 guiBatteryLevel       = UIDFT_BATTERY_LVL;
static UINT32 guiCardStatus         = UIDFT_CARD_STATUS;
static UINT32 guiFsStatus           = UIDFT_FS_STATUS;
static UINT32 guiMP3FtbRenewIndex   = UIDFT_MP3FTB_RENEW;

// Version
static CHAR gMainVer[]              = DSC_FW_VERSION_STR;
static CHAR gVersion[MAX_VER_STR_LEN];

// Current menu info
static UIMenuStoreInfo gCurrentMenuInfo;

// Must in the same order of those in enum FS_STATUS
static const CHAR *strFSStatusTable[FS_STATUS_TOTAL_NUM] =
{
    "FS NOT INIT",
    "FS INIT OK",
    "FS UNFORMATTED",
    "FS UNKNOWN FORMAT",
    "FS DISK ERROR"
};

// Bootloader info
BLINFO_OBJ gBLInfo;
extern BOOL GPSSetTime;

//-----------------------------------------------------------------------------
// USB serial string desc3
//-----------------------------------------------------------------------------
void SetUSBSerialStringDesc3(void)
{
    // generate USB Serial string desc3
    USBMakerInit_GenRandomSerialStrDesc3();

    // get desc string pointer
    memcpy((void *)gCurrentMenuInfo.USBSerialStrDesc3,(void *)USBMakerInit_GetDesc3String(),sizeof(gCurrentMenuInfo.USBSerialStrDesc3));
}

UINT16 *GetUSBSerialStringDesc3(void)
{
    return (UINT16 *)gCurrentMenuInfo.USBSerialStrDesc3;
}

//-----------------------------------------------------------------------------
// Firmware Version String
//-----------------------------------------------------------------------------
CHAR *GetVersionString(void)
{
    sprintf(gVersion, "%s", gMainVer);
    return gVersion;
}

//-----------------------------------------------------------------------------
// Card Status
//-----------------------------------------------------------------------------
void SetCardStatus(UINT32 uiStatus)
{
    guiCardStatus = uiStatus;
}
UINT32 GetCardStatus(void)
{
    return guiCardStatus;
}

//-----------------------------------------------------------------------------
// FS Status
//-----------------------------------------------------------------------------
void SetFSStatus(UINT32 uiStatus)
{
    guiFsStatus = uiStatus;
    debug_ind(("[SetFSStatus]%d\r\n", guiFsStatus));
}
UINT32 GetFSStatus(void)
{
    debug_ind(("[GetFSStatus]%d\r\n", guiFsStatus));
    return guiFsStatus;
}
const CHAR *GetFSStatusString(UINT32 uiStatus)
{
    if(uiStatus < FS_STATUS_TOTAL_NUM)
        return strFSStatusTable[uiStatus];
    else
        return NULL;
}

//-----------------------------------------------------------------------------
// Battery Level
//-----------------------------------------------------------------------------
void SetBatteryLevel(UINT32 uiStatus)
{
    guiBatteryLevel = uiStatus;
}
UINT32 GetBatteryLevel(void)
{
    return guiBatteryLevel;
}

//-----------------------------------------------------------------------------
// MP3 FileTable Renew
//-----------------------------------------------------------------------------
void Set_MP3FtbRenewIndex(UINT32 uiIndex)
{
    guiMP3FtbRenewIndex = uiIndex;
}
UINT32 Get_MP3FtbRenewIndex(void)
{
    return guiMP3FtbRenewIndex;
}

//-----------------------------------------------------------------------------
// Bootloader Info
//-----------------------------------------------------------------------------
void Init_BLInfo(void)
{
    memcpy((void *)&gBLInfo, (void *)BLINFO_START_ADDR, sizeof(BLINFO_OBJ));

    debug_err(("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n"));
    debug_err(("Bootloader Info\r\n"));
    debug_err(("BL Name     : %s\r\n", gBLInfo.strBLName));
    debug_err(("FW Name     : %s\r\n", gBLInfo.strFWName));
    debug_err(("BL Ver      : %s\r\n", gBLInfo.strBLVer));
    debug_err(("BL Date     : %s\r\n", gBLInfo.strBLDate));
    debug_err(("BL Time     : %s\r\n", gBLInfo.strBLTime));
    debug_err(("DRAM Type   : %s\r\n", gBLInfo.strDramType));
    debug_err(("DRAM Size   : %s\r\n", gBLInfo.strDramSize));
    debug_err(("Storage Int : %s\r\n", gBLInfo.strStorageInt));
    debug_err(("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n"));
}
void Get_BLInfo(PBLINFO_OBJ pBLInfo)
{
/*
    if(pBLInfo == NULL)
    {
        return;
    }
    memcpy((void *)pBLInfo, (void *)&gBLInfo, sizeof(BLINFO_OBJ));
    */
    pBLInfo=(void *)&gBLInfo;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Validate Storage
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//#NT#2011/03/25#Chris Chung -begin
//#NT#remove useless DCIM check
#if 0
BOOL IsDCIMWritable(void)
{
    CHAR   DCIM_DIR[12] = "A:\\DCIM";
    UINT32 uiStatus = 0;
    UINT8  ucAttrib = 0;
    BOOL   ret = FALSE;

    // Make sure filesystem is ready.
    FilesysWaitCmdFinish(FST_TIME_INFINITE);

    uiStatus = FilesysGetAttribByName(DCIM_DIR, &ucAttrib);
    if(uiStatus == FST_STA_OK)
    {
        if(ucAttrib & FS_ATTRIB_DIRECTORY)
        {
            if (GetCardStatus() == CARD_REMOVED)
                debug_ind(("IsDCIMWritable: SD Card is removed, DCIM is located on NAND.\r\n"));

            if(ucAttrib & FS_ATTRIB_READ)
            {
                debug_err(("IsDCIMWritable: A:\\DCIM Directory is READONLY.\r\n"));
            }
            else
            {
                debug_ind(("IsDCIMWritable: A:\\DCIM Directory is existing and writable.\r\n"));
                ret = TRUE;
            }
        }
        else
            debug_err(("IsDCIMWritable: A:\\DCIM is not existing.\r\n"));
    }
    return ret;
}
#endif
//#NT#2011/03/25#Chris Chung -end

BOOL UI_Validate_Storage(UINT32 uiCheckItem, UINT32 *pFreeCount)
{
    UINT16 uhFSStatusId = 0;
    UINT32 uiCountTmp = 0;
    BOOL bIsValid = TRUE;
    FILE_TSK_DISK_INFO *pDiskInfo;

    debug_ind(("UI_Validate_Storage: Check item = 0x%x\r\n", uiCheckItem));

    //#NT#2009/12/08#Chris Chung -begin
    //#NT#It should wait FS init finish to get correct FS status.
    FilesysWaitInitFinish(FST_TIME_INFINITE);
    //#NT#2009/12/08#Chris Chung -end

    switch(uiCheckItem)
    {
        case STORAGE_CHECK_FULL:
            uhFSStatusId = GetFSStatus();
            if(uhFSStatusId == FS_INIT_OK)
            {
                FST_DCF_INFO *pDCFInfo = FilesysGetDCFInfo();

                if(pDCFInfo->MaxDirID == MAX_DCF_DIR_NUM)
                {
                    //#NT#2010/06/02#Chris Chung -begin
                    //#NT#Use FS API instead of PB API since photo/movie mode will use this funciton.
                    UH dirId = MAX_DCF_DIR_NUM, fileId = MAX_DCF_FILE_NUM, fileFmt = FST_FMT_ANYFORMAT;

                    debug_err(("UI_Validate_Storage: The 999DIR is existing!\r\n"));
                    if(FilesysQueryID(FST_CMD_QUERY_SPECI_ID, &dirId, &fileId, &fileFmt, FALSE, 0) == FST_STA_OK)
                    {
                        debug_err(("UI_Validate_Storage: 999-9999 is existing!\r\n"));
                        bIsValid = FALSE;
                        break;
                    }
                    //#NT#2010/06/02#Chris Chung -end
                }

                //#NT#2008/03/20#Ryan Ho -begin
                //If Directory 999 is existing and ReadOnly, the storage is thought as CARDFULL
                //If Directory 999 is existing and dir name is not 999MEDIA, the storage is thought as CARDFULL
                if(pDCFInfo->MaxDirID == MAX_DCF_DIR_NUM)
                {
                    CHAR FolderName[6], FullFolderName[19];
                    UINT32 uiStatus = 0;
                    UINT8 ucAttrib = 0;

                    FilesysGetDCFFolderName(FolderName, MAX_DCF_DIR_NUM);
                    if(strncmp(FolderName, "MEDIA", 5) != 0)
                    {
                        debug_err(("UI_Validate_Storage: The 999DIR is %s, no valid DCF folder id avaliable!\r\n", FolderName));
                        bIsValid = FALSE;
                        break;
                    }
                    sprintf(FullFolderName, "%s%3d%s\0", "A:\\DCIM\\", MAX_DCF_DIR_NUM, FolderName);

                    uiStatus = FilesysGetAttribByName(FullFolderName ,&ucAttrib);
                    if(ucAttrib & FS_ATTRIB_READ)
                    {
                        debug_err(("UI_Validate_Storage: %s is READONLY.\r\n", FullFolderName));
                        bIsValid = FALSE;
                        break;
                    }
                }
                //#NT#2008/03/20#Ryan Ho -end

                /* Check storage free space */
                switch(Primary_GetCurrentMode())
                {
                    case PRIMARY_MODE_PHOTO:
                        /* Get current free picture number */
                        uiCountTmp = Get_FreePicNum(Get_SizeIndex(), Get_QualityIndex());
                        if(uiCountTmp == PHOTO_PIC_NUM_MIN)
                        {
                            debug_err(("UI_Validate_Storage: STORAGE FULL!\r\n"));
                            bIsValid = FALSE;
                        }
                        break;

#if 0
                    case DSC_MODE_PHOTO_PANORAMA:
                        /* Get current free picture number (Note: Panorama mode fixed each image size to 1M) */
                        uiCountTmp = Get_FreePicNum(SIZE_2M, Get_QualityIndex());
                        if(uiCountTmp < PANO_PIC_NUM_MIN)
                        {
                            debug_err(("UI_Validate_Storage: STORAGE LESS THAN PANORAMA REQUIREMENTS!\r\n"));
                            bIsValid = FALSE;
                        }
                        break;
#endif

                    case PRIMARY_MODE_MOVIE:
                        /* Get current free recording time */
                        //uiCountTmp = MediaRec_Disk2Second();
                        MediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_INDEXBUF,0);
                        uiCountTmp = MediaRec_Disk2SecondWithType(MEDIAREC_COUNTTYPE_FS,0);//AVIRec_Disk2Second();
                        if(uiCountTmp < MOVIE_REC_TIME_MIN)
                        {
                            debug_err(("UI_Validate_Storage: STORAGE FULL!\r\n"));
                            bIsValid = FALSE;
                        }
                        break;

                    default:
                        break;
                }

                /* Update specific free count variable */
                if(pFreeCount)
                    *pFreeCount = uiCountTmp;
            }
            else
            {
                debug_err(("UI_Validate_Storage: STORAGE ERR! - %s (0x%x)\r\n", GetFSStatusString(uhFSStatusId), uhFSStatusId));
            }
            break;

        case STORAGE_CHECK_LOCKED:
            //#NT#2011/03/25#Chris Chung -begin
            //#NT#remove useless DCIM check
            #if 0
            if((GetCardStatus() == CARD_LOCKED) || (IsDCIMWritable() == FALSE))
            #else
            if(GetCardStatus() == CARD_LOCKED)
            #endif
            //#NT#2011/03/25#Chris Chung -end
            {
                debug_err(("UI_Validate_Storage: CARD LOCKED!\r\n"));
                bIsValid = FALSE;
            }
            break;

        case STORAGE_CHECK_ERROR:
            uhFSStatusId = GetFSStatus();
            if(uhFSStatusId != FS_INIT_OK)
            {
                debug_err(("UI_Validate_Storage: STORAGE ERR! - %s (0x%x)\r\n", GetFSStatusString(uhFSStatusId), uhFSStatusId));
                bIsValid = FALSE;
            }
            break;

        case STORAGE_CHECK_SPACE_FULL:

            pDiskInfo = FilesysGetDiskInfo();

            if(pDiskInfo->FreeSpaceSizeHi == 0 &&
               pDiskInfo->FreeSpaceSize < (pDiskInfo->ClusterSize*STORAGE_RESERVED_CLUS_NUM))
            {
                bIsValid = FALSE;
            }
            break;

        case STORAGE_CHECK_CLUSTER:

            pDiskInfo = FilesysGetDiskInfo();
            if (pDiskInfo->ClusterSize < 0x8000) // card's cluster size is smaller than 32KB
            {
                debug_msg("%s,cluster_size=%x\r\n",__func__,pDiskInfo->ClusterSize);
                bIsValid = FALSE;
            }
            break;


        default:
            debug_err(("UI_Validate_Storage: Unknown storage check item 0x%x\r\n", uiCheckItem));
            break;
    }

    return bIsValid;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Init Menu Info
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Init_MenuInfo(void)
{
    PPSTORE_SECTION_HANDLE  pSection;

    // Read system info from PStore
    if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)&gCurrentMenuInfo, 0, sizeof(UIMenuStoreInfo), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        debug_err(("[Init_MenuInfo][PStore Read sys param fail]\r\n"));
    }

    // Check FW Version
    if((strncmp(gCurrentMenuInfo.strFwVersion, GetVersionString(), MAX_VER_STR_LEN) != 0) ||
        (gCurrentMenuInfo.uiInfoSize != sizeof(gCurrentMenuInfo)))
    {
        // If different, do system reset
        SysReset_MenuInfo();
    }
    else
    {
        // If identical, init UI settings and corresponding functions
        /*--- Photo ---*/
        if(gCurrentMenuInfo.uiQualityIndex >= QUALITY_SETTING_MAX)
            gCurrentMenuInfo.uiQualityIndex = UIDFT_QUALITY;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_QUALITY,1,gCurrentMenuInfo.uiQualityIndex);

        if(gCurrentMenuInfo.uiISOIndex >= ISO_SETTING_MAX)
            gCurrentMenuInfo.uiISOIndex = UIDFT_ISO;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_ISO,1,gCurrentMenuInfo.uiISOIndex);

        if(gCurrentMenuInfo.uiEVIndex >= EV_SETTING_MAX)
            gCurrentMenuInfo.uiEVIndex = UIDFT_EV;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_EV,1,gCurrentMenuInfo.uiEVIndex);


        if(gCurrentMenuInfo.uiWBIndex >= WB_SETTING_MAX)
            gCurrentMenuInfo.uiWBIndex = UIDFT_WB;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,1,gCurrentMenuInfo.uiWBIndex);

        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_COLOR,1,UIDFT_COLOR);

        if(gCurrentMenuInfo.uiMeteringIndex >= METERING_SETTING_MAX)
            gCurrentMenuInfo.uiMeteringIndex = UIDFT_METERING;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_METERING,1,gCurrentMenuInfo.uiMeteringIndex);

        if(gCurrentMenuInfo.uiSharpnessIndex >= SHARPNESS_SETTING_MAX)
            gCurrentMenuInfo.uiSharpnessIndex = UIDFT_SHARPNESS;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SHARPNESS,1,gCurrentMenuInfo.uiSharpnessIndex);

        if(gCurrentMenuInfo.uiSaturationIndex >= SATURATION_SETTING_MAX)
            gCurrentMenuInfo.uiSaturationIndex = UIDFT_SATURATION;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SATURATION,1,gCurrentMenuInfo.uiSaturationIndex);

        if(gCurrentMenuInfo.uiFDIndex >= FD_SETTING_MAX)
            gCurrentMenuInfo.uiFDIndex = UIDFT_FD;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_FD,1,gCurrentMenuInfo.uiFDIndex);

        if(gCurrentMenuInfo.uiAntiShakingIndex >= ANTISHAKING_SETTING_MAX)
            gCurrentMenuInfo.uiAntiShakingIndex = UIDFT_ANTISHAKING;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_ANTISHAKING,1,gCurrentMenuInfo.uiAntiShakingIndex);

        if(gCurrentMenuInfo.uiQuickReviewIndex >= QREVIEW_SETTING_MAX)
            gCurrentMenuInfo.uiQuickReviewIndex = UIDFT_QREVIEW;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_QUICKREVIEW,1,gCurrentMenuInfo.uiQuickReviewIndex);

        if(gCurrentMenuInfo.uiDateImprintIndex >= DATEIMPRINT_SETTING_MAX)
            gCurrentMenuInfo.uiDateImprintIndex = UIDFT_DATEIMPRINT;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_DATEIMPRINT,1,gCurrentMenuInfo.uiDateImprintIndex);

        if(gCurrentMenuInfo.uiSizeIndex >= SIZE_SETTING_MAX)
            gCurrentMenuInfo.uiSizeIndex = UIDFT_PHOTO_SIZE;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SIZE,1,gCurrentMenuInfo.uiSizeIndex);

        //Notice: Scene config must be set after all Photo related settings!!!
        //if(gCurrentMenuInfo.uiSceneIndex >= SCENE_SETTING_MAX)
        gCurrentMenuInfo.uiSceneIndex = UIDFT_SCENE;
        Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SCENEMODE,1,gCurrentMenuInfo.uiSceneIndex);

        Set_FilesysDirId(gCurrentMenuInfo.uiDirNumber);
        Set_FilesysFileId(gCurrentMenuInfo.uiFileNumber);

        /*--- Movie ---*/
        if(gCurrentMenuInfo.uiAVISizeIndex >= MOVIE_SIZE_SETTING_MAX)
            gCurrentMenuInfo.uiAVISizeIndex = UIDFT_MOVIE_SIZE;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIESIZE,1,gCurrentMenuInfo.uiAVISizeIndex);

        if(gCurrentMenuInfo.uiAVIQualityIndex >= MOVIE_QUALITY_SETTING_MAX)
            gCurrentMenuInfo.uiAVIQualityIndex = UIDFT_MOVIE_QUALITY;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_QUALITY,1,gCurrentMenuInfo.uiAVIQualityIndex);

        if(gCurrentMenuInfo.uiAVIColorIndex >= MOVIE_COLOR_SETTING_MAX)
            gCurrentMenuInfo.uiAVIColorIndex = UIDFT_MOVIE_COLOR;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_COLOR,1,gCurrentMenuInfo.uiAVIColorIndex);

        if(gCurrentMenuInfo.uiMovieDISIndex >= MOVIE_DIS_SETTING_MAX)
            gCurrentMenuInfo.uiMovieDISIndex = UIDFT_MOVIE_DIS;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIEDIS,1,gCurrentMenuInfo.uiMovieDISIndex);

        if(gCurrentMenuInfo.uiMovieDateImprintIndex >= MOVIE_DATEIMPRINT_SETTING_MAX)
            gCurrentMenuInfo.uiMovieDateImprintIndex = UIDFT_MOVIE_DATEIMPRINT;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_DATEIMPRINT,1,gCurrentMenuInfo.uiMovieDateImprintIndex);

        if(gCurrentMenuInfo.uiMovieAudioOnIndex >= MOVIE_AUDIO_SETTING_MAX)
            gCurrentMenuInfo.uiMovieAudioOnIndex = UIDFT_MOVIE_AUDIO;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_AUDIO_SET,1,gCurrentMenuInfo.uiMovieAudioOnIndex);

        if(gCurrentMenuInfo.uiMovieMotionDetIndex >= MOVIE_MOTIONDET_SETTING_MAX)
            gCurrentMenuInfo.uiMovieMotionDetIndex = UIDFT_MOVIE_MOTIONDET;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOTION_DET,1,gCurrentMenuInfo.uiMovieMotionDetIndex);

        if(gCurrentMenuInfo.uiMovieCyclicRecIndex >= MOVIE_CYCLIC_REC_SETTING_MAX)
            gCurrentMenuInfo.uiMovieCyclicRecIndex = UIDFT_MOVIE_CYCLIC_REC;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_CYCLIC_REC,1,gCurrentMenuInfo.uiMovieCyclicRecIndex);

        if(gCurrentMenuInfo.uiMovieGodBlessIndex >= MOVIE_GODBLESS_SETTING_MAX)
            gCurrentMenuInfo.uiMovieGodBlessIndex = UIDFT_MOVIE_GODBLESS;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_GOD_BLESS,1,gCurrentMenuInfo.uiMovieGodBlessIndex);

        if(gCurrentMenuInfo.uiMovieGolfShotIndex >= MOVIE_GOLFSHOT_SETTING_MAX)
            gCurrentMenuInfo.uiMovieGolfShotIndex = UIDFT_MOVIE_GOLF_SHOT;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_GOLF_SHOT,1,gCurrentMenuInfo.uiMovieGolfShotIndex);

        if(gCurrentMenuInfo.uiMovieFlashRecIndex >= MOVIE_FLASHREC_SETTING_MAX)
            gCurrentMenuInfo.uiMovieFlashRecIndex = UIDFT_MOVIE_FLASH_REC;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_FLASH_REC,1,gCurrentMenuInfo.uiMovieFlashRecIndex);


        if(gCurrentMenuInfo.uiMovieGsensorIndex>= MOVIE_GSENSOR_SETTING_MAX)
            gCurrentMenuInfo.uiMovieGsensorIndex = UIDFT_MOVIE_GSENSOR;
        Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_GSENSOR,1,gCurrentMenuInfo.uiMovieGsensorIndex);

        /*--- Setup ---*/
        if(gCurrentMenuInfo.uiLanguageIndex >= LANGUAGE_SETTING_MAX)
            gCurrentMenuInfo.uiLanguageIndex = UIDFT_LANGUAGE;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LANGUAGE,1,gCurrentMenuInfo.uiLanguageIndex);

        if(gCurrentMenuInfo.uiTVModeIndex >= TV_MODE_SETTING_MAX)
            gCurrentMenuInfo.uiTVModeIndex = UIDFT_TV_MODE;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_TVMODE,1,gCurrentMenuInfo.uiTVModeIndex);

        if(gCurrentMenuInfo.uiPowerOffIndex >= POWEROFF_SETTING_MAX)
            gCurrentMenuInfo.uiPowerOffIndex = UIDFT_POWEROFF;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_POWEROFF,1,gCurrentMenuInfo.uiPowerOffIndex);

        if(gCurrentMenuInfo.uiBeepIndex >= BEEP_SETTING_MAX)
            gCurrentMenuInfo.uiBeepIndex = UIDFT_BEEP;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_BEEP,1,gCurrentMenuInfo.uiBeepIndex);

        if(gCurrentMenuInfo.uiVolumeIndex >= VOLUME_SETTING_MAX)
            gCurrentMenuInfo.uiVolumeIndex = UIDFT_VOLUME;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_VOLUME,1,gCurrentMenuInfo.uiVolumeIndex);

        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DISPLAY,1,UIDFT_DISPLAY);

        if(gCurrentMenuInfo.uiFreqIndex >= FREQ_SETTING_MAX)
            gCurrentMenuInfo.uiFreqIndex = UIDFT_FREQUENCY;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_FREQ,1,gCurrentMenuInfo.uiFreqIndex);

        if(gCurrentMenuInfo.uiDateFormat >= DATE_FORMAT_SETTING_MAX)
            gCurrentMenuInfo.uiDateFormat = UIDFT_DATE_FORMAT;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DATEFORMAT,1,gCurrentMenuInfo.uiDateFormat);

        if(gCurrentMenuInfo.uiBrightIndex >= LCDBRT_LVL_SETTING_MAX)
            gCurrentMenuInfo.uiBrightIndex = UIDFT_LCDBRIGHTNESS;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_BRIGHT,1,gCurrentMenuInfo.uiBrightIndex);

        if(gCurrentMenuInfo.uiOpening >= OPENING_SETTING_MAX)
            gCurrentMenuInfo.uiOpening = UIDFT_OPENING;
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_OPENING,1,gCurrentMenuInfo.uiOpening);

	if(gCurrentMenuInfo.uiLcdOffIndex>= LCDOFF_MAX)
            gCurrentMenuInfo.uiLcdOffIndex = UIDFT_LCD_OFF;		
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LCDOFF,1,gCurrentMenuInfo.uiLcdOffIndex);

	if(gCurrentMenuInfo.uiDelayPowerOffIndex>= DELAY_POWER_OFF_MAX)
            gCurrentMenuInfo.uiLcdOffIndex = DELAY_POWER_OFF_OFF;		
        Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DELAYPOWEROFF,1,gCurrentMenuInfo.uiDelayPowerOffIndex);


	//if(gCurrentMenuInfo.uiRotateIndex>= IMAGE_ROTATE_MAX)
            //gCurrentMenuInfo.uiRotateIndex = IMAGE_ROTATE_OFF;	
	    //Set_ImageRotateIndex(gCurrentMenuInfo.uiRotateIndex);
    //Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_IMAGEROTATE,1,Get_ImageRotateIndex());

	if(gCurrentMenuInfo.uiACCOffIndex>= ACC_OFF_POWEROFF_MAX)
            gCurrentMenuInfo.uiACCOffIndex = ACC_OFF_POWEROFF_FALSE;
	Set_ACCPowerOffIndex(gCurrentMenuInfo.uiACCOffIndex);

	
        /*--- Print ---*/
        //Rsvd

        #if (_TouchPanel_ != DISABLE)
        if (gCurrentMenuInfo.bIsTouchCal == TRUE)
        {
            Set_TPCalFlag(gCurrentMenuInfo.bIsTouchCal);
            Gesture_SetCalData(&(gCurrentMenuInfo.GestureCaliData));
        }
        #endif
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Save Menu Info
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Save_MenuInfo(void)
{
    PPSTORE_SECTION_HANDLE  pSection;

    GPIOMap_SetSPIWp(TRUE);	

    // Read system info from PStore
    if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)&gCurrentMenuInfo, 0, sizeof(UIMenuStoreInfo), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        debug_err(("[Init_MenuInfo][PStore Read sys param fail]\r\n"));
    }

    /*--- General ---*/
    strncpy(gCurrentMenuInfo.strFwVersion, GetVersionString(), (MAX_VER_STR_LEN - 1));
    gCurrentMenuInfo.strFwVersion[strlen(gCurrentMenuInfo.strFwVersion)] = '\0';
    gCurrentMenuInfo.uiInfoSize                 =   sizeof(UIMenuStoreInfo);

    /*--- Photo ---*/
    gCurrentMenuInfo.uiSizeIndex                =   Get_SizeIndex();
    gCurrentMenuInfo.uiQualityIndex             =   Get_QualityIndex();
    gCurrentMenuInfo.uiSceneIndex               =   Get_SceneModeIndex();
    gCurrentMenuInfo.uiISOIndex                 =   Get_ISOIndex();
    gCurrentMenuInfo.uiEVIndex                  =   Get_EVIndex();
    gCurrentMenuInfo.uiWBIndex                  =   Get_WBIndex();
    gCurrentMenuInfo.uiMeteringIndex            =   Get_MeteringIndex();
    gCurrentMenuInfo.uiSharpnessIndex           =   Get_SharpnessIndex();
    gCurrentMenuInfo.uiSaturationIndex          =   Get_SaturationIndex();
    gCurrentMenuInfo.uiFDIndex                  =   Get_FDIndex();
    gCurrentMenuInfo.uiAntiShakingIndex         =   Get_AntiShakingIndex();
    gCurrentMenuInfo.uiQuickReviewIndex         =   Get_QuickReviewIndex();
    gCurrentMenuInfo.uiDateImprintIndex         =   Get_DateImprintIndex();
    gCurrentMenuInfo.uiDirNumber                =   Get_FilesysDirId();
    gCurrentMenuInfo.uiFileNumber               =   Get_FilesysFileId();

    /*--- Movie ---*/
    gCurrentMenuInfo.uiAVISizeIndex             =   Get_MovieSizeIndex();
    gCurrentMenuInfo.uiAVIQualityIndex          =   Get_MovieQualityIndex();
    gCurrentMenuInfo.uiAVIColorIndex            =   Get_MovieColorIndex();
    gCurrentMenuInfo.uiMovieDISIndex            =   Get_MovieDISIndex();
    gCurrentMenuInfo.uiMovieDateImprintIndex    =   Get_MovieDateImprintIndex();
    gCurrentMenuInfo.uiMovieAudioOnIndex        =   Get_MovieAudioIndex();
    gCurrentMenuInfo.uiMovieMotionDetIndex      =   Get_MovieMotionDetIndex();
    gCurrentMenuInfo.uiMovieGodBlessIndex       =   Get_MovieGodBlessIndex();
    gCurrentMenuInfo.uiMovieCyclicRecIndex      =   Get_MovieCyclicRecIndex();
    gCurrentMenuInfo.uiMovieGolfShotIndex       =   Get_MovieGolfShotIndex();
    gCurrentMenuInfo.uiMovieFlashRecIndex       =   Get_MovieFlashRecIndex();
    gCurrentMenuInfo.uiMovieGsensorIndex       =   Get_MovieGsensorIndex();
    /*--- Setup ---*/
    gCurrentMenuInfo.uiBeepIndex                =   Get_BeepIndex();
    gCurrentMenuInfo.uiVolumeIndex              =   Get_VolumeIndex();
    gCurrentMenuInfo.uiLanguageIndex            =   Get_LanguageIndex();
    gCurrentMenuInfo.uiTVModeIndex              =   Get_TVModeIndex();
    gCurrentMenuInfo.uiPowerOffIndex            =   Get_PowerOffIndex();
    gCurrentMenuInfo.uiFreqIndex                =   Get_FreqIndex();
    gCurrentMenuInfo.uiBrightIndex              =   Get_BrightIndex();
    gCurrentMenuInfo.uiDateFormat               =   Get_DateFormatIndex();
    gCurrentMenuInfo.uiOpening                  =   Get_OpeningIndex();
	gCurrentMenuInfo.uiLcdOffIndex	=   Get_LcdOffIndex();
	gCurrentMenuInfo.uiDelayPowerOffIndex=   Get_DelayPowerOffIndex();
	gCurrentMenuInfo.uiRotateIndex=   Get_ImageRotateIndex();
	gCurrentMenuInfo.uiACCOffIndex = Get_ACCPowerOffIndex();
	

    /*--- Print ---*/
    //Rsvd

    /*--- TouchPanel ---*/
    #if (_TouchPanel_ != DISABLE)
    gCurrentMenuInfo.bIsTouchCal = Get_TPCalFlag();
    Gesture_GetCalData(&(gCurrentMenuInfo.GestureCaliData));
    #endif
    /*--- init USB serial string desc3 ---*/
    //SetUSBSerialStringDesc3();

    //Using PStore to read / sys param
    if ((pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)&gCurrentMenuInfo, 0, sizeof(UIMenuStoreInfo), pSection);
        PStore_CloseSection(pSection);
    }
    else
    {
        debug_err(("\nPStore Write sys param fail\r\n"));
    }
    GPIOMap_SetSPIWp(FALSE);	

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Reset Menu Info to Default
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SysReset_MenuInfo(void)
{
    debug_err((">>>>>>>>>>>>>>>>>>>> System resetting......\r\n"));

    /*--- Photo ---*/
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_CAPTUREMODE,1,UIDFT_CAPTUREMODE);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_FLASH,1,UIDFT_FLASH);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SIZE,1,UIDFT_PHOTO_SIZE);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_QUALITY,1,UIDFT_QUALITY);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_ISO,1,UIDFT_ISO);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_EV,1,UIDFT_EV);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_WB,1,UIDFT_WB);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_COLOR,1,UIDFT_COLOR);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_METERING,1,UIDFT_METERING);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SHARPNESS,1,UIDFT_SHARPNESS);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SATURATION,1,UIDFT_SATURATION);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_FD,1,UIDFT_FD);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_ANTISHAKING,1,UIDFT_ANTISHAKING);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_QUICKREVIEW,1,UIDFT_QREVIEW);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_DATEIMPRINT,1,UIDFT_DATEIMPRINT);
    Ux_SendEvent(&UIPhotoObjCtrl,NVTEVT_EXE_SCENEMODE,1,UIDFT_SCENE);   //Notice: Must be set after all Photo related settings!!!

    /*--- Movie ---*/
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIESIZE,1,UIDFT_MOVIE_SIZE);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_QUALITY,1,UIDFT_MOVIE_QUALITY);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_COLOR,1,UIDFT_MOVIE_COLOR);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOVIEDIS,1,UIDFT_MOVIE_DIS);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_DATEIMPRINT,1,UIDFT_MOVIE_DATEIMPRINT);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_AUDIO_SET,1,UIDFT_MOVIE_AUDIO);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_MOTION_DET,1,UIDFT_MOVIE_MOTIONDET);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_CYCLIC_REC,1,UIDFT_MOVIE_CYCLIC_REC);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_GOD_BLESS,1,UIDFT_MOVIE_GODBLESS);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_GOLF_SHOT,1,UIDFT_MOVIE_GOLF_SHOT);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_FLASH_REC,1,UIDFT_MOVIE_FLASH_REC);
    Ux_SendEvent(&UIMovieObjCtrl,NVTEVT_EXE_GSENSOR,1,UIDFT_MOVIE_GSENSOR);

    /*--- Playback ---*/
    Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_EXE_SLIDE,1,UIDFT_SLIDE_INTERVAL);

    /*--- Setup ---*/
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LANGUAGE,1,UIDFT_LANGUAGE);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_TVMODE,1,UIDFT_TV_MODE);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_POWEROFF,1,UIDFT_POWEROFF);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_BEEP,1,UIDFT_BEEP);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_VOLUME,1,UIDFT_VOLUME);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DISPLAY,1,UIDFT_DISPLAY);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_FREQ,1,UIDFT_FREQUENCY);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DATEFORMAT,1,UIDFT_DATE_FORMAT);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_BRIGHT,1,UIDFT_LCDBRIGHTNESS);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_OPENING,1,UIDFT_OPENING);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_LCDOFF,1,UIDFT_LCD_OFF);
    Ux_SendEvent(&UISystemObjCtrl,NVTEVT_EXE_DELAYPOWEROFF,1,UIDFT_DELAYPOWEROFF);
	
    /*--- Print ---*/
    //Rsvd
#if 0
    	rtc_setDate(2013,1,1);
    	rtc_setTime(8,0,0);
#endif		
    /* After reset, save to NAND */
     GPSSetTime = TRUE;
	
    Save_MenuInfo();
}

