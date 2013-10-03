
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


   const char * addrToStr(int family, void * src, char * dst, int dstLen)
   {
      // Call the Windows version of InetNtop()
      return InetNtop(family, src, dst, dstLen);
   }


   int strToAddr(int family, char * src, void * dst)
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


   const char * addrToStr(int family, void * src, char * dst, int dstLen)
   {
      // Call the standard version of inet_ntop()
      return inet_ntop(family, src, dst, dstLen);
   }


   int strToAddr(int family, char * src, void * dst)
   {
      // Call the standard version of inet_pton()
      return inet_pton(family, src, dst);
   }
#endif

