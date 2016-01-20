
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


#ifndef AT_SYMBOLS_H
#define AT_SYMBOLS_H


// Symbol-specific linkage directives.  Windows requires explicit export
// and import instructions for its shared libraries (DLL's) and GCC can
// provide such capabilities as well (although it's not required).
// The following sections define tokens that specify the linkage and
// are used in the Makefile (or Project Settings for Windows).  
//
// Constants for the appropriate library (or even sub-library) are
// defined to one of these constants in the Makefile.  For example,
//
//   g++ -o test test.c++ -DATLAS_SYM=IMPORT
//
// and a class might be defined such as:
// 
// class ATLAS_SYM Test
// {
// };
//
// Typical use would export symbols when making a library that
// will be used by others, and import symbols when making an
// application that uses the library.

#ifdef _WIN32
   #define IMPORT __declspec(dllimport)
   #define EXPORT __declspec(dllexport)
#elif __ANDROID__
   #define IMPORT __attribute__((visibility("default")))
   #define EXPORT __attribute__((visibility("default")))
#else
   #define IMPORT __attribute__((visibility("default")))
   #define EXPORT __attribute__((visibility("default")))
#endif


#endif

