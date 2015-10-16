#ifndef _UI_RESORUCE_H_
#define _UI_RESORUCE_H_
#include "GxGfx.h"
#include "DemoKit_String.h"
#include "DemoKit_Image.h"
#include "DemoKit_Font.h"
#include "DemoKit_Palette.h"


extern PALETTE_ITEM gMenuBluePalette[];
extern PALETTE_ITEM gMenuGreenPalette[];
extern PALETTE_ITEM gMenuRedPalette[];

//reserved
#define _OSD_INDEX_TRANSPART             CLRID_IDX_TRANSPART
#define _OSD_INDEX_BLACK                 CLRID_IDX_BLACK
#define _OSD_INDEX_WHITE                 CLRID_IDX_WHITE
#define _OSD_INDEX_RED                   CLRID_IDX_RED
#define _OSD_INDEX_DARKBLUE              CLRID_IDX_DARKBLUE
#define _OSD_INDEX_BLUE                  CLRID_IDX_BLUE
#define _OSD_INDEX_GRAY                  CLRID_IDX_WHITE // temporary; no GRAY color in current palette
#define _OSD_INDEX_YELLOW                CLRID_IDX_YELLOW
#define _OSD_INDEX_BLACK50               CLRID_IDX_BLACK50
#define _OSD_INDEX_GREEN                 CLRID_IDX_GREEN

#endif //_UI_RESORUCE_H_
