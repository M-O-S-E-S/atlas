
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


#include "atUUID.h++"


#ifdef _MSC_VER
   #include <rpc.h>

   void getUUID(char * id)
   {
      UUID       uuid;
      u_char *   uuidStr;

      // Create a UUID using Windows RPC services
      UuidCreate(&uuid);

      // Convert the UUID structure to a string
      UuidToString(&uuid, &uuidStr);

      // Copy the UUID string to the user's string
      strcpy(id, (char *) uuidStr);

      // Free the RPC string
      RpcStringFree(&uuidStr);
   }
#else
   #include <stdlib.h>
   #include "uuid.h"

   void getUUID(char * id)
   {
      uuid_t *   uuid;
      void *     uuidStr;

      // Get a UUID (Universally Unique Identifier) using the uuid library
      uuid_create(&uuid);
      uuid_make(uuid, UUID_MAKE_V1);

      // Then get the UUID as a string and get rid of the UUID itself
      uuidStr = NULL;
      uuid_export(uuid, UUID_FMT_STR, &uuidStr, NULL);
      strcpy(id, (char *) uuidStr);
      uuid_destroy(uuid);
   }
#endif

