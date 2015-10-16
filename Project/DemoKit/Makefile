
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = $(CHIP)Demo

#----------------------------------------------------------------------
# set the scatter file name here
#----------------------------------------------------------------------
SCATTER = $(PRJ_NAME).scf

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../Include \

#----------------------------------------------------------------------
# add/delete the project assembly files here
#----------------------------------------------------------------------
ASM = \
	SrcCode/SysCfg/VersionInfo.s \
	SrcCode/SysCfg/Startup/Loader.s

#----------------------------------------------------------------------
# add/delete the project C source files here
#----------------------------------------------------------------------
SRC = \
	SrcCode/BuildDateInfo.c \
	SrcCode/BuildBmpInfo.c \
	SrcCode/CommandTsk.c \
	SrcCode/GPSFileDB.c \
	SrcCode/USBMakerParamInit.c \
	SrcCode/GlobalVar.c \
	SrcCode/UserPStore.c \
	SrcCode/UserFolder.c \
	SrcCode/SensorFP.c \
	SrcCode/SystemClock.c \
	SrcCode/SystemInfoGet.c \
	SrcCode/ReadNand.c \
	SrcCode/SysInit/SystemInit.c \
	SrcCode/SysInit/SystemTask.c \
	SrcCode/SysCfg/SysCfg.c \
	SrcCode/SysCfg/Startup/CopyRest.c \
	SrcCode/SysCfg/Startup/DATKER.c \
	SrcCode/SysCfg/Startup/InitCLibMMU.c \
	SrcCode/SysCfg/Startup/KerInit.c \
	SrcCode/PlaySound/PlaySoundAPI.c \
	SrcCode/PlaySound/PlaySoundTsk.c \
	SrcCode/PlaySound/PlaySoundUtil.c \
	SrcCode/PlaySound/SoundData.c \
	SrcCode/KeyIOScan/KeyScanFlashLight.c \
	SrcCode/KeyIOScan/KeyScanPWR.c \
	SrcCode/KeyIOScan/KeyScanTsk.c \
	SrcCode/KeyIOScan/KeyScanUSB.c \
	SrcCode/KeyIOScan/KeyScanLED.c \
	SrcCode/KeyIOScan/KeyScanLCD.c \
	SrcCode/KeyIOScan/KeyScanSD.c \
	SrcCode/KeyIOScan/KeyScanSound.c \
	SrcCode/KeyIOScan/KeyScanTv.c \
	SrcCode/KeyIOScan/KeyScanHDMI.c \
	SrcCode/KeyIOScan/KeyScanKey.c \
	SrcCode/KeyIOScan/KeyScanMisc.c \
	SrcCode/KeyIOScan/KeyScanIrRx.c \
	SrcCode/KeyIOScan/UserGPIO.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/GPIOMapping.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/KeyADC.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/VoltageDet.c \
	SrcCode/KeyIOScan/$(PCB_DIR)/IrRxParser.c \
	SrcCode/DeviceControl/DeviceCtrl.c \
	SrcCode/DeviceControl/DevCtrlMovie.c \
	SrcCode/DeviceControl/DevCtrlMp3.c \
	SrcCode/DeviceControl/DevCtrlPhoto.c \
	SrcCode/DeviceControl/DevCtrlPlayback.c \
	SrcCode/DeviceControl/DevCtrlSetup.c \
	SrcCode/DeviceControl/DevCtrlUSB.c \
	SrcCode/DeviceControl/DevCtrlDummy.c \
	SrcCode/AppInit/AppInitUSB.c \
	SrcCode/AppInit/AppInitMovie.c \
	SrcCode/AppInit/AppInitMp3.c \
	SrcCode/AppInit/AppInitPhoto.c \
	SrcCode/AppInit/AppInitPlayback.c \
	SrcCode/AppInit/AppInitSetup.c \
	SrcCode/AppInit/AppInit.c \
	SrcCode/UpdateNand/UpdateNand.c \
	SrcCode/UpdateNand/UpdateNandProgress.c \
	SrcCode/UpdateNand/UpdateNandReset.c \
	SrcCode/UIApp/UIPhotoObj.c \
	SrcCode/UIApp/UIPlayObj.c \
	SrcCode/UIApp/UIPlayStamp.c \
	SrcCode/UIApp/UIPlayMov.c \
	SrcCode/UIApp/UIMovieObj.c \
	SrcCode/UIApp/UISystemObj.c \
	SrcCode/UIApp/UIPrintObj.c \
	SrcCode/UIApp/UIBackgroundObj.c \
	SrcCode/UIInfo/UIMenuPhotoSettings.c \
	SrcCode/UIInfo/UIMenuMovieSettings.c \
	SrcCode/UIInfo/UIMenuPlaySettings.c \
	SrcCode/UIInfo/UIMenuSetupSettings.c \
	SrcCode/UIInfo/UISystemStatusSettings.c \
	SrcCode/UIInfo/UIMenuPrintSettings.c \
	SrcCode/UIWnd/UIFlow.c \
	SrcCode/UIDisplay/BGDB/BG_Imags.c \
	SrcCode/UIDisplay/BGDB/BG_Opening.c \
	SrcCode/UIDisplay/BGDB/BG_Goodbye.c \
	SrcCode/UIDisplay/BGDB/BG_Watermark.c \
	SrcCode/UIDisplay/UIGraphics.c \
	SrcCode/UIDisplay/UIResource.c \
	SrcCode/UIDisplay/UIResource/DemoKit_Font.c \
	SrcCode/UIDisplay/UIResource/DemoKit_Image.c \
	SrcCode/UIDisplay/UIResource/DemoKit_Palette.c \
	SrcCode/MSDCVendor/MSDCVendorSample.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Int.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Photo.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Lcd.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Disp.c \
	SrcCode/MSDCVendor/MsdcNvtCb_File.c \
	SrcCode/MSDCVendor/MsdcNvtCb_CustomSi.c \
	SrcCode/MSDCVendor/MsdcNvtCb_IQSim.c \
	SrcCode/MSDCVendor/MsdcNvtCb_TimeSi.c \
	SrcCode/MSDCVendor/MsdcNvtCb_Adj.c \
	SrcCode/DateImprint/DateStampFont10x16.c \
	SrcCode/DateImprint/DateStampFont12x20.c \
	SrcCode/DateImprint/DateStampFont18x30.c \
	SrcCode/DateImprint/DateStampFont26x44.c \
	SrcCode/DateImprint/DateStampFont30x52.c \
	SrcCode/DateImprint/DateStampFont36x60.c \
	SrcCode/DateImprint/DateStampFont42x72.c \
	SrcCode/DateImprint/DateStampFont50x86.c \
	SrcCode/DateImprint/DateStampFont56x96.c \
	SrcCode/DateImprint/GenDateImprintData.c \
	SrcCode/DateImprint/GenDateImprintData4Movie.c

#Add the UI-dependent file with different project
ifeq "$(UI_STYLE)" "UI_STYLE_DRIVERECORDER"
SRC += \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPhoto/UIFlowCBPhoto.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowCBWav.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowCBPlayMovie.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowWndPlay.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowWndPlayIcons.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowMovie/UIFlowCBRecMovie.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowMovie/UIFlowWndMovie.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowMP3/UIFlowWndMP3.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowMP3/UIFlowCBMP3.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowPrint/UIFlowWndPrint.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowCommon/UIFlowWndCommon.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowCommon/UIFlowWndWrnStrgHotPlug.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieResolution.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieVideo.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieCyclicRec.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieFlashRec.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieGolfRec.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieMotionDetect.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieAudioSet.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieDateStamp.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieEV.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieWB.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuMovie/UIMenuWndMovieGsensor.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoStill.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoCaptureMode.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoResolution.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoCompression.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoSharpness.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoISO.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoEV.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoFD.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoAntiSha.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoQuickView.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPhoto/UIMenuWndPhotoDateStamp.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlayback.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlayQuickConfirmDel.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlayQuickDel.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPlay/UIMenuWndPlayProtect.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetup.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupAutoPowerOff.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupBeepSound.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupLanguage.c \
	Srccode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupTVMode.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupFrequency.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
	Srccode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
	Srccode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupLcdOff.c\
	Srccode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupDelayPowerOff.c\
	Srccode/UIWnd/DriveRecorder/UIMenu/UIMenuSetup/UIMenuWndSetupImageRotate.c\
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_SelectImages.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintAll.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintSetting.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_Confirm.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintProgress.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_Crop.c \
    SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintDPOF.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuCalibration/UIMenuWndCalibration.c \
	SrcCode/UIWnd/DriveRecorder/UIMenu/UIMenuCalibration/UIMenuWndPCB.c
ifneq "$(TouchPanel)" "DISABLE"
	SRC += \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest.c \
	SrcCode/UIWnd/DriveRecorder/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest_Cal.c
endif
endif

ifeq "$(UI_STYLE)" "UI_STYLE_DRIVERECORDERTOUCH"
SRC += \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPhoto/UIFlowCBPhoto.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowCBWav.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowCBPlayMovie.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowWndPlay.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowWndPlayIcons.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowMovie/UIFlowCBRecMovie.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowMovie/UIFlowWndMovie.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowMP3/UIFlowWndMP3.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowMP3/UIFlowCBMP3.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowPrint/UIFlowWndPrint.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowCommon/UIFlowWndCommon.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowCommon/UIFlowWndWrnStrgHotPlug.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieResolution.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieVideo.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieCyclicRec.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieFlashRec.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieGolfRec.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieMotionDetect.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieAudioSet.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuMovie/UIMenuWndMovieDateStamp.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayback.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayQuickConfirmDel.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayQuickDel.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayFormat.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPlay/UIMenuWndPlayFormatConfirm.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetup.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupAutoPowerOff.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupBeepSound.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupLanguage.c \
	Srccode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupTVMode.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupFrequency.c \
	Srccode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_SelectImages.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintAll.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintSetting.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_Confirm.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintProgress.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_Crop.c \
  SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintDPOF.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuCalibration/UIMenuWndCalibration.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIMenu/UIMenuCalibration/UIMenuWndPCB.c
ifneq "$(TouchPanel)" "DISABLE"
	SRC += \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest.c \
	SrcCode/UIWnd/DriveRecorderTouch/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest_Cal.c
endif
endif

ifeq "$(UI_STYLE)" "UI_STYLE_DVPLUS"
	SRC += \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPhoto/UIFlowCBPhoto.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowCBWav.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowCBPlayMovie.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowWndPlay.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowWndPlayIcons.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowMovie/UIFlowCBRecMovie.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowMovie/UIFlowWndMovie.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowMP3/UIFlowWndMP3.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowMP3/UIFlowCBMP3.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowPrint/UIFlowWndPrint.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowCommon/UIFlowWndCommon.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowCommon/UIFlowWndWrnStrgHotPlug.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuMovie/UIMenuWndMovieResolution.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuMovie/UIMenuWndMovieVideo.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuMovie/UIMenuWndMovieDIS.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuMovie/UIMenuWndMovieDateStamp.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoStill.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoCaptureMode.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoResolution.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoCompression.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoScene.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoISO.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoSharpness.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoFD.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoAntiSha.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoQuickView.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoDateStamp.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPhoto/UIMenuWndPhotoQuickSetting.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayback.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayProtect.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayCopyToCard.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayDPOF.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlaySetDPOF.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayQuickConfirmDel.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPlay/UIMenuWndPlayQuickDel.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetup.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupAutoPowerOff.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupBeepSound.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupLanguage.c \
	Srccode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupTVMode.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupFrequency.c \
	Srccode/UIWnd/DVPlus/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_SelectImages.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintAll.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintSetting.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_Confirm.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintProgress.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_Crop.c \
    SrcCode/UIWnd/DVPlus/UIMenu/UIMenuPrint/UIMenuWndPrint_PrintDPOF.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuCalibration/UIMenuWndCalibration.c \
	SrcCode/UIWnd/DVPlus/UIMenu/UIMenuCalibration/UIMenuWndPCB.c
ifneq "$(TouchPanel)" "DISABLE"
	SRC += \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest.c \
	SrcCode/UIWnd/DVPlus/UIFlow/UIFlowTouchTest/UIFlowWndTouchTest_Cal.c
endif
endif

ifeq "$(SENSOR)" "Sensor_OV5633_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
  SrcCode/Calibration/NewSensor/Sensor630/Cal_OV5653/Cal_OV5653_630.c
endif

ifeq "$(SENSOR)" "Sensor_OV5653_630_HVFlip"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
  SrcCode/Calibration/NewSensor/Sensor630/Cal_OV5653/Cal_OV5653_630.c
endif

ifeq "$(SENSOR)" "Sensor_OV5653_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
  SrcCode/Calibration/NewSensor/Sensor630/Cal_OV5653/Cal_OV5653_630.c
endif

ifeq "$(SENSOR)" "Sensor_A1040_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_A1040/Cal_A1040_630.c
endif

ifeq "$(SENSOR)" "Sensor_MI5100_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_MI5100/Cal_MI5100_630.c
endif

ifeq "$(SENSOR)" "Sensor_MI5100_TVP5150_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_MI5100/Cal_MI5100_630.c
endif

ifeq "$(SENSOR)" "Sensor_OV9710_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_OV9710/Cal_OV9710_630.c
endif

ifeq "$(SENSOR)" "Sensor_OV9710_Dual_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_OV9710/Cal_OV9710_630.c
endif

ifeq "$(SENSOR)" "Sensor_NT99140_632"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_NT99140/Cal_NT99140_632.c
endif


ifeq "$(SENSOR)" "Sensor_ZT3150_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_DUMMY/Cal_DUMMY_630.c
endif

ifeq "$(SENSOR)" "Sensor_ZT3120_630"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_DUMMY/Cal_DUMMY_630.c
endif

ifeq "$(SENSOR)" "Sensor_NT99141_632"
	SRC += \
	SrcCode/Calibration/Cal_Alg.c \
	SrcCode/Calibration/CalibrationTsk.c \
	SrcCode/Calibration/CalibrationAPI.c \
	SrcCode/Calibration/CalibrationTbl.c \
	SrcCode/Calibration/BurnIn/CalBurnIn.c \
	SrcCode/Calibration/CIPA/CalCIPA.c \
	SrcCode/Calibration/LCD/CalLCD.c \
	SrcCode/Calibration/NewSensor/Sensor630/CalSensor.c \
	SrcCode/Calibration/NewSensor/Sensor630/cal_common_630.c \
	SrcCode/Calibration/NewSensor/Sensor630/Cal_NT99141/Cal_NT99141_632.c
endif
ifeq "$(IQSIM)" "IQSIM_ON"
	SRC += \
	SrcCode/MSDCVendor/MsdcNvtCb_IQSim.c \
	../../LIBExt/LIBExt_src/SubSystem/dcAlgApiLib/ImageQualitySim.c
endif

ifeq "1" "1"
	SRC += \
	SrcCode/UIDisplay/UIResource/DemoKit_String_EN.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_DE.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_FR.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_ES.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_IT.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_PO.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_TC.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_SC.c \
	SrcCode/UIDisplay/UIResource/DemoKit_String_JP.c\
	SrcCode/UIDisplay/UIResource/DemoKit_String_RU.c
endif


#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the forced rebuild objects
#----------------------------------------------------------------------
FORCED_REBUILD_OBJ = $(PRJ_NAME)_Data/Release/ObjectCode/BuildDateInfo.o

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
APP_BASE_DIR = ../../Application
APPEX_BASE_DIR = ../../AppExt
LIB_BASE_DIR = ../../LIB
LIBEX_BASE_DIR = ../../LIBExt

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS_D = \
	$(CHIP_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(UI_STYLE_PARAM) \
	$(OSD_DUMP_FLAG_PARAM) \
	$(DRAMSIZE_PARAM) \
	$(FREEMEMORYSIZE_PARAM) \
	$(LCD_PARAM) \
	$(TOUCH_PANEL_PARAM) \
	$(SENSOR_PARAM) \
	$(LENS_PARAM) \
	$(MOTOR_PARAM) \
	$(FLASHLIGHT_PARAM) \
	$(AUDIO_PARAM) \
	$(STORAGE_PARAM) \
	$(FWCOMPRESS_PARAM) \
	$(PSTORE_PARAM) \
	$(FD_PARAM) \
	$(SD_PARAM) \
	$(IRRX_PARAM) \
	$(MSDCVENDOR_PARAM)

ASMFLAGS_D = \
	$(CHIP_PARAM_ASM) \
	$(DRAMSIZE_PARAM_ASM) \
    $(FWCOMPRESS_PARAM_ASM)

CFLAGS_R = \
	$(CHIP_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(UI_STYLE_PARAM) \
	$(OSD_DUMP_FLAG_PARAM) \
	$(DRAMSIZE_PARAM) \
	$(FREEMEMORYSIZE_PARAM) \
	$(LCD_PARAM) \
	$(TOUCH_PANEL_PARAM) \
	$(SENSOR_PARAM) \
	$(LENS_PARAM) \
	$(MOTOR_PARAM) \
	$(FLASHLIGHT_PARAM) \
	$(AUDIO_PARAM) \
	$(STORAGE_PARAM) \
	$(PSTORE_PARAM) \
	$(FWCOMPRESS_PARAM) \
	$(FD_PARAM) \
	$(SD_PARAM) \
	$(IRRX_PARAM) \
	$(MSDCVENDOR_PARAM)

ASMFLAGS_R = \
	$(CHIP_PARAM_ASM) \
	$(DRAMSIZE_PARAM_ASM) \
    $(FWCOMPRESS_PARAM_ASM)

GCCFLAGS = \
	$(CHIP_PARAM) \
	$(PCB_STAGE_PARAM) \
	$(UI_STYLE_PARAM) \
	$(OSD_DUMP_FLAG_PARAM) \
	$(DRAMSIZE_PARAM) \
	$(FREEMEMORYSIZE_PARAM) \
	$(LCD_PARAM) \
	$(SENSOR_PARAM) \
	$(LENS_PARAM) \
	$(MOTOR_PARAM) \
	$(FLASHLIGHT_PARAM) \
	$(AUDIO_PARAM) \
	$(STORAGE_PARAM) \
	$(PSTORE_PARAM) \
	$(FWCOMPRESS_PARAM) \
	$(FD_PARAM) \
	$(SD_PARAM) \
	$(IRRX_PARAM) \
	$(MSDCVENDOR_PARAM)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputImg.txt
