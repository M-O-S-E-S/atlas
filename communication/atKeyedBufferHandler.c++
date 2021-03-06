
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


#include <stdlib.h>
#include <string.h>
#include "atKeyedBufferHandler.h++"


atKeyedBufferHandler::atKeyedBufferHandler(u_long bufferType, 
   char * bufferTarget, u_long classID, u_long bufferKey, 
   u_char * buffer, u_long bufferLen)
{
   // Save the data of this buffer
   buffer_type = bufferType;
   buffer_target = bufferTarget;
   buffer_class = classID;
   buffer_key = bufferKey;
   buffer_text = buffer;
   buffer_len = bufferLen;
}


atKeyedBufferHandler::~atKeyedBufferHandler()
{
}


void atKeyedBufferHandler::retrieveBuffer(u_long * bufferType, 
   char ** bufferTarget, u_long * classID, u_long * bufferKey,
   u_char ** buffer, u_long * bufferLen)
{
   // Save the data of this buffer
   *bufferType = buffer_type;
   *bufferTarget = buffer_target;
   *classID = buffer_class;
   *bufferKey = buffer_key;
   *buffer = buffer_text;
   *bufferLen = buffer_len;
}


void atKeyedBufferHandler::setBuffer(u_long bufferType, char * bufferTarget,
   u_long classID, u_long bufferKey, u_char * buffer, u_long bufferLen)
{
   // Save the data of this buffer
   buffer_type = bufferType;
   buffer_target = bufferTarget;
   buffer_class = classID;
   buffer_key = bufferKey;
   buffer_text = buffer;
   buffer_len = bufferLen;
}


void atKeyedBufferHandler::print()
{
   // Call notify with the buffer (right now this is at an INFO level,
   // but should probably be lowered once lower levels are implemented)
   notify(AT_INFO, "Buffer Handler has buffer of \"%s\".\n", buffer_text);
}

