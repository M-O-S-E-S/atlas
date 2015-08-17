
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

