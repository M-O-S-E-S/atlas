
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include "atSharedQueue.h++"


// CONSTANTS
#define AT_SHQ_LOCK_SEMAPHORE_NUM   0

#define AT_SHQ_NUM_LOCK_SEQUENCE_OPS     2
#define AT_SHQ_NUM_UNLOCK_SEQUENCE_OPS   1


// TYPES
union semunion
{
   int               val;
   struct semid_ds   *buf;
   ushort            *array;
};

// In certain cases we need to define the "union semun" (see comments)
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// union semun is defined by including <sys/sem.h> 
#else
// according to X/OPEN we have to define it ourselves
union semun 
{
   int val;                  /* value for SETVAL */
   struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
   unsigned short *array;    /* array for GETALL, SETALL */
                             /* Linux specific part: */
   struct seminfo *__buf;    /* buffer for IPC_INFO */
};
#endif


// GLOBAL VARIABLES
struct sembuf atShqLockSequence[AT_SHQ_NUM_LOCK_SEQUENCE_OPS] =
{
   AT_SHQ_LOCK_SEMAPHORE_NUM, 0, 0,
   AT_SHQ_LOCK_SEMAPHORE_NUM, 1, 0
};

struct sembuf atShqUnlockSequence[AT_SHQ_NUM_UNLOCK_SEQUENCE_OPS] =
{
   AT_SHQ_LOCK_SEMAPHORE_NUM, -1, 0
};


atSharedQueue::atSharedQueue(key_t controlKey, key_t dataKey, 
                             u_long initialSize, u_long incrementSize)
{
   u_long        queueInfoSize;
   union semun   zero;

   // Keep the keys to use later
   data_shm_key = dataKey;
   control_shm_key = controlKey;
   sem_key = controlKey;

   // Compute how big our control info is (so we can allocate space later)
   queueInfoSize = sizeof(*queue_head) + sizeof(*queue_tail) + 
                   sizeof(*queue_size) + sizeof(*queue_used) +
                   sizeof(*realloc_num);

   // Keep the increment size for later use as well
   memory_increment_size = incrementSize;

   // Get a semaphore to control access to this shared memory buffer
   sem_id = semget(sem_key, 1, 0666 | IPC_CREAT);
   if (sem_id == -1)
   {
      notify(AT_FATAL_ERROR, 
             "Failed to get lock mechanism for shared queue.\n");
   }

   // Initialize the semaphore value
   zero.val = 0;
   semctl(sem_id, AT_SHQ_LOCK_SEMAPHORE_NUM, SETVAL, zero);

   // Get shared memory for the control info
   control_shm_id = shmget(control_shm_key, queueInfoSize, 0666 | IPC_CREAT);
   if (control_shm_id == -1)
      notify(AT_FATAL_ERROR, "Failed to get memory for shared queue info.\n");

   // Get shared memory for the data buffer
   data_shm_id = shmget(data_shm_key, initialSize, 0666 | IPC_CREAT);
   if (data_shm_id == -1)
      notify(AT_FATAL_ERROR, "Failed to get memory for shared queue.\n");

   // Attach to shared memory for the control info
   shared_control_info = (u_char *) shmat(control_shm_id, NULL, 0);
   if ((int ) shared_control_info == -1)
      notify(AT_FATAL_ERROR, "Failed to attach to memory for queue info.\n");

   // Attach to shared memory for the data buffer
   shared_buffer = (u_char *) shmat(data_shm_id, NULL, 0);
   if ((int ) shared_buffer == -1)
      notify(AT_FATAL_ERROR, "Failed to attach to memory for shared queue.\n");

   // Initialize queue (lock it to make sure we don't initialize it twice)
   lock();
   queue_head = (u_long *) &shared_control_info[0];
   queue_tail = (u_long *) queue_head + sizeof(*queue_head);
   queue_size = (u_long *) queue_tail + sizeof(*queue_tail);
   queue_used = (u_long *) queue_size + sizeof(*queue_size);
   realloc_num = (u_long *) queue_used + sizeof(*queue_used);

   *queue_head = 0;
   *queue_tail = 0;
   *queue_size = initialSize;
   *queue_used = 0;
   *realloc_num = 0;
   unlock();

   // Initialize our local queue reallocation flag
   last_realloc_num = 0;
}


atSharedQueue::~atSharedQueue()
{
   union semunion   zero;

   // Lock everything
   lock();

   // Detach from the shared memory
   if (shmdt(shared_buffer) == -1)
      notify(AT_WARN, "Failed to detach from memory for shared queue.\n");

   // Remove shared memory
   if (shmctl(data_shm_id, IPC_RMID, NULL) == -1)
      notify(AT_WARN, "Failed to remove shared memory for shared queue.\n");

   // Detach from the shared memory control info
   if (shmdt(shared_control_info) == -1)
      notify(AT_WARN, "Failed to detach from memory for shared queue info.\n");

   // Remove shared memory control info
   if (shmctl(control_shm_id, IPC_RMID, NULL) == -1)
   {
      notify(AT_WARN, 
             "Failed to remove shared memory for shared queue info.\n");
   }

   // Unlock everything
   unlock();

   // Remove locking semaphore
   zero.val = 0;
   semctl(sem_id, 1, IPC_RMID, zero);
}


int atSharedQueue::lock()
{
   // Try to do the lock and return either success or failure.  If we failed
   // because of a reason besides that we were interrupted (by a signal), then
   // notify that fact to the user as well.
   if (semop(sem_id, &atShqLockSequence[0], AT_SHQ_NUM_LOCK_SEQUENCE_OPS) == -1)
   {
      if (errno != EINTR)
         notify(AT_ERROR, "Failed to lock access to shared queue.\n");

      return 0;
   }
   else
      return 1;
}


int atSharedQueue::unlock()
{
   // Try to do the unlock and return either success or failure.  If we failed
   // because of a reason besides that we were interrupted (by a signal), then
   // notify that fact to the user as well.
   if (semop(sem_id, &atShqUnlockSequence[0], 
             AT_SHQ_NUM_UNLOCK_SEQUENCE_OPS) == -1)
   {
      if (errno != EINTR)
         notify(AT_ERROR, "Failed to unlock access to shared queue.\n");

      return 0;
   }
   else
      return 1;
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
      if (shmdt(shared_buffer) == -1)
         notify(AT_WARN, "Failed to detach from memory for shared queue.\n");

      // Remove shared memory
      if (shmctl(data_shm_id, IPC_RMID, NULL) == -1)
         notify(AT_WARN, "Failed to remove shared memory for shared queue.\n");

      // Get new shared memory buffer
      data_shm_id = shmget(data_shm_key, *queue_size + increment, 
                           0666 | IPC_CREAT);
      if (data_shm_id == -1)
         notify(AT_FATAL_ERROR, "Failed to get memory for shared queue.\n");

      // Attach to the new shared memory
      shared_buffer = (u_char *) shmat(data_shm_id, NULL, 0);
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
   }
}


void atSharedQueue::checkForReallocatedQueue()
{
   if (last_realloc_num != *realloc_num)
   {
      // Somebody did reallocate the queue so let's get the new info
      notify(AT_INFO, "Somebody reallocated queue...getting new queue info.\n");

      // Detach from the shared memory
      if (shmdt(shared_buffer) == -1)
         notify(AT_WARN, "Failed to detach from memory for shared queue.\n");

      // Get new shared memory buffer
      data_shm_id = shmget(data_shm_key, *queue_size, 0666 | IPC_CREAT);
      if (data_shm_id == -1)
         notify(AT_FATAL_ERROR, "Failed to get memory for shared queue.\n");

      // Attach to the new shared memory
      shared_buffer = (u_char *) shmat(data_shm_id, NULL, 0);
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

