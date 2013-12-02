/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <boost/filesystem.hpp>

#include "gdImage.h"
#include "gdCore.h"

using namespace Magick;
using namespace std;

gdImageProperties::gdImageProperties(double pWidth, double pHeight)
  : width(pWidth), height(pHeight)
{}

int gdImage_resizeImage(std::string pathIn, Blob& rBlob, std::string size)
{
#if defined GD_HAVE_GM
  InitializeMagick(0);
  try {
    Image master(pathIn);
    Geometry newSize = Geometry(size);
    master.sample(newSize);
    master.write(&rBlob);
    return 0;
  } catch ( Exception& error_ ) {
    cout << "gdImage_resizeImage() to Blob Caught exception: " << error_.what() << endl;
    return -1;
  }
#endif
}

int gdImage_resizeImage(std::string pathIn, std::string pathOut, std::string size)
{
#if defined GD_HAVE_GM
  InitializeMagick(0);
  try {
    Image master(pathIn);
    Geometry newSize = Geometry(size);
    master.sample(newSize);
    master.write(pathOut);
    return 0;
  } catch ( Exception& error_ ) {
    cout << "gdImage_resizeImage() to path Caught exception: " << error_.what() << endl;
    return -1;
  }
#endif
}

int gdImage_cropImage(std::string pathIn, Blob& rBlob, std::string size)
{
#if defined GD_HAVE_GM
  InitializeMagick(0);
  try {
    Image master(pathIn);
    Geometry newSize = Geometry(size);
    master.crop(newSize);
    master.write(&rBlob);
    return 0;
  } catch ( Exception& error_ ) {
    cout << "gdImage_resizeImage() to Blob Caught exception: " << error_.what() << endl;
    return -1;
  }
#endif
}

int gdImage_cropImage(std::string pathIn, std::string pathOut, std::string size)
{
#if defined GD_HAVE_GM
  InitializeMagick(0);
  try {
    Image master(pathIn);
    Geometry newSize = Geometry(size);
    master.crop(newSize);
    master.write(pathOut);
    return 0;
  } catch ( Exception& error_ ) {
    cout << "gdImage_resizeImage() to path Caught exception: " << error_.what() << endl;
    return -1;
  }
#endif
}

gdImageProperties gdImage_size(const std::string& imagePath)
{
  gdImageProperties imgProp;
#if defined GD_HAVE_GM
  /*
  //fprintf(stderr, "gdImage_size image path = %s\n", imagePath.c_str());
  ImageInfo *image_info;
  ExceptionInfo exception;
  Image* image;

  InitializeMagick(0);
  GetExceptionInfo(&exception);
  image_info = CloneImageInfo((ImageInfo*) NULL);
  strcpy(image_info->filename, imagePath.c_str());
  image = ReadImage(image_info, &exception);
  if ( image )
     {
      // see http://www.graphicsmagick.org/api/types.html#image
      imgProp.height = image->rows;
      imgProp.width  = image->columns;
      DestroyImage(image);
     }
   else
     fprintf(stderr, "gdcore::gdImage_size cannot open : %s\n", imagePath.c_str());
  DestroyImageInfo(image_info);
  DestroyExceptionInfo(&exception);
  */
  InitializeMagick(0);
  Image image;
  try {
    image.read(imagePath);
    imgProp.height = image.rows();
    imgProp.width = image.columns();
  } catch ( Exception& error_ ) {
    return imgProp;
  }
#endif
  return imgProp;
}

std::string gdImage_imageFromText(const std::string& text, const std::string& rFont, double rFontSize)
{
  std::string ret;
#if defined GD_HAVE_GM
  InitializeMagick(0);
  Image image("250x20", "white");
  image.magick("PNG");
  list<Drawable> text_draw_list;
  text_draw_list.push_back(DrawableText(0, 00, text));
  text_draw_list.push_back(DrawableGravity(CenterGravity));
  if ( rFont.size() > 0 )
    text_draw_list.push_back(DrawableFont(rFont));
  if ( rFontSize > 0 )
    text_draw_list.push_back(DrawablePointSize(rFontSize));
  image.draw(text_draw_list);
  Blob pBlob;
  image.write(&pBlob);
  ret = gdcore_b64enc_bin2text((const char*) pBlob.data(), pBlob.length());
#endif
  return ret;
}

