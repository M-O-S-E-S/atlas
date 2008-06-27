
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

      if (ioctlsocket(socket, FIONBIO, &blockMode) == SOCKET_ERROR)
         notify(AT_ERROR, "Unable to set blocking on socket.\n");
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
      // First, get the current status of the socket
      if ( (statusFlags = fcntl(socket, F_GETFL)) < 0 )
         notify(AT_ERROR, "Unable to get status of socket.\n");
      else
      {
         // Set new status as appropriate
         if (block == true)
         {
            // Set socket to blocking
            if (fcntl(socket, F_SETFL, statusFlags & (~FNONBLOCK)) < 0)
               notify(AT_ERROR, "Unable to enable blocking on socket.\n");
         }
         else
         {
            // Set socket to non-blocking
            if (fcntl(socket, F_SETFL, statusFlags | FNONBLOCK) < 0)
               notify(AT_ERROR, "Unable to disable blocking on socket.\n");
         }
      }
   }
#endif

