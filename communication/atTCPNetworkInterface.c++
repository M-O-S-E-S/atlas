
// INCLUDES
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/select.h>
#include "atTCPNetworkInterface.h++"


atTCPNetworkInterface::atTCPNetworkInterface(char * address, short port)
{
   char               hostname[MAXHOSTNAMELEN];
   struct hostent *   host;

   // Open the socket
   if ( (socket_value = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

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


atTCPNetworkInterface::atTCPNetworkInterface(short port)
{
   char               hostname[MAXHOSTNAMELEN];
   struct hostent *   host;

   // Open the socket
   if ( (socket_value = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

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
   // Close the socket
   close(socket_value);
}


void atTCPNetworkInterface::allowConnections(int backlog)
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


int atTCPNetworkInterface::acceptConnection()
{
   int                  newSocket;
   struct sockaddr_in   connectingName;
   socklen_t            connectingNameLength;
   char *               address;

   // Try to accept a connection
   connectingNameLength = sizeof(connectingName);
   newSocket = accept(socket_value, (struct sockaddr *) &connectingName, 
                      &connectingNameLength);

   // If we had an error and it wasn't that we would block on a non-blocking
   // socket (a blocking socket shouldn't generate an EWOULDBLOCK error), then
   // notify the user; otherwise, store the socket and return an ID to the user
   if (newSocket == -1)
   {
      if (errno != EWOULDBLOCK) 
         notify(AT_ERROR, "Could not accept a connection.\n");
      return -1;
   }
   else
   {
      client_sockets[num_client_sockets] = newSocket;
      address = (char *) connectingName.sin_addr.s_addr;
      sprintf(client_addrs[num_client_sockets].address, "%d.%d.%d.%d",
              address[0], address[1], address[2], address[3]);
      client_addrs[num_client_sockets].port = connectingName.sin_port;
      num_client_sockets++;
      return num_client_sockets - 1;
   }
}


void atTCPNetworkInterface::enableBlockingOnClient(int clientID)
{
   int   statusFlags;

   // Get the current flags on the socket (return an error if we fail)
   if ( (statusFlags = fcntl(client_sockets[clientID], F_GETFL)) < 0 )
      notify(AT_ERROR, "Unable to get status of socket.\n");
   else
   {
      // Now set the flags back while removing the non-blocking bit
      if (fcntl(client_sockets[clientID], F_SETFL, 
                statusFlags & (~FNONBLOCK)) < 0)
      {
         // Report an error if we fail
         notify(AT_ERROR, "Unable to disable blocking on socket.\n");
      }
   }
}


void atTCPNetworkInterface::disableBlockingOnClient(int clientID)
{
   int   statusFlags;

   // Get the current flags on the socket (return an error if we fail)
   if ( (statusFlags = fcntl(client_sockets[clientID], F_GETFL)) < 0 )
      notify(AT_ERROR, "Unable to get status of socket.\n");
   else
   {
      // Now set the flags back while adding the non-blocking bit (report any
      // errors we get if we fail)
      if (fcntl(client_sockets[clientID], F_SETFL, statusFlags | FNONBLOCK) < 0)
         notify(AT_ERROR, "Unable to disable blocking on socket.\n");
   }
}


ClientAddr atTCPNetworkInterface::getClientInfo(int clientID)
{
   // Return the information for this client ID
   return client_addrs[clientID];
}


int atTCPNetworkInterface::makeConnection()
{
   int                  statusFlags;
   int                  keepTrying;
   struct sockaddr_in   connectingName;
   fd_set               readFds;
   fd_set               writeFds;
   struct timeval       timeout;

   // Get flags on our current socket (so we can put them on new sockets if
   // needed)
   if ( (statusFlags = fcntl(socket_value, F_GETFL)) < 0 )
      notify(AT_ERROR, "Unable to get status of socket.\n");

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
         if ( (fcntl(socket_value, F_GETFL) & FNONBLOCK) != 0 )
         {
            // We are non-blocking so we could be failing to connect
            // or we could just need more time (EINPROGRESS) so
            // use select() to give it some time and then check again
            if (errno == EINPROGRESS)
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
                  keepTrying = 0;
                  close(socket_value);
                  socket_value = -1;
               }
               else
               {
                  // We actually did connect!
                  keepTrying = 0;
               }
            }
            else
            {
               // Just give up
               notify(AT_INFO, "Failed to connect to server.  Giving up.\n");
               keepTrying = 0;
               close(socket_value);
               socket_value = -1;
            }
         }
         else
         {
            // We didn't connect so close the socket
            close(socket_value);
            socket_value = -1;

            notify(AT_INFO, "Failed to connect to server.  Trying again.\n");

            // Re-open the socket
            if ( (socket_value = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
               notify(AT_ERROR, "Unable to open socket for communication.\n");

            // Put flags from previous socket on this new socket
            if (fcntl(socket_value, F_SETFL, statusFlags) < 0)
               notify(AT_ERROR, "Unable to disable blocking on socket.\n");
         }
      }
   }

   // Tell the user whether or not we succeeded to connect
   if (socket_value == -1)
      return -1;
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
   packetLength = recvfrom(socket_value, buffer, len, MSG_WAITALL, 
                           (struct sockaddr *) &fromAddress, 
                           &fromAddressLength);

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
   packetLength = recvfrom(client_sockets[clientID], buffer, len, MSG_WAITALL,
                           (struct sockaddr *) &fromAddress, 
                           &fromAddressLength);

   // Tell user how many bytes we read (-1 means an error)
   return packetLength;
}


int atTCPNetworkInterface::write(u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(socket_value, buffer, len, 0, 
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}


int atTCPNetworkInterface::write(int clientID, u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(client_sockets[clientID], buffer, len, 0,  
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}

