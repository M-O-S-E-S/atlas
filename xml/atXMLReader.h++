
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


#ifndef AT_XML_READER_H
#define AT_XML_READER_H


#include <libxml/xmlreader.h>
#include "atNotifier.h++"
#include "atOSDefs.h++"
#include "atXMLDocument.h++"


// TYPES
// These types help encapsulate the particular XML library we are using
// (theoretically, we could switch from libxml2 to something else by just
// reimplementing this class and these types)
typedef xmlTextReaderPtr    atXMLTextReaderPtr;


class ATLAS_SYM atXMLReader : public atNotifier
{
   protected:
      atXMLTextReaderPtr   xml_reader;

   public:
      atXMLReader(char * filename);
      atXMLReader(int fileDescriptor);
      atXMLReader(char * memory, u_long memorySize);
      atXMLReader(atXMLDocument * doc);

      virtual ~atXMLReader();

      atXMLDocument *   getSubDocument(char * elementName);
};


#endif

