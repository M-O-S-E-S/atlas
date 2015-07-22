
#ifndef AT_TRIPLE_H
#define AT_TRIPLE_H


#include "atItem.h++"


class ATLAS_SYM atTriple : public atItem
{
   protected:
      atItem *   first_item;
      atItem *   second_item;
      atItem *   third_item;

   public:
      atTriple(atItem * first, atItem * second, atItem * third);
      virtual ~atTriple();

      atItem *   getFirst();
      atItem *   getSecond();
      atItem *   getThird();

      void       setFirst(atItem * first);
      void       setSecond(atItem * second);
      void       setThird(atItem * third);

      void       removeFirst();
      void       removeSecond();
      void       removeThird();

      virtual bool   equals(atItem * otherItem);
      virtual int    compare(atItem * otherItem);
};


#endif

