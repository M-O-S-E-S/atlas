
#ifndef AT_STRING_BUFFER_H
#define AT_STRING_BUFFER_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atItem.h++"
#include "atString.h++"


class atStringBuffer : public atItem
{
   protected:
      char *   local_buffer;
      int      buffer_size;

   public:
      atStringBuffer();
      atStringBuffer(char * stringToCopy);
      atStringBuffer(char * stringToCopy, u_long maxLength);
      virtual ~atStringBuffer();

      virtual atStringBuffer   clone();

      virtual void             append(const atString & stringToAppend);
      virtual atStringBuffer   concat(const atString & stringToConcat);

      virtual void             append(char * stringToAppend);
      virtual atStringBuffer   concat(char * stringToConcat);

      virtual void             setString(char * stringToCopy);
      virtual void             setString(char * stringToCopy, 
                                         u_long maxLength);
      virtual void             setString(atString & stringToCopy);

      virtual char *           getString();
      virtual atString         getAsString();

      virtual char             getCharAt(int index);
      virtual int              getLength();

      virtual bool             equals(atItem * otherItem);
      virtual int              compare(atItem * otherItem);

      virtual void             operator=(atString stringToCopy);
};


#endif

