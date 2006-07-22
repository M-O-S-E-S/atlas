
#ifndef AT_LIST_H
#define AT_LIST_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"
#include "atItem.h++"


// TYPES
typedef struct atEntry
{
   atItem *           item;
   struct atEntry *   next;
   struct atEntry *   previous;
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
      virtual bool       removeAllEntries();

      virtual atItem *   getFirstEntry();
      virtual atItem *   getNextEntry();
      virtual atItem *   getPreviousEntry();
      virtual atItem *   getNthEntry(u_long n);

      virtual atItem *   findEntry(atItem * item);
};

#endif

