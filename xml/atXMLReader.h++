
#ifndef AT_XML_READER_H
#define AT_XML_READER_H


#include <libxml/xmlreader.h>
#include "atNotifier.h++"
#include "atOSDefs.h++"
#include "atXMLDocument.h++"


// TYPES
// These types help encapsulate the particular XML library we are using
// (theoretically, we could switch from libxml2 to something else by just
// reimplementing this class and these types)
typedef xmlTextReaderPtr    atXMLTextReaderPtr;


class ATLAS_SYM atXMLReader : public atNotifier
{
   protected:
      atXMLTextReaderPtr   xml_reader;

   public:
      atXMLReader(char * filename);
      atXMLReader(int fileDescriptor);
      atXMLReader(char * memory, u_long memorySize);
      atXMLReader(atXMLDocument * doc);

      virtual ~atXMLReader();

      atXMLDocument *   getSubDocument(char * elementName);
};


#endif

