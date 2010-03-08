
#ifndef AT_IMAGE_H
#define AT_IMAGE_H


#include "atItem.h++"


enum atImageFormat
{
   AT_IMAGE_FORMAT_RGB8,
   AT_IMAGE_FORMAT_RGBA8
};


class ATLAS_SYM atImage : public atItem
{
   protected:
      u_char *        image_data;
      u_long          image_width;
      u_long          image_height;
      atImageFormat   image_format;

   public:
      atImage(u_char * newData, u_long newWidth, u_long newHeight, 
              atImageFormat newImageFormat);
      virtual ~atImage();

      virtual atImage   clone();

      u_long            getBytesPerPixel();
      u_long            getHeight();
      u_long            getWidth();
      u_long            getDataSize();
      atImageFormat     getImageFormat();
      u_char *          getData();
};


#endif

