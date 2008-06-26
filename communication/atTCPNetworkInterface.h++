
#ifndef AT_TCP_NETWORK_INTERFACE_H
#define AT_TCP_NETWORK_INTERFACE_H


// INCLUDES
#include "atNetworkInterface.h++"
#include "atOSDefs.h"


// TYPES
typedef struct
{
   char     address[256];
   u_long   port;
} ClientAddr;


class ATLAS_SYM atTCPNetworkInterface : public atNetworkInterface
{
   protected:
      int          client_sockets[256];
      ClientAddr   client_addrs[256];
      int          num_client_sockets;

   public:
      atTCPNetworkInterface(char * address, short port);
      atTCPNetworkInterface(short port);
      virtual ~atTCPNetworkInterface();

      void         allowConnections(int backlog);
      int          acceptConnection();
      void         enableBlockingOnClient(int clientID);
      void         disableBlockingOnClient(int clientID);
      ClientAddr   getClientInfo(int clientID);

      int          makeConnection();

      int          read(u_char * buffer, u_long len);
      int          read(int clientID, u_char * buffer, u_long len);
      int          write(u_char * buffer, u_long len);
      int          write(int clientID, u_char * buffer, u_long len);
};

#endif

