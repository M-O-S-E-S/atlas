
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <string.h>
#include <stdlib.h>
#include "atImage.h++"


atImage::atImage(u_char * newData, u_long newWidth, u_long newHeight, 
                 atImageFormat newImageFormat)
{
   u_long   dataSize;

   // Initialize the width and height of the image
   image_width = newWidth;
   image_height = newHeight;

   // Initialize the image format
   image_format = newImageFormat;

   // Get the size of the image buffer
   dataSize = getDataSize();

   // Check to see if the data size is valid
   if (dataSize > 0)
   {
      // If it is a valid size then allocate space for the data and copy it
      image_data = new u_char[dataSize];
      memcpy(image_data, newData, dataSize);
   }
}


atImage::~atImage()
{
   // Empty out our image data buffer
   if (image_data != NULL)
      delete [] image_data;
   image_data = NULL;

   // Reset the size of the image
   image_width = 0;
   image_height = 0;
}


atImage atImage::clone()
{
   // Return a copy of this image
   return atImage(image_data, image_width, image_height, image_format);
}


u_long atImage::getBytesPerPixel()
{
   switch (image_format)
   {
      case AT_IMAGE_FORMAT_RGB8:
         return 3;
         break;
      case AT_IMAGE_FORMAT_RGBA8:
         return 4;
         break;
   }
}


u_long atImage::getHeight()
{
   // Return the current height of the image
   return image_height;
}


u_long atImage::getWidth()
{
   // Return the current width of the image
   return image_width;
}


u_long atImage::getDataSize()
{
   // Return how many bytes the image is using
   return image_width * image_height * getBytesPerPixel();
}


atImageFormat atImage::getImageFormat()
{
   // Return the format of the image
   return image_format;
}


u_char * atImage::getData()
{
   // Return the raw image data
   return image_data;
}

