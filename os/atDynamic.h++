
#ifndef AT_DYNAMIC_H
#define AT_DYNAMIC_H


#include "atSymbols.h++"


#ifdef _MSC_VER
   #define RTLD_GLOBAL   0
   #define RTLD_LAZY     0
   #define RTLD_NOW      0
#elif __ANDROID__
   #include <dlfcn.h>
#else
   #include <dlfcn.h>
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM void *   dlOpen(const char * filename, int flag);
      ATLAS_SYM char *   dlError();
      ATLAS_SYM void *   dlSym(void * handle, const char * symbol);
      ATLAS_SYM int      dlClose(void * handle);
   }
#else
   ATLAS_SYM void *   dlOpen(const char * filename, int flag);
   ATLAS_SYM char *   dlError();
   ATLAS_SYM void *   dlSym(void * handle, const char * symbol);
   ATLAS_SYM int      dlClose(void * handle);
#endif


#endif

