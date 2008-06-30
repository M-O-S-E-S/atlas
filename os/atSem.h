
#ifndef AT_SEM_H
#define AT_SEM_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #include <windows.h>

   typedef HANDLE   SemID;
   typedef u_long   SemKey;

   #define INVALID_SEM_ID INVALID_HANDLE_VALUE
#else
   #include <errno.h>
   #include <sys/sem.h>

   typedef int     SemID;
   typedef key_t   SemKey;

   #define INVALID_SEM_ID -1
#endif


extern "C"
{
   bool    semGet(SemKey key, SemID * id);
   void    semRemove(SemID id);
   int     semLock(SemID id);
   int     semUnlock(SemID id);
}


#endif

