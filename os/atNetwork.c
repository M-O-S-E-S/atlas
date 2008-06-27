
#include "atNetwork.h"


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


   void setBlockingFlag(Socket socket, bool block)
   {
      u_long   blockMode;

      // Set up a mode field
      if (block == true)
         blockMode = 1;
      else
         blockMode = 0;

      ioctlsocket(socket, FIONBIO, &blockMode);
   }
#else
   void initNetwork()
   {
   }


   void cleanupNetwork()
   {
   }


   void setBlockingFlag(Socket socket, bool block)
   {
      int statusFlags;

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
#endif

