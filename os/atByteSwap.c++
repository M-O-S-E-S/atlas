
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


#include "atByteSwap.h++"


// Need to have winsock in the windows version
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <winsock2.h>
   #undef WIN32_LEAN_AND_MEAN
#else
   #include <netinet/in.h>
#endif


float htonf(float x)
{
   char   *tmp;
   char   c;

   if (htons(1) != 1)
   {
      // Machine is little endian
      tmp = (char *) &x;

      c = tmp[0];
      tmp[0] = tmp[3];
      tmp[3] = c;
      c = tmp[1];
      tmp[1] = tmp[2];
      tmp[2] = c;
   }

   return x;
}


float ntohf(float x)
{
   return htonf(x);
}


double htond(double x)
{
   char   *tmp;
   char   c;

   if (htons(1) != 1)
   {
      // Machine is little endian
      tmp = (char *) &x;

      c = tmp[0];
      tmp[0] = tmp[7];
      tmp[7] = c;
      c = tmp[1];
      tmp[1] = tmp[6];
      tmp[6] = c;
      c = tmp[2];
      tmp[2] = tmp[5];
      tmp[5] = c;
      c = tmp[3];
      tmp[3] = tmp[4];
      tmp[4] = c;
   }

   return x;
}


double ntohd(double x)
{
   return htond(x);
}

