
#ifndef AT_LIST_H
#define AT_LIST_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"
#include "atItem.h++"


// TYPES
typedef struct Entry
{
   atItem *         item;
   struct Entry *   next;
   struct Entry *   previous;
} atListEntry;


class atList : public atNotifier
{
   protected:
      atListEntry *   list_head;
      atListEntry *   list_tail;
      
      u_long          num_entries;

      atListEntry *   current_entry;

   public:
      atList();
      virtual ~atList();

      virtual u_long     getNumEntries();

      virtual bool       addEntry(atItem * item);
      virtual bool       removeCurrentEntry();

      virtual atItem *   getFirstEntry();
      virtual atItem *   getNextEntry();
};

#endif

