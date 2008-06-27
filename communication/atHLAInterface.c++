
#include "atHLAInterface.h++"
#include "atRTIInterface.h++"
#include "atNameValuePair.h++"
#include "atConfigFile.h++"


atHLAInterface::atHLAInterface(char * fedExecName, char * metaFilename)
{
   atConfigFile *   cfg;
   int              count;
   char *           values[100];
   char             fedDir[1024];
   char             fedFilename[1024];

   // Open the meta filename
   cfg = new atConfigFile(metaFilename);

   // Get the base fed filename
   if (cfg->getNextTuple("fedDir", &count, values))
      strcpy(fedDir, values[0]);
   else
      fedDir[0] = '\0';

   // Get the base fed filename
   cfg->getNextTuple("baseFedFile", &count, values);
   if (strlen(fedDir) > 0)
      sprintf(fedFilename, "%s/%s", fedDir, values[0]);
   else
      strcpy(fedFilename, values[0]);

   // Create the interface to the RTI
   rti_interface = new atRTIInterface(fedExecName, fedFilename);

   // Initialize the object and interaction counters
   num_classes = 0;
   num_attributes = 0;
   num_interactions = 0;
   num_parameters = 0;

   // Register our interests with the RTI
   registerVars(cfg);
   publishAndSubscribe();

   // Done with the meta file
   delete cfg;

   // Create a list that is used to return name-value pairs
   return_list = new atList();
}


atHLAInterface::~atHLAInterface()
{
   // Get rid of the stuff we created
   if (return_list != NULL)
      delete return_list;
   if (rti_interface != NULL)
      delete rti_interface;
}


int atHLAInterface::read(u_char * buffer, u_long length)
{
   atList *            pairList;
   u_long              bufferIndex;
   atNameValuePair *   pair;
   time_t              timestamp;
   u_long              len;

   // Initialize where we are in the buffer building process
   bufferIndex = 0;

   // Get the new update from the RTI
   pairList = update();

   // See if we got anything
   if (pairList->getNumEntries() > 0)
   {
      // Put a timestamp (as a header field)
      timestamp = time(NULL);
      memcpy(&buffer[bufferIndex], &timestamp, sizeof(timestamp));
      bufferIndex += sizeof(timestamp);

      // Put how many pairs we have in the buffer first (as a header field)
      len = pairList->getNumEntries();
      memcpy(&buffer[bufferIndex], &len, sizeof(len));
      bufferIndex += sizeof(len);

      // Now we need to serialize the set of name-value pairs
      pair = (atNameValuePair *) pairList->getFirstEntry();
      while (pair != NULL)
      {
         // Add the length of name to the buffer
         len = pair->getNameLen();
         memcpy(&buffer[bufferIndex], &len, sizeof(len));
         bufferIndex += sizeof(len);

         // Add the name to the buffer
         memcpy(&buffer[bufferIndex], pair->getName(), pair->getNameLen());
         bufferIndex += pair->getNameLen();

         // Add the length of value to the buffer
         len = pair->getValueLen();
         memcpy(&buffer[bufferIndex], &len, sizeof(len));
         bufferIndex += sizeof(len);

         // Add the value to the buffer
         memcpy(&buffer[bufferIndex], pair->getValue(), pair->getValueLen());
         bufferIndex += pair->getValueLen();

         // Get the next pair
         pair = (atNameValuePair *) pairList->getNextEntry();
      }
   }

   // Return the final length
   return bufferIndex;
}


int atHLAInterface::write(u_char * buffer, u_long length)
{
   // Return that everything was written (for now)
   return (int ) length;
}


void atHLAInterface::registerVars(atConfigFile * cfg)
{
   int               count;
   char *            values[100];
   atClassID         objectClass;
   atInteractionID   interaction;

   // Go through all the object classes in the meta file
   while (cfg->getNextTuple("objectClass", &count, values))
   {
      // Store the name of this object class and register it with the RTI
      // (keeping the RTI ID)
      strcpy(rti_classes[num_classes].name, values[0]);
      rti_classes[num_classes].rtiClass = 
         rti_interface->registerClass(values[0]);

      // Increment the number of classes we have
      num_classes++;
   }

   // Go through all the attributes in the meta file
   while (cfg->getNextTuple("attribute", &count, values))
   {
      // Store the name of this attribute and its object
      strcpy(rti_attributes[num_attributes].name, values[0]);
      strcpy(rti_attributes[num_attributes].className, values[1]);

      // Figure out what the object RTI ID is
      objectClass = lookupRTIObjectClass(values[1]);

      // Register the attribute with the RTI and store the ID
      rti_attributes[num_attributes].rtiAttribute = 
         rti_interface->registerAttribute(values[0], objectClass);

      // Increment the number of attributes we have
      num_attributes++;
   }

   // Go through all the interactions in the meta file
   while (cfg->getNextTuple("interaction", &count, values))
   {
      // Store the name of this interaction and register it with the RTI
      // (keeping the RTI ID)
      strcpy(rti_interactions[num_interactions].name, values[0]);
      rti_interactions[num_interactions].rtiInteraction = 
         rti_interface->registerInteraction(values[0]);

      // Increment the number of classes we have
      num_interactions++;
   }

   // Go through all the parameters in the meta file
   while (cfg->getNextTuple("parameter", &count, values))
   {
      // Store the name of this parameter and its object
      strcpy(rti_parameters[num_parameters].name, values[0]);
      strcpy(rti_parameters[num_parameters].interactionName, values[1]);

      // Figure out what the object RTI ID is
      interaction = lookupRTIInteraction(values[1]);

      // Register the parameter with the RTI and store the ID
      rti_parameters[num_parameters].rtiParameter =
         rti_interface->registerParameter(values[0], interaction);

      // Increment the number of parameters we have
      num_parameters++;
   }
}


void atHLAInterface::publishAndSubscribe()
{
   atAttributeID     attributes[1000];
   atInteractionID   interactions[1000];
   u_long            i;
   u_long            numAttrs;
   u_long            j;

   // Go through each class
   for (i=0; i < num_classes; i++)
   {
      // Collect up the attributes for this class
      numAttrs = 0;
      for (j=0; j < num_attributes; j++)
      {
         // See if this attribute goes with this class
         if (strcmp(rti_classes[i].name, rti_attributes[j].className) == 0)
         {
            // It does so save it in the array
            attributes[numAttrs] = rti_attributes[j].rtiAttribute;
            numAttrs++;
         }
      }

      // Subscribe to the attributes for this class
      rti_interface->subscribeAttributes(rti_classes[i].rtiClass,
                                         numAttrs, attributes);
      notify(AT_INFO, "Subscribed to %d attributes.\n", numAttrs);
   }
   notify(AT_INFO, "Subscribed to %d classes.\n", num_classes);

   // Go through each interaction and set it up in the array
   for (i=0; i < num_interactions; i++)
      interactions[i] = rti_interactions[i].rtiInteraction;

   // Subscribe to the interactions
   rti_interface->subscribeInteractions(num_interactions, interactions);
   notify(AT_INFO, "Subscribed to %d interactions.\n", num_interactions);
}


void atHLAInterface::instance()
{
   // We don't create instances of any class, so this function does nothing 
}


atList * atHLAInterface::update()
{
   atKeyedBufferHandler *   rtiBuffer;
   bool                     terminator;
   bool                     typeAdded;
   u_long                   bufferType;
   char *                   bufferTarget;
   u_long                   bufferClassID;
   u_long                   bufferKey;
   u_char *                 buffer;
   u_long                   bufferSize;
   u_long                   i;

   // Tick the RTI
   rti_interface->tick();

   // Clear the return list
   if (return_list != NULL)
   {
      delete return_list;
      return_list = new atList();
   }

   // Try to read from the RTI interface
   if (rtiBuffer = rti_interface->read())
   {
      // Processing a new message so assume we haven't seen a terminator yet
      typeAdded = false;
      terminator = false;

      while ((rtiBuffer != NULL) && (terminator == false))
      {
         // Unpack the data from the buffer (bufferType is the type of
         // update it is [CONTROL, ATTRIBUTE or INTERACTION], bufferTarget
         // is the object name being updated, bufferKey is the ID of
         // the attribute/interaction being updated, and buffer and 
         // bufferSize represents the data of the update itself)
         rtiBuffer->retrieveBuffer(&bufferType, &bufferTarget, &bufferClassID,
                                   &bufferKey, &buffer, &bufferSize);

         if (bufferType == AT_RTI_CONTROL_TYPE)
         {
            // If we haven't added a type of update yet, do so
            if (typeAdded == false)
            {
               addPair(return_list, "Control", "RemoveObject");
               addPair(return_list, "ObjName", bufferTarget);
               addPair(return_list, "ObjType", 
                       lookupRTIObjectClassID(bufferClassID));
               typeAdded = true;
            }

            // See if we received the terminator yet
            if ((bufferKey == 0) && (buffer == NULL) && (bufferSize == 0))
            {
               // This is the terminator for this message.  (0, NULL, 0)
               // is the special terminator type
               terminator = true;
            }
         }
         else if (bufferType == AT_RTI_ATTRIBUTE_TYPE)
         {
            // See if we received the terminator
            if ((bufferKey == 0) && (buffer == NULL) && (bufferSize == 0))
            {
               // (0, NULL, 0) is the terminator so set the flag to true
               terminator = true;
            }
            else
            {
               // If we haven't added a type of update yet, do so
               if (typeAdded == false)
               {
                  addPair(return_list, "Object", "Update");
                  addPair(return_list, "ObjName", bufferTarget);
                  addPair(return_list, "ObjType", 
                          lookupRTIObjectClassID(bufferClassID));
                  typeAdded = true;
               }

               // Then find our name for this attribute
               for (i=0; i < num_attributes; i++)
               {
                  if (bufferKey == rti_attributes[i].rtiAttribute)
                  {
                     addPair(return_list, rti_attributes[i].name,
                             (char *) buffer, bufferSize);
                  }
               }
            }
         }
         else if (bufferType == AT_RTI_INTERACTION_TYPE)
         {
            // See if we received the terminator
            if ((bufferKey == 0) && (buffer == NULL) && (bufferSize == 0))
            {
               // (0, NULL, 0) is the terminator so set the flag to true
               terminator = true;
            }
            else
            {
               // If we haven't added a type of update yet, do so
               if (typeAdded == false)
               {
                  addPair(return_list, "Interaction", "Update");
                  addPair(return_list, "IntName", bufferTarget);
                  typeAdded = true;
               }

               // Then find our name for this parameter
               for (i=0; i < num_parameters; i++)
               {
                  if (bufferKey == rti_parameters[i].rtiParameter)
                  {
                     addPair(return_list, rti_parameters[i].name,
                             (char *) buffer, bufferSize);
                  }
               }
            }
         }

         // Free the buffer and bufferTarget
         if (buffer != NULL)
            free(buffer);
         if (bufferTarget != NULL)
            free(bufferTarget);

         // We're done with the buffer so get rid of it
         delete rtiBuffer;

         // Read the next parameter, if we didn't hit the terminator
         if (terminator == false)
            rtiBuffer = rti_interface->read();
      }
   }

   // Return the list of things we built up
   return return_list;
}


atClassID atHLAInterface::lookupRTIObjectClass(char * name)
{
   u_long   i;

   // Go through the known classes and see if we match the name
   for (i=0; i < num_classes; i++)
   {
      // See if it matches and then return the RTI Class if it does
      if (strcmp(name, rti_classes[i].name) == 0)
         return rti_classes[i].rtiClass;
   }

   // Return a sentinel that we didn't find the class
   return -1;
}


char * atHLAInterface::lookupRTIObjectClassID(atClassID id)
{
   u_long   i;

   // Go through the known classes and see if we match the name
   for (i=0; i < num_classes; i++)
   {
      // See if it matches and then return the RTI Class if it does
      if (id == rti_classes[i].rtiClass)
         return rti_classes[i].name;
   }

   // Return a sentinel that we didn't find the class
   return NULL;
}


atInteractionID atHLAInterface::lookupRTIInteraction(char * name)
{
   u_long   i;

   // Go through the known interactions and see if we match the name
   for (i=0; i < num_interactions; i++)
   {
      // See if it matches and then return the RTI Interaction if it does
      if (strcmp(name, rti_interactions[i].name) == 0)
         return rti_interactions[i].rtiInteraction;
   }

   // Return a sentinel that we didn't find the interaction
   return -1;
}


void atHLAInterface::addPair(atList * list, char * name, char * value)
{
   // Add a name-value pair to the given list
   list->addEntry(new atNameValuePair(name, strlen(name),
                                      value, strlen(value)));
}


void atHLAInterface::addPair(atList * list, char * name,
                             char * value, int valueSize)
{
   // Add a name-value pair to the given list
   list->addEntry(new atNameValuePair(name, strlen(name),
                                      value, valueSize));
}

