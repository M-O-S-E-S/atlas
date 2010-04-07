
#include "atBluetooth.h++"


#ifdef _MSC_VER
   int ba2str(const bdaddr_t * btaddr, char * straddr)
   {
      // WSAAddressToString() is not useful, it adds parentheses
      u_char *   b;

      b = btaddr;
      return sprintf(straddr, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
                     b[7], b[6], b[5], b[4], b[3], b[2]);
   }

   int str2ba(const char * straddr, bdaddr_t * btaddr)
   {
      int        i;
      u_int      aaddr[6];
      bdaddr_t   tmpaddr;

      tmpaddr = 0;

      if (sscanf(straddr, "%02x:%02x:%02x:%02x:%02x:%02x",
                 &aaddr[0], &aaddr[1], &aaddr[2], 
                 &aaddr[3], &aaddr[4], &aaddr[5]) != 6)
      {
         return 1;
      }
      else
      {
         *btaddr = 0;
         for (i = 0; i < 6; i++) 
         {
            tmpaddr = (bdaddr_t) (aaddr[i] & 0xff);
            *btaddr = ((*btaddr) << 8) + tmpaddr;
         }

         return 0;
      }
   }

   void setAddress(BluetoothSockAddr * btAddr, char * address, u_char channel)
   {
      btAddr->addressFamily = AF_BLUETOOTH;
      str2ba(address, &(btAddr->btAddr));
      btAddr->port = channel;
      btAddr->serviceClassId = NULL_GUID;
   }
#else
   void setAddress(BluetoothSockAddr * btAddr, char * address, u_char channel)
   {
      btAddr->rc_family = AF_BLUETOOTH;
      str2ba(address, &(btAddr->rc_bdaddr));
      btAddr->rc_channel = channel;
   }
#endif

