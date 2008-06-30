
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "atSharedQueue.h++"


atSharedQueue::atSharedQueue(ShmKey controlKey, ShmKey dataKey, 
                             u_long initialSize, u_long incrementSize)
{
   u_long        queueInfoSize;
   bool          createdControl;

   // Keep the keys to use later
   data_shm_key = dataKey;
   control_shm_key = controlKey;
   sem_key = controlKey;

   // Compute how big our control info is (so we can allocate space later)
   queueInfoSize = sizeof(*queue_head) + sizeof(*queue_tail) + 
                   sizeof(*queue_size) + sizeof(*queue_used) +
                   sizeof(*realloc_num) + sizeof(*connected_count);

   // Keep the increment size for later use as well
   memory_increment_size = incrementSize;

   // Get a semaphore to control access to this shared memory buffer
   createdControl = semGet(sem_key, &sem_id);
   if (sem_id == INVALID_SEM_ID)
   {
      // Having serious issues so tell user and bail
      notify(AT_FATAL_ERROR, 
             "Failed to get lock mechanism for shared queue.\n");
   }

   // Get shared memory for the control info
   shmGet(control_shm_key, queueInfoSize, &control_shm_id);
   if (control_shm_id == INVALID_SHM_ID)
   {
      // Failed big time
      notify(AT_FATAL_ERROR, "Failed to get memory for shared queue info.\n");
   }

   // Get shared memory for the data buffer
   shmGet(data_shm_key, initialSize, &data_shm_id);
   if (data_shm_id == INVALID_SHM_ID)
   {
      // Failed big time
      notify(AT_FATAL_ERROR, "Failed to get memory for shared queue.\n");
   }

   // Attach to shared memory for the control info
   shared_control_info = shmAttach(control_shm_id);
   if ((int ) shared_control_info == -1)
      notify(AT_FATAL_ERROR, "Failed to attach to memory for queue info.\n");

   // Attach to shared memory for the data buffer
   shared_buffer = shmAttach(data_shm_id);
   if ((int ) shared_buffer == -1)
      notify(AT_FATAL_ERROR, "Failed to attach to memory for shared queue.\n");

   // Initialize queue (lock it to make sure we don't initialize it twice)
   lock();
   queue_head = (u_long *) shared_control_info;
   queue_tail = (u_long *) queue_head + sizeof(*queue_head);
   queue_size = (u_long *) queue_tail + sizeof(*queue_tail);
   queue_used = (u_long *) queue_size + sizeof(*queue_size);
   realloc_num = (u_long *) queue_used + sizeof(*queue_used);
   connected_count = (u_long *) realloc_num + sizeof(*realloc_num);

   // If we created the control info, we need to initialize it
   if (createdControl == true)
   {
      *queue_head = 0;
      *queue_tail = 0;
      *queue_size = initialSize;
      *queue_used = 0;
      *realloc_num = 0;
      *connected_count = 1;
   }
   else
   {
      // We didn't create the memory so just increment the count
      // to tell everybody that we're here
      *connected_count = (*connected_count) + 1;
   }
   unlock();

   // Initialize our local queue reallocation flag
   last_realloc_num = 0;
}


atSharedQueue::~atSharedQueue()
{
   u_long           newConnectedCount;

   // Lock everything
   lock();

   // First, remove us from the count of people attached and keep a copy
   *connected_count = (*connected_count) - 1;
   newConnectedCount = *connected_count;

   // Detach from the shared memory
   if (shmDetach(shared_buffer) == -1)
      notify(AT_WARN, "Failed to detach from memory for shared queue.\n");

   // Detach from the shared memory control info
   if (shmDetach(shared_control_info) == -1)
      notify(AT_WARN, "Failed to detach from memory for shared queue info.\n");

   // If we are the last one using the semaphore, just remove it; otherwise,
   // just unlock it because we're done with it.  Similarly, if we were the
   // last one, then we need to remove the shared memory (if we do it too
   // early, people cannot find it by key anymore since the remove clears
   // the key in the shared memory table)
   if (newConnectedCount == 0)
   {
      // Remove shared memory
      shmRemove(data_shm_id);

      // Remove shared memory control info
      shmRemove(control_shm_id);

      // Remove locking semaphore
      semRemove(sem_id);
   }
   else
   {
      // Just unlock everything
      unlock();
   }
}


int atSharedQueue::lock()
{
   int   result;

   // Try to do the lock and return either success or failure (if we failed
   // because of a reason besides that we were interrupted (by a signal), then
   // notify that fact to the user as well)
   result = semLock(sem_id);
   
   // Process the result accordingly
   if (result == -1)
   {
      // This means an interrupt occurred
      notify(AT_ERROR, "Failed to lock access to shared queue.\n");

      // Return failure
      return 0;
   }
   else if (result == 0)
   {
      // Return failure
      return 0;
   }
   else
   {
      // Return success
      return 1;
   }
}


int atSharedQueue::unlock()
{
   int   result;

   // Try to do the unlock and return either success or failure (if we failed
   // because of a reason besides that we were interrupted (by a signal), then
   // notify that fact to the user as well)
   result = semUnlock(sem_id);
   
   // Process the result accordingly
   if (result == -1)
   {
      // This means that an interrupt occured
      notify(AT_ERROR, "Failed to unlock access to shared queue.\n");

      // Return failure
      return 0;
   }
   else if (result == 0)
   {
      // Return failure
      return 0;
   }
   else
   {
      // Return success
      return 1;
   }
}


u_long atSharedQueue::spaceAvailable()
{
   // Determine free space available in current queue
   return *queue_size - *queue_used;
}


void atSharedQueue::reallocateQueue(u_long minimumToAdd)
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
   tmpData = (u_char *) malloc(*queue_size);

   // Make sure we allocated memory properly
   if (tmpData == NULL)
      notify(AT_WARN, "Unable to allocate temporary memory in queue.\n");
   else
   {
      // Copy the current data to the temporary space
      if (*queue_tail > *queue_head)
      {
         // Tail is "ahead" of the head in memory (normal case)
         memcpy(tmpData, shared_buffer, *queue_size);
         headOffset = *queue_head;
         tailOffset = *queue_tail;
      }
      else
      {
         // Tail is "behind" of the head in memory (wrapped case)
         // Here, we re-work the queue to have the two segments together
         // again thereby keeping all free space (including the newly added
         // free space) at the end of the memory buffer.
         partialSize = *queue_size - *queue_head;
         memcpy(tmpData, &shared_buffer[*queue_head], partialSize);
         memcpy(&tmpData[partialSize], shared_buffer, 
                *queue_size - partialSize);
         headOffset = 0;
         tailOffset = *queue_used;
      }

      // Detach from the shared memory
      if (shmDetach(shared_buffer) == -1)
         notify(AT_WARN, "Failed to detach from memory for shared queue.\n");

      // Remove shared memory
      shmRemove(data_shm_id);

      // Get new shared memory buffer
      shmGet(data_shm_key, *queue_size + increment, &data_shm_id);
      if (data_shm_id == INVALID_SHM_ID)
         notify(AT_FATAL_ERROR, "Failed to get memory for shared queue.\n");

      // Attach to the new shared memory
      shared_buffer = shmAttach(data_shm_id);
      if ((int ) shared_buffer == -1)
      {
         notify(AT_FATAL_ERROR, 
                "Failed to attach to memory for shared queue.\n");
      }

      // Copy temporary buffer back into the shared memory buffer
      memcpy(shared_buffer, tmpData, *queue_size);

      // Free temp space
      free(tmpData);

      // Save the new (changed) queue control information
      *queue_size = *queue_size + increment;
      *queue_head = headOffset;
      *queue_tail = tailOffset;

      // Increment the "realloc" flag count to let others know we reallocated
      (*realloc_num)++;

      // Set our realloc tracker to this flag count
      last_realloc_num = *realloc_num;
   }
}


void atSharedQueue::checkForReallocatedQueue()
{
   if (last_realloc_num != *realloc_num)
   {
      // Somebody did reallocate the queue so let's get the new info
      notify(AT_INFO, "Somebody reallocated queue...getting new queue info.\n");

      // Detach from the shared memory
      if (shmDetach(shared_buffer) == -1)
         notify(AT_WARN, "Failed to detach from memory for shared queue.\n");

      // Get new shared memory buffer
      shmGet(data_shm_key, *queue_size, &data_shm_id);
      if (data_shm_id == INVALID_SHM_ID)
         notify(AT_FATAL_ERROR, "Failed to get memory for shared queue.\n");

      // Attach to the new shared memory
      shared_buffer = shmAttach(data_shm_id);
      if ((int ) shared_buffer == -1)
      {
         notify(AT_FATAL_ERROR, 
                "Failed to attach to memory for shared queue.\n");
      }

      // Set our realloc tracker to this flag count
      last_realloc_num = *realloc_num;
   }
}


void atSharedQueue::enqueue(u_char * buffer, u_long bufferLen)
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

   // Check to see if somebody else reallocated the queue
   checkForReallocatedQueue();

   // Check to make sure there is enough space in queue for new entry and
   // reallocate if necessary
   if (spaceAvailable() < bufferWithHeaderLen)
      reallocateQueue(bufferWithHeaderLen - spaceAvailable());

   // Add new entry to queue
   if (*queue_tail >= *queue_head)
   {
      // Tail is "ahead" of the head in memory (normal case)
      
      // Check to see if we can add the entry between the tail of the queue
      // and the end of the shared buffer space
      if (*queue_size - *queue_tail < bufferWithHeaderLen)
      {
         // Not all will fit so add in the piece that will and then wrap 
         // to the front of the shared buffer space and add the rest
         partialSize = *queue_size - *queue_tail;
         memcpy(&shared_buffer[*queue_tail], bufferWithHeader, partialSize);
         memcpy(shared_buffer, &bufferWithHeader[partialSize], 
                bufferWithHeaderLen - partialSize);
         *queue_tail = bufferWithHeaderLen-partialSize;
         *queue_used += bufferWithHeaderLen;

         // Wrap the tail if we're right at the bottom end
         *queue_tail %= *queue_size;
      }
      else
      {
         // All will fit so just add it all at once
         memcpy(&shared_buffer[*queue_tail], bufferWithHeader, 
                bufferWithHeaderLen);
         *queue_tail += bufferWithHeaderLen;
         *queue_used += bufferWithHeaderLen;

         // Wrap the tail if we're right at the bottom end
         *queue_tail %= *queue_size;
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
      memcpy(&shared_buffer[*queue_tail], bufferWithHeader, 
             bufferWithHeaderLen);
      *queue_tail += bufferWithHeaderLen;
      *queue_used += bufferWithHeaderLen;

      // Wrap the tail if we're right at the bottom end
      *queue_tail %= *queue_size;
   }

   // Unlock access to the memory
   unlock();

   // Get rid of temporary buffer
   free(bufferWithHeader);
}


bool atSharedQueue::dequeue(u_char * buffer, u_long * bufferLen)
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

   // Check to see if somebody else reallocated the queue
   checkForReallocatedQueue();

   // Check to see if we have anything to dequeue
   if (*queue_used > 0)
   {
      // Get the length of the next entry (note length includes the header).
      // However, we have to be careful in case the length itself is being
      // wrapped within the circular queue.
      if ( (*queue_size - *queue_head) >= sizeof(bufferWithHeaderLen) )
      {
         // The length is all in one piece
         memcpy(&bufferWithHeaderLen, &shared_buffer[*queue_head], 
                sizeof(bufferWithHeaderLen));
      }
      else
      {
         // The length itself is being wrapped
         tmpBufferPtr = (u_char *) &bufferWithHeaderLen;
         partialSize = *queue_size - *queue_head;

         // Copy the first part
         memcpy(tmpBufferPtr, &shared_buffer[*queue_head], partialSize);

         // Copy the second part
         memcpy(&tmpBufferPtr[partialSize], shared_buffer, 
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
         if ( (*queue_size - *queue_head) >= bufferWithHeaderLen)
         {
            // Entry is in one piece (normal case)
            memcpy(bufferWithHeader, &shared_buffer[*queue_head], 
                   bufferWithHeaderLen);
            *queue_head += bufferWithHeaderLen;
            *queue_used -= bufferWithHeaderLen;
 
            // Wrap the head if we're right at the bottom end
            *queue_head %= *queue_size;
         }
         else
         {
            // Entry is in two pieces (wrapped case)
            partialSize = *queue_size - *queue_head;
            memcpy(bufferWithHeader, &shared_buffer[*queue_head], partialSize);
            memcpy(&bufferWithHeader[partialSize], shared_buffer, 
                   bufferWithHeaderLen - partialSize);
            *queue_head = bufferWithHeaderLen - partialSize;
            *queue_used -= bufferWithHeaderLen;

            // Wrap the head if we're right at the bottom end
            *queue_head %= *queue_size;
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

