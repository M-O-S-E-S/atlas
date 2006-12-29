
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "atThreadQueue.h++"


atThreadQueue::atThreadQueue(u_long initialSize, u_long incrementSize)
{
   // Keep the increment size for later use
   memory_increment_size = incrementSize;

   // Get memory for the queue buffer
   queue_buffer = (u_char *) calloc(initialSize, sizeof(u_char));
   if (queue_buffer == NULL)
      notify(AT_FATAL_ERROR, "Failed to get memory for queue.\n");

   // Initialize queue (lock it to make sure we don't initialize it twice)
   lock();
   queue_head = 0;
   queue_tail = 0;
   queue_size = initialSize;
   queue_used = 0;
   unlock();
}


atThreadQueue::~atThreadQueue()
{
   // Free the queue
   if (queue_buffer != NULL)
      free(queue_buffer);
}


bool atThreadQueue::lock()
{
   return true;
}


bool atThreadQueue::unlock()
{
   return true;
}


u_long atThreadQueue::spaceAvailable()
{
   // Determine free space available in current queue
   return queue_size - queue_used;
}


void atThreadQueue::reallocateQueue(u_long minimumToAdd)
{
   u_long     increment;
   u_char *   tmpData;
   u_long     headOffset;
   u_long     tailOffset;
   u_long     partialSize;

   // Determine how many multiples of the increment size we need to satisfy
   // request
   increment = 0;
   while (increment < minimumToAdd)
      increment += memory_increment_size;
   notify(AT_INFO, "Reallocating shared queue by %d bytes.\n", increment);

   // Allocate some temporary, unshared memory to hold the data
   tmpData = (u_char *) malloc(queue_size);

   // Make sure we allocated memory properly
   if (tmpData == NULL)
      notify(AT_WARN, "Unable to allocate temporary memory in queue.\n");
   else
   {
      // Copy the current data to the temporary space
      if (queue_tail > queue_head)
      {
         // Tail is "ahead" of the head in memory (normal case)
         memcpy(tmpData, queue_buffer, queue_size);
         headOffset = queue_head;
         tailOffset = queue_tail;
      }
      else
      {
         // Tail is "behind" of the head in memory (wrapped case)
         // Here, we re-work the queue to have the two segments together
         // again thereby keeping all free space (including the newly added
         // free space) at the end of the memory buffer.
         partialSize = queue_size - queue_head;
         memcpy(tmpData, &queue_buffer[queue_head], partialSize);
         memcpy(&tmpData[partialSize], queue_buffer, 
                queue_size - partialSize);
         headOffset = 0;
         tailOffset = queue_used;
      }

      // Free the current queue buffer
      if (queue_buffer != NULL)
         free(queue_buffer);

      // Get a new queue buffer
      queue_buffer = (u_char *) calloc(queue_size + increment,
                                       sizeof(u_char));
      if (queue_buffer == NULL)
         notify(AT_FATAL_ERROR, "Failed to get memory for new queue.\n");

      // Copy temporary buffer back into the shared memory buffer
      memcpy(queue_buffer, tmpData, queue_size);

      // Free temp space
      free(tmpData);

      // Save the new (changed) queue control information
      queue_size = queue_size + increment;
      queue_head = headOffset;
      queue_tail = tailOffset;
   }
}


void atThreadQueue::enqueue(u_char * buffer, u_long bufferLen)
{
   u_long     bufferWithHeaderLen;
   u_char *   bufferWithHeader;
   u_long     partialSize;

   // Allocate space for a header and the buffer
   bufferWithHeaderLen = bufferLen + sizeof(u_long);
   bufferWithHeader = (u_char *) malloc(bufferWithHeaderLen);

   // Copy buffer contents into new buffer with header that holds the length 
   // (note that length includes the header).  We need this header in order 
   // to dequeue this buffer (we need to know how long it is).
   memcpy(&bufferWithHeader[0], &bufferWithHeaderLen, 
          sizeof(bufferWithHeaderLen));
   memcpy(&bufferWithHeader[sizeof(bufferWithHeaderLen)], buffer, bufferLen);

   // Lock access to the memory
   lock();

   // Check to make sure there is enough space in queue for new entry and
   // reallocate if necessary
   if (spaceAvailable() < bufferWithHeaderLen)
      reallocateQueue(bufferWithHeaderLen - spaceAvailable());

   // Add new entry to queue
   if (queue_tail >= queue_head)
   {
      // Tail is "ahead" of the head in memory (normal case)
      
      // Check to see if we can add the entry between the tail of the queue
      // and the end of the shared buffer space
      if (queue_size - queue_tail < bufferWithHeaderLen)
      {
         // Not all will fit so add in the piece that will and then wrap 
         // to the front of the shared buffer space and add the rest
         partialSize = queue_size - queue_tail;
         memcpy(&queue_buffer[queue_tail], bufferWithHeader, partialSize);
         memcpy(queue_buffer, &bufferWithHeader[partialSize], 
                bufferWithHeaderLen - partialSize);
         queue_tail = bufferWithHeaderLen-partialSize;
         queue_used += bufferWithHeaderLen;

         // Wrap the tail if we're right at the bottom end
         queue_tail %= queue_size;
      }
      else
      {
         // All will fit so just add it all at once
         memcpy(&queue_buffer[queue_tail], bufferWithHeader, 
                bufferWithHeaderLen);
         queue_tail += bufferWithHeaderLen;
         queue_used += bufferWithHeaderLen;

         // Wrap the tail if we're right at the bottom end
         queue_tail %= queue_size;
      }
   }
   else
   {
      // Tail is "behind" of the head in memory (wrapped case)

      // In this case, we know that the new entry will fit in the empty
      // space between the tail and the head (the space is of size
      // queue_head-queue_tail) since we would have reallocated the queue to 
      // make room for it if there wasn't space (and all of the free space
      // would then be at the end and we wouldn't be in this case in the first
      // place)
      memcpy(&queue_buffer[queue_tail], bufferWithHeader, 
             bufferWithHeaderLen);
      queue_tail += bufferWithHeaderLen;
      queue_used += bufferWithHeaderLen;

      // Wrap the tail if we're right at the bottom end
      queue_tail %= queue_size;
   }

   // Unlock access to the memory
   unlock();

   // Get rid of temporary buffer
   free(bufferWithHeader);
}


bool atThreadQueue::dequeue(u_char * buffer, u_long * bufferLen)
{
   u_long     bufferWithHeaderLen;
   u_char *   bufferWithHeader;
   u_char *   tmpBufferPtr;
   u_long     partialSize;
   int        returnCode;

   // Initialize
   bufferWithHeader = NULL;

   // Lock access to the memory
   lock();

   // Check to see if we have anything to dequeue
   if (queue_used > 0)
   {
      // Get the length of the next entry (note length includes the header).
      // However, we have to be careful in case the length itself is being
      // wrapped within the circular queue.
      if ( (queue_size - queue_head) >= sizeof(bufferWithHeaderLen) )
      {
         // The length is all in one piece
         memcpy(&bufferWithHeaderLen, &queue_buffer[queue_head], 
                sizeof(bufferWithHeaderLen));
      }
      else
      {
         // The length itself is being wrapped
         tmpBufferPtr = (u_char *) &bufferWithHeaderLen;
         partialSize = queue_size - queue_head;

         // Copy the first part
         memcpy(tmpBufferPtr, &queue_buffer[queue_head], partialSize);

         // Copy the second part
         memcpy(&tmpBufferPtr[partialSize], queue_buffer, 
                sizeof(bufferWithHeaderLen) - partialSize);
      }

      // Make sure the user's buffer can hold this entry
      if (bufferWithHeaderLen - sizeof(bufferWithHeaderLen) > *bufferLen)
      {
         // There isn't enough room in the user's buffer to hold the queue
         // entry so notify and fail
         notify(AT_WARN, 
                "Queue entry is larger than buffer.  Not dequeueing.\n");
         notify(AT_WARN, "Queue entry size was %d, buffer size was %d.\n",
                bufferWithHeaderLen - sizeof(bufferWithHeaderLen), *bufferLen);
         returnCode = false;
      }
      else
      {
         // Allocate space to hold the entry
         bufferWithHeader = (u_char *) malloc(bufferWithHeaderLen);

         // Get the actual buffer contents including the header
         if ( (queue_size - queue_head) >= bufferWithHeaderLen)
         {
            // Entry is in one piece (normal case)
            memcpy(bufferWithHeader, &queue_buffer[queue_head], 
                   bufferWithHeaderLen);
            queue_head += bufferWithHeaderLen;
            queue_used -= bufferWithHeaderLen;
 
            // Wrap the head if we're right at the bottom end
            queue_head %= queue_size;
         }
         else
         {
            // Entry is in two pieces (wrapped case)
            partialSize = queue_size - queue_head;
            memcpy(bufferWithHeader, &queue_buffer[queue_head], partialSize);
            memcpy(&bufferWithHeader[partialSize], queue_buffer, 
                   bufferWithHeaderLen - partialSize);
            queue_head = bufferWithHeaderLen - partialSize;
            queue_used -= bufferWithHeaderLen;

            // Wrap the head if we're right at the bottom end
            queue_head %= queue_size;
         }

         // We were successful
         returnCode = true;
      }
   }
   else
   {
      // There was nothing to dequeue
      returnCode = false;
   }

   // Unlock access to the memory
   unlock();

   // If we dequeued something, copy it into the user's buffer (minus the
   // internal header)
   if (returnCode != false)
   {
      *bufferLen = bufferWithHeaderLen - sizeof(bufferWithHeaderLen);
      memcpy(buffer, &bufferWithHeader[sizeof(bufferWithHeaderLen)],
             *bufferLen);
   }
   else
      *bufferLen = 0;

   // Get rid of temporary buffer
   if (bufferWithHeader != NULL)
      free(bufferWithHeader);

   // Return the error code to the user (true for success, false for failure)
   return returnCode;
}

