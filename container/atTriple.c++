
#include "atTriple.h++"


atTriple::atTriple(atItem * first, atItem * second, atItem * third)
{
   // Store the triplet of items
   first_item = first;
   second_item = second;
   third_item = third;
}


atTriple::~atTriple()
{
   // Delete the items we are holding
   if (first_item != NULL)
      delete first_item;
   if (second_item != NULL)
      delete second_item;
   if (third_item != NULL)
      delete third_item;
}


atItem * atTriple::getFirst()
{
   // Return the first item
   return first_item;
}


atItem * atTriple::getSecond()
{
   // Return the second item
   return second_item;
}


atItem * atTriple::getThird()
{
   // Return the third item
   return third_item;
}

void atTriple::removeFirst()
{
   // Clear the first item
   first_item = NULL;
}


void atTriple::removeSecond()
{
   // Clear the second item
   second_item = NULL;
}


void atTriple::removeThird()
{
   // Clear the third item
   third_item = NULL;
}

bool atTriple::equals(atItem * otherItem)
{
   atTriple *   pairItem;
   bool         firsts;
   bool         seconds;
   bool         thirds;

   // Try to convert it to a pair to make sure it is a pair
   pairItem = dynamic_cast<atTriple *>(otherItem);

   // Return whether the two strings are equal or not
   if (pairItem != NULL) 
   {
      // Compare the "first" items and the "second" items and return 
      // if both sets match
      firsts = ((first_item == NULL) && (pairItem->first_item == NULL)) ||
               (first_item->equals(pairItem->first_item) == true);
      seconds = ((second_item == NULL) && (pairItem->second_item == NULL)) ||
                (second_item->equals(pairItem->second_item) == true);
      thirds = ((third_item == NULL) && (pairItem->third_item == NULL)) ||
                (third_item->equals(pairItem->third_item) == true);
      return (firsts && seconds && thirds);
   }
   else
      return false;
}


int atTriple::compare(atItem * otherItem)
{
   atTriple *   pairItem;
   int        com;

   // Try to cast to an atTriple
   pairItem = dynamic_cast<atTriple *>(otherItem);

   // See if the other item is valid
   if (pairItem != NULL)
   {
      // Check to see how the first items compare
      com = first_item->compare(pairItem->getFirst());
      if (com == 0)
      {
         // The first items are equal so let's check the second ones now
         com = second_item->compare(pairItem->getSecond());
         if (com == 0)
         {
            // The second items are equal so let's check the third ones now
            com = third_item->compare(pairItem->getThird());
         }
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

