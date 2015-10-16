#ifndef _IQSETTING_OV5653_630_H
#define _IQSETTING_OV5653_630_H

#include    "ipe_lib.h"

//#NT#2011/01/27#JJ Huang -begin
#define IQ_VER_DVPLS_OV5653_0 0 // 2010.11.08 Get
#define IQ_VER_DVPLS_OV5653_1 1 // IQ From ADPK O7, Color From FDV350
#define IQ_VER_DVPLS_OV5653_2 2 // IQ Simulated by LiteArray Lens, compared with O6
#define IQ_VER_DVPLS_OV5653 IQ_VER_DVPLS_OV5653_2
//#NT#2011/01/27#JJ Huang -end

//#NT#2010/05/28#ethanlau -begin
typedef enum
{
    ISONUM0, //ISO100
    ISONUM1, //ISO200
    ISONUM2, //ISO400
//#NT#2009/11/28#JJ Huang -begin
    ISONUM3, //Cap ISO100
//#NT#2009/11/28#JJ Huang -end
    ISONUM4,
    ISONUM5,
    Total_ISONum
}IPE_ISONumEnum;
//#NT#2010/05/28#ethanlau -end

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
extern __align(4)UINT32 GammaLUT_128Tab[130];
extern __align(4)UW ChrCurveLUT[130];
extern __align(4)UH PreGeoLUT[66];
extern __align(4) unsigned char Shading_Table_PreF[20];
extern __align(4) unsigned char Shading_Table[20];
extern __align(4) unsigned char Shading_Table1280X480[20];
extern __align(4) unsigned char Shading_Table720p[20];
extern __align(4) unsigned char Shading_Table1080p[20];
//#NT#2009/08/12#Jarkko Chang -begin
//#NT#update vig function
extern __align(4) unsigned char Shading_Table_R[20];
extern __align(4) unsigned char Shading_Table_G[20];
extern __align(4) unsigned char Shading_Table_B[20];
//#NT#2009/08/12#Jarkko Chang -end
extern __align(4) unsigned char Shading_Table_Flash[20];
extern __align(4) unsigned char Shading_LineTable[20];
extern __align(4) IPE_COLOR_EFFECT_PARAM ColorEffectTable[Max_ImgEffect + 1];
//#NT#20091127#ethanlau -begin
extern __align(4) UINT32 Sharpness[Total_ISONum][Max_Sharp+1];
extern __align(4) UINT32 Saturation[Max_Saturation+1];
//#NT#20091127#ethanlau -end
//#NT#20091207#ethanlau -begin
extern __align(4) IME_FILT_PARA IMECHROMAPARAM[Total_ISONum];
//#NT#20091207#ethanlau -end
//#NT#20091209#ethanlau -begin
extern __align(4) UINT32 CHRFILTPARAM[Total_ISONum][4] ;
//#NT#20091209#ethanlau -end
//#NT#2010/09/28#ethanlau -begin
//#NT#2010/11/09#JJ Huang -begin
extern __align(4) UINT8 IPE_ddsTab[8];
extern __align(4) UINT8 IPE_hueTab[24];
extern __align(4) UINT8 IPE_intTab[24];
extern __align(4) UINT8 IPE_satTab[24];
//#NT#2010/11/09#JJ Huang -end

extern __align(4) STR_IPE_EDGE_KER_PARA DIS_Ker;
extern __align(4)IPE_ETH_PARAM DIS_EthParam;
#endif

