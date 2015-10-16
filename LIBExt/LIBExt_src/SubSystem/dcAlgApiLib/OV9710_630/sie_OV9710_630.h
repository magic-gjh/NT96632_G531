#ifndef _SIE_OV9710_630_H_
#define _SIE_OV9710_630_H_


#define MONITOR_MODE_9PIX   0


#define MONITOR_MODE    1
#define MONITOR_MODE_2  2   //720p
#define MONITOR_MODE_3  3   //1080
#define STILL_MODE      4

#define IPP_PRV_MODE_NUM        0x03

/************** monitor mode 30 fps **************/

#define VD_Sync_Prv                 1
#define HD_Sync_Prv                 1
#define VD_Total_Prv                804
#define HD_Total_Prv                1688

#define ACT_STRX_Prv                8
#define ACT_STRY_Prv                1

#define SIE_PRV_XCENTER         (8 + (1280/2) + 1)
#define SIE_PRV_YCENTER         (1 + (800/2) + 1)

#define Padding_Prv                 0

/************** monitor mode 2**************/
#define VD_Sync_Prv_1               1
#define HD_Sync_Prv_1               1
#define VD_Total_Prv_1              790
#define HD_Total_Prv_1              1500

#define ACT_STRX_Prv_1              8//4//
#define ACT_STRY_Prv_1              8//4//

#define SIE_PRV_XCENTER_1       (8+(1408/2)+1)
#define SIE_PRV_YCENTER_1       (8+(780/2)+1)

#define Padding_Prv_1               0

/************** monitor mode 3**************/
#define VD_Sync_Prv_2               1
#define HD_Sync_Prv_2               1

#define VD_Total_Prv_2              1440
#define HD_Total_Prv_2              1860


#define ACT_STRX_Prv_2              8
#define ACT_STRY_Prv_2              8

#define SIE_PRV_XCENTER_2       (8+(1800/2)+1)
#define SIE_PRV_YCENTER_2       (8+(1380/2)+1)


#define Padding_Prv_2               0
/************** Capture mode **************/
#define VD_Sync_Cap             1
#define HD_Sync_Cap             1
#define VD_Total_Cap            804//2892    //2010/04/02 Meg
#define HD_Total_Cap            1688//3776   //2010/04/02 Meg
#define HD_Division             1
#define HD_Div1_Num             0
#define HD_Div2_Num             0
#define HD_Div3_Num             0
#define HD_Div4_Num             0
#define HD_Div5_Num             0


#define IMAGE_SIZE_H            1280//3664  //2010/04/02 Meg
#define IMAGE_SIZE_V            800//2748   //2010/04/02 Meg

#define ACT_STRX_Cap            8
#define ACT_STRY_Cap            1//8        //2010/04/02 Meg
#define ACT_STRY_Cap2           0xff
#define ACT_STRY_Cap3           0xff
#define ACT_STRY_Cap4           0xff
#define ACT_STRY_Cap5           0xff
#define ACT_STRY_Cap6           0xff
#define ACT_STRY_Cap7           0xff
#define ACT_STRY_Cap8           0xff

#define SensorCapTotalFld       1
#define Field_Num1              1
#define Field_Num2              0x1
#define Field_Num3              0x1
#define Field_Num4              0x1
#define Field_Num5              0x1
#define Field_Num6              0x1
#define Field_Num7              0x1
#define Field_Num8              0x1

#define Padding_Cap             0
#define SIE_CAP_XCENTER         (8+(1280/2)+1)//(8+(3648/2)+1)  //2010/04/02 Meg
#define SIE_CAP_YCENTER         (1+(800/2)+1)//(8+(2736/2)+1)   //2010/04/02 Meg


/************** OB Setting **************/
#define PRV_OB_GAIN         0x00
#define PRV_OB_THRES        0xF0
#define PRV_OB_SUBSAMPLE    0x02
#define PRV_OB_X0           1264
#define PRV_OB_X1           4900
#define PRV_OB_Y0           9
#define PRV_OB_Y1           9

#define CAP_OB_GAIN         0x00
#define CAP_OB_THRES        0xF0
#define CAP_OB_SUBSAMPLE    0x05
#define CAP_OB_X0           1200
#define CAP_OB_X1           4900
#define CAP_OB_Y0           17
#define CAP_OB_Y1           17

#define OB_OFS_PRV      0x04
#define OB_OFS_CAP      0x04

/************** PRE output for IPE CFA pattern selection **************/
#define CFAPAT_PRV    IPE_PATR
#define CFAPAT_CAP    IPE_PATR

/************** Sensor mode selection for preview**************/
#define SENSOR_PRV_MODE     MONITOR_MODE
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
        *ptr = _1xBining;                   \
    else if(sen_mode==MONITOR_MODE_2)       \
        *ptr = _1xBining;                   \
    else if(sen_mode==MONITOR_MODE_3)       \
        *ptr = _1xBining;                   \
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
