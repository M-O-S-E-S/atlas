
#ifndef AT_KEYED_BUFFER_HANDLER_H
#define AT_KEYED_BUFFER_HANDLER_H


#include <sys/types.h>
#include "atItem.h++"


class atKeyedBufferHandler : public atItem
{
   protected:
      u_long     buffer_type;
      char *     buffer_target;
      u_long     buffer_class;
      u_long     buffer_key;
      u_char *   buffer_text;
      u_long     buffer_len;

   public:
      atKeyedBufferHandler(u_long bufferType, 
                           char * targetID, u_long classID,
                           u_long bufferKey, u_char * buffer, u_long bufferLen);
      virtual ~atKeyedBufferHandler();

      void retrieveBuffer(u_long * bufferType, 
                          char ** targetID, u_long * classID,
                          u_long * bufferKey, u_char ** buffer, 
                          u_long * bufferLen);
      void setBuffer(u_long bufferType, char * targetID, u_long classID,
                     u_long bufferKey, u_char * buffer, u_long bufferLen);

      void print();
};


#endif

