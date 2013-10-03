
#ifndef AT_UDP_NETWORK_INTERFACE_H
#define AT_UDP_NETWORK_INTERFACE_H


// INCLUDES
#include "atString.h++"
#include "atNetworkInterface.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atUDPNetworkInterface : public atNetworkInterface
{
   protected:
      bool   ignore_our_own;

   public:
      atUDPNetworkInterface(char * readAddress, char * writeAddress, 
                            short port);
      atUDPNetworkInterface(char * writeAddress, short port);
      atUDPNetworkInterface(short port);
      atUDPNetworkInterface(char * address, short srcPort, short dstPort);
      virtual ~atUDPNetworkInterface();

      int   read(u_char * buffer, u_long len);
      int   read(u_char * buffer, u_long len, atString * senderAddr);
      int   write(u_char * buffer, u_long len);
};

#endif

