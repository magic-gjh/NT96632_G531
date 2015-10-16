#ifndef _SIE_MI5100_630_H_
#define _SIE_MI5100_630_H_


#define MONITOR_MODE_9PIX   0


#define MONITOR_MODE    1   //VGA
#define MONITOR_MODE_2  2   //720P
#define MONITOR_MODE_3  3   //1440X1080
#define STILL_MODE      4

#define IPP_PRV_MODE_NUM        0x03

/************** MONITOR_MODE **************/
#if 0   //1280x960
#define VD_Sync_Prv               9
#define HD_Sync_Prv               312
#define VD_Total_Prv              994
#define HD_Total_Prv              2204//2192

#define ACT_STRX_Prv              8
#define ACT_STRY_Prv              2

#define SIE_PRV_XCENTER       (ACT_STRX_Prv_1+0+(1280/2))
#define SIE_PRV_YCENTER       (ACT_STRY_Prv_1+6+(960/2)+1)

#define Padding_Prv               0
#else   //640x480
#define VD_Sync_Prv             9
#define HD_Sync_Prv             312
#define VD_Total_Prv            514
#define HD_Total_Prv            2204

#define ACT_STRX_Prv            8
#define ACT_STRY_Prv            2
#define SIE_PRV_XCENTER			(ACT_STRX_Prv+0+(640/2))
#define SIE_PRV_YCENTER			(ACT_STRY_Prv+6+(480/2)+1)

#define Padding_Prv             0
#endif
/************** MONITOR_MODE_2 **************/
//720P
#define VD_Sync_Prv_1               9
#define HD_Sync_Prv_1               312
#define VD_Total_Prv_1              994
//#NT#2010/06/10#Jarkko Chang -begin
//#NT#modify for pclk 96Mhz using
//#define HD_Total_Prv_1              2204//2192
#define HD_Total_Prv_1              2856//2192
//#NT#2010/06/10#Jarkko Chang -end

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
/************** STILL_MODE **************/
#define VD_Sync_Cap             9
#define HD_Sync_Cap             312
//#define VD_Total_Cap            1975
#define VD_Total_Cap            1978
//#define HD_Total_Cap            3450
#define HD_Total_Cap            3504

#define HD_Division             1
#define HD_Div1_Num             0
#define HD_Div2_Num             0
#define HD_Div3_Num             0
#define HD_Div4_Num             0
#define HD_Div5_Num             0


#define IMAGE_SIZE_H            2592
#define IMAGE_SIZE_V            1944

#define ACT_STRX_Cap            17//7
//#define ACT_STRX_Cap            7   //lsc using
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

//#define SIE_CAP_XCENTER                     (ACT_STRX_Cap+3+(2592/2))   //lsc using
//#define SIE_CAP_YCENTER                     (ACT_STRY_Cap+8+(1944/2))   //lsc using
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

//#NT#2009/12/02#JJ Huang -begin
#define OB_OFS_PRV     0xa8//0xB0
#define OB_OFS_CAP     0xa8//0xB0
//#NT#2009/12/02#JJ Huang -end
//#define OB_OFS_PRV     0x0
//#define OB_OFS_CAP     0x0
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

typedef enum
{
    _1xBining=1,
    _2xBining=2,
    _4xBining=4,
    _8xBining=8
}_BiningMode;
#endif
