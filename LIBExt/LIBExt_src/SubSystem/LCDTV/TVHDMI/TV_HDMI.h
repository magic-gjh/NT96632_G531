/*
    TV HDMI TX module internal header file

    TV HDMI TX module internal header file

    @file       TV_HDMI.h
    @ingroup    mIDISPHDMITx
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


#ifndef _TVHDMI_H
#define _TVHDMI_H

#include "hdmitx.h"

#define HDMI_TX_INT_STS             (0x1<<0)
#define HDMI_CEC_INT_STS            (0x1<<1)

#define ACT_HIGH                    0
#define ACT_LOW                     1


typedef struct
{
    HDMI_VIDEOID    VID;                ///< Video ID

    BOOL            HS;                 ///< H-Sync Active edge
    BOOL            VS;                 ///< V-Sync Active edge
    BOOL            HVLD;               ///< H-Valid Active edge
    BOOL            VVLD;               ///< V-Valid Active edge
    BOOL            CLK;                ///< Pixel-Clock Active edge
    BOOL            DE;                 ///< Data Enable Active edge
    BOOL            PROGRESSIVE;        ///< Progressive/Interlaced Selection

    UINT32          H_SYNCTIME;         ///< H-Sync Time
    UINT32          H_TOTAL;            ///< H-Total Time
    UINT32          H_STARTTIME;        ///< H-Start Time
    UINT32          H_ENDTIME;          ///< H-End Time

    UINT32          V_SYNCTIME;         ///< V-Sync Time
    UINT32          V_TOTAL;            ///< V-Total Time
    UINT32          V_ODD_STARTTIME;    ///< V-Odd field Start Time
    UINT32          V_ODD_ENDTIME;      ///< V-Odd field End Time
    UINT32          V_EVEN_STARTTIME;   ///< V-Even field Start Time
    UINT32          V_EVEN_ENDTIME;     ///< V-Even field End Time

    UINT32          WINX_OFS;           ///< IDE Window Offset X when TV overscan
    UINT32          WINY_OFS;           ///< IDE Window Offset Y when TV overscan
}TVHDMI_TIMING,*PTVHDMI_TIMING;




//
//  HDMI Specific PLL2 frequency
//
#define HDMI_108_PLL2_LOOPDIV       0x12    // 108 = 12*18/2/1
#define HDMI_108_PLL2_PREDIV        0x01

#define HDMI_144_PLL2_LOOPDIV       0x18    // 144 = 12*24/2/1
#define HDMI_144_PLL2_PREDIV        0x01

#define HDMI_216_PLL2_LOOPDIV       0x24    // 216 = 12*36/2/1
#define HDMI_216_PLL2_PREDIV        0x01

#define HDMI_252_PLL2_LOOPDIV       0x2A    // 252 = 12*42/2/1
#define HDMI_252_PLL2_PREDIV        0x01

#define HDMI_297_PLL2_LOOPDIV       0x63    // 297 = 12*99/2/2
#define HDMI_297_PLL2_PREDIV        0x02



/*
    HDMI Video detail format
*/
//@{
typedef struct _VID_FORMAT_
{
    /* Timing */
    BOOL    HS;
    BOOL    VS;
    BOOL    HVLD;
    BOOL    VVLD;
    BOOL    CLK;
    BOOL    DE;
    BOOL    PROGRESSIVE;
    UINT16  WIDTH;
    UINT16  HEIGHT;
    UINT16  H_SYNC_TIME;
    UINT16  H_PERIOD_TIME;
    UINT16  H_VALID_START_TIME;
    UINT16  H_VALID_END_TIME;
    UINT16  V_SYNC_TIME;
    UINT16  V_PERIOD_TIME;
    UINT16  V_ODD_VALID_START_TIME;
    UINT16  V_ODD_VALID_END_TIME;
    UINT16  V_EVEN_VALID_START_TIME;
    UINT16  V_EVEN_VALID_END_TIME;

    /* Display mode */
    HDMI_MODE       DISP_DEV;
    HDMI_VIDEOID    VID;
}VID_FORMAT,*pVID_FORMAT;
//@}



#if 0
/* VID1 640X480P @59.94/60Hz */
#define VID1_HS                     ACTIVE_LOW
#define VID1_VS                     ACTIVE_LOW
#define VID1_HVLD                   ACTIVE_HIGH
#define VID1_VVLD                   ACTIVE_HIGH
#define VID1_CLK                    ACTIVE_LOW /* positive edge */
#define VID1_DE                     ACTIVE_HIGH
#define VID1_PROGRESSIVE            TRUE

#define VID1_DTV_HDMI_width         (640)
#define VID1_DTV_HDMI_height        (480)

#define VID1_H_SYNC_TIME            (96)
#define VID1_H_PERIOD_TIME          (800)
#define VID1_H_VALID_START_TIME     (VID1_H_SYNC_TIME+48)
#define VID1_H_VALID_END_TIME       (VID1_H_VALID_START_TIME+VID1_DTV_HDMI_width)

#define VID1_V_SYNC_TIME            (2)
#define VID1_V_PERIOD_TIME          (VID1_DTV_HDMI_height+45)
#define VID1_V_ODD_VALID_START_TIME (VID1_V_SYNC_TIME+33)
#define VID1_V_ODD_VALID_END_TIME   (VID1_V_ODD_VALID_START_TIME+VID1_DTV_HDMI_height)
#define VID1_V_EVEN_VALID_START_TIME (VID1_V_SYNC_TIME+33)
#define VID1_V_EVEN_VALID_END_TIME  (VID1_V_EVEN_VALID_START_TIME+VID1_DTV_HDMI_height)

/* VID2/3 720X480P @59.94/60Hz */
#define VID2_HS                     ACTIVE_LOW
#define VID2_VS                     ACTIVE_LOW
#define VID2_HVLD                   ACTIVE_HIGH
#define VID2_VVLD                   ACTIVE_HIGH
#define VID2_CLK                    ACTIVE_LOW /* positive edge */
#define VID2_DE                     ACTIVE_HIGH
#define VID2_PROGRESSIVE            TRUE

#define VID2_DTV_HDMI_width         (720)
#define VID2_DTV_HDMI_height        (480)

#define VID2_H_SYNC_TIME            (62)
#define VID2_H_PERIOD_TIME          (720+138)
#define VID2_H_VALID_START_TIME     (VID2_H_SYNC_TIME+60)
#define VID2_H_VALID_END_TIME       (VID2_H_VALID_START_TIME+VID2_DTV_HDMI_width)

#define VID2_V_SYNC_TIME            (6)
#define VID2_V_PERIOD_TIME          (VID2_DTV_HDMI_height+45)
#define VID2_V_ODD_VALID_START_TIME (VID2_V_SYNC_TIME+30)
#define VID2_V_ODD_VALID_END_TIME   (VID2_V_ODD_VALID_START_TIME+VID2_DTV_HDMI_height)
#define VID2_V_EVEN_VALID_START_TIME (VID2_V_SYNC_TIME+30)
#define VID2_V_EVEN_VALID_END_TIME  (VID2_V_EVEN_VALID_START_TIME+VID2_DTV_HDMI_height)


/* VID4 1280X720P @59.94/60Hz */
#define VID4_HS                     ACTIVE_HIGH
#define VID4_VS                     ACTIVE_HIGH
#define VID4_HVLD                   ACTIVE_LOW
#define VID4_VVLD                   ACTIVE_LOW
#define VID4_CLK                    ACTIVE_LOW /* positive edge */
#define VID4_DE                     ACTIVE_HIGH
#define VID4_PROGRESSIVE            TRUE

//#NT#2009/12/14#Klins Chen#[0008145] -begin
#define VID4_WINX_OFS               30
#define VID4_WINY_OFS               20
#define VID4_H_WINBUF_OFS           (VID4_WINX_OFS*2)
#define VID4_V_WINBUF_OFS           (VID4_WINY_OFS*2)
//#NT#2009/12/14#Klins Chen#[0008145] -end

#define VID4_DTV_HDMI_width         (1280)
#define VID4_DTV_HDMI_height        (720)

#define VID4_H_SYNC_TIME            (40)
#define VID4_H_PERIOD_TIME          (1280+370)
#define VID4_H_VALID_START_TIME     (VID4_H_SYNC_TIME+220)
#define VID4_H_VALID_END_TIME       (VID4_H_VALID_START_TIME+VID4_DTV_HDMI_width)

#define VID4_V_SYNC_TIME            (5)
#define VID4_V_PERIOD_TIME          (VID4_DTV_HDMI_height+30)
#define VID4_V_ODD_VALID_START_TIME (VID4_V_SYNC_TIME+20)
#define VID4_V_ODD_VALID_END_TIME   (VID4_V_ODD_VALID_START_TIME+VID4_DTV_HDMI_height)
#define VID4_V_EVEN_VALID_START_TIME (VID4_V_SYNC_TIME+20)
#define VID4_V_EVEN_VALID_END_TIME  (VID4_V_EVEN_VALID_START_TIME+VID4_DTV_HDMI_height)

/* VID5 1920X1080i @59.94/60Hz */
#define VID5_HS                     ACTIVE_HIGH
#define VID5_VS                     ACTIVE_HIGH
#define VID5_HVLD                   ACTIVE_LOW
#define VID5_VVLD                   ACTIVE_LOW
#define VID5_CLK                    ACTIVE_LOW /* positive edge */
#define VID5_DE                     ACTIVE_HIGH
#define VID5_PROGRESSIVE            FALSE

#define VID5_DTV_HDMI_width         (1920)
#define VID5_DTV_HDMI_height        (540)

#define VID5_H_SYNC_TIME            (44)
#define VID5_H_PERIOD_TIME          (VID5_DTV_HDMI_width+280)
#define VID5_H_VALID_START_TIME     (VID5_H_SYNC_TIME+148)
#define VID5_H_VALID_END_TIME       (VID5_H_VALID_START_TIME+VID5_DTV_HDMI_width)

#define VID5_V_SYNC_TIME            (5)
#define VID5_V_PERIOD_TIME          (VID5_DTV_HDMI_height +22*2+1) //23 is also a choice
#define VID5_V_ODD_VALID_START_TIME (VID5_V_SYNC_TIME+15)
#define VID5_V_ODD_VALID_END_TIME   (VID5_V_ODD_VALID_START_TIME+ VID5_DTV_HDMI_height )
#define VID5_V_EVEN_VALID_START_TIME (VID5_V_SYNC_TIME+15)
#define VID5_V_EVEN_VALID_END_TIME  (VID5_V_EVEN_VALID_START_TIME+ VID5_DTV_HDMI_height )


#endif
#if 0




/* VID6/7 720(1440)X480i @59.94/60Hz */
#define VID6_HS                     ACTIVE_LOW
#define VID6_VS                     ACTIVE_LOW
#define VID6_HVLD                   ACTIVE_HIGH
#define VID6_VVLD                   ACTIVE_HIGH
#define VID6_CLK                    ACTIVE_LOW /* positive edge */
#define VID6_DE                     ACTIVE_HIGH
#define VID6_PROGRESSIVE            FALSE

#define VID6_DTV_HDMI_width         (1440)
#define VID6_DTV_HDMI_height        (480)

#define VID6_H_SYNC_TIME            (124)
#define VID6_H_PERIOD_TIME          (1440+276)
#define VID6_H_VALID_START_TIME     (VID6_H_SYNC_TIME+114)
#define VID6_H_VALID_END_TIME       (VID6_H_VALID_START_TIME+VID6_DTV_HDMI_width)

#define VID6_V_SYNC_TIME            (6)
#define VID6_V_PERIOD_TIME          (VID6_DTV_HDMI_height+23*2)
#define VID6_V_ODD_VALID_START_TIME (VID6_V_SYNC_TIME+15*2)
#define VID6_V_ODD_VALID_END_TIME   (VID6_V_ODD_VALID_START_TIME+VID6_DTV_HDMI_height)
#define VID6_V_EVEN_VALID_START_TIME (VID6_V_SYNC_TIME+15*2)
#define VID6_V_EVEN_VALID_END_TIME  (VID6_V_EVEN_VALID_START_TIME+VID6_DTV_HDMI_height)


/* VID17/18 720X576P @50Hz */
#define VID17_HS                    ACTIVE_LOW
#define VID17_VS                    ACTIVE_LOW
#define VID17_HVLD                  ACTIVE_HIGH
#define VID17_VVLD                  ACTIVE_HIGH
#define VID17_CLK                   ACTIVE_LOW /* positive edge */
#define VID17_DE                    ACTIVE_HIGH
#define VID17_PROGRESSIVE           TRUE

#define VID17_DTV_HDMI_width        (720)
#define VID17_DTV_HDMI_height       (576)

#define VID17_H_SYNC_TIME           (64)
#define VID17_H_PERIOD_TIME         (720+144)
#define VID17_H_VALID_START_TIME    (VID17_H_SYNC_TIME+68)
#define VID17_H_VALID_END_TIME      (VID17_H_VALID_START_TIME+VID17_DTV_HDMI_width)

#define VID17_V_SYNC_TIME           (5)
#define VID17_V_PERIOD_TIME         (VID17_DTV_HDMI_height+49)
#define VID17_V_ODD_VALID_START_TIME (VID17_V_SYNC_TIME+39)
#define VID17_V_ODD_VALID_END_TIME  (VID17_V_ODD_VALID_START_TIME+VID17_DTV_HDMI_height)
#define VID17_V_EVEN_VALID_START_TIME (VID17_V_SYNC_TIME+39)
#define VID17_V_EVEN_VALID_END_TIME (VID17_V_EVEN_VALID_START_TIME+VID17_DTV_HDMI_height)

/* VID19 1280X720P @50Hz */
#define VID19_HS                    ACTIVE_HIGH
#define VID19_VS                    ACTIVE_HIGH
#define VID19_HVLD                  ACTIVE_LOW
#define VID19_VVLD                  ACTIVE_LOW
#define VID19_CLK                   ACTIVE_LOW /* positive edge */
#define VID19_DE                    ACTIVE_HIGH
#define VID19_PROGRESSIVE           TRUE

#define VID19_DTV_HDMI_width        (1280)
#define VID19_DTV_HDMI_height       (720)

#define VID19_H_SYNC_TIME           (40)
#define VID19_H_PERIOD_TIME         (VID19_DTV_HDMI_width+700)
#define VID19_H_VALID_START_TIME    (VID19_H_SYNC_TIME+220)
#define VID19_H_VALID_END_TIME      (VID19_H_VALID_START_TIME+VID19_DTV_HDMI_width)

#define VID19_V_SYNC_TIME           (5)
#define VID19_V_PERIOD_TIME         (VID19_DTV_HDMI_height+30)
#define VID19_V_ODD_VALID_START_TIME (VID19_V_SYNC_TIME+20)
#define VID19_V_ODD_VALID_END_TIME  (VID19_V_ODD_VALID_START_TIME+VID19_DTV_HDMI_height)
#define VID19_V_EVEN_VALID_START_TIME (VID19_V_SYNC_TIME+20)
#define VID19_V_EVEN_VALID_END_TIME (VID19_V_EVEN_VALID_START_TIME+VID19_DTV_HDMI_height)

/* VID20 1920X1080i @50Hz */
#define VID20_HS                    ACTIVE_HIGH
#define VID20_VS                    ACTIVE_HIGH
#define VID20_HVLD                  ACTIVE_LOW
#define VID20_VVLD                  ACTIVE_LOW
#define VID20_CLK                   ACTIVE_LOW /* positive edge */
#define VID20_DE                    ACTIVE_HIGH
#define VID20_PROGRESSIVE           FALSE

#define VID20_DTV_HDMI_width        (1920)
#define VID20_DTV_HDMI_height       (540)

#define VID20_H_SYNC_TIME           (44)
#define VID20_H_PERIOD_TIME         (VID20_DTV_HDMI_width+720)
#define VID20_H_VALID_START_TIME    (VID20_H_SYNC_TIME+148)
#define VID20_H_VALID_END_TIME      (VID20_H_VALID_START_TIME+VID20_DTV_HDMI_width)

#define VID20_V_SYNC_TIME           (5)
#define VID20_V_PERIOD_TIME         (VID20_DTV_HDMI_height +23) //23 is also a choice
#define VID20_V_ODD_VALID_START_TIME (VID20_V_SYNC_TIME+15)
#define VID20_V_ODD_VALID_END_TIME  (VID20_V_ODD_VALID_START_TIME+ VID20_DTV_HDMI_height )
#define VID20_V_EVEN_VALID_START_TIME (VID20_V_SYNC_TIME+15)
#define VID20_V_EVEN_VALID_END_TIME (VID20_V_EVEN_VALID_START_TIME+ VID20_DTV_HDMI_height )
#endif




#endif
