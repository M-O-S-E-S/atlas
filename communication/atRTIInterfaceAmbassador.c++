
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

