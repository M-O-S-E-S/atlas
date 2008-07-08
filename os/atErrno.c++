
#include "atErrno.h++"


#ifdef _MSC_VER
   #include <winsock.h>

   int getLastError()
   {
      // Return the last error from Winsock
      return WSAGetLastError();
   }
#else
   #include <errno.h>

   int getLastError()
   {
      // Return the errno
      return errno;
   }
#endif

