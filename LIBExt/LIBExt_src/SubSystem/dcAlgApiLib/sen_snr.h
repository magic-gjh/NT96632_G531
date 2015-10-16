//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
#include "type.h"
//#NT#2010/12/15#JJ Huang -end


//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
typedef enum
{
    SenSNR_DATA_MEAN_CH0,
    SenSNR_DATA_MEAN_CH1,
    SenSNR_DATA_MEAN_CH2,
    SenSNR_DATA_MEAN_CH3,
    SenSNR_DATA_SNR_CH0,
    SenSNR_DATA_SNR_CH1,
    SenSNR_DATA_SNR_CH2,
    SenSNR_DATA_SNR_CH3,
    SenSNR_DATA_UNIFORMITY_C0,
    SenSNR_DATA_UNIFORMITY_C1,
    SenSNR_DATA_UNIFORMITY_C2,
    SenSNR_DATA_UNIFORMITY_C3,
    SenSNR_DATA_UNIFORMITY_C4,
    SenSNR_DATA_WEIGHTED_SNR,
    SenSNR_DATA_WEIGHTED_MEAN,
    SenSNR_DATA_WEIGHTED_NONUNIFORMITY,
    ENUM_DUMMY4WORD(SenSNR_DATA)
} SenSNR_DATA;
//#NT#2010/12/15#JJ Huang -end

#define SenSNR_UNITY (1000)

#define SenSNR_DSP_PREPARE_ON (1)

//#NT#2010/12/15#JJ Huang -begin
//#NT#For SNR Measure
extern UINT32 SenSNR_EnableSNRMeasure(BOOL bEn);
extern UINT32 SenSNR_IsSNRMeasureEnable(void);
extern UINT32 SenSNR_GetSNRData(SenSNR_DATA uiIndex);
extern UINT32 SenSNR_Calculate_SNR(UINT32 RawAdd,UINT32 ImgHsize,UINT32 ImgVsize,UINT32 CfaPat,UINT32 BitDepth,UINT32 BlockH,UINT32 BlockV, UINT32 *puiMean, UINT32 *puiSNR);
//#NT#2010/12/15#JJ Huang -end

