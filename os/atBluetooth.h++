
#ifndef AT_BLUETOOTH_H
#define AT_BLUETOOTH_H


// Because bluetooth implementations build it on top of the socket
// libraries, we can't doubly define here (or we get linking errors)
// so we will reference the networking stuff (from atNetwork.c++) here 
// and then only define Bluetooth-specific elements here (constants, etc.)
#include "atNetwork.h++"
#include "atSymbols.h++"


// Under Windows, define stuff that we need
#ifdef _MSC_VER
    #define WIN32_LEAN_AND_MEAN
    #include <bthsdpdef.h>
    #include <bluetoothapis.h>
    #include <winsock2.h>
    #include <ws2bth.h>
    #undef WIN32_LEAN_AND_MEAN

    #define AF_BLUETOOTH     AF_BTH
    #define BTPROTO_RFCOMM   BTHPROTO_RFCOMM
    #define BTPROTO_L2CAP    BTHPROTO_L2CAP

    typedef SOCKADDR_BTH   BluetoothSockAddr;

   // FUNCTIONS
   #ifdef __cplusplus
      extern "C"
      {
         ATLAS_SYM int ba2str(const bdaddr_t * btaddr, char * straddr);
         ATLAS_SYM int str2ba(const char * straddr, bdaddr_t * btaddr);
      }
   #else
      ATLAS_SYM int ba2str(const bdaddr_t * btaddr, char * straddr);
      ATLAS_SYM int str2ba(const char * straddr, bdaddr_t * btaddr);
   #endif
#else
    #include <bluetooth/bluetooth.h>
    #include <bluetooth/rfcomm.h>
    #include <bluetooth/l2cap.h>

    typedef sockaddr_rc   BluetoothSockAddr;
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM void   setBTAddress(BluetoothSockAddr * btAddr, 
                                    char * address, u_char channel);
      ATLAS_SYM void   getBTAddress(BluetoothSockAddr * btAddr, 
                                    char * address);
      ATLAS_SYM void   getBTChannel(BluetoothSockAddr * btAddr, 
                                    u_char * channel);
   }

   // These are defined in atNetwork.c++
   extern "C"
   {
      void   initNetwork();
      void   cleanupNetwork();

      Socket   openSocket(int domain, int type, int protocol);
      void     closeSocket(Socket socket);

      void   setBlockingFlag(Socket socket, bool block);
      bool   getBlockingFlag(Socket socket);
   }
#else
   ATLAS_SYM void   setBTAddress(BluetoothSockAddr * btAddr, 
                                 char * address, u_char channel);
   ATLAS_SYM void   getBTAddress(BluetoothSockAddr * btAddr, char * address);
   ATLAS_SYM void   getBTChannel(BluetoothSockAddr * btAddr, u_char * channel);


   // These are defined in atNetwork.c++
   extern
   {
      void   initNetwork();
      void   cleanupNetwork();

      Socket   openSocket(int domain, int type, int protocol);
      void     closeSocket(Socket socket);

      void   setBlockingFlag(Socket socket, bool block);
      bool   getBlockingFlag(Socket socket);
   }
#endif


#endif

