/*
    FDE module driver

    NT96630 FDE driver extern header file.

    @file       fde_lib.h
    @ingroup    mIIPPFDE
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _FDE_LIB_H
#define _FDE_LIB_H

#define FDE_INTE_ALL                    0x00000001  //bit 0
#define FDE_INTE_SIZE                   0x00000002  //bit 1
#define FDE_INTE_16                     0x00000004  //bit 2
#define FDE_INTE_OFAIL                  0x00000008  //bit 3
#define FDE_INT_ALL                     0x00000001  //bit 0
#define FDE_INT_SIZE                    0x00000002  //bit 1
#define FDE_INT_16                      0x00000004  //bit 2
#define FDE_INT_OFAIL                   0x00000008  //bit 3

#define FACE_NUM_MAX                    36

/**
    FDE FD Function Selection
*/
typedef enum
{
    FDE_FD_FACEDETECTION    = 0,
    FDE_FD_EDGE_EXTRACTION  = 1,
    FDE_FD_SCALING          = 2
}_FDE_FDFUNC;

/**
    FDE FD OPeration Mode Selection
*/
typedef enum
{
    FDE_OPMODE_FD       = 0,
    FDE_OPMODE_LUMIN    = 1,
    FDE_OPMODE_EDGH     = 2,
    FDE_OPMODE_EDGV     = 3,
    FDE_OPMODE_EDGHV0   = 4,
    FDE_OPMODE_EDGHV1   = 5
}_FDE_FDOPMODE;

/**
    FDE FD Scaling Direction
*/
typedef enum
{
    FDE_HORIZONTAL  = 0,
    FDE_VERTICAL    = 1
}_FDE_FDDIRECTION;

/**
    FDE OPP En/Disable Selection
*/
typedef enum
{
    FDE_OPPDISABLE   = 0,
    FDE_OPPENABLE    = 1,
    FDE_OPPSEQUEN    = 2
}_FDE_FDOPP;


/*
    FDE Result Selection
*/
typedef enum
{
    FDE_RSLT_NONOVERLAP   = 0,
    FDE_RSLT_ORIGINAL     = 1
}_FD_RSLT_OVLP;

/**
    @name Struct FDE open object.
*/
typedef struct _FDE_OPENOBJ{
    void (*FP_FDEISR_CB)(UINT32 intstatus);
} FDE_OPENOBJ;

/**
    @name Struct FDE Result Lowpass
*/
typedef struct _FDE_LPF_PRAM
{
    BOOL    enable;         ///< enable result lowpass
    UINT32  shift;          ///< result window moving speed
    UINT32  overdue;        ///< result window overdue
} FDE_LPF_PRAM;

/**
    @name Struct FDE target threshold
*/
typedef struct _FDE_FDE_THPRAM
{
    UINT32  weight0;        ///< channel0 weighting
    UINT32  weight1;        ///< channel1 weighting
    UINT32  weight2;        ///< channel2 weighting
    UINT32  score_shf;      ///< score shift
    UINT32  score_th;       ///< score threshold
    UINT32  std_th;         ///< std threshold
    UINT32  chan_th0;       ///< channel threshold 0
    UINT32  chan_th1;       ///< channel threshold 1
    UINT32  chan_th2;       ///< channel threshold 0
}FDE_FDE_THPRAM;

/**
    @name Struct FDE Information
*/
typedef struct _FDE_FDE_PRAM
{
    UINT32  in_addr;        ///< input starting address
    UINT32  out_addr0;      ///< output starting address 0
    UINT32  out_addr1;      ///< output starting address 1
    UINT32  out_addr2;      ///< output starting address 2
    UINT32  width;          ///< image width
    UINT32  height;         ///< image height
    UINT32  lineoffset;     ///< image lineoffset
    _FDE_FDOPMODE  opmode;  ///< operation mode
    UINT32  sclnum;         ///< scaling number
    UINT32  shf0;           ///< channel0 shift
    UINT32  shf1;           ///< channel1 shift
    UINT32  shf2;           ///< channel2 shift
    UINT32  hfact;          ///< horizontal scaling factor
    UINT32  vfact;          ///< vertical scaling factor
    FDE_FDE_THPRAM *trgth;  ///< target threshold
    INT16   *target;        ///< face target
    _FDE_FDOPP   opp;       ///< output ping-pong buffer enable
    UINT8   orient_pri;     ///< orientation piority
    FDE_LPF_PRAM rslt_lpf;  ///< result low pass parameter
} FDE_FDE_PRAM;


/**
    @name Struct FDE Location and Score
*/
typedef struct _FACE
{
    UINT16  stx;            ///< result start x
    UINT16  sty;            ///< result start y
    UINT16  width;          ///< result width
    UINT16  height;         ///< result height
    UINT16  score;          ///< result score
} FACE, *PFACE;

/**
    @name Struct FDE input size infor
*/
typedef struct _FACESIZEINFOR
{
    UINT16  width;          ///< input width
    UINT16  height;         ///< input height
    UINT16  lineoffset;     ///< input lineoffset
} FACESIZEINFOR, *P_FACESIZEINFOR;

extern ER       fde_setFDMode(_FDE_FDFUNC mode, FDE_FDE_PRAM *fde_info);
extern ID       fde_getLockStatus(void);
extern ER       fde_open(FDE_OPENOBJ *pObjCB);
extern BOOL     fde_isOpened(void);
extern ER       fde_close(void);
extern ER       fde_EnableInt(UINT32 intr);
extern UINT32   fde_IntStatus(void);
extern ER       fde_ClearInt(UINT32 intr);
extern ER       fde_SetRsltNum(UINT32  num);
extern UINT32   fde_GetDmaInAddr(void);
extern UINT32   fde_GetDmaOutAddrRslt(void);
extern UINT32   fde_GetRsltNum(_FD_RSLT_OVLP overlap);
extern ER       fde_GetInSizeInfor(FACESIZEINFOR *sizeinfor);
extern UINT32   fde_GetScaleFactor(_FDE_FDDIRECTION direction);

#endif

//@}
