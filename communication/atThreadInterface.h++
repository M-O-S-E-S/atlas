
#ifndef AT_THREAD_INTERFACE_H
#define AT_THREAD_INTERFACE_H


// INCLUDES
#include <sys/types.h>
#include "atIPCInterface.h++"
#include "atOSDefs.h++"
#include "atThreadQueue.h++"
#include "atThreadCount.h++"


class ATLAS_SYM atThreadInterface : public atIPCInterface
{
   protected:
      SemKey            read_key;
      SemKey            write_key;

      atThreadQueue *   read_queue;
      atThreadQueue *   write_queue;

      atThreadCount *   read_count;
      atThreadCount *   write_count;

   public:
      atThreadInterface(SemKey readKey, SemKey writeKey);
      virtual ~atThreadInterface();

      int   read(u_char * buffer, u_long length);
      int   write(u_char * buffer, u_long length);
};

#endif

