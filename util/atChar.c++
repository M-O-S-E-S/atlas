
#include "atChar.h++"


atChar::atChar(char val)
{
   // Set the value
   char_value = val;
}


atChar::~atChar()
{
}


void atChar::setValue(char val)
{
   // Set the value
   char_value = val;
}


char atChar::getValue()
{
   // Return the value
   return char_value;
}

