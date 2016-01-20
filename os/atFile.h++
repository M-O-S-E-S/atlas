
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


#ifndef AT_FILE_HPP
#define AT_FILE_HPP


#include "atSymbols.h++"


// Microsoft has a different name for the access function.
// The flags for read access, write access, and so on also need to be defined.
#ifdef _MSC_VER
   #include <direct.h>
   #include <io.h>

   #define chdir    _chdir
   #define access   _access
   #define F_OK   0x00
   #define W_OK   0x02
   #define R_OK   0x04

   #define DIRECTORY_SEPARATOR   '\\'
   #define PATH_SEPARATOR        ';'
   #define TMP_DIR               getenv("TEMP")

   #define EXEC_EXTENSION        ".exe"
   #define ICON_EXTENSION        ".ico"
#elif __ANDROID__
   #include <unistd.h>

   #define DIRECTORY_SEPARATOR   '/'
   #define PATH_SEPARATOR        ':'
   #define TMP_DIR               "/sdcard"

   #define EXEC_EXTENSION        ""
   #define ICON_EXTENSION        ".png"
#else
   #include <unistd.h>

   #define DIRECTORY_SEPARATOR   '/'
   #define PATH_SEPARATOR        ':'
   #define TMP_DIR               "/tmp"

   #define EXEC_EXTENSION        ""
   #define ICON_EXTENSION        ".png"
#endif


enum atPersonalPathKey
{
   AT_PATH_HOME,
   AT_PATH_DESKTOP,
   AT_PATH_DOCUMENTS,
   AT_PATH_DOWNLOADS,
   AT_PATH_MUSIC,
   AT_PATH_PICTURES,
   AT_PATH_PUBLIC,
   AT_PATH_TEMPLATES,
   AT_PATH_VIDEOS
};


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool     isDirectory(char * path);
      ATLAS_SYM bool     createDirectory(char * path);
      ATLAS_SYM int      listFiles(char * path, char ** results, int count);
      ATLAS_SYM char *   getPersonalPath(atPersonalPathKey key);
   }
#else
   ATLAS_SYM bool     isDirectory(char * path);
   ATLAS_SYM bool     createDirectory(char * path);
   ATLAS_SYM int      listFiles(char * path, char ** results, int count);
   ATLAS_SYM char *   getPersonalPath(atPersonalPathKey key);
#endif


#endif

