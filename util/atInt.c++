
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


#include "atInt.h++"


atInt::atInt(int val)
{
   // Set the value
   int_value = val;
}


atInt::~atInt()
{
}


void atInt::setValue(int val)
{
   // Set the value
   int_value = val;
}


int atInt::getValue()
{
   // Return the value
   return int_value;
}


bool atInt::equals(atItem * otherItem)
{
   atInt *   intItem;
   
   // Try to cast the other item to an atInt
   intItem = dynamic_cast<atInt *>(otherItem);

   // See if the other item is valid
   if (intItem != NULL)
   {
      // Return the int comparison of the values
      return int_value == intItem->getValue();
   }
   else
   {
      // The types of items didn't match so it has to be different integer
      // values
      return false;
   }
}


int atInt::compare(atItem * otherItem)
{
   atInt *   intItem;

   // Try to cast the other item to an atInt
   intItem = dynamic_cast<atInt *>(otherItem);

   // See if the other item is valid
   if (intItem != NULL)
   {
      // Return the difference between the integer values
      return int_value - intItem->getValue();
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

