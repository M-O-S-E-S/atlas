
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


#include "atTimer.h++"

#include <stdio.h>


atTimer::atTimer()
{
   // Set mark_time to 0.0
   mark_time = 0.0;

   // Call mark() twice to initialize the mark interval time
   // and set a sane mark_time
   mark();
   mark();
}


atTimer::~atTimer()
{
}


void atTimer::mark()
{
   struct timeval timeStruct;
   double newMark;

   // Get the current time from the system
   gettimeofday(&timeStruct, NULL);

   // Record the current time in seconds
   newMark = timeStruct.tv_sec + (timeStruct.tv_usec / 1000000.0);

   // Compute the new mark interval
   mark_interval = newMark - mark_time;

   // Save the new mark time
   mark_time = newMark;
}


void atTimer::markAtInterval(double intervalTime)
{
   struct timeval timeStruct;
   double newMark;

   // Get the current time from the system
   gettimeofday(&timeStruct, NULL);

   // Record the current time in seconds
   newMark = timeStruct.tv_sec + (timeStruct.tv_usec / 1000000.0);

   // Compute the new mark interval
   mark_interval = newMark - mark_time;

   // See if the given interval is less than the elapsed interval
   if (mark_interval > intervalTime)
   {
      // Use the given interval, and set the new mark time to the time
      // that the given interval would have expired
      mark_time = newMark - (mark_interval - intervalTime);
      mark_interval = intervalTime;
   }
   else
   {
      // Use the actual elapsed interval and set the new mark time as if
      // mark() had been called
      mark_time = newMark;
   }
}


double atTimer::getInterval()
{
   return mark_interval;
}


double atTimer::getElapsed()
{
   struct timeval timeStruct;
   double currentTime;

   // Get the current time in seconds from the system
   gettimeofday(&timeStruct, NULL);
   currentTime = timeStruct.tv_sec + (timeStruct.tv_usec / 1000000.0);
    
   // Return the difference between the current time and the last mark()
   return (currentTime - mark_time);
}
