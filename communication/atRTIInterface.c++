
#include "atRTIInterface.h++"


atRTIInterface * atRTIInterface::rti_interface_instance = NULL;


atRTIInterface::atRTIInterface(char * fedExecName, char * fedFilename)
{
   bool   joined;
   int    numTries;

   // Set the singleton instance
   if (rti_interface_instance == NULL)
      rti_interface_instance = this;

   // Initialize variables
   num_class_handles = 0;
   num_object_handles = 0;
   memset(num_attribute_handles, 0, sizeof(num_attribute_handles));
   num_object_attribute_map_entries = 0;

   num_interaction_class_handles = 0;
   memset(num_interaction_parameter_handles, 0, 
      sizeof(num_interaction_parameter_handles));
   num_interaction_parameter_map_entries = 0;

   strcpy(fed_exec_name, fedExecName);
   buf_list = new atList();

   // Create federation execution (it will succeed if we're first; otherwise,
   // catch the exception so we know we're not)
   try
   {
      // Note that we use the .fed name as the federation execution name
      // as well
      rti_amb.createFederationExecution(fedExecName, fedFilename);
      notify(AT_INFO, "Created federation execution.\n");
   }
   catch (rti13::FederationExecutionAlreadyExists& e)
   {
   }
   catch (rti13::Exception& e)
   {
   }

   // Join the federation (we need to loop to allow the system that
   // created the federation to get it up and running)
   joined = false;
   numTries = 0;
   while ( (joined == false) && (numTries < 20) )
   {
      try
      {
         // My mind to your mind, my thoughts to your thoughts...
         federate_id = rti_amb.joinFederationExecution("IST-ATLAS",
                                                       fedExecName,
                                                       &fed_amb);

         // We joined so mark it as such
         joined = true;
      }
      catch (rti13::FederateAlreadyExecutionMember& e)
      {
      }
      catch (rti13::FederationExecutionDoesNotExist& e)
      {
      }
      catch (rti13::Exception& e)
      {
      }

      numTries++;
   }
   
   // Tell user if we joined the federation execution
   if (joined == true)
      notify(AT_INFO, "Joined federation execution.\n");
}


atRTIInterface::~atRTIInterface()
{
   // Resign from the federation
   try
   {
      rti_amb.resignFederationExecution(
         rti13::NO_ACTION);
      notify(AT_INFO, "Resigned federation execution.\n");
   }
   catch (rti13::Exception& e)
   {
   }

   // Destroy the federation execution (if we are last)
   try
   {
      rti_amb.destroyFederationExecution(fed_exec_name);
   }
   catch (rti13::Exception& e)
   {
   }

   // Set singleton to NULL
   rti_interface_instance = NULL;
}


atRTIInterface * atRTIInterface::getInstance(char * fedExecName, 
                                             char * fedFilename)
{
   // Check to see if an instance exists, and create one if not
   if (rti_interface_instance == NULL)
   {
      rti_interface_instance = new atRTIInterface(fedExecName, fedFilename);
   }

   // Return the singleton instance of this class
   return rti_interface_instance;
}


atRTIInterface * atRTIInterface::getInstance()
{
   // Return the singleton instance of this class
   return rti_interface_instance;
}


atClassID atRTIInterface::registerClass(char * classTypeStr)
{
   // Get the object class handle and save it
   notify(AT_INFO, "Registering object class '%s'.\n", classTypeStr);
   class_handles[num_class_handles] = 
      rti_amb.getObjectClassHandle(classTypeStr);
   num_class_handles++;

   // Return an ID for the user to use
   return (num_class_handles-1);
}


atAttributeID atRTIInterface::registerAttribute(char *attrTypeStr, 
                                                atClassID classID)
{
   // Get the attribute handle and save it
   notify(AT_INFO, "Registering attribute '%s'.\n", attrTypeStr);
   attribute_handles[classID][num_attribute_handles[classID]] = 
      rti_amb.getAttributeHandle(attrTypeStr, class_handles[classID]);

   // Register the class/attribute pair in the attribute handle map
   object_attribute_map[num_object_attribute_map_entries].objClass = 
      class_handles[classID];
   object_attribute_map[num_object_attribute_map_entries].attribute = 
      attribute_handles[classID][num_attribute_handles[classID]];
   num_object_attribute_map_entries++;

   // Increment the number of known attribute handles for this class
   num_attribute_handles[classID]++;

   // Return an ID for the user to use
   return (num_object_attribute_map_entries-1);
}


atInteractionID atRTIInterface::registerInteraction(char * interactionTypeStr)
{
   // Get the object class handle and save it
   notify(AT_INFO, "Registering interaction '%s'.\n", interactionTypeStr);
   interaction_class_handles[num_interaction_class_handles] = 
      rti_amb.getInteractionClassHandle(interactionTypeStr);
   strcpy(interaction_names[num_interaction_class_handles], interactionTypeStr);
   num_interaction_class_handles++;

   // Return an ID for the user to use
   return (num_interaction_class_handles-1);
}


atParameterID atRTIInterface::registerParameter(char * parameterTypeStr,
   atInteractionID interactionTypeID)
{
   // Get the attribute handle and save it
   notify(AT_INFO, "Registering parameter '%s'.\n", parameterTypeStr);
   interaction_parameter_handles[interactionTypeID]
      [num_interaction_parameter_handles[interactionTypeID]] = 
         rti_amb.getParameterHandle(parameterTypeStr, 
                                 interaction_class_handles[interactionTypeID]);

   // Add this interaction/parameter pair to the parameter handle map
   interaction_parameter_map[num_interaction_parameter_map_entries].interaction = 
      interaction_class_handles[interactionTypeID];
   interaction_parameter_map[num_interaction_parameter_map_entries].parameter = 
      interaction_parameter_handles[interactionTypeID]
         [num_interaction_parameter_handles[interactionTypeID]];
   num_interaction_parameter_map_entries++;

   // Increment the number of known parameters for this interaction
   num_interaction_parameter_handles[interactionTypeID]++;

   // Return an ID for the user to use
   return (num_interaction_parameter_map_entries-1);
}


void atRTIInterface::subscribeAttributes(atClassID classTypeID,
                                         u_long numAttributes,
                                         atAttributeID attrTypeIDs[])
{
   rti13::AttributeHandleSet *   attributes;
   u_long                        i;

   // Create an attribute set
   attributes = rti13::AttributeHandleSetFactory::create(numAttributes);

   // Add the attributes to the attribute set
   for (i=0; i < numAttributes; i++)
      attributes->add(object_attribute_map[attrTypeIDs[i]].attribute);

   // Subscribe to the attributes
   rti_amb.subscribeObjectClassAttributes(class_handles[classTypeID],
                                          *attributes);

   // We're done with the attribute set
   attributes->empty();
   delete attributes;
}


void atRTIInterface::publishAttributes(atClassID classTypeID,
                                       u_long numAttributes,
                                       atAttributeID attrTypeIDs[])
{
   rti13::AttributeHandleSet *   attributes;
   u_long                        i;

   // Create an attribute set to hold the attributes
   attributes = rti13::AttributeHandleSetFactory::create(numAttributes);

   // Add the attributes to the set
   for (i=0; i < numAttributes; i++)
      attributes->add(attribute_handles[classTypeID][attrTypeIDs[i]]);

   // Publish the attributes
   rti_amb.publishObjectClass(class_handles[classTypeID], *attributes);

   // We're done with the attribute set
   attributes->empty();
   delete attributes;
}


void atRTIInterface::subscribeInteractions(u_long numInteractions,
                                           atInteractionID interactionTypeIDs[])
{
   u_long   i;

   // Go through the interactions and subscribe to each
   for (i=0; i < numInteractions; i++)
   {
      rti_amb.subscribeInteractionClass(
         interaction_class_handles[interactionTypeIDs[i]]);
   }
}


void atRTIInterface::publishInteractions(u_long numInteractions,
                                         atInteractionID interactionTypeIDs[])
{
}


atInstanceID atRTIInterface::createInstance(atClassID classTypeID)
{
   return -1;
}


void atRTIInterface::tick()
{
   rti_amb.tick();
}


void atRTIInterface::processAmbDiscover(rti13::ObjectHandle obj,
   rti13::ObjectClassHandle objClass, const char * name)
{
   int                           nameLength;
   rti13::AttributeHandleSet *   attributes;
   u_long                        i;
   u_long                        numAttributes;

   // Copy the object handle and class handle
   object_handles[num_object_handles].obj = obj;
   object_handles[num_object_handles].objClass = objClass;

   // Copy the object name and null-terminate it
   nameLength = sizeof(object_handles[num_object_handles].name);
   strncpy(object_handles[num_object_handles].name, name, nameLength);
   object_handles[num_object_handles].name[nameLength-1] = 0;

   printf("New entity:  handle = %d   class = %d  name = %s\n",
      obj, objClass, name);

   // Increment the number of object handles
   num_object_handles++;

   // Go through all the object-attribute map entries to see how many go
   // with this object
   numAttributes = 0;
   for (i=0; i < num_object_attribute_map_entries; i++)
   {
      // See if this entry goes with the new object's class and count if so
      if (object_attribute_map[i].objClass == objClass)
         numAttributes++;
   }
   printf("Requesting %d attributes\n", numAttributes);

   // Create an attribute set
   attributes = rti13::AttributeHandleSetFactory::create(numAttributes);

   // Now go through all the object-attribute map entries and add the 
   // ones that go with this entity to the attribute set
   for (i=0; i < num_object_attribute_map_entries; i++)
   {
      // See if this entry goes with the new object's class
      if (object_attribute_map[i].objClass == objClass)
      {
         // It does so add the attribute to the set
         attributes->add(object_attribute_map[i].attribute);
      }
   }

   // Now take the attribute set and request updates of them
   rti_amb.requestObjectAttributeValueUpdate(obj, *attributes);

   // We're done with the attribute set
   attributes->empty();
   delete attributes;
}


void atRTIInterface::processAmbRemove(rti13::ObjectHandle obj)
{
   int                      i;
   u_long                   c;
   atClassID                classID;
   atKeyedBufferHandler *   buf;
   char *                   targetID;
   char *                   tmpMsg;

   // Find the object handle in our object array
   for (i = 0; i < num_object_handles; i++)
   {
      // See if we found the object
      if (object_handles[i].obj == obj)
      {
         printf("Remove entity:  handle = %d   class = %d  name = %s\n",
            obj, object_handles[i].objClass, object_handles[i].name);

         // Figure out the class ID for this object
         classID = 0;
         for (c=0; c < num_class_handles; c++)
         {
            if (class_handles[c] == object_handles[i].objClass)
               classID = c;
         }

         // Create a buffer with the remove entity information
         tmpMsg = (char *) calloc(8, sizeof(char));
         sprintf(tmpMsg, "REMOVE");
         buf = new atKeyedBufferHandler(AT_RTI_CONTROL_TYPE, 
                                        strdup(object_handles[i].name),
                                        classID,
                                        i, (u_char *) tmpMsg, 7);
         buf_list->addEntry(buf);

         // Initialize space for the message target ID
         // (in this case we just use "Control" for the target)
         targetID = (char *) calloc(8, sizeof(char));
         sprintf(targetID, "Control");

         // Create the terminator buffer to terminate this interaction
         // parameter set
         buf = new atKeyedBufferHandler(AT_RTI_CONTROL_TYPE, 
                                        targetID, classID,
                                        0, NULL, 0);
         buf_list->addEntry(buf);

         // If we're not at the last element in the object list, we need
         // to move the remaining data
         if (i < num_object_handles - 1)
         {
            // Remove the object by sliding the remaining objects down
            // into place
            memmove(&object_handles[i], &object_handles[i+1],
               sizeof(atObjectInstance) * (num_object_handles - i - 1));
         }

         // Decrement the object count
         num_object_handles--;
      }
   }
}


void atRTIInterface::processAmbAttribute(rti13::ObjectHandle obj, 
   const rti13::AttributeHandleValuePairSet& attrs)
{
   char *                     msg;
   u_long                     i;
   u_long                     j;
   u_long                     k;
   u_long                     c;
   atClassID                  classID;
   rti13::AttributeHandle     attrHandle;
   rti13::ULong               valueLen;
   char *                     targetID;
   atKeyedBufferHandler *     buf;
   bool                       found;

   // Go through the object classes, looking for a match
   found = false;
   for (i=0; i < num_object_handles; i++)
   {
      if (obj == object_handles[i].obj)
      {
         // printf("Updating entity %d (%s) (%d attributes)\n", obj, 
         //        object_handles[i].name, attrs.size());

         // Figure out the class ID for this object
         classID = 0;
         for (c=0; c < num_class_handles; c++)
         {
            if (class_handles[c] == object_handles[i].objClass)
               classID = c;
         }

         // Found the object so go through the attributes
         for (j=0; j < attrs.size(); j++)
         {
            attrHandle = attrs.getHandle(j);

            for (k=0; k < num_object_attribute_map_entries; k++)
            {
               if ((object_handles[i].objClass == 
                    object_attribute_map[k].objClass) &&
                   (attrHandle == object_attribute_map[k].attribute))
               {
                  found = true;

                  // Initialize space for the message target ID
                  targetID = (char *) calloc(strlen(object_handles[i].name) + 1,
                                             sizeof(char));
                  strcpy(targetID, object_handles[i].name);

                  // Initialize space for the message
                  msg = (char *) calloc(1024, sizeof(char));

                  attrs.getValue(j, (char *) msg, valueLen);

                  buf = new atKeyedBufferHandler(AT_RTI_ATTRIBUTE_TYPE, 
                                                 targetID, classID,
                                                 k, (u_char *) msg, valueLen);

                  buf_list->addEntry(buf);
               }
            }
         }

         // Initialize space for the message target ID
         targetID = (char *) calloc(strlen(object_handles[i].name) + 1,
                                    sizeof(char));
         strcpy(targetID, object_handles[i].name);

         // Create the terminator buffer to terminate this attribute set
         buf = new atKeyedBufferHandler(AT_RTI_ATTRIBUTE_TYPE, 
                                        targetID, classID,
                                        0, NULL, 0);
         buf_list->addEntry(buf);
      }
   }

   if (found == false)
      notify(AT_WARN, "Didn't find match for attribute update.\n");
}


void atRTIInterface::processAmbInteraction(
   rti13::InteractionClassHandle interaction,
   const rti13::ParameterHandleValuePairSet& parameters)
{
   u_long                   i;
   u_long                   j;
   u_long                   k;
   rti13::ParameterHandle   paramHandle;
   rti13::ULong             valueLen;
   char *                   targetID;
   char *                   msg;
   atKeyedBufferHandler *   buf;

   printf("New interaction:  handle = %d\n", interaction);

   // Go through the interaction classes, looking for a match
   for (i=0; i < num_interaction_class_handles; i++)
   {
      if (interaction == interaction_class_handles[i])
      {
         // Found the interaction so go through the parameters
         for (j=0; j < parameters.size(); j++)
         {
            paramHandle = parameters.getHandle(j);

            for (k=0; k < num_interaction_parameter_map_entries; k++)
            {
               if ((interaction_class_handles[i] == 
                    interaction_parameter_map[k].interaction) &&
                   (paramHandle == interaction_parameter_map[k].parameter))
               {
                  // Initialize space for the message target ID
                  targetID = (char *)
                     calloc(strlen(interaction_names[i]) + 1, sizeof(char));
                  strcpy(targetID, interaction_names[i]);

                  // Initialize space for the message
                  msg = (char *) calloc(1024, sizeof(char));

                  parameters.getValue(j, (char *) msg, valueLen);

                  buf = new atKeyedBufferHandler(AT_RTI_INTERACTION_TYPE, 
                                                 targetID, 0,
                                                 k, (u_char *) msg, valueLen);

                  buf_list->addEntry(buf);
               }
            }
         }

         // Initialize space for the message target ID if we don't have one
         // (in this case we just use "Interaction" for the target)
         targetID = (char *) calloc(12, sizeof(char));
         sprintf(targetID, "Interaction");

         // Create the terminator buffer to terminate this interaction
         // parameter set
         buf = new atKeyedBufferHandler(AT_RTI_INTERACTION_TYPE, 
                                        targetID, 0,
                                        0, NULL, 0);
         buf_list->addEntry(buf);
      }
   }
}


atKeyedBufferHandler * atRTIInterface::read()
{
   atKeyedBufferHandler *   buf;

   buf = (atKeyedBufferHandler *) buf_list->getFirstEntry();
   if (buf != NULL)
      buf_list->removeCurrentEntry();

   return buf;
}

