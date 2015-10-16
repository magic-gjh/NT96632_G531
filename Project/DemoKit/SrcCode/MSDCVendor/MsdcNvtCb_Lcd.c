#include "OSCommon.h"
#include "SysCfg.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Int.h"
#include "MsdcNvtCb_Lcd.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "Utility.h"
#include "GlobalVar.h"
#include "CoordinateTrans.h"

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


typedef struct _MSDCEXT_LCD_CTRL{
    tLCD_PARAM*         pMode;          //!< Mode Setting Address
    tPANEL_CMD*         pCmdQueueClose; //!< Close Command Queue
    UINT32              nCmdClose;      //!< Number of Close Command
    LCD_DEVICE_OBJ      tLCD_DeviceObj; //!< LCD Device Object
    tLCD_DEFINE_VALUE   tDefValues;     //!< Real Driver Define Values
    void (*sif_clk_init)(UINT32 pin);   //!< Equal to SIF_CLK_INIT()
    void (*sif_clk_begin)(UINT32 pin);  //!< Equal to SIF_CLK_BEGIN()
    void (*sif_clk_end)(UINT32 pin);    //!< Equal to SIF_CLK_END()
    void (*sif_sen_init)(UINT32 pin);   //!< Equal to SIF_SEN_INIT()
    void (*sif_sen_begin)(UINT32 pin);  //!< Equal to SIF_SEN_BEGIN()
    void (*sif_sen_end)(UINT32 pin);    //!< Equal to SIF_SEN_END()    
}tMSDCEXT_LCD_CTRL;


//Declare Local Functions
static tMSDCEXT_LCD_CTRL* MsdcNvtCb_LcdGetCtrl(void);
static void delay(UINT uiUs);
static void sendtoLCDreg_sif(UINT8 addr, UINT8 value);
static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value);
static void sendtoLCDreg(UINT8 addr, UINT8 value);
static void DeviceInit(void);
static void DeviceOpen(LCDSIZE *psizeDisplay);
static void DeviceClose(void);

//static variable
static tMSDCEXT_LCD_CTRL gMsdcNvtLcdCtrl = {0};

//Function Implementation

static tMSDCEXT_LCD_CTRL* MsdcNvtCb_LcdGetCtrl(void)
{
    return &gMsdcNvtLcdCtrl;
}

void MsdcNvtCb_LcdGetFreeMem(void* pData)
{
    tMSDCEXT_LCD_DRV_MEM_POOL_GET* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_LCD_DRV_MEM_POOL_GET,pData);
    if(pDesc==NULL)
    {
        debug_err(("MsdcNvtCb_LcdGetFreeMem(): is failed!\r\n"));
        return;
    }
    get_blk((void *)&pDesc->PoolAddr,  POOL_ID_CAPTURE);
    rel_blk(POOL_ID_CAPTURE, (VP)pDesc->PoolAddr);
    pDesc->PoolSize = POOL_SIZE_CAPTURE;

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_LcdOpen(void* pData)
{
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();
    tMSDCEXT_LCD_DRV_OPEN* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_LCD_DRV_OPEN,pData);
    if(pDesc==NULL)
        return;

    pCtrl->pMode = pDesc->pParam;    
    pCtrl->tDefValues = pDesc->tDefValues;
    pCtrl->tLCD_DeviceObj = g_LCDCtrlObj;
    pCtrl->tLCD_DeviceObj.fIDEDstCLK = pCtrl->pMode->Panel.DCLK;

    switch(pDesc->tDefValues._SETTING_SIF_MODE)
    {
        case OPTION_SIF_0000:   //Confirmed (AUO,WinTek)
            pCtrl->sif_clk_init = gpio_setPin;
            pCtrl->sif_clk_begin = gpio_clearPin;
            pCtrl->sif_clk_end = gpio_setPin;
            pCtrl->sif_sen_init = gpio_setPin;
            pCtrl->sif_sen_begin = gpio_clearPin;
            pCtrl->sif_sen_end = gpio_setPin;
            break;
        case OPTION_SIF_0001:   //Not Confirmed
            pCtrl->sif_clk_init = gpio_clearPin;
            pCtrl->sif_clk_begin = gpio_setPin;
            pCtrl->sif_clk_end = gpio_clearPin;
            pCtrl->sif_sen_init = gpio_setPin;
            pCtrl->sif_sen_begin = gpio_clearPin;
            pCtrl->sif_sen_end = gpio_setPin;
            break;
        case OPTION_SIF_0010:   //Not Confirmed
            pCtrl->sif_clk_init = gpio_setPin;
            pCtrl->sif_clk_begin = gpio_clearPin;
            pCtrl->sif_clk_end = gpio_setPin;
            pCtrl->sif_sen_init = gpio_clearPin;
            pCtrl->sif_sen_begin = gpio_setPin;
            pCtrl->sif_sen_end = gpio_clearPin;
            break;
        case OPTION_SIF_0011:   //Not Confirmed
            pCtrl->sif_clk_init = gpio_clearPin;
            pCtrl->sif_clk_begin = gpio_setPin;
            pCtrl->sif_clk_end = gpio_clearPin;
            pCtrl->sif_sen_init = gpio_clearPin;
            pCtrl->sif_sen_begin = gpio_setPin;
            pCtrl->sif_sen_end = gpio_clearPin;
            break;
        case OPTION_SIF_0100:   //Confirmed (Toppoly)
            pCtrl->sif_clk_init = gpio_clearPin;
            pCtrl->sif_clk_begin = gpio_setPin;
            pCtrl->sif_clk_end = gpio_clearPin;
            pCtrl->sif_sen_init = gpio_setPin;
            pCtrl->sif_sen_begin = gpio_clearPin;
            pCtrl->sif_sen_end = gpio_setPin;
            break;
        case OPTION_SIF_0101:   //Not Confirmed
            pCtrl->sif_clk_init = gpio_setPin;
            pCtrl->sif_clk_begin = gpio_clearPin;
            pCtrl->sif_clk_end = gpio_setPin;
            pCtrl->sif_sen_init = gpio_setPin;
            pCtrl->sif_sen_begin = gpio_clearPin;
            pCtrl->sif_sen_end = gpio_setPin;
            break;
        case OPTION_SIF_0110:   //Not Confirmed
            pCtrl->sif_clk_init = gpio_clearPin;
            pCtrl->sif_clk_begin = gpio_setPin;
            pCtrl->sif_clk_end = gpio_clearPin;
            pCtrl->sif_sen_init = gpio_clearPin;
            pCtrl->sif_sen_begin = gpio_setPin;
            pCtrl->sif_sen_end = gpio_clearPin;
            break;
        case OPTION_SIF_0111:   //Not Confirmed
            pCtrl->sif_clk_init = gpio_setPin;
            pCtrl->sif_clk_begin = gpio_clearPin;
            pCtrl->sif_clk_end = gpio_setPin;
            pCtrl->sif_sen_init = gpio_clearPin;
            pCtrl->sif_sen_begin = gpio_setPin;
            pCtrl->sif_sen_end = gpio_clearPin;
            break;
        case OPTION_SIF_1000:   //Confirmed
            pCtrl->sif_clk_init = gpio_clearPin;
            pCtrl->sif_clk_begin = gpio_setPin;
            pCtrl->sif_clk_end = gpio_clearPin;
            pCtrl->sif_sen_init = gpio_setPin;
            pCtrl->sif_sen_begin = gpio_clearPin;
            pCtrl->sif_sen_end = gpio_setPin;
            break;
    }
    
    DeviceOpen(&pDesc->tLCDSize);
    
    g_LCDSize = pDesc->tLCDSize;
    //Coordinate Trans between OSD Size and Video Size are not the same, here for init
    CoordinateTrans_SetSizeOriginal(g_LCDSize.uiWidth,g_LCDSize.uiHeight);
    CoordinateTrans_SetSizeTarget(g_LCDSize.uiWidth,g_LCDSize.uiHeight);
    
    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_LcdClose(void* pData)
{
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();
    tMSDCEXT_LCD_DRV_CLOSE* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_LCD_DRV_CLOSE,pData);
    if(pDesc==NULL)
        return;

    pCtrl->pCmdQueueClose = pDesc->pCmdQueue;
    pCtrl->nCmdClose = pDesc->nCmd;
    DeviceClose();

    pDesc->tParent.bOK = TRUE;
}

void MsdcNvtCb_LcdShowImage(void* pData)
{    
    VIDEO_BUF_ADDR      FB_Addr = {0};
    VIDEO_BUF_ATTR      FB_attr = {0};
    VOSD_WINDOW_ATTR    FB_win  = {0};  

    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();
    tMSDCEXT_LCD_DRV_SHOW_IMG* pDesc = BEGIN_MSDC_NVT_CB(tMSDCEXT_LCD_DRV_SHOW_IMG,pData);
    
    if(pDesc==NULL)
        return;
            
    FB_Addr.B0_Y            = pDesc->AddrY;
    FB_Addr.B0_CB           = pDesc->AddrU;
    FB_Addr.B0_CR           = pDesc->AddrV;
    FB_Addr.uiBufSel        = IDE_VIDEO_ADDR_SEL_BUFFER0;

    FB_attr.Buf_W           = pDesc->Width;
    FB_attr.Buf_H           = pDesc->Height;
    FB_attr.Buf_Lineoffset  = pDesc->PitchY >> 2;
    FB_attr.Buf_L2R         = IDE_BUFFER_READ_L2R;
    FB_attr.Buf_T2B         = IDE_BUFFER_READ_T2B;
    FB_attr.Buf_BJmode      = IDE_VIDEO_BJMODE_BUFFER_REPEAT;

    FB_attr.Buf_Opt         = IDE_VIDEO_BUFFER_OPT_0;
    FB_attr.Buf_Num         = IDE_VIDEO_BUFFER_NUM_1;

    FB_win.Source_W         = pCtrl->pMode->Panel.BufferWidth;
    FB_win.Source_H         = pCtrl->pMode->Panel.BufferHeight;
    FB_win.Des_W            = pCtrl->pMode->Panel.WindowWidth;
    FB_win.Des_H            = pCtrl->pMode->Panel.WindowHeight;
    FB_win.Win_Format       = COLOR_YCBCR422;
    FB_win.Win_X            = 0;
    FB_win.Win_Y            = 0;
    FB_win.High_Addr        = 0;

    ide_set_video_buf_addr(IDE_VIDEOID_1, &FB_Addr);
    ide_set_video_buf_attr(IDE_VIDEOID_1, &FB_attr);
    ide_set_video_win_attr(IDE_VIDEOID_1, &FB_win);
    ide_set_load();    

    pDesc->tParent.bOK = TRUE;
}

static void delay(UINT uiUs)
{
    UINT32 i;

    // When CPU_CLK is 80Mhz, uiNs = 1 will do 1.057 us.
    // Use FCLKSEL_CLK80 as base.
    switch (pll_getClockRate(PLL_CLKSEL_CPU))
    {
    #ifdef PLL_CLKSEL_CPU_48
    case PLL_CLKSEL_CPU_48:
        uiUs = uiUs * 48 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_60
    case PLL_CLKSEL_CPU_60:
        uiUs = uiUs * 60 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_120
    case PLL_CLKSEL_CPU_120:
        uiUs = uiUs * 120 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_160
    case PLL_CLKSEL_CPU_160:
        uiUs = uiUs * 160 / 80;
        break;
    #endif

    #ifdef PLL_CLKSEL_CPU_240
    case PLL_CLKSEL_CPU_240:
        uiUs = uiUs * 240 / 80;
        break;
    #endif
    
    case PLL_CLKSEL_CPU_80:     // when uiUs is 1: delay 1.057 us
    default:
        // ratio: 1/1
        break;
    }

    for (i=((uiUs<<3)+(uiUs<<2)); i>0; i--)
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
static void sendtoLCDreg(UINT8 addr, UINT8 value)
{
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();
    if (pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
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
static void DeviceOpen(LCDSIZE *psizeDisplay)
{
    UINT8   uiClockDiv;
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();
    tIDE_PARAM*     pIDE = &pCtrl->pMode->IDE;
    tPANEL_PARAM*   pPanel = &pCtrl->pMode->Panel;

    if (ide_open() != E_OK)
    {
        return;
    }

    // Disable IDE    
    ide_set_en(FALSE);
    ide_wait_frame_end();

    // Set IDE Clock Source
    if (pCtrl->tLCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_240);
    }
    else
    {
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }

    // wait for Power Stable
    delay(10000);

    // Set IDE Register
    pinmux_select_lcd(pIDE->pinmux_select_lcd);
    // Set DCLK
    if(pPanel->bYCbCrFormat)
    {
        uiClockDiv = (UINT8)((float)pCtrl->tLCD_DeviceObj.uiIDESrcCLK/(float)pCtrl->tLCD_DeviceObj.fIDEDstCLK);
    }
    else
    {
        uiClockDiv = (UINT8)((float)pCtrl->tLCD_DeviceObj.uiIDESrcCLK/(float)pCtrl->tLCD_DeviceObj.fIDEDstCLK/(float)2);
    }

    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

    // Enable IDE Clock
    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Wait for Power Stable
    delay(10000);

    ide_set_icst(pIDE->icst);
    if(pPanel->bYCbCrFormat)
        ide_configIcst(FALSE, 0);
    else
        ide_configIcst(TRUE, CST_YCBCR2RGB);
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
            ,((pPanel->HSyncActivePeriod+pPanel->HSyncBackPorch)>>1)-1);
    }
    else
    {
        ide_set_hor_timing(0x00
            ,pPanel->HSyncTotalPeriod
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
        
    TimerDelayMs(33);
    ide_set_en(TRUE);    
    TimerDelayMs(33); //avoid vsync active area

    DeviceInit();
}

/**
  Close driver

  Release driver, private function for internal used

*/
static void DeviceClose(void)
{
    UINT32  i;
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();

    // Disable IDE
    ide_set_en(FALSE);

    for(i=0;i<pCtrl->nCmdClose;i++)
    {
        sendtoLCDreg(pCtrl->pCmdQueueClose[i].Address,pCtrl->pCmdQueueClose[i].Value);
    }

    delay(80000);

    if(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uimode != LCD_CTRL_MODE_GPIO)
    {
        sif_close();
    }

    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
}

static void DeviceInit(void)
{
    UINT32      i;
    SIF_CTRL    SIFCtrl;
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();
    UINT uiPinMuxSifs = PINMUX_SIF_CH0;

    switch(g_LCDCtrlObj.LCDCtrl_Pin.uisif_channel)
    {
        case SIF_CH0:
            uiPinMuxSifs = PINMUX_SIF_CH0;
            break;
        case SIF_CH1:
            uiPinMuxSifs = PINMUX_SIF_CH1;
            break;
        case SIF_CH2:
            uiPinMuxSifs = PINMUX_SIF_CH2;
            break;
        case SIF_CH3:
            uiPinMuxSifs = PINMUX_SIF_CH3;
            break;
    }    

    if(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uimode == LCD_CTRL_MODE_SIF)
    {
        pinmux_enable_sif(PINMUX_SIF_CH0|PINMUX_SIF_CH1|PINMUX_SIF_CH2|PINMUX_SIF_CH3);
                
        sif_open();
        // Max clock = 1000 / 320(ns) = 3.125MHz, uiClkDiv = (48 / (3.125 * 2)) - 1 = 6.68
        // Min uiClkDiv = 6.68, set uiClkDiv = 7, thus clock = 3MHz
        SIFCtrl.uiClkDiv      = 7;
        SIFCtrl.uiMode        = pCtrl->tDefValues._SETTING_SIF_MODE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;
        sif_setControl((SIF_CH)pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uisif_channel, &SIFCtrl);
    }
    else
    {
        pinmux_disable_sif(PINMUX_SIF_CH0|PINMUX_SIF_CH1|PINMUX_SIF_CH2|PINMUX_SIF_CH3);
    }

    if(pCtrl->pMode->pCmdQueue!=NULL)
    {
        for(i=0;i<pCtrl->pMode->nCmd;i++)
        {
            sendtoLCDreg(pCtrl->pMode->pCmdQueue[i].Address,pCtrl->pMode->pCmdQueue[i].Value);            
        }
    }
}

static void sendtoLCDreg_sif(UINT8 addr, UINT8 value)
{
    UINT32 uiSIFData;
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();

    uiSIFData = (((UINT32)addr << pCtrl->tDefValues._SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
        |((UINT32)value<<pCtrl->tDefValues._SETTING_PANEL_REG_BIT_SHIFT_VALUE)
        |(pCtrl->tDefValues._SETTING_PANEL_REG_MODE_WRITE<<pCtrl->tDefValues._SETTING_PANEL_REG_BIT_WRITE)) << 16;
    sif_sendMSB((SIF_CH)pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uisif_channel, uiSIFData);
}

static void sendtoLCDreg_gpio(UINT8 addr, UINT8 value)
{
    UINT32 data;
    UINT32 j;
    tMSDCEXT_LCD_CTRL* pCtrl = MsdcNvtCb_LcdGetCtrl();

    gpio_setDir(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_sen, GPIO_DIR_OUTPUT);
    gpio_setDir(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_clk, GPIO_DIR_OUTPUT);
    gpio_setDir(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_data, GPIO_DIR_OUTPUT);

    pCtrl->sif_sen_init(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    pCtrl->sif_clk_init(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
    gpio_setPin(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

    data = ((UINT32)addr << pCtrl->tDefValues._SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
        | ((UINT32)value<<pCtrl->tDefValues._SETTING_PANEL_REG_BIT_SHIFT_VALUE)
        | (pCtrl->tDefValues._SETTING_PANEL_REG_MODE_WRITE<<pCtrl->tDefValues._SETTING_PANEL_REG_BIT_WRITE);

    delay(2000);

    pCtrl->sif_sen_begin(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
    for(j = 16; j > 0; j--)
    {
        if(data & 0x8000)
            gpio_setPin(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_data);
        else
            gpio_clearPin(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_data);

        delay(400);
        pCtrl->sif_clk_begin(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(800);
        pCtrl->sif_clk_end(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_clk);
        delay(400);
        data <<= 1;
    }

    delay(2000);
    pCtrl->sif_sen_end(pCtrl->tLCD_DeviceObj.LCDCtrl_Pin.uigpio_sen);
}
