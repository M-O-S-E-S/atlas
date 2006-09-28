
#include <string.h>
#include "atXMLDocument.h++"


atXMLDocument::atXMLDocument(atXMLDocumentDocPtr doc)
{
   // Save a pointer to the XML document
   xml_doc = (xmlDocPtr ) doc;

   // Initialize the xml text pointer
   xml_text = NULL;
}


atXMLDocument::atXMLDocument(atXMLDocument * doc)
{
   // Save a pointer to the XML document of the document we are copying
   xml_doc = xmlCopyDoc(doc->xml_doc, 1);

   // Initialize the xml text pointer
   xml_text = NULL;
}


atXMLDocument::~atXMLDocument()
{
   // Free up the text buffer 
   if (xml_text != NULL)
      xmlFree(xml_text);

   // Free the XML document itself
   xmlFreeDoc(xml_doc);
}


/* GAM These two methods are GEMINI specific, need to be reworked */
char * atXMLDocument::getClass()
{
   xmlChar *   xmlClass;

   // Get the "class" property
   xmlClass = xmlGetProp(xmlDocGetRootElement(xml_doc), (xmlChar *) "class");

   // Copy the XML "class" into our longer-term string
   strcpy(current_xml_class, (char *) xmlClass);

   // Toss the XML property now
   xmlFree((xmlChar *) xmlClass);

   // Return to the user
   return current_xml_class;
}


u_long atXMLDocument::getFamily()
{
   char *   family;
   u_long   familyType;

   // Get the "family" property
   family = (char *) xmlGetProp(xmlDocGetRootElement(xml_doc), 
                                (xmlChar *) "family");

   // Get a family "type" based on the property
   if (strcasecmp(family, "control") == 0)
       familyType = AT_XML_CONTROL_FAMILY;
   else if (strcasecmp(family, "request") == 0)
       familyType = AT_XML_REQUEST_FAMILY;
   else if (strcasecmp(family, "response") == 0)
       familyType = AT_XML_RESPONSE_FAMILY;
   else if (strcasecmp(family, "command") == 0)
       familyType = AT_XML_COMMAND_FAMILY;
   else
       familyType = AT_XML_UNKNOWN_FAMILY;

   // Free up the property we queried
   xmlFree((xmlChar *) family);

   // Return the family
   return familyType;
}


atXMLDocumentDocPtr atXMLDocument::getDoc()
{
   // Return the base xml doc
   return xml_doc;
}


atXMLDocumentNodePtr atXMLDocument::getRootNode()
{
   // Get the root node pointer and return it
   return (atXMLDocumentNodePtr ) xmlDocGetRootElement(xml_doc);
}


atXMLDocumentNodePtr atXMLDocument::getNextSiblingNode(
   atXMLDocumentNodePtr node)
{
   xmlNodePtr   xmlNode;

   // Get the next sibling node of this node and return it
   xmlNode = (xmlNodePtr ) node;
   return (atXMLDocumentNodePtr ) node->next;
}


atXMLDocumentNodePtr atXMLDocument::getNextChildNode(atXMLDocumentNodePtr node)
{
   xmlNodePtr   xmlNode;
                                                                                
   // Get the child node of this node and return it
   xmlNode = (xmlNodePtr ) node;
   return (atXMLDocumentNodePtr ) node->xmlChildrenNode;
}


char * atXMLDocument::getNodeName(atXMLDocumentNodePtr node)
{
   return (char * ) ((xmlNodePtr ) node)->name;
}


char * atXMLDocument::getNodeAttribute(atXMLDocumentNodePtr node, 
                                       char *attrName)
{
   char *   prop;

   // Get the property
   prop = (char *) xmlGetProp(node, (const xmlChar *) attrName);

   // Copy the property value into our internal string
   if (prop != NULL)
      strcpy(string_value, prop);
   else
      strcpy(string_value, "");

   // Free up the XML structure
   xmlFree((xmlChar *) prop);

   // Return the string
   return string_value;
}


char * atXMLDocument::getNodeText(atXMLDocumentNodePtr node)
{
   char *   str;

   // Get the string
   str = (char *) xmlNodeListGetString(xml_doc, node, 1);

   // Copy the string value (if there is one) into our internal string
   if (str != NULL)
      strcpy(string_value, str);
   else
      string_value[0] = '\0';

   // Free up the XML structure
   xmlFree((xmlChar *) str);

   // Return the string
   return string_value;
}


void atXMLDocument::retrieveXML(u_char ** buffer, u_long * bufferLen)
{
   // If we previously called this method (xml_text is not NULL), then
   // we need to free it first
   if (xml_text != NULL)
      xmlFree(xml_text);

   // Now get the XML tree as text and save it in the internal text buffer
   xmlDocDumpFormatMemory(xml_doc, &xml_text, (int *) bufferLen, 1);

   // Then point the user's pointer to this buffer
   *buffer = (u_char *) xml_text;
}

