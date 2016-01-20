
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


#ifndef AT_STRING_TOKENIZER_H
#define AT_STRING_TOKENIZER_H


#include "atNotifier.h++"
#include "atOSDefs.h++"
#include "atString.h++"


class ATLAS_SYM atStringTokenizer : public atNotifier
{
   protected:
      atString *   the_string;
      char *       the_chars;

      u_long       string_index;

   public:
      atStringTokenizer(atString str);
      virtual ~atStringTokenizer();

      atString *   getToken(atString delimiters);
      atString *   getToken(char delimiters[]);
};


#endif

