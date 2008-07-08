
#ifndef AT_TIME_H
#define AT_TIME_H


// Under Windows, define the gettimeofday() function with corresponding types
#ifdef _MSC_VER
    #include <windows.h>
    #include <time.h>
    #include "atSymbols.h++"


    // TYPES
    struct timezone
    {
       int   tz_minuteswest;
       int   tz_dsttime;
    };


    // FUNCTIONS
    #ifdef __cplusplus
       extern "C"
       {
          ATLAS_SYM int gettimeofday(struct timeval * tv, struct timezone * tz);
       }
    #else
       ATLAS_SYM int gettimeofday(struct timeval * tv, struct timezone * tz);
    #endif
#else
   #include <sys/time.h>
#endif


#endif

