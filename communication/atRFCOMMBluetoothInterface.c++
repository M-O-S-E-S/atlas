
// INCLUDES
#include "atRFCOMMBluetoothInterface.h++"


// CONSTANTS
#define AT_RFCOMM_MAX_PACKET_SIZE   65536


atRFCOMMBluetoothInterface::atRFCOMMBluetoothInterface(char * address,
   u_char channel)
{
   char               hostname[MAXHOSTNAMELEN];
   struct hostent *   host;

   // Open the socket
   if ( (socket_value =
      openSocket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0 )
   {
      notify(AT_ERROR, "Unable to open socket for communication.\n");
   }

   // Set the read name field
   setBTAddress(&read_name, address, channel);

   // Set the write name field
   setBTAddress(&write_name, address, channel);

   // Initialize remaining instance variables
   num_client_sockets = 0;
}


atRFCOMMBluetoothInterface::atRFCOMMBluetoothInterface(u_char channel)
{
   char               hostname[MAXHOSTNAMELEN];
   struct hostent *   host;

   // Open the socket
   if ( (socket_value =
      openSocket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0 )
   {
      notify(AT_ERROR, "Unable to open socket for communication.\n");
   }

   // Set the read name field
   setBTAddress(&read_name, NULL, channel);

   // Set the write name field
   setBTAddress(&write_name, NULL, channel);

   // Initialize remaining instance variables
   num_client_sockets = 0;
}



atRFCOMMBluetoothInterface::~atRFCOMMBluetoothInterface()
{
   u_long   i;

   // Close all the client sockets
   for (i=0; i < num_client_sockets; i++)
      closeSocket(client_sockets[i]);

   // Close the socket
   if (socket_value != -1)
      closeSocket(socket_value);
}


void atRFCOMMBluetoothInterface::allowConnections(int backlog)
{
   // Bind to the port
   if (bind(socket_value, (struct sockaddr *) &read_name, 
            sizeof(read_name)) < 0)
   {
      notify(AT_ERROR, "Unable to bind to the port.\n");
   }

   // Notify our willingness to accept connections and give a backlog limit
   listen(socket_value, backlog);
}


int atRFCOMMBluetoothInterface::acceptConnection()
{
   fd_set               readFds;
   struct timeval       timeout;
   Socket               newSocket;
   BluetoothSockAddr    connectingName;
   socklen_t            connectingNameLength;

   // If we are in non-blocking mode, we use select() to see if
   // there is a client connecting; othwew
   FD_ZERO(&readFds);
   FD_SET(socket_value, &readFds);
   timeout.tv_sec = 0;
   timeout.tv_usec = 10000;
   if (select(socket_value+1, &readFds, NULL, NULL, &timeout) <= 0)
   {
      // We did not get a connection in time so determine our action
      // based on whether we're in non-blocking mode or not
      if (blocking_mode == false)
      {
         // We're in non-blocking mode so just return -1; otherwise, we
         // we fill fall through and just call accept() and block
         return -1;
      }
   }

   // Try to accept a connection
   connectingNameLength = sizeof(connectingName);
   newSocket = accept(socket_value, (struct sockaddr *) &connectingName, 
                      &connectingNameLength);

   // If we had an error and it wasn't that we would block on a non-blocking
   // socket (a blocking socket shouldn't generate an EWOULDBLOCK error), then
   // notify the user; otherwise, store the socket and return an ID to the user
   if (newSocket == -1)
   {
      // Output to user if necessary and return
      if (getLastError() != EWOULDBLOCK) 
         notify(AT_ERROR, "Could not accept a connection.\n");
      return -1;
   }
   else
   {
      // Store the data for this connection (address handling is a bit
      // nasty so that it also works in Windows)
      client_sockets[num_client_sockets] = newSocket;
      getBTAddress(&connectingName, client_addrs[num_client_sockets].address);
      getBTChannel(&connectingName, &client_addrs[num_client_sockets].channel);
      client_blocking[num_client_sockets] = true;
      num_client_sockets++;
      return num_client_sockets - 1;
   }
}


void atRFCOMMBluetoothInterface::enableBlockingOnClient(int clientID)
{
   // Set the client socket to blocking
   client_blocking[clientID] = true;
   setBlockingFlag(client_sockets[clientID], true);
}


void atRFCOMMBluetoothInterface::disableBlockingOnClient(int clientID)
{
   // Set the client socket to non-blocking
   client_blocking[clientID] = false;
   setBlockingFlag(client_sockets[clientID], false);
}


BTClientAddr atRFCOMMBluetoothInterface::getClientInfo(int clientID)
{
   // Return the information for this client ID
   return client_addrs[clientID];
}


int atRFCOMMBluetoothInterface::makeConnection()
{
   int                  keepTrying;
   BluetoothSockAddr    connectingName;
   fd_set               readFds;
   fd_set               writeFds;
   struct timeval       timeout;
   int                  errorCode;
   socklen_t            errorLength;
   SocketOptionValue    max;

   // Loop until we stop it
   keepTrying = 1;
   while (keepTrying == 1)
   {
      // Try to connect
      connectingName = write_name;
      if (connect(socket_value, (struct sockaddr *) &connectingName,
                  sizeof(connectingName)) != -1)
      {
         // We connected so signal the loop to end
         keepTrying = 0;
      }
      else
      {
         // If we are not in blocking mode, tell the loop to stop (we give up);
         // Otherwise, tell the user the info that we failed this time and
         // re-open the socket
         if (blocking_mode == false)
         {
            // We are non-blocking so we could be failing to connect
            // or we could just need more time (EINPROGRESS on normal systems, 
            // EWOULDBLOCK on Windows) so use select() to give it some time and
            // then check again
            errorCode = getLastError();
            if ((errorCode == EINPROGRESS) || (errorCode == EWOULDBLOCK))
            {
               notify(AT_INFO, "Waiting for connection.\n");
               FD_ZERO(&readFds);
               FD_SET(socket_value, &readFds);
               FD_ZERO(&writeFds);
               FD_SET(socket_value, &writeFds);
               timeout.tv_sec = 1;
               timeout.tv_usec = 0;
               if (select(socket_value+1, &readFds, 
                          &writeFds, NULL, &timeout) < 1)
               {
                  // We didn't connect so close the socket
                  notify(AT_INFO, "Failed to connect to server.  Giving up.\n");
                  keepTrying = 0;
                  closeSocket(socket_value);
                  socket_value = -1;
               }
               else
               {
                  // Normally, we would know we would have connected here,
                  // but low-level issues (I guess) are causing issues here
                  // so check the socket to make sure all is okay before
                  // declaring success
                  errorLength = sizeof(errorCode);
                  getsockopt(socket_value, SOL_SOCKET, SO_ERROR, 
                             (char * ) &errorCode, &errorLength);

                  // Check the socket error status
                  if (errorCode == 0)
                  {
                     // We actually did connect!
                     notify(AT_INFO, "Connected!\n");
                     keepTrying = 0;
                  }
                  else
                  {
                     // Failed to connect, but we thought we were okay
                     // so just give up because something is wrong
                     notify(AT_INFO,
                            "Failed to connect to server.  Giving up.\n");
                     keepTrying = 0;
                     closeSocket(socket_value);
                     socket_value = -1;
                  }
               }
            }
            else
            {
               // Just give up
               notify(AT_INFO, "Failed to connect to server.  Giving up.\n");
               keepTrying = 0;
               closeSocket(socket_value);
               socket_value = -1;
            }
         }
         else
         {
            // We didn't connect so close the socket
            closeSocket(socket_value);
            socket_value = -1;

            notify(AT_INFO, "Failed to connect to server.  Trying again.\n");

            // Re-open the socket
            if ( (socket_value = 
               openSocket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0 )
            {
               notify(AT_ERROR, "Unable to open socket for communication.\n");
            }

            // Put flags from previous socket on this new socket
            if (blocking_mode == true)
               enableBlocking();
            else
               disableBlocking();
         }
      }
   }

   // Tell the user whether or not we succeeded to connect
   if (socket_value == -1)
   {
      // Let's be nice and set-up the socket for next time (we assume
      // the application will just try to re-connect again)
      if ( (socket_value = 
         openSocket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0 )
      {
         notify(AT_ERROR, "Unable to open socket for communication.\n");
      }

      // Put flags from previous socket on this new socket
      if (blocking_mode == true)
         enableBlocking();
      else
         disableBlocking();

      return -1;
   }
   else
      return 0;
}


int atRFCOMMBluetoothInterface::read(u_char * buffer, u_long len)
{
   struct sockaddr_in   fromAddress;
   socklen_t            fromAddressLength;
   int                  packetLength;

   // Get a packet
   fromAddressLength = sizeof(fromAddress);
   if (blocking_mode == true)
   {
      packetLength = recvfrom(socket_value, (char *) buffer, len, MSG_WAITALL, 
                              (struct sockaddr *) &fromAddress, 
                              &fromAddressLength);
   }
   else
   {
      packetLength = recvfrom(socket_value, (char *) buffer, len, 0, 
                              (struct sockaddr *) &fromAddress, 
                              &fromAddressLength);
   }

   // Tell user how many bytes we read (-1 means an error)
   return packetLength;
}


int atRFCOMMBluetoothInterface::read(int clientID, u_char * buffer, u_long len)
{
   struct sockaddr_in   fromAddress;
   socklen_t            fromAddressLength;
   int                  packetLength;

   // Get a packet
   fromAddressLength = sizeof(fromAddress);
   if (client_blocking[clientID] == true)
   {
      packetLength = recvfrom(client_sockets[clientID], (char *) buffer, len, 
                              MSG_WAITALL,
                              (struct sockaddr *) &fromAddress, 
                              &fromAddressLength);
   }
   else
   {
      packetLength = recvfrom(client_sockets[clientID], (char *) buffer, len, 
                              0,
                              (struct sockaddr *) &fromAddress, 
                              &fromAddressLength);
   }

   // Tell user how many bytes we read (-1 means an error)
   return packetLength;
}


int atRFCOMMBluetoothInterface::write(u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(socket_value, (const char *) buffer, len, 0, 
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}


int atRFCOMMBluetoothInterface::write(int clientID, u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(client_sockets[clientID], (const char *) buffer, len,
                          0,  
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}

