
#ifndef _JOBMANAGER_H_
#define _JOBMANAGER_H_

#include    "Type.h"
#include    "imageAlgInfor.h"
#include    "sie_lib.h"
#include    "pre_lib.h"
#include    "ipe_lib.h"
#include    "ime_lib.h"

#define THREE_LINE_DUMMY_PIX            6
#define PADDING_REPEATING_PIX           16
#define LAST_ONE_LINE_OMITT             1


/*--------------------------------------*/
/*   IPP system interface return code defination   */
/*--------------------------------------*/
#define IPP_OFFSET          (-256)
#define IPP_OK              (0)                       // IPP ok
#define IPP_FAIL            ((-1)+ IPP_OFFSET)        // IPP fail
#define IPP_NOMORE_SPACE    ((-2)+ IPP_OFFSET)        // IPP no memory
#define IPP_NOT_EXIST       ((-3)+ IPP_OFFSET)        // IPP ID Not exist

typedef enum
{
    DUMMY_JOB,
    PREVIEW_JOB,
    VIDEO_JOB,
    CAPTURE_JOB,
    PRE_DRAM_RAWIMGIN_JOB,
    IPE_DRAM_RAWIMGIN_JOB,
    IPE_IME_FRAME_1PATH_JOB,
    IPE_IME_FRAME_2PATH_JOB,
    IPE_IME_FRAME_DVI_JOB
}JobTypeEnum;


enum LinesPerHdEnum
{
    ZERO_LINE,
    ONE_LINE,
    TWO_LINE,
    THREE_LINE
};

enum PRE_DFSOPSel
{
    AMinusB,
    BMinusA,
    APlusB,
    AMultiplyB
};

enum PRE_DFSTHSrc
{
    SrcDram,
    SrcSie
};

typedef struct SIE_PRE_PARAM_STRUCT
{
    UINT32 RawCenterX;
    UINT32 RawCenterY;
    UINT32 SiePreAddr0;
    UINT32 SiePreAddr1;
    UINT32 SiePreAddr2;
    UINT32 ModeSel;
    UINT32 VDTotal;
    UINT32 HDTotal;
    UINT32 VDSync;
    UINT32 HDSync;
    UINT32 HDActX;
    UINT32 HDActY[8];

    UINT32 PreCropSizeH;
    UINT32 PreCropSizeV;
    UINT32 PreOutSizeH;
    UINT32 PreOutAdd0;
    UINT32 PreOutAdd1;
    UINT32 PreOutLineOfs;
    UINT32 PreInLineOfs;
    UINT32 PreDramInSizeH;
    UINT32 PreDramInSizeV;
    PRE_InPackBitSelEnum PreInBitDepth;
    PRE_OutPackBitSelEnum PreBitDepth;
    UINT32 PreBp1;
    UINT32 PreIntEn;
    SIE_OutPackBitSelEnum SieBitDepth;
    UINT32 SieOutAdd0;
    UINT32 SieOutAdd1;
    UINT32 SieOutLineOfs;// Output
    UINT32 SieOutSizeV;// Output
    UINT32 SieIntEn;
    UINT32 SieBp1;
    UINT32 SieBp2;
    UINT32 SieMaster;
    UINT32 SieHDPhase;
    UINT32 SieVDPhase;
    UINT32 SieDataPhase;
    PRE_VGAFormatEnum VGAType;
    UINT32 AutoObEn;
    UINT32 SieObOfs;
    UINT32 SieObGain;
    STR_OB_Param SieObParam;
    BOOL   IsPadding;
    SIE_OutDestEnum  SieOutDest;
    PRE_OutputPathEnum PreSrcSel;
    STR_PRE_VA_INFOR    VaccIn;
    UINT32 MShutterTime;
    UINT32 PreFuncEn;
    UINT32 PreCfaPat;
    UINT32 PreCfaPat2;
    UINT32 SieHDInv;
    UINT32 SieVDInv;
    STR_FLDNUM SieFieldNum;
    //#NT#20090706#ETHAN LAU -BEGIN
    UINT32 Bining;
//#NT#20090706#ETHAN LAU -END
} SIE_PRE_PARAM;

typedef struct IPP_PARAM_STRUCT
{
    SIE_PRE_PARAM *Sie_Struct;
    IPE_PARAM *Ipe_Struct;
    IME_FBADDR_PRAM *Ime_Struct;
} IPP_PARAM;

typedef struct SIE_PRE_INFOR_STRUCT
{

    UINT32                          PreOutVsize;
    UINT32                          PreScaleOutH;
}SIE_PRE_INFOR;

struct PUBLIC_STR_JOBREQS {

    UINT32                          FunType;

    SIE_PRE_PARAM                   *SIE_PRE_PRV;
    SIE_PRE_PARAM                   *SIE_PRE_CAP;
    SIE_PRE_PARAM                   *SIE_PRE_D2D;

    STR_PRE_GAINCTRL_INFOR          pre_gainctrl_A;
    STR_PRE_GAINCTRL_INFOR          pre_gainctrl_B;
    STR_PRE_CG_INFOR                pre_cg_ofs;
    STR_PRE_VIG_INFOR               pre_vig_setting;
    STR_PRE_VIGLINE_INFOR           pre_vigline_setting;
    STR_PRE_GEO_INFOR               pre_geo_setting;
    STR_PRE_DFS_INFOR               pre_dfs_setting;
    //#NT#2009/10/16#Jarkko Chang -begin
    STR_PRE_CA_INFOR                pre_ca_setting;
    //#NT#2009/10/16#Jarkko Chang -end
    UINT32                          *pre_gamma_r;
    UINT32                          *pre_gamma_g;
    UINT32                          *pre_gamma_b;

    struct SIE_PRE_INFOR_STRUCT     PRV_INFO;
    struct SIE_PRE_INFOR_STRUCT     CAP_INFO;


    IPE_PARAM *IpeParam;
    IME_FBADDR_PRAM *ImeParam1;
    IME_FBADDR_PRAM *ImeParam2;
};

extern int Sensor_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int Sensor_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int Sensor_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int Sensor_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int SIE_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int SIE_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int SIE_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int SIE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int PRE_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int PRE_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int PRE_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int PRE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int IPE_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IPE_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IPE_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IPE_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);

extern int IME_BeginIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IME_AllocatResourceIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IME_ExecIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);
extern int IME_EndIPP(struct PUBLIC_STR_JOBREQS *job_req, JobTypeEnum JobType);


extern struct PUBLIC_STR_JOBREQS  IPPMgrReq;
extern IPE_MODEINFO IPEModeInfo;
extern SIE_PRE_PARAM SIE_PRE_Para_prv;
extern SIE_PRE_PARAM SIE_PRE_Para_cap;
extern SIE_PRE_PARAM SIE_PRE_Para_d2d;
extern IPE_PARAM IPE_Para;
//#NT#2009/10/02#SA4 -begin
extern IME_FBADDR_PRAM IME_D1_Para,IME_D2_Para;
//#NT#2009/10/02#SA4 -end
extern IPP_PARAM Ipp_Param;
//#NT#2009/06/25#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
extern STR_IME_STRIPE_INFOR    GetStripe;
extern STR_STRIPE_INFOR    IpeHstipe,IpeVstipe;
//#NT#2009/06/25#Jarkko Chang -end
extern IME_MODE_PRAM IMEParam;
#endif
