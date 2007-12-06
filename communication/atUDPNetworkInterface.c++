
// INCLUDES
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include "atUDPNetworkInterface.h++"


atUDPNetworkInterface::atUDPNetworkInterface(char * readAddress, 
   char * writeAddress, short port)
{
   char               hostname[MAXHOSTNAMELEN];
   struct hostent *   host;
   int                on;
   u_long             firstOctet;
   struct ip_mreq     mreq;

   // Open the socket
   if ( (socket_value = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

   // Get information about the read address and initialize the read name field
   // (we mark it as reading from any node so that a node can respond
   // to the broadcast address if desired and we'll still get it)
   host = gethostbyname(readAddress);
   read_name.sin_family = AF_INET;
   memcpy(&read_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   read_name.sin_port = htons(port);

   // Get information about remote host and initialize the write name field
   host = gethostbyname(writeAddress);
   write_name.sin_family = AF_INET;
   memcpy(&write_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   write_name.sin_port = htons(port);

   // Allow multiple sockets to use the same port number
   on = 1;
   if (setsockopt(socket_value, SOL_SOCKET, SO_REUSEADDR, 
                  &on, sizeof(on)) < 0)
   {
      notify(AT_WARN, "Unable to reuse the port.\n");
   }

   // Bind to the port
   if (bind(socket_value, (struct sockaddr *) &read_name, 
            sizeof(read_name)) < 0)
   {
      notify(AT_ERROR, "Unable to bind to the port.\n");
   }

   // If we are connecting to a multicast address, set the socket
   // appropriately
   firstOctet = ntohl(write_name.sin_addr.s_addr) >> 24;
   if ( (firstOctet >= 224) && (firstOctet <= 239) )
   {
      // Join the multicast group
      notify(AT_INFO, "Joining multicast group.\n");
      memcpy(&mreq.imr_multiaddr.s_addr, host->h_addr_list[0], host->h_length);
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);
      if (setsockopt(socket_value, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                     &mreq, sizeof(mreq)) < 0)
         notify(AT_WARN, "Unable to join multicast group on socket.\n");
   }

   // Ignore our own broadcasted packets by default
   ignore_our_own = true;
}


atUDPNetworkInterface::atUDPNetworkInterface(char * writeAddress, short port)
{
   char               hostname[MAXHOSTNAMELEN];
   struct hostent *   host;
   int                on;
   u_long             firstOctet;
   struct ip_mreq     mreq;

   // Open the socket
   if ( (socket_value = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

   // Get information about this host and initialize the read name field
   // (we mark it as reading from any node so that a node can respond
   // to the broadcast address if desired and we'll still get it)
   gethostname(hostname, sizeof(hostname));
   host = gethostbyname(hostname);
   read_name.sin_family = AF_INET;
   read_name.sin_addr.s_addr = htonl(INADDR_ANY);
   read_name.sin_port = htons(port);

   // Get information about remote host and initialize the write name field
   host = gethostbyname(writeAddress);
   write_name.sin_family = AF_INET;
   memcpy(&write_name.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   write_name.sin_port = htons(port);

   // Allow multiple sockets to use the same port number
   on = 1;
   if (setsockopt(socket_value, SOL_SOCKET, SO_REUSEADDR, 
                  &on, sizeof(on)) < 0)
   {
      notify(AT_WARN, "Unable to reuse the port.\n");
   }

   // Bind to the port
   if (bind(socket_value, (struct sockaddr *) &read_name, 
            sizeof(read_name)) < 0)
   {
      notify(AT_ERROR, "Unable to bind to the port.\n");
   }

   // If we are connecting to a multicast address, set the socket
   // appropriately
   firstOctet = ntohl(write_name.sin_addr.s_addr) >> 24;
   if ( (firstOctet >= 224) && (firstOctet <= 239) )
   {
      // Join the multicast group
      notify(AT_INFO, "Joining multicast group.\n");
      memcpy(&mreq.imr_multiaddr.s_addr, host->h_addr_list[0], host->h_length);
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);
      if (setsockopt(socket_value, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                     &mreq, sizeof(mreq)) < 0)
         notify(AT_WARN, "Unable to join multicast group on socket.\n");
   }

   // Ignore our own broadcasted packets by default
   ignore_our_own = true;
}


atUDPNetworkInterface::atUDPNetworkInterface(short port)
{
   int   on;

   // Open the socket
   if ( (socket_value = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
      notify(AT_ERROR, "Unable to open socket for communication.\n");

   // Initialize the name fields
   read_name.sin_family = AF_INET;
   read_name.sin_addr.s_addr = htonl(INADDR_ANY);
   read_name.sin_port = htons(port);
   write_name.sin_family = AF_INET;
   write_name.sin_addr.s_addr = htonl(INADDR_BROADCAST);
   write_name.sin_port = htons(port);

   // Set the options we need for broadcasting
   on = 1;
   if (setsockopt(socket_value, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
      notify(AT_WARN, "Unable to set broadcasting on socket.\n");

   // Bind to the port
   if (bind(socket_value, (struct sockaddr *) &read_name, 
            sizeof(read_name)) < 0)
   {
      notify(AT_ERROR, "Unable to bind to the port.\n");
   }

   // Ignore our own broadcasted packets by default
   ignore_our_own = true;
}


atUDPNetworkInterface::~atUDPNetworkInterface()
{
   // Close the socket
   close(socket_value);
}


int atUDPNetworkInterface::read(u_char * buffer, u_long len)
{
   bool                 loop;
   struct sockaddr_in   fromAddress;
   socklen_t            fromAddressLength;
   int                  packetLength;
   char                 hostname[MAXHOSTNAMELEN];
   struct hostent *     host;

   // Keep looping if necessary
   loop = true;
   while (loop == true)
   {
      // Get a packet
      fromAddressLength = sizeof(fromAddress);
      packetLength = recvfrom(socket_value, buffer, len, 0, 
                              (struct sockaddr *) &fromAddress, 
                              &fromAddressLength);

      // In broadcast mode we're going to receive our own packets back
      // so make sure we ignore them if requested
      if ( (ignore_our_own == true) && (packetLength > 0) )
      {
         gethostname(hostname, sizeof(hostname));
         host = gethostbyname(hostname);
         if (memcmp(&fromAddress.sin_addr.s_addr, 
                    host->h_addr_list[0], host->h_length) == 0)
         {
            // It was our own
   
            // If we were not blocking, we should just return that nothing 
            // was read (if we were blocking, we just loop around again)
            if ((fcntl(socket_value, F_GETFL) & FNONBLOCK) != 0)
            {
               // Set length to zero and stop looping
               packetLength = 0;
               loop = false;
            }
         }
         else
         {
            // It wasn't from us so accept it (stop looping)
            loop = false;
         }
      }
      else
      {
         // We don't ignore out own so accept it (stop looping)
         loop = false;
      }
   }

   // Tell user how many bytes we read (-1 if error)
   return packetLength;
}


int atUDPNetworkInterface::write(u_char * buffer, u_long len)
{
   int   lengthWritten;

   // Write the packet
   lengthWritten = sendto(socket_value, buffer, len, 0, 
                          (struct sockaddr *) &write_name, write_name_length);

   // Tell user how many bytes we wrote (-1 if error)
   return lengthWritten;
}

