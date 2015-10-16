/** 2007/02/26 yuppie added */
/** last update 2008/08/28  */
/**                         */
//@{
#ifndef _SD_LIB_H
#define _SD_LIB_H

extern void     sd_SmileDetectionProcess(void);
extern UINT32   sd_GetResult(void);
extern UINT32   sd_calcBuffSize(void);
//#NT#2010/06/03#YC Peng#[0011077] -begin
//#NT#modify SD init interface
void sd_setInitFlag(UINT32 Flag);
//#NT#2010/06/03#YC Peng#[0011077] -end
//#NT#2010/10/06#JJ -begin
extern void sd_SdReset(void);
//#NT#2010/10/06#JJ -end
#endif
//@}
