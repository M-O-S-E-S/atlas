
#ifndef AT_SHM_H
#define AT_SHM_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   typedef HANDLE   ShmID;
   typedef u_long   ShmKey;
#else
   #include <sys/sem.h>

   typedef int     ShmID;
   typedef key_t   ShmKey;
#endif


bool       shmGet(ShmKey key, u_long memSize, ShmID * id);
void       shmRemove(ShmID id);
u_char *   shmAttach(ShmID id);
int        shmDetach(u_char * mem);


#endif

