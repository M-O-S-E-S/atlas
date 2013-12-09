
#ifndef AT_STRING_H
#define AT_STRING_H


// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atString : public atItem
{
   protected:
      char *   local_string;
      u_long   string_length;

   public:
      atString();
      atString(const char * stringToCopy);
      atString(const char * stringToCopy, u_long maxLength);
      atString(const atString & stringToCopy);
      virtual ~atString();

      virtual atString       clone() const;

      virtual void           append(const atString & stringToAppend);
      virtual atString       concat(const atString & stringToConcat) const;

      virtual void           setString(const char * stringToCopy);
      virtual void           setString(const char * stringToCopy, 
                                       u_long maxLength);
      virtual void           setString(const atString & stringToCopy);

      virtual char *         getString() const;

      virtual char           getCharAt(u_long index) const;
      virtual u_long         getLength() const;

      virtual void           replaceAll(char * stringToReplace, 
                                        char * replacementString);

      virtual atString       subString(int start);
      virtual atString       subString(int start, int end);

      virtual bool           equals(atItem * otherItem);
      virtual int            compare(atItem * otherItem);
      
      virtual atString &     operator=(const atString & stringToCopy);
};


#endif

