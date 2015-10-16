/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanLCD.c
    @ingroup    mIPRJAPKey

    @brief      Detect LCD is rotated or not
                Detect LCD is rotated or not

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "GPIOMapping.h"

extern void LCD_Rotate(BOOL bRotate);

static BOOL bLCDRotate = TRUE;//FALSE;

/**
  Detect LCD is rotated or not

  Detect LCD is rotated or not.

  @param void
  @return void
*/
void KeyScan_DetLCDRotate(void)
{
    if (bLCDRotate)
    {
        if (!GPIOMap_DetLCDRotate())
        {
            //SysKickIdle();   //#NewUx porting: Temporarily marked for compile error
            bLCDRotate = FALSE;
            LCD_Rotate(FALSE);
        }
    }
    else
    {
        if (GPIOMap_DetLCDRotate())
        {
            //SysKickIdle();   //#NewUx porting: Temporarily marked for compile error
            bLCDRotate = TRUE;
            LCD_Rotate(TRUE);
        }
    }
}

/**
  Return LCD rotate status

  Return LCD rotate status.

  @param void
  @return BOOL: TRUE -> LCD is rotated, FALSE -> LCD is not rotated
*/
BOOL KeyScan_IsLCDRotate(void)
{
    return bLCDRotate;
}

//@}
