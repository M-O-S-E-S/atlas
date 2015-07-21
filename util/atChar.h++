
#ifndef AT_CHAR_H
#define AT_CHAR_H


#include "atItem.h++"


class ATLAS_SYM atChar : public atItem
{
   protected:
      int   char_value;

   public:
      atChar(char val);
      virtual ~atChar();

      virtual void   setValue(char val);
      virtual char   getValue();
};


#endif

