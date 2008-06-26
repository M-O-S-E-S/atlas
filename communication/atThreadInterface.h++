
#ifndef AT_THREAD_INTERFACE_H
#define AT_THREAD_INTERFACE_H


// INCLUDES
#include <sys/types.h>
#include "atIPCInterface.h++"
#include "atOSDefs.h"
#include "atThreadQueue.h++"


class ATLAS_SYM atThreadInterface : public atIPCInterface
{
   protected:
      atThreadQueue *   read_queue;
      atThreadQueue *   write_queue;

   public:
      atThreadInterface();
      virtual ~atThreadInterface();

      int   read(u_char * buffer, u_long length);
      int   write(u_char * buffer, u_long length);
};

#endif

