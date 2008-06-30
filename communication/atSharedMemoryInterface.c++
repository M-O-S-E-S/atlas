
#include "atSharedMemoryInterface.h++"


// CONSTANTS
#define AT_SHM_INIT_QUEUE_SIZE   5000 
#define AT_SHM_INC_QUEUE_SIZE   10000


atSharedMemoryInterface::atSharedMemoryInterface(ShmKey readKey, ShmKey writeKey)
{
   // Create a read queue and a write queue for communication messages
   // through shared memory (we create a second key by subtracting it from
   // 0x1000 since the Shared Queue needs separate "control" and "data" 
   // keys)
   read_queue = new atSharedQueue(readKey, 0x10000-readKey, 
                                  AT_SHM_INIT_QUEUE_SIZE, 
                                  AT_SHM_INC_QUEUE_SIZE);
   write_queue = new atSharedQueue(writeKey, 0x10000-writeKey, 
                                   AT_SHM_INIT_QUEUE_SIZE, 
                                   AT_SHM_INC_QUEUE_SIZE);
}


atSharedMemoryInterface::~atSharedMemoryInterface()
{
   // Delete the shared queues
   delete read_queue;
   delete write_queue;
}


int atSharedMemoryInterface::read(u_char * buffer, u_long length)
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


int atSharedMemoryInterface::write(u_char * buffer, u_long length)
{
   // Write the buffer to the queue
   write_queue->enqueue(buffer, length);

   // At least for now, we assume enqueuing to the shared buffer will work
   // everytime
   return length;
}

