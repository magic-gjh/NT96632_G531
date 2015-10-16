/*

Abstract:
   AAC Encoder API

*/
#ifndef _AACE_H_
#define _AACE_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"
{
#endif


typedef struct AACEProperty
{
  unsigned int SampleRate;
  unsigned int BitRate;
  unsigned int Channel;

  /* 
    1. It is used by AAC Codec. It determines that the encoded or decoded frame will have header or not.
    2. Encoder :  one => encode with header.
                 zero => encode without header.
    3. Decoder:   one => decode with header.
                 zero => decode without header.
  */
  int CodingType;  

  
}AACEProperty;


/*
   Memory Block Information.
*/
typedef struct AACEMemInfo
{
  unsigned int align;
  unsigned int size;     /* In Bytes */
  void *ptr;
}AACEMemInfo;


/*
  Audio Error Type
*/
typedef enum _AACEStatus
{
  AACE_STATUS_OK            = 0x0000,   /* no error */
  AACE_STATUS_UNKNOWN       = 0x0001,   /* unknown error */
  AACE_STATUS_NONCONFIGURE  = 0x0002,   /* not yet configure the encoder */
  AACE_STATUS_CONFIGUREFAIL = 0x0003,   /* fail to configure the encoder */
  AACE_STATUS_RUNFAIL        = 0x0004    /* fail to run the encoder */
}AACEStatus;


/* 
  1. Allocate the private memory space which is used by AAC Encoder.
*/
extern void *AACE_Create(void (*CreateObj)(AACEMemInfo *, int));


/* 
  1. Delete the private memory space which is used by AAC Encoder.
*/
extern void AACE_Delete(void *object, void (*DeleteObj)(AACEMemInfo *, int));


/* 
  1. Configure the property of AAC Encoder.
*/
extern AACEStatus AACE_Configure(void *object, AACEProperty *property);


/* 
  1. Encode One Frame.
  2. Frame content : (Header + Raw Data) or ( Raw Data)
*/
extern AACEStatus AACE_Run(void *object, unsigned char *outbuf, short *inbuf);


/*
  1. Return value : Bitstream Length when encoding one Frame. (Bytes)
  2. It gives the information about one encoded frame size.
  3. It is used by output buffer, one encoded frame size.
*/
extern unsigned int AACE_GetBitstreamLength(void *object);


/*
  1. Return value: Data Length is used by encoder when encoding one Frame. (Bytes)
  2. It gives the information about the size of raw data to encode one frame.
  3. It is used by input buffer, total sample size used by encoding one frame.
*/
extern unsigned int AACE_GetDataLength(void *object);


extern void AACECreateObj(AACEMemInfo *memblk, int num);


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif //_AACE_H_
