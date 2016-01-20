
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


#ifndef AT_SHARED_QUEUE_H
#define AT_SHARED_QUEUE_H


// INCLUDES
#include <sys/types.h>
#include "atNotifier.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atSharedQueue : public atNotifier
{
   protected:
      SemKey     sem_key;
      SemID      sem_id;
      ShmKey     control_shm_key;
      ShmID      control_shm_id;
      ShmKey     data_shm_key;
      ShmID      data_shm_id;

      u_long     memory_increment_size;

      u_char *   shared_control_info;
      u_char *   shared_buffer;

      u_long *   queue_head;
      u_long *   queue_tail;
      u_long *   queue_size;
      u_long *   queue_used;
      u_long *   realloc_num;
      u_long *   connected_count;

      u_long     last_realloc_num;

      int      lock();
      int      unlock();
      u_long   spaceAvailable();
      void     reallocateQueue(u_long minimumToAdd);
      void     checkForReallocatedQueue();


   public:
      atSharedQueue(ShmKey controlKey, ShmKey dataKey, u_long initialSize, 
                    u_long incrementSize);
      virtual ~atSharedQueue();

      virtual void   enqueue(u_char * buffer, u_long bufferLen);
      virtual bool   dequeue(u_char * buffer, u_long * bufferLen);
};


#endif

