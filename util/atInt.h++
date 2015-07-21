
#ifndef AT_INT_H
#define AT_INT_H


#include "atItem.h++"


class ATLAS_SYM atInt : public atItem
{
   protected:
      int   int_value;

   public:
      atInt(int val);
      virtual ~atInt();

      virtual void   setValue(int val);
      virtual int    getValue();
};


#endif

