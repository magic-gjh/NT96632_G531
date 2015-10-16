/*
    @file       AUA027DN01.c

    @ingroup    mILIBLCDAUA027DN01

    @brief      AU LCD Driver of AUA027DN01 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDAUA027DN01

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0000).

@endverbatim

@{
*/
#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Utility.h"


#define AU_DN01_UPS051_Buf_width       960
#define AU_DN01_UPS051_Buf_height      240
#define AU_DN01_UPS051_Win_width       960
#define AU_DN01_UPS051_Win_height      240

#if 1
#define AU_DN01_UPS052_Buf_width       320
#define AU_DN01_UPS052_Buf_height      240
#define AU_DN01_UPS052_Win_width       320
#define AU_DN01_UPS052_Win_height      240
#else
#define AU_DN01_UPS052_Buf_width       360
#define AU_DN01_UPS052_Buf_height      240
#define AU_DN01_UPS052_Win_width       360
#define AU_DN01_UPS052_Win_height      240
#endif

#define AU_DCN01_CCIR656_Buf_width      720
#define AU_DN01_CCIR656_Buf_height     480
#define AU_DN01_CCIR656_Win_width      720
#define AU_DN01_CCIR656_Win_height     480

//#NT#2007/03/07#Shirley Cheng -begin
//Add YUV 640 mode support
#define AU_DN01_YUV640_Buf_width       640
#define AU_DN01_YUV640_Buf_height      240
#define AU_DN01_YUV640_Win_width       640
#define AU_DN01_YUV640_Win_height      240
//#NT#2007/03/07#Shirley Cheng -end

//UINT8 gAUCN01_VCOM_DC = 0x18;

//function declaration
static void delay(UINT32 times);
static void sendtoAUDN01LCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoAUDN01LCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoAUDN01LCDreg(UINT8 addr, UINT8 value);

static void Init_TXDT270M_LCD(void);
static void OpenTXDT270M(LCDSIZE *psizeDisplay);
void CloseTXDT270M(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

//#NT#2010/06/07#Niven Cho -begin
//#NT#Adjust Gamma
static UINT8 GammaTbl[] = {
       0, 14, 28, 41, 55, 68, 80, 91,102,112,
    120,127,134,146,154,161,168,
};
//#NT#2010/06/07#Niven Cho -end

/**
  Open LCD panel driver

  This function should be called before calling any other functions during initial LCD driver, public function for global use

  @param lcddeviceobj initial value by upper layer
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenPanel(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay)
{
    LCD_DeviceObj = *lcddeviceobj;

    if (LCD_DeviceObj.fIDEDstCLK == 0)
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case LCD_MODE_RGB:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;

        case LCD_MODE_RGBD:
            LCD_DeviceObj.fIDEDstCLK = 24.535;
            break;
        //#NT#2007/03/07#Shirley Cheng -begin
        case LCD_MODE_YUV:
            LCD_DeviceObj.fIDEDstCLK = 24.535;
            break;
        //#NT#2007/03/07#Shirley Cheng -end

        }
    }

    OpenTXDT270M(psizeDisplay);
}

/**
    Write LCD Internal Register

    Optional Implementation

    @param[in] addr     LCD panel register address
    @param[in] value    Write value

    @return void
*/
void LCD_RegWrite(UINT32 addr, UINT32 value)
{
 
}

/**
  delay time period

  @param times : time period

*/
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/**
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoAUDN01LCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = ((((UINT8)addr << 8)|(UINT8)value ) << 16);
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/**
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoAUDN01LCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)addr << 8 | (UINT32)value;

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 16; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(200);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendtoAUDN01LCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoAUDN01LCDreg_sif(addr, value);
    }
    else
    {
         sendtoAUDN01LCDreg_gpio(addr, value);
    }
}

/**
    Initial LCD of AU027DN01, configure SIF intarface and send data to panel register to configure

*/
static void Init_TXDT270M_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();
        //#NT#2007/05/08#Steven Wang -begin
        //#Modify init flow, SIF driver will change pinmux
        /*
        // Init SIF
        switch (LCD_DeviceObj.LCDCtrl_Pin.uisif_channel)
        {
        case SIF_CH0:
        default:
            pinmux_enable_sif(PINMUX_SIF_CH0);
            break;
        case SIF_CH1:
            pinmux_enable_sif(PINMUX_SIF_CH1);
            break;
        case SIF_CH2:
            pinmux_enable_sif(PINMUX_SIF_CH2);
            break;
        case SIF_CH3:
            pinmux_enable_sif(PINMUX_SIF_CH3);
            break;
        }
        */
        //#NT#2007/05/08#Steven Wang -end
        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = SIF_MODE_0000;
        SIFCtrl.bSyncEn       = FALSE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;

        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    //#NT#2010/04/28#Philex Lin -begin
    // Tcon common register setting
    // panel startup procedure
    sendtoAUDN01LCDreg(0x05,0x1E);
    TimerDelayMs(100);
    sendtoAUDN01LCDreg(0x05,0x5d);
    TimerDelayMs(100);
    //#NT#2010/04/28#Philex Lin -end

    switch (LCD_DeviceObj.uiLCDMode)
    {
        case LCD_MODE_RGB:
        sendtoAUDN01LCDreg(0x04,0x04);  //8 bit RGB

        sendtoAUDN01LCDreg(0x0B,0x81);
//        sendtoAUDN01LCDreg(0x2B,0x01);
        sendtoAUDN01LCDreg(0x01,0x90); //Vcom DC
        //sendtoAUDN01LCDreg(0x03,0x00);
        sendtoAUDN01LCDreg(0x00,0x00); //Vcom AC
//        sendtoAUDN01LCDreg(0x05,0x5d); // set power mode(standby)
        sendtoAUDN01LCDreg(0x06,0x15);
//        sendtoAUDN01LCDreg(0x03,0x20); //set brightness
//        sendtoAUDN01LCDreg(0x07,0x00); // set HBLK
        sendtoAUDN01LCDreg(0x97,0x03); //VGL
        sendtoAUDN01LCDreg(0x96,0x02); //VGH


        //gamma
        sendtoAUDN01LCDreg(0x16,0x00); //SET Gamma reg.
        sendtoAUDN01LCDreg(0x17,0x00); //V16;V8
        sendtoAUDN01LCDreg(0x18,0x00); //V50;V32
        sendtoAUDN01LCDreg(0x19,0x00); //V96;V72
        sendtoAUDN01LCDreg(0x1A,0x00); //V120;V110
        sendtoAUDN01LCDreg(0xAC,0xff);
        //-------
        sendtoAUDN01LCDreg(0xB3,0x01);
        sendtoAUDN01LCDreg(0xB5,0x06);
        break;
    case LCD_MODE_RGBD:
        sendtoAUDN01LCDreg(0x05,0x1E);
        TimerDelayMs(150);
        sendtoAUDN01LCDreg(0x05,0x5d);
        TimerDelayMs(150);         
        sendtoAUDN01LCDreg(0x00,0x0b);  
        sendtoAUDN01LCDreg(0x01,0x94); //0xA0); Vcom DC
        sendtoAUDN01LCDreg(0x0B,0x80);
		sendtoAUDN01LCDreg(0xa5,0x22);//0x22
        //gamma
        sendtoAUDN01LCDreg(0x16,0x52); //(Gamma offset function turn off)
        sendtoAUDN01LCDreg(0x17,0x76);
        sendtoAUDN01LCDreg(0x18,0x97);
        sendtoAUDN01LCDreg(0x19,0xcb);
        sendtoAUDN01LCDreg(0x1a,0x99);
        sendtoAUDN01LCDreg(0x1b,0x63);
        sendtoAUDN01LCDreg(0x2f,0x6d);
        sendtoAUDN01LCDreg(0x5a,0x03);
        sendtoAUDN01LCDreg(0x2b,0x01);        
        sendtoAUDN01LCDreg(0x04,0x1f); //
        break;
    case LCD_MODE_YUV:
        sendtoAUDN01LCDreg(0x00,0xC0);//sendtoAUBL02LCDreg(0x00,0x0F);
        //sendtoAUDN01LCDreg(0x40,0x03);
        sendtoAUDN01LCDreg(0x04,0x44);  //YUV 640x240

        break;
    //#NT#2007/03/07#Shirley Cheng -end
    }
}

/**
  Open AU027DN01 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenTXDT270M(LCDSIZE *psizeDisplay)
{
    UINT8   uiClockDiv;

    if (ide_open() != E_OK)
        return;

    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Set IDE Clock Source
    if (LCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        LCD_DeviceObj.uiIDESrcCLK = 240;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_240);
    }
    else
    {
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq() / 1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }

    // wait for Power Stable
    //delay(0x10000);

    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv-1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for Power Stable
        //delay(0x10000);

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
        ide_set_pdir(IDE_PDIR_RBG);
        ide_set_odd(IDE_LCD_R);        // 0
        ide_set_even(IDE_LCD_B);    // 1
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);            ///////
        ide_set_shw_st();
        //ide_set_en(1);
        //ide_set_load();


        ide_set_hor_timing(0, 1715, 70, 1029);
        ide_set_ver_timing(0, 261, 21, 261, 21, 261);
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

//        ide_set_ctrst(0x40);
        ide_set_ctrst(0x48);
        ide_set_brt(0x00);
        //ide_set_cmults(0x40);
        ide_set_cmults(0x42);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay -> uiWidth = AU_DN01_UPS051_Buf_width;
        psizeDisplay -> uiHeight = AU_DN01_UPS051_Buf_height;
        psizeDisplay -> uiWinWidth = AU_DN01_UPS051_Win_width;
        psizeDisplay -> uiWinHeight = AU_DN01_UPS051_Win_height;

        break;
    case LCD_MODE_RGBD:
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv-1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for Power Stable
        //delay(0x10000);

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_odd(IDE_LCD_R);
        ide_set_even(IDE_LCD_G);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(1);
        ide_set_shw_st();


        ide_set_hor_timing(1, 1559, 241, 1520);
        ide_set_ver_timing(0, 261, 21, 260, 21, 260);
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x50);
        ide_set_brt(0x40);
        ide_set_cmults(0x50);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x01);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay -> uiWidth = AU_DN01_UPS052_Buf_width;
        psizeDisplay -> uiHeight = AU_DN01_UPS052_Buf_height;
        psizeDisplay -> uiWinWidth = AU_DN01_UPS052_Win_width;
        psizeDisplay -> uiWinHeight = AU_DN01_UPS052_Win_height;
        break;

    case LCD_MODE_YUV:
    //#NT#2007/03/07#Shirley Cheng -begin
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv-1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //delay(0x10000);

        ide_set_icst(0);
        ide_setICST(FALSE, CST_RGB2YCBCR);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_TOPPOLY);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(1);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_shw_st();
        //ide_set_en(1);
        //ide_set_load();
        /* //720x240
        ide_set_hor_timing(0x00, 0x359, 0x7D, 0x34D);
        ide_set_ver_timing(0x00, 0x105, 0x0D, 0xFE, 0x0D, 0xFE);  */
           //320x240
#if 1
        ide_set_hor_timing(0, 1559, 240,1519);
        ide_set_ver_timing(0, 261, 21, 260, 21, 260);
#else
        ide_set_hor_timing(0x01, 1716-1, 241, 1440+241/*1681*/);
        ide_set_ver_timing(0x01, 0x106, 0x15, 0x105, 0x15, 0x105);
#endif
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = AU_DN01_YUV640_Buf_width;
        psizeDisplay->uiHeight      = AU_DN01_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = AU_DN01_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = AU_DN01_YUV640_Win_height;
        break;
    //#NT#2007/03/07#Shirley Cheng -end

    }

    //delay(0x2000);
    //#NT#2010/06/07#Niven Cho -begin
    //#NT#Adjust Gamma
    ide_setGammaEn(TRUE);
    ide_fillGamma(GammaTbl);
    //#NT#2010/06/07#Niven Cho -end

    ide_set_en(TRUE);
    //avoid vsync active area
    ide_waitFrameEnd();

    Init_TXDT270M_LCD();
}

/**
  Close AU027DN01 driver

  Release AU027DN01 driver, private function for internal used

*/
void ClosePanel(void)
{
    // Disable IDE
    ide_wait_frame_end();
    ide_set_en(FALSE);

    // Change to standby mode
    sendtoAUDN01LCDreg(0x05,0x54);

    delay(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}
void TXDT270MOpenLcdBacklightReg(void)
{
	sendtoAUDN01LCDreg(0x05,0x5f);
}

//#NT#For LCD rotation
void LCD_Rotate(UINT32 bRotate)
{
    if (bRotate)
    {
        switch(LCD_DeviceObj.uiLCDMode)
        {
            case LCD_MODE_RGBD:
                sendtoAUDN01LCDreg(0x04,0x17);
                break;
            case LCD_MODE_RGB:
                sendtoAUDN01LCDreg(0x04,0x07);
                ide_wait_frame_end();
                ide_set_pdir(IDE_PDIR_RGB);
                ide_set_even(IDE_LCD_G);
                ide_set_load();
                break;
        }
    }
    else
    {
        switch(LCD_DeviceObj.uiLCDMode)
        {
            case LCD_MODE_RGBD:
                sendtoAUDN01LCDreg(0x04,0x14);
                break;
            case LCD_MODE_RGB:
                ide_wait_frame_end();
                sendtoAUDN01LCDreg(0x04,0x04);
                ide_set_pdir(IDE_PDIR_RBG);
                ide_set_even(IDE_LCD_B);
                ide_set_load();
                break;
        }

    }
}

//@}
