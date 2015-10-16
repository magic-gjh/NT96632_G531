/*
   @file       TM027CDH01.c
   @brief      LCD Driver of TM027CDH01 panel, TCON:MICRON
   @author     Jacky Huang
   @date       2000/04/13
   Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
   */
#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Debug.h"

#define TM027CDH01_RGB_BUF_WIDTH            960
#define TM027CDH01_RGB_BUF_HEIGHT           240
#define TM027CDH01_RGB_WIN_WIDTH            960
#define TM027CDH01_RGB_WIN_HEIGHT           240

#define TM027CDH01_RGBD_BUF_WIDTH           320
#define TM027CDH01_RGBD_BUF_HEIGHT          240
#define TM027CDH01_RGBD_WIN_WIDTH           320
#define TM027CDH01_RGBD_WIN_HEIGHT          240

#define TM027CDH01_RGBTHROUGH_BUF_WIDTH     320
#define TM027CDH01_RGBTHROUGH_BUF_HEIGHT    240
#define TM027CDH01_RGBTHROUGH_WIN_WIDTH     320
#define TM027CDH01_RGBTHROUGH_WIN_HEIGHT    240

#define TM027CDH01_YUV640_BUF_WIDTH         640
#define TM027CDH01_YUV640_BUF_HEIGHT        240
#define TM027CDH01_YUV640_WIN_WIDTH         640
#define TM027CDH01_YUV640_WIN_HEIGHT        240


#define USED_SIF_MODE                       SIF_MODE_0000

#define PanelFormCmdDataL16(A, V)   ((((A) & 0x40) << 15) | (0x00 << 14) | (((A) & 0x3F) << 8) | ((V) & 0xFF))
#define PanelFormCmdDataH16(A, V)   ((PanelFormCmdDataL16(A, V)) << 16)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Step 1: End
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//function declaration
void        sendtoLCDreg_sif(UINT32 addr, UINT32 value);
void        sendtoLCDreg_gpio(UINT32 addr, UINT32 value);
void        sendtoLCDreg(UINT32 addr, UINT32 value);
static void delay_TM027CDH01(UINT32 times);
static void Init_TM027CDH01_LCD(void);
static void OpenTM027CDH01(LCDSIZE *psizeDisplay);
static void CloseTM027CDH01(void);

//static variable
static LCD_DEVICE_OBJ   LCD_DeviceObj;



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
        LCD_DeviceObj.fIDEDstCLK = 27;
    }

    OpenTM027CDH01(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseTM027CDH01();
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
    sendtoLCDreg(addr, value);
}
#endif

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
void sendtoLCDreg(UINT32 addr, UINT32 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sendtoLCDreg_sif(addr, value);
    }
    else
    {
        sendtoLCDreg_gpio(addr, value);
    }
}

/**
  delay time period

  @param times : time

*/
static void delay_TM027CDH01(UINT32 times)
{
    for( ; times>0; times--)
        ;
}
/**
  Open panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
  */
void OpenTM027CDH01(LCDSIZE *psizeDisplay)
{
    UINT8   uiClockDiv;

    if (ide_open() != E_OK)
    {
        return;
    }

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
    case LCD_MODE_RGB_THROUGH:
        // Top control for LCD type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK to 12.9MHz
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/2;
        debug_err(("Set IDE clock div = %d",uiClockDiv-1));
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        ide_set_icst(0);
        //#NT#2009/12/24#Klins Chen -begin
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        //#NT#2009/12/24#Klins Chen -end
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
        ide_set_through_mode(1);

        ide_set_hor_timing(0x00, 0x6B3, 0x46, 0x405);
        ide_set_ver_timing(0x00, 0x106, 0x15, 0x104, 0x15, 0x104);

        ide_set_interlace(0);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = TM027CDH01_RGBTHROUGH_BUF_WIDTH;
        psizeDisplay->uiHeight      = TM027CDH01_RGBTHROUGH_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TM027CDH01_RGBTHROUGH_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TM027CDH01_RGBTHROUGH_WIN_HEIGHT;
        break;
/*
    case LCD_MODE_YUV:
        // Top control for LCD type
        pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK to 27MHz
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        //Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //delay_TM027CDH01(0x10000);

        ide_set_icst(0);
        //#NT#2009/12/24#Klins Chen -begin
        ide_configIcst(FALSE, CST_YCBCR2RGB);
        //#NT#2009/12/24#Klins Chen -end
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

        ide_set_hor_timing(1-1, 1560-1, 120-1, 120+TM027CDH01_RGB_WIN_WIDTH*4-1);
        ide_set_ver_timing(1-1, 21+TM027CDH01_RGB_WIN_HEIGHT+5-1, 21-1, 21+TM027CDH01_RGB_WIN_HEIGHT-1, 21-1, 21+TM027CDH01_RGB_WIN_HEIGHT-1);

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

        psizeDisplay->uiWidth       = TM027CDH01_YUV640_BUF_WIDTH;
        psizeDisplay->uiHeight      = TM027CDH01_YUV640_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TM027CDH01_YUV640_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TM027CDH01_YUV640_WIN_HEIGHT;
        break;
*/
    case LCD_MODE_RGBD:
        // Top control for LCD type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK to 27MHz
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        ide_set_icst(0);
        //#NT#2009/12/24#Klins Chen -begin
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        //#NT#2009/12/24#Klins Chen -end
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

        ide_set_hor_timing(1-1, 1560-1, 241-1, 241+TM027CDH01_RGB_WIN_WIDTH*4-1);
        ide_set_ver_timing(1-1, 21+TM027CDH01_RGB_WIN_HEIGHT+5-1, 21-1, 21+TM027CDH01_RGB_WIN_HEIGHT-1, 21-1, 21+TM027CDH01_RGB_WIN_HEIGHT-1);

        ide_set_interlace(0x01);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = TM027CDH01_RGBD_BUF_WIDTH;
        psizeDisplay->uiHeight      = TM027CDH01_RGBD_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TM027CDH01_RGBD_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TM027CDH01_RGBD_WIN_HEIGHT;
        break;

    case LCD_MODE_RGB:
        // Top control for LCD type
        pinmux_select_lcd(PINMUX_LCD_SERIAL_RGB_8BIT);

        // Set DCLK
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/LCD_DeviceObj.fIDEDstCLK/2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);


        ide_set_icst(0);
        //#NT#2009/12/24#Klins Chen -begin
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        //#NT#2009/12/24#Klins Chen -end
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
        ide_set_through_mode(0);

        ide_set_hor_timing(0x00, 0x6B3, 0x46, 0x405);
        ide_set_ver_timing(0x00, 0x106, 0x15, 0x104, 0x15, 0x104);

        ide_set_interlace(0);
        ide_set_startfield(0x00);

        ide_set_ctrst(0x40);
        ide_set_brt(0x00);
        ide_set_cmults(0x40);
        ide_set_cex(0x00);
        ide_set_standby_signal(0x00);

        ide_set_clamp(0x01);
        ide_set_tv_powerdown(0x01);

        ide_set_sif_startend(0x00, 0x00);

        psizeDisplay->uiWidth       = TM027CDH01_RGB_BUF_WIDTH;
        psizeDisplay->uiHeight      = TM027CDH01_RGB_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TM027CDH01_RGB_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TM027CDH01_RGB_WIN_HEIGHT;
        break;

    default:
        debug_err(("Not support this mode yet\r\n"));
        return;
    }


    ide_set_en(TRUE);
    ide_wait_frame_end();


    Init_TM027CDH01_LCD();


}

/**
  Close driver

  Release driver, private function for internal used

*/
void CloseTM027CDH01(void)
{

    //Power off sequence for this panel
    sendtoLCDreg(0x05,0xC4); //BL PWM control ;Turn off

    sendtoLCDreg(0x2B,0x00); //Enter standby mode
    //Must wait for 5 frame sync duration after entering standby mode
    ide_wait_frame_end();
    ide_wait_frame_end();
    ide_wait_frame_end();
    ide_wait_frame_end();
    ide_wait_frame_end();

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_close();
    }
    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();



    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

void Init_TM027CDH01_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = USED_SIF_MODE;
        SIFCtrl.bSyncEn       = FALSE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;
        sif_setControl((SIF_CH)LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }
    else
    {
        gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
        gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
        gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);
    }

    //Set LCD register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    //Wait for at least 40ms to start SPI access after VDD is ON.
    case LCD_MODE_RGB_THROUGH:
        sendtoLCDreg(0x05,0x06); //Reset
        //Wait for at least 40ms to do SPI access after Reset.
        delay_TM027CDH01(0x8000);

        sendtoLCDreg(0x05,0xC6); //BL PWM control

        sendtoLCDreg(0x00,0x0C); //VCAC setting

        sendtoLCDreg(0x01,0xA8); //VCOMDC

        sendtoLCDreg(0x04,0x0F); //8bit RGB interface

        sendtoLCDreg(0x0B,0x80); //off MTP, use R01h to control VCOMDC

        sendtoLCDreg(0x2B,0x01); //exit standby mode

        sendtoLCDreg(0x2F,0x6D); //SOPC, Normal LC,CF,VGH
        break;

/*    case LCD_MODE_YUV:
        sendtoLCDreg(0x05,0x06); //Reset
        //Wait for at least 40ms to do SPI access after Reset.
        delay_TM027CDH01(0x8000);

        sendtoLCDreg(0x05,0xC6); //BL PWM control

        sendtoLCDreg(0x00,0x4C); //VCAC setting + Enable CCIR601
        sendtoLCDreg(0x01,0xA8); //VCOMDC

        sendtoLCDreg(0x04,0x4F); //YUV640 interface + Enable CCIR656
        sendtoLCDreg(0x0B,0x80); //off MTP, use R01h to control VCOMDC

        sendtoLCDreg(0x2B,0x01); //exit standby mode
        sendtoLCDreg(0x2F,0x6D); //SOPC, Normal LC,CF,VGH
        break;
*/
    case LCD_MODE_RGBD:
        sendtoLCDreg(0x05,0x06); //Reset
        //Wait for at least 40ms to do SPI access after Reset.
        delay_TM027CDH01(0x8000);

        sendtoLCDreg(0x05,0xC6); //BL PWM control

        sendtoLCDreg(0x00,0x0C); //VCAC setting
        sendtoLCDreg(0x01,0xA8); //VCOMDC

        sendtoLCDreg(0x04,0x1F); //8bit RGBD interface
        sendtoLCDreg(0x0B,0x80); //off MTP, use R01h to control VCOMDC

        sendtoLCDreg(0x2B,0x01); //exit standby mode
        sendtoLCDreg(0x2F,0x6D); //SOPC, Normal LC,CF,VGH
        break;

    case LCD_MODE_RGB:
        sendtoLCDreg(0x05,0x06); //Reset
        //Wait for at least 40ms to do SPI access after Reset.
        delay_TM027CDH01(0x8000);

        sendtoLCDreg(0x05,0xC6); //BL PWM control

        sendtoLCDreg(0x00,0x0C); //VCAC setting

        sendtoLCDreg(0x01,0xA8); //VCOMDC

        sendtoLCDreg(0x04,0x0F); //8bit RGB interface

        sendtoLCDreg(0x0B,0x80); //off MTP, use R01h to control VCOMDC

        sendtoLCDreg(0x2B,0x01); //exit standby mode

        sendtoLCDreg(0x2F,0x6D); //SOPC, Normal LC,CF,VGH
        break;
    }
}

void sendtoLCDreg_sif(UINT32 addr, UINT32 value)
{
    UINT32 uiSIFData;

    uiSIFData = PanelFormCmdDataH16(addr, value);

    sif_sendMSB((SIF_CH)LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

void sendtoLCDreg_gpio(UINT32 addr, UINT32 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

    data = PanelFormCmdDataL16(addr, value);

    delay_TM027CDH01(0x3000);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    delay_TM027CDH01(2000);

    for(j = 16; j > 0; j--)
    {
        if(data & 0x8000)
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay_TM027CDH01(3000);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_TM027CDH01(2000);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_TM027CDH01(3000);

        data <<= 1;
    }

    delay_TM027CDH01(2000);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

}

// add for compile issue
void LCD_Rotate(UINT32 bRotate)
{

}

