
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


#ifndef AT_CONFIG_FILE_H
#define AT_CONFIG_FILE_H


#include "atNotifier.h++"
#include "atOSDefs.h++"
#include "atString.h++"
#include "atStringTokenizer.h++"


#define AT_MAX_KEY_LENGTH        256
#define AT_MAX_ARGUMENT_LENGTH   256


typedef struct atCfgArgument
{
   char                     argument[AT_MAX_ARGUMENT_LENGTH];
   struct atCfgArgument *   next;
} atTupleArgument;

typedef struct atCfgHead
{
   atTupleArgument *    argumentList;
   atTupleArgument *    argumentListEnd;
   struct atCfgHead *   next;
} atTupleHead;

typedef struct atCfgKey
{
   char                key[AT_MAX_KEY_LENGTH];
   atTupleHead *       tupleList;
   atTupleHead *       tupleListEnd;
   atTupleHead *       currentTuple;
   struct atCfgKey *   next;
} atTupleKey;


class ATLAS_SYM atConfigFile : public atNotifier
{
   protected:
      atTupleKey *          tuple_list;
      atTupleKey *          tuple_list_end;

      atStringTokenizer *   current_tokenizer;

      atString *          getToken();
      atTupleKey *        findKey(char * key);
      atTupleKey *        addKey(char * key);
      atTupleHead *       addTuple(atTupleKey * key);
      atTupleArgument *   addArgument(atTupleHead * head, char * arg);

   public:
      atConfigFile(char * filename);
      ~atConfigFile();

      virtual bool   getNextTuple(char * key, int * argc, char * argv[]);
      virtual bool   resetTupleList(char * key);
};

#endif

