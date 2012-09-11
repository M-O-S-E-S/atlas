
#include "atThreadCount.h++"


atThreadCount::atThreadCount()
{
   // Initialize count
   thread_count = 0;
}


atThreadCount::~atThreadCount()
{
}


u_long atThreadCount::getCount()
{
   // Return the current count
   return thread_count;
}


void atThreadCount::inc()
{
   // Add one to the count
   thread_count++;
}


void atThreadCount::dec()
{
   // Subtract one to the count
   thread_count--;
}

