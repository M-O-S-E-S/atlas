
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


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


void atPair::setFirst(atItem * first)
{
   // Delete existing first and then set new one
   if (first_item != NULL)
      delete first_item;
   first_item = first;
}


void atPair::setSecond(atItem * second)
{
   // Delete existing second and then set new one
   if (second_item != NULL)
      delete second_item;
   second_item = second;
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
   if (pairItem != NULL) 
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

