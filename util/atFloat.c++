
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

