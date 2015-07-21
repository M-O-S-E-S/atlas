
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

