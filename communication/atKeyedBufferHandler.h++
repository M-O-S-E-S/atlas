
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


#ifndef AT_KEYED_BUFFER_HANDLER_H
#define AT_KEYED_BUFFER_HANDLER_H


#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atKeyedBufferHandler : public atItem
{
   protected:
      u_long     buffer_type;
      char *     buffer_target;
      u_long     buffer_class;
      u_long     buffer_key;
      u_char *   buffer_text;
      u_long     buffer_len;

   public:
      atKeyedBufferHandler(u_long bufferType, 
                           char * targetID, u_long classID,
                           u_long bufferKey, u_char * buffer, u_long bufferLen);
      virtual ~atKeyedBufferHandler();

      void retrieveBuffer(u_long * bufferType, 
                          char ** targetID, u_long * classID,
                          u_long * bufferKey, u_char ** buffer, 
                          u_long * bufferLen);
      void setBuffer(u_long bufferType, char * targetID, u_long classID,
                     u_long bufferKey, u_char * buffer, u_long bufferLen);

      void print();
};


#endif

