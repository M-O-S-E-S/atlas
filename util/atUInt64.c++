
#include "atUInt64.h++"


atUInt64::atUInt64()
{
   // Set the default value
   int_value = 0;
}


atUInt64::atUInt64(uint64_t val)
{
   // Set the value
   int_value = val;
}


atUInt64::~atUInt64()
{
}


void atUInt64::setValue(uint64_t val)
{
   // Set the value
   int_value = val;
}


uint64_t atUInt64::getValue()
{
   // Return the value
   return int_value;
}


bool atUInt64::equals(atItem * otherItem)
{
   atUInt64 *   intItem;

   // Try to cast the other item to an atUInt64
   intItem = dynamic_cast<atUInt64 *>(otherItem);

   // See if the other item is valid
   if (intItem != NULL)
   {
      // Return the comparision of the uint64_t values
      return int_value == intItem->getValue();
   }
   else
   {
      // The types of items didn't match so it has to be different integer
      // values
      return false;
   }
}


int atUInt64::compare(atItem * otherItem)
{
   atUInt64 *   intItem;

   // Try to cast the other item to an atUInt64
   intItem = dynamic_cast<atUInt64 *>(otherItem);

   // See if the other item is valid
   if (intItem != NULL)
   {
      // Return the difference between the uint64_t values
      return int_value - intItem->getValue();
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

