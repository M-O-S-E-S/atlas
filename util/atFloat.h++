
#ifndef AT_FLOAT_H
#define AT_FLOAT_H


#include "atItem.h++"


class ATLAS_SYM atFloat : public atItem
{
   protected:
      float   float_value;

   public:
      atFloat(float val);
      virtual ~atFloat();

      virtual void    setValue(float val);
      virtual float   getValue();

      virtual bool   equals(atItem * otherItem);
      virtual int    compare(atItem * otherItem);
};


#endif

