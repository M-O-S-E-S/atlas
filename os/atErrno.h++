
#ifndef AT_ERRNO_H
#define AT_ERRNO_H


#include "atSymbols.h++"

#ifdef _MSC_VER
   #include <winsock.h>
#else
   #include <errno.h>
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM int   getLastError();
   }
#else
   ATLAS_SYM int   getLastError();
#endif


#endif

