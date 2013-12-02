/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef _gdImage_H__
#define _gdImage_H__  1

#include <string>
#include <Magick++.h>

/*!
   \file gdImage.h
   \brief Basic functions for manipulate an images.

 */

class gdImageProperties {
public :
  gdImageProperties(double pWidth = 16, double pHeight = 16);
  double           width;
  double           height;
};


/*! \brief Resize an image : return 0 = success -1 otherwise

 */
int                 gdImage_resizeImage(std::string pathIn, Magick::Blob& rblob, std::string size);
int                 gdImage_resizeImage(std::string pathIn, std::string pathOut, std::string size);
/*! \brief crop image
 *  Geometry specifications are in the form "<width>x<height>{+-}<xoffset>{+-}<yoffset>"
 */
int                 gdImage_cropImage(std::string pathIn, Magick::Blob& rblob, std::string size);
int                 gdImage_cropImage(std::string pathIn, std::string pathOut, std::string size);
gdImageProperties   gdImage_size(const std::string& imagePath);

/* rFont : the full path to the font prefix by @ for a ttf font, example :
 * "@/var/www/wt/resources/themes/default/wittyshare/Fonts/Titillium/TitilliumText25L001-webfont.ttf"
 * */
std::string         gdImage_imageFromText(const std::string& text, const std::string& rFont = std::string(), double rFontSize = 0);

#endif // ifndef _gdImage_H__

