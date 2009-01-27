
#include "atUUID.h++"


#ifdef _MSC_VER
   #include <rpc.h>

   char * getUUID()
   {
      UUID       uuid;
      u_char *   uuidStr;

      char *     id;

      // Create a UUID using Windows RPC services
      UuidCreate(&uuid);

      // Convert the UUID structure to a string
      UuidToString(&uuid, &uuidStr);

      // Allocate a new string and copy the UUID string to it
      id = (char *) calloc(sizeof(char), 128);
      strcpy(id, (char *) uuidStr);

      // Free the RPC string
      RpcStringFree(&uuidStr);

      // Return the new UUID string
      return id;
   }

#else
   #include <stdlib.h>
   #include "uuid.h"

   char * getUUID()
   {
      uuid_t *   uuid;
      void *     uuidStr;

      char *     id;

      // Get a UUID (Universally Unique Identifier) using the uuid library
      uuid_create(&uuid);
      uuid_make(uuid, UUID_MAKE_V1);

      // Allocate a new string for the UUID
      id = (char *) calloc(sizeof(char), 128);

      // Then get the UUID as a string and get rid of the UUID itself
      uuidStr = NULL;
      uuid_export(uuid, UUID_FMT_STR, &uuidStr, NULL);
      strcpy(id, (char *) uuidStr);
      uuid_destroy(uuid);

      // Return the exported UUID string
      return id;
   }

#endif

