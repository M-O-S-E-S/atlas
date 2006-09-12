
#ifndef AT_STRING_H
#define AT_STRING_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atItem.h++"


class atString : public atItem
{
   protected:
      char *   local_string;
      int      string_length;

   public:
      atString();
      atString(char * stringToCopy);
      atString(atString * stringToCopy);
      virtual ~atString();

      virtual atString *  clone();

      virtual void        setString(char * stringToCopy);
      virtual void        setString(atString * stringToCopy);

      virtual char *      getString();
      virtual char        getCharAt(int index);
      virtual int         getLength();

      virtual bool        equals(atItem * otherItem);
};

#endif

