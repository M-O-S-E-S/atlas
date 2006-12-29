
#ifndef AT_SERIAL_INTERFACE_H
#define AT_SERIAL_INTERFACE_H


// INCLUDES
#include "atIPCInterface.h++"


class atSerialInterface : public atIPCInterface
{
   public:
      atSerialInterface();
      virtual ~atSerialInterface();
};

#endif

