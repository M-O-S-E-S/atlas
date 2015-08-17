
#include "atChar.h++"


atChar::atChar(char val)
{
   // Set the value
   char_value = val;
}


atChar::~atChar()
{
}


void atChar::setValue(char val)
{
   // Set the value
   char_value = val;
}


char atChar::getValue()
{
   // Return the value
   return char_value;
}


bool atChar::equals(atItem * otherItem)
{
   atChar *   charItem;
   
   // Try to cast the other item to an atChar
   charItem = dynamic_cast<atChar *>(otherItem);

   // See if the other item is valid
   if (charItem != NULL)
   {
      // Return the char comparison of the values
      return char_value == charItem->getValue();
   }
   else
   {
      // The types of items didn't match so it has to be different char
      // values
      return false;
   }
}


int atChar::compare(atItem * otherItem)
{
   atChar *   charItem;

   // Try to cast the other item to an atChar
   charItem = dynamic_cast<atChar *>(otherItem);

   // See if the other item is valid
   if (charItem != NULL)
   {
      // Return the difference between the char values
      return char_value - charItem->getValue();
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

