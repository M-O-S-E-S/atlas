
#ifndef AT_NETWORK_INTERFACE_H
#define AT_NETWORK_INTERFACE_H


// INCLUDES
#include "atIPCInterface.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atNetworkInterface : public atIPCInterface
{
   protected:
      Socket               socket_value;

      bool                 blocking_mode;

      struct sockaddr_in   read_name;
      socklen_t            read_name_length;
      struct sockaddr_in   write_name;
      socklen_t            write_name_length;


   public:
      atNetworkInterface();
      virtual ~atNetworkInterface();

      virtual void   enableBlocking();
      virtual void   disableBlocking();
};


#endif

