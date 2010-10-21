
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


void atPair::removeFirst()
{
   // Clear the first item
   first_item = NULL;
}


void atPair::removeSecond()
{
   // Clear the second item
   second_item = NULL;
}


bool atPair::equals(atItem * otherItem)
{
   atPair *   pairItem;
   bool       firsts;
   bool       seconds;

   // Try to convert it to a pair to make sure it is a pair
   pairItem = dynamic_cast<atPair *>(otherItem);

   // Return whether the two strings are equal or not
   if ( (pairItem != NULL) 
   {
      // Compare the "first" items and the "second" items and return 
      // if both sets match
      firsts = ((first_item == NULL) && (pairItem->first_item == NULL)) ||
               (first_item->equals(pairItem->first_item) == true);
      seconds = ((second_item == NULL) && (pairItem->second_item == NULL)) ||
                (second_item->equals(pairItem->second_item) == true);
      return (firsts && seconds);
   }
   else
      return false;
}


int atPair::compare(atItem * otherItem)
{
   atPair *   pairItem;
   int        com;

   // Try to cast to an atPair
   pairItem = dynamic_cast<atPair *>(otherItem);

   // See if the other item is valid
   if (pairItem != NULL)
   {
      // Check to see how the first items compare
      com = first_item->compare(pairItem->getFirst());
      if (com == 0)
      {
         // The first items are equal so let's check the second ones now
         com = second_item->compare(pairItem->getSecond());
      }

      // Return our result
      return com;
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

