
#ifndef AT_RTI_INTERFACE
#define AT_RTI_INTERFACE


#include "atNotifier.h++"
#include "atList.h++"
#include "atKeyedBufferHandler.h++"

#define RTI_USES_STD_FSTREAM
#include "NullFederateAmbassador13.h"

#include "atRTIInterfaceAmbassador.h++"


#define AT_RTI_MAX_CLASS_HANDLES       100
#define AT_RTI_MAX_OBJECT_HANDLES      300
#define AT_RTI_MAX_ATTRIBUTE_HANDLES   300
#define AT_RTI_MAX_PARAMETER_HANDLES   300

#define AT_RTI_MAX_MAP_ENTRIES         AT_RTI_MAX_CLASS_HANDLES * AT_RTI_MAX_PARAMETER_HANDLES

#define AT_RTI_ATTRIBUTE_TYPE     1
#define AT_RTI_INTERACTION_TYPE   2
#define AT_RTI_CONTROL_TYPE       3


typedef long   atClassID;
typedef long   atAttributeID;
typedef long   atInteractionID;
typedef long   atParameterID;
typedef long   atInstanceID;


typedef struct atInteractionParameterMapEntry
{
   rti13::InteractionClassHandle  interaction;
   rti13::ParameterHandle         parameter;
};

typedef struct atObjectAttributeMapEntry
{
   rti13::ObjectClassHandle  objClass;
   rti13::AttributeHandle    attribute;
};

typedef struct atObjectInstance
{
   rti13::ObjectHandle        obj;
   rti13::ObjectClassHandle   objClass;
   char                       name[256];
};


class atRTIInterface : public atNotifier
{
   protected:
      static atRTIInterface *          rti_interface_instance;

      rti13::RTIambassador             rti_amb;
      atRTIInterfaceAmbassador         fed_amb;

      rti13::FederateHandle            federate_id;
      char                             fed_exec_name[255];

      u_long                           num_class_handles;
      rti13::ObjectClassHandle         class_handles[AT_RTI_MAX_CLASS_HANDLES];

      u_long                           num_attribute_handles[AT_RTI_MAX_CLASS_HANDLES];
      rti13::AttributeHandle           attribute_handles[AT_RTI_MAX_CLASS_HANDLES][AT_RTI_MAX_ATTRIBUTE_HANDLES];

      u_long                           num_object_handles;
      atObjectInstance                 object_handles[AT_RTI_MAX_OBJECT_HANDLES];

      atObjectAttributeMapEntry        object_attribute_map[AT_RTI_MAX_MAP_ENTRIES];
      u_long                           num_object_attribute_map_entries;

      u_long                           num_interaction_class_handles;
      rti13::InteractionClassHandle    interaction_class_handles[AT_RTI_MAX_CLASS_HANDLES];
      char                             interaction_names[AT_RTI_MAX_CLASS_HANDLES][256];

      u_long                           num_interaction_parameter_handles[AT_RTI_MAX_CLASS_HANDLES];
      rti13::ParameterHandle           interaction_parameter_handles[AT_RTI_MAX_CLASS_HANDLES][AT_RTI_MAX_PARAMETER_HANDLES];

      atInteractionParameterMapEntry   interaction_parameter_map[AT_RTI_MAX_MAP_ENTRIES];
      u_long                           num_interaction_parameter_map_entries;

      atList *                         buf_list;

   // HLA with its separate ambassador is evil (where is the object-oriented
   // design?)
   public:
      void   processAmbDiscover(rti13::ObjectHandle obj,
                rti13::ObjectClassHandle, const char *name);

      void   processAmbRemove(rti13::ObjectHandle obj);

      void   processAmbAttribute(rti13::ObjectHandle obj,
                const rti13::AttributeHandleValuePairSet& attrs);

      void   processAmbInteraction(rti13::InteractionClassHandle interaction,
                const rti13::ParameterHandleValuePairSet& parameters);

   public:
      atRTIInterface(char* fedExecName, char * fedFilename);
      virtual ~atRTIInterface();

      static atRTIInterface *   getInstance(char * fedExecName, 
                                            char * fedFilename);
      static atRTIInterface *   getInstance();

      atClassID         registerClass(char * classTypeStr);
      atAttributeID     registerAttribute(char * attrTypeStr, 
                                          atClassID classTypeID);
      atInteractionID   registerInteraction(char * interactionTypeStr);
      atParameterID     registerParameter(char * parameterTypeStr, 
                                          atInteractionID interactionTypeID);

      void   subscribeAttributes(atClassID classTypeID,
                                 u_long numAttributes,
                                 atAttributeID attrTypeIDs[]);
      void   publishAttributes(atClassID classTypeID,
                               u_long numAttributes,
                               atAttributeID attrTypeIDs[]);

      void   subscribeInteractions(u_long numInteractions,
                                   atInteractionID interactionTypeIDs[]);
      void   publishInteractions(u_long numInteractions,
                                 atInteractionID interactionTypeIDs[]);

      atInstanceID   createInstance(atClassID classTypeID);

      void   tick();

      atKeyedBufferHandler *   read();

};


#endif

