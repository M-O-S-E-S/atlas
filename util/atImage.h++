
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
      u_char * data;
      u_long          width, height;
      atImageFormat   imageFormat;

   public:
      atImage(u_char * newData, u_long newWidth, u_long newHeight, 
              atImageFormat newImageFormat);
      virtual          ~atImage();

      virtual atImage  clone();

      u_long           getDataSize();
      u_long           getBytesPerPixel();
      atImageFormat    getImageFormat();
      u_long           getWidth();
      u_long           getHeight();
      const u_char *         getData();
};

#endif
