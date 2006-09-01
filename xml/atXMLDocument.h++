
#ifndef AT_XML_DOCUMENT_HPP
#define AT_XML_DOCUMENT_HPP


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "atItem.h++"


// CONSTANTS
#define AT_XML_UNKNOWN_FAMILY    0
#define AT_XML_CONTROL_FAMILY    1
#define AT_XML_REQUEST_FAMILY    2
#define AT_XML_RESPONSE_FAMILY   3
#define AT_XML_COMMAND_FAMILY    4


// TYPES
// These types help encapsulate the particular XML library we are using
// (theoretically, we could switch from libxml2 to something else by just
// reimplementing this class and these types)
typedef xmlDocPtr    atXMLDocumentDocPtr;
typedef xmlNodePtr   atXMLDocumentNodePtr;


class atXMLDocument : public atItem
{
   protected:
      xmlDocPtr   xml_doc;
      xmlChar *   xml_text;

      char        string_value[1024];

      char        current_xml_class[256];

   public:
      atXMLDocument(atXMLDocumentDocPtr doc);
      atXMLDocument(atXMLDocument * doc);
      virtual ~atXMLDocument();

      char *                 getClass();
      u_long                 getFamily();

      atXMLDocumentNodePtr   getRootNode();
      atXMLDocumentNodePtr   getNextSiblingNode(atXMLDocumentNodePtr node);
      atXMLDocumentNodePtr   getNextChildNode(atXMLDocumentNodePtr node);

      char *                 getNodeName(atXMLDocumentNodePtr node);
      char *                 getNodeAttribute(atXMLDocumentNodePtr node,
                                              char *attrName);
      char *                 getNodeText(atXMLDocumentNodePtr node);

      void                   retrieveXML(u_char ** buffer, u_long * bufferLen);
};

#endif

