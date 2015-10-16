/*
    @file       ToppolyTD24.c

    @ingroup    mILIBLCDToppolyTD24THEA1

    @brief      AU LCD Driver of ToppolyTD24THEA1 panel

    @note       Nothing.

    @author

    @date       2007/05/08

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

*/

/**
@addtogroup mILIBLCDToppolyTD24THEA1

@verbatim

    Usage:  Configure LCD_DEVICE_OBJ and LCDSIZE structure such as SysInit.c, then call OpenPanel to open specific LCD panel driver.

            Setup SIF pad pull up/down for designated SIF channel according to SIF mode setting (Use SIF mode 0100).

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

// Address ==  0x02
#define INPUT_FORMAT_RGBD           0x00
#define INPUT_FORMAT_YUV            0x01
#define INPUT_FORMAT_THROUGH        0x02
#define INPUT_FORMAT_STD_NTSC       0x00
#define INPUT_FORMAT_STD_PAL        0x04
#define INPUT_VALID_DATA_1280       0x00
#define INPUT_VALID_DATA_1440       0x08
#define INPUT_CLK_EDGE_POS          0x00
#define INPUT_CLK_EDGE_NEG          0x10
#define INPUT_HD_POL_LOW            0x00
#define INPUT_HD_POL_HIGH           0x20
#define INPUT_VD_POL_LOW            0x00
#define INPUT_VD_POL_HIGH           0x40

// Address ==  0x03
#define INTERLACE_MODE              0x00
#define NONINTERLACE_MODE           0x01
#define YUV_INPUT_TRANSFER_MATRIX   0x10
#define UV_OFFSET_MATRIX            0x20

// Address == 0x04
#define POWER_STANDBY               0x00
#define POWER_NORMAL                0x01
#define CP_CLK_DISABLE              0x00
#define CP_CLK_ENABLE               0x02
#define PWM_OUTPUT_DISABLE          0x00
#define PWM_OUTPUT_ENABLE           0x04
#define PRE_CHARGE_DISABLE          0x00
#define PRE_CHARGE_ENABLE           0x08
#define OUTPUT_DRIVER_50            0x00
#define OUTPUT_DRIVER_65            0x10
#define OUTPUT_DRIVER_80            0x20
#define OUTPUT_DRIVER_100           0x30

// Address == 0x05
#define HOR_REVERSE_MODE            0x00
#define HOR_NORMAL_MODE             0x01
#define VER_REVERSE_MODE            0x00
#define VER_NORMAL_MODE             0X02
#define COLOR_FILTER_DELTA          0x00
#define COLOR_FILTER_STRIP          0x04
#define PHASE_NORMAL                0x00
#define PHASE_SH2                   0x08
#define PHASE_SH4                   0x10
#define PHASE_SH6                   0x18
#define PHASE_SH8                   0x20
#define PHASE_SH10                  0x28
#define PHASE_SH12                  0x30
#define PHASE_SH14                  0x38

// Address == 0x06
#define HOR_POS_RIGHT_24            0x00
#define HOR_POS_CENTER              0x18
#define HOR_POS_LEFT_21             0x2D

// Address == 0x07
#define VER_POS_DOWN_8              0x00
#define VER_POS_CENTER              0x08
#define VER_POS_UP_7                0x0F

// Address == 0x08
#define ENB_ORIGIN                  0x00
#define ENB_RIGHT_63                0x3F

// Window and buffer size
// Through mode
#define TOPPOLY_BUF_WIDTH           480
#define TOPPOLY_BUF_HEIGHT          240
#define TOPPOLY_WIN_WIDTH           480
#define TOPPOLY_WIN_HEIGHT          240


// Function declaration
static void delay_Toppoly(UINT32 times);
static void sendtoToppolyLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoToppolyLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoToppolyLCDreg(UINT8 addr, UINT8 value);

static void Init_Toppoly_LCD(void);
static void OpenToppoly(LCDSIZE *psizeDisplay);
static void CloseToppoly(void);

// Static variable
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
        switch (LCD_DeviceObj.uiLCDMode)
        {
        case LCD_MODE_RGB:
            LCD_DeviceObj.fIDEDstCLK = 10.36;
            break;
        }
    }

    OpenToppoly(psizeDisplay);
}

/**
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @return void
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

#endif

/*
  delay time period

  @param times : time period

*/
static void delay_Toppoly(UINT32 times)
{
    for( ; times>0; times--)
       ;
}

/*
  Send data to specific address by SIF

  @param addr specific address
  @param value value

*/
static void sendtoToppolyLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    uiSIFData = (((UINT32)addr << 10)|(UINT32)value ) << 16;
    sif_sendMSB(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

/*
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

    data = (UINT32)addr << 10 | (UINT32)value;

    delay_Toppoly(500);

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

        delay_Toppoly(100);

        gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_Toppoly(200);

        gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);

        delay_Toppoly(100);

        data <<= 1;
    }

    delay_Toppoly(500);
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}

/*
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value
*/
static void sendtoToppolyLCDreg(UINT8 addr, UINT8 value)
{
    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
         sendtoToppolyLCDreg_sif(addr, value);
    }
    else
    {
         sendtoToppolyLCDreg_gpio(addr, value);
    }
}

/*
    Initial LCD of ToppolyTD24THEA1, configure SIF intarface and send data to panel register to configure

*/
static void Init_Toppoly_LCD(void)
{
    SIF_CTRL    SIFCtrl;

    if (LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();

        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv        = 7;
        SIFCtrl.uiMode          = SIF_MODE_0100;
        SIFCtrl.uiLength        = 16;
        SIFCtrl.bMSB            = TRUE;
        //SIFCtrl.bSyncEn         = FALSE;
        //SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration   = 0;
        SIFCtrl.uiSenH          = 0;

        // Init SIF
        sif_setControl(LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    if(LCD_DeviceObj.fpPostAdjusement)
    {
        LCD_DeviceObj.fpPostAdjusement();
    }
    else
    {

        // Set LCD register
        switch (LCD_DeviceObj.uiLCDMode)
        {
            case LCD_MODE_RGB:
                sendtoToppolyLCDreg(0x02, INPUT_FORMAT_THROUGH|INPUT_FORMAT_STD_NTSC|INPUT_VALID_DATA_1280|INPUT_CLK_EDGE_POS|INPUT_HD_POL_LOW|INPUT_VD_POL_LOW);
                break;
            case LCD_MODE_YUV:
            case LCD_MODE_RGBD:
            default:
                break;
        }

        // Configure register 0x03
        sendtoToppolyLCDreg(0x03, NONINTERLACE_MODE|YUV_INPUT_TRANSFER_MATRIX|UV_OFFSET_MATRIX);

        // Configure register 0x04
        sendtoToppolyLCDreg(0x04, POWER_NORMAL|CP_CLK_ENABLE|PWM_OUTPUT_ENABLE|PRE_CHARGE_ENABLE|OUTPUT_DRIVER_50);

        // Configure register 0x05
        sendtoToppolyLCDreg(0x05, HOR_NORMAL_MODE|VER_NORMAL_MODE|PHASE_SH4);

        // Configure register 0x06
        sendtoToppolyLCDreg(0x06, HOR_POS_CENTER);

        // Configure register 0x07
        sendtoToppolyLCDreg(0x07, VER_POS_CENTER);
        // Configure register 0x08
        sendtoToppolyLCDreg(0x08, ENB_ORIGIN);
    }
}
/*
  Open ToppolyTD24THEA1 panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
static void OpenToppoly(LCDSIZE *psizeDisplay)
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

    // Wait for power stable
    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay_Toppoly(0x10000);
    //#NT#2009/11/04#Klins Chen#[0007607] -end

    // Set IDE Register
    switch (LCD_DeviceObj.uiLCDMode)
    {
    case LCD_MODE_RGB:
        // Top control for lcd type
        pinmux_select_lcd(PINMUX_LCD_SERL_YCBCR_8BIT);

        // Set DCLK to 10.36MHz
        uiClockDiv = (float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

        // Enable IDE Clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        // Wait for power stable
        //#NT#2009/11/04#Klins Chen#[0007607] -begin
        //#NT#Remove unnecessary dummy delay
        //delay_Toppoly(0x10000);
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

        ide_set_hor_timing(0x00, 0x293, 0x60, 0x240);
        ide_set_ver_timing(0x00, 0x106, 0x0E, 0x0FD, 0x0E, 0x0FD);
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

        psizeDisplay->uiWidth       = TOPPOLY_BUF_WIDTH;
        psizeDisplay->uiHeight      = TOPPOLY_BUF_HEIGHT;
        psizeDisplay->uiWinWidth    = TOPPOLY_WIN_WIDTH;
        psizeDisplay->uiWinHeight   = TOPPOLY_WIN_HEIGHT;
        break;
    case LCD_MODE_YUV:
    case LCD_MODE_RGBD:
        debug_err(("Not support\r\n"));
        break;
    }


    ide_set_en(TRUE);
    // Avoid vsync active area
    ide_wait_frame_end();

    Init_Toppoly_LCD();
}

/*
  Close ToppolyTD24THEA1 driver

  Release ToppolyTD24THEA1 driver, private function for internal used

*/
static void CloseToppoly(void)
{
    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Change to standby mode
    sendtoToppolyLCDreg(0x04, POWER_STANDBY);

    delay_Toppoly(0x80000);

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}
//@}
