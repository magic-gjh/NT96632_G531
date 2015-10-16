//#NT#2008/12/23#Niven Cho -begin
//#NT#Refine,Re-Organize
/*
    @file       WD_F4823V7_7FLWA.c
    @brief      LCD Driver of WD_F4823V7_7FLWA panel, TCON:HX8224-B
    @author     Niven Cho
    @date       2008/12/22
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Kernel.h"

//Register Description
#define REG_R00 0x00
#define REG_R01 0x01
#define REG_R02 0x02
#define REG_R03 0x03
#define REG_R04 0x04
#define REG_R05 0x05
#define REG_R06 0x06
#define REG_R07 0x07
#define REG_T00 0x08
#define REG_T02 0x0A
#define REG_T06 0x0E
#define REG_T07 0x0F

//#NT#2008/12/23#Niven Cho -begin
//#NT#Refine,Macro Setting
//Register Setting Value Description
//R00 Single Settings
#define R00_GRB_GLOBAL_RESET    0x00 //Bit0
#define R00_GRB_NORMAL_OP       0x01 //Bit0
#define R00_STB_STANDBY         0x00 //Bit1
#define R00_STB_NORMAL_OP       0x02 //Bit1
#define R00_SHCB_CHARGEPUMP_OFF 0x00 //Bit2
#define R00_SHCB_CHARGEPUMP_ON  0x04 //Bit2
#define R00_SHDB_PWM_OFF        0x00 //Bit3
#define R00_SHDB_PWM_ON         0x08 //Bit3
//R00 Macro Settings
#define R00_MODE_RESET          (R00_GRB_GLOBAL_RESET|R00_STB_STANDBY  |R00_SHCB_CHARGEPUMP_OFF|R00_SHDB_PWM_OFF)
#define R00_MODE_INIT           (R00_GRB_NORMAL_OP   |R00_STB_NORMAL_OP|R00_SHCB_CHARGEPUMP_ON |R00_SHDB_PWM_ON )
#define R00_MODE_STANDBY        (R00_GRB_NORMAL_OP   |R00_STB_STANDBY  |R00_SHCB_CHARGEPUMP_OFF|R00_SHDB_PWM_OFF)
//#NT#2008/12/23#Niven Cho -end
//R03 settings
//#define


#define OPTION_SIF_0000  0x0   ///< SIF bus mode 0000
#define OPTION_SIF_0001  0x1   ///< SIF bus mode 0001
#define OPTION_SIF_0010  0x2   ///< SIF bus mode 0010
#define OPTION_SIF_0011  0x3   ///< SIF bus mode 0011
#define OPTION_SIF_0100  0x4   ///< SIF bus mode 0100
#define OPTION_SIF_0101  0x5   ///< SIF bus mode 0101
#define OPTION_SIF_0110  0x6   ///< SIF bus mode 0110
#define OPTION_SIF_0111  0x7   ///< SIF bus mode 0111
#define OPTION_SIF_1000  0x8   ///< SIF bus mode 1000
#define OPTION_SIF_1001  0x9   ///< SIF bus mode 1001
#define OPTION_SIF_1010  0xA   ///< SIF bus mode 1010
#define OPTION_SIF_1011  0xB   ///< SIF bus mode 1011
#define OPTION_SIF_1100  0xC   ///< SIF bus mode 1100
#define OPTION_SIF_1101  0xD   ///< SIF bus mode 1101
#define OPTION_SIF_1110  0xE   ///< SIF bus mode 1110
#define OPTION_SIF_1111  0xF   ///< SIF bus mode 1111


typedef struct _PANEL_CMD{
    UINT8 Address;
    UINT8 Value;
}tPANEL_CMD;

typedef struct _PANEL_PARAM{
    LCD_MODE    LCDMode;
    float       DCLK;
    UINT32      HSyncTotalPeriod;
    UINT32      HSyncActivePeriod;
    UINT32      HSyncBackPorch;
    UINT32      VSyncTotalPeriod;
    UINT32      VSyncActivePeriod;
    UINT32      VSyncBackPorchOdd;
    UINT32      VSyncBackPorchEven;
    UINT32      BufferWidth;
    UINT32      BufferHeight;
    UINT32      WindowWidth;
    UINT32      WindowHeight;
    BOOL        bYCbCrFormat; //!< TRUE(YUV): YUV, FALSE(RGB): RGB/RGBD
}tPANEL_PARAM;

typedef struct _IDE_PARAM{
    UINT            pinmux_select_lcd;
    ICST_TYPE       icst;
    BOOL            dithering[2];
    IDE_DEVICE_TYPE device;
    BOOL            pdir;
    UINT32          odd;
    UINT32          even;
    BOOL            hsinv;
    BOOL            vsinv;
    BOOL            hvldinv;
    BOOL            vvldinv;
    BOOL            clkinv;
    BOOL            fieldinv;
    BOOL            rgbdummy;
    BOOL            interlace;
    BOOL            startfield;
    UINT32          ctrst;
    UINT32          brt;
    UINT32          cmults;
    BOOL            cex;
    BOOL            standby_signal;
    BOOL            clamp;
    BOOL            tv_powerdown;
    UINT32          sif_startend[2];
}tIDE_PARAM;

typedef struct _LCD_PARAM{
    tPANEL_PARAM Panel;
    tIDE_PARAM   IDE;
    tPANEL_CMD*  pCmdQueue;
    int          nCmd;
}tLCD_PARAM;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Step 1: Set All Parameters
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--- Check SIF of Settings ---//

//#NT#2008/12/23#Niven Cho -begin
//#NT#Bug Fix,Wrong Reg Writing Settings
#define SETTING_SIF_MODE                    OPTION_SIF_0000 //SIF MODE: SIF_MODE_0000 ~ SIF_MODE_1111
#define SETTING_PANEL_REG_BIT_SHIFT_ADDRESS 10              //D10 is Address LSB
#define SETTING_PANEL_REG_BIT_SHIFT_VALUE   0               //D0  is Content LSB
#define SETTING_PANEL_REG_BIT_WRITE         9               //D9  is Bit of Write Command
//#NT#2008/12/23#Niven Cho -end

//#NT#2008/12/23#Niven Cho -begin
//#NT#Refine,Macro Setting
//--- Prepare LCD Command Queue ---//
static tPANEL_CMD tCmdStandby[] = { //Standby Mode, for ClosePanel()
    {REG_R00,R00_MODE_STANDBY}, //Set R00 to Standby
};

static tPANEL_CMD tCmdModeRGB[] = {
    {REG_R00,R00_MODE_RESET}, //Global Reset
    {REG_R00,R00_MODE_INIT},  //Internal PWM On,Others are Default
    //{REG_R03,0x01},
    //{REG_T00,0x07},
    //{REG_T02,0x3F},
    //{REG_R02,0x00},
};
//#NT#2008/12/23#Niven Cho -end

//--- Make Specification ---//
static tLCD_PARAM tMode[] = {
    {//LCD_MODE_RGB Begin
        {
            LCD_MODE_RGB,   //!< LCDMode;
            //24.54f,
            9.7f,           //!< DCLK;
            //1560,
            616,            //!< HSyncTotalPeriod;
            //1280,
            480,            //!< HSyncActivePeriod;
            //253,
            100,            //!< HSyncBackPorch;
            262,            //!< VSyncTotalPeriod;
            240,            //!< VSyncActivePeriod;
            13,             //!< VSyncBackPorchOdd;
            13,             //!< VSyncBackPorchEven;
            //320,
            480,            //!< BufferWidth;
            240,            //!< BufferHeight;
            //320,
            480,            //!< WindowWidth;
            240,            //!< WindowHeight;
            FALSE           //!< bYCbCrFormat;
        },
        {
            PINMUX_LCD_SERIAL_RGB_8BIT,    //!< pinmux_select_lcd;
            ICST_CCIR601,           //!< icst;
            {FALSE,FALSE},          //!< dithering[2];
            //DISPLAY_DEVICE_TOPPOLY,
            DISPLAY_DEVICE_CASIO2G, //!< device;
            FALSE,                  //!< pdir;
            0,                      //!< odd;
            1,                      //!< even;
            TRUE,                   //!< hsinv;
            TRUE,                   //!< vsinv;
            FALSE,                  //!< hvldinv;
            FALSE,                  //!< vvldinv;
            TRUE,                   //!< clkinv;
            FALSE,                  //!< fieldinv;
            //TRUE,                   //!< rgbdummy;
            FALSE,                  //!< rgbdummy;
            FALSE,                  //!< interlace;
            FALSE,                  //!< startfield;
            0x40,                   //!< ctrst;
            0x00,                   //!< brt;
            0x40,                   //!< cmults;
            FALSE,                  //!< cex;
            FALSE,                  //!< standby_signal;
            TRUE,                   //!< clamp;
            TRUE,                   //!< tv_powerdown;
            {0x00,0x00},            //!< sif_startend[2];
        },
        tCmdModeRGB,                               //!< pCmdQueue
        sizeof(tCmdModeRGB)/sizeof(tPANEL_CMD),    //!< nCmd
    },//LCD_MODE_RGB End
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Step 1: End
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//function declaration
static void delay(UINT times);
static void sendtoLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoLCDreg(UINT8 addr, UINT8 value);

static void DeviceInit(void);
static void DeviceOpen(LCDSIZE *psizeDisplay);
static void DeviceClose(void);

//static variable
static tLCD_PARAM* pMode; //Current Mode
static LCD_DEVICE_OBJ LCD_DeviceObj;
static BOOL         bPanelIsOpened = FALSE;

/**
  Open LCD panel driver

  This function should be called before calling any other functions during initial LCD driver, public function for global use

  @param lcddeviceobj initial value by upper layer
  @param psizeDisplay initial LCD size by upper layer
*/
void OpenPanel(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay)
{
    UINT32  i;

    LCD_DeviceObj = *lcddeviceobj;

    //Check Mode Valid
    pMode = NULL;
    for(i=0;i<sizeof(tMode)/sizeof(tLCD_PARAM);i++)
    {
        if(tMode[i].Panel.LCDMode==LCD_DeviceObj.uiLCDMode)
        {
            pMode = &tMode[i];
            break;
        }
    }

    if(pMode==NULL)
    {
        //debug_msg("g_LCDCtrlObj.uiLCDMode=%d not support\r\n", &LCD_DeviceObj.uiLCDMode);
        return;
    }

    LCD_DeviceObj.fIDEDstCLK = pMode->Panel.DCLK;

    DeviceOpen(psizeDisplay);
}

/**
  Close LCD driver, public function for global use

  Release LCD driver

*/
void ClosePanel(void)
{
    DeviceClose();
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


/**
  delay time period

  @param times : time period

*/
void delay(UINT uiNs)
{
    UINT32 i;


    switch (pll_getClockRate(PLL_CLKSEL_CPU))
    {
    #ifdef PLL_CLKSEL_CPU_48
    case PLL_CLKSEL_CPU_48:
        uiNs = uiNs * 48 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_60
    case PLL_CLKSEL_CPU_60:
        uiNs = uiNs * 60 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_120
    case PLL_CLKSEL_CPU_120:
        uiNs = uiNs * 120 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_160
    case PLL_CLKSEL_CPU_160:
        uiNs = uiNs * 160 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_240
    case PLL_CLKSEL_CPU_240:
        uiNs = uiNs * 240 / 80;
        break;
    #endif

    case PLL_CLKSEL_CPU_80:     // when uiUs is 1: delay 1.057 us
    default:
        // ratio: 1/1
        break;
    }

    for (i=((uiNs<<3)+(uiNs<<2)); i>0; i--)
    {
        __asm
        {
            nop;
        }
    }

}

/**
  Configure LCD register, using GPIO mode or SIF mode

  @param addr specific address
  @param value value

*/
void sendtoLCDreg(UINT8 addr, UINT8 value)
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
  Open panel driver, configure IDE related register, private function for internal used
  @param psizeDisplay initial LCD size by upper layer
*/
void DeviceOpen(LCDSIZE *psizeDisplay)
{
    UINT8   uiClockDiv;
    tIDE_PARAM*     pIDE = &pMode->IDE;
    tPANEL_PARAM*   pPanel = &pMode->Panel;

    if (ide_open() != E_OK)
    {
        return;
    }

    if (bPanelIsOpened == TRUE)
    {
        return;
    }
    bPanelIsOpened = TRUE;

    // Disable IDE
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Set IDE Clock Source
    if (LCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        //LCD_DeviceObj.uiIDESrcCLK = 240;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_240);
    }
    else
    {
        //LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }



    // wait for Power Stable
    delay(0x10000);

    // Set IDE Register
    pinmux_select_lcd(pIDE->pinmux_select_lcd);
    // Set DCLK
    if(pPanel->bYCbCrFormat)
    {
        uiClockDiv = (UINT8)((float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK);
    }
    else
    {
        uiClockDiv = (UINT8)((float)LCD_DeviceObj.uiIDESrcCLK/(float)LCD_DeviceObj.fIDEDstCLK/(float)2);
    }

    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    // Enable IDE clock
    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Wait for Power Stable
    delay(0x10000);

    ide_set_icst(pIDE->icst);
    ide_set_dithering(pIDE->dithering[0],pIDE->dithering[1]);
    ide_set_device(pIDE->device);
    ide_set_pdir(pIDE->pdir);
    ide_set_odd(pIDE->odd);
    ide_set_even(pIDE->even);
    ide_set_hsinv(pIDE->hsinv);
    ide_set_vsinv(pIDE->vsinv);
    ide_set_hvldinv(pIDE->hvldinv);
    ide_set_vvldinv(pIDE->vvldinv);
    ide_set_clkinv(pIDE->clkinv);
    ide_set_fieldinv(pIDE->fieldinv);
    ide_set_rgbdummy(pIDE->rgbdummy);
    ide_set_shw_st();

    if(pPanel->bYCbCrFormat)
    {
        ide_set_hor_timing(0x0
            ,(pPanel->HSyncTotalPeriod>>1)
            ,(pPanel->HSyncBackPorch>>1)
            ,(pPanel->HSyncActivePeriod+pPanel->HSyncBackPorch)>>1);
    }
    else
    {
        ide_set_hor_timing(0x00
            ,pPanel->HSyncTotalPeriod-1
            ,pPanel->HSyncBackPorch
            ,(pPanel->HSyncActivePeriod+pPanel->HSyncBackPorch)-1);
    }

        ide_set_ver_timing(0x00
        ,pPanel->VSyncTotalPeriod-1
        ,pPanel->VSyncBackPorchOdd
        ,(pPanel->VSyncActivePeriod+pPanel->VSyncBackPorchOdd)-1
        ,pPanel->VSyncBackPorchEven
        ,(pPanel->VSyncActivePeriod+pPanel->VSyncBackPorchEven)-1);

    ide_set_interlace(pIDE->interlace);
    ide_set_startfield(pIDE->startfield);

    ide_set_ctrst(pIDE->ctrst);
    ide_set_brt(pIDE->brt);
    ide_set_cmults(pIDE->cmults);
    ide_set_cex(pIDE->cex);
    ide_set_standby_signal(pIDE->standby_signal);

    ide_set_clamp(pIDE->clamp);
    ide_set_tv_powerdown(pIDE->tv_powerdown);

    ide_set_sif_startend(pIDE->sif_startend[0],pIDE->sif_startend[1]);

    psizeDisplay ->uiWidth      = pPanel->BufferWidth;
    psizeDisplay ->uiHeight     = pPanel->BufferHeight;
    psizeDisplay ->uiWinWidth   = pPanel->WindowWidth;
    psizeDisplay ->uiWinHeight  = pPanel->WindowHeight;

    delay(0x2000);
/*
    ide_set_pdir(IDE_PDIR_RBG);
    ide_set_even(IDE_LCD_B);
    ide_set_load();
*/
    ide_set_en(TRUE);
    //avoid vsync active area
    delay(0x2000);

    DeviceInit();
}

/**
  Close driver

  Release driver, private function for internal used

*/
void DeviceClose(void)
{
    UINT32  i;

    if (bPanelIsOpened == FALSE)
    {
        return;
    }
    bPanelIsOpened = FALSE;

    // Disable IDE
    //#NT#2009/01/06#Charles Lin -begin
    ide_wait_frame_end();
    //#NT#2009/01/06#Charles Lin -end
    ide_set_en(FALSE);

    for(i=0;i<sizeof(tCmdStandby)/sizeof(tPANEL_CMD);i++)
    {
        sendtoLCDreg(tCmdStandby[i].Address,tCmdStandby[i].Value);
    }

    //#NT#2009/04/02#Chris Hsu -begin
    //#NT#Speed up close process, delay time need to confirm again
    // Delay, This value is OK for customer's project to prevent seeing garbage
    // when open / close panel quickly.
    delay(0x2B000);
    //#NT#2009/04/02#Chris Hsu -end

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

#if (SETTING_SIF_MODE==OPTION_SIF_0000)     //Confirmed
    #define USED_SIF_MODE     SIF_MODE_0000
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0001)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0001
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0010)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0010
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0011)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0011
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0100)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0100
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0101)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0101
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0110)
    #define USED_SIF_MODE     SIF_MODE_0110 //Not Confirmed
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0111)   //Not Confirmed
    #define USED_SIF_MODE     SIF_MODE_0111
    #define SIF_CLK_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#elif (SETTING_SIF_MODE==OPTION_SIF_1000)   //Confirmed
    #define USED_SIF_MODE     SIF_MODE_1000
    #define SIF_CLK_INIT()    gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_BEGIN()   gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_CLK_END()     gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk)
    #define SIF_SEN_INIT()    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_BEGIN()   gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
    #define SIF_SEN_END()     gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen)
#else
    #error("This SIF Mode has not implement!");
#endif

static void DeviceInit(void)
{
    UINT32      i;
    SIF_CTRL    SIFCtrl;

    if(LCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        sif_open();
        // Max clock = 1000 / 100(ns) = 10MHz, uiClkDiv = (48 / (10 * 2)) - 1 = 1.4
        // Min uiClkDiv = 2, set uiClkDiv = 1, thus clock = 8MHz
        SIFCtrl.uiClkDiv      = 2;
        //#NT#2008/12/23#Niven Cho -begin
        //#NT#Refine,Use Macro
        SIFCtrl.uiMode        = USED_SIF_MODE;
        //#NT#2008/12/23#Niven Cho -end
        SIFCtrl.bSyncEn       = FALSE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSyncSrc     = SIF_SYNCSRC_LCD;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;
        sif_setControl((SIF_CH)LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }

    if(pMode->pCmdQueue!=NULL)
    {
        for(i=0;i<pMode->nCmd;i++)
        {
            sendtoLCDreg(pMode->pCmdQueue[i].Address,pMode->pCmdQueue[i].Value);
        }
    }
}

static void sendtoLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;

    //#NT#2008/12/23#Niven Cho -begin
    //#NT#Bug Fix,OR the bit of reg writing
    uiSIFData = (((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)|((UINT32)value<<SETTING_PANEL_REG_BIT_SHIFT_VALUE)|(1<<SETTING_PANEL_REG_BIT_WRITE)) << 16;
    //#NT#2008/12/23#Niven Cho -end

    sif_sendMSB((SIF_CH)LCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;

    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    SIF_SEN_INIT();
    SIF_CLK_INIT();
    gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

    //#NT#2008/12/23#Niven Cho -begin
    //#NT#Bug Fix,OR the bit of reg writing
    data = ((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS) | ((UINT32)value<<SETTING_PANEL_REG_BIT_SHIFT_VALUE) | (1<<SETTING_PANEL_REG_BIT_WRITE);
    //#NT#2008/12/23#Niven Cho -end

    delay(500);

    SIF_SEN_BEGIN();
    for(j = 16; j > 0; j--)
    {
        if(data & 0x8000)
            gpio_setPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(LCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(2);
        SIF_CLK_BEGIN();
        delay(8);
        SIF_CLK_END();
        delay(2);
        data <<= 1;
    }

    delay(500);
    SIF_SEN_END();
}

//#NT#2009/01/21#Chris Hsu -begin
//#NT#Add interface to let upper layer to send command to panel
void VideoController_SendCommand(UINT32 *puiSettingTable)
{
    while (((*puiSettingTable) & 0x0000FF00) != 0x0000FF00)
    {
        sendtoLCDreg(((*puiSettingTable) & 0x0000FF00) >> 8, (*puiSettingTable) & 0x000000FF);
        puiSettingTable++;
    }
}
//#NT#2009/01/21#Chris Hsu -end
//#NT#2008/12/23#Niven Cho -end

// add for compile issue
void LCD_Rotate(UINT32 bRotate)
{

}


