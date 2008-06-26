
#ifndef AT_RTI_INTERFACE_AMBASSADOR
#define AT_RTI_INTERFACE_AMBASSADOR


#define RTI_USES_STD_FSTREAM

#include "atOSDefs.h"
#include "NullFederateAmbassador13.h"


class ATLAS_SYM atRTIInterfaceAmbassador : public rti13::NullFederateAmbassador
{
   public:
      atRTIInterfaceAmbassador();
      virtual ~atRTIInterfaceAmbassador() throw (rti13::FederateInternalError);

      void   discoverObjectInstance(rti13::ObjectHandle obj,  
                rti13::ObjectClassHandle objClass, const char * objName)
                throw (rti13::CouldNotDiscover, rti13::ObjectClassNotKnown,
                       rti13::FederateInternalError);

      void   reflectAttributeValues(rti13::ObjectHandle obj, 
                const rti13::AttributeHandleValuePairSet& attrs,
                const rti13::FedTime& time, const char * tag,
                rti13::EventRetractionHandle handle)
                throw (rti13::ObjectNotKnown, rti13::AttributeNotKnown,
                       rti13::FederateOwnsAttributes, 
                       rti13::InvalidFederationTime,
                       rti13::FederateInternalError);

      void   reflectAttributeValues(rti13::ObjectHandle obj, 
                const rti13::AttributeHandleValuePairSet& attrs,
                const char * tag)
                throw (rti13::ObjectNotKnown, rti13::AttributeNotKnown,
                       rti13::FederateOwnsAttributes,
                       rti13::FederateInternalError);

      void   receiveInteraction(rti13::InteractionClassHandle interaction,
                const rti13::ParameterHandleValuePairSet& parameters,
                const rti13::FedTime& time, const char * tag,
                rti13::EventRetractionHandle handle)
                throw (rti13::InteractionClassNotKnown, 
                       rti13::InteractionParameterNotKnown,
                       rti13::InvalidFederationTime, 
                       rti13::FederateInternalError);

      void   receiveInteraction(rti13::InteractionClassHandle interaction,
                const rti13::ParameterHandleValuePairSet& parameters, 
                const char * tag)
                throw (rti13::InteractionClassNotKnown, 
                       rti13::InteractionParameterNotKnown,
                       rti13::FederateInternalError);

      void   removeObjectInstance(rti13::ObjectHandle obj, 
                const rti13::FedTime& time, const char * tag, 
                rti13::EventRetractionHandle handle)
                throw (rti13::ObjectNotKnown, rti13::InvalidFederationTime,
                       rti13::FederateInternalError);

      void   removeObjectInstance(rti13::ObjectHandle obj, const char * tag)
                throw (rti13::ObjectNotKnown, rti13::FederateInternalError);

};


#endif

