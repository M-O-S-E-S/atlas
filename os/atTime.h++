
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


#ifndef AT_TIME_H
#define AT_TIME_H


#include "atSymbols.h++"


// Under Windows, define the gettimeofday() function with corresponding types
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <winsock2.h>
   #include <time.h>
   #include "atSymbols.h++"

   // VS2015 seems to have a struct timespec now, yet pthreads is still
   // released defining its own (which then conflicts) so define this to
   // tell pthreads not to define it
   #if _MSC_VER >= 1900
      #define _TIMESPEC_DEFINED
   #endif

   // TYPES
   struct timezone
   {
      int   tz_minuteswest;
      int   tz_dsttime;
   };


   // FUNCTIONS
   #ifdef __cplusplus
      extern "C"
      {
         ATLAS_SYM int gettimeofday(struct timeval * tv, struct timezone * tz);
      }
   #else
      ATLAS_SYM int gettimeofday(struct timeval * tv, struct timezone * tz);
   #endif
#elif __ANDROID__
   // Nothing
#else
   #include <sys/time.h>
#endif


#endif

