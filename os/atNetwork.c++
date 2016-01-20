
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


#include "atNetwork.h++"


#ifdef _MSC_VER
   void initNetwork()
   {
      WORD      wsVersion;
      WSADATA   wsData;

      // Initialize Winsock (in Windows, we have to do this before
      // doing any networking)
      wsVersion = MAKEWORD(2, 0);
      WSAStartup(wsVersion, &wsData);
   }


   void cleanupNetwork()
   {
      // Shutdown Winsock
      WSACleanup();
   }


   Socket openSocket(int domain, int type, int protocol)
   {
      // Create a socket and return it
      return socket(domain, type, protocol);
   }


   void closeSocket(Socket socket)
   {
      // Close the socket
      closesocket(socket);
   }


   void setBlockingFlag(Socket socket, bool block)
   {
      u_long   nonblockMode;

      // Set up a mode field
      if (block == true)
         nonblockMode = 0;
      else
         nonblockMode = 1;

      ioctlsocket(socket, FIONBIO, &nonblockMode);
   }


   const char * netAddrToStr(int family, void * src, char * dst, int dstLen)
   {
      // Call the Windows version of InetNtop()
      return InetNtop(family, src, dst, dstLen);
   }


   int strToNetAddr(int family, char * src, void * dst)
   {
      // Call the Windows version of InetPton()
      return InetPton(family, src, dst);
   }
#else
   void initNetwork()
   {
   }


   void cleanupNetwork()
   {
   }


   Socket openSocket(int domain, int type, int protocol)
   {
      // Create a socket and return it
      return socket(domain, type, protocol);
   }


   void closeSocket(Socket socket)
   {
      // Close the socket
      close(socket);
   }


   void setBlockingFlag(Socket socket, bool block)
   {
      long statusFlags;

      // First, get the current status of the socket
      if ( !((statusFlags = fcntl(socket, F_GETFL)) < 0) )
      {
         // Set new status as appropriate
         if (block == true)
         {
            // Set socket to blocking
            fcntl(socket, F_SETFL, statusFlags & (~FNONBLOCK));
         }
         else
         {
            // Set socket to non-blocking
            fcntl(socket, F_SETFL, statusFlags | FNONBLOCK);
         }
      }
   }


   const char * netAddrToStr(int family, void * src, char * dst, int dstLen)
   {
      // Call the standard version of inet_ntop()
      return inet_ntop(family, src, dst, dstLen);
   }


   int strToNetAddr(int family, char * src, void * dst)
   {
      // Call the standard version of inet_pton()
      return inet_pton(family, src, dst);
   }
#endif

