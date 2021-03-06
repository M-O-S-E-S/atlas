
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


#include <stdio.h>
#include "atShm.h++"


#ifdef _MSC_VER
   #include <stdio.h>

   bool shmGet(ShmKey key, u_long memSize, ShmID * id)
   {
      char    shmName[256];
      ShmID   shmID;

      // Get a name for the semaphore
      sprintf(shmName, "%llu-%lu", key, memSize);

      // Get shared memory (with a FileMapping)
      shmID = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                0, memSize, shmName);

      // Set the return shared memory ID
      *id = shmID;

      // Return whether we created it
      if (GetLastError() == ERROR_ALREADY_EXISTS)
         return false;
      else
         return true;
   }


   void shmRemove(ShmID id)
   {
      // Clear the shared memory
      CloseHandle(id);
   }


   u_char * shmAttach(ShmID id)
   {
      u_char *   mem;

      // Get a map view of the file
      mem = (u_char *) MapViewOfFile(id, FILE_MAP_WRITE, 0, 0, 0);

      // Return the memory
      return mem;
   }


   int shmDetach(u_char * mem)
   {
      // Unmap the memory
      if (UnmapViewOfFile(mem) == FALSE)
         return 0;
      else
         return 1;
   }
#elif __ANDROID__
   // Android does not have any good shared memory support
   bool shmGet(ShmKey key, u_long memSize, ShmID * id)
   {
      printf("Shared Memory not supported in ATLAS for Android.\n");
      return false;
   }


   void shmRemove(ShmID id)
   {
      printf("Shared Memory not supported in ATLAS for Android.\n");
   }


   u_char * shmAttach(ShmID id)
   {
      printf("Shared Memory not supported in ATLAS for Android.\n");
      return (u_char *) -1;
   }


   int shmDetach(u_char * mem)
   {
      printf("Shared Memory not supported in ATLAS for Android.\n");
      return -1;
   }
#elif __IOS__
   // iOS does not have any good shared memory support
   bool shmGet(ShmKey key, u_long memSize, ShmID * id)
   {
      printf("Shared Memory not supported in ATLAS for iOS.\n");
      return false;
   }


   void shmRemove(ShmID id)
   {
      printf("Shared Memory not supported in ATLAS for iOS.\n");
   }


   u_char * shmAttach(ShmID id)
   {
      printf("Shared Memory not supported in ATLAS for iOS.\n");
      return (u_char *) -1;
   }


   int shmDetach(u_char * mem)
   {
      printf("Shared Memory not supported in ATLAS for iOS.\n");
      return -1;
   }
#else
   bool shmGet(ShmKey key, u_long memSize, ShmID * id)
   {
      int    shmID;
      bool   created;

      shmID = shmget(key, memSize, 0666 | IPC_CREAT | IPC_EXCL);
      if (shmID == -1)
      {
         // See if we failed because it exists already
         if (errno == EEXIST)
         {
            // It does exist so get it "non-exclusively" then
            shmID = shmget(key, memSize, 0666);

            // Save the fact that we did not create the shared memory
            created = false;
         }
      }
      else
      {
         // Save the fact that we did actually create the shared memory
         created = true;
      }

      // Store the return ID
      *id = shmID;

      // Return whether we created it or just attached
      return created;
   }


   void shmRemove(ShmID id)
   {
      // Remove shared memory
      shmctl(id, IPC_RMID, NULL);
   }


   u_char * shmAttach(ShmID id)
   {
      u_char *   mem;

      // Attach the shared memory
      mem = (u_char *) shmat(id, NULL, 0);

      // Return what we have
      return mem;
   }


   int shmDetach(u_char * mem)
   {
      // Detach from the shared memory
      shmdt(mem);
   }
#endif

