
#include "atFloat.h++"
#include "atGlobals.h++"


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
   atFloat *   floatItem;
   
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


int atFloat::compare(atItem * otherItem)
{
   atFloat *   floatItem;

   // Try to cast the other item to an atFloat
   floatItem = dynamic_cast<atFloat *>(otherItem);

   // See if the other item is valid
   if (floatItem != NULL)
   {
      // Due to the inaccurate nature of floating point numbers use a series 
      // of checks to determine the compare return value
      if (AT_EQUAL(float_value, floatItem->getValue()))
      {
         // The two floating numbers are equal within a tolerance located 
         // inside of atGlobals so return 0
         return 0;
      }
      else if (float_value - floatItem->getValue() < 0.0f)
      {
         // The current value is less than the other value so return -1 to 
         // indicate this
         return -1;
      }
      else
      {
         // The current value is greater than the other value so return 1 
         // to indicate this
         return 1;
      }
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

