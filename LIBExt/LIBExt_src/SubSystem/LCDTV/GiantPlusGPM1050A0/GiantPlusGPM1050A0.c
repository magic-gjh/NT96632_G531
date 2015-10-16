/*
    @file       AUBL02.c

    @ingroup    mILIBLCDAUBL02

    @brief      AU LCD Driver of AUBL02 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDAUBL02

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Using SIF mode 1000).

@endverbatim

@{
*/

#include <stdio.h>
#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Utility.h"

#define AU_BL02_UPS051_Buf_width       504
#define AU_BL02_UPS051_Buf_height      240
#define AU_BL02_UPS051_Win_width       504
#define AU_BL02_UPS051_Win_height      240

// Address == 0x04
#define INPUT_FORMAT_RAW           0x0B//through mode,default
#define INPUT_FORMAT_UPS052_320            0x1B
#define INPUT_FORMAT_UPS052_360        0x2B
//#NT#2009/09/24#SA4 -begin
#define INPUT_FORMAT_UPS052_320_ROTATE            0x18
//#NT#2009/09/24#SA4 -end

// Window and buffer size
// Through mode
#define GiantPlus_BUF_WIDTH           960
#define GiantPlus_BUF_HEIGHT          240
#define GiantPlus_WIN_WIDTH           960
#define GiantPlus_WIN_HEIGHT          240

//#NT#2007/04/16#Chris Hsu -begin
//#Add version and definitions for THEB5
// RGB dummy, YUV mode
//#define GiantPlus_RGBD_BUF_WIDTH      640
#define GiantPlus_RGBD_BUF_WIDTH      320
#define GiantPlus_RGBD_BUF_HEIGHT     234
//#define GiantPlus_RGBD_WIN_WIDTH      640
#define GiantPlus_RGBD_WIN_WIDTH      320
#define GiantPlus_RGBD_WIN_HEIGHT     234
//#NT#2007/04/16#Chris Hsu -end

//#NT#2008/10/20#Hideo Lin -begin
//#Modify for RGB dummy 320 mode
#define GiantPlus_RGBD320_BUF_WIDTH     320
#define GiantPlus_RGBD320_BUF_HEIGHT    240
#define GiantPlus_RGBD320_WIN_WIDTH     320
#define GiantPlus_RGBD320_WIN_HEIGHT    240
//#NT#2008/10/20#Hideo Lin -end

// Function declaration
static void delay(UINT32 times);
static void sendtoGiantPlusLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoGiantPlusLCDreg_gpio(UINT8 addr, UINT8 value);
void GetFromGiantPlusLCDreg_gpio(UINT8 addr);
static void sendtoGiantPlusLCDreg(UINT8 addr, UINT8 value);

static void Init_GiantPlus_LCD(void);
static void OpenGiantPlus(LCDSIZE *psizeDisplay);
static void CloseGiantPlus(void);

// Static variable
static LCD_DEVICE_OBJ   LCD_DeviceObj;

#if 1
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
            LCD_DeviceObj.fIDEDstCLK = 10.15;
            break;
        //#NT#2008/10/20#Hideo Lin -begin
        //#Modify for RGB dummy 320 mode
        case LCD_MODE_RGBD:
            LCD_DeviceObj.fIDEDstCLK = 24.535;
            break;
        //#NT#2008/10/20#Hideo Lin -end
        }
    }

    OpenGiantPlus(psizeDisplay);
}

/**
  Close LCD driver, public function for global use

  Release LCD driver

*/
void ClosePanel(void)
{
    CloseGiantPlus();
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
    sendtoGiantPlusLCDreg(addr, value);
}

#endif
/**
  delay time period

  @param times : time

*/
void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/**
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoGiantPlusLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = (((UINT32)addr << 12)|(UINT32)value ) << 16;
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/**
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoGiantPlusLCDreg_gpio(UINT8 addr, UINT8 value)
#if 0
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
#else
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

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j=16; j>0; j--)
    {
        if (data & 0x8000)
        {
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        }
        else
        {
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        }

        delay(100);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay(200);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay(100);

        data <<= 1;
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}
#endif

void GetFromGiantPlusLCDreg_gpio(UINT8 addr)
{
    UINT8 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
	
    addr = addr | 0x40;//set bit 6

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 8; j > 0; j--)
    {
        if(((addr  >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(200);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
    }
	
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_INPUT);	
    for(j = 8; j > 0; j--)
    {
       

        delay(100);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(200);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
	
        if(gpio_getPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data) == 1)
		data = (data | 0x01) << (j -1);	
     
        delay(100);
    }


    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    //debug_err(("addr = %x,data = %x\n\r",addr,data));
}

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendtoGiantPlusLCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
         sendtoGiantPlusLCDreg_sif(addr, value);
    }
    else
    {
         sendtoGiantPlusLCDreg_gpio(addr, value);
    }
}

/**
    Initial LCD of AUBL02, configure SIF intarface and send data to panel register to configure

*/
static void Init_GiantPlus_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = SIF_MODE_0000;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        //SIFCtrl.bSyncEn       = FALSE;
        //SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;

        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    // Set LCD register
    sendtoGiantPlusLCDreg(0x05,0x1E);//MAINLCD_CMD(0x051E);//
    delay(200);////Delay_us(1000);
    switch(LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
        sendtoGiantPlusLCDreg(0x04,INPUT_FORMAT_RAW);//MAINLCD_CMD(0x051E);//	
        break;

    case LCD_MODE_YUV:
        sendtoGiantPlusLCDreg(0x04,INPUT_FORMAT_UPS052_360);//MAINLCD_CMD(0x051E);//	
        break;

        case LCD_MODE_RGBD:       //052 mode
        default:
        	sendtoGiantPlusLCDreg(0x04,INPUT_FORMAT_UPS052_320);//MAINLCD_CMD(0x051E);//	
        break;
    }
        sendtoGiantPlusLCDreg(0x05,0x5f);////MAINLCD_CMD(0x055f);//
		sendtoGiantPlusLCDreg(0x95,0x01);////MAINLCD_CMD(0x9501);//	
		sendtoGiantPlusLCDreg(0x01,0x82);//MAINLCD_CMD(0x0182);//Vcom DC
		sendtoGiantPlusLCDreg(0x00,0x03);//MAINLCD_CMD(0x0003);//Vcom AC

		//sendtoGiantPlusLCDreg(0x06,0x03);//MAINLCD_CMD(0x0603);//
		//sendtoGiantPlusLCDreg(0x07,0x31);//MAINLCD_CMD(0x0731);//
		
		sendtoGiantPlusLCDreg(0xbd,0x51);//MAINLCD_CMD(0xbd51);//
		sendtoGiantPlusLCDreg(0xbe,0x84);//MAINLCD_CMD(0xbe84);//
		sendtoGiantPlusLCDreg(0xba,0x01);//MAINLCD_CMD(0xba01);//
		sendtoGiantPlusLCDreg(0xbb,0x40);//MAINLCD_CMD(0xbb40);//
		delay(200);//Delay_us(1000);
	
		delay(200);//Delay_us(10000);
		delay(200);//Delay_us(10000);
		sendtoGiantPlusLCDreg(0xa1,0x03);//MAINLCD_CMD(0xa103);//
		sendtoGiantPlusLCDreg(0xa4,0x00);//MAINLCD_CMD(0xa400);//
		sendtoGiantPlusLCDreg(0xa5,0xa8);//MAINLCD_CMD(0xa5a8);//

		//gamma 
		sendtoGiantPlusLCDreg(0x30,0x00);//MAINLCD_CMD(0x3000);//v0
		sendtoGiantPlusLCDreg(0x31,0x0F);//MAINLCD_CMD(0x310F);//v3
		sendtoGiantPlusLCDreg(0x32,0x1F);//MAINLCD_CMD(0x321F);//v6
		sendtoGiantPlusLCDreg(0x33,0x0D);//MAINLCD_CMD(0x330D);//v8
		sendtoGiantPlusLCDreg(0x34,0x0C);//MAINLCD_CMD(0x340C);//v11
		sendtoGiantPlusLCDreg(0x35,0x17);//MAINLCD_CMD(0x3517);//v15
		sendtoGiantPlusLCDreg(0x36,0x0F);//MAINLCD_CMD(0x360F);//v20
		sendtoGiantPlusLCDreg(0x37,0x0C);//MAINLCD_CMD(0x370C);//v25
		sendtoGiantPlusLCDreg(0x38,0x00);//MAINLCD_CMD(0x3800);//v38
		sendtoGiantPlusLCDreg(0x39,0x08);//MAINLCD_CMD(0x3908);//v43
		sendtoGiantPlusLCDreg(0x3a,0x04);//MAINLCD_CMD(0x3a04);//v48
		sendtoGiantPlusLCDreg(0x3b,0x09);//MAINLCD_CMD(0x3b09);//v52
		sendtoGiantPlusLCDreg(0x3c,0x0f);//MAINLCD_CMD(0x3c0f);//v55
		sendtoGiantPlusLCDreg(0x3d,0x26);//MAINLCD_CMD(0x3d26);//v57
		sendtoGiantPlusLCDreg(0x3e,0x20);//MAINLCD_CMD(0x3e20);//v60
		sendtoGiantPlusLCDreg(0x3f,0x00);//MAINLCD_CMD(0x3f00);//v63
		
		sendtoGiantPlusLCDreg(0x8f,0x00);//MAINLCD_CMD(0x8f00);//v0
		sendtoGiantPlusLCDreg(0x8e,0x07);//MAINLCD_CMD(0x8e07);//v3
		sendtoGiantPlusLCDreg(0x8d,0x18);//MAINLCD_CMD(0x8d18);//v6
		sendtoGiantPlusLCDreg(0x8c,0x10);//MAINLCD_CMD(0x8c10);//v8
		sendtoGiantPlusLCDreg(0x8b,0x10);//MAINLCD_CMD(0x8b10);//v11
		sendtoGiantPlusLCDreg(0x8a,0x16);//MAINLCD_CMD(0x8a16);//v15
		sendtoGiantPlusLCDreg(0x89,0x0B);//MAINLCD_CMD(0x890B);//v20
		sendtoGiantPlusLCDreg(0x88,0x07);//MAINLCD_CMD(0x8807);//v25
		sendtoGiantPlusLCDreg(0x87,0x02);//MAINLCD_CMD(0x8702);//v38
		sendtoGiantPlusLCDreg(0x86,0x09);//MAINLCD_CMD(0x8609);//v43
		sendtoGiantPlusLCDreg(0x85,0x09);//MAINLCD_CMD(0x8509);//v48
		sendtoGiantPlusLCDreg(0x84,0x0d);//MAINLCD_CMD(0x840d);//v52
		sendtoGiantPlusLCDreg(0x83,0x0f);//MAINLCD_CMD(0x830f);//v55
		sendtoGiantPlusLCDreg(0x82,0x28);//MAINLCD_CMD(0x8228);//v57
		sendtoGiantPlusLCDreg(0x81,0x21);//MAINLCD_CMD(0x8121);//v60
		sendtoGiantPlusLCDreg(0x80,0x00);//MAINLCD_CMD(0x8000);//v63

		//------------------
		//sendtoGiantPlusLCDreg(0x04,0x0F);//MAINLCD_CMD(0x040F);  //RGB  8BIT 
        sendtoGiantPlusLCDreg(0x97,0x02);//MAINLCD_CMD(0x9702);  //VGL		
        sendtoGiantPlusLCDreg(0x96,0x03);//MAINLCD_CMD(0x9603);  //VGH
    // Change to standby mode
    //sendtoGiantPlusLCDreg(0x00, STB_ON);
    // Gamma
    //sendtoGiantPlusLCDreg(0x0a, 0x60);
    
}

/**
  Open AUBL02 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenGiantPlus(LCDSIZE *psizeDisplay)
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
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }

    // wait for Power Stable
    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay(0x10000);
    //#NT#2009/11/04#Klins Chen#[0007607] -end

    switch(LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:       //051 mode

        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007607] -end

        ide_set_icst(0);
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


        ide_set_hor_timing(0, 1029, 49, 1024);
        ide_set_ver_timing(0, 243, 1, 240, 2, 242);
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


        psizeDisplay->uiWidth       = GiantPlus_BUF_WIDTH;
        psizeDisplay->uiHeight      = GiantPlus_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = GiantPlus_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = GiantPlus_WIN_HEIGHT;
        break;

    case LCD_MODE_RGBD:
    default:

        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for Power Stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007607] -end

        ide_set_icst(0);
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

        //ide_set_hor_timing(0, 1306, 2, 1304);
        ide_set_hor_timing(1, 1559, 241, 1280+241-1);
        //ide_set_ver_timing(0, 243, 1, 241, 2, 242);
        ide_set_ver_timing(1, 261, 21, 240+21-1, 21, 240+21-1);
     
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

        //#NT#2008/10/20#Hideo Lin -begin
        //#Modify for RGB dummy 320 mode
        psizeDisplay->uiWidth       = GiantPlus_RGBD320_BUF_WIDTH;
        psizeDisplay->uiHeight      = GiantPlus_RGBD320_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = GiantPlus_RGBD320_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = GiantPlus_RGBD320_WIN_HEIGHT;
        //#NT#2008/10/20#Hideo Lin -end
        //#NT#2007/04/13#Steven Wang -end
        break;
    }



    ide_set_en(TRUE);
    //avoid vsync active area
    ide_wait_frame_end();

    Init_GiantPlus_LCD();
}

/**
  Close AUBL02 driver

  Release AUBL02 driver, private function for internal used

*/
static void CloseGiantPlus(void)
{
    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Change to standby mode
    //sendtoGiantPlusLCDreg(0x00, STB_OFF);



    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

//#NT#2009/09/24#SA4 -begin
//#NT#2009/08/11#Hideo Lin -begin
//#NT#For LCD rotation
void LCD_Rotate(UINT32 bRotate)
{
    UINT32 id_type;


    if (bRotate)
    {
        ide_wait_frame_end();
         ide_set_odd(IDE_LCD_R);
         ide_set_even(IDE_LCD_B);
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_load();
        sendtoGiantPlusLCDreg(0x04,INPUT_FORMAT_UPS052_320_ROTATE);
    }
    else
    {
        ide_wait_frame_end();
        ide_set_odd(IDE_LCD_R);
        ide_set_even(IDE_LCD_G);
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_load();
        sendtoGiantPlusLCDreg(0x04,INPUT_FORMAT_UPS052_320);
    }
}

//@}
