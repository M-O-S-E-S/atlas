
#ifndef AT_OS_DEFS_H
#define AT_OS_DEFS_H


// We don't want to use Microsoft's ntonf, etc. functions for floats/doubles
#ifdef _MSC_VER
#define NO_EXTRA_HTON_FUNCTIONS
#endif


// Include other files to give OS-independent interfaces
#include "atBluetooth.h++"
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
#include "atUUID.h++"


#endif

