
#ifndef AT_SEM_H
#define AT_SEM_H


#include "atSymbols.h++"


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <winsock2.h>
   #include <inttypes.h>

   typedef HANDLE     SemID;
   typedef uint64_t   SemKey;

   #define INVALID_SEM_ID   INVALID_HANDLE_VALUE
#elif __ANDROID__
   #include <semaphore.h>

   typedef sem_t *   SemID;
   typedef int       SemKey;

   #define INVALID_SEM_ID   0x00000000
#else
   #include <errno.h>
   #include <sys/sem.h>

   typedef int     SemID;
   typedef key_t   SemKey;

   #define INVALID_SEM_ID   -1
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool   semGet(SemKey key, SemID * id);
      ATLAS_SYM void   semRemove(SemID id);
      ATLAS_SYM int    semLock(SemID id);
      ATLAS_SYM int    semUnlock(SemID id);
   }
#else
   ATLAS_SYM bool   semGet(SemKey key, SemID * id);
   ATLAS_SYM void   semRemove(SemID id);
   ATLAS_SYM int    semLock(SemID id);
   ATLAS_SYM int    semUnlock(SemID id);
#endif


#endif

