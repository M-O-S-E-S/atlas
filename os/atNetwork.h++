
#ifndef AT_NETWORK_H
#define AT_NETWORK_H


#include "atSymbols.h++"


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #undef WIN32_LEAN_AND_MEAN

   #define EWOULDBLOCK   WSAEWOULDBLOCK
   #define EINPROGRESS   WSAEINPROGRESS

   #define MAXHOSTNAMELEN   64

   typedef SOCKET   Socket;
   typedef int      socklen_t;
   typedef char     SocketOptionFlag;
   typedef int      SocketOptionValue;
#else
   #include <unistd.h>
   #include <string.h>
   #include <netdb.h>
   #include <fcntl.h>
   #include <errno.h>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <sys/param.h>
   #include <sys/select.h>
   #include <netinet/in.h>
   #include <netinet/tcp.h>

   #ifndef MAXHOSTNAMELEN
      #define MAXHOSTNAMELEN   64
   #endif

   typedef int   Socket;
   typedef int   SocketOptionFlag;
   typedef int   SocketOptionValue;
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM void   initNetwork();
      ATLAS_SYM void   cleanupNetwork();

      ATLAS_SYM Socket   openSocket(int domain, int type, int protocol);
      ATLAS_SYM void     closeSocket(Socket socket);

      ATLAS_SYM void   setBlockingFlag(Socket socket, bool block);
      ATLAS_SYM bool   getBlockingFlag(Socket socket);
   }
#else
   ATLAS_SYM void   initNetwork();
   ATLAS_SYM void   cleanupNetwork();

   ATLAS_SYM Socket   openSocket(int domain, int type, int protocol);
   ATLAS_SYM void     closeSocket(Socket socket);

   ATLAS_SYM void   setBlockingFlag(Socket socket, bool block);
   ATLAS_SYM bool   getBlockingFlag(Socket socket);
#endif


#endif

