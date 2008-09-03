
#include <string.h>
#include "atXMLBuffer.h++"


atXMLBuffer::atXMLBuffer(char * xmlName)
{
   // Save the header and footer for this overall XML document
   sprintf(xml_header, "<%s>", xmlName);
   sprintf(xml_footer, "</%s>", xmlName);

   // Initialize the internal XML buffer length
   xml_buffer_size = 0;

   // Initialize the xml DTD (we aren't using one in this case and also
   // don't need a context then)
   xml_dtd = NULL;

   // Initialize the documents buffer (we will collect any documents
   // from the XML elements into this list and send them back)
   xml_document_list = new atList();
}


atXMLBuffer::atXMLBuffer(char * xmlName, char * dtdFilename)
{
   // Save the header and footer for this overall XML document
   sprintf((char *) xml_header, "<%s>", xmlName);
   sprintf((char *) xml_footer, "</%s>", xmlName);

   // Initialize the internal XML buffer length
   xml_buffer_size = 0;

   // Open and initialize the DTD file (used for validating XML)
   xml_dtd = xmlParseDTD(NULL, (const xmlChar *) dtdFilename);

   // Initialize the XML context information (used during XML validation
   // to print warnings and errors)
   xml_context.userData = stderr;
   xml_context.error = (xmlValidityErrorFunc ) fprintf;
   xml_context.warning = (xmlValidityWarningFunc ) fprintf;

   // Initialize the documents buffer (we will collect any documents
   // from the XML elements into this list and send them back)
   xml_document_list = new atList();
}


atXMLBuffer::~atXMLBuffer()
{
   // Free up the DTD if we created it
   if (xml_dtd != NULL)
      xmlFreeDtd(xml_dtd);

   // Free up XML document list
   if (xml_document_list != NULL)
      delete xml_document_list;
}


void atXMLBuffer::processXMLDocument()
{
   xmlDocPtr         doc;
   xmlNodePtr        current;
   char              fullHeader[256];
   xmlChar *         version;
   atXMLDocument *   xmlDoc;

   // Open the memory buffer as an XML document
   doc = xmlParseMemory((const char *) xml_buffer, xml_buffer_size);

   // Check to make sure the XML library understood the buffer
   if (doc == NULL)
   {
      // We didn't recognize the XML at all so warn the user and bail out
      notify(AT_WARN, "XML document not parsed successfully.\n");
      return;
   }

   // Check to see if we're checking the XML against the DTD
   if (xml_dtd != NULL)
   {
      // We are so go ahead and validate the XML against the DTD
      if (xmlValidateDtd(&xml_context, doc, xml_dtd) == 0)
      {
         // The document failed the DTD check so warn the user and bail out
         notify(AT_WARN, "XML document not valid.\n");
         xmlFreeDoc(doc);
         return;
      }
   }

   // Ok, now let's get the XML root element (all non-empty XML documents
   // have a root)
   current = xmlDocGetRootElement(doc);

   // Check to see if we got something in the root (i.e. it's not an 
   // empty document)
   if (current == NULL)
   {
      notify(AT_WARN, "XML document is empty.\n");
      xmlFreeDoc(doc);
      return;
   }

   // Now, check to make sure the root element is the header
   // (all XML documents start with the header and end with the footer
   sprintf(fullHeader, "<%s>", current->name);
   if (xmlStrcmp((xmlChar *) fullHeader, (xmlChar *) xml_header))
   {
      notify(AT_WARN, "XML document is of wrong type.\n");
      xmlFreeDoc(doc);
      return;
   }

   // Get the version of XML we're using from the header
   // (later on, we'll assume that NULL means 1.0)
   version = xmlGetProp(current, (const xmlChar *) "version");

   // The document is "certified" so add the document to the document list
   xmlDoc = new atXMLDocument((atXMLDocumentDocPtr ) doc);
   xml_document_list->addEntry(xmlDoc);

   // We're done so free the stuff we're required to free ourselves.  We
   // added the document itself to the list so we'll assume it's deallocated
   // later.
   xmlFree(version);
}


atList * atXMLBuffer::processBuffer(atBufferHandler * packetBuffer)
{
   u_char *   buffer;
   u_long     lengthRead;
   u_char *   nextNull;
   u_char *   newBuffer;
   atItem *   item;
   u_char *   endTag;
   int        partialChunkSize;
   u_long     numWhiteSpace;

   // Get the info from this buffer
   packetBuffer->retrieveBuffer(&buffer, &lengthRead);

   // Go through and eradicate NULL characters (the XML library will
   // consider it the end of the document)
   nextNull = (u_char *) memchr(buffer, '\0', lengthRead);
   while (nextNull != NULL)
   {
      // Slide the buffer up one to get rid of this NULL
      memmove(nextNull, nextNull+1, lengthRead - (nextNull - buffer));
      lengthRead--;
 
      // Find the next NULL
      nextNull = (u_char *) memchr(buffer, '\0', lengthRead);
   }

   // Allocate a buffer to hold this buffer and left over from the last pass
   newBuffer = (u_char *) calloc(xml_buffer_size+lengthRead+1, sizeof(u_char));
   if (newBuffer == NULL)
   {
      // We failed to allocate space (bad!) so assume we have nothing
      notify(AT_WARN, "Failed to allocate space for combining buffers.\n");
      lengthRead = 0;
   }
   else
   {
      // Stick any partial buffer that was saved last go around in front
      // of the new buffer (this is necessary to cover the case when the
      // end tag footer gets split between packets)
      memcpy(newBuffer, xml_buffer, xml_buffer_size);
      memcpy(&newBuffer[xml_buffer_size], buffer, lengthRead);
      lengthRead += xml_buffer_size;
      xml_buffer_size = 0;
   }

   // Make a string out of the buffer
   newBuffer[lengthRead] = '\0';

   // We will build up each recognized XML document and add it to a list
   // to return to the user so we need to clear it first (so go through
   // each list item in the list now and remove it)
   item = xml_document_list->getFirstEntry();
   while (item != NULL)
   {
      // Remove this list item
      xml_document_list->removeCurrentEntry();

      // Delete this list item to free up the memory since we're done
      // with it
      delete item;

      // Get the first node again (if there is one)
      item = xml_document_list->getFirstEntry();
   }

   // Check if we see the ending footer
   endTag = (u_char *) strstr((const char *) newBuffer, xml_footer);
   if (endTag != NULL)
   {
      // We need to loop in case there are multiple documents within 
      // the buffer
      while (endTag != NULL)
      {
         // Copy through the ending footer to the internal
         // collected xml buffer
         partialChunkSize = (int ) (endTag - &newBuffer[0]) + 
                            (int ) strlen(xml_footer);
         memcpy(&xml_buffer[xml_buffer_size], newBuffer, partialChunkSize);
         xml_buffer_size += partialChunkSize;

         // Delete the part we just used (+1 for the NULL we tacked on)
         memmove(&newBuffer[0], &newBuffer[partialChunkSize], 
                 lengthRead - partialChunkSize + 1);
         lengthRead -= partialChunkSize;

         // Eliminate any leading white space lines left in the buffer
         numWhiteSpace = (u_long ) strspn((const char *) newBuffer, " \r\n\t");
         if ( (numWhiteSpace > 0) && (numWhiteSpace <= lengthRead) )
         {
            // Move up the remaining part of the buffer (+1 for the NULL
            // we tacked on)
            memmove(&newBuffer[0], &newBuffer[numWhiteSpace],
                    (lengthRead - numWhiteSpace + 1) * sizeof(u_char));
            lengthRead -= numWhiteSpace;
         }

         // Process the document
         processXMLDocument();

         // Clear the internal xml buffer
         xml_buffer_size = 0;

         // See if there is another footer (in case there are two
         // or more within the buffer)
         endTag = (u_char *) strstr((const char *) newBuffer, xml_footer);
      }

      // Store remaining chunk (if any) in internal collected xml buffer
      if (lengthRead > 0)
      {
         // Copy remaining chunk into xml buffer and initialize
         // size to size of that remaining chunk
         memcpy(&xml_buffer[0], &newBuffer[0], lengthRead);
         xml_buffer_size = lengthRead;
      }
      else
      {
         // Nothing left in buffer so just initialize xml buffer
         xml_buffer[0] = '\0';
         xml_buffer_size = 0;
      }
   }
   else
   {
      // No ending found so just add what we received to the
      // internal collected xml buffer
      memcpy(&xml_buffer[xml_buffer_size], newBuffer, lengthRead);
      xml_buffer_size += lengthRead;
   }

   // Free the temporary buffer
   if (newBuffer != NULL)
      free(newBuffer);

   // Return the list of documents that were collected
   return xml_document_list;
}

