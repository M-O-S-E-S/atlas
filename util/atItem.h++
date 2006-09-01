
#ifndef AT_ITEM_H
#define AT_ITEM_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atNotifier.h++"


class atItem : public atNotifier
{
   public:
      atItem();
      virtual ~atItem();

      virtual bool   equals(atItem *otherItem);
};

#endif

