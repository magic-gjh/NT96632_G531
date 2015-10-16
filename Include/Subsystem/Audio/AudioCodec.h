/**
    Header file for Audio Codec module

    This file is the header file that define the API and data type
    for Audio Codec module.

    @file       AudioCodec.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _AUDIOCODEC_H
#define _AUDIOCODEC_H

#include "Audio.h"

/**
    @addtogroup mISYSAud
*/
//@{

// Get function pointer for default audio codec (By linking library)
extern void     audcodec_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);

// Add more supported extended audio codec here
// Extended audio codec for HDMI TX (Embedded or External)
extern void     audcodecHDMI_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);

//@}

#endif
