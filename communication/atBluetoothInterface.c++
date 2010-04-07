
// INCLUDES
#include <fcntl.h>
#include <string.h>
#include "atBluetoothInterface.h++"


atBluetoothInterface::atBluetoothInterface()
{
   // Initialize the network library if necessary
   initNetwork();
}


atBluetoothInterface::~atBluetoothInterface()
{
   // Clean-up the network library if necessary
   cleanupNetwork();
}


void atBluetoothInterface::enableBlocking()
{
   // Set the socket to be blocking
   setBlockingFlag(socket_value, true);
   blocking_mode = true;
}


void atBluetoothInterface::disableBlocking()
{
   // Set the socket to be non-blocking
   setBlockingFlag(socket_value, false);
   blocking_mode = false;
}

