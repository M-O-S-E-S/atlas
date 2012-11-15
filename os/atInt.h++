
#ifndef AT_INT_H


#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>

   // If the glext.h header file from OpenGL is included, we'll get some
   // symbol redefinitions, because it doesn't expect an inttypes.h to be
   // available.  Define the appropriate symbol so that the glext.h 
   // integer definitions are skipped...
   #define GLEXT_64_TYPES_DEFINED

   // Include the MSVC-specific inttypes.h here
   #include "inttypes.h"
#elif __ANDROID__
   // Just include the C99 standard inttypes.h
   #include <inttypes.h>
#elif __IOS__
   // Just include the C99 standard inttypes.h
   #include <inttypes.h>
#else
   // Just include the C99 standard inttypes.h
   #include <inttypes.h>
#endif


#endif

