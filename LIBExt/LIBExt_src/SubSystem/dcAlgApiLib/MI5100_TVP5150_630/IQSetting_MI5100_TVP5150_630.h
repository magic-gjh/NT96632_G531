#ifndef _IQSETTING_MI5100_630_H
#define _IQSETTING_MI5100_630_H

#include    "ipe_lib.h"

typedef enum
{
    ISONUM0,
    ISONUM1,
    ISONUM2,
    ISONUM3,
    ISONUM4,
    ISONUM5,
    ISONUM6,
    Total_ISONum
}IPE_ISONumEnum;

typedef struct
{
    STR_IPE_NOVANR2_PARA *NRParamSel;
    UINT8 *NRStabSel;
    UINT8 *NRDtabSel;
    UINT8 *FStabSel;
    UINT8 *FDtabSel;
    STR_IPE_EDGE_ENHMAP_PARA *EdgeParamSel;
    STR_IPE_EDGE_KER_PARA *EdgeKerDivSel;
    UINT8 *EmapSel;
    UINT8 *EstabSel;
} IPE_ISO_PARAM, *PIPE_ISO_PARAM;

extern __align(4) STR_IPE_EDGE_KER_PARA IPE_edgeKernelDiv[Total_ISONum][11];
extern __align(4) STR_IPE_EDGE_KER_PARA IPE_AFedgeKernelDiv[11];
extern __align(4) UINT8 IPE_nrStab[Total_ISONum][16];
extern __align(4) UINT8 IPE_nrDtab[Total_ISONum][16];
extern __align(4) UINT8 IPE_emStab[Total_ISONum][16];
extern __align(4) UINT8 IPE_fmStab[Total_ISONum][16];
extern __align(4) UINT8 IPE_fmDtab[Total_ISONum][16];
extern __align(4) UINT8 IPE_emap16tab[Total_ISONum][16];

extern __align(4) STR_IPE_NOVANR2_PARA IPE_NRParam[Total_ISONum][16];
extern __align(4) STR_IPE_EDGE_ENHMAP_PARA IPE_EdgeParam[Total_ISONum][5];

extern IPE_ISO_PARAM  IPE_ISOParam[Total_ISONum];

extern __align(4) UINT CHR_Param[5][28];

extern __align(4)INT16 Normal_CC[9];
extern __align(4)INT16 Indenity_CC[9];
extern __align(4)INT16 Normal_CC_Video[9];
extern __align(4)UINT32 GammaLUT_128Tab[130];
extern __align(4)UINT32 GammaLUT_Vid_128Tab[130];

extern __align(4)UW ChrCurveLUT[130];
extern __align(4)UH PreGeoLUT[66];
extern __align(4) unsigned char Shading_Table_PreF[20];
extern __align(4) unsigned char Shading_Table[20];
//#NT#2009/08/12#Jarkko Chang -begin
//#NT#update vig function
extern __align(4) unsigned char Shading_Table_R[20];
extern __align(4) unsigned char Shading_Table_G[20];
extern __align(4) unsigned char Shading_Table_B[20];

extern __align(4) unsigned char Shading_Table_R_MD2[20];
extern __align(4) unsigned char Shading_Table_G_MD2[20];
extern __align(4) unsigned char Shading_Table_B_MD2[20];

extern __align(4) unsigned char Shading_Table_R_CAP[20];
extern __align(4) unsigned char Shading_Table_G_CAP[20];
extern __align(4) unsigned char Shading_Table_B_CAP[20];
//#NT#2009/08/12#Jarkko Chang -end
extern __align(4) unsigned char Shading_Table_Flash[20];
extern __align(4) unsigned char Shading_LineTable[20];
extern __align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1];
//#NT#2009/12/02#Jarkko Chang -begin
extern __align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1];
//#NT#2009/12/02#Jarkko Chang -end
//#NT#2009/12/09#Jarkko Chang -begin
extern __align(4) IME_FILT_PARA IMECHROMAPARAM[Total_ISONum];
//#NT#2009/12/09#Jarkko Chang -end
#endif

