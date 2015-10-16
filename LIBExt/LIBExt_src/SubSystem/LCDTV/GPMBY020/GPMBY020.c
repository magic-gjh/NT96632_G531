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

#define AU_BL02_UPS051_Buf_width       960
#define AU_BL02_UPS051_Buf_height      240
#define AU_BL02_UPS051_Win_width       960
#define AU_BL02_UPS051_Win_height      240

#define AU_BL02_UPS052_Buf_width       320
#define AU_BL02_UPS052_Buf_height      240
#define AU_BL02_UPS052_Win_width       320
#define AU_BL02_UPS052_Win_height      240


#define AUBL02_REG5_SHDB1      0x02

//function declaration
static void delay(UINT32 times);
static void sendtoAUBL02LCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoAUBL02LCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoAUBL02LCDreg(UINT8 addr, UINT8 value);

static void Init_AUBL02_LCD(void);
static void OpenAUBL02(LCDSIZE *psizeDisplay);
static void CloseAUBL02(void);

//static variable
static LCD_DEVICE_OBJ LCD_DeviceObj;

//variable
UINT8 gAUBL02_VCOM_DC = 0x18;
UINT8 gAUBL02_POWER_CTRL;
UINT8 gAUBL02_POWERBOOST = AUBL02_REG5_SHDB1;

static UINT8 GammaTbl[] = {
0, 13, 27, 41, 55, 69, 83, 98,114,130,
146,164,182,200,216,233,255
};

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

    debug_msg("GPMBY020\r\n");
    if (LCD_DeviceObj.fIDEDstCLK == 0)
    {
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case LCD_MODE_RGB:
            LCD_DeviceObj.fIDEDstCLK = 27;
            break;
        default:            
            debug_msg("non-RGB mode");
            return;            
        }
    }

    OpenAUBL02(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
*/
void ClosePanel(void)
{
    CloseAUBL02();
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
    sendtoAUBL02LCDreg(addr, value);
}
#endif
/*
  delay time period

  @param times : time

*/
static void delay(UINT32 times)
{
    for(;times > 0; times--)
        ;
}


static void Write_COMMAND8i(unsigned int nCmd)
{
    UINT32 data;
    UINT32 j;

    data = (UINT32)nCmd;
    data = data & 0x00ff;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 9; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);;
        delay(200);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
    }


    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

}

static void Write_DATA8i(unsigned int nData)
{
 UINT32 data,j;


    data = (UINT32)nData;
    data |= 0x0100;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 9; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);;
        delay(200);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(100);
    }


    delay(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}


/*
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoAUBL02LCDreg_sif(UINT8 addr, UINT8 value)
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
static void sendtoAUBL02LCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

    data = (ULONG)addr << 8 | (ULONG)value;


    delay(500);

    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);

    for(j = 16; j > 0; j--)
    {
        if(((data >> (j - 1)) & 0x01))
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(100);
        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);;
        delay(200);
        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
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
static void sendtoAUBL02LCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
         sendtoAUBL02LCDreg_sif(addr, value);
    }
    else
    {
         sendtoAUBL02LCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of AUBL02, configure SIF intarface and send data to panel register to configure

*/
static void Init_AUBL02_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();
        // Max clock = 1000 / 210(ns) = 4.762MHz, uiClkDiv = (48 / (4.762 * 2)) - 1 = 4.039
        // Min uiClkDiv = 4.039, set uiClkDiv = 5, thus clock = 4.8MHz
        SIFCtrl.uiClkDiv      = 5;
        SIFCtrl.uiMode        = SIF_MODE_0100;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        //SIFCtrl.bSyncEn       = FALSE;
        //SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 4;
        SIFCtrl.uiSenH        = 4;

        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

        switch(LCD_DeviceObj.uiLCDMode)
        {
            case LCD_MODE_RGB:       //051 mode

            Write_COMMAND8i(0xB9);
            Write_DATA8i(0xFF);
            Write_DATA8i(0x93);
            Write_DATA8i(0x42);

            Write_COMMAND8i(0x21);

            Write_COMMAND8i(0x36);
            Write_DATA8i(0xC8);     //08 旋轉方向
            //Write_DATA16i(0x08);     //08 旋轉方向

            Write_COMMAND8i(0x3A);
            Write_DATA8i(0x66);     //18BIT PIX FOMART


            Write_COMMAND8i(0xb0);
            Write_DATA8i(0xe0);

            Write_COMMAND8i(0xF6);
            Write_DATA8i(0x01);
            Write_DATA8i(0x00);
            Write_DATA8i(0x07);

            Write_COMMAND8i(0xC0);
            Write_DATA8i(0x28);
            Write_DATA8i(0x0A);

            Write_COMMAND8i(0xC1);
            Write_DATA8i(0x02);

            Write_COMMAND8i(0xC5);
            Write_DATA8i(0x2F);
            Write_DATA8i(0x3a);

            Write_COMMAND8i(0xC7);
            Write_DATA8i(0xC2);
            Write_DATA8i(0xB8);
            Write_DATA8i(0x0A);

            Write_COMMAND8i(0xE0);
            Write_DATA8i(0x0F);
            Write_DATA8i(0x23);
            Write_DATA8i(0x20);
            Write_DATA8i(0x09);
            Write_DATA8i(0x0F);
            Write_DATA8i(0x07);
            Write_DATA8i(0x50);
            Write_DATA8i(0x76);
            Write_DATA8i(0x40);
            Write_DATA8i(0x0A);
            Write_DATA8i(0x15);
            Write_DATA8i(0x06);
            Write_DATA8i(0x11);
            Write_DATA8i(0x0E);
            Write_DATA8i(0x08);

            Write_COMMAND8i(0xE1);
            Write_DATA8i(0x08);
            Write_DATA8i(0x1F);
            Write_DATA8i(0x21);
            Write_DATA8i(0x06);
            Write_DATA8i(0x0F);
            Write_DATA8i(0x03);
            Write_DATA8i(0x32);
            Write_DATA8i(0x23);
            Write_DATA8i(0x41);
            Write_DATA8i(0x01);
            Write_DATA8i(0x08);
            Write_DATA8i(0x08);
            Write_DATA8i(0x30);
            Write_DATA8i(0x33);
            Write_DATA8i(0x0F);

            Write_COMMAND8i(0x11); //Exit Sleep
            TimerDelayMs(10);

            Write_COMMAND8i(0x11); //Exit Sleep
            TimerDelayMs(80);

            Write_COMMAND8i(0x29); //Display ON

            Write_COMMAND8i(0x2c); //Display ON

            break;

            case LCD_MODE_RGBD:       //052 mode
            if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
            {
                sendtoAUBL02LCDreg(0x05,0x57);
                sendtoAUBL02LCDreg(0x02,0x04);
                sendtoAUBL02LCDreg(0x03,0x40);
                sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
                sendtoAUBL02LCDreg(0x05,0x55|gAUBL02_POWERBOOST);
                gAUBL02_POWER_CTRL = 0x55|gAUBL02_POWERBOOST;
            }
            else
            {
                sendtoAUBL02LCDreg(0x05,0x57);
                sendtoAUBL02LCDreg(0x05,0x57);
                sendtoAUBL02LCDreg(0x02,0x04);
                sendtoAUBL02LCDreg(0x02,0x04);
                sendtoAUBL02LCDreg(0x03,0x40);
                sendtoAUBL02LCDreg(0x03,0x40);
                sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
                sendtoAUBL02LCDreg(0x04,0x1B);  //UPS052
                sendtoAUBL02LCDreg(0x05,0x55|gAUBL02_POWERBOOST);
                sendtoAUBL02LCDreg(0x05,0x55|gAUBL02_POWERBOOST);
                gAUBL02_POWER_CTRL = 0x55|gAUBL02_POWERBOOST;

                sendtoAUBL02LCDreg(0x01,gAUBL02_VCOM_DC);
                sendtoAUBL02LCDreg(0x01,gAUBL02_VCOM_DC);
            }
            break;
        }

}
/*
  Open AUBL02 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenAUBL02(LCDSIZE *psizeDisplay)
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

        // wait for Power Stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay(0x10000);
        //#NT#2009/11/04#Klins Chen#[0007607] -end

        ide_set_icst(0);
        ide_setICST(TRUE, CST_YCBCR2RGB);
        ide_set_dithering(0, 0);
        ide_set_device(DISPLAY_DEVICE_CASIO2G);
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

        ide_set_shw_st();
        //ide_set_en(1);
        //ide_set_load();


//        ide_set_hor_timing(0x00, 359, 30, 350);
//        ide_set_ver_timing(0x00, 247, 4, 244, 4, 244);
//        ide_set_hor_timing(0x00, 0x284, 0x1E, 0x214);
//        ide_set_ver_timing(0x00, 0x105, 0x15, 0x104, 0x15, 0x104);
        ide_set_hor_timing(0x9, 990, 20, 979);
        ide_set_ver_timing(1, 246, 2, 241, 2, 241);
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


        psizeDisplay->uiWidth       = AU_BL02_UPS051_Buf_width;
        psizeDisplay->uiHeight      = AU_BL02_UPS051_Buf_height;
        psizeDisplay->uiWinWidth    = AU_BL02_UPS051_Win_width;
        psizeDisplay->uiWinHeight   = AU_BL02_UPS051_Win_height;
        break;

    case LCD_MODE_RGBD:       //052 mode

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
        //ide_set_en(1);
        //ide_set_load();


        ide_set_hor_timing(0x00, 0x617, 0xF1, 0x5F0);
        ide_set_ver_timing(0x01, 0x105, 0x15, 0x104, 0x15, 0x104);
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


        psizeDisplay->uiWidth       = AU_BL02_UPS052_Buf_width;
        psizeDisplay->uiHeight      = AU_BL02_UPS052_Buf_height;
        psizeDisplay->uiWinWidth    = AU_BL02_UPS052_Win_width;
        psizeDisplay->uiWinHeight   = AU_BL02_UPS052_Win_height;
        break;
    }


    ide_setGammaEn(TRUE);
    ide_fillGamma(GammaTbl);
    ide_setCmults(0x42);

    ide_set_en(TRUE);
    //avoid vsync active area
    ide_wait_frame_end();

    Init_AUBL02_LCD();
}

/*
  Close AUBL02 driver

  Release AUBL02 driver, private function for internal used

*/
static void CloseAUBL02(void)
{
    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    sendtoAUBL02LCDreg(0x05,0x54);

    delay(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

/*
  Turn On / Off AUBL02 Power Boost

  @Param bTurnOn TRUE --> turn on FALSE --> turn off
*/
void TurnOnOffAUBL02_PowerBoost(BOOL bTurnOn)
{
    if (bTurnOn == TRUE)
    {
        gAUBL02_POWER_CTRL |= (AUBL02_REG5_SHDB1);
        gAUBL02_POWERBOOST = AUBL02_REG5_SHDB1;
    }
    else
    {
        gAUBL02_POWER_CTRL &= ~(AUBL02_REG5_SHDB1);
        gAUBL02_POWERBOOST = 0x00;
    }

    sendtoAUBL02LCDreg(0x05,gAUBL02_POWER_CTRL);
}

/*
  Turn On / Off AUBL02 Power Boost before open

  @Param bTurnOn TRUE --> turn on FALSE --> turn off
*/
void TurnOnOffAUBL02_PowerBoost_BeforeOpen(BOOL bTurnOn)
{
    if (bTurnOn == TRUE)
        gAUBL02_POWERBOOST = AUBL02_REG5_SHDB1;
    else
        gAUBL02_POWERBOOST = 0x00;
}

/*
  Write data to BL02 panel by SIF interface

  @Param para_str write data
*/
void WriteBL02SIF(char *para_str)
{
    UINT32    uiRegadd, uiRegValue;
    UINT8    arg_num;
    arg_num=sscanf(para_str,"%x %x \n",&uiRegadd,&uiRegValue);
    sendtoAUBL02LCDreg(uiRegadd, uiRegValue);
}

void LCD_Rotate(UINT32 bRotate)
{

}

//@}
