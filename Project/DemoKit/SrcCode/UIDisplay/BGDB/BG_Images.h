#ifndef _BG_IMAGES_H
#define _BG_IMAGES_H

#include "Type.h"

#define BG_MOVIE        DISABLE
#define BG_THUMBNAIL    ENABLE
//#NT#2007/05/16#Scottie -begin
//#Support showing bad file warning image
#define BG_BADFILE      DISABLE
//#NT#2007/05/16#Scottie -end

#if (BG_MOVIE)
#define BG_MOVIE_BS_SIZE    13591
extern const UINT8 g_ucBGMovie_320x240[];
#endif

#if (BG_THUMBNAIL)
#define BG_THUMB_BS_SIZE    15542
extern const UINT8 g_ucBGThumb_320x240[];
#endif

//#NT#2007/05/16#Scottie -begin
//#Support showing bad file warning image
#if (BG_BADFILE)
#define BG_BADFILE_BS_SIZE  4888
extern const UINT8 g_ucBGBadFile_320x240[];
#endif
//#NT#2007/05/16#Scottie -end

#define BG_OPENING_BS_SIZE  13598
extern const UINT8 g_ucBGOpening[];
#define BG_GOODBYE_BS_SIZE  20172
extern const UINT8 g_ucBGGoodbye[];
#define BG_GOD_BS_SIZE  41561
extern const UINT8 g_ucBGGOD[];


//#NT#2011/09/14#Philex Lin -begin
#define BG_WATERMARK_Y_SIZE  4096
extern const UINT8 g_ucWaterMarkY[];
#define BG_WATERMARK_UVPack_SIZE  2048
extern const UINT8 g_ucWaterMarkUVPack[];
//#NT#2011/09/14#Philex Lin -end
#endif