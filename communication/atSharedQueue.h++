
#ifndef AT_SHARED_QUEUE_H
#define AT_SHARED_QUEUE_H


// INCLUDES
#include <sys/types.h>
#include "atNotifier.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atSharedQueue : public atNotifier
{
   protected:
      SemKey     sem_key;
      SemID      sem_id;
      ShmKey     control_shm_key;
      ShmID      control_shm_id;
      ShmKey     data_shm_key;
      ShmID      data_shm_id;

      u_long     memory_increment_size;

      u_char *   shared_control_info;
      u_char *   shared_buffer;

      u_long *   queue_head;
      u_long *   queue_tail;
      u_long *   queue_size;
      u_long *   queue_used;
      u_long *   realloc_num;
      u_long *   connected_count;

      u_long     last_realloc_num;

      int      lock();
      int      unlock();
      u_long   spaceAvailable();
      void     reallocateQueue(u_long minimumToAdd);
      void     checkForReallocatedQueue();


   public:
      atSharedQueue(ShmKey controlKey, ShmKey dataKey, u_long initialSize, 
                    u_long incrementSize);
      virtual ~atSharedQueue();

      virtual void   enqueue(u_char * buffer, u_long bufferLen);
      virtual bool   dequeue(u_char * buffer, u_long * bufferLen);
};


#endif

