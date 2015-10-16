#ifndef _SIE_OV5633_630_H_
#define _SIE_OV5633_630_H_


#define MONITOR_MODE_9PIX   0

#define STILL_MODE                  4//FULL_FREX
#define MONITOR_MODE                1//VGA
#define MONITOR_MODE_2              2//SXGA
#define MONITOR_MODE_3              3//5M


/************** MONITOR_MODE **************/

#define VD_Sync_Prv             2
#define HD_Sync_Prv             2
#define VD_Total_Prv            516
#define HD_Total_Prv            1550

#define ACT_STRX_Prv            8
#define ACT_STRY_Prv            3
#define SIE_PRV_XCENTER         (ACT_STRX_Prv+(640/2))
#define SIE_PRV_YCENTER         (ACT_STRY_Prv+(480/2))

#define Padding_Prv             0

/************** MONITOR_MODE_2 **************/
#define VD_Sync_Prv_2               2
#define HD_Sync_Prv_2               2
#define VD_Total_Prv_2              996
#define HD_Total_Prv_2              2006

#define ACT_STRX_Prv_2              8
#define ACT_STRY_Prv_2              3

#define SIE_PRV_XCENTER_2           ACT_STRX_Prv_2+(1280/2)
#define SIE_PRV_YCENTER_2           ACT_STRY_Prv_2+(960/2)

#define Padding_Prv_2               0

/************** MONITOR_MODE_3 **************/
#define VD_Sync_Prv_3               2
#define HD_Sync_Prv_3               2
#define VD_Total_Prv_3              1980
#define HD_Total_Prv_3              3232

#define ACT_STRX_Prv_3              4
#define ACT_STRY_Prv_3              4

#define SIE_PRV_XCENTER_3           (ACT_STRX_Cap+(2592/2))
#define SIE_PRV_YCENTER_3           (ACT_STRY_Cap+(1944/2))

#define Padding_Prv_2               0

/************** STILL_MODE **************/
#define VD_Sync_Cap             2
#define HD_Sync_Cap             2
#define VD_Total_Cap            1980
#define HD_Total_Cap            3232

#define HD_Division             1
#define HD_Div1_Num             0
#define HD_Div2_Num             0
#define HD_Div3_Num             0
#define HD_Div4_Num             0
#define HD_Div5_Num             0

#define IMAGE_SIZE_H            2592
#define IMAGE_SIZE_V            1920

#define ACT_STRX_Cap            8
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
#define SIE_CAP_XCENTER         (ACT_STRX_Cap+(2592/2))
#define SIE_CAP_YCENTER         (ACT_STRY_Cap+(1944/2))


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

#define OB_OFS_PRV      0x60
#define OB_OFS_CAP      0x30

/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV    IPE_PATB//IPE_PATGB
#define CFAPAT_CAP    IPE_PATGR//IPE_PATR

/************** Sensor mode selection for preview**************/
#define SENSOR_PRV_MODE     MONITOR_MODE_2
#define SENSOR_PRV_VGATYPE  SENSOR_OUT_PROGRESSIVE
//#NT#20090711#ETHAN LAU -begin
typedef enum
{
    _1xBining=1,
    _2xBining=2,
    _4xBining=4,
    _8xBining=8
}_BiningMode;

#define BiningSetting(sen_mode,ptr)         \
{                                           \
    if(sen_mode==MONITOR_MODE)              \
        *ptr = _2xBining;                   \
    else if(sen_mode==MONITOR_MODE_2)       \
        *ptr = _2xBining;                   \
    else if(sen_mode==MONITOR_MODE_3)       \
        *ptr = _2xBining;                   \
    else                                    \
        *ptr = _1xBining;                   \
}
//#NT#20090711#ETHAN LAU -end


#define PRV_SIE_BP1         10
#define PRV_SIE_BP2         SIE_PRV_YCENTER

#define PCC_SIE_BP1         10
#define PCC_SIE_BP2         SIE_PRV_YCENTER

#define AF_SIE_BP1          1
#define AF_SIE_BP2          SIE_PRV_YCENTER_2

#define CAP_SIE_BP1         100
#define CAP_SIE_BP2         200
#endif
