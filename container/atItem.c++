
#include "atItem.h++"


atItem::atItem()
{
}


atItem::~atItem()
{
}

bool atItem::equals(atItem *otherItem)
{
   // Just compare pointer values at this level.  Child classes can
   // have more sophisticated comparisons
   if (this == otherItem)
      return true;
   else
      return false;
}

