
#ifndef AT_NAME_VALUE_PAIR_H
#define AT_NAME_VALUE_PAIR_H


// INCLUDES
#include "atItem.h++"


class atNameValuePair : public atItem
{
   protected:
      char     pair_name[255];
      u_long   pair_name_len;

      char     pair_value[1024];
      u_long   pair_value_len;

   public:
      atNameValuePair(char * name, u_long nameLen, 
                      char * value, u_long valueLen);
      virtual ~atNameValuePair();

      virtual char *   getName();
      virtual u_long   getNameLen();
      virtual char *   getValue();
      virtual u_long   getValueLen();

      virtual void     setName(char * name, u_long nameLen);
      virtual void     setValue(char * value, u_long valueLen);
};

#endif

