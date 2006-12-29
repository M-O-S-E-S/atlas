
#ifndef AT_TCP_NETWORK_INTERFACE_H
#define AT_TCP_NETWORK_INTERFACE_H


// INCLUDES
#include "atNetworkInterface.h++"


class atTCPNetworkInterface : public atNetworkInterface
{
   protected:
      int   client_sockets[20];
      int   num_client_sockets;

   public:
      atTCPNetworkInterface(char * address, short port);
      atTCPNetworkInterface(short port);
      virtual ~atTCPNetworkInterface();

      void   allowConnections(int backlog);
      int    acceptConnection();
      void   enableBlockingOnClient(int clientID);
      void   disableBlockingOnClient(int clientID);

      int    makeConnection();

      int    read(u_char * buffer, u_long len);
      int    read(int clientID, u_char * buffer, u_long len);
      int    write(u_char * buffer, u_long len);
      int    write(int clientID, u_char * buffer, u_long len);
};

#endif

