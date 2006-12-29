
#include "atThreadInterface.h++"


// CONSTANTS
#define AT_THR_INIT_QUEUE_SIZE   5000 
#define AT_THR_INC_QUEUE_SIZE   10000


atThreadInterface::atThreadInterface()
{
   // Create a read queue and a write queue for communication messages
   read_queue = new atThreadQueue(AT_THR_INIT_QUEUE_SIZE, 
                                  AT_THR_INC_QUEUE_SIZE);
   write_queue = new atThreadQueue(AT_THR_INIT_QUEUE_SIZE, 
                                   AT_THR_INC_QUEUE_SIZE);
}


atThreadInterface::~atThreadInterface()
{
   // Delete the queues
   delete read_queue;
   delete write_queue;
}


int atThreadInterface::read(u_char * buffer, u_long length)
{
   // Try to read the next queue entry
   if (read_queue->dequeue(buffer, &length) == true)
   {
      // If we succeeded, return the length of what was read
      return length;
   }
   else
   {
      // Otherwise, return zero (we did not succeed in reading from the queue)
      return 0;
   }
}


int atThreadInterface::write(u_char * buffer, u_long length)
{
   // Write the buffer to the queue
   write_queue->enqueue(buffer, length);

   // At least for now, we assume enqueuing to the shared buffer will work
   // everytime
   return length;
}

