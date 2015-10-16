#ifndef FD_H
#define FD_H

/**
    @name Struct input address infor.
    @brief none.
*/
//@{
typedef struct _FDAPP_INADDRINFOR
{
    UINT32  y_addr;     ///< input Y address
    UINT32  cb_addr;    ///< input Cb address
    UINT32  cr_addr;    ///< input Cr address
} FDAPP_INADDRINFOR, *P_FDAPP_INADDRINFOR;
//@}

/**
    @name Struct FD input size infor.
    @brief none.
*/
//@{
typedef struct _FDAPP_INSIZEINFOR
{
    UINT16  width;          ///< input width
    UINT16  height;         ///< input height
    UINT16  lineoffset;     ///< input lineoffset
} FDAPP_INSIZEINFOR, *P_FDAPP_INSIZEINFOR;
//@}

/**
    @name Face Coordinate.
    @brief PROP_COORD propotional coordinate(0~100%: 0~1024)
           ABS_COORD  absolute coordinate
           OSD_COORD  OSD coordinate
           ORI_COORD  original coordinate(regardless of rotation)
*/
//@{
typedef enum
{
    PROP_COORD,
    ABS_COORD,
    OSD_COORD,
    ORI_COORD,
    ENUM_DUMMY4WORD(_FDAPP_FACECOORD)
}_FDAPP_FACECOORD;
//@}

/**
    @name Face Orientation Priority.
    @brief FDE_OORIENT_EQUAL    0->90->270->0 degree
           FDE_OORIENT_ALL      0->90->270->180->0 degree
*/
//@{
typedef enum
{
    FDE_OORIENT_0_ONLY,
    FDE_OORIENT_90_ONLY,
    FDE_OORIENT_270_ONLY,
    FDE_OORIENT_EQUAL,
    FDE_OORIENT_ALL,
    ENUM_DUMMY4WORD(_FDAPP_FACEORIPRI)
}_FDAPP_FACEORIPRI;
//@}

extern void     fd_FDprocess(void);
//#NT#2010/01/26#Yuppie Lin -begin
//#NT#Rename API
extern void     fd_FdReset(void);
extern UINT32   fd_DetectOnePic(FACE *Face,UINT32 inaddr,UINT32 FDBufferAddr,FACESIZEINFOR in_size);
extern void     fd_GetRsltFace(FACE *Face, _FDAPP_FACECOORD corrdinate);
//#NT#2010/01/26#Yuppie Lin -end
extern UINT32   fd_GetWorkingBuffer(void);
extern void     fd_SetWorkingBuffer(UINT32 bufferaddr);
extern ER       fd_getInputAddr(FDAPP_INADDRINFOR *in_addr);
extern ER       fd_getInputSize(FDAPP_INSIZEINFOR *in_size);
extern UINT32   fd_calcBuffSize(void);
extern void     fd_SetOrient(UINT8 orient);

#endif
