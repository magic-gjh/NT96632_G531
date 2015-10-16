#ifndef _SIE_OV5653_630_H_
#define _SIE_OV5653_630_H_


#define MONITOR_MODE_9PIX   0

//must follow _SEN_MODE
#define MONITOR_MODE    1//VGA
#define MONITOR_MODE_2  2//720P
#define MONITOR_MODE_3  3//1280*960
#define MONITOR_MODE_4  4//1280*480
#define MONITOR_MODE_5  5//2560*960
#define MONITOR_MODE_6  6//2560*720
#define MONITOR_MODE_7  7//2560*1096
#define STILL_MODE      8//full

#define IPP_PRV_MODE_NUM        0x03

/************** MONITOR_MODE(VGA) **************/

#define VD_Sync_Prv                 1
#define HD_Sync_Prv                 640
#define VD_Total_Prv                653//688//830
#define HD_Total_Prv                1220//1606

#define ACT_STRX_Prv                  7
#define ACT_STRY_Prv                  1

#define SIE_PRV_XCENTER         (ACT_STRX_Prv+(624/2)+1)
#define SIE_PRV_YCENTER         (ACT_STRY_Prv+(468/2)+1)

#define Padding_Prv                 0

/************** MONITOR_MODE_2(720p) **************/
#define VD_Sync_Prv_2               1
#define HD_Sync_Prv_2               1281
#define VD_Total_Prv_2              1066//1060
#define HD_Total_Prv_2              2144//2120

#define ACT_STRX_Prv_2              8
#define ACT_STRY_Prv_2              2

#define SIE_PRV_XCENTER_2       (ACT_STRX_Prv_2+(1272/2))
#define SIE_PRV_YCENTER_2       (ACT_STRY_Prv_2+(716/2))

#define Padding_Prv_2               0

/************** MONITOR_MODE_3(1280*960)**************/
#define VD_Sync_Prv_3               1
#define HD_Sync_Prv_3               1281

#define VD_Total_Prv_3              970
#define HD_Total_Prv_3              2200//2177


#define ACT_STRX_Prv_3              8
#define ACT_STRY_Prv_3              2

#define SIE_PRV_XCENTER_3       (ACT_STRX_Prv_3+(1272/2))
#define SIE_PRV_YCENTER_3       (ACT_STRY_Prv_3+(956/2))


#define Padding_Prv_3               0

/************** MONITOR_MODE_4(1280*480)**************/

#define VD_Sync_Prv_4               1
#define HD_Sync_Prv_4               1300

#define VD_Total_Prv_4              494//485
#define HD_Total_Prv_4              2279//2245


#define ACT_STRX_Prv_4              8
#define ACT_STRY_Prv_4              2

#define SIE_PRV_XCENTER_4       (ACT_STRX_Prv_4+(1272/2))
#define SIE_PRV_YCENTER_4       (ACT_STRY_Prv_4+(476/2))

#define Padding_Prv_4               0
/************** MONITOR_MODE_5(2560*960)**************/
#define VD_Sync_Prv_5               1
#define HD_Sync_Prv_5               2572

#define VD_Total_Prv_5              976
#define HD_Total_Prv_5              4880//4924


#define ACT_STRX_Prv_5              8
#define ACT_STRY_Prv_5              2

#define SIE_PRV_XCENTER_5       (ACT_STRX_Prv_5+(2556/2))
#define SIE_PRV_YCENTER_5       (ACT_STRY_Prv_5+(956/2))

#define Padding_Prv_5               0
/************** MONITOR_MODE_6(2560*720)**************/
#define VD_Sync_Prv_6               1
#define HD_Sync_Prv_6               2572

#define VD_Total_Prv_6              776
#define HD_Total_Prv_6              4880


#define ACT_STRX_Prv_6              8
#define ACT_STRY_Prv_6              2

#define SIE_PRV_XCENTER_6       (ACT_STRX_Prv_6+(2556/2))
#define SIE_PRV_YCENTER_6       (ACT_STRY_Prv_6+(776/2))

#define Padding_Prv_6               0

/************** MONITOR_MODE_7(2560*1096)**************/

#define VD_Sync_Prv_7               1
#define HD_Sync_Prv_7               2572

#define VD_Total_Prv_7              1424
#define HD_Total_Prv_7              3252

#define ACT_STRX_Prv_7              8
#define ACT_STRY_Prv_7              2

#define SIE_PRV_XCENTER_7       (ACT_STRX_Prv_7+(2556/2))
#define SIE_PRV_YCENTER_7       (ACT_STRY_Prv_7+(1424/2))

#define Padding_Prv_7               0
/************** STILL_MODE **************/
#define VD_Sync_Cap             2
#define HD_Sync_Cap             2592
#define VD_Total_Cap            1968//1975
#define HD_Total_Cap            3252//3247

#define HD_Division             1
#define HD_Div1_Num             0
#define HD_Div2_Num             0
#define HD_Div3_Num             0
#define HD_Div4_Num             0
#define HD_Div5_Num             0


#define IMAGE_SIZE_H            2560
#define IMAGE_SIZE_V            1920

#define ACT_STRX_Cap            10
#define ACT_STRY_Cap            4
#define ACT_STRY_Cap2           0xff
#define ACT_STRY_Cap3           0xff
#define ACT_STRY_Cap4           0xff
#define ACT_STRY_Cap5           0xff
#define ACT_STRY_Cap6           0xff
#define ACT_STRY_Cap7           0xff
#define ACT_STRY_Cap8           0xff

#define SensorCapTotalFld       1
#define Field_Num1              1
#define Field_Num2              0xff
#define Field_Num3              0xff
#define Field_Num4              0xff
#define Field_Num5              0xff
#define Field_Num6              0xff
#define Field_Num7              0xff
#define Field_Num8              0xff

#define Padding_Cap             0
#define SIE_CAP_XCENTER     (ACT_STRX_Cap+(IMAGE_SIZE_H/2))
#define SIE_CAP_YCENTER     (ACT_STRY_Cap+(IMAGE_SIZE_V/2))


/************** OB Setting **************/
#define PRV_OB_GAIN         0x20
#define PRV_OB_THRES        0xF0
#define PRV_OB_SUBSAMPLE    0x02
#define PRV_OB_X0           1264
#define PRV_OB_X1           4900
#define PRV_OB_Y0           9
#define PRV_OB_Y1           9

#define CAP_OB_GAIN         0x20
#define CAP_OB_THRES        0xF0
#define CAP_OB_SUBSAMPLE    0x05
#define CAP_OB_X0           1200
#define CAP_OB_X1           4900
#define CAP_OB_Y0           17
#define CAP_OB_Y1           17

#define OB_OFS_PRV      0x50//0x320//
#define OB_OFS_CAP      0x50//0x320//

/************** PRE output for IPE CFA pattern selection **************/
//#NT#2010/10/15#ethanlau -begin
//#NT#for 565 demo only
//#NT#2011/01/13#JJ Huang -begin
//#NT# OV5653 Sensor Flip Switch
#ifdef OV5653_630_HVFlip
//For dv565, HV-flip
#define CFAPAT_PRV    IPE_PATB
#define CFAPAT_CAP    IPE_PATB
#else
//For Normal(HJT/TPK/EVB), no-flip
#define CFAPAT_PRV    IPE_PATGR//for HJT//IPE_PATB
#define CFAPAT_CAP    IPE_PATGR//for HJT//IPE_PATB
#endif
//#NT#2011/01/13#JJ Huang -end
//#define CFAPAT_PRV    IPE_PATGR
//#define CFAPAT_CAP    IPE_PATGR
//#NT#2010/10/15#ethanlau -end
/************** Sensor mode selection for preview**************/
//#NT#20090912#ethanlau -begin
//move to sensorsys_ov5633.c
//#define SENSOR_PRV_MODE     MONITOR_MODE
//#NT#20090912#ethanlau -end
#define SENSOR_PRV_VGATYPE  SENSOR_OUT_PROGRESSIVE
//#NT#20090711#ETHAN LAU -begin
typedef enum
{
    _1xBining=1,
    _2xBining=2,
    _4xBining=4,
    _8xBining=8
}_BiningMode;

//#NT#2011/01/17#JJ Huang -begin
//#NT# For x4 Bining
#if 1
#define BiningSetting(sen_mode,ptr)         \
{                                           \
    if(sen_mode==MONITOR_MODE)              \
        *ptr = _4xBining;                   \
    else if(sen_mode==MONITOR_MODE_2)       \
        *ptr = _4xBining;                   \
    else if(sen_mode==MONITOR_MODE_3)       \
        *ptr = _4xBining;                   \
    else if(sen_mode==MONITOR_MODE_4)       \
        *ptr = _4xBining;                   \
    else if(sen_mode==MONITOR_MODE_6)       \
        *ptr = _2xBining;                   \
    else                                    \
        *ptr = _1xBining;                   \
}
#else
#define BiningSetting(sen_mode,ptr)         \
{                                           \
    if(sen_mode==MONITOR_MODE)              \
        *ptr = _2xBining;                   \
    else if(sen_mode==MONITOR_MODE_2)       \
        *ptr = _2xBining;                   \
    else if(sen_mode==MONITOR_MODE_3)       \
        *ptr = _2xBining;                   \
    else if(sen_mode==MONITOR_MODE_4)       \
        *ptr = _2xBining;                   \
    else                                    \
        *ptr = _1xBining;                   \
}
#endif
//#NT#2011/01/17#JJ Huang -end
//#NT#20090711#ETHAN LAU -end


#define PRV_SIE_BP1         10
#define PRV_SIE_BP2         VD_Total_Prv_4 - 20

#define PCC_SIE_BP1         10
#define PCC_SIE_BP2         SIE_PRV_YCENTER

#define AF_SIE_BP1          1
#define AF_SIE_BP2          SIE_PRV_YCENTER_2

#define CAP_SIE_BP1         100
#define CAP_SIE_BP2         200
#endif
