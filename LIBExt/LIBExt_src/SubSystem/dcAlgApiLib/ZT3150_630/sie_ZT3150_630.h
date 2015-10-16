#ifndef _SIE_ZT3150_630_H_
#define _SIE_ZT3150_630_H_


#define MONITOR_MODE_9PIX   0


#define MONITOR_MODE       1
#define MONITOR_MODE_2     2
#define MONITOR_MODE_3     3
#define MONITOR_MODE_DVI1  4   //video
#define MONITOR_MODE_DVI2  5   //sensor
#define MONITOR_MODE_DVI3  6   //side by side
#define STILL_MODE         99

/************** MONITOR_MODE **************/
#define VD_Sync_Prv             64
#define HD_Sync_Prv             64
#define VD_Total_Prv            514
#define HD_Total_Prv            2280

#define ACT_STRX_Prv            68
#define ACT_STRY_Prv            2
#define SIE_PRV_XCENTER			(ACT_STRX_Prv+0+(640/2))
#define SIE_PRV_YCENTER			(ACT_STRY_Prv+6+(480/2)+1)

#define Padding_Prv             0

/************** MONITOR_MODE_2 **************/
//720P
#define VD_Sync_Prv_1               9
#define HD_Sync_Prv_1               312
#define VD_Total_Prv_1              994
#define HD_Total_Prv_1              2856//2192

#define ACT_STRX_Prv_1              8
#define ACT_STRY_Prv_1              2

#define SIE_PRV_XCENTER_1       (ACT_STRX_Prv_1+0+(1280/2))//327
#define SIE_PRV_YCENTER_1       (ACT_STRY_Prv_1+6+(960/2)+1)//244

#define Padding_Prv_1               0
/************** MONITOR_MODE_3 **************/
//1440X1080
#define VD_Sync_Prv_2               9
#define HD_Sync_Prv_2               312
#define VD_Total_Prv_2              994
#define HD_Total_Prv_2              2204//2192

#define ACT_STRX_Prv_2              8
#define ACT_STRY_Prv_2              2

#define SIE_PRV_XCENTER_2       (ACT_STRX_Prv_1+0+(1280/2))//327
#define SIE_PRV_YCENTER_2       (ACT_STRY_Prv_1+6+(960/2)+1)//244

#define Padding_Prv_2               0

//#NT#2011/09/09#Spark Chou -begin
//#NT#
/************** MONITOR_MODE_DVI1 **************/
//Single sensor 0
#define VD_Sync_Prv_DVI1             2
#define HD_Sync_Prv_DVI1             64

#define VD_Total_Prv_DVI1            740
#define HD_Total_Prv_DVI1            2800

#define ACT_STRX_Prv_DVI1            64
#define ACT_STRY_Prv_DVI1            2

#define SIE_PRV_XCENTER_DVI1         (ACT_STRX_Prv_DVI1+(2560/2))
#define SIE_PRV_YCENTER_DVI1         (ACT_STRY_Prv_DVI1+(720/2))

#define Padding_Prv_DVI1   0

/************** MONITOR_MODE_DVI2 **************/
//Single sensor 1
#define VD_Sync_Prv_DVI2             2
#define HD_Sync_Prv_DVI2             64

#define VD_Total_Prv_DVI2            740
#define HD_Total_Prv_DVI2            2800

#define ACT_STRX_Prv_DVI2            64
#define ACT_STRY_Prv_DVI2            2

#define SIE_PRV_XCENTER_DVI2         (ACT_STRX_Prv_DVI2+(2560/2))
#define SIE_PRV_YCENTER_DVI2         (ACT_STRY_Prv_DVI2+(720/2))

#define Padding_Prv_DVI2   0

/************** MONITOR_MODE_DVI3 **************/
#define VD_Sync_Prv_DVI3             2
#define HD_Sync_Prv_DVI3             64

#define VD_Total_Prv_DVI3            580
#define HD_Total_Prv_DVI3            4000

#define ACT_STRX_Prv_DVI3            64
#define ACT_STRY_Prv_DVI3            2

#define SIE_PRV_XCENTER_DVI3         (ACT_STRX_Prv_DVI1+(3840/2))
#define SIE_PRV_YCENTER_DVI3         (ACT_STRY_Prv_DVI1+(544/2))

#define Padding_Prv_DVI1   0

/************** STILL_MODE **************/
#define VD_Sync_Cap             9
#define HD_Sync_Cap             312
#define VD_Total_Cap            1978
#define HD_Total_Cap            3504

#define HD_Division             1
#define HD_Div1_Num             0
#define HD_Div2_Num             0
#define HD_Div3_Num             0
#define HD_Div4_Num             0
#define HD_Div5_Num             0


#define IMAGE_SIZE_H            2592
#define IMAGE_SIZE_V            1944

#define ACT_STRX_Cap            17
#define ACT_STRY_Cap            1
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
#define SIE_CAP_XCENTER                     (ACT_STRX_Cap+1+(2560/2))
#define SIE_CAP_YCENTER                     (ACT_STRY_Cap+8+(1920/2))

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


#define OB_OFS_PRV     0xa8//0xB0
#define OB_OFS_CAP     0xa8//0xB0

/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV    IPE_PATR
#define CFAPAT_CAP    IPE_PATR

/************** Sensor mode selection for preview**************/
#define SENSOR_PRV_MODE     MONITOR_MODE
#define SENSOR_PRV_VGATYPE  SENSOR_OUT_PROGRESSIVE

#define PRV_SIE_BP1         10
#define PRV_SIE_BP2         SIE_PRV_YCENTER

#define PCC_SIE_BP1         10
#define PCC_SIE_BP2         SIE_PRV_YCENTER

#define AF_SIE_BP1          1
#define AF_SIE_BP2          SIE_PRV_YCENTER_2

#define CAP_SIE_BP1         100
#define CAP_SIE_BP2         200

//#NT#2011/09/09#Spark Chou -begin
//#NT#
#define DVI_FORMAT          DVI_CBYCRY
#define DVI_HSUB            DVI_HSUB1
//#NT#2011/09/09#Spark Chou -end

typedef enum
{
    _1xBining=1,
    _2xBining=2,
    _4xBining=4,
    _8xBining=8
}_BiningMode;
#endif
