/*
    @file       GPM1125A0.c

    @ingroup    mILIBLCDGPM1125A0

    @brief      AU LCD Driver of AU GPM1125A0 panel

    @note       Nothing.

    @author     Charles Lin

    @date       2010/06/02

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

*/

/**
@addtogroup mILIBLCDGPM1125A0

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0000).

@endverbatim

@{
*/
#include "OSCommon.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "pad.h"
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
#define INPUT_FORMAT_RAW                0x00
#define INPUT_FORMAT_SERIAL0            0x01
#define INPUT_FORMAT_SERIAL1            0x02
#define INPUT_FORMAT_CCIR656            0x07
#define INPUT_FORMAT_STD_NTSC           0x00
#define INPUT_FORMAT_STD_PAL            0x08
#define INPUT_VALID_DATA_280            0x00
#define INPUT_VALID_DATA_288            0x10

#define GPM1125A0_UPS051_Buf_width       480
#define GPM1125A0_UPS051_Buf_height      240
#define GPM1125A0_UPS051_Win_width       480
#define GPM1125A0_UPS051_Win_height      240

#define GPM1125A0_THROUGH_Buf_width      320
#define GPM1125A0_THROUGH_Buf_height     240
#define GPM1125A0_THROUGH_Win_width      320
#define GPM1125A0_THROUGH_Win_height     240

#define GPM1125A0_UPS052_Buf_width       320
#define GPM1125A0_UPS052_Buf_height      240
#define GPM1125A0_UPS052_Win_width       320
#define GPM1125A0_UPS052_Win_height      240

#define GPM1125A0_CCIR656_Buf_width      720
#define GPM1125A0_CCIR656_Buf_height     480
#define GPM1125A0_CCIR656_Win_width      720
#define GPM1125A0_CCIR656_Win_height     480

#define GPM1125A0_YUV640_Buf_width       640
#define GPM1125A0_YUV640_Buf_height      240
#define GPM1125A0_YUV640_Win_width       640
#define GPM1125A0_YUV640_Win_height      240

#define GPM1125A0_RGBD320_BUF_WIDTH      320
#define GPM1125A0_RGBD320_BUF_HEIGHT     240
#define GPM1125A0_RGBD320_WIN_WIDTH      320
#define GPM1125A0_RGBD320_WIN_HEIGHT     240

// Image rotate & flip
#define GPM1125A0_ROTATE_AND_FLIP        1

//function declaration
static void delay(UINT32 times);
static void sendtoGPM1125A0LCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoGPM1125A0LCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoGPM1125A0LCDreg(UINT8 addr, UINT8 value);

static void Init_GPM1125A0_LCD(void);
static void OpenGPM1125A0(LCDSIZE *psizeDisplay);
static void CloseGPM1125A0(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;


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
            LCD_DeviceObj.fIDEDstCLK = 24.54;
            break;
        case LCD_MODE_CCIR656:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        case LCD_MODE_YUV:
            LCD_DeviceObj.fIDEDstCLK = 24.535;
            break;
        case LCD_MODE_UPS051:
            LCD_DeviceObj.fIDEDstCLK = 9.7;
            break;
        case LCD_MODE_RGB_THROUGH:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        }
    }

    OpenGPM1125A0(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseGPM1125A0();
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
    sendtoGPM1125A0LCDreg(addr, value);
}


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
static void sendtoGPM1125A0LCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = ((((UINT8)addr << 12)|(UINT8)value ) << 16);
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/*
  Send data to specific address by GPIO

  @param addr specific address
  @param value value

*/
static void sendtoGPM1125A0LCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)addr << 12 | (UINT32)value;

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
static void sendtoGPM1125A0LCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoGPM1125A0LCDreg_sif(addr, value);
    }
    else
    {
         sendtoGPM1125A0LCDreg_gpio(addr, value);
    }
}
//#NT#2009/09/24#SA4 -begin
void sendtoGPM1125A0LCDreg2(UINT8 addr, UINT8 value)
{
    debug_err(("sendtoGPM1125A0LCDreg2,addr = 0x%x,value=0x%x\n\r",addr,value));
    sendtoGPM1125A0LCDreg(addr,value);
}
/*
void readGPM1125A0LCDreg_gpio(UINT8 addr, UINT8 *value)
{
    UINT32 data;
    UINT32 j;
    UINT32 temp;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (UINT32)((addr | 0x08) << 8) | (UINT32)(0);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    delay(500);

    for(j = 16; j > 8; j--)
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

    *value = 0;
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_INPUT);

    delay(200);
    for(j = 0; j < 8; j++)
    {
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        temp = gpio_getPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        *value |= temp << (7 - j);
        delay(200);
    }

    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}
*/
//#NT#2009/09/24#SA4 -end
/*
    Initial LCD of GPM1125A0, configure SIF intarface and send data to panel register to configure
*/
static void Init_GPM1125A0_LCD(void)
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
        sendtoGPM1125A0LCDreg(0x00,0x0D);
        delay(0x18000);   //1ms
        sendtoGPM1125A0LCDreg(0x00,0x05);   // Global reset
        delay(0x1C2C00);   //20ms
        sendtoGPM1125A0LCDreg(0x00,0x0D);
        sendtoGPM1125A0LCDreg(0x06,INPUT_FORMAT_SERIAL0|INPUT_FORMAT_STD_NTSC);   // Set SERIAL 320 mode
        #if (GPM1125A0_ROTATE_AND_FLIP)
        sendtoGPM1125A0LCDreg(0x04,0x00);   // Set image rotate and flip
        sendtoGPM1125A0LCDreg(0x08,0x11);   // Set H-data start delay (-2)
        #endif
        sendtoGPM1125A0LCDreg(0x0C,0x07);   // Set VCOM AC Level to 5.2V
        sendtoGPM1125A0LCDreg(0x01,0x64);   // GAMMAH
        sendtoGPM1125A0LCDreg(0x05,0x64);   // Set VCOM DC Level ON and 1.69V
        sendtoGPM1125A0LCDreg(0x09,0x50);
        sendtoGPM1125A0LCDreg(0x00,0x0F);   // Normal display with dc-dc on
        break;
    case LCD_MODE_CCIR656:
        sendtoGPM1125A0LCDreg(0x00,0x01);    // Global reset
        sendtoGPM1125A0LCDreg(0x00,0x09);    // Global reset recovery
        sendtoGPM1125A0LCDreg(0x06,0x01);    // Set UPS052 320 mode
        sendtoGPM1125A0LCDreg(0x00,0x0D);    // Release standby
        break;
    case LCD_MODE_YUV:
        sendtoGPM1125A0LCDreg(0x00,0x01);    // Global reset
        sendtoGPM1125A0LCDreg(0x00,0x09);    // Global reset recovery
        sendtoGPM1125A0LCDreg(0x06,0x01);    // Set UPS052 320 mode
        sendtoGPM1125A0LCDreg(0x00,0x0D);    // Release standby
        break;
    case LCD_MODE_UPS051:
        sendtoGPM1125A0LCDreg(0x00,0x0D);
        delay(0x18000);                     // Delay 1ms
        sendtoGPM1125A0LCDreg(0x00,0x05);   // Global reset
        delay(0x1C2C00);                    // Delay 20ms
        sendtoGPM1125A0LCDreg(0x00,0x0D);
        sendtoGPM1125A0LCDreg(0x06,INPUT_FORMAT_RAW|INPUT_FORMAT_STD_NTSC);   // Set SERIAL 320 mode
        sendtoGPM1125A0LCDreg(0x0C,0x07);   // Set VCOM AC Level to 5.2V
        sendtoGPM1125A0LCDreg(0x01,0x64);   // GAMMAH
        sendtoGPM1125A0LCDreg(0x05,0x64);   // Set VCOM DC Level ON and 1.69V
        sendtoGPM1125A0LCDreg(0x09,0x50);
        sendtoGPM1125A0LCDreg(0x00,0x0F);   // Normal display with dc-dc on
        break;
    case LCD_MODE_RGB_THROUGH:
        sendtoGPM1125A0LCDreg(0x00,0x01);    // Global reset
        sendtoGPM1125A0LCDreg(0x00,0x09);    // Global reset recovery
        sendtoGPM1125A0LCDreg(0x06,0x01);    // Set UPS052 320 mode
        sendtoGPM1125A0LCDreg(0x00,0x0D);    // Release standby
        break;
    default:
        debug_err(("Init_GPM1125A0_LCD() not support %d\r\n", LCD_DeviceObj.uiLCDMode));
        break;
    }
}
}
/*
  Open GPM1125A0 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenGPM1125A0(LCDSIZE *psizeDisplay)
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

        ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
        ide_set_pdir(0);
        #if (GPM1125A0_ROTATE_AND_FLIP)
        ide_set_odd(0);
        ide_set_even(1);
        #else
        ide_set_odd(2);
        ide_set_even(0);
        #endif
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(1);
        ide_set_through_mode(0);
        ide_set_shw_st();

        ide_set_hor_timing(0, 1559, 251, 1280+251-1);
        ide_set_ver_timing(0, 261, 12, 240+12-1, 12, 240+12-1);

        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        #if 0
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        #else
        ide_set_brt(0x15);
        ide_set_cmults(0x50);
        #endif
        ide_set_cex(0x00);
        ide_set_standby_signal(0x01);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth      = GPM1125A0_RGBD320_BUF_WIDTH;
        psizeDisplay->uiHeight     = GPM1125A0_RGBD320_BUF_HEIGHT;
        psizeDisplay->uiWinWidth   = GPM1125A0_RGBD320_WIN_WIDTH;
        psizeDisplay->uiWinHeight  = GPM1125A0_RGBD320_WIN_HEIGHT;
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

        ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_configOutputLimit(1);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CCIR656);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(0);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_through_mode(0);
        ide_set_shw_st();

        ide_set_hor_timing(0x00, 0x359, 0x88, 0x357);
        ide_set_ver_timing(0x05, 0x20C, 0x28, 0x207, 0x29, 0x208);
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

        psizeDisplay->uiWidth = GPM1125A0_CCIR656_Buf_width;
        psizeDisplay->uiHeight = GPM1125A0_CCIR656_Buf_height;
        psizeDisplay->uiWinWidth = GPM1125A0_CCIR656_Win_width;
        psizeDisplay->uiWinHeight = GPM1125A0_CCIR656_Win_height;
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

        ide_set_icst(0);
        ide_configIcst(FALSE, 0);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_TOPPOLY);
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(0);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_through_mode(0);
        ide_set_shw_st();

        ide_set_hor_timing(0x00, 0x30C, 0x78, 0x2F8);
        ide_set_ver_timing(0x01, 0x106, 0x15, 0x104, 0x15, 0x104);
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

        psizeDisplay->uiWidth       = GPM1125A0_YUV640_Buf_width;
        psizeDisplay->uiHeight      = GPM1125A0_YUV640_Buf_height;
        psizeDisplay->uiWinWidth    = GPM1125A0_YUV640_Win_width;
        psizeDisplay->uiWinHeight   = GPM1125A0_YUV640_Win_height;
        break;

    case LCD_MODE_UPS051:
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        ide_set_icst(0);
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
        ide_set_pdir(0);
        ide_set_odd(2);
        ide_set_even(0);
        ide_set_hsinv(1);
        ide_set_vsinv(1);
        ide_set_hvldinv(0);
        ide_set_vvldinv(0);
        ide_set_clkinv(1);
        ide_set_fieldinv(0);
        ide_set_rgbdummy(0);
        ide_set_through_mode(0);
        ide_set_shw_st();

        ide_set_hor_timing(0, (617-1), (100-1), (480+(100-1)-1));
        ide_set_ver_timing(0, (262-1), (13-1), (240+(13-1)-1), (13-1), (240+(13-1)-1));

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

        psizeDisplay->uiWidth       = GPM1125A0_UPS051_Buf_width;
        psizeDisplay->uiHeight      = GPM1125A0_UPS051_Buf_height;
        psizeDisplay->uiWinWidth    = GPM1125A0_UPS051_Win_width;
        psizeDisplay->uiWinHeight   = GPM1125A0_UPS051_Win_height;
        break;

    case LCD_MODE_RGB_THROUGH:
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

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
        ide_set_through_mode(1);
        ide_set_shw_st();

        ide_set_hor_timing(0x00, 0x6B3, 0x46, 0x405);
        ide_set_ver_timing(0x00, 0x106, 0x15, 0x104, 0x15, 0x104);

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

        psizeDisplay->uiWidth       = GPM1125A0_THROUGH_Buf_width;
        psizeDisplay->uiHeight      = GPM1125A0_THROUGH_Buf_height;
        psizeDisplay->uiWinWidth    = GPM1125A0_THROUGH_Win_width;
        psizeDisplay->uiWinHeight   = GPM1125A0_THROUGH_Win_height;
        break;

    default:
        debug_err(("OpenGPM1125A0() Not support %d mode\r\n",LCD_DeviceObj.uiLCDMode));
        break;
    }

    ide_set_en(TRUE);
    //avoid vsync active area
    ide_wait_frame_end();

    Init_GPM1125A0_LCD();
}

/*
  Close GPM1125A0 driver

  Release GPM1125A0 driver, private function for internal used
*/
static void CloseGPM1125A0(void)
{
    // Change to standby mode
    sendtoGPM1125A0LCDreg(0x00, STB_OFF);

    delay(0x80000);

    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

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
    id_type = bRotate & 0x80;

    if (bRotate)
    {
        ide_wait_frame_end();
        if(id_type != 0x80)//AUO
        {
            ide_set_odd(IDE_LCD_G);
            ide_set_even(IDE_LCD_R);
        }
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_load();
        sendtoGPM1125A0LCDreg2(0x04,0x08);
    }
    else
    {
        ide_wait_frame_end();
        ide_set_odd(IDE_LCD_R);
        ide_set_even(IDE_LCD_G);
        ide_set_pdir(IDE_PDIR_RGB);
        ide_set_load();
        sendtoGPM1125A0LCDreg2(0x04,0x0B);
    }
}
typedef enum
{
    GMP_LCDBRT_LVL_00 = 0,
    GMP_LCDBRT_LVL_01,
    GMP_LCDBRT_LVL_02,
    GMP_LCDBRT_LVL_03,
    GMP_LCDBRT_LVL_SETTING_MAX
} GMP_LCDBRT_LVL_SETTING;

void LCD_Brightness(UINT32 level)
{
    switch(level)
    {
    case GMP_LCDBRT_LVL_00:
        sendtoGPM1125A0LCDreg2(0x06,(0x15 | 0x60));
        break;
    case GMP_LCDBRT_LVL_01:
        sendtoGPM1125A0LCDreg2(0x06,(0x15 | 0x40));
        break;
    case GMP_LCDBRT_LVL_02:
        sendtoGPM1125A0LCDreg2(0x06,(0x15 | 0x00));
        break;
    case GMP_LCDBRT_LVL_03:
        sendtoGPM1125A0LCDreg2(0x06,(0x15 | 0x20));
        break;
    }
}

void LCD_BackLight(BOOL status)
{
    if(status == TRUE)
        sendtoGPM1125A0LCDreg(0x05,0x5F);
    else
        sendtoGPM1125A0LCDreg(0x05,0x5D);
}
//#NT#2009/08/11#Hideo Lin -end
//#NT#2009/09/24#SA4 -end
//@}
