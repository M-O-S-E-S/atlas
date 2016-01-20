
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


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
      atRFCOMMBluetoothInterface(u_char channel);
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

