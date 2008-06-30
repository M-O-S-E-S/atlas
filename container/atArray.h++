
#ifndef AT_ARRAY_H
#define AT_ARRAY_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"
#include "atItem.h++"
#include "atOSDefs.h"


class ATLAS_SYM atArray : public atNotifier
{
   protected:
      atItem **       array_items;
      
      u_long          num_entries;
      u_long          current_capacity;

      virtual bool    ensureCapacity(u_long capacity);

   public:
      atArray();
      virtual ~atArray();

      virtual u_long     getNumEntries();

      virtual bool       addEntry(atItem * item);
      virtual atItem *   setEntry(long index, atItem * item);
      virtual bool       insertEntry(long index, atItem * item);
      virtual bool       removeEntry(atItem * item);
      virtual bool       removeEntryAtIndex(long index);
      virtual bool       removeAllEntries();

      virtual atItem *   getEntry(long index);
      virtual long       getIndexOf(atItem * item);
};

#endif

