
#ifndef AT_SEM_H
#define AT_SEM_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   typedef HANDLE   SemID;
   typedef u_long   SemKey;
#else
   #include <sys/sem.h>

   typedef int     SemID;
   typedef key_t   SemKey;
#endif


bool    semGet(SemKey key, SemID * id);
void    semRemove(SemID id);
int     semLock(SemID id);
int     semUnlock(SemID id);


#endif

