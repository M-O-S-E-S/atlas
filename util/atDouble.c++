
#include "atDouble.h++"
#include "atGlobals.h++"


atDouble::atDouble(double val)
{
   // Set the value
   double_value = val;
}


atDouble::~atDouble()
{
}


void atDouble::setValue(double val)
{
   // Set the value
   double_value = val;
}


double atDouble::getValue()
{
   // Return the value
   return double_value;
}


bool atDouble::equals(atItem * otherItem)
{
   atDouble *   doubleItem;
   
   // Try to cast the other item to an atDouble
   doubleItem = dynamic_cast<atDouble *>(otherItem);

   // See if the other item is valid
   if (doubleItem != NULL)
   {
      // Return the double comparison of the values
      return (AT_EQUAL(double_value, doubleItem->getValue()));
   }
   else
   {
      // The types of items didn't match so it has to be different double
      // values
      return false;
   }
}


int atDouble::compare(atItem * otherItem)
{
   atDouble *   doubleItem;

   // Try to cast the other item to an atDouble
   doubleItem = dynamic_cast<atDouble *>(otherItem);

   // See if the other item is valid
   if (doubleItem != NULL)
   {
      // Due to the inaccurate nature of floating point numbers use a series 
      // of checks to determine the compare return value
      if (AT_EQUAL(double_value, doubleItem->getValue()))
      {
         // The two floating numbers are equal within a tolerance located 
         // inside of atGlobals so return 0
         return 0;
      }
      else if (double_value - doubleItem->getValue() < 0.0f)
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

