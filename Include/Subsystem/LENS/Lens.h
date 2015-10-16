/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       Lens.h
    @ingroup    mISYSLens

    @brief      Header file of Lens control functions
                Lens control functions header file

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#ifndef _LENS_H
#define _LENS_H

#include "Type.h"

/*
    1. Add TOTOL_NUM in enum type.
    2. Use upper case in enum type.
    3. Use "zoom_gotoSection" instead of "zoomgotosection" in function table.
*/

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for overall Lens Module related definition.
//--------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    LENS_IO_MODE_1,
    LENS_IO_MODE_2,
    LENS_IO_MODE_3,
    LENS_IO_IN_0,
    LENS_IO_IN_1,
    LENS_IO_IN_2,
    LENS_IO_IN_3,
    LENS_IO_IN_4,
    LENS_IO_IN_5,
    LENS_IO_PWR,
    LENS_IO_ZOOM_PI,
    LENS_IO_ZOOM_PR,
    LENS_IO_FOCUS_PI,
    LENS_IO_MOT_EXT1,
    LENS_IO_MOT_EXT2,
    LENS_IO_MOT_MOB1,
    LENS_IO_MOT_MOB2,
    LENS_IO_MOT_WIDE_INT,
    LENS_IO_MOT_TELE_INT,
    LENS_IO_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_IO_MAP)
} LENS_MODULE_IO_MAP;

typedef enum
{
    LENS_PAD_ZOOM_PI,
    LENS_PAD_ZOOM_PR,
    LENS_PAD_FOCUS_PI,
    LENS_PAD_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_PAD_MAP)
} LENS_MODULE_PAD_MAP;

typedef enum
{
    LENS_NONE,
    LENS_INITOK,
    LENS_IDLE,
    LENS_ERROR,
    LENS_ZOOMMOVING,
    LENS_FOCUSMOVING,
    LENS_IRISMOVING,
    LENS_SHUTTERMOVING,
    ENUM_DUMMY4WORD(LENS_MODULE_STATE)
} LENS_MODULE_STATE;

typedef enum
{
    LENS_SIF_CONTROLLER,
    LENS_SIF_GPIOSIM,
    ENUM_DUMMY4WORD(LENS_SIF_TYPE)
} LENS_SIF_TYPE;

typedef enum
{
    MOTOR_CTRL_GPIO = 0x01,
    MOTOR_CTRL_SIF  = 0x02,
    MOTOR_CTRL_I2C  = 0x04,
    MOTOR_CTRL_RESV = 0x08,
    ENUM_DUMMY4WORD(MOTOR_CTRL)
} MOTOR_CTRL;

typedef struct _LENS_SIF_PINDEF
{
    LENS_SIF_TYPE   uiSIFType;                  ///< SIF use SIF controller or GPIO simulation
    UINT32          uiSIFChannel;               ///< SIF channel SIF_CH0 ~ 3, refer to SIF.h, valid in LCD_CTRL_MODE_SIF
    UINT32          uiGPIOSen;                  ///< GPIO pin number for SEN, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32          uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32          uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO
} LENS_SIF_PINDEF, *PLENS_SIF_PINDEF;

typedef enum
{
    LENS_I2C_CONTROLLER,
    LENS_I2C_GPIOSIM,
    ENUM_DUMMY4WORD(LENS_I2C_TYPE)
} LENS_I2C_TYPE;

typedef struct _LENS_I2C_PINDEF
{
    LENS_I2C_TYPE   uiI2CType;                  ///< I2C use I2C controller or GPIO simulation
    UINT32          uiDeviceAddress;            ///< I2C device address
    UINT32          uiGPIOClk;                  ///< GPIO pin number for Clock, valid when uimode == LCD_CTRL_MODE_GPIO
    UINT32          uiGPIOData;                 ///< GPIO pin number for Data, valid when uimode == LCD_CTRL_MODE_GPIO
} LENS_I2C_PINDEF, *PLENS_I2C_PINDEF;

/**
    Lens device object.
*/
typedef struct _LENS_DEVICE_OBJ
{
    MOTOR_CTRL          uiMotorCtrlMode;        ///< Motor control interface mode
    LENS_SIF_PINDEF     LensSIFPinObj;          ///< Lens SIF motor control pin definition data
    LENS_I2C_PINDEF     LensI2CPinObj;          ///< Lens I2C motor control pin definition data
    UINT32              uiMotorCtrlGPIOIntrID;  ///< GPIO interrupt ID for zoom PI
    UINT32              *puiMotorCtrlGPIOArray; ///< GPIO pin array for HW connection of DSP, motor driver and lens
    UINT32              *puiMotorCtrlPADArray;  ///< PAD pin array for voltage pull
    UINT32              *puiMotorCtrlPWMArray;  ///< PWM parameter array for zoom speed control
} LENS_DEVICE_OBJ, *PLENS_DEVICE_OBJ;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Motor Driver related definition.
//--------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    MOTOR_POWER_OFF,
    MOTOR_POWER_ON,
    MOTOR_POWER_MAX,
    ENUM_DUMMY4WORD(MOTOR_POWER)
} MOTOR_POWER;

typedef enum
{
    MOTOR_MODE_STANDBY,
    MOTOR_MODE_ZOOM,
    MOTOR_MODE_FOCUS,
    MOTOR_MODE_APERTURE,
    MOTOR_MODE_SHUTTER,
    ENUM_DUMMY4WORD(MOTOR_MODE)
} MOTOR_MODE;

typedef enum
{
    MOTOR_ZOOM_BRAKE,
    MOTOR_ZOOM_FWD,
    MOTOR_ZOOM_BWD,
    MOTOR_ZOOM_OFF,
    ENUM_DUMMY4WORD(MOTOR_ZOOM_ACT)
} MOTOR_ZOOM_ACT;

typedef enum
{
    MOTOR_FOCUS_FWD,
    MOTOR_FOCUS_BWD,
    ENUM_DUMMY4WORD(MOTOR_FOCUS_ACT)
} MOTOR_FOCUS_ACT;

/**
    Motor Info object.
    Some motor information can be transfered via motor info object.
*/
typedef struct _MOTOR_INFO_TAB
{
    //PWM
    BOOL          bPwmCtrl;
    UINT32        uiPwmPeriodTime;
    UINT32        uiPwmOnTime;

    //Zoom
    BOOL          bWaitZoomFinish;
} MOTOR_INFO_TAB, *PMOTOR_INFO_TAB;

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_TAB
{
    //common related cb
    void          (*init)(PLENS_DEVICE_OBJ);
    void          (*power)(MOTOR_POWER);
    void          (*changeMode)(MOTOR_MODE);//mode type
    void          (*setMotorInfo)(PMOTOR_INFO_TAB);

    //zoom related cb
    void          (*zoom_setState)(MOTOR_ZOOM_ACT, UINT32);//zoom action
    void          (*zoom_setPIInt)(BOOL);//zoom PI int set
    void          (*zoom_clearPIInt)(void);//zoom PI int clear
    BOOL          (*zoom_checkPIInt)(void);//zoom PI int check
    BOOL          (*zoom_checkPR)(void);//zoom PR check
    BOOL          (*zoom_checkPI)(void);//zoom PR check

    //focus related cb
    BOOL          (*focus_checkPI)(void);//focus PI check
    void          (*focus_setState)(UINT32, UINT32);//focus action

    //aperture related cb
    void          (*aperture_setState)(UCHAR);//aperture action

    //shutter related cb
    void          (*shutter_setState)(UCHAR);//shutter action
} MOTOR_TAB, *PMOTOR_TAB;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for Lens Driver related definition.
//--------------------------------------------------------------------------------------------------------------------------
typedef enum
{
    LENS_ZOOM_INIT_PART1,
    LENS_ZOOM_INIT_PART2,
    ENUM_DUMMY4WORD(LENS_ZOOM_INIT_STATE)
} LENS_ZOOM_INIT_STATE;

typedef enum
{
    LENS_ZOOM_IN,
    LENS_ZOOM_OUT,
    ENUM_DUMMY4WORD(LENS_ZOOM_ACT)
} LENS_ZOOM_ACT;

typedef enum
{
    LENS_APERTURE_BIG,
    LENS_APERTURE_SMALL,
    ENUM_DUMMY4WORD(LENS_APERTURE_ACT)
} LENS_APERTURE_ACT;

typedef enum
{
    LENS_SHUTTER_CLOSE,
    LENS_SHUTTER_OPEN,
    LENS_SHUTTER_TOTAL_NUM,
    ENUM_DUMMY4WORD(LENS_SHUTTER_ACT)
} LENS_SHUTTER_ACT;

typedef struct _LENS_TAB
{
    //Lens
    void                (*init)(PLENS_DEVICE_OBJ, PMOTOR_TAB);
    BOOL                (*isFFLens)(void);
    //Zoom
    INT32               (*zoom_initPart1)(void);
    INT32               (*zoom_initPart2)(void);
    INT32               (*zoom_ctrlRelease)(void);
    INT32               (*zoom_ctrlPress)(LENS_ZOOM_ACT);
    UINT8               (*zoom_getCurrentSection)(void);
    UINT8               (*zoom_getMaxSection)(void);
    UINT8               (*zoom_getNextSection)(void);
    INT32               (*zoom_retract)(void);
    void                (*zoom_gotoSection)(UINT8);
    void                (*zoom_setAutoBoundaryCheck)(BOOL);
    void                (*zoomIsr_enable)(BOOL);
    //Focus
    void                (*focus_gotoPosition)(INT16);
    INT32               (*focus_gotoHome)(void);
    INT32               (*focus_gotoGarage)(void);
    INT16               (*focus_getCurrentPosition)(void);
    INT16               (*focus_getMaxPosition)(void);
    UINT32              (*focus_getFL)(UINT32);
    INT16               (*focus_getZoomTblValue)(UINT32, UINT32);
    //Aperture
    INT32               (*aperture_gotoState)(LENS_APERTURE_ACT);
    LENS_APERTURE_ACT   (*aperture_getState)(void);
    UINT32              (*aperture_getFNO)(LENS_APERTURE_ACT, UINT32);
    //Shutter
    INT32               (*shutter_gotoState)(LENS_SHUTTER_ACT);
    UINT32              (*shutter_getPulsePeriod)(void);
    //Others
    PMOTOR_TAB          pMotor;
} LENS_TAB, *PLENS_TAB;

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for other old definition.
//--------------------------------------------------------------------------------------------------------------------------
#define ERR_OK_TIMER_POWEROFF   1   //Operation is OK and indicate caller that let Timer do power off.
#define ERR_OK                  0
#define ERR_NO_HW_AF            (-1)
#define ERR_NO_HW_ZOOM          (-2)
#define ERR_NO_HW_IRIS          (-3)
#define ERR_OPERATION_FAIL      (-4)
#define OPEN                    1
#define CLOSE                   0

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for function declaration.
//--------------------------------------------------------------------------------------------------------------------------
//Lens Command
extern void                 Lens_Cmd(CHAR *pcCmdStr);

//Lens API: Lens
extern void                 Lens_Module_Init(PLENS_DEVICE_OBJ pLensDev, PLENS_TAB pLensTab, PMOTOR_TAB pMotorTab);
extern UINT32               Lens_Module_GetState(void);
extern void                 Lens_OnOff(unsigned char ON_OFF);

//Lens API: Zoom
extern INT32                Lens_Zoom_Init(LENS_ZOOM_INIT_STATE part);
extern INT32                Lens_Zoom_GetSection(void);
extern INT32                Lens_Zoom_GetMaxSection(void);
extern INT32                Lens_Zoom_Goto(UINT8 section);
extern INT32                Lens_Zoom_In(void);
extern INT32                Lens_Zoom_Out(void);
extern INT32                Lens_Zoom_Stop(void);
extern INT32                Lens_Zoom_Retract(void);
extern void                 Lens_Zoom_EnableIsr(BOOL enable);

//Lens API: Focus
extern INT32                Lens_Focus_Goto(INT16 position);
extern INT32                Lens_Focus_GotoHome(void);
extern INT32                Lens_Focus_GetCurrentPosition(void);
extern INT32                Lens_Focus_GetMaxPosition(void);
extern UINT32               Lens_Focus_GetFL(UINT32 zoomSection);
extern INT16                Lens_Focus_GetZoomTableValue(UINT32 zoomSection, UINT32 index);

//Lens API: Aperture
extern INT32                Lens_Aperture_Move(unsigned short NewPosition);
extern INT32                Lens_Aperture_GetPosition(void);
extern UINT32               Lens_Aperture_GetFNO(LENS_APERTURE_ACT irisPos, UINT32 zoomSection);

//Lens API: Shutter
extern INT32                Lens_Shutter_Move(unsigned char open);

//Driver: Motor
extern PMOTOR_TAB           motorPSEUDO_getObject(void);
extern PMOTOR_TAB           motorR2A30423_getObject(void);

//Driver: Lens
extern PLENS_TAB            lensFF_getObject(void);
extern PLENS_TAB            lensDZ5311_getObject(void);

#endif
