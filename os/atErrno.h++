
#ifndef AT_ERRNO_H
#define AT_ERRNO_H


#include "atSymbols.h++"

#ifdef _MSC_VER
   #include <winsock2.h>

   // Windows doesn't have a strerror_r but does have strerror_s!
   #define strerror_r(errno, buf, len)   strerror_s(buf, len, errno)
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

