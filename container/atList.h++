
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


#ifndef AT_LIST_H
#define AT_LIST_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"
#include "atItem.h++"
#include "atOSDefs.h++"


// TYPES
typedef struct atEntry
{
   atItem *           item;
   struct atEntry *   next;
   struct atEntry *   previous;
} atListEntry;


class ATLAS_SYM atList : public atNotifier
{
   protected:
      atListEntry *   list_head;
      atListEntry *   list_tail;
      
      u_long          num_entries;

      atListEntry *   current_entry;
      atListEntry *   next_entry;

   public:
      atList();
      virtual ~atList();

      virtual u_long     getNumEntries();

      virtual bool       addEntry(atItem * item);
      virtual bool       insertEntry(atItem * item);
      virtual bool       removeCurrentEntry();
      virtual bool       removeAllEntries();

      virtual atItem *   getFirstEntry();
      virtual atItem *   getNextEntry();
      virtual atItem *   getPreviousEntry();
      virtual atItem *   getLastEntry();
      virtual atItem *   getNthEntry(u_long n);

      virtual atItem *   findEntry(atItem * item);
};

#endif

