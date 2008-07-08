
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

