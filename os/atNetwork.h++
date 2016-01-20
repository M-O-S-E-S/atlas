
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef AT_NETWORK_H
#define AT_NETWORK_H


#include "atSymbols.h++"


// Under Windows, define stuff that we need
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <winsock2.h>
   #include <ws2tcpip.h>
   #undef WIN32_LEAN_AND_MEAN

   // Visual Studio 2010 and up define EWOULDBLOCK and EINPROGRESS in
   // <errno.h>.  Unfortunately, these definitions are not used by
   // Winsock, so checking the errno.h values for Winsock-related errors
   // is useless
   #if (_MSC_VER >= 1600)
      #include <errno.h>
      #undef EWOULDBLOCK
      #undef EINPROGRESS
   #endif

   #define EWOULDBLOCK   WSAEWOULDBLOCK
   #define EINPROGRESS   WSAEINPROGRESS

   #define MAXHOSTNAMELEN   64

   typedef SOCKET   Socket;
   typedef int      socklen_t;
   typedef char     SocketOptionFlag;
   typedef int      SocketOptionValue;
#elif __ANDROID__
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
   #include <arpa/inet.h>

   #ifndef MAXHOSTNAMELEN
      #define MAXHOSTNAMELEN   64
   #endif

   #ifndef FNONBLOCK
      #define FNONBLOCK   O_NONBLOCK
   #endif

   typedef int   Socket;
   typedef int   SocketOptionFlag;
   typedef int   SocketOptionValue;
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
   #include <arpa/inet.h>

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

      ATLAS_SYM const char *   netAddrToStr(int family, void * src, 
                                            char * dst, int dstLen);
      ATLAS_SYM int            strToNetAddr(int family, char * src, void * dst);
   }
#else
   ATLAS_SYM void   initNetwork();
   ATLAS_SYM void   cleanupNetwork();

   ATLAS_SYM Socket   openSocket(int domain, int type, int protocol);
   ATLAS_SYM void     closeSocket(Socket socket);

   ATLAS_SYM void   setBlockingFlag(Socket socket, bool block);
   ATLAS_SYM bool   getBlockingFlag(Socket socket);

   ATLAS_SYM const char *   netAddrToStr(int family, void * src, 
                                         char * dst, int dstLen);
   ATLAS_SYM int            strToNetAddr(int family, char * src, void * dst);
#endif


#endif

