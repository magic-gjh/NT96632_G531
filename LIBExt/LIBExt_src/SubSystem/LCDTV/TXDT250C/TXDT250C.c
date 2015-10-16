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
#include "Debug.h"

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

#define AU_DCN01_CCIR656_Buf_width     720
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

static void Init_TXDT270C_LCD(void);
static void OpenTXDT270C(LCDSIZE *psizeDisplay);
void CloseTXDT270C(void);

#define TXD250_WRITE_REG (0x0B)
//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

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
        //#NT#2007/03/07#Shirley Cheng -begin
        case LCD_MODE_YUV:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        //#NT#2007/03/07#Shirley Cheng -end

        }
    }
debug_err((">>> LCD Open Test\r\n"));
    OpenTXDT270C(psizeDisplay);
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
    sendtoAUDN01LCDreg(addr, value);
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

    if((addr >> 6)&0x01)
    addr = (addr|0x80)&TXD250_WRITE_REG;
	
   // debug_err(("TXD250 WHRITE REG[%x-->%x]\r\n",addr,value));
	
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
static void Init_TXDT270C_LCD(void)
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
debug_err((">>>>>Init TXD250 LCD mode %d\r\n",LCD_DeviceObj.uiLCDMode));
    switch (LCD_DeviceObj.uiLCDMode)
    {
        case LCD_MODE_RGB:
        //sendtoAUDN01LCDreg(0x00,0x03);//sendtoAUBL02LCDreg(0x00,0x0F);
        //sendtoAUDN01LCDreg(0x40,0x03);
        sendtoAUDN01LCDreg(0x04,0x0B);  //UPS052 360x240 --> 2B
        sendtoAUDN01LCDreg(0x05,0x5D);
        break;
    case LCD_MODE_RGBD:
sendtoAUDN01LCDreg(0x05, 0x5e);  // 
sendtoAUDN01LCDreg(0x05, 0x1e);  // reset
delay(100);
sendtoAUDN01LCDreg(0x05,0x5c);//
sendtoAUDN01LCDreg(0x2b,0x01);//sleep out
delay(100);
sendtoAUDN01LCDreg(0x2f, 0x44);//0x44);
sendtoAUDN01LCDreg(0x9e, 0x80);//0x8d
sendtoAUDN01LCDreg(0x00, 0x07);//
sendtoAUDN01LCDreg(0x01, 0xbb);//8d
sendtoAUDN01LCDreg(0xa1, 0x01);
sendtoAUDN01LCDreg(0x03,0x40);//0x48

//sendtoAUDN01LCDreg(0x04,0x1f);//rotate 180
sendtoAUDN01LCDreg(0x04,0x1c);//rotate 180

//-------------VCOMH ----------//
sendtoAUDN01LCDreg(0x80,0x01); // V63 //
sendtoAUDN01LCDreg(0x81,0x16); // V60 //
sendtoAUDN01LCDreg(0x82,0x1A); // V57 //
sendtoAUDN01LCDreg(0x83,0x0C); // V55 //
sendtoAUDN01LCDreg(0x84,0x0C); // V52 //
sendtoAUDN01LCDreg(0x85,0x07); // V48 //
sendtoAUDN01LCDreg(0x86,0x09); // V43 //
sendtoAUDN01LCDreg(0x87,0x01); // V38 //
sendtoAUDN01LCDreg(0x88,0x0A); // V25 //
sendtoAUDN01LCDreg(0x89,0x0F); // V20 //
sendtoAUDN01LCDreg(0x8A,0x15); // V15 //
sendtoAUDN01LCDreg(0x8B,0x10); // V11 //
sendtoAUDN01LCDreg(0x8C,0x0F); // V8 //
sendtoAUDN01LCDreg(0x8D,0x35); // V6 //
sendtoAUDN01LCDreg(0x8E,0x27); // V3 //
sendtoAUDN01LCDreg(0x8F,0x18); // V0 //
//-------------VCOML---------- //
sendtoAUDN01LCDreg(0x30,0x06); // V0 //
sendtoAUDN01LCDreg(0x31,0x14); // V3 //
sendtoAUDN01LCDreg(0x32,0x22); // V6 //
sendtoAUDN01LCDreg(0x33,0x10); // V8 //
sendtoAUDN01LCDreg(0x34,0x10); // V11 //
sendtoAUDN01LCDreg(0x35,0x17); // V15 //
sendtoAUDN01LCDreg(0x36,0x0E); // V20 //
sendtoAUDN01LCDreg(0x37,0x09); // V25 //
sendtoAUDN01LCDreg(0x38,0x01); // V38 //
sendtoAUDN01LCDreg(0x39,0x09); // V43 //
sendtoAUDN01LCDreg(0x3A,0x05); // V48 //
sendtoAUDN01LCDreg(0x3B,0x0C); // V52 //
sendtoAUDN01LCDreg(0x3C,0x0E); // V55//
sendtoAUDN01LCDreg(0x3D,0x2D); // V57 //
sendtoAUDN01LCDreg(0x3E,0x29); // V60 //
sendtoAUDN01LCDreg(0x3F,0x14); // V63 //
        break;
    case LCD_MODE_YUV:
        sendtoAUDN01LCDreg(0x00,0x43);//sendtoAUBL02LCDreg(0x00,0x0F);
        //sendtoAUDN01LCDreg(0x40,0x03);
        sendtoAUDN01LCDreg(0x04,0x4B);  //YUV 640x240
        sendtoAUDN01LCDreg(0x05,0x5D);
        break;

    //#NT#2007/03/07#Shirley Cheng -end
    }
}

/**
  Open AU027DN01 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenTXDT270C(LCDSIZE *psizeDisplay)
{
    UINT8   uiClockDiv;

    if (ide_open() != E_OK)
        return;

    // Disable IDE
    ide_wait_frame_end();
    ide_set_en(FALSE);

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
    delay(0x10000);
debug_err((">>>>>Open LCD mode %d\r\n",LCD_DeviceObj.uiLCDMode));
    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
//mark for compiling error,        pinmux_select_lcd(PINMUX_LCD_CASIONEW);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;

        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv-1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for Power Stable
        delay(0x10000);

        ide_set_icst(0);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
        ide_set_pdir(0);
        ide_set_odd(0);        // 0
        ide_set_even(1);    // 1
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

        //ide_set_hor_timing(0x00, 0x6B4, 0x49, 0x409);
        //ide_set_ver_timing(0x00, 0x106, 0x15, 0x106, 0x15, 0x106);
        ide_set_interlace(0x00);
        ide_set_startfield(0x00);

        //ide_set_ctrst(0x40);
        ide_set_ctrst(0x48);
        ide_set_brt(0x00);
        //ide_set_cmults(0x40);
        ide_set_cmults(0x58);
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
        ////delay(0x10000);

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);////
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
        delay(0x10000);

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
        ide_set_rgbdummy(0);
        ide_set_shw_st();
        //ide_set_en(1);
        //ide_set_load();
        /* //720x240
        ide_set_hor_timing(0x00, 0x359, 0x7D, 0x34D);
        ide_set_ver_timing(0x00, 0x105, 0x0D, 0xFE, 0x0D, 0xFE);  */
           //320x240
        ide_set_hor_timing(0x01, 1716-1, 241, 1440+241/*1681*/);
        ide_set_ver_timing(0x01, 0x106, 0x15, 0x105, 0x15, 0x105);

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

    ////delay(0x2000);

    ide_set_en(TRUE);
    //avoid vsync active area
    ////delay(0x2000);
    ide_waitFrameEnd();

    Init_TXDT270C_LCD();
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
void TXDT270COpenLcdBacklightReg(void)
{
	sendtoAUDN01LCDreg(0x05,0x5f);
}

//#NT#For LCD rotation
void LCD_Rotate(UINT32 bRotate)
{
    if (bRotate)
    {
        sendtoAUDN01LCDreg(0x04,0x1f);
        ide_wait_frame_end();
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(2);
        ide_set_load();
    }
    else
    {
        sendtoAUDN01LCDreg(0x04,0x1c);
        ide_wait_frame_end();
        ide_set_pdir(0);
        ide_set_odd(0);
        ide_set_even(2);
        ide_set_load();
    }
}

//@}
