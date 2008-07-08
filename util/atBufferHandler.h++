
#ifndef AT_BUFFER_HANDLER_H
#define AT_BUFFER_HANDLER_H


#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atBufferHandler : public atItem
{
   protected:
      u_char *   buffer_text;
      u_long     buffer_len;

   public:
      atBufferHandler(u_char * buffer, u_long bufferLen);
      virtual ~atBufferHandler();

      void retrieveBuffer(u_char ** buffer, u_long * bufferLen);
      void setBuffer(u_char * buffer, u_long bufferLen);

      void print();
};


#endif

