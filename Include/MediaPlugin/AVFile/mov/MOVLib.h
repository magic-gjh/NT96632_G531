/**
    Header file of mov fileformat.

    Exported header file of mov fileformat.

    @file       movLib.h
    @ingroup    mIAVMOV
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _MOVLIB_H
#define _MOVLIB_H

#include "type.h"

/**
    @addtogroup mIAVMOV
*/
//@{

//timescale
#define MOV_TIMESCALE  600//old 1000

/*
    MOV_offset as a 32-bit pointer
*/
typedef UINT32  MOV_Offset;


/**
    @ name mov entry structure.

    Structure definition for a video/audio frame, including file offset, frame size,
    frame duration, and frame type.

    
*/
typedef struct 
{
    UINT32      pos;        ///< file offset of this frame
    UINT32      size;       ///< frame size
    //UINT32      duration;   ///< frame duration, used for av-sync
    UINT16      key_frame;  ///< frame type, whether key frame or not
    UINT8       updated;
    UINT8       rev;
} MOV_Ientry;



#endif//_MOVLIB_H

//@}