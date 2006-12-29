
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "atMessageQueueInterface.h++"


// CONSTANTS
#define MSG_SIZE   65535 


// TYPES
typedef struct
{
   long   msgType;
   char   msgText[MSG_SIZE];
} atMessageBuffer;


atMessageQueueInterface::atMessageQueueInterface(key_t key)
{
   // Keep the key to use later
   msg_key = key;

   // Get a message queue
   msg_id = msgget(msg_key, 0666 | IPC_CREAT);
   if (msg_id == -1)
      notify(AT_FATAL_ERROR, "Failed to get message queue.\n");

   // Initialize blocking variable
   want_blocking = true;
}


atMessageQueueInterface::~atMessageQueueInterface()
{
   // Delete the message queue
   msgctl(msg_key, IPC_RMID, NULL);
}


void atMessageQueueInterface::enableBlocking()
{
   // Set our internal variable to designate that the user wants to block
   // when reading or writing (used in read() and write())
   want_blocking = true;
}


void atMessageQueueInterface::disableBlocking()
{
   // Set our internal variable to designate that the user does not want to 
   // block when reading or writing (used in read() and write())
   want_blocking = false;
}


int atMessageQueueInterface::read(u_char *buffer, u_long length)
{
   atMessageBuffer   msg;

   // Get a message from the queue
   if (want_blocking)
   {
      // Try to receive a message -- We will block by default so this
      // will only return prematurely due to a real error
      if ( msgrcv(msg_id, &msg, length, 0, 0) < 0 )
      {
         // Tell the user we had a problem and return zero bytes read
         notify(AT_ERROR, "Failed to remove message from message queue.\n");
         return 0;
      }
      else
      {
         // Copy the received text into the user's buffer and return length
         memcpy(buffer, &msg.msgText, length);
         return length;
      }
   }
   else
   {
      // Try to receive a message and tell it not to block
      if ( msgrcv(msg_id, &msg, length, 0, IPC_NOWAIT) < 0 )
      {
         // Only output an error if it wasn't because we wouldn't block
         if (errno != ENOMSG)
            notify(AT_ERROR, "Failed to remove message from message queue.\n");

         return 0;
      }
      else
      {
         // Copy the received text into the user's buffer and return length
         memcpy(buffer, &msg.msgText, length);
         return length;
      }
   }
}


int atMessageQueueInterface::write(u_char * buffer, u_long length)
{
   atMessageBuffer   msg;

   // Define a message
   msg.msgType = 1;
   memcpy(msg.msgText, buffer, length);

   // Send the message and return to the user how many bytes we sent
   if (want_blocking)
   {
      // Try to send message and report any error
      if (msgsnd(msg_id, &msg, length, 0) < 0)
      {
         // Report errors and return zero if we failed
         notify(AT_ERROR, "Failed to add message to message queue.\n");
         return 0;
      }
      else
      {
         // If we succeeded, return the length of what we wrote
         return length;
      }
   }
   else
   {
      // Send message and do not allow blocking
      if (msgsnd(msg_id, &msg, length, IPC_NOWAIT) < 0)
      {
         // Tell the user about errors not related to blocking
         if (errno != EAGAIN)
            notify(AT_ERROR, "Failed to add message to message queue.\n");

         // Return zero that we had an error
         return 0;
      }
      else
      {
         // If we succeeded, return the length of what we wrote
         return length;
      }
   }
}

