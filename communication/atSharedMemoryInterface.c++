
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


#include "atSharedMemoryInterface.h++"


// CONSTANTS
#define AT_SHM_INIT_QUEUE_SIZE   5000 
#define AT_SHM_INC_QUEUE_SIZE   10000


atSharedMemoryInterface::atSharedMemoryInterface(ShmKey readKey, ShmKey writeKey)
{
   // Create a read queue and a write queue for communication messages
   // through shared memory (we create a second key by subtracting it from
   // 0x1000 since the Shared Queue needs separate "control" and "data" 
   // keys)
   read_queue = new atSharedQueue(readKey, 0x10000-readKey, 
                                  AT_SHM_INIT_QUEUE_SIZE, 
                                  AT_SHM_INC_QUEUE_SIZE);
   write_queue = new atSharedQueue(writeKey, 0x10000-writeKey, 
                                   AT_SHM_INIT_QUEUE_SIZE, 
                                   AT_SHM_INC_QUEUE_SIZE);
}


atSharedMemoryInterface::~atSharedMemoryInterface()
{
   // Delete the shared queues
   delete read_queue;
   delete write_queue;
}


int atSharedMemoryInterface::read(u_char * buffer, u_long length)
{
   // Try to read the next queue entry
   if (read_queue->dequeue(buffer, &length) == true)
   {
      // If we succeeded, return the length of what was read
      return length;
   }
   else
   {
      // Otherwise, return zero (we did not succeed in reading from the queue)
      return 0;
   }
}


int atSharedMemoryInterface::write(u_char * buffer, u_long length)
{
   // Write the buffer to the queue
   write_queue->enqueue(buffer, length);

   // At least for now, we assume enqueuing to the shared buffer will work
   // everytime
   return length;
}

