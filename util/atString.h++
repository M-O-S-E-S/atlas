
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


#ifndef AT_STRING_H
#define AT_STRING_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atString : public atItem
{
   protected:
      char *   local_string;
      u_long   string_length;

   public:
      atString();
      atString(const char * stringToCopy);
      atString(const char * stringToCopy, u_long maxLength);
      atString(const atString & stringToCopy);
      virtual ~atString();

      virtual atString       clone() const;

      virtual void           append(const atString & stringToAppend);
      virtual atString       concat(const atString & stringToConcat) const;

      virtual void           setString(const char * stringToCopy);
      virtual void           setString(const char * stringToCopy, 
                                       u_long maxLength);
      virtual void           setString(const atString & stringToCopy);

      virtual char *         getString() const;

      virtual char           getCharAt(u_long index) const;
      virtual u_long         getLength() const;

      virtual void           replaceAll(char * stringToReplace, 
                                        char * replacementString);

      virtual atString       subString(int start);
      virtual atString       subString(int start, int end);

      virtual bool           equals(atItem * otherItem);
      virtual int            compare(atItem * otherItem);
      
      virtual atString &     operator=(const atString & stringToCopy);
};


#endif

