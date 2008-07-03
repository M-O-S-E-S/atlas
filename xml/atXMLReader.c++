
#include <string.h>
#include "atXMLReader.h++"


atXMLReader::atXMLReader(char * filename)
{
   // Open an XML file for reading using an XML reader
   xml_reader = xmlReaderForFile(filename, NULL, 0);

   // Check to make sure it worked okay
   if (xml_reader == NULL)
      notify(AT_WARN, "XML Reader cannot process file (%s).\n", filename);
}


atXMLReader::atXMLReader(int fileDescriptor)
{
   // Open an XML file for reading using an XML reader
   xml_reader = xmlReaderForFd(fileDescriptor, NULL, NULL, 0);

   // Check to make sure it worked okay
   if (xml_reader == NULL)
      notify(AT_WARN, "XML Reader cannot process file.\n");
}


atXMLReader::atXMLReader(char * memory, u_long memorySize)
{
   // Open an XML file for reading using an XML reader
   xml_reader = xmlReaderForMemory(memory, memorySize, NULL, NULL, 0);

   // Check to make sure it worked okay
   if (xml_reader == NULL)
      notify(AT_WARN, "XML Reader cannot process memory.\n");
}


atXMLReader::atXMLReader(atXMLDocument * doc)
{
   // Open an XML file for reading using an XML reader
   if (doc != NULL)
      xml_reader = xmlReaderWalker(doc->getDoc());
   else
      xml_reader = NULL;

   // Check to make sure it worked okay
   if (xml_reader == NULL)
      notify(AT_WARN, "XML Reader cannot process document.\n");
}


atXMLReader::~atXMLReader()
{
   // Close the reader
   if (xml_reader != NULL)
      xmlFreeTextReader(xml_reader);
}


atXMLDocument * atXMLReader::getSubDocument(char * elementName)
{
   int               ret;
   xmlChar *         name;
   xmlNodePtr        node; 
   xmlNodePtr        newNode;
   xmlDocPtr         doc;
   atXMLDocument *   xmlDoc;

   // Read a node from the xml reader
   ret = xmlTextReaderRead(xml_reader);

   // Loop through the nodes looking for the name requested
   xmlDoc = NULL;
   while ( (ret == 1) && (xmlDoc == NULL) )
   {
      // Get the name at the node we're at
      name = xmlTextReaderName(xml_reader);

      // See if it matches the one we're looking for and is the opening
      // node for that subdocument
      if ( (xmlStrcmp(name, (xmlChar *) elementName) == 0) &&
           (xmlTextReaderNodeType(xml_reader) == 1) )
      {
         // Tell the reader to expand this subtree since we
         // recognize the name
         node = xmlTextReaderExpand(xml_reader);

         // Get a copy of this subtree for us (we'll leave the
         // reader's copy alone)
         newNode = xmlCopyNode(node, 1);

         // Create an XML document with this subtree
         doc = xmlNewDoc((xmlChar *) "1.0");
         xmlDocSetRootElement(doc, newNode);

         // Create an XMLDocument instance with this XML doc
         xmlDoc = new atXMLDocument(doc);

         // Since we processed this subtree we need to tell the
         // reader to skip it
         xmlTextReaderNext(xml_reader);
      }
      else
      {
         // Read the next node from the xml reader
         ret = xmlTextReaderRead(xml_reader);
      }

      // Either way, clean up the name
      if (name != NULL)
         xmlFree(name);
   }

   // Return any XML doc we found
   return xmlDoc;
}

