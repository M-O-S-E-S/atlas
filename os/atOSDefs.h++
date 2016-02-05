
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


#ifndef AT_OS_DEFS_H
#define AT_OS_DEFS_H


// We don't want to use Microsoft's ntohf, etc. functions for floats/doubles
#ifdef _MSC_VER
#define NO_EXTRA_HTON_FUNCTIONS
#endif


// Include other files to give OS-independent interfaces
#if defined(__AT_BLUETOOTH_ENABLED__)
#include "atBluetooth.h++"
#endif
#include "atByteSwap.h++"
#include "atDynamic.h++"
#include "atErrno.h++"
#include "atFile.h++"
#include "atFloatTypes.h++"
#include "atIntTypes.h++"
#include "atLang.h++"
#include "atNetwork.h++"
#include "atSem.h++"
#include "atShm.h++"
#include "atSleep.h++"
#include "atStr.h++"
#include "atSymbols.h++"
#include "atThread.h++"
#include "atTime.h++"
#if defined(__AT_UUID_ENABLED__)
#include "atUUID.h++"
#endif


#endif

