/*
    @file       AUCN01.c

    @ingroup    mILIBLCDAUCN01

    @brief      AU LCD Driver of AUCN01 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDAUCN01

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
#include "Debug.h"
#include "Utility.h"

#define AU_CN01_UPS051_Buf_width       502  /*480 For Analog signal*/
#define AU_CN01_UPS051_Buf_height      240
#define AU_CN01_UPS051_Win_width       502  /*480 For Analog signal*/
#define AU_CN01_UPS051_Win_height      240

#define AU_CN01_UPS052_Buf_width       320
#define AU_CN01_UPS052_Buf_height      240
#define AU_CN01_UPS052_Win_width       320
#define AU_CN01_UPS052_Win_height      240

#define AU_CN01_CCIR656_Buf_width      720
#define AU_CN01_CCIR656_Buf_height     480
#define AU_CN01_CCIR656_Win_width      720
#define AU_CN01_CCIR656_Win_height     480

#define AU_CN01_YUV640_Buf_width       640
#define AU_CN01_YUV640_Buf_height      240
#define AU_CN01_YUV640_Win_width       640
#define AU_CN01_YUV640_Win_height      240

//UINT8 gAUCN01_VCOM_DC = 0x18;

//function declaration
static void delay(UINT32 times);
static void sendtoAUCN01LCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoAUCN01LCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoAUCN01LCDreg(UINT8 addr, UINT8 value);

static void Init_AUCN01_LCD(void);
static void OpenAUCN01(LCDSIZE *psizeDisplay);
static void CloseAUCN01(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

#if 1
/**
    Open LCD panel driver

    This function should be called before calling any other functions during initializing LCD driver.
    It would initilize the settings for specified type of LCD device and set it to specified operation mode.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

    @param[in,out]  lcddeviceobj    LCD device main object, refer the definition of LCD_DEVICE_OBJ.
    @param[in]      psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

    @return void
*/
void OpenPanel(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay)
{
    LCD_DeviceObj = *lcddeviceobj;

    if (LCD_DeviceObj.fIDEDstCLK == 0)
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case LCD_MODE_RGBD:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        case LCD_MODE_CCIR656:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        case LCD_MODE_YUV:
            LCD_DeviceObj.fIDEDstCLK = 24.55;
            break;

        case LCD_MODE_UPS051:
            LCD_DeviceObj.fIDEDstCLK = 9.7;
            break;
        }
    }

    OpenAUCN01(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseAUCN01();
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
    sendtoAUCN01LCDreg(addr, value);
}

#endif

/*
  delay time period

  @param times : time period

*/
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}

/*
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoAUCN01LCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = ((((UINT8)addr << 8)|(UINT8)value ) << 16);
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/*
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoAUCN01LCDreg_gpio(UINT8 addr, UINT8 value)
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

/*
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
static void sendtoAUCN01LCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoAUCN01LCDreg_sif(addr, value);
    }
    else
    {
         sendtoAUCN01LCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of AUCN01, configure SIF intarface and send data to panel register to configure

*/
static void Init_AUCN01_LCD(void)
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

    if(LCD_DeviceObj.fpPostAdjusement)
    {
        LCD_DeviceObj.fpPostAdjusement();
    }
    else
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
            case LCD_MODE_RGBD:
                sendtoAUCN01LCDreg(0x00,0x0F);//sendtoAUBL02LCDreg(0x00,0x0F);
                sendtoAUCN01LCDreg(0x40,0x03);
                sendtoAUCN01LCDreg(0x60,0x01);
                sendtoAUCN01LCDreg(0xC0,0x06);
                sendtoAUCN01LCDreg(0xE0,0x13);
                //   sendtoAUBL02LCDreg(0x03,0x40);
                //   sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
                //   sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
                //   sendtoAUBL02LCDreg(0x05,0x57);
                //   sendtoAUBL02LCDreg(0x05,0x57);

                //   sendtoAUBL02LCDreg(0x01,gAUBL02_VCOM_DC);
                //   sendtoAUBL02LCDreg(0x01,gAUBL02_VCOM_DC);
                break;
            case LCD_MODE_CCIR656:
                sendtoAUCN01LCDreg(0x00,0x0F);//sendtoAUBL02LCDreg(0x00,0x0F);
                sendtoAUCN01LCDreg(0x40,0x03);
                sendtoAUCN01LCDreg(0x60,0x07);
                sendtoAUCN01LCDreg(0xC0,0x06);
                //sendtoAUCN01LCDreg(0xE0,0x13);
                break;
            case LCD_MODE_YUV:
                sendtoAUCN01LCDreg(0x00,0x0F);
                sendtoAUCN01LCDreg(0x20,0x03);  //PDON(bit1) = 1, bit0:default 1
                sendtoAUCN01LCDreg(0x40,0x03);
                sendtoAUCN01LCDreg(0x60,0x03);
                sendtoAUCN01LCDreg(0xC0,0x06);
                break;
            case LCD_MODE_UPS051:
                sendtoAUCN01LCDreg(0x00,0x0F);
                sendtoAUCN01LCDreg(0x40,0x03);
                sendtoAUCN01LCDreg(0x60,0x00);
                sendtoAUCN01LCDreg(0xC0,0x06);
                break;
            default:
                debug_err(("Init_AUCN01_LCD() not support %d\r\n", LCD_DeviceObj.uiLCDMode));
                break;
        }
    }

}

/*
  Open AUCN01 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenAUCN01(LCDSIZE *psizeDisplay)
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

    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGBD:
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
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_AU);
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

        ide_set_hor_timing(0x00, 0x617, 0xFD, 0x5FD);
        ide_set_ver_timing(0x01, 0x105, 0x0C, 0xFC, 0x0C, 0xFC);
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

        psizeDisplay->uiWidth      = AU_CN01_UPS052_Buf_width;
        psizeDisplay->uiHeight     = AU_CN01_UPS052_Buf_height;
        psizeDisplay->uiWinWidth   = AU_CN01_UPS052_Win_width;
        psizeDisplay->uiWinHeight  = AU_CN01_UPS052_Win_height;
        break;

    case LCD_MODE_CCIR656:
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_CCIR656);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK;
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
        ide_configIcst(FALSE, 0);
        ide_configOutputLimit(1);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CCIR656);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(1);
        ide_set_hsinv(0);
        ide_set_vsinv(0);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_shw_st();

        /*ide_set_hor_timing(0x00, 0x6B3, 0x110, 0x6AF);
        ide_set_ver_timing(0x01, 0x105, 0x11, 0x100, 0x11, 0x100);
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);*/
        ide_set_hor_timing(0x00, 0x359, 0x88, 0x357);
        ide_set_ver_timing(0x05, 0x20C, 0x24, 0x203, 0x25, 0x204);
        ide_set_interlace(0x01);
        ide_set_startfield(0x01);


        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth = AU_CN01_CCIR656_Buf_width;
        psizeDisplay->uiHeight = AU_CN01_CCIR656_Buf_height;
        psizeDisplay->uiWinWidth = AU_CN01_CCIR656_Win_width;
        psizeDisplay->uiWinHeight = AU_CN01_CCIR656_Win_height;
        break;

    case LCD_MODE_YUV:
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK
        uiClockDiv = ((float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK);
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007607] -end

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

        ide_set_hor_timing(0x00, 0x30C, 0x7E, 0x2FE);
        ide_set_ver_timing(0x00, 0x105, 0x0D, 0x0FC, 0x0D, 0xFC);
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

        psizeDisplay->uiWidth       = AU_CN01_YUV640_Buf_width;
        psizeDisplay->uiHeight      = AU_CN01_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = AU_CN01_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = AU_CN01_YUV640_Win_height;
        break;
    case LCD_MODE_UPS051:
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));


        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007607] -end

        ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_AU);
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


        ide_set_hor_timing(0x00, 616-1, 100, (480+100) - 1);
        ide_set_ver_timing(0x05, 262, 18, (240+18)-1, 18, (240+18)-1);
        //ide_set_hor_timing(0x00, 0x359, 0x1E, 0x29D);
        //ide_set_ver_timing(0x00, 0x105, 0x11, 0x100, 0x11, 0x100);

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

        psizeDisplay->uiWidth       = AU_CN01_UPS051_Buf_width;
        psizeDisplay->uiHeight      = AU_CN01_UPS051_Buf_height;
        psizeDisplay->uiWinWidth    = AU_CN01_UPS051_Win_width;
        psizeDisplay->uiWinHeight   = AU_CN01_UPS051_Win_height;
        break;
    default:
        debug_err(("OpenAUCN01() Not support %d mode\r\n",LCD_DeviceObj.uiLCDMode));
        break;
    }


    ide_set_en(TRUE);
    //avoid vsync active area
    ide_wait_frame_end();

    Init_AUCN01_LCD();
}

/*
  Close AUCN01 driver

  Release AUCN01 driver, private function for internal used
*/
static void CloseAUCN01(void)
{
    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Change to standby mode
    //Fix change to standby mode register setting not match with spec.
    sendtoAUCN01LCDreg(0x00,0x08);

    delay(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

//#NT#2010/02/11#SA4 Chris Chung -begin
//#NT#add for compile issue
void LCD_Rotate(UINT32 bRotate)
{

}
//#NT#2010/02/11#SA4 Chris Chung -end
//@}
