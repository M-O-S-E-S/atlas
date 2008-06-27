
#ifndef AT_OS_DEFS_H
#define AT_OS_DEFS_H


// Include the window files if this is the windows version.
#ifdef _MSC_VER
    #include <windows.h>
#endif

// Include other files to give OS-independent interfaces
#include "atErrno.h"
#include "atNetwork.h"
#include "atSleep.h"
#include "atStr.h"
#include "atSymbols.h"
#include "atTime.h"


#endif

