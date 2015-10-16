/*
    @file       ToppolyTD20.c

    @ingroup    mILIBLCDToppolyTD20

    @brief      AU LCD Driver of ToppolyTD24THEA1 panel, Only support YUV and RGBD mode now.

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDToppolyTD20

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
#include "debug.h"

#define Toppoly_Buf_width              640
#define Toppoly_Buf_height             240
#define Toppoly_Win_width              640
#define Toppoly_Win_height             240

#define Toppoly_RGBD_Buf_width         320
#define Toppoly_RGBD_Buf_height        240
#define Toppoly_RGBD_Win_width         320
#define Toppoly_RGBD_Win_height        240



//function declaration
static void delay_Toppoly(UINT32 times);
static void sendtoToppolyLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoToppolyLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoToppolyLCDreg(UINT8 addr, UINT8 value);

static void Init_Toppoly_LCD(void);
static void OpenToppoly(LCDSIZE *psizeDisplay);
static void CloseToppoly(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;
static UINT8 GammaTbl[] = {
0, 13, 27, 41, 55, 69, 83, 98,114,130,
146,164,182,200,216,233,255
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
            LCD_DeviceObj.fIDEDstCLK = 24.54;//27;
            break;
        case LCD_MODE_YUV:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        }
    }

    OpenToppoly(psizeDisplay);
}

/**
  Close LCD driver, public function for global use

  Release LCD driver

*/
void ClosePanel(void)
{
    CloseToppoly();
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
    sendtoToppolyLCDreg(addr, value);
}

/**
  delay time period

  @param times : time period

*/
static void delay_Toppoly(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/**
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = (((UINT32)addr << 8)|(UINT32)value ) << 16;

    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/**
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg_gpio(UINT8 addr, UINT8 value)
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

    delay_Toppoly(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 16; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay_Toppoly(100);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(200);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(100);
    }

    delay_Toppoly(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoToppolyLCDreg_sif(addr, value);
    }
    else
    {
         sendtoToppolyLCDreg_gpio(addr, value);
    }
}

/**
    Initial LCD of ToppolyTD20, configure SIF intarface and send data to panel register to configure

*/
static void Init_Toppoly_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

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
    
    //Set LCD register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
        //---software reset----// 
		sendtoToppolyLCDreg(0x00, 0x0F);  
		sendtoToppolyLCDreg(0x00, 0x05);//0x01
		TimerDelayMs(50);
		sendtoToppolyLCDreg(0x00, 0x0F);//
		sendtoToppolyLCDreg(0x00, 0x05);//
		TimerDelayMs(50);
		sendtoToppolyLCDreg(0x00, 0x0F);  //
		TimerDelayMs(100);
		//---software reset----// 
		sendtoToppolyLCDreg(0x50, 0x40);   //VCOM DC SETTING
		TimerDelayMs(10);
		sendtoToppolyLCDreg(0x60, 0x01);   //8 bit RGB Dummy
		TimerDelayMs(10);
		sendtoToppolyLCDreg(0x60, 0x01);   //8 bit RGB Dummy 
		//sendtoToppolyLCDreg(0x40, 0x03); // direction selection
		sendtoToppolyLCDreg(0x30, 0x08);   //300a
		sendtoToppolyLCDreg(0x70, 0x3b);   //0x36 
		sendtoToppolyLCDreg(0xc0, 0x05); 
        break;
    case LCD_MODE_YUV:
        //---software reset----// 
		sendtoToppolyLCDreg(0x00, 0x0F);  
		sendtoToppolyLCDreg(0x00, 0x05);//0x01
		TimerDelayMs(50);
		sendtoToppolyLCDreg(0x00, 0x0F);//
		sendtoToppolyLCDreg(0x00, 0x05);//
		TimerDelayMs(50);
		sendtoToppolyLCDreg(0x00, 0x0F);  //
		TimerDelayMs(100);
		//---software reset----// 
		sendtoToppolyLCDreg(0x50, 0x40);   //VCOM DC SETTING
		TimerDelayMs(10);
		sendtoToppolyLCDreg(0x60, 0x01);   //8 bit RGB Dummy
		TimerDelayMs(10);
		sendtoToppolyLCDreg(0x60, 0x01);   //8 bit RGB Dummy 
		//sendtoToppolyLCDreg(0x40, 0x03); // direction selection
		sendtoToppolyLCDreg(0x30, 0x08);   //300a
		sendtoToppolyLCDreg(0x70, 0x3b);   //0x36 
		sendtoToppolyLCDreg(0xc0, 0x05); 
        break;
    case LCD_MODE_RGBD:
    default:
        #if 1//#NTK#2011/07/14#Robin Chen add for TXD240C
        //---------Reset----------//
        sendtoToppolyLCDreg(0x00,0x0d);  
        TimerDelayMs(10);
        sendtoToppolyLCDreg(0x00,0x05);
        TimerDelayMs(10);
        sendtoToppolyLCDreg(0x00,0x05);
        TimerDelayMs(50);
        sendtoToppolyLCDreg(0x00,0x0d);
        TimerDelayMs(100);
        //---------Reset----------//
        sendtoToppolyLCDreg(0x60,0x01);//Select RGB Dummy Format input.
        sendtoToppolyLCDreg(0x40,0x03);//Vertical and Horzontal Direction Setting
        sendtoToppolyLCDreg(0xa0,0x08);//Vertical Data start delay selection
        sendtoToppolyLCDreg(0x50,0x68);//0x68 VCOMDC --1.8V
        sendtoToppolyLCDreg(0x70,0x3b);//0x40);      
        sendtoToppolyLCDreg(0x30,0x09);     
        sendtoToppolyLCDreg(0xe0,0x13);      
        #else
        //---software reset----// 
		sendtoToppolyLCDreg(0x00, 0x0F);  
		sendtoToppolyLCDreg(0x00, 0x05);//0x01
		TimerDelayMs(50);
		sendtoToppolyLCDreg(0x00, 0x0F);//
		sendtoToppolyLCDreg(0x00, 0x05);//
		TimerDelayMs(50);
		sendtoToppolyLCDreg(0x00, 0x0F);  //
		TimerDelayMs(100);
		//---software reset----// 
		sendtoToppolyLCDreg(0x50, 0x40);   //VCOM DC SETTING
		TimerDelayMs(10);
		sendtoToppolyLCDreg(0x60, 0x01);   //8 bit RGB Dummy
		TimerDelayMs(10);
		sendtoToppolyLCDreg(0x60, 0x01);   //8 bit RGB Dummy 
		//sendtoToppolyLCDreg(0x40, 0x03); // direction selection
		sendtoToppolyLCDreg(0x30, 0x08);   //300a
		sendtoToppolyLCDreg(0x70, 0x3b);   //0x36 
		sendtoToppolyLCDreg(0xc0, 0x05); 
        #endif
        break;
    }
}

/**
  Open ToppolyTD20 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenToppoly(LCDSIZE *psizeDisplay)
{
    UINT8 uiClockDiv;

    if(ide_open() != E_OK)
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
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }

    // Wait for power stable
    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay_Toppoly(0x10000);
    //#NT#2009/11/04#Klins Chen#[0007607] -end

    //Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGBD:
        //top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        //Set DCLK to 27MHz
        //uiDesClock = 27;
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        //wait Power Stable
        delay_Toppoly(0x10000);

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);
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
        ide_set_rgbdummy(1);
        ide_set_shw_st();

        //ide_set_hor_timing(0x00, 0x617, 0xFD, 0x5FD);
        //ide_set_ver_timing(0x01, 0x105, 0x0C, 0xFC, 0x0C, 0xFC);
        ide_set_hor_timing(0x01, 1716-1, 241, 1681);
        ide_set_ver_timing(0x01, 0x106, 0x15, 0x105, 0x15, 0x105);
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x01);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay -> uiWidth = Toppoly_RGBD_Buf_width;
        psizeDisplay -> uiHeight = Toppoly_RGBD_Buf_height;
        psizeDisplay -> uiWinWidth = Toppoly_RGBD_Win_width;
        psizeDisplay -> uiWinHeight = Toppoly_RGBD_Win_height;
        break;
    case LCD_MODE_RGB:
        //top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        //Set DCLK to 13.5MHz
        //uiDesClock = 13.5;
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK)/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay_Toppoly(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007607] -end

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
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

        ide_set_hor_timing(0x00, 0x333, 0x78, 0x2F7);
        ide_set_ver_timing(0x00, 0x105, 0x0E, 0x0FD, 0x0E, 0x0FD);
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


        psizeDisplay -> uiWidth = Toppoly_Buf_width;
        psizeDisplay -> uiHeight = Toppoly_Buf_height;
        psizeDisplay -> uiWinWidth = Toppoly_Win_width;
        psizeDisplay -> uiWinHeight = Toppoly_Win_height;
        break;

    case LCD_MODE_YUV:
        //top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        //Set DCLK to 27MHz
        //uiDesClock = 27;
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        //wait Power Stable
        delay_Toppoly(0x10000);

        ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_TOPPOLY);
        ide_set_pdir(0);
        ide_set_odd(1);
        ide_set_even(2);
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

        ide_set_hor_timing(0x00, 0x30B, 0x78, 0x2F7);
        ide_set_ver_timing(0x00, 0x105, 0x15, 0x104, 0x15, 0x104);
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


        psizeDisplay -> uiWidth = Toppoly_Buf_width;
        psizeDisplay -> uiHeight = Toppoly_Buf_height;
        psizeDisplay -> uiWinWidth = Toppoly_Win_width;
        psizeDisplay -> uiWinHeight = Toppoly_Win_height;
        break;

    default:

        //top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        //Set DCLK to 27MHz
        //uiDesClock = 27;
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        //wait Power Stable
        delay_Toppoly(0x10000);

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);
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
        ide_set_rgbdummy(1);
        ide_set_through_mode(0);
        ide_set_shw_st();
        //ide_set_en(1);
        //ide_set_load();

        //ide_set_hor_timing(0x00, 0x617, 0x125, 0x604);
        //ide_set_hor_timing(0x00, 0x617, 0x0F4, 0x5F3);
        //ide_set_ver_timing(0x00, 0x105, 0x15, 0x104, 0x15, 0x104);
        ide_set_hor_timing(0x00, 0x617, 0xFD, 0x5FD);
        ide_set_ver_timing(0x01, 0x105, 0x0C+1, 0xFC+1, 0x0C+1, 0xFC+1);
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

        psizeDisplay -> uiWidth = Toppoly_RGBD_Buf_width;
        psizeDisplay -> uiHeight = Toppoly_RGBD_Buf_height;
        psizeDisplay -> uiWinWidth = Toppoly_RGBD_Win_width;
        psizeDisplay -> uiWinHeight = Toppoly_RGBD_Win_height;
        break;

    }

    //#NT#2010/06/07#Niven Cho -begin
    //#NT#Adjust Gamma
    ide_setGammaEn(FALSE);//TRUE);
    ide_fillGamma(GammaTbl);
    //#NT#2010/06/07#Niven Cho -end

    ide_setCmults(0x42);
    //ide_setBrt(0xFFFFFFEE);


    ide_set_en(TRUE);
    // Avoid vsync active area
    ide_wait_frame_end();

    Init_Toppoly_LCD();
}

/**
  Close ToppolyTD20 driver

  Release ToppolyTD20 driver, private function for internal used

*/
static void CloseToppoly(void)
{
    UINT32 i;

    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Change to standby mode
    //sendtoToppolyLCDreg(0x04,Power_standby);
    //Must wait for 5 frame sync duration after entering standby mode
    for (i = 0; i < 5; i++)
        ide_wait_frame_end();

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

// add for compile issue
void LCD_Rotate(UINT32 bRotate)
{
    return;
    if (bRotate)
    {
        switch(LCD_DeviceObj.uiLCDMode)
        {
            case LCD_MODE_RGBD:
                sendtoToppolyLCDreg(0x05,0x13);
                break;
            case LCD_MODE_RGB:
                sendtoToppolyLCDreg(0x05,0x13);
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
                sendtoToppolyLCDreg(0x05,0x10);
                break;
            case LCD_MODE_RGB:
                ide_wait_frame_end();
                sendtoToppolyLCDreg(0x05,0x10);
                ide_set_pdir(IDE_PDIR_RBG);
                ide_set_even(IDE_LCD_B);
                ide_set_load();
                break;
        }

    }
}
//@}