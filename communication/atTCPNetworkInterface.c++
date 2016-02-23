
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


// INCLUDES

#include "atTCPNetworkInterface.h++"


// CONSTANTS
#define AT_TCP_MAX_PACKET_SIZE   65536


atTCPNetworkInterface::atTCPNetworkInterface(char * address, u_short port)
{
   SocketOptionValue   max;
   SocketOptionValue   on;
   char                hostname[MAXHOSTNAMELEN];
   struct hostent *    host;

   // Open the socket
   if ( (socket_value = openSocket(AF_INET, SOCK_STREAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

   // Set the max buffer size for this new socket (Windows has very
   // low defaults so we fix them across our applications)
   max = AT_TCP_MAX_PACKET_SIZE;
   if (setsockopt(socket_value, SOL_SOCKET, SO_SNDBUF, 
                  (char *) &max, sizeof(max)) < 0)
      perror("setsockopt sndbuf");
   if (setsockopt(socket_value, SOL_SOCKET, SO_RCVBUF, 
                  (char *) &max, sizeof(max)) < 0)
      perror("setsockopt sndbuf");

   // Set option to send KEEPALIVE (this prevents TCP connections from 
   // being closed on inactivity)
   on = 1;
   if (setsockopt(socket_value, SOL_SOCKET, SO_KEEPALIVE, 
                  (char *) &on, sizeof(on)) < 0)
   {
      perror("setsockopt keepalive");
   }

   // Get information about this host and initialize the read name field
   gethostname(hostname, sizeof(hostname));
   host = gethostbyname(hostname);
   read_name.sin_family = AF_INET;
   memcpy(&read_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   read_name.sin_port = htons(port);

   // Get information about remote host and initialize the write name field
   host = gethostbyname(address);
   write_name.sin_family = AF_INET;
   memcpy(&write_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   write_name.sin_port = htons(port);

   // Initialize remaining instance variables
   num_client_sockets = 0;
}


atTCPNetworkInterface::atTCPNetworkInterface(u_short port)
{
   SocketOptionValue   max;
   SocketOptionValue   on;
   char                hostname[MAXHOSTNAMELEN];
   struct hostent *    host;

   // Open the socket
   if ( (socket_value = openSocket(AF_INET, SOCK_STREAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

   // Set the max buffer size for this new socket (Windows has very
   // low defaults so we fix them across our applications)
   max = AT_TCP_MAX_PACKET_SIZE;
   if (setsockopt(socket_value, SOL_SOCKET, SO_SNDBUF, 
                  (char *) &max, sizeof(max)) < 0)
      perror("setsockopt sndbuf");
   if (setsockopt(socket_value, SOL_SOCKET, SO_RCVBUF, 
                  (char *) &max, sizeof(max)) < 0)
      perror("setsockopt sndbuf");

   // Set option to send KEEPALIVE (this prevents TCP connections from 
   // being closed on inactivity)
   on = 1;
   if (setsockopt(socket_value, SOL_SOCKET, SO_KEEPALIVE, 
                  (char *) &on, sizeof(on)) < 0)
   {
      perror("setsockopt keepalive");
   }


   // Get information about this host and initialize the read name field
   gethostname(hostname, sizeof(hostname));
   host = gethostbyname(hostname);
   read_name.sin_family = AF_INET;
   memcpy(&read_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   read_name.sin_port = htons(port);

   // Get information about remote host and initialize the write name field
   write_name.sin_family = AF_INET;
   memcpy(&write_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   write_name.sin_port = htons(port);

   // Initialize remaining instance variables
   num_client_sockets = 0;
}


atTCPNetworkInterface::~atTCPNetworkInterface()
{
   u_long   i;

   // Close all the client sockets
   for (i=0; i < num_client_sockets; i++)
      closeSocket(client_sockets[i]);

   // Close the socket
   if (socket_value != -1)
      closeSocket(socket_value);
}


bool atTCPNetworkInterface::allowConnections(int backlog)
{
   bool   bindSuccess;

   // Bind to the port
   if (bind(socket_value, (struct sockaddr *) &read_name, 
            sizeof(read_name)) >= 0)
   {
      // Indicate success
      bindSuccess = true;
   }
   else
   {
      // This is a serious problem
      notify(AT_ERROR, "Unable to bind to the port.\n");
      bindSuccess = false;
   }

   // Notify our willingness to accept connections and give a backlog limit
   listen(socket_value, backlog);

   // Indicate to the user whether the port could be successfully bound
   return bindSuccess;
}


int atTCPNetworkInterface::acceptConnection()
{
   fd_set               readFds;
   struct timeval       timeout;
   Socket               newSocket;
   struct sockaddr_in   connectingName;
   socklen_t            connectingNameLength;
   SocketOptionValue    max;
   SocketOptionValue    on;
   u_char *             address;
   u_short              addr0;
   u_short              addr1;
   u_short              addr2;
   u_short              addr3;

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
      // Set the max buffer size for this new socket (Windows has very
      // low defaults so we fix them across our applications)
      max = AT_TCP_MAX_PACKET_SIZE;
      if (setsockopt(socket_value, SOL_SOCKET, SO_SNDBUF, 
                     (char *) &max, sizeof(max)) < 0)
         perror("setsockopt sndbuf");
      if (setsockopt(socket_value, SOL_SOCKET, SO_RCVBUF, 
                     (char *) &max, sizeof(max)) < 0)
         perror("setsockopt sndbuf");

      // Set option to send KEEPALIVE (this prevents TCP connections from 
      // being closed on inactivity)
      on = 1;
      if (setsockopt(socket_value, SOL_SOCKET, SO_KEEPALIVE, 
                     (char *) &on, sizeof(on)) < 0)
      {
         perror("setsockopt keepalive");
      }

      // Store the data for this connection (address handling is a bit
      // nasty so that it also works in Windows)
      client_sockets[num_client_sockets] = newSocket;
      address = (u_char *) &connectingName.sin_addr.s_addr;
      addr0 = 0x00ff & address[0];
      addr1 = 0x00ff & address[1];
      addr2 = 0x00ff & address[2];
      addr3 = 0x00ff & address[3];
      sprintf(client_addrs[num_client_sockets].address, "%hhu.%hhu.%hhu.%hhu",
              addr0, addr1, addr2, addr3);
      client_addrs[num_client_sockets].port = connectingName.sin_port;
      client_blocking[num_client_sockets] = true;
      num_client_sockets++;
      return num_client_sockets - 1;
   }
}


void atTCPNetworkInterface::enableBlockingOnClient(int clientID)
{
   // Set the client socket to blocking
   client_blocking[clientID] = true;
   setBlockingFlag(client_sockets[clientID], true);
}


void atTCPNetworkInterface::disableBlockingOnClient(int clientID)
{
   // Set the client socket to non-blocking
   client_blocking[clientID] = false;
   setBlockingFlag(client_sockets[clientID], false);
}


void atTCPNetworkInterface::enableDelay()
{
   SocketOptionFlag   flag;

   // Set the NO_DELAY flag to false
   flag = 0;

   // Try to enable TCP delay on the socket
   if (setsockopt(socket_value, IPPROTO_TCP, TCP_NODELAY,
                  &flag, sizeof(int)) < 0)
      notify(AT_ERROR, "Unable to enable TCP delay on socket.\n");
}


void atTCPNetworkInterface::disableDelay()
{
   SocketOptionFlag   flag;

   // Set the NO_DELAY flag to true
   flag = 1;

   // Try to disable TCP delay on the socket
   if (setsockopt(socket_value, IPPROTO_TCP, TCP_NODELAY,
                  &flag, sizeof(int)) < 0)
      notify(AT_ERROR, "Unable to disable TCP delay on socket.\n");
}


void atTCPNetworkInterface::enableDelayOnClient(int clientID)
{
   SocketOptionFlag   flag;

   // Set the NO_DELAY flag to false
   flag = 0;

   // Try to enable TCP delay on the socket
   if (setsockopt(client_sockets[clientID], IPPROTO_TCP, TCP_NODELAY,
                  &flag, sizeof(int)) < 0)
      notify(AT_ERROR, "Unable to enable TCP delay on socket.\n");
}


void atTCPNetworkInterface::disableDelayOnClient(int clientID)
{
   SocketOptionFlag   flag;

   // Set the NO_DELAY flag to true
   flag = 1;

   // Try to disable TCP delay on the socket
   if (setsockopt(client_sockets[clientID], IPPROTO_TCP, TCP_NODELAY,
                  &flag, sizeof(int)) < 0)
      notify(AT_ERROR, "Unable to disable TCP delay on socket.\n");
}


ClientAddr atTCPNetworkInterface::getClientInfo(int clientID)
{
   // Return the information for this client ID
   return client_addrs[clientID];
}


int atTCPNetworkInterface::makeConnection()
{
   int                  keepTrying;
   struct sockaddr_in   connectingName;
   fd_set               readFds;
   fd_set               writeFds;
   struct timeval       timeout;
   int                  errorCode;
   socklen_t            errorLength;
   SocketOptionValue    max;
   SocketOptionValue    on;

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
            if ( (socket_value = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
               notify(AT_ERROR, "Unable to open socket for communication.\n");

            // Put flags from previous socket on this new socket
            if (blocking_mode == true)
               enableBlocking();
            else
               disableBlocking();

            // Set the max buffer size for this new socket (Windows has very
            // low defaults so we fix them across our applications)
            max = AT_TCP_MAX_PACKET_SIZE;
            if (setsockopt(socket_value, SOL_SOCKET, SO_SNDBUF, 
                           (char *) &max, sizeof(max)) < 0)
               perror("setsockopt sndbuf");
            if (setsockopt(socket_value, SOL_SOCKET, SO_RCVBUF, 
                           (char *) &max, sizeof(max)) < 0)
               perror("setsockopt sndbuf");

            // Set option to send KEEPALIVE (this prevents TCP connections from 
            // being closed on inactivity)
            on = 1;
            if (setsockopt(socket_value, SOL_SOCKET, SO_KEEPALIVE, 
                           (char *) &on, sizeof(on)) < 0)
            {
               perror("setsockopt keepalive");
            }
         }
      }
   }

   // Tell the user whether or not we succeeded to connect
   if (socket_value == -1)
   {
      // Let's be nice and set-up the socket for next time (we assume
      // the application will just try to re-connect again)
      if ( (socket_value = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
         notify(AT_ERROR, "Unable to open socket for communication.\n");

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


int atTCPNetworkInterface::read(u_char * buffer, u_long len)
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


int atTCPNetworkInterface::read(int clientID, u_char * buffer, u_long len)
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


int atTCPNetworkInterface::write(u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(socket_value, (const char *) buffer, len, 0, 
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}


int atTCPNetworkInterface::write(int clientID, u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(client_sockets[clientID], (const char *) buffer, len,
                          0,  
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}

