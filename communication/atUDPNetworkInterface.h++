
#ifndef AT_UDP_NETWORK_INTERFACE_H
#define AT_UDP_NETWORK_INTERFACE_H


// INCLUDES
#include "atNetworkInterface.h++"


class atUDPNetworkInterface : public atNetworkInterface
{
   public:
      atUDPNetworkInterface(char * address, short port);
      atUDPNetworkInterface(short port);
      virtual ~atUDPNetworkInterface();

      int   read(u_char * buffer, u_long len);
      int   write(u_char * buffer, u_long len);
};

#endif

