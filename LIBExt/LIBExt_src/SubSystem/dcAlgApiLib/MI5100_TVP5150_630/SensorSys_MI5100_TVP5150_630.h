
#ifndef _SENSORSYS_MI5100_630_H_
#define _SENSORSYS_MI5100_630_H_

#define RaiseClock          0

#define DZOOM_IDLE          0x00000001
#define DZOOM_SET_SIEPRE    0x00000002
#define DZOOM_SET_IPEIME    0x00000004
#define DZOOM_SET_CA        0x00000008

#define SENSOR_GRS_MODE 0

void SetDzoom2Preview(unsigned int ZoomIndex);
void SetDzoom2Video(unsigned int ZoomIndex);

void SetSensorMode(INT32 Mode);
ER IPP_ClockIOInit(PRE_VGAFormatEnum Type,UINT32 sen_mode);
void Set_DZoomFlag(UINT32 Flag);
void Clr_DZoomFlag(UINT32 Flag);
UINT32 Get_DZoomFlag(void);
void SetSiePreCapParam(SIE_PRE_PARAM *SiePreInfo);
void siepre_GetCapRaw(SIE_PRE_PARAM *SiePreParam);
void pre_PrvParameter(struct  PUBLIC_STR_JOBREQS *job_req);
void pre_CapParameter(struct  PUBLIC_STR_JOBREQS *job_req);
void pre_D2DParameter(struct  PUBLIC_STR_JOBREQS *job_req);
void DFS_OnOffSwitch(void);
void IPPFunc_OnOffSwitch(void);
void AAFD_LockSwitch(UINT32 ImgMode);
UINT32 getCapRawSize(void);
void VideoDzoomSet(void);
INT32 UI_CheckParameter(void);
void SetColorMode(unsigned char color);
void SetSharpness(unsigned char Index);
void SetSaturation(unsigned char Index);
unsigned int WriteDebugData(unsigned char *Add);
UINT32  Sensor_CheckNeedSize(void);
void Sensor_Cap2Prv(void);
void CaptureOne(SIE_PRE_PARAM *SiePreParam);
void Get_CapInfo(UINT InfoID, UINT *ReturnInfo);
UINT32* Get_ChromaInfo(void);
void Set2Video(IPL_OBJ *Obj);
void Set2Preview(IPL_OBJ *Obj);
void Set2Pcc(IPL_OBJ *Obj);
void Set2VideoLiveview(IPL_OBJ *Obj);
UINT32 Sen_GetMemMode(void);
//#NT#20090918#ethanlau -begin
extern void SensorMode(IPL_MODE mode);
//#NT#20090918#ethanlau -end
extern const UINT32 CapBufAlloc[Max_BurstNum][3];
extern unsigned char JPEG_Quality[];
//#NT#2010/05/28#ethanlau -begin
IME_FILT_PARA* Get_ImeFilterInfo(void);
//#NT#2010/05/28#ethanlau -end
extern void AppInit_Protect(void);
#endif
