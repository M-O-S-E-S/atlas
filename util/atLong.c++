
#include "atLong.h++"


atLong::atLong(long val)
{
   // Set the value
   long_value = val;
}


atLong::~atLong()
{
}


void atLong::setValue(long val)
{
   // Set the value
   long_value = val;
}


long atLong::getValue()
{
   // Return the value
   return long_value;
}


bool atLong::equals(atItem * otherItem)
{
   atItem *   longItem;
   
   // Try to cast the other item to an atLong
   longItem = dynamic_cast<atLong *>(otherItem);

   // See if the other item is valid
   if (longItem != NULL)
   {
      // Return the long comparison of the values
      return long_value == longItem->getValue();
   }
   else
   {
      // The types of items didn't match so it has to be different long
      // values
      return false;
   }
}


long atLong::compare(atItem * otherItem)
{
   atItem *   longItem;

   // Try to cast the other item to an atLong
   longItem = dynamic_cast<atLong *>(otherItem);

   // See if the other item is valid
   if (longItem != NULL)
   {
      // Return the difference between the long values
      return long_value - longItem->getValue();
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

