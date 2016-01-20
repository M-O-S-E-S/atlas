
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


#ifndef AT_SPAWN_H
#define AT_SPAWN_H


#include "atSymbols.h++"

#ifdef _MSC_VER

   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #undef WIN32_LEAN_AND_MEAN

   typedef HANDLE   atProcessHandle;

#elif defined(__ANDROID__)

   // Process control isn't possible in Android

#else

   #include <unistd.h>

   typedef pid_t   atProcessHandle;

#endif


#ifndef __ANDROID__

   #ifdef __cplusplus
      extern "C"
      {
         ATLAS_SYM atProcessHandle   spawnProcess(char * execName,
                                                  char * cmdParameters,
                                                  char * workingDir);
         ATLAS_SYM int               getProcessStatus(atProcessHandle p,
                                                      int * exitCode);
         ATLAS_SYM bool              exitProcess(atProcessHandle p);
         ATLAS_SYM bool              killProcess(atProcessHandle p);
      }
   #else
      ATLAS_SYM atProcessHandle   spawnProcess(char * execName,
                                               char * cmdParameters,
                                               char * workingDir);
      ATLAS_SYM int               getProcessStatus(atProcessHandle p,
                                                   int * exitCode);
      ATLAS_SYM bool              exitProcess(atProcessHandle p);
      ATLAS_SYM bool              killProcess(atProcessHandle p);
   #endif

#endif

#endif
