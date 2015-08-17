
#include "atInt.h++"


atInt::atInt(int val)
{
   // Set the value
   int_value = val;
}


atInt::~atInt()
{
}


void atInt::setValue(int val)
{
   // Set the value
   int_value = val;
}


int atInt::getValue()
{
   // Return the value
   return int_value;
}


bool atInt::equals(atItem * otherItem)
{
   atInt *   intItem;
   
   // Try to cast the other item to an atInt
   intItem = dynamic_cast<atInt *>(otherItem);

   // See if the other item is valid
   if (intItem != NULL)
   {
      // Return the int comparison of the values
      return int_value == intItem->getValue();
   }
   else
   {
      // The types of items didn't match so it has to be different integer
      // values
      return false;
   }
}


int atInt::compare(atItem * otherItem)
{
   atInt *   intItem;

   // Try to cast the other item to an atInt
   intItem = dynamic_cast<atInt *>(otherItem);

   // See if the other item is valid
   if (intItem != NULL)
   {
      // Return the difference between the integer values
      return int_value - intItem->getValue();
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

