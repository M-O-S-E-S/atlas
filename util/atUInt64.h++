
#ifndef AT_UINT_64_H
#define AT_UINT_64_H


#include "atItem.h++"


class ATLAS_SYM atUInt64 : public atItem
{
   protected:
      uint64_t   int_value;

   public:
      atUInt64();
      atUInt64(uint64_t val);
      virtual   ~atUInt64();

      virtual void       setValue(uint64_t val);
      virtual uint64_t   getValue();

      virtual bool       equals(atItem * otherItem);
      virtual int        compare(atItem * otherItem);
};


#endif
