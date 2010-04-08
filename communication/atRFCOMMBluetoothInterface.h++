
#ifndef AT_RFCOMM_BLUETOOTH_INTERFACE_H
#define AT_RFCOMM_BLUETOOTH_INTERFACE_H


// INCLUDES
#include "atBluetoothInterface.h++"
#include "atOSDefs.h++"


// TYPES
typedef struct
{
   char     address[256];
   u_char   channel;
} BTClientAddr;


class ATLAS_SYM atRFCOMMBluetoothInterface : public atBluetoothInterface
{
   protected:
      BluetoothSockAddr    read_name;
      socklen_t            read_name_length;
      BluetoothSockAddr    write_name;
      socklen_t            write_name_length;

      Socket         client_sockets[256];
      BTClientAddr   client_addrs[256];
      bool           client_blocking[256];
      u_long         num_client_sockets;

   public:
      atRFCOMMBluetoothInterface(char * address, u_char channel);
      virtual ~atRFCOMMBluetoothInterface();

      void           allowConnections(int backlog);
      int            acceptConnection();
      void           enableBlockingOnClient(int clientID);
      void           disableBlockingOnClient(int clientID);

      BTClientAddr   getClientInfo(int clientID);

      int            makeConnection();

      int            read(u_char * buffer, u_long len);
      int            read(int clientID, u_char * buffer, u_long len);
      int            write(u_char * buffer, u_long len);
      int            write(int clientID, u_char * buffer, u_long len);
};


#endif

