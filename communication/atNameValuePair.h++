
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


#ifndef AT_NAME_VALUE_PAIR_H
#define AT_NAME_VALUE_PAIR_H


// INCLUDES
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atNameValuePair : public atItem
{
   protected:
      char     pair_name[255];
      u_long   pair_name_len;

      char     pair_value[1024];
      u_long   pair_value_len;

   public:
      atNameValuePair(char * name, u_long nameLen, 
                      char * value, u_long valueLen);
      virtual ~atNameValuePair();

      virtual char *   getName();
      virtual u_long   getNameLen();
      virtual char *   getValue();
      virtual u_long   getValueLen();

      virtual void     setName(char * name, u_long nameLen);
      virtual void     setValue(char * value, u_long valueLen);
};

#endif

