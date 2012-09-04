
#include <stdio.h>
#include "atBluetooth.h++"


#ifdef _MSC_VER
   int ba2str(const bdaddr_t * btaddr, char * straddr)
   {
      // WSAAddressToString() is not useful, it adds parentheses
      u_char *   b;

      b = (u_char *) btaddr;
      return sprintf(straddr, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
                     b[5], b[4], b[3], b[2], b[1], b[0]);
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

   void getBTDevices(BluetoothDevice * devices, u_long * numDevices)
   {
      WSADATA            wsa;
      WSAQUERYSET        btQuerySet;
      BTH_QUERY_DEVICE   btDevQuery;
      BLOB               btBlob;
      HANDLE             lookupHandle;
      unsigned char      buf[4096];
      DWORD              bufLength;
      bool               done;
      int                result;
      char *             btName;
      BTH_ADDR           btAddr;
      int                num;

      WSAStartup(MAKEWORD(2, 0), &wsa);

      btDevQuery.LAP = 0;
      btDevQuery.length = 5;
      btBlob.cbSize = sizeof(BTH_QUERY_DEVICE);
      btBlob.pBlobData = (BYTE *) &btDevQuery;

      memset(&btQuerySet, 0, sizeof(WSAQUERYSET));
      btQuerySet.dwSize = sizeof(WSAQUERYSET);
      btQuerySet.dwNameSpace = NS_BTH;
      btQuerySet.lpBlob = &btBlob;

      result = WSALookupServiceBegin(&btQuerySet, 
                                     LUP_CONTAINERS | LUP_FLUSHCACHE,
                                     &lookupHandle);
      if (result != 0)
      {
         printf("WSALookupServiceBegin error:  %d\n", WSAGetLastError());
      }

      num = 0;
      done = false;
      while (!done)
      {
         bufLength = sizeof(buf);
         result = WSALookupServiceNext(lookupHandle,
                                       LUP_RETURN_NAME | LUP_RETURN_ADDR |
                                       LUP_RETURN_BLOB,
                                       &bufLength, (WSAQUERYSET *) buf);

         if (result == 0)
         {
            btName = ((WSAQUERYSET *) buf)->lpszServiceInstanceName;
            btAddr = ((SOCKADDR_BTH *)((WSAQUERYSET *)buf)->
               lpcsaBuffer[0].RemoteAddr.lpSockaddr)->btAddr;

            if (num < *numDevices)
            {
               ba2str(&btAddr, devices[num].address);
               strcpy(devices[num].name, btName);
               num++;
            }
            else
            {
               printf("Scan warning:  Ran out of space for devices\n");
               done = true;
            }
         }
         else
         {
            printf("WSALookupServiceNext error:  %d\n", WSAGetLastError());
            done = true;
         }
      }

      *numDevices = num;

      WSALookupServiceEnd(lookupHandle);

      WSACleanup();
   }

   void setBTAddress(BluetoothSockAddr * btAddr, char * address, 
                     u_char channel)
   {
      memset(btAddr, 0, sizeof(*btAddr));
      btAddr->addressFamily = AF_BLUETOOTH;
      if (address != NULL)
         str2ba(address, &(btAddr->btAddr));
      btAddr->port = channel;
      btAddr->serviceClassId = RFCOMM_PROTOCOL_UUID;
   }

   void getBTAddress(BluetoothSockAddr * btAddr, char * address)
   {
      ba2str(&(btAddr->btAddr), address);
   }

   void getBTChannel(BluetoothSockAddr * btAddr, u_char * channel)
   {
      *channel = (u_char ) btAddr->port;
   }
#elif __ANDROID__
   // Android has no Bluetooth support 
#else
   void getBTDevices(BluetoothDevice * devices, u_long * numDevices)
   {
      int              devID;
      int              sock;
      int              len;
      inquiry_info *   ii;
      int              flags;
      int              numResponses;
      int              i;

      // Open the device
      devID = hci_get_route(NULL);
      sock = hci_open_dev(devID);
      if ( (devID < 0) || (sock < 0) )
         printf("Failed to open socket in device scan.\n");

      // Make the inquiry (which will last len*1.28 seconds)
      len = 4;
      ii = (inquiry_info * ) malloc((*numDevices) * sizeof(inquiry_info));
      flags = IREQ_CACHE_FLUSH;
      numResponses = hci_inquiry(devID, len, *numDevices, NULL, &ii, flags);
      if (numResponses < 0)
         printf("Failed to complete inquiry in device scan.\n");

      // Then store the responses
      *numDevices = numResponses;
      for (i=0; i < numResponses; i++)
      {
         ba2str(&(ii+i)->bdaddr, devices[i].address);
         memset(devices[i].name, 0, sizeof(devices[i].name));
         if (hci_read_remote_name(sock, &(ii+i)->bdaddr,
                                  sizeof(devices[i].name), 
                                  devices[i].name, 0) < 0)
         {
            strcpy(devices[i].name, "UNKNOWN");
         }
      }

      // Clean-up
      free(ii);
      close(sock);
   }

   void setBTAddress(BluetoothSockAddr * btAddr, char * address, 
                     u_char channel)
   {
      btAddr->rc_family = AF_BLUETOOTH;
      if (address == NULL)
         btAddr->rc_bdaddr = *BDADDR_ANY;
      else
         str2ba(address, &(btAddr->rc_bdaddr));
      btAddr->rc_channel = channel;
   }

   void getBTAddress(BluetoothSockAddr * btAddr, char * address)
   {
      ba2str(&(btAddr->rc_bdaddr), address);
   }

   void getBTChannel(BluetoothSockAddr * btAddr, u_char * channel)
   {
      *channel = btAddr->rc_channel;
   }
#endif

