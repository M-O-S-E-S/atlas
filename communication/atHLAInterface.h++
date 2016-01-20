
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


#ifndef AT_HLA_INTERFACE_H
#define AT_HLA_INTERFACE_H


#include "atIPCInterface.h++"
#include "atRTIInterface.h++"
#include "atList.h++"
#include "atConfigFile.h++"
#include "atOSDefs.h++"


typedef struct
{
   char        name[255];
   atClassID   rtiClass;
} atClassDefinition;

typedef struct
{
   char            name[255];
   char            className[255];
   atAttributeID   rtiAttribute;
} atAttributeDefinition;

typedef struct
{
   char              name[255];
   atInteractionID   rtiInteraction;
} atInteractionDefinition;

typedef struct
{
   char            name[255];
   char            interactionName[255];
   atParameterID   rtiParameter;
} atParameterDefinition;


class ATLAS_SYM atHLAInterface : public atIPCInterface
{
   protected:
      atRTIInterface *          rti_interface;

      u_long                    num_classes;
      u_long                    num_attributes;
      u_long                    num_interactions;
      u_long                    num_parameters;

      atClassDefinition         rti_classes[500];
      atAttributeDefinition     rti_attributes[500];
      atInteractionDefinition   rti_interactions[500];
      atParameterDefinition     rti_parameters[500];

      atList *                  return_list;

      void              registerVars(atConfigFile * cfg);
      void              publishAndSubscribe();
      void              instance();

      atList *          update();

      atClassID         lookupRTIObjectClass(char * name);
      char *            lookupRTIObjectClassID(atClassID id);
      atInteractionID   lookupRTIInteraction(char * name);

      void              addPair(atList * list, char * name, char * value);
      void              addPair(atList * list, char * name, 
                                char * value, int valueSize);

   public:
      atHLAInterface(char * fedExecName, char * fedMetafile);
      ~atHLAInterface();

      int   read(u_char * buffer, u_long length);
      int   write(u_char * buffer, u_long length);
};


#endif

