#ifndef DZOOM_OV9710_DUAL_630_H
#define DZOOM_OV9710_DUAL_630_H

#define IMG_RATIO_UINT      256

typedef struct _SIZE_INFOR {
            unsigned long H_Size;
            unsigned long V_Size;
} SIZE_INFOR;

typedef struct
{
    UINT32 SensorRatio;
    UINT16 (*DzTable)[8];
    UINT32 TblSize;
} DZOOM_INFOR;

UINT32 Get_DzoomCapMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);
UINT32 Get_DzoomPrvMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);
UINT32 Get_DzoomPrvMaxBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);
void Set_digitalzoom(UINT DzoomIdx, struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM );
UINT32 Get_DzoomPrvMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);
void SetImageOutSize(unsigned short Hsize, unsigned short Vsize);
int Get_DzoomIndex(void);
UINT32 Get_DzoomCapMaxFieldBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);

UINT16 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
UINT16 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx);
UINT32 Adj_VSize(UINT32 InSize, UINT16 VRatio, UINT16 Align);
UINT32 Adj_HSize(UINT32 InSize, UINT16 HRatio, UINT16 Align);
//#NT#2010/09/11#ethanlau -begin
UINT32 GetRealDZoomRatio(UINT32 idx,struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);
UINT GetDZoomIdxFromRatio(UINT Ratio,struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM);
//#NT#2010/09/11#ethanlau -end
#endif
