
#ifndef AT_IPC_INTERFACE_H
#define AT_IPC_INTERFACE_H


// INCLUDES
#include <sys/types.h>
#include "atItem.h++"


class atIPCInterface : public atItem
{
   public:
      atIPCInterface();
      virtual ~atIPCInterface();

      virtual int   read(u_char * buffer, u_long length) = 0;
      virtual int   write(u_char * buffer, u_long length) = 0;
};

#endif

