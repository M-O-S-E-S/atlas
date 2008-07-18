
#ifndef AT_SEM_H
#define AT_SEM_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <winsock.h>

   typedef HANDLE   SemID;
   typedef u_long   SemKey;

   #define INVALID_SEM_ID   INVALID_HANDLE_VALUE
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
      bool   semGet(SemKey key, SemID * id);
      void   semRemove(SemID id);
      int    semLock(SemID id);
      int    semUnlock(SemID id);
   }
#else
   bool   semGet(SemKey key, SemID * id);
   void   semRemove(SemID id);
   int    semLock(SemID id);
   int    semUnlock(SemID id);
#endif


#endif

