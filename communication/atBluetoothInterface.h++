
#ifndef AT_BLUETOOTH_INTERFACE_H
#define AT_BLUETOOTH_INTERFACE_H


// INCLUDES
#include "atIPCInterface.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atBluetoothInterface : public atIPCInterface
{
   protected:
      Socket   socket_value;

      bool     blocking_mode;

   public:
      atBluetoothInterface();
      virtual ~atBluetoothInterface();

      virtual void   enableBlocking();
      virtual void   disableBlocking();
};


#endif

