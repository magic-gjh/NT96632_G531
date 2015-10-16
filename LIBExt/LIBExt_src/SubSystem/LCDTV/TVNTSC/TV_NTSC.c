#include "Kernel.h"
#include "IDE.h"
#include "LCDTV.h"
#include "Pll.h"
#include "top.h"

#define TV_NTSC_Buf_width              640
#define TV_NTSC_Buf_height             448
#define TV_NTSC_Win_width              640
#define TV_NTSC_Win_height             448

//Different FSADJ need different configuration
//HW FSADJ 3.6 K
#define BURST_LEVEL_3_6                 0x3B
#define BLAND_LEVEL_3_6                 0x71
#define Y_SCALE_3_6                     0x8A
#define CB_SCALE_3_6                    0x80
#define CR_SCALE_3_6                    0xAC

//HW FSADJ 3.3 K
#define BURST_LEVEL_3_3                 0x39
#define BLAND_LEVEL_3_3                 0x69
#define Y_SCALE_3_3                     0x83
#define CB_SCALE_3_3                    0x78
#define CR_SCALE_3_3                    0xA5

//HW FSADJ 3.0 K
#define BURST_LEVEL_3_0                 0x33
#define BLAND_LEVEL_3_0                 0x60
#define Y_SCALE_3_0                     0x76
#define CB_SCALE_3_0                    0x72
#define CR_SCALE_3_0                    0x9A




static TV_DEVICE_OBJ TVNTSC_Obj = {270, LCD_SRCCLK_PLL2, TV_FSADJ_DEFAULT};

//#NT#2009/11/04#Klins Chen#[0007607] -begin
//#NT#Remove unnecessary dummy delay
//static void delay_ntsc(UINT32 times);
//static void delay_ntsc(UINT32 times)
//{
//    for ( ; times>0; times--)
//        ;
//}
//#NT#2009/11/04#Klins Chen#[0007607] -end

/**
    Init TV/NTSC device object.

    Init TV/NTSC device object for proper clock configuration.

    @param[in] ptvntscobj   Pointer to TV device object.

    @return void
*/
void InitTVNTSC_OBJ(TV_DEVICE_OBJ *ptvntscobj)
{
    TVNTSC_Obj = *ptvntscobj;
}

/**
    Open TV/NTSC display.

    Open TV/NTSC display.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

   @param[in]   psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

   @return  void
*/
void OpenTVNTSC(LCDSIZE *psizeDisplay)
{
    //Different FSADJ need different configuration
    UINT8 uiBurstLevel = 0;
    UINT8 uiBlandLevel = 0;
    UINT8 uiYScale = 0, uiCbScale = 0, uiCrScale = 0;
    UINT8 uiClockDiv;


    if (TVNTSC_Obj.bIDESrcPLL2 == LCD_SRCCLK_PLL2)
    {
        TVNTSC_Obj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
        pll_setPLL2En(TRUE);
    }
    else
    {
        TVNTSC_Obj.uiIDESrcCLK = 240;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_240);
    }


    //#NT#2010/02/25#Klins Chen -begin
    //#NT#TV is internal HW engine, should not use pin out.
    pinmux_select_lcd(PINMUX_LCD_GPIO);
    //#NT#2010/02/25#Klins Chen -end

    //set to 27MHz
    uiClockDiv = (float)TVNTSC_Obj.uiIDESrcCLK/(float)27/(float)2;
    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));


    pll_enableClock(IDE1_CLK);
    pll_enableClock(IDE2_CLK);

    // Wait for Power Stable
    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay_ntsc(0x10000);
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

    ide_set_hor_timing(0x7F, 0x6B3, 0x144, 0x69D);
    ide_set_ver_timing(0x05, 0x20C, 0x32, 0x205, 0x33, 0x206);
    ide_set_interlace(0x01);
    ide_set_startfield(0x01);

    ide_set_ctrst(0x40);
    ide_set_brt(0x00);
    ide_set_cmults(0x40);
    ide_set_cex(0x00);
    ide_set_standby_signal(0x00);

    ide_set_clamp(0x01);
    ide_set_tv_mode(0x00, 0x00, 0x01);

    //Different FSADJ need different configuration
    if(TVNTSC_Obj.uiTVFSADJ == TV_FSADJ_3_6_K)
    {
        uiBurstLevel = BURST_LEVEL_3_6;
        uiBlandLevel = BLAND_LEVEL_3_6;
        uiYScale = Y_SCALE_3_6;
        uiCbScale = CB_SCALE_3_6;
        uiCrScale = CR_SCALE_3_6;
    }
    else if(TVNTSC_Obj.uiTVFSADJ == TV_FSADJ_3_0_K)
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
    ide_setTvNtscSetupLevel(0x24);

    ide_set_tv_burstLevel(uiBurstLevel);
    ide_set_tv_blandLevel(uiBlandLevel);
    ide_set_tv_YCbCr_Scale(uiYScale, uiCbScale, uiCrScale);
    ide_set_tv_sela(0x3);
    ide_set_sif_startend(0x00, 0x00);

    ide_set_tv_amp(0x2D, 0x10, 0x10, 0x10);

    ide_set_tv_powerdown(FALSE);

    psizeDisplay->uiWidth       = TV_NTSC_Buf_width;
    psizeDisplay->uiHeight      = TV_NTSC_Buf_height;
    psizeDisplay->uiWinWidth    = TV_NTSC_Win_width;
    psizeDisplay->uiWinHeight   = TV_NTSC_Win_height;

    //#NT#2009/11/04#Klins Chen#[0007607] -begin
    //#NT#Remove unnecessary dummy delay
    //delay_ntsc(0x2000);
    //#NT#2009/11/04#Klins Chen#[0007607] -end
}

/**
    Close TV/NTSC display.

    Call this api to close the TV/NTSC device.

    @return void
*/
void CloseTVNTSC(void)
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
