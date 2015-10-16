/**
    Graphic module driver

    This module driver use 3 types of state(IDLE, READY, RUN)
\n  IDLE: Driver is not opened, change register setting is not allowed.
\n  READY: driver is open and is ready for using, change register setting is allowed.
\n  RUN: graphic engine is running, change register setting is not allowed.

    @file       grph.h
    @ingroup    mIGraph
    @note
\n              - grph_open() to start this module, hook the ISR.
\n              - grph_close() to close this module, and un-hook the ISR.
\n
\n              This module will enable and wait the system interrupt(_INT_GRAPHY).
\n
\n              - grph_isr() will be called when Graph interrupt is on.
\n
\n              - grph_setImg1, grph_setImg2, grph_setImg3. set parameters of images.
\n
\n              - grph_setAOP, grph_setAOP2, grph_setGOP, grph_setGOP2 to set either operation.
\n                and engine will start at the end of these function.
\n
\n              Usage example:
\n
\n              (1)grph_open();     // Open graphic engine for access
\n              (2)grph_setImg1();  // Setup Image-1 buffer information
\n                 grph_setImg2();  // Setup Image-2 buffer information
\n                 grph_setGOP();   // Start Geometric Operation.
\n
\n                 grph_setImg1();  // Setup Image-1 buffer information
\n                 grph_setImg2();  // Setup Image-2 buffer information
\n                 grph_setAOP();   // Start Arithmetic Operation.
\n
\n                 grph_setImg1();  // Setup Image-1 buffer information
\n                 grph_setImg2();  // Setup Image-2 buffer information
\n                 grph_setGOP2();  // Start Geometric Operation.
\n
\n                 grph_setImg1();  // Setup Image-1 buffer information
\n                 grph_setImg2();  // Setup Image-2 buffer information
\n                 grph_setImg3();  // Setup Image-3 buffer information
\n                 grph_setAOP2();  // Start Arithmetic Operation.
\n                      :
\n                      :
\n                      :
\n              (3)grph_close();

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef   _GRPH_H
#define   _GRPH_H


/** \addtogroup mIGraph */
//@{

/**
    Destination output selection

    Select which destination output for graphic engine operation result.
\n  Used for grph_setGOP()/ grph_setGOP2()/ grph_setAOP()
*/
//@{
typedef enum
{
    GRPH_DST_1 = 0x00,              ///< Destination use image 1
    GRPH_DST_2 = 0x20               ///< Destination use image 2
}GRPH_DST_SEL;
//@}

/**
    Destination bitskip selection

    Select which destination bitskip selection for graphic engine 10/12 bits operation.
\n  Used for grph_setAOP2().
*/
//@{
typedef enum
{
    GRPH_DST_BITSKIP1 = 0x00,      ///< Destination use bitskip 1
    GRPH_DST_BITSKIP2 = 0x20       ///< Destination use bitskip 2
}GRPH_DST_BITSKIP_SEL;
//@}

/**
    Geometric operation mode

    Select which Geometric operation mode for graphic engine operation.
\n  Used for grph_setGOP()/ grph_setGOP2().
*/
//@{
typedef enum
{
    GRPH_GOP_ROT_90,            //0x00        ///< Rotate 90 degree(Clockwise)
    GRPH_GOP_ROT_270,           //0x01        ///< Rotate 270 degree(Clockwise)
    GRPH_GOP_ROT_180,           //0x02        ///< Rotate 180 degree
    GRPH_GOP_HRZ_FLIP,          //0x03        ///< Horizontal flip
    GRPH_GOP_VTC_FLIP,          //0x04        ///< Vertical flip
    GRPH_GOP_HRZ_CFLT,          //0x05        ///< Horizontal chroma filter, Must use grph_setGOP2()
    GRPH_GOP_VTC_CFLT           //0x06        ///< Vertical chroma filter, Must use grph_setGOP2()

}GRPH_GOP_MODE;
//@}

/**
    Arithmetric operation control

    Select which Arithmetric operation control mode for graphic engine operation.
\n  Used for grph_setAOP()/ grph_setAOP2().
*/
//@{
typedef enum
{
    GRPH_AOP_A_COPY,            //0x00        ///< A -> destination
    GRPH_AOP_PLUS_SHF,          //0x01        ///< (A+(B>>SHF[1:0])) -> destination
    GRPH_AOP_MINUS_SHF,         //0x02        ///< (A-(B>>SHF[1:0])) -> destination
    GRPH_AOP_COLOR_EQ,          //0x03        ///< A -> destination with color key (=)
    GRPH_AOP_COLOR_LE,          //0x04        ///< A -> destination with color key (<)
    GRPH_AOP_A_AND_B,           //0x05        ///< A & B -> destination
    GRPH_AOP_A_OR_B,            //0x06        ///< A | B -> destination
    GRPH_AOP_A_XOR_B,           //0x07        ///< A ^ B -> destination
    GRPH_AOP_TEXT_COPY,         //0x08        ///< text -> destination
    GRPH_AOP_TEXT_AND_A,        //0x09        ///< text & A -> destination
    GRPH_AOP_TEXT_OR_A,         //0x0A        ///< text | A -> destination
    GRPH_AOP_TEXT_XOR_A,        //0x0B        ///< text ^ A -> destination
    GRPH_AOP_TEXT_AND_AB,       //0x0C        ///< (text & A) | (~text & B) -> destination
    GRPH_AOP_BLENDING,          //0x0D        ///< (A * WA) + (B * WB) -> destination
    GRPH_AOP_ACC,               //0x0E        ///< Acc(A):256 pixel accumulation
    GRPH_AOP_MULTIPLY_DIV,      //0x0F        ///< (A * B)>>DIV[2:0] -> destination, Must use grph_setAOP2()
    GRPH_AOP_PACKING,           //0x10        ///< Pack(A,B)->C, Must use grph_setAOP2()
    GRPH_AOP_DEPACKING,         //0x11        ///< Un-Pack(A)->(B,C), Must use grph_setAOP2()


    GRPH_AOP_MINUS_SHF_ABS=0x80,//0x80        ///< abs(A-(B>>SHF[1:0])) -> destination
    GRPH_AOP_COLOR_MR           //0x81        ///< A -> destination with color key (>)
}GRPH_AOP_MODE;
//@}

/**
    Blending control

    Select which blending control weighting for graphic engine operation.
\n  Used for grph_setAOP(). Please refer to grph_setAOP() for usage.

    @name   Blending control
*/
//@{
#define GRPH_BLD_WA_PER_2           0x01  ///< Weighting-A Value 1/2
#define GRPH_BLD_WA_PER_4           0x02  ///< Weighting-A Value 1/4
#define GRPH_BLD_WA_PER_8           0x04  ///< Weighting-A Value 1/8
#define GRPH_BLD_WA_PER_16          0x08  ///< Weighting-A Value 1/16

#define GRPH_BLD_WB_PER_2           0x10  ///< Weighting-B Value 1/2
#define GRPH_BLD_WB_PER_4           0x20  ///< Weighting-B Value 1/4
#define GRPH_BLD_WB_PER_8           0x40  ///< Weighting-B Value 1/8
#define GRPH_BLD_WB_PER_16          0x80  ///< Weighting-B Value 1/16
//@}

/**
    Advanced blending control

    Select which blending control weighting & blending threshold for graphic engine operation.
\n  Used for grph_setAOP2(). Please refer to grph_setAOP2() for usage.

    @name   Advanced blending control
*/
//@{
#define GRPH_BLD_WA_WB_THR(WA,WB,THR)   ((WA&0xFF)|((WB&0xFF)<<8)|((THR&0xFF)<<16))  ///< Weighting is (WA/256) & (WB/256).
//@}

/**
    Chroma filter control

    Config wChroma filter control for graphic engine operation.
\n  For GOP = 5,6: property = FLTSIZE[25..24], FLTCOEF[20..16], CVMDELTA[12..8] and CVMCENTER[7..0]
\n  Example: Use GRPH_CFLT_PARAM(FLTSIZE, FLTCOEF, CVMDELTA, CVMCENTER) to set the chroma filter parameters.

    @name   Chroma filter control
*/
//@{
#define GRPH_CFLT_FLTSIZE_2         0x00    ///< Chroma filter tap size 2.
#define GRPH_CFLT_FLTSIZE_4         0x01    ///< Chroma filter tap size 4.
#define GRPH_CFLT_FLTSIZE_8         0x02    ///< Chroma filter tap size 8.
#define GRPH_CFLT_FLTSIZE_16        0x03    ///< Chroma filter tap size 16.

#define GRPH_CFLT_FLTCOEF_MAX       0x1F
#define GRPH_CFLT_CVMDELTA_MAX      0x1F
#define GRPH_CFLT_CVMCENTER_MAX     0xFF

#define GRPH_CFLT_PARAM(FLTSIZE, FLTCOEF, CVMDELTA, CVMCENTER)  \
(((FLTSIZE & 0x03)<<24) | ((FLTCOEF & GRPH_CFLT_FLTCOEF_MAX)<<16) | ((CVMDELTA & GRPH_CFLT_CVMDELTA_MAX)<<8) | (CVMCENTER & GRPH_CFLT_CVMCENTER_MAX))  ///< Chroma filter paramter setting definition
//@}


/**
    Graphic engine API return code (ER type)

    Return operation result for graphic engine operation.
*/
//@{
typedef enum
{
    GRPH_SUCCESS,                       ///< Operation is done and success.
    GRPH_ER_ENGINE_NOT_READY,           ///< Not ready for usage, maybe is running or not opened.
    GRPH_ER_ENGINE_NOT_SUPPORT          ///< Not valid input parameters / Engine no support.

}GRPH_ERROR;
//@}


/**
    Accumulation control

    Accumulation skip control, used for grph_setAccCtl().
*/
//@{
typedef enum
{
    GRPH_ACC_SKIP_NONE,                 ///< Accumulation none skip.
    GRPH_ACC_SKIP_ODD,                  ///< Accumulation odd skip.
    GRPH_ACC_SKIP_EVEN                  ///< Accumulation even skip.

}GRPH_ACC_SKIP_CTL;
//@}

#define grph_swReset()
#define grph_enableDMA()
#define grph_disableDMA()


extern ER       grph_open(void);
extern BOOL     grph_isOpened(void);
extern ER       grph_close(void);
extern ER       grph_init(UINT32 uiClkRate);

extern ER       grph_setImg1(UINT32 uiAddr, UINT32 uiLnOff, UINT32 uiLnCnt, UINT32 uiXRgn);
extern ER       grph_setImg2(UINT32 uiAddr, UINT32 uiLnOff);
extern ER       grph_setImg3(UINT32 uiAddr, UINT32 uiLnOff);
extern ER       grph_setAccCtl(GRPH_ACC_SKIP_CTL uiSkipCtl);
extern ER       grph_getAccResult(BOOL *bCntFull, UINT8 *uiPixCnt, UINT32 *uiVldPixCnt, UINT32 *uiAccResult);
extern ER       grph_setGOP(BOOL b16BitPrc, GRPH_DST_SEL dstSel, BOOL bByteSwap, GRPH_GOP_MODE gopMode);
extern ER       grph_setGOP2(BOOL b16BitPrc, GRPH_DST_SEL dstSel, GRPH_GOP_MODE gopMode, UINT32 uiProperty);
extern ER       grph_setAOP(BOOL b16BitPrc, GRPH_DST_SEL dstSel, GRPH_AOP_MODE aopMode, UINT32 uiProperty);
extern ER       grph_setAOP2(UINT8 BitPrc, GRPH_DST_BITSKIP_SEL dstSel, GRPH_AOP_MODE aopMode, UINT32 uiProperty,UINT32 bitskip1, UINT32 bitskip2);

//@}

#endif
