
#include "atFloat.h++"


atFloat::atFloat(float val)
{
   // Set the value
   float_value = val;
}


atFloat::~atFloat()
{
}


void atFloat::setValue(float val)
{
   // Set the value
   float_value = val;
}


float atFloat::getValue()
{
   // Return the value
   return float_value;
}


bool atFloat::equals(atItem * otherItem)
{
   atItem *   floatItem;
   
   // Try to cast the other item to an atFloat
   floatItem = dynamic_cast<atFloat *>(otherItem);

   // See if the other item is valid
   if (floatItem != NULL)
   {
      // Return the float comparison of the values
      return (AT_EQUAL(float_value, floatItem->getValue()));
   }
   else
   {
      // The types of items didn't match so it has to be different float
      // values
      return false;
   }
}


float atFloat::compare(atItem * otherItem)
{
   atItem *   floatItem;

   // Try to cast the other item to an atFloat
   floatItem = dynamic_cast<atFloat *>(otherItem);

   // See if the other item is valid
   if (floatItem != NULL)
   {
      // Return the difference between the float values
      return float_value - floatItem->getValue();
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

