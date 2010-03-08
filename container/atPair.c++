
#include "atPair.h++"


atPair::atPair(atItem * first, atItem * second)
{
   // Store the pair of items
   first_item = first;
   second_item = second;
}


atPair::~atPair()
{
   // Delete the items we are holding
   if (first_item != NULL)
      delete first_item;
   if (second_item != NULL)
      delete second_item;
}


atItem * atPair::getFirst()
{
   // Return the first item
   return first_item;
}


atItem * atPair::getSecond()
{
   // Return the second item
   return second_item;
}

