
#ifndef AT_STRING_BUFFER_H
#define AT_STRING_BUFFER_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"
#include "atString.h++"


class ATLAS_SYM atStringBuffer : public atItem
{
   protected:
      char *   local_buffer;
      char *   buffer_tail;
      u_long   buffer_size;

   public:
      atStringBuffer();
      atStringBuffer(const char * stringToCopy);
      atStringBuffer(const char * stringToCopy, u_long maxLength);
      virtual ~atStringBuffer();

      virtual atStringBuffer     clone() const;

      virtual void               append(const atString & stringToAppend);
      virtual atStringBuffer     concat(const atString & stringToConcat) const;

      virtual void               append(const char * stringToAppend);
      virtual atStringBuffer     concat(const char * stringToConcat) const;

      virtual void               append(char charToAppend);
      virtual atStringBuffer     concat(char charToConcat) const;

      virtual void               setString(const char * stringToCopy);
      virtual void               setString(const char * stringToCopy, 
                                           u_long maxLength);
      virtual void               setString(const atString & stringToCopy);

      virtual char *             getString() const;
      virtual atString           getAsString() const;

      virtual char               getCharAt(u_long index) const;
      virtual u_long             getLength() const;

      virtual bool               equals(atItem * otherItem);
      virtual int                compare(atItem * otherItem);

      virtual atStringBuffer &   operator=(const atString & stringToCopy);
      virtual atStringBuffer &   operator=(
                                    const atStringBuffer & stringBufferToCopy);
};


#endif

