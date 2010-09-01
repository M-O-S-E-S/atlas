
#include <string.h>
#include "atItem.h++"


atItem::atItem()
{
}


atItem::~atItem()
{
}


bool atItem::equals(atItem * otherItem)
{
   // Just compare pointer values at this level (child classes can
   // have more sophisticated comparisons)
   if (this == otherItem)
      return true;
   else
      return false;
}


int atItem::compare(atItem * otherItem)
{
   // Return the difference between the items' pointer values
   return (int) otherItem - (int) this;
}

