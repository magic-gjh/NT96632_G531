/*
    @file       GiantPlusGPG48238QS5.c
    
    @ingroup    mILIBLCDGiantPlusGPG48238QS5

    @brief      AU LCD Driver of GiantPlusGPG48238QS5 panel 

    @note       Nothing.

    @author     

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.   

*/

/**
@addtogroup mILIBLCDGiantPlusGPG48238QS5
    
@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0100).  

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
// For the different version
// Address ==  0x00
#define GRB_OFF 0x00
#define GRB_ON 0x08
#define STB_OFF 0x00
#define STB_ON 0x04
#define SHDB_OFF 0x00
#define SHDB_ON 0x02
#define SHCB_OFF 0x00
#define SHCB_ON 0x01

// Address ==  0x01
#define GAMAH_LEVEL0 0x00
#define GAMAH_LEVEL1 0x20
#define GAMAH_LEVEL2 0x40
#define GAMAH_LEVEL3 0x60
#define GAMAH_LEVEL4 0x80
#define GAMAH_LEVEL5 0xA0
#define GAMAH_LEVEL6 0xC0
#define GAMAH_LEVEL7 0xE0

#define PTDY_DUTY0 0x00
#define PTDY_DUTY1 0x08
#define PTDY_DUTY2 0x10
#define PTDY_DUTY3 0x30

#define FBV_LEVEL0 0x00
#define FBV_LEVEL1 0x01
#define FBV_LEVEL2 0x02
#define FBV_LEVEL3 0x03
#define FBV_LEVEL4 0x04
#define FBV_LEVEL5 0x05
#define FBV_LEVEL6 0x06
#define FBV_LEVEL7 0x07

// Address ==  0x02
#define DITHB_8BIT 0x00
#define DITHB_6BIT 0x04
#define PFON_OFF 0x00
#define PFON_ON 0x20
// Address == 0x05
#define INPUT_FORMAT_RAW           0x00//through mode
#define INPUT_FORMAT_SERIAL0            0x01
#define INPUT_FORMAT_SERIAL1        0x02
#define INPUT_FORMAT_CCIR656        0x07
#define INPUT_FORMAT_STD_NTSC       0x00
#define INPUT_FORMAT_STD_PAL        0x08
#define INPUT_VALID_DATA_280       0x00
#define INPUT_VALID_DATA_288       0x10




// Window and buffer size
// Through mode
#define GiantPlus_BUF_WIDTH           480
#define GiantPlus_BUF_HEIGHT          240
#define GiantPlus_WIN_WIDTH           480
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
static void delay_GiantPlus(UINT32 times);
static void sendtoGiantPlusLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoGiantPlusLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoGiantPlusLCDreg8_gpio(UINT8 addr, UINT8 value);
static void sendtoGiantPlusLCDreg(UINT8 addr, UINT8 value);

static void Init_GiantPlus_LCD(void);
static void OpenGiantPlus(LCDSIZE *psizeDisplay);
static void CloseGiantPlus(void);

// Static variable
static LCD_DEVICE_OBJ   LCD_DeviceObj;

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
            LCD_DeviceObj.fIDEDstCLK = 9.70;
            break;

        //#NT#2007/04/13#Steven Wang -begin
        //#Add RGBD and YUV mode
        case LCD_MODE_YUV:
            LCD_DeviceObj.fIDEDstCLK = 27.0;
            break;
        //#NT#2007/04/13#Steven Wang -end

        //#NT#2008/10/20#Hideo Lin -begin
        //#Modify for RGB dummy 320 mode
        case LCD_MODE_RGBD:
            LCD_DeviceObj.fIDEDstCLK = 24.54;
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
    
}
/**
  delay time period

  @param times : time

*/
static void delay_GiantPlus(UINT32 times)
{
    for( ; times>0; times--)
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
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)addr << 12 | (UINT32)value;

    delay_GiantPlus(500);

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

        delay_GiantPlus(100);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_GiantPlus(200);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_GiantPlus(100);

        data <<= 1;
    }

    delay_GiantPlus(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}
static void sendtoGiantPlusLCDreg8_gpio(UINT8 addr, UINT8 value)
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

    delay_GiantPlus(500);

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

        delay_GiantPlus(100);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_GiantPlus(200);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_GiantPlus(100);

        data <<= 1;
    }

    delay_GiantPlus(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
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
    Initial LCD of GiantPlusTD24THEB5, configure SIF intarface and send data to panel register to configure 

*/
static void Init_GiantPlus_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
    #if 0
        sif_open();

        

        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv        = 7;
        //#NT#2007/05/07#Chris Hsu -begin
        //#Modify to conform to datasheet
        SIFCtrl.uiMode          = SIF_MODE_0100;
        //#NT#2007/05/07#Chris Hsu -end
        SIFCtrl.bSyncEn         = FALSE;
        SIFCtrl.uiLength        = 16;
        SIFCtrl.bMSB            = TRUE;
        SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration   = 0;
        SIFCtrl.uiSenH          = 0;

        // Init SIF
        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
#endif		
    }
	/*
     0x0005;      //Global reset,
    delay_5ms;
   
    MAINLCD_CMD(0x000D);  // PWM OFF ; (PWM ON=000F)
    MAINLCD_CMD(0xF546);
    MAINLCD_CMD(0x507C);  // Internal VcomDC Adjust,Register=50,Data=70 ~ 7F
    */

    sendtoGiantPlusLCDreg8_gpio(0x00,0x05);
    delay_GiantPlus(50000);
      sendtoGiantPlusLCDreg8_gpio(0x00,0x0d);
    delay_GiantPlus(50000);
	sendtoGiantPlusLCDreg8_gpio(0x00,0x05);
    delay_GiantPlus(50000);
	
    sendtoGiantPlusLCDreg8_gpio(0x00,0x0d);
    delay_GiantPlus(50000);
    sendtoGiantPlusLCDreg8_gpio(0xf5,0x46);
    sendtoGiantPlusLCDreg8_gpio(0x50,0x7c);
    //#NT#2009/09/26#Winston Li -end
    // Set LCD register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
        sendtoGiantPlusLCDreg(0x06, INPUT_FORMAT_RAW|INPUT_FORMAT_STD_NTSC);
        break;

    case LCD_MODE_YUV:
    //#NT#2007/04/13#Steven Wang -begin
    //#Add RGBD and YUV mode
        sendtoGiantPlusLCDreg(0x06, INPUT_FORMAT_SERIAL1|INPUT_FORMAT_STD_NTSC);
        break;

    case LCD_MODE_RGBD:
    default:
        //#NT#2008/10/20#Hideo Lin -begin
        //#Modify for RGB dummy 320 mode
        sendtoGiantPlusLCDreg(0x06, INPUT_FORMAT_SERIAL0|INPUT_FORMAT_STD_NTSC);
        //#NT#2008/10/20#Hideo Lin -end
        break;
    }
	//contrast
    //sendtoGiantPlusLCDreg(0x03, 0x0a);//0x08
	//brightness
    sendtoGiantPlusLCDreg(0x07, 0x42);//0x40
    // Change to standby mode
    //sendtoGiantPlusLCDreg(0x00, STB_ON);
    // Gamma
    //sendtoGiantPlusLCDreg(0x0a, 0x60);
    
    
    //#NT#2007/04/13#Steven Wang -end
}

/**
  Open GiantPlusTD24THEB5 panel driver, configure IDE related register, private function for internal used
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
   // delay_GiantPlus(0x10000);

    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
#if 0
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

        //ide_set_hor_timing(0, 616, 100, 580);
        //ide_set_ver_timing(0x00, 262, 18, 255, 17, 255);

        ide_set_hor_timing(0, 616, 100, 580);
        ide_set_ver_timing(0x00, 262, 12, 251, 11, 251);
      
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        //ide_set_cmults(0x40);
        ide_set_cmults(0x46);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = GiantPlus_BUF_WIDTH;
        psizeDisplay->uiHeight      = GiantPlus_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = GiantPlus_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = GiantPlus_WIN_HEIGHT;
#endif
        break;

    case LCD_MODE_YUV:
#if 0		
        //#NT#2007/04/13#Steven Wang -begin
        //#Add RGBD and YUV mode
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_TOPPOLY);

        // Set DCLK to 27MHz
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(IDE_CLKDIV, uiClockDiv - 1);

        // Enable IDE Clock
        pll_enableClock(IDE_CLK1);
        pll_enableClock(IDE_CLK2);

        // Wait for power stable
        delay_GiantPlus(0x10000);

        ide_set_icst(0);
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

        psizeDisplay->uiWidth       = GiantPlus_RGBD_BUF_WIDTH;
        psizeDisplay->uiHeight      = GiantPlus_RGBD_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = GiantPlus_RGBD_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = GiantPlus_RGBD_WIN_HEIGHT;
        break;
#endif
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

        // Wait for power stable
        //delay_GiantPlus(0x10000);

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

        //#NT#2008/10/20#Hideo Lin -begin
        //#Modify for RGB dummy 320 mode
        //ide_set_hor_timing(0x00, 0x617, 0x0F4, 0x5F3);
        ide_set_hor_timing(0x00, 0x617, 0xFD, 0x5FD);
        //ide_set_ver_timing(0x00, 0x105, 0x16, 0x105, 0x16, 0x105);
        ide_set_ver_timing(0x00, 0x105, 0x10, 0xFF, 0x10, 0xFF);
        //#NT#2008/10/20#Hideo Lin -end

		
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
    //delay_GiantPlus(0x2000);


    ide_set_en(TRUE);
    //avoid vsync active area
    ide_wait_frame_end();

    Init_GiantPlus_LCD();
}

/**
  Close GiantPlusTD24THEB5 driver

  Release GiantPlusTD24THEB5 driver, private function for internal used

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
        sendtoGiantPlusLCDreg(0x04,0x00);//rotate
    }
    else
    {
        ide_wait_frame_end();
        ide_set_odd(IDE_LCD_R);
        ide_set_even(IDE_LCD_G);
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_load();
        sendtoGiantPlusLCDreg(0x04,0x03);
    }
}

//@}
