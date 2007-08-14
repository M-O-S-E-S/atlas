
#include <typeinfo>
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
   // Return the string comparison difference between the two item's class
   // names
   // JPD:  This seems kind of weak, but it will be a consistent ordering.
   //       We'll try it for now.
   return strcmp(typeid(*this).name(), typeid(*otherItem).name());
}

