
#include "atErrno.h++"


#ifdef _MSC_VER
   int getLastError()
   {
      // Return the last error from Winsock
      return WSAGetLastError();
   }
#else
   int getLastError()
   {
      // Return the errno
      return errno;
   }
#endif

