/**
    @file       SystemInit.c
    @ingroup    mIPRJAPCfg

    @brief      Initialize system.

                Initialize HW and FW.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

/**
    @addtogroup mIPRJAPCfg
*/
//@{

#include "SystemInit.h"
#include "SystemTask.h"
#include "Driver.h"
#include "SysCfg.h"
#include "GPIOMapping.h"
#include "USB.h"
#include "LCDTV.h"
#include "ide.h"
#include "SIF.h"
#include "I2C.h"
#include "SPI.h"
#include "Top.h"
#include "JpgDec.h"
#include "Audio.h"
#include "AudioCodec.h"
#include "Pll.h"
#include "UART.h"
#include "IrRx.h"
#include "ImageAlgInfor.h"
#include "PrimaryTsk.h"
#include "GlobalVar.h"
#include "SystemClock.h"
#include "BG_Images.h"
#include "VoltageDet.h"
#include "CopyRest.h"
#include "Utility.h"
#include "SensorFP.h"
#include "UserGPIO.h"
#include "Clock.h"
#include "PlaySoundTsk.h"
#include "PlaySoundInt.h"
#include "Nand.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "AF_Lib.h"
#include "RamDisk.h"
#include "PStore.h"
#include "PSDrv.h"
#include "FileSysTsk.h"
#include "Cal_Alg.h"
#include "Timer.h"
#include "pad.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "DMA.h"
#include "UIFlow.h"
#include "UISystemStatusSettings.h"
#include "UIMenuSetupSettings.h"
#include "UICustomer.h"
#include "VendorEXIF.h"
#include "UserFolder.h"
#include "Gesture_Public.h"
#include "TouchPanel.h"
#if (_STORAGECARD_ == _STORAGECARD_XD_)
#include "XD.h"
#elif (_STORAGECARD_ == _STORAGECARD_MSPRO_)
#include "MSPRO.h"
#elif (_STORAGECARD_ == _STORAGECARD_CF_)
#include "CF.h"
#elif ((_STORAGECARD_ == _STORAGECARD_SDIO_) || (_STORAGECARD_ == _STORAGECARD_SDIO2_))
#include "sdio.h"
#else
    #error Unknown storage card
#endif
#include "ck235.h"


#if 0
#define DEBUG_MSG(msg)    debug_msg msg
#else
#define DEBUG_MSG(msg)
#endif
BOOL PowerOnMau = FALSE;
#define ENCRYPTE_NONE   0
#define CK235_PW01    1
#define CK235_PW03    2

#define  _ENCRYPTE_   CK235_PW03//ENCRYPTE_NONE//ENABLE

#define tmp_dev_addr 0xC0
#define tmp_sub_addr 0xA0
/**
  Init essential HW

  Init essentail HW that might used in any kind of FW code.
  For example: UART (output debug message)...

  @param void
  @return void
*/
static void SysInit_InitEssentialHW(void)
{
    //#NT#2009/09/25#Jeffery Chuang -begin
    //UINT32 reg0, reg1;
    //#NT#2009/09/25#Jeffery Chuang -end
    //GPIOMap_Init();
    
    // Initialize driver, must be executed before any driver open API
    drv_init();

    // Init UART
    // Usually, the UART output message ability is enabled in boot loader.
    // And this is for UART getting character.
    // But if your boot loader didn't enable UART or you load code via
    // ARM AXD, this should be called before any task might output message.
    uart_open();
    // Wait for all TX data are sent
    // During kernel initialization phase, there might be some data in UART
    // FIFO, uart_init() will clear these data. To make sure all data are sent,
    // we have to wait for TX empty.
    // >> NOTE << You can mark these codes, but you might lose some error message.
    // Recommend: Mark these codes if there is no more error message when boot up.
    while ((uart_checkIntStatus() & UART_INT_STATUS_TX_EMPTY) == 0)
    {
        ;
    }
    // Set UART parameter
    uart_init(UART_BAUDRATE_115200, UART_LEN_L8_S1, UART_PARITY_NONE);
	
#if 0//(_NAVI_COMUNICATION_PRO_ == ENABLE)
    // Init UART
    uart2_open();
    // Set UART parameter
    uart2_init(UART_BAUDRATE_38400, UART_LEN_L8_S1, UART_PARITY_NONE);
#endif
    // Init RTC
    // Ususally, the RTC is enabled and force updated in boot loader.
    // But if your boot loader didn't enable RTC or you load code via
    // ARM AXD, this should be called ASAP.
    rtc_open();

    // Sometimes RTC power alarm will be enabled accidentally.
    // If you don't need the RTC power alarm function, keep this code to
    // prevent power up by RTC. Otherwise, please implement your code to
    // control power alarm function.
    rtc_disablePWRAlarm();

    //#Add sample code to set RTC base date to another date (default is 2000/01/01)
    // Set RTC base date to 2009/1/1
    rtc_setBaseDate(2013, 1, 1);


    // Since clk_open() won't increase the power consumption but calling
    // clk_open() and clk_close() when changing clock will decrease the
    // performance. We move clk_open() to here and don't call clk_close()
    // when FW is active.
    //#NT#2009/09/30#HH Chuang -begin
    SysClk_Init();
//    clk_open();
    //#NT#2009/09/30#HH Chuang -end

//#NT#2010/06/08#Jarkko Chang -begin
//#NT#2009/09/25#Jeffery Chuang -begin
//set priority
//    dma_getPriority(&reg0, &reg1);
//    reg0 |= (P0_PRE_DVI|P0_PRE_DPC|P0_PRE_ECS|P0_PRE_PREO);
//    dma_setPriority(reg0, reg1);

    //#NT#2011/11/1#Philex Lin - begin
    // set IME DMA priority depending on NT96632's bandwith
    dma_setChannelPriority(DMA_CH_IME2_Y, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IME3_Y, DMA_PRIORITY_MIDDLE);

    dma_setChannelPriority(DMA_CH_PRE_DPC,    DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_PRE_ECS,    DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_PRE_PREO,   DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDEV1_Y,  DMA_PRIORITY_HIGH);

    dma_setChannelPriority(DMA_CH_IDEV1_CB, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDEV1_CR, DMA_PRIORITY_MIDDLE);

    dma_setChannelPriority(DMA_CH_IDE_OSD1, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDE_OSD2, DMA_PRIORITY_HIGH);
    //#NT#2011/11/1#Philex Lin - end

//    dma_setChannelPriority(DMA_CH_PRE_DPC,    DMA_PRIORITY_HIGH);
//    dma_setChannelPriority(DMA_CH_PRE_ECS,    DMA_PRIORITY_HIGH);
//    dma_setChannelPriority(DMA_CH_PRE_PREO,   DMA_PRIORITY_HIGH);
//    dma_setChannelPriority(DMA_CH_PRE_DVI,    DMA_PRIORITY_HIGH);
//#NT#2009/09/25#Jeffery Chuang -end
//#NT#2010/06/08#Jarkko Chang -end

    //#NT#2008/10/30#KS Hung - begin
    // Reset shutdown timer
    rtc_resetShutdownTimer();
    //#NT#2008/10/30#KS ung - end
    #if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    {
        SPI_INIT_INFO spiConfig;

        spiConfig.bCSActiveLow = TRUE;
        spiConfig.bLSB = FALSE;
        spiConfig.bMasterMode = TRUE;
        spiConfig.pinmuxChannel = SPI_PINMUX_STORAGE;
        spiConfig.spiMODE = SPI_MODE_0;
        spiConfig.uiClkDiv = 1;
        spi_init(&spiConfig);
        pad_setDrivingSink(PAD_DS_CARD, PAD_DRIVINGSINK_8MA);
    }
    #endif

    #if (_RAM_DISK_ == ENABLE)
    debug_ind(("ramdsk_getStorageObject()\r\n"));
    Primary_SetPerStrgObject(ramdsk_getStorageObject());
    #else //nand object
    debug_ind(("nand_getStorageObject()\r\n"));
    Primary_SetPerStrgObject(nand_getStorageObject());
    #endif
}

BOOL UI_ShowLogoFile(PS_SECNAME_ID psSectID)
{
    UINT32      uiFileSize;
    UINT8       *BgBuf;
    ER          erReturn;
    PSTORE_SECTION_HANDLE* pSecHdl; // to save the handle of PStore section

    get_blk((void *)&BgBuf,  POOL_ID_SICD);
    rel_blk(POOL_ID_SICD, BgBuf);

    pSecHdl = PStore_OpenSection(psSectID, PS_RDONLY);

    if (pSecHdl == E_PS_SECHDLER)
    {
        debug_err(("Section open fail\r\n"));
        PStore_CloseSection(pSecHdl);

        return FALSE;
    }

    erReturn = PStore_ReadSection(BgBuf, 0, 4, pSecHdl);

    if (erReturn != E_PS_OK)
    {
        PStore_CloseSection(pSecHdl);
        debug_err(("PStore read error\r\n"));
    }
    uiFileSize = MAKE_LONG(MAKE_WORD(BgBuf[0],BgBuf[1]),MAKE_WORD(BgBuf[2],BgBuf[3]));

    erReturn = PStore_ReadSection(BgBuf, 0, (uiFileSize+4), pSecHdl);

    PStore_CloseSection(pSecHdl);
    if (erReturn != E_PS_OK)
    {
        debug_err(("PStore read error\r\n"));
        return FALSE;
    }

    SysInit_DisplaySplashScreen(LOGO_CUSTOM,(UINT8 *)(BgBuf+4), uiFileSize);
    return TRUE;
}

/**
  Display startup screen

  Display startup screen

  @param void
  @return void
*/
void SysInit_DisplaySplashScreen(UINT32 uiShowLogo,UINT8 *pBgBuf, UINT32 uiSize)
{
//#NT#2009/05/20#Corey Ke -begin
//#NT#Refine for 96630
    UINT32              uiPoolAddr;//, uiFlag;
//#NT#2009/05/20#Corey Ke -end
    DECJPG_INFO         DecOneJPGInfo;
    JPGHEAD_DEC_CFG     JPGCfg;
    VIDEO_BUF_ADDR      FB_Addr;
    VIDEO_BUF_ATTR      FB_attr;
    VOSD_WINDOW_ATTR    FB_win;

    if (get_blk((void*)&uiPoolAddr, POOL_ID_CAPTURE) != E_OK)
    {
        return;
    }
    rel_blk(POOL_ID_CAPTURE, (VP)uiPoolAddr);

    // JPG information
    DecOneJPGInfo.pJPGScalarHandler = NULL;
    if (uiShowLogo == LOGO_STARTUP) {
        DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGOpening;
        DecOneJPGInfo.JPGFileSize       = BG_OPENING_BS_SIZE;
    } else if (uiShowLogo == LOGO_POWEROFF) {
        DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGGoodbye;
        DecOneJPGInfo.JPGFileSize       = BG_GOODBYE_BS_SIZE;
    } else if (uiShowLogo == LOGO_GOD) {
        DecOneJPGInfo.pSrcAddr          = (UINT8 *)g_ucBGGOD;
        DecOneJPGInfo.JPGFileSize       = BG_GOD_BS_SIZE;
    } else {
        DecOneJPGInfo.pSrcAddr          = pBgBuf;//custom' logo
        DecOneJPGInfo.JPGFileSize       = uiSize;
    }
    DecOneJPGInfo.pDstAddr          = (UINT8 *)uiPoolAddr;
    DecOneJPGInfo.jdcfg_p           = &JPGCfg;
    DecOneJPGInfo.IfDecodeThumb     = DEC_PRIMARY;
    DecOneJPGInfo.bEnableTimeOut = FALSE;
    DecOneJPGInfo.bEnableCrop = FALSE;

    // Decode startup JPG, DecodeOneJPG() will open JPEG driver
    if (DecodeOneJPG(&DecOneJPGInfo) != E_OK)
    {
        return;
    }

    FB_Addr.B0_Y            = DecOneJPGInfo.uiOutAddrY;
    FB_Addr.B0_CB           = DecOneJPGInfo.uiOutAddrCb;
    FB_Addr.B0_CR           = DecOneJPGInfo.uiOutAddrCr;
    FB_Addr.uiBufSel        = IDE_VIDEO_ADDR_SEL_BUFFER0;

    FB_attr.Buf_W           = JPGCfg.imagewidth;
    FB_attr.Buf_H           = JPGCfg.imageheight;
    FB_attr.Buf_Lineoffset  = JPGCfg.imagewidth >> 2;
    FB_attr.Buf_L2R         = IDE_BUFFER_READ_L2R;
    FB_attr.Buf_T2B         = IDE_BUFFER_READ_T2B;
    FB_attr.Buf_BJmode      = IDE_VIDEO_BJMODE_BUFFER_REPEAT;

    FB_attr.Buf_Opt         = IDE_VIDEO_BUFFER_OPT_0;
    FB_attr.Buf_Num         = IDE_VIDEO_BUFFER_NUM_1;

    FB_win.Source_W         = JPGCfg.imagewidth;
    FB_win.Source_H         = JPGCfg.imageheight;
    FB_win.Des_W            = g_LCDSize.uiWinWidth;
    FB_win.Des_H            = g_LCDSize.uiWinHeight;
    FB_win.Win_Format       = JPGCfg.fileformat + 5;
    FB_win.Win_X            = 0;
    FB_win.Win_Y            = 0;
    FB_win.High_Addr        = 0;

#if (SYSINIT_LOGO_V1 == ENABLE)
    ide_setVideoBufAddr(IDE_VIDEOID_1, &FB_Addr);
    ide_setVideoBufAttr(IDE_VIDEOID_1, &FB_attr);
    ide_setVideoWinAttr(IDE_VIDEOID_1, &FB_win);

    // Enable video1
    ide_enableVideo(IDE_VIDEOID_1);
#else
    ide_setVideoBufAddr(IDE_VIDEOID_2, &FB_Addr);
    ide_setVideoBufAttr(IDE_VIDEOID_2, &FB_attr);
    ide_setVideoWinAttr(IDE_VIDEOID_2, &FB_win);

    // Video1 is enabled by default, must disable video1
    ide_disableVideo(IDE_VIDEOID_1);

    // Enable video2
    ide_enableVideo(IDE_VIDEOID_2);
#endif
}

/**
  Init Touch Panel

  Init Touch Panel

  @param void
  @return void
*/
void SysInit_InitTouchPanel(void)
{
    #if (_TouchPanel_ != DISABLE)
    GESTURE_INIT_STRUCT         TempGestureInit;
    GESTURE_CALI_DATA_STRUCT    TempGestureCalData;
    TP_INIT_STRUCT              TempTPInit;

    #if (_TouchPanel_ != _TouchPanel_NVT_)
    GPIOMap_SetTPPin(&TempTPInit);
    #endif

    tp_init(&TempTPInit);

    TempGestureInit.Threshold.SlideRange = 30;
    TempGestureInit.Threshold.HoldRange = 30;
    TempGestureInit.Threshold.HoldTime = 100;
    TempGestureInit.Threshold.DoubleClickTime = 5;
    TempGestureInit.LcdInfo.Width = OSD_W;//g_LCDSize.uiWinWidth;
    TempGestureInit.LcdInfo.Height = OSD_H;//g_LCDSize.uiWinHeight;

    Gesture_Init(&TempGestureInit);

    TempGestureCalData.ReverseX = FALSE;
    TempGestureCalData.ReverseY = FALSE;
    TempGestureCalData.MinX = 63;//196;
    TempGestureCalData.MinY = 82;//377;
    TempGestureCalData.MaxX = 980;//3864;
    TempGestureCalData.MaxY = 928;//3816;
    TempGestureCalData.TopLeftX = TempGestureCalData.MinX;
    TempGestureCalData.TopLeftY = TempGestureCalData.MinY;
    TempGestureCalData.BottomRightX = TempGestureCalData.MaxX;
    TempGestureCalData.BottomRightY = TempGestureCalData.MaxY;

    Gesture_SetCalData(&TempGestureCalData);
    #endif
}

//#NT#2012/09/13#Philex Lin -begin
#if (_LONG_STARTUP_ == ENABLE)
#define RESET_SHUTDOWN_TIMER_INTERVAL   100 //100ms
#define RESET_SHUTDOWN_TIMER_COUNT       20
static volatile UINT32  guiResetShutDownTimerID;
static volatile UINT32  guiResetSDTimerCount = 0;
void SysInit_ResetShutDownTimerHdl(void)
{
   // checking whether power butter pressing or not
   if (rtc_getPWRStatus()==FALSE)
   {
      rtc_poweroffPWR();
      while (1)
      {
          debug_err(("*"));
      }
   } else {
       if (guiResetSDTimerCount > RESET_SHUTDOWN_TIMER_COUNT)
       {
         // over 2 sec
         timer_close(guiResetShutDownTimerID);
       } else {
         guiResetSDTimerCount++;
         // reset shutdown timer once
         rtc_resetShutdownTimer();
       }
   }
}

void SysInit_ResetShutDownTimer(void)
{
  // reset shutdown timer first
  rtc_resetShutdownTimer();

  if (timer_open((UINT *)&guiResetShutDownTimerID, SysInit_ResetShutDownTimerHdl) != E_OK)
     return;

  timer_set(guiResetShutDownTimerID, RESET_SHUTDOWN_TIMER_INTERVAL, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
}
#endif
//#NT#2012/09/13#Philex Lin -end

//#NT#2009/02/05#Hideo Lin -begin
//#Should delay for a while to wait power stable for GiantPlus LCD panel, otherwise will see some garbages
//#To avoid influencing power up time performance, use timer to turn on LCD backlight power
static volatile UINT32  guiLCDPowerDelayTimerID;

void SysInit_LCDPowerDelayTimerHdl(void)
{
    timer_close(guiLCDPowerDelayTimerID);
    GPIOMap_TurnOnLCDBacklight();
}

void SysInit_LCDPowerDelay(void)
{
    if (timer_open((UINT *)&guiLCDPowerDelayTimerID, SysInit_LCDPowerDelayTimerHdl) != E_OK)
        return;

    timer_set(guiLCDPowerDelayTimerID, 140, _TIMER_CTRL_FREE_RUN | _TIMER_CTRL_INT_ENABLE, _TIMER_PLAY);
}
//#NT#2009/02/05#Hideo Lin -end

/**
  Init LCD

  Init LCD

  @param void
  @return void
*/
void SysInit_InitLCD(void)
{
    // LCD Control interface
#if   (_LCDTYPE_ == _LCDTYPE_AUCN01_)
    // Support RGBD, CCIR656, UPS051 and YUV mode
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_YUV;
#elif (_LCDTYPE_ == _LCDTYPE_GPM940A0_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB_THROUGH;
#elif (_LCDTYPE_ == _LCDTYPE_GPM1125A0_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_AUBL02_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_TOPPOLYTD020_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;      // GTA
#elif (_LCDTYPE_ == _LCDTYPE_TOPPOLYTD020_FLIP_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_GIANTPLUSGPM1050A0_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_GiantPlusGPG48238QS5_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_MF4823Z_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB;
#elif (_LCDTYPE_ == _LCDTYPE_GPG4823011QS3_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_TXDT200L_5182A_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_TXDT250C_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;    
#elif (_LCDTYPE_ == _LCDTYPE_TXDT270M_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB;
#elif (_LCDTYPE_ == _LCDTYPE_TXDT270C_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_TXDT300C_)
    ImgCapture_SetParameter(_DispRatio, _Ratio_16_9);
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGBD;
#elif (_LCDTYPE_ == _LCDTYPE_WD_F4823V7_7FLWA_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB;
#elif (_LCDTYPE_ == _LCDTYPE_ALT020DSLN_J2_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB;
#elif   (_LCDTYPE_ == _LCDTYPE_TM027CDH01_)
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB;
#else
    g_LCDCtrlObj.uiLCDMode                  = LCD_MODE_RGB;
#endif
    g_LCDCtrlObj.uiIDESrcCLK                = SYSINIT_PLL_OSC_CLOCK_MHZ * SYSINIT_PLL2_LOOPDIV / SYSINIT_PLL2_PREDIV / 2;
    g_LCDCtrlObj.bIDESrcPLL2                = LCD_SRCCLK_PLL2;
    // fIDEDstClk = 0 -> Use driver default setting, or you can set the clock that you want
    g_LCDCtrlObj.fIDEDstCLK                 = 0;
    // LCD_CTRL_MODE_GPIO -> Use GPIO for the control signal
    // LCD_CTRL_MODE_SIF  -> Use SIF for the control signal
    g_LCDCtrlObj.LCDCtrl_Pin.uimode         = LCD_CTRL_MODE_GPIO;
    // Specify the SIF channel in LCD_CTRL_MODE_SIF mode, don't care in LCD_CTRL_MODE_GPIO mode
    g_LCDCtrlObj.LCDCtrl_Pin.uisif_channel  = SIF_CH0;
    // Specify the GPIO pin number in LCD_CTRL_MODE_GPIO, don't care in LCD_CTRL_MODE_SIF mode
    g_LCDCtrlObj.LCDCtrl_Pin.uigpio_sen     = GPIO_LCD_SIF_SEN;
    g_LCDCtrlObj.LCDCtrl_Pin.uigpio_clk     = GPIO_LCD_SIF_SCK;
    g_LCDCtrlObj.LCDCtrl_Pin.uigpio_data    = GPIO_LCD_SIF_SDA;

    // Configure SIF pins to default state
    if (g_LCDCtrlObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        // You have to configure correct channel (Here we use channel 0)
#if   ((_LCDTYPE_ == _LCDTYPE_AUCN01_) || (_LCDTYPE_ == _LCDTYPE_GPM940A0_))
        // SIF mode 0000, CK -> High, DAT -> low, CS -> High
        pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLUP);
        pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_GPM1125A0_)
        // SIF mode 0000, CK -> High, DAT -> low, CS -> High
        pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLUP);
        pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_AUBL02_)
        // SIF mode 0100, CK -> low, DAT -> low, CS -> High
        pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_GIANTPLUSGPM1050A0_)
        // SIF mode 0100, CK -> low, DAT -> low, CS -> High
        pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_MF4823Z_)
                // SIF mode 0100, CK -> low, DAT -> low, CS -> High
                pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLDOWN);
                pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
                pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_GPG4823011QS3_)
        // SIF mode 0100, CK -> low, DAT -> low, CS -> High
        pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_GiantPlusGPG48238QS5_)
        // SIF mode 0100, CK -> low, DAT -> low, CS -> High
        pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
        pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#elif (_LCDTYPE_ == _LCDTYPE_ToppolyTD020_)
                                // SIF mode 0100, CK -> low, DAT -> low, CS -> High
                                pad_setPullUpDown(PAD_PIN_SBCK0, PAD_PULLDOWN);
                                pad_setPullUpDown(PAD_PIN_SBDAT0, PAD_PULLDOWN);
                                pad_setPullUpDown(PAD_PIN_SBCS0, PAD_PULLUP);
#endif
    }

    //#NT#2009/05/26#KS Hung -begin
    pad_setDrivingSink(PAD_DS_LCD8, PAD_DRIVINGSINK_8MA);
    //#NT#2009/05/26#KS Hung -end

    //#NT#2010/06/17#Klins Chen -begin
    //#NT#Move IDE DMA priority adjustment to project code.
    // Setup IDE DMA High priority
#if (_SENSORLIB_ == _SENSORLIB_OV5653_630_ ||\
     _SENSORLIB_ == _SENSORLIB_ZT3150_630_ ||\
     _SENSORLIB_ == _SENSORLIB_ZT3120_630_)

    dma_setChannelPriority(DMA_CH_IDEV1_Y,  DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDEV1_CB, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDEV1_CR, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDEV2_Y,  DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDEV2_CB, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDEV2_CR, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDE_OSD1, DMA_PRIORITY_MIDDLE);
    dma_setChannelPriority(DMA_CH_IDE_OSD2, DMA_PRIORITY_MIDDLE);
#else
    dma_setChannelPriority(DMA_CH_IDEV1_Y,  DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDEV1_CB, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDEV1_CR, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDEV2_Y,  DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDEV2_CB, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDEV2_CR, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDE_OSD1, DMA_PRIORITY_HIGH);
    dma_setChannelPriority(DMA_CH_IDE_OSD2, DMA_PRIORITY_HIGH);
#endif
    //#NT#2010/06/17#Klins Chen -end

#if (_PCB_ == _PCB_X1_)
    gpio_clearPin(GPIO_LCD_RESET);
    TimerDelayMs(10);
    gpio_setPin(GPIO_LCD_RESET);
#elif (_PCB_ == _PCB_GTA_)
    GPIOMap_TurnOnLCDPower();
#endif

    // Open panel
    OpenPanel(&g_LCDCtrlObj, &g_LCDSize);

    //#NT#2009/02/05#Hideo Lin -begin
    //#Should delay for a while to wait power stable for GiantPlus LCD panel, otherwise will see some garbages
    //#To avoid influencing power up time performance, use timer to turn on LCD backlight power
    SysInit_LCDPowerDelay();
    //#NT#2009/02/05#Hideo Lin -end

    SysInit_InitTouchPanel();

    // Detect LCD rotate
    //KeyScan_DetLCDRotate();
}
void SysInit_InitENDE(void)
{
#if(_ENCRYPTE_ != ENCRYPTE_NONE)
    InitEnDe();
#endif
}


/**
  Init Lens Module

  Init Lens Module

  @param void
  @return void
*/
void SysInit_InitLens(void)
{
    LENS_DEVICE_OBJ LensDevice;
    PLENS_TAB       pLensTab = NULL;
    PMOTOR_TAB      pMotorTab = NULL;
    UINT32          uiBufGPIO[LENS_IO_MAX];
    UINT32          uiBufPAD[LENS_PAD_MAX];

    LensDevice.puiMotorCtrlGPIOArray                    = &uiBufGPIO[0];
    LensDevice.puiMotorCtrlPADArray                     = &uiBufPAD[0];

    // Init lens object
#if (_LENSLIB_ == _LENSLIB_FF_)

    // Get lens function table
    pLensTab                    = lensFF_getObject();
#elif (_LENSLIB_ == _LENSLIB_ST8002_)

    // Get lens function table
    pLensTab                    = lensST8002_getObject();
#elif (_LENSLIB_ == _LENSLIB_VM18001_)

    // Get lens function table
    pLensTab                    = lensVM18001_getObject();
#else

    // Get lens function table
    pLensTab                    = lensFF_getObject();
#endif

    // Init motor object
#if (_MOTORLIB_ == _MOTORLIB_PSEUDO_)

    // Get motor function table
    pMotorTab                   = motorPSEUDO_getObject();
#elif (_MOTORLIB_ == _MOTORLIB_A3901_)
    //assign motor control mode
    LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_GPIO;

    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = 53; //GPIO_MOTOR_DRV_IN_0;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = 91; //GPIO_MOTOR_DRV_IN_1;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_2]      = 54; //GPIO_MOTOR_DRV_IN_2;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_3]      = 55; //GPIO_MOTOR_DRV_IN_3;

    // Get motor function table
    pMotorTab                   = motorA3901_getObject();
#elif (_MOTORLIB_ == _MOTORLIB_MD152_)
    //assign motor control mode
    LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_GPIO;

    //GPIO pin array for HW connection of DSP, motor driver and lens
    //GPIO mode array order(motor input pin + lens output pin)
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = (3|GPIO_IS_DGPIO);//GPIO_MOTOR_DRV_MODE_1;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_2]    = (4|GPIO_IS_DGPIO);//GPIO_MOTOR_DRV_MODE_2;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_3]    = (5|GPIO_IS_DGPIO);//GPIO_MOTOR_DRV_MODE_3;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = 20;               //GPIO_MOTOR_DRV_IN_0;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = 21;               //GPIO_MOTOR_DRV_IN_1;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_2]      = (6|GPIO_IS_DGPIO);//GPIO_MOTOR_DRV_IN_2;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_3]      = (7|GPIO_IS_DGPIO);//GPIO_MOTOR_DRV_IN_3;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = (14|GPIO_IS_DGPIO);//GPIO_MOTOR_DRV_PWR;

    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI]   = (0|GPIO_IS_DGPIO);//GPIO_LENS_ZOOM_PI;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR]   = (1|GPIO_IS_DGPIO);//GPIO_LENS_ZOOM_PR;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI]  = (2|GPIO_IS_DGPIO);//GPIO_LENS_FOCUS_PI;

    //GPIO interrupt ID for zoom PI
    LensDevice.uiMotorCtrlGPIOIntrID                    = GPIO_INT_08;      //GPIO_INT_LENS_ZOOM_PI;

    //PAD pin array for voltage pull
    LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI]   = PAD_PIN_DGPIO0;   //PAD_LENS_ZOOM_PI;
    LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR]   = PAD_PIN_DGPIO2;   //PAD_LENS_ZOOM_PR;
    LensDevice.puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI]  = PAD_PIN_DGPIO1;   //PAD_LENS_FOCUS_PI;

    // Get motor function table
    pMotorTab                   = motorMD152_getObject();
#elif (_MOTORLIB_ == _MOTORLIB_MD127_)
    //assign motor control mode
    LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_GPIO;

    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = GPIO_MSHUT_1;     //GPIO_MOTOR_DRV_IN_0;
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = GPIO_MSHUT_2;     //GPIO_MOTOR_DRV_IN_1;

    // Get motor function table
    pMotorTab                   = motorMD127_getObject();
#elif (_MOTORLIB_ == _MOTORLIB_AT5510_)
    //assign motor control mode
    LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_I2C;

    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = 20; //SN_MES0 = AGPIO20
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = 21; //SN_MES1 = AGPIO21
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = (14|GPIO_IS_DGPIO);
    LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = (15|GPIO_IS_DGPIO);

    // Get motor function table
    pMotorTab                   = motorAT5510_getObject();
#else

    // Get motor function table
    pMotorTab                   = motorPSEUDO_getObject();
#endif

    LensCtrl_RegCB((FPLENSCALLBACK)FlowPhoto_ImageCB);
    Lens_Module_Init(&LensDevice, pLensTab, pMotorTab);
}

//#NT#2009/04/15#Chris Chung -begin
//#NT#add new AF lib
UINT32 SysInit_GetVariactionCB(void)
{
    //Todo
    return 0;
}

/**
  Init AF Library

  Init AF Library

  @param void
  @return void
*/
void SysInit_InitAF(void)
{
    AF_RegGetVariationCB(SysInit_GetVariactionCB);
}
//#NT#2009/04/15#Chris Chung -end


/**
  Init sensor

  Init sensor

  @param void
  @return void
*/
#if (_SENSORLIB_ == _SENSORLIB_A1040_630_||_SENSORLIB_ == _SENSORLIB_NT99140_632_ ||_SENSORLIB_ == _SENSORLIB_NT99141_632_)
void SensorReset(void)
{
    debug_err(("Set GPIO Iyen\r\n"));

    TimerDelayMs(100);
    gpio_setPin(GPIO_CCD_RESET);//enable sensor power
    TimerDelayMs(10);
    gpio_clearPin(GPIO_CCD_RESET);//enable sensor power
    TimerDelayMs(10);
    gpio_setPin(GPIO_CCD_RESET);//enable sensor power
}
#endif

static void SysInit_InitSensor(void)
{
    SENSOR_APPOBJ   SensorObj;

// Sensor with I2C control interface
#if ((_SENSORLIB_ == _SENSORLIB_OV5653_630_) || \
     (_SENSORLIB_ == _SENSORLIB_MI5100_630_) || \
     (_SENSORLIB_ == _SENSORLIB_OV9710_630_) || \
     (_SENSORLIB_ == _SENSORLIB_OV5633_630_) || \
     (_SENSORLIB_ == _SENSORLIB_A1040_630_)  || \
     (_SENSORLIB_ == _SENSORLIB_NT99140_632_) || \
	 	 (_SENSORLIB_ == _SENSORLIB_NT99141_632_) || \
     (_SENSORLIB_ == _SENSORLIB_MI1030_630_) || \
     (_SENSORLIB_ == _SENSORLIB_ZT3150_630_) || \
     (_SENSORLIB_ == _SENSORLIB_ZT3120_630_))
    // Open I2C driver
    if (i2c_open() != E_OK)
    {
        debug_err(("Error open I2C driver!\r\n"));
    }

    // Enable I2C controller
    i2c_enable();

    // Set pinmux to I2C
    pinmux_select_i2c(PINMUX_I2C);

    #if (_SENSORLIB_ == _SENSORLIB_NT99140_632_ ||_SENSORLIB_ == _SENSORLIB_NT99141_632_)
        pinmux_select_sensor(PINMUX_SENSOR_10BIT);
    #endif

    // Set I2C bus clock to 240 KHz
    // I2C bus clock = APB clock / (2*clkdiv + 4 + GSR)
    // = 48000000 / (2*96 + 4 + 4) = 240000 Hz
    if (i2c_initMaster(96, 10, 4) != I2C_SUCCESS)
    {
        debug_err(("Init I2C driver fail!\r\n"));
    }

    // Enable Data transmit and receive interrupt
    //i2c_enableInt(I2C_INT_DT_EN|I2C_INT_DR_EN);
#if (_SENSORLIB_ == _SENSORLIB_A1040_630_)
    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(100);
    gpio_setPin(GPIO_CCD_RESET);
    gpio_setPin(GPIO_SN_OE);
    TimerDelayMs(100);
    gpio_clearPin(GPIO_SN_OE);
#endif

#if (_SENSORLIB_ == _SENSORLIB_OV5633_630_)
    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(100);
    gpio_setPin(GPIO_CCD_RESET);
#endif

#if (_SENSORLIB_ == _SENSORLIB_MI5100_630_)
    gpio_clearPin(GPIO_CCD_RESET);
    TimerDelayMs(100);
    gpio_setPin(GPIO_CCD_RESET);
//    gpio_clearPin(GPIO_SN_OE);
#endif

#if (_SENSORLIB_ == _SENSORLIB_ZT3150_630_ || \
     _SENSORLIB_ == _SENSORLIB_ZT3120_630_)
    gpio_setDir(GPIO_CCD_PWND, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_CCD_PWND);
    TimerDelayMs(100);
    gpio_clearPin(GPIO_CCD_PWND);
    TimerDelayMs(1000);
    gpio_setPin(GPIO_CCD_PWND);

#endif


// Sensor with SIF control interface
#else
    SIF_CTRL        SIFCtrl;

    // Open SIF driver
    sif_open();

    #if ((_SENSORLIB_ == _SENSORLIB_PANA10M_630_))
    SIFCtrl.uiMode          = SIF_MODE_0100;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 40;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = FALSE;
    SIFCtrl.uiSenH          = 0;
    sif_setControl(SIF_CH5, &SIFCtrl);

    SIFCtrl.uiMode          = SIF_MODE_0100;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 42;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = FALSE;
    SIFCtrl.uiSenH          = 0;
    sif_setControl(SIF_CH3, &SIFCtrl);

#elif (_SENSORLIB_ == _SENSORLIB_SONY8M_)
    SIFCtrl.uiMode          = SIF_MODE_0000;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 40;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = FALSE;
    sif_setControl(SIF_CH2, &SIFCtrl);

    // Setup SIF channel 3 parameter
    SIFCtrl.uiMode          = SIF_MODE_0100;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 16;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = FALSE;
    sif_setControl(SIF_CH3, &SIFCtrl);
#else
    // For Panasonic TG(16 bits) & AFE(16 bits) usage,
    // Setup SIF channel 2 parameter
    SIFCtrl.uiMode          = SIF_MODE_0000;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 16;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = FALSE;
    sif_setControl(SIF_CH2, &SIFCtrl);

     // Setup SIF channel 3 parameter
    SIFCtrl.uiMode          = SIF_MODE_0100;
    SIFCtrl.bSyncEn         = 0;
    SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_CCD_TG;
    SIFCtrl.uiSenDuration   = 2;
    SIFCtrl.uiLength        = 16;
    SIFCtrl.uiClkDiv        = 16;
    SIFCtrl.bMSB            = FALSE;
    sif_setControl(SIF_CH3, &SIFCtrl);
#endif
#endif

    SensorObj.FP_SetFlashCharge     = SenFP_ChargeFlash;
    SensorObj.FP_CheckFlashReady    = SenFP_IsFlashReady;
    SensorObj.FP_CheckFlashADCVlaue = SenFP_GetFlashADCValue;
    SensorObj.FP_ShowOSDString      = SenFP_ShowOSDString;
    SensorObj.FP_ClearOSD           = SenFP_ClearOSD;
    SensorObj.FP_PowerOffFlow       = SenFP_PowerOff;
    SensorObj.FP_GPIOMap_TurnOnCCDPower= GPIOMap_TurnOnCCDPower;
    SensorObj.FP_GPIOMap_TurnOffCCDPower= GPIOMap_TurnOffCCDPower;
    SensorObj.FP_GPIOMap_CloseShutter = NULL;
    SensorObj.FP_GPIOMap_OpenShutter  = NULL;

//#NT#2008/07/18#Bowen Li -begin
//#NT#Add DrawOSDRect function pointer
    SensorObj.FP_DrawOSDRect = NULL;
//#NT#2008/07/18#Bowen Li -end
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
#if (SYSINIT_OUTPUT_MCLK == DISABLE)
    SensorObj.bSieMClkEn = DISABLE;
#else
    SensorObj.bSieMClkEn = ENABLE;
#endif
//#NT#2009/10/08#Bowen Li -end
    Open_Sensor(&SensorObj);

    GPIOMap_TurnOnCCDPower();
   TimerDelayMs(300);
}

/**
  Init audio

  Init audio

  @param void
  @return void
*/
static void SysInit_InitAudio(void)
{
    AUDIO_SETTING       AudioSetting;
    AUDIO_DEVICE_OBJ    AudioDevice;
    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
#if (_AUDIOEXTCODEC_ == _AUDIOEXTCODEC_HDMI630_)
    AUDIO_CODEC_FUNC    AudCodecFunc;
#endif
    //#NT#2010/01/29#Chris Hsu -end

#if (_AUDIOCODEC_ == _AUDIOCODEC_EAC630_)
    // Default ALC off, MIC BOOST on
    // Each task using Audio must set the corresponding EventHandler if desired
    AudioDevice.pEventHandler       = NULL;
    // The following settings are "Don't care" for NT96432 built-in audio codec.
    AudioDevice.uiGPIOColdReset     = 0;
    AudioDevice.uiI2SCtrl           = AUDIO_I2SCTRL_NONE;
    AudioDevice.uiChannel           = 0;
    AudioDevice.uiGPIOData          = 0;
    AudioDevice.uiGPIOClk           = 0;
    AudioDevice.uiGPIOCS            = 0;
    AudioDevice.uiADCZero           = 0;

    // The following settings are for NT96630 built-in audio codec.
    // Don't modify the value that is "must be".
    // Set default sampling rate, channel, etc.
    AudioSetting.Clock.bClkExt      = FALSE;                        // Must be
    AudioSetting.Clock.Clk          = AUDIO_CLK_12288;              // Clock = Sampling rate * 256
    AudioSetting.I2S.bMaster        = TRUE;                         // Must be
    AudioSetting.I2S.I2SFmt         = AUDIO_I2SFMT_LIKE_LSB;        // Must be
    AudioSetting.I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
    AudioSetting.Fmt                = AUDIO_FMT_I2S;                // Must be
    AudioSetting.SamplingRate       = AUDIO_SR_32000;               // Sampling rate = Clock / 256
    AudioSetting.Channel            = AUDIO_CH_RIGHT;               // Audio channel
    AudioSetting.RecSrc             = AUDIO_RECSRC_MIC;             // Must be
    AudioSetting.Output             = AUDIO_OUTPUT_SPK;             // Output source
    AudioSetting.bEmbedded          = TRUE;                         // Must be, if you want to use the audio I/O pin
#else
    #error Unknow Audio Codec
#endif

    //#NT#2010/01/29#Chris Hsu -begin
    //#NT#Support multiple audio codec
    // Configure extended audio codec, must handle before aud_init
#if   (_AUDIOEXTCODEC_ == _AUDIOEXTCODEC_NONE_)
    // Do nothing
#else
  #if (_AUDIOEXTCODEC_ == _AUDIOEXTCODEC_HDMI630_)
    audcodecHDMI_getFP(&AudCodecFunc);
#endif
    aud_setExtCodec(AUDIO_CODECSEL_EXT_0, &AudCodecFunc);
#endif
    //#NT#2010/01/29#Chris Hsu -end

    // Open audio driver
    aud_open(&AudioDevice);

    // Init Audio driver
    aud_init(&AudioSetting);

    // Enable ALC function
    aud_setFeature(AUDIO_FEATURE_ALC, TRUE);

    //#NT#2010/04/14#Meg Lin -begin
    //fixbug: avoid pop noise when tv plug in
    aud_setFeature(AUDIO_FEATURE_HP_PWR_ALWAYSON, TRUE);
    //#NT#2010/04/14#Meg Lin -end

    // Enable Mic BOOST function
    aud_setFeature(AUDIO_FEATURE_MICBOOST, TRUE);
    // Enable Noise Gate with boost function
    // Must enable if Microphone BOOST function is enabled
    //aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST, TRUE);
    aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST, FALSE);//2010/01/20 Meg Lin, disable this to fix audio bug

    // Enable playback PCM data expand function (duplicate mono PCM data to playback on stereo ch)
    aud_setFeature(AUDIO_FEATURE_PLAYBACK_PCM_EXPAND, TRUE);


    // Set default Gain
    aud_setGain(AUDIO_GAIN_6);

    // Set default Volume
    // Only default value, UI will set volume again whenever UI starts!
    aud_setVolume(AUDIO_VOL_7);

    // Let another task can open Audio driver
    aud_close();
}

/**
  Init Gx lib

  Init Gx lib

  @param void
  @return void
*/
static void SysInit_InitGx(void)
{
    UINT32 uiPoolAddr;

    get_blk((VP*)&(uiPoolAddr), POOL_ID_GX);
    rel_blk(POOL_ID_GX, (VP)uiPoolAddr);
    debug_err(("\n\r"));
    debug_err(("GX buf=%08x, size=%d;\n\r",uiPoolAddr, POOL_SIZE_GX));
    GxGfx_Init((UINT32*)uiPoolAddr,POOL_SIZE_GX);       //initial Graphics

    // Init Gx Display
    debug_err(("GX win.w=%d, win.h=%d;\n\r",g_LCDSize.uiWinWidth, g_LCDSize.uiWinHeight));
    GxDisplay_Init(0, g_LCDSize.uiWinWidth, g_LCDSize.uiWinHeight);

    // Init UI Render for UI Framework
    Ux_SetRender(&demoRender);

    // Init OSD buffer
    UI_BeginDisplay(DD_OSD, TRUE);
    UI_BeginDisplay(DD_OSD2, TRUE);

    // Init VDO buffer
    UI_BeginDisplay(DD_VDO2, TRUE);

    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDOCOMPARE, COMPARE_KEY_NEVER); //enable VDO1/VDO2 color key
    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDOBLEND, BLEND_OPAQUE_000); //set blending VDO1=0%, VDO2=100%
    GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDOSWAP, FALSE);
    GxDisplay_Set(LAYER_VDO1, LAYER_STATE_ENABLE, 0); //turn on VDO1
    GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, 0); //turn on VDO2
    GxDisplay_FlushAll();
}

/**
  Init IR Remote Module

  Init IR Remote Module

  @param void
  @return void
*/
#if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
static void SysInit_InitIrRx(void)
{
    IRRX_DEVICE_OBJ IrRxDevice;

    //memset((void *)&IrRxDevice, 0, sizeof(IRRX_DEVICE_OBJ));

    // Open IrRx driver
    irrx_open((PIRRX_DEVICE_OBJ)&IrRxDevice);
}
#endif

/**
  Init watch dog timer

  Init watch dog timer

  @param void
  @return void
*/
#if (_WDT_CONTROL_ == ENABLE)
static void sysInit_InitWDT(void)
{
    wdt_open();
    wdt_setIntEn(FALSE);
    wdt_setRstEn(TRUE);
    wdt_setNahbRstEn(FALSE);
    wdt_setMS4B(0xF); // system will reboot if no tirgger wdt in 5.59 sec interval
    wdt_setTrig();
    wdt_setEnable();
}
#endif

/**
  Init hardware

  Init hardware

  @param void
  @return void
*/
static void SysInit_InitHW(void)
{
#if (SYSINIT_OUTPUT_MCLK == ENABLE)
    //#NT#2010/02/10#Chris Hsu -begin
    //#NT#Sensor MCLK clock source is PLL1
    // Output MCLK to sensor
    pinmux_select_mclk(PINMUX_MCLK);
    // MCLK clock source is PLL1
    pll_setClockRate(PLL_CLKSEL_SIE_MCLKSRC, PLL_CLKSEL_SIE_MCLKSRC_PLL1);
    //#NT#2010/02/10#Chris Hsu -end
#endif

    // Default USB high speed enabled
    usb_setChargerDetection(TRUE);
    usb_setChargerDetectTimeout(800);
    usb_PowerOnInit(TRUE);

#if (_STORAGECARD_ == _STORAGECARD_XD_)
    // Init xD card
    nand_enableCardProtection(TRUE);
    XDUser_EnableCardPower(FALSE);
    xd_initCardIOFunc(XDUser_EnableCardPower);
#endif

#if(_ENCRYPTE_ != ENCRYPTE_NONE)
   // Init Encrypte Funtion
    SysInit_InitENDE();
    EnDeFunction();//MP
#endif     
    SysInit_InitSensor();

    SysInit_InitAudio();

    VolDet_Init();

    KeyADC_Init();

#if (_WDT_CONTROL_ == ENABLE)
    sysInit_InitWDT();
#endif

    //#NT#2010/03/31#JustinLee -begin
    // Init IR Remote controller
    #if (_IR_REMOTE_ != _IR_REMOTE_NONE_)
    SysInit_InitIrRx();
    #endif
    //#NT#2010/03/31#JustinLee -end

    // Open Play Sound Task
    PlaySound_Open(NULL);

    // Play opening sound
    KeyScan_PlaySound(KEYSCAN_SOUND_STARTUP);


}

#if 0
#pragma mark -
#endif

/**
  Get system parameters from NAND

  Get system parameters from NAND

  @param void
  @return void
*/
static void SysInit_GetNANDSysParam(void)
{
    debug_ind(("SysInit_GetNANDSysParam\n\r"));

    Init_MenuInfo();
}

/**
  Get sensor adjust data from NAND

  Get sensor adjust data from NAND

  @param void
  @return void
*/
void SysInit_GetNANDSensorParam(void)
{
    ER err_code;
    err_code = ReadCalData();
    if(err_code !=E_OK)
        debug_err(("SysInit_GetNANDSensorParam: check num 0x%x\r\n",err_code));
}

/**
  Init bootloader info object from memory

  Init bootloader info object from memory

  @param void
  @return void
*/
static void SysInit_InitBLInfo(void)
{
    debug_ind(("SysInit_InitBLInfo\n\r"));

    Init_BLInfo();
}

/**
  Get PStore size by user define

  Get PStore size by user define

  @param void
  @return void
*/
UINT32 SysInit_GetPStoreSize(void)
{
    UINT32 uiSize = 0;

#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    // SPI: 300KB, temp size
    uiSize = (200 * 1024);
#else
    #if (_MP3_MODE_ != ENABLE)
    // NAND: 1MB
    uiSize = (1 * 1024 * 1024);
    #else
    // NAND: 6MB, MP3 unicode font need more 5MB size!
    uiSize = (6 * 1024 * 1024);
    #endif
#endif

    debug_err(("SysInit_GetPStoreSize: PStore size = 0x%08X\n\r", uiSize));
    return uiSize;
}

/**
  Initialize Persist Store

  Initialize Persist Store.

  @param void
  @return void
*/
void SysInit_InitPStore(void)
{
    PSTORE                  PStoreStruct;
    PPSTORE_SECTION_HANDLE  pSection;
    PPSSTRG_TAB             pPSStorage;
    UINT32                  uiFWLen;
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    UINT32                  uiFlashSize;
#endif

#if (_USBMSDC_MODE_ == ENABLE)
#if ((_MSDC_MODE_ == _MSDC_ALL_NAND3_) || (_MSDC_MODE_ == _MSDC_SINGLE_NAND2_) || (_MSDC_MODE_ == _MSDC_ALL_NAND2_))
    nand2_setSize(NAND2_SIZE);
#endif
#if ((_MSDC_MODE_ == _MSDC_SINGLE_NAND3_) || (_MSDC_MODE_ == _MSDC_ALL_NAND3_))
    nand3_setSize(NAND3_SIZE);
#endif
#endif

#if (_PSTOREDEVICE_ != _PSTOREDEVICE_SPIFLASH_)
    // Set reserved size for FW code
    #if 0
    nand_setReservedSize(FW_CODE_SIZE);
    #else
    {
        UINT32 uiFWReservedSZ;

        uiFWReservedSZ = *(volatile UINT32 *)(FW_RESERVED_SIZE_OFFSET);
        debug_ind(("FW_RESERVED_SIZE = 0x%x\r\n", uiFWReservedSZ));
        nand_setReservedSize(uiFWReservedSZ);
        //#NT#2009/10/29#Niven Cho -begin
        //#NT#Added., for MSDC update F/W needs to set start block
        nand_setReservedStartBlockIndex(FW_START_NAND_BLOCK);
        //#NT#2009/10/29#Niven Cho -end
    }
    #endif

    // Set system parameter size to zero since PStore doesn't need this
    nand_setSysParamBlockNum(0);
#endif
    // Init PSTore size to 1MB
    uiFWLen = *(volatile UINT32 *)FW_LENGTH_OFFSET;

    if(uiFWLen == 0)
    {
        debug_err(("FW len = 0 might be error\r\n"));
        uiFWLen = 1024 * 16;
    }


#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    uiFlashSize = *(UINT32 *)FLASH_SIZE_OFFSET;
    debug_err(("Flash Size = 0x%x\r\n", uiFlashSize));
    if(uiFlashSize == 0)
    {
        debug_err(("something wrong ...\r\n"));
        uiFlashSize = 0x200000;
    }
#endif

    // Init PSTore size
    PStoreStruct.uiSignature  = 0x55AA55AA;
    PStoreStruct.uiPSVersion  = 0x0001;
    PStoreStruct.uiPStoreSize = SysInit_GetPStoreSize();
        debug_err(("uiFlashSize : %08x  PS size :%08x  uiFWLen : %08x\r\n",uiFlashSize,PStoreStruct.uiPStoreSize,uiFWLen));
	
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
#if 0
    if ((PStoreStruct.uiPStoreSize+0x4000+uiFWLen)>uiFlashSize) {
        debug_err(("PStore & FW overlap\r\n"));
        while (1);
    }
#endif
#endif

    // Get PStore storage object
#if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
    pPSStorage = nandps_getStorageObject();
#elif (_PSTOREDEVICE_ == _PSTOREDEVICE_SPIFLASH_)
    pPSStorage = norps_getStorageObject();
#else
    #error PStore type has not selected yet and PStore is not supported!
#endif
        debug_err(("PStore init....\r\n"));

    // Open PStore
    switch (PStore_Init(&PStoreStruct, pPSStorage))
    {
    case E_PS_CHKER:
    case E_PS_EMPTY:
    case E_PS_VERNEW:
		
        // Enable PStore
        PStore_EnablePS();

        // Format PStore
        PStore_Format(&PStoreStruct);

        // Create default section
        // System parameter
        pSection = PStore_OpenSection(PS_SYS_PARAM, PS_RDWR | PS_CREATE);
        PStore_CloseSection(pSection);

        // Calibration data
        pSection = PStore_OpenSection(PS_CAL_DATA, PS_RDWR | PS_CREATE);
        PStore_CloseSection(pSection);
        break;

    case E_PS_VEROLD:
        break;

    case E_OK:
        debug_err(("PStore init....E_OK\r\n"));
		
        // Everything is OK, open system parameter section?
        break;
    }
}

CHAR pFolderName[6]="MEDIA";
CHAR pFileName[5]="PTDC";

static void SysInit_InitTimeDCFFolder(void)
{

  RTC_DATE  curDate;
  RTC_TIME  curTime;

    curDate = rtc_getDate();
    curTime = rtc_getTime();
    sprintf(pFolderName,"%1d%02d%02d",curDate.s.year%0x0A,curDate.s.month,curDate.s.day);
    sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
}

/**
  Init firmware

  Init firmware

  @param void
  @return void
*/
static void SysInit_InitFW()
{
 UINT32              uiPoolAddr;
    PRIMARY_DCFNAME DCFName = {
                                FALSE,       // Create folder when init File System or not
                                "MEDIA",    // Folder name
                                "PTDC",     // File name
                                "PTDC",     // Video file name
#if (_MOVIE_FMT_ == _MOVIE_FMT_MOVH264_)
                                "AVI",      // Video extension name //2010/12/15 Meg Lin
#else //_MOVIE_FMT_AVIH264_
                                "AVI",      // Video extension name
#endif
                                "PTDC",     // Audio file name
                                "WAV",      // Audio extension name
                                "PTDC",     // RAW file name
                                "PTDC",     // TIFF file name
                                //#NT#2011/04/08#Chris Chung -begin
                                //#add custom type
                                "BIN"       // Custom extension name
                                //#NT#2011/04/08#Chris Chung -end
                              };

    PSTRG_TAB   pStorageObj;
#if (_RAM_DISK_ == ENABLE)
    RamDiskSettings     RamDisk;
#endif

    //#NT#add more SD init sample code
#if 0
    SDMMC_HOST_CONFIG_STRUCT sdConfig;
#endif
    //#NT#2009/03/17#HH Chuang -end

#if (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
    memcpy(DCFName.pDirName,pFolderName,sizeof(pFolderName));
    memcpy(DCFName.pFileName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pVideoName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pAudioName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pRAWName,pFileName,sizeof(pFileName));
    memcpy(DCFName.pTIFFName,pFileName,sizeof(pFileName));
    MediaRec_SetCrashFolderName("ABCDE\0"); // this is just a sample name and can be customized
    MediaRec_SetTheSameCrashDid(1);
#endif
    // Initialize DCF name
    Primary_InitDCFName(&DCFName);

    FilesysEnableRecycler(FALSE);

    // Initialize card's storage object for file system
#if   (_STORAGECARD_ == _STORAGECARD_XD_)
    pStorageObj = xd_getStorageObject();
#elif (_STORAGECARD_ == _STORAGECARD_MSPRO_)
    pStorageObj = mspro_getStorageObject();
#elif (_STORAGECARD_ == _STORAGECARD_CF_)
    pStorageObj = cf_getStorageObject();
    cfcard_selectMode(CFC_CARDTYPE_CF,CF_MEM_MAP_MODE);
#elif (_STORAGECARD_ == _STORAGECARD_SDIO_)
    pStorageObj = sdio_getStorageObject();
    sdio_setDetectCardExistHdl(SDIOCardUser_CheckCardInserted);
    sdio_setDetectCardProtectHdl(SDIOCardUser_CheckCardWP);
    //#NT#2009/03/17#HH Chuang -begin
    //#NT#add more SD init sample code
#if 0
    sdConfig.bEnableHS = TRUE;
    sdConfig.bSetDriveSink = TRUE;
    sdConfig.ucMaxBusWidth = SD_HOST_BUS_4_BITS;
    sdio_setConfiguration(&sdConfig);
    sdio_setFormatParam("NOVATEK", "SD_VOL", TRUE, 0);
#endif
    //#NT#2009/03/17#HH Chuang -end
#elif (_STORAGECARD_ == _STORAGECARD_SDIO2_)
    pStorageObj = sdio2_getStorageObject();
    sdio2_setDetectCardExistHdl(SDIO2CardUser_CheckCardInserted);
    sdio2_setDetectCardProtectHdl(SDIO2CardUser_CheckCardWP);
#else
    #error Unknown storage card
#endif
    Primary_InitStorageCard(pStorageObj);

#if (_RAM_DISK_ == ENABLE)
    SysGetRamDiskAddr(&RamDisk.baseaddr);
    RamDisk.size = POOL_SIZE_RAM_DISK;
    ramdsk_init(&RamDisk);
    ramdsk_open();
    ramdsk_format();
    ramdsk_close();

    /**
    SysGetRam2DiskAddr(&RamDisk.baseaddr);
    RamDisk.size = POOL_SIZE_RAM2_DISK;
    ramdsk2_init(&RamDisk);
    ramdsk2_open();
    ramdsk2_format();
    ramdsk2_close();
    **/
#endif

    // Init Gx Gfx
    SysInit_InitGx();

    // Read system parameters from NAND
    SysInit_GetNANDSysParam();

    // Read sensor adjust data from NAND
    SysInit_GetNANDSensorParam();

    // Init bootloader info object from memory
    SysInit_InitBLInfo();

    // Init AF
    SysInit_InitAF();

#if (_GPS_RECEIVER_ == ENABLE && _GPS_RECEIVER_LOG_ == ENABLE)
    // Init GPS Log buffer
    // Get memory for GPS log task
    get_blk((VP*)&(uiPoolAddr), POOL_ID_DEFECT_PIXEL);
    rel_blk(POOL_ID_DEFECT_PIXEL, (VP)uiPoolAddr);
    GPSRec_SetGPSBufAddr(uiPoolAddr,(UINT32)(POOL_SIZE_DEFECT_PIXEL));
#endif

}

void SysInitExifHeader(void)
{
    Vendor_EXIF_HEAD userExifHeader;

    userExifHeader.pMakerString = "Novatek";
    userExifHeader.pModelString = "DV Turnkey";
    userExifHeader.pSoftwareVer = (CHAR *)GetVersionString();
    userExifHeader.pMakeNote = NULL;
    userExifHeader.pImageDescription = NULL;
    JpgSetEXIFString(&userExifHeader);
}
#define RAND_MAX_NUM 100

#if(_ENCRYPTE_ == CK235_PW01)
int EnDeFunction(void)
{
	//	8字节随机数
	UINT8 randata[8]={0x29,0xf5,0x25,0xf3,0xaa,0x6f,0x84,0xe9};
	//	10字节加密后的数据
	UINT8 returndata[10]={0};
	//	8字节解密后的数据
	UINT8 exdata[8]={0};

	UINT8 i=0;
	UINT8 j=0;

 	RTC_TIME  curTime;

    curTime = rtc_getTime();
    //sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
	//srand( (UINT8)time( NULL ) );
	srand( (UINT8)(curTime.s.second) );

	for(i=0;i<8;i++)
	{
		randata[i]=1+(UINT8)(100.0*rand()/(RAND_MAX_NUM+1.0));
		DEBUG_MSG(("EnDeFunction Rand data[%d] 0x%x\r\n",i,randata[i]));
	}
    _i2c_write(tmp_dev_addr, tmp_sub_addr,randata, 8);
    _i2c_read(tmp_dev_addr, tmp_sub_addr,returndata, 10);

	//	对returndata用密钥key解密,结果在exdata
	EDesEn_Crypt(returndata,exdata);

	//	比较原始数据randata与解密出来的exdata
	for(j=0;j<8;j++)
	{
		if(randata[j] !=exdata[j])
		{
			DEBUG_MSG(("exdata[%d] : 0x%x\r\n",j,exdata[j]));
			break;
		}
		else
		{
			DEBUG_MSG(("EnDeFunction================================>ok %d\r\n",j));
		}
	}
	if(j==8)
	{
		DEBUG_MSG(("EnDeFunction================================>ok\r\n"));
	}
	else
	{
		DEBUG_MSG(("EnDeFunction================================>Err\r\n"));
		KeyScan_PoweroffFlow();
	}
	return 0;
}
#else
int EnDeFunction(void)
{
	//	8字节随机数
	UINT8 randata[8]={0x29,0xf5,0x25,0xf3,0xaa,0x6f,0x84,0xe9};
	//	10字节加密后的数据
	UINT8 returndata[8]={0};
	UINT8 tmpdata[8]={0};
	//	8字节解密后的数据
	UINT8 exdata[8]={0};

	UINT8 a;
	UINT8 b;
	UINT8 c;
	UINT8 d;
	UINT8 e;
	UINT8 f;
	UINT8 g;
	UINT8 h;
	
	UINT8 i=0;
	UINT8 j=0;

 	RTC_TIME  curTime;

    curTime = rtc_getTime();
    //sprintf(pFileName,"%02d%02d",curTime.s.hour,curTime.s.minute);
	//srand( (UINT8)time( NULL ) );
	srand( (UINT8)(curTime.s.second) );

	#if 1
	for(i=0;i<8;i++)
	{
		randata[i]=1+(UINT8)(100.0*rand()/(RAND_MAX_NUM+1.0));
		//DEBUG_MSG(("EnDeFunction================================> %x\r\n",randata[i]));
		debug_msg("EnDeFunction================================> %x\r\n",randata[i]);
	}
	#endif
	//randata[8] = {A*B , B-H, C+D, E*C, A-C, B+F, F*H, A-E}; 

	for(i=0;i<8;i++)
	{
		//randata[i]= i;
		DEBUG_MSG(("EnDeFunction================================> %x\r\n",randata[i]));
		//debug_msg("Send================================> %x\r\n",randata[i]);
	}

	if(randata[0]==0xC1)
	{
		randata[0] = 0xBB;
	}
	a = randata[0];
	b = randata[1];
	c = randata[2];
	d = randata[3];
	e = randata[4];
	f = randata[5];
	g = randata[6];
	h = randata[7];

	tmpdata[0] = a*b;//3d
	tmpdata[1] = b-h;//0c
	tmpdata[2] = c+d;//18
	tmpdata[3] = e*c;//92
	tmpdata[4] = a-c;//04
	tmpdata[5] = b+f;//64
	tmpdata[6] = f*h;//07
	tmpdata[7] = a-e;//7f

	for(i=0;i<8;i++)
	{
		//DEBUG_MSG(("tmpdata================================>Err %x\r\n",randata[i]));
		//debug_msg("tmpdata================================>tmpdata %x\r\n",tmpdata[i]);
	}
    _i2c_write(tmp_dev_addr, tmp_sub_addr,randata, 8);
	TimerDelayMs(30);
    _i2c_read(tmp_dev_addr, tmp_sub_addr,returndata, 8);

	for(i=0;i<8;i++)
	{
		//returndata[i]= i;
		DEBUG_MSG(("Read================================> %x\r\n",returndata[i]));
		//debug_msg("Read================================> %x\r\n",returndata[i]);
	}		
	//	对returndata用密钥key解密,结果在exdata
	//EDesEn_Crypt(returndata,exdata);

	//	比较原始数据randata与解密出来的exdata
	for(j=0;j<8;j++)
	{
		if(tmpdata[j] !=returndata[j])
		{
			DEBUG_MSG(("EnDeFunction================================>Err %x\r\n",j));
			break;
		}
		else
		{
			DEBUG_MSG(("EnDeFunction================================>ok %x\r\n",j));
		}
	}
	if(j==8)
	{
		DEBUG_MSG(("EnDeFunction================================>ok\r\n"));
	}
	else
	{
		DEBUG_MSG(("EnDeFunction================================>Err\r\n"));
		KeyScan_PoweroffFlow();
	}
	return 0;
}
#endif

/**
  System initialization

  Do system initialization

  @param void
  @return void
*/
void SystemInit(void)
{
#if (_LONG_STARTUP_ == ENABLE)
    // start up guiResetShutDownTimerID timer
    SysInit_ResetShutDownTimer();
#endif
    // Init essential HW
    SysInit_InitEssentialHW();

    // Start some task before HW and FW initialization
    SysTask_StartTaskPhase1();

    // For partial loading
    // Run part one code
    CopyRest_RunPartOne();
	

    // Copy part two FW code
    CopyRest_CopyPartTwo();

    // Run part two FW code
    CopyRest_RunPartTwo();

//#NT#2012/09/05#Philex Lin -begin
//#NT# pressing power button longer to startup system
#if (_LONG_STARTUP_ == ENABLE)

    while (guiResetSDTimerCount < RESET_SHUTDOWN_TIMER_COUNT)
        ;

    // Init LCD and display splash screen
    SysInit_InitLCD();

    // Display poweron logo here
    if (FALSE == UI_ShowLogoFile(PS_BG_LOGO))
     {
      SysInit_DisplaySplashScreen(LOGO_STARTUP,0,0);
     }

    TimerDelayMs(1000);

#endif
//#NT#2012/09/05#Philex Lin -end

    // Copy part three FW code
    //CopyRest_CopyPartThree();

    // Initialize HW
    SysInit_InitHW();

#if (UI_STYLE == UI_STYLE_DRIVERECORDER||UI_STYLE == UI_STYLE_DRIVERECORDERTOUCH)
    // init
    SysInit_InitTimeDCFFolder();
#endif
    // gsensor init
    gsensor_open();
    // Initialize FW
    
    SysInit_InitFW();

    // Start some task after HW and FW initialization
    SysTask_StartTaskPhase2();

    SysInitExifHeader();

    //EnDeFunction();//MP

}

//@}
