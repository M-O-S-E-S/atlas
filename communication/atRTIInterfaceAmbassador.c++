
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


#include "atRTIInterfaceAmbassador.h++"
#include "atRTIInterface.h++"


atRTIInterfaceAmbassador::atRTIInterfaceAmbassador()
{
}


atRTIInterfaceAmbassador::~atRTIInterfaceAmbassador() 
   throw (rti13::FederateInternalError)
{
}


void atRTIInterfaceAmbassador::discoverObjectInstance(
   rti13::ObjectHandle obj, rti13::ObjectClassHandle objClass,
   const char * objName)
   throw (rti13::CouldNotDiscover, rti13::ObjectClassNotKnown,
          rti13::FederateInternalError)
{
    atRTIInterface::getInstance()->processAmbDiscover(obj, objClass, objName);
}


void atRTIInterfaceAmbassador::reflectAttributeValues(
   rti13::ObjectHandle obj, const rti13::AttributeHandleValuePairSet& attrs,
   const rti13::FedTime& time, const char * tag,
   rti13::EventRetractionHandle handle)
   throw (rti13::ObjectNotKnown, rti13::AttributeNotKnown,
          rti13::FederateOwnsAttributes,
          rti13::InvalidFederationTime,
          rti13::FederateInternalError)
{
   reflectAttributeValues(obj, attrs, tag);
}


void atRTIInterfaceAmbassador::reflectAttributeValues(
   rti13::ObjectHandle obj, const rti13::AttributeHandleValuePairSet& attrs,
   const char * tag)
   throw (rti13::ObjectNotKnown, rti13::AttributeNotKnown,
          rti13::FederateOwnsAttributes,
          rti13::FederateInternalError)
{
   atRTIInterface::getInstance()->processAmbAttribute(obj, attrs);
}


void atRTIInterfaceAmbassador::receiveInteraction(
   rti13::InteractionClassHandle interaction,
   const rti13::ParameterHandleValuePairSet& parameters,
   const rti13::FedTime& time, const char * tag,
   rti13::EventRetractionHandle handle)
   throw (rti13::InteractionClassNotKnown, rti13::InteractionParameterNotKnown,
          rti13::InvalidFederationTime, rti13::FederateInternalError)
{
   receiveInteraction(interaction, parameters, tag);
}
 

void atRTIInterfaceAmbassador::receiveInteraction(
   rti13::InteractionClassHandle interaction,
   const rti13::ParameterHandleValuePairSet& parameters, const char * tag)
   throw (rti13::InteractionClassNotKnown, rti13::InteractionParameterNotKnown,
          rti13::FederateInternalError)
{
   atRTIInterface::getInstance()->processAmbInteraction(interaction, parameters);
}


void atRTIInterfaceAmbassador::removeObjectInstance(
   rti13::ObjectHandle obj, const rti13::FedTime& time, 
   const char * tag, rti13::EventRetractionHandle handle)
   throw (rti13::ObjectNotKnown, rti13::InvalidFederationTime,
          rti13::FederateInternalError)
{
   removeObjectInstance(obj, tag);
}


void atRTIInterfaceAmbassador::removeObjectInstance(
   rti13::ObjectHandle obj, const char * tag)
   throw (rti13::ObjectNotKnown, rti13::FederateInternalError)
{
    atRTIInterface::getInstance()->processAmbRemove(obj);
}

