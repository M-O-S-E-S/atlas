
#ifndef AT_SHARED_QUEUE_H
#define AT_SHARED_QUEUE_H


// INCLUDES
#include <sys/types.h>
#include "atNotifier.h++"


class atSharedQueue : public atNotifier
{
   protected:
      key_t      sem_key;
      int        sem_id;
      key_t      control_shm_key;
      int        control_shm_id;
      key_t      data_shm_key;
      int        data_shm_id;

      u_long     memory_increment_size;

      u_char *   shared_control_info;
      u_char *   shared_buffer;

      u_long *   queue_head;
      u_long *   queue_tail;
      u_long *   queue_size;
      u_long *   queue_used;
      u_long *   realloc_num;

      u_long     last_realloc_num;

      int      lock();
      int      unlock();
      u_long   spaceAvailable();
      void     reallocateQueue(u_long minimumToAdd);
      void     checkForReallocatedQueue();


   public:
      atSharedQueue(key_t controlKey, key_t dataKey, u_long initialSize, 
                    u_long incrementSize);
      virtual ~atSharedQueue();

      virtual void   enqueue(u_char * buffer, u_long bufferLen);
      virtual bool   dequeue(u_char * buffer, u_long * bufferLen);
};


#endif

