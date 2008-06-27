
// INCLUDES
#include <fcntl.h>
#include <string.h>
#include "atNetworkInterface.h++"


atNetworkInterface::atNetworkInterface()
{
   // Initialize the network library if necessary
   initNetwork();

   // Initialize blocking mode
   blocking_mode = false;
   disableBlocking();

   // Clear the name fields
   read_name_length = sizeof(read_name);
   memset(&read_name, 0, read_name_length);
   write_name_length = sizeof(write_name);
   memset(&write_name, 0, write_name_length);
}


atNetworkInterface::~atNetworkInterface()
{
   // Clean-up the network library if necessary
   cleanupNetwork();
}


void atNetworkInterface::enableBlocking()
{
   // Set the socket to be blocking
   setBlockingFlag(socket_value, true);
   blocking_mode = true;
}


void atNetworkInterface::disableBlocking()
{
   // Set the socket to be non-blocking
   setBlockingFlag(socket_value, false);
   blocking_mode = false;
}

