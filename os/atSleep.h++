
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


#ifndef AT_SLEEP_H
#define AT_SLEEP_H


// Under Windows, define the sleep() and usleep() functions as macros of
// the Windows Sleep() function
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <wingdi.h>

   // Sleep() takes milliseconds so multiply x by 1000 for sleep()
   #define sleep(x)  Sleep((x) * 1000)

   // Sleep() takes milliseconds, so divide x by 1000 for usleep()
   // if the result of (x/1000) is zero, the thread will still sleep
   // (give up the processor)
   #define usleep(x) Sleep((x) / 1000)
#endif


#endif

