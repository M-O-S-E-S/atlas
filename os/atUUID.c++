
#include "atUUID.h++"


#ifdef _MSC_VER
   #include <rpc.h>

   void getUUID(char * id)
   {
      UUID       uuid;
      u_char *   uuidStr;
      char *     id;

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
      char *     id;

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

