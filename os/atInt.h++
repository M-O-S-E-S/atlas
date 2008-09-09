
#ifndef AT_INT_H

#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>

   // On Windows, the size of "long" doesn't change between 32-bit and 64-bit
   // versions (as it does on most platforms), so the following declarations
   // work for both the Win32 and Win64 platforms

   typedef signed char        int8_t;
   typedef short              int16_t;
   typedef long               int32_t;
   typedef __int64            int64_t;

   typedef unsigned char      uint8_t;
   typedef unsigned short     uint16_t;
   typedef unsigned long      uint32_t;
   typedef unsigned __int64   uint64_t;

   typedef unsigned char      u_int8_t;
   typedef unsigned short     u_int16_t;
   typedef unsigned long      u_int32_t;
   typedef unsigned __int64   u_int64_t;
#else
   #include <inttypes.h>
#endif

#endif
