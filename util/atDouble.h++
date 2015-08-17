
#ifndef AT_DOUBLE_H
#define AT_DOUBLE_H


#include "atItem.h++"


class ATLAS_SYM atDouble : public atItem
{
   protected:
      double   double_value;

   public:
      atDouble(double val);
      virtual ~atDouble();

      virtual void     setValue(double val);
      virtual double   getValue();

      virtual bool   equals(atItem * otherItem);
      virtual int    compare(atItem * otherItem);
};


#endif

