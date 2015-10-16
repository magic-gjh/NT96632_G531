/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SlideEffect.h
    @ingroup    mISYSPlaySS

    @brief      All function for Slide effect
                This module can be a plug-in to Application Playback

    @note       Nothing.

    @version    V0.00.001
    @author     Corey Ke
    @date       2006/12/04
*/

/** \addtogroup mISYSPlaySS */
//@{

#ifndef _SLIDEEFFECT_H
#define _SLIDEEFFECT_H

#include "Type.h"
#include "JpgDec.h"
#include "OSCommon.h"

//-------------------------------------------------------------------------

//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
#if 0
#define SLIDE_EFFECT_IMAGE_STEP                 8       // 8 step
#endif
#define SLIDE_EFFECT_IMAGE_DEFAULT_STEP         8       // 8 step
#define SLIDE_EFFECT_1X                         1
#define SLIDE_EFFECT_2X                         2
#define SLIDE_EFFECT_3X                         3
#define SLIDE_EFFECT_4X                         4
//#NT#2009/06/03#Corey Ke -end

#define SLIDE_EFFECT_NONE                       0x00    // No effect
#define SLIDE_EFFECT_UP2DOWN                    0x01    // Move image from up to down
#define SLIDE_EFFECT_DOWN2UP                    0x02    // Move image from down to up
#define SLIDE_EFFECT_LEFT2RIGHT                 0x03    // Move image from left to right
#define SLIDE_EFFECT_RIGHT2LEFT                 0x04    // Move image from right to left

#define SLIDE_EFFECT_RIGHTDOWN                  0x05    // Move image from left up to right down
#define SLIDE_EFFECT_LEFTDOWN                   0x06    // Move image from right up to left down
#define SLIDE_EFFECT_RIGHTUP                    0x07    // Move image from left down to right up
#define SLIDE_EFFECT_LEFTUP                     0x08    // Move image from right down to left up

#define SLIDE_EFFECT_R5HUP2DOWN                 0x09    // Move image from up to down sequence in 5 horizontal area
#define SLIDE_EFFECT_R5VLEFT2RIGHT              0x0A    // Move image from left to right sequence in 5 vertical area

#define SLIDE_EFFECT_IN2OUT                     0x0B    // Move image from in to out
#define SLIDE_EFFECT_OUT2IN                     0x0C    // Move image from out to in

#define SLIDE_EFFECT_R5VVERTICAL                0x0D    // Move image severence in 5 vertical area
#define SLIDE_EFFECT_R5HHORIZONTAL              0x0E    // Move image severence in 5 hotizontal area
#define SLIDE_EFFECT_R10VVERTICAL               0x0F    // Move image severence in 10 vertical area
#define SLIDE_EFFECT_R10HHORIZONTAL             0x10    // Move image severence in 10 hotizontal area

#define SLIDE_EFFECT_R5HDRAPEUP2DOWN            0x11    // Move image from up to down with drape effect in 5 horizontal area
#define SLIDE_EFFECT_R5VDRAPELEFT2RIGHT         0x12    // Move image from left to right with drape effect in 5 vertical area

#define SLIDE_EFFECT_BLOCKCHECKERLEFT2RIGHT     0x13    // Move image in several small square area

#define SLIDE_EFFECT_SLIDEINUP2DOWN             0x14    // Move image from up to down with slide in effect
#define SLIDE_EFFECT_SLIDEINDOWN2UP             0x15    // Move image from down to up with slide in effect
#define SLIDE_EFFECT_SLIDEINLEFT2RIGHT          0x16    // Move image from left to right with slide in effect
#define SLIDE_EFFECT_SLIDEINRIGHT2LEFT          0x17    // Move image from right to left with slide in effect

#define SLIDE_EFFECT_R5HSLIDEINUP2DOWN          0x18    // Move image from up to down sequence in 5 horizontal area with slide in effect
#define SLIDE_EFFECT_R5VSLIDEINLEFT2RIGHT       0x19    // Move image from left to right sequence in 5 vertical area with slide in effect

#define SLIDE_EFFECT_R5VSLIDEINVERTICAL         0x1A    // Move image severence in 5 vertical area with slide in effect
#define SLIDE_EFFECT_R5HSLIDEINHORIZONTAL       0x1B    // Move image severence in 5 hotizontal area with slide in effect
#define SLIDE_EFFECT_R10VSLIDEINVERTICAL        0x1C    // Move image severence in 10 vertical area with slide in effect
#define SLIDE_EFFECT_R10HSLIDEINHORIZONTAL      0x1D    // Move image severence in 10 hotizontal area with slide in effect

#define SLIDE_EFFECT_R2VSLIDEINOUT2IN           0x1E    // Move image from out to in in 2 vertical area with slide in effect
#define SLIDE_EFFECT_R2HSLIDEINOUT2IN           0x1F    // Move image from out to in in 2 horizontal area with slide in effect

#define SLIDE_EFFECT_PUSHUP2DOWN                0x20    // Move image from up to down with push effect
#define SLIDE_EFFECT_PUSHDOWN2UP                0x21    // Move image from down to up with push effect
#define SLIDE_EFFECT_PUSHLEFT2RIGHT             0x22    // Move image from left to right with push effect
#define SLIDE_EFFECT_PUSHRIGHT2LEFT             0x23    // Move image from right to left with push effect

#define SLIDE_EFFECT_R5HPUSHUP2DOWN             0x24    // Move image from up to down sequence in 5 horizontal area with push effect
#define SLIDE_EFFECT_R5VPUSHLEFT2RIGHT          0x25    // Move image from left to right sequence in 5 vertical area with push effect

#define SLIDE_EFFECT_R5VPUSHVERTICAL            0x26    // Move image severence in 5 vertical area with push effect
#define SLIDE_EFFECT_R5HPUSHHORIZONTAL          0x27    // Move image severence in 5 hotizontal area with push effect
#define SLIDE_EFFECT_R10VPUSHVERTICAL           0x28    // Move image severence in 10 vertical area with push effect
#define SLIDE_EFFECT_R10HPUSHHORIZONTAL         0x29    // Move image severence in 10 hotizontal area with push effect

#define SLIDE_EFFECT_ROLLUP2DOWN                0x2A    // Move image from up to down with roll effect
#define SLIDE_EFFECT_ROLLDOWN2UP                0x2B    // Move image from down to up with roll effect
#define SLIDE_EFFECT_ROLLLEFT2RIGHT             0x2C    // Move image from left to right with roll effect
#define SLIDE_EFFECT_ROLLRIGHT2LEFT             0x2D    // Move image from right to left with roll effect

#define SLIDE_EFFECT_R5HFLYINUP2DOWN            0x2E    // Move image from up to down with fly in effect in 5 horizontal area
#define SLIDE_EFFECT_R5VFLYINLEFT2RIGHT         0x2F    // Move image from left to right with fly in effect in 5 vertical area

#define SLIDE_EFFECT_FADE                       0x30    // Move image with fade effect
#define SLIDE_EFFECT_FADEBLACK                  0x31    // Move image with fade and fade black effect
#define SLIDE_EFFECT_FADEWHITE                  0x32    // Move image with fade and fade white effect
#define SLIDE_EFFECT_FADEFOCUS                  0x33    // Move image with fade focus

#define SLIDE_EFFECT_R5VFADEVERTICAL            0x34    // Move image severence in 5 vertical area with fade effect
#define SLIDE_EFFECT_R5HFADEHORIZONTAL          0x35    // Move image severence in 5 hotizontal area with fade effect
#define SLIDE_EFFECT_R10VFADEVERTICAL           0x36    // Move image severence in 10 vertical area with fade effect
#define SLIDE_EFFECT_R10HFADEHORIZONTAL         0x37    // Move image severence in 10 hotizontal area with fade effect

#define SLIDE_EFFECT_FADEIN2OUT                 0x38    // Move image from in to out with fade effect
#define SLIDE_EFFECT_FADEOUT2IN                 0x39    // Move image from out to in with fade effect
#define SLIDE_EFFECT_R2VSLIDEFADEOUT2IN         0x3A    // Move image from out to in with fade & slide in effect in 2 vertical area
#define SLIDE_EFFECT_R2HSLIDEFADEOUT2IN         0x3B    // Move image from out to in with fade & slide in effect in 2 hotizontal area

#define SLIDE_EFFECT_PUSHFADEUP2DOWN            0x3C    // Move image from up to down with push & fade effect
#define SLIDE_EFFECT_PUSHFADEDOWN2UP            0x3D    // Move image from down to up with push & fadeeffect
#define SLIDE_EFFECT_PUSHFADELEFT2RIGHT         0x3E    // Move image from left to right with push & fade effect
#define SLIDE_EFFECT_PUSHFADERIGHT2LEFT         0x3F    // Move image from right to left with push & fade effect

#define SLIDE_EFFECT_FADEBLOCKCHECKER           0x40    // Move image in several small square area with fade effect

#define SLIDE_EFFECT_FADEDIAMONDIN2OUT          0x41    // Move image from in to out in several small square area with fade effect
#define SLIDE_EFFECT_FADEBLACKDIAMONDIN2OUT     0x42    // Move image from in to out in several small square area with fade black effect
#define SLIDE_EFFECT_FADEWHITEDIAMONDIN2OUT     0x43    // Move image from in to out in several small square area with fade white effect

#define SLIDE_EFFECT_FADECROSSIN2OUT            0x44    // Move image from in to out in several small square area with fade effect in cross
#define SLIDE_EFFECT_FADEBLACKCROSSIN2OUT       0x45    // Move image from in to out in several small square area with fade black effect in cross
#define SLIDE_EFFECT_FADEWHITECROSSIN2OUT       0x46    // Move image from in to out in several small square area with fade white effect in cross

#define SLIDE_EFFECT_FADEWHITELINELEFTUP        0x47    // Move image to left up in several small square area with fade white effect
#define SLIDE_EFFECT_FADEBLACKLINERIGHTUP       0x48    // Move image to right up in several small square area with fade black effect

#define SLIDE_EFFECT_POPOUT                     0x49    // Move image with popout effect
#define SLIDE_EFFECT_POPOUTBLOCK                0x4A    // Move image with popout block effect

#define SLIDE_EFFECT_FLIP                       0x4B    // Move image with popout flip effect
#define SLIDE_EFFECT_POPFLIP                    0x4C    // Move image with popout pop-flip effect

//#NT#2007/04/23#Corey Ke -begin
//#Add slide effect saw right down, right up, left down, and left up
#define SLIDE_EFFECT_SAWRIGHTDOWN               0x4D    // Move image from left up to right down with saw effect
#define SLIDE_EFFECT_SAWLEFTDOWN                0x4E    // Move image from right up to left down with saw effect
#define SLIDE_EFFECT_SAWRIGHTUP                 0x4F    // Move image from left down to right up with saw effect
#define SLIDE_EFFECT_SAWLEFTUP                  0x50    // Move image from right down to left up with saw effect
//#NT#2007/04/23#Corey Ke -end

#define SLIDE_EFFECT_RANDOM                     0x51    // Move image with all above effect in sequence

#define SLIDE_EFFECT_FIRST                      0x01
#define SLIDE_EFFECT_LAST                       SLIDE_EFFECT_RANDOM - 1

#define SLIDE_EFFECT_MAX                        SLIDE_EFFECT_RANDOM

#define SLIDE_EFFECT_OK                         0
#define SLIDE_EFFECT_EMPTY                      1

//-----------------------------------------------------------------------------------

typedef struct _SLIDE_DISPLAY_OBJ
{
    UINT32      FSrcBufAddrY;              // the frame-buffer-addr-Y  of this display device
    UINT32      FSrcBufAddrCb;             // the frame-buffer-addr-Y  of this display device
    UINT32      FSrcBufAddrCr;             // the frame-buffer-addr-Y  of this display device
    UINT32      FDstBufAddrY;              // the frame-buffer-addr-Y  of this display device
    UINT32      FDstBufAddrCb;             // the frame-buffer-addr-Y  of this display device
    UINT32      FDstBufAddrCr;             // the frame-buffer-addr-Y  of this display device
    UINT32      FTmpBuf1AddrY;             // the frame-buffer-addr-Y  of this display device
    UINT32      FTmpBuf2AddrY;             // the frame-buffer-addr-Y  of this display device
    UINT32      FTmpBuf3AddrY;             // the frame-buffer-addr-Y  of this display device
} SLIDE_DISPLAY_OBJ, *PSLIDE_DISPLAY_OBJ;


typedef struct _SLIDE_EFFECT_OBJ{
    UINT32  SlideEffectIndex;
    INT32 (*SlideEffectFunc)(UINT32  ImgWidth_Y, UINT32 ImgHeight_Y, UINT32 FileFormat, SLIDE_DISPLAY_OBJ buf, UINT32 DelayTime);
    INT8* SlideEffectName;
} SLIDE_EFFECT_OBJ,*PSLIDE_EFFECT_OBJ;//SensorLibraryNeedInfo,*pSensorLibraryNeedInfo;


//-----------------------------------------------------------------------------------

extern SLIDE_EFFECT_OBJ g_SlideEffectTable[];
//#NT#2009/06/03#Corey Ke -begin
//#NT#fine tune slide show performance
extern void  SE_SetSpeed(UINT32 uiSpeed);
//#NT#2009/06/03#Corey Ke -end

//-----------------------------------------------------------------------------------

#endif

//@}
