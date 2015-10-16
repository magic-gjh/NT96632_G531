/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensCmd.c
    @ingroup    mISYSLens

    @brief      Lens Test command functions

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lens_int.h"
#include "LensCmd.h"
#include "LensAPI.h"
#include "Utility.h"
#include "gpio.h"
#include "pad.h"
#include "sif.h"

// ----------------------------------------------------------------------------
// Globals & Definitions
// ----------------------------------------------------------------------------
// Lens model id list
#define LENSMID_FF              0
#define LENSMID_ST8002          1
#define LENSMID_VM18001         2
#define LENSMID_DZ5311          3
#define LENSMID_RSV3            4
#define LENSMID_UNKNOWN         0xFF

// Motor model id list
#define MOTORMID_PSEUDO         0
#define MOTORMID_A3901          1
#define MOTORMID_MD152          2
#define MOTORMID_AT5510         3
#define MOTORMID_R2A30423       4
#define MOTORMID_UNKNOWN        0xFF

// Lens cmd user current using lens/motor model id
// If user do not need lens commands, please set as unknown state,
// to save image size after lib linking!
#define USER_LENSMID            LENSMID_DZ5311
#define USER_MOTORMID           MOTORMID_R2A30423

// ----------------------------------------------------------------------------
// Static Variables & Functions
// ----------------------------------------------------------------------------
static LENS_CMD_LENSMODEL gLensModelTbl[] =
{
    { LENSMID_FF,               "FF"            },
    { LENSMID_ST8002,           "ST8002"        },
    { LENSMID_VM18001,          "VM18001"       },
    { LENSMID_DZ5311,           "DZ5311"        },
    { LENSMID_RSV3,             "RSV3"          },


    { LENSMID_UNKNOWN,          "UNKNOWN"       }
};

static LENS_CMD_MOTORMODEL gMotorModelTbl[] =
{
    { MOTORMID_PSEUDO,          "PSEUDO"        },
    { MOTORMID_A3901,           "A3901"         },
    { MOTORMID_MD152,           "MD152"         },
    { MOTORMID_AT5510,          "AT5510"        },
    { MOTORMID_R2A30423,        "R2A30423"      },


    { MOTORMID_UNKNOWN,         "UNKNOWN"       }
};

static CHAR *Lens_Cmd_GetLensModelString(UINT32 uiModelID);
static CHAR *Lens_Cmd_GetMotorModelString(UINT32 uiModelID);

// ----------------------------------------------------------------------------
// Lens Command APIs
// ----------------------------------------------------------------------------
static CHAR *Lens_Cmd_GetLensModelString(UINT32 uiModelID)
{
    UINT32 uiTotal = (sizeof(gLensModelTbl)/sizeof(LENS_CMD_LENSMODEL));
    UINT32 i = 0;

    while(i < uiTotal)
    {
        if(gLensModelTbl[i].uiLensModelID == uiModelID)
        {
            return gLensModelTbl[i].pcLensModelStr;
        }
        i++;
    }
    debug_err(("LENS: Unknown lens model id %d\r\n", uiModelID));
    return NULL;
}

static CHAR *Lens_Cmd_GetMotorModelString(UINT32 uiModelID)
{
    UINT32 uiTotal = (sizeof(gMotorModelTbl)/sizeof(LENS_CMD_MOTORMODEL));
    UINT32 i = 0;

    while(i < uiTotal)
    {
        if(gMotorModelTbl[i].uiMotorModelID == uiModelID)
        {
            return gMotorModelTbl[i].pcMotorModelStr;
        }
        i++;
    }
    debug_err(("LENS: Unknown motor model id %d\r\n", uiModelID));
    return NULL;
}

// ----------------------------------------------------------------------------
// Lens User Command List
// ----------------------------------------------------------------------------
void Lens_Cmd(CHAR *pcCmdStr)
{
    static PLENS_TAB pLensTab = NULL;
    static PMOTOR_TAB pMotorTab = NULL;
    static BOOL bLensInit = FALSE;
    INT32 status;

    switch (*pcCmdStr)
    {
    /* Init */
    case 'i':
        if (!strncmp(pcCmdStr, "init", 4))
        {
            LENS_DEVICE_OBJ LensDevice;
            UINT32          uiBufGPIO[LENS_IO_MAX];
            UINT32          uiBufPAD[LENS_PAD_MAX];

            debug_err(("lens init\r\n"));
            debug_err(("LENS: LensDrv (%s), MotorDrv (%s)\r\n",
            Lens_Cmd_GetLensModelString(USER_LENSMID),
            Lens_Cmd_GetMotorModelString(USER_MOTORMID)));

            LensDevice.puiMotorCtrlGPIOArray                    = &uiBufGPIO[0];
            LensDevice.puiMotorCtrlPADArray                     = &uiBufPAD[0];

            //Lens
            #if (USER_LENSMID == LENSMID_FF)
            pLensTab = lensFF_getObject();
            #elif (USER_LENSMID == LENSMID_DZ5311)
            pLensTab = lensDZ5311_getObject();
            #else
            //Do nothing
            #endif

            //Motor
            #if (USER_MOTORMID == MOTORMID_PSEUDO)
            pMotorTab = motorPSEUDO_getObject();
            #elif (USER_MOTORMID == MOTORMID_R2A30423)
            LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_SIF|MOTOR_CTRL_GPIO;

#if 1
            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_CONTROLLER;
            LensDevice.LensSIFPinObj.uiSIFChannel               = SIF_CH1;
#else
            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_GPIOSIM;
            LensDevice.LensSIFPinObj.uiSIFChannel               = SIF_CH1;
            LensDevice.LensSIFPinObj.uiGPIOSen                  = P_GPIO_10;
            LensDevice.LensSIFPinObj.uiGPIOClk                  = P_GPIO_11;
            LensDevice.LensSIFPinObj.uiGPIOData                 = P_GPIO_12;
#endif

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = (D_GPIO_1|GPIO_IS_DGPIO);

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = L_GPIO_11;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_2]    = C_GPIO_48;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR]   = C_GPIO_50;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI]  = C_GPIO_49;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_EXT1]  = C_GPIO_33;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_EXT2]  = C_GPIO_34;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_MOB1]  = C_GPIO_17;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_MOB2]  = P_GPIO_35;//C_GPIO_32;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_WIDE_INT]  = GPIO_INT_12;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_TELE_INT]  = GPIO_INT_11;

            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR]   = PAD_PIN_MC50;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI]  = PAD_PIN_MC49;

            pMotorTab = motorR2A30423_getObject();
            #else
            //Do nothing
            #endif

            Lens_Module_Init(&LensDevice, pLensTab, pMotorTab);
            bLensInit = TRUE;
        }
        else if (!strncmp(pcCmdStr, "info", 4))
        {
            debug_err(("power = %d\r\n", gpio_getPin(D_GPIO_1|GPIO_IS_DGPIO)));
            debug_err(("clk en = %d\r\n", gpio_getPin(L_GPIO_11)));
            debug_err(("reset = %d\r\n", gpio_getPin(C_GPIO_48)));
            debug_err(("zoom PR = %d\r\n", pMotorTab->zoom_checkPR()));
            debug_err(("focus PI = %d\r\n", pMotorTab->focus_checkPI()));
        }

        break;

    /* Focus */
    case 'f':
        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "fh", 2))
        {
            debug_err(("lens focus home\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            pLensTab->focus_gotoHome();

            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "ffw", 3))
        {
            debug_err(("lens focus fw\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            //pLensTab->focus_gotoPosition(pLensTab->focus_getMaxPosition());
            pMotorTab->focus_setState(MOTOR_FOCUS_FWD, 256);

            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "fbw", 3))
        {
            debug_err(("lens focus bw\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            //pLensTab->focus_gotoPosition(0);
            pMotorTab->focus_setState(MOTOR_FOCUS_BWD, 256);

            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "ffull", 5))
        {
            int i;

            Perf_Open();
            Perf_Mark();

            debug_err(("lens focus full range\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            for(i=0; i<=pLensTab->focus_getMaxPosition(); i++)
            {
                pLensTab->focus_gotoPosition(i);
            }

            pMotorTab->power(MOTOR_POWER_OFF);

            debug_err(("Execution Time: %d ms\r\n",Perf_GetDuration()/1000));
            Perf_Close();
        }
       else if (!strncmp(pcCmdStr, "fpi", 3))
        {
            UINT32 test;
            pMotorTab->power(MOTOR_POWER_ON);
            test = pMotorTab->focus_checkPI();
            debug_err(("focus pi = %d\r\n",test));
            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "ftest", 5))
        {
            UINT32 i;
            UINT32 step;
            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            sscanf((pcCmdStr+6), "%d", &step);
            debug_err(("lens test step = %d\r\n",step));

            for(i = 0;i<20;i++)
            {
                debug_err(("\n\rlens focus test[%d]\r\n",i));
                pMotorTab->focus_setState(MOTOR_FOCUS_BWD, step);
            }
            pMotorTab->focus_setState(MOTOR_FOCUS_FWD, (step*20));
            pMotorTab->power(MOTOR_POWER_OFF);
        }
        break;

    /* Aperture */
    case 'a':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "ab", 2))
        {
            debug_err(("lens aperture big\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_APERTURE);

            pLensTab->aperture_gotoState(LENS_APERTURE_BIG);

            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "as", 2))
        {
            debug_err(("lens aperture small\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_APERTURE);

            pLensTab->aperture_gotoState(LENS_APERTURE_SMALL);

            pMotorTab->power(MOTOR_POWER_OFF);
        }

        break;

    /* Shutter */
    case 's':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "so", 2))
        {
            debug_err(("lens shutter open\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_SHUTTER);

            pLensTab->shutter_gotoState(LENS_SHUTTER_OPEN);

            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "sc", 2))
        {
            debug_err(("lens shutter close\r\n"));

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_SHUTTER);

            status = pLensTab->shutter_gotoState(LENS_SHUTTER_CLOSE);

            if(status != ERR_OK_TIMER_POWEROFF)
                pMotorTab->power(MOTOR_POWER_OFF);
        }

        break;

    /* Zoom */
    case 'z':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "zfw", 3))
        {
            debug_err(("lens zoom fw\r\n"));

            Lens_Zoom_EnableIsr(FALSE);

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_ZOOM);
            pMotorTab->zoom_setState(MOTOR_ZOOM_FWD, 300);
            TimerDelayMs(100);
            pMotorTab->zoom_setState(MOTOR_ZOOM_OFF, 0);
            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "zbw", 3))
        {
            debug_err(("lens zoom bw\r\n"));

            Lens_Zoom_EnableIsr(FALSE);

            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_ZOOM);
            pMotorTab->zoom_setState(MOTOR_ZOOM_BWD, 300);
            TimerDelayMs(100);
            pMotorTab->zoom_setState(MOTOR_ZOOM_OFF, 0);
            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "zinit1", 6))
        {
            Lens_Zoom_Init(LENS_ZOOM_INIT_PART1);
        }
        else if (!strncmp(pcCmdStr, "zinit2", 6))
        {
            Lens_Zoom_Init(LENS_ZOOM_INIT_PART2);
        }
        else if (!strncmp(pcCmdStr, "zre", 3))
        {
            Lens_Zoom_Retract();
        }
        else if (!strncmp(pcCmdStr, "zpr", 3))
        {
            UINT32 test;
            pMotorTab->power(MOTOR_POWER_ON);
            test = pMotorTab->zoom_checkPR();
            debug_err(("zoom pr = %d\r\n",test));
            pMotorTab->power(MOTOR_POWER_OFF);
        }
        else if (!strncmp(pcCmdStr, "ztest", 5))
        {
            UINT32 i;
            UINT32 step;
            pMotorTab->power(MOTOR_POWER_ON);
            pMotorTab->changeMode(MOTOR_MODE_ZOOM);

            sscanf((pcCmdStr+6), "%d", &step);
            debug_err(("lens test step = %d\r\n",step));

            for(i = 0;i<20;i++)
            {
                debug_err(("\n\rlens zoom test[%d]\r\n",i));
                pMotorTab->zoom_setState(MOTOR_ZOOM_BWD, step);
                pMotorTab->zoom_setState(MOTOR_ZOOM_OFF, 0);
            }
            pMotorTab->zoom_setState(MOTOR_ZOOM_FWD, step*20);
            pMotorTab->zoom_setState(MOTOR_ZOOM_OFF, 0);
            pMotorTab->power(MOTOR_POWER_OFF);
        }
        break;
    }
}
