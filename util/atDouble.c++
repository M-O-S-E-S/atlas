
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


#include "atDouble.h++"
#include "atGlobals.h++"


atDouble::atDouble(double val)
{
   // Set the value
   double_value = val;
}


atDouble::~atDouble()
{
}


void atDouble::setValue(double val)
{
   // Set the value
   double_value = val;
}


double atDouble::getValue()
{
   // Return the value
   return double_value;
}


bool atDouble::equals(atItem * otherItem)
{
   atDouble *   doubleItem;
   
   // Try to cast the other item to an atDouble
   doubleItem = dynamic_cast<atDouble *>(otherItem);

   // See if the other item is valid
   if (doubleItem != NULL)
   {
      // Return the double comparison of the values
      return (AT_EQUAL(double_value, doubleItem->getValue()));
   }
   else
   {
      // The types of items didn't match so it has to be different double
      // values
      return false;
   }
}


int atDouble::compare(atItem * otherItem)
{
   atDouble *   doubleItem;

   // Try to cast the other item to an atDouble
   doubleItem = dynamic_cast<atDouble *>(otherItem);

   // See if the other item is valid
   if (doubleItem != NULL)
   {
      // Due to the inaccurate nature of floating point numbers use a series 
      // of checks to determine the compare return value
      if (AT_EQUAL(double_value, doubleItem->getValue()))
      {
         // The two floating numbers are equal within a tolerance located 
         // inside of atGlobals so return 0
         return 0;
      }
      else if (double_value - doubleItem->getValue() < 0.0f)
      {
         // The current value is less than the other value so return -1 to 
         // indicate this
         return -1;
      }
      else
      {
         // The current value is greater than the other value so return 1 
         // to indicate this
         return 1;
      }
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}

