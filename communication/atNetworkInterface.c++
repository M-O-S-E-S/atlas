
// INCLUDES
#include <fcntl.h>
#include <string.h>
#include "atNetworkInterface.h++"


atNetworkInterface::atNetworkInterface()
{
   // Clear the name fields
   read_name_length = sizeof(read_name);
   memset(&read_name, 0, read_name_length);
   write_name_length = sizeof(write_name);
   memset(&write_name, 0, write_name_length);
}


atNetworkInterface::~atNetworkInterface()
{
}


void atNetworkInterface::enableBlocking()
{
   int   statusFlags;

   if ( (statusFlags = fcntl(socket_value, F_GETFL)) < 0 )
      notify(AT_ERROR, "Unable to get status of socket.\n");
   else
   {
      if (fcntl(socket_value, F_SETFL, statusFlags & (~FNONBLOCK)) < 0)
         notify(AT_ERROR, "Unable to enable blocking on socket.\n");
   }
}


void atNetworkInterface::disableBlocking()
{
   int   statusFlags;

   if ( (statusFlags = fcntl(socket_value, F_GETFL)) < 0 )
      notify(AT_ERROR, "Unable to get status of socket.\n");
   else
   {
      if (fcntl(socket_value, F_SETFL, statusFlags | FNONBLOCK) < 0)
         notify(AT_ERROR, "Unable to disable blocking on socket.\n");
   }
}

