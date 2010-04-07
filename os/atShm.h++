
#ifndef AT_SHM_H
#define AT_SHM_H


#include "atSymbols.h++"


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <winsock2.h>
   #include <inttypes.h>

   typedef HANDLE     ShmID;
   typedef uint64_t   ShmKey;

   #define INVALID_SHM_ID   INVALID_HANDLE_VALUE
#else
   #include <errno.h>
   #include <stdlib.h>
   #include <sys/ipc.h>
   #include <sys/sem.h>
   #include <sys/shm.h>

   typedef int     ShmID;
   typedef key_t   ShmKey;

   #define INVALID_SHM_ID   -1
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
      ATLAS_SYM void       shmRemove(ShmID id);
      ATLAS_SYM u_char *   shmAttach(ShmID id);
      ATLAS_SYM int        shmDetach(u_char * mem);
   }
#else
   ATLAS_SYM bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
   ATLAS_SYM void       shmRemove(ShmID id);
   ATLAS_SYM u_char *   shmAttach(ShmID id);
   ATLAS_SYM int        shmDetach(u_char * mem);
#endif


#endif

