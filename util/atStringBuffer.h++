
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


#ifndef AT_STRING_BUFFER_H
#define AT_STRING_BUFFER_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"
#include "atString.h++"


class ATLAS_SYM atStringBuffer : public atItem
{
   protected:
      char *   local_buffer;
      char *   buffer_tail;
      u_long   buffer_size;

   public:
      atStringBuffer();
      atStringBuffer(const char * stringToCopy);
      atStringBuffer(const char * stringToCopy, u_long maxLength);
      virtual ~atStringBuffer();

      virtual atStringBuffer     clone() const;

      virtual void               append(const atString & stringToAppend);
      virtual atStringBuffer     concat(const atString & stringToConcat) const;

      virtual void               append(const char * stringToAppend);
      virtual atStringBuffer     concat(const char * stringToConcat) const;

      virtual void               append(char charToAppend);
      virtual atStringBuffer     concat(char charToConcat) const;

      virtual void               setString(const char * stringToCopy);
      virtual void               setString(const char * stringToCopy, 
                                           u_long maxLength);
      virtual void               setString(const atString & stringToCopy);

      virtual char *             getString() const;
      virtual atString           getAsString() const;

      virtual char               getCharAt(u_long index) const;
      virtual u_long             getLength() const;

      virtual bool               equals(atItem * otherItem);
      virtual int                compare(atItem * otherItem);

      virtual atStringBuffer &   operator=(const atString & stringToCopy);
      virtual atStringBuffer &   operator=(
                                    const atStringBuffer & stringBufferToCopy);
};


#endif

