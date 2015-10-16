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

#define TOPPOLY_025

#ifdef TOPPOLY_025
//address ==  0x02
#define Input_format_RGBD      0x00
#define Input_format_YUV       0x01
#define Input_format_Through   0x02
#define Input_format_std_NTSC  0x00
#define Input_format_std_PAL   0x04
#define Input_valid_data_1280  0x00
#define Input_valid_data_1440  0x08
#define Input_clk_edge_pos     0x00
#define Input_clk_edge_neg     0x10
#define Input_HD_pol_low       0x00
#define Input_HD_pol_high      0x20
#define Input_VD_pol_low       0x00
#define Input_VD_pol_high      0x40
//address ==  0x03
#define Interlace_mode         0x00
#define NonInterlace_mode      0x01
//address == 0x04
#define Power_standby          0x00
#define Power_normal           0x01
#define CP_CLK_disable         0x00
#define CP_CLK_enable          0x02
#define PWM_output_disable     0x00
#define PWM_output_enable      0x04
#define Pre_charge_disable     0x00
#define Pre_charge_enable      0x08
#define Output_driver_50       0x00
#define Output_driver_65       0x10
#define Output_driver_80       0x20
#define Output_driver_100      0x30
#endif




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
//#NT#2010/06/07#Niven Cho -begin
//#NT#Adjust Gamma
#if 0
static UINT8 GammaTbl[] = {
0, 17, 24, 35, 47, 61, 75, 88,106,123,
140,157,176,194,213,232,255
};
#else
static UINT8 GammaTbl[] = {
0, 13, 27, 41, 55, 69, 83, 98,114,130,
146,164,182,200,216,233,255
};
#endif
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
            LCD_DeviceObj.fIDEDstCLK = 13.5;
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

#ifdef TOPPOLY_025
    uiSIFData = (((UINT32)addr << 10)|(UINT32)value ) << 16;
#else
    uiSIFData = (((UINT32)addr << 8)|(UINT32)value ) << 16;
#endif

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
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

#ifdef TOPPOLY_025
    data = (UINT32)addr << 10 | (UINT32)value;
#else
    data = (UINT32)addr << 8 | (UINT32)value;
#endif

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
    case LCD_MODE_RGB:
        sendtoToppolyLCDreg(0x02,Input_format_Through|Input_format_std_NTSC|Input_valid_data_1280|Input_clk_edge_pos|Input_HD_pol_low|Input_VD_pol_low);
        break;
    case LCD_MODE_YUV:
        sendtoToppolyLCDreg(0x02,Input_format_YUV|Input_format_std_NTSC|Input_valid_data_1280|Input_clk_edge_pos|Input_HD_pol_low|Input_VD_pol_low);
        break;
    case LCD_MODE_RGBD:
    default:
        sendtoToppolyLCDreg(0x02,Input_format_RGBD|Input_format_std_NTSC|Input_valid_data_1280|Input_clk_edge_pos|Input_HD_pol_low|Input_VD_pol_low);
        break;
    }
    sendtoToppolyLCDreg(0x03,NonInterlace_mode);
    sendtoToppolyLCDreg(0x04,Power_normal|CP_CLK_enable|PWM_output_disable|Pre_charge_enable|Output_driver_50);

    //for gamma
    sendtoToppolyLCDreg(0x10, 0x36);
    sendtoToppolyLCDreg(0x11, 0x3F);
    sendtoToppolyLCDreg(0x14, 0x40);
    sendtoToppolyLCDreg(0x15, 0x55);
    sendtoToppolyLCDreg(0x16, 0x55);
    sendtoToppolyLCDreg(0x17, 0x35);
    sendtoToppolyLCDreg(0x18, 0x08);

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
        ide_configIcst(TRUE, CST_YCBCR2RGB);
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

    case LCD_MODE_RGBD:
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
        ide_configIcst(TRUE, CST_YCBCR2RGB);
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
        //ide_set_en(1);
        //ide_set_load();

        //ide_set_hor_timing(0x00, 0x617, 0x125, 0x604);
        ide_set_hor_timing(0x00, 0x617, 0x0F4, 0x5F3);
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

        psizeDisplay -> uiWidth = Toppoly_RGBD_Buf_width;
        psizeDisplay -> uiHeight = Toppoly_RGBD_Buf_height;
        psizeDisplay -> uiWinWidth = Toppoly_RGBD_Win_width;
        psizeDisplay -> uiWinHeight = Toppoly_RGBD_Win_height;
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

    // Change to standby mode
    sendtoToppolyLCDreg(0x04,Power_standby);
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

}
//@}
