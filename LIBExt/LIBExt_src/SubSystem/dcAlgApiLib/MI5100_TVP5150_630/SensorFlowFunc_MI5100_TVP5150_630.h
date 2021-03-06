#ifndef SENSOR_FLOW_FUNC_MI5100_630_H
#define SENSOR_FLOW_FUNC_MI5100_630_H

typedef enum
{
    AE_PRV_INIT = 0,
    AE_PRV_SETTING,
    AE_PRV_GAIN_SETTING,
    AE_CAP_SETTING,
    AWB_PRV_INIT,
    AWB_PRV_SETTING,
    AWB_CAP_SETTING,
    PREFLASH_FLOW,
    PRE_PRV_SETTING,
    PRE_CAP_SETTING,
    PRE_D2D_SETTING,
    SIE_PRE_GET_CAP_RAW,
    IPE_PRV_SETTING,
    IPE_PRV_AE_SETTING,
    IPE_CAP_SETTING,
    IPE_D2D_SETTING,
    TG_INIT,
    AFE_INIT,
    SENSOR_MODE_SETTING,
    IPPFUNC_Switch,
    AAFD_Switch,
    IPE_PRV_AF_SETTING,
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
    IPL_SLEEP,
    IPL_WAKEUP,
//#NT#2009/10/08#Bowen Li -end
//#NT#2009/12/09#Jarkko Chang -begin
    IME_Chrom_Prv_SETTING,
    IME_Chrom_Prv_AE_SETTING,
    IME_Chrom_Cap_SETTING,
//#NT#2009/12/09#Jarkko Chang -end
    SEN_FLOW_MSG_NUM
}SEN_FLOW_MSG;

typedef struct _SEN_CB_INFO {
    UINT32 IPPImgMode;
    struct  PUBLIC_STR_JOBREQS *IppMgrInfo;
    SIE_PRE_PARAM *SiePreCapInfo;
    SIE_PRE_PARAM *SiePrePrvInfo;
    IPE_MODEINFO *IPEModeInfo;
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
    IPL_SLEEP_LEVEL IplSleepLevel;
//#NT#2009/10/08#Bowen Li -end
} SenCBInfo;

extern SenCBInfo gSenCBInfo;

void Sen_FlowFunc(SEN_FLOW_MSG MsgID, SenCBInfo *SenInfo);
#define SenFlowFunc(a, b) Sen_FlowFunc(a, b);
#endif //SENSOR_FLOW_FUNC_H


