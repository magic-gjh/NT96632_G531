/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       VendorExif.h
    @ingroup    mIAVJPEG

    @brief      Vendor EXIF Setting Header Declaration.
                Customers can use this file declaration to change the default setting tag value in EXIF header.
                This header file should be exported to our customers.
*/

#ifndef _VENDOREXIF_H
#define _VENDOREXIF_H

#include <Type.h>

/** \addtogroup mIAVJPEG
*/
//@{

/**
  Customers can set the value in the structure Vendor_EXIF_HEAD to change the following setting
  1. Maker String. (32 bytes, NULL for default string.)
  2. Model String. (16 bytes, NULL for default string)
  3. Software Version String. (20 bytes, NULL for default string)
  4. MakeNote String. (128 bytes.)
**/
typedef struct {
    CHAR     *pMakerString;
    CHAR     *pModelString;
    CHAR     *pSoftwareVer;
    CHAR     *pMakeNote;
    //Reason: add one interface for setting image description
    CHAR     *pImageDescription;
    //#NT#2010/09/16#Hideo Lin -begin
    //#NT#Add User Comment tag (for social network)
    CHAR     *pUserComment;
    //#NT#2010/09/16#Hideo Lin -end
}Vendor_EXIF_HEAD, *PVendor_EXIF_HEAD;


//
// exported Function
//
ER JpgSetEXIFString(PVendor_EXIF_HEAD pCustomerEXIF);

//@}

#endif
