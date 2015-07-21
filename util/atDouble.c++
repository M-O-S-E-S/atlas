
#include "atDouble.h++"


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

