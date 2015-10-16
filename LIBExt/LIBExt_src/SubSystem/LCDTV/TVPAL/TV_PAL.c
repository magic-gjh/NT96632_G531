#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Pll.h"
#include "top.h"

#define TV_PAL_Buf_width               640
#define TV_PAL_Buf_height              528
#define TV_PAL_Win_width               640
#define TV_PAL_Win_height              528

//Different FSADJ need different configuration
//HW FSADJ 3.6 K
#define BURST_LEVEL_3_6                 0x2C
#define BLAND_LEVEL_3_6                 0x78
#define Y_SCALE_3_6                     0x89
#define CB_SCALE_3_6                    0x85
#define CR_SCALE_3_6                    0xBB

//HW FSADJ 3.3 K
#define BURST_LEVEL_3_3                 0x29
#define BLAND_LEVEL_3_3                 0x71
#define Y_SCALE_3_3                     0x7F
#define CB_SCALE_3_3                    0x7E
#define CR_SCALE_3_3                    0xB1

//HW FSADJ 3.0 K
#define BURST_LEVEL_3_0                 0x26
#define BLAND_LEVEL_3_0                 0x65
#define Y_SCALE_3_0                     0x75
#define CB_SCALE_3_0                    0x76
#define CR_SCALE_3_0                    0xA4



static TV_DEVICE_OBJ TVPAL_Obj = {270, LCD_SRCCLK_PLL2, TV_FSADJ_DEFAULT};

//#NT#2009/11/04#Klins Chen#[0007607] -begin
//#NT#Remove unnecessary dummy delay
//static void delay_pal(UINT32 times);
//static void delay_pal(UINT32 times)
//{
//    for ( ; times>0; times--)
//        ;
//}
//#NT#2009/11/04#Klins Chen#[0007607] -end

/**
    Init TV/PAL device object.

    Init TV/PAL device object for proper clock configuration.

    @param[in] ptvpalobj    Pointer to TV device object.

    @return void
*/
void InitTVPAL_OBJ(TV_DEVICE_OBJ *ptvpalobj)
{
    TVPAL_Obj = *ptvpalobj;
}

/**
    Open TV/PAL display.

    Open TV/PAL display.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

   @param[in]  psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

   @return  void
*/
void OpenTVPAL(LCDSIZE *psizeDisplay)
{
    //Different FSADJ need different configuration
    UINT8 uiBurstLevel = 0;
    UINT8 uiBlandLevel = 0;
    UINT8 uiYScale = 0, uiCbScale = 0, uiCrScale = 0;
    UINT8 uiClockDiv;


    if (TVPAL_Obj.bIDESrcPLL2 == LCD_SRCCLK_PLL2)
    {
        TVPAL_Obj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }
    else
    {
        TVPAL_Obj.uiIDESrcCLK = 240;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_240);
    }

    //#NT#2010/02/25#Klins Chen -begin
    //#NT#TV is internal HW engine, should not use pin out.
    pinmux_select_lcd(PINMUX_LCD_GPIO);
    //#NT#2010/02/25#Klins Chen -end

    //set to 27MHz
    uiClockDiv = (float)TVPAL_Obj.uiIDESrcCLK/(float)27/(float)2;
    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));


    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Wait for Power Stable
    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay_pal(0x10000);
    //#NT#2009/11/04#Klins Chen#[0007607] -end

    if(ide_open() != E_OK)
        return;

    ide_set_icst(0);
    ide_set_dithering(0, 0);
    ide_set_device(DISPLAY_DEVICE_TV);
    ide_set_pdir(0);
    ide_set_odd(0);
    ide_set_even(1);
    ide_set_hsinv(0);
    ide_set_vsinv(0);
    ide_set_hvldinv(0);
    ide_set_vvldinv(0);
    ide_set_clkinv(0);
    ide_set_fieldinv(0);
    ide_set_rgbdummy(0);
    ide_set_through_mode(0);
    ide_set_shw_st();

    ide_set_hor_timing(0x01, 0x6BF, 0x15C, 0x69B);
    ide_set_ver_timing(0x04, 0x270, 0x040, 0x267, 0x041, 0x268);
    ide_set_interlace(0x01);
    ide_set_startfield(0x01);

    ide_set_ctrst(0x40);
    ide_set_brt(0x00);
    ide_set_cmults(0x40);
    ide_set_cex(0x00);
    ide_set_standby_signal(0x00);

    ide_set_clamp(0x01);
    ide_set_tv_mode(0x01, 0x00, 0x01);

    //Different FSADJ need different configuration
    if(TVPAL_Obj.uiTVFSADJ == TV_FSADJ_3_6_K)
    {
        uiBurstLevel = BURST_LEVEL_3_6;
        uiBlandLevel = BLAND_LEVEL_3_6;
        uiYScale = Y_SCALE_3_6;
        uiCbScale = CB_SCALE_3_6;
        uiCrScale = CR_SCALE_3_6;
    }
    else if(TVPAL_Obj.uiTVFSADJ == TV_FSADJ_3_0_K)
    {
        uiBurstLevel = BURST_LEVEL_3_0;
        uiBlandLevel = BLAND_LEVEL_3_0;
        uiYScale = Y_SCALE_3_0;
        uiCbScale = CB_SCALE_3_0;
        uiCrScale = CR_SCALE_3_0;
    }
    else
    {
        uiBurstLevel = BURST_LEVEL_3_3;
        uiBlandLevel = BLAND_LEVEL_3_3;
        uiYScale = Y_SCALE_3_3;
        uiCbScale = CB_SCALE_3_3;
        uiCrScale = CR_SCALE_3_3;
    }

    ide_setTvSearchEn(TRUE);
    ide_setTvCkpnPol(TRUE);
    ide_setTvNtscSetupLevel(0x00);

    ide_set_tv_burstLevel(uiBurstLevel);
    ide_set_tv_blandLevel(uiBlandLevel);
    ide_set_tv_YCbCr_Scale(uiYScale, uiCbScale, uiCrScale);
    ide_set_tv_sela(0x3);

    ide_set_sif_startend(0x00, 0x00);

    ide_set_tv_amp(0x2D, 0x11, 0x11, 0x10);

    ide_set_tv_powerdown(FALSE);

    psizeDisplay->uiWidth       = TV_PAL_Buf_width;
    psizeDisplay->uiHeight      = TV_PAL_Buf_height;
    psizeDisplay->uiWinWidth    = TV_PAL_Win_width;
    psizeDisplay->uiWinHeight   = TV_PAL_Win_height;

    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay_pal(0x2000);
    //#NT#2009/11/04#Klins Chen#[0007607] -end

}

/**
    Close TV/PAL display.

    Call this api to close the TV/PAL device.

    @return void
*/
void CloseTVPAL(void)
{
    UINT32 i;
    // Disable IDE
    ide_set_en(FALSE);
    for (i=12; i>0; i--)
    {		
    ide_wait_frame_end();
    }
    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);

    ide_set_tv_powerdown(TRUE);
}
