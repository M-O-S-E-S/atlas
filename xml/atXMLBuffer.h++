
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


#ifndef AT_XML_BUFFER_HPP
#define AT_XML_BUFFER_HPP

#include <sys/types.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "atNotifier.h++"
#include "atList.h++"
#include "atOSDefs.h++"

#include "atXMLDocument.h++"
#include "atBufferHandler.h++"

#include "atString.h++"


#define INITIAL_XML_DOCUMENT_SIZE    2000000
#define MAX_XML_TAG_SIZE         256


class ATLAS_SYM atXMLBuffer : public atNotifier
{
   protected:
      u_char *       xml_buffer;
      u_long         xml_buffer_len;
      u_long         xml_buffer_size;

      char           xml_header[MAX_XML_TAG_SIZE];
      char           xml_footer[MAX_XML_TAG_SIZE];

      xmlDtdPtr      xml_dtd;
      xmlValidCtxt   xml_context;

      atList *       xml_document_list;

      float          xmlToFloat(xmlChar * tmpStr);
      int            xmlToInt(xmlChar * tmpStr);

      void           checkBufferSize(u_long sizeNeeded);
      void           processXMLDocument();

   public:
      atXMLBuffer(char * xmlName);
      atXMLBuffer(char * xmlName, char * dtdFilename);
      virtual ~atXMLBuffer();

      atList *   processBuffer(atBufferHandler * packetBuffer);
};

#endif

