/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2005.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Sensor Common Header                                                   *
* Description:                                                             *
* Author: Adrian                                                               *
****************************************************************************/
#ifndef _SEN_COMMON_H
#define _SEN_COMMON_H
/** \addtogroup SEN_COMMON_Lib */
//@{


/***************************************************************************
* sen_common_630.c                                                         *
****************************************************************************/




//IPE_VaccTabUpdatedEnum: CPU_UpdatedIPEVarAccTab return value
enum IPE_VaccTabUpdatedEnum
{
    NO_TAB_UPDATED,
    TAb1_UPDATED,
    TAb1_TAb2_UPDATED,
    TAb3_UPDATED,
    TAb3_TAb4_UPDATED
};

enum DFSPICEnum
{
    DFS_FIRSTPIC,
    DFS_SECONDPIC
};

enum CapInfoEnum
{
    Cap_CFAPat,
    Cap_SyncMode,
    Cap_ChromaParam,
    Cap_MaxID
};

//#NT#Add IPP mode ID
enum IPPIMGMode
{
    IPPImg_Preview,
    IPPImg_Video,
    IPPImg_PCC,
    IPPImg_AF,
    IPPImg_BeforCap,
    IPPImg_DzoomEnter,
    IPPImg_DzoomLeave,
//#NT#20090912#ethanlau -begin
    IPPImg_SenModeChg,
//#NT#20090912#ethanlau -end
    IPPImg_MaxID
};

typedef enum
{
    SEN_MEM_SEPARATE_MODE = 0,
    SEN_MEM_COMBINE_MODE
} SEN_MEM_MODE;

#define AWBScaleBit 8
#define AFScaleBit  8
#define VAC_WIN8    8
#define VAC_WIN16   16
#define IPE_VAC_HNF_MINIMUN  2
#define PRE_VAC_HNF_MINIMUN  8
#define PRE_VAC_NNF_MINIMUN  16
#define DEFAULT_SHUTTER_TIME 31500


void SIEReset(void);
void IPEReset(void);
void IMEReset(void);
//--------------------------------------------------------------------------------------------------------------------------
void ipe_mp4_isr(void);
void ime_mp4_isr(void);
unsigned int GetImeFrameCnt(void);
void ResetImeFrameCnt(void);
//#NT#2009/12/03#Yuppie Lin -begin
//#NT#remove useless code
//void GetImgFBAddr(FBADDR_PRAM *pFBAddr_para);
//#NT#2009/12/03#Yuppie Lin -end
void GetActImgFBAddr(ACTFBADDR_PRAM *pActFBAddr_para);
void GetActImgFBAddr_Path1(ACTFBADDR_PRAM *pActFBAddr_para);
void GetActImgFBAddr_Path2(ACTFBADDR_PRAM *pActFBAddr_para);
//--------------------------------------------------------------------------------------------------------------------------
int TimerCountUp(unsigned char action);
//--------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------

void ide_Set2Preview(unsigned int    FB0_Addr,
                     unsigned int   FB1_Addr,
                     unsigned int   FB2_Addr,
                     unsigned int   Hsize,
                     unsigned int   Vsize,
                     unsigned int   LineOffset,
                     unsigned int   Format);

void ide_Set2Preview2(unsigned int    FB0_Addr,
                     unsigned int   FB1_Addr,
                     unsigned int   FB2_Addr,
                     unsigned int    Hsize,
                     unsigned int    Vsize,
                     unsigned int    OutHsize,
                     unsigned int    OutVsize,
                     unsigned int    LineOffset,
                     unsigned int   Format);

void ide_Set2Video(pIME_FBADDR_PRAM pFB_Para);
void ide_Set2Video2(pIME_FBADDR_PRAM pFB_Para);
void ide_SetSize(pIME_FBADDR_PRAM pFB_Para);
//--------------------------------------------------------------------------------------------------------------------------
void pre_Dram2Dram(SIE_PRE_PARAM *SiePreParam);
//--------------------------------------------------------------------------------------------------------------------------
void ipe_Set2Preview(IPE_PARAM *IpeParam);
ER ipe_Dram2Dram(IPE_PARAM *IpeParam);
//--------------------------------------------------------------------------------------------------------------------------
void ime_Set2Preview(pIME_FBADDR_PRAM  pIME_Para1);

void ime_SetDzoom2Preview(unsigned int    D_Addr,
                          unsigned int    S_Hsize,
                          unsigned int    S_Vsize,
                          unsigned int    D_Hsize,
                          unsigned int    D_Vsize,
                          unsigned int     LineOffset);
void ime_Set2Video  (pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2);
void ime_Set2VideoForModeS  (pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2);
void ime_Set2Capture(STR_YCBCR_ADDR in_addr,STR_YCBCR_ADDR out_addr,unsigned int in_format,unsigned int out_format,STR_SCALE_SIZE_INFO io_size,BOOL src);
//--------------------------------------------------------------------------------------------------------------------------
typedef void (*FPMPEG4PutSrcImgIdxCallBack)(void);
void SetupMPEG4CallBackFun(FPMPEG4PutSrcImgIdxCallBack pmp4callback);
//--------------------------------------------------------------------------------------------------------------------------

void TrgImeSlice(void);
UINT GetSlcBufAddr(void);
void jpg_SliceCfg(UINT JpgYAddr,UINT JpgCbAddr,UINT JpgCrAddr,UINT ImeHsize,UINT ImeVsize,UINT JpgOutputAddr,UINT JpgBufSize, UINT8 JpgQuality);

void Time2_Line_Pixel2(UINT32 TimeUs, UINT32 PixClkKHz, UINT32 HDTotal, UINT16 *Line, UINT16 *Pixel2);
//#NT#2010/05/28#ethanlau -begin
//move to imagealginfo
//void DzoomStop(void);
//void Set_DzoomFlag(unsigned char CDZoom);
//#NT#2010/05/28#ethanlau -end
void StopPreviewDramData(void);
void StartPreviewDramData(void);
void StopIPPForModeSwitch(void);
void ipe_SliceCfg(STR_STRIPE_INFOR *Hstripe,STR_STRIPE_INFOR *Vstripe,UW InAddr0,UINT cfapat,UINT ipe_omat, JobTypeEnum Job);
//#NT#2009/06/25#Jarkko Chang -begin
//#NT#add for Prv MultiStripe using
void ime_to_ipe_Stripe_trans(STR_STRIPE_INFOR *Hstripe,STR_STRIPE_INFOR *Vstripe);
//#NT#2009/06/25#Jarkko Chang -end
void GetIMEJPGAddr(UINT StartAddr,IME_FBADDR_PRAM* TmpAddr,UINT IME_OutHeight,UINT IME_OutWidth,UB OutFormat,UB OPPBnum);
void ime_SliceCfg(IME_FBADDR_PRAM *pIME_Para1,IME_FBADDR_PRAM *pIME_Para2, JobTypeEnum Job);
//#NT#2010/05/28#ethanlau -begin
//632 chromafilter
void ChromaFilter(UINT CbAddr, UINT CrAddr, UINT bufferAddr,UINT LineOffset, UINT Hsize, UINT Vsize, UINT *Chroma_Param,IME_FILT_PARA* ime_filt_para);
//#NT#2010/05/28#ethanlau -end
void SavePrvPRERaw(void);
void DisplayFBLock(UINT32 LockEn);
void ChromaCurve(UINT32 hSize, UINT32 vSize, UINT32 sourceAddr, UINT32 distAddr, UINT ipe_GaEn, UINT32 *chrSmLUT, UINT32 *rGaLUT, UINT32 *gGaLUT, UINT32 *bGaLUT);

typedef struct FLASHPARAM_STRUCT
{
    unsigned long PreAGC;
    unsigned long MainAGC;
    unsigned short FlshEna;
    unsigned short Polarity;
    unsigned short ExpChg;
    unsigned short AgcChg;
    unsigned short RedeyeEn;
    unsigned long DelayLines;
    unsigned long Delay2Pix;
    unsigned long PreWidth;
    unsigned long MainWidth;
    unsigned long FTP;
    unsigned long Prv_ExpoTime;
    unsigned long ChargeStatus;
    unsigned long PreAgcComp;

} FLASH_STRUCT;

#define      _SEN_CHECK_PARAM_       1
#if _SEN_CHECK_PARAM_
    #define SEN_ERRMSG(msg)  debug_err((msg))
    #define SEN_WARMSG(msg)  debug_wrn((msg))
    #define SEN_INDMSG(msg)  debug_ind((msg))
    #define SEN_CALMSG(msg)  debug_ind((msg))
    #define SEN_MSG(msg)     debug_ind((msg))
#else
    #define SEN_ERRMSG(msg)
    #define SEN_WARMSG(msg)
    #define SEN_INDMSG(msg)
    #define SEN_CALMSG(msg)  debug_ind((msg))
    #define SEN_MSG(msg)
#endif

//For TIFF FileFmt Header
typedef __packed struct
{
    UCHAR   Byte_Order[2];
    UCHAR   Version[2];
    UCHAR   IFD_Offset[4];
} IMAGE_FILE_HEADER;

typedef __packed struct
{
    UCHAR   Tag_Code[2];
    UCHAR   Tag_Type[2];
    UCHAR   Count[4];
    UCHAR   Value_Offset[4];
} IFD_Entry; //IMAGE_FILE_DIRECTORY Entry

typedef __packed struct
{
    IMAGE_FILE_HEADER   ImageFileHeader;
    UCHAR               IFD_Entry_Number[2];
    IFD_Entry           NewSubFileType;
    IFD_Entry           ImageWidth;
    IFD_Entry           ImageLength;
    IFD_Entry           BitsPerSample;
    IFD_Entry           Compression;
    IFD_Entry           PhotometricInterpretation;
    IFD_Entry           StripOffsets;
    IFD_Entry           SamplesPerPixel;
    IFD_Entry           RowsPerStrip;
    IFD_Entry           StripByteCounts;
    IFD_Entry           XResolution;
    IFD_Entry           YResolution;
    IFD_Entry           PlanarConfiguration;
    IFD_Entry           ResolutionUnit;
    UCHAR               NextIDFOffset[4];
    UCHAR               BitsPerSampleValue[3][2];
    UCHAR               StripOffsetsValue[3][4];
    UCHAR               StripByteCountsValue[3][4];
    UCHAR               XResolutionValue[8];
    UCHAR               YResolutionValue[8];
} RGB_IMAGE_TIFF_HEADER, *PRGB_IMAGE_TIFF_HEADER; // IFD for RGB full color image

typedef __packed struct
{
    IMAGE_FILE_HEADER   ImageFileHeader;
    UCHAR               IFD_Entry_Number[2];
    IFD_Entry           NewSubFileType;
    IFD_Entry           ImageWidth;
    IFD_Entry           ImageLength;
    IFD_Entry           BitsPerSample;
    IFD_Entry           Compression;
    IFD_Entry           PhotometricInterpretation;
    IFD_Entry           StripOffsets;
    IFD_Entry           SamplesPerPixel;
    IFD_Entry           RowsPerStrip;
    IFD_Entry           StripByteCounts;
    IFD_Entry           PlanarConfiguration;
    IFD_Entry           ReferenceBlackWhite;
    UCHAR               NextIDFOffset[4];
    UCHAR               BitsPerSampleValue[3][2];
    UCHAR               ReferenceBlackWhiteValue[6][8];
} JPEG_IMAGE_TIFF_HEADER, *PJPEG_IMAGE_TIFF_HEADER; // IFD for JPEG image

typedef struct
{
    UINT32  ImageHsize;
    UINT32  ImageVsize;
    UINT32  ImageByteCounts;
    UINT32  PlaneByteCounts;
} TIFF_Param, *PTIFF_Param;

#define FQV_SAMPLE_RATE     5
#define CAP_QV_PRE_H        640
#define CAP_QV_PRE_V        480
#define CAP_SCREEN_H        640//640
//#NT#2009/07/31#Jarkko Chang -begin
//#NT#fixed qv ratio issue
#define CAP_SCREEN_V        480//240//480
//#NT#2009/07/31#Jarkko Chang -end
#define CAP_QV_H            320
#define CAP_QV_V            240
#define CAP_THUMB_H         160
#define CAP_THUMB_V         120
#define CHROMA_BUF_NUM      2
#define SLICE_BUF_NUM       3
//#NT#2009/11/04#Yuppie Lin -begin
//#NT#Modify IME define, please refer to ime_lib.h
#define SLICE_MODE          IMEALG_64
//#NT#2009/11/04#Yuppie Lin -end
//#NT#2009/07/16#Jarkko Chang -begin
//#NT#modify for tv out issue
//#NT#2010/09/20#ethanlau -begin
//#NT#smaller P1 output size,only support 720p so far
//#define IME_PRV_MAX_SIZE (1440 * 1080 * 2 * 3) //MAX(LCD, NTSC, PAL) * FMT_422 * buf number
#define IME_PRV_MAX_SIZE (1280 * 720 * 2 * 3) //MAX(LCD, NTSC, PAL) * FMT_422 * buf number
//#NT#2010/09/20#ethanlau -end
//#NT#2009/07/16#Jarkko Chang -end
//#NT#2010/02/08#Connie Yu -begin
//#NT#Enlarge buffer size for DIS compensation
//#define IME_PRV_P2_MAX_SIZE (1440 * 1080 * 2 * 3) //MAX(LCD, NTSC, PAL) * FMT_422 * buf number
//#NT#2010/09/20#ethanlau -begin
//dis use 110 ratio, h.264 must use 420 format
#define IME_PRV_P2_MAX_SIZE (1584 * 1188 * 3/2 * 3) //MAX(LCD, NTSC, PAL) * FMT_422 * buf number
//#define IME_PRV_P2_MAX_SIZE (1728 * 1296 * 2 * 3) //MAX(LCD, NTSC, PAL) * FMT_422 * buf number
//#define IME_PRV_P2_MAX_SIZE (1920 * 1080 * 3/2 * 3) //MAX(LCD, NTSC, PAL) * FMT_422 * buf number
//#NT#2010/02/08#Connie Yu -end
//#NT#2010/09/20#ethanlau -end
#define JpgHeaderSize   (0x10000 + 0x250)
#define CAP_DEBUGINFO_SIZE   (4 * 1024)//(2 * 1024)
#define CAP_JPG_BSTM_MIN_SIZE  0x180000



void SetPCC2Cap(UINT *Addr, UINT *JPGsize);
void SetPCC2CapRawJpg(UINT *rawAddr, UINT *rawWidth, UINT *rawHeight, UINT *rawBitDepth, UINT *jpgAddr, UINT *jpgSize);

void Set_RGB_TIFF_Header(PTIFF_Param pTIFFparam, PRGB_IMAGE_TIFF_HEADER pRGB_TIFFhearder);
void Set_JPEG_TIFF_Header(PTIFF_Param pTIFFparam, PJPEG_IMAGE_TIFF_HEADER pJPEG_TIFFhearder);

//for quickview quality improvement
void    Y_LPF1( UINT Addr, UINT Hsize,UINT Vsize,UINT LineOffset);
void    Y_LPF2( UINT Addr, UINT Hsize,UINT Vsize,UINT LineOffset);
void    Y_LPF3( UINT Addr, UINT Hsize,UINT Vsize,UINT LineOffset, UINT BuffAddr);
UINT RawBinning(UINT SrcAddr, UINT SrcLineOffset, UINT Hsize, UINT Vsize, UB Mode);//  0:H_bin            1:H_bin + V_Bin
UINT YGaussianBlur(UINT YAddr, UINT LineOffset, UINT Hsize, UINT Vsize, UB  YRadius);// pixel
UINT CbCrGaussianBlur(UINT CbAddr, UINT CrAddr, UINT LineOffset, UINT Hsize, UINT Vsize, UB  CbRadius, UB  CrRadius);// pixel

void Sensor_OpenI2C(void);
void Sensor_CloseI2C(void);

void ime_SetDzoom2Video_Path1(pIME_FBADDR_PRAM  pIME_Para1);
BOOL CPU_WriteGammaTab(UW *pre_Gamma_R,UW *pre_Gamma_G,UW *pre_Gamma_B);
UINT32 CPU_UpdatedIPEVarAccTab(IPE_PARAM *IpeParam);

ER User_VABegin(AF_IPEINFO *AFInfo);
int  User_VAEnd(AF_IPEINFO *AFInfo);
ER  SetIPEVarAccInfo(STR_IMG_SIZE *InputSize, STR_VA_INFOR *VAInfo, const UINT32 *VA_Table);
void SubSampleRaw(UINT32 SourceAdd, UINT32 TargetAdd, UINT32 InH, UINT32 InV,UINT32 OutH, UINT32 OutV, UINT32 OutOfs, UINT32 FieldNum);
void SetImeVideoResolution(UINT Hsize,UINT Vsize, UINT BufNum);
void SetImePccResolution(UINT Hsize,UINT Vsize);
void ime_SetDzoom2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2);
UINT32 Set2Prv_Common(IPP_PARAM *IppParam_Prv);
//#NT#2010/09/11#ethanlau -begin
//separate parameter settting from Set2Prv_Common
void SetPrvParams(IPP_PARAM *IppParam_Prv);
//#NT#2010/09/11#ethanlau -end
void sie_Set2Dzoom(SIE_PRE_PARAM *SiePreParam);
void sie_Set2Preview(SIE_PRE_PARAM *SiePreParam);
void sie_Set2Capture(SIE_PRE_PARAM *SiePreParam);
UINT8 Sen_GetIPECfaPat(UINT8 OrgPat, UINT8 FlipType);
UINT32 Sen_GetPREAddrOfs(UINT8 FlipType, UINT32 PreOutVSize, UINT32 PreOutLineOfs, UINT32 PreOutBitDepth);
UINT32 Sen_GetIPEAddrOfs(UINT8 FlipType, UINT32 PreOutHSize, UINT32 PreOutLineOfs, UINT32 PreOutBitDepth);
UINT32 Sen_GetPREFlipParam(UINT8 FlipType);
//#NT#2009/11/02#Bowen Li -begin
//#NT#modify CA function for 2 & 3 line
ER Sen_GetCASubRatio(PRE_VGAFormatEnum VGAType, PreCA_SubsampleRate HSub, PreCA_SubsampleRate VSub, UINT32 *HRatio, UINT32 *VRatio);
UINT32 Sen_GetIntlvRatio(PRE_VGAFormatEnum VGAType);
//#NT#2009/11/02#Bowen Li -end
//#NT#2009/12/04#YC Peng -begin
//#NT#Add drive mode AdjFocusShot for adjust FF lens
extern void SenAdjFocusMode(void);
//#NT#2009/12/04#YC Peng -end
extern RGB_IMAGE_TIFF_HEADER RGB_TIFF_Header;
extern JPEG_IMAGE_TIFF_HEADER JPEG_TIFF_Header;
extern char message[];
extern BOOL MotionDetection(void);
//#NT#2013/02/05#Jarkko Chang -begin
//#NT# add motion detection from ime y for av in
BOOL MotionDetection_RAW(void);
BOOL MotionDetection_IME_Y(void);
//#NT#2013/02/05#Jarkko Chang -end
extern UINT32 DVISkipFrmEn;
//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
void AddVideoIQInfo( const char *format, ...);
char* GetVideoIQInfo(void);
void ResetVideoIQInfo(void);
//#NT#2011/10/14#Spark Chou -end

#endif
