/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef GD_BARCODE_H__
#define GD_BARCODE_H__    1

/*!
   \file gdBarcode.h
   \brief bar code facilities functions

 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Compute the checksum of a bar code.

 */
int gdBarcode_doEan13Checksum(const char* BufIn, char* BufOut);

/*! \brief Return the string for rendering the bar code with the Ean13 font.

 */
int gdBarcode_toEan13Font(const char* BufIn, unsigned char* BufOut);

#ifdef __cplusplus
}
#endif

#endif // ifndef GD_BARCODE_H__

