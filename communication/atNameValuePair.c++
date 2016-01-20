
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
#include "atNameValuePair.h++"


atNameValuePair::atNameValuePair(char * name, u_long nameLen, 
                                 char * value, u_long valueLen)
{
   // Store the info
   memcpy(pair_name, name, nameLen);
   memcpy(pair_value, value, valueLen);
   pair_name_len = nameLen;
   pair_value_len = valueLen;
}


atNameValuePair::~atNameValuePair()
{
}


char * atNameValuePair::getName()
{
   // Return the name
   return pair_name;
}


u_long atNameValuePair::getNameLen()
{
   // Return the name length
   return pair_name_len;
}


char * atNameValuePair::getValue()
{
   // Return the value
   return pair_value;
}


u_long atNameValuePair::getValueLen()
{
   // Return the value length
   return pair_value_len;
}


void atNameValuePair::setName(char * name, u_long nameLen)
{
   // Set a new name
   memcpy(pair_name, name, nameLen);
   pair_name_len = nameLen;
}


void atNameValuePair::setValue(char * value, u_long valueLen)
{
   // Set a new value
   memcpy(pair_value, value, valueLen);
   pair_value_len = valueLen;
}

