
#ifndef AT_PRIORITY_QUEUE_H
#define AT_PRIORITY_QUEUE_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"
#include "atItem.h++"


// TYPES
typedef struct atPriorityQueueEntry
{
   atItem *                 item;
   atPriorityQueueEntry *   next;
   atPriorityQueueEntry *   previous;
};


class atPriorityQueue : public atNotifier
{
   protected:
      atPriorityQueueEntry *   queue_head;
      atPriorityQueueEntry *   queue_tail;

      u_long          num_entries;

   public:
      atPriorityQueue();
      virtual ~atPriorityQueue();

      virtual u_long   getNumEntries();

      virtual bool   insertEntry(atItem * item);

      virtual atItem *   peekEntry();
      virtual atItem *   removeEntry();
      virtual void       removeAllEntries();
};


#endif

