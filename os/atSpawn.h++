
#ifndef AT_SPAWN_H
#define AT_SPAWN_H


#include "atSymbols.h++"

#ifdef _MSC_VER

   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #undef WIN32_LEAN_AND_MEAN

   typedef HANDLE   atProcessHandle;

#elif defined(__ANDROID__)

   // Process control isn't possible in Android

#else

   #include <unistd.h>

   typedef pid_t   atProcessHandle;

#endif


#ifndef __ANDROID__

   #ifdef __cplusplus
      extern "C"
      {
         ATLAS_SYM atProcessHandle   spawnProcess(char * execName,
                                                  char * cmdParameters,
                                                  char * workingDir);
         ATLAS_SYM int               getProcessStatus(atProcessHandle p,
                                                      int * exitCode);
         ATLAS_SYM bool              exitProcess(atProcessHandle p);
         ATLAS_SYM bool              killProcess(atProcessHandle p);
      }
   #else
      ATLAS_SYM atProcessHandle   spawnProcess(char * execName,
                                               char * cmdParameters,
                                               char * workingDir);
      ATLAS_SYM int               getProcessStatus(atProcessHandle p,
                                                   int * exitCode);
      ATLAS_SYM bool              exitProcess(atProcessHandle p);
      ATLAS_SYM bool              killProcess(atProcessHandle p);
   #endif

#endif

#endif
