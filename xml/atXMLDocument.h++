
#ifndef AT_XML_DOCUMENT_HPP
#define AT_XML_DOCUMENT_HPP


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "atItem.h++"
#include "atOSDefs.h++"
#include "atString.h++"


// TYPES
// These types help encapsulate the particular XML library we are using
// (theoretically, we could switch from libxml2 to something else by just
// reimplementing this class and these types)
typedef xmlDocPtr    atXMLDocumentDocPtr;
typedef xmlNodePtr   atXMLDocumentNodePtr;


class ATLAS_SYM atXMLDocument : public atItem
{
   protected:
      xmlDocPtr    xml_doc;
      xmlChar *    xml_text;

      atString *   string_value;

      char         current_xml_class[256];

   public:
      atXMLDocument(atXMLDocumentDocPtr doc);
      atXMLDocument(atXMLDocument * doc);
      virtual ~atXMLDocument();

      atXMLDocumentDocPtr    getDoc();

      atXMLDocumentNodePtr   getRootNode();
      atXMLDocumentNodePtr   getNextSiblingNode(atXMLDocumentNodePtr node);
      atXMLDocumentNodePtr   getNextChildNode(atXMLDocumentNodePtr node);

      char *                 getNodeName(atXMLDocumentNodePtr node);
      char *                 getNodeAttribute(atXMLDocumentNodePtr node,
                                              char *attrName);
      char *                 getNodeText(atXMLDocumentNodePtr node);
      char *                 getNodeText(atXMLDocumentNodePtr node, bool val);

      void                   retrieveXML(u_char ** buffer, u_long * bufferLen);

      void                   print();
};


#endif

