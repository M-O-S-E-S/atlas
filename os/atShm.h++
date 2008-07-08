
#ifndef AT_SHM_H
#define AT_SHM_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #include <windows.h>

   typedef HANDLE   ShmID;
   typedef u_long   ShmKey;

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
      bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
      void       shmRemove(ShmID id);
      u_char *   shmAttach(ShmID id);
      int        shmDetach(u_char * mem);
   }
#else
   bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
   void       shmRemove(ShmID id);
   u_char *   shmAttach(ShmID id);
   int        shmDetach(u_char * mem);
#endif


#endif

