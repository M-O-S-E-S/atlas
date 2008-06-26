
#ifndef AT_PRIORITY_QUEUE_H
#define AT_PRIORITY_QUEUE_H


#include "atNotifier.h++"
#include "atItem.h++"
#include "atOSDefs.h"


#define AT_PRIORITY_QUEUE_DEFAULT_CAPACITY   15


typedef struct atPriorityQueueEntry
{
   atItem *   item;
};


class ATLAS_SYM atPriorityQueue : public atNotifier
{
   protected:
      atPriorityQueueEntry *   heap_array;

      u_long   max_capacity;
      u_long   num_entries;

      void   bubbleUp(int index);
      void   trickleDown(int index);

   public:
      atPriorityQueue();
      atPriorityQueue(u_long initialCapacity);
      virtual ~atPriorityQueue();

      virtual u_long   getNumEntries();

      virtual bool   addEntry(atItem * item);

      virtual atItem *   peekEntry();
      virtual atItem *   removeEntry();
      virtual void       removeAllEntries();
};


#endif

