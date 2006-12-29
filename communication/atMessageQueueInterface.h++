
#ifndef AT_MESSAGE_QUEUE_INTERFACE_H
#define AT_MESSAGE_QUEUE_INTERFACE_H


// INCLUDES
#include <sys/types.h>
#include "atIPCInterface.h++"


class atMessageQueueInterface : public atIPCInterface
{
   protected:
      key_t   msg_key;
      int     msg_id;

      bool    want_blocking;


   public:
      atMessageQueueInterface(key_t key);
      virtual ~atMessageQueueInterface();

      void   enableBlocking();
      void   disableBlocking();

      int    read(u_char * buffer, u_long length);
      int    write(u_char * buffer, u_long length);
};

#endif

