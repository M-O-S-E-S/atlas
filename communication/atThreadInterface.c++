
#include "atThreadInterface.h++"
#include "atMap.h++"
#include "atString.h++"
#include "atPair.h++"


// CONSTANTS
#define AT_THR_INIT_QUEUE_SIZE   5000 
#define AT_THR_INC_QUEUE_SIZE   10000

#define AT_THR_SEM_KEY   3099


// GLOBALS
SemID          atlas_thread_interface_sem_id;
static atMap   atlas_thread_interface_key_map;


atThreadInterface::atThreadInterface(SemKey readKey, SemKey writeKey)
{
   char         tmp[256];
   atString *   readKeyStr;
   atString *   writeKeyStr;
   atPair *     pair;

   // Get the semaphore that controls access to the key map (maps keys
   // to instances of atThreadQueue)
   semGet(AT_THR_SEM_KEY, &atlas_thread_interface_sem_id);

   // Store keys for later use
   read_key = readKey;
   write_key = writeKey;

   // Now, lock access and then see if the thread queues exist already
   semLock(atlas_thread_interface_sem_id);

   // Look for the queue for the read key
   sprintf(tmp, "%d", readKey);
   readKeyStr = new atString(tmp);
   pair = 
      (atPair *) atlas_thread_interface_key_map.getValue(readKeyStr);
   if (pair == NULL)
   {
      // Create a read queue for communication messages
      read_queue = new atThreadQueue(readKey,
                                     AT_THR_INIT_QUEUE_SIZE, 
                                     AT_THR_INC_QUEUE_SIZE);
      read_count = new atThreadCount();
      read_count->inc();

      // Add it to the map
      pair = new atPair(read_queue, read_count);
      atlas_thread_interface_key_map.addEntry(readKeyStr, pair);
   }
   else
   {
      // Keep a pointer to the queue and count
      read_queue = (atThreadQueue *) pair->getFirst();
      read_count = (atThreadCount *) pair->getSecond();

      // Increment count as we're using the queue now
      read_count->inc();

      // Don't need this key string anymore
      delete readKeyStr;
   }

   // Look for the queue for the write key
   sprintf(tmp, "%d", writeKey);
   writeKeyStr = new atString(tmp);
   pair = 
      (atPair *) atlas_thread_interface_key_map.getValue(writeKeyStr);
   if (pair == NULL)
   {
      // Create a write queue for communication messages
      write_queue = new atThreadQueue(writeKey,
                                      AT_THR_INIT_QUEUE_SIZE, 
                                      AT_THR_INC_QUEUE_SIZE);
      write_count = new atThreadCount();
      write_count->inc();

      // Add it to the map
      pair = new atPair(write_queue, write_count);
      atlas_thread_interface_key_map.addEntry(writeKeyStr, pair);
   }
   else
   {
      // Keep a pointer to the queue and count
      write_queue = (atThreadQueue *) pair->getFirst();
      write_count = (atThreadCount *) pair->getSecond();

      // Increment count as we're using the queue now
      write_count->inc();

      // Don't need this key string anymore
      delete writeKeyStr;
   }

   // Finally, unlock access
   semUnlock(atlas_thread_interface_sem_id);
}


atThreadInterface::~atThreadInterface()
{
   char         tmp[256];
   atString *   keyStr;

   // Lock access
   semLock(atlas_thread_interface_sem_id);

   // Decrement the count for the read queue and write queue
   read_count->dec();
   write_count->dec();

   // Now, check read queue count (if it goes to zero, nobody is using it)
   if (read_count->getCount() == 0)
   {
      // Remove the entry from the map (this will also clean up the memory)
      sprintf(tmp, "%d", read_key);
      keyStr = new atString(tmp);
      atlas_thread_interface_key_map.deleteEntry(keyStr);

      // Clean-up
      delete keyStr;
   }

   // Now, check write queue count (if it goes to zero, nobody is using it)
   if (write_count->getCount() == 0)
   {
      // Remove the entry from the map (this will also clean up the memory)
      sprintf(tmp, "%d", write_key);
      keyStr = new atString(tmp);
      atlas_thread_interface_key_map.deleteEntry(keyStr);

      // Clean-up
      delete keyStr;
   }

   // Unlock
   semUnlock(atlas_thread_interface_sem_id);
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

