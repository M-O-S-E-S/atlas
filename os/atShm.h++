
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


#ifndef AT_SHM_H
#define AT_SHM_H


#include "atSymbols.h++"


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <winsock2.h>
   #include <inttypes.h>

   typedef HANDLE     ShmID;
   typedef uint64_t   ShmKey;

   #define INVALID_SHM_ID   INVALID_HANDLE_VALUE
#elif __ANDROID__
   // Android has no good shared memory support so make placeholders
   #include <sys/types.h>

   typedef int   ShmID;
   typedef int   ShmKey;

   #define INVALID_SHM_ID   -1
#elif __IOS__
   // iOS has no good shared memory support so make placeholders
   #include <sys/types.h>

   typedef int   ShmID;
   typedef int   ShmKey;

   #define INVALID_SHM_ID   -1
#else
   #include <errno.h>
   #include <stdlib.h>
   #include <sys/ipc.h>
   #include <sys/sem.h>
   #include <sys/shm.h>

   typedef int     ShmID;
   typedef key_t   ShmKey;

   #define INVALID_SHM_ID   -1
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
      ATLAS_SYM void       shmRemove(ShmID id);
      ATLAS_SYM u_char *   shmAttach(ShmID id);
      ATLAS_SYM int        shmDetach(u_char * mem);
   }
#else
   ATLAS_SYM bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
   ATLAS_SYM void       shmRemove(ShmID id);
   ATLAS_SYM u_char *   shmAttach(ShmID id);
   ATLAS_SYM int        shmDetach(u_char * mem);
#endif


#endif

