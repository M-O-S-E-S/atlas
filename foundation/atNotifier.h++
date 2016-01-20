
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


#ifndef AT_NOTIFIER_H
#define AT_NOTIFIER_H


// INCLUDES
#include <stdio.h>

#include "atOSDefs.h++"


// CONSTANTS
// Simple error notification constants used by setNotifyLevel() and notify()
enum atNotifyLevel
{
   AT_INFO = 0x0001,
   AT_WARN = 0x0002,
   AT_ERROR = 0x0004,
   AT_FATAL_ERROR = 0x0008,
   AT_NO_HEADER = 0x0100,
   AT_FORCE = 0x0200,
   AT_DEBUG = 0x0400,
   AT_SILENT = 0x0800,
   AT_LEVEL_MASK = 0x00FF,
   AT_MOD_MASK = 0xFF00
};


class ATLAS_SYM atNotifier
{
   protected:
      char            header_name[80];
      static FILE *   output_file_ptr;

      static int      notify_level;

   public:
      atNotifier();
      virtual ~atNotifier();

      virtual void   setName(char * name);
      virtual void   setNotifyLevel(int desiredLevel);
      virtual void   setNotifyDestination(FILE * output);
      virtual void   notify(int level, char * str, ...);
};

#endif

