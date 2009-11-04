
#ifndef AT_XML_BUFFER_HPP
#define AT_XML_BUFFER_HPP

#include <sys/types.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "atNotifier.h++"
#include "atList.h++"
#include "atOSDefs.h++"

#include "atXMLDocument.h++"
#include "atBufferHandler.h++"

#include "atString.h++"


#define INITIAL_XML_DOCUMENT_SIZE    2000000
#define MAX_XML_TAG_SIZE         256


class ATLAS_SYM atXMLBuffer : public atNotifier
{
   protected:
      u_char *       xml_buffer;
      u_long         xml_buffer_len;
      u_long         xml_buffer_size;

      char           xml_header[MAX_XML_TAG_SIZE];
      char           xml_footer[MAX_XML_TAG_SIZE];

      xmlDtdPtr      xml_dtd;
      xmlValidCtxt   xml_context;

      atList *       xml_document_list;

      float          xmlToFloat(xmlChar * tmpStr);
      int            xmlToInt(xmlChar * tmpStr);

      void           checkBufferSize(u_long sizeNeeded);
      void           processXMLDocument();

   public:
      atXMLBuffer(char * xmlName);
      atXMLBuffer(char * xmlName, char * dtdFilename);
      virtual ~atXMLBuffer();

      atList *   processBuffer(atBufferHandler * packetBuffer);
};

#endif

