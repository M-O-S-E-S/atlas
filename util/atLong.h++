
#ifndef AT_LONG_H
#define AT_LONG_H


#include "atItem.h++"


class ATLAS_SYM atLong : public atItem
{
   protected:
      long   long_value;

   public:
      atLong(long val);
      virtual ~atLong();

      virtual void   setValue(long val);
      virtual long   getValue();
};


#endif

