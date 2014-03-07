
#include <string.h>
#include "atXMLDocument.h++"


atXMLDocument::atXMLDocument(atXMLDocumentDocPtr doc)
{
   // Save a pointer to the XML document
   xml_doc = (xmlDocPtr ) doc;

   // Initialize the value for holding temp strings
   string_value = new atString();

   // Initialize the xml text pointer
   xml_text = NULL;
}


atXMLDocument::atXMLDocument(atXMLDocument * doc)
{
   // Save a pointer to the XML document of the document we are copying
   xml_doc = xmlCopyDoc(doc->xml_doc, 1);

   // Initialize the value for holding temp strings
   string_value = new atString();

   // Initialize the xml text pointer
   xml_text = NULL;
}


atXMLDocument::~atXMLDocument()
{
   // Free up the text buffer 
   if (xml_text != NULL)
      xmlFree(xml_text);

   // Free up the temp string
   if (string_value != NULL)
      delete string_value;

   // Free the XML document itself
   xmlFreeDoc(xml_doc);
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
      string_value->setString(prop);
   else
      string_value->setString("");

   // Free up the XML structure
   if (prop != NULL)
      xmlFree((xmlChar *) prop);

   // Return the string
   return string_value->getString();
}


// This method should be considered deprecated and any commit should use 
// the method that also takes in a value. When all projects are up to date
// this should be removed
char * atXMLDocument::getNodeText(atXMLDocumentNodePtr node)
{
   char *   str;

   // Get the string from the XML node
   str = (char *) xmlNodeListGetString(xml_doc, node, 0);

   // Copy the string value (if there is one) into our internal string
   if (str != NULL)
      string_value->setString(str);
   else
      string_value->setString("");

   // Free up the XML structure
   if (str != NULL)
      xmlFree((xmlChar *) str);

   // Return the string
   return string_value->getString();
}


char * atXMLDocument::getNodeText(atXMLDocumentNodePtr node, bool val)
{
   char *   str;

   // Get the string from the xml node use 1 or 0 for val
   // Note: If val is a 1 this will also convert entity references for 
   // example &amp; would become & (Using 1 will also keep CDATA text from
   // changing)
   str = (char *) xmlNodeListGetString(xml_doc, node, val);

   // Copy the string value (if there is one) into our internal string
   if (str != NULL)
      string_value->setString(str);
   else
      string_value->setString("");

   // Free up the XML structure
   if (str != NULL)
      xmlFree((xmlChar *) str);

   // Return the string
   return string_value->getString();
}


void atXMLDocument::retrieveXML(u_char ** buffer, u_long * bufferLen)
{
   int   len;

   // If we previously called this method (xml_text is not NULL), then
   // we need to free it first
   if (xml_text != NULL)
      xmlFree(xml_text);

   // Now get the XML tree as text and save it in the internal text buffer
   xmlDocDumpFormatMemory(xml_doc, &xml_text, &len, 1);

   // Then point the user's pointer to this buffer
   *buffer = (u_char *) xml_text;
   *bufferLen = (u_long ) len;
}


void atXMLDocument::print()
{
   u_char *   buffer;
   u_long     bufferLen;

   // Get the XML
   retrieveXML(&buffer, &bufferLen);

   // Then print it
   buffer[bufferLen] = '\0';
   printf("%s\n", buffer);
}

