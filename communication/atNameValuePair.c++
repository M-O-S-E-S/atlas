
#include <string.h>
#include "atNameValuePair.h++"


atNameValuePair::atNameValuePair(char * name, u_long nameLen, 
                                 char * value, u_long valueLen)
{
   // Store the info
   memcpy(pair_name, name, nameLen);
   memcpy(pair_value, value, valueLen);
   pair_name_len = nameLen;
   pair_value_len = valueLen;
}


atNameValuePair::~atNameValuePair()
{
}


char * atNameValuePair::getName()
{
   // Return the name
   return pair_name;
}


u_long atNameValuePair::getNameLen()
{
   // Return the name length
   return pair_name_len;
}


char * atNameValuePair::getValue()
{
   // Return the value
   return pair_value;
}


u_long atNameValuePair::getValueLen()
{
   // Return the value length
   return pair_value_len;
}


void atNameValuePair::setName(char * name, u_long nameLen)
{
   // Set a new name
   memcpy(pair_name, name, nameLen);
   pair_name_len = nameLen;
}


void atNameValuePair::setValue(char * value, u_long valueLen)
{
   // Set a new value
   memcpy(pair_value, value, valueLen);
   pair_value_len = valueLen;
}

