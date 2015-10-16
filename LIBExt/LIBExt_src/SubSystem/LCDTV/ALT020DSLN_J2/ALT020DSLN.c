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

#define LCD_TYPE_ALT_200  1
#define LCD_TYPE_ILI9342_231_GROUP1   2
#define LCD_TYPE_ILI9342_231_GROUP2   3
#define LCD_TYPE_ILI9342_C_HS_231   4
#define LCD_TYPE_ILI9342_C_TM_231   5

#define LCD_TYPE_   LCD_TYPE_ALT_200

#define Toppoly_Buf_width              960
#define Toppoly_Buf_height             240
#define Toppoly_Win_width              960
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
            LCD_DeviceObj.fIDEDstCLK = 42.0;//27.0;
            break;
        case LCD_MODE_RGBD:
            LCD_DeviceObj.fIDEDstCLK = 27;
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
static void sendtoToppolyLCDreg_gpio_cmd(UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)value;

    delay_Toppoly(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    //COMMAND
    for(j = 9; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        delay_Toppoly(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(200);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(100);
    }

    delay_Toppoly(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}
/**
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg_gpio_data(UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)value;
    data |= 0x100;
    delay_Toppoly(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 9; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        delay_Toppoly(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(200);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(100);
    }

    delay_Toppoly(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
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
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

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
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay_Toppoly(200);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
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

    case LCD_MODE_RGBD:
    default:
#if 0//(LCD_TYPE_ == LCD_TYPE_ALT_200)///2.0' ALT 	
        //---software reset----//
        sendtoToppolyLCDreg_gpio_cmd(0x01);
        TimerDelayMs(10);

        sendtoToppolyLCDreg_gpio_cmd(0xB9);
        sendtoToppolyLCDreg_gpio_data(0xFF);
        sendtoToppolyLCDreg_gpio_data(0x93);
        sendtoToppolyLCDreg_gpio_data(0x42);
        sendtoToppolyLCDreg_gpio_cmd(0x21);
        sendtoToppolyLCDreg_gpio_cmd(0x36);
        sendtoToppolyLCDreg_gpio_data(0Xc8);//0xC8);     //08 Ðý×ª·½Ïò
        sendtoToppolyLCDreg_gpio_cmd(0x3A);
        sendtoToppolyLCDreg_gpio_data(0x55);     //16BIT PIX FOMART
//        sendtoToppolyLCDreg_gpio_data(0x66);     //18BIT PIX FOMART

        sendtoToppolyLCDreg_gpio_cmd(0xb0);
        sendtoToppolyLCDreg_gpio_data(0xe0);
        /*
        //------------------------------
        sendtoToppolyLCDreg_gpio_cmd(0x3a);
        sendtoToppolyLCDreg_gpio_data(0x66);
        sendtoToppolyLCDreg_gpio_cmd(0xb0);
        sendtoToppolyLCDreg_gpio_data(0x60);
        //------------------------------
        */

        sendtoToppolyLCDreg_gpio_cmd(0xF6);
        sendtoToppolyLCDreg_gpio_data(0x01);
        sendtoToppolyLCDreg_gpio_data(0x00);
        sendtoToppolyLCDreg_gpio_data(0x07);

        //sendtoToppolyLCDreg_gpio_cmd(0xb5);
        //sendtoToppolyLCDreg_gpio_data(0xe0);

        sendtoToppolyLCDreg_gpio_cmd(0xC0);
        sendtoToppolyLCDreg_gpio_data(0x28);
        sendtoToppolyLCDreg_gpio_data(0x0A);
        sendtoToppolyLCDreg_gpio_cmd(0xC1);
        sendtoToppolyLCDreg_gpio_data(0x02);
        sendtoToppolyLCDreg_gpio_cmd(0xC5);
        sendtoToppolyLCDreg_gpio_data(0x2F);
        sendtoToppolyLCDreg_gpio_data(0x3a);
        sendtoToppolyLCDreg_gpio_cmd(0xC7);
        sendtoToppolyLCDreg_gpio_data(0xC2);
        sendtoToppolyLCDreg_gpio_data(0xB8);
        sendtoToppolyLCDreg_gpio_data(0x0A);
        sendtoToppolyLCDreg_gpio_cmd(0xE0);
        sendtoToppolyLCDreg_gpio_data(0x0F);
        sendtoToppolyLCDreg_gpio_data(0x23);
        sendtoToppolyLCDreg_gpio_data(0x20);
        sendtoToppolyLCDreg_gpio_data(0x09);
        sendtoToppolyLCDreg_gpio_data(0x0F);
        sendtoToppolyLCDreg_gpio_data(0x07);
        sendtoToppolyLCDreg_gpio_data(0x50);
        sendtoToppolyLCDreg_gpio_data(0x76);
        sendtoToppolyLCDreg_gpio_data(0x40);
        sendtoToppolyLCDreg_gpio_data(0x0A);
        sendtoToppolyLCDreg_gpio_data(0x15);
        sendtoToppolyLCDreg_gpio_data(0x06);
        sendtoToppolyLCDreg_gpio_data(0x11);
        sendtoToppolyLCDreg_gpio_data(0x0E);
        sendtoToppolyLCDreg_gpio_data(0x08);
        sendtoToppolyLCDreg_gpio_cmd(0xE1);
        sendtoToppolyLCDreg_gpio_data(0x08);
        sendtoToppolyLCDreg_gpio_data(0x1F);
        sendtoToppolyLCDreg_gpio_data(0x21);
        sendtoToppolyLCDreg_gpio_data(0x06);
        sendtoToppolyLCDreg_gpio_data(0x0F);
        sendtoToppolyLCDreg_gpio_data(0x03);
        sendtoToppolyLCDreg_gpio_data(0x32);
        sendtoToppolyLCDreg_gpio_data(0x23);
        sendtoToppolyLCDreg_gpio_data(0x41);
        sendtoToppolyLCDreg_gpio_data(0x01);
        sendtoToppolyLCDreg_gpio_data(0x08);
        sendtoToppolyLCDreg_gpio_data(0x08);
        sendtoToppolyLCDreg_gpio_data(0x30);
        sendtoToppolyLCDreg_gpio_data(0x33);
        sendtoToppolyLCDreg_gpio_data(0x0F);

        sendtoToppolyLCDreg_gpio_cmd(0x11); //Exit Sleep
        TimerDelayMs(10);

        sendtoToppolyLCDreg_gpio_cmd(0x11); //Exit Sleep
        TimerDelayMs(80);
        sendtoToppolyLCDreg_gpio_cmd(0x29); //Display ON

        sendtoToppolyLCDreg_gpio_cmd(0x2c); //Display ON
#else
sendtoToppolyLCDreg_gpio_cmd(0x01);     //software reset  
TimerDelayMs(20);

sendtoToppolyLCDreg_gpio_cmd(0xC8);   
sendtoToppolyLCDreg_gpio_data(0xFF); 
sendtoToppolyLCDreg_gpio_data(0x93); 
sendtoToppolyLCDreg_gpio_data(0x42); 
 
 
 
sendtoToppolyLCDreg_gpio_cmd(0x36);   
sendtoToppolyLCDreg_gpio_data(0xC8); 
 
sendtoToppolyLCDreg_gpio_cmd(0xC0);   
sendtoToppolyLCDreg_gpio_data(0x09);//(0xFF);   
sendtoToppolyLCDreg_gpio_data(0x09);//(0xFF);   
sendtoToppolyLCDreg_gpio_cmd(0xC1);   
sendtoToppolyLCDreg_gpio_data(0x01);//(0x01); 

sendtoToppolyLCDreg_gpio_cmd(0xC5);   
sendtoToppolyLCDreg_gpio_data(0xe7);//(0xF7);  

 
sendtoToppolyLCDreg_gpio_cmd(0xC7);   
//sendtoToppolyLCDreg_gpio_data(0Xc6); 
sendtoToppolyLCDreg_gpio_data(0x00); 
sendtoToppolyLCDreg_gpio_data(0x02); 

sendtoToppolyLCDreg_gpio_cmd(0xB4);   
sendtoToppolyLCDreg_gpio_data(0x02);

 
sendtoToppolyLCDreg_gpio_cmd(0xB8);   
sendtoToppolyLCDreg_gpio_data(0x00);   
 
sendtoToppolyLCDreg_gpio_cmd(0xE0); //Set Gamma 
sendtoToppolyLCDreg_gpio_data (0x00); 
sendtoToppolyLCDreg_gpio_data (0x05); 
sendtoToppolyLCDreg_gpio_data (0x08); 
sendtoToppolyLCDreg_gpio_data (0x04); 
sendtoToppolyLCDreg_gpio_data (0x13); 
sendtoToppolyLCDreg_gpio_data (0x0a); 
sendtoToppolyLCDreg_gpio_data (0x34); 
sendtoToppolyLCDreg_gpio_data (0x8a); 
sendtoToppolyLCDreg_gpio_data (0x46); 
sendtoToppolyLCDreg_gpio_data (0x07); 
sendtoToppolyLCDreg_gpio_data (0x0e); 
sendtoToppolyLCDreg_gpio_data (0x0a); 
sendtoToppolyLCDreg_gpio_data (0x1b); 
sendtoToppolyLCDreg_gpio_data (0x1d); 
sendtoToppolyLCDreg_gpio_data (0x0F); 

sendtoToppolyLCDreg_gpio_cmd(0XE1); //Set Gamma 
sendtoToppolyLCDreg_gpio_data (0x00); 
sendtoToppolyLCDreg_gpio_data (0x25); 
sendtoToppolyLCDreg_gpio_data (0x27); 
sendtoToppolyLCDreg_gpio_data (0x03); 
sendtoToppolyLCDreg_gpio_data (0x0F); 
sendtoToppolyLCDreg_gpio_data (0x06); 
sendtoToppolyLCDreg_gpio_data (0x3C); 
sendtoToppolyLCDreg_gpio_data (0x37); 
sendtoToppolyLCDreg_gpio_data (0x4D); 
sendtoToppolyLCDreg_gpio_data (0x05); 
sendtoToppolyLCDreg_gpio_data (0x0E); 
sendtoToppolyLCDreg_gpio_data (0x0B); 
sendtoToppolyLCDreg_gpio_data (0x31); 
sendtoToppolyLCDreg_gpio_data (0x34); 
sendtoToppolyLCDreg_gpio_data (0x0F);  

  
sendtoToppolyLCDreg_gpio_cmd(0xB0);   
sendtoToppolyLCDreg_gpio_data(0xe0);

sendtoToppolyLCDreg_gpio_cmd(0x3A);   
sendtoToppolyLCDreg_gpio_data(0x66);
sendtoToppolyLCDreg_gpio_cmd(0xf6);   
sendtoToppolyLCDreg_gpio_data(0x01);
sendtoToppolyLCDreg_gpio_data(0x00);
sendtoToppolyLCDreg_gpio_data(0x07);	

sendtoToppolyLCDreg_gpio_cmd(0x11);      //Exit Sleep 
TimerDelayMs(10); 
sendtoToppolyLCDreg_gpio_cmd(0x11);      //Exit Sleep 
 
TimerDelayMs(80);

 
sendtoToppolyLCDreg_gpio_cmd(0x29);     //Display ON 
TimerDelayMs(180); 

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
    if ((LCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1))
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
    case LCD_MODE_RGB:
        //top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        //Set DCLK to 13.5MHz
        //uiDesClock = 13.5;
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK);///(float)2;
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
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_odd(IDE_LCD_R);
        ide_set_even(IDE_LCD_R);
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

        //ide_set_hor_timing(0x00, 0x333, 0x78, 0x2F7);
        //ide_set_ver_timing(0x00, 0x105, 0x0E, 0x0FD, 0x0E, 0x0FD);
        //ide_set_hor_timing(0x00, 0x6B4, 0x49-10, 0x409-10);
        //ide_set_ver_timing(0x00, 0x106, 0x15-10, 0x106-10, 0x15-10, 0x106-10);
			        
        ide_set_hor_timing(0x9, 990, 20, 979);
        ide_set_ver_timing(1, 246, 2, 241, 2, 241);
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
        break;

    }

    //#NT#2010/06/07#Niven Cho -begin
    //#NT#Adjust Gamma
    ide_setGammaEn(TRUE);
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

sendtoToppolyLCDreg_gpio_cmd(0xC0);   
sendtoToppolyLCDreg_gpio_data(0x00);
sendtoToppolyLCDreg_gpio_data(0x00);	

sendtoToppolyLCDreg_gpio_cmd(0x10);      // Sleep 
//TimerDelayMs(10); 	
sendtoToppolyLCDreg_gpio_cmd(0x28);     //Display OFF
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
    if(bRotate)
    	{
		sendtoToppolyLCDreg_gpio_cmd(0x36);   
		sendtoToppolyLCDreg_gpio_data(0x08); 
	}
	else
	{
		sendtoToppolyLCDreg_gpio_cmd(0x36);   
		sendtoToppolyLCDreg_gpio_data(0xC8); 
	}
}
//@}
