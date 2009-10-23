
#include <string.h>
#include <stdlib.h>
#include "atImage.h++"

atImage::atImage(u_char * newData, u_long newWidth, u_long newHeight, 
                 atImageFormat newImageFormat)
{
   // Initialize the width and height of the image
   width = newWidth;
   height = newHeight;

   // Initialize the image format
   imageFormat = newImageFormat;

   // Get the size of the image buffer
   u_long dataSize = getDataSize();

   // Check to see if the data size is valid
   if (dataSize > 0)
   {
      // If it is a valid size then allocate space for the data and copy it
      data = new u_char[dataSize];
      memcpy(data, newData, dataSize);
   }
}

atImage::~atImage()
{
   // Empty out our image data buffer
   if (data != NULL)
      delete [] data;
   data = NULL;

   // Reset the size of the image
   width = 0;
   height = 0;
}

atImage atImage::clone()
{
   // Return a copy of this image
   return atImage(data, width, height, imageFormat);
}

u_long atImage::getBytesPerPixel()
{
   switch(imageFormat)
   {
      case AT_IMAGE_FORMAT_RGB8:
         return 3;
      case AT_IMAGE_FORMAT_RGBA8:
         return 4;
   }
}

u_long atImage::getHeight()
{
   // Return the current height of the image
   return height;
}

u_long atImage::getWidth()
{
   // Return the current width of the image
   return width;
}

u_long atImage::getDataSize()
{
   // Return how many bytes the image is using
   return width * height * getBytesPerPixel();
}

const u_char * atImage::getData()
{
   // Return the raw image data
   return data;
}
