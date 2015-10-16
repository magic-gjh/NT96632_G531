/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SystemTask.c
    @ingroup    mIPRJAPCfg

    @brief      Start system task
                Start system task (KeyScan, Command, Lens, PlaySound...)

    @note       Nothing.

    @date       2005/12/28
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "SysCfg.h"
#include "SystemTask.h"
#include "UIFramework.h"
#include "UIBackgroundObj.h"

static UIFRAMEWORK_OBJ  uiFrameworkObj =
{
    UIFRAMEWORKTSK_ID,
    UIBACKGROUNDTSK_ID,
    FLG_ID_FRAMEWORK,
    SEMID_FRAMEWORK_WINDOW,
    SEMID_FRAMEWORK_DRAW,
    gBackgroundExtFuncTable
};

/**
  Start phase 1 tasks

  Start phase 1 tasks.
  Now are the tasks that should start before HW / FW initialization.

  @param void
  @return void
*/
void SysTask_StartTaskPhase1(void)
{
    // Start lens control task and play sound task...
//    LensCtrl_Open(NULL);
}

/**
  Start phase 2 tasks

  Start phase 2 tasks.
  Now are the tasks that should start after HW / FW initialization.

  @param void
  @return void
*/
void SysTask_StartTaskPhase2(void)
{
    // Start KeyScan task and Command Task
    sta_tsk(KEYSCANTSK_ID);
#if (_COMMANDTSK_MODE_ == ENABLE)
    sta_tsk(COMMANDTSK_ID);
#endif
    Ux_Open(&uiFrameworkObj);
}

//@}
