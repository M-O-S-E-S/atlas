
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


#ifndef AT_ARRAY_H
#define AT_ARRAY_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atArray : public atNotifier
{
   protected:
      atItem **       array_items;
      
      long            num_entries;
      long            current_capacity;

      virtual bool    ensureCapacity(u_long capacity);

   public:
      atArray();
      virtual ~atArray();

      virtual long       getNumEntries();

      virtual bool       addEntry(atItem * item);
      virtual atItem *   setEntry(long index, atItem * item);
      virtual bool       insertEntry(long index, atItem * item);
      virtual bool       removeEntry(atItem * item);
      virtual bool       removeEntryAtIndex(long index);
      virtual bool       removeAllEntries();

      virtual atItem *   getEntry(long index);
      virtual long       getIndexOf(atItem * item);
};

#endif

