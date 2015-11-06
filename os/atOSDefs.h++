
#ifndef AT_OS_DEFS_H
#define AT_OS_DEFS_H


// We don't want to use Microsoft's ntohf, etc. functions for floats/doubles
#ifdef _MSC_VER
#define NO_EXTRA_HTON_FUNCTIONS
#endif


// Include other files to give OS-independent interfaces
#if defined(__AT_BLUETOOTH_ENABLED__)
#include "atBluetooth.h++"
#endif
#include "atByteSwap.h++"
#include "atDynamic.h++"
#include "atErrno.h++"
#include "atFile.h++"
#include "atIntTypes.h++"
#include "atLang.h++"
#include "atNetwork.h++"
#include "atSem.h++"
#include "atShm.h++"
#include "atSleep.h++"
#include "atStr.h++"
#include "atSymbols.h++"
#include "atThread.h++"
#include "atTime.h++"
#if defined(__AT_UUID_ENABLED__)
#include "atUUID.h++"
#endif


#endif

