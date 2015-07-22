
#ifndef AT_PAIR_H
#define AT_PAIR_H


#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atPair : public atItem
{
   protected:
      atItem *   first_item;
      atItem *   second_item;

   public:
      atPair(atItem * first, atItem * second);
      virtual ~atPair();

      atItem *   getFirst();
      atItem *   getSecond();

      void       setFirst(atItem * first);
      void       setSecond(atItem * second);

      void       removeFirst();
      void       removeSecond();

      virtual bool   equals(atItem * otherItem);
      virtual int    compare(atItem * otherItem);
};


#endif

